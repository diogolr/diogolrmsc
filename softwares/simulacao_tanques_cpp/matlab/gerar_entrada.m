% Arquivo para gerar pontos de entrada para treinamento da RNA
clear;
clc;

%% Valores fixos
a1_peq = 0.079173043608984;
a2_peq = 0.079173043608984;
a1_med = 0.178139348120214;
a2_med = 0.178139348120214;
a1_gde = 0.242467446052514;
a2_gde = 0.242467446052514;

%% Parametros
% Tempo de simulacao em segundos
tempo_seg = 60 * 10; % 10 minutos

% Periodo de amostragem em segundos
periodo_amost = 0.1;

% Vetor de tempo
t = 0.0 : periodo_amost : tempo_seg - periodo_amost;

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

% Area do orifício de vazamento
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

%% Referencias
% Como as referencias devem variar a cada 15 segundos, haverao 3
% possibilidades: variar somente a referencia do tanque 1, variar somente a
% referencia do tanque 2 ou ainda variar a referencia dos 2 tanques. Entao
% essa probabilidade sera de 33.33% de chance para cada uma dessas
% variacoes

% Tempo em segundos para que haja a variacao da referencia
tempo_variacao_seg = 15;
tempo_var_milisegundos = tempo_variacao_seg / periodo_amost;

num_amostras = length( t );

ref_t1 = zeros( num_amostras, 1 );
ref_t2 = zeros( num_amostras, 1 );

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

%% Composicao da matriz

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
  
%% Modificacao da matriz para adicionar os parametros modificados
% porcent_ruido_sensor_t1 = 0.0025;
% porcent_ruido_sensor_t2 = 0.0025;
% 
% % Do meio ate o final das amostras (degrau)
% M( num_amostras/2:end , 6 ) = ones( num_amostras/2+1, 1 ) * ...
%                               porcent_ruido_sensor_t1;
% M( num_amostras/2:end , 7 ) = ones( num_amostras/2+1, 1 ) * ...
%                               porcent_ruido_sensor_t2;

%% Saida para arquivo
% nome_arq_saida = strcat( 'config_', ...
%                          strcat( num2str( num_amostras ), '.cfg' ) );

% nome_arq_saida = '10_min_normal_treinamento.cfg';
nome_arq_saida = '10_min_normal_v3.cfg';
% nome_arq_saida = '10_min_FSeSR_0.0025_T1.cfg';
% nome_arq_saida = '10_min_FSeSR_0.0025_T2.cfg';
% nome_arq_saida = '10_min_FSeSR_0.0025_T1_FSeSR_0.0025_T2.cfg';

dlmwrite( nome_arq_saida, M, 'delimiter', '\t' );