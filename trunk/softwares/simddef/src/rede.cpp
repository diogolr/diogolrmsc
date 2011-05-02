#ifndef REDE_CPP_
#define REDE_CPP_

#include "rede.h"

#include <iostream>
using namespace::std;

Rede :: Rede() : Modulo( Modulo::RNA )
{
    inicializar();
}


Rede :: ~Rede()
{
}


MatrizD Rede :: executar()
{
    normalizar( &entrada );

    MatrizD saida = rede.calculate_output_matrix( entrada );

    desnormalizar( &saida );

    return saida;
}


QString Rede :: nome_tipo()
{
    return QString( "RNA" );
}


void Rede :: ler_arquivos()
{
    try
    {
        ler_entrada( arquivos[0] );
        ler_rede( arquivos[1] );
        ler_limites( arquivos[2] );
    }
    catch( ExcecaoArquivo e )
    {
        throw e;
    }
    catch( ExcecaoConversao e )
    {
        throw e;
    }
    catch( ExcecaoFuncao e )
    {
        throw e;
    }
    
    criar_rede();
}


void Rede :: configurar_funcoes_ativacao()
{
    for ( uint c = 0 ; (uint)c < n_camadas ; c++ )
    {
        switch ( f_ativacao[ c ] )
        {
            case 'l':
                if ( c == n_camadas - 1 )
                {
                    rede.set_output_layer_activation_function( LINEAR );
                }
                else
                {
                    rede.set_hidden_layer_activation_function( c, LINEAR );
                }
                break;

            case 's':
                if ( c == n_camadas - 1 )
                {
                    rede.set_output_layer_activation_function( LOGSIG );
                }
                else
                {
                    rede.set_hidden_layer_activation_function( c, LOGSIG );
                }
                break;

            case 't':
                if ( c == n_camadas - 1 )
                {
                    rede.set_output_layer_activation_function( TANH );
                }
                else
                {
                    rede.set_hidden_layer_activation_function( c, TANH ); 
                }
                break;

            default:
                throw ExcecaoFuncao( "Função de ativação inválida" );
        }
    }
}


void Rede :: configurar_pesos()
{
    for ( uint c = 0 ; c < n_camadas - 1 ; c++ )
    {
        rede.set_hidden_layer_synaptic_weights( c, pesos[c] );
        rede.set_hidden_layer_biases( c, biases[c] );
    }

    rede.set_output_layer_synaptic_weights( pesos[n_camadas - 1] );
    rede.set_output_layer_biases( biases[n_camadas - 1] );
}


void Rede :: criar_rede()
{
    // Como o atributo n_neuronios inclui a camada de saida e a biblioteca Flood
    // contabiliza as camadas escondidas separadas da camada de saida, faz-se
    // necessario criar um novo vetor temporario somente para informar o numero
    // de neuronios de cada camada oculta para a funcao set_network_architecture
    Vector< int > n_neur_cam_ocultas( n_neuronios.get_size() - 1 );

    for ( int i = 0 ; i < n_neur_cam_ocultas.get_size() ; i++ )
    {
        n_neur_cam_ocultas[i] = n_neuronios[i];
    }

    rede.set_network_architecture( n_entradas, n_neur_cam_ocultas, n_saidas );

    configurar_pesos();
    configurar_funcoes_ativacao();
}


void Rede :: desnormalizar( MatrizD *valores )
{
    for ( uint a = 0 ; a < n_amostras ; a++ )
    {
        // A desnormalizacao so acontece para os dados de saida, por isso so se
        // utiliza o numero de saidas
        for ( uint n = 0 ; n < n_saidas ; n++ )
        {
            (*valores)[a][n] = ( (*valores)[a][n] + 1.0 ) * 
                               ( y_range[n] / 2.0 ) + y_min[n];
        }
    }
}


void Rede :: inicializar()
{
    n_amostras = n_camadas = n_entradas = n_saidas = 0;
}

void Rede :: ler_entrada( const QString &nome_arq )
{
    QFile arquivo( nome_arq );

    if ( !arquivo.open( QIODevice::ReadOnly ) )
    {
        throw ExcecaoArquivo( "O arquivo <b>" + nome_arq + "</b> não pôde ser "
                              "aberto." );
    }
    else
    {
        // Iniciando a stream para leitura do arquivo
        QTextStream stream( &arquivo );

        int num_colunas = 0;

        QString linha = stream.readLine();

        QStringList valores;
            
        valores = linha.split( QRegExp( "\t" ) );

        num_colunas = valores.size();

        // Adicionando uma linha so para que se possa utilizar o metodo add_row
        // corretamente, pois o metodo add_row precisa saber o numero de colunas
        // da matriz. No final do laco while esta linha sera removida
        entrada.set( 1, num_colunas, 0.0 );

        bool ok = true;

        double valor = 0.0;

        while( !linha.isNull() )
        {
            valores = linha.split( QRegExp( "\t" ) );

            VetorD lin( num_colunas );

            for ( int j = 0 ; j < valores.size() ; j++ )
            {
                valor = valores[j].toDouble( &ok );

                if ( !ok )
                {
                    throw( ExcecaoConversao( "Conversão inválida" ) );
                }
                
                lin[j] = valor;
            }

            entrada.add_row( lin );

            linha = stream.readLine();
        }

        entrada.subtract_row( 0 );

        n_amostras = entrada.get_rows_number();

        arquivo.close();
    }
}


