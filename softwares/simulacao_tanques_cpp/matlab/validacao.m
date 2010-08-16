% Arquivo para validacao das redes treinadas

% Removendo os avisos de adicao de nova planilha do Excel
warning off MATLAB:xlswrite:AddSheet;

clear;
clc;
close all;

% pasta_valid = input( 'Pasta dos arquivos de validacao: ' );
pasta_valid = 'D:\documentos\Diogo\diogolrmsc@ggc\softwares\simulacao_tanques_cpp\saidas\20_min_FSeG\';
% pasta_valid = 'E:\documentos\diogolrmsc@ggc\softwares\simulacao_tanques_cpp\saidas\20_min_FSeG\';

% pasta_rnas = input( 'Pasta dos arquivos das redes treinadas: ' );
% pasta_rnas = 'D:\documentos\Diogo\diogolrmsc@ggc\qualificacao\dados_matlab\identificacao\';
pasta_rnas = 'D:\documentos\Diogo\diogolrmsc@ggc\qualificacao\dados_matlab\deteccao\FSeG\';
% pasta_rnas = 'E:\documentos\diogolrmsc@ggc\qualificacao\dados_matlab\deteccao\FAVK\';

% Determinando se sera treinada uma RNA para identificacao do sistema ou
% para deteccao de falha --------------------------------------------------
% falha = input( 'RNA para deteccao de falha [0 ou 1]: ' );
falha = 1;

% Determinando o numero de redes a serem treinadas ------------------------
% n_treinamentos = input( 'Numero de treinamentos: ' );
n_treinamentos = 6;

% Determinando o numero de regressores de acordo com a ordem do modelo ----
ordem = input( 'Ordem do modelo: ' );

regressores = ordem - 1;

% Gerar planilha do excel
% gerar_planilha = input( 'Gerar planilha do excel [0 ou 1]: ' );
gerar_planilha = 1;

if falha
    % Determinando o numero de neuronios nas camadas ocultas --------------
    % Para compor o vetor/matriz do numero de neuronios das camadas ocultas
    % deve-se considerar que cada rede so tera uma camada oculta. Logo, se
    % foram realizados os treinamentos com 8, 10 e 12 neuronios, o 
    % vetor nco sera:
    %
    % nco = [8 10 12]; 
    nco = input( 'Neuronios das camadas ocultas [vetor]: ' );
else
    % Determinando a proposta para adequar a saida da rede ----------------
    % disp( ' ' );
    % disp( 'Propostas: ');
    % disp( '[1] RNA para identificacao global' );
    % disp( '[2] RNA para identificacao individual ou em separado' );
    % disp( ' ' );
    % proposta = input( 'Proposta: ' );
    % disp( ' ' );
    proposta = 1;

    % Determinando o numero de neuronios nas camadas ocultas --------------
    % Para compor o vetor/matriz do numero de neuronios das camadas ocultas
    % deve-se considerar que cada rede so tera uma camada oculta. Logo, se
    % foram realizados os treinamentos com 8, 10 e 12 neuronios, o 
    % vetor/matriz nco sera:
    %
    % Proposta 1
    % nco = [8 10 12]; 
    %
    % Proposta 2
    %
    % nco = [ 8 10 12 ; -> Sequencia da RNA 1
    %         8 10 12 ] -> Sequencia da RNA 2
    if proposta == 1
        % Determinando o numero de neuronios nas camadas ocultas
        nco = input( 'Neuronios das camadas ocultas [vetor]: ' );
    else
        % Determinando o numero de neuronios nas camadas ocultas
        nco = input( 'Neuronios das camadas ocultas [matriz]: ' );
    end
end

