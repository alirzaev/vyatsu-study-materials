from functools import partial
from PyQt5 import QtWidgets, uic
from color_dialog import ColorDialog


class SettingsDialog(QtWidgets.QDialog):
    _style_sheet_template = 'background-color: #{:06x}'

    def __init__(self, settings: dict, resources: dict, parent=None):
        super(SettingsDialog, self).__init__(parent)
        uic.loadUi('./data/settings_dialog.ui', self)

        self.settings = settings
        self._res = resources
        self._used_colors = list(settings['rings'])
        self._updates = [None] * 12

        self.buttonShuffle.clicked.connect(self._shuffle_colors)
        for i, c in enumerate(settings['rings'], 1):
            getattr(self, 'labelColor_{}'.format(i)).setStyleSheet(self._style_sheet_template.format(c))
            getattr(self, 'buttonColor_{}'.format(i))\
                .clicked.connect(partial(self._update_ring_color, i))

    @staticmethod
    def _apply_update(seq: list, index: int, color: int):
        seq[index] = color

    def _update_ring_color(self, ring: int):
        dialog = ColorDialog(self.settings['colors'], self)
        if dialog.exec() == QtWidgets.QDialog.Rejected:
            return
        color = dialog.selected_color
        if color not in self._used_colors:
            getattr(self, 'labelColor_{}'.format(ring)).setStyleSheet(self._style_sheet_template.format(color))
            self._used_colors[ring - 1] = color
            self._updates[ring - 1] = partial(self._apply_update, self.settings['rings'], ring - 1, color)
        else:
            QtWidgets.QMessageBox(QtWidgets.QMessageBox.Critical, self._res['titles']['error'],
                                  self._res['messages']['already_used_color'], QtWidgets.QMessageBox.Ok, self).exec()

    def _shuffle_colors(self):
        from random import shuffle
        colors = self.settings['colors'][:]
        shuffle(colors)
        self._used_colors = colors[:12]
        for ring, color in enumerate(self._used_colors, 1):
            getattr(self, 'labelColor_{}'.format(ring)).setStyleSheet(self._style_sheet_template.format(color))
        self._updates = [partial(self._apply_update, self.settings['rings'], ring, color)
                         for ring, color in enumerate(colors[:12])]

    def accept(self):
        super(SettingsDialog, self).accept()
        for update in (func for func in self._updates if func is not None):
            update()


class SettingsManager:
    @staticmethod
    def load(path: str) -> dict:
        import json
        with open(path, 'r') as file:
            settings = json.load(file)
        return settings

    @staticmethod
    def defaults() -> dict:
        import pickle
        defaults = b'\x80\x03}q\x00(X\x05\x00\x00\x00ringsq\x01]q\x02(J\xff3f\x00J3\xfff\x00J==\xff\x00J\x00\x8a\xb8' \
                   b'\x00J\xfff3\x00J\xbc\x0e5\x00J\xff\xcc3\x00J\xcc\xff3\x00J\xcc3\xff\x00MR\xa3Jf3\xff\x00J3f\xff' \
                   b'\x00eX\x06\x00\x00\x00colorsq\x03]q\x04(J\xff3f\x00J3\xfff\x00J==\xff\x00J\x00\x8a\xb8\x00J\xfff3' \
                   b'\x00J\xbc\x0e5\x00J\xff\xcc3\x00J\xcc\xff3\x00J\xcc3\xff\x00MR\xa3Jf3\xff\x00J3f\xff\x00J\x1f\xd5' \
                   b'\xd3\x00J\x9c\x9cO\x00J3\x00f\x00J\x00\xb8\xf5\x00eu.'

        return pickle.loads(defaults)

    @staticmethod
    def validate(settings: dict) -> bool:
        if not ('rings' in settings and
                'colors' in settings and
                'speed' in settings):
            return False
        if not all(isinstance(c, int) and 0 <= c <= 0xFFFFFF for c in settings['colors']):
            return False
        if not all(isinstance(c, int) and 0 <= c <= 0xFFFFFF for c in settings['rings']):
            return False
        if not isinstance(settings['speed'], int):
            return False
        return True

    @staticmethod
    def save(settings: dict, path: str):
        import json
        try:
            with open(path, 'w') as file:
                json.dump(settings, file)
            return True
        except OSError:
            return False
