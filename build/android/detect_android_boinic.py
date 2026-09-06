#!/usr/bin/env python3
# Copyright 2026 fangqiuhang. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

# Detect if the compiler targets Android (bionic) by checking for |__ANDROID__|.
#
# This is needed for environments like Termux, where `uname` reports "Linux" so
# GN sets |current_os == "linux"|, but the compiler still defines |__ANDROID__|
# because it targets Android's bionic libc. Such builds need -llog for Abseil's
# Android log sink.

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
    is_android = b"__ANDROID__" in result.stdout
    print("true" if is_android else "false")
  except Exception as e:
    print(f"// Warning: detect_android_bionic.py: {e}", file=sys.stderr)
    print("false")


if __name__ == "__main__":
  main()
