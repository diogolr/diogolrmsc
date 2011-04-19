#ifndef PRINCIPAL_H_
#define PRINCIPAL_H_

#include <QDebug>

#include <QMainWindow>

#include <Matrix.h>
using Flood::Matrix;

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
        Ui_JanelaPrincipal *ui;
};

#endif
