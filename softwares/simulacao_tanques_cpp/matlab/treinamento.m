% Arquivo de Treinamento das RNAs que detectam as falhas

clear;
clc;

% arq_niveis = input( 'Niveis [treinamento]: ' );
arq_niveis = 'D:\documentos\Diogo\diogolrmsc@ggc\softwares\simulacao_tanques_cpp\saidas\10_min_normal\niveis_treinamento.dat';

% arq_erro_sc = input( 'Erro e Sinal de controle [treinamento]: ' );
arq_erro_sc= 'D:\documentos\Diogo\diogolrmsc@ggc\softwares\simulacao_tanques_cpp\saidas\10_min_normal\erro_sc_treinamento.dat';

mat_niveis = dlmread( arq_niveis, '\t' );
mat_erro_sc = dlmread( arq_erro_sc, '\t' );

% Ordem das curvas - Grafico de Niveis ------------------------------------
% [2] - Referencia T1
% [3] - Referencia T2
% [4] - Nivel T1
% [5] - Nivel T2
% Ordem das curvas - Grafico de Erro/Sinal de Controle --------------------
% [2] - Erro T1
% [3] - Erro T2
% [4] - Sinal de controle T1
% [5] - Sinal de controle T2
% [6] - Acao Proporcional T1
% [7] - Acao Proporcional T2
% [8] - Acao Integral T1
% [9] - Acao Integral T2
% [10] - Acao Derivativa T1
% [11] - Acao Derivativa T2

% Matriz de entrada sem regressores ---------------------------------------
mat_dados = [mat_niveis( :, 4 ) mat_niveis( :, 5 ) ...
             mat_erro_sc( :, 2 ) mat_erro_sc( :, 3 ) ...
             mat_erro_sc( :, 4 ) mat_erro_sc( :, 5 ) ...
             mat_erro_sc( :, 6 ) mat_erro_sc( :, 7 ) ...
             mat_erro_sc( :, 8 ) mat_erro_sc( :, 9 ) ...
             mat_erro_sc( :, 10 ) mat_erro_sc( :, 11 ) ];
         
% Determinando se sera treinada uma RNA para identificacao do sistema ou
% para deteccao de falha --------------------------------------------------
falha = input( 'RNA para deteccao de falha [0 ou 1]: ' );

% =========================================================================
% Treinamento para deteccao de FALHA
% =========================================================================
if falha
    % Determinacao da entrada da rede -------------------------------------
    entrada = mat_dados;

    % A matriz de entrada nao podera conter o nivel atual dos tanques. 
    % Assim, sera considerado somente o nivel anterior.
    entrada = adicionar_regressores( entrada, [1 1 0 0 0 0 0 0 0 0 0 0] );
    entrada( :, [ 1 3 ] ) = [];

    nomes_colunas = { 'L1(k-1)' 'L2(k-1)' 'E1(k-1)' 'E2(k-1)' ...
                      'Vp1(k)' 'Vp2(k)' 'AcaoP1(k)' 'AcaoP2(k)' ...
                      'AcaoI1(k)' 'AcaoI2(k)' 'AcaoD1(k)' 'AcaoD2(k)' };

    indices_mantidos = 1 : length( nomes_colunas );

    disp( 'Colunas existentes: ' );

    for i = 1 : length( indices_mantidos )
        disp( strcat( '[', num2str( i ), '] ', ...
                      nomes_colunas{ indices_mantidos( i ) } ) );
    end

    disp( ' ' );

    % Determinando as colunas a serem excluidas ...........................
    colunas = input( 'Colunas a serem excluidas [vetor]: ' );
    
    % Determinacao da saida da rede ---------------------------------------
    
    % Determinacao dos parametros de treinamento --------------------------

    % Treinamento ---------------------------------------------------------

    % Salvar redes --------------------------------------------------------

    
    
