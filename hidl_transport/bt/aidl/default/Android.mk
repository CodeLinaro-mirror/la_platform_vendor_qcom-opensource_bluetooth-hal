LOCAL_PATH:= $(call my-dir)

ifndef BOARD_HAVE_DUAL_BLUETOOTH
BOARD_HAVE_DUAL_BLUETOOTH := true
endif

include $(CLEAR_VARS)
LOCAL_MODULE := android.hardware.bluetooth-impl-qti
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_VENDOR_MODULE := true
LOCAL_SRC_FILES := \
    BluetoothHci.cpp

LOCAL_CFLAGS += -Werror=unused-variable

LOCAL_C_INCLUDES += vendor/qcom/opensource/bluetooth-hal/hidl_transport/bt/1.0/default

LOCAL_SHARED_LIBRARIES := \
    libbase \
    libbinder \
    libbinder_ndk \
    libcutils \
    liblog \
    libutils \
    libdiag \
    libqmiservices \
    libqmi_cci \
    libbtnv \
    libsoc_helper \
    android.hardware.bluetooth-V1-ndk \
    android.hardware.bluetooth@1.0 \
    android.hardware.bluetooth@1.0-impl-qti

LOCAL_HEADER_LIBRARIES := libril-qc-qmi-services-headers libdiag_headers vendor_common_inc

include $(BUILD_SHARED_LIBRARY)

ifeq ($(TARGET_USE_QTI_BT_AIDL), true)
include $(CLEAR_VARS)
LOCAL_MODULE := android.hardware.bluetooth-service-qti
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_VENDOR_MODULE := true

LOCAL_INIT_RC := android.hardware.bluetooth-service-qti.rc
LOCAL_VINTF_FRAGMENTS := android.hardware.bluetooth-service-qti.xml

LOCAL_SRC_FILES := \
  service.cpp

LOCAL_SHARED_LIBRARIES := \
    libbinder \
    libbinder_ndk \
    liblog \
    libcutils \
    libutils \
    libhidlbase \
    android.hardware.bluetooth-V1-ndk \
    android.hardware.bluetooth-impl-qti\
    android.hardware.bluetooth@1.0 \
    android.hardware.bluetooth@1.0-impl-qti

LOCAL_C_INCLUDES += \
    vendor/qcom/opensource/bluetooth-hal/hidl_transport/bt/1.0/default \

include $(BUILD_EXECUTABLE)
endif

#
## New BT HIDL for the 2nd BT chip
#
ifeq ($(BOARD_HAVE_DUAL_BLUETOOTH),true)
include $(CLEAR_VARS)
LOCAL_MODULE := android.hardware.bluetooth-service-qti1
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_VENDOR_MODULE := true

LOCAL_SRC_FILES := \
  service.cpp

LOCAL_SHARED_LIBRARIES := \
    libbinder \
    libbinder_ndk \
    liblog \
    libcutils \
    libutils \
    libhidlbase \
    android.hardware.bluetooth-V1-ndk \
    android.hardware.bluetooth-impl-qti \
    android.hardware.bluetooth@1.0 \
    android.hardware.bluetooth@1.0-impl-qti

LOCAL_CPPFLAGS += -DDUAL_BT

LOCAL_C_INCLUDES += \
    vendor/qcom/opensource/bluetooth-hal/hidl_transport/bt/1.0/default \
    vendor/qcom/opensource/bluetooth-hal/hidl_transport/bt/aidl/api

include $(BUILD_EXECUTABLE)

endif # BOARD_HAVE_DUAL_BLUETOOTH
