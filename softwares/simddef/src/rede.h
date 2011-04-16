#ifndef REDE_H_
#define REDE_H_

#include <QFile>
#include <QIODevice>
#include <QRegExp>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QVector>

#include "doublefann.h"
#include "fann_cpp.h"
using FANN::neural_net;
using FANN::connection;

#include "excecoes.h"
#include "funcoes.h"
#include "matriz.h"

class Rede
{
    // Atributos publicos
    public:
        // enum TipoRede{ NNARX = 0 };
        // enum FuncaoAtvacao{ Linear = 0, TgHiperbolica, Sigmoidal };

    // Metodos
    public:
        Rede( const QString &, const QString & );
        ~Rede();
        
        Matriz< double > para_frente();

    private:
        Rede();
        
        double funcao_ativacao( const int &, const double & );

        void configurar_pesos();
        void criar_rede();
        void inicializar();
        void ler_entrada( const QString & );
        void ler_rede( const QString & );
    
    // Atributos
    private:
        int n_amostras;
        int n_camadas;
        int n_entradas;
        
        Matriz< double > *entrada;

        QVector< int > n_neuronios;
        QVector< char > f_ativacao;
        QVector< Matriz< double > * > pesos;
        QVector< Matriz< double > * > biases;

        // Rede neural da biblioteca FANN
        neural_net rede;
};

#endif
