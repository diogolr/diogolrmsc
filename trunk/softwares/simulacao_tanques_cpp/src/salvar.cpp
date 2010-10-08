#ifndef SALVAR_CPP_
#define SALVAR_CPP_

#include "salvar.h"

Salvar :: Salvar( QWidget *pai, const TipoJanela &tipo ) : QDialog ( pai )
{
    ui = new Ui_Salvar;

    ui->setupUi( this );

    tipo_janela = tipo;

    inicializar();
}


Salvar :: ~Salvar()
{
    delete ui;
}


QStringList Salvar :: nomes_arquivos()
{
    arquivos.clear();

    // Real - Niveis
    if ( ui->chb_real_niveis->isChecked() )
    {
        arquivos << ui->end_real_niveis->text();
    }
    else
    {
        arquivos << "";
    }

    // Real - Erro / Sin. Cont.
    if ( ui->chb_real_erro_sc->isChecked() )
    {
        arquivos << ui->end_real_erro_sc->text();
    }
    else
    {
        arquivos << "";
    }

    // Simulacao - Niveis
    if ( ui->chb_sim_niveis->isChecked() )
    {
        arquivos << ui->end_sim_niveis->text();
    }
    else
    {
        arquivos << "";
    }
    
    // Simulacao - Erro / Sin. Cont.
    if ( ui->chb_sim_erro_sc->isChecked() )
    {
        arquivos << ui->end_sim_erro_sc->text();
    }
    else
    {
        arquivos << "";
    }

    return arquivos;
}


void Salvar :: config_tipo_janela( const TipoJanela &tipo )
{
    tipo_janela = tipo;
}


void Salvar :: inicializar()
{
    QString pasta_corrente = QDir::currentPath() + "/";

    switch ( tipo_janela )
    {
        default:
        case Salvar::SalvarDados:
            ui->end_real_niveis->setText( pasta_corrente + 
                                          "real_niveis.dat" );
            ui->end_real_erro_sc->setText( pasta_corrente + 
                                           "real_erro_sc.dat" );
            ui->end_sim_niveis->setText( pasta_corrente + 
                                         //"sim_niveis.dat" );
                                         "niveis_qualif.dat" );
            ui->end_sim_erro_sc->setText( pasta_corrente + 
                                         //"sim_erro_sc.dat" );
                                          "erro_sc_qualif.dat" );

            extensoes = "Arquivo de dados (*.dat)";
            break;

        case Salvar::ExportarGraficos:
            ui->end_real_niveis->setText( pasta_corrente + 
                                          "real_niveis.svg" );
            ui->end_real_erro_sc->setText( pasta_corrente + 
                                           "real_erro_sc.svg" );
            ui->end_sim_niveis->setText( pasta_corrente + 
                                         "sim_niveis.svg" );
            ui->end_sim_erro_sc->setText( pasta_corrente + 
                                          "sim_erro_sc.svg" );
            
            extensoes = "Bitmap (*.bmp);;"
                        "Joint Photographic Experts Group (*.jpg);;"
                        "Portable Document Format (*.pdf);;"
                        "Portable Network Graphics (*.png);;"
                        "PostScript (*.ps);;"
                        "Scalable Vector Graphics (*.svg)";
            break;
    }
}

        
void Salvar :: on_selec_real_niveis_clicked()
{
    QString nome = QFileDialog::getSaveFileName( this, 
                                                 "Salvar como...",
                                                 "real_niveis",
                                                 extensoes );

    if ( nome == "" )
        return;

    ui->end_real_niveis->setText( nome );
}


void Salvar :: on_selec_real_erro_sc_clicked()
{
    QString nome = QFileDialog::getSaveFileName( this, 
                                                 "Salvar como...",
                                                 "real_erro_sc",
                                                 extensoes );
    
    if ( nome == "" )
        return;

    ui->end_real_erro_sc->setText( nome );
}


void Salvar :: on_selec_sim_niveis_clicked()
{
    QString nome = QFileDialog::getSaveFileName( this, 
                                                 "Salvar como...",
                                                 "sim_niveis",
                                                 extensoes );
    
    if ( nome == "" )
        return;

    ui->end_sim_niveis->setText( nome );
}


void Salvar :: on_selec_sim_erro_sc_clicked()
{
    QString nome = QFileDialog::getSaveFileName( this, 
                                                 "Salvar como...",
                                                 "sim_erro_sc",
                                                 extensoes );
    
    if ( nome == "" )
        return;

    ui->end_sim_erro_sc->setText( nome );
}

#endif
