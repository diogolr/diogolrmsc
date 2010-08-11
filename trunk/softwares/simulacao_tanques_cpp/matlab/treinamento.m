% Arquivo de Treinamento das RNAs que detectam as falhas

clear;
clc;
close all;

% arq_niveis = input( 'Niveis [treinamento]: ' );
arq_niveis = 'D:\documentos\Diogo\diogolrmsc@ggc\softwares\simulacao_tanques_cpp\saidas\10_min_normal\niveis_treinamento.dat';
% arq_niveis = 'E:\documentos\diogolrmsc@ggc\softwares\simulacao_tanques_cpp\saidas\10_min_normal\niveis_treinamento.dat';

% arq_erro_sc = input( 'Erro e Sinal de controle [treinamento]: ' );
arq_erro_sc = 'D:\documentos\Diogo\diogolrmsc@ggc\softwares\simulacao_tanques_cpp\saidas\10_min_normal\erro_sc_treinamento.dat';
% arq_erro_sc = 'E:\documentos\diogolrmsc@ggc\softwares\simulacao_tanques_cpp\saidas\10_min_normal\erro_sc_treinamento.dat';

mat_niveis = dlmread( arq_niveis, '\t' );
mat_erro_sc = dlmread( arq_erro_sc, '\t' );

% Determinando se sera treinada uma RNA para identificacao do sistema ou
% para deteccao de falha --------------------------------------------------
falha = input( 'RNA para deteccao de falha [0 ou 1]: ' );

% Determinando o numero de redes a serem treinadas ------------------------
n_treinamentos = input( 'Numero de treinamentos: ' );

% Determinando o numero de regressores de acordo com a ordem do modelo ----
ordem = input( 'Ordem do modelo: ' );

regressores = ordem - 1;

% Ajustanto os dados das matrizes de entrada e saida ----------------------
[entrada_pad saida_pad] = ajustar_dados( falha, mat_niveis, mat_erro_sc );

clear mat_niveis mat_erro_sc;

if falha
    % TODO
    % TODO
else
    % Determinando a proposta para adequar a saida da rede ----------------
    disp( ' ' );
    disp( 'Propostas: ');
    disp( '[1] RNA para identificacao global' );
    disp( '[2] RNA para identificacao individual ou em separado' );
    disp( ' ' );
    proposta = input( 'Proposta: ' );
    disp( ' ' );
    
    if proposta == 1
        % Determinando o numero de neuronios nas camadas ocultas
        nco = input( 'Neuronios das camadas ocultas [vetor]: ' );
    else
        % Determinando o numero de neuronios nas camadas ocultas
        nco = input( 'Neuronios das camadas ocultas [matriz]: ' );
    end
end

% Exibindo as colunas existentes na matriz --------------------------------
nomes_colunas = { 'L1(k-1)' 'L2(k-1)' 'E1(k-1)' 'E2(k-1)' ...
                  'Vp1(k)' 'Vp2(k)' 'AcaoP1(k)' 'AcaoP2(k)' ...
                  'AcaoI1(k)' 'AcaoI2(k)' 'AcaoD1(k)' 'AcaoD2(k)' };

indices_mantidos = 1 : length( nomes_colunas );

disp( ' ' );

disp( 'Colunas existentes: ' );

for i = 1 : length( indices_mantidos )
    disp( strcat( '[', num2str( i ), '] ', ...
                  nomes_colunas{ indices_mantidos( i ) } ) );
end

disp( ' ' );

% Determinando os parametros de treinamento -------------------------------
% normalizar = input( 'Normalizar dados [0 ou 1]: ' );
normalizar = 0;

% if normalizar == 1
%     min = input( 'Valor minimo: ' );
%     max = input( 'Valor maximo: ' );
% end

min = -1;
max = 1;

for r = 1 : n_treinamentos
    entrada = entrada_pad;
    saida = saida_pad;
    
    disp( ' ' );
    disp( strcat( 'Iniciando treinamento ', num2str( r ), '...' ) );
    
    indices_mantidos = 1 : length( nomes_colunas );
    
    % =====================================================================
    % Treinamento para deteccao de FALHA
    % =====================================================================
    if falha
        % Determinando as colunas a serem excluidas .......................
        colunas = input( 'Colunas a serem excluidas [vetor]: ' );

        % Determinacao da saida da rede -----------------------------------

        % Determinacao dos parametros de treinamento ----------------------

        % Treinamento -----------------------------------------------------

        % Salvar redes ----------------------------------------------------


    % =====================================================================
    % Treinamento para IDENTIFICACAO do modelo
    % =====================================================================
    else
        % Proposta 1
        if proposta == 1
            % Excluindo as colunas que nao serao utilizadas
            entrada( :, [3:4 7:12] ) = [];
            indices_mantidos( [3:4 7:12] ) = [];

            % Exibindo as colunas que restaram       
%             disp( 'Colunas existentes apos remocao: ' );
% 
%             for i = 1 : length( indices_mantidos )
%                 disp( strcat( '[', num2str( i ), '] ', ...
%                               nomes_colunas{ indices_mantidos( i ) } ) );
%             end
% 
%             disp( ' ' );

            % Determinando o numero de regressores por coluna
%            regressores = input( 'Regressores [vetor]: ' );

            vetor = ones( size( indices_mantidos ) );

            entrada = adicionar_regressores( entrada, regressores*vetor );

            clear vetor;

            % Transpondo a entrada/saida para adequar a da RNA
            entrada = entrada';
            saida = saida';

            % Treinando a RNA          
            tic
            [rede lim_ent lim_sai] = treinar_rna( entrada, ...
                                                  saida, ...
                                                  nco, min, max, ...
                                                  normalizar );
                                              
            tempo_treinamento = toc;

            % Salvando o ambiente
