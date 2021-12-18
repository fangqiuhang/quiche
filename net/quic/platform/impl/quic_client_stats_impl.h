// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_PLATFORM_IMPL_QUIC_CLIENT_STATS_IMPL_H_
#define NET_QUIC_PLATFORM_IMPL_QUIC_CLIENT_STATS_IMPL_H_

namespace quic {

// By convention, all QUIC histograms are prefixed by "Net.".
#define QUIC_HISTOGRAM_NAME(raw_name) "Net." raw_name

// TODO(fangqiuhang): maybe impl histogram
#define QUIC_CLIENT_HISTOGRAM_ENUM_IMPL(name, sample, enum_size, docstring)

#define QUIC_CLIENT_HISTOGRAM_BOOL_IMPL(name, sample, docstring)

#define QUIC_CLIENT_HISTOGRAM_TIMES_IMPL(name, sample, min, max, bucket_count, \
                                         docstring)

#define QUIC_CLIENT_HISTOGRAM_COUNTS_IMPL(name, sample, min, max, \
                                          bucket_count, docstring)

inline void QuicClientSparseHistogramImpl(const std::string& name, int sample) {
  // TODO(fangqiuhang):
}

}  // namespace quic

#endif  // NET_QUIC_PLATFORM_IMPL_QUIC_CLIENT_STATS_IMPL_H_
