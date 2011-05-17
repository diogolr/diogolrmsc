#ifndef REDE_CPP_
#define REDE_CPP_

#include "rede.h"

Rede :: Rede() : Modulo( Modulo::RNA )
{
    configurar_ordem( -1 );
}


Rede :: ~Rede()
{
}


int Rede :: ordem()
{
    return ord;
}


QHash< int, QString > Rede :: curvas_a_exibir()
{
    // Observe que as entradas das redes dependem do numero de regressores.
    // Logo, as curvas a serem exibidas deverao pular os regressores. Ou seja, o
    // incremento da variavel do loop devera ser igual a ordem da rede (no caso
    // em que existe a mesma quantidade de regressores para cada variavel de
    // entrada)
    QHash< int, QString > curvas;

    QStringList nomes;

    nomes << "Nível T<sub>1</sub>"
          << "Nível T<sub>2</sub>"
          << "Sinal Cont. T<sub>1</sub>"
          << "Sinal Cont. T<sub>2</sub>"
          << "Erro Est. T<sub>1</sub>"
          << "Erro Est. T<sub>2</sub>";

    int contador = 0;

    // Como os regressores sao aplicados somente aos niveis e aos sinais de
    // controle, as duas ultimas colunas sempre serao os residuos produzidos
    // pelo erro de estimativa
    int num_entradas_sem_residuos = entrada.get_columns_number() - 2;

    for ( int i = 0 ; i < num_entradas_sem_residuos ; i += ord )
    {
        curvas[ i ] = nomes[contador];
        contador++;
    }

    curvas[ num_entradas_sem_residuos ] = nomes[ contador++ ];
    curvas[ num_entradas_sem_residuos + 1 ] = nomes[ contador ];

    return curvas;
}


QString Rede :: nome_tipo()
{
    return QString( "RNA" );
}


void Rede :: configurar_ordem( const int &o )
{
    ord = o;
}


void Rede :: ler_arquivos()
{
    inicializar();

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
    processar_saida();
}


MatrizD Rede :: desnormalizar( const MatrizD &valores, 
                               const QList< QList< double > > &limites )
{
    MatrizD matriz = valores;

    QList< double > min = limites[0];
    QList< double > range = limites[1];

    for ( uint a = 0 ; a < n_amostras ; a++ )
    {
        // A desnormalizacao so acontece para os dados de saida, por isso so se
        // utiliza o numero de saidas
        for ( uint n = 0 ; n < n_saidas ; n++ )
        {
            matriz[a][n] = ( matriz[a][n] + 1.0 ) * ( range[n] / 2.0 ) + min[n];
        }
    }

    return matriz;
}


