/*==========================================================================
Description
  It has implementation for IPC logging mechanism.

# Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
# SPDX-License-Identifier: BSD-3-Clause-Clear.

===========================================================================*/
#pragma once

#include <hidl/HidlSupport.h>
#define UART_LOG_PREFIX "ramdump_bt_uart_ftrace_"
#define NEW_UART_LOG_PREFIX "ramdump_new_bt_uart_ftrace_"

#define UART_LOG_PATH_BUF_SIZE (255)
#define COMPLETE_UART_LOGS (0xFFFFFFFF)

namespace android {
namespace hardware {
namespace bluetooth {
namespace V1_0 {
namespace implementation {

class UartLogs {
 private:
  void *logger_;
  bool DumpUartLogs(const char *, const char *, long);

 public:
  const unsigned long UART_LOG_MAX_SIZE = COMPLETE_UART_LOGS;
  const unsigned long UART_LOG_MAX_READ_PER_ITERATION = 16 * 1024;

  void DumpLogs();
  UartLogs() {};
};

} // namespace implementation
} // namespace V1_0
} // namespace bluetooth
} // namespace hardware
} // namespace android
