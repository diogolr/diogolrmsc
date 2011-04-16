#ifndef MATRIZ_CPP_
#define MATRIZ_CPP_

#include "matriz.h"

// TODO corrigir comentarios do codigo... Metade em ingles, metade em portugues
// nao da :)

// Construtores ----------------------------------------------------------------
/*! Matrizes quadradas
 *  @param tam Dimensao
 */
template< class T >
Matriz<T> :: Matriz( const int &tam )
{
    this->lin = tam;
    this->col = tam;
    
    this->matriz = new T [tam*tam];
    this->zero();
}

/*! Matrizes definidas
 *  @param i Numero de linhas
 *  @param j Numero de colunas
 */
template< class T >
Matriz<T> :: Matriz( const int &i , const int &j )
{
    this->lin = i;
    this->col = j;
    
    this->matriz = new T [lin*col];
    this->zero();
}

/*! Construtor copia
 *  @param mat Matriz a ser copiada
 */
template< class T >
Matriz<T> :: Matriz( const Matriz<T> &mat )
{
    this->lin = mat.linhas();
    this->col = mat.colunas();
    
    this->matriz = new T [lin*col];

    for ( int i = 0 ; i < this->lin ; ++i)
    {
        for ( int j = 0 ; j < this->col ; ++j )
        {
            (*this)(i,j) = mat(i,j);
        }
    }
}

// Destrutor -------------------------------------------------------------------
/*! Desaloca a memoria associada a matriz
 */
template< class T >
Matriz<T> :: ~Matriz<T>()
{
    delete []matriz;
}

// Transformacoes --------------------------------------------------------------
/*! Modifica todos os elementos da matriz para zero
 */
template< class T >
void Matriz<T> :: zero()
{
    for ( int i = 0 ; i < this->lin ; ++i )
    {
        for ( int j = 0 ; j < this->col ; ++j )
        {
            (*this)(i,j) = 0;
        }
    }
}

/*! Faz a matriz ser igual a matriz identidade de mesma dimensao
 */
template< class T >
void Matriz<T> :: identidade()
{
    if ( this->lin == this->col )
    {   
        for ( int i = 0 ; i < lin ; ++i )
        {
            for ( int j = 0 ; j < col ; ++j )
            {
                if ( i == j )
                {
                    this->matriz [i*col + j] = 1;
                }
                else
                {
                    this->matriz [i*col + j] = 0;
                }
            }
        }
    }
    else 
    {
        throw ExcecaoMatrizQuadrada( "Matriz não quadrada." );
    }
}

// Accesso ---------------------------------------------------------------------
/*! Numero de linhas
 *  @return Numero de linhas
 */
template< class T >
int Matriz<T> :: linhas() const
{
    return this->lin;
}

/*! Numero de colunas
 *  @return Numero de colunas
 */
template< class T >
int Matriz<T> :: colunas() const
{
    return this->col;
}

/*! Retorna um ponteiro para o inicio da linha i
 *  @param i Linha a ser retornada
 */
template< class T >
const T* Matriz<T> :: operator[] ( const int &i ) const
{
    if ( i < this->lin && i > 0 )
    {
        return &this->matriz[i*col];
    }
    else 
    {
        throw ExcecaoDimensao( "Dimensão inválida." );
    }
}

/*! Retorna um ponteiro para o inicio da linha i.
 *  @param i Row to return.
 */
template< class T >
T* Matriz<T> :: operator[] ( const int &i )
{
    if ( i < this->lin && i > 0 )
    {
        return &this->matriz[i*col];
    }
    else
    {
        throw ExcecaoDimensao( "Dimensão inválida." );
    }
}

/*! Returns the reference to the element.
 *  @param i Row of the element.
 *  @param j Column of the element.
 *  @return The reference to the element.
 */
template< class T >
T& Matriz<T> :: operator() ( const int &i , const int &j )
{
    if ( i >= this->lin || i < 0 )
        throw ExcecaoDimensao( "Dimensão inválida." );
    else if ( j >= this->col || j < 0)
        throw ExcecaoDimensao( "Dimensão inválida." );
    
    return this->matriz[i*col + j];
}

/*! Read a value of the matriz.
 *  @param i Row of the element.
 *  @param j Column of the element.
 *  @return Element in row i and column j.
 */
template< class T >
T Matriz<T> :: operator() ( const int & i , const int & j ) const
{
    if ( i >= this->lin || i < 0 )
        throw ExcecaoDimensao( "Dimensão inválida." );
    else if ( j >= this->col || j < 0)
        throw ExcecaoDimensao( "Dimensão inválida." );
    
    return this->matriz [i*col + j];
}


// Exibicao
template< class T2 >
ostream& operator<< ( ostream &o, Matriz<T2> &m )
{
    for ( int i = 0 ; i < m.linhas() ; ++i )
    {
        for ( int j = 0 ; j < m.colunas() ; ++j )
        {
            o << m(i,j) << "\t";
        }
        o << "\n";
    }

    return o;
}


