#!/usr/bin/env python3
# Copyright 2026 fangqiuhang. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

# Minimal replacement for Chromium's build/toolchain/win/tool_wrapper.py.
# This glue build only needs the `recursive-mirror` (copy), `delete-file` and
# `create-archive` subcommands.

import os
import shlex
import shutil
import subprocess
import sys


def recursive_mirror(source, dest):
  if os.path.isdir(source):
    if os.path.exists(dest):
      shutil.rmtree(dest)
    shutil.copytree(source, dest)
  else:
    os.makedirs(os.path.dirname(dest), exist_ok=True)
    shutil.copy2(source, dest)


def delete_file(path):
  if os.path.exists(path):
    os.remove(path)


def create_archive(archive, rspfile, ar="ar"):
  # Build a fresh archive into a temp file and atomically replace the output.
  # `ar rcs` keeps stale members of an existing archive, and cmd's
  # `del`/`move` cannot handle forward-slash paths (ninja passes them), so a
  # bare `del /f ... & ar rcs ...` silently keeps objects from removed
  # sources. This always produces a clean archive.
  with open(rspfile, "r") as f:
    objects = shlex.split(f.read())
  tmp = archive + ".tmp"
  if os.path.exists(tmp):
    os.remove(tmp)
  # GN passes the absolute path of the toolchain's ar via --ar=..., so the
  # build does not depend on MinGW being on PATH.
  ar_command = shutil.which(ar) or ar
  subprocess.check_call([ar_command, "rcs", tmp] + objects)
  if os.path.exists(archive):
    os.remove(archive)
  os.replace(tmp, archive)


def main():
  if len(sys.argv) < 3:
    print("usage: tool_wrapper.py <command> [args...]", file=sys.stderr)
    return 1
  command = sys.argv[1]
  if command == "recursive-mirror":
    if len(sys.argv) != 4:
      print("recursive-mirror: expected source and dest", file=sys.stderr)
      return 1
    recursive_mirror(sys.argv[2], sys.argv[3])
    return 0
  if command == "delete-file":
    if len(sys.argv) != 3:
      print("delete-file: expected path", file=sys.stderr)
      return 1
    delete_file(sys.argv[2])
    return 0
  if command == "create-archive":
    if len(sys.argv) != 4 and len(sys.argv) != 5:
      print("create-archive: expected archive and rspfile", file=sys.stderr)
      return 1
    rest = sys.argv[2:]
    ar = "ar"
    if rest and rest[0].startswith("--ar="):
      ar = rest[0][len("--ar="):]
      rest = rest[1:]
    if len(rest) != 2:
      print("create-archive: expected archive and rspfile", file=sys.stderr)
      return 1
    create_archive(rest[0], rest[1], ar)
    return 0
  print("unknown command: %s" % command, file=sys.stderr)
  return 1


if __name__ == "__main__":
  sys.exit(main())
