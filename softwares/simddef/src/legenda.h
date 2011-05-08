#ifndef LEGENDA_H_
#define LEGENDA_H_

#include <QFrame>
#include <QWidget>

#include "ui_legenda.h"

class Legenda : public QWidget
{
    // Metodos
    public:
        Legenda( QWidget *pai = NULL );
        ~Legenda();

    private:
        void inicializar();

    // Atributos
    Ui_Legenda *ui;
};

#endif
