package com.eightman.client;

import android.app.Activity;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;
import com.customdev.service.powermanager.CustomPowerManager;

public class EightManActivity extends Activity implements OnClickListener {
  private static final String TAG = "EightManActivity";
  private boolean setInteractiveEnabled;

  private final CustomPowerManager customPowerManager = CustomPowerManager.getInstance();

  @Override
  public void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    super.setContentView(R.layout.eightman);
    Button button = (Button)super.findViewById(R.id.button);
    button.setOnClickListener(this);
    setInteractive(true);
  }

  @Override
  public void onClick(View view) {
    setInteractive(!setInteractiveEnabled);
  }

  private void setInteractive(boolean enable) {
    customPowerManager.getInstance().setInteractive(enable);
    setInteractiveEnabled = enable;
  }
}
