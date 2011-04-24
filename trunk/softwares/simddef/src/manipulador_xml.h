#ifndef MANIPULADOR_XML_H_
#define MANIPULADOR_XML_H_

#include <QDebug>

#include <QObject>
#include <QString>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QXmlAttributes>
#include <QXmlDefaultHandler>
#include <QXmlParseException>

#include "excecoes.h"

class ManipuladorXml : public QXmlDefaultHandler
{
    // Metodos
    public:
        ManipuladorXml( QObject * );
        ~ManipuladorXml();

        bool fatalError( const QXmlParseException & );

        bool startElement( const QString &,
                           const QString &,
                           const QString &,
                           const QXmlAttributes & );

    // Atributos
        QObject *parametro;
};

#endif
