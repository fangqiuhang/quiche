// Copyright (c) 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/quic/platform/impl/quic_test_impl.h"

std::string QuicGetTestMemoryCachePathImpl() {
  // TODO(fangqiuhang):
  return "/tmp/cache";
}
