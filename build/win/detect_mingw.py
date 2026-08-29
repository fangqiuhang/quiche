#!/usr/bin/env python3
# Copyright 2026 fangqiuhang. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

# Detect if the compiler is MinGW by checking for |__MINGW32__| preprocessor
# define.
#
# This is more robust than checking GN variables like |is_win && !is_clang|,
# because it directly queries the compiler, and works regardless of whether
# the build later adds MSVC or other Windows compilers.

import subprocess
import sys


def main():
  cc = sys.argv[1] if len(sys.argv) > 1 else "gcc"

  try:
    result = subprocess.run(
        [cc, "-E", "-dM", "-x", "c", "-"],
        input=b"",
        capture_output=True,
        timeout=10,
    )
    is_mingw = b"__MINGW32__" in result.stdout
    # Write via stdout.buffer (no newline translation) so GN's exec_script
    # "value" mode gets a clean "true"/"false" without a trailing CR/LF.
    sys.stdout.buffer.write(b"true" if is_mingw else b"false")
  except Exception as e:
    # Compiler not found or failed — assume not MinGW
    print(f"// Warning: detect_mingw.py: {e}", file=sys.stderr)
    sys.stdout.buffer.write(b"false")


if __name__ == "__main__":
  main()
