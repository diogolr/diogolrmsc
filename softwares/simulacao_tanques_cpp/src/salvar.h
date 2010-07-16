#ifndef SALVAR_H_
#define SALVAR_H_

#include <QDialog>
#include <QDir>
#include <QFileDialog>
#include <QString>
#include <QStringList>
#include <QWidget>

#include "funcoes.h"
#include "ui_salvar.h"

class Salvar : public QDialog
{
    Q_OBJECT
    // Atributos publicos
    public:
        enum TipoJanela{ SalvarDados = 0, ExportarGraficos };

    // Metodos
    public:
        Salvar( QWidget *pai = NULL, const TipoJanela &tipo = SalvarDados );
        ~Salvar();

        QStringList nomes_arquivos();
        void config_tipo_janela( const TipoJanela & );

    private:
        void inicializar();

    private slots:
        void on_selec_real_niveis_clicked();
        void on_selec_real_erro_sc_clicked();
        void on_selec_sim_niveis_clicked();
        void on_selec_sim_erro_sc_clicked();

    // Atributos
    private:
        QString extensoes;

        QStringList arquivos;

        TipoJanela tipo_janela;

        Ui_Salvar *ui;
};

#endif
