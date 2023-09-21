LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libshowlogo
LOCAL_SRC_FILES := main.c utils.c fb.c logo.c decompressor.c
LOCAL_STATIC_LIBRARIES := libfs_mgr libcutils libz
include $(BUILD_STATIC_LIBRARY)

