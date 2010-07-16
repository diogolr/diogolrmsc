#ifndef GRAFICO_CPP_
#define GRAFICO_CPP_

#include "grafico.h"

Grafico :: Grafico( QWidget *p ) : QwtPlot( p ), pai( p )
{
    configurar();
}


Grafico :: ~Grafico()
{
    for ( int i = 0 ; i < curvas.size() ; i++ )
    {
        delete x[i];
        delete y[i];
        delete curvas[i];
    }

    x.clear();
    y.clear();
    curvas.clear();
}


QwtPlotCurve * Grafico :: curva( const int &curva )
{
    if ( curva < 0 || curva >= x.size() )
    {
        exibir_mensagem( (QWidget *) this->parent(),
                         "Erro", 
                         "A curva especificada não existe", 
                         Aviso );
        return NULL;
    }

    return curvas[curva];
}


void Grafico :: adicionar_curva( const QPen &linha, const QString &titulo )
{
    QwtPlotCurve *curva;

    QVector< double > *x_pontos;
    QVector< double > *y_pontos;

    x_pontos = new QVector< double >();
    y_pontos = new QVector< double >();

    x.push_back( x_pontos );
    y.push_back( y_pontos );

    curva = new QwtPlotCurve( utf8( titulo ) );

    curva->setRenderHint( QwtPlotItem::RenderAntialiased );
    curva->setPen( linha );

    curvas.push_back( curva );

    curvas[ curvas.size() - 1 ]->attach( this );
}


void Grafico :: adicionar_xy( const int &c, 
                              const double &novo_x, 
                              const double &novo_y,
                              const bool &replote )
{
    if ( c < 0 || c >= x.size() )
    {
        exibir_mensagem( (QWidget *) this->parent(),
                         "Erro", 
                         "A curva especificada não existe", 
                         Aviso );
        return;
    }

    x[c]->push_back( novo_x );
    y[c]->push_back( novo_y );

    curvas[c]->setData( x[c]->data(), y[c]->data(), x[c]->size() );

    if ( replote )
        this->replot();
}


void Grafico :: configurar_legenda( QwtLegend *legenda, 
                                    const QwtPlot::LegendPosition &posicao )
{
    this->insertLegend( legenda, posicao );
    legenda->setItemMode( QwtLegend::CheckableItem );
}


void Grafico :: configurar_limites( const QwtPlot::Axis &eixo, 
                                    const double &min,
                                    const double &max )
{
    this->setAxisScale( eixo, min, max );
}


void Grafico :: configurar_nome_eixo( const QwtPlot::Axis &eixo, 
                                      const QwtText &texto )
{
    this->setAxisTitle( eixo, texto );
}


void Grafico :: configurar_titulo( const QwtText &texto )
{
    this->setTitle( texto );
}


void Grafico :: exportar( const QString &nome_arquivo )
{
    // Considera-se que todas as extensoes dos arquivos possuem 3 caracteres.
    QString extensao = nome_arquivo.right( 3 );

    QwtPlotPrintFilter filtro;

    int opcoes = ( QwtPlotPrintFilter::PrintAll & 
                   ~QwtPlotPrintFilter::PrintBackground ) |
                 QwtPlotPrintFilter::PrintFrameWithScales;

    filtro.setOptions( opcoes );

    if ( extensao == "svg" )
    {
        QSvgGenerator gerador;

        gerador.setFileName( nome_arquivo );
        gerador.setSize( this->size() );

        this->print( gerador, filtro );
    }
    else if ( extensao == "pdf" || extensao == "ps" )
    {
        /*
        QPrinter impressora;

        if ( extensao == "pdf" )
        {
            impressora.setOutputFormat( QPrinter::PdfFormat );
        }
        else
        {
            impressora.setOutputFormat( QPrinter::PostScriptFormat );
        }

        impressora.setOutputFileName( nome_arquivo );
        impressora.setCreator( utf8( "Controle para o Sistema de Tanques" ) );
        impressora.setOrientation( QPrinter::Landscape );

        this->print( impressora, filtro );
        */
    }
    else
    {
        QPixmap imagem( 2 * this->size() );

        if ( extensao == "png" )
        {
            imagem.fill( Qt::transparent );
        }
        else
        {
            imagem.fill( Qt::white );
        }

        this->print( imagem, filtro );

        imagem.save( nome_arquivo, qstring_char( extensao ), 100 );
    }
}


