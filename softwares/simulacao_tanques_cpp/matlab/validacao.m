% Arquivo para validacao das redes treinadas

clear;
clc;
close all;

% pasta_valid = input( 'Pasta dos arquivos de validacao: ' );
pasta_valid = 'D:\documentos\Diogo\diogolrmsc@ggc\softwares\simulacao_tanques_cpp\saidas\10_min_normal\';

% pasta_rnas = input( 'Pasta dos arquivos das redes treinadas: ' );
pasta_rnas = 'D:\documentos\Diogo\diogolrmsc@ggc\softwares\simulacao_tanques_cpp\matlab\identificacao\';

% Determinando se sera treinada uma RNA para identificacao do sistema ou
% para deteccao de falha --------------------------------------------------
falha = input( 'RNA para deteccao de falha [0 ou 1]: ' );

% Determinando o numero de redes a serem treinadas ------------------------
n_treinamentos = input( 'Numero de treinamentos: ' );

% Determinando o numero de regressores de acordo com a ordem do modelo ----
ordem = input( 'Ordem do modelo: ' );

regressores = ordem - 1;

if falha
    % TODO
    % TODO
else
    % Determinando a proposta para adequar a saida da rede ----------------
    disp( ' ' );
    disp( 'Propostas: ');
    disp( '[1] RNA para identificacao global' );
    disp( '[2] RNA para identificacao individual ou em separado' );
    disp( ' ' );
    proposta = input( 'Proposta: ' );
    disp( ' ' );
    
    % Determinando o numero de neuronios nas camadas ocultas ------------------
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
        nco = input( 'Neuronios das camadas ocultas [vetor]: ' );
    else
        nco = input( 'Neuronios das camadas ocultas [matriz]: ' );
    end
end

for v = 1 : 3
    % Lendo o arquivo de validacao ----------------------------------------
    nome_arq_niveis = strcat( 'niveis_v', num2str( v ), '.dat' );
    nome_arq_erro_sc = strcat( 'erro_sc_v', num2str( v ), '.dat' );
    
    mat_niveis = dlmread( strcat( pasta_valid, nome_arq_niveis ), '\t' );
    mat_erro_sc = dlmread( strcat( pasta_valid, nome_arq_erro_sc ), '\t' );

    % Ajustanto os dados das matrizes de entrada e saida ------------------
    [entrada saida] = ajustar_dados( falha, mat_niveis, mat_erro_sc );
    
    clear mat_niveis;
    clear mat_erro_sc;

    % Proposta 1 ----------------------------------------------------------
    if proposta == 1
        % Excluindo as colunas que nao serao utilizadas
        entrada( :, [3:4 7:12] ) = [];
        
        % Adicionando regressores
        vetor = ones( 1, 4 );

        entrada = adicionar_regressores( entrada, regressores*vetor );

        clear vetor;
        
        % Transpondo a entrada/saida para adequar a da RNA
        entrada = entrada';
        saida = saida';
        
        % Configurando o nome das subpastas para ler os arquivos
        % corretamente
        for n = 1 : size( nco, 2 )
            subpasta = strcat( 'P1\O', num2str( ordem ), ...
                               '\N', num2str( nco( n ) ), '\' );

            % Nome do arquivo a ser aberto - Removendo as contrabarras
            nome_arq = regexprep( subpasta, '\', '' );
            
            % Validacao para cada treinamento
            for t = 1 : n_treinamentos
                % Adicionando o numero de treinamento ao nome do arquivo
                nome_arq_trein = strcat( nome_arq, 'T', num2str( t ) );
                
                % Limpando as variaveis abertas
                clear rede tempo_treinamento lim_ent lim_sai;
                
                % Abrindo o arquivo da rede salva
                load( strcat( pasta_rnas, subpasta, nome_arq_trein ) );
                
                % Validando a rede
                validar_rede( rede, entrada, saida, ...
                              strcat( nome_arq_trein, '_v', ...
                                      num2str( v ) ) );
            end
        end

    % Proposta 2 ----------------------------------------------------------
    else
        % Excluindo as colunas que nao serao utilizadas
        entrada_rna1 = entrada;
        entrada_rna2 = entrada;
    end
end