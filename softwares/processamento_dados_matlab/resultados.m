clear;
clc;
close all;

% falha = input( 'Sigla da falha: ' );
falhas = { 'FSeDG' 'FSeDO' 'FSeSR' 'FSeQ' 'FADG' 'FADO' 'FASR' 'FAVK' ...
           'FAQ' 'FSiVzT' 'FSiVrOS' 'FSiVrGMP' 'FSiEOS' };

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
    disp( falhas{i} );
    
    falha = falhas{i};
    
    % pasta_valid = input( 'Pasta dos arquivos de validacao: ' );
    pasta_valid = strcat( '../simulacao_tanques_cpp/saidas/', falha, '/' );

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
    
    % Validacoes
    for v = 1 : 3
        disp( strcat( 'Validacao ', num2str( v ), '...........' ) );
        
        % Lendo o arquivo de validacao ------------------------------------
        arq_niveis = strcat( pasta_saidas, 'niveis_qual.dat' );
        arq_erro_sc = strcat( pasta_saidas, 'erro_sc_qual.dat' );

        [entrada saida residuos] = config_ent_sai_valid( arq_niveis, ...
                                                         arq_erro_sc,
                                                         regressores );
        
        % Determinando o nome do arquivo final e chamando o metodo para 
        % gerar a saida
        nome_final = strcat( nome_arq_sem_ext, '_v', num2str( v ) );
        
        gerar_saida_tikz( rede_detec, entrada, saida, nome_final );
    end
      
    % Limpando as variaveis de ambiente carregadas com o load
    clear rede rede_detec arq_config lim_ent lim_sai tempo_treinamento;
end