void Grafico :: limpar( const int &curva )
{
    if ( curva < -1 || curva >= x.size() )
    {
        exibir_mensagem( (QWidget *) this->parent(),
                         "Erro", 
                         "A curva especificada não existe", 
                         Aviso );
        return;
    }

    // Se nao for identificada nenhuma curva em especifico
    if ( curva == -1 )
    {
        for ( int i = 0 ; i < x.size() ; i++ )
        {
            x[i]->clear();
            y[i]->clear();
        }
    }
    else
    {
        x[curva]->clear();
        y[curva]->clear();
    }
}


void Grafico :: salvar( const QString &nome_arquivo,
                        const char &separador,
                        const int &c,
                        const bool &msm_valor_x )
{
    if ( c < -1 || c >= x.size() )
    {
        exibir_mensagem( (QWidget *) this->parent(),
                         "Erro", 
                         "A curva especificada não existe", 
                         Aviso );
        return;
    }

    ofstream of( qstring_char( nome_arquivo ), ios::out );

    if ( !of )
    {
        exibir_mensagem( (QWidget *) this->parent(),
                         "Erro", 
                         "Não foi possível abrir o arquivo para escrita.", 
                         Aviso );
        return;
    }
    
    // Se nao for identificada nenhuma curva em especifico
    if ( c == -1 )
    {
        // Se o X e o mesmo para todas as colunas, entao o numermo de
        // linhas devera ser o mesmo. Portanto, adota-se que o numero de
        // linhas (elementos) e o numero de linhas da primeira curva. 
        for ( int elemento = 0 ; elemento < x[0]->size() ; elemento ++ )
        {
            for ( int curva = 0 ; curva < y.size() ; curva++ )
            {
                // Se todos os valores de X (para todas as curvas) forem iguais,
                // nao ha necessidade de salvar os mesmos valores repetidas
                // vezes no arquivo de saida. Portanto, somente a primeira
                // coluna do arquivo de saida correspondera aos valores de X e o
                // restante das colunas serao compostas pelos valores de Y de
                // cada uma das curvas.
                if ( msm_valor_x )
                {
                    if ( curva == 0 )
                    {
                        of << (*x[curva])[elemento] << separador;
                    }
                }
                else
                {
                    of << (*x[curva])[elemento] << separador;
                }
                
                of << (*y[curva])[elemento];
               
                // Nao adicionar o separador na ultima coluna
                if ( curva != y.size() -1 )
                {
                    of << separador;
                }
            }

            of << endl;
        }
    }
    else
    {
        for ( int elemento = 0 ; elemento < x[0]->size() ; elemento ++ )
        {
            of << (*x[c])[elemento] << separador
               << (*y[c])[elemento] << endl;
        }
    }

    of.close();
}


void Grafico :: utilizar_grid( const bool &g )
{
    if ( g )
    {
        grid.attach( this );
    }
    else
    {
        grid.detach();
    }
}


void Grafico :: configurar()
{
    // Modificando as fontes dos eixos
    for ( int i = 0 ; i < QwtPlot::axisCnt ; i++ )
    {
        this->setAxisFont( i, pai->font() );
    }

    // Ajustando o canvas
    this->canvas()->setFrameStyle( QFrame::Box | QFrame::Plain );
    this->canvas()->setLineWidth( 0 );

    this->plotLayout()->setAlignCanvasToScales( true );

    // Grid
    QPen linha( Qt::lightGray );
    linha.setStyle( Qt::DotLine );
    grid.setPen( linha );
}

#endif
