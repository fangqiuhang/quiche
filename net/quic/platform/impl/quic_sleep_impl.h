// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_PLATFORM_IMPL_QUIC_SLEEP_IMPL_H_
#define NET_QUIC_PLATFORM_IMPL_QUIC_SLEEP_IMPL_H_

#include "net/third_party/quiche/src/quic/core/quic_time.h"

namespace quic {

inline void QuicSleepImpl(QuicTime::Delta duration) {
  // TODO(fangqiuhang): impl base::PlatformThread::Sleep. seems no need.
}

}  // namespace quic

#endif  // NET_QUIC_PLATFORM_IMPL_QUIC_SLEEP_IMPL_H_
