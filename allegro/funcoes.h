#ifndef FUNCOES_H
#define FUNCOES_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>    

#define SCREEN_W 2500 // tamanho da janela em largura
#define SCREEN_H 1500 // tamanho da janela em altura
#define ESCALA 4 // ESCALA dos sprites
#define SPR_NAVE_T_W 41   // largura de um frame da sprite
#define SPR_NAVE_T_H 32   // altura de um frame
#define FRAME_COUNT 5 // qtd de frames no spritespr_nave
#define SPEED 7     // velocidade
#define TEMPO_ENTRE_COLISIONS 2 // tempo minimo entre tiros em colisões entre a nave e um meteoro em segundos
#define SPR_BALA_T_W 13   // largura de um frame da sprite da bala
#define SPR_BALA_T_H 5   // altura de um frame da sprite da bala
#define BALA_SPEED 14 // velocidade da bala
#define TEMPO_ENTRE_BALAS 0.45 // tempo minimo entre tiros em segundos
#define SPR_METEORO_G_T_W 48   // largura de um frame da sprite do meteoro grande
#define SPR_METEORO_G_T_H 48   // altura de um frame da sprite do meteoro grande
#define METEORO_G_SPEED 5 // velocidade do meteoro grande
#define METEORO_G_VIDA 3 // vida do meteoro grande
#define METEORO_G_VEL_ROTACAO 0.06 // velocidade de rotação do meteoro grande
#define SPR_METEORO_M_T_W 32   // largura de um frame da sprite do meteoro médio
#define SPR_METEORO_M_T_H 32   // altura de um frame da sprite do meteoro médio
#define METEORO_M_SPEED 8 // velocidade do meteoro médio
#define METEORO_M_VIDA 2 // vida do meteoro médio
#define METEORO_M_VEL_ROTACAO 0.1 // velocidade de rotação do meteoro médio
#define SPR_METEORO_P_T_W 16   // largura de um frame da sprite do meteoro médio
#define SPR_METEORO_P_T_H 16   // altura de um frame da sprite do meteoro médio
#define METEORO_P_SPEED 12 // velocidade do meteoro médio
#define METEORO_P_VIDA 1 // vida do meteoro médio
#define METEORO_P_VEL_ROTACAO 0.15 // velocidade de rotação do meteoro médio
#define SPR_CORACAO_T_W 32  // largura de um frame da sprite do coração
#define SPR_CORACAO_T_H 32  // altura de um frame da sprite do coração
#define SPR_ESTRELAS_G_T_W 8  // largura de um frame da sprite da estrela
#define SPR_ESTRELAS_G_T_H 8  // altura de um frame da sprite da estrela
#define NUM_ESTRELAS_G 400 // numero de estrelas
#define SPR_ESTRELAS_P_T_W 4  // largura de um frame da sprite da estrela
#define SPR_ESTRELAS_P_T_H 4  // altura de um frame da sprite da estrela
#define NUM_ESTRELAS_P 600 // numero de estrelas

typedef struct {
    float x, y;
} Ponto;


float cross(Ponto a, Ponto b, Ponto c);

// produto escalar
static inline float dot(float ax, float ay, float bx, float by);

bool pontoDentroTriangulo(Ponto p, Ponto a, Ponto b, Ponto c);

bool pontoDentroQuadrado(Ponto p, Ponto q[4]);

bool segmentosCruzam(Ponto p1, Ponto p2, Ponto q1, Ponto q2);

bool colide_nave_meteoro(Ponto nave[3], Ponto meteoro[4]);

// projeta os vértices num eixo e retorna min/max
void projecao_poligno(float *vertices, float axis_x, float axis_y, float *min, float *max);

// verifica colisão entre 2 retângulos rotacionados
bool colide_retangulos(float *vertsA, float *vertsB);

#endif