#ifndef EXCECOES_H_
#define EXCECOES_H_

#include "funcoes.h"

#include <QString>

class Excecao
{
    // Metodos
    public:
        Excecao( const QString & = "Erro Genérico" );
        ~Excecao();

        QString msg_erro();

    // Atributos
    protected:
        QString erro;
};


class ExcecaoConversao : public Excecao
{
    // Metodo
    public:
        ExcecaoConversao( const QString &e ) : Excecao( e ){ /* Vazio */ }
};

class ExcecaoDimensao : public Excecao
{
    // Metodo
    public:
        ExcecaoDimensao( const QString &e ) : Excecao( e ){ /* Vazio */ }
};

class ExcecaoOperacao : public Excecao
{
    // Metodo
    public:
        ExcecaoOperacao( const QString &e ) : Excecao( e ){ /* Vazio */ }
};

class ExcecaoMatrizQuadrada : public Excecao
{
    // Metodo
    public:
        ExcecaoMatrizQuadrada ( const QString &e ) : Excecao( e ){ /* Vazio */ }
};

class ExcecaoCampoInvalido : public Excecao
{
    // Metodo
    public:
        ExcecaoCampoInvalido( const QString &e ) : Excecao( e ){ /* Vazio */ }
};

#endif
