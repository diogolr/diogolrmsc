#ifndef REDE_CPP_
#define REDE_CPP_

// TODO remover iostream
#include <iostream>
using namespace::std;

#include "rede.h"

Rede :: Rede( const QString &nome_arq_rede,
              const QString &nome_arq_ent )
{
    inicializar();
    ler_entrada( nome_arq_ent );
    ler_rede( nome_arq_rede );
}


Rede :: ~Rede()
{
    if ( entrada != NULL )
        delete entrada;
}


Matriz< double > Rede :: para_frente()
{
    Matriz< double > *ent = entrada;
    Matriz< double > *saida = NULL;

    // Para cada camada
    for ( int c = 0 ; c < n_camadas ; c++ )
    {
        (*saida) = (*ent) * (*pesos[c]);

        // Para cada amostra, somar o bias
        for ( int n = 0 ; n < n_amostras ; n++ )
        {
            for ( int b = 0 ; b < biases[c]->colunas() ; b++ )
            {
                (*saida)( n, b ) += (*biases[c])( 0, b );
            }
        }

        ent = saida;
    }

    cout << (*saida);

    return (*saida);
}


void Rede :: inicializar()
{
    entrada = NULL;
    n_entradas = n_camadas = 0;
}

void Rede :: ler_entrada( const QString &nome_arq )
{
    QFile arquivo( nome_arq );

    if ( arquivo.open( QIODevice::ReadOnly ) )
    {
        // Iniciando a stream para leitura do arquivo
        QTextStream stream( &arquivo );

        int linha_atual = 0;
        int num_colunas = 0;

        QString linha = stream.readLine();

        QStringList valores;
            
        valores = linha.split( QRegExp( "\t" ) );

        num_colunas = valores.size();

        if ( num_colunas > 0 )
            entrada = new Matriz< double >( 1, num_colunas );
            
        linha = stream.readLine();

        linha_atual++;

        bool ok = true;
        double valor = 0.0;

        while( !linha.isNull() )
        {
            valores = linha.split( QRegExp( "\t" ) );

            Matriz< double > lin( 1, num_colunas );

            entrada->adicionar_linha( linha_atual, lin );

            for ( int j = 0 ; j < valores.size() ; j++ )
            {
                valor = valores[j].toDouble( &ok );

                if ( !ok )
                {
                    throw( ExcecaoConversao( "Conversão inválida" ) );
                }
                
                (*entrada)( linha_atual, j ) = valor;
            }

            linha = stream.readLine();
            
            linha_atual++;
        }

        n_amostras = entrada->linhas();

        // Transposta da entrada
        // (*entrada) = entrada->trans();
    }
}


void Rede :: ler_rede( const QString &nome_arq )
{
    QFile arquivo( nome_arq );

    if ( arquivo.open( QIODevice::ReadOnly ) )
    {
        // Iniciando a stream para leitura do arquivo
        QTextStream stream( &arquivo );

        QString linha = stream.readLine();

        QStringList valores;

        valores = linha.split( QRegExp( " \t " ) );

        // Os primeiros valores do arquivo de configuracao da rede, segundo o
        // salvar_rede.m, serao sempre o numero de entradas da rede e o numero
        // de camadas
        bool ok = true;

        n_entradas = valores[0].toInt( &ok );

        if ( !ok )
        {
            throw( ExcecaoConversao( "Conversão inválida" ) );
        }

        n_camadas = valores[1].toInt( &ok );

        if ( !ok )
        {
            throw( ExcecaoConversao( "Conversão inválida" ) );
        }

        // Para cada camada
        for ( int c = 0 ; c < n_camadas ; c++ )
        {
            // As linhas entre as camadas sao vazias e por isso devem ser
            // desprezadas
            stream.readLine();

            linha = stream.readLine();

            valores = linha.split( QRegExp( " \t " ) );

            n_neuronios.append( valores[0].toInt( &ok ) );

            if ( !ok )
            {
                throw( ExcecaoConversao( "Conversão inválida" ) );
            }

            f_ativacao.append( qstring_char( valores[1] )[0] );
            
            // Configurando os pesos da rede. Importante observar que a ultima
            // coluna sera sempre o bias daquela camada
            Matriz< double > *matriz;

            if ( c == 0 )
            {
                matriz = new Matriz<double>( n_neuronios[ c ],
                                             n_entradas );
            }
            else
            {
                matriz = new Matriz<double>( n_neuronios[ c ],
                                             n_neuronios[ c - 1 ] );
            }

            Matriz< double > *bias = new Matriz< double >( n_neuronios[ c ], 
                                                           1 );

            for ( int i = 0 ; i < matriz->linhas() ; i++ )
            {
                linha = stream.readLine();

                valores = linha.split( QRegExp( "\t" ) );

                for ( int j = 0 ; j < matriz->colunas() ; j++ )
                {
                    (*matriz)( i, j ) = valores[j].toDouble( &ok );

                    if ( !ok )
                    {
                        throw( ExcecaoConversao( "Conversão inválida" ) );
                    }
                }
                
                (*bias)( i, 0 ) = valores[ matriz->colunas() ].toDouble( &ok );

                if ( !ok )
                {
                    throw( ExcecaoConversao( "Conversão inválida" ) );
                }
            }

            (*matriz) = matriz->transposta();
            (*bias) = bias->transposta();

            pesos.append( matriz );
            biases.append( bias );
        }

        arquivo.close();
    }
}


#endif