// Atribuicao ------------------------------------------------------------------
/*! Creates a copy of a matriz.
 *  @param rhs Matriz to be copied.
 *  @return The new Matriz.
 */
template< class T >
Matriz<T>& Matriz<T> :: operator= ( const Matriz<T> & rhs )
{
    delete this->matriz;

    this->lin = rhs.linhas();
    this->col = rhs.colunas();
    this->matriz = new T[lin*col];
    
    for ( int i = 0 ; i < this->lin ; ++i )
    {
        for ( int j = 0 ; j < this->col ; ++j )
        {
            (*this)(i,j) = rhs (i,j);
        }
    }

    Matriz<T> * pointer = this;

    return *pointer;
}

// Comparison ------------------------------------------------------------------
/*! Implements the == (equals) operator.
 *  @param rhs The Matriz<T> to be comparated.
 *  @return true if they are equals, else false.
 */
template< class T >
bool Matriz<T> :: operator== ( const Matriz<T> & rhs )
{
    for ( int i = 0 ; i < this->lin ; ++i )
    {
        for ( int j = 0 ; j < this->col ; ++j )
        {
            if ( (*this)(i,j) != rhs(i,j) )
            {
                return false;
            }
        }
    }
    
    return true;
}

/*! Implements the != (diferent) operator.
 *  @param rhs The Matriz<T> to be comparated.
 *  @return true if they are diferent, else false.
 */
template< class T >
bool Matriz<T> :: operator!= ( const Matriz<T> & rhs )
{
    return !(*this == rhs );
}

/*! Implements the < (less than) operator.
 *  @param rhs The Matriz<T> to be comparated.
 *  @return true if they are less than rhs, else false.
 */
template< class T >
bool Matriz<T> :: operator< ( const Matriz<T> & rhs )
{
    for ( int i = 0 ; i < this->lin ; ++i )
    {
        for ( int j = 0 ; j < this->col ; ++j )
        {
            if ( (*this)(i,j) >= rhs(i,j) )
            {
                return false;
            }
        }
    }
    
    return true;
}

/*! Implements the <= (less than or equals) operator.
 *  @param rhs The Matriz<T> to be comparated.
 *  @return true if they are less than os equals to rhs, else false.
 */
template< class T >
bool Matriz<T> :: operator<= ( const Matriz<T> & rhs )
{
    for ( int i = 0 ; i < this->lin ; ++i )
    {
        for ( int j = 0 ; j < this->col ; ++j )
        {
            if ( (*this)(i,j) > rhs(i,j) )
            {
                return false;
            }
        }
    }

    return true;
}

/*! Implements the > (greater than) operator.
 *  @param rhs The Matriz<T> to be comparated.
 *  @return true if they are greater than rhs, else false.
 */
template< class T >
bool Matriz<T> :: operator> ( const Matriz<T> & rhs )
{
    for ( int i = 0 ; i < this->lin ; ++i )
    {
        for ( int j = 0 ; j < this->col ; ++j )
        {
            if ( (*this)(i,j) <= rhs(i,j) )
            {
                return false;
            }
        }
    }

    return true;
}

/*! Implements the >= (greater than or equals) operator.
 *  @param rhs The Matriz<T> to be comparated.
 *  @return true if they are less than os equals to rhs, else false.
 */
template< class T >
bool Matriz<T> :: operator>= ( const Matriz<T> & rhs )
{
    for ( int i = 0 ; i < this->lin ; ++i )
    {
        for ( int j = 0 ; j < this->col ; ++j )
        {
            if ( (*this)(i,j) < rhs(i,j) )
            {
                return false;
            }
        }
    }

    return true;
}


// Atithmetic Operations ------------------------------------------------------
/*! This method implements the + (sum) operator for Matriz.
 *  @param rhs The Matriz<T> to be add.
 *  @return The new Matriz<T> with the sum of this and rhs.
 */
template< class T >
Matriz<T> Matriz<T> :: operator+ ( const Matriz<T> & rhs ) const
{
    
    if ( this->lin == rhs.linhas() && this->col == rhs.colunas() )
    {
        Matriz<T>* result;
        
        result = new Matriz<T>( this->lin , this->col );

        for ( int i = 0 ; i < this->lin ; ++i )
        {
            for ( int j = 0 ; j < this->col ; ++j )
            {
                (*result)(i,j) = (*this)(i,j) + rhs(i,j);
            }
        }
        return result;
    }
    else
    {
        throw ExcecaoDimensao( "Dimensões incompatíveis." );
    }
}

/*! This method implements the - (minus) operator for Matriz.
 *  @param rhs The Matriz<T> to be deducted.
 *  @return The new Matriz<T> with the detucted of rhs to this.
 */
