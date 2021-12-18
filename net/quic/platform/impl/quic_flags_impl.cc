// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/quic/platform/impl/quic_flags_impl.h"

#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <limits>
#include <set>
#include <string>

#include "net/third_party/quiche/src/quic/platform/api/quic_logging.h"

#define DEFINE_QUIC_PROTOCOL_FLAG_SINGLE_VALUE(type, flag, value, doc) \
  type FLAGS_##flag = value;

#define DEFINE_QUIC_PROTOCOL_FLAG_TWO_VALUES(type, flag, internal_value, \
                                             external_value, doc)        \
  type FLAGS_##flag = external_value;

// Preprocessor macros can only have one definition.
// Select the right macro based on the number of arguments.
#define GET_6TH_ARG(arg1, arg2, arg3, arg4, arg5, arg6, ...) arg6
#define QUIC_PROTOCOL_FLAG_MACRO_CHOOSER(...) /* TODO(fangqiuhang): */
#define QUIC_PROTOCOL_FLAG(...) \
  QUIC_PROTOCOL_FLAG_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

#include "net/third_party/quiche/src/quic/core/quic_protocol_flags_list.h"

#undef QUIC_PROTOCOL_FLAG
#undef QUIC_PROTOCOL_FLAG_MACRO_CHOOSER
#undef GET_6TH_ARG
#undef DEFINE_QUIC_PROTOCOL_FLAG_TWO_VALUES
#undef DEFINE_QUIC_PROTOCOL_FLAG_SINGLE_VALUE

namespace quic {

namespace {

char* kWhitespaceASCII = " \t";

// Overload for platforms where base::CommandLine::StringType == std::string.
std::vector<std::string> __attribute__((unused))
ToQuicStringVector(const std::vector<std::string>& v) {
  return v;
}

#if defined(WCHAR_T_IS_UTF16)
// Overload for platforms where base::CommandLine::StringType == std::wstring.
std::vector<std::string> __attribute__((unused))
ToQuicStringVector(const std::vector<std::wstring>& v) {
  std::vector<std::string> qsv;
  // TODO(fangqiuhang):
  return qsv;
}
#endif  // defined(WCHAR_T_IS_UTF16)

size_t FindLineWrapPosition(const std::string& s, size_t desired_len) {
  if (s.length() <= desired_len) {
    return std::string::npos;
  }
  size_t pos = s.find_last_of(kWhitespaceASCII, desired_len);
  if (pos != std::string::npos) {
    return pos;
  }
  pos = s.find_first_of(kWhitespaceASCII, desired_len);
  if (pos != std::string::npos) {
    return pos;
  }
  return std::string::npos;
}

// Pretty-print a flag description in the format:
//
// --flag_name      Some text describing the flag that can
//                  wrap around to the next line.
void AppendFlagDescription(const std::string& name,
                           std::string help,
                           std::string* out) {
  const int kStartCol = 20;
  const int kEndCol = 80;
  const int kMinPadding = 2;
  static const char kDashes[] = "--";

  // TODO(fangqiuhang): maybe impl base::StrAppend
  out += kDashes + name;
  int col = strlen(kDashes) + name.length();
  if (col + kMinPadding < kEndCol) {
    // Start help text on same line
    int pad_len = std::max(kMinPadding, kStartCol - col);
    out += std::string(pad_len, ' ');
    col += pad_len;
  } else {
    // Start help text on next line
    out += "\n" + std::string(kStartCol, ' ');
    col = kStartCol;
  }

  while (!help.empty()) {
    size_t desired_len = kEndCol - col;
    size_t wrap_pos = FindLineWrapPosition(help, desired_len);
    if (wrap_pos == std::string::npos) {
      out += help;
      break;
    }
    out += help.substr(0, wrap_pos) + "\n" + std::string(kStartCol, ' ');
    help = help.substr(wrap_pos + 1);
    col = kStartCol;
  }
  out += "\n";
}

std::string ToLowerASCII(std::string s) {
  char buff[1000];
  char* ins = s.c_str();
  int i = 0;
  for (; i < s.length(); i++) {
    buff[i] = tolower(ins[i]);
  }
  buff[i] = '\0';
  return std::string(buff);
}

bool StringToInt(std::string& s, int* value) {
  *value = atoi(s.c_str());
  return true;
}

bool StringToInt64(std::string& s, int64_t value) {
  *value = atoi(s.c_str());
  return true;
}

}  // namespace

// static
QuicFlagRegistry& QuicFlagRegistry::GetInstance() {
  static QuicFlagRegistry instance;
  return instance;
}

void QuicFlagRegistry::RegisterFlag(const char* name,
                                    std::unique_ptr<QuicFlagHelper> helper) {
  flags_.emplace(std::string(name), std::move(helper));
}

void QuicFlagRegistry::ResetFlags() const {
  for (const auto& kv : flags_) {
    kv.second->ResetFlag();
    QUIC_LOG(INFO) << "Reset flag --" << kv.first;
  }
}

std::string QuicFlagRegistry::GetHelp() const {
  std::string help;
  AppendFlagDescription("help", "Print this help message.", &help);
  for (const auto& kv : flags_) {
    AppendFlagDescription(kv.first, kv.second->GetHelp(), &help);
  }
  return help;
}

template <>
bool TypedQuicFlagHelper<bool>::SetFlag(const std::string& s) const {
  static const std::set<std::string> kTrueValues(
      std::initializer_list<std::string>({"", "1", "t", "true", "y", "yes"}));
  static const std::set<std::string> kFalseValues(
      std::initializer_list<std::string>({"0", "f", "false", "n", "no"}));
  if (kTrueValues.find(ToLowerASCII(s)) != kTrueValues.end()) {
    *flag_ = true;
    return true;
  }
  if (kFalseValues.find(ToLowerASCII(s)) != kFalseValues.end()) {
    *flag_ = false;
    return true;
  }
  return false;
}

template <>
bool TypedQuicFlagHelper<uint16_t>::SetFlag(const std::string& s) const {
  int value;
  if (!StringToInt(s, &value) || value < std::numeric_limits<uint16_t>::min() ||
      value > std::numeric_limits<uint16_t>::max()) {
    return false;
  }
  *flag_ = static_cast<uint16_t>(value);
  return true;
}

template <>
bool TypedQuicFlagHelper<int32_t>::SetFlag(const std::string& s) const {
  int32_t value;
  if (!StringToInt(s, &value)) {
    return false;
  }
  *flag_ = value;
  return true;
}

template <>
bool TypedQuicFlagHelper<std::string>::SetFlag(const std::string& s) const {
  *flag_ = s;
  return true;
}

template class EXPORT_TEMPLATE_DEFINE(QUIC_EXPORT_PRIVATE)
    TypedQuicFlagHelper<bool>;
template class EXPORT_TEMPLATE_DEFINE(QUIC_EXPORT_PRIVATE)
    TypedQuicFlagHelper<uint16_t>;
template class EXPORT_TEMPLATE_DEFINE(QUIC_EXPORT_PRIVATE)
    TypedQuicFlagHelper<int32_t>;
template class EXPORT_TEMPLATE_DEFINE(QUIC_EXPORT_PRIVATE)
    TypedQuicFlagHelper<std::string>;

std::vector<std::string> QuicParseCommandLineFlagsImpl(
    const char* usage,
    int argc,
    const char* const* argv) {
  // TODO(fangqiuhang): parse args and set flags.

  return result.non_flag_args;
}

void QuicPrintCommandLineFlagHelpImpl(const char* usage) {
  std::cout << usage << std::endl
            << "Options:" << std::endl
            << QuicFlagRegistry::GetInstance().GetHelp() << std::endl;
}

QuicFlagRegistry::QuicFlagRegistry() = default;
QuicFlagRegistry::~QuicFlagRegistry() = default;
QuicParseCommandLineFlagsResult::QuicParseCommandLineFlagsResult() = default;
QuicParseCommandLineFlagsResult::QuicParseCommandLineFlagsResult(
    const QuicParseCommandLineFlagsResult&) = default;
QuicParseCommandLineFlagsResult::~QuicParseCommandLineFlagsResult() = default;

}  // namespace quic

