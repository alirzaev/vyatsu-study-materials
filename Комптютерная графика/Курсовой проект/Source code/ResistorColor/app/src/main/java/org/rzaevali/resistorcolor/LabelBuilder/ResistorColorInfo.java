package org.rzaevali.resistorcolor.LabelBuilder;

import com.google.common.collect.ImmutableMap;

import org.rzaevali.resistorcolor.ColorInfo;

import java.util.ArrayList;
import static org.rzaevali.resistorcolor.ColorInfo.Colors;

/**
 * Created by ali on 06.12.16.
 */

public abstract class ResistorColorInfo {

    //таблица соответствия значащая цифра - цвет
    protected static final ImmutableMap<Integer, Colors> digitMap = ImmutableMap.<Integer, Colors>builder().
            put(0, Colors.BLACK).put(1, Colors.BROWN).put(2, Colors.RED).
            put(3, Colors.ORANGE).put(4, Colors.YELLOW).put(5, Colors.GREEN).
            put(6, Colors.BLUE).put(7, Colors.VIOLET).put(8, Colors.GREY).
            put(9, Colors.WHITE).build();

    //таблица соответствия множитлель - цвет
    protected static final ImmutableMap<String, Colors> multiplierMap = ImmutableMap.<String, Colors>builder().
            put("0.01", Colors.SILVER).put("0.1", Colors.GOLD).put("1", Colors.BLACK).
            put("10", Colors.BROWN).put("100", Colors.RED).put("1k", Colors.ORANGE).
            put("10k", Colors.YELLOW).put("100k", Colors.GREEN).put("1M", Colors.BLUE).
            put("10M", Colors.VIOLET).build();

    //таблица соответствия точность - цвет
    protected static final ImmutableMap<String, Colors> accuracyMap = ImmutableMap.<String, Colors>builder().
            put("0.05%", Colors.GREY).put("0.1%", Colors.VIOLET).put("0.25%", Colors.BLUE).
            put("0.5%", Colors.GREEN).put("1.0%", Colors.BROWN).put("2.0%", Colors.RED).
            put("5.0%", Colors.GOLD).put("10.0%", Colors.SILVER).build();

    //таблица соответствия температурный коэффициент - цвет
    protected static final ImmutableMap<String, Colors> coefficientMap = ImmutableMap.<String, Colors>builder().
            put("1 ppm/°C", Colors.WHITE).put("5 ppm/°C", Colors.VIOLET).put("10 ppm/°C", Colors.BLUE).
            put("25 ppm/°C", Colors.YELLOW).put("15 ppm/°C", Colors.ORANGE).put("50 ppm/°C", Colors.RED).
            put("100 ppm/°C", Colors.BROWN).build();

    //получить количество полос маркировки
    abstract int getBandsCount();

    //получить цвета полос маркировки
    public abstract ArrayList<ColorInfo.Colors> getBandsColors();
}