template< class T >
Matriz<T> Matriz<T> :: operator- ( const Matriz<T> & rhs ) const
{
    if ( this->lin == rhs.linhas() && this->col == rhs.colunas() )
    {
        Matriz<T> *result = new Matriz<T>( this->lin , this->col );

        for ( int i = 0 ; i < this->lin ; ++i )
        {
            for ( int j = 0 ; j < this->col ; ++j )
            {
               (*result)(i,j) = (*this)(i,j) - rhs(i,j);
            }
        }
        
        return *result;
    }
    else
    {
        throw ExcecaoDimensao( "Dimensões incompatíveis." );
    }   
}

/*! This method implements the * (multiplication) operator for Matriz.
 *  @param rhs The Matriz<T> to be multiplicated.
 *  @return The new Matriz<T> with the multiplication of this and rhs.
 */
template< class T >
Matriz<T> Matriz<T> :: operator* ( const Matriz<T> & rhs ) const
{
    
    if ( this->col == rhs.linhas() )
    {
        Matriz<T>* result = NULL;

        result = new Matriz<T>( this->lin , rhs.colunas() );

        for ( int i = 0 ; i < result->linhas() ; ++i )
        {
            for ( int j = 0 ; j < result->colunas() ; ++j )
            {
                for ( int k = 0 ; k < this->col ; k++ )
                {
                    (*result)(i,j) = (*result)(i,j) + ( (*this)(i,k) * rhs(k,j) );
                }
            }
        }
        return *result;
    }
    else
    {
        throw ExcecaoDimensao( "Dimensões incompatíveis." );
    }
}

/*! This method implements the * (mult.) operator for Matriz<T> and a Scalar.
 *  @param rhs The T object that will multiply the Matriz.
 *  @return The new Matriz<T> with the elements multiplyed by T.
 */
template< class T >
Matriz<T> Matriz<T> :: operator* ( const T & rhs ) const
{
    Matriz<T>* result = NULL;
    result = new Matriz<T>( this->lin , this->col );

    for ( int i = 0 ; i < this->lin ; ++i )
    {
        for ( int j = 0 ; j < this->col ; ++j )
        {
            (*result)(i,j) = (*this)(i,j) * rhs;
        }
    }
      
    return *result;
}

/*! This method implements the / (division) operator for Matriz<T> and a Scalar.
 *  @param rhs The T object that will to divid the Matriz.
 *  @return The new Matriz<T> with the elements divided by rhs.
 */
template< class T >
Matriz<T> Matriz<T> :: operator/ ( const T & rhs ) const
{
    Matriz<T>* result;
    result = new Matriz<T>( this->lin , this->col );

    for ( int i = 0 ; i < this->lin ; ++i )
    {
        for ( int j = 0 ; j < this->col ; ++j )
        {
            (*result)(i,j) = (*this)(i,j) / rhs;
        }
    }
        
    return result;
}

/*! This method implements the - (inverse) operator for the Matriz's elements.
 *  @return The new Matriz<T> with the elements inverted.
 */
template< class T >
Matriz<T> Matriz<T> :: operator- () const
{
    Matriz<T>* result = NULL;
    result = new Matriz<T>( this->lin , this->col );

    for ( int i = 0 ; i < this->lin ; ++i )
    {
        for ( int j = 0 ; j < this->col ; ++j )
        {
            (*result)(i,j) = -( (*this)(i,j) );
        }
    }
        
    return result;
}


// Atithmetic Uptades ----------------------------------------------------------
/*! This method implements the += (increase) operator for Matriz.
 *  @param rhs The Matriz<T> wiht the values that will increase this Matriz.
 *  @return The this pointer value.
 */
template< class T >
Matriz<T> Matriz<T> :: operator+= ( const Matriz<T> & rhs )
{
    if ( this->lin == rhs.linhas() && this->col == rhs.colunas() )
    {
        for ( int i = 0 ; i < this->lin ; ++i )
        {
            for ( int j = 0 ; j < this->col ; ++j )
            {
                (*this)(i,j) = (*this)(i,j) + rhs(i,j);
            }
        }
        return *this;
    }
    else
    {
        throw ExcecaoDimensao( "Dimensões incompatíveis." );
    }
}

/*! This method implements the -= (dencrease) operator for Matriz.
 *  @param rhs The Matriz<T> wiht the values that will dencrease this Matriz.
 *  @return The this pointer value.
 */
template< class T >
Matriz<T> Matriz<T> :: operator-= ( const Matriz<T> & rhs )
{
    if ( this->lin == rhs.linhas() && this->col == rhs.colunas() )
    {
        for ( int i = 0 ; i < this->lin ; ++i )
        {
            for ( int j = 0 ; j < this->col ; ++j )
            {
                (*this)(i,j) = (*this)(i,j) - rhs(i,j);
            }
        }
        return *this;
    }
    else
    {
        throw ExcecaoDimensao( "Dimensões incompatíveis." );
    }
}

/*! This method implements the *= (mult.) operator for Matriz<T> by a Scalar.
 *  @param rhs The scalar value that will multiply all elements of the Matriz.
 *  @return The this pointer value.
 */
