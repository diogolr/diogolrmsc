#ifndef SIMULACAO_CPP_
#define SIMULACAO_CPP_

#include "simulacao.h"

Simulacao :: Simulacao()
{
    reiniciar_valores();
}


Simulacao :: ~Simulacao()
{
}


double Simulacao :: ler( const int &canal )
{
    // Vazamento ---------------------------------------------------------------
    // Evitando que seja extraida a raiz quadrada de um numero negativo
    if ( nivel_t1 > 0.0 )
        nivel_t1 -= ( area_orif_vazamento_t1 / A1 ) * 
                    ( sqrt( 2 * g * nivel_t1 ) );
    // Evitando que seja extraida a raiz quadrada de um numero negativo
    if ( nivel_t2 > 0.0 )
        nivel_t2 -= ( area_orif_vazamento_t2 / A2 ) * 
                    ( sqrt( 2 * g * nivel_t2 ) );

    // Ruido -------------------------------------------------------------------
    nivel_t1 = random( nivel_t1 * ( 1.0 - porcent_ruido_sensor_t1 ),
                       nivel_t1 * ( 1.0 + porcent_ruido_sensor_t1 ) );
    nivel_t2 = random( nivel_t2 * ( 1.0 - porcent_ruido_sensor_t2 ),
                       nivel_t2 * ( 1.0 + porcent_ruido_sensor_t2 ) );

    // Nivel DC ----------------------------------------------------------------
    nivel_t1 += dc_sensor_t1;
    nivel_t2 += dc_sensor_t2;

    if ( canal == canal_leitura_t1 )
    {
        return ganho_sensor_t1 * GANHO_SENSOR * nivel_t1;
    }
    else if ( canal == canal_leitura_t2 )
    {
        return ganho_sensor_t2 * GANHO_SENSOR * nivel_t2;
    }
    
    return 0.0;
}


void Simulacao :: configurar_canais_escrita( const int &canal_t1,
                                             const int &canal_t2 )
{
    canal_escrita_t1 = canal_t1;
    canal_escrita_t2 = canal_t2;
}


void Simulacao :: configurar_canais_leitura( const int &canal_t1,
                                             const int &canal_t2 )
{
    canal_leitura_t1 = canal_t1;
    canal_leitura_t2 = canal_t2;
}


void Simulacao :: escrever( const int &canal, const double &valor )
{
    if ( canal == canal_escrita_t1 )
    {
        tensao_bomba_1 = ganho_mp_t1 * valor;
        
        // Ganho ---------------------------------------------------------------
        tensao_bomba_1 *= ganho_atuador_t1;

        // Ruido ---------------------------------------------------------------
        tensao_bomba_1 = random( tensao_bomba_1 * 
                                 ( 1.0 - porcent_ruido_atuador_t1 ),
                                 tensao_bomba_1 * 
                                 ( 1.0 + porcent_ruido_atuador_t1 ) );

        // Nivel DC ------------------------------------------------------------
        tensao_bomba_1 += dc_atuador_t1;

        if ( tensao_bomba_1 > 15.0 || tensao_bomba_1 < -15.0 )
        {
            emit erro( "[Erro Simulação]: Bomba 1 danificada" );
        }
    }
    else if ( canal == canal_escrita_t2 )
    {
        tensao_bomba_2 = ganho_mp_t2 * valor;
        
        // Ganho ---------------------------------------------------------------
        tensao_bomba_2 *= ganho_atuador_t2;

        // Ruido ---------------------------------------------------------------
        tensao_bomba_2 = random( tensao_bomba_2 * 
                                 ( 1.0 - porcent_ruido_atuador_t2 ),
                                 tensao_bomba_2 * 
                                 ( 1.0 + porcent_ruido_atuador_t2 ) );

        // Nivel DC ------------------------------------------------------------
        tensao_bomba_2 += dc_atuador_t2;
        
        if ( tensao_bomba_2 > 15.0 || tensao_bomba_2 < -15.0 )
        {
            emit erro( "[Erro Simulação]: Bomba 2 danificada" );
        }
    }

    return;
}


