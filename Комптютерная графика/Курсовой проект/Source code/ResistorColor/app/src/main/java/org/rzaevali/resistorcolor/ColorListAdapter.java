package org.rzaevali.resistorcolor;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;
import java.util.List;

import static org.rzaevali.resistorcolor.ColorInfo.Colors;

/**
 * Created by ali on 04.11.16.
 */
public class ColorListAdapter extends BaseAdapter {
    private LayoutInflater _inflater;
    private List<Colors> _items;

    public ColorListAdapter(Context context, List<Colors> items) {
        _inflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        _items = items;
    }

    @Override
    public int getCount() {
        return _items.size();
    }

    @Override
    public Object getItem(int position) {
        return _items.get(position);
    }

    @Override
    public long getItemId(int position) {
        return position + 1;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        if (convertView == null) {
            convertView = _inflater.inflate(R.layout.color_list_item, null);
        }

        TextView colorName = (TextView) convertView.findViewById(R.id.color);
        TextView circle = (TextView) convertView.findViewById(R.id.circle);

        colorName.setText(ColorInfo.getInstance().getColorNameMap().get(getItem(position)));
        circle.setTextColor(ColorInfo.getInstance().getARGBColorMap().get(getItem(position)));

        return convertView;
    }
}
