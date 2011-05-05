clear;
clc;

falhas = { 'FSeDG' 'FSeDO' 'FSeSR' 'FSeQ' 'FADG' 'FADO' 'FASR' 'FAVK' ...
           'FAQ' 'FSiVzT' 'FSiVrOS' 'FSiVrGMP' 'FSiEOS' };
       
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
    
    % pasta_rnas = input( 'Pasta dos arquivos das redes treinadas: ' );
    pasta_rnas = strcat( falhas{i}, '/' );
    
    % Determinando o nome do arquivo de configuracao da melhor rede
    nome_arq_sem_ext = strcat( pasta_rnas, ...
                               'O', num2str( melhores( i, 1 ) ), ...
                               'N', num2str( melhores( i, 2 ) ), ...
                               'T', num2str( melhores( i, 3 ) ) );

    arq_config_detec = strcat( nome_arq_sem_ext, '.mat' );
    
    load( arq_config_detec );
    
    x_min = rede.inputs{1}.processSettings{3}.xmin;
    x_max = rede.inputs{1}.processSettings{3}.xmax;
    x_range = x_max - x_min;

    y_min = rede.outputs{2}.processSettings{2}.xmin;
    y_max = rede.outputs{2}.processSettings{2}.xmax;
    y_range = y_max - y_min;

    matriz = cell( 2, 1 );
    matriz{1} = [x_min x_max x_range];
    matriz{2} = [y_min y_max y_range];
    
    arq_saida = strcat( nome_arq_sem_ext, '_RNA.lim' );
    
    dlmwrite( arq_saida, matriz, 'delimiter', '\t', 'precision', '%.6f' );
    
    clear x_min x_max x_range y_min y_max y_range matriz;
end