void Simulacao :: modificar_dc_atuador( const int &tanque,
                                        const double &valor )
{
    switch ( tanque )
    {
        case TANQUE_1:
            dc_atuador_t1 = valor;
            break;

        case TANQUE_2:
            dc_atuador_t2 = valor;
            break;

        default:
            return;
    }
}


void Simulacao :: modificar_dc_sensor( const int &tanque,
                                       const double &valor )
{
    switch ( tanque )
    {
        case TANQUE_1:
            dc_sensor_t1 = valor;
            break;

        case TANQUE_2:
            dc_sensor_t2 = valor;
            break;

        default:
            return;
    }
}


void Simulacao :: modificar_ganho_atuador( const int &tanque,
                                           const double &valor )
{
    switch ( tanque )
    {
        case TANQUE_1:
            ganho_atuador_t1 = valor;
            break;

        case TANQUE_2:
            ganho_atuador_t2 = valor;
            break;

        default:
            return;
    }
}


void Simulacao :: modificar_ganho_mod_pot( const int &tanque, 
                                           const double &valor )
{
    switch ( tanque )
    {
        case TANQUE_1:
            ganho_mp_t1 = valor * GANHO_MOD_POT;
            break;

        case TANQUE_2:
            ganho_mp_t2 = valor * GANHO_MOD_POT;
            break;

        default:
            return;
    }
}


void Simulacao :: modificar_ganho_sensor( const int &tanque,
                                          const double &valor )
{
    switch ( tanque )
    {
        case TANQUE_1:
            ganho_sensor_t1 = valor;
            break;

        case TANQUE_2:
            ganho_sensor_t2 = valor;
            break;

        default:
            return;
    }
}


void Simulacao :: modificar_orif_saida( const int &tanque,
                                        const double &valor )
{
    switch ( tanque )
    {
        case TANQUE_1:
            orif_saida_t1 = valor;
            break;

        case TANQUE_2:
            orif_saida_t2 = valor;
            break;

        default:
            return;
    }
}


void Simulacao :: modificar_orif_vazamento( const int &tanque,
                                            const double &valor )
{
    switch ( tanque )
    {
        case TANQUE_1:
            area_orif_vazamento_t1 = valor;
            break;

        case TANQUE_2:
            area_orif_vazamento_t2 = valor;
            break;

        default:
            return;
    }
}


void Simulacao :: modificar_ruido_atuador( const int &tanque,
                                           const double &valor )
{
    switch ( tanque )
    {
        case TANQUE_1:
            porcent_ruido_atuador_t1 = valor;
            break;

        case TANQUE_2:
            porcent_ruido_atuador_t2 = valor;
            break;

        default:
            return;
    }
}


void Simulacao :: modificar_ruido_sensor( const int &tanque,
                                          const double &valor )
{
    switch ( tanque )
    {
        case TANQUE_1:
            porcent_ruido_sensor_t1 = valor;
            break;

        case TANQUE_2:
            porcent_ruido_sensor_t2 = valor;
            break;

        default:
            return;
    }
}


void Simulacao :: modificar_km( const int &tanque, const double &valor )
{
    switch ( tanque )
    {
        case TANQUE_1:
            km_1 = valor;
            break;

        case TANQUE_2:
            km_2 = valor;
            break;

        default:
            return;
    }
}