MatrizD Rede :: normalizar( const MatrizD &valores, 
                            const QList< QList< double > > &limites )
{
    MatrizD matriz = valores;

    QList< double > min = limites[0];
    QList< double > range = limites[1];

    for ( uint a = 0 ; a < n_amostras ; a++ )
    {
        // A normalizacao so acontece para os dados de entrada, por isso so se
        // utiliza o numero de entradas
        for ( uint n = 0 ; n < n_entradas ; n++ )
        {
            matriz[a][n] = ( matriz[a][n] - min[n] ) * ( 2.0 / range[n] ) - 1.0;
        }
    }

    return matriz;
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


void Rede :: inicializar()
{
    // Valores iniciais das variaveis do modulo
    n_amostras = n_camadas = n_entradas = n_saidas = 0;

    // Limpando as listas auxiliares
    f_ativacao.clear();
    x_min.clear();
    x_max.clear();
    x_range.clear();
    y_min.clear();
    y_max.clear();
    y_range.clear();

    // Limpando as deteccoes de falhas
    deteccoes.clear();

    // Limpando as matrizes de entrada/saida
    entrada.set( 0, 0 );
    saida.set( 0, 0 );

    // Limpando os vetores dos neuronios, pesos e biases
    n_neuronios.set( 0 );
    pesos.set( 0 );
    biases.set( 0 );
}


// Metodo utilizado para ler um arquivo que ja contem a entrada da rede
// completamente configurada, sem que haja necessidade de adicionar nenhum
// regressor ou fazer qualquer tipo de processamento
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
            throw( ExcecaoConversao( "Conversão inválida na "
                                     "leitura dos limites." ) );
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
                throw( ExcecaoConversao( "Conversão inválida na "
                                         "leitura dos limites." ) );
            }

            x_min.append( valor );

            // x_max
            valor = valores[1].toDouble( &ok );
        
            if ( !ok )
            {
                throw( ExcecaoConversao( "Conversão inválida na "
                                         "leitura dos limites." ) );
            }

            x_max.append( valor );

            // x_range
            valor = valores[2].toDouble( &ok );
            
            if ( !ok )
            {
                throw( ExcecaoConversao( "Conversão inválida na "
                                         "leitura dos limites." ) );
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
                throw( ExcecaoConversao( "Conversão inválida na "
                                         "leitura dos limites." ) );
            }

            y_min.append( valor );

            // x_max
            valor = valores[1].toDouble( &ok );
        
            if ( !ok )
            {
                throw( ExcecaoConversao( "Conversão inválida na "
                                         "leitura dos limites." ) );
            }

            y_max.append( valor );

            // x_range
            valor = valores[2].toDouble( &ok );
            
            if ( !ok )
            {
                throw( ExcecaoConversao( "Conversão inválida na "
                                         "leitura dos limites." ) );
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
            throw( ExcecaoConversao( "Conversão inválida na "
                                     "leitura da rede." ) );
        }

        n_camadas = valores[1].toUInt( &ok );

        if ( !ok )
        {
            throw( ExcecaoConversao( "Conversão inválida na "
                                     "leitura da rede." ) );
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
                throw( ExcecaoConversao( "Conversão inválida na "
                                         "leitura da rede." ) );
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
                        throw( ExcecaoConversao( "Conversão inválida na "
                                                 "leitura da rede." ) );
                    }
                }
                
                bias[i] = valores[ matriz.get_columns_number() ].toDouble( &ok );

                if ( !ok )
                {
                    throw( ExcecaoConversao( "Conversão inválida na "
                                             "leitura da rede." ) );
                }
            }

            pesos[c] = matriz;
            biases[c] = bias;
        }

        n_saidas = n_neuronios[ n_camadas - 1 ];

        arquivo.close();
    }
}


