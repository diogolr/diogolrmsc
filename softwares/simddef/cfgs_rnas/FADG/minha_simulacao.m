function [saida] = minha_simulacao( rede, entrada )

    saida = rede.IW{1} * entrada;
    
    for i = 1 : size( saida, 2 )
        saida( :, i ) = saida( :, i ) + rede.b{1};
    end
    
    saida = tansig( saida );
    
    saida = rede.LW{2,1} * saida;
    
    for i = 1 : size( saida, 2 )
        saida( :, i ) = saida( :, i ) + rede.b{2};
    end
end