/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: BSD-3-Clause-Clear.
 */

#include <fuzzbinder/libbinder_ndk_driver.h>
#include <fuzzer/FuzzedDataProvider.h>

#include <aidl/android/hardware/bluetooth/IBluetoothHci.h>
#include <cutils/properties.h>
#include "BluetoothHci.h"

using ::aidl::android::hardware::bluetooth::implementation::BluetoothHci;
using ::aidl::android::hardware::bluetooth::BnBluetoothHci;

std::shared_ptr<BnBluetoothHci> service;

extern "C" int LLVMFuzzerInitialize(int* /* argc */, char*** /* argv */) {
  char value[PROPERTY_VALUE_MAX] = { '\0' };
  property_get("persist.vendor.bluetooth.hal_mode", value, "");

  service = ndk::SharedRefBase::make<BluetoothHci>();
  return 0;
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  if  (service == nullptr ) {
    return -1;
  }

  android::fuzzService(service->asBinder().get(), FuzzedDataProvider(data, size));
  return 0;
}
