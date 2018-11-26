package com.customdev.service.powermanager;

import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.util.Log;
import android.util.Slog; /* This is to avoid generating events for ourselves */
import java.util.HashMap;
//import com.customdev.service.powermanager.ICustomePowerManager;

public class CustomPowerManager {
  private static final String TAG = "CustomPowerManager";
  private static final String REMOTE_SERVICE_NAME = ICustomPowerManager.class.getName();
  private static final boolean DEBUG = false; // change to true to enable debugging

  //Singleton instance
  private static CustomPowerManager sInstance;

  private final ICustomPowerManager mService;

  public static synchronized CustomPowerManager getInstance() {
    if (sInstance == null) {
      sInstance = new CustomPowerManager();
    }
    return sInstance;
  }

  private CustomPowerManager() {
    Log.d(TAG, "Connecting to ICustomPowerManager by name [" + REMOTE_SERVICE_NAME + "]");
    mService = ICustomPowerManager.Stub.asInterface(
      ServiceManager.getService(REMOTE_SERVICE_NAME));
    if (mService == null) {
      throw new IllegalStateException("Failed to find ICustomPowerManager by name [" + REMOTE_SERVICE_NAME + "]");
    }
  }

  public void setInteractive(boolean enable) {
    try {
      if (DEBUG) Slog.d(TAG, "setInteractive");
      mService.setInteractive(enable);
    } catch (RemoteException e) {
      throw new RuntimeException("Failed to setInteractive", e);
    }
  }
}
