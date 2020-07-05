package org.rzaevali.resistorcolor;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;
import java.util.ArrayList;

import static org.rzaevali.resistorcolor.LabelReader.ResistorReader.BAND_IDS;
import static org.rzaevali.resistorcolor.ColorInfo.Colors;

public class ColorChooserActivity extends AppCompatActivity {
    private BAND_IDS _band;
    private ArrayList<Colors> _colorList;

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_color_chooser);

        _band = (BAND_IDS) getIntent().getSerializableExtra("band_id");
        _colorList = (ArrayList<Colors>) getIntent().getSerializableExtra("color_set");

        ListView list = (ListView) findViewById(R.id.listView);
        ColorListAdapter adapter = new ColorListAdapter(this, _colorList);
        list.setAdapter(adapter);

        list.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                Intent intent = new Intent();
                intent.putExtra("color_id", _colorList.get(position));
                intent.putExtra("band_id", _band);
                setResult(Activity.RESULT_OK, intent);
                finish();
            }
        });
    }
}
