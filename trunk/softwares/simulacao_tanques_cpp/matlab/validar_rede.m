function [saida_rna EMQ ERROS EST] = validar_rede( rede, ...
                                                   entrada, ...
                                                   saida_desejada, ...
                                                   nome_arq, ...
                                                   proposta )
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
    
    if ( proposta == 1 )
        emq_tot = sum( emq_var, 1 );
    end

    % Matrizes de saida a serem salvas
    % Erros medios quadraticos
    if proposta == 1
        EMQ = [emq_var; emq_tot];
    else
        EMQ = emq_var;
    end
    
    % Erros absoluto e percentual
    ERROS = [erro_abs' erro_perc'];
    % Dados estatisticos
    EST = [medias desvio'];
    
    % Gerando o vetor de tempo para ser inserido na primeira coluna
    tempo = 1 : num_amostras;
    tempo = tempo * 0.1;
    
    % Salvando os dados de validacao   
    dlmwrite( strcat( nome_arq, '_SAIDA.dat' ), [tempo' saida_rna'], ...
              'delimiter', '\t', 'precision', '%.12f' );
    dlmwrite( strcat( nome_arq, '_EMQ.dat' ), EMQ, ...
              'delimiter', '\t', 'precision', '%.12f' );
    dlmwrite( strcat( nome_arq, '_ERROS.dat' ), [tempo' ERROS], ...
              'delimiter', '\t', 'precision', '%.12f' );
    dlmwrite( strcat( nome_arq, '_ESTATISTICA.dat' ), EST, ...
              'delimiter', '\t', 'precision', '%.12f' );
end