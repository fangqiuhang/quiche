// Copyright (c) 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_PLATFORM_IMPL_QUIC_TEST_IMPL_H_
#define NET_QUIC_PLATFORM_IMPL_QUIC_TEST_IMPL_H_

#include "net/quic/platform/impl/quic_test_flags_utils.h"
#include "net/third_party/quiche/src/quic/core/quic_versions.h"
#include "net/third_party/quiche/src/quic/platform/api/quic_flags.h"

class QuicTestImpl : public ::testing::Test {
 private:
  QuicFlagChecker checker_;
  QuicFlagSaverImpl saver_;  // Save/restore all QUIC flag values.
};

template <class T>
class QuicTestWithParamImpl : public ::testing::TestWithParam<T> {
 private:
  QuicFlagChecker checker_;
  QuicFlagSaverImpl saver_;  // Save/restore all QUIC flag values.
};

class ScopedEnvironmentForThreadsImpl {
 public:
  // TODO(fangqiuhang):
};

#define QUIC_TEST_DISABLED_IN_CHROME_IMPL(name) DISABLED_##name

std::string QuicGetTestMemoryCachePathImpl();

#define QUIC_SLOW_TEST_IMPL(name) DISABLED_##name

#endif  // NET_QUIC_PLATFORM_IMPL_QUIC_TEST_IMPL_H_
