% Arquivo de Treinamento das RNAs que detectam as falhas

clear;
clc;
close all;

% arq_niveis = input( 'Niveis [treinamento]: ' );
% Identificacao
% arq_niveis = 'D:\documentos\Diogo\diogolrmsc@ggc\softwares\simulacao_tanques_cpp\saidas\10_min_normal\niveis_treinamento.dat';
% arq_niveis = 'E:\documentos\diogolrmsc@ggc\softwares\simulacao_tanques_cpp\saidas\10_min_normal\niveis_treinamento.dat';

% Falha
arq_niveis = 'D:\documentos\Diogo\diogolrmsc@ggc\softwares\simulacao_tanques_cpp\saidas\10_min_FAVK_0.8\niveis_treinamento.dat';

% arq_erro_sc = input( 'Erro e Sinal de controle [treinamento]: ' );
% Identificacao
% arq_erro_sc = 'D:\documentos\Diogo\diogolrmsc@ggc\softwares\simulacao_tanques_cpp\saidas\10_min_normal\erro_sc_treinamento.dat';
% arq_erro_sc = 'E:\documentos\diogolrmsc@ggc\softwares\simulacao_tanques_cpp\saidas\10_min_normal\erro_sc_treinamento.dat';

% Falha
arq_erro_sc = 'D:\documentos\Diogo\diogolrmsc@ggc\softwares\simulacao_tanques_cpp\saidas\10_min_FAVK_0.8\erro_sc_treinamento.dat';


mat_niveis = dlmread( arq_niveis, '\t' );
mat_erro_sc = dlmread( arq_erro_sc, '\t' );

num_amostras = size( mat_niveis, 1 );

% Determinando se sera treinada uma RNA para identificacao do sistema ou
% para deteccao de falha --------------------------------------------------
% falha = input( 'RNA para deteccao de falha [0 ou 1]: ' );
falha = 1;

% Determinando o numero de redes a serem treinadas ------------------------
% n_treinamentos = input( 'Numero de treinamentos: ' );
n_treinamentos = 6;

% Determinando o numero de regressores de acordo com a ordem do modelo ----
ordem = input( 'Ordem do modelo: ' );
% ordem = 2;

regressores = ordem - 1;

% Ajustanto os dados das matrizes de entrada e saida ----------------------
[entrada_pad saida_pad] = ajustar_dados( mat_niveis, mat_erro_sc );

clear mat_niveis mat_erro_sc;

if falha
    % Determinando o numero de neuronios nas camadas ocultas --------------
    % Para compor o vetor/matriz do numero de neuronios das camadas ocultas
    % deve-se considerar que cada rede so tera uma camada oculta. Logo, se
    % foram realizados os treinamentos com 8, 10 e 12 neuronios, o 
    % vetor nco sera:
    %
    % nco = [8 10 12]; 
    nco = input( 'Neuronios das camadas ocultas [vetor]: ' );
else
    % Determinando a proposta para adequar a saida da rede ----------------
    % disp( ' ' );
    % disp( 'Propostas: ');
    % disp( '[1] RNA para identificacao global' );
    % disp( '[2] RNA para identificacao individual ou em separado' );
    % disp( ' ' );
    % proposta = input( 'Proposta: ' );
    % disp( ' ' );
    proposta = 1;

    % Determinando o numero de neuronios nas camadas ocultas --------------
    % Para compor o vetor/matriz do numero de neuronios das camadas ocultas
    % deve-se considerar que cada rede so tera uma camada oculta. Logo, se
    % foram realizados os treinamentos com 8, 10 e 12 neuronios, o 
    % vetor/matriz nco sera:
    %
    % Proposta 1
    % nco = [8 10 12]; 
    %
    % Proposta 2
    %
    % nco = [ 8 10 12 ; -> Sequencia da RNA 1
    %         8 10 12 ] -> Sequencia da RNA 2
    if proposta == 1
        % Determinando o numero de neuronios nas camadas ocultas
        nco = input( 'Neuronios das camadas ocultas [vetor]: ' );
    else
        % Determinando o numero de neuronios nas camadas ocultas
        nco = input( 'Neuronios das camadas ocultas [matriz]: ' );
    end
end
    
% Exibindo as colunas existentes na matriz --------------------------------
nomes_colunas = { 'L1(k-1)' 'L2(k-1)' 'E1(k-1)' 'E2(k-1)' ...
                  'Vp1(k)' 'Vp2(k)' 'AcaoP1(k)' 'AcaoP2(k)' ...
                  'AcaoI1(k)' 'AcaoI2(k)' 'AcaoD1(k)' 'AcaoD2(k)' };

indices_mantidos = 1 : length( nomes_colunas );

disp( ' ' );

disp( 'Colunas existentes: ' );

for i = 1 : length( indices_mantidos )
    disp( strcat( '[', num2str( i ), '] ', ...
                  nomes_colunas{ indices_mantidos( i ) } ) );
