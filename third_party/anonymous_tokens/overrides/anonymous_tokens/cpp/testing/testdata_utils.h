// Glue override of anonymous_tokens::GetTestdataPath for this standalone GN
// build.
//
// The upstream implementation returns the CWD-relative path
// "anonymous_tokens/testdata/", which forces tests to run from the build
// output directory (where a copy action placed the data). Mirrors Chromium's
// approach: resolve the repository root from the test executable's location
// and read the test data straight from the source tree, so the tests run from
// any working directory without copying any data. Falls back to the upstream
// relative path if the source root cannot be located.

#ifndef ANONYMOUS_TOKENS_CPP_TESTING_TESTDATA_UTILS_H_
#define ANONYMOUS_TOKENS_CPP_TESTING_TESTDATA_UTILS_H_

#include <string>

#include "build/source_root.h"

namespace anonymous_tokens {

inline std::string GetTestdataPath() {
  std::string src_root = glue::GetSourceRoot();
  if (!src_root.empty()) {
    return src_root +
           "/third_party/anonymous_tokens/src/anonymous_tokens/testdata/";
  }
  return "anonymous_tokens/testdata/";
}

}  // namespace anonymous_tokens

#endif  // ANONYMOUS_TOKENS_CPP_TESTING_TESTDATA_UTILS_H_
