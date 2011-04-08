#ifndef EXECUTA_REDE_CPP_
#define EXECUTA_REDE_CPP_

#include "executa_rede.h"

ExecutaRede :: ExecutaRede( const QString nome_arq_rede,
                            const QString nome_arq_ent )
{
    configurar_rede( nome_arq_ent );
    configurar_entrada( nome_arq_rede );
}


void ExecutaRede :: ler_arquivo( const QString nome_arq )
{
    QFile arquivo( nome_arq );

    if ( arquivo.open( QIODevice::ReadOnly ) )
    {
        QTextStream stream( &arquivo );

        QString linha = stream.readLine();

/* TODO
        QStringList numeros;

        while ( !linha.isNull() )
        {
            numeros = linha.split( QRegExp( " " ) );

            for ( int i = 0 ; i < numeros.size() ; i++ )
            {
                array[ numeros[i].toInt() - 1 ] += 1;
            }

            linha = stream.readLine();
        }
*/
        arquivo.close();
    }
}


#endif
