// Copyright (c) 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QUICHE_QUIC_PLATFORM_API_QUIC_LOGGING_H_
#define QUICHE_QUIC_PLATFORM_API_QUIC_LOGGING_H_

#include "quiche/common/platform/api/quiche_logging.h"

// Please note following QUIC_LOG are platform dependent:
// INFO severity can be degraded (to VLOG(1) or DVLOG(1)).
// Some platforms may not support QUIC_LOG_FIRST_N or QUIC_LOG_EVERY_N_SEC, and
// they would simply be translated to LOG.

// On MinGW/MSVC, wingdi.h (unconditionally included via windows.h) defines
// ERROR as 0, which breaks the normal macro pass-through chain. Use ##severity
// directly to prevent the expansion. QUICHE_MINGW/QUICHE_MSVC is defined by the
// GN build config when on Windows.
#if defined(QUICHE_MINGW) || defined(QUICHE_MSVC)
#define QUIC_DLOG(severity) QUICHE_DLOG_IMPL_##severity()
#define QUIC_DLOG_IF(severity, condition) \
  QUICHE_DLOG_IF_IMPL_##severity(condition)
#define QUIC_LOG(severity) QUICHE_LOG_IMPL_##severity()
#define QUIC_LOG_FIRST_N(severity, n) QUICHE_LOG_FIRST_N_IMPL_##severity(n)
#define QUIC_LOG_EVERY_N_SEC(severity, seconds) \
  QUICHE_LOG_EVERY_N_SEC_IMPL_##severity(seconds)
#define QUIC_LOG_IF(severity, condition) \
  QUICHE_LOG_IF_IMPL_##severity(condition)
#define QUIC_PLOG(severity) QUICHE_PLOG_IMPL_##severity()
#else
#define QUIC_DLOG QUICHE_DLOG
#define QUIC_DLOG_IF QUICHE_DLOG_IF
#define QUIC_LOG QUICHE_LOG
#define QUIC_LOG_FIRST_N QUICHE_LOG_FIRST_N
#define QUIC_LOG_EVERY_N_SEC QUICHE_LOG_EVERY_N_SEC
#define QUIC_LOG_IF QUICHE_LOG_IF
#define QUIC_PLOG QUICHE_PLOG
#endif

#define QUIC_DVLOG QUICHE_DVLOG
#define QUIC_DVLOG_IF QUICHE_DVLOG_IF
#define QUIC_VLOG QUICHE_VLOG

#define QUIC_DLOG_INFO_IS_ON QUICHE_DLOG_INFO_IS_ON
#define QUIC_LOG_INFO_IS_ON QUICHE_LOG_INFO_IS_ON
#define QUIC_LOG_WARNING_IS_ON QUICHE_LOG_WARNING_IS_ON
#define QUIC_LOG_ERROR_IS_ON QUICHE_LOG_ERROR_IS_ON

#endif  // QUICHE_QUIC_PLATFORM_API_QUIC_LOGGING_H_