template< class T >
Matriz<T> Matriz<T> :: operator*= ( const T & rhs )
{
    for ( int i = 0 ; i < this->lin ; ++i )
    {
        for ( int j = 0 ; j < this->col ; ++j )
        {
            (*this)(i,j) = (*this)(i,j) * rhs;
        }
    }
    return *this;
}

/*! This method implements the \= (mult.) operator for Matriz<T> by a Scalar.
 *  @param rhs The scalar value that will devide all elements of the Matriz.
 *  @return The this pointer value.
 */
template< class T >
Matriz<T> Matriz<T> :: operator/= ( const T & rhs )
{
    for ( int i = 0 ; i < this->lin ; ++i )
    {
        for ( int j = 0 ; j < this->col ; ++j )
        {
            (*this)(i,j) = (*this)(i,j) / rhs;
        }
    }

    return *this;
}


// Other Matriz Operations -----------------------------------------------------
/*! This method implements the Transpose Function to the Matriz.
 *  @return The tranposed matriz.
 */
template< class T >
Matriz<T> Matriz<T> :: transposta() const
{
    Matriz<T> * result = new Matriz<T> ( this->col , this->lin );
    
    for ( int i = 0 ; i < result->linhas() ; ++i )
    {
        for ( int j = 0 ; j < result->colunas() ; ++j )
        {
            (*result)(i,j) = (*this)(j,i);
        }
    }
    
    return *result;
}


// Method based in http://www.phys.ufl.edu/~coldwell/class2K/cpp/GAUSSJ.CPP
/*! Realiza o calculo da matriz inversa.
 *  @return Matriz<T> inversa.
 */
template< class T >
Matriz<T> Matriz<T> :: inversa() const
{
    if ( this->lin == this->col )
    {   
        int i, j, k, l, ll, irow, icol,
            ipiv[this->lin], indxr[this->lin], indxc[this->lin];

        T big, temp, pivinv;
        Matriz<T>* a = new Matriz<T> ( *this );

        int n = this->lin;
    
        for(i=0;i<n;++i)
            ipiv[i]=0;
    
        for(i=0;i<n;++i)  /* DO 22  main loop over cols to be replaced */
        {
            big=0;
            for(j=0;j<n;++j)  /* DO 13 */
            {
                if(ipiv[j]!=1)
                {
                    for(k=0;k<n;k++) /* DO 12 */
                    {
                        if(ipiv[k]==0)
                        {
                            if( fabs( (*a)(j,k) ) >= big )
                            {
                                big=fabs( (*a)(j,k) );
                                irow=j;
                                icol=k;
                            }
                            else
                            {
                                if(ipiv[k]>1)
                                {
                                    throw ExcecaoOperacao( "Operação inválida "
                                          "durante a inversão matricial." );
                                }
                            }
                        } /* ENDIF OF IF THEN ELSE */
                    } /* 12 CONTINUE */
                } /* ENDIF OF IPIV(J).NE.1 */
            } /* 13 CONTINUE */
        
            ipiv[icol]+=1;/* DO 13 ABOVE */

            if(irow!=icol)
            {
                for (l=0;l<n;l++)
                {
                    temp=(*a)( irow, l);
                    (*a)(irow, l) = (*a)( icol, l);
                    (*a)(icol,l)=temp;
                } /* 14 CONTINUE */
            } /* ENDIF */
    
            indxr[i]=irow;
            indxc[i]=icol;
        
            if(a[icol][icol]==0)
            {
                throw ExcecaoOperacao( "Operação inválida [Matriz Singular]." );
            } /* ENDIF ON SINGULAR MATRIX */

            pivinv=1/(*a)(icol,icol);
            (*a)(icol,icol)=1;

            for (l=0;l<n;l++)
                (*a)(icol,l)*=pivinv; /* DO 16 LOOP */
      
            for (ll=0;ll<n;ll++)  /* DO 21 LOOP */
            {
                if(ll!=icol)
                {
                    temp = (*a)(ll, icol);
                    (*a)(ll, icol)=0;
                
                    for(l=0;l<n;l++)
                        (*a)(ll, l)-=(*a)(icol,l)*temp; /* DO 18 LOOP */
          
                } /* ENDIF */
            } /* 21 CONTINUE */
        } /* 22 CONTINUE  end of main loop over cols to be replaced */
   
        for (j=0;j<n;++j) /* DO 24 MAKES A AN INVERSE MATRIX*/
        {
            l=n-1-j;  /* a bit diff from l=n+1-j in fortran */
            if(indxr[l]!=indxc[l])
            {
                for (k=0;k<n;k++)
                {
                    temp=(*a)(k, indxr[l]);
                    (*a)(k, indxr[l])=(*a)(k, indxc[l]);
                    (*a)(k,indxc[l])=temp;
                }
            }
        }
    
        return *a;
    }
    else 
    {
        throw ExcecaoMatrizQuadrada( "Matriz não quadrada." );
    }
}