end

disp( ' ' );

% Determinando os parametros de treinamento -------------------------------
% normalizar = input( 'Normalizar dados [0 ou 1]: ' );
normalizar = 0;

% Tolerancia
tol = 1e-3;

% if normalizar == 1
%     min = input( 'Valor minimo: ' );
%     max = input( 'Valor maximo: ' );
% end

min = -1;
max = 1;

% Determinando a(s) entradas/saidas da(s) RNA(s) --------------------------
% Deteccao de falha
if falha == 1    
    entrada = entrada_pad;
    saida = saida_pad;
    
    entrada( :, [3:4 7:12] ) = [];
    indices_mantidos( [3:4 7:12] ) = [];
    
    % Vetor auxiliar para determinacao dos regressores
    vetor = ones( size( indices_mantidos ) );
    
    % Entrada para a melhor rede com regressores
    ordem_melhor_rede = 2;
    regressores_melhor_rede = ordem_melhor_rede - 1;
    
    entrada_melhor_rede = adicionar_regressores( entrada, ...
                                           regressores_melhor_rede*vetor );
    
    % Entrada para rede atual com regressores
    entrada = adicionar_regressores( entrada, regressores*vetor );

    clear vetor;
    
    entrada_melhor_rede = entrada_melhor_rede';
    entrada = entrada';
    saida = saida';
    
    [num_entradas num_amostras] = size( entrada );
    
    if falha
        % Adicionando as colunas dos erros de estimativa
        arq_config = 'D:\documentos\Diogo\diogolrmsc@ggc\qualificacao\dados_matlab\identificacao\P1\O2\N8\P1O2N8T2';

        load( arq_config );

        saida_rna = sim( rede, entrada_melhor_rede );

        erros = saida - saida_rna;
        
        entrada( num_entradas + 1 : num_entradas + 2, : ) = erros;
        
        % Configurando a nova saida
        saida = zeros( num_amostras, 2 );
        
% % % %         % As falhas sao consideradas da seguinte forma:
% % % %         % 0.0 < Saida <   5.0 => Nao ha falha
% % % %         % 5.0 < Saida < 100.0 => Ha falha
% % % %         for i = 1 : num_amostras
% % % %             saida( i, 1 ) = random( -1.0 , 0.0 );
% % % %         end
        
        % Dentro do tempo total, sera simulado um periodo sem falha, um 
        % periodo com falha somente no tanque 1, um periodo com falha 
        % somente no tanque 2 e um ultimo periodo com falha nos dois 
        % tanques. Por esse motivo o numero de amostras e dividido por 
        % quatro.
        %
        % No primeiro intervalo nao sera simulada nenhuma falha, logo so ha
        % necessidade de modificar os intervalos 2 a 4. Portanto, para um 
        % caso com 12000 amstras as linhas a serem modificadas serao da 
        % 3001 ate a 6000, da 6001 ate a 9000 e da 9001 ate a 12000
        div = num_amostras / 4;
        
        linhas = [div+1 2*div; 2*div+1 3*div; 3*div+1 4*div];
        
        % Falha T1
        saida( linhas( 1, 1 ) : linhas( 1, 2 ), 1 ) = -1.0;
        % Falha T2
        saida( linhas( 2, 1 ) : linhas( 2, 2 ), 2 ) = 1.0;
        % Falha T1 + Falha T2
        saida( linhas( 3, 1 ) : linhas( 3, 2 ), 1 ) = -1.0;
        saida( linhas( 3, 1 ) : linhas( 3, 2 ), 2 ) = 1.0;
        
        saida = saida';
    end
% Identificacao
else
    % Proposta 1
    if proposta == 1
        entrada = entrada_pad;
        saida = saida_pad;

        entrada( :, [3:4 7:12] ) = [];
        indices_mantidos( [3:4 7:12] ) = [];

        % Determinando o numero de regressores por coluna
        vetor = ones( size( indices_mantidos ) );

        entrada = adicionar_regressores( entrada, regressores*vetor );

        clear vetor;

        entrada = entrada';
        saida = saida';
    % Proposta 2
    else
        entrada_rna1 = entrada_pad;
        entrada_rna2 = entrada_pad;

        indices_mantidos_rna1 = indices_mantidos;
        indices_mantidos_rna2 = indices_mantidos;

        % Excluindo as colunas que nao serao utilizadas
        entrada_rna1( :, [2:4 6:12] ) = [];
        indices_mantidos_rna1( [2:4 6:12] ) = [];

        entrada_rna2( :, [3:4 7:12] ) = [];
        indices_mantidos_rna2( [3:4 7:12] ) = [];
        
        % Determinando o numero de regressores por coluna para cada
        vetor_rna1 = ones( size( indices_mantidos_rna1 ) );
        vetor_rna2 = ones( size( indices_mantidos_rna2 ) );

        entrada_rna1 = adicionar_regressores( entrada_rna1, ...
                                              regressores*vetor_rna1 );
        entrada_rna2 = adicionar_regressores( entrada_rna2, ...
                                              regressores*vetor_rna2 );

        clear vetor_rna1 vetor_rna2;

        % Determinando as saidas
        saida_rna1 = saida( :, 1 );
        saida_rna2 = saida( :, 2 );

        % Trasnpondo as entradas/saidas para se adequarem das RNAs
        entrada_rna1 = entrada_rna1';
        entrada_rna2 = entrada_rna2';
        saida_rna1 = saida_rna1';
        saida_rna2 = saida_rna2';
    end
