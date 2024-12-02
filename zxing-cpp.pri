INCLUDEPATH += $$PWD/zxing-cpp/core/src

DEFINES += \
    ZXING_VERSION_MAJOR=1 \
    ZXING_VERSION_MINOR=0 \
    ZXING_VERSION_PATCH=5

HEADERS += \
    $$files($$PWD/zxing-cpp/core/src/*.h, true)

SOURCES += \
    $$files($$PWD/zxing-cpp/core/src/*.cpp, true)