/*! This method calculate the Adjoint.
 *  @return Adjoint.
 */
template< class T >
Matriz<T> Matriz<T> :: adjunta() const
{
    return this->determinante() * this->inversa();
}


template< class T >
T Matriz<T> :: traco() const
{
    int n = this->linhas();

    if ( n != this->colunas() )
    {
        //TODO Exception
    }

    T traco = 0;

    for ( int i = 0 ; i < n ; ++i )
    {
        for ( int j = 0 ; j < n ; ++j )
        {
            if ( i == j )
                traco += (*this)(i,j);
        }
    }

    return traco;
}

/*! This method calculate the det for Matriz<T> using recursirve calcs.
 *  @return Matriz<T> determinants.
 */
template< class T >
T Matriz<T> :: determinante() const
{
    if ( this->lin == this->col )
    {
        if ( this->lin == 1 )
        {
            return (*this)(0,0);
        }
        else if ( this->lin == 2 )
        {
            return ((*this)(0,0) * (*this)(1,1)) -
                   ((*this)(1,0) * (*this)(0,1));
        }
        else if ( this->lin == 3 )
        {       
            return ((*this)(0,0) * (*this)(1,1) * (*this)(2,2)) + 
                   ((*this)(0,1) * (*this)(1,2) * (*this)(2,0)) +
                   ((*this)(0,2) * (*this)(1,0) * (*this)(2,1)) -
                   ((*this)(0,2) * (*this)(1,1) * (*this)(2,0)) -
                   ((*this)(1,2) * (*this)(2,1) * (*this)(0,0)) -
                   ((*this)(2,2) * (*this)(0,1) * (*this)(1,0));
        }
        else
        {                
            int dj = 0; // drumped column
           
            for ( dj = 0 ; dj < this->lin ; ++dj )
            {
                if ( (*this)(0,dj) != 0 )
                {
                    break;
                }
            }

            if ( dj == this->lin ) // First Line == 0?
            {
                return 0;
            }
                
            Matriz<T> mat( this->lin -1 , this->col -1);
                
            int c;

            for ( int i = 1 ; i < this->lin ; ++i )
            {
                for ( int j = 0 ; j < this->lin ; ++j )
                {
                    if ( j < dj )
                    {
                        c = j;
                    }
                    else
                    {
                        c = j - 1;
                    }

                    if ( j != dj )
                    {
                        mat(i-1,c) = (*this)(i,j) -
                                     ( (*this)(0,j) * (*this)(i,dj) );
                    }
                }
            }

            return (*this)(0,dj) * (dj%2 == 0 ? 1 : -1) * mat.determinante();
        }
    }
    else 
    {
        throw ExcecaoMatrizQuadrada( "Matriz não quadrada." );
    }
}


// Outras operacoes ------------------------------------------------------------
template< class T >
Matriz<T> Matriz<T> :: coluna( const int &n ) const
{
    if ( n < 0 )
    {
        //TODO Exception
    }

    if ( n >= this->col )
    {
        //TODO Exception
    }

    Matriz<T> *c = new Matriz<T> ( this->lin, 1 );

    for ( int i = 0 ; i < this->lin ; ++i )
    {
        (*c)( i, 0 ) = (*this)( i, n );
    }

    return *c;
}


template< class T >
Matriz<T> Matriz<T> :: linha( const int &n ) const
{
    if ( n < 0 )
    {
        //TODO Exception
    }

    if ( n >= this->lin )
    {
        //TODO Exception
    }

    Matriz<T> *l = new Matriz<T> ( 1, this->col );

    for ( int j = 0 ; j < this->col ; ++j )
    {
        (*l)( 0, j ) = (*this)( n, j );
    }

    return *l;
}

template< class T >
void Matriz<T> :: adicionar_coluna( const int &coluna, const Matriz<T> &c )
{
    if ( c.linhas() != this->lin )
    {
        throw( ExcecaoDimensao( "Dimensões incompatíveis." ) );
    }

    if ( c.colunas() != 1 )
    {
        throw( ExcecaoDimensao( "Dimensão inválida." ) );
    }

    Matriz<T> *nova = new Matriz<T> ( this->lin, this->col + 1 );

    bool adicionada = false;

    // copiando as linhas de this 
    for ( int j = 0 ; j < this->col + 1 ; ++j )
    {
        for ( int i = 0 ; i < this->lin ; ++i )
        {
            if ( j == coluna )
            {
                (*nova)(i,j) = c( i, 0 );
                
                if ( i == this->lin - 1 )
                    adicionada = true;
            }
            else
            {
                if ( !adicionada )
                {
                    (*nova)(i,j) = (*this)(i,j);
                }
                else
                {
                    (*nova)(i,j) = (*this)(i,j-1);
                }
            }
        }
    }

    *this = *nova;

    delete nova;
}