// As saidas das redes de deteccao obedecerao as seguintes condicoes:
//
// Falha em T1  =>  Bit 1 = -1
// Falha em T2  =>  Bit 2 = +1 
//
// Assim, a tabela verdade das redes de deteccao terao sempre o formato
// representado abaixo
//
// -----------------------------------------------
// |                Tabela Verdade               |
// -----------------------------------------------
// |   Bit 1   |   Bit 2   |      Resultado      |
// -----------------------------------------------
// |     0     |     0     |     Nao ha falha    |
// |    -1     |     0     |       Falha T1      |
// |     0     |     1     |       Falha T2      |
// |    -1     |     1     | Falha T1 + Falha T2 |
// -----------------------------------------------
//
// Como a saida da rede nunca e precisa, serao considerados na pratica os
// valores expostos abaixo. Observe que todas as situacoes possiveis estao 
// descritas
//
// -----------------------------------------------------------------------------
//                                   Tanque 1
// -----------------------------------------------------------------------------
//  |   Desejado   |    Obtido    |                   Resultado
// -----------------------------------------------------------------------------
//  |      0       |   >= -0.5    | Correto (Nao ha falha em T1)
//  |     -1       |    < -0.5    | Correto (Ha falha em T1)
//  |      0       |    < -0.5    | Falso positivo
//  |     -1       |   >= -0.5    | Falso negativo
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//                                   Tanque 2
// -----------------------------------------------------------------------------
//  |   Desejado   |    Obtido    |                   Resultado
// -----------------------------------------------------------------------------
//  |      0       |    <= 0.5    | Correto (Nao ha falha em T1)
//  |      1       |     > 0.5    | Correto (Ha falha em T1)
//  |      0       |     > 0.5    | Falso positivo
//  |      1       |    <= 0.5    | Falso negativo
// -----------------------------------------------------------------------------
void Rede :: processar_saida()
{
    // Obtendo a saida da rede neural
    QList< QList< double > > limites;

    limites << x_min << x_range;

    saida = rede.calculate_output_matrix( normalizar( entrada, limites ) );

    limites.clear();

    limites << y_min << y_range;

    saida = desnormalizar( saida, limites );

    limites.clear();

    // As matrizes serao criadas com 1 linha e 2 colunas. Ao final do laco, a
    // primeira linha devera ser removida
    MatrizI falhas_t1( 1, 2, 0 );
    MatrizI falhas_t2( 1, 2, 0 );

    VetorI intervalo_t1( 2, 0 );
    VetorI intervalo_t2( 2, 0 );

    bool flag_t1 = false;
    bool flag_t2 = false;

    for ( int i = 0 ; i < saida.get_rows_number() ; i++ )
    {
        // Falhas em T1 --------------------------------------------------------
        if ( saida[i][0] < -0.5 && !flag_t1 )
        {
            flag_t1 = true;

            intervalo_t1[0] = i;
        }
        else if ( saida[i][0] >= -0.5 && flag_t1 )
        {
            flag_t1 = false;

            // Tratando os casos especiais de deteccao
            // Nas falhas FSeDG, FSeDO e FSiVzT acontece o problema da deteccao
            // somente nos instantes em que ha a mudanca.
            if( falha == "FSeDG" || falha == "FSeDO" || falha == "FSiVzT" )
            {
            }
            /*
            else if ( falha == "" )
            {
            }
            */
            else
            {
                // Se o intervalo for menor do que X amostras, a deteccao sera
                // desconsiderada
                if ( ( i - intervalo_t1[0] ) < INTERVALO_MIN_DETECCAO )
                {
                    intervalo_t1.initialize( 0 );
                    continue;
                }
            }

            intervalo_t1[1] = i;

            falhas_t1.add_row( intervalo_t1 );

            intervalo_t1.initialize( 0 );
        }

        // Falhas em T2 --------------------------------------------------------
        if ( saida[i][1] > 0.5 && !flag_t2 )
        {
            flag_t2 = true;

            intervalo_t2[0] = i;
        }
        else if ( saida[i][1] <= 0.5 && flag_t2 )
        {
            flag_t2 = false;

            // Tratando os casos especiais de deteccao
            // Nas falhas FSeDG, FSeDO e FSiVzT acontece o problema da deteccao
            // somente nos instantes em que ha a mudanca.
            if( falha == "FSeDG" || falha == "FSeDO" || falha == "FSiVzT" )
            {
            }
            /*
            else if ( falha == "" )
            {
            }
            */
            else
            {
                // Se o intervalo for menor do que X amostras, a deteccao sera
                // desconsiderada
                if ( ( i - intervalo_t2[0] ) < INTERVALO_MIN_DETECCAO )
                {
                    intervalo_t2.initialize( 0 );
                    continue;
                }
            }

            intervalo_t2[1] = i;

            falhas_t2.add_row( intervalo_t2 );

            intervalo_t2.initialize( 0 );
        }
    }

    // Removendo as linhas criadas na alocacao das matrizes
    falhas_t1.subtract_row( 0 );
    falhas_t2.subtract_row( 0 );

    deteccoes[ "Falhas em T<sub>1</sub>" ] = falhas_t1;
    deteccoes[ "Falhas em T<sub>2</sub>" ] = falhas_t2;
}

#endif
