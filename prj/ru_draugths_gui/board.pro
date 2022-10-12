QT       += core gui widgets

TARGET = board
TEMPLATE = app


SOURCES += main.cpp\
    ../../src/rules/board.cpp \
    ../../src/rules/rules.cpp \
    ../../src/utils/bit_op.cpp \
        mainwindow.cpp \
    boardgui.cpp

HEADERS  += mainwindow.h \
    ../../src/ai/ai.h \
    ../../src/rules/board.h \
    ../../src/rules/rules.h \
    ../../src/utils/utils.h \
    ../../src/utils/bit_op.h \
    boardgui.h \
    move_maker.h \
    move_path.h

INCLUDEPATH += ../../src/rules/ \
    ../../src/ai/ \
    ../../src/utils/

FORMS    += mainwindow.ui

RESOURCES += \
    images.qrc
