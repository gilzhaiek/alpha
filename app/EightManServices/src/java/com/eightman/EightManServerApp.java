package com.eightman;

import android.app.Application;
import android.os.ServiceManager;
import android.util.Log;
import com.customdev.service.powermanager.ICustomPowerManager;
import com.customdev.CustomPowerManagerService;

public class EightManServerApp extends Application {
  private static final String TAG = "EightManServerApp";
  private static final String REMOTE_CUSTOM_POWER_MANAGER_SERVICE_NAME = ICustomPowerManager.class.getName();
  private CustomPowerManagerService customPowerManagerService;

  public void onCreate() {
    super.onCreate();

    this.customPowerManagerService = new CustomPowerManagerService(this);
    ServiceManager.addService(REMOTE_CUSTOM_POWER_MANAGER_SERVICE_NAME,
                this.customPowerManagerService.getImpl());
    Log.d(TAG, "Registered " + REMOTE_CUSTOM_POWER_MANAGER_SERVICE_NAME + "");
  }

  public void onTerminate() {
    super.onTerminate();
    Log.d(TAG, "Terminated");
  }
}


