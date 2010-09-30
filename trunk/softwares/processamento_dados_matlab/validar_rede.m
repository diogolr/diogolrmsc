function [saida_rna saida_2 saida_3 saida_4] = validar_rede( rede, ...
                                                             entrada, ...
                                                             saida_desejada, ...
                                                             nome_arq, ...
                                                             proposta )
	saida_rna = sim( rede, entrada );

    [num_var num_amostras] = size( saida_rna );

    % Gerando o vetor de tempo para ser inserido na primeira coluna
    tempo = 1 : num_amostras;
    tempo = tempo * 0.1;

    % Salvando os dados de validacao   
    dlmwrite( strcat( nome_arq, '_SAIDA.dat' ), ...
              [tempo' saida_rna'], ...
              'delimiter', '\t', 'precision', '%.12f' );

	% Validacao das deteccoes de falha
	if proposta == -1
        acertos_t1 = 0;
        acertos_t2 = 0;

        falhas_tipo_1_t1 = 0;
        falhas_tipo_1_t2 = 0;
        falhas_tipo_2_t1 = 0;
        falhas_tipo_2_t2 = 0;

        palavra_t1_desej = '';
        palavra_t2_desej = '';
        palavra_t1_rna = '';
        palavra_t2_rna = '';
        
        for j = 1 : num_amostras
            % Tanque 1 ----------------------------------------------------
            % Acerto - Nao ha falha
            if saida_desejada( 1, j ) == 0 && saida_rna( 1, j ) >= -0.5
                acertos_t1 = acertos_t1 + 1;
                palavra_t1_desej = strcat( palavra_t1_desej, 'L' );
                palavra_t1_rna = strcat( palavra_t1_rna, 'L' );
            % Acerto - Ha falha no tanque 1
            elseif saida_desejada( 1, j ) == -1 && saida_rna( 1, j ) < -0.5
                acertos_t1 = acertos_t1 + 1;
                palavra_t1_desej = strcat( palavra_t1_desej, 'H' );
                palavra_t1_rna = strcat( palavra_t1_rna, 'H' );
            % Erro - Falso positivo (detectou falha quando nao ha)
            elseif saida_desejada( 1, j ) == 0 && saida_rna( 1, j ) < -0.5
                falhas_tipo_1_t1 = falhas_tipo_1_t1 + 1;
                palavra_t1_desej = strcat( palavra_t1_desej, 'L' );
                palavra_t1_rna = strcat( palavra_t1_rna, 'H' );
            % Erro - Falso negativo (nao detectou falha quando deveria ter 
            % detectado)
            elseif saida_desejada( 1, j ) == -1 && saida_rna( 1, j ) >= -0.5
                falhas_tipo_2_t1 = falhas_tipo_2_t1 + 1;
                palavra_t1_desej = strcat( palavra_t1_desej, 'H' );
                palavra_t1_rna = strcat( palavra_t1_rna, 'L' );
            end

            % Tanque 2 ----------------------------------------------------
            % Acerto - Nao ha falha
            if saida_desejada( 2, j ) == 0 && saida_rna( 2, j ) <= 0.5
                acertos_t2 = acertos_t2 + 1;
                palavra_t2_desej = strcat( palavra_t2_desej, 'L' );
                palavra_t2_rna = strcat( palavra_t2_rna, 'L' );
            % Acerto - Ha falha no tanque 2
            elseif saida_desejada( 2, j ) == 1 && saida_rna( 2, j ) > 0.5
                acertos_t2 = acertos_t2 + 1;
                palavra_t2_desej = strcat( palavra_t2_desej, 'H' );
                palavra_t2_rna = strcat( palavra_t2_rna, 'H' );
            % Erro - Falso positivo (detectou falha quando nao ha)
            elseif saida_desejada( 2, j ) == 0 && saida_rna( 2, j ) > 0.5
                falhas_tipo_1_t2 = falhas_tipo_1_t2 + 1;
                palavra_t2_desej = strcat( palavra_t2_desej, 'L' );
                palavra_t2_rna = strcat( palavra_t2_rna, 'H' );
            % Erro - Falso negativo (nao detectou falha quando deveria ter 
            % detectado)
            elseif saida_desejada( 2, j ) == 1 && saida_rna( 2, j ) <= 0.5
                falhas_tipo_2_t2 = falhas_tipo_2_t2 + 1;
                palavra_t2_desej = strcat( palavra_t2_desej, 'H' );
                palavra_t2_rna = strcat( palavra_t2_rna, 'L' );
            end
        end
        
        % Matriz de saida
        matriz = [acertos_t1 falhas_tipo_1_t1 falhas_tipo_2_t1 ; ...
                  acertos_t2 falhas_tipo_1_t2 falhas_tipo_2_t2 ];
              
        saida_2 = matriz;

        dlmwrite( strcat( nome_arq, '_RESULT.dat' ), matriz, ...
                  'delimiter', '\t', 'precision', '%.12f' );
        
        % Saida para tikz-timing - Tanque 1
        fid_t1 = fopen( strcat( nome_arq, '_t1.tex' ), 'w' );

        fprintf( fid_t1, 'Desejado T1 & %s\\\n Obtido T1 & %s', ...
                 palavra_t1_desej, palavra_t1_rna );

        fclose( fid_t1 );

        % Saida para tikz-timing - Tanque 2
        fid_t2 = fopen( strcat( nome_arq, '_t2.tex' ), 'w' );

        fprintf( fid_t2, 'Desejado T1 & %s\\\n Obtido T1 & %s', ...
                 palavra_t2_desej, palavra_t2_rna );

        fclose( fid_t2 );

	% Validacao da identificacao (proposta == 1 ou proposta == 2)
    else
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
        
        saida_2 = EMQ;
        saida_3 = ERROS;
        saida_4 = EST;

        dlmwrite( strcat( nome_arq, '_EMQ.dat' ), EMQ, ...
                  'delimiter', '\t', 'precision', '%.12f' );
        dlmwrite( strcat( nome_arq, '_ERROS.dat' ), [tempo' ERROS], ...
                  'delimiter', '\t', 'precision', '%.12f' );
        dlmwrite( strcat( nome_arq, '_ESTATISTICA.dat' ), EST, ...
                  'delimiter', '\t', 'precision', '%.12f' );
    end   

end
