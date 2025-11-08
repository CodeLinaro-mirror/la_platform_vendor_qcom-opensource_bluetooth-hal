//
// Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
// SPDX-License-Identifier: BSD-3-Clause-Clear.
//
// Copyright 2016 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#include <sys/types.h>
#include <sys/stat.h>
#include <android/hardware/bluetooth/1.0/IBluetoothHci.h>
#include <hidl/LegacySupport.h>
#include <cutils/properties.h>
#include <hwbinder/ProcessState.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>

#ifdef QTI_BT_QCV_SUPPORTED
#include  "soc_properties.h"
extern "C" {
#include "libsoc_helper.h"
}
#endif

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#ifdef LAZY_SERVICE
#define LOG_TAG "android.hardware.bluetooth@1.0-service-lazy"
#else
#define LOG_TAG "android.hardware.bluetooth@1.0-service"
#endif

// Generated HIDL files
using android::hardware::bluetooth::V1_0::IBluetoothHci;

using android::hardware::registerLazyPassthroughServiceImplementation;
using android::hardware::registerPassthroughServiceImplementation;
using android::hardware::defaultPassthroughServiceImplementation;
using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;
using android::OK;

#ifdef QTI_VND_FWK_DETECT_SUPPORTED
#ifdef __cplusplus
extern "C" {
#include "vndfwk-detect.h"
}
#endif
#endif

static const int BT_TX_RT_PRIORITY = 1;

int main() {
    ALOGI("BT-Transport driver main");
    (void)umask(S_IWGRP | S_IWOTH);

    struct sched_param rt_params;
    rt_params.sched_priority = BT_TX_RT_PRIORITY;
/*  TBD: For future
    if (sched_setscheduler(gettid(), SCHED_FIFO, &rt_params)) {
     ALOGE("%s unable to set SCHED_FIFO for pid %d, tid %d, error %s", __func__,
            getpid(), gettid(), strerror(errno));
    }
*/

    //Initialize Mmap size the moment the process starts
    android::hardware::ProcessState::initWithMmapSize((size_t)(256144));

    configureRpcThreadpool(1, true /*callerWillJoin*/);

    bool isVendorEnhancedFramework;
#ifdef QTI_BT_QCV_SUPPORTED
    setVendorPropertiesDefault();
#endif

#ifdef QTI_VND_FWK_DETECT_SUPPORTED
    /* 0: Pure AOSP for both system and odm
       1: Pure AOSP for system and QC Value-adds for odm
       2: QC value-adds for system and Pure AOSP for odm
       3: QC value-adds for both system and odm
    */
    int vendorEnhancedInfo = getVendorEnhancedInfo();
    ALOGI("vendorEnhancedInfo: %d", vendorEnhancedInfo);
    isVendorEnhancedFramework = (vendorEnhancedInfo & 1) ? true : false;
#else
    isVendorEnhancedFramework = false;
#endif

    ALOGI("isVendorEnhancedFramework: %d", isVendorEnhancedFramework);
    ALOGI("Registering BT Service");

    android::status_t status;

#ifdef LAZY_SERVICE
    status = registerLazyPassthroughServiceImplementation<IBluetoothHci>();
#else
    status = registerPassthroughServiceImplementation<IBluetoothHci>();
#endif

    if (status != OK)
      ALOGI("Error while registering BT service: %d", status);

    ALOGI("BTTPI: Main, joinRpcThreadpool for HIDL");
    joinRpcThreadpool();
    return status;
}
