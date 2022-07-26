QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dialogone.cpp \
    dialogtwo.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    dialogone.h \
    dialogtwo.h \
    mainwindow.h

FORMS += \
    dialogone.ui \
    dialogtwo.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


INCLUDEPATH += D:\OpenCV451\buildForQt\install\include\
                                D:\OpenCV451\buildForQt\install\include\opencv2

LIBS += -L D:\OpenCV451\buildForQt\install\x64\mingw\lib\libopencv_*.a
