function [matriz] = contar_sequencia( vetor )

    tam = length( vetor );
    
    if tam == 0
        return;
    end
    
    contador = 1;
    
    inicio = vetor( 1 );
    fim = 0;
    
    linha = 1;
    
    while contador ~= tam
        % testando se o proximo elemento do vetor e a sequencia do elemento
        % atual
        if vetor( contador + 1 ) == vetor( contador ) + 1
            if ~inicio
                inicio = vetor( contador );
            end
        else
            fim = vetor( contador );
        end
        
        % se o fim ja tiver sido determinado, deve-se escrever na matriz
        if fim
            matriz( linha, 1 ) = inicio;
            matriz( linha, 2 ) = fim;
            
            inicio = vetor( contador + 1 );
            fim = 0;
            
            linha  = linha + 1;
        end
        
        contador = contador + 1;
    end
    
    % escrevendo os dois ultimos elementos
    matriz( linha, 1 ) = inicio;
    matriz( linha, 2 ) = vetor( contador );
end