#ifndef MODULO_H_
#define MODULO_H_

#include <QString>
#include <QStringList>

#include <Matrix.h>
using Flood::Matrix;

typedef Matrix< double > MatrizD;

class Modulo
{
    // Atributos publicos
    public:
        enum TipoModulo{ RNA = 0, Fuzzy, Estatistico, Personalizado };

    // Metodos publicos
    public:
        Modulo( const Modulo::TipoModulo &tipo = Modulo::RNA );
        ~Modulo();
        
        Modulo::TipoModulo tipo();

        QString nome_falha();

        QStringList endereco_arquivos();

        virtual MatrizD executar() = 0;
        virtual QString nome_tipo() = 0;
        virtual void ler_arquivos() = 0;

        void config_arquivos( const QStringList & );
        void config_falha( const QString & );

    // Metodos protegidos
    protected:
        virtual void nome_entrada( const int &, const QString & );
        virtual void nome_saida( const int &, const QString & );
        virtual void nomes_entradas( const QStringList & );
        virtual void nomes_saidas( const QStringList & );

    // Atributos
    protected:
        MatrizD entrada;
        MatrizD saida;

        QString falha;

        QStringList arquivos;

        TipoModulo tipo_modulo;

        uint n_entradas;
        uint n_saidas;
};

#endif
