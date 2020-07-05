package org.rzaevali.resistorcolor;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.google.common.base.Joiner;
import com.google.common.collect.ImmutableMap;

import org.rzaevali.resistorcolor.DataBase.ResistorDB;
import org.rzaevali.resistorcolor.LabelReader.ResistorReader;
import org.rzaevali.resistorcolor.LabelReader.ResistorTextInfo;

import java.util.ArrayList;

import info.hoang8f.widget.FButton;

import static org.rzaevali.resistorcolor.LabelReader.ResistorReader.BANDS_COUNT;
import static org.rzaevali.resistorcolor.LabelReader.ResistorReader.BAND_IDS;
import static org.rzaevali.resistorcolor.ColorInfo.Colors;

public class ResistorsActivity extends AppCompatActivity {

    private static final int CHOOSE_COLOR_ACTIVITY = 0;
    private ImmutableMap<BAND_IDS, FButton> _buttonsMap;
    private ResistorReader _resistorReader;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_resistors);

        _buttonsMap = ImmutableMap.<BAND_IDS, FButton>builder().
            put(BAND_IDS.FIRST, (FButton) findViewById(R.id.button1stBand)).
            put(BAND_IDS.SECOND, (FButton) findViewById(R.id.button2ndBand)).
            put(BAND_IDS.THIRD, (FButton) findViewById(R.id.button3rdBand)).
            put(BAND_IDS.FOURTH, (FButton) findViewById(R.id.button4thBand)).
            put(BAND_IDS.FIFTH, (FButton) findViewById(R.id.button5thBand)).
            put(BAND_IDS.SIXTH, (FButton) findViewById(R.id.button6thBand)).build();



        if (savedInstanceState == null) {
            _resistorReader = new ResistorReader();
            chooseSixBands(null);
        }
        else { //восстановление состояния
            _resistorReader = (ResistorReader) savedInstanceState.getSerializable("resistor_reader");
            BANDS_COUNT count = _resistorReader.getBandsCount();
            if (count.equals(BANDS_COUNT.FOUR)) {
                chooseFourBands(null);
            }
            else if (count.equals(BANDS_COUNT.FIVE)) {
                chooseFiveBands(null);
            }
            else if (count.equals(BANDS_COUNT.SIX)) {
                chooseSixBands(null);
            }
        }
        updateBandButtons();
        updateTextInfo();
    }

    public void chooseFourBands(View view) {
        FButton band5 = (FButton) findViewById(R.id.button5thBand),
                band6 = (FButton) findViewById(R.id.button6thBand);
        band5.setVisibility(Button.INVISIBLE);
        band6.setVisibility(Button.INVISIBLE);
        _resistorReader.setBandsCount(BANDS_COUNT.FOUR);
        updateBandButtons();
        updateTextInfo();
    }

    public void chooseFiveBands(View view) {
        FButton band5 = (FButton) findViewById(R.id.button5thBand),
                        band6 = (FButton) findViewById(R.id.button6thBand);
        band5.setVisibility(Button.VISIBLE);
        band6.setVisibility(Button.INVISIBLE);
        _resistorReader.setBandsCount(BANDS_COUNT.FIVE);
        updateBandButtons();
        updateTextInfo();
    }

    public void chooseSixBands(View view) {
        FButton band5 = (FButton) findViewById(R.id.button5thBand),
                        band6 = (FButton) findViewById(R.id.button6thBand);
        band5.setVisibility(Button.VISIBLE);
        band6.setVisibility(Button.VISIBLE);
        _resistorReader.setBandsCount(BANDS_COUNT.SIX);
        updateBandButtons();
        updateTextInfo();
    }

    private void openColorChooserActivity(View view, BAND_IDS band) {
        //открыть окно выбора цвета
        Intent intent = new Intent(ResistorsActivity.this, ColorChooserActivity.class);
        intent.putExtra("color_set", _resistorReader.getColorSet(band));
        intent.putExtra("band_id", band);
        startActivityForResult(intent, CHOOSE_COLOR_ACTIVITY);
    }

    private void setNthBandColor(BAND_IDS band, Colors color) {
        //установить цвет текущей полосы
        _resistorReader.setBandColor(band, color);
    }

    private void updateTextInfo() {
        //вывести информацию на экран
        TextView text = (TextView) findViewById(R.id.infoView);
        ResistorTextInfo info = _resistorReader.getTextInfo();
        if (info != null) { //проверка на корректность маркировки
            ArrayList<String> nearest = ResistorDB.getInstance().getNearest(info);
            text.setText(info.toString() + "\n" + Joiner.on("\n").join(nearest));
        }
        else {
            text.setText("");
        }
    }

    private void updateBandButtons() {
        BAND_IDS[] bandList = {
                BAND_IDS.FIRST, BAND_IDS.SECOND, BAND_IDS.THIRD,
                BAND_IDS.FOURTH, BAND_IDS.FIFTH, BAND_IDS.SIXTH };
        for (int i = 0; i < _resistorReader.getBandsCount().count(); ++i) {
            FButton button = _buttonsMap.get(bandList[i]);
            button.setButtonColor(ColorInfo.getInstance().getARGBColorMap().get(_resistorReader.getBandColor(bandList[i])));
        }
    }

    private void setChosenBandColor(Intent data) {
        Colors color = (Colors) data.getSerializableExtra("color_id");
        BAND_IDS band = (BAND_IDS) data.getSerializableExtra("band_id");

        if (color != null && band != null) {
            setNthBandColor(band, color);
            updateBandButtons();
            updateTextInfo();
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == CHOOSE_COLOR_ACTIVITY &&
            resultCode == Activity.RESULT_OK &&
            data != null) {
            setChosenBandColor(data);
        }
    }

    @Override
    public void onSaveInstanceState(Bundle outState) {
        //сохранение состояния
        super.onSaveInstanceState(outState);
        outState.putSerializable("resistor_reader", _resistorReader);
    }

    //обработчики нажатия на каждую полосу
    public void set1stBandColor(View view) {
        openColorChooserActivity(view, BAND_IDS.FIRST);
    }

    public void set2ndBandColor(View view) {
        openColorChooserActivity(view, BAND_IDS.SECOND);
    }

    public void set3rdBandColor(View view) {
        openColorChooserActivity(view, BAND_IDS.THIRD);
    }

    public void set4thBandColor(View view) {
        openColorChooserActivity(view, BAND_IDS.FOURTH);
    }

    public void set5thBandColor(View view) {
        openColorChooserActivity(view, BAND_IDS.FIFTH);
    }

    public void set6thBandColor(View view) {
        openColorChooserActivity(view, BAND_IDS.SIXTH);
    }

    public void openResistorTextActivity(View view) {
        //перейти в режим генерации маркировки
        startActivity(new Intent(ResistorsActivity.this, TextRActivity.class));
    }
}
