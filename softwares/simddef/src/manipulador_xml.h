#ifndef MANIPULADOR_XML_H_
#define MANIPULADOR_XML_H_

#include <QDebug>

#include <QList>
#include <QObject>
#include <QString>
#include <QXmlAttributes>
#include <QXmlDefaultHandler>
#include <QXmlParseException>

#include "excecoes.h"
#include "modulo.h"

class ManipuladorXml : public QXmlDefaultHandler
{
    // Metodos
    public:
        ManipuladorXml( void * );
        ~ManipuladorXml();

        bool fatalError( const QXmlParseException & );

        bool startElement( const QString &,
                           const QString &,
                           const QString &,
                           const QXmlAttributes & );

        bool characters( const QString & );

    // Atributos
    private:
        void *parametro;
};

#endif
