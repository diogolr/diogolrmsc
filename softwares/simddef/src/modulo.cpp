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

// A saida desse metodo corresponde aos intervalos de deteccao da falha. Esses
// intervalos sao representados pelos respectivos numeros das amostras em que a
// falha tenha sido detectada.
//
// Cada deteccao devera ser representada por dois valores, 'a' e 'b',
// correspondentes ao inicio e fim de deteccao, respectivamente. Como cada
// modulo podera ter varias saidas, o retorno desta funcao devera ser composto
// por um conjunto (QList) de matrizes de duas colunas (inicio e fim)
//
// O processamento das falhas devera ser feito no metodo processar_saida(), a
// ser implementado na classe derivada. No final do metodo prrocessar_saida,
// todos os intervalos de falhas das saidas do modulo deverao estar configurados
// na variavel intervalos.
//
// Opcionalmente, o usuario podera reimplementar este metodo para inserir mais
// alguma caracteristica que deseje
QList< MatrizI > Modulo :: falhas()
{
    return intervalos;
}


void Modulo :: configurar_arquivos( const QStringList &arqs )
{
    arquivos = arqs;
}


void Modulo :: configurar_falha( const QString &f )
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
