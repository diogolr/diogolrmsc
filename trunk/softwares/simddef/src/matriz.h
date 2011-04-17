#ifndef MATRIZ_H_
#define MATRIZ_H_
#include <QDebug>
#include <cmath>     // abs(), sqrt(), pow()

#include <algorithm> // min(), max()
using std::max;

#include <ostream>   // Saida padrao
using std::ostream;

#include <fstream>   // Saida para arquivo
using std::ifstream;
using std::ofstream;

#include <iostream>
using std::ios;

#include "excecoes.h"

template < class T >
class Matriz
{
    // Metodos
    private:
        Matriz<T>();

    public:
        // Construtores
        Matriz<T>( const int & );
        Matriz<T>( const int & , const int & );
        Matriz<T>( const Matriz<T> & );
        
        // Destrutor
        ~Matriz<T>();

        // Transformacoes
        void zero();
        void identidade();
        
        // Acessos
        int linhas() const;
        int colunas() const;
        T* operator[] ( const int & );
        const T* operator[] ( const int & ) const;
        T& operator() ( const int & , const int & );
        T operator() ( const int & , const int & ) const;

        // Exibicao
        template < class T2 >
        friend ostream& operator<< ( ostream &, Matriz<T2> & );

        // Atribuicao
        Matriz<T>& operator= ( const Matriz<T> & );

        // Comparacoes
        bool operator== ( const Matriz<T> & );
        bool operator!= ( const Matriz<T> & );
        bool operator< ( const Matriz<T> & );
        bool operator<= ( const Matriz<T> & );
        bool operator> ( const Matriz<T> & );
        bool operator>= ( const Matriz<T> & );

        // Operacoes aritmeticas
        Matriz<T> operator+ ( const Matriz<T> & ) const;
        Matriz<T> operator- ( const Matriz<T> & ) const;
        Matriz<T> operator* ( const Matriz<T> & ) const;
        Matriz<T> operator* ( const T & ) const;
        Matriz<T> operator/ ( const T & ) const;
        Matriz<T> operator- () const;

        Matriz<T> operator+= ( const Matriz<T> & );
        Matriz<T> operator-= ( const Matriz<T> & );
        Matriz<T> operator*= ( const T & );
        Matriz<T> operator/= ( const T & );
        
        // Operacoes com matrizes
        Matriz<T> transposta() const;
        Matriz<T> inversa() const;
        Matriz<T> adjunta() const;
        T traco() const;
        T determinante() const;

        // Operacoes de estatistica
        Matriz<T> cov() const;
        Matriz<T> cor() const;

        static T med( const Matriz<T> & );
        static T mediana( const Matriz<T> & ); // TODO
        static T var( const Matriz<T> & );
        static T desv_pad( const Matriz<T> & );
        static T med( const Matriz<T> &, const Matriz<T> & ); // TODO
        static T var( const Matriz<T> &, const Matriz<T> & ); // TODO
        static T desv_pad( const Matriz<T> &, const Matriz<T> & ); // TODO
        static T cov( const Matriz<T> &, const Matriz<T> & );
        static T cor( const Matriz<T> &, const Matriz<T> & );
        
        // Outros metodos
        Matriz<T> coluna( const int & ) const;
        Matriz<T> linha( const int & ) const;

        void adicionar_coluna( const int &, const Matriz<T> & );
        void adicionar_linha( const int &, const Matriz<T> & );
        void adicionar_regressores( int * );
        void descer_coluna( const int &, const int &, const T & );
        void tred2( Matriz<T> *, Matriz<T> * );
        void tql2( Matriz<T> *, Matriz<T> * );

    // Atributos
    private:
        T *matriz;
        int lin;
        int col;
};

// Classes templates nao podem ter a implementacao separada em .h e .cpp. Por
// esse motivo, o .cpp e adicionado logo abaixo do .h
#include "matriz.cpp"

#endif