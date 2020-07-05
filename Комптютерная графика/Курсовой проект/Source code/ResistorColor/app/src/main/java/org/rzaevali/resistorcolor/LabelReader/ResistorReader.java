package org.rzaevali.resistorcolor.LabelReader;

import android.support.annotation.NonNull;
import android.support.annotation.Nullable;

import com.google.common.collect.ImmutableMap;

import static org.rzaevali.resistorcolor.ColorInfo.Colors;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Arrays;

public class ResistorReader implements Serializable {

    public enum BANDS_COUNT implements Serializable {
        FOUR(4), FIVE(5), SIX(6);

        private int _count;

        BANDS_COUNT(int count) {
            _count = count;
        }

        public int count() {
            return _count;
        }
    }

    public enum BAND_IDS implements Serializable {
        FIRST(0), SECOND(1), THIRD(2), FOURTH(3), FIFTH(4), SIXTH(5);

        private int _id;

        BAND_IDS(int id) {
            _id = id;
        }

        public int id() {
            return _id;
        }
    }

    private final ImmutableMap<Integer, int[]> _bandsMap = ImmutableMap.<Integer, int[]>builder().
            put(4, new int[]{0, 1, 3, 4}).
            put(5, new int[]{0, 1, 2, 3, 4}).
            put(6, new int[]{0, 1, 2, 3, 4, 5}).build();

    //таблица соответствий цвет - значащая цифра
    private final ImmutableMap<Colors, Integer> _digitMap = ImmutableMap.<Colors, Integer>builder().
            put(Colors.BLACK,  0).put(Colors.BROWN,  1).put(Colors.RED,   2).
            put(Colors.ORANGE, 3).put(Colors.YELLOW, 4).put(Colors.GREEN, 5).
            put(Colors.BLUE,   6).put(Colors.VIOLET, 7).put(Colors.GREY,  8).
            put(Colors.WHITE,  9).build();

    //таблица соответствий цвет - множитель
    private final ImmutableMap<Colors, Double> _multiplierMap = ImmutableMap.<Colors, Double>builder().
            put(Colors.SILVER, 0.01).put(Colors.GOLD, 0.1).put(Colors.BLACK,  1.0).
            put(Colors.BROWN,  1e1).put(Colors.RED,   1e2).put(Colors.ORANGE, 1e3).
            put(Colors.YELLOW, 1e4).put(Colors.GREEN, 1e5).put(Colors.BLUE,   1e6).
            put(Colors.VIOLET, 1e7).build();

    //таблица соответствий цвет - точность
    private final ImmutableMap<Colors, String> _accuracyMap = ImmutableMap.<Colors, String>builder().
            put(Colors.GREY,  "0.05").put(Colors.VIOLET, "0.1").put(Colors.BLUE, "0.25").
            put(Colors.GREEN, "0.5").put(Colors.BROWN,   "1.0").put(Colors.RED,  "2.0").
            put(Colors.GOLD,  "5.0").put(Colors.SILVER,  "10.0").build();

    //таблица соответствий цвет - температурный коэффициент
    private final ImmutableMap<Colors, Integer> _tcoefficientMap = ImmutableMap.<Colors, Integer>builder().
            put(Colors.WHITE,  1).put(Colors.VIOLET,  5).put(Colors.BLUE, 10).
            put(Colors.YELLOW, 25).put(Colors.ORANGE, 15).put(Colors.RED, 50).
            put(Colors.BROWN,  100).build();

    //допустимые наборы цветов для каждой полосы
    private final Colors[][] _bandsColorSets = {
            { Colors.BROWN, Colors.RED, Colors.ORANGE, Colors.YELLOW, Colors.GREEN, Colors.BLUE, Colors.VIOLET, Colors.GREY, Colors.WHITE },
            { Colors.BLACK, Colors.BROWN, Colors.RED, Colors.ORANGE, Colors.YELLOW, Colors.GREEN, Colors.BLUE, Colors.VIOLET, Colors.GREY, Colors.WHITE },
            { Colors.BLACK, Colors.BROWN, Colors.RED, Colors.ORANGE, Colors.YELLOW, Colors.GREEN, Colors.BLUE, Colors.VIOLET, Colors.GREY, Colors.WHITE },
            { Colors.BLACK, Colors.BROWN, Colors.RED, Colors.ORANGE, Colors.YELLOW, Colors.GREEN, Colors.BLUE, Colors.VIOLET, Colors.GOLD, Colors.SILVER },
            { Colors.BROWN, Colors.RED, Colors.GREEN, Colors.BLUE, Colors.VIOLET, Colors.GREY, Colors.SILVER, Colors.GOLD },
            { Colors.BROWN, Colors.RED, Colors.ORANGE, Colors.YELLOW, Colors.BLUE, Colors.VIOLET, Colors.WHITE }
    };

