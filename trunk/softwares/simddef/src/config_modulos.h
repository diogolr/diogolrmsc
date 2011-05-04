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
#include <QXmlStreamWriter>

#include "funcoes.h"
#include "modulo.h"
#include "rede.h"
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
        void carregar_modulos();
        void desabilitar_ordem();
        void habilitar_ordem();
        void inicializar();

    private slots:
        void on_adicionar_clicked();
        void on_adicionar_arq_clicked();
        void on_descer_arq_clicked();
        void on_modulos_cad_itemDoubleClicked( QTableWidgetItem * );
        void on_remover_clicked();
        void on_remover_arq_clicked();
        void on_salvar_clicked();
        void on_subir_arq_clicked();
        void on_tipos_textChanged( QString );

    // Atributos
    private:
        QHash< QTableWidgetItem *, Modulo * > mapeamento_modulos;

        QList< Modulo * > modulos;

        Ui_ConfigModulos *ui;
};

#endif
