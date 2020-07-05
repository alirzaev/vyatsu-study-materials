package org.rzaevali.resistorcolor.LabelReader;

import android.support.annotation.NonNull;
import android.support.annotation.Nullable;

import com.google.common.base.Strings;

import org.rzaevali.resistorcolor.BaseApplication;
import org.rzaevali.resistorcolor.R;

/**
 * Created by ali on 12.11.16.
 */

public class ResistorTextInfo6Bands implements ResistorTextInfo {
    private int _value;
    private double _multiplier;
    private String _accuracy;
    private int _tcoefficient;

    @NonNull
    @Override
    public String getValue(int value, double multiplier) {
        String s_value = "";
        String s_multiplier = "";
        if (multiplier <= 1.0) {
            s_value = String.format("%.2f", value * multiplier);
            s_multiplier = BaseApplication.getInstance().getResources().getString(R.string.om_title);
        } else if (multiplier >= 10.0 && multiplier <= 1000.0) {
            s_value = String.format("%.2f", value * multiplier / 1000.0);
            s_multiplier = BaseApplication.getInstance().getResources().getString(R.string.kiloom_title);
        } else if (multiplier > 1000.0) {
            s_value = String.format("%.2f", value * multiplier / 1e6);
            s_multiplier = BaseApplication.getInstance().getResources().getString(R.string.megaom_title);
        }

        return String.format("%s %s", s_value, s_multiplier);
    }
    
    @NonNull
    @Override
    public Integer getDigits() {
        return _value;
    }

    @NonNull
    @Override
    public Double getMultiplier() {
        return _multiplier;
    }

    @NonNull
    @Override
    public String getAccuracy() {
        return _accuracy;
    }

    @Nullable
    @Override
    public Integer getCoefficient() {
        return _tcoefficient;
    }

    @Override
    public String toString() {
        return String.format("%s ±%s  %s ppm/°C", getValue(_value, _multiplier), _accuracy + "%", _tcoefficient);
    }

    public ResistorTextInfo6Bands(int value, double multiplier, String accuracy, int tcoefficient) {
        _value = value;
        _multiplier = multiplier;
        _accuracy = accuracy;
        _tcoefficient = tcoefficient;
    }
}
