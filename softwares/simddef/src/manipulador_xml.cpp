#ifndef MANIPULADOR_XML_CPP_
#define MANIPULADOR_XML_CPP_

#include "manipulador_xml.h"

ManipuladorXml :: ManipuladorXml( QObject *p ) : QXmlDefaultHandler()
{
    parametro = p;
}


ManipuladorXml :: ~ManipuladorXml()
{
}


bool ManipuladorXml :: fatalError( const QXmlParseException &exception )
{
    qWarning() << "Fatal error on line" << exception.lineNumber()
               << ", column" << exception.columnNumber() << ":"
               << exception.message();
    
    return false;
}


bool ManipuladorXml :: startElement( const QString &namespace_uri,
                                     const QString &local_name,
                                     const QString &q_name,
                                     const QXmlAttributes &atribs )
{
    if ( q_name == "Simddef" && atribs.count() == 2 )
    {
        if ( atribs.qName( 1 ) == "tipo" &&
             atribs.value( 1 ) == "falhas" )
            return true;
    }

    if ( q_name == "Falha" && atribs.count() == 3 )
    {
        if ( atribs.qName( 0 ) == "local" &&
             atribs.qName( 1 ) == "abrv" &&
             atribs.qName( 2 ) == "descricao" )
        {
            QTableWidget *tabela = (QTableWidget *)parametro;

            int lin = tabela->rowCount();

            tabela->insertRow( lin );

            // Desabilitando a ordenacao para inserir o item corretamente e so
            // depois ordenar
            tabela->setSortingEnabled( false );

            QTableWidgetItem *local = new QTableWidgetItem( atribs.value( 0 ) );
            QTableWidgetItem *abrv = new QTableWidgetItem( atribs.value( 1 ) );
            QTableWidgetItem *desc = new QTableWidgetItem( atribs.value( 2 ) );

            local->setTextAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
            abrv->setTextAlignment( Qt::AlignHCenter | Qt::AlignVCenter );

            tabela->setItem( lin, 0, local );
            tabela->setItem( lin, 1, abrv );
            tabela->setItem( lin, 2, desc );

            tabela->sortItems( 1, Qt::AscendingOrder );

            // Reabilitando a ordenacao
            tabela->setSortingEnabled( true );

            return true;
        }
    }

    return false;
}

#endif
