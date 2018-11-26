#
# Copyright 2013 The Android Open-Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# Include all makefiles in sub-directories (one level deep)
include $(call all-subdir-makefiles)

$(call inherit-product, $(SRC_TARGET_DIR)/product/aosp_x86_64.mk)

# Include all packages from this file
include $(LOCAL_PATH)/packages.mk

# Override inherited values
# I found that some things in the emulator require to have generic as the
# device name, so I had to keep PRODUCT_DEVICE as the original one
PRODUCT_NAME := full_alpha
PRODUCT_MODEL := Full Alpha Image for Emulator

# Include SELinux policy additions for our services
BOARD_SEPOLICY_DIRS += device/eightman/alpha/sepolicy
BOARD_PLAT_PRIVATE_SEPOLICY_DIR := device/eightman/alpha/sepolicy/private

