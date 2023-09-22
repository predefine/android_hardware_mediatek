LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libshowlogo
LOCAL_SRC_FILES := main.c utils.c fb.c logo.c decompressor.c
LOCAL_SHARED_LIBRARIES := libcutils libz
LOCAL_STATIC_LIBRARIES := libfs_mgr
include $(BUILD_SHARED_LIBRARY)

