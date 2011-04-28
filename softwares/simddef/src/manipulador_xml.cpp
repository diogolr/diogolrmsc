#ifndef MANIPULADOR_XML_CPP_
#define MANIPULADOR_XML_CPP_

#include "manipulador_xml.h"

ManipuladorXml :: ManipuladorXml( void *p ) : QXmlDefaultHandler()
{
    parametro = p;
}


ManipuladorXml :: ~ManipuladorXml()
{
}


bool ManipuladorXml :: fatalError( const QXmlParseException &excecao )
{
    QString msg( "Erro de leitura no arquivo XML, linha %1, coluna %2. "
                  "Mensagem de erro: %3" );

    msg = msg.arg( excecao.lineNumber() )
             .arg( excecao.columnNumber() )
             .arg( excecao.message() );

    throw ExcecaoLeituraXML( msg );
    
    return false;
}


bool ManipuladorXml :: startElement( const QString &namespace_uri,
                                     const QString &local_name,
                                     const QString &q_name,
                                     const QXmlAttributes &atribs )
{
    if ( q_name == "Simddef" && atribs.count() == 2 )
    {
        if ( atribs.qName( 0 ) == "versao" &&
             atribs.qName( 1 ) == "tipo" ) 
            return true;
    }

    if ( q_name == "Falha" && atribs.count() == 3 )
    {
        if ( atribs.qName( 0 ) == "local" &&
             atribs.qName( 1 ) == "abrv" &&
             atribs.qName( 2 ) == "descricao" )
        {
            if ( atribs.value( 0 ) != "Atuador" &&
                 atribs.value( 0 ) != "Sensor" &&
                 atribs.value( 0 ) != "Sistema" )
                return false;

            QList< QStringList > *falhas = (QList< QStringList > *)parametro;

            QStringList atributos;

            atributos << atribs.value( 0 ) 
                      << atribs.value( 1 ) 
                      << atribs.value( 2 );

            falhas->append( atributos );

            return true;
        }
    }

    if ( q_name == "Modulo" && atribs.count() == 2 )
    {
        if ( atribs.qName( 0 ) == "abrv_falha" &&
             atribs.qName( 1 ) == "tipo_modulo" )
        {
            // TODO
        }
    }

    return false;
}


bool ManipuladorXml :: characters( const QString &dados )
{
    // if ( dentro de tal tag )
    //     ...
    //     return true;

    return true;
}

#endif
