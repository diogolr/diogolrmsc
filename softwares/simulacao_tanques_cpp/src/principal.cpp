#ifndef PRINCIPAL_CPP_
#define PRINCIPAL_CPP_

#include "principal.h"

JanelaPrincipal :: JanelaPrincipal()
{
    ui = new Ui_JanelaPrincipal;

    ui->setupUi( this );

    inicializar();
    configurar_graficos();
}


JanelaPrincipal :: ~JanelaPrincipal()
{
    // Quanser
    if ( quanser != NULL )
        delete quanser;

    // Dialogs
    if ( calibrar_sensores != NULL )
        delete calibrar_sensores;

    if ( config_parametros != NULL )
        delete config_parametros;
    
    if ( config_canais != NULL )
        delete config_canais;

    if ( salvar != NULL )
        delete salvar;

    // Outros
    delete ui;
    delete simulacao;
    delete temporizador;
    delete []imgs_barra_status;

    if ( num_amostras != 0 )
    {
        deletar_matriz();
    }
}


void JanelaPrincipal :: atualizar_graficos_planta_real( const bool &replote )
{
    ui->real_niveis->adicionar_xy( 0, tempo_atual, referencia_t1, replote );
    ui->real_niveis->adicionar_xy( 1, tempo_atual, referencia_t2, replote );
    ui->real_niveis->adicionar_xy( 2, tempo_atual, nivel_t1, replote );
    ui->real_niveis->adicionar_xy( 3, tempo_atual, nivel_t2, replote );

    erro_t1 = referencia_t1 - nivel_t1;
    erro_t2 = referencia_t2 - nivel_t2;

    ui->real_erro_sc->adicionar_xy( 0, tempo_atual, erro_t1, replote );
    ui->real_erro_sc->adicionar_xy( 1, tempo_atual, erro_t2, replote );
    ui->real_erro_sc->adicionar_xy( 2, tempo_atual, sinal_cont_t1, replote );
    ui->real_erro_sc->adicionar_xy( 3, tempo_atual, sinal_cont_t2, replote );
    ui->real_erro_sc->adicionar_xy( 4, tempo_atual, acao_p_t1, replote );
    ui->real_erro_sc->adicionar_xy( 5, tempo_atual, acao_p_t2, replote );
    ui->real_erro_sc->adicionar_xy( 6, tempo_atual, acao_i_t1, replote );
    ui->real_erro_sc->adicionar_xy( 7, tempo_atual, acao_i_t2, replote );
    ui->real_erro_sc->adicionar_xy( 8, tempo_atual, acao_d_t1, replote );
    ui->real_erro_sc->adicionar_xy( 9, tempo_atual, acao_d_t2, replote );
}


void JanelaPrincipal :: atualizar_graficos_simulacao( const bool &replote )
{
    ui->sim_niveis->adicionar_xy( 0, tempo_atual, referencia_t1, replote );
    ui->sim_niveis->adicionar_xy( 1, tempo_atual, referencia_t2, replote );
    ui->sim_niveis->adicionar_xy( 2, tempo_atual, nivel_t1, replote );
    ui->sim_niveis->adicionar_xy( 3, tempo_atual, nivel_t2, replote );

    erro_t1 = referencia_t1 - nivel_t1;
    erro_t2 = referencia_t2 - nivel_t2;

    ui->sim_erro_sc->adicionar_xy( 0, tempo_atual, erro_t1, replote );
    ui->sim_erro_sc->adicionar_xy( 1, tempo_atual, erro_t2, replote );
    ui->sim_erro_sc->adicionar_xy( 2, tempo_atual, sinal_cont_t1, replote );
    ui->sim_erro_sc->adicionar_xy( 3, tempo_atual, sinal_cont_t2, replote );
    ui->sim_erro_sc->adicionar_xy( 4, tempo_atual, acao_p_t1, replote );
    ui->sim_erro_sc->adicionar_xy( 5, tempo_atual, acao_p_t2, replote );
    ui->sim_erro_sc->adicionar_xy( 6, tempo_atual, acao_i_t1, replote );
    ui->sim_erro_sc->adicionar_xy( 7, tempo_atual, acao_i_t2, replote );
    ui->sim_erro_sc->adicionar_xy( 8, tempo_atual, acao_d_t1, replote );
    ui->sim_erro_sc->adicionar_xy( 9, tempo_atual, acao_d_t2, replote );
}


void JanelaPrincipal :: atualizar_simulacao_arquivo()
{
    for ( int i = 0 ; i < num_amostras ; i++ )
    {
        // Atualizando os parametros da simulacao
        tempo_atual = matriz[i][0];

        simulacao->modificar_ganho_sensor( TANQUE_1, matriz[i][1] );
        simulacao->modificar_ganho_sensor( TANQUE_2, matriz[i][2] );
        simulacao->modificar_dc_sensor( TANQUE_1, matriz[i][3] );
        simulacao->modificar_dc_sensor( TANQUE_2, matriz[i][4] );
        simulacao->modificar_ruido_sensor( TANQUE_1, matriz[i][5] );
        simulacao->modificar_ruido_sensor( TANQUE_2, matriz[i][6] );

        simulacao->modificar_ganho_atuador( TANQUE_1, matriz[i][7] );
        simulacao->modificar_ganho_atuador( TANQUE_2, matriz[i][8] );
        simulacao->modificar_dc_atuador( TANQUE_1, matriz[i][9] );
        simulacao->modificar_dc_atuador( TANQUE_2, matriz[i][10] );
        simulacao->modificar_ruido_atuador( TANQUE_1, matriz[i][11] );
        simulacao->modificar_ruido_atuador( TANQUE_2, matriz[i][12] );

        simulacao->modificar_orif_vazamento( TANQUE_1, matriz[i][13] );
        simulacao->modificar_orif_vazamento( TANQUE_2, matriz[i][14] );

        simulacao->modificar_km( TANQUE_1, matriz[i][15] );
        simulacao->modificar_km( TANQUE_2, matriz[i][16] );

        simulacao->modificar_orif_saida( TANQUE_1, matriz[i][17] );
        simulacao->modificar_orif_saida( TANQUE_2, matriz[i][18] );

        simulacao->modificar_ganho_mod_pot( TANQUE_1, matriz[i][19] );
        simulacao->modificar_ganho_mod_pot( TANQUE_2, matriz[i][20] );

        referencia_t1 = matriz[i][21];
        referencia_t2 = matriz[i][22];

        controlador_t1.configurar_referencia( referencia_t1 );
        controlador_t2.configurar_referencia( referencia_t2 );
   
        // Niveis
        nivel_t1 = 6.25 * simulacao->ler( sim_canal_leitura_t1 );
        nivel_t2 = 6.25 * simulacao->ler( sim_canal_leitura_t2 );

        // Sinal de Controle
        sinal_cont_t1 = controlador_t1.sinal_controle( nivel_t1 );
        sinal_cont_t2 = controlador_t2.sinal_controle( nivel_t2 );
        
        /*
        // Evitar transbordamento ou sugar em vazio
        if ( sinal_cont_t1 < 0.0 && nivel_t1 <= 2.0 )
        {
            sinal_cont_t1 = 0.0;
        }
        else if ( nivel_t1 >= 29.0 )
        {
            sinal_cont_t1 = -0.5;
        }
        else if ( nivel_t1 >= 28.0 )
        {
            sinal_cont_t1 = tensao_nivel( 28.0 );
        }
        
        if ( sinal_cont_t2 < 0.0 && nivel_t2 <= 2.0 )
        {
            sinal_cont_t2 = 0.0;
        }
        else if ( nivel_t2 >= 29.0 )
        {
            sinal_cont_t2 = -0.5;
        }
        else if ( nivel_t2 >= 28.0 )
        {
            sinal_cont_t2 = tensao_nivel( 28.0 );
        }
        */

        acao_p_t1 = controlador_t1.acao_p();
        acao_p_t2 = controlador_t2.acao_p();
        acao_i_t1 = controlador_t1.acao_i();
        acao_i_t2 = controlador_t2.acao_i();
        acao_d_t1 = controlador_t1.acao_d();
        acao_d_t2 = controlador_t2.acao_d();

        // Atualizacao
        simulacao->escrever( sim_canal_escrita_t1, sinal_cont_t1 );
        simulacao->escrever( sim_canal_escrita_t2, sinal_cont_t2 );

        simulacao->atualizar( tempo_atual );

        // Graficos
        if ( i == num_amostras - 1 )
        {
            atualizar_graficos_simulacao();
        }
        else
        {
            atualizar_graficos_simulacao( false );
        }

        // TODO retirar
        if ( i % 1000 == 0 )
        {
            qDebug() << i;
        }
    }
}


