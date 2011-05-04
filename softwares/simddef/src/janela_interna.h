#ifndef JANELA_INTERNA_H_
#define JANELA_INTERNA_H_

#include <QList>
#include <QWidget>

#include "ui_janela_interna.h"

class JanelaInterna : public QWidget
{
    // Metodos
    public:
        JanelaInterna( QWidget *pai = 0 );
        ~JanelaInterna();

        void cfg_dados( const QList< QList< double > > & );
        void exibir_saida();
        void exibir_deteccoes();

    // Atributos
    private:
        Ui_JanelaInterna *ui;
};

#endif
