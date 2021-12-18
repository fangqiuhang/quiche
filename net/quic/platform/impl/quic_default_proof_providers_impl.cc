// Copyright (c) 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/quic/platform/impl/quic_default_proof_providers_impl.h"

#include <utility>

#include "net/quic/platform/impl/quic_chromium_clock.h"
#include "net/third_party/quiche/src/quic/platform/api/quic_flags.h"
#include "net/third_party/quiche/src/quic/tools/simple_ticket_crypter.h"

DEFINE_QUIC_COMMAND_LINE_FLAG(
    bool,
    allow_unknown_root_cert,
    false,
    "If true, don't restrict cert verification to known roots");

DEFINE_QUIC_COMMAND_LINE_FLAG(std::string,
                              certificate_file,
                              "",
                              "Path to the certificate chain.");

DEFINE_QUIC_COMMAND_LINE_FLAG(std::string,
                              key_file,
                              "",
                              "Path to the pkcs8 private key.");

namespace quic {

namespace {

std::set<std::string> UnknownRootAllowlistForHost(std::string host) {
  if (!GetQuicFlag(FLAGS_allow_unknown_root_cert)) {
    return std::set<std::string>();
  }
  return {host};
}

}  // namespace

std::unique_ptr<ProofVerifier> CreateDefaultProofVerifierImpl(
    const std::string& host) {
  // TODO(fangqiuhang):
  return nullptr;
}

std::unique_ptr<ProofSource> CreateDefaultProofSourceImpl() {
  // TODO(fangqiuhang):
  return nullptr;
}

}  // namespace quic
