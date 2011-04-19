######################################################################
# Automatically generated by qmake (2.01a) sex 8. abr 08:37:07 2011
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += . src ui
INCLUDEPATH += . \
               flood/MultilayerPerceptron \
               flood/ObjectiveFunctional \
               flood/Perceptron \
               flood/TrainingAlgorithm \
               flood/Utilities \
               qwt

unix {
    LIBS += /usr/lib/qwt-5.2.1/libqwt.so.5 \
            /usr/lib/flood/libflood.so.1
}
win32 {
    QMAKE_LIBDIR += lib
    LIBS += -lqwt5 -lflood
}

OBJECTS_DIR = obj
UI_DIR = ui
MOC_DIR = moc

CONFIG += release console
QT += svg

# Input
FORMS += ui/config_modulo.ui \
         ui/principal.ui

HEADERS += src/config_modulo.h \
           src/excecoes.h \
           src/funcoes.h \
           src/grafico.h \
           src/modulo.h \
           src/principal.h \
           src/rede.h

SOURCES += src/config_modulo.cpp \
           src/excecoes.cpp \
           src/funcoes.cpp \
           src/grafico.cpp \
           src/main.cpp \
           src/modulo.cpp \
           src/principal.cpp \
           src/rede.cpp \

RESOURCES += imgs/imgs.qrc
