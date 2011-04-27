#ifndef MODULO_CPP_
#define MODULO_CPP_

#include "modulo.h"

Modulo :: Modulo( const Modulo::TipoModulo &tipo ) : tipo_modulo( tipo )
{
}


Modulo :: ~Modulo()
{
}


Modulo::TipoModulo Modulo :: tipo()
{
    return tipo_modulo;
}


void Modulo :: nome_entrada( const int &, const QString & )
{
}


void Modulo :: nome_saida( const int &, const QString & )
{
}


void Modulo :: nomes_entradas( const QStringList & )
{
}


void Modulo :: nomes_saidas( const QStringList & )
{
}


#endif
