// Copyright 2025 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QUICHE_COMMON_PLATFORM_DEFAULT_QUICHE_PLATFORM_IMPL_QUICHE_FUZZTEST_IMPL_H_
#define QUICHE_COMMON_PLATFORM_DEFAULT_QUICHE_PLATFORM_IMPL_QUICHE_FUZZTEST_IMPL_H_

#include <initializer_list>

// Stub of the fuzztest API for builds that don't include
// //third_party/fuzztest. FUZZ_TEST registrations compile to no-ops and won't
// produce any fuzz tests.

namespace fuzztest {

template <typename T>
class Arbitrary {
 public:
  Arbitrary() = default;
  template <typename U>
  Arbitrary& WithMinSize(U&&) {
    return *this;
  }
};

class ElementOfImpl {
 public:
  template <typename... Args>
  explicit ElementOfImpl(Args&&...) {}
};

template <typename T>
ElementOfImpl ElementOf(std::initializer_list<T>) {
  return ElementOfImpl();
}

template <typename Container>
ElementOfImpl ElementOf(const Container&) {
  return ElementOfImpl();
}

namespace internal {

class FuzzTestRegistrationStub {
 public:
  template <typename... Args>
  FuzzTestRegistrationStub& WithDomains(Args&&...) {
    return *this;
  }
  template <typename T>
  FuzzTestRegistrationStub& WithSeeds(std::initializer_list<T>) {
    return *this;
  }
  template <typename... Args>
  FuzzTestRegistrationStub& WithSeeds(Args&&...) {
    return *this;
  }
};

}  // namespace internal
}  // namespace fuzztest

#define FUZZ_TEST(test_suite_name, test_name)                 \
  static ::fuzztest::internal::FuzzTestRegistrationStub       \
      fuzztest_registration_##test_suite_name##_##test_name = \
          ::fuzztest::internal::FuzzTestRegistrationStub()

#endif  // QUICHE_COMMON_PLATFORM_DEFAULT_QUICHE_PLATFORM_IMPL_QUICHE_FUZZTEST_IMPL_H_
