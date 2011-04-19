#ifndef REDE_H_
#define REDE_H_

#include <QDebug>

#include <QFile>
#include <QIODevice>
#include <QRegExp>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QVector>

#include <MultilayerPerceptron.h>
using Flood::MultilayerPerceptron;

#define LINEAR MultilayerPerceptron::Linear
#define LOGSIG MultilayerPerceptron::Logistic
#define TANH   MultilayerPerceptron::HyperbolicTangent

#include <Matrix.h>
using Flood::Matrix;

#include <Vector.h>
using Flood::Vector;

#include "excecoes.h"
#include "funcoes.h"

class Rede
{
    // Atributos publicos
    public:
        // enum TipoRede{ MLP = 0 };

    // Metodos
    public:
        Rede( const QString &, const QString &, const QString & );
        ~Rede();
        
        Matrix< double > executar();

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
        
        Matrix< double > entrada;
        Matrix< double > saida;
        
        // Rede neural da biblioteca Flood
        MultilayerPerceptron rede;

        QVector< char > f_ativacao;
        QVector< double > x_min;
        QVector< double > x_max;
        QVector< double > x_range;
        QVector< double > y_min;
        QVector< double > y_max;
        QVector< double > y_range;

        uint n_amostras;
        uint n_camadas;
        uint n_entradas;
        uint n_saidas;
        
        Vector< int > n_neuronios;
        Vector< Matrix< double > > pesos;
        Vector< Vector< double > > biases;
};

#endif
