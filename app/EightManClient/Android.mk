LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := $(call all-java-files-under,src)
LOCAL_JAVA_LIBRARIES := com.customdev.service.powermanager
LOCAL_PRIVATE_PLATFORM_APIS := true
LOCAL_PACKAGE_NAME := EightManClient

include $(BUILD_PACKAGE)
