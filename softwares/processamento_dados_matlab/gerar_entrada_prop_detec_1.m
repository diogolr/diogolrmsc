% Arquivo para gerar pontos de entrada para treinamento da RNA
clear;
clc;

% =========================================================================
% Valores fixos
% =========================================================================
a1_peq = 0.079173043608984;
a2_peq = 0.079173043608984;
a1_med = 0.178139348120214;
a2_med = 0.178139348120214;
a1_gde = 0.242467446052514;
a2_gde = 0.242467446052514;

% =========================================================================
% Parametros
% =========================================================================
% Ganhos dos sensores
ganho_sensor_t1 = 1.0;
ganho_sensor_t2 = 1.0;

% Offset dos sensores
offset_sensor_t1 = 0.0;
offset_sensor_t2 = 0.0;

% Porcentagem de ruido dos sensores
porcent_ruido_sensor_t1 = 0.0;
porcent_ruido_sensor_t2 = 0.0;

% Ganho dos atuadores
ganho_atuador_t1 = 1.0;
ganho_atuador_t2 = 1.0;

% Offset dos atuadores
offset_atuador_t1 = 0.0;
offset_atuador_t2 = 0.0;

% Porcentagem de ruido dos atuadores
porcent_ruido_atuador_t1 = 0.0;
porcent_ruido_atuador_t2 = 0.0;

% Area do orificio de vazamento
area_orif_vazamento_t1 = 0.0; %a1_med / 10;
area_orif_vazamento_t2 = 0.0; %a2_med / 10;

% Constantes das bombas
constante_bomba_t1 = 4.6;
constante_bomba_t2 = 4.6;

% Orificios de saida
area_orif_saida_t1 = a1_med;
area_orif_saida_t2 = a2_med;

% Ganho dos modulos de potencia
ganho_mod_pot_t1 = 1.0;
ganho_mod_pot_t2 = 1.0;

% =========================================================================
% Determinando a ordem das falhas para gerar os dados de maneira adequada
% =========================================================================
falhas = { 'FSeDG' 'FSeDO' 'FSeSR' 'FSeQ' ...
    'FADG' 'FADO' 'FASR' 'FAVK' 'FAQ' ...
    'FSiVzT' 'FSiVrOS' 'FSiVrGMP' 'FSiEOS' };

% Falhas a serem combinadas
comb_falhas = [3 9 ; ... % FSeSR + FAQ
               8 13];    % FAVK + FSiEOS

colunas = [ 2 3 ; 4 5 ; 6 7 ; 2 3 ; ...             % Falhas nos sensores
            8 9 ; 10 11 ; 12 13 ; 16 17 ; 8 9 ; ... % Falhas nos atuadores
            14 15 ; 18 19 ; 20 21 ; 18 19 ];        % Falhas no sistema

val_param = [ ganho_sensor_t1 ...    % FSeDG
              1.0 ...                % FSeDO
              1.0 ...                % FSeSR
              ganho_sensor_t1 ...    % FSeQ
              ganho_atuador_t1 ...   % FADG
              1.0 ...                % FADO
              1.0 ...                % FASR
              constante_bomba_t1 ... % FAVK
              ganho_atuador_t1 ...   % FAQ
              a1_med ...             % FSiVzT
              a1_med ...             % FSiVrOS
              ganho_mod_pot_t1 ...   % FSiVrGMP
              a1_med ];              % FSiEOS

% Treinamento -------------------------------------------------------------
min_max = [ 0.8 1.2 ; ...
    -3.0 3.0 ;
    0.03 0.03 ; % A simulacao faz a variacao de -X ate +X
    0.0 0.0 ; ...
    0.8 1.0 ;
    -1.0 0.0 ;
    0.03 0.03 ; % A simulacao faz a variacao de -X ate +X
    0.7 1.1 ;
    0.0 0.0 ; ...
    0.25 0.75 ;
    0.75 1.25 ;
    0.8 1.0 ;
    0.0 0.5 ];

% Validacao ---------------------------------------------------------------
% min_max = [ 0.8 0.8 ; ...
%             -2.0 -2.0 ;
%             0.02 0.02 ;
%             0.0 0.0 ; ...
%             0.8 0.8 ;
%             -0.5 -0.5 ;
%             0.02 0.02 ;
%             0.75 0.75 ;
%             0.0 0.0 ; ...
%             0.5 0.5 ;
%             0.85 0.85 ;
%             0.9 0.9 ;
%             0.25 0.25 ];

