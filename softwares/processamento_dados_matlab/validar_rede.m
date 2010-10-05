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

        for j = 1 : num_amostras
            % Tanque 1 ----------------------------------------------------
            % Acerto - Nao ha falha
            if saida_desejada( 1, j ) == 0 && saida_rna( 1, j ) >= -0.5
                acertos_t1 = acertos_t1 + 1;
            % Acerto - Ha falha no tanque 1
            elseif saida_desejada( 1, j ) == -1 && saida_rna( 1, j ) < -0.5
                acertos_t1 = acertos_t1 + 1;
            % Erro - Falso positivo (detectou falha quando nao ha)
            elseif saida_desejada( 1, j ) == 0 && saida_rna( 1, j ) < -0.5
                falhas_tipo_1_t1 = falhas_tipo_1_t1 + 1;
            % Erro - Falso negativo (nao detectou falha quando deveria ter 
            % detectado)
            elseif saida_desejada( 1, j ) == -1 && saida_rna( 1, j ) >= -0.5
                falhas_tipo_2_t1 = falhas_tipo_2_t1 + 1;
            end

            % Tanque 2 ----------------------------------------------------
            % Acerto - Nao ha falha
            if saida_desejada( 2, j ) == 0 && saida_rna( 2, j ) <= 0.5
                acertos_t2 = acertos_t2 + 1;
            % Acerto - Ha falha no tanque 2
            elseif saida_desejada( 2, j ) == 1 && saida_rna( 2, j ) > 0.5
                acertos_t2 = acertos_t2 + 1;
            % Erro - Falso positivo (detectou falha quando nao ha)
            elseif saida_desejada( 2, j ) == 0 && saida_rna( 2, j ) > 0.5
                falhas_tipo_1_t2 = falhas_tipo_1_t2 + 1;
            % Erro - Falso negativo (nao detectou falha quando deveria ter 
            % detectado)
            elseif saida_desejada( 2, j ) == 1 && saida_rna( 2, j ) <= 0.5
                falhas_tipo_2_t2 = falhas_tipo_2_t2 + 1;
            end
        end
        
        % Matriz de saida
        matriz = [acertos_t1 falhas_tipo_1_t1 falhas_tipo_2_t1 ; ...
                  acertos_t2 falhas_tipo_1_t2 falhas_tipo_2_t2 ];
              
        saida_2 = matriz;

        dlmwrite( strcat( nome_arq, '_RESULT.dat' ), matriz, ...
                  'delimiter', '\t', 'precision', '%.12f' );

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
