function saida = repetir_colunas( matriz, num_repet )

    [ num_linhas num_colunas ] = size( matriz );
    
    saida = zeros( num_linhas, num_colunas * ( num_repet + 1 ) );
    
    for i = 1 : num_colunas
        for j = 1 : num_repet + 1
            saida( :, ( i - 1 ) * ( num_repet + 1 ) + j ) = matriz( :, i );
        end
    end
end