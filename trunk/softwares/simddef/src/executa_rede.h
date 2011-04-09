#ifndef EXECUTA_REDE_H_
#define EXECUTA_REDE_H_

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

class ExecutaRede
{
    // Metodos
    public:
        ExecutaRede( const QString &, const QString & );
        ~ExecutaRede();

    private:
        ExecutaRede();

        void inicializar();
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
