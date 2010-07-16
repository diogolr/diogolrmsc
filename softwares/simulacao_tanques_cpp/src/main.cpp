#ifndef MAIN_CPP_
#define MAIN_CPP_

#include <QApplication>

#include "principal.h"

int main( int argc, char **argv )
{
    QApplication app( argc, argv );

    JanelaPrincipal principal;

    principal.show();

    return app.exec();
}

#endif
