import sys
import window
from PyQt5 import QtWidgets


def main():
    app = QtWidgets.QApplication(sys.argv)
    try:
        win = window.Window()
        win.show()
        app.exec()
    except OSError:
        QtWidgets.QMessageBox(QtWidgets.QMessageBox.Critical, 'Ошибка', 'Поврежден файл ресурсов!', None)
        exit(1)


if __name__ == "__main__":
    main()