void JanelaPrincipal :: carregar_matriz( const QString &nome_arquivo )
{
    ifstream i( qstring_char( nome_arquivo ), ios::in );

    i >> num_amostras;

    // Alocando a matriz e inserindo os elementos
    matriz = new double *[num_amostras];

    for ( int l = 0 ; l < num_amostras ; l++ )
    {
        matriz[l] = new double[ num_colunas ];
    }

    for ( int l = 0 ; l < num_amostras ; l++ )
    {
        for ( int c = 0 ; c < num_colunas ; c++ )
        {
            i >> matriz[l][c];
        }
    }

    i.close();
}


void JanelaPrincipal :: conectar_sinais_slots()
{
    // Integracao condicional
    connect( ui->integ_cond_t1, SIGNAL( stateChanged( int ) ),
             this, SLOT( habilitar_aplicar() ) );
    connect( ui->integ_cond_t2, SIGNAL( stateChanged( int ) ),
             this, SLOT( habilitar_aplicar() ) );

    // Referencia
    connect( ui->referencia_t1, SIGNAL( returnPressed() ),
             this, SLOT( enter_pressionado() ) );
    connect( ui->referencia_t2, SIGNAL( returnPressed() ),
             this, SLOT( enter_pressionado() ) );

    connect( ui->referencia_t1, SIGNAL( textChanged( const QString & ) ),
             this, SLOT( habilitar_aplicar() ) );
    connect( ui->referencia_t2, SIGNAL( textChanged( const QString & ) ),
             this, SLOT( habilitar_aplicar() ) );

    // Ganhos controladores
    connect( ui->kp_t1, SIGNAL( returnPressed() ),
             this, SLOT( enter_pressionado() ) );
    connect( ui->ki_t1, SIGNAL( returnPressed() ),
             this, SLOT( enter_pressionado() ) );
    connect( ui->kd_t1, SIGNAL( returnPressed() ),
             this, SLOT( enter_pressionado() ) );
    connect( ui->kp_t2, SIGNAL( returnPressed() ),
             this, SLOT( enter_pressionado() ) );
    connect( ui->ki_t2, SIGNAL( returnPressed() ),
             this, SLOT( enter_pressionado() ) );
    connect( ui->kd_t2, SIGNAL( returnPressed() ),
             this, SLOT( enter_pressionado() ) );

    connect( ui->kp_t1, SIGNAL( textChanged( const QString & ) ),
             this, SLOT( habilitar_aplicar() ) );
    connect( ui->ki_t1, SIGNAL( textChanged( const QString & ) ),
             this, SLOT( habilitar_aplicar() ) );
    connect( ui->kd_t1, SIGNAL( textChanged( const QString & ) ),
             this, SLOT( habilitar_aplicar() ) );
    connect( ui->kp_t2, SIGNAL( textChanged( const QString & ) ),
             this, SLOT( habilitar_aplicar() ) );
    connect( ui->ki_t2, SIGNAL( textChanged( const QString & ) ),
             this, SLOT( habilitar_aplicar() ) );
    connect( ui->kd_t2, SIGNAL( textChanged( const QString & ) ),
             this, SLOT( habilitar_aplicar() ) );

    // Acoes e botoes com mesma funcionalidade
    connect( ui->acao_salvar_dados, SIGNAL( triggered() ),
             this, SLOT( salvar_dados_graficos() ) );
    connect( ui->salvar, SIGNAL( clicked() ),
             this, SLOT( salvar_dados_graficos() ) );

    connect( ui->acao_exportar, SIGNAL( triggered() ),
             this, SLOT( exportar_dados_graficos() ) );
    connect( ui->exportar, SIGNAL( clicked() ),
             this, SLOT( exportar_dados_graficos() ) );

    connect( ui->acao_imprimir, SIGNAL( triggered() ),
             this, SLOT( imprimir_graficos() ) );
    connect( ui->imprimir, SIGNAL( clicked() ),
             this, SLOT( imprimir_graficos() ) );
}