    private Colors[] _bands;
    private BANDS_COUNT _bandsCount;

    public ResistorReader() {
        //маркировка по умолчанию
        _bands = new Colors[] {
                Colors.NONE, Colors.NONE, Colors.NONE,
                Colors.NONE, Colors.NONE, Colors.NONE };
        _bandsCount = BANDS_COUNT.SIX;
        setBandColor(BAND_IDS.FIRST, Colors.BROWN);
        setBandColor(BAND_IDS.SECOND, Colors.YELLOW);
        setBandColor(BAND_IDS.THIRD, Colors.GREEN);
        setBandColor(BAND_IDS.FOURTH, Colors.RED);
        setBandColor(BAND_IDS.FIFTH, Colors.VIOLET);
        setBandColor(BAND_IDS.SIXTH, Colors.ORANGE);
    }

    public void setBandColor(@NonNull BAND_IDS band, @NonNull Colors color) {
        int[] map = _bandsMap.get(_bandsCount.count());
        if (band.id() < map.length &&
            getColorSet(band).indexOf(color) != -1) {
            _bands[map[band.id()]] = color;
        } else {
            _bands[map[band.id()]] = Colors.NONE;
        }
    }

    @NonNull
    public Colors getBandColor(@NonNull BAND_IDS band) {
        int[] map = _bandsMap.get(_bandsCount.count());
        if (band.id() < map.length) {
            return _bands[map[band.id()]];
        } else {
            return Colors.NONE;
        }
    }

    public void setBandsCount(@NonNull BANDS_COUNT count) {
        _bandsCount = count;
    }

    @NonNull
    public BANDS_COUNT getBandsCount() {
        return _bandsCount;
    }

    @Nullable
    public ResistorTextInfo getTextInfo() {
        //получить характеристики резистора
        //проверка маркировки на корректность
        boolean valid = true;
        int[] map = _bandsMap.get(_bandsCount.count());
        for (int band : map) {
            if (_bands[band].equals(Colors.NONE)) {
                valid = false;
                break;
            }
        }

        if (!valid) {
            return null;
        } else {
            //получение "сырых" данных
            int first = _digitMap.get(_bands[0]);
            int second = _digitMap.get(_bands[1]);
            int value = first * 10 + second;
            if (_bandsCount.count() >= 5) {
                value = value * 10 + _digitMap.get(_bands[2]);
            }

            double multiplier = _multiplierMap.get(_bands[3]);
            String accuracy = _accuracyMap.get(_bands[4]);
            int tcoefficient = 0;
            if (_bandsCount.count() == 6) {
                tcoefficient = _tcoefficientMap.get(_bands[5]);
            }

            if (_bandsCount.equals(BANDS_COUNT.FOUR)) {
                return new ResistorTextInfo4Bands(value, multiplier, accuracy);
            } else if (_bandsCount.equals(BANDS_COUNT.FIVE)) {
                return new ResistorTextInfo5Bands(value, multiplier, accuracy);
            } else if (_bandsCount.equals(BANDS_COUNT.SIX)) {
                return new ResistorTextInfo6Bands(value, multiplier, accuracy, tcoefficient);
            } else {
                return null;
            }
        }
    }

    @NonNull
    public ArrayList<Colors> getColorSet(@NonNull BAND_IDS band) {
        int[] map = _bandsMap.get(_bandsCount.count());
        if (band.id() < map.length) {
            return new ArrayList<>(Arrays.asList(_bandsColorSets[map[band.id()]]));
        } else {
            return new ArrayList<>();
        }
    }
}
