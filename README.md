# serialtool_gui
Just a bare-bones GUI-based serial port example for embedded development applications written in C++ with Qt.

![serialtool](/doc/img/main_window.png)

This example reads in data packets over UART from an FTDI USB-to-serial cable at 115200 8-N-1, parses the data, and displays it graphically depending on whether it's "data type 1" or "data type 2".

## Build

Assuming you've already installed the necessary software (see below),...
* Clone the repository into a new directory
```
git clone git@github.com:burke-david/serialtool_gui.git
```

* Open Qt Creator and open the project by selecting the CMakeLists.txt file in the project directory.

* Configure the project by selecting a "kit" to use for building the project. For Windows machines, select the MSVC kit rather than the MinGW. MinGW seems to cause problems.
![configure kits](/doc/img/configure_kits.png)
* In the Edit -> Projects view, right-click on the serialtool folder and "Run CMake"
* Right-click again and select "Build"
* Finally, click the green arrow in the lower-left-hand side of the screen and run the project.




## Troubleshooting

* If running on Windows and you have problems with the build using the default MinGW Kit, I recommend running the Qt Maintenance Tool once more and in the Qt -> Qt 6.7.0 menu, select the `MSVC 2019 64-bit` option and use the MSVC kit to build the project instead of MinGW.
* If you cannot find your COM port on Windows, for instance for an FTDI TTL232R-3v3 cable, make sure to install the appropriate drivers. In the case of the FTDI cable, install the [VCP Driver](http://www.ftdichip.com/drivers/vcp-drivers)




## Software Installation

* Prior to installing Qt Creator, you'll need a build system installed for your particular OS. Note, `qmake` is included with Qt, but this project uses `CMake`, so you'll want to install that as well. Here is a summary:

    <u>**Windows:**</u>

    **Compiler:** MSVC (Microsoft Visual C++) or MinGW. MSVC is recommended for better compatibility with Windows libraries. You can install MSVC by installing Visual Studio.

    NOTE: For this project, you may have problems if you try to use MinGW. Highly recommend MSVC. See Troubleshooting section regarding installing the `MSVC 2019 64-bit` kit.
    
    **Build System:** qmake (included with Qt) or CMake.

    <u>**Linux:**</u>

    **Compiler:** GCC (GNU Compiler Collection). It is usually pre-installed on most Linux distributions.
    
    **Build System:** qmake (included with Qt) or CMake.

    <u>**Mac:**</u>

    **Compiler:** Clang, which is included with Xcode.

    **Build System:** qmake (included with Qt) or CMake.



* Download the [Qt Online Installer](https://www.qt.io/download-qt-installer-oss?hsCtaTracking=99d9dd4f-5681-48d2-b096-470725510d34%7C074ddad0-fdef-4e53-8aa8-5e8a876d6ab4) for open source use.

* This code was built using the Qt 6.7.0 Source Packages.

* Run the installer and login or create an account with Qt
![Qt Installer](/doc/img/sw_installation/1-login_or_create_account.png)
* Agree to the open source terms
![Agree to terms](/doc/img/sw_installation/2-agree_to_terms.png)
* Select Qt 6.7 for Desktop development
![Select Version](/doc/img/sw_installation/3-select_qt_6p7_for_desktop.png)

* After installation is complete, open the Qt Maintenance Tool and install additional components. Specifically, we're looking for the `Qt Serial Port` module.
![Additional components](/doc/img/sw_installation/4-add_or_remove_components.png)
* This should be found under Qt -> Qt 6.7.0 -> Additional Libraries -> Qt Serial Port
![Serial Port](/doc/img/sw_installation/5-select_Qt_Serial_Port.png)