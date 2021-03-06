// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_PLATFORM_IMPL_QUIC_IOVEC_IMPL_H_
#define NET_QUIC_PLATFORM_IMPL_QUIC_IOVEC_IMPL_H_

#include <stddef.h>

// TODO(fangqiuhang): maybe impl build_config.h

#if defined(OS_WIN)
/* Structure for scatter/gather I/O.  */
struct iovec {
  void* iov_base; /* Pointer to data.  */
  size_t iov_len; /* Length of data.  */
};
#elif defined(OS_POSIX) || defined(OS_FUCHSIA)
#include <sys/uio.h>
#endif  // defined(OS_WIN)

#endif  // NET_QUIC_PLATFORM_IMPL_QUIC_IOVEC_IMPL_H_
