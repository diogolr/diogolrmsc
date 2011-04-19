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

class ExcecaoArquivo : public Excecao
{
    // Metodos
    public:
        ExcecaoArquivo( const QString &e ) : Excecao( e ){ /* Vazio */ }
};

class ExcecaoCampoInvalido : public Excecao
{
    // Metodos
    public:
        ExcecaoCampoInvalido( const QString &e ) : Excecao( e ){ /* Vazio */ }
};

class ExcecaoConversao : public Excecao
{
    // Metodos
    public:
        ExcecaoConversao( const QString &e ) : Excecao( e ){ /* Vazio */ }
};

class ExcecaoDimensao : public Excecao
{
    // Metodos
    public:
        ExcecaoDimensao( const QString &e ) : Excecao( e ){ /* Vazio */ }
};

class ExcecaoFuncao : public Excecao
{
    // Metodos
    public:
        ExcecaoFuncao( const QString &e ) : Excecao( e ){ /* Vazio */ }
};

class ExcecaoMatrizQuadrada : public Excecao
{
    // Metodos
    public:
        ExcecaoMatrizQuadrada ( const QString &e ) : Excecao( e ){ /* Vazio */ }
};

class ExcecaoOperacao : public Excecao
{
    // Metodos
    public:
        ExcecaoOperacao( const QString &e ) : Excecao( e ){ /* Vazio */ }
};

#endif
