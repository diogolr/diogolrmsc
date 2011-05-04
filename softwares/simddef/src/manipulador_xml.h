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
#include "rede.h"
// TODO incluir arquivos dos modulos Fuzzy, Estatistico, Personalizado ...

class ManipuladorXml
{
    // Metodos
    public:
        ManipuladorXml();
        ~ManipuladorXml();

        QList< QStringList > ler_falhas( const QString & );
        QList< Modulo * > ler_modulos( const QString & );

    private:
        void processar_arquivos( const int &, 
                                 QXmlStreamReader &, 
                                 QStringList & );
        void processar_falhas( QXmlStreamReader &,
                               QList< QStringList > & );
        void processar_modulos( QXmlStreamReader &,
                                QList< Modulo * > & );
        void processar_rede( QXmlStreamReader &, Rede * );
};

#endif
