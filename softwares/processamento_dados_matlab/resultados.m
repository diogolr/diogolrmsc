clear;
clc;
close all;

% falha = input( 'Sigla da falha: ' );
falhas = { 'FSeDG' 'FSeDO' 'FSeSR' 'FSeQ' 'FADG' 'FADO' 'FASR' 'FAVK' ...
           'FAQ' 'FSiVzT' 'FSiVrOS' 'FSiVrGMP' 'FSiEOS' };

% saida_tikz = input( 'Gerar saida para tikz-timing [0 ou 1]: ' );
saida_tikz = 1;

% saida_gnuplot = input( 'Gerar saida para gnuplot [0 ou 1]: ' );
saida_gnuplot = 1;

% Analisar arquivos de validacao
validacao = 0;

% Melhores redes para as deteccoes de falha
% Cada linha contem [ORDEM NCO TREINAMENTO]
melhores = [ 4 28 2 ; ... % FSeDG
             4 28 5 ; ... % FSeDO
             4 20 3 ; ... % FSeSR
             4 20 4 ; ... % FSeQ
             2  8 3 ; ... % FADG
             4 28 3 ; ... % FADO
             2  8 6 ; ... % FASR
             2  8 5 ; ... % FAVK
             4 28 6 ; ... % FAQ
             4 24 1 ; ... % FSiVzT
             2  8 3 ; ... % FSiVrOS
             4 20 2 ; ... % FSiVrGMP
             2 12 4 ; ... % FSiEOS
           ];

for i = 1 : length( falhas )
    disp( strcat( falhas{i}, ' .................' ) );
    
    falha = falhas{i};
    
    % pasta_valid = input( 'Pasta dos arquivos de validacao: ' );
    pasta_saidas = strcat( '../simulacao_tanques_cpp/saidas/', ...
                           falha, '/' );

    % pasta_rnas = input( 'Pasta dos arquivos das redes treinadas: ' );
    pasta_rnas = strcat( '../../dados/deteccao/', falha, '/' );
    
    % Determinando o nome do arquivo de configuracao da melhor rede
    nome_arq_sem_ext = strcat( pasta_rnas, ...
                               'O', num2str( melhores( i, 1 ) ), '/', ...
                               'N', num2str( melhores( i, 2 ) ), '/', ...
                               'O', num2str( melhores( i, 1 ) ), ...
                               'N', num2str( melhores( i, 2 ) ), ...
                               'T', num2str( melhores( i, 3 ) ) );

    arq_config_detec = strcat( nome_arq_sem_ext, '.mat' );

    load( arq_config_detec );
    
    rede_detec = rede;
    
    % Numero de regressores = ORDEM - 1
    ordem = melhores( i, 1 );
    
    regressores = ordem - 1;
    
    % Lendo o arquivo de validacao ------------------------------------
    arq_niveis = strcat( pasta_saidas, 'niveis_qualif.dat' );
    arq_erro_sc = strcat( pasta_saidas, 'erro_sc_qualif.dat' );

    [entrada saida residuos] = config_ent_sai_valid( arq_niveis, ...
                                                     arq_erro_sc, ...
                                                     regressores, ...
                                                     validacao );

    % Determinando o nome do arquivo final e chamando o metodo para 
    % gerar a saida
    nome_final = strcat( '../../dados/qualificacao/', falhas{i} );
    
    % Salvando os residuos
    dlmwrite( strcat( nome_final, '_residuos.dat' ), ...
              [(0.1:0.1:size(entrada,2)*0.1)' residuos'], ...
              'delimiter', '\t' );

    % Capturando as palavras de saida geradas para o tikz
    palavras = deteccao_tikz( rede_detec, entrada, saida, ...
                              nome_final, saida_tikz, validacao );

    % Determinando as sequencias de deteccao
    sequencias = { contar_sequencia( find( palavras{1,1} == 'H' ) ) ...
                   contar_sequencia( find( palavras{1,2} == 'H' ) );...
                   contar_sequencia( find( palavras{2,1} == 'H' ) ) ...
                   contar_sequencia( find( palavras{2,2} == 'H' ) ) };
               
    % Eliminando as deteccoes que nao formem uma sequencia, como por
    % exemplo aquelas em que foi detectada a falha no somente no instante X
    % e não em X ate X + kT, onde T e o periodo de amostragem. Isso somente
    % se aplica as deteccoes das redes neurais, pois as saidas desejadas
    % passadas como parametro do metodo deteccao_tikz sao sempre em
    % sequencia
     
    linhas_com_colunas_identicas = sequencias{1,2}( :, 1 ) - ...
                                   sequencias{1,2}( :, 2 ) == 0;
                                      
    sequencias{1,2}( linhas_com_colunas_identicas, : ) = [];
    
    linhas_com_colunas_identicas = sequencias{2,2}( :, 1 ) - ...
                                   sequencias{2,2}( :, 2 ) == 0;
                                      
    sequencias{2,2}( linhas_com_colunas_identicas, : ) = [];
    
    % Escrevendo a saida para o gnuplot
    if saida_gnuplot
        % Multiplica-se pelo periodo de amostragem para se ter os instantes
        % corretos em que as falhas foram detectadas
        matriz_saida_t1 = [ sequencias{1,2}*0.1 ];
        matriz_saida_t2 = [ sequencias{2,2}*0.1 ];
        
        deteccao_gnuplot( strcat( nome_final, '_t1.gp' ), matriz_saida_t1, 1 );
        deteccao_gnuplot( strcat( nome_final, '_t2.gp' ), matriz_saida_t2, 2 );
    end

    % Limpando as variaveis de ambiente carregadas com o load
    clear rede rede_detec arq_config lim_ent lim_sai tempo_treinamento ...
          linhas_com_colunas_identicas;
end