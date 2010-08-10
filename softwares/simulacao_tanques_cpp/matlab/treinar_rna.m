function [rede lim_ent lim_sai] = treinar_rna( ent, ...
                                               sai, ...
                                               neur_cam_oculta, ...
                                               min, ...
                                               max, ...
                                               normalizar )

if normalizar == 1
    [entrada lim_ent] = normalizar( ent, min, max, 0 );
    [saida lim_sai] = normalizar( sai, min, max, 0 );
else
    lim_ent = 0;
    lim_sai = 0;
    
    entrada = ent;
    saida = sai;
end        

rede = newff( entrada, saida, neur_cam_oculta );

rede.trainParam.show = 20;
rede.trainParam.goal = 1e-5;
rede.trainParam.epochs = 40000;
rede.trainParam.max_fail = 50;

% Inicializacao dos pesos e do bias
rede = init( rede );

% Treinamento
rede = train( rede, entrada, saida );

end