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

#include "excecoes.h"
#include "ui_janela_interna.h"

typedef Matrix< double > MatrizD;
typedef Matrix< int > MatrizI;
typedef Vector< double > VetorD;
typedef Vector< int > VetorI;

class JanelaInterna : public QWidget
{
    Q_OBJECT

    // Metodos
    public:
        JanelaInterna( QWidget *pai = 0 );
        ~JanelaInterna();

        void atualizar_deteccoes();
        void configurar_curvas( const QString &nome_falha,
                                const MatrizD &, 
                                const QHash< int, QString > &,
                                const double &periodo_amost = 1.0 );
        void configurar_deteccoes( const QString &,
                                   const QHash< QString, MatrizI > &,
                                   const double & = 1.0 );
        void exibir_curvas( const bool & );
        void exibir_deteccoes( const bool & );
        void limpar();
        void remover_conjunto( const QString & );
        void remover_curvas( const QString & );
        void remover_deteccoes( const QString & );

    private:
        void inicializar();

    private slots:
        void on_zoom_clicked( bool );
        void on_imprimir_clicked();
        void on_salvar_clicked();

    // Atributos
    private:
        QList< QColor > lista_cores;

        Ui_JanelaInterna *ui;
};

#endif
