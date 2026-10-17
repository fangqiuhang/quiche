// Copyright 2026 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Polyfill for Chromium's generated build/robolectric_buildflags.h. Robolectric
// is Android-only, so the flag is always false in this standalone build.

#ifndef OVERRIDES_BUILD_ROBOLECTRIC_BUILDFLAGS_H_
#define OVERRIDES_BUILD_ROBOLECTRIC_BUILDFLAGS_H_

#include "build/buildflag.h"

#define BUILDFLAG_INTERNAL_IS_ROBOLECTRIC() (0)

#endif  // OVERRIDES_BUILD_ROBOLECTRIC_BUILDFLAGS_H_
