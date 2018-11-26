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

#define LOG_TAG "CustomPowerManagerService-JNI"

#define LOG_NDEBUG 1

#include <android/hardware/power/1.1/IPower.h>
#include <nativehelper/JNIHelp.h>
#include "jni.h"

#include <nativehelper/ScopedUtfChars.h>

#include <limits.h>

#include <android-base/chrono_utils.h>
#include <android_runtime/AndroidRuntime.h>
#include <android_runtime/Log.h>
#include <utils/Timers.h>
#include <utils/misc.h>
#include <utils/String8.h>
#include <utils/Log.h>
#include <hardware/power.h>
#include <hardware_legacy/power.h>
#include <suspend/autosuspend.h>

#include "com_customdev_CustomPowerManagerService.h"

using android::hardware::Return;
using android::hardware::Void;
using android::hardware::power::V1_1::IPower;
using android::hardware::power::V1_0::PowerHint;
using android::hardware::power::V1_0::Feature;
using android::String8;

namespace customdev {


// ----------------------------------------------------------------------------

static jobject gPowerManagerServiceObj;
android::sp<android::hardware::power::V1_0::IPower> gPowerHalV1_0 = nullptr;
android::sp<android::hardware::power::V1_1::IPower> gPowerHalV1_1 = nullptr;
bool gPowerHalExists = true;
std::mutex gPowerHalMutex;


// Check validity of current handle to the power HAL service, and call getService() if necessary.
// The caller must be holding gPowerHalMutex.
bool getPowerHal() {
    if (gPowerHalExists && gPowerHalV1_0 == nullptr) {
        gPowerHalV1_0 = android::hardware::power::V1_0::IPower::getService();
        if (gPowerHalV1_0 != nullptr) {
            gPowerHalV1_1 =  android::hardware::power::V1_1::IPower::castFrom(gPowerHalV1_0);
            ALOGI("Loaded power HAL service");
        } else {
            ALOGI("Couldn't load power HAL service");
            gPowerHalExists = false;
        }
    }
    return gPowerHalV1_0 != nullptr;
}

// Check if a call to a power HAL function failed; if so, log the failure and invalidate the
// current handle to the power HAL service. The caller must be holding gPowerHalMutex.
static void processReturn(const Return<void> &ret, const char* functionName) {
    if (!ret.isOk()) {
        ALOGE("%s() failed: power HAL service not available.", functionName);
        gPowerHalV1_0 = nullptr;
    }
}

// ----------------------------------------------------------------------------

static void nativeInit(JNIEnv* env, jobject obj) {
    gPowerManagerServiceObj = env->NewGlobalRef(obj);

    gPowerHalMutex.lock();
    getPowerHal();
    gPowerHalMutex.unlock();
}

static void nativeAcquireSuspendBlocker(JNIEnv *env, jclass /* clazz */, jstring nameStr) {
    ScopedUtfChars name(env, nameStr);
    acquire_wake_lock(PARTIAL_WAKE_LOCK, name.c_str());
}

static void nativeReleaseSuspendBlocker(JNIEnv *env, jclass /* clazz */, jstring nameStr) {
    ScopedUtfChars name(env, nameStr);
    release_wake_lock(name.c_str());
}

static void nativeSetInteractive(JNIEnv* /* env */, jclass /* clazz */, jboolean enable) {
    ALOGD("nativeSetInteractive %s", enable ? "true" : "false");
    std::lock_guard<std::mutex> lock(gPowerHalMutex);
    if (getPowerHal()) {
        android::base::Timer t;
        Return<void> ret = gPowerHalV1_0->setInteractive(enable);
        processReturn(ret, "setInteractive");
        if (t.duration() > 20ms) {
            ALOGD("Excessive delay in setInteractive(%s) while turning screen %s",
                  enable ? "true" : "false", enable ? "on" : "off");
        }
    }
}

static void nativeSetAutoSuspend(JNIEnv* /* env */, jclass /* clazz */, jboolean enable) {
    if (enable) {
        android::base::Timer t;
        autosuspend_enable();
        if (t.duration() > 100ms) {
            ALOGD("Excessive delay in autosuspend_enable() while turning screen off");
        }
    } else {
        android::base::Timer t;
        autosuspend_disable();
        if (t.duration() > 100ms) {
            ALOGD("Excessive delay in autosuspend_disable() while turning screen on");
        }
    }
}

static void nativeSendPowerHint(JNIEnv *env, jclass clazz, jint hintId, jint data) {
    std::lock_guard<std::mutex> lock(gPowerHalMutex);
    if (getPowerHal()) {
        Return<void> ret;
        if (gPowerHalV1_1 != nullptr) {
            ret =  gPowerHalV1_1->powerHintAsync((PowerHint)hintId, data);
        } else {
            ret =  gPowerHalV1_0->powerHint((PowerHint)hintId, data);
        }
        processReturn(ret, "powerHint");
    }
}

static void nativeSetFeature(JNIEnv *env, jclass clazz, jint featureId, jint data) {
    std::lock_guard<std::mutex> lock(gPowerHalMutex);
    if (getPowerHal()) {
        Return<void> ret = gPowerHalV1_0->setFeature((Feature)featureId, static_cast<bool>(data));
        processReturn(ret, "setFeature");
    }
}

// ----------------------------------------------------------------------------

static const JNINativeMethod gPowerManagerServiceMethods[] = {
    /* name, signature, funcPtr */
    { "nativeInit", "()V",
            (void*) nativeInit },
    { "nativeAcquireSuspendBlocker", "(Ljava/lang/String;)V",
            (void*) nativeAcquireSuspendBlocker },
    { "nativeReleaseSuspendBlocker", "(Ljava/lang/String;)V",
            (void*) nativeReleaseSuspendBlocker },
    { "nativeSetInteractive", "(Z)V",
            (void*) nativeSetInteractive },
    { "nativeSetAutoSuspend", "(Z)V",
            (void*) nativeSetAutoSuspend },
    { "nativeSendPowerHint", "(II)V",
            (void*) nativeSendPowerHint },
    { "nativeSetFeature", "(II)V",
            (void*) nativeSetFeature },
};

#define FIND_CLASS(var, className) \
        var = env->FindClass(className); \
        LOG_FATAL_IF(! (var), "Unable to find class " className);

#define GET_METHOD_ID(var, clazz, methodName, methodDescriptor) \
        var = env->GetMethodID(clazz, methodName, methodDescriptor); \
        LOG_FATAL_IF(! (var), "Unable to find method " methodName);

#define GET_FIELD_ID(var, clazz, fieldName, fieldDescriptor) \
        var = env->GetFieldID(clazz, fieldName, fieldDescriptor); \
        LOG_FATAL_IF(! (var), "Unable to find field " fieldName);

int register_customdev_CustomPowerManagerService(JNIEnv* env) {
    int res = jniRegisterNativeMethods(env, "com/customdev/CustomPowerManagerService",
            gPowerManagerServiceMethods, NELEM(gPowerManagerServiceMethods));
    (void) res;  // Faked use when LOG_NDEBUG.
    LOG_FATAL_IF(res < 0, "Unable to register native methods.");

    // Callbacks

    jclass clazz;
    FIND_CLASS(clazz, "com/customdev/CustomPowerManagerService");

    gPowerManagerServiceObj = NULL;
    return 0;
}

} /* namespace customdev */
