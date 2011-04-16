#ifndef FUNCOES_H_
#define FUNCOES_H_

#include <QByteArray>
#include <QMessageBox>
#include <QString>
#include <QWidget>

#include <cmath>

#define logsig( n ) 1 / ( 1 + exp( -n ) )
#define tansig( n ) 2 / ( 1 + exp( -2 * n ) ) - 1

enum TipoMensagem{ Geral = 0, 
                   Informacao, 
                   Pergunta, 
                   Aviso, 
                   Critica, 
                   Personalizada /* TODO */ };

// O campo de endereco da imagem so sera utilizado no caso de mensagens
// personalizadas
void exibir_mensagem( QWidget *pai = NULL,
                      const QString &titulo = "Título Genérico", 
                      const QString &msg = "Mensagem Genérica.", 
                      const TipoMensagem &tipo = Geral,
                      const QString &end_img = "" );

QString utf8( const QString &texto );

char * qstring_char( const QString &texto );

double random( const double &min, const double &max );

float random( const float &min, const float &max );

#endif
