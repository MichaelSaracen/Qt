QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addcinemaform.cpp \
    addgenreform.cpp \
    addmovieform.cpp \
    addplaytimeform.cpp \
    data/appdata.cpp \
    main.cpp \
    widget.cpp

HEADERS += \
    addcinemaform.h \
    addgenreform.h \
    addmovieform.h \
    addplaytimeform.h \
    data/appdata.h \
    widget.h

FORMS += \
    addcinemaform.ui \
    addgenreform.ui \
    addmovieform.ui \
    addplaytimeform.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
