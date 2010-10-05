function gerar_saida_tikz ( rede, entrada, saida_desejada, nome_arq )

	saida_rna = sim( rede, entrada );

    [num_var num_amostras] = size( saida_rna );

	palavra_t1_desej = '';
    palavra_t2_desej = '';
    palavra_t1_rna = '';
    palavra_t2_rna = '';

    for j = 1 : num_amostras
        % Tanque 1 ----------------------------------------------------
        % Acerto - Nao ha falha
        if saida_desejada( 1, j ) == 0 && saida_rna( 1, j ) >= -0.5
            palavra_t1_desej = strcat( palavra_t1_desej, 'L' );
            palavra_t1_rna = strcat( palavra_t1_rna, 'L' );
        % Acerto - Ha falha no tanque 1
        elseif saida_desejada( 1, j ) == -1 && saida_rna( 1, j ) < -0.5
            palavra_t1_desej = strcat( palavra_t1_desej, 'H' );
            palavra_t1_rna = strcat( palavra_t1_rna, 'H' );
        % Erro - Falso positivo (detectou falha quando nao ha)
        elseif saida_desejada( 1, j ) == 0 && saida_rna( 1, j ) < -0.5
            palavra_t1_desej = strcat( palavra_t1_desej, 'L' );
            palavra_t1_rna = strcat( palavra_t1_rna, 'H' );
        % Erro - Falso negativo (nao detectou falha quando deveria ter 
        % detectado)
        elseif saida_desejada( 1, j ) == -1 && saida_rna( 1, j ) >= -0.5
            palavra_t1_desej = strcat( palavra_t1_desej, 'H' );
            palavra_t1_rna = strcat( palavra_t1_rna, 'L' );
        end

        % Tanque 2 ----------------------------------------------------
        % Acerto - Nao ha falha
        if saida_desejada( 2, j ) == 0 && saida_rna( 2, j ) <= 0.5
            palavra_t2_desej = strcat( palavra_t2_desej, 'L' );
            palavra_t2_rna = strcat( palavra_t2_rna, 'L' );
        % Acerto - Ha falha no tanque 2
        elseif saida_desejada( 2, j ) == 1 && saida_rna( 2, j ) > 0.5
            palavra_t2_desej = strcat( palavra_t2_desej, 'H' );
            palavra_t2_rna = strcat( palavra_t2_rna, 'H' );
        % Erro - Falso positivo (detectou falha quando nao ha)
        elseif saida_desejada( 2, j ) == 0 && saida_rna( 2, j ) > 0.5
            palavra_t2_desej = strcat( palavra_t2_desej, 'L' );
            palavra_t2_rna = strcat( palavra_t2_rna, 'H' );
        % Erro - Falso negativo (nao detectou falha quando deveria ter 
        % detectado)
        elseif saida_desejada( 2, j ) == 1 && saida_rna( 2, j ) <= 0.5
            palavra_t2_desej = strcat( palavra_t2_desej, 'H' );
            palavra_t2_rna = strcat( palavra_t2_rna, 'L' );
        end
    end
    
    % Separadores
    div = num_amostras / 4;

    linhas = [div+1 2*div; 2*div+1 3*div; 3*div+1 4*div];

    % Saida para tikz-timing - Tanques 1 e 2
    fid = fopen( strcat( nome_arq, '.tikz' ), 'w' );

    fprintf( fid, '\\begin{tikztimingtable}\n' );
    fprintf( fid, 'Desejado & ' );
    fprintf( fid, strcat( num2str( div ), 'U' ) );
    fprintf( fid, strcat( num2str( div ), 'D{Falha T1}' ) );
    fprintf( fid, strcat( num2str( div ), 'D{Falha T2}' ) );
    fprintf( fid, strcat( num2str( div ), 'D{Falha T1 + T2}\\\\\n' ) );
    fprintf( fid, 'Detecção T1 & %s%s%s\\\\\n', ...
                  palavra_t1_rna( linhas( 1,1 ) : linhas( 1,2 ) ), ...
                  palavra_t1_rna( linhas( 2,1 ) : linhas( 2,2 ) ), ...
                  palavra_t1_rna( linhas( 3,1 ) : linhas( 3,2 ) ) );

    fprintf( fid, 'Detecção T2 & %s%s%s\\\\\n', ...
                  palavra_t2_rna( linhas( 1,1 ) : linhas( 1,2 ) ), ...
                  palavra_t2_rna( linhas( 2,1 ) : linhas( 2,2 ) ), ...
                  palavra_t2_rna( linhas( 3,1 ) : linhas( 3,2 ) ) );
	fprintf( fid, '\\end{tikztimingtable}\n' );

    fclose( fid );
end