LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := android.hardware.bluetooth@1.0-impl-qti
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_VENDOR_MODULE := true

ifeq ($(UART_BAUDRATE_3_0_MBPS),true)
LOCAL_CPPFLAGS := -DUART_BAUDRATE_3_0_MBPS
endif

$(info PRODUCT_DEVICE=$(PRODUCT_DEVICE))
ifneq (,$(filter gen5_gvm gen5_gvm_gy gen4_gvm_gy gen5_gvm_sgt, $(PRODUCT_DEVICE)))
LOCAL_CPPFLAGS += -DTARGET_QTI_VM
endif

LOCAL_SRC_FILES := \
    bluetooth_hci.cpp \
    bluetooth_address.cpp \
    async_fd_watcher.cpp \
    hci_packetizer.cpp \
    soc_properties.cpp \
    data_handler.cpp \
    uart_controller.cpp \
    patch_dl_manager.cpp \
    hci_uart_transport.cpp \
    power_manager.cpp \
    ibs_handler.cpp \
    logger.cpp \
    uart_ipc.cpp \
    uart_logs.cpp \
    ring_buffer.cpp \
    wake_lock.cpp \
    diag_interface.cpp \
    mct_controller.cpp \
    hci_mct_transport.cpp \
    nvm_tags_manager.cpp \
    health_info_log.cpp \
    device_management_service_v01.c \
    state_info.cpp\
    util.cpp

ifeq ($(BOARD_HAVE_DUAL_BLUETOOTH), true)
LOCAL_CPPFLAGS += -DDUAL_BT
endif

ifeq ($(BOARD_HAVE_QTI_BT_SERVICE_VER_1_1), true)
LOCAL_CFLAGS += -DBT_VER_1_1
endif

ifeq ($(ENABLE_PERIPHERAL_STATE_UTILS), true)
LOCAL_CFLAGS += -DBT_SECURE_PERIPHERAL_ENABLED
endif

LOCAL_CFLAGS += -DDIAG_ENABLED
LOCAL_CFLAGS += -Werror=unused-variable
LOCAL_CFLAGS += -Wno-unused-lambda-capture

# disable below flag to disable IBS/OBS
ifeq ($(TARGET_USE_QTI_BT_OBS), true)
LOCAL_CFLAGS += -DWCNSS_OBS_ENABLED
LOCAL_SRC_FILES += obs_handler.cpp
else
LOCAL_CFLAGS += -DWCNSS_IBS_ENABLED
endif

# Disable this flag for disabling wakelocks
LOCAL_CFLAGS += -DWAKE_LOCK_ENABLED

ifeq ($(BOARD_HAVE_QTI_BT_LAZY_SERVICE),true)
LOCAL_CFLAGS += -DLAZY_SERVICE
endif

ifneq (,$(filter userdebug eng,$(TARGET_BUILD_VARIANT)))
LOCAL_CFLAGS += -DDUMP_IPC_LOG -DDUMP_RINGBUF_LOG -DDETECT_SPURIOUS_WAKE -DENABLE_HEALTH_TIMER
LOCAL_CFLAGS += -DENABLE_FW_CRASH_DUMP -DUSER_DEBUG
endif
ifeq ($(TARGET_HAS_BT_QCV_FOR_SPF), true)
LOCAL_CPPFLAGS += -DQTI_BT_QCV_SUPPORTED
endif #TARGET_HAS_BT_QCV_FOR_SPF

ifeq ($(TARGET_BOARD_AUTO),true)
LOCAL_CPPFLAGS += -DTARGET_BOARD_AUTO
endif # TARGET_BOARD_AUTO

LOCAL_C_INCLUDES += $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr/include

LOCAL_ADDITIONAL_DEPENDENCIES += \
$(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr

LOCAL_SHARED_LIBRARIES := \
    libbase \
    libcutils \
    libhidlbase \
    liblog \
    libutils \
    libdiag \
    libqmi_cci \
    libqmi_common_so \
    libbtnv \
    android.hardware.bluetooth@1.0 \
    libsoc_helper

ifeq ($(TARGET_EXCLUDE_QTI_BT_HIDL), true)
LOCAL_CFLAGS += -DEXCLUDE_BT_HIDL
endif

LOCAL_HEADER_LIBRARIES := libril-qc-qmi-services-headers libdiag_headers vendor_common_inc

include $(BUILD_SHARED_LIBRARY)

ifneq ($(TARGET_EXCLUDE_QTI_BT_HIDL), true)
include $(CLEAR_VARS)
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_VENDOR_MODULE := true

ifeq ($(BOARD_HAVE_QTI_BT_LAZY_SERVICE),true)
LOCAL_MODULE := android.hardware.bluetooth@1.0-service-qti-lazy
LOCAL_CPPFLAGS += -DLAZY_SERVICE
ifeq ($(TARGET_BOARD_PLATFORM), msm8937)
LOCAL_INIT_RC := lazy-hal-rc/msm8937_32go/android.hardware.bluetooth@1.0-service-qti-lazy.rc
else
LOCAL_INIT_RC := lazy-hal-rc/common/android.hardware.bluetooth@1.0-service-qti-lazy.rc
endif
else
LOCAL_MODULE := android.hardware.bluetooth@1.0-service-qti
ifneq (,$(filter userdebug eng, $(TARGET_BUILD_VARIANT)))
LOCAL_INIT_RC := android.hardware.bluetooth@1.0-service-qti-debug.rc
else
LOCAL_INIT_RC := android.hardware.bluetooth@1.0-service-qti.rc
endif
endif #BOARD_HAVE_QTI_BT_LAZY_SERVICE

LOCAL_SRC_FILES := \
  service.cpp \
  util.cpp

LOCAL_SHARED_LIBRARIES := \
  liblog \
  libcutils \
  libutils \
  libhidlbase \
  libbinder_ndk \
  android.hardware.bluetooth@1.0

LOCAL_C_INCLUDES += vendor/qcom/opensource/bluetooth-hal/hidl_transport/bttpi/default/

LOCAL_HEADER_LIBRARIES := libdiag_headers vendor_common_inc

ifeq ($(TARGET_USE_QTI_VND_FWK_DETECT),true)
LOCAL_SHARED_LIBRARIES += libqti_vndfwk_detect_vendor
LOCAL_CPPFLAGS += -DQTI_VND_FWK_DETECT_SUPPORTED
endif # TARGET_USE_QTI_VND_FWK_DETECT

include $(BUILD_EXECUTABLE)
endif
