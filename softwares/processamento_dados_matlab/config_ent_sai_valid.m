% Funcao para configuracao da entrada e saida dos resultados/validacoes

function [entrada saida erros] = config_ent_sai_valid( arq_niveis, ...
                                                       arq_erro_sc, ...
                                                       regressores, ...
                                                       validacao )       
    mat_niveis = dlmread( arq_niveis, '\t' );
    mat_erro_sc = dlmread( arq_erro_sc, '\t' );

    % Ajustanto os dados das matrizes de entrada e saida -----------------------
    [entrada saida] = ajustar_dados( mat_niveis, mat_erro_sc );

    clear mat_niveis;
    clear mat_erro_sc;

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

    % Dentro do tempo total, sera simulado um periodo sem falha, um periodo com
    % falha somente no tanque 1, um periodo com falha somente no tanque 2 e um
    % ultimo periodo com falha nos dois tanques. Por esse motivo o numero de
    % amostras e dividido por quatro.
    %
    % EXEMPLO
    % No primeiro intervalo nao sera simulada nenhuma falha, logo so ha
    % necessidade de modificar os intervalos 2 a 4. Portanto, para um caso com
    % 12000 amstras as linhas a serem modificadas serao da 3001 ate a 6000, da
    % 6001 ate a 9000 e da 9001 ate a 12000
    
    % Para validacao ===========================================================
    if validacao
        div = num_amostras / 4;
        linhas = [div+1 2*div; 2*div+1 3*div; 3*div+1 4*div];
    % Para os resultados da qualificacao =======================================
    else
        div = num_amostras / 7;
        linhas = [ div+1 2*div ; 3*div+1 4*div ; 5*div+1 6*div ];
    end
        
    % Falha T1
    saida( linhas( 1, 1 ) : linhas( 1, 2 ), 1 ) = -1.0;
    % Falha T2
    saida( linhas( 2, 1 ) : linhas( 2, 2 ), 2 ) = 1.0;
    % Falha T1 + Falha T2
    saida( linhas( 3, 1 ) : linhas( 3, 2 ), 1 ) = -1.0;
    saida( linhas( 3, 1 ) : linhas( 3, 2 ), 2 ) = 1.0;

    saida = saida';
end
