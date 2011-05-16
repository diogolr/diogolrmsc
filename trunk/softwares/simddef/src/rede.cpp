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
        configurar_entrada( arquivos[0], arquivos[1] );
        ler_rede( arquivos[2] );
        ler_limites( arquivos[3] );
    }
    catch( Excecao e )
    {
        throw e;
    }
    
    criar_rede();
    processar_saida();
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


MatrizD Rede :: executar()
{
    QList< QList< double > > limites;

    limites << x_min << x_range;

    saida = rede.calculate_output_matrix( normalizar( entrada, limites ) );

    limites.clear();

    limites << y_min << y_range;

    saida = desnormalizar( saida, limites );

    limites.clear();

    return saida;
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
                    throw( ExcecaoConversao( "ConversÃ£o invÃ¡lida na "
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


void Rede :: configurar_entrada( const MatrizD &ent )
{
    n_entradas = ent.get_columns_number();
    n_amostras = ent.get_rows_number();

    entrada = ent;
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

    // Matriz de saída desejada [L1(k) e L2(k)] para a rede de identificação,
    // utilizada para gerar os resíduos e(k)
    QList< int > colunas_niveis;

    colunas_niveis << 0 << 1 << 2;

    MatrizD saida_desejada_ident = remover_colunas( colunas_niveis, niveis );

    // A matriz de entrada da rede de identificação não poderá conter o nível
    // atual dos tanques, uma vez que estes são as variáveis de saída desejadas.
    // Assim, será considerado somente o nível anterior e so depois os
    // regressores serao adicionados de acordo com a ordem da rede
    QList< int > regressores;

    regressores << 0 << 0 << 0 << 1 << 1;

    niveis = adicionar_regressores( niveis, regressores );

    // Colunas a serem removidas (Matriz de níveis)
    // Observação: Após adicionar os regressores dos níveis, foram acrescidas
    // duas colunas à matriz original.
    colunas_niveis << 3 << 5;

    // Colunas a serem removidas (Matriz de erro e sinais de controle)
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
    regressores.clear();

    regressores << ordem_ident - 1
                << ordem_ident - 1
                << ordem_ident - 1
                << ordem_ident - 1;

    // Adicionando os regressores para compor a entrada da rede de identificacao
    try
    {
        entrada_ident = adicionar_regressores( entrada_ident, regressores );
    }
    catch( Excecao e )
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
    catch( Excecao e )
    {
        throw e;
    }

    rede_ident.configurar_ordem( ordem_ident );
    rede_ident.configurar_entrada( entrada_ident );
    
    // Obtendo a saida da rede de identificacao
    MatrizD saida_ident = rede_ident.executar();

    // Obtendo os erros e1(k) e e2(k)
    MatrizD erros = saida_desejada_ident - saida_ident;

    // Compondo a entrada da matriz de deteccao
    MatrizD entrada_detec = concatenar_colunas( niveis, erro_sc );

    entrada_detec = concatenar_colunas( entrada_detec, erros );

    // Adicionando os regressores da entrada de deteccao
    regressores.clear();

    // A variavel 'ord', pertencente a esta classe (this), se refere a ordem da
    // rede de deteccao que foi configurada ao ler o arquivo XML. As duas
    // últimas colunas são os erros de estimativa e não possuem regressores
    regressores << ord - 1
                << ord - 1
                << ord - 1
                << ord - 1
                << 0
                << 0;

    try
    {
        entrada = adicionar_regressores( entrada_detec, regressores );
    }
    catch( ExcecaoTamanho e )
    {
        throw e;
    }

    entrada.save( "entrada_lavai.dat" );

    n_entradas = entrada.get_columns_number();
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
        throw ExcecaoArquivo( "O arquivo <b>" + nome_arq + "</b> não pôde ser "
                              "aberto." );
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
        throw ExcecaoArquivo( "O arquivo <b>" + nome_arq + "</b>não pôde ser "
                              "aberto." );
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
    this->executar();

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