void Rede :: ler_limites( const QString &nome_arq )
{
    QFile arquivo( nome_arq );

    if ( !arquivo.open( QIODevice::ReadOnly ) )
    {
        throw ExcecaoArquivo( "O arquivo não pôde ser aberto." );
    }
    else
    {
        // Iniciando a stream para leitura do arquivo
        QTextStream stream( &arquivo );

        QString linha;

        QStringList valores;

        double valor = 0.0;
        
        bool ok = true;

        if ( !ok )
        {
            throw( ExcecaoConversao( "Conversão inválida" ) );
        }
       
        // Configurando x_min x_max e x_range
        for ( uint i = 0 ; i < n_entradas ; i++ )
        {
            linha = stream.readLine();

            valores = linha.split( QRegExp( "\t" ) );

            // x_min
            valor = valores[0].toDouble( &ok );
        
            if ( !ok )
            {
                throw( ExcecaoConversao( "Conversão inválida" ) );
            }

            x_min.append( valor );

            // x_max
            valor = valores[1].toDouble( &ok );
        
            if ( !ok )
            {
                throw( ExcecaoConversao( "Conversão inválida" ) );
            }

            x_max.append( valor );

            // x_range
            valor = valores[2].toDouble( &ok );
            
            if ( !ok )
            {
                throw( ExcecaoConversao( "Conversão inválida" ) );
            }

            x_range.append( valor );
        }
        
        // Configurando y_min y_max e y_range
        for ( uint i = 0 ; i < n_saidas ; i++ )
        {
            linha = stream.readLine();

            valores = linha.split( QRegExp( "\t" ) );

            // x_min
            valor = valores[0].toDouble( &ok );
        
            if ( !ok )
            {
                throw( ExcecaoConversao( "Conversão inválida" ) );
            }

            y_min.append( valor );

            // x_max
            valor = valores[1].toDouble( &ok );
        
            if ( !ok )
            {
                throw( ExcecaoConversao( "Conversão inválida" ) );
            }

            y_max.append( valor );

            // x_range
            valor = valores[2].toDouble( &ok );
            
            if ( !ok )
            {
                throw( ExcecaoConversao( "Conversão inválida" ) );
            }

            y_range.append( valor );
        }

        arquivo.close();
    }
}


void Rede :: ler_rede( const QString &nome_arq )
{
    QFile arquivo( nome_arq );

    if ( !arquivo.open( QIODevice::ReadOnly ) )
    {
        throw ExcecaoArquivo( "O arquivo não pôde ser aberto." );
    }
    else
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

        n_entradas = valores[0].toUInt( &ok );

        if ( !ok )
        {
            throw( ExcecaoConversao( "Conversão inválida" ) );
        }

        n_camadas = valores[1].toUInt( &ok );

        if ( !ok )
        {
            throw( ExcecaoConversao( "Conversão inválida" ) );
        }

        n_neuronios.set_size( n_camadas );

        pesos.set_size( n_camadas );
        biases.set_size( n_camadas );

        // Para cada camada
        for ( uint c = 0 ; c < n_camadas ; c++ )
        {
            // As linhas entre as camadas sao vazias e por isso devem ser
            // desprezadas
            stream.readLine();

            linha = stream.readLine();

            valores = linha.split( QRegExp( " \t " ) );

            n_neuronios[c] = valores[0].toUInt( &ok );

            if ( !ok )
            {
                throw( ExcecaoConversao( "Conversão inválida" ) );
            }

            f_ativacao.append( qstring_char( valores[1] )[0] );
            
            // Configurando os pesos da rede. Importante observar que a ultima
            // coluna sera sempre o bias daquela camada
            MatrizD matriz;

            if ( c == 0 )
            {
                matriz.set( n_neuronios[c], n_entradas );
            }
            else
            {
                matriz.set( n_neuronios[c], n_neuronios[ c - 1 ] );
            }

            VetorD bias( n_neuronios[ c ] );

            for ( int i = 0 ; i < matriz.get_rows_number() ; i++ )
            {
                linha = stream.readLine();

                valores = linha.split( QRegExp( "\t" ) );

                for ( int j = 0 ; j < matriz.get_columns_number() ; j++ )
                {
                    matriz[i][j] = valores[j].toDouble( &ok );

                    if ( !ok )
                    {
                        throw( ExcecaoConversao( "Conversão inválida" ) );
                    }
                }
                
                bias[i] = valores[ matriz.get_columns_number() ].toDouble( &ok );

                if ( !ok )
                {
                    throw( ExcecaoConversao( "Conversão inválida" ) );
                }
            }

            pesos[c] = matriz;
            biases[c] = bias;
        }

        n_saidas = n_neuronios[ n_camadas - 1 ];

        arquivo.close();
    }
}


void Rede :: normalizar( MatrizD *valores )
{
    for ( uint a = 0 ; a < n_amostras ; a++ )
    {
        // A normalizacao so acontece para os dados de entrada, por isso so se
        // utiliza o numero de entradas
        for ( uint n = 0 ; n < n_entradas ; n++ )
        {
            (*valores)[a][n] = ( (*valores)[a][n] - x_min[n] ) * 
                               ( 2.0 / x_range[n] ) - 1.0;
        }
    }
}

#endif
