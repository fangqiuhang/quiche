// Copyright 2026 fangqiuhang. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Stub override for absl/base/nullability.h not present in this abseil version.
#ifndef ABSL_BASE_NULLABILITY_H_
#define ABSL_BASE_NULLABILITY_H_

#define ABSL_NONNULL
#define ABSL_NULLABLE
#define ABSL_NULLABILITY_COMPATIBLE
#define absl_nonnull
#define absl_nullable
#define absl_nullability_compatible
#define absl_nullability_unknown

namespace absl {

template <typename T>
using Nonnull = T;

template <typename T>
using Nullable = T;

}  // namespace absl

#endif  // ABSL_BASE_NULLABILITY_H_
