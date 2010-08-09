#ifndef SIMULACAO_H_
#define SIMULACAO_H_

#include <QDebug>
#include <QObject>

#include <cmath>

#include "funcoes.h"
#include "definicoes.h"

class Simulacao : public QObject
{
    Q_OBJECT

    // Metodos
    public:
        Simulacao();
        ~Simulacao();

        double ler( const int & );

        void configurar_canais_escrita( const int &, const int & );
        void configurar_canais_leitura( const int &, const int & );
        void escrever( const int &, const double & );

        void modificar_dc_atuador( const int &tanque,
                                   const double &valor = 0.0 );
        void modificar_dc_sensor( const int &tanque,
                                  const double &valor = 0.0 );
        void modificar_ganho_atuador( const int &tanque,
                                      const double &valor = 1.0 );
        void modificar_ganho_mod_pot( const int &tanque, 
                                      const double &valor = 5.0 );
        void modificar_ganho_sensor( const int &tanque,
                                     const double &valor = 1.0 );
        void modificar_orif_saida( const int &tanque,
                                   const double &valor = a1_med );
        void modificar_orif_vazamento( const int &tanque,
                                       const double &valor = 0.0 );
        void modificar_ruido_atuador( const int &tanque,
                                      const double &valor = 0.0 );
        void modificar_ruido_sensor( const int &tanque,
                                     const double &valor = 0.0 );
        void modificar_km( const int &tanque, 
                           const double &valor = 4.6 );

        void reiniciar_valores();

    private:
        double dLdt( const double &, double *, const int & );
        void runge_kutta_4_mod( double *yo,
                                const double &,
                                const double &,
                                const int & );

    public slots:
        void atualizar( const double & );

    signals:
        void erro( QString );

    // Atributos
    private:
        /*
        bool hab_dc_sensor_t1;
        bool hab_dc_sensor_t2;
        bool hab_dc_atuador_t1;
        bool hab_dc_atuador_t2;
        bool hab_ruido_sensor_t1;
        bool hab_ruido_sensor_t2;
        bool hab_ruido_atuador_t1;
        bool hab_ruido_atuador_t2;
        ...
        */

        //double altura_orif_vazamento_t1;
        //double altura_orif_vazamento_t2;
        double ganho_mp_t1;
        double ganho_mp_t2;
        double ganho_atuador_t1;
        double ganho_atuador_t2;
        double ganho_sensor_t1;
        double ganho_sensor_t2;
        double km_1;
        double km_2;
        double L[2];
        double nivel_t1;
        double nivel_t2;
        double dc_atuador_t1;
        double dc_atuador_t2;
        double dc_sensor_t1;
        double dc_sensor_t2;
        double orif_saida_t1;
        double orif_saida_t2;
        double porcent_ruido_atuador_t1;
        double porcent_ruido_atuador_t2;
        double porcent_ruido_sensor_t1;
        double porcent_ruido_sensor_t2;
        double area_orif_vazamento_t1;
        double area_orif_vazamento_t2;
        double tensao_bomba_1;
        double tensao_bomba_2;

        int canal_escrita_t1;
        int canal_escrita_t2;
        int canal_leitura_t1;
        int canal_leitura_t2;
};

#endif