void JanelaPrincipal :: configurar_controladores()
{
    controlador_t1.configurar_ganhos( kp_t1, ki_t1, kd_t1 );
    controlador_t2.configurar_ganhos( kp_t2, ki_t2, kd_t2 );

    int tc_t1 = ui->tipo_controlador_t1->currentIndex();
    int tc_t2 = ui->tipo_controlador_t2->currentIndex();
    
    controlador_t1.configurar_controlador( (Controle::TipoControlador)tc_t1 );
    controlador_t2.configurar_controlador( (Controle::TipoControlador)tc_t2 ); 

    // Se o controlador 1 possuir parte integrativa, configura-se a parte da
    // integracao condicional.
    if ( tc_t1 != Controle::P && tc_t1 != Controle::PD )
    {
        switch ( ui->integ_cond_t1->checkState() )
        {
            case Qt::PartiallyChecked:
            case Qt::Checked:
                controlador_t1.habilitar_integracao_cond( true );
                break;

            case Qt::Unchecked:
                controlador_t1.habilitar_integracao_cond( false );
                break;
        }
    }
    else
    {
        controlador_t1.habilitar_integracao_cond( false );
    }
    
    // Se o controlador 2 possuir parte integrativa, configura-se a parte da
    // integracao condicional.
    if ( tc_t2 != Controle::P && tc_t2 != Controle::PD )
    {
        switch ( ui->integ_cond_t2->checkState() )
        {
            case Qt::PartiallyChecked:
            case Qt::Checked:
                controlador_t2.habilitar_integracao_cond( true );
                break;

            case Qt::Unchecked:
                controlador_t2.habilitar_integracao_cond( false );
                break;
        }
    }
}


void JanelaPrincipal :: configurar_graficos()
{
    QPen linha( Qt::SolidPattern,
                0.0,
                Qt::SolidLine,
                Qt::SquareCap,
                Qt::RoundJoin );

    // Grid
    ui->real_niveis->utilizar_grid( true );
    ui->real_erro_sc->utilizar_grid( true );
    ui->sim_niveis->utilizar_grid( true );
    ui->sim_erro_sc->utilizar_grid( true );

    // Eixos
    QwtText y_nivel( utf8( "Nível [cm]" ) );
    QwtText y_erro_sc( "Erro [cm] / Sinal de Controle [V]" );
    QwtText x( "Tempo aproximado [s]" );

    QFont fonte = this->font();
    y_nivel.setFont( fonte );
    y_erro_sc.setFont( fonte );
    x.setFont( fonte );

    ui->real_niveis->configurar_nome_eixo( QwtPlot::yLeft, y_nivel );
    ui->real_niveis->configurar_nome_eixo( QwtPlot::xBottom, x );
    ui->real_erro_sc->configurar_nome_eixo( QwtPlot::yLeft, y_erro_sc );
    ui->real_erro_sc->configurar_nome_eixo( QwtPlot::xBottom, x );
    ui->sim_niveis->configurar_nome_eixo( QwtPlot::yLeft, y_nivel );
    ui->sim_niveis->configurar_nome_eixo( QwtPlot::xBottom, x );
    ui->sim_erro_sc->configurar_nome_eixo( QwtPlot::yLeft, y_erro_sc );
    ui->sim_erro_sc->configurar_nome_eixo( QwtPlot::xBottom, x );

    // Curvas e Legendas
    ui->real_legenda_niveis->associar_grafico( ui->real_niveis );
    ui->real_legenda_erro_sc->associar_grafico( ui->real_erro_sc );
    ui->sim_legenda_niveis->associar_grafico( ui->sim_niveis );
    ui->sim_legenda_erro_sc->associar_grafico( ui->sim_erro_sc );

    // Ordem da insercao das curvas - Grafico de Niveis
    // [0] - Referencia T1
    // [1] - Referencia T2
    // [2] - Nivel T1
    // [3] - Nivel T2
    // Ordem da insercao das curvas - Grafico de Erro/Sinal de Controle
    // [0] - Erro T1
    // [1] - Erro T2
    // [2] - Sinal de controle T1
    // [3] - Sinal de controle T2
    // [4] - Acao Proporcional T1
    // [5] - Acao Proporcional T2
    // [6] - Acao Integral T1
    // [7] - Acao Integral T2
    // [8] - Acao Derivativa T1
    // [9] - Acao Derivativa T2
    
    // Referencia T1
    linha.setStyle( Qt::DashLine );
    linha.setColor( Qt::darkMagenta );

    ui->real_niveis->adicionar_curva( linha, "Referência T1" );
    ui->sim_niveis->adicionar_curva( linha, "Referência T1" );

    ui->real_legenda_niveis->adicionar_item( 
                                    ui->real_niveis->curva( 0 ) );
    ui->sim_legenda_niveis->adicionar_item( 
                                  ui->sim_niveis->curva( 0 ) );

    // Referencia T2
    linha.setColor( Qt::darkBlue );

    ui->real_niveis->adicionar_curva( linha, "Referência T2" );
    ui->sim_niveis->adicionar_curva( linha, "Referência T2" );

    ui->real_legenda_niveis->adicionar_item( 
                                    ui->real_niveis->curva( 1 ) );
    ui->sim_legenda_niveis->adicionar_item( 
                                  ui->sim_niveis->curva( 1 ) );

    // Nivel T1
    linha.setStyle( Qt::SolidLine );
    linha.setColor( Qt::red );

    ui->real_niveis->adicionar_curva( linha, "Nível T1" );
    ui->sim_niveis->adicionar_curva( linha, "Nível T1" );

    ui->real_legenda_niveis->adicionar_item( 
                                    ui->real_niveis->curva( 2 ) );
    ui->sim_legenda_niveis->adicionar_item( 
                                  ui->sim_niveis->curva( 2 ) );

    // Nivel T2
    linha.setColor( Qt::blue );

    ui->real_niveis->adicionar_curva( linha, "Nível T2" );
    ui->sim_niveis->adicionar_curva( linha, "Nível T2" );

    ui->real_legenda_niveis->adicionar_item( 
                                    ui->real_niveis->curva( 3 ) );
    ui->sim_legenda_niveis->adicionar_item( 
                                  ui->sim_niveis->curva( 3 ) );

    // Erro T1
    linha.setColor( Qt::red );

    ui->real_erro_sc->adicionar_curva( linha, "Erro T1" );
    ui->sim_erro_sc->adicionar_curva( linha, "Erro T1" );

    ui->real_legenda_erro_sc->adicionar_item( 
                                     ui->real_erro_sc->curva( 0 ) );
    ui->sim_legenda_erro_sc->adicionar_item( 
                                   ui->sim_erro_sc->curva( 0 ) );
   
    // Erro T2 
    linha.setColor( Qt::darkRed );

    ui->sim_erro_sc->adicionar_curva( linha, "Erro T2" );
    ui->real_erro_sc->adicionar_curva( linha, "Erro T2" );

    ui->real_legenda_erro_sc->adicionar_item( 
                                     ui->real_erro_sc->curva( 1 ) );
    ui->sim_legenda_erro_sc->adicionar_item( 
                                   ui->sim_erro_sc->curva( 1 ) );

    // Sinal de Controle T1
    linha.setColor( Qt::green );

    ui->real_erro_sc->adicionar_curva( linha, "Sinal de Controle T1" );
    ui->sim_erro_sc->adicionar_curva( linha, "Sinal de Controle T1" );

    ui->real_legenda_erro_sc->adicionar_item( 
                                     ui->real_erro_sc->curva( 2 ) );
    ui->sim_legenda_erro_sc->adicionar_item( 
                                   ui->sim_erro_sc->curva( 2 ) );

    // Sinal de controle T2
    linha.setColor( Qt::darkGreen );

    ui->real_erro_sc->adicionar_curva( linha, "Sinal de Controle T2" );
    ui->sim_erro_sc->adicionar_curva( linha, "Sinal de Controle T2" );

    ui->real_legenda_erro_sc->adicionar_item( 
                                     ui->real_erro_sc->curva( 3 ) );
    ui->sim_legenda_erro_sc->adicionar_item( 
                                   ui->sim_erro_sc->curva( 3 ) );

    // Acao Proporcional T1
    linha.setColor( Qt::magenta );

    ui->real_erro_sc->adicionar_curva( linha, "Ação Proporcional T1" );
    ui->sim_erro_sc->adicionar_curva( linha, "Ação Proporcional T1" );

    ui->real_legenda_erro_sc->adicionar_item( 
                                     ui->real_erro_sc->curva( 4 ) );
    ui->sim_legenda_erro_sc->adicionar_item( 
                                   ui->sim_erro_sc->curva( 4 ) );
    
    // Acao Proporcional T2
    linha.setColor( Qt::darkMagenta );

    ui->real_erro_sc->adicionar_curva( linha, "Ação Proporcional T2" );
    ui->sim_erro_sc->adicionar_curva( linha, "Ação Proporcional T2" );

    ui->real_legenda_erro_sc->adicionar_item( 
                                     ui->real_erro_sc->curva( 5 ) );
    ui->sim_legenda_erro_sc->adicionar_item( 
                                   ui->sim_erro_sc->curva( 5 ) );

    // Acao Integrativa T1
    linha.setColor( Qt::cyan );

    ui->real_erro_sc->adicionar_curva( linha, "Ação Integrativa T1" );
    ui->sim_erro_sc->adicionar_curva( linha, "Ação Integrativa T1" );

    ui->real_legenda_erro_sc->adicionar_item( 
                                     ui->real_erro_sc->curva( 6 ) );
    ui->sim_legenda_erro_sc->adicionar_item( 
                                   ui->sim_erro_sc->curva( 6 ) );

    // Acao Integrativa T2
    linha.setColor( Qt::darkCyan );

    ui->real_erro_sc->adicionar_curva( linha, "Ação Integrativa T2" );
    ui->sim_erro_sc->adicionar_curva( linha, "Ação Integrativa T2" );

    ui->real_legenda_erro_sc->adicionar_item( 
                                     ui->real_erro_sc->curva( 7 ) );
    ui->sim_legenda_erro_sc->adicionar_item( 
                                   ui->sim_erro_sc->curva( 7 ) );
   
    // Acao Derivativa T1
    linha.setColor( Qt::blue );

    ui->real_erro_sc->adicionar_curva( linha, "Ação Derivativa T1" );
    ui->sim_erro_sc->adicionar_curva( linha, "Ação Derivativa T1" );

    ui->real_legenda_erro_sc->adicionar_item( 
                                     ui->real_erro_sc->curva( 8 ) );
    ui->sim_legenda_erro_sc->adicionar_item( 
                                   ui->sim_erro_sc->curva( 8 ) );

    // Acao Derivativa T2
    linha.setColor( Qt::darkBlue );

    ui->real_erro_sc->adicionar_curva( linha, "Ação Derivativa T2" );
    ui->sim_erro_sc->adicionar_curva( linha, "Ação Derivativa T2" );

    ui->real_legenda_erro_sc->adicionar_item( 
                                     ui->real_erro_sc->curva( 9 ) );
    ui->sim_legenda_erro_sc->adicionar_item( 
                                   ui->sim_erro_sc->curva( 9 ) );
}


