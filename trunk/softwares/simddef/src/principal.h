#ifndef PRINCIPAL_H_
#define PRINCIPAL_H_

#include <QDebug>

#include <QMainWindow>

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

    // Atributos
    private:
        Ui_JanelaPrincipal *ui;
};

#endif
