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
    /* Teste da adicao de regressores ***********************************
    Matriz<double> mat( 5, 4 );

    // 3 2 1 3
    // 4 1 3 2
    // 1 2 1 1   (Matriz original)
    // 5 4 1 3
    // 3 3 2 4

    mat( 0, 0 ) = 3.0; mat( 0, 1 ) = 2.0; mat( 0, 2 ) = 1.0; mat( 0, 3 ) = 3.0;
    mat( 1, 0 ) = 4.0; mat( 1, 1 ) = 1.0; mat( 1, 2 ) = 3.0; mat( 1, 3 ) = 2.0;
    mat( 2, 0 ) = 1.0; mat( 2, 1 ) = 2.0; mat( 2, 2 ) = 1.0; mat( 2, 3 ) = 1.0;
    mat( 3, 0 ) = 5.0; mat( 3, 1 ) = 4.0; mat( 3, 2 ) = 1.0; mat( 3, 3 ) = 3.0;
    mat( 4, 0 ) = 3.0; mat( 4, 1 ) = 3.0; mat( 4, 2 ) = 2.0; mat( 4, 3 ) = 4.0;

    cout << mat << endl << endl;

    int x[] = { 1, 2, 4, 3 };

    mat.adicionar_regressores( x );

    cout << mat << endl << endl;
    */

    /* Teste da leitura do arquivo de entrada e da rede ***********************************
    */
/*    Rede r( "D:/documentos/diogolrmsc@ggc/softwares/simddef/cfgs_rnas/FADG/O2N8T3_RNA.cfg", 
            "D:/documentos/diogolrmsc@ggc/softwares/simddef/cfgs_rnas/FADG/ent_rna_detec.dat" );
    Rede r( "D:/documentos/diogolrmsc@ggc/softwares/simddef/cfgs_rnas/FADG/O2N8T3_RNA.cfg", 
            "D:/documentos/diogolrmsc@ggc/softwares/simddef/cfgs_rnas/FADG/ent_rna_detec_linha_300.dat" );*/
    Rede r( "D:/documentos/diogolrmsc@ggc/softwares/simddef/cfgs_rnas/FADG/O2N8T3_RNA.cfg", 
            "D:/documentos/diogolrmsc@ggc/softwares/simddef/cfgs_rnas/FADG/ent_rna_detec_linhas_296_a_305.dat" );

    Matriz< double > saida = r.para_frente();
}

#endif
