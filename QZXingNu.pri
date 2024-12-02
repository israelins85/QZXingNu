QT += core multimedia concurrent

CONFIG += qt

include($$PWD/zxing-cpp.pri)

INCLUDEPATH  += $$PWD

HEADERS += \
    $$PWD/qzxingnu.h \
    $$PWD/qzxingnufilter.h

SOURCES += \
    $$PWD/qzxingnu.cpp \
    $$PWD/qzxingnufilter.cpp