template< class T >
void Matriz<T> :: adicionar_linha( const int &linha, const Matriz<T> &l )
{
    if ( l.colunas() != this->col )
    {
        throw( ExcecaoDimensao( "Dimensões incompatíveis." ) );
    }

    if ( l.linhas() != 1 )
    {
        throw( ExcecaoDimensao( "Dimensão inválida." ) );
    }

    Matriz<T> *nova = new Matriz<T> ( this->lin + 1, this->col );

    bool adicionada = false;

    // copiando as linhas de this 
    for ( int i = 0 ; i < this->lin + 1 ; ++i )
    {
        for ( int j = 0 ; j < this->col ; ++j )
        {
            if ( i == linha )
            {
                (*nova)(i,j) = l( 0, j );

                if ( j == this->col - 1 )
                    adicionada = true;
            }
            else
            {
                if ( !adicionada )
                {
                    (*nova)(i,j) = (*this)(i,j);
                }
                else
                {
                    (*nova)(i,j) = (*this)(i-1,j);
                }
            }
        }
    }

    *this = *nova;

    delete nova;
}


// Adiciona regressores de acordo com o vetor de inteiros para cada coluna
// desejada. O numero de regressores de cada coluna devera ser especificado no
// vetor. Se o numero for maior que zero, serao adicionados X regressores, onde
// X se refere ao numero contido no vetor
template< class T >
void Matriz<T> :: adicionar_regressores( int *regressores ) 
{
    Matriz<T> *nova = new Matriz<T>( *this );

    int soma = 0;

    for ( int j = 0 ; j < this->col ; j++ )
    {
        if ( regressores[j] > 0 )
        {
            // Repetindo as colunas dos regressores
            for ( int k = 1 ; k <= regressores[j] ; k++ )
            {
                nova->adicionar_coluna( j + soma + k, this->coluna( j ) );

                nova->descer_coluna( j + soma + k, k, 0.0 );
            }

            soma += regressores[j];
        }
    }

    *this = *nova;

    delete nova;
}


template< class T >
void Matriz<T> :: descer_coluna( const int &c, const int &k, const T &valor )
{
    for ( int i = this->lin - 1 ; i >= 0 ; i-- )
    {
        if ( i >= k )
        {
            (*this)( i, c ) = (*this)( i - k, c );
        }
        else
        {
            (*this)( i, c ) = valor;
        }
    }
}


/* Based @ JAMA/C++ (see http://math.nist.gov/tnt/download.html)
 *
 * This is derived from the Algol procedures tred2 by Bowdler, Martin, Reinsch,
 * and Wilkinson, Handbook for Auto. Comp., Vol.ii-Linear Algebra, and the
 * corresponding Fortran subroutine in EISPACK.
 */
template< class T >
void Matriz<T> :: tred2( Matriz<T> *d, Matriz<T> *e )
{
    if ( (*d).linhas() != 1 )
    {
        // TODO Exception
    }

    if ( (*e).linhas() != 1 )
    {
        // TODO Exception
    }

    if ( (*d).colunas() != this->col || (*e).colunas() != this->col )
    {
        // TODO Exception
    }

    if ( this->lin != this->col )
    {
        // TODO Exception
    }

    int n = this->col;

    for (int j = 0; j < n; j++)
    {
         (*d)(0, j) = (*this)(n-1, j);
    }

    // Householder reduction to tridiagonal form.
    for (int i = n-1; i > 0; i--)
    {
        // Scale to avoid under/overflow.
        T scale = 0.0;
        T h = 0.0;

        for (int k = 0; k < i; k++)
        {
            scale = scale + fabs((*d)(0, k));
        }
        if (scale == 0.0)
        {
            (*e)(0, i) = (*d)(0, i-1);
            
            for (int j = 0; j < i; j++)
            {
                (*d)(0, j) = (*this)(i-1, j);
                (*this)(i, j) = 0.0;
                (*this)(j, i) = 0.0;
            }
        }
        else
        {
            // Generate Householder vector.
            for (int k = 0; k < i; k++)
            {
               (*d)(0, k) /= scale;
               h += (*d)(0, k) * (*d)(0, k);
            }

            T f = (*d)(0, i-1);
            T g = sqrt(h);

            if (f > 0)
            {
               g = -g;
            }

            (*e)(0, i) = scale * g;
            h = h - f * g;
            (*d)(0, i-1) = f - g;
            
            for (int j = 0; j < i; j++)
            {
                (*e)(0, j) = 0.0;
            }
   
            // Apply similarity transformation to remaining columns.
            for (int j = 0; j < i; j++)
            {
                f = (*d)(0, j);
                (*this)(j, i) = f;
                g = (*e)(0, j) + (*this)(j, j) * f;

                for (int k = j+1; k <= i-1; k++)
                {
                    g += (*this)(k, j) * (*d)(0, k);
                    (*e)(0, k) += (*this)(k, j) * f;
                }

                (*e)(0, j) = g;
            }

            f = 0.0;

            for (int j = 0; j < i; j++)
            {
                (*e)(0, j) /= h;
                f += (*e)(0, j) * (*d)(0, j);
            }

            T hh = f / (h + h);
            
            for (int j = 0; j < i; j++)
            {
                (*e)(0, j) -= hh * (*d)(0, j);
            }

            for (int j = 0; j < i; j++)
            {
                f = (*d)(0, j);
                g = (*e)(0, j);
                
                for (int k = j; k <= i-1; k++)
                {
                    (*this)(k, j) -= (f * (*e)(0, k) + g * (*d)(0, k));
                }

                (*d)(0, j) = (*this)(i-1, j);
                (*this)(i, j) = 0.0;
            }
        }
         
        (*d)(0, i) = h;
    }
   
    // Accumulate transformations.
    for (int i = 0; i < n-1; i++)
    {
        (*this)(n-1, i) = (*this)(i, i);
        (*this)(i, i) = 1.0;

        T h = (*d)(0, i+1);

        if (h != 0.0)
        {
            for (int k = 0; k <= i; k++)
            {
                (*d)(0, k) = (*this)(k, i+1) / h;
            }

            for (int j = 0; j <= i; j++)
            {
                T g = 0.0;
                for (int k = 0; k <= i; k++)
                {
                    g += (*this)(k, i+1) * (*this)(k, j);
                }

                for (int k = 0; k <= i; k++)
                {
                    (*this)(k, j) -= g * (*d)(0, k);
                }
            }
         
        }
        
        for (int k = 0; k <= i; k++)
        {
            (*this)(k, i+1) = 0.0;
        }
    }
    
    for (int j = 0; j < n; j++)
    {
        (*d)(0, j) = (*this)(n-1, j);
        (*this)(n-1, j) = 0.0;
    }
    
    (*this)(n-1, n-1) = 1.0;
    (*e)(0, 0) = 0.0;
}


