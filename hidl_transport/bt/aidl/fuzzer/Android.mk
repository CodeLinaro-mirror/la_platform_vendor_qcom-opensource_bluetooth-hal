LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := aidl_fuzzer_bluetooth_hal
LOCAL_VENDOR_MODULE := true

LOCAL_HEADER_LIBRARIES := \
    libdiag_headers \
    vendor_common_inc

LOCAL_C_INCLUDES := \
    vendor/qcom/opensource/bluetooth-hal/hidl_transport/bt/1.0/default \
    vendor/qcom/opensource/bluetooth-hal/hidl_transport/bt/aidl/api \
    vendor/qcom/opensource/bluetooth-hal/hidl_transport/bt/aidl/default

LOCAL_SRC_FILES := \
    fuzzer.cpp

LOCAL_SHARED_LIBRARIES := \
    libbase \
    libbinder \
    libbinder_ndk \
    libcutils \
    liblog \
    libutils \
    libdiag \
    libsoc_helper \
    android.hardware.bluetooth-V1-ndk \
    android.hardware.bluetooth-impl-qti \
    android.hardware.bluetooth@1.0 \
    android.hardware.bluetooth@1.0-impl-qti

ifeq ($(TARGET_USE_QTI_BT_HAL_RPC), true)
LOCAL_CFLAGS += -DENABLE_BT_HAL_RPC
LOCAL_SHARED_LIBRARIES += libqti-bluetooth-rpc
endif

LOCAL_STATIC_LIBRARIES := \
    libbinder_random_parcel

include $(BUILD_FUZZ_TEST)
