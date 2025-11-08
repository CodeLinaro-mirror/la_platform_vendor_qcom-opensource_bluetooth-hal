LOCAL_DIR_PATH:= $(call my-dir)
ifeq ($(BOARD_HAVE_BLUETOOTH_QCOM),true)

LOCAL_PATH := $(LOCAL_DIR_PATH)
TMP_LOCAL_PATH := $(LOCAL_DIR_PATH)
ifeq ($(TARGET_USE_QTI_BT_AIDL), true)
include $(LOCAL_PATH)/bt/aidl/default/Android.mk
include $(TMP_LOCAL_PATH)/bt/aidl/fuzzer/Android.mk
else
ifeq ($(BOARD_HAVE_QTI_BT_SERVICE_VER_1_1), true)
include $(LOCAL_PATH)/bt/1.1/default/Android.mk
endif # BOARD_HAVE_QTI_BT_SERVICE_VER_1_1
endif # TARGET_USE_QTI_BT_AIDL
include $(TMP_LOCAL_PATH)/bt/1.0/default/Android.mk

endif # BOARD_HAVE_BLUETOOTH_QCOM
