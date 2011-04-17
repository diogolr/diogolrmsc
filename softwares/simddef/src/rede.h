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
using FANN::activation_function_enum;
using FANN::neural_net;
using FANN::connection;

#include "excecoes.h"
#include "funcoes.h"
#include "matriz.h"

class Rede
{
    // Atributos publicos
    public:
        // enum TipoRede{ MLP = 0 };
        // enum FuncaoAtivacao{ Linear = 0, TgHiperbolica, Sigmoidal };

    // Metodos
    public:
        Rede( const QString &, const QString &, const QString & );
        ~Rede();
        
        Matriz< double > executar();

    private:
        Rede();
        
        void configurar_funcoes_ativacao();
        void configurar_pesos();
        void criar_rede();
        void desnormalizar( double * );
        void inicializar();
        void ler_entrada( const QString & );
        void ler_limites( const QString & );
        void ler_rede( const QString & );
        void normalizar( double * );
    
    // Atributos
    private:
        
        Matriz< double > *entrada;
        Matriz< double > *saida;

        QVector< char > f_ativacao;
        QVector< Matriz< double > * > pesos;
        QVector< Matriz< double > * > biases;
        QVector< double > x_min;
        QVector< double > x_max;
        QVector< double > x_range;
        QVector< double > y_min;
        QVector< double > y_max;
        QVector< double > y_range;
        QVector< uint > n_neuronios;

        // Rede neural da biblioteca FANN
        neural_net rede;
        
        uint n_amostras;
        uint n_camadas;
        uint n_conexoes;
        uint n_entradas;
        uint n_saidas;
};

#endif
