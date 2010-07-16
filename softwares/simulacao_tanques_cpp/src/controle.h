#ifndef CONTROLE_H_
#define CONTROLE_H_

#include <QDebug>

class Controle
{
    // Atributos Publicos
    public:
        enum TipoControlador{ P = 0, PI, PD, PID, PI_D };

    // Metodos
    public:
        Controle( const TipoControlador &tc = Controle::PID );
        ~Controle();

        double acao_controle( const double & );
        double acao_p( const double & );
        double acao_p_saida( const double & );
        double acao_i( const double & );
        double acao_d( const double & );
        double acao_d_saida( const double & );
        double acao_p();
        double acao_i();
        double acao_d();
        double ref();

        int tipo();

        void configurar_controlador( const TipoControlador & );
        void configurar_ganhos( const double &kp = 0.0, 
                                const double &ki = 0.0, 
                                const double &kd = 0.0 );
        void configurar_limites( const double &, const double & );
        void configurar_per_amost( const double &T = 0.1 );
        void configurar_referencia( const double & );
        void habilitar_integracao_cond( const bool & );
        void inicializar();

    private:

    // Atributos
    private:
        bool integ_cond;

        double acao;
        double acao_P;
        double acao_I;
        double acao_D;
        double acao_P_ant;
        double acao_I_ant;
        double acao_D_ant;
        double erro;
        double erro_ant;
        double referencia;
        double Kp;
        double Ki;
        double Kd;
        double lim_inf;
        double lim_sup;
        double periodo_amostragem;
        double saida_ant;

        TipoControlador controlador;
};

#endif
