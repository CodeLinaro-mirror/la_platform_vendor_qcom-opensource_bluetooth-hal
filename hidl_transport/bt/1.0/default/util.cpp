/**
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: BSD-3-Clause-Clear.
 */

#include <android-base/file.h>
#include <android-base/strings.h>
#include <utils/Log.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "util.h"

namespace android {
namespace hardware {
namespace bluetooth {
namespace V1_0 {
namespace implementation {

#undef LOG_TAG
#define LOG_TAG "vendor.qti.bluetooth-util"

/* Property to switch BT chips between slot0 and slot1 */
#define BT_SWITCH_PROP              "persist.vendor.bt.switch"

/* Property to enable diag for BT */
#define BT_ENABLE_DIAG_PROP         "persist.vendor.bt.enable_diag"

/* Property for UART device which controls Bluetooth in slot0 */
#define BT_AIDL_UART_PROP           "persist.vendor.bt.hidl.uart"

/* Property for UART device which controls Bluetooth in slot1 */
#define BT_AIDL1_UART_PROP          "persist.vendor.bt.hidl1.uart"

#define UART_DEVICE                 "/dev/ttyHS0"
#define UART1_DEVICE                "/dev/ttyHS1"

#define BT_RFKILL_NAME              "bt_power"
#define BT_RFKILL1_NAME             "bt_power_new"

#define BT_SOC_PROP                 "persist.vendor.qcom.bluetooth.soc"
#define BT_SOC1_PROP                "persist.vendor.qcom.bluetooth.soc1"

/* Flag to enable diag for BT */
#define BT_ENABLE_DIAG_NONE         0x0
#define BT_ENABLE_DIAG_DEFAULT      0x1
#define BT_ENABLE_DIAG_NEW          0x2
#define BT_ENABLE_DIAG_ALL          0x3
#define BT_ENABLE_DIAG_MASK         BT_ENABLE_DIAG_ALL

int Util::hw_slot_ = 0;
bool Util::is_default_aidl_ = true;

void Util::init(const char *aidlInstanceName)
{
  bool switch_bt = isBtSwitchEnabled();
  is_default_aidl_ = (aidlInstanceName == NULL) ||
                     (strlen(aidlInstanceName) == 0) ||
                     (strcmp(aidlInstanceName, "/default") == 0);
  hw_slot_ = (aidlInstanceName != NULL) ?
            ((is_default_aidl_ && !switch_bt) ||
            (!is_default_aidl_ && switch_bt) ?
            BLUETOOTH_SLOT_0 :
            BLUETOOTH_SLOT_1) :
            BLUETOOTH_SLOT_0;
}

int Util::getHwSlot()
{
  return hw_slot_;
}

bool Util::isHwSlot1(int slot)
{
  return slot == BLUETOOTH_SLOT_1;
}

bool Util::isHwSlot1()
{
  return isHwSlot1(getHwSlot());
}

bool Util::isDefaultaidl()
{
  return is_default_aidl_;
}

const char* Util::getString(const char* str, const char* new_str)
{
  return isDefaultaidl() ? str : new_str;
}

const char* Util::getBtSocProp()
{
  return isHwSlot1() ? BT_SOC1_PROP : BT_SOC_PROP;
}

const char* Util::getBtRfkillName()
{
  return isHwSlot1() ? BT_RFKILL1_NAME : BT_RFKILL_NAME;
}

bool Util::getUartDevice(char *device)
{
  return getUartDevice(getHwSlot(), device);
}

bool Util::getUartDevice(int slot, char *device)
{
  if (!device)
    return false;

  switch (slot) {
    case BLUETOOTH_SLOT_0: {
      property_get(BT_AIDL_UART_PROP, device, UART_DEVICE);
      return true;
    }
    case BLUETOOTH_SLOT_1: {
      property_get(BT_AIDL1_UART_PROP, device, UART1_DEVICE);
      return true;
    }
    default: {
      /* Invalid Bluetooth slot */
      return false;
    }
  }
}

const char *Util::getBdAddrProp()
{
    return isHwSlot1() ? PROPERTY_BT_BDADDR1_PATH : PROPERTY_BT_BDADDR_PATH;
}

const char *Util::getPersistBdAddrProp()
{
    return isHwSlot1() ? PERSIST_BDADDR1_PROPERTY : PERSIST_BDADDR_PROPERTY;
}

const char *Util::getFactoryBdAddrProp()
{
    return isHwSlot1() ? FACTORY_BDADDR1_PROPERTY : FACTORY_BDADDR_PROPERTY;
}

bool Util::isBtSwitchEnabled()
{
  return getPropBool(BT_SWITCH_PROP);
}

bool Util :: isDiagEnabled()
{
  int32_t enable_diag_flag = getPropInt(BT_ENABLE_DIAG_PROP, BT_ENABLE_DIAG_DEFAULT);
  return isDefaultaidl() ?
        enable_diag_flag & BT_ENABLE_DIAG_DEFAULT :
        enable_diag_flag & BT_ENABLE_DIAG_NEW;
}

bool Util::fileExist(const char *file_name)
{
  int fd;

  if (!file_name)
    return false;

  fd = open(file_name, O_RDONLY);
  if (fd < 0)
    return false;

  close(fd);
  return true;
}

bool Util::fileContainString(const char *file_name, const char *str)
{
  int fd;
  ssize_t size;
  size_t str_len;
  char buf[PROPERTY_VALUE_MAX] = { '\0' };

  if (!file_name || !str)
    return false;

  fd = open(file_name, O_RDONLY);
  if (fd < 0)
    return false;

  size = read(fd, buf, sizeof(buf));
  close(fd);
  if (size > 0) {
    if (buf[size - 1] == '\n')
      size --;
  } else {
    return false;
  }

  str_len = strlen(str);

  return (size == str_len) ? !memcmp(buf, str, str_len) : false;
}

bool Util::getPropBool(const char *prop, bool def)
{
  return property_get_bool(prop, def);
}

int32_t Util::getPropInt(const char *prop, int32_t def)
{
  return property_get_int32(prop, def);
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace bluetooth
}  // namespace hardware
}  // namespace android