%             % Nome com camada oculta
%             nome_arq = strcat( 'P1O', num2str( ordem ), ...
%                                'CO', num2str( size( nco, 2 ) ), ...
%                                'T', num2str( r ) );
%
%             for i = 1 : length( nco )
%                 nome_arq = strcat( nome_arq, '_', num2str( nco( i ) ) );
%             end

            % Nome sem camada oculta
            nome_arq = strcat( 'P1O', num2str( ordem ), ...
                               'N', num2str( nco ), ...
                               'T', num2str( r ) );

            save( nome_arq, 'rede', 'lim_ent', 'lim_sai', ...
                            'tempo_treinamento' );

            % Salvando as informacoes da rede
            salvar_rede( rede, strcat( nome_arq, '_RNA' ) );

        % Proposta 2    
        else
            % Excluindo as colunas que nao serao utilizadas
            entrada_rna1 = entrada;
            entrada_rna2 = entrada;
            indices_mantidos_rna1 = indices_mantidos;
            indices_mantidos_rna2 = indices_mantidos;

            clear entrada;
            clear indices_mantidos;

            % Excluindo as colunas que nao serao utilizadas
            entrada_rna1( :, [2:4 6:12] ) = [];
            indices_mantidos_rna1( [2:4 6:12] ) = [];

            entrada_rna2( :, [3:4 7:12] ) = [];
            indices_mantidos_rna2( [3:4 7:12] ) = [];

            % Exibindo as colunas que restaram
%             disp( ' ' );
% 
%             disp( 'Colunas existentes apos remocao - RNA 1: ' );
% 
%             for i = 1 : length( indices_mantidos_rna1 )
%                 disp( strcat( '[', num2str( i ), '] ', ...
%                            nomes_colunas{ indices_mantidos_rna1( i ) } ) );
%             end
% 
%             disp( ' ' );
% 
%             % Exibindo as colunas que restaram
%             disp( ' ' );
% 
%             disp( 'Colunas existentes apos remocao - RNA 2: ' );
% 
%             for i = 1 : length( indices_mantidos_rna2 )
%                 disp( strcat( '[', num2str( i ), '] ', ...
%                            nomes_colunas{ indices_mantidos_rna2( i ) } ) );
%             end
% 
%             disp( ' ' );
% 
%             % Determinando o numero de regressores por coluna para cada
%             % proposta
%             regressores_rna1 = input( 'Regressores RNA 1 [vetor]: ' );
%             regressores_rna2 = input( 'Regressores RNA 2 [vetor]: ' );
% 
%             entrada_rna1 = adicionar_regressores( entrada_rna1, ...
%                                                   regressores_rna1 );
%             entrada_rna2 = adicionar_regressores( entrada_rna2, ...
%                                                   regressores_rna2 );
            vetor_rna1 = ones( size( indices_mantidos_rna1 ) );
            vetor_rna2 = ones( size( indices_mantidos_rna2 ) );

            entrada_rna1 = adicionar_regressores( entrada_rna1, ...
                                                  regressores*vetor_rna1 );
            entrada_rna2 = adicionar_regressores( entrada_rna2, ...
                                                  regressores*vetor_rna2 );

            clear vetor_rna1 vetor_rna2;

            % Determinando as saidas
            saida_rna1 = saida( :, 1 );
            saida_rna2 = saida( :, 2 );

            % Trasnpondo as entradas/saidas para se adequarem das RNAs
            entrada_rna1 = entrada_rna1';
            entrada_rna2 = entrada_rna2';
            saida_rna1 = saida_rna1';
            saida_rna2 = saida_rna2';

            % Treinando a RNA
            tic
            [rede_1 lim_ent_rna1 lim_sai_rna1] = treinar_rna( ...
                                                          entrada_rna1, ...
                                                          saida_rna1, ...
                                                          nco( 1, : ), ...
                                                          min, ...
                                                          max, ...
                                                          normalizar );
            tempo_treinamento_rna1 = toc;
                                                      
            tic
            [rede_2 lim_ent_rna2 lim_sai_rna2] = treinar_rna( ...
                                                          entrada_rna2, ...
                                                          saida_rna2, ...
                                                          nco( 2, : ), ...
                                                          min, ...
                                                          max, ...
                                                          normalizar );
            tempo_treinamento_rna2 = toc;

            % Salvando o ambiente
%             % Nome com camada oculta
%             nome_arq = strcat( 'P2O', num2str( regressores ), ...
%                                'T', num2str( r ), ...
%                                'CO', num2str( size( nco, 2 ) ) );
% 
%             for i = 1 : size( nco, 2 )
%                 nome_arq = strcat( nome_arq, '_', num2str( nco( 1, i ) ) );
%             end
% 
%             nome_arq = strcat( nome_arq, '_-' );
% 
%             for i = 1 : size( nco, 2 )
%                 nome_arq = strcat( nome_arq, '_', num2str( nco( 2, i ) ) );
%             end
            
            % Nome sem camada oculta
            nome_arq = strcat( 'P1O', num2str( ordem ), ...
                               'N', num2str( nco(1) ), ...
                               'N', num2str( nco(2) ), ...
                               'T', num2str( r ) );

            save( nome_arq, 'rede_1', 'rede_2', ...
                            'lim_ent_rna1', 'lim_ent_rna1', ...
                            'lim_sai_rna1', 'lim_sai_rna2', ...
                            'tempo_treinamento_rna1', ...
                            'tempo_treinamento_rna2' );

            % Salvando as informacoes da rede
            salvar_rede( rede_1, strcat( nome_arq, '_R1' ) );
            salvar_rede( rede_2, strcat( nome_arq, '_R2' ) );
        end
    end
end