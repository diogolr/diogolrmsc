% Arquivo de Treinamento das RNAs de deteccao - Proposta 1
clear;
clc;
close all;

ORDEM = input( 'Ordem: ' );

% -------------------------------------------------------------------------
% Falhas a serem treinadas
% -------------------------------------------------------------------------
falhas = { 'FSeDG' 'FSeDO' 'FSeSR' 'FSeQ' ...
           'FADG' 'FADO' 'FASR' 'FAVK' 'FAQ' ...
           'FSiVzT' 'FSiVrOS' 'FSiVrGMP' 'FSiEOS' ...
           'FSeSR+FAQ' 'FAVK+FSiEOS' };

% Determinando as palavras de saida da rede
num_bits = 6; % inicialmente serao 64 palavras

bits = zeros( num_bits, 1 );
palavra_saida = cell( 1, 2^num_bits );

for i = 0 : 2^num_bits - 1
    bin = dec2bin( i, num_bits );
    
    for n = 1 : num_bits
        if bin(n) == '0'
            bits(n) = -1.0;
        elseif bin(n) == '1'
            bits(n) = 1.0;
        end
    end
    
    palavra_saida{i+1} = bits;
end

% Configuracao dos arquivos a serem lidos e inicio do treinamento
for i = 1 : length( falhas )
    disp( strcat( falhas{i}, ' .................' ) );
    
    arq_niveis = strcat( '..\simulacao_tanques_cpp\saidas\', ...
                         falhas{i}, '\niveis_tr.dat' );
                     
    arq_erro_sc = strcat( '..\simulacao_tanques_cpp\saidas\', ...
                          falhas{i}, '\erro_sc_tr.dat' );

	% Pasta em que as RNAs serao salvas
	pasta_rnas = strcat( '..\..\dados\deteccao\', falhas{i}, '\' );
    
    % Carregando as matrizes
    mat_niveis = dlmread( arq_niveis, '\t' );
    mat_erro_sc = dlmread( arq_erro_sc, '\t' );
    
    num_amostras = size( mat_niveis, 1 );
    
    % Matrizes de entrada e saida
    [entrada_pad saida_pad] = ajustar_dados( mat_niveis, mat_erro_sc );
    
    clear mat_niveis mat_erro_sc;
    
    % ---------------------------------------------------------------------
    % Parametros de treinamento
    % ---------------------------------------------------------------------
    % Numero de redes a serem treinadas
    n_treinamentos = 6;

    % Numero de regressores de acordo com a ordem do modelo
    ordem = ORDEM;

    regressores = ordem - 1;

    ncos_deteccao = [ 8 12 16 ; 14 18 22 ; 20 24 28 ];

    % normalizar = input( 'Normalizar dados [0 ou 1]: ' );
    normalizar = 0;

    % Tolerancia
    tol = 1e-3;

    % if normalizar == 1
    %     min = input( 'Valor minimo: ' );
    %     max = input( 'Valor maximo: ' );
    % end

    min = -1;
    max = 1;
    
    % Numero de neuronios da camada oculta
    nco = ncos_deteccao( ordem - 1, : );
    
    % ---------------------------------------------------------------------
    % Estabelecimento da entrada e da saida
    % ---------------------------------------------------------------------
    entrada = entrada_pad;
    saida = saida_pad;
   
    % Eliminando as colunas para ficar somente com L1(k-1), L2(k-1), Vp1(k)
    % e Vp2(k)
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
    
    % ---------------------------------------------------------------------
    % Combinacao de falhas
    % ---------------------------------------------------------------------
    if i == 14
        saida = [ palavra_saida{1} ...  % SF
                  palavra_saida{42} ... % F1 + F1
                  palavra_saida{43} ... % F1 + F2
                  palavra_saida{44} ... % F2 + F1
                  palavra_saida{45} ];  % F2 + F2
    elseif i == 15
        saida = [ palavra_saida{1} ...  % SF
                  palavra_saida{46} ... % F1 + F1
                  palavra_saida{47} ... % F1 + F2
                  palavra_saida{48} ... % F2 + F1
                  palavra_saida{49} ];  % F2 + F2        
    % ---------------------------------------------------------------------
    % Falhas individuais
    % ---------------------------------------------------------------------
    else
        saida = [ palavra_saida{1} ...           % Sem Falha
                  palavra_saida{(i-1)*4 + 2} ... % F1
                  palavra_saida{(i-1)*4 + 3} ... % F2
                  palavra_saida{(i-1)*4 + 4} ];  % F1 + F2      
    end
    
    % Determinacao da saida
    saida = repetir_colunas( saida, 2999 );
    
    % ---------------------------------------------------------------------
    % Treinamento
    % ---------------------------------------------------------------------
    % Determinacao do numero de redes neurais a serem treinadas de acordo
    % com o vetor da camada oculta
    num_redes = length( nco );
    
    for r = 1 : num_redes
        disp( ' ' );
        disp( strcat( 'Rede N', num2str( nco( r ) ), ' ==============' ) );

        subpasta = strcat( 'O', num2str( ordem ), '\', ...
                           'N', num2str( nco( r ) ), '\' );
                       
        for t = 1 : n_treinamentos
            disp( ' ' );
            disp( strcat( 'Treinamento ', num2str( t ), ' ...' ) );
            
            % Treinando a RNA
            tic
            [rede lim_ent lim_sai] = treinar_rna( entrada, ...
                                                  saida, ...
                                                  nco( r ), ...
                                                  min, max, ...
                                                  normalizar, tol );

            tempo_treinamento = toc;

            % Salvando o ambiente
            nome_arq = strcat( pasta_rnas, subpasta, ...
                               'P1_', ...
                               'O', num2str( ordem ), ...
                               'N', num2str( nco( r ) ), ...
                               'T', num2str( t ) );

            save( nome_arq, 'rede', 'lim_ent', 'lim_sai', ...
                            'tempo_treinamento' );

            % Salvando as informacoes da rede
            salvar_rede( rede, strcat( nome_arq, '_RNA' ) );
        end
    end
end