for v = 1 : 3
    disp( strcat( 'Iniciando a validacao ', num2str( v ), '...' ) );
    
    % Lendo o arquivo de validacao ----------------------------------------
    nome_arq_niveis = strcat( 'niveis_v', num2str( v ), '.dat' );
    nome_arq_erro_sc = strcat( 'erro_sc_v', num2str( v ), '.dat' );
    
    mat_niveis = dlmread( strcat( pasta_valid, nome_arq_niveis ), '\t' );
    mat_erro_sc = dlmread( strcat( pasta_valid, nome_arq_erro_sc ), '\t' );

    % Ajustanto os dados das matrizes de entrada e saida ------------------
    [entrada saida] = ajustar_dados( mat_niveis, mat_erro_sc );
    
    clear mat_niveis;
    clear mat_erro_sc;
    
    % Deteccao de falha
    if falha == 1
        entrada( :, [3:4 7:12] ) = [];

        % Vetor auxiliar para determinacao dos regressores
        vetor = ones( 1, 4 );

        % Entrada para a melhor rede com regressores
        ordem_melhor_rede = 2;
        regressores_melhor_rede = ordem_melhor_rede - 1;

        entrada_melhor_rede = adicionar_regressores( entrada, ...
                                           regressores_melhor_rede*vetor );

        % Entrada para rede atual com regressores
        entrada = adicionar_regressores( entrada, regressores*vetor );

        clear vetor;

        entrada_melhor_rede = entrada_melhor_rede';
        entrada = entrada';
        saida = saida';

        [num_entradas num_amostras] = size( entrada );

        % Adicionando as colunas dos erros de estimativa
        arq_config = 'P1O2N8T2';

        load( arq_config );

        saida_rna = sim( rede, entrada_melhor_rede );

        erros = saida - saida_rna;

        entrada( num_entradas + 1 : num_entradas + 2, : ) = erros;

        % Configurando a nova saida
        saida = zeros( num_amostras, 2 );

        % Dentro do tempo total, sera simulado um periodo sem falha, um 
        % periodo com falha somente no tanque 1, um periodo com falha 
        % somente no tanque 2 e um ultimo periodo com falha nos dois 
        % tanques. Por esse motivo o numero de amostras e dividido por 
        % quatro.
        %
        % No primeiro intervalo nao sera simulada nenhuma falha, logo so ha
        % necessidade de modificar os intervalos 2 a 4. Portanto, para um 
        % caso com 12000 amstras as linhas a serem modificadas serao da 
        % 3001 ate a 6000, da 6001 ate a 9000 e da 9001 ate a 12000
        div = num_amostras / 4;

        linhas = [div+1 2*div; 2*div+1 3*div; 3*div+1 4*div];

        % Falha T1
        saida( linhas( 1, 1 ) : linhas( 1, 2 ), 1 ) = -1.0;
        % Falha T2
        saida( linhas( 2, 1 ) : linhas( 2, 2 ), 2 ) = 1.0;
        % Falha T1 + Falha T2
        saida( linhas( 3, 1 ) : linhas( 3, 2 ), 1 ) = -1.0;
        saida( linhas( 3, 1 ) : linhas( 3, 2 ), 2 ) = 1.0;

        saida = saida';
        
        % Configurando o nome das subpastas para ler os arquivos
        % corretamente
        for n = 1 : size( nco, 2 )
            subpasta = strcat( 'O', num2str( ordem ), ...
                               '\N', num2str( nco( n ) ), '\' );

            % Nome do arquivo a ser aberto - Removendo as contrabarras
            nome_arq = regexprep( subpasta, '\', '' );

            % Validacao para cada treinamento
            for t = 1 : n_treinamentos
                % Adicionando o numero de treinamento ao nome do arquivo
                nome_arq_trein = strcat( nome_arq, 'T', num2str( t ) );

                % Limpando as variaveis abertas
                clear rede tempo_treinamento lim_ent lim_sai;

                % Abrindo o arquivo da rede salva
                load( strcat( pasta_rnas, subpasta, nome_arq_trein ) );

                % Validando a rede
                [saida_rna resultados] = validar_rede( rede, ...
                                                       entrada, ...
                                                       saida, ...
                                                strcat( nome_arq_trein, ...
                                                        '_v', ...
                                                        num2str( v ) ), ...
                                                       -1 );

                % Salvando uma planilha para facilitar a analise dos
                % resultados
                if gerar_planilha
                    % Nome do arquivo do excel
                    nome_arq_excel = strcat( 'O', ...
                                             num2str( ordem ), ...
                                             '.xls' );

                    % Matriz a ser salva
                    if t == 1
                        matriz = { 'Acertos' ...
                                   'Erros Tipo I - T1' ...
                                   'Erros Tipo I - T2' ...
                                   'Erros Tipo I' ...
                                   'Erros Tipo II - T1' ...
                                   'Erros Tipo II - T2' ...
                                   'Erros Tipo II' ; ...
                            ( resultados( 1, 1 ) + resultados( 2, 1 ) ) ...
                                   resultados( 1, 2 ) ...
                                   resultados( 2, 2 ) ...
                            ( resultados( 1, 2 ) + resultados( 2, 2 ) ) ...
                                   resultados( 1, 3 ) ...
                                   resultados( 2, 3 ) ...
                            ( resultados( 1, 3 ) + resultados( 2, 3 ) ) };
                    else
                        matriz = { ...
                            ( resultados( 1, 1 ) + resultados( 2, 1 ) ) ...
                                   resultados( 1, 2 ) ...
                                   resultados( 2, 2 ) ...
                            ( resultados( 1, 2 ) + resultados( 2, 2 ) ) ...
                                   resultados( 1, 3 ) ...
                                   resultados( 2, 3 ) ...
                            ( resultados( 1, 3 ) + resultados( 2, 3 ) ) };
                    end

                    % Celula a ser escrita
                    if t == 1
                        linha = t;
                    else
                        linha = t+1;
                    end
                    
                    % Pulando o numero de linhas necessarias para cada
                    % validacao
                    if v >= 2
                        linha = linha + ( v - 1 ) * ( n_treinamentos + 2 );
                    end

                    coluna = 'A';

                    celula = strcat( coluna, num2str( linha ) );

                    % Nome da planilha
                    nome_planilha = strcat( 'N', num2str( nco( n ) ) );

                    % Escrevendo o arquivo
                    xlswrite( nome_arq_excel, ...
                              matriz, ...
                              nome_planilha, ...
                              celula );
                end
            end
        end
    % Identificacao
    else
        % Proposta 1 ----------------------------------------------------------
        if proposta == 1
            % Excluindo as colunas que nao serao utilizadas
            entrada( :, [3:4 7:12] ) = [];

            % Adicionando regressores
            vetor = ones( 1, 4 );

            entrada = adicionar_regressores( entrada, regressores*vetor );

            clear vetor;

            % Transpondo a entrada/saida para adequar a da RNA
            entrada = entrada';
            saida = saida';

            % Configurando o nome das subpastas para ler os arquivos
            % corretamente
            for n = 1 : size( nco, 2 )
                subpasta = strcat( 'P1\O', num2str( ordem ), ...
                                   '\N', num2str( nco( n ) ), '\' );

                % Nome do arquivo a ser aberto - Removendo as contrabarras
                nome_arq = regexprep( subpasta, '\', '' );

                % Validacao para cada treinamento
                for t = 1 : n_treinamentos
                    % Adicionando o numero de treinamento ao nome do arquivo
                    nome_arq_trein = strcat( nome_arq, 'T', num2str( t ) );

                    % Limpando as variaveis abertas
                    clear rede tempo_treinamento lim_ent lim_sai;

                    % Abrindo o arquivo da rede salva
                    load( strcat( pasta_rnas, subpasta, nome_arq_trein ) );

                    % Validando a rede
                    [saida_rna emq] = validar_rede( rede, entrada, ...
                                                    saida, ...
                                                strcat( nome_arq_trein, ...
                                                        '_v', ...
                                                        num2str( v ) ), ...
                                                    proposta );


                    % Salvando uma planilha para facilitar a analise dos
                    % resultados
                    if gerar_planilha
                        % Nome do arquivo do excel
                        nome_arq_excel = strcat( 'P1O', ...
                                                 num2str( ordem ), ...
                                                 '.xls' );

                        % Matriz a ser salva
                        if t == 1
                            matriz = { 'EMQ L1' 'EMQ L2' ; ...
                                       emq( 1, 1 ) emq( 2, 1 ) };
                        else
                            matriz = { emq( 1, 1 ) emq( 2, 1 ) };
                        end

                        % Celula a ser escrita
                        if t == 1
                            linha = t;
                        else
                            linha = t+1;
                        end

                        if v == 1
                            coluna = 'A';
                        elseif v == 2
                            coluna = 'D';
                        else
                            coluna = 'G';
                        end

                        celula = strcat( coluna, num2str( linha ) );

                        % Nome da planilha
                        nome_planilha = strcat( 'N', num2str( nco( n ) ) );

                        % Escrevendo o arquivo
                        xlswrite( nome_arq_excel, ...
                                  matriz, ...
                                  nome_planilha, ...
                                  celula );
                    end
                end
            end

        % Proposta 2 ----------------------------------------------------------
        else
            % Excluindo as colunas que nao serao utilizadas
            entrada_rna1 = entrada;
            entrada_rna2 = entrada;

            entrada_rna1( :, [2:4 6:12] ) = [];
            entrada_rna2( :, [3:4 7:12] ) = [];

            % Adicionando os regressores
            vetor_rna1 = ones( 1, 2 );
            vetor_rna2 = ones( 1, 4 );

            entrada_rna1 = adicionar_regressores( entrada_rna1, ...
                                                  regressores*vetor_rna1 );
            entrada_rna2 = adicionar_regressores( entrada_rna2, ...
                                                  regressores*vetor_rna2 );

            clear vetor_rna1 vetor_rna2;

            % Transpondo as matrizes de entrada/saida para adequar as
            % entradas/saidas das RNAs
            entrada_rna1 = entrada_rna1';
            entrada_rna2 = entrada_rna2';
            saida = saida';

            % Configurando o nome das subpastas para ler os arquivos
            % corretamente
            for n = 1 : size( nco, 2 )
                subpasta = strcat( 'P2\O', num2str( ordem ), ...
                                   '\N', num2str( nco( 1, n ) ), ...
                                   'N', num2str( nco( 2, n ) ), '\' );

                % Nome do arquivo a ser aberto - Removendo as contrabarras
                nome_arq = regexprep( subpasta, '\', '' );

                % Validacao para cada treinamento
                for t = 1 : n_treinamentos
                    % Adicionando o numero de treinamento ao nome do arquivo
                    nome_arq_trein = strcat( nome_arq, 'T', num2str( t ) );

                    % Limpando as variaveis abertas
                    clear rede_1 rede_2 ...
                          tempo_treinamento_rna1 tempo_treinamento_rna2 ...
                          lim_ent_rna1 lim_ent_rna2 lim_sai_rna1 lim_sai_rna2;

                    % Abrindo o arquivo da rede salva
                    load( strcat( pasta_rnas, subpasta, nome_arq_trein ) );

                    % Validando a rede
                    [saida_r1 emq_r1] = validar_rede( rede_1, ...
                                                      entrada_rna1, ...
                                                      saida( 1, : ), ...
                                                strcat( nome_arq_trein, ...
                                                        '_v', ...
                                                        num2str( v ), ...
                                                        '_R1' ), ...
                                                      proposta );
                    [saida_r2 emq_r2] = validar_rede( rede_2, ...
                                                      entrada_rna2, ...
                                                      saida( 2, : ), ...
                                                strcat( nome_arq_trein, ...
                                                        '_v', ...
                                                        num2str( v ), ...
                                                        '_R2' ), ...
                                                      proposta );

                    % Salvando uma planilha para facilitar a analise dos
                    % resultados
                    if gerar_planilha
                        % Nome do arquivo do excel
                        nome_arq_excel = strcat( 'P2O', ...
                                                 num2str( ordem ), ...
                                                 '.xls' );

                        % Matriz a ser salva
                        if t == 1
                            matriz = { 'EMQ L1' 'EMQ L2' ; emq_r1 emq_r2 };
                        else
                            matriz = { emq_r1 emq_r2 };
                        end

                        % Celula a ser escrita
                        if t == 1
                            linha = t;
                        else
                            linha = t+1;
                        end

                        if v == 1
                            coluna = 'A';
                        elseif v == 2
                            coluna = 'D';
                        else
                            coluna = 'G';
                        end

                        celula = strcat( coluna, num2str( linha ) );

                        % Nome da planilha
                        nome_planilha = strcat( 'N', ...
                                                num2str( nco( 1, n ) ), ...
                                                'N', ...
                                                num2str( nco( 2, n ) ) );

                        % Escrevendo o arquivo
                        xlswrite( nome_arq_excel, ...
                                  matriz, ...
                                  nome_planilha, ...
                                  celula );
                    end
                end
            end
        end
    end     
end