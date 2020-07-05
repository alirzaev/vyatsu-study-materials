package org.rzaevali.resistorcolor;

import com.google.common.collect.ImmutableMap;

import java.io.Serializable;


/**
 * Created by ali on 12.11.16.
 */

public class ColorInfo {
    private static ColorInfo _instance = new ColorInfo();
    private ImmutableMap<Colors, Integer> _ARGBColorMap;
    private ImmutableMap<Colors, String> _colorNameMap;
    
    private ColorInfo() {
        _ARGBColorMap = ImmutableMap.<Colors, Integer>builder().
            put(Colors.BLACK,  BaseApplication.getInstance().getResources().getInteger(R.integer.color_black_id)).
            put(Colors.RED,    BaseApplication.getInstance().getResources().getInteger(R.integer.color_red_id)).
            put(Colors.GREEN,  BaseApplication.getInstance().getResources().getInteger(R.integer.color_green_id)).
            put(Colors.BLUE,   BaseApplication.getInstance().getResources().getInteger(R.integer.color_blue_id)).
            put(Colors.BROWN,  BaseApplication.getInstance().getResources().getInteger(R.integer.color_brown_id)).
            put(Colors.ORANGE, BaseApplication.getInstance().getResources().getInteger(R.integer.color_orange_id)).
            put(Colors.YELLOW, BaseApplication.getInstance().getResources().getInteger(R.integer.color_yellow_id)).
            put(Colors.VIOLET, BaseApplication.getInstance().getResources().getInteger(R.integer.color_violet_id)).
            put(Colors.WHITE,  BaseApplication.getInstance().getResources().getInteger(R.integer.color_white_id)).
            put(Colors.SILVER, BaseApplication.getInstance().getResources().getInteger(R.integer.color_silver_id)).
            put(Colors.GOLD,   BaseApplication.getInstance().getResources().getInteger(R.integer.color_gold_id)).
            put(Colors.GREY,   BaseApplication.getInstance().getResources().getInteger(R.integer.color_grey_id)).build();
        
        _colorNameMap = ImmutableMap.<Colors, String>builder().
            put(Colors.BLACK,  BaseApplication.getInstance().getResources().getString(R.string.color_black_title)).
            put(Colors.BROWN,  BaseApplication.getInstance().getResources().getString(R.string.color_brown_title)).
            put(Colors.RED,    BaseApplication.getInstance().getResources().getString(R.string.color_red_title)).
            put(Colors.ORANGE, BaseApplication.getInstance().getResources().getString(R.string.color_orange_title)).
            put(Colors.YELLOW, BaseApplication.getInstance().getResources().getString(R.string.color_yellow_title)).
            put(Colors.GREEN,  BaseApplication.getInstance().getResources().getString(R.string.color_green_title)).
            put(Colors.BLUE,   BaseApplication.getInstance().getResources().getString(R.string.color_blue_title)).
            put(Colors.VIOLET, BaseApplication.getInstance().getResources().getString(R.string.color_violet_title)).
            put(Colors.GREY,   BaseApplication.getInstance().getResources().getString(R.string.color_grey_title)).
            put(Colors.WHITE,  BaseApplication.getInstance().getResources().getString(R.string.color_white_title)).
            put(Colors.GOLD,   BaseApplication.getInstance().getResources().getString(R.string.color_gold_title)).
            put(Colors.SILVER, BaseApplication.getInstance().getResources().getString(R.string.color_silver_title)).build();
    }

    public enum Colors implements Serializable {
        NONE, BLACK, BROWN, RED, ORANGE, YELLOW, GREEN, BLUE, VIOLET, GREY, WHITE, GOLD, SILVER
    }
    
    public ImmutableMap<Colors, Integer> getARGBColorMap() {
        return _ARGBColorMap;
    }
    
    public ImmutableMap<Colors, String> getColorNameMap() {
        return _colorNameMap;
    }
        
    public static ColorInfo getInstance() {
        return _instance;
    }
}
