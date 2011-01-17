function [rede lim_ent lim_sai] = treinar_rna( ent, ...
                                               sai, ...
                                               neur_cam_oculta, ...
                                               min, ...
                                               max, ...
                                               norm, ...
                                               tol )
                 
if norm == 1
    [entrada lim_ent] = normalizar( ent, min, max, 0 );
    [saida lim_sai] = normalizar( sai, min, max, 0 );
else
    lim_ent = 0;
    lim_sai = 0;
    
    entrada = ent;
    saida = sai;
end        

[ lin col ] = size( ent );

if lin * col > 10^6
    rede = newcf( entrada, saida, neur_cam_oculta );
else
    rede = newff( entrada, saida, neur_cam_oculta );
end

rede.trainParam.show = 20;
rede.trainParam.goal = tol;
rede.trainParam.epochs = 40000;
rede.trainParam.max_fail = 10;

% Inicializacao dos pesos e do bias
rede = init( rede );

% Treinamento
if lin * col > 10^6
    rede.trainFcn = 'trainbfg';
else
    rede.trainFcn = 'trainlm';
end

rede = train( rede, entrada, saida );

end