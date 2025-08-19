QT += core gui multimedia concurrent

CONFIG += qt warn_on

INCLUDEPATH += $$PWD

# zxing-cpp
INCLUDEPATH += $$PWD/zxing-cpp/core/src

HEADERS += \
    $$files($$PWD/zxing-cpp/core/src/*.h, true)

SOURCES += \
    $$files($$PWD/zxing-cpp/core/src/*.cpp, true)


HEADERS += \
    $$PWD/qzxingnu.h \
    $$PWD/qzxingnudecoderesult.h \
    $$PWD/qzxingnufilter.h

SOURCES += \
    $$PWD/qzxingnu.cpp \
    $$PWD/qzxingnufilter.cpp

QMAKE_CXXFLAGS_RELEASE += -DNDEBUG
QMAKE_CFLAGS_RELEASE += -DNDEBUG
