package org.rzaevali.resistorcolor.LabelBuilder;

import android.support.annotation.NonNull;

import static org.rzaevali.resistorcolor.ColorInfo.Colors;

import java.util.ArrayList;

/**
 * Created by ali on 06.12.16.
 */

public class ResistorColorInfo6Bands extends ResistorColorInfo {
    private ArrayList<Colors> _bandsColors;

    public ResistorColorInfo6Bands(int value, @NonNull String multiplier, @NonNull String accuracy, @NonNull String coefficient) {
        _bandsColors = new ArrayList<>();
        int digit1 = value / 100, digit2 = value / 10 % 10, digit3 = value % 10;
        _bandsColors.add(digitMap.get(digit1));
        _bandsColors.add(digitMap.get(digit2));
        _bandsColors.add(digitMap.get(digit3));
        _bandsColors.add(multiplierMap.get(multiplier));
        _bandsColors.add(accuracyMap.get(accuracy));
        _bandsColors.add(coefficientMap.get(coefficient));
    }
    @Override
    public int getBandsCount() {
        return 6;
    }

    @Override
    public ArrayList<Colors> getBandsColors() {
        return _bandsColors;
    }
}
