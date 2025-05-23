QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    source/aboutdialog.cpp \
    source/additionalparametersdialog.cpp \
    source/cfg_loadmapsdialog.cpp \
    source/configconvardialog.cpp \
    source/filedownloader.cpp \
    source/main.cpp \
    source/mainwindow.cpp \
    source/selectmapdialog.cpp \
    source/serverconsoledialog.cpp \
    source/serverwindow.cpp \
    source/settingsdialog.cpp \
    source/steamcmddialog.cpp \

HEADERS += \
    source/aboutdialog.h \
    source/additionalparametersdialog.h \
    source/cfg_loadmapsdialog.h \
    source/configconvardialog.h \
    source/filedownloader.h \
    source/mainwindow.h \
    source/selectmapdialog.h \
    source/serverconsoledialog.h \
    source/serverwindow.h \
    source/settingsdialog.h \
    source/steamcmddialog.h \

FORMS += \
    ui/aboutdialog.ui \
    ui/additionalparametersdialog.ui \
    ui/cfg_loadmapsdialog.ui \
    ui/configconvardialog.ui \
    ui/mainwindow.ui \
    ui/selectmapdialog.ui \
    ui/serverconsoledialog.ui \
    ui/serverwindow.ui \
    ui/settingsdialog.ui \
    ui/steamcmddialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

RC_ICONS = tf2dsm.ico
