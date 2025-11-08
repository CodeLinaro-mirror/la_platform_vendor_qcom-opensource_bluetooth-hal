/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: BSD-3-Clause-Clear.
 */

#pragma once

#include <cutils/properties.h>

namespace android {
namespace hardware {
namespace bluetooth {
namespace V1_0 {
namespace implementation {

#define BLUETOOTH_SLOT_0            0
#define BLUETOOTH_SLOT_1            1
#define MAX_BLUETOOTH_SLOT          2

// The property key stores the storage location of Bluetooth Device Address
#define PROPERTY_BT_BDADDR_PATH     "ro.vendor.bt.bdaddr_path"
#define PROPERTY_BT_BDADDR1_PATH    "ro.vendor.bt.bdaddr1_path"

// Check for a legacy address stored as a property.
#define PERSIST_BDADDR_PROPERTY     "persist.vendor.service.bdroid.bdaddr"
#define PERSIST_BDADDR1_PROPERTY    "persist.vendor.service.bdroid.bdaddr1"

// If there is no valid bdaddr available from PROPERTY_BT_BDADDR_PATH and there
// is no available persistent bdaddr available from PERSIST_BDADDR_PROPERTY,
// use a factory set address.
#define FACTORY_BDADDR_PROPERTY     "ro.vendor.bt.boot.macaddr"
#define FACTORY_BDADDR1_PROPERTY    "ro.vendor.bt.boot.macaddr1"


class Util {
public:
  static void init(const char *aidlInstanceName);

  /* Get Bluetooth hardware slot */
  static int getHwSlot();

  static bool isHwSlot1(int slot);
  static bool isHwSlot1();

  static bool isDefaultaidl();

  static const char* getString(const char* str, const char* new_str);

  static const char* getBtSocProp();

  static const char* getBtAvailableProp();

  static const char* getBtRfkillName();

  static bool getUartDevice(char *device);

  static const char *getBdAddrProp();
  static const char *getPersistBdAddrProp();
  static const char *getFactoryBdAddrProp();

  static bool isDiagEnabled();

  static bool fileExist(const char *file_name);

  static bool fileContainString(const char *file_name, const char *str);

private:
  static bool isBtSwitchEnabled();

  static bool getUartDevice(int slot, char *device);

  static bool getPropBool(const char *prop, bool def = false);

  static int32_t getPropInt(const char *prop, int32_t def = 0);

  static int hw_slot_;
  static bool is_default_aidl_;
};

} // namespace implementation
} // namespace V1_0
} // namespace bluetooth
} // namespace hardware
} // namespace android
