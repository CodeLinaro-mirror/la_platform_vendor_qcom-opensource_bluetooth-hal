/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: BSD-3-Clause-Clear.
 */

#pragma once

#include <aidl/android/hardware/bluetooth/IBluetoothHci.h>

using aidl::android::hardware::bluetooth::IBluetoothHci;


/**
 * Create Bluetooth AIDL instance for QCSR BlueCore (e.g. CSR8311)
 *
 *   name: Bluetooth AIDL instance's name (e.g. "default")
 *
 *   return: Bluetooth AIDL instance for BlueCore
 */
std::shared_ptr<IBluetoothHci> create_bluetooth_hci_bluecore(const char* name);

/**
 * Create Bluetooth AIDL instance for RPC
 *
 *   name: Bluetooth AIDL instance's name (e.g. "default")
 *
 *   return: Bluetooth AIDL instance for RPC
 */
std::shared_ptr<IBluetoothHci> create_bluetooth_hci_rpc(const char* name);