end

clear entrada_pad;
clear saida_pad;
clear indices_mantidos;

% Determinando o numero de redes neurais a serem treinadas de acordo com o
% vetor da camada oculta
num_redes = length( nco );

for r = 1 : num_redes  
    % =====================================================================
    % Treinamento para deteccao de FALHA
    % =====================================================================
    if falha
        disp( ' ' );
        disp( strcat( 'Rede N', num2str( nco( r ) ), ' ===============' ) );
        
        for t = 1 : n_treinamentos
            disp( ' ' );
            disp( strcat( 'Treinamento ', num2str( t ), ' ...' ) );
            
            % Treinando a RNA
            tic
            [rede lim_ent lim_sai] = treinar_rna( entrada, ...
                                                  saida, ...
                                                  nco( r ), ...
                                                  min, max, ...
                                                  normalizar, tol );

            tempo_treinamento = toc;

            % Salvando o ambiente
            nome_arq = strcat( 'O', num2str( ordem ), ...
                               'N', num2str( nco( r ) ), ...
                               'T', num2str( t ) );

            save( nome_arq, 'rede', 'lim_ent', 'lim_sai', ...
                            'tempo_treinamento' );

            % Salvando as informacoes da rede
            salvar_rede( rede, strcat( nome_arq, '_RNA' ) );
        end

    % =====================================================================
    % Treinamento para IDENTIFICACAO do modelo
    % =====================================================================
    else      
        % Proposta 1
        if proposta == 1
            disp( ' ' );
            disp( strcat( 'Rede N', num2str( nco( r ) ), ' ===========' ) );
            
            for t = 1 : n_treinamentos
                disp( ' ' );
                disp( strcat( 'Treinamento ', num2str( t ), ' ...' ) );

                % Treinando a RNA         
                tic
                [rede lim_ent lim_sai] = treinar_rna( entrada, ...
                                                      saida, ...
                                                      nco( r ), ...
                                                      min, max, ...
                                                      normalizar, tol );

                tempo_treinamento = toc;

                % Salvando o ambiente
                nome_arq = strcat( 'P1O', num2str( ordem ), ...
                                   'N', num2str( nco( r ) ), ...
                                   'T', num2str( t ) );

                save( nome_arq, 'rede', 'lim_ent', 'lim_sai', ...
                                'tempo_treinamento' );

                % Salvando as informacoes da rede
                salvar_rede( rede, strcat( nome_arq, '_RNA' ) );
            end
            
        % Proposta 2    
        else
            disp( ' ' );
            disp( strcat( 'Rede N', nco( 1, r ), ...
                          'N', nco( 2, r ), '==================' ) );
            
            for t = 1 : n_treinamentos
                disp( ' ' );
                disp( strcat( 'Treinamento ', num2str( t ), ' ...' ) );
                
                % Treinando as RNAs
                tic
                [rede_1 lim_ent_rna1 lim_sai_rna1] = treinar_rna( ...
                                                          entrada_rna1, ...
                                                          saida_rna1, ...
                                                          nco( 1, r ), ...
                                                          min, ...
                                                          max, ...
                                                          normalizar, ...
                                                          tol );
                tempo_treinamento_rna1 = toc;

                tic
                [rede_2 lim_ent_rna2 lim_sai_rna2] = treinar_rna( ...
                                                          entrada_rna2, ...
                                                          saida_rna2, ...
                                                          nco( 2, r ), ...
                                                          min, ...
                                                          max, ...
                                                          normalizar, ...
                                                          tol );
                tempo_treinamento_rna2 = toc;

                % Salvando o ambiente
                nome_arq = strcat( 'P2O', num2str( ordem ), ...
                                   'N', num2str( nco( 1, r ) ), ...
                                   'N', num2str( nco( 2, r ) ), ...
                                   'T', num2str( t ) );

                save( nome_arq, 'rede_1', 'rede_2', ...
                                'lim_ent_rna1', 'lim_ent_rna2', ...
                                'lim_sai_rna1', 'lim_sai_rna2', ...
                                'tempo_treinamento_rna1', ...
                                'tempo_treinamento_rna2' );

                % Salvando as informacoes da rede
                salvar_rede( rede_1, strcat( nome_arq, '_R1' ) );
                salvar_rede( rede_2, strcat( nome_arq, '_R2' ) );
            end
        end
    end
end