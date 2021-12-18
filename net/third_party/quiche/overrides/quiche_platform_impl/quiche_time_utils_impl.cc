// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/third_party/quiche/overrides/quiche_platform_impl/quiche_time_utils_impl.h"

// TODO(fangqiuhang):
#include <iostream>

namespace quiche {
absl::optional<int64_t> QuicheUtcDateTimeToUnixSecondsInner(int year,
                                                            int month,
                                                            int day,
                                                            int hour,
                                                            int minute,
                                                            int second) {
  return absl::nullopt;
}

absl::optional<int64_t> QuicheUtcDateTimeToUnixSecondsImpl(int year,
                                                           int month,
                                                           int day,
                                                           int hour,
                                                           int minute,
                                                           int second) {
  return QuicheUtcDateTimeToUnixSecondsInner(year, month, day, hour, minute,
                                             second);
}

}  // namespace quiche