/* Based @ JAMA/C++ (see http://math.nist.gov/tnt/download.html)
 *
 * This is derived from the Algol procedures tql2 by Bowdler, Martin, Reinsch,
 * and Wilkinson, Handbook for Auto. Comp., Vol.ii-Linear Algebra, and the
 * corresponding Fortran subroutine in EISPACK.
 */
template< class T >
void Matriz<T> :: tql2( Matriz<T> *d, Matriz<T> *e )
{
    int n = this->col;

    for (int i = 1; i < n; i++)
    {
        (*e)(0, i-1) = (*e)(0, i);
    }
    
    (*e)(0, n-1) = 0.0;
   
    T f = 0.0;
    T tst1 = 0.0;
    T eps = pow(2.0,-52.0);

    for (int l = 0; l < n; l++)
    {
        // Find small subdiagonal element
        T tmp = fabs((*d)(0, l)) + fabs((*e)(0, l)); 

        tst1 = max(tst1,tmp);
        int m = l;

        // Original while-loop from Java code
        while (m < n) 
        {
            if (fabs((*e)(0, m)) <= eps*tst1) 
                break;
            m++;
        }

        // If m == l, (*d)(0, l) is an eigenvalue,
        // otherwise, iterate.
   
        if (m > l)
        {
            int iter = 0;
            do
            {
                iter = iter + 1;  // (Could check iteration count here.)
   
                // Compute implicit shift
   
                T g = (*d)(0, l);
                T p = ((*d)(0, l+1) - g) / (2.0 * (*e)(0, l));
                T r = hypot(p,1.0);

                if (p < 0) 
                    r = -r;
              
                (*d)(0, l) = (*e)(0, l) / (p + r);
                (*d)(0, l+1) = (*e)(0, l) * (p + r);
                
                T dl1 = (*d)(0, l+1);
                T h = g - (*d)(0, l);

                for (int i = l+2; i < n; i++) 
                    (*d)(0, i) -= h;
              
                f = f + h;
   
                // Implicit QL transformation.
                p = (*d)(0, m);

                T c = 1.0;
                T c2 = c;
                T c3 = c;
                T el1 = (*e)(0, l+1);
                T s = 0.0;
                T s2 = 0.0;
                
                for (int i = m-1; i >= l; i--) 
                {
                    c3 = c2;
                    c2 = c;
                    s2 = s;
                    g = c * (*e)(0, i);
                    h = c * p;
                    r = hypot(p,(*e)(0, i));
                    
                    (*e)(0, i+1) = s * r;
                    s = (*e)(0, i) / r;
                    c = p / r;
                    p = c * (*d)(0, i) - s * g;
                    (*d)(0, i+1) = h + s * (c * g + s * (*d)(0, i));
                 
                    // Accumulate transformation.
                    for (int k = 0; k < n; k++) 
                    {
                        h = (*this)(k, i+1);
                        (*this)(k, i+1) = s * (*this)(k, i) + c * h;
                        (*this)(k, i) = c * (*this)(k, i) - s * h;
                    }
                }
              
                p = -s * s2 * c3 * el1 * (*e)(0, l) / dl1;
                (*e)(0, l) = s * p;
                (*d)(0, l) = c * p;
   
                // Check for convergence.
            } while ( fabs((*e)(0, l)) > eps*tst1 );
        }
        
        (*d)(0, l) = (*d)(0, l) + f;
        (*e)(0, l) = 0.0;
    }
     
    // Sort eigenvalues and corresponding vectors.
    for (int i = 0; i < n-1; i++) 
    {
        int k = i;
        T p = (*d)(0, i);
       
        for (int j = i+1; j < n; j++)
        {
            if ((*d)(0, j) < p)
            {
                k = j;
                p = (*d)(0, j);
            }
        }
       
        if (k != i) 
        {
            (*d)(0, k) = (*d)(0, i);
            (*d)(0, i) = p;

            for (int j = 0; j < n; j++) 
            {
                p = (*this)(j, i);
                (*this)(j, i) = (*this)(j, k);
                (*this)(j, k) = p;
            }
        }
    }
}


