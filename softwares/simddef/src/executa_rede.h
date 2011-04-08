#ifndef EXECUTA_REDE_H_
#define EXECUTA_REDE_H_

#include "matriz.h"

#include <QFile>
#include <QIODevice>
#include <QVector>
#include <QRegExp>
#include <QString>
#include <QStringList>
#include <QTextStream>

class ExecutaRede
{
    // Metodos
    public:
        ExecutaRede( const QString &, const QString & );
        ~ExecutaRede();

    private:
//        ler_arquivo();
    
    // Atributos
    private:
        vector< Matriz<double> > matrizes;
};

#endif
