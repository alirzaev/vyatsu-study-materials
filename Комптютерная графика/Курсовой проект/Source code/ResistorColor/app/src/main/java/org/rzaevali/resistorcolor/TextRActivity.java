package org.rzaevali.resistorcolor;

import android.support.annotation.NonNull;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.Spinner;

import org.rzaevali.resistorcolor.LabelBuilder.ResistorBuilder;
import org.rzaevali.resistorcolor.LabelBuilder.ResistorColorInfo;

public class TextRActivity extends AppCompatActivity {
    private Button[] _buttons;
    private ResistorBuilder _resistorBuilder;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_text_r);

        //установка обработчиков для списков выбора характеристик резистора
        //для множителя
        final Spinner multiplier_spinner = (Spinner) findViewById(R.id.multiplier_spinner);
        if (multiplier_spinner != null) {
            multiplier_spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
                @Override
                public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                    String m = (String) multiplier_spinner.getItemAtPosition(i);
                    setMultiplier(m);
                }

                @Override
                public void onNothingSelected(AdapterView<?> adapterView) {
                }
            });
        }
        //для точности
        final Spinner accuracy_spinner = (Spinner) findViewById(R.id.accuracy_spinner);
        if (accuracy_spinner != null) {
            accuracy_spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
                @Override
                public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                    String a = (String) accuracy_spinner.getItemAtPosition(i);
                    setAccuracy(a);
                }

                @Override
                public void onNothingSelected(AdapterView<?> adapterView) {
                }
            });
        }
        //для температурного коэффициента
        final Spinner coefficient_spinner = (Spinner) findViewById(R.id.coefficient_spinner);
        if (coefficient_spinner != null) {
            coefficient_spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
                @Override
                public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                    String c = (String) coefficient_spinner.getItemAtPosition(i);
                    setCoefficient(c);
                }

                @Override
                public void onNothingSelected(AdapterView<?> adapterView) {
                }
            });
        }

        _resistorBuilder = new ResistorBuilder();
    }

    private void setMultiplier(@NonNull String multiplier) {
        _resistorBuilder.setMultiplier(multiplier);
    }

    private void setAccuracy(@NonNull String accuracy) {
        _resistorBuilder.setAccuracy(accuracy);
    }

    private void setCoefficient(@NonNull String coefficient) {
        _resistorBuilder.setCoefficient(coefficient);
    }

    public void getColorInfo(View view) {
        EditText value = (EditText) findViewById(R.id.resistor_value);
        try {
            //получение значения сопротивления
            _resistorBuilder.setValue(Integer.parseInt(value.getText().toString()));

            //построение маркировки
            ResistorColorInfo info = _resistorBuilder.getColorInfo();
            if (info != null) { //проверка на корректность характеристик
                ListView color_view = (ListView) findViewById(R.id.colors_list_view);
                ColorListAdapter adapter = new ColorListAdapter(this, info.getBandsColors());
                color_view.setAdapter(adapter);
            } else { //сообщить о некорректных характеристиках резистора
                new AlertDialog.Builder(view.getContext()).
                        setMessage(getResources().getString(R.string.error_msg_no_such_color_mark)).
                        setIcon(android.R.drawable.ic_dialog_alert).
                        show();
            }
        } catch (NumberFormatException ex) {
            new AlertDialog.Builder(view.getContext()).
                    setMessage(getResources().getString(R.string.error_msg_no_such_color_mark)).
                    setIcon(android.R.drawable.ic_dialog_alert).
                    show();
        }
    }
}
