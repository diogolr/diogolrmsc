#ifndef REDE_CPP_
#define REDE_CPP_

#include "rede.h"

Rede :: Rede( const QString &nome_arq_rede,
              const QString &nome_arq_lim,
              const QString &nome_arq_ent )
{
    inicializar();
    ler_entrada( nome_arq_ent );
    ler_rede( nome_arq_rede );
    ler_limites( nome_arq_lim );
    criar_rede();
}


Rede :: ~Rede()
{
    if ( entrada != NULL )
        delete entrada;

    if ( saida != NULL )
        delete saida;

    rede.destroy();
}


Matriz< double > Rede :: executar()
{
    saida = new Matriz< double >( n_amostras, n_saidas );

    saida->zero();

    double *valores = new double[ n_saidas ];

    for ( uint n = 0 ; n < n_amostras ; n++ )
    {
        valores = (*entrada)[n];

        normalizar( valores );

        valores = rede.run( valores );

        desnormalizar( valores );

        for ( uint s = 0 ; s < n_saidas ; s++ )
        {
            (*saida)( n, s ) = valores[s];
        }
    }

    return (*saida);
}


void Rede :: configurar_funcoes_ativacao()
{
    // A camada zero equivale a camada de entrada, a qual nao possui funcao de
    // ativacao. Ao chamar essa funcao, a rede ja devera ter sido configurada e,
    // portanto, o numero de camadas em n_camadas sera sempre uma unidade maior.
    // Por esse motivo, utiliza-se f_ativacao[c - 1]
    for ( int c = 1 ; (uint)c < n_camadas ; c++ )
    {
        switch ( f_ativacao[ c - 1 ] )
        {
            case 'l':
                rede.set_activation_function_layer( FANN::LINEAR, c );
                rede.set_activation_steepness_layer( 1.0, c );
                break;

            case 's':
                rede.set_activation_function_layer( FANN::SIGMOID, c );
                rede.set_activation_steepness_layer( 1.0, c );
                break;

            case 't':
                rede.set_activation_function_layer( FANN::SIGMOID_SYMMETRIC, c );
                rede.set_activation_steepness_layer( 1.0, c );
                break;

            default:
                throw ExcecaoFuncao( "Função de ativação inválida" );
        }
    }
}


void Rede :: configurar_pesos()
{
    n_conexoes = rede.get_total_connections();

    connection *conexoes = new connection[ sizeof( connection ) * n_conexoes ];

    rede.get_connection_array( conexoes );

    uint contador = 0;

    while( contador < n_conexoes )
    {
        for ( uint c = 0 ; c < n_camadas - 1 ; c++ )
        {
            for ( uint i = 0 ; i < n_neuronios[c + 1] ; i++ )
            {
                for ( uint j = 0 ; j < n_neuronios[c] ; j++ )
                {
                    conexoes[contador].weight = (*pesos[c])( i, j );
                    contador++;

                    // Adicionar peso do bias
                    if ( j == n_neuronios[c] - 1 )
                    {
                        conexoes[contador].weight = (*biases[c])( i, 0 );
                        contador++;
                    }
                }
            }
        }
    }

    rede.set_weight_array( conexoes, n_conexoes );
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

    configurar_pesos();
    configurar_funcoes_ativacao();
}


void Rede :: desnormalizar( double *valores )
{
    // A desnormalizacao so acontece para os dados de saida, por isso so se
    // utiliza o numero de saidas
    for ( uint n = 0 ; n < n_saidas ; n++ )
    {
        valores[n] = ( valores[n] + 1.0 ) * ( y_range[n] / 2.0 ) + y_min[n];
    }
}


void Rede :: inicializar()
{
    entrada = saida = NULL;
    n_amostras = n_camadas = n_conexoes = n_entradas = 0;
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

        arquivo.close();
    }
}


void Rede :: ler_limites( const QString &nome_arq )
{
    QFile arquivo( nome_arq );

    if ( arquivo.open( QIODevice::ReadOnly ) )
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

        // Para cada camada
        for ( unsigned int c = 0 ; c < n_camadas ; c++ )
        {
            // As linhas entre as camadas sao vazias e por isso devem ser
            // desprezadas
            stream.readLine();

            linha = stream.readLine();

            valores = linha.split( QRegExp( " \t " ) );

            n_neuronios.append( valores[0].toUInt( &ok ) );

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

            pesos.append( matriz );
            biases.append( bias );
        }

        n_saidas = n_neuronios.last();

        arquivo.close();
    }
}


void Rede :: normalizar( double *valores )
{
    // A normalizacao so acontece para os dados de entrada, por isso so se
    // utiliza o numero de entradas
    for ( uint n = 0 ; n < n_entradas ; n++ )
    {
        valores[n] = ( valores[n] - x_min[n] ) * ( 2.0 / x_range[n] ) - 1.0;
    }
}


#endif
