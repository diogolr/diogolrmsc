#ifndef PRINCIPAL_H_
#define PRINCIPAL_H_

#include <QDebug>

#include <QFileDialog>
#include <QMainWindow>
#include <QStringList>

#include "config_falhas.h"
#include "config_modulo.h"
#include "rede.h"
#include "ui_principal.h"

class JanelaPrincipal : public QMainWindow
{
    Q_OBJECT

    // Metodos
    public:
        JanelaPrincipal();
        ~JanelaPrincipal();

    private:
        void atualizar_falhas();
        void inicializar();

    private slots:
        void on_acao_cfg_falhas_triggered();
        void on_botao_carregar_falhas_clicked();
        void on_botao_clicked();
    
    // Atributos
    private:
        bool arq_falhas_configurado;

        ConfigFalhas *cfg_falhas;
        ConfigModulo *cfg_modulo;

        QStringList abreviaturas;
        QStringList descricoes;
        
        Ui_JanelaPrincipal *ui;
};

#endif
