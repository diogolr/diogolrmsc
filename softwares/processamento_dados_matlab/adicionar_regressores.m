% Adiciona os regressores aos dados de acordo com o vetor regs
% Exemplo:
% a = [  1   2   3   4 ;
%        4   5   6   7 ;
%        7   8   9  10 ;
%       10  11  12  13 ];
%
% regs = [ 0 1 2 0 ];
%
% A saida entao sera:
%
% matriz = [  1   2  0   3  0  0   4 ;
%             4   5  2   6  3  0   7 ;
%             7   8  5   9  6  3  10 ;
%            10  11  8  12  9  6  13 ];

function [matriz] = adicionar_regressores( dados, regs )

    [linhas colunas] = size( dados );
    
    matriz = zeros( linhas, colunas + sum( regs ) );
     
    for i = 1 : colunas
        if regs( i ) > 0
            nova_col = zeros( linhas, 1 + regs( i ) );
            
            nova_col( :, 1 ) = dados( :, i );
            
            for j = 1 : regs( i )
                nova_col( 1+j:end, 1+j ) = dados( 1:end-j, i );
            end  
            
            matriz( :, i + sum( regs( 1:i-1 ) ) : ...
                       i + sum( regs( 1:i-1 ) ) + regs( i ) ) = nova_col;
        else
            matriz( :, i + sum( regs( 1:i ) ) ) = dados( :, i );
        end
    end
end
