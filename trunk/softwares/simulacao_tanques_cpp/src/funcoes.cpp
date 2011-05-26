#ifndef FUNCOES_CPP_
#define FUNCOES_CPP_

#include "funcoes.h"

void exibir_mensagem( QWidget *pai,
                      const QString &titulo,
                      const QString &msg,
                      const TipoMensagem &tipo,
                      const QString &end_img )
{
    switch( tipo )
    {
        case Geral:
        case Informacao:
            QMessageBox::information( pai, utf8( titulo ), utf8( msg ) );
            break;

        case Pergunta:
            QMessageBox::question( pai, utf8( titulo ), utf8( msg ) );
            break;

        case Aviso:
            QMessageBox::warning( pai, utf8( titulo ), utf8( msg ) );
            break;

        case Critica:
            QMessageBox::critical( pai, utf8( titulo ), utf8( msg ) );
            break;

        case Personalizada:
            // TODO
            Q_UNUSED( end_img );
            break;
    }
}


QString utf8( const QString &texto )
{
    return QString::fromUtf8( ( texto.toStdString() ).c_str() );
}


char * qstring_char( const QString &texto )
{
    return texto.toUtf8().data();
}


double random( const double &min, const double &max )
{
    double zero_um = (((double)rand())/((double)RAND_MAX));

    return min + ( max - min ) * zero_um;
}


float random( const float &min, const float &max )
{
    float zero_um = (((float)rand())/((float)RAND_MAX));

    return min + ( max - min ) * zero_um;
}

#endif
