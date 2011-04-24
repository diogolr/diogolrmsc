#ifndef PRINCIPAL_H_
#define PRINCIPAL_H_

#include <QDebug>

#include <QDialog>
#include <QMainWindow>

#include <Matrix.h>
using Flood::Matrix;

#include "config_falha.h"
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

    private slots:
        void on_botao_clicked();
    
    // Atributos
    private:
        ConfigModulo config;

        Ui_JanelaPrincipal *ui;
};

#endif
