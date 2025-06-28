QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

INCLUDEPATH +=  $$PWD/include \
                $$PWD/vdi \
                $$PWD/workstation \
                $$PWD/io \

QT += openglwidgets network

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
	io/clientmsg.cc \
	io/connectionmgr.cc \
	io/transport.cc \
	vdi/vdi.cc \
    workstation/glwidget.cc \
    workstation/physicalws.cc \
    workstation/screenws.cc \
    workstation/workstation.cc \
    workstation/screen.cc \
    main.cc \


HEADERS += \
	include/debug.h \
	include/macros.h \
	io/clientmsg.h \
	io/connectionmgr.h \
	io/transport.h \
	vdi/vdi.h \
    workstation/glwidget.h \
    workstation/physicalws.h \
    workstation/screenws.h \
    workstation/workstation.h \
    workstation/screen.h \
    include/gemdefs.h \
    include/properties.h \
    include/structures.h \

FORMS += \
    screen.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
