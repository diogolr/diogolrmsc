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
                processar_falhas( xml, falhas );
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


QList< Modulo * > ManipuladorXml :: ler_modulos( const QString &nome_arq )
{
    QFile arq( nome_arq );

    // Se o arquivo nao puder ser aberto
    if ( !arq.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        throw ExcecaoArquivo( "O arquivo não pôde ser aberto para leitura." );
    }

    QList< Modulo * > modulos;

    QXmlStreamReader xml( &arq );

    QXmlStreamReader::TokenType token;

    bool lendo_modulo = true;
    
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
                    if ( atribs.value( "tipo" ) == "modulos" )
                        continue;
                }

                // Se nao puder continuar, lancar excecao de leitura XML
                xml.raiseError( "Erro da tag <b>Simddef</b>: versão ou tipo de "
                                "arquivo incompatível para a leitura dos "
                                "módulos." );
            }
            // Tag de modulo
            // Cada modulo possui tres tags internas: tipo (RNA, Fuzzy...),
            // falha (falha a qual o modulo devera detectar) e arquivos (contem
            // o numero de arquivos e seus enderecos, utilizados no
            // processamento do modulo). Cada uma das tags sera lida no laco
            // interno do metodo processar_mopulo(...)
            else if ( xml.name() == "Modulo" )
            {
                lendo_modulo = true;
                processar_modulos( xml, modulos );
            }
            else
            {
                xml.raiseError( "Tag <b>" + xml.name().toString() + "</b> "
                                "inválida." );
            }
        }
        // Se for o fim de um elemento, verificar se e o fim de um modulo ou se
        // e o fim da tag Simddef
        else if ( token == QXmlStreamReader::EndElement )
        {
            if ( lendo_modulo && xml.name() != "Modulo" )
            {
                xml.raiseError( "Esperado o fim da tag <b>Modulo</b>." );
            }
            else if ( !lendo_modulo && xml.name() != "Simddef" )
            {
                xml.raiseError( "Esperado o fim da tag <b>Simddef</b>." );
            }

            lendo_modulo = false;
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

    return modulos;
}


void ManipuladorXml :: processar_arquivos( const int &num_arqs,
                                           QXmlStreamReader &xml, 
                                           QStringList &arquivos )
{
    for ( int i = 0 ; i < num_arqs ; i++ )
    {
        xml.readNext();

        // Ignorando os espacos em branco antes do inicio da tag
        while ( xml.isWhitespace() )
            xml.readNext();

        if ( xml.tokenType() != QXmlStreamReader::StartElement )
        {
            xml.raiseError( "Não foi possível ler os arquivos. Verifique se "
                            "a tag <b>arq</b> está corretamente configurada." );
            arquivos.clear();
            return;
        }

        QString nome_tag = xml.name().toString();

        if ( nome_tag != "arq" )
        {
            xml.raiseError( "Tag <b>" + nome_tag + "</b> inválida." );
            arquivos.clear();
            return;
        }

        QXmlStreamAttributes atribs = xml.attributes();

        if ( !atribs.hasAttribute( "end" ) )
        {
            xml.raiseError( "Atributo <b>end</b> não definido para a tag "
                            "<b>arq</b>." );
            arquivos.clear();
            return;
        }

        arquivos << atribs.value( "end" ).toString();

        // Fim da tag
        xml.readNext();

        if ( xml.tokenType() != QXmlStreamReader::EndElement )
        {
            xml.raiseError( "Fim da tag <b>arq</b> não encontrado." );
            arquivos.clear();
            return;
        }
    }
}


