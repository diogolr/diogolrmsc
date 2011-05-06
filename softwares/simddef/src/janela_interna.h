#ifndef JANELA_INTERNA_H_
#define JANELA_INTERNA_H_

#include <QColor>
#include <QHash>
#include <QList>
#include <QPen>
#include <QString>
#include <QWidget>

#include <Matrix.h>
#include <Vector.h>
using Flood::Matrix;
using Flood::Vector;

#include <cstdlib>

#include "ui_janela_interna.h"

typedef Matrix< double > MatrizD;
typedef Matrix< int > MatrizI;
typedef Vector< double > VetorD;
typedef Vector< int > VetorI;

class JanelaInterna : public QWidget
{
    // Metodos
    public:
        JanelaInterna( QWidget *pai = 0 );
        ~JanelaInterna();

        void configurar_curvas( const MatrizD &, 
                                const QHash< int, QString > &,
                                const double &periodo_amost = 1.0 );
        void configurar_deteccoes( const QHash< QString, MatrizI > & );
        void exibir_curvas();
        void exibir_deteccoes();
        void limpar();

    private:
        void inicializar();

    // Atributos
    private:
        QList< QColor > lista_cores;

        QList< MatrizI > intervalos;
        QList< QString > nomes_deteccoes;

        Ui_JanelaInterna *ui;
};

#endif
