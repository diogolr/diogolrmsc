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
using std::cout;
void JanelaPrincipal :: on_botao_clicked()
{
    Rede r( "E:/documentos/diogolrmsc@ggc/softwares/simddef/cfgs_rnas/FADG/O2N8T3_RNA.cfg", 
            "E:/documentos/diogolrmsc@ggc/softwares/simddef/cfgs_rnas/FADG/O2N8T3_RNA.lim",
            "E:/documentos/diogolrmsc@ggc/softwares/simddef/cfgs_rnas/FADG/ent_rna_detec_linhas_296_a_305.dat" );

//    Matrix< double > saida = r.executar();

//    cout << saida;
}

#endif
