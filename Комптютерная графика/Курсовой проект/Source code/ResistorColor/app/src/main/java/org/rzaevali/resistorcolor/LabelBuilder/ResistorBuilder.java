package org.rzaevali.resistorcolor.LabelBuilder;

import android.support.annotation.NonNull;
import android.support.annotation.Nullable;

import com.google.common.collect.ImmutableMap;

import static org.rzaevali.resistorcolor.ColorInfo.Colors;

import java.io.Serializable;
import java.util.Arrays;

/**
 * Created by ali on 10.10.16.
 */
public class ResistorBuilder implements Serializable {
    private int _value;
    private String _multiplier;
    private String _accuracy;
    private String _coefficient;

    private static final String[] _multiplierOrder = {
            "0.01",
            "0.1",
            "1",
            "10",
            "100",
            "1k",
            "10k",
            "100k",
            "1M",
            "10M"
    };

    public ResistorBuilder() {
        _value = 100;
        _multiplier = "0.01";
        _accuracy = "1.0%";
        _coefficient = "1 ppm/°C";
    }

    public void setValue(int value) {
        _value = value;
    }
    
    public int getValue() {
        return _value;
    }

    public void setMultiplier(@NonNull String multiplier) {
        _multiplier = multiplier;
    }
    
    @NonNull
    public String getMultiplier() {
        return _multiplier;
    }

    public void setAccuracy(@NonNull String accuracy) {
        _accuracy = accuracy;
    }
    
    @NonNull
    public String getAccuracy() {
        return _accuracy;
    }

    public void setCoefficient(@NonNull String coefficient) {
        _coefficient = coefficient;
    }
    
    @NonNull
    public String getCoefficient() {
        return _coefficient;
    }
    
    @Nullable
    public ResistorColorInfo getColorInfo() {
        //получить цвета полос маркировки
        //обработка "сырых" данных
        int value = _value;
        String multiplier = _multiplier;
        String accuracy = _accuracy;
        String coefficient = _coefficient;

        if (_value / 100 == 0 && !_coefficient.equals("")) {
            int i = Arrays.asList(_multiplierOrder).indexOf(_multiplier);
            if (i != 0) {
                multiplier = _multiplierOrder[i - 1];
                value *= 10;
                return new ResistorColorInfo6Bands(value, multiplier, accuracy, coefficient);
            } else {
                return null;
            }
        } else if (_value / 100 == 0) {
            return new ResistorColorInfo4Bands(value, multiplier, accuracy);
        } else if (_value / 100 != 0 && _coefficient.equals("")) {
            return new ResistorColorInfo5Bands(value, multiplier, accuracy);
        } else {
            return new ResistorColorInfo6Bands(value, multiplier, accuracy, coefficient);
        }
    }
}
