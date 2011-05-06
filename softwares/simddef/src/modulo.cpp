#ifndef MODULO_CPP_
#define MODULO_CPP_

#include "modulo.h"

Modulo :: Modulo( const Modulo::TipoModulo &tipo ) : tipo_modulo( tipo )
{
}


Modulo :: ~Modulo()
{
}


bool Modulo :: ativo()
{
    return ativ;
}


Modulo::TipoModulo Modulo :: tipo()
{
    return tipo_modulo;
}


QList< MatrizD > Modulo :: dados()
{
    QList< MatrizD > lista;

    lista << entrada << saida;

    return lista;
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
// por um conjunto de matrizes de duas colunas (inicio e fim) e os nomes de cada
// uma dessas saidas. Por esse motivo, o retorno da funcao deve ser um QHash que
// mapeia uma MatrizI em uma QString
//
// O processamento das falhas devera ser feito no metodo processar_saida(), a
// ser implementado na classe derivada. No final do metodo processar_saida,
// todos os intervalos de falhas, juntamente com os nomes das saidas do modulo,
// deverao estar configurados na variavel deteccoes
//
// Opcionalmente, o usuario podera reimplementar este metodo para inserir mais
// alguma caracteristica que deseje
QHash< QString, MatrizI > Modulo :: deteccoes_falhas()
{
    return deteccoes;
}


void Modulo :: configurar_arquivos( const QStringList &arqs )
{
    arquivos = arqs;
}


void Modulo :: configurar_ativo( const bool &a )
{
    ativ = a;
}


void Modulo :: configurar_falha( const QString &f )
{
    falha = f;
}

#endif
