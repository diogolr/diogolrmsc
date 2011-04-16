######################################################################
# Automatically generated by qmake (2.01a) sex 8. abr 08:37:07 2011
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += . src ui
INCLUDEPATH += . qwt fann

unix {
    LIBS += /usr/lib/qwt-5.2.1/libqwt.so.5
}
win32 {
    QMAKE_LIBDIR += lib
    LIBS += -lqwt5 -lfann
}

OBJECTS_DIR = obj
UI_DIR = ui
MOC_DIR = moc

CONFIG += release console
QT += svg

# Input
FORMS += ui/principal.ui

HEADERS += src/excecoes.h \
           src/funcoes.h \
           src/grafico.h \
           src/matriz.h \
           src/principal.h \
           src/rede.h

SOURCES += src/excecoes.cpp \
           src/funcoes.cpp \
           src/grafico.cpp \
           src/main.cpp \
           src/matriz.cpp \
           src/principal.cpp \
           src/rede.cpp \

RESOURCES += imgs/imgs.qrc