void JanelaPrincipal :: deletar_matriz()
{
    for ( int i = 0 ; i < num_amostras ; i++ )
    {
        delete []matriz[i];
    }

    delete []matriz;

    num_amostras = 0;
}


void JanelaPrincipal :: estabelecer_conexao()
{
    try
    {
        if ( quanser != NULL )
        {
            quanser->configurar_ip_porta( ui->ip_real->text(),
                                          ui->porta_real->text().toInt() );
        }
        else
        {
            quanser = new Quanser( ui->ip_real->text(),
                                   ui->porta_real->text().toInt() );
        }
    }
    catch( ExcecaoConexao e )
    {
        exibir_mensagem( this, "Erro na conexão", e.msg_erro(), Aviso );
        return;
    }
    
    imgs_barra_status[0].setPixmap( QPixmap( 
                                    ":imgs/network_on_22x22.png" ) );
    imgs_barra_status[0].setToolTip( "Conectado ao servidor" );
}


void JanelaPrincipal :: inicializar()
{
    // Inicializacao das variaveis
    erro_t1 = 0.0;
    erro_t2 = 0.0;

    kp_t1 = 0.0;
    ki_t1 = 0.0;
    kd_t1 = 0.0;
    kp_t2 = 0.0;
    ki_t2 = 0.0;
    kd_t2 = 0.0;
    
    nivel_t1 = 0.0;
    nivel_t2 = 0.0;

    referencia_t1 = 0.0;
    referencia_t2 = 0.0;
    
    acao_p_t1 = 0.0;
    acao_p_t2 = 0.0;
    acao_i_t1 = 0.0;
    acao_i_t2 = 0.0;
    acao_d_t1 = 0.0;
    acao_d_t2 = 0.0;
    sinal_cont_t1 = 0.0;
    sinal_cont_t2 = 0.0;
    
    tempo_atual = 0.0;
    
    matriz = NULL;

    num_amostras = 0;
    num_colunas = 23;

    real_canal_escrita_t1 = CANAL_ESCRITA_T1;
    real_canal_escrita_t2 = CANAL_ESCRITA_T2;
    real_canal_leitura_t1 = CANAL_LEITURA_T1;
    real_canal_leitura_t2 = CANAL_LEITURA_T2;
    sim_canal_escrita_t1 = CANAL_ESCRITA_T1;
    sim_canal_escrita_t2 = CANAL_ESCRITA_T2;
    sim_canal_leitura_t1 = CANAL_LEITURA_T1;
    sim_canal_leitura_t2 = CANAL_LEITURA_T2;

    // Configuracao dos tanques
    configuracao = Configuracao_1;

    // Configuracao basica dos controladores
    controlador_t1.configurar_limites( -3.0, 3.0 );
    controlador_t2.configurar_limites( -3.0, 3.0 );

    controlador_t1.configurar_per_amost( PERIODO_AMOSTRAGEM );
    controlador_t2.configurar_per_amost( PERIODO_AMOSTRAGEM );

    // Dialogs
    calibrar_sensores = NULL;
    config_parametros = NULL;
    config_canais = NULL;
    salvar = NULL;

    // Modo de funcionamento
    //TODO modo_funcionamento = RealSimulacao;
    modo_funcionamento = SomenteSimulacao;

    // Comunicacao
    quanser = NULL;

    // Imagens da barra de status
    imgs_barra_status = new QLabel[2];

    imgs_barra_status[0].setPixmap( QPixmap( ":imgs/network_off_22x22.png" ) );
    imgs_barra_status[1].setPixmap( QPixmap( ":imgs/player_stop_22x22.png" ) );
    imgs_barra_status[0].setToolTip( utf8( "Não conectado ao servidor" ) );
    imgs_barra_status[1].setToolTip( "Sistema parado" );

    ui->barra_status->addPermanentWidget( &imgs_barra_status[0] );
    ui->barra_status->addPermanentWidget( &imgs_barra_status[1] );

    // Inicializacao do temporizador
    temporizador = new QTimer( this );

    connect( temporizador, SIGNAL( timeout() ), 
             this, SLOT( atualizar_dados() ) );

    // Inicializacao da simulacao
    simulacao = new Simulacao();

    // Habilitando/Desabilitando os campos da interface
    ui->parar->setEnabled( false );

    // Conectando os sinais que habilitam o botao aplicar dos controladores ou
    // que aplicam os valores ao pressionar enter nos campos, alem de outros
    // sinais da interface
    conectar_sinais_slots();
}


