// Copyright 2015 The BoringSSL Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Glue replacement for BoringSSL's crypto/thread_pthread.cc, used by the
// MinGW build.
//
// Why: winpthreads implements PTHREAD_RWLOCK_INITIALIZER as a lazy-init
// sentinel (-1); the implicit initialization is NOT thread-safe. When
// multiple threads take the FIRST lock on the same rwlock simultaneously
// (e.g. BoringSSL's X509Test.VerifyThreads, where 10 threads verify the same
// X509 objects at once), the lazy init races and pthread_rwlock_rdlock
// returns EINVAL, tripping BoringSSL's BSSL_CHECK and aborting. MSVC builds
// are unaffected because they use SRWLOCK (OPENSSL_WINDOWS_THREADS).
//
// This file eagerly initializes each rwlock exactly once (atomically) before
// any locking, avoiding winpthreads' racy lazy path. Everything else mirrors
// the upstream implementation; the internal thread-local destructor table
// guard is switched to an SRWLOCK, whose static init is race-free.

#define _BORINGSSL_PROHIBIT_OPENSSL_MALLOC
#include "src/crypto/internal.h"

#if defined(OPENSSL_PTHREADS)

#include <windows.h>

#include <assert.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

BSSL_NAMESPACE_BEGIN

// PTHREAD_RWLOCK_INITIALIZER is a lazy-init sentinel in winpthreads; its
// implicit initialization is racy under concurrent first use. Eagerly create
// a real rwlock, publishing it with a compare-and-swap so exactly one thread
// performs the initialization.
static void static_mutex_ensure_initialized(pthread_rwlock_t* lock) {
  if (*lock != PTHREAD_RWLOCK_INITIALIZER) {
    return;
  }
  pthread_rwlock_t new_lock;
  if (pthread_rwlock_init(&new_lock, nullptr) != 0) {
    abort();
  }
  if (InterlockedCompareExchangePointer(
          reinterpret_cast<void* volatile*>(lock),
          reinterpret_cast<void*>(new_lock),
          reinterpret_cast<void*>(PTHREAD_RWLOCK_INITIALIZER)) !=
      reinterpret_cast<void*>(PTHREAD_RWLOCK_INITIALIZER)) {
    // Another thread initialized it first; discard ours.
    pthread_rwlock_destroy(&new_lock);
  }
}

void StaticMutex::LockRead() {
  static_mutex_ensure_initialized(&lock_);
  BSSL_CHECK(pthread_rwlock_rdlock(&lock_) == 0);
}

void StaticMutex::UnlockRead() {
  BSSL_CHECK(pthread_rwlock_unlock(&lock_) == 0);
}

void StaticMutex::LockWrite() {
  static_mutex_ensure_initialized(&lock_);
  BSSL_CHECK(pthread_rwlock_wrlock(&lock_) == 0);
}

void StaticMutex::UnlockWrite() {
  BSSL_CHECK(pthread_rwlock_unlock(&lock_) == 0);
}

Mutex::~Mutex() {
  if (lock_ != PTHREAD_RWLOCK_INITIALIZER) {
    pthread_rwlock_destroy(&lock_);
  }
}

void CRYPTO_once(CRYPTO_once_t* once, void (*init)()) {
  BSSL_CHECK(pthread_once(once, init) == 0);
}

// Guard for the thread-local destructor table. SRWLOCK_INIT is zero and is
// safe to use without an initialization call (unlike winpthreads' lazy
// PTHREAD_MUTEX_INITIALIZER).
static SRWLOCK g_destructors_lock = SRWLOCK_INIT;
static thread_local_destructor_t g_destructors[NUM_OPENSSL_THREAD_LOCALS];

// thread_local_destructor is called when a thread exits. It releases thread
// local data for that thread only.
static void thread_local_destructor(void* arg) {
  if (arg == nullptr) {
    return;
  }

  thread_local_destructor_t destructors[NUM_OPENSSL_THREAD_LOCALS];
  AcquireSRWLockExclusive(&g_destructors_lock);
  OPENSSL_memcpy(destructors, g_destructors, sizeof(destructors));
  ReleaseSRWLockExclusive(&g_destructors_lock);

  unsigned i;
  void** pointers = reinterpret_cast<void**>(arg);
  for (i = 0; i < NUM_OPENSSL_THREAD_LOCALS; i++) {
    if (destructors[i] != nullptr) {
      destructors[i](pointers[i]);
    }
  }

  free(pointers);
}

static pthread_once_t g_thread_local_init_once = PTHREAD_ONCE_INIT;
static pthread_key_t g_thread_local_key;
static int g_thread_local_key_created = 0;

static void thread_local_init() {
  g_thread_local_key_created =
      pthread_key_create(&g_thread_local_key, thread_local_destructor) == 0;
}

void* CRYPTO_get_thread_local(thread_local_data_t index) {
  CRYPTO_once(&g_thread_local_init_once, thread_local_init);
  if (!g_thread_local_key_created) {
    return nullptr;
  }

  void** pointers =
      reinterpret_cast<void**>(pthread_getspecific(g_thread_local_key));
  if (pointers == nullptr) {
    return nullptr;
  }
  return pointers[index];
}

int CRYPTO_set_thread_local(thread_local_data_t index,
                            void* value,
                            thread_local_destructor_t destructor) {
  CRYPTO_once(&g_thread_local_init_once, thread_local_init);
  if (!g_thread_local_key_created) {
    destructor(value);
    return 0;
  }

  void** pointers =
      reinterpret_cast<void**>(pthread_getspecific(g_thread_local_key));
  if (pointers == nullptr) {
    pointers = reinterpret_cast<void**>(
        malloc(sizeof(void*) * NUM_OPENSSL_THREAD_LOCALS));
    if (pointers == nullptr) {
      destructor(value);
      return 0;
    }
    OPENSSL_memset(pointers, 0, sizeof(void*) * NUM_OPENSSL_THREAD_LOCALS);
    if (pthread_setspecific(g_thread_local_key, pointers) != 0) {
      free(pointers);
      destructor(value);
      return 0;
    }
  }

  AcquireSRWLockExclusive(&g_destructors_lock);
  g_destructors[index] = destructor;
  ReleaseSRWLockExclusive(&g_destructors_lock);

  pointers[index] = value;
  return 1;
}

BSSL_NAMESPACE_END

#endif  // OPENSSL_PTHREADS