void Simulacao :: reiniciar_valores()
{
    /*
    hab_dc_sensor_t1 = false;
    hab_dc_sensor_t2 = false;
    hab_dc_atuador_t1 = false;
    hab_dc_atuador_t2 = false;
    hab_ruido_sensor_t1 = false;
    hab_ruido_sensor_t2 = false;
    hab_ruido_atuador_t1 = false;
    hab_ruido_atuador_t2 = false;
    ...
    */

    canal_escrita_t1 = CANAL_ESCRITA_T1;
    canal_escrita_t2 = CANAL_ESCRITA_T2;
    canal_leitura_t1 = CANAL_LEITURA_T1;
    canal_leitura_t2 = CANAL_LEITURA_T2;

    //altura_orif_vazamento_t1 = 0.0;
    //altura_orif_vazamento_t2 = 0.0;

    ganho_mp_t1 = GANHO_MOD_POT;
    ganho_mp_t2 = GANHO_MOD_POT;
    ganho_atuador_t1 = 1.0;
    ganho_atuador_t2 = 1.0;
    ganho_sensor_t1 = 1.0;
    ganho_sensor_t2 = 1.0;

    km_1 = Km;
    km_2 = Km;

    L[0] = 0.0;
    L[1] = 0.0;

    nivel_t1 = 0.0; 
    nivel_t2 = 0.0;

    dc_atuador_t1 = 0.0;
    dc_atuador_t2 = 0.0;
    dc_sensor_t1 = 0.0;
    dc_sensor_t2 = 0.0;

    orif_saida_t1 = a1_med;
    orif_saida_t2 = a2_med;

    porcent_ruido_atuador_t1 = 0.0;
    porcent_ruido_atuador_t2 = 0.0;
    porcent_ruido_sensor_t1 = 0.0;
    porcent_ruido_sensor_t2 = 0.0;
    
    area_orif_vazamento_t1 = 0.0;
    area_orif_vazamento_t2 = 0.0;

    tensao_bomba_1 = 0.0;
    tensao_bomba_2 = 0.0;
}


double Simulacao :: dLdt( const double &t, double *y, const int &i )
{
    if ( y[0] < 0.0 )
        y[0] = 0.0;
    if ( y[1] < 0.0 )
        y[1] = 0.0;

    if ( i == 0 )
    {
        return ( km_1 / A1 ) * tensao_bomba_1 -
               ( orif_saida_t1 / A1 ) * sqrt( 2 * g * y[0] );
    }
    else
    {
        return ( orif_saida_t1 / A1 ) * sqrt( 2 * g * y[0] ) - 
               ( orif_saida_t2 / A2 ) * sqrt( 2 * g * y[1] ) +
               ( km_2 / A2 ) * tensao_bomba_2;
    }
}


void Simulacao :: runge_kutta_4_mod( double *y,           // Valores iniciais
                                     const double &passo, // Passo
                                     const double &x,     // Vetor X (tempo)
                                     const int &N )       // Numero de Equacoes
{
    double h = passo/2.0, // Ponto intermediario
	       t1[N],         // Vetores auxiliares
           t2[N],
           t3[N],
	       k1[N],         // Vetores do metodo runge kutta
           k2[N],
           k3[N],
           k4[N];

    int i;

    // Passos do Runge-Kutta
    for ( i = 0 ; i < N ; i++ ) 
    {
        k1[i] = passo * dLdt( x, y, i );
        t1[i] = y[i] + 0.5 * ( k1[i] );
    }

    for ( i = 0 ; i < N ; i++ )
    {
        k2[i] = passo * dLdt( x + h, t1, i );
        t2[i] = y[i] + 0.5 * ( k2[i] );
    }

    for ( i = 0 ; i < N ; i++ ) 
    {
        k3[i] = passo * dLdt( x + h, t2, i );
        t3[i] = y[i] + ( k3[i] );
    }

    for ( i = 0 ; i < N ; i++ ) 
    {
        k4[i] =	passo * dLdt( x + passo, t3, i );
    }
   
    // Saida 
    for ( i = 0 ; i < N ; i++ )
    { 
        y[i] += ( k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i] ) / 6.0;
    }
}


void Simulacao :: atualizar( const double &t )
{
    double *y = L;

    runge_kutta_4_mod( y, PERIODO_AMOSTRAGEM, t, 2 );

    nivel_t1 = y[0];
    nivel_t2 = y[1];
}

#endif
