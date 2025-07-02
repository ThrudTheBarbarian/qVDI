QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

INCLUDEPATH +=  $$PWD/../common \
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
	io/v_curhome.cc \
	ops/v_clrwk.cc \
	ops/v_curdown.cc \
	ops/v_curleft.cc \
	ops/v_curright.cc \
	ops/v_curtext.cc \
	ops/v_curup.cc \
	ops/v_enter_cur.cc \
	ops/v_exit_cur.cc \
	ops/v_opnwk.cc \
	ops/vq_chcells.cc \
	vdi/vdi.cc \
	workstation/fontmgr.cc \
    workstation/glwidget.cc \
    workstation/physicalws.cc \
    workstation/screenws.cc \
    workstation/workstation.cc \
    workstation/screen.cc \
    main.cc \


HEADERS += \
	../common/constants.h \
	../common/debug.h \
	../common/gem.h \
	../common/gemTypes.h \
	../common/macros.h \
	../common/properties.h \
	../common/structures.h \
	io/clientmsg.h \
	io/connectionmgr.h \
	io/transport.h \
	vdi/vdi.h \
	workstation/fontmgr.h \
    workstation/glwidget.h \
    workstation/physicalws.h \
    workstation/screenws.h \
    workstation/workstation.h \
    workstation/screen.h \

FORMS += \
    screen.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
