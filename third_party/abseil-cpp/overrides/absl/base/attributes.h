// Copyright 2024 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// Override for absl/base/attributes.h that prevents ABSL_CONST_INIT
// from expanding to constinit on MSVC. MSVC's STL does not have
// constexpr std::make_pair, which causes C2127 errors when used with
// constinit in web_transport_headers.cc.
//
// This file is placed in the overrides include path before the real
// abseil-cpp includes, so it shadows the real attributes.h.

#ifndef OVERRIDE_ABSL_BASE_ATTRIBUTES_H_
#define OVERRIDE_ABSL_BASE_ATTRIBUTES_H_

// Include the real attributes.h first. Use the full path from the
// build root to avoid recursive inclusion of this override file.
#include "third_party/abseil-cpp/src/absl/base/attributes.h"

// Now redefine ABSL_CONST_INIT to empty for MSVC, since constinit
// causes C2127 errors with std::make_pair in MSVC's STL.
#ifdef ABSL_CONST_INIT
#undef ABSL_CONST_INIT
#endif
#define ABSL_CONST_INIT

#endif  // OVERRIDE_ABSL_BASE_ATTRIBUTES_H_
