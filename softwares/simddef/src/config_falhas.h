#ifndef CONFIG_FALHAS_H_
#define CONFIG_FALHAS_H_

#include <QDebug>

#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <QIODevice>
#include <QList>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QTableWidgetItem>
#include <QWidget>
#include <QXmlStreamWriter>

#include "excecoes.h"
#include "funcoes.h"
#include "ui_config_falhas.h"

class ConfigFalhas : public QDialog
{
    Q_OBJECT

    // Metodos
    public:
        ConfigFalhas( QWidget *pai = 0, 
                      const QList< QStringList > &falhas = 
                            QList< QStringList >() );
        ~ConfigFalhas();

    private:
        void atualizar_lista();
        void carregar_falhas();

    private slots:
        void on_adicionar_clicked();
        void on_remover_clicked();
        void on_salvar_clicked();

    // Atributos
    private:
        QList< QStringList > falhas;

        Ui_ConfigFalhas *ui;
};

#endif
