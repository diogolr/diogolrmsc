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
    criar_rede();
}


Rede :: ~Rede()
{
    if ( entrada != NULL )
        delete entrada;
}


Matriz< double > Rede :: para_frente()
{
    Matriz< double > saida( n_amostras, 2 );

    saida.zero();

    

    return saida;
    /*
    Matriz< double > *ent = entrada;
    Matriz< double > *saida = NULL;

    // Para cada camada
    for ( int c = 0 ; c < n_camadas ; c++ )
    {
        saida = new Matriz< double >( (*pesos[c]) * (*ent)  );

        // Para cada amostra, somar o bias
        for ( int n = 0 ; n < n_amostras ; n++ )
        {
            for ( int b = 0 ; b < biases[c]->linhas() ; b++ )
            {
                (*saida)( b, n ) += (*biases[c])( b, 0 );
                (*saida)( b, n ) = funcao_ativacao( c, (*saida)( b, n ) );
            }
        }

        cout << "==================" << endl << *saida << endl << endl;

        ent = saida;
    }

    return (*saida);*/
}


double Rede :: funcao_ativacao( const int &camada, const double &valor )
{
    switch ( f_ativacao[ camada ] )
    {
        case 'l':
            return valor;
            break;

        case 's':
            return logsig( valor ); // Definida em funcoes.h
            break;

        case 't':
            return tansig( valor ); // Definida em funcoes.h
            break;

        default:
            throw ExcecaoFuncao( "Função de ativação inválida" );
    }
}


void Rede :: configurar_pesos()
{
    // TODO corrigir... ao inves de usar o array conexoes, pode-se chamar
    // diretamente a funcao rede.set_weight( i, j, peso )

    unsigned int tam = rede.get_total_connections();

    connection *conexoes = new connection[ sizeof( connection ) * tam ];

    rede.get_connection_array( conexoes );

    for ( int c = 0 ; c < n_camadas - 1 ; c++ )
    {
        for ( int i = 0 ; i < n_neuronios[c + 1] ; i++ )
        {
            for ( int j = 0 ; j < n_neuronios[c] ; j++ )
            {
                conexoes[i].weight = (*pesos[c])( i, j );
                
                // Bias
                if ( j == n_neuronios[c] - 1 )
                {
                    conexoes[i].weight = (*biases[c])( i, 0 );
                }
            }
        }
    }
    
    rede.set_weight_array( conexoes, tam );

    cout << endl;
    cout << endl;
    cout << endl;

    cout << (*pesos[0]);

    cout << endl;
    cout << endl;
    cout << endl;
    rede.get_connection_array( conexoes );
    for ( unsigned int i = 0 ; i < tam ; i++ )
    {
        cout << conexoes[i].weight << "\t";
    }
    cout << endl;
    cout << endl;
    cout << endl;
}


void Rede :: criar_rede()
{
    // O metodo create_standard_array da biblioteca FANN recebe como parametros
    // o numero de camadas e um array contendo o numero de neuronios de cada
    // camada. O numero de camadas inclui a camada de entrada, portanto, faz-se
    // necessario adicionar o numero de entradas no inicio do vetor n_neuronios
    // desta classe e somar 1 ao numero de camadas
    n_neuronios.push_front( n_entradas );

    n_camadas++;

    rede.create_standard_array( n_camadas, 
                                (const unsigned int *)n_neuronios.data() );

    rede.print_connections();

    configurar_pesos();
    
    rede.print_connections();
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
        (*entrada) = entrada->transposta();
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

            //(*matriz) = matriz->transposta();
            //(*bias) = bias->transposta();

            pesos.append( matriz );
            biases.append( bias );
        }

        arquivo.close();
    }
}


#endif
