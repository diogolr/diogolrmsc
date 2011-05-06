#ifndef MODULO_H_
#define MODULO_H_

#include <QHash>
#include <QString>
#include <QStringList>

#include <Matrix.h>
using Flood::Matrix;

typedef Matrix< double > MatrizD;
typedef Matrix< int > MatrizI;

class Modulo
{
    // Atributos publicos
    public:
        enum TipoModulo{ RNA = 0, Fuzzy, Estatistico, Personalizado };

    // Metodos publicos
    public:
        Modulo( const Modulo::TipoModulo &tipo = Modulo::RNA );
        ~Modulo();

        bool ativo();
        
        Modulo::TipoModulo tipo();

        QList< MatrizD > dados();

        QString nome_falha();

        QStringList endereco_arquivos();

        virtual QHash< int, QString > curvas_a_exibir() = 0;
        virtual QHash< QString, MatrizI > deteccoes_falhas();

        virtual QString nome_tipo() = 0;

        virtual void ler_arquivos() = 0;

        void configurar_arquivos( const QStringList & );
        void configurar_ativo( const bool & );
        void configurar_falha( const QString & );

    // Metodos protegidos
    protected:
        virtual void processar_saida() = 0;

    // Atributos
    protected:
        bool ativ;

        MatrizD entrada;
        MatrizD saida;

        QHash< QString, MatrizI > deteccoes;

        QString falha;

        QStringList arquivos;

        TipoModulo tipo_modulo;

        uint n_entradas;
        uint n_saidas;
};

#endif
