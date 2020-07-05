import sys
from cx_Freeze import setup, Executable

# Dependencies are automatically detected, but it might need fine tuning.
build_exe_options = {"packages": ["os"],
                     "excludes": ["tkinter"],
                     "include_files": [("data/window.ui", "data/window.ui"),
                                       ("data/settings_dialog.ui", "data/settings_dialog.ui"),
                                       ("data/color_dialog.ui", "data/color_dialog.ui"),
                                       ("data/about_dialog.ui", "data/about_dialog.ui"),
                                       ("data/settings.json", "data/settings.json"),
                                       ("data/.resources.json", "data/.resources.json")]}

# GUI applications require a different base on Windows (the default is for a
# console application).
base = None
if sys.platform == "win32":
    base = "Win32GUI"

setup(name="Hanoi",
      version="0.1",
      description="Hanoi",
      options={"build_exe": build_exe_options},
      executables=[Executable("main.pyw", base=base, targetName="Hanoi.exe")])
