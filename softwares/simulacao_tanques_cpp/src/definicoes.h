#ifndef DEFINICOES_H_
#define DEFINICOES_H_

#include <cmath>

// Aceleracao da gravidade para Natal/RN
#define ACELERACAO_GRAVIDADE_M_S 9.780705618678075
#define ACELERACAO_GRAVIDADE_CM_S 100 * ACELERACAO_GRAVIDADE_M_S
#define g ACELERACAO_GRAVIDADE_CM_S

#define PERIODO_AMOSTRAGEM 0.1 // segundos

// Informacoes dos manuais da Quanser
#define DIAM_ENTRADA_Out1 0.635                                        // cm
#define DIAM_ENTRADA_Out2 0.4763                                       // cm
#define RAIO_ENTRADA_Out1 DIAM_ENTRADA_Out1/2                          // cm
#define RAIO_ENTRADA_Out2 DIAM_ENTRADA_Out2/2                          // cm
#define AREA_ENTRADA_Out1 M_PI * RAIO_ENTRADA_Out1 * RAIO_ENTRADA_Out1 // cm^2
#define AREA_ENTRADA_Out2 M_PI * RAIO_ENTRADA_Out2 * RAIO_ENTRADA_Out2 // cm^2

#define DIAM_SAIDA_PEQ 0.3175                                 // cm
#define DIAM_SAIDA_MED 0.47625                                // cm
#define DIAM_SAIDA_GDE 0.555625                               // cm
#define RAIO_SAIDA_PEQ DIAM_SAIDA_PEQ/2                       // cm
#define RAIO_SAIDA_MED DIAM_SAIDA_MED/2                       // cm
#define RAIO_SAIDA_GDE DIAM_SAIDA_GDE/2                       // cm
#define AREA_SAIDA_PEQ M_PI * RAIO_SAIDA_PEQ * RAIO_SAIDA_PEQ // cm^2
#define AREA_SAIDA_MED M_PI * RAIO_SAIDA_MED * RAIO_SAIDA_MED // cm^2
#define AREA_SAIDA_GDE M_PI * RAIO_SAIDA_GDE * RAIO_SAIDA_GDE // cm^2
#define a1_peq AREA_SAIDA_PEQ
#define a1_med AREA_SAIDA_MED
#define a1_gde AREA_SAIDA_GDE
#define a2_peq AREA_SAIDA_PEQ
#define a2_med AREA_SAIDA_MED
#define a2_gde AREA_SAIDA_GDE

#define AREA_SECCAO_TRANSVERSAL 15.5179 // cm^2
#define A1 AREA_SECCAO_TRANSVERSAL
#define A2 AREA_SECCAO_TRANSVERSAL

#define GANHO_MOD_POT 5.0
#define Km 4.6
#define GANHO_SENSOR 1 / 6.25

// Canais para comunicacao com a planta
#define CANAL_ESCRITA_T1 0
#define CANAL_ESCRITA_T2 1
#define CANAL_LEITURA_T1 0
#define CANAL_LEITURA_T2 1
#define TANQUE_1 0
#define TANQUE_2 1

// Tensoes para cada um dos niveis
#define tensao_nivel( nivel )\
(\
    ( ( a1_med / Km ) * sqrt( 2.0 * g * nivel ) ) / GANHO_MOD_POT\
)


#endif
