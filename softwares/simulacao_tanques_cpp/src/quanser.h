#ifndef QUANSER_H_
#define QUANSER_H_

#include <QDataStream>
#include <QMessageBox>
#include <QObject>
#include <QString>
#include <QTcpSocket>

#include "excecoes.h"

class Quanser : public QObject
{
    Q_OBJECT

    // METODOS
    public:
        Quanser( const QString &ip, const int &porta );
        ~Quanser();

        void escrever( const int &, const float & );
        float ler( const int & );

        void configurar_ip_porta( const QString &, const int & );
        void conectar();
        void desconectar();

    private:
        QString receber_dados();

        void inicializar();
        void enviar_dados( const QString & );

    signals:
        void erro( QString );

    // ATRIBUTOS
    private:
        QTcpSocket *socket;
        QDataStream *dados;

        QString host;
        int porta;
};

#endif
