// Glue implementation of GetTestData for the standalone GN build.
//
// Mirrors Chromium's approach (third_party/boringssl/test_data_chromium.cc):
// test data is read straight from the source tree
// (<source_root>/third_party/boringssl/src/<path>), located via the test
// executable's own path, so the tests run from any working directory and no
// data needs to be copied to the output directory. BORINGSSL_TEST_DATA_ROOT
// still overrides the root; a CWD-relative "boringssl_test_data" fallback
// keeps the old copied-data layout working if the source root is not found.
//
// This file is compiled with BORINGSSL_CUSTOM_GET_TEST_DATA defined.

#include <stdio.h>
#include <stdlib.h>

#include <string>

#include "build/source_root.h"
#include "src/crypto/test/file_util.h"
#include "src/crypto/test/test_data.h"

std::string GetTestData(const char* path) {
  std::string root;
  const char* root_env = getenv("BORINGSSL_TEST_DATA_ROOT");
  if (root_env != nullptr) {
    root = root_env;
  } else {
    std::string src_root = glue::GetSourceRoot();
    if (!src_root.empty()) {
      root = src_root + "/third_party/boringssl/src";
    } else {
      root = "boringssl_test_data";
    }
  }

  std::string full_path = root;
  full_path.push_back('/');
  full_path.append(path);

  bssl::ScopedFILE file(fopen(full_path.c_str(), "rb"));
  if (file == nullptr) {
    fprintf(stderr, "Could not open '%s'.\n", full_path.c_str());
    abort();
  }

  std::string ret;
  for (;;) {
    char buf[512];
    size_t n = fread(buf, 1, sizeof(buf), file.get());
    if (n == 0) {
      if (feof(file.get())) {
        return ret;
      }
      fprintf(stderr, "Error reading from '%s'.\n", full_path.c_str());
      abort();
    }
    ret.append(buf, n);
  }
}