// Dependendo da situacao, pode ser mais conveniente utilizar os metodos
// readElementText(), readNextStartElement()... No caso do processamento das
// falhas e dos modulos foi preferivel tratar todos os elementos atraves de
// readNext para gerar as strings de erro corretamente.
void ManipuladorXml :: processar_falhas( QXmlStreamReader &xml,
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

        // Testando se a tag e ou nao valida
        if ( nome_tag == "local" || 
             nome_tag == "abrv" || 
             nome_tag == "descricao" )
        {
            xml.readNext();

            if( xml.tokenType() != QXmlStreamReader::Characters )
            {
                xml.raiseError( "Erro no processamento da falha. Esperado o "
                                "valor da tag <b>" + nome_tag + "</b>." );
                return;
            }
        }
        else
        {
            xml.raiseError( "Tag <b>" + nome_tag + "</b> inválida" );
            return;
        }

        valores << xml.text().toString();
        
        xml.readNext();
        
        if ( xml.tokenType() != QXmlStreamReader::EndElement )
        {
            xml.raiseError( "Erro no processamento da falha. Fim da tag "
                            "<b>" + nome_tag + "</b> inválido." );
            return;
        }
    }

    falhas << valores;
}


void ManipuladorXml :: processar_modulos( QXmlStreamReader &xml, 
                                          QList< Modulo * > &modulos )
{
    Modulo *modulo;

    QString nome_tag = "";

    QStringList arquivos;

    // Cada modulo possui 3 tags internas
    for ( int c = 0 ; c < 3 ; c++ )
    {
        xml.readNext();

        // Ignorando os espacos em branco antes do inicio da tag
        while ( xml.isWhitespace() )
            xml.readNext();

        // Inicio da tag
        if ( xml.tokenType() != QXmlStreamReader::StartElement )
        {
            xml.raiseError( "Erro no processamento do módulo. Início da tag "
                            "<b>" + xml.name().toString() + "</b> inválido." );
            return;
        }
       
        // Valores da tag
        nome_tag = xml.name().toString();

        // Tipo do modulo
        if ( nome_tag == "tipo" )
        {
            xml.readNext();
            
            if( xml.tokenType() != QXmlStreamReader::Characters )
            {
                xml.raiseError( "Erro no processamento da falha. Esperado o "
                                "valor da tag <b>" + nome_tag + "</b>." );
                return;
            }

            // RNA
            if ( xml.text().toString() == "RNA" )
            {
                modulo = new Rede;
            }
            // TODO Fuzzy
            // TODO Estatistico
            // TODO Personalizado
        }
        // Falha  a ser detectada
        else if ( nome_tag == "falha" )
        {
            xml.readNext();
            
            if( xml.tokenType() != QXmlStreamReader::Characters )
            {
                xml.raiseError( "Erro no processamento da falha. Esperado o "
                                "valor da tag <b>" + nome_tag + "</b>." );
                return;
            }

            modulo->config_falha( xml.text().toString() );
        }
        // Arquivos a serem utilizados pelo modulo
        else if ( nome_tag == "arquivos" )
        {
            QXmlStreamAttributes atribs = xml.attributes();

            if ( !atribs.hasAttribute( "qtde" ) )
            {
                xml.raiseError( "Atributo <b>qtde</b> não definido para a tag "
                                "<b>" + nome_tag + "</b>." );
                return;
            }

            bool ok = true;

            int num_arqs = atribs.value( "qtde" ).toString().toInt( &ok );

            if ( !ok )
            {
                xml.raiseError( "Valor do atributo <b>qtde</b> inválido." );
                return;
            }

            processar_arquivos( num_arqs, xml, arquivos );

            // Houve um erro no processamento dos arquivos. String de erro ja
            // configurada pelo metodo processar_arquivos(...)
            if ( arquivos.count() == 0 )
                return;

            modulo->config_arquivos( arquivos );

            // Limpando a lista de arquivos para o proximo modulo
            arquivos.clear();
        }
        else
        {
            xml.raiseError( "Tag <b>" + nome_tag + "</b> inválida" );
            return;
        }
        
        // Fim da tag
        xml.readNext();
        
        // Ignorando os espacos em branco
        while ( xml.isWhitespace() )
            xml.readNext();

        if ( xml.tokenType() != QXmlStreamReader::EndElement )
        {
            xml.raiseError( "Erro no processamento do modulo. Fim da tag "
                            "<b>" + nome_tag + "</b> inválido." );
            return;
        }
    }

    // Adicionando o modulo a lista de modulos
    modulos << modulo;
}

#endif
