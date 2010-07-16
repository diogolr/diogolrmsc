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

class ExcecaoAplicacao : public Excecao
{
    // Metodo
    public:
        ExcecaoAplicacao( const QString &e ) : Excecao( e ){ /* Vazio */ }
};

class ExcecaoConexao : public Excecao
{
    // Metodo
    public:
        ExcecaoConexao( const QString &e ) : Excecao( e ){ /* Vazio */ }
};

class ExcecaoEscrita : public Excecao
{
    // Metodo
    public:
        ExcecaoEscrita( const QString &e ) : Excecao( e ){ /* Vazio */ }
};

class ExcecaoLeitura : public Excecao
{
    // Metodo
    public:
        ExcecaoLeitura( const QString &e ) : Excecao( e ){ /* Vazio */ }
};

class ExcecaoCampoInvalido : public Excecao
{
    // Metodo
    public:
        ExcecaoCampoInvalido( const QString &e ) : Excecao( e ){ /* Vazio */ }
};

#endif
