function [entrada saida] = ajustar_dados( deteccao_falha, ...
                                          niveis, erro_sc )
    % Ordem das curvas - Grafico de Niveis ................................
    % [2] - Referencia T1
    % [3] - Referencia T2
    % [4] - Nivel T1
    % [5] - Nivel T2
    % Ordem das curvas - Grafico de Erro/Sinal de Controle ................
    % [2] - Erro T1
    % [3] - Erro T2
    % [4] - Sinal de controle T1
    % [5] - Sinal de controle T2
    % [6] - Acao Proporcional T1
    % [7] - Acao Proporcional T2
    % [8] - Acao Integral T1
    % [9] - Acao Integral T2
    % [10] - Acao Derivativa T1
    % [11] - Acao Derivativa T2

	% Configuracao da saida -----------------------------------------------
	if deteccao_falha
        % TODO
        % TODO
    else
        saida( :, 1 ) = niveis( :, 4 );
        saida( :, 2 ) = niveis( :, 5 );
    end
    
    % Configurando a entrada ----------------------------------------------
    % Matriz de entrada sem regressores
    dados = [ niveis( :, 4 ) niveis( :, 5 ) ...
              erro_sc( :, 2 ) erro_sc( :, 3 ) ...
              erro_sc( :, 4 ) erro_sc( :, 5 ) ...
              erro_sc( :, 6 ) erro_sc( :, 7 ) ...
              erro_sc( :, 8 ) erro_sc( :, 9 ) ...
              erro_sc( :, 10 ) erro_sc( :, 11 ) ];

    entrada = dados;

    % A matriz de entrada nao podera conter o nivel atual dos 
    % tanques. Assim, sera considerado somente o nivel anterior.
    entrada = adicionar_regressores( entrada, ...
                                     [1 1 0 0 0 0 0 0 0 0 0 0] );

    entrada( :, [1 3] ) = [];
end