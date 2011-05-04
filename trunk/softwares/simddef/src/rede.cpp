#ifndef REDE_CPP_
#define REDE_CPP_

#include "rede.h"

Rede :: Rede() : Modulo( Modulo::RNA )
{
    inicializar();
}


Rede :: ~Rede()
{
}


int Rede :: ordem()
{
    return ord;
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
        // Configurando a entrada da rede a partir das matrizes de saida da
        // simulacao
        configurar_entrada( arquivos[0], arquivos[1] );

        // Lendo a configuracao da rede (para salvar a rede com a configuracao
        // adequada, utilize o arquivo salvar_rede.m)
        ler_rede( arquivos[2] );

        // Lendo os limites que serao aplicados para a normalizacao dos dados de
        // entrada/saida
        ler_limites( arquivos[3] );

        // Criando efetivamente a rede
        criar_rede();
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
    catch( ExcecaoTamanho e )
    {
        throw e;
    }
}


MatrizD Rede :: adicionar_regressores( const MatrizD &matriz_orig,
                                       const QList< int > &regressores )
{
    // O numero de colunas na lista regressores tem que ser obrigatoriamente
    // igual ao numero de colunas da matriz original
    if ( regressores.count() != matriz_orig.get_columns_number() )
    {
        throw ExcecaoTamanho( "Tamanho do vetor de regressores incompatível "
                              "com o número de colunas da matriz." );
    }

    // Numero de regressores a serem adicionados
    int num_regressores = 0;

    for ( int i = 0 ; i < regressores.count() ; i++ )
    {
        if ( regressores[i] > 0 )
            num_regressores++;
    }

    // Alocando a nova matriz com o mesmo numero de linhas e o numero de colunas
    // igual ao da matriz original + os regressores
    MatrizD matriz( matriz_orig.get_rows_number(),
                    matriz_orig.get_columns_number() + num_regressores,
                    0.0 );

    // Adicionando as colunas originais e os regressores
    int contador = 0;

    VetorD coluna;
    VetorD coluna_reg;

    for ( int i = 0 ; i < regressores.count() ; i++ )
    {
        coluna = matriz_orig.get_column( i );

        matriz.set_column( contador, coluna );

        // Se houver regressores
        if ( regressores[i] > 0 )
        {
            for ( int r = 0 ; r < regressores[i] ; r++ )
            {
                coluna_reg = descer_coluna( coluna, r + 1 );

                contador++;

                matriz.set_column( contador, coluna_reg );
            }
        }

        contador++;
    }

    return matriz;
}


MatrizD Rede :: concatenar_colunas( const MatrizD &matriz_1, 
                                    const MatrizD &matriz_2 )
{
    MatrizD matriz = matriz_1;

    VetorD coluna;

    for ( int i = 0 ; i < matriz_2.get_columns_number() ; i++ )
    {
        coluna = matriz_2.get_column( i );
        matriz.add_column( coluna );
    }

    return matriz;
}


MatrizD Rede :: ler_dados( const QString &nome_arq )
{
    MatrizD matriz;

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
        matriz.set( 1, num_colunas, 0.0 );

        bool ok = true;

        double valor = 0.0;

        while( !linha.isNull() )
        {
            valores = linha.split( QRegExp( "\t" ) );

            VetorD lin( num_colunas );

            for ( int j = 0 ; j < valores.count() ; j++ )
            {
                valor = valores[j].toDouble( &ok );

                if ( !ok )
                {
                    throw( ExcecaoConversao( "Conversão inválida na "
                                             "leitura da matriz." ) );
                }
                
                lin[j] = valor;
            }

            matriz.add_row( lin );

            linha = stream.readLine();
        }

        matriz.subtract_row( 0 );

        arquivo.close();
    }

    return matriz;
}


