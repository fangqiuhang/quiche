#!/usr/bin/env python3
# Copyright 2024 The Chromium Authors
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

# Simplified VS toolchain setup script for GN builds.
# Detects Visual Studio installation, runs vcvarsall.bat, and outputs GN scope
# variables for the MSVC toolchain.
#
# Usage:
#   setup_toolchain.py <visual_studio_path> <target_cpu>
#
# If visual_studio_path is empty, auto-detects using vswhere.exe.

import os
import re
import subprocess
import sys


def _to_gn_string(s):
  """Format a string for GN output, escaping as needed."""
  # GN uses double quotes for strings
  escaped = s.replace('\\', '\\\\').replace('"', '\\"').replace('$', '\\$')
  return f'"{escaped}"'


def _find_vs_with_vswhere():
  """Auto-detect Visual Studio installation using vswhere.exe."""
  vswhere_paths = [
      r"C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe",
  ]
  vswhere = None
  for path in vswhere_paths:
    if os.path.exists(path):
      vswhere = path
      break

  if not vswhere:
    raise Exception("vswhere.exe not found. Please install Visual Studio or "
                    "set visual_studio_path in args.gn.")

  result = subprocess.run(
      [vswhere, "-latest", "-property", "installationPath"],
      capture_output=True,
      text=True,
  )
  vs_path = result.stdout.strip()
  if not vs_path:
    raise Exception("No Visual Studio installation found by vswhere.exe. "
                    "Please set visual_studio_path in args.gn.")
  return vs_path


def _find_latest_msvc_tools(vs_path):
  """Find the latest MSVC tools version in the VS installation."""
  tools_path = os.path.join(vs_path, "VC", "Tools", "MSVC")
  if not os.path.exists(tools_path):
    raise Exception(f"MSVC tools not found at {tools_path}")

  versions = []
  for entry in os.listdir(tools_path):
    full_path = os.path.join(tools_path, entry)
    if os.path.isdir(full_path):
      versions.append(entry)

  if not versions:
    raise Exception(f"No MSVC tool versions found at {tools_path}")

  # Sort versions and pick the latest
  versions.sort()
  return os.path.join(tools_path, versions[-1])


def _run_vcvars_and_get_env(vs_path, arch):
  """Run vcvarsall.bat and extract the environment variables."""
  # vcvarsall.bat location
  vcvars = os.path.join(vs_path, "VC", "Auxiliary", "Build", "vcvarsall.bat")
  if not os.path.exists(vcvars):
    raise Exception(f"vcvarsall.bat not found at {vcvars}")

  # Map GN arch to vcvarsall arch argument
  arch_map = {
      "x86": "x86",
      "x64": "amd64",
      "arm64": "amd64_arm64",
  }
  if arch not in arch_map:
    raise Exception(f"Unsupported architecture: {arch}")
  vcvars_arch = arch_map[arch]

  # Run vcvarsall.bat and then print the environment
  cmd = f'"{vcvars}" {vcvars_arch} >nul && set'
  result = subprocess.run(
      cmd,
      shell=True,
      capture_output=True,
      text=True,
  )
  if result.returncode != 0:
    raise Exception(f"vcvarsall.bat failed with code {result.returncode}:\n"
                    f"{result.stderr}")

  # Parse environment variables
  env = {}
  for line in result.stdout.splitlines():
    if "=" in line:
      key, _, value = line.partition("=")
      key = key.strip().upper()
      env[key] = value.strip()

  return env


def _find_cl_in_path(env):
  """Find the directory containing cl.exe from the PATH."""
  path_entries = env.get("PATH", "").split(os.pathsep)
  for entry in path_entries:
    cl_path = os.path.join(entry, "cl.exe")
    if os.path.exists(cl_path):
      return entry
  raise Exception("cl.exe not found in PATH after running vcvarsall.bat")


def main():
  if len(sys.argv) < 3:
    print(
        "Usage: setup_toolchain.py <visual_studio_path> <target_cpu>",
        file=sys.stderr,
    )
    sys.exit(2)
  vs_path = sys.argv[1]
  target_cpu = sys.argv[2]
  # If VS path is empty, auto-detect
  if not vs_path:
    vs_path = _find_vs_with_vswhere()
  # Normalize path
  vs_path = os.path.normpath(vs_path)
  if not os.path.exists(vs_path):
    raise Exception(f"Visual Studio path does not exist: {vs_path}")
  # Get environment from vcvarsall.bat
  env = _run_vcvars_and_get_env(vs_path, target_cpu)
  # Find cl.exe directory
  vc_bin_dir = _find_cl_in_path(env)
  # Extract include paths from INCLUDE environment variable
  include_paths = []
  if "INCLUDE" in env:
    include_paths = [
        os.path.normpath(p) for p in env["INCLUDE"].split(";")
        if p and os.path.exists(p)
    ]
  # Extract lib paths from LIB environment variable
  lib_paths = []
  if "LIB" in env:
    lib_paths = [
        os.path.normpath(p) for p in env["LIB"].split(";")
        if p and os.path.exists(p)
    ]
  # Format flags for cl.exe (/I for includes)
  include_flags = " ".join(f'/I"{p}"' for p in include_paths)
  # Format flags for link.exe (/LIBPATH for lib paths)
  libpath_flags = " ".join(f'/LIBPATH:"{p}"' for p in lib_paths)
  # Output GN scope variables
  print(f"vc_bin_dir = {_to_gn_string(vc_bin_dir)}")
  print(f"include_flags = {_to_gn_string(include_flags)}")
  print(f"libpath_flags = {_to_gn_string(libpath_flags)}")
  print(f"vs_path = {_to_gn_string(vs_path)}")


if __name__ == "__main__":
  main()
