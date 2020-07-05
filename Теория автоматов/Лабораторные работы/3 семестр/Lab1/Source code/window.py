from PyQt5 import QtWidgets, uic
from settings_tools import SettingsDialog, SettingsManager
import graphics


class Window(QtWidgets.QMainWindow):
    def __init__(self):
        from json import load
        super(Window, self).__init__()
        uic.loadUi("./data/window.ui", self)

        try:
            raw = SettingsManager.load('./data/settings.json')
            settings = raw if SettingsManager.validate(raw) else SettingsManager.defaults()
        except OSError:
            settings = SettingsManager.defaults()

        with open('./data/.resources.json', encoding='cp1251') as res:
            self._res = load(res)

        self._scene = graphics.HanoiScene(0, 0, 650, 390, self, settings)
        self.graphicsView.setScene(self._scene)

        self.buttonStop.clicked.connect(self._stop)
        self.buttonRun.clicked.connect(self._start)
        self.buttonPause.clicked.connect(self._pause)
        self.buttonResume.clicked.connect(self._resume)
        self.buttonShuffle.clicked.connect(self._scene.shuffle)
        self.buttonSort.clicked.connect(self._scene.sort)
        self._scene.timer_stopped.connect(self._end)
        self.speedSlider.valueChanged.connect(self._update_speed)
        self.speedSlider.sliderReleased.connect(lambda: self._scene.set_fps(self.speedSlider.value()))
        self.actionEdit.triggered.connect(self._update_colors_settings)
        self.actionLoad.triggered.connect(self._load_settings)
        self.actionSave.triggered.connect(self._save_settings)
        self.actionProgramme.triggered.connect(self._show_about_programme_dialog)
        self.actionAuthor.triggered.connect(
            lambda: self._show_message(self._res['titles']['author'],
                                       self._res['messages']['about_author'])
        )

        self._reset()

    def _update_speed(self, value=None):
        if value is None:
            value = self.speedSlider.value()
        else:
            self.speedSlider.setValue(value)
        speed = -9e-5 * value + 9.999
        self.labelSpeedView.setText('{:.2f}'.format(speed))

    def _start(self):
        self._scene.settings['speed'] = self.speedSlider.value()
        self._lock_settings()
        self._scene.start()

        self.buttonPause.setDisabled(False)
        self.buttonShuffle.setDisabled(True)
        self.buttonSort.setDisabled(True)
        self.buttonRun.hide()
        self.buttonStop.show()

    def _stop(self):
        self._scene.stop()
        self._reset()

    def _reset(self):
        self._unlock_settings()
        self._update_speed(self._scene.settings['speed'])
        self._scene.reset_order()
        self._scene.fps = self._scene.settings['speed']

        self.buttonPause.setDisabled(True)
        self.buttonShuffle.setDisabled(False)
        self.buttonSort.setDisabled(False)
        self.buttonPause.show()
        self.buttonResume.hide()
        self.buttonRun.show()
        self.buttonStop.hide()

    def _pause(self):
        self._scene.pause()
        self.buttonPause.hide()
        self.buttonResume.show()

    def _resume(self):
        self._scene.resume()
        self.buttonPause.show()
        self.buttonResume.hide()

    def _end(self):
        self._show_message(self._res['titles']['main'], self._res['messages']['automate_stopped'])
        self.buttonPause.setDisabled(True)

    def _lock_settings(self):
        for action in (self.actionEdit, self.actionLoad):
            action.setDisabled(True)

    def _unlock_settings(self):
        for action in (self.actionEdit, self.actionLoad):
            action.setDisabled(False)

    def _update_colors_settings(self):
        dialog = SettingsDialog(self._scene.settings, self._res, self)
        dialog.exec()
        self._scene.update_colors()

    def _load_settings(self):
        result = QtWidgets.QFileDialog.getOpenFileName(self, self._res['titles']['load_settings'], '',
                                                       self._res['file_filter'], self._res['default_file_type'])
        if result[0] == '':
            return

        try:
            settings = SettingsManager.load(result[0])
            self._scene.settings = settings if SettingsManager.validate(settings) else SettingsManager.defaults()
            self._update_speed(settings['speed'])
        except OSError:
            pass

    def _save_settings(self):
        result = QtWidgets.QFileDialog.getSaveFileName(self, self._res['titles']['save_settings'], '',
                                                       self._res['file_filter'], self._res['default_file_type'])

        if result[0] == '':
            return

        if self.actionLoad.isEnabled():
            self._scene.settings['speed'] = self.speedSlider.value()
        SettingsManager.save(self._scene.settings, result[0])

    def _show_message(self, title, message):
        QtWidgets.QMessageBox(QtWidgets.QMessageBox.Information, title,
                              message, QtWidgets.QMessageBox.Ok, self).exec()

    def _show_about_programme_dialog(self):
        dialog = QtWidgets.QDialog(self)
        uic.loadUi("data/about_dialog.ui", dialog)
        dialog.show()
