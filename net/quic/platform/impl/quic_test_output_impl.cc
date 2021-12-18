// Copyright (c) 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/quic/platform/impl/quic_test_output_impl.h"

#include <stdlib.h>
#include <time.h>

#include "net/third_party/quiche/src/quic/platform/api/quic_logging.h"

namespace quic {

void QuicRecordTestOutputToFile(absl::string_view filename,
                                absl::string_view data) {
  // TODO(fangqiuhang):
}

void QuicSaveTestOutputImpl(absl::string_view filename,
                            absl::string_view data) {
  QuicRecordTestOutputToFile(filename, data);
}

bool QuicLoadTestOutputImpl(absl::string_view filename, std::string* data) {
  // TODO(fangqiuhang):
}

void QuicRecordTraceImpl(absl::string_view identifier, absl::string_view data) {
  // TODO(fangqiuhang):

  QuicRecordTestOutputToFile(filename, data);
}

}  // namespace quic