% =========================================================================
% Parametros de configuracao da geracao dos dados
% =========================================================================
% Tempo de simulacao em segundos
% tempo_seg = 60 * 10; % 10 minutos (Identificacao)
tempo_seg = 60 * 25; % 25 minutos (Deteccao de falhas - Combinacoes)
% tempo_seg = 60 * 20; % 20 minutos (Deteccao de falhas - Individuais)
% tempo_seg = 60 * 1.75; % 1 minuto e 45 segundos (Resultados)

% Periodo de amostragem em segundos
periodo_amost = 0.1;

% Vetor de tempo
t = 0.0 : periodo_amost : tempo_seg - periodo_amost;

num_amostras = length( t );

% =========================================================================
% Referencias dos tanques
% =========================================================================
% Treinamento -------------------------------------------------------------
% Tempo em segundos para que haja a variacao da referencia
tempo_variacao_seg = 15;
tempo_var_milisegundos = tempo_variacao_seg / periodo_amost;

ref_t1 = zeros( num_amostras, 1 );
ref_t2 = zeros( num_amostras, 1 );

% Como as referencias devem variar a cada 15 segundos, haverao 3
% possibilidades: variar somente a referencia do tanque 1, variar somente a
% referencia do tanque 2 ou ainda variar a referencia dos 2 tanques. Entao
% essa probabilidade sera de 33.33% de chance para cada uma dessas
% variacoes
ref_t1( 1 ) = random( 0, 30 );
ref_t2( 1 ) = random( 0, 30 );

for i = 2 : num_amostras
    if mod( i, tempo_var_milisegundos ) == 0
        sorteio = rand();
        
        % Variar somente referencia do tanque 1
        if sorteio < 1/3
            ref_t1( i ) = random( 0, 30 );
            ref_t2( i ) = ref_t2( i - 1 );
            % Variar somente referencia do tanque 2
        elseif sorteio > 1/3 && sorteio < 2/3
            ref_t1( i ) = ref_t1( i - 1 );
            ref_t2( i ) = random( 0, 30 );
            % Variar referencia dos 2 tanques
        else
            ref_t1( i ) = random( 0, 30 );
            ref_t2( i ) = random( 0, 30 );
        end
    else
        ref_t1( i ) = ref_t1( i - 1 );
        ref_t2( i ) = ref_t2( i - 1 );
    end
end

% Validacao ---------------------------------------------------------------
% referencia_t1 = 15;
% referencia_t2 = 20;

% ref_t1 = referencia_t1*ones( num_amostras, 1 );
% ref_t2 = referencia_t2*ones( num_amostras, 1 );

