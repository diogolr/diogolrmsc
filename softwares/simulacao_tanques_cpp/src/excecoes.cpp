#ifndef EXCECOES_CPP_
#define EXCECOES_CPP_

#include "excecoes.h"

Excecao :: Excecao( const QString &e ) : erro( e )
{
}


Excecao :: ~Excecao()
{
}


QString Excecao :: msg_erro()
{
    return erro;
}

#endif
