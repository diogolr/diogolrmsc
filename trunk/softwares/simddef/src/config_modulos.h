#ifndef CONFIG_MODULOS_H_
#define CONFIG_MODULOS_H_

#include <QDebug>

#include <QDialog>
#include <QDir>
#include <QFileDialog>
#include <QHash>
#include <QList>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QWidget>

#include "funcoes.h"
#include "modulo.h"
#include "ui_config_modulos.h"

class ConfigModulos : public QDialog
{
    Q_OBJECT

    // Metodos
    public:
        ConfigModulos( QWidget *pai = 0,
                       const QStringList & = QStringList(),
                       const QList< Modulo * > & = QList< Modulo * >() );
        ~ConfigModulos();

    private:
        void atualizar_modulos();
        void carregar_modulos();

    private slots:
        void on_adicionar_clicked();
        void on_adicionar_arq_clicked();
        void on_descer_arq_clicked();
        void on_modulos_cad_itemDoubleClicked( QTableWidgetItem * );
        void on_remover_clicked();
        void on_remover_arq_clicked();
        void on_subir_arq_clicked();

    // Atributos
    private:
        QHash< QTableWidgetItem *, Modulo * > mapeamento_modulos;

        QList< Modulo * > modulos;

        Ui_ConfigModulos *ui;
};

#endif
