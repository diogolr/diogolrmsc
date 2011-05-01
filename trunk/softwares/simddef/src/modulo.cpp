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


QString Modulo :: nome_falha()
{
    return falha;
}


QStringList Modulo :: endereco_arquivos()
{
    return arquivos;
}


void Modulo :: config_arquivos( const QStringList &arqs )
{
    arquivos = arqs;
}


void Modulo :: config_falha( const QString &f )
{
    falha = f;
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