namespace {

void SetQuicFlagByName_bool(bool* flag, const std::string& value) {
  if (value == "true" || value == "True")
    *flag = true;
  else if (value == "false" || value == "False")
    *flag = false;
}
void SetQuicFlagByName_double(double* flag, const std::string& value) {
  double val;
  if (StringToDouble(value, &val))
    *flag = val;
}

void SetQuicFlagByName_uint64_t(uint64_t* flag, const std::string& value) {
  uint64_t val;
  if (StringToUint64(value, &val) && val >= 0)
    *flag = val;
}

void SetQuicFlagByName_int32_t(int32_t* flag, const std::string& value) {
  int val;
  if (StringToInt(value, &val))
    *flag = val;
}

void SetQuicFlagByName_int64_t(int64_t* flag, const std::string& value) {
  int64_t val;
  if (StringToInt64(value, &val))
    *flag = val;
}

}  // namespace

void SetQuicFlagByName(const std::string& flag_name, const std::string& value) {
#define QUIC_FLAG(flag, default_value)    \
  if (flag_name == #flag) {               \
    SetQuicFlagByName_bool(&flag, value); \
    return;                               \
  }
#include "net/third_party/quiche/src/quic/core/quic_flags_list.h"
#undef QUIC_FLAG

#define QUIC_PROTOCOL_FLAG(type, flag, ...)         \
  if (flag_name == "FLAGS_" #flag) {                \
    SetQuicFlagByName_##type(&FLAGS_##flag, value); \
    return;                                         \
  }
#include "net/third_party/quiche/src/quic/core/quic_protocol_flags_list.h"
#undef QUIC_PROTOCOL_FLAG
}