MatrizD Rede :: remover_colunas( const QList< int > &colunas,
                                 const MatrizD &m )
{
    MatrizD matriz = m;

    for ( int i = 0 ; i < colunas.count() ; i++ )
    {
        // Cada vez que uma coluna e removida, o numero de colunas eh reduzido
        // em uma unidade. Por esse motivo, ha a subtracao de i
        matriz.subtract_column( colunas[i] - i );
    }

    return matriz;
}


VetorD Rede :: descer_coluna( const VetorD &coluna, 
                              const int &num_vezes,
                              const double &valor )
{
    VetorD vetor( coluna.get_size(), valor );

    // Repetindo o restante dos elementos
    for ( int i = num_vezes ; i < vetor.get_size() ; i++ )
    {
        vetor[i] = coluna[ i - num_vezes ];
    }

    return vetor;
}


void Rede :: configurar_entrada( const MatrizD &e )
{
    entrada = e;
}


void Rede :: configurar_entrada( const QString &arq_niveis,
                                 const QString &arq_erro_sc )
{
    // A entrada padrao das redes de configuracao e composta pelas seguintes
    // colunas:
    //
    // =========================================================================
    // Configuracao de colunas das redes de deteccao de falhas
    // Obs: A ordem do modelo ira acrescentar novas colunas com os respectivos
    //      regressores
    // -------------------------------------------------------------------------
    // Entrada
    // -------------------------------------------------------------------------
    // Coluna 0: L1(k-1)
    // Coluna 1: L2(k-1)
    // Coluna 2: Vp1(k)
    // Coluna 3: Vp2(k)
    // Coluna 4: e1(k)
    // Coluna 5: e2(k)
    // -------------------------------------------------------------------------
    // Saida (Palavra binaria de 2 bits)
    // -------------------------------------------------------------------------
    // Coluna 0: FN1(k) - Bit de saida 0
    // Coluna 1: FN2(k) - Bit de saida 1
    //
    // FN1 FN2   Resultado
    //  0   0    Sem Falha
    //  0   1    Falha T1
    //  1   0    Falha T2
    //  1   1    Falha T1 + Falha T2
    // =========================================================================
    //
    // Os erros e1(k) e e2(k) sao obtidos a partir da diferenca entre o valor
    // real do nivel e o valor esperado. Para isso, precisa-se fazer uma
    // simulacao do sistema atraves da rede neural de identificacao ou atraves
    // de um script do matlab.
    //
    // A simulacao utilizada aqui sera feita atraves de uma rede de
    // identificacao. Portanto, precisa-se criar a rede com os pesos do
    // modelo identificado, configurar a entrada desta rede de acordo com as
    // colunas indicadas abaixo e obter a saida da simulacao. De posse desa
    // saida, sera possivel obter os erros e1(k) e e2(k) utilizados na rede de
    // deteccao de falhas.
    //
    // =========================================================================
    // Configuracao de colunas da entrada da rede de identificacao do modelo
    // Obs: A ordem do modelo ira acrescentar novas colunas com os respectivos
    //      regressores
    // -------------------------------------------------------------------------
    // Entrada
    // -------------------------------------------------------------------------
    // Coluna 0: L1(k-1)
    // Coluna 1: L2(k-1)
    // Coluna 2: Vp1(k)
    // Coluna 3: Vp2(k)
    // -------------------------------------------------------------------------
    // Saida
    // -------------------------------------------------------------------------
    // Coluna 0: L1_est(k)
    // Coluna 1: L2_est(k)
    // =========================================================================
    //
    // As colunas dos arquivos de niveis e erro_sc obedecem a seguinte ordem
    //
    // -------------------------------------------------------------------------
    // Ordem das colunas - Arquivo de niveis
    // -------------------------------------------------------------------------
    // [0] - Tempo
    // [1] - Referencia T1
    // [2] - Referencia T2
    // [3] - Nivel T1
    // [4] - Nivel T2
    // -------------------------------------------------------------------------
    // Ordem das colunas - Arquivo de erro/sinal de controle
    // -------------------------------------------------------------------------
    // [0] - Tempo
    // [1] - Erro T1
    // [2] - Erro T2
    // [3] - Sinal de controle T1
    // [4] - Sinal de controle T2
    // [5] - Acao Proporcional T1
    // [6] - Acao Proporcional T2
    // [7] - Acao Integral T1
    // [8] - Acao Integral T2
    // [9] - Acao Derivativa T1
    // [10] - Acao Derivativa T2 

    // Matrizes de saida da simulacao
    MatrizD niveis = ler_dados( arq_niveis );
    MatrizD erro_sc = ler_dados( arq_erro_sc );

    // A matriz de entrada nao podera conter o nivel atual dos tanques. Assim,
    // sera considerado somente o nivel anterior e so depois os regressores
    // serao adicionados de acordo com a ordem da rede

    // Colunas a serem removidas
    QList< int > colunas_niveis;
    colunas_niveis << 0 << 1 << 2;

    QList< int > colunas_erro_sc;
    colunas_erro_sc << 0 << 1 << 2 << 5 << 6 << 7 << 8 << 9 << 10;

    niveis = remover_colunas( colunas_niveis, niveis );
    erro_sc = remover_colunas( colunas_erro_sc, erro_sc );

    MatrizD entrada_ident = concatenar_colunas( niveis, erro_sc );

    // A melhor rede de identificacao obtida foi a P1 O2 N8 T2 (proposta 1,
    // ordem 2, 8 neuronios na camada oculta, treinamento 2)
    int ordem_ident = 2;

    // Determinando quantos regressores serao adicionados para cada uma das
    // colunas da matriz
    QList< int > regressores;
    regressores << ordem_ident - 1
                << ordem_ident - 1
                << ordem_ident - 1
                << ordem_ident - 1;

    // Adicionando os regressores para compor a entrada da rede de identificacao
    try
    {
        entrada_ident = adicionar_regressores( entrada_ident, regressores );
    }
    catch( ExcecaoTamanho e )
    {
        throw e;
    }

    // Nesse ponto a entrada da rede de identificacao esta configurada. Assim
    // sendo, serao abertos os arquivos de configuracao da rede de identificacao
    // e em seguida a rede sera executada. Os arquivos de configuracao da melhor
    // rede de identificacao encontram-se no recurso (":rnas/rede_ident.cfg" e
    // ":rnas/rede_ident.lim")
    Rede rede_ident;

    try
    {
        rede_ident.ler_rede( ":rnas/rede_ident.cfg" );
        rede_ident.ler_limites( ":rnas/rede_ident.lim" );
        rede_ident.criar_rede();
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

    rede_ident.configurar_ordem( ordem_ident );
    rede_ident.configurar_entrada( entrada_ident );
    
    // Obtendo a saida da rede de identificacao
    MatrizD saida_ident = rede_ident.executar();

    // Obtendo os erros e1(k) e e2(k)
    MatrizD erros = niveis - saida_ident;

    // Compondo a entrada da matriz de deteccao
    MatrizD entrada_detec = concatenar_colunas( niveis, erro_sc );

    entrada_detec = concatenar_colunas( entrada_detec, erros );

    // Adicionando os regressores da entrada de deteccao
    regressores.clear();

    // A variavel 'ord', pertencente a esta classe (this), se refere a ordem da
    // rede de deteccao que foi configurada ao ler o arquivo XML
    regressores << ord - 1
                << ord - 1
                << ord - 1
                << ord - 1
                << ord - 1
                << ord - 1;

    try
    {
        entrada = adicionar_regressores( entrada_detec, regressores );
    }
    catch( ExcecaoTamanho e )
    {
        throw e;
    }

    n_amostras = entrada.get_rows_number();
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


void Rede :: configurar_ordem( const int &o )
{
    ord = o;
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
    ord = -1;
    n_amostras = n_camadas = n_entradas = n_saidas = 0;
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
