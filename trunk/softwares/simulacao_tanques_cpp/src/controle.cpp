#ifndef CONTROLE_CPP_
#define CONTROLE_CPP_

#include "controle.h"

Controle :: Controle( const TipoControlador &tc )
{
    inicializar();
    configurar_controlador( tc );
}


Controle :: ~Controle()
{

}


double Controle :: acao_p( const double &erro )
{
    return Kp * erro;
}


double Controle :: acao_p_saida( const double &saida )
{
    return Kp * saida;
}


double Controle :: acao_i( const double &erro )
{
    return acao_I_ant + Ki * periodo_amostragem * erro;
}


double Controle :: acao_d( const double &erro )
{
    return Kd * ( ( erro - erro_ant ) / periodo_amostragem );
}


double Controle :: acao_d_saida( const double &saida )
{
    return Kd * ( ( saida - saida_ant ) / periodo_amostragem );
}


double Controle :: acao_p()
{
    return acao_P;
}


double Controle :: acao_i()
{
    return acao_I;
}


double Controle :: acao_d()
{
    return acao_D;
}


double Controle :: ref()
{
    return referencia;
}


int Controle :: tipo()
{
    return controlador;
}


void Controle :: configurar_controlador( const TipoControlador &tc )
{
    controlador = tc;

    if ( controlador == ( Controle::P || Controle::PD ) )
        integ_cond = false;
}


void Controle :: configurar_ganhos( const double &kp, 
                                    const double &ki, 
                                    const double &kd )
{
    Kp = kp;
    Ki = ki;
    Kd = kd;
}


void Controle :: configurar_limites( const double &li, const double &ls )
{
    lim_inf = li;
    lim_sup = ls;
}


void Controle :: configurar_per_amost( const double &T )
{
    periodo_amostragem = T;
}


void Controle :: configurar_referencia( const double &ref )
{
    referencia = ref;
}


void Controle :: habilitar_integracao_cond( const bool &h )
{
    integ_cond = h;
}


void Controle :: inicializar()
{
    integ_cond = false;

    Kp = Ki = Kd = 0.0;
    acao_P = acao_I = acao_D = 0.0;
    acao_P_ant = acao_I_ant = acao_D_ant = 0.0;

    referencia = 0.0;

    erro = erro_ant = 0.0;
    saida_ant = 0.0;

    lim_inf = 0.0;
    lim_sup = 0.0;

    controlador = Controle::PID;
}


void Controle :: reiniciar_valores()
{
    Kp = Ki = Kd = 0.0;
    acao_P = acao_I = acao_D = 0.0;
    acao_P_ant = acao_I_ant = acao_D_ant = 0.0;

    referencia = 0.0;

    erro = erro_ant = 0.0;
    saida_ant = 0.0;
}


double Controle :: sinal_controle( const double &saida )
{
    sinal = 0.0;

    erro = referencia - saida;

    switch ( controlador )
    {
        case P:
            acao_P = acao_p( erro );
            sinal = acao_P;
            break;

        case PI:
            acao_P = acao_p( erro );
            acao_I = acao_i( erro );

            sinal = acao_P + acao_I;
            break;

        case PD:
            acao_P = acao_p( erro );
            acao_D = acao_d( erro );

            sinal = acao_P + acao_D;
            break;

        default:
        case PID:
            acao_P = acao_p( erro );
            acao_I = acao_i( erro );
            acao_D = acao_d( erro );

            sinal = acao_P + acao_I + acao_D;
            break;

        case PI_D:
            acao_P = acao_p( erro );
            acao_I = acao_i( erro );
            acao_D = acao_d_saida( saida );

            sinal = acao_P + acao_I + acao_D;
            break;
    }

    if ( integ_cond && ( sinal > lim_sup || sinal < lim_inf ) )
    {
        sinal = sinal - acao_I + acao_I_ant;
        acao_I = acao_I_ant;
    }

    if ( sinal > lim_sup )
    {
        sinal = lim_sup;
    }
    else if ( sinal < lim_inf )
    {
        sinal = lim_inf;
    }
    
    acao_P_ant = acao_P;
    acao_I_ant = acao_I;
    acao_D_ant = acao_D;

    erro_ant = erro;
    saida_ant = saida;

    return sinal;
}


#endif
