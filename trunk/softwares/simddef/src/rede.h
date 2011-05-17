#ifndef REDE_H_
#define REDE_H_

#include <QDebug>

#include <QDialog>
#include <QFile>
#include <QHash>
#include <QIODevice>
#include <QList>
#include <QRegExp>
#include <QString>
#include <QStringList>
#include <QTextStream>

#include <Matrix.h>
#include <MultilayerPerceptron.h>
#include <Vector.h>
using Flood::MultilayerPerceptron;
using Flood::Matrix;
using Flood::Vector;

#include "excecoes.h"
#include "funcoes.h"
#include "modulo.h"

typedef Matrix< double > MatrizD;
typedef Matrix< int > MatrizI;
typedef Vector< double > VetorD;
typedef Vector< int > VetorI;

#define LINEAR MultilayerPerceptron::Linear
#define LOGSIG MultilayerPerceptron::Logistic
#define TANH   MultilayerPerceptron::HyperbolicTangent

#define INTERVALO_MIN_DETECCAO 3

class Rede : public Modulo
{
    // Atributos publicos
    public:
        // enum TipoRede{ MLP = 0 };

    // Metodos
    public:
        Rede();
        ~Rede();
        
        int ordem();

        QHash< int, QString > curvas_a_exibir();

        QString nome_tipo();

        void configurar_ordem( const int & );
        void ler_arquivos();

    private:
        MatrizD desnormalizar( const MatrizD &, const QList< QList< double > > & );
        MatrizD normalizar( const MatrizD &, const QList< QList< double > > & );

        void configurar_funcoes_ativacao();
        void configurar_pesos();
        void criar_rede();
        void desnormalizar( MatrizD * );
        void inicializar();
        void ler_entrada( const QString & );
        void ler_limites( const QString & );
        void ler_rede( const QString & );
        void normalizar( MatrizD * );

    protected:
        void processar_saida();
    
    // Atributos
    private:
        // Ordem da rede
        int ord;

        // Rede neural da biblioteca Flood
        MultilayerPerceptron rede;

        QList< char > f_ativacao;
        QList< double > x_min;
        QList< double > x_max;
        QList< double > x_range;
        QList< double > y_min;
        QList< double > y_max;
        QList< double > y_range;

        uint n_amostras;
        uint n_camadas;
        
        Vector< int > n_neuronios;
        Vector< MatrizD > pesos;
        Vector< VetorD > biases;
};

#endif
