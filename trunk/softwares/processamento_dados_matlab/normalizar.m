% FUNCAO: normalizaDados
% normalizacao de dados para serem utilizados pela rede (entradas e saidas)

% dados: matriz contendo os dados a serem normalizados
% ymin, ymax: valores minimos e maximos estipulados para os dados normalizados
% lim: limites minimos e maximos das 'variaveis' a serem normalizadas
%      se lim = 0, os limites sao calculados pela funcao a partir dos 
%      dados fornecidos

% dadosNorm: matriz contendo os dados normalizados
% lim: limites minimos e maximos das variaveis

function [dadosNorm, lim] = normalizar( dados, ymin, ymax, lim )

  MIN = 1;
  MAX = 2;

  % se limites nao fornecidos, sao calculados
  if ~lim
    lim = minmax( dados );  
  end

  % numero de variaveis e de amostras
  % cada coluna corresponde a uma amostra
  % entrada da rede neural  
  [nVar, nAm] = size( dados );    
  
  % normalizacao dos dados (formula retirada da funcao mapminmax)
  dadosNorm = zeros( nVar, nAm );
  for i = 1 : nVar
    dadosNorm(i,:) = ymin+ (ymax - ymin)*( dados(i,:) - lim(i,MIN) )/...
                                         ( lim(i,MAX) - lim(i,MIN) );
  end