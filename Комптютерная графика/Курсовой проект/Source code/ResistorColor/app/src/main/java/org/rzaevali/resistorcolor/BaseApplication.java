package org.rzaevali.resistorcolor;

import android.app.Application;

/**
 * Created by ali on 12.11.16.
 */

public class BaseApplication extends Application {
    private static BaseApplication _instance;

    public static BaseApplication getInstance() {
        return _instance;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        _instance = this;
    }
}
