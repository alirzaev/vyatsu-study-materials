from functools import partial
from PyQt5 import QtWidgets, uic


class ColorDialog(QtWidgets.QDialog):
    _style_sheet_template = 'background-color: #{:06x}'

    def __init__(self, colors: list, parent=None):
        super(ColorDialog, self).__init__(parent)
        uic.loadUi('./data/color_dialog.ui', self)
        if len(colors) < 16:
            raise IndexError('Invalid count of colors')

        size = min(16, len(colors))
        for i, c in enumerate(colors[:size], 1):
            button = getattr(self, 'pushButton_{}'.format(i))
            button.setStyleSheet(self._style_sheet_template.format(c))
            button.clicked.connect(partial(self._update_selected_color, c))
        self._selected_color = -1

    def _update_selected_color(self, color: int):
        self.labelColor.setStyleSheet(self._style_sheet_template.format(color))
        self._selected_color = color

    selected_color = property(lambda self: self._selected_color)
