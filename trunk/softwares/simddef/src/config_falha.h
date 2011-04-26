#ifndef CONFIG_FALHA_H_
#define CONFIG_FALHA_H_

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
#include "ui_config_falha.h"

class ConfigFalha : public QDialog
{
    Q_OBJECT

    // Metodos
    public:
        ConfigFalha( const QString &, QWidget *pai = 0 );
        ~ConfigFalha();

        QList< QStringList > ler_falhas();

    private:
        void carregar_falhas();
        void configurar_arquivo( const QString & );
        void inicializar();
        void ler_tabela();

    private slots:
        void on_adicionar_clicked();
        void on_remover_clicked();
        void on_salvar_clicked();

    // Atributos
    private:
        QString nome_arq;

        QList< QStringList > falhas;

        Ui_ConfigFalha *ui;
};

#endif
