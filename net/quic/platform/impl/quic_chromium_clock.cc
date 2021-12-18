// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/quic/platform/impl/quic_chromium_clock.h"

namespace quic {

QuicChromiumClock* QuicChromiumClock::GetInstance() {
  // TODO(fangqiuhang): maybe impl base::NoDestructor.
  static QuicChromiumClock instance;
  return &instance;
}

QuicChromiumClock::QuicChromiumClock() = default;

QuicChromiumClock::~QuicChromiumClock() = default;

QuicTime QuicChromiumClock::ApproximateNow() const {
  // At the moment, Chrome does not have a distinct notion of ApproximateNow().
  // We should consider implementing this using MessageLoop::recent_time_.
  return Now();
}

QuicTime QuicChromiumClock::Now() const {
  // TODO(fangqiuhang):
  int64_t ticks = 0;
  DCHECK_GE(ticks, 0);
  return CreateTimeFromMicroseconds(ticks);
}

QuicWallTime QuicChromiumClock::WallNow() const {
  // TODO(fangqiuhang):
  int64_t time_since_unix_epoch_micro = 1;
  DCHECK_GE(time_since_unix_epoch_micro, 0);
  return QuicWallTime::FromUNIXMicroseconds(time_since_unix_epoch_micro);
}

}  // namespace quic
