/* * Copyright (C) 2008 The Android Open Source Project
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

package com.customdev;

import android.app.ActivityManager;
import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.os.Trace;
import android.util.Slog;
import com.customdev.service.powermanager.ICustomPowerManager;

public class CustomPowerManagerService {
    static final String TAG = "CustomPMService";
    static final boolean DEBUG = false;

    private final CustomPowerManagerImpl customPowerManagerImpl;

    private final Context mContext;

    private final class CustomPowerManagerImpl extends ICustomPowerManager.Stub {
        @Override
        public void setInteractive(boolean enable) {
            synchronized (this) {
                Trace.traceBegin(Trace.TRACE_TAG_POWER, "setInteractive()");
                try {
                    nativeSetInteractive(enable);
                } finally {
                    Trace.traceEnd(Trace.TRACE_TAG_POWER);
                }
            }
        }
    }

    private native void nativeInit();

    private static native void nativeAcquireSuspendBlocker(String name);
    private static native void nativeReleaseSuspendBlocker(String name);
    private static native void nativeSetInteractive(boolean enable);
    private static native void nativeSetAutoSuspend(boolean enable);
    private static native void nativeSendPowerHint(int hintId, int data);
    private static native void nativeSetFeature(int featureId, int data);

    public CustomPowerManagerImpl getImpl() {
        return customPowerManagerImpl;
    }

    public CustomPowerManagerService(Context context) {
        mContext = context;
        synchronized (this) { // Better to synchronized by a static object
            nativeInit();
            nativeSetInteractive(true);
        }
        customPowerManagerImpl = new CustomPowerManagerImpl();
    }

    static {
        System.loadLibrary("customdev_jni");
    }
}