% =========================================================================
% Determinando que tipo de dados serao gerados
% =========================================================================
% disp( 'Gerar dados para:' )
% disp( '0 - Falhas Individuais'
% disp( '1 - Combinacoes de falhas' );
% disp( ' ' );
% tipo = input( 'Tipo: ' );
tipo = 1;

if tipo == 1
    % =====================================================================
    % Modificacao da matriz de dados - Combinacoes de falhas
    % =====================================================================
    for N = 1 : length( comb_falhas )
        disp( strcat( falhas{ comb_falhas( N, 1 ) }, ...
                      '+', ...
                      falhas{ comb_falhas( N, 2 ) }, ...
                      ' ............' ) );
                
        % Composicao da matriz de dados sem falhas
        M = [ t', ...
            ones( num_amostras, 1 ) * ganho_sensor_t1, ...
            ones( num_amostras, 1 ) * ganho_sensor_t2, ...
            ones( num_amostras, 1 ) * offset_sensor_t1, ...
            ones( num_amostras, 1 ) * offset_sensor_t2, ...
            ones( num_amostras, 1 ) * porcent_ruido_sensor_t1, ...
            ones( num_amostras, 1 ) * porcent_ruido_sensor_t2, ...
            ones( num_amostras, 1 ) * ganho_atuador_t1, ...
            ones( num_amostras, 1 ) * ganho_atuador_t2, ...
            ones( num_amostras, 1 ) * offset_atuador_t1, ...
            ones( num_amostras, 1 ) * offset_atuador_t2, ...
            ones( num_amostras, 1 ) * porcent_ruido_atuador_t1, ...
            ones( num_amostras, 1 ) * porcent_ruido_atuador_t2, ...
            ones( num_amostras, 1 ) * area_orif_vazamento_t1, ...
            ones( num_amostras, 1 ) * area_orif_vazamento_t2, ...
            ones( num_amostras, 1 ) * constante_bomba_t1, ...
            ones( num_amostras, 1 ) * constante_bomba_t2, ...
            ones( num_amostras, 1 ) * area_orif_saida_t1, ...
            ones( num_amostras, 1 ) * area_orif_saida_t2, ...
            ones( num_amostras, 1 ) * ganho_mod_pot_t1, ...
            ones( num_amostras, 1 ) * ganho_mod_pot_t2, ...
            ref_t1, ...
            ref_t2 ];
        
        % Parametros a serem modificados
        % Nome da falha
        nome_falhas = strcat( falhas{ comb_falhas( N, 1 ) }, ...
                              '+', ...
                              falhas{ comb_falhas( N, 2 ) } );
        
        % Colunas a serem modificadas para as falhas no tanque 1 e 2
        cols = [ colunas( comb_falhas( N, 1 ), : )
                 colunas( comb_falhas( N, 2 ), : ) ];
        
        % Porcentagens minima e maxima de alteracao
        min = [ min_max( comb_falhas( N, 1 ), 1 ) ...
                min_max( comb_falhas( N, 2 ), 1 ) ];
        max = [ min_max( comb_falhas( N, 1 ), 2 ) ...
                min_max( comb_falhas( N, 2 ), 2 ) ];
        
        % Intervalos de ativacao de cada uma das falhas
        div = num_amostras / 5;
        
        linhas = [ div+1 2*div; ...
                   2*div+1 3*div; ...
                   3*div+1 4*div; ...
                   4*div+1 5*div];
        
        valor_param = [ val_param( comb_falhas( N, 1 ) ) ...
                        val_param( comb_falhas( N, 2 ) ) ];
        
        for i = 1 : size( linhas, 1 )
            for j = linhas( i, 1 ) : linhas( i, 2 )
                % F1 + F1
                if i == 1
                    M( j, cols( 1, 1 ) ) = ...
                        valor_param( 1 ) * random( min( 1 ), max( 1 ) );
                    M( j, cols( 2, 1 ) ) = ...
                        valor_param( 2 ) * random( min( 2 ), max( 2 ) );
                % F1 + F2
                elseif i == 2
                    M( j, cols( 1, 1 ) ) = ...
                        valor_param( 1 ) * random( min( 1 ), max( 1 ) );
                    M( j, cols( 2, 2 ) ) = ...
                        valor_param( 2 ) * random( min( 2 ), max( 2 ) );
                % F2 + F1
                elseif i == 3
                    M( j, cols( 1, 2 ) ) = ...
                        valor_param( 1 ) * random( min( 1 ), max( 1 ) );
                    M( j, cols( 2, 1 ) ) = ...
                        valor_param( 2 ) * random( min( 2 ), max( 2 ) );
                % F2 + F2
                elseif i == 4
                    M( j, cols( 1, 2 ) ) = ...
                        valor_param( 1 ) * random( min( 1 ), max( 1 ) );
                    M( j, cols( 2, 2 ) ) = ...
                        valor_param( 2 ) * random( min( 2 ), max( 2 ) );
                end
            end
        end
        
        % =====================================================================
        % Salvando a saida em arquivo
        % =====================================================================
        nome_arq_saida = strcat( '../simulacao_tanques_cpp/entradas/', ...
                                 nome_falhas, '/treinamento.cfg' );
        
        fid = fopen( nome_arq_saida, 'w' );
        
        fprintf( fid, '%d\n', num_amostras );
        
        fclose( fid );
        
        dlmwrite( nome_arq_saida, M, '-append', 'delimiter', '\t' );
        
        clear M;
    end
else
    % =====================================================================
    % Modificacao da matriz de dados - Falhas individuais
    % =====================================================================
    for N = 1 : length( falhas )
        disp( strcat( falhas{N}, ' ............' ) );
                
        % Composicao da matriz de dados sem falhas
        M = [ t', ...
            ones( num_amostras, 1 ) * ganho_sensor_t1, ...
            ones( num_amostras, 1 ) * ganho_sensor_t2, ...
            ones( num_amostras, 1 ) * offset_sensor_t1, ...
            ones( num_amostras, 1 ) * offset_sensor_t2, ...
            ones( num_amostras, 1 ) * porcent_ruido_sensor_t1, ...
            ones( num_amostras, 1 ) * porcent_ruido_sensor_t2, ...
            ones( num_amostras, 1 ) * ganho_atuador_t1, ...
            ones( num_amostras, 1 ) * ganho_atuador_t2, ...
            ones( num_amostras, 1 ) * offset_atuador_t1, ...
            ones( num_amostras, 1 ) * offset_atuador_t2, ...
            ones( num_amostras, 1 ) * porcent_ruido_atuador_t1, ...
            ones( num_amostras, 1 ) * porcent_ruido_atuador_t2, ...
            ones( num_amostras, 1 ) * area_orif_vazamento_t1, ...
            ones( num_amostras, 1 ) * area_orif_vazamento_t2, ...
            ones( num_amostras, 1 ) * constante_bomba_t1, ...
            ones( num_amostras, 1 ) * constante_bomba_t2, ...
            ones( num_amostras, 1 ) * area_orif_saida_t1, ...
            ones( num_amostras, 1 ) * area_orif_saida_t2, ...
            ones( num_amostras, 1 ) * ganho_mod_pot_t1, ...
            ones( num_amostras, 1 ) * ganho_mod_pot_t2, ...
            ref_t1, ...
            ref_t2 ];
        
        % Parametros a serem modificados
        % Nome da falha
        nome_falha = falhas{N};
        
        % Colunas a serem modificadas para as falhas no tanque 1 e 2 (olhar
        % acima)
        cols = colunas( N, : );
        
        % Valor a ser modificado
        valor_param = val_param( N );
        
        % Porcentagens minima e maxima de alteracao
        min = min_max( N, 1 );
        max = min_max( N, 2 );
        
        % Falhas para treinamento -----------------------------------------
        % Dentro do tempo total, sera simulado um periodo sem falha, um
        % periodo com falha somente no tanque 1, um periodo com falha
        % somente no tanque 2 e um ultimo periodo com falha nos dois
        % tanques. Esse processo se repetira para cada uma das falhas. Por
        % esse motivo o numero de amostras e dividido por 4*length(falhas)
        
        div = num_amostras / 4;
        
        linhas = [div+1 2*div; 2*div+1 3*div; 3*div+1 4*div];
        
        for i = 1 : size( linhas, 1 )
            for j = linhas( i, 1 ) : linhas( i, 2 )
                % Falha somente no tanque 1 ou somente no tanque 2
                if i == 1 || i == 2
                    M( j, cols( i ) ) = valor_param * random( min, max );
                % Falha nos dois tanques
                else
                    M( j, cols( 1 ) ) = valor_param * random( min, max );
                    M( j, cols( 2 ) ) = valor_param * random( min, max );
                end
            end
        end
        
        % Validacao -------------------------------------------------------
        % As simulacoes serao feitas em intervalos de 105 segundos (1
        % minuto e 45 segundos) no qual existirao 7 segmentos, comecando
        % com um intervalo sem falha e terminando com um intervalo sem
        % falha. Cada intervalo sera de 15 segundos (tempo suficiente para
        % estabilizacao do processo)
        
        %     num_amostras = size( M, 1 );
        %
        %     div = num_amostras / 7;
        %
        %     linhas = [  div+1 2*div ; 3*div+1 4*div ; 5*div+1 6*div ];
        %
        %     for i = 1 : size( linhas, 1 )
        %         for j = linhas( i, 1 ) : linhas( i, 2 )
        %             if i == 1 || i == 2
        %                 M( j, cols( i ) ) = valor_param * random( min, max );
        %             % Falha nos dois tanques
        %             else
        %                 M( j, cols( 1 ) ) = valor_param * random( min, max );
        %                 M( j, cols( 2 ) ) = valor_param * random( min, max );
        %             end
        %         end
        %     end
        
        % =====================================================================
        % Salvando a saida em arquivo
        % =====================================================================
        nome_arq_saida = strcat( '../simulacao_tanques_cpp/entradas/', ...
            nome_falha, '/qualif.cfg' );
        
        fid = fopen( nome_arq_saida, 'w' );
        
        fprintf( fid, '%d\n', num_amostras );
        
        fclose( fid );
        
        dlmwrite( nome_arq_saida, M, '-append', 'delimiter', '\t' );
        
        clear M;
    end
end