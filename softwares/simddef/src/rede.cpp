#ifndef REDE_CPP_
#define REDE_CPP_

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


void Rede :: inicializar()
{
    entrada = NULL;
    n_entradas = n_camadas = 0;
}


void Rede :: ler_entrada( const QString &nome_arq )
{
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
            // TODO disparar excecao
        }

        n_camadas = valores[1].toInt( &ok );

        if ( !ok )
        {
            // TODO disparar excecao
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
                // TODO disparar excecao
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

            Matriz< double > *bias = new Matriz< double >( n_neuronios[ c ], 1 );

            for ( int i = 0 ; i < matriz->linhas() ; i++ )
            {
                linha = stream.readLine();

                valores = linha.split( QRegExp( "\t" ) );

                for ( int j = 0 ; j < matriz->colunas() ; j++ )
                {
                    (*matriz)( i, j ) = valores[j].toDouble( &ok );

                    if ( !ok )
                    {
                        // TODO disparar excecao
                    }
                }
                
                (*bias)( i, 0 ) = valores[ matriz->colunas() ].toDouble( &ok );

                if ( !ok )
                {
                    // TODO disparar excecao
                }
            }

            pesos.append( matriz );
            biases.append( bias );
        }

        arquivo.close();
    }
}


#endif
