#!/usr/bin/env python3
# Copyright 2026 fangqiuhang. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

# Resolves |tool| (e.g. "gcc") to its absolute path using the PATH visible to
# the `gn gen` invocation, so the generated build.ninja does not depend on the
# compiler being on PATH in whatever shell later runs ninja.
#
# Usage: python find_tool.py <tool>
# Prints the resolved absolute path (with forward slashes), or exits 1.

import shutil
import sys


def main():
  path = None
  for tool in sys.argv[1:]:
    path = shutil.which(tool)
    if path:
      break
  if not path:
    print(f"// Error: find_tool.py: '{tool}' not found on PATH. "
          "Add the MinGW/CLang bin directory to PATH before running gn gen.",
          file=sys.stderr)
    sys.exit(1)
  sys.stdout.write(path.replace("\\", "/"))


if __name__ == "__main__":
  main()
