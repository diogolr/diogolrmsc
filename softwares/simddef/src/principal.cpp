#ifndef PRINCIPAL_CPP_
#define PRINCIPAL_CPP_

#include "principal.h"

JanelaPrincipal :: JanelaPrincipal()
{
    ui = new Ui_JanelaPrincipal;

    ui->setupUi( this );
}


JanelaPrincipal :: ~JanelaPrincipal()
{
    delete ui;
}

#include <iostream>
using namespace::std;
void JanelaPrincipal :: on_botao_clicked()
{
    Matriz<double> mat( 5, 5 );

    mat( 0, 0 ) = 1.0; mat( 0, 1 ) = 2.0; mat( 0, 2 ) = 0.0;  mat( 0, 3 ) = 4.0; mat( 0, 4 ) = 3.0;
    mat( 1, 0 ) = 2.0; mat( 1, 1 ) = 1.0; mat( 1, 2 ) = -1.0; mat( 1, 3 ) = 9.0; mat( 1, 4 ) = 2.0;
    mat( 2, 0 ) = 3.0; mat( 2, 1 ) = 4.0; mat( 2, 2 ) = 2.0;  mat( 2, 3 ) = 5.0; mat( 2, 4 ) = 7.0;
    mat( 3, 0 ) = 4.0; mat( 3, 1 ) = 3.0; mat( 3, 2 ) = 3.5;  mat( 3, 3 ) = 1.0; mat( 3, 4 ) = 6.0;
    mat( 4, 0 ) = 5.0; mat( 4, 1 ) = 0.0; mat( 4, 2 ) = 43.0; mat( 4, 3 ) = 4.0; mat( 4, 4 ) = 1.0;

    cout << mat << endl << endl;

    Matriz<double> c( 5, 1 );

    c( 0, 0 ) = 1.3;
    c( 1, 0 ) = 2.3;
    c( 2, 0 ) = 3.3;
    c( 3, 0 ) = 4.3;
    c( 4, 0 ) = 5.3;
    
    mat = mat.adicionar_col( 4, c );

    cout << mat << endl << endl;

    int x[] = { 0, 2, 0, 3, 0, 0 };

    mat = mat.adicionar_regressores( x );

    cout << mat << endl << endl;
    
}

#endif
