#ifndef MANIPULADOR_XML_H_
#define MANIPULADOR_XML_H_

#include <QDebug>

#include <QFile>
#include <QList>
#include <QString>
#include <QStringList>
#include <QXmlStreamReader>
#include <QXmlStreamAttributes>

#include "excecoes.h"
#include "modulo.h"

class ManipuladorXml
{
    // Metodos
    public:
        ManipuladorXml();
        ~ManipuladorXml();

        QList< Modulo * > ler_modulos( const QString & );
        QList< QStringList > ler_falhas( const QString & );

    private:
        void processar_falha( QXmlStreamReader &,
                              QList< QStringList > & );
};

#endif
