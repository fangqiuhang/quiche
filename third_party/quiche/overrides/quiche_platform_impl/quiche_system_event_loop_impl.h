// Copyright 2022 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QUICHE_COMMON_PLATFORM_DEFAULT_QUICHE_PLATFORM_IMPL_QUICHE_SYSTEM_EVENT_LOOP_IMPL_H_
#define QUICHE_COMMON_PLATFORM_DEFAULT_QUICHE_PLATFORM_IMPL_QUICHE_SYSTEM_EVENT_LOOP_IMPL_H_

#if defined(_WIN32)
#include <winsock2.h>
#endif

#include <string>

#include "quiche/common/platform/api/quiche_export.h"
#include "quiche/common/platform/api/quiche_logging.h"

namespace quiche {

inline void QuicheRunSystemEventLoopIterationImpl() {}

class QUICHE_EXPORT QuicheSystemEventLoopImpl {
 public:
  QuicheSystemEventLoopImpl(std::string context_name) {
#if defined(_WIN32)
    WSADATA wsa_data;
    int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (result != 0) {
      QUICHE_LOG(ERROR) << "WSAStartup failed: " << result;
    }
#endif
    QUICHE_LOG(INFO) << "Starting event loop for " << context_name;
  }

  ~QuicheSystemEventLoopImpl() {
#if defined(_WIN32)
    WSACleanup();
#endif
  }
};

}  // namespace quiche

#endif  // QUICHE_COMMON_PLATFORM_DEFAULT_QUICHE_PLATFORM_IMPL_QUICHE_SYSTEM_EVENT_LOOP_IMPL_H_
