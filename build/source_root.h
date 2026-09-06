// Copyright 2025 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Shared helper for this standalone GN build: locate the repository (source)
// root from the test executable's own location, so test data can be read
// straight from the source tree regardless of the current working directory.
//
// This mirrors Chromium's test-data approach (base::PathService::Get(
// base::DIR_SRC_TEST_DATA_ROOT) in third_party/boringssl/test_data_chromium.cc)
// without depending on Chromium's //base.

#ifndef GLUE_BUILD_SOURCE_ROOT_H_
#define GLUE_BUILD_SOURCE_ROOT_H_

#include <stdio.h>

#include <string>

#if defined(_WIN32)
#include <windows.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#include <sys/param.h>
#else
#include <unistd.h>
#endif

namespace glue {

// Returns the directory containing the test executable, or "" on failure.
inline std::string GetExecutableDir() {
#if defined(_WIN32)
  wchar_t buf[MAX_PATH];
  DWORD len = GetModuleFileNameW(nullptr, buf, MAX_PATH);
  if (len == 0 || len >= MAX_PATH) {
    return "";
  }
  std::wstring wpath(buf, len);
  if (wpath.empty()) {
    return "";
  }
  int size_needed =
      WideCharToMultiByte(CP_UTF8, 0, wpath.c_str(), (int)wpath.size(), nullptr,
                          0, nullptr, nullptr);
  if (size_needed <= 0) {
    return "";
  }
  std::string path(size_needed, 0);
  WideCharToMultiByte(CP_UTF8, 0, wpath.c_str(), (int)wpath.size(), &path[0],
                      size_needed, nullptr, nullptr);
  size_t pos = path.find_last_of("\\/");
  return pos == std::string::npos ? "" : path.substr(0, pos);
#elif defined(__APPLE__)
  char buf[PATH_MAX];
  uint32_t size = sizeof(buf);
  if (_NSGetExecutablePath(buf, &size) != 0) {
    return "";
  }
  std::string path(buf);
  size_t pos = path.find_last_of('/');
  return pos == std::string::npos ? "" : path.substr(0, pos);
#else
  char buf[4096];
  ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
  if (len <= 0) {
    return "";
  }
  buf[len] = '\0';
  std::string path(buf);
  size_t pos = path.find_last_of('/');
  return pos == std::string::npos ? "" : path.substr(0, pos);
#endif
}

// Walks up from the executable directory until it finds the repository root
// (a directory containing third_party/boringssl/OWNERS). Returns "" if not
// found, in which case callers fall back to CWD-relative paths.
inline std::string GetSourceRoot() {
  std::string dir = GetExecutableDir();
  while (!dir.empty()) {
    std::string check = dir + "/third_party/boringssl/OWNERS";
    FILE* f = fopen(check.c_str(), "rb");
    if (f != nullptr) {
      fclose(f);
      return dir;
    }
    size_t pos = dir.find_last_of("/\\");
    if (pos == std::string::npos) {
      break;
    }
    dir = dir.substr(0, pos);
  }
  return "";
}

}  // namespace glue

#endif  // GLUE_BUILD_SOURCE_ROOT_H_