void JanelaPrincipal :: ler_campos()
{
    bool ok = true;
    double valor = 0.0;

    // Referencias
    valor = ui->referencia_t1->text().toDouble( &ok );

    if ( !ok || valor < 0.0 || valor > 30.0 )
    {
        throw ExcecaoCampoInvalido( "Referência do tanque 1 inválida" );
    }
   
    referencia_t1 = valor;

    controlador_t1.configurar_referencia( referencia_t1 );

    valor = ui->referencia_t2->text().toDouble( &ok );

    if ( !ok || valor < 0.0 || valor > 30.0 )
    {
        throw ExcecaoCampoInvalido( "Referência do tanque 2 inválida" );
    }

    referencia_t2 = valor; 
    controlador_t2.configurar_referencia( referencia_t2 );

    // Ganhos - Controlador 1
    valor = ui->kp_t1->text().toDouble( &ok );

    if ( !ok )
    {
        throw ExcecaoCampoInvalido( "Ganho Kp do controlador 1 inválido" );
    }

    kp_t1 = valor;
    
    valor = ui->ki_t1->text().toDouble( &ok );

    if ( !ok )
    {
        throw ExcecaoCampoInvalido( "Ganho Ki do controlador 1 inválido" );
    }

    ki_t1 = valor;

    valor = ui->kd_t1->text().toDouble( &ok );

    if ( !ok )
    {
        throw ExcecaoCampoInvalido( "Ganho Kd do controlador 1 inválido" );
    }

    kd_t1 = valor;

    // Ganhos - Controlador 2
    valor = ui->kp_t2->text().toDouble( &ok );

    if ( !ok )
    {
        throw ExcecaoCampoInvalido( "Ganho Kp do controlador 2 inválido" );
    }

    kp_t2 = valor;
    
    valor = ui->ki_t2->text().toDouble( &ok );

    if ( !ok )
    {
        throw ExcecaoCampoInvalido( "Ganho Ki do controlador 2 inválido" );
    }

    ki_t2 = valor;

    valor = ui->kd_t2->text().toDouble( &ok );

    if ( !ok )
    {
        throw ExcecaoCampoInvalido( "Ganho Kd do controlador 2 inválido" );
    }

    kd_t2 = valor;
}


void JanelaPrincipal :: salvar_matriz( const QString &nome_arquivo )
{
    ofstream o( qstring_char( nome_arquivo ), ios::out );

    for ( int i = 0 ; i < num_amostras ; i++ )
    {
        for ( int j = 0 ; j < num_colunas ; j++ )
        {
            o << matriz[i][j] << "\t";
        }

        o << endl;
    }

    o.close();
}


