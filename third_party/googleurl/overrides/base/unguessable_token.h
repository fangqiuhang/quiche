// Copyright 2026 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Polyfill for Chromium's base/unguessable_token.h, which url/origin.h
// includes for its opaque-origin nonce. Only a complete, copyable type is
// needed for compilation; origin.cc (not part of this standalone build) holds
// the real logic.

#ifndef OVERRIDES_BASE_UNGUESSABLE_TOKEN_H_
#define OVERRIDES_BASE_UNGUESSABLE_TOKEN_H_

#include <compare>
#include <cstdint>

namespace gurl_base {

class UnguessableToken {
 public:
  UnguessableToken() = default;
  UnguessableToken(const UnguessableToken&) = default;
  UnguessableToken& operator=(const UnguessableToken&) = default;
  UnguessableToken(UnguessableToken&&) = default;
  UnguessableToken& operator=(UnguessableToken&&) = default;

  bool is_empty() const { return high_ == 0 && low_ == 0; }

  friend bool operator==(const UnguessableToken&,
                         const UnguessableToken&) = default;
  friend std::strong_ordering operator<=>(const UnguessableToken&,
                                          const UnguessableToken&) = default;

 private:
  uint64_t high_ = 0;
  uint64_t low_ = 0;
};

}  // namespace gurl_base

#endif  // OVERRIDES_BASE_UNGUESSABLE_TOKEN_H_
