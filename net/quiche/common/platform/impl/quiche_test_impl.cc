// Copyright (c) 2021 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/quiche/common/platform/impl/quiche_test_impl.h"

#include <string>

namespace quiche {
namespace test {

std::string QuicheGetCommonSourcePathImpl() {
  return "third_party/quiche/common";
}

}  // namespace test
}  // namespace quiche
