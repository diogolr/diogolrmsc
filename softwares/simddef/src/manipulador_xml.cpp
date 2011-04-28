#ifndef MANIPULADOR_XML_CPP_
#define MANIPULADOR_XML_CPP_

#include "manipulador_xml.h"

ManipuladorXml :: ManipuladorXml()
{
}


ManipuladorXml :: ~ManipuladorXml()
{
}


QList< QStringList > ManipuladorXml :: ler_falhas( const QString &nome_arq )
{
    QFile arq( nome_arq );

    // Se o arquivo nao puder ser aberto
    if ( !arq.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        throw ExcecaoArquivo( "O arquivo não pôde ser aberto para leitura." );
    }

    QList< QStringList > falhas;

    QXmlStreamReader xml( &arq );

    QXmlStreamReader::TokenType token;

    bool lendo_falha = false;

    // Inicio da leitura do arquivo
    while ( !xml.atEnd() && !xml.hasError() )
    {
        token = xml.readNext();

        // Se for o inicio do documento, nao faca nada
        if ( token == QXmlStreamReader::StartDocument )
            continue;

        // Se for o inicio de um elemento, verifica-se as tags
        if ( token == QXmlStreamReader::StartElement )
        {
            // Tag de controle de versao do programa e outros parametros do
            // arquivo xml
            // A tag Simddef deve possuir dois atributos: versao e tipo
            // O atributo versao podera ser usado no futuro para controle de
            // versoes do programa, enquanto que o atributo tipo descreve o tipo
            // de arquivo que esta sendo lido
            if ( xml.name() == "Simddef" )
            {
                QXmlStreamAttributes atribs = xml.attributes();

                // Processamento dos atributos
                if ( atribs.hasAttribute( "versao" ) && 
                     atribs.hasAttribute( "tipo" ) )
                {
                    // Fazer teste de versao se necessario
                    // if ( atribs.value( 0 ) ) ...
                    
                    // Teste de tipo de arquivo
                    if ( atribs.value( "tipo" ) == "falhas" )
                        continue;
                }

                // Se nao puder continuar, lancar excecao de leitura XML
                xml.raiseError( "Erro da tag <b>Simddef</b>: versão ou tipo de "
                                "arquivo incompatível para a leitura das "
                                "falhas." );
            }
            // Tag de falha
            // Cada falha possui tres tags internas: local, abreviatura e
            // descricao (laco na funcao processar_falha(...))
            else if ( xml.name() == "Falha" )
            {
                lendo_falha = true;
                processar_falha( xml, falhas );
            }
            else
            {
                xml.raiseError( "Tag <b>" + xml.name().toString() + "</b> "
                                "inválida." );
            }
        }
        // Se for o fim de um elemento, verificar se e o fim de uma falha ou se
        // e o fim da tag Simddef
        else if ( token == QXmlStreamReader::EndElement )
        {
            if ( lendo_falha && xml.name() != "Falha" )
            {
                xml.raiseError( "Esperado o fim da tag <b>Falha</b>." );
            }
            else if ( !lendo_falha && xml.name() != "Simddef" )
            {
                xml.raiseError( "Esperado o fim da tag <b>Simddef</b>." );
            }

            lendo_falha = false;
        }
    }

    arq.close();

    // Se houver erro, lancar excecao de leitura XML
    if ( xml.hasError() )
    {
        throw ExcecaoLeituraXML( "Erro na leitura do arquivo XML (linha " + 
                                 QString::number( xml.lineNumber() ) +
                                 ", coluna " +
                                 QString::number( xml.columnNumber() ) + "). "
                                 "Mensagem de erro: " + xml.errorString() );
    }

    return falhas;
}


// Dependendo da situacao, pode ser mais conveniente utilizar os metodos
// readElementText(), readNextStartElement()... Nesse caso foi preferivel tratar
// todos os elementos atraves de readNext para gerar as strings de erro
// corretamente
void ManipuladorXml :: processar_falha( QXmlStreamReader &xml,
                                        QList< QStringList > &falhas )
{
    QStringList valores;

    QString nome_tag = "";

    // Cada falha possui 3 tags internas
    for ( int c = 0 ; c < 3 ; c++ )
    {
        xml.readNext();

        // Ignorando os espacos em branco antes do inicio da tag
        while ( xml.isWhitespace() )
            xml.readNext();

        if ( xml.tokenType() != QXmlStreamReader::StartElement )
        {
            xml.raiseError( "Erro no processamento da falha. Início da tag "
                            "<b>" + xml.name().toString() + "</b> inválido." );
            return;
        }
        
        nome_tag = xml.name().toString();

        xml.readNext();

        if( xml.tokenType() != QXmlStreamReader::Characters )
        {
            xml.raiseError( "Erro no processamento da falha. Esperado o valor "
                            "da tag <b>" + nome_tag + "</b>." );
            return;
        }

        valores << xml.text().toString();
        
        xml.readNext();
        
        if ( xml.tokenType() != QXmlStreamReader::EndElement )
        {
            xml.raiseError( "Erro no processamento da falha. Fim da tag "
                            "<b>" + xml.name().toString() + "</b> inválido." );
            return;
        }
    }

    falhas << valores;
}

#endif
