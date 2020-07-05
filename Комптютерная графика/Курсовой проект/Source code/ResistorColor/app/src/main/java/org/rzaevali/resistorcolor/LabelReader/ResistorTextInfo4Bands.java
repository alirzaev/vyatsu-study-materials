package org.rzaevali.resistorcolor.LabelReader;

import android.support.annotation.NonNull;

import org.rzaevali.resistorcolor.BaseApplication;
import org.rzaevali.resistorcolor.R;

/**
 * Created by ali on 12.11.16.
 */

public class ResistorTextInfo4Bands implements ResistorTextInfo {
    private int _value;
    private double _multiplier;
    private String _accuracy;

    @NonNull
    @Override
    public String getValue(int value, double multiplier) {
        String s_value = "";
        String s_multiplier = "";
        value /= 10;
        if (multiplier <= 10.0 && multiplier >= 1.0) {
            s_value = String.format("%.0f", value * multiplier);
            s_multiplier = BaseApplication.getInstance().getResources().getString(R.string.om_title);
        } else if (multiplier < 1.0) {
            s_value = String.format("%.2f", value * multiplier);
            s_multiplier = BaseApplication.getInstance().getResources().getString(R.string.om_title);
        } else if (multiplier >= 100.0 && multiplier <= 10000.0) {
            s_value = String.format("%.2f", value * multiplier / 1000.0);
            s_multiplier = BaseApplication.getInstance().getResources().getString(R.string.kiloom_title);
        } else if (multiplier > 10000.0) {
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

    @Override
    public Integer getCoefficient() {
        return null;
    }

    @Override
    public String toString() {
        return String.format("%s ±%s", getValue(_value, _multiplier), _accuracy + "%");
    }

    public ResistorTextInfo4Bands(int value, double multiplier, String accuracy) {
        _value = value * 10;
        _multiplier = multiplier;
        _accuracy = accuracy;
    }
}
