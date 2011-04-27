#ifndef PRINCIPAL_H_
#define PRINCIPAL_H_

#include <QDebug>

#include <QFileDialog>
#include <QMainWindow>
#include <QList>
#include <QString>
#include <QStringList>
#include <QTreeWidgetItem>
#include <QXmlInputSource>
#include <QXmlSimpleReader>

#include "config_falhas.h"
#include "config_modulo.h"
#include "excecoes.h"
#include "manipulador_xml.h"
#include "rede.h"
#include "ui_principal.h"

class JanelaPrincipal : public QMainWindow
{
    Q_OBJECT

    // Metodos
    public:
        JanelaPrincipal();
        ~JanelaPrincipal();

    private:
        void atualizar_falhas( const QString & );
        void inicializar();
        void limpar_falhas();

    private slots:
        void on_acao_cfg_falhas_triggered();
        void on_botao_adicionar_modulo_clicked();
        void on_botao_carregar_falhas_clicked();
        void on_botao_carregar_modulos_clicked();
        void on_botao_recarregar_falhas_clicked();
    
    // Atributos
    private:
        bool arq_falhas_configurado;

        ConfigFalhas *cfg_falhas;
        ConfigModulo *cfg_modulo;

        QList< Modulo * > modulos;
        QList< QStringList > lista_falhas;

        QStringList abreviaturas;
        QStringList descricoes;
        
        Ui_JanelaPrincipal *ui;
};

#endif
