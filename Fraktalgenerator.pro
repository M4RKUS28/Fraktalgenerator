QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

INCLUDEPATH += src/ \
               src/video/

win32:RC_ICONS += src/qss_icons/icon.ico


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/dialogcreatevideo.cpp \
    src/imagesetting.cpp \
    src/video/avi-utils.cpp \
    src/video/fileio.cpp \
    src/video/gwavi.cpp \
    src/video/QAviWriter.cpp \
    src/dialogimageserie.cpp \
    src/imageview.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/workerthread.cpp

HEADERS += \
    src/dialogcreatevideo.h \
    src/imagesetting.h \
    src/video/gwavi.h \
    src/video/avi-utils.h \
    src/video/fileio.h \
    src/video/gwavi_private.h \
    src/video/QAviWriter.h \
    src/dialogimageserie.h \
    src/imageview.h \
    src/mainwindow.h \
    src/ui_mainwindow.h \
    src/workerthread.h

FORMS += \
    src/dialogcreatevideo.ui \
    src/dialogimageserie.ui \
    src/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src/styles.qrc