void JanelaPrincipal :: atualizar_dados()
{
    tempo_atual += PERIODO_AMOSTRAGEM;

    // Se o programa estiver executando no modo RealSimulacao ou no modo Real,
    // ou seja, se houver aquisicao da planta real
    if ( modo_funcionamento != SomenteSimulacao )
    {
        nivel_t1 = 6.25 * quanser->ler( real_canal_leitura_t1 );
        nivel_t2 = 6.25 * quanser->ler( real_canal_leitura_t2 );

        sinal_cont_t1 = controlador_t1.sinal_controle( nivel_t1 );
        sinal_cont_t2 = controlador_t2.sinal_controle( nivel_t2 );

        // Evitar transbordamento ou sugar em vazio
        if ( sinal_cont_t1 < 0.0 && nivel_t1 <= 2.0 )
        {
            sinal_cont_t1 = 0.0;
        }
        else if ( nivel_t1 >= 29.0 )
        {
            sinal_cont_t1 = -0.5;
        }
        else if ( nivel_t1 >= 28.0 )
        {
            sinal_cont_t1 = tensao_nivel( 28.0 );
        }
        
        if ( sinal_cont_t2 < 0.0 && nivel_t2 <= 2.0 )
        {
            sinal_cont_t2 = 0.0;
        }
        else if ( nivel_t2 >= 29.0 )
        {
            sinal_cont_t2 = -0.5;
        }
        else if ( nivel_t2 >= 28.0 )
        {
            sinal_cont_t2 = tensao_nivel( 28.0 );
        }

        acao_p_t1 = controlador_t1.acao_p();
        acao_p_t2 = controlador_t2.acao_p();
        acao_i_t1 = controlador_t1.acao_i();
        acao_i_t2 = controlador_t2.acao_i();
        acao_d_t1 = controlador_t1.acao_d();
        acao_d_t2 = controlador_t2.acao_d();
        
        quanser->escrever( real_canal_escrita_t1, sinal_cont_t1 );
        quanser->escrever( real_canal_escrita_t2, sinal_cont_t2 );

        atualizar_graficos_planta_real();
    }

    // Se o programa estiver executando no modo RealSimulacao ou no modo
    // Simulacao, ou seja, se houver a simulacao
    if ( modo_funcionamento != SomenteReal )
    {
        // Se houver um arquivo de configuracao para a Simulacao e este
        // arquivo ainda nao tiver sido executado
        if ( num_amostras != 0 && !simulacao_encerrada )
        {
            atualizar_simulacao_arquivo();

            // Se o programa estiver executando no modo Simulacao, ou seja, se
            // nao houver aquisicao de dados da planta real, nao havera
            // necessidade de continuar apos toda a execucao do arquivo de
            // configuracao. Portanto, o temporizador devera ser parado e os
            // campos da interface habilitados/desabilitados.
            if ( modo_funcionamento == SomenteSimulacao )
            {
                simulacao_encerrada = true;

                temporizador->stop();

                ui->iniciar->setEnabled( true );
                ui->parar->setEnabled( false );

                imgs_barra_status[1].setPixmap( 
                                     QPixmap( ":imgs/player_stop_22x22.png" ) );
                imgs_barra_status[1].setToolTip( "Sistema parado" );

                habilitar_campos_parametros();

                return;
            }
        }
        // Se nao houver um arquivo de configuracao para a simulacao, a
        // simulacao sera executada passo a passo assim como a aquisicao da
        // planta real
        else
        {
            // Atualizacao dos graficos da simulacao
            nivel_t1 = 6.25 * simulacao->ler( sim_canal_leitura_t1 );
            nivel_t2 = 6.25 * simulacao->ler( sim_canal_leitura_t2 );

            sinal_cont_t1 = controlador_t1.sinal_controle( nivel_t1 );
            sinal_cont_t2 = controlador_t2.sinal_controle( nivel_t2 );
            
            /*
            // Evitar transbordamento ou sugar em vazio
            if ( sinal_cont_t1 < 0.0 && nivel_t1 <= 2.0 )
            {
                sinal_cont_t1 = 0.0;
            }
            else if ( nivel_t1 >= 29.0 )
            {
                sinal_cont_t1 = -0.5;
            }
            else if ( nivel_t1 >= 28.0 )
            {
                sinal_cont_t1 = tensao_nivel( 28.0 );
            }
            
            if ( sinal_cont_t2 < 0.0 && nivel_t2 <= 2.0 )
            {
                sinal_cont_t2 = 0.0;
            }
            else if ( nivel_t2 >= 29.0 )
            {
                sinal_cont_t2 = -0.5;
            }
            else if ( nivel_t2 >= 28.0 )
            {
                sinal_cont_t2 = tensao_nivel( 28.0 );
            }
            */

            acao_p_t1 = controlador_t1.acao_p();
            acao_p_t2 = controlador_t2.acao_p();
            acao_i_t1 = controlador_t1.acao_i();
            acao_i_t2 = controlador_t2.acao_i();
            acao_d_t1 = controlador_t1.acao_d();
            acao_d_t2 = controlador_t2.acao_d();
            
            // Atualizacao da simulacao
            simulacao->escrever( sim_canal_escrita_t1, sinal_cont_t1 );
            simulacao->escrever( sim_canal_escrita_t2, sinal_cont_t2 );

            simulacao->atualizar( tempo_atual );

            atualizar_graficos_simulacao();
        }
    }
}


void JanelaPrincipal :: desabilitar_campos_parametros()
{
    // Se houver algum arquivo de configuracao, os parametros dos controladores
    // nao poderao ser configurados e a referencia sera atualizada conforme o
    // arquivo
    if ( num_amostras != 0 )
    {
        ui->referencia_t1->setEnabled( false );
        ui->kp_t1->setEnabled( false );
        ui->ki_t1->setEnabled( false );
        ui->kd_t1->setEnabled( false );

        ui->referencia_t2->setEnabled( false );
        ui->kp_t2->setEnabled( false );
        ui->ki_t2->setEnabled( false );
        ui->kd_t2->setEnabled( false );
    }

    ui->modo_funcionamento->setEnabled( false );
    ui->configuracao->setEnabled( false );
    ui->acao_config_parametros->setEnabled( false );
    ui->acao_config_canais->setEnabled( false );
    ui->acao_calibrar_sensores->setEnabled( false );
    ui->acao_exportar->setEnabled( false );
    ui->acao_imprimir->setEnabled( false );
    ui->acao_salvar_dados->setEnabled( false );
    ui->ip_real->setEnabled( false );
    ui->porta_real->setEnabled( false );
    ui->iniciar->setEnabled( false );
    ui->parar->setEnabled( true );

    // Desabilitando o zoom dos graficos
    ui->sim_niveis->habilitar_zoom( false );
    ui->sim_erro_sc->habilitar_zoom( false );
    ui->real_niveis->habilitar_zoom( false );
    ui->real_erro_sc->habilitar_zoom( false );
}


void JanelaPrincipal :: enter_pressionado()
{
    // Mesma acao do botao aplicar_controladores
    try
    {
        ler_campos();
    }
    catch( ExcecaoCampoInvalido e )
    {
        exibir_mensagem( this, "Erro de conversão", e.msg_erro(), Aviso );
        return;
    }

    configurar_controladores();

    ui->aplicar_controladores->setEnabled( false );
}


void JanelaPrincipal :: exportar_dados_graficos()
{
    salvar = new Salvar( this, Salvar::ExportarGraficos );
    salvar->exec();

    QStringList arquivos = salvar->nomes_arquivos();

    switch( salvar->result() )
    {
        case QDialog::Accepted:
            // A variavel 'arquivos' sempre tera 4 valores referentes a:
            // real_niveis, real_erro_sc, sim_niveis e sim_erro_sc
            if ( arquivos[0] != "" )
            {
                ui->real_niveis->exportar( arquivos[0] );
            }

            if ( arquivos[1] != "" )
            {
                ui->real_erro_sc->exportar( arquivos[1] );
            }

            if ( arquivos[2] != "" )
            {
                ui->sim_niveis->exportar( arquivos[2] );
            }

            if ( arquivos[3] != "" )
            {
                ui->sim_erro_sc->exportar( arquivos[3] );
            }

            ui->barra_status->showMessage( "Arquivos salvos com sucesso.",
                                           4000 );
            break;
        
        case QDialog::Rejected:
            ui->barra_status->showMessage( utf8( "Os arquivos não foram "
                                                 "salvos." ),
                                           4000 );
            break;
    }
}


void JanelaPrincipal :: habilitar_aplicar()
{
    ui->aplicar_controladores->setEnabled( true );
}


