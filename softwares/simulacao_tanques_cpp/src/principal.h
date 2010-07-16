#ifndef PRINCIPAL_H_
#define PRINCIPAL_H_

#include <QDebug>

#include <QBrush>
#include <QFont>
#include <QLabel>
#include <QMainWindow>
#include <QPen>
#include <QPixmap>
#include <QString>
#include <QStringList>
#include <QTimer>

#include <fstream>
using std::ifstream;
using std::ofstream;
using std::ios;

#include <iostream>
using std::endl;

#include "calibrar.h"
#include "config_parametros.h"
#include "config_canais.h"
#include "controle.h"
#include "definicoes.h"
#include "excecoes.h"
#include "quanser.h"
#include "qwt_plot.h"
#include "qwt_text.h"
#include "salvar.h"
#include "simulacao.h"
#include "ui_principal.h"

class JanelaPrincipal : public QMainWindow
{
    Q_OBJECT
    // Atributos publicos
    public:
        enum Configuracao{ Configuracao_1 = 0, Configuracao_2 };
        enum ModoFuncionamento{ RealSimulacao = 0, 
                                SomenteReal, 
                                SomenteSimulacao };

    // Metodos
    public:
        JanelaPrincipal();
        ~JanelaPrincipal();

    private:
        void atualizar_graficos_planta_real( const bool &replote = true );
        void atualizar_graficos_simulacao( const bool &replote = true );
        void atualizar_simulacao_arquivo();
        void carregar_matriz( const QString & );
        void conectar_sinais_slots();
        void configurar_controladores();
        void configurar_graficos();
        void deletar_matriz();
        void estabelecer_conexao();
        void inicializar();
        void ler_campos();
        void salvar_matriz( const QString & );

    private slots:
        void atualizar_dados();
        void desabilitar_campos();
        void enter_pressionado();
        void exportar_dados_graficos();
        void habilitar_aplicar();
        void habilitar_campos();
        void imprimir_graficos();
        void on_acao_calibrar_sensores_triggered();
        void on_acao_config_canais_triggered();
        void on_acao_config_parametros_triggered();
        void on_acao_excluir_config_triggered();
        void on_aplicar_controladores_clicked();
        void on_configuracao_activated( int );
        void on_iniciar_clicked();
        void on_modo_funcionamento_activated( int );
        void on_parar_clicked();
        void on_tipo_controlador_t1_activated( int );
        void on_tipo_controlador_t2_activated( int );
        void salvar_dados_graficos();

    // Atributos
    private:
        bool simulacao_encerrada;

        double acao_p_t1;
        double acao_p_t2;
        double acao_i_t1;
        double acao_i_t2;
        double acao_d_t1;
        double acao_d_t2;
        double erro_t1;
        double erro_t2;
        double kp_t1;
        double ki_t1;
        double kd_t1;
        double kp_t2;
        double ki_t2;
        double kd_t2;
        double nivel_t1;
        double nivel_t2;
        double referencia_t1;
        double referencia_t2;
        double sinal_cont_t1;
        double sinal_cont_t2;
        double tempo_atual;

        double **matriz;

        int num_amostras;
        int num_colunas;
        int real_canal_escrita_t1;
        int real_canal_escrita_t2;
        int real_canal_leitura_t1;
        int real_canal_leitura_t2;
        int sim_canal_escrita_t1;
        int sim_canal_escrita_t2;
        int sim_canal_leitura_t1;
        int sim_canal_leitura_t2;

        Calibrar *calibrar_sensores;

        Configuracao configuracao;

        Controle controlador_t1;
        Controle controlador_t2;

        ConfigParametros *config_parametros;
        ConfigCanais *config_canais;

        ModoFuncionamento modo_funcionamento;

        Quanser *quanser;

        QLabel *imgs_barra_status;
        QTimer *temporizador;

        Salvar *salvar;

        Simulacao *simulacao;

        Ui_JanelaPrincipal *ui;
};

#endif
