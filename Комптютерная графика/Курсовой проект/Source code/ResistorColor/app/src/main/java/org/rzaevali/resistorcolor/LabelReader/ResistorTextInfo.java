package org.rzaevali.resistorcolor.LabelReader;

import android.support.annotation.NonNull;
import android.support.annotation.Nullable;

import java.io.Serializable;

/**
 * Created by ali on 10.10.16.
 */

public interface ResistorTextInfo extends Serializable {
    @NonNull
    //получить строковое представление зачения сопротивления
    String getValue(int value, double multiplier);

    @NonNull
    //получить значащие цифры сопротивления
    Integer getDigits();

    @NonNull
    //получить значение множителя
    Double getMultiplier();

    @NonNull
    //получить значение точности
    String getAccuracy();

    @Nullable
    //получить значение температурного коэффициента
    Integer getCoefficient();

    @Override
    //получить текстовое представление характеристик резистора
    String toString();
}
