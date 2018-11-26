/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _CUSTOMEDEV_CUSTOM_POWER_MANAGER_SERVICE_H
#define _CUSTOMEDEV_CUSTOM_POWER_MANAGER_SERVICE_H

#include <nativehelper/JNIHelp.h>
#include "jni.h"

#include <powermanager/PowerManager.h>

namespace customdev {

extern void com_customdev_CustomPowerManagerService_userActivity(nsecs_t eventTime, int32_t eventType);

} // namespace android

#endif // _CUSTOMEDEV_CUSTOM_POWER_MANAGER_SERVICE_H