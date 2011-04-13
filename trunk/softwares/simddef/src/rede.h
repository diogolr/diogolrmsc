#ifndef REDE_H_
#define REDE_H_

#include <QFile>
#include <QIODevice>
#include <QRegExp>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QVector>

#include "excecoes.h"
#include "funcoes.h"
#include "matriz.h"

class Rede
{
    // Atributos publicos
    public:
        enum TipoRede{ NNARX = 0 };

    // Metodos
    public:
        Rede( const QString &, const QString & );
        ~Rede();

    private:
        Rede();

        void inicializar();
        void ler_entrada( const QString & );
        void ler_rede( const QString & );
    
    // Atributos
    private:
        int n_entradas;
        int n_camadas;
        
        Matriz< double > *entrada;

        QVector< int > n_neuronios;
        QVector< char > f_ativacao;
        QVector< Matriz< double > * > pesos;
        QVector< Matriz< double > * > biases;
};

#endif
