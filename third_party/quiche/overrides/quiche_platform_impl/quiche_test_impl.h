// Copyright 2022 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// Glue override of
// quiche/common/platform/default/quiche_platform_impl/quiche_test_impl.h.
//
// The upstream implementations of QuicheGetCommonSourcePathImpl() and
// QuicheGetTestMemoryCachePathImpl() return CWD-relative paths, which forces
// tests to run from a working directory where "quiche/common" /
// "quiche/quic/test_tools/..." resolve. Mirrors Chromium's approach: resolve
// the repository root from the test executable's location (build/source_root.h)
// and point at the real source-tree directories, so the tests run from any
// working directory. Falls back to the upstream relative paths if the source
// root cannot be located.

#ifndef QUICHE_COMMON_PLATFORM_DEFAULT_QUICHE_PLATFORM_IMPL_QUICHE_TEST_IMPL_H_
#define QUICHE_COMMON_PLATFORM_DEFAULT_QUICHE_PLATFORM_IMPL_QUICHE_TEST_IMPL_H_

#include "build/source_root.h"
#include "gmock/gmock.h"
#include "gtest/gtest-spi.h"
#include "gtest/gtest.h"

#define EXPECT_QUICHE_DEBUG_DEATH_IMPL(condition, message) \
  EXPECT_DEBUG_DEATH(condition, message)

#define EXPECT_QUICHE_DEATH_IMPL(condition, message) \
  EXPECT_DEATH(condition, message)

#define QUICHE_TEST_DISABLED_IN_CHROME_IMPL(name) name
#define QUICHE_SLOW_TEST_IMPL(test) test

class QuicheFlagSaverImpl {
 public:
  QuicheFlagSaverImpl();
  ~QuicheFlagSaverImpl();

 private:
#define QUICHE_FLAG(type, flag, internal_value, external_value, doc) \
  type saved_##flag##_;
#include "quiche/common/quiche_feature_flags_list.h"
#undef QUICHE_FLAG

#define QUICHE_PROTOCOL_FLAG(type, flag, ...) type saved_##flag##_;
#include "quiche/common/quiche_protocol_flags_list.h"
#undef QUICHE_PROTOCOL_FLAG
};

class ScopedEnvironmentForThreadsImpl {};

namespace quiche::test {

class QuicheTestImpl : public ::testing::Test {
 private:
  QuicheFlagSaverImpl saver_;
};

template <class T>
class QuicheTestWithParamImpl : public ::testing::TestWithParam<T> {
 private:
  QuicheFlagSaverImpl saver_;
};

inline std::string QuicheGetCommonSourcePathImpl() {
  std::string src_root = glue::GetSourceRoot();
  if (!src_root.empty()) {
    return src_root + "/third_party/quiche/src/quiche/common";
  }
  return "quiche/common";
}

}  // namespace quiche::test

inline std::string QuicheGetTestMemoryCachePathImpl() {
  std::string src_root = glue::GetSourceRoot();
  if (!src_root.empty()) {
    return src_root +
           "/third_party/quiche/src/quiche/quic/test_tools/"
           "quic_http_response_cache_data";
  }
  return "quiche/quic/test_tools/quic_http_response_cache_data";
}

class QuicheScopedDisableExitOnDFatalImpl {};

#endif  // QUICHE_COMMON_PLATFORM_DEFAULT_QUICHE_PLATFORM_IMPL_QUICHE_TEST_IMPL_H_
