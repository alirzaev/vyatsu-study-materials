package org.rzaevali.resistorcolor.LabelBuilder;

import android.support.annotation.NonNull;

import static org.rzaevali.resistorcolor.ColorInfo.Colors;

import java.util.ArrayList;

/**
 * Created by ali on 06.12.16.
 */

public class ResistorColorInfo4Bands extends ResistorColorInfo {
    private ArrayList<Colors> _bandsColors;

    public ResistorColorInfo4Bands(int value, @NonNull String multiplier, @NonNull String accuracy) {
        _bandsColors = new ArrayList<>();
        int digit1 = value / 10, digit2 = value % 10;
        _bandsColors.add(digitMap.get(digit1));
        _bandsColors.add(digitMap.get(digit2));
        _bandsColors.add(multiplierMap.get(multiplier));
        _bandsColors.add(accuracyMap.get(accuracy));
    }

    @Override
    public int getBandsCount() {
        return 4;
    }

    @Override
    public ArrayList<Colors> getBandsColors() {
        return _bandsColors;
    }
}
