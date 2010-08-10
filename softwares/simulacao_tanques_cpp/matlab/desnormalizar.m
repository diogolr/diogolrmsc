% FUNCAO: desnormalizaDados
% desnormalizacao dos dados de saida da rede neural

% dadosNorm: dados normalizados para sofrem desnormalizacao
% lim: limites minimo e maximo das variaveis (saidas)
% ymin, ymax: valores maximos e minimos estipulados para os dados normalizados

% saida: dados dernormalizados

function [saida] = desnormalizaDados( dadosNorm, lim, ymin, ymax )

  MIN = 1;
  MAX = 2;

  % numero de variaveis e de amostras de dados  
  [nVar, nAm] = size( dadosNorm );
  
  % desnormalizacao dos dados (formula retirada da funcao mapminmax)
  saida = zeros( nVar, nAm );  
  for i = 1 : nVar
    saida(i,:) = ( dadosNorm(i,:) - ymin )*( lim(i,MAX) - lim(i,MIN) )/( ymax - ymin ) + lim(i,MIN);
  end