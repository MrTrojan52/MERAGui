#-------------------------------------------------
#
# Project created by QtCreator 2018-11-11T18:34:10
#
#-------------------------------------------------

QT       += core gui
QT       += mqtt
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MERAGui
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += /usr/local/include
INCLUDEPATH += /usr/local/include/pocketsphinx
INCLUDEPATH += /usr/local/include/sphinxbase
INCLUDEPATH += $$PWD/MaterialWidgets/include

LIBS +=-lpocketsphinx
LIBS += -lsphinxbase
LIBS += -lsphinxad
LIBS += $$PWD/MaterialWidgets/lib/libcomponents.a

CONFIG += c++11

SOURCES += \
        main.cpp \
        dialog.cpp \        
        SphinxReconizer/src/SphinxRecognizer.cpp \
    connectiondialog.cpp \
    SwitchWidget/src/switch.cpp \
    Device/src/ADevice.cpp \
    Device/src/switchdevice.cpp \
    recognizersettingsdialog.cpp \
    adddevicedialog.cpp \
    custommaterialdialog.cpp \
    Factories/src/DeviceFactory.cpp \
    Device/src/infodevice.cpp \
    Device/src/editdevice.cpp

HEADERS += \
        dialog.h \
        SphinxReconizer/include/SphinxRecognizer.h \
    connectiondialog.h \
    connectiondata.h \
    SwitchWidget/include/switch.h \
    SwitchWidget/include/style.h \
    Device/include/IDevice.h \
    Device/include/adevice.h \
    Device/include/switchdevice.h \
    recognizersettingsdialog.h \
    adddevicedialog.h \
    custommaterialdialog.h \
    Device/include/availabledeviceinfo.h \
    Factories/include/IFactory.h \
    Factories/include/devicefactory.h \
    Device/include/infodevice.h \
    Device/include/editdevice.h

FORMS += \
        dialog.ui \
    connectiondialog.ui \
    recognizersettingsdialog.ui \
    adddevicedialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    recource.qrc
