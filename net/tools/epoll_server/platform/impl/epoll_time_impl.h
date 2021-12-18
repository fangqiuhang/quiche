// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TOOLS_EPOLL_SERVER_PLATFORM_IMPL_EPOLL_TIME_IMPL_H_
#define NET_TOOLS_EPOLL_SERVER_PLATFORM_IMPL_EPOLL_TIME_IMPL_H_

namespace epoll_server {

inline int64_t WallTimeNowInUsecImpl() {
  // TODO(fangqiuhang):
  return 0;
}

}  // namespace epoll_server

#endif  // NET_TOOLS_EPOLL_SERVER_PLATFORM_IMPL_EPOLL_TIME_IMPL_H_
