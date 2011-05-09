#ifndef EXCECOES_H_
#define EXCECOES_H_

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

class ExcecaoAssociacao : public Excecao
{
    // Metodos
    public:
        ExcecaoAssociacao( const QString &e ) : Excecao( e ){ /* Vazio */ }
};

class ExcecaoConjunto : public Excecao
{
    // Metodos
    public:
        ExcecaoConjunto( const QString &e ) : Excecao( e ){ /* Vazio */ }
};

class ExcecaoConversao : public Excecao
{
    // Metodos
    public:
        ExcecaoConversao( const QString &e ) : Excecao( e ){ /* Vazio */ }
};

class ExcecaoCurva : public Excecao
{
    // Metodos
    public:
        ExcecaoCurva( const QString &e ) : Excecao( e ){ /* Vazio */ }
};

class ExcecaoDeteccao : public Excecao
{
    // Metodos
    public:
        ExcecaoDeteccao( const QString &e ) : Excecao( e ){ /* Vazio */ }
};

class ExcecaoFuncao : public Excecao
{
    // Metodos
    public:
        ExcecaoFuncao( const QString &e ) : Excecao( e ){ /* Vazio */ }
};

class ExcecaoLeituraXML : public Excecao
{
    // Metodos
    public:
        ExcecaoLeituraXML( const QString &e ) : Excecao( e ){ /* Vazio */ }
};

class ExcecaoTamanho : public Excecao
{
    // Metodos
    public:
        ExcecaoTamanho( const QString &e ) : Excecao( e ){ /* Vazio */ }
};

#endif
