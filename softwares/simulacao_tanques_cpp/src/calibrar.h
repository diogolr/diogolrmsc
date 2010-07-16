#ifndef CALIBRAR_H_
#define CALIBRAR_H_

#include <QDialog>
#include <QTimer>

#include "funcoes.h"
#include "quanser.h"
#include "ui_calibrar.h"

class Calibrar : public QDialog
{
    Q_OBJECT

    // Metodos
    public:
        Calibrar( QWidget *pai = NULL );
        ~Calibrar();

    private:
        void inicializar();

    private slots:
        void ler();
        void on_iniciar_clicked();
        void on_um_clicked();
        void on_dois_clicked();
        void on_parar_clicked();
        void on_tres_clicked();
        void on_zero_clicked();

    // Atributos
    private:
        int canal_esc;
        int canal_leit_t1;
        int canal_leit_t2;

        double nivel_t1;
        double nivel_t2;

        QTimer *temporizador;

        Quanser *quanser;

        Ui_Calibrar *ui;
};

#endif