// Operacoes de estatistica
// -----------------------------------------------------------------------------
template< class T >
Matriz<T> Matriz<T> :: cov() const
{
    Matriz<T> covariancia( this->lin, this->lin );
    
    for ( int i = 0 ; i < this->lin ; ++i )
    {
        for ( int j = 0 ; j < this->lin ; ++j )
        {
            Matriz li ( this->linha( i ) );
            Matriz lj ( this->linha( j ) );

            T val = cov ( li, lj );

            // Como a matriz eh simetrica cov ( i, j ) = cov ( j, i )
            // Logo basta fazer a diagonal superior ( j > i )
            if ( j > i )
            {
                covariancia ( i, j ) = val;
                covariancia ( j, i ) = val;
            }
            else if ( i == j )
            {
                covariancia ( i, j ) = val;
            }
        }
    }

    return covariancia;
}


template< class T >
Matriz<T> Matriz<T> :: cor() const
{
    Matriz<T> correlacao( this->lin, this->lin );
    
    for ( int i = 0 ; i < this->lin ; ++i )
    {
        for ( int j = 0 ; j < this->lin ; ++j )
        {
            Matriz li ( this->linha( i ) );
            Matriz lj ( this->linha( j ) );

            T val = cor ( li, lj );

            // Como a matriz eh simetrica cor ( i, j ) = cor ( j, i )
            // Logo basta fazer a diagonal superior ( j > i )
            if ( j > i )
            {
                correlacao ( i, j ) = val;
                correlacao ( j, i ) = val;
            }
            else if ( i == j )
            {
                correlacao ( i, j ) = val;
            }
        }
    }

    return correlacao;
}


template< class T >
T Matriz<T> :: med( const Matriz<T> &x )
{
    int n = x.colunas();

    T media = 0;
    
    for ( int i = 0 ; i < n ; ++i )
    {
        media += x(0,i);
    }
    
    return media/n;
}


template< class T >
T Matriz<T> :: med( const Matriz<T> &x, const Matriz<T> &px )
{
    // TODO
    return 1;
}


template< class T >
T Matriz<T> :: desv_pad( const Matriz<T> &x )
{
    // variancia
    return sqrt ( var(x) );
}


template< class T >
T Matriz<T> :: var( const Matriz<T> &x )
{
    T media = med(x);
    T soma = 0;
    
    int n = x.colunas();
    
    for ( int i = 0 ; i < n ; ++i )
    {
        soma += pow ( x(0,i) - media, 2 );
    }

    return soma/(n-1);
}


template< class T >
T Matriz<T> :: cov( const Matriz<T> &x,  const Matriz<T> &y )
{
    int ncx = x.colunas();
    int ncy = y.colunas();

    if ( x.linhas() != 1 || y.linhas() != 1 )
    {
        // TODO exception
    }    
    
    if ( ncx != ncy )
    {
        // TODO exception
    }
    
    T medx = med(x);
    T medy = med(y);

    T soma = 0;

    for ( int i = 0 ; i < ncx ; ++i )
    {
        soma += ( x(0,i) - medx ) * ( y(0,i) - medy );
    }

    return soma/(ncx-1);
}


template< class T >
T Matriz<T> :: cor( const Matriz<T> &x,  const Matriz<T> &y )
{
    int ncx = x.colunas();
    int ncy = y.colunas();

    if ( x.linhas() != 1 || y.linhas() != 1 )
    {
        // TODO exception
    }    
    
    if ( ncx != ncy )
    {
        // TODO exception
    }
    
    // medias
    T medx = med(x);
    T medy = med(y);

    // variancias
    T s2xx = 0;
    T s2yy = 0;
    T s2xy = 0;

    for ( int i = 0 ; i < ncx ; ++i )
    {
        s2xx += pow ( x(0,i) - medx, 2 );
        s2yy += pow ( y(0,i) - medy, 2 );
        s2xy += ( x(0,i) - medx ) * ( y(0,i) - medy );
    }

    return s2xy / ( sqrt ( s2xx * s2yy ) );
}

#endif
