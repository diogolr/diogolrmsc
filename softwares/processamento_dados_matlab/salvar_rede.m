% Salva a rede para um formato que pode ser lido e C++

function salvar_rede( rede, nome_arq )
    % abre/cria nome_arquivo
    fid = fopen( strcat( nome_arq, '.cfg' ), 'w' );
    
    n_camadas = rede.numLayers;        % numero de camadas da rede
    n_entradas = rede.inputs{1}.size;  % numero de entradas da rede
    
    % adicionando informacoes no nome_arquivo
    fprintf( fid, '%d \t %d\n\n', n_entradas, n_camadas );
    
    % percorrendo as camadas da rede
    for i = 1 : n_camadas
        n_neuronios = rede.layers{i}.size;       % numero de neuronios
        f_ativacao = rede.layers{i}.transferFcn; % funcao de ativacao
        
        if ( strcmp(f_ativacao,'logsig') )
            f_ativacao = 's';                    % sigmoide
        elseif ( strcmp(f_ativacao,'tansig') )
            f_ativacao = 't';                    % tangente hiperbolica
        else
            f_ativacao = 'l';                    % linear
        end
        
        % adicionando informacoes no nome_arquivo
        fprintf( fid, '%d \t %c\n', n_neuronios, f_ativacao );
        
        % salvando os pesos da camada
        if ( i == 1 )
            pesos = [rede.IW{1} rede.b{1}]; % pesos de entrada
        else
            pesos = [rede.LW{2} rede.b{i}]; % pesos das demais camadas
        end
        
        [lin,col] = size(pesos);
        
        % adicionando informacoes no nome_arquivo
        for i = 1 : lin
            for j = 1 : col
                fprintf( fid, '%.12f\t', pesos(i,j) );
            end
            fprintf( fid, '\n' );
        end
        
        fprintf( fid,'\n' );
    end
    
    fclose(fid); % fechando nome_arquivo
end