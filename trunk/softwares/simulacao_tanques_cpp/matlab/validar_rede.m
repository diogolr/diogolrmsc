function [emq] = validar_rede( rede, entrada, saida_desejada, nome_arq )
    % "Executando" a RNA treinada
    saida_rna = sim( rede, entrada );
    
    % Dimensoes
    [num_var num_amostras] = size( saida_rna );
    
    % Erro absoluto
    erro_abs = saida_desejada - saida_rna;
    
    % Erro percentual
    erro_perc = (erro_abs ./ saida_rna ) * 100;
    
    % Minimo e maximo erro percentual
    erro_min_max = minmax( erro_perc );
    
    % Media do erro percentual
    medias = mean( erro_perc, 2 );
    
    % Desvio padrao do erro percentual
    desvio = std( erro_perc' );
    
    % EMQ por variavel e EMQ total
    emq_var = sum( ( erro_abs .^ 2 ) / 2, 2 ) / num_amostras;
    emq_tot = sum( emq_var, 1 );

    % Matrizes de saida a serem salvas
    EMQ = [emq_var; emq_tot];
    ERROS = [erro_abs' erro_perc'];
    ESTATISTICA = [medias desvio'];
    
    % Salvando os dados de validacao
    dlmwrite( strcat( nome_arq, '_EMQ.dat' ), EMQ, ...
              'delimiter', '\t', 'precision', '%.12f' );
    dlmwrite( strcat( nome_arq, '_ERROS.dat' ), ERROS, ...
              'delimiter', '\t', 'precision', '%.12f' );
    dlmwrite( strcat( nome_arq, '_ESTATISTICA.dat' ), ESTATISTICA, ...
              'delimiter', '\t', 'precision', '%.12f' );
end