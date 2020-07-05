package org.rzaevali.resistorcolor.DataBase;

import android.support.annotation.NonNull;
import android.support.annotation.Nullable;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import org.rzaevali.resistorcolor.BaseApplication;
import org.rzaevali.resistorcolor.LabelReader.ResistorTextInfo;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;

/**
 * Created by ali on 15.11.16.
 */

public class ResistorDB {
    private static ResistorDB _instance = new ResistorDB();

    private HashMap<String, ArrayList<Integer>> _ranges;

    @Nullable
    private String readAll(@NonNull String path) {
        //считать их файла все символы
        BufferedReader reader = null;
        try {
            reader = new BufferedReader(new InputStreamReader(
                    BaseApplication.getInstance().getAssets().open(path), "UTF-8"));
            StringBuilder input = new StringBuilder();
            String line;
            while ((line = reader.readLine()) != null) {
                input.append(line);
            }

            return input.toString();
        } catch (IOException e) {
            if (reader != null) {
                try {
                    reader.close();
                } catch (IOException e1) {

                }
            }
            return null;
        }
    }

    private ResistorDB() {
        //разбор JSON-файла
        String input = readAll("resistors_db.json");
        if (input == null) {
            _ranges = null;
        } else {
            try {
                //чтение стандартных рядов номиналов
                JSONObject db = new JSONObject(input);
                _ranges = new HashMap<>();
                Iterator<String> keys = db.keys();

                while (keys.hasNext()) {
                    String range_name = keys.next();
                    JSONArray range_obj = db.getJSONArray(range_name);
                    ArrayList<Integer> range_list = new ArrayList<>();

                    for (int i = 0; i < range_obj.length(); ++i) {
                        range_list.add(range_obj.getInt(i));
                    }
                    _ranges.put(range_name, range_list);
                }
            } catch (JSONException e) {
                _ranges = null;
            }
        }
    }

    @NonNull
    public static ResistorDB getInstance() {
        return _instance;
    }

    @NonNull
    public ArrayList<String> getNearest(@NonNull ResistorTextInfo info) {
        //получить ближайшие номиналы
        ArrayList<String> res = new ArrayList<>();
        if (_ranges == null) {
            return res;
        } else {
            for (String range : _ranges.keySet()) {
                //для каждого ряда найти ближайший, собрать их все в ArrayList
                ArrayList<Integer> list = _ranges.get(range);
                int min = 0, diff = 1000;
                int val = info.getDigits();
                double multiplier = info.getMultiplier();

                for (int i = 0; i < list.size(); ++i) {
                    if (Math.abs(val - list.get(i)) < diff) {
                        min = list.get(i);
                        diff = Math.abs(val - list.get(i));
                    }
                }
                res.add(String.format("%s\t%s", range, info.getValue(min, multiplier)));
            }
            return res;
        }
    }
}
