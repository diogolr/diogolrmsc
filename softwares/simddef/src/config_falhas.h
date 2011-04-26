#ifndef CONFIG_FALHAS_H_
#define CONFIG_FALHAS_H_

#include <QDebug>

#include <QDialog>
#include <QFile>
#include <QIODevice>
#include <QList>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QTableWidgetItem>
#include <QWidget>
#include <QXmlInputSource>
#include <QXmlSimpleReader>
#include <QXmlStreamWriter>

#include "excecoes.h"
#include "funcoes.h"
#include "manipulador_xml.h"
#include "ui_config_falhas.h"

class ConfigFalhas : public QDialog
{
    Q_OBJECT

    // Metodos
    public:
        ConfigFalhas( QWidget *pai = 0, 
                      const QString &arq = "../cfgs/falhas.sdd" );
        ~ConfigFalhas();

        QList< QStringList > lista_falhas();

        QStringList abreviaturas();
        QStringList descricoes();

        void ler_falhas();

    private:
        void atualizar_lista();
        void carregar_falhas();

    private slots:
        void on_adicionar_clicked();
        void on_remover_clicked();
        void on_salvar_clicked();

    // Atributos
    private:
        QString nome_arq;

        QList< QStringList > falhas;

        Ui_ConfigFalhas *ui;
};

#endif
