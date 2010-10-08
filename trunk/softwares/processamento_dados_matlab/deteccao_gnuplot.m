function deteccao_gnuplot( nome_arq, matriz, tanque )

    num_linhas = size( matriz,1 );

    fid = fopen( nome_arq, 'w' );
    
    for i = 1 : num_linhas
        if tanque == 1
            fprintf( fid, ...
                     strcat( 'set obj rect from %f, graph 0 to %f, graph 1', ...
                             'fc rgb "#A00000" fs transparent solid 0.2 noborder\n' ), ...
                     matriz( i, 1 ), matriz( i, 2 ) );
        else
            fprintf( fid, ...
                     strcat( 'set obj rect from %f, graph 0 to %f, graph 1', ...
                             'fc rgb "#0055FF" fs transparent solid 0.2 noborder\n' ), ...
                     matriz( i, 1 ), matriz( i, 2 ) );
        end
    end
    
    fclose( fid );
end