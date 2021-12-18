// Copyright (c) 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_PLATFORM_IMPL_QUIC_FILE_UTILS_IMPL_H_
#define NET_QUIC_PLATFORM_IMPL_QUIC_FILE_UTILS_IMPL_H_

#include <vector>

namespace quic {

// Traverses the directory |dirname| and returns all of the files it contains.
std::vector<std::string> ReadFileContentsImpl(const std::string& dirname) {
  std::vector<std::string> files;
  // TODO(fangqiuhang):
  return files;
}

// Reads the contents of |filename| as a string into |contents|.
void ReadFileContentsImpl(absl::string_view filename, std::string* contents) {
  // TODO(fangqiuhang):
}

}  // namespace quic

#endif  // NET_QUIC_PLATFORM_IMPL_QUIC_FILE_UTILS_IMPL_H_
