/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: BSD-3-Clause-Clear.
 */
/*
 * Copyright 2022 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "aidl.android.hardware.bluetooth.service-qti.default"

#include <aidl/android/hardware/bluetooth/BnBluetoothHci.h>
#include <aidl/android/hardware/bluetooth/IBluetoothHci.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <cutils/properties.h>
#include <utils/Log.h>

#include "BluetoothHci.h"
#include "util.h"

using android::hardware::bluetooth::V1_0::implementation::Util;
using ::aidl::android::hardware::bluetooth::implementation::BluetoothHci;
using ::aidl::android::hardware::bluetooth::BnBluetoothHci;

static std::shared_ptr<BluetoothHci> CreateBluetoothHci(const char* name) {
  char value[PROPERTY_VALUE_MAX] = { '\0' };
  property_get("persist.vendor.bluetooth.hal_mode", value, "");

  Util::init(name);
  return ndk::SharedRefBase::make<BluetoothHci>(Util::getHwSlot());
}

int main(int /* argc */, char** /* argv */) {
  ALOGI("Bluetooth HAL starting");
  if (!ABinderProcess_setThreadPoolMaxThreadCount(2)) {
    ALOGI("failed to set thread pool max thread count");
    return 1;
  }

#ifdef DUAL_BT
  const char *bt_hci_instance = "/hci1";
#else
  const char *bt_hci_instance = "/default";
#endif

  std::shared_ptr<BluetoothHci> service = CreateBluetoothHci(bt_hci_instance);
  if (service == nullptr) {
    ALOGE("Fail to create BluetoothHci");
    return 1;
  }
  std::string instance = std::string() + BluetoothHci::descriptor + bt_hci_instance;
  auto result =
      AServiceManager_addService(service->asBinder().get(), instance.c_str());
  if (result == STATUS_OK) {
    ABinderProcess_joinThreadPool();
  } else {
    ALOGE("Could not register as a service!");
  }
  return 0;
}