void JanelaPrincipal :: habilitar_campos_parametros()
{
    // Se houver algum arquivo de configuracao, os campos dos parametros dos
    // controladores, assim como os campos das referencias que estavam
    // desabilitados deverao ser reabilitados. Ja a acao_config_parametros
    // devera permanecer desabilitada
    if ( num_amostras != 0 )
    {
        ui->referencia_t1->setEnabled( true );
        ui->kp_t1->setEnabled( true );
        ui->ki_t1->setEnabled( true );
        ui->kd_t1->setEnabled( true );

        ui->referencia_t2->setEnabled( true );
        ui->kp_t2->setEnabled( true );
        ui->ki_t2->setEnabled( true );
        ui->kd_t2->setEnabled( true );
    }
    else
    {
        ui->acao_config_parametros->setEnabled( true );
    }

    ui->modo_funcionamento->setEnabled( true );
    ui->configuracao->setEnabled( true );
    ui->acao_config_canais->setEnabled( true );
    ui->acao_calibrar_sensores->setEnabled( true );
    ui->acao_exportar->setEnabled( true );
    ui->acao_imprimir->setEnabled( true );
    ui->acao_salvar_dados->setEnabled( true );
    ui->ip_real->setEnabled( true );
    ui->porta_real->setEnabled( true );
    ui->iniciar->setEnabled( true );
    ui->parar->setEnabled( false );

    // Habilitando o zoom dos graficos
    ui->sim_niveis->habilitar_zoom( true );
    ui->sim_erro_sc->habilitar_zoom( true );
    ui->real_niveis->habilitar_zoom( true );
    ui->real_erro_sc->habilitar_zoom( true );
}


void JanelaPrincipal :: imprimir_graficos()
{
}


void JanelaPrincipal :: on_acao_calibrar_sensores_triggered()
{
    calibrar_sensores = new Calibrar( this );
    calibrar_sensores->exec();

    delete calibrar_sensores;
    calibrar_sensores = NULL;
}


void JanelaPrincipal :: on_acao_config_canais_triggered()
{
    config_canais = new ConfigCanais( this );
    config_canais->exec();
    
    switch ( config_canais->result() )
    {
        case QDialog::Accepted:
            real_canal_escrita_t1 = config_canais->real_canal_escrita( 
                                                   TANQUE_1 );
            real_canal_escrita_t2 = config_canais->real_canal_escrita( 
                                                   TANQUE_2 );
            real_canal_leitura_t1 = config_canais->real_canal_leitura( 
                                                   TANQUE_1 );
            real_canal_leitura_t2 = config_canais->real_canal_leitura(
                                                   TANQUE_2 );

            sim_canal_escrita_t1 = config_canais->sim_canal_escrita( TANQUE_1 );
            sim_canal_escrita_t2 = config_canais->sim_canal_escrita( TANQUE_2 );
            sim_canal_leitura_t1 = config_canais->sim_canal_leitura( TANQUE_1 );
            sim_canal_leitura_t2 = config_canais->sim_canal_leitura( TANQUE_2 );

            ui->barra_status->showMessage( "Canais alterados com sucesso.", 
                                           4000 );
            break;

        case QDialog::Rejected:
            ui->barra_status->showMessage( "Os canais foram mantidos.", 
                                           4000 );
            break;
    }

    delete config_canais;
    config_canais = NULL;
}


void JanelaPrincipal :: on_acao_config_parametros_triggered()
{
    config_parametros = new ConfigParametros( this );
    config_parametros->exec();
    
    QString nome_arquivo = config_parametros->arquivo();

    switch ( config_parametros->result() )
    {
        case QDialog::Accepted:

            if ( nome_arquivo != "" )
            {
                // Algum arquivo ja havia sido aberto. Necessario deletar a
                // matriz antes de realocar os elementos
                if ( num_amostras != 0 )
                {
                    deletar_matriz();
                }
                
                carregar_matriz( nome_arquivo );

                ui->barra_status->showMessage( "Arquivo carregado: " +
                                               nome_arquivo, 
                                               4000 );

                ui->acao_config_parametros->setEnabled( false );
                ui->acao_excluir_config->setEnabled( true );
            }
            else
            {
                ui->barra_status->showMessage( "Nenhum arquivo carregado.", 
                                               4000 );
            }

            break;

        case QDialog::Rejected:
            ui->barra_status->showMessage( "Nenhum arquivo carregado.", 
                                           4000 );
            break;
    }

    delete config_parametros;
    config_parametros = NULL;
}


void JanelaPrincipal :: on_acao_excluir_config_triggered()
{
    deletar_matriz();
   
    ui->acao_config_parametros->setEnabled( true );
    ui->acao_excluir_config->setEnabled( false );
}


void JanelaPrincipal :: on_aplicar_controladores_clicked()
{
    try
    {
        ler_campos();
    }
    catch( ExcecaoCampoInvalido e )
    {
        exibir_mensagem( this, "Erro de conversão", e.msg_erro(), Aviso );
        return;
    }

    configurar_controladores();

    ui->aplicar_controladores->setEnabled( false );
}


void JanelaPrincipal :: on_configuracao_activated( int ind )
{
    configuracao = (Configuracao)ind;

    switch ( ind )
    {
        case Configuracao_1:
            ui->referencia_t1->setEnabled( true );
            ui->kp_t1->setEnabled( true );
            ui->ki_t1->setEnabled( true );
            ui->kd_t1->setEnabled( true );

            ui->referencia_t2->setEnabled( true );
            ui->kp_t2->setEnabled( true );
            ui->ki_t2->setEnabled( true );
            ui->kd_t2->setEnabled( true );
            break;

        case Configuracao_2:
        default:
            ui->referencia_t2->setEnabled( false );
            ui->kp_t2->setEnabled( false );
            ui->ki_t2->setEnabled( false );
            ui->kd_t2->setEnabled( false );
            break;
    }
}


