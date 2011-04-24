#ifndef CONFIG_FALHA_H_
#define CONFIG_FALHA_H_

#include <QDebug>

#include <QDialog>
#include <QFile>
#include <QString>
#include <QXmlInputSource>
#include <QXmlSimpleReader>

#include "manipulador_xml.h"
#include "ui_config_falha.h"

class ConfigFalha : public QDialog
{
    // Metodos
    public:
        ConfigFalha();
        ~ConfigFalha();

        bool carregar_falhas( const QString & );

    // Atributos
    private:
        Ui_ConfigFalha *ui;
};

#endif
