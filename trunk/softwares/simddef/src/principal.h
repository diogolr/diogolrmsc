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
#include "config_modulos.h"
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
        void atualizar_modulos( const QString & );
        void desabilitar_botoes_falhas();
        void desabilitar_botoes_modulos();
        void habilitar_botoes_falhas();
        void habilitar_botoes_modulos();
        void inicializar();
        void limpar_falhas();
        void limpar_modulos();

    private slots:
        void on_acao_cfg_falhas_triggered();
        void on_acao_cfg_modulos_triggered();
        void on_botao_carregar_falhas_clicked();
        void on_botao_carregar_modulos_clicked();
        void on_botao_recarregar_falhas_clicked();
        void on_botao_recarregar_modulos_clicked();
    
    // Atributos
    private:
        bool arq_falhas_configurado;

        ConfigFalhas *cfg_falhas;
        ConfigModulos *cfg_modulos;

        ManipuladorXml manipulador_xml;

        QList< Modulo * > lista_modulos;
        QList< QStringList > lista_falhas;

        QStringList abreviaturas;
        QStringList descricoes;
        
        Ui_JanelaPrincipal *ui;
};

#endif