% =========================================================================
% Treinamento para IDENTIFICACAO do modelo
% =========================================================================
else
    % Determinando a proposta para adequar a saida da rede
    disp( 'Propostas: ');
    disp( '[1] RNA para identificacao global' );
    disp( '[2] RNA para identificacao individual ou em separado' );
    disp( ' ' );
    
    proposta = input( 'Proposta:' );
    
    % Determinacao da entrada/saida da rede -------------------------------
    entrada = mat_dados;

    % A matriz de entrada nao podera conter o nivel atual dos tanques. 
    % Assim, sera considerado somente o nivel anterior.
    entrada = adicionar_regressores( entrada, [1 1 0 0 0 0 0 0 0 0 0 0] );
    entrada( :, [ 1 3 ] ) = [];

    nomes_colunas = { 'L1(k-1)' 'L2(k-1)' 'E1(k-1)' 'E2(k-1)' ...
                      'Vp1(k)' 'Vp2(k)' 'AcaoP1(k)' 'AcaoP2(k)' ...
                      'AcaoI1(k)' 'AcaoI2(k)' 'AcaoD1(k)' 'AcaoD2(k)' };

    indices_mantidos = 1 : length( nomes_colunas );

    disp( 'Colunas existentes: ' );

    for i = 1 : length( indices_mantidos )
        disp( strcat( '[', num2str( i ), '] ', ...
                      nomes_colunas{ indices_mantidos( i ) } ) );
    end

    disp( ' ' );

    % Proposta 1
    if proposta == 1
        % Excluindo as colunas que nao serao utilizadas
        entrada( :, [3:4 7:12] ) = [];
        indices_mantidos( [3:4 7:12] ) = [];
        
        % Exibindo as colunas que restaram
        disp( ' ' );
        
        disp( 'Colunas existentes: ' );
        
        for i = 1 : length( indices_mantidos )
            disp( strcat( '[', num2str( i ), '] ', ...
                          nomes_colunas{ indices_mantidos( i ) } ) );
        end
        
        disp( ' ' );
        
        % Determinando o numero de regressores por coluna
        regressores = input( 'Regressores [vetor]: ' );
        
        entrada = adicionar_regressores( entrada, regressores );
        
        % Transpondo a entrada para adequar a entrada da RNA
        entrada = entrada';
        
        % Determinando a saida
        saida = mat_dados( :, 1:2 );
        
        % Transpondo a saida para adequar a entrada da RNA
        saida = saida';
        
    % Proposta 2    
    else
        % Excluindo as colunas que nao serao utilizadas
        entrada_rna1 = entrada;
        entrada_rna2 = entrada;
        indices_mantidos_rna1 = indices_mantidos;
        indices_mantidos_rna2 = indices_mantidos;
        
        clear entrada;
        clear indices_mantidos;
        
        entrada_rna1( :, [2:4 6:12] ) = [];
        indices_mantidos_rna1( [2:4 6:12] ) = [];
        
        entrada_rna2( :, [3:4 7:12] ) = [];
        indices_mantidos_rna2( [3:4 7:12] ) = [];
       
        % Exibindo as colunas que restaram
        disp( ' ' );
        
        disp( 'Colunas existentes RNA 1: ' );
        
        for i = 1 : length( indices_mantidos_rna1 )
            disp( strcat( '[', num2str( i ), '] ', ...
                          nomes_colunas{ indices_mantidos_rna1( i ) } ) );
        end
        
        disp( ' ' );
        
        % Exibindo as colunas que restaram
        disp( ' ' );
        
        disp( 'Colunas existentes RNA 2: ' );
        
        for i = 1 : length( indices_mantidos_rna1 )
            disp( strcat( '[', num2str( i ), '] ', ...
                          nomes_colunas{ indices_mantidos_rna2( i ) } ) );
        end
        
        disp( ' ' );
        
        % Determinando o numero de regressores por coluna para cada
        % proposta
        regressores_rna1 = input( 'Regressores RNA 1 [vetor]: ' );
        regressores_rna2 = input( 'Regressores RNA 2 [vetor]: ' );
        
        entrada_rna1 = adicionar_regressores( entrada_rna1, ...
                                              regressores_rna1 );
        entrada_rna2 = adicionar_regressores( entrada_rna2, ...
                                              regressores_rna2 );
        
        % Transpondo as entradas para adequar as entradas das RNAs
        entrada_rna1 = entrada_rna1';
        entrada_rna2 = entrada_rna2';
        
        % Determinando as saidas
        saida_rna1 = mat_dados( :, 1 );
        saida_rna2 = mat_dados( :, 2 );
        
        % Trasnpondo as saidas para se adequarem as saidas das RNAs
        saida_rna1 = saida_rna1';
        saida_rna2 = saida_rna2';
    end
    
    % Determinacao dos parametros de treinamento --------------------------
    % normalizar = input( 'Normalizar dados [0 ou 1]: ' );
    normalizar = 0;

    % if normalizar == 1
    %     min = input( 'Valor minimo: ' );
    %     max = input( 'Valor maximo: ' );
    % end

    min = -1;
    max = 1;
    
    % Treinamento ---------------------------------------------------------
    
    % Salvar redes --------------------------------------------------------
end