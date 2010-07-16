#ifndef QUANSER_CPP_
#define QUANSER_CPP_

#include "quanser.h"

Quanser :: Quanser( const QString &i, const int &p ) : host( i ), porta( p )
{
    inicializar();
}


Quanser :: ~Quanser()
{
    desconectar();

    delete dados;
    delete socket;
}


void Quanser :: escrever( const int &canal, const float &valor )
{
    QString msg_saida = "WRITE ";
    msg_saida.append( QString::number( canal ) );
    msg_saida.append( " " );
    msg_saida.append( QString::number( valor ) );
    msg_saida.append( "\n" );

    enviar_dados( msg_saida );

    socket->waitForBytesWritten();
    socket->waitForReadyRead();

    QString msg_ent = receber_dados();

    if ( msg_ent != "ACK\n" )
    {
        emit( erro( "Erro na escrita dos dados: " + msg_ent ) );
        throw( ExcecaoEscrita( "Erro na escrita dos dados: " + 
                               msg_ent ) );
    }
}


float Quanser :: ler( const int &canal )
{
    QString msg_saida = "READ ";
    msg_saida.append( QString::number( canal ) );
    msg_saida.append( " \n" );

    enviar_dados( msg_saida );
    
    socket->waitForBytesWritten();
    socket->waitForReadyRead();

    QString msg_ent = receber_dados();

    bool ok;

    float valor = msg_ent.toFloat( &ok );

    if ( !ok )
    {
        emit( erro( "Erro na leitura dos dados: Conversão para float." ) );
        throw( ExcecaoLeitura( "Erro na leitura dos dados: Conversão para "
                               "float." ) );
    }

    return valor;
}


void Quanser :: configurar_ip_porta( const QString &ip, const int &p )
{
    host = ip;
    porta = p;

    desconectar();
    conectar();
}


void Quanser :: conectar()
{
    socket->connectToHost( host, porta );

    if ( !socket->waitForConnected() )
    {
        emit( erro( "Não foi possível estabelecer a conexão com o host " + 
                    host + " na porta " + QString::number( porta ) + ". "
                    "Verifique o endereço do host, a porta e tente "
                    "novamente." ) );
        throw( ExcecaoConexao( "Não foi possível estabelecer a conexão com o "
                               "host " + host + " na porta " + 
                               QString::number( porta ) + ". Verifique o "
                               "endereço do host, a porta e tente "
                               "novamente." ) );
    }
}


void Quanser :: desconectar()
{
    socket->abort();
    socket->close();
    socket->waitForDisconnected();
}


QString Quanser :: receber_dados()
{
    QString msg( socket->readAll() );

    return msg;
}


void Quanser :: inicializar()
{
    socket = new QTcpSocket( this );

    try
    {
        conectar();
    }
    catch( ExcecaoConexao e )
    {
        throw( e );
    }

    dados = new QDataStream( socket );
}


void Quanser :: enviar_dados( const QString &msg )
{
    *dados << qstring_char( msg );
}


#endif