void JanelaPrincipal :: on_iniciar_clicked()
{
    ui->real_niveis->limpar();
    ui->real_erro_sc->limpar();
    ui->sim_niveis->limpar();
    ui->sim_erro_sc->limpar();

    // Resetando a simulacao
    // TODO configurar portas de leitura/escrita da simulacao apos o reset
    simulacao->reiniciar_valores();

    // Variaveis
    erro_t1 = 0.0;
    erro_t2 = 0.0;
    tempo_atual = 0.0;
    simulacao_encerrada = false;

    // Lendo os campos e configurando os controladores
    controlador_t1.reiniciar_valores();
    controlador_t2.reiniciar_valores();

    sinal_cont_t1 = 0.0;
    sinal_cont_t2 = 0.0;

    acao_p_t1 = acao_p_t2 = 0.0;
    acao_i_t1 = acao_i_t2 = 0.0;
    acao_d_t1 = acao_d_t2 = 0.0;

    try
    {
        ler_campos();
    }
    catch( ExcecaoCampoInvalido e )
    {
        exibir_mensagem( this, "Erro de conversão", e.msg_erro(), Aviso );
        return;
    }

    configurar_controladores();

    // Se houver um arquivo de configuracao para a simulacao, os graficos nao
    // devem ser plotados no inicio
    if ( num_amostras != 0 )
    {
        // As referencias sao reconfiguradas porque ao chamar o metodo
        // ler_campos() as referencias serao configuradas conforme os valores
        // da interface
        referencia_t1 = 0.0;
        referencia_t2 = 0.0;
        
        // Atualizando os graficos da planta real
        // Se o programa estiver executando no modo RealSimulacao ou no modo
        // SomenteReal, ou seja, se houver a planta real
        if ( modo_funcionamento != SomenteSimulacao )
        {
            estabelecer_conexao();
            
            nivel_t1 = quanser->ler( real_canal_leitura_t1 );
            nivel_t2 = quanser->ler( real_canal_leitura_t2 );
        }
        
        atualizar_graficos_planta_real( false );
        
        // Atualizando os graficos da simulacao
        // Se o programa estiver executando no modo RealSimulacao ou no modo
        // SomenteSimulacao, ou seja, se houver a simulacao
        if ( modo_funcionamento != SomenteReal )
        {
            nivel_t1 = simulacao->ler( sim_canal_leitura_t1 );
            nivel_t2 = simulacao->ler( sim_canal_leitura_t2 );
        }

        atualizar_graficos_simulacao( false );
    }
    else
    {
        // Atualizando os graficos da planta real
        // Se o programa estiver executando no modo RealSimulacao ou no modo
        // Simulacao, ou seja, se houver a simulacao
        if ( modo_funcionamento != SomenteSimulacao )
        {
            estabelecer_conexao();
            
            nivel_t1 = quanser->ler( real_canal_leitura_t1 );
            nivel_t2 = quanser->ler( real_canal_leitura_t2 );
        }
        
        atualizar_graficos_planta_real();
        
        // Atualizando os graficos da simulacao
        // Se o programa estiver executando no modo RealSimulacao ou no modo
        // Simulacao, ou seja, se houver a simulacao
        if ( modo_funcionamento != SomenteReal )
        {
            nivel_t1 = simulacao->ler( sim_canal_leitura_t1 );
            nivel_t2 = simulacao->ler( sim_canal_leitura_t2 );
        }

        atualizar_graficos_simulacao();
    }

    imgs_barra_status[1].setPixmap( QPixmap( ":imgs/player_play_22x22.png" ) );
    imgs_barra_status[1].setToolTip( utf8( "Sistema em execução" ) );

    desabilitar_campos_parametros();

    temporizador->start( (int)( PERIODO_AMOSTRAGEM * 1000 ) );
}


void JanelaPrincipal :: on_modo_funcionamento_activated( int ind )
{
    modo_funcionamento = (ModoFuncionamento)ind;
}


void JanelaPrincipal :: on_parar_clicked()
{
    temporizador->stop();

    if ( quanser != NULL )
        quanser->desconectar();

    imgs_barra_status[1].setPixmap( QPixmap( ":imgs/player_stop_22x22.png" ) );
    imgs_barra_status[1].setToolTip( "Sistema parado" );

    habilitar_campos_parametros();
}


void JanelaPrincipal :: on_tipo_controlador_t1_activated( int ind )
{
    switch( ind )
    {
        // P
        case 0:
            ui->ki_t1->setEnabled( false );
            ui->kd_t1->setEnabled( false );
            ui->integ_cond_t1->setEnabled( false );
            break;

        // PI
        case 1:
            ui->ki_t1->setEnabled( true );
            ui->kd_t1->setEnabled( false );
            ui->integ_cond_t1->setEnabled( true );
            break;

        // PD
        case 2:
            ui->ki_t1->setEnabled( false );
            ui->kd_t1->setEnabled( true );
            ui->integ_cond_t1->setEnabled( false );
            break;

        // PID
        case 3:
            ui->ki_t1->setEnabled( true );
            ui->kd_t1->setEnabled( true );
            ui->integ_cond_t1->setEnabled( true );
            break;

        // PI-D
        case 4:
            ui->ki_t1->setEnabled( true );
            ui->kd_t1->setEnabled( true );
            ui->integ_cond_t1->setEnabled( true );
            break;

        // I-PD
        case 5:
            ui->ki_t1->setEnabled( true );
            ui->kd_t1->setEnabled( true );
            ui->integ_cond_t1->setEnabled( true );
            break;
    }

    habilitar_aplicar();
}


void JanelaPrincipal :: on_tipo_controlador_t2_activated( int ind )
{
    switch( ind )
    {
        // P
        case 0:
            ui->ki_t2->setEnabled( false );
            ui->kd_t2->setEnabled( false );
            ui->integ_cond_t2->setEnabled( false );
            break;

        // PI
        case 1:
            ui->ki_t2->setEnabled( true );
            ui->kd_t2->setEnabled( false );
            ui->integ_cond_t2->setEnabled( true );
            break;

        // PD
        case 2:
            ui->ki_t2->setEnabled( false );
            ui->kd_t2->setEnabled( true );
            ui->integ_cond_t2->setEnabled( false );
            break;

        // PID
        case 3:
            ui->ki_t2->setEnabled( true );
            ui->kd_t2->setEnabled( true );
            ui->integ_cond_t2->setEnabled( true );
            break;

        // PI-D
        case 4:
            ui->ki_t2->setEnabled( true );
            ui->kd_t2->setEnabled( true );
            ui->integ_cond_t2->setEnabled( true );
            break;

        // I-PD
        case 5:
            ui->ki_t2->setEnabled( true );
            ui->kd_t2->setEnabled( true );
            ui->integ_cond_t2->setEnabled( true );
            break;
    }

    habilitar_aplicar();
}


void JanelaPrincipal :: salvar_dados_graficos()
{
    salvar = new Salvar( this, Salvar::SalvarDados );
    salvar->exec();

    QStringList arquivos = salvar->nomes_arquivos();

    switch( salvar->result() )
    {
        case QDialog::Accepted:
            // A variavel 'arquivos' sempre tera 4 valores referentes a:
            // real_niveis, real_erro_sc, sim_niveis e sim_erro_sc
            if ( arquivos[0] != "" )
            {
                ui->real_niveis->salvar( arquivos[0] );
            }

            if ( arquivos[1] != "" )
            {
                ui->real_erro_sc->salvar( arquivos[1] );
            }

            if ( arquivos[2] != "" )
            {
                ui->sim_niveis->salvar( arquivos[2] );
            }

            if ( arquivos[3] != "" )
            {
                ui->sim_erro_sc->salvar( arquivos[3] );
            }
            
            ui->barra_status->showMessage( "Dados salvos com sucesso.", 
                                           4000 );
            break;

        case QDialog::Rejected:
            ui->barra_status->showMessage( utf8( "Os dados não foram salvos." ),
                                           4000 );
            break;
    }

    delete salvar;
    salvar = NULL;
}

#endif
