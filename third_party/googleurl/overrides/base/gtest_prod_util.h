// Copyright 2026 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Polyfill for Chromium's base/gtest_prod_util.h, which googleurl's sources
// include unconditionally. FRIEND_TEST itself comes from gtest's gtest_prod.h.

#ifndef OVERRIDES_BASE_GTEST_PROD_UTIL_H_
#define OVERRIDES_BASE_GTEST_PROD_UTIL_H_

#include "gtest/gtest_prod.h"

// Chromium also offers FRIEND_TEST_ALL_PREFIXES / FRIEND_TEST_READONLY_*
// variants; nothing in googleurl uses them, so a trivial alias suffices.
#define FRIEND_TEST_ALL_PREFIXES(test_case_name, test_name) \
  FRIEND_TEST(test_case_name, test_name)

#endif  // OVERRIDES_BASE_GTEST_PROD_UTIL_H_
