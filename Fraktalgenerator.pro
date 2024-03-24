QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

INCLUDEPATH += src/ \
               src/video/ \
               src/openexr/

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

#CONFIG(Q_OS_WEB_ST) {
#    DEFINES += Q_OS_WEB
#    DEFINES += SINGLE_THREADTED_WEB_ASSEMBLY
#}

#CONFIG(Q_OS_WEB_MT) {
#    DEFINES += Q_OS_WEB
#}


# ICON
# CMAKE - Win:
#     set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/qt5app.rc")
#     add_executable(qt5app main.cpp ${APP_ICON_RESOURCE_WINDOWS})
# QMake - Win:


win32:RC_ICONS += src/icons/icon256_2.ico

# MAC qmake

# ICON = src/qss_icons/icon.icns

# MAC C make


# NOTE: Don't include the path in MACOSX_BUNDLE_ICON_FILE -- this is
# the property added to Info.plist
# oda hier "icon"?
## set(MACOSX_BUNDLE_ICON_FILE qt5app.icns)

# And this part tells CMake where to find and install the file itself
#oder hier ohne src/ ?
## set(APP_ICON_MACOSX ${CMAKE_CURRENT_SOURCE_DIR}/src/qss_icons/icon.icns)

## set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES
##         MACOSX_PACKAGE_LOCATION "Resources")
## add_executable(qt5app MACOSX_BUNDLE main.cpp ${APP_ICON_MACOSX})


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/dialogcreatevideo.cpp \
    src/dialogueber.cpp \
    src/imagesetting.cpp \
    src/openexr/miniz.c \
    src/updater.cpp \
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
    src/dialogueber.h \
    src/imagesetting.h \
    src/openexr/miniz.h \
    src/openexr/tinyexr.h \
    src/updater.h \
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
    src/dialogueber.ui \
    src/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src/styles.qrc
