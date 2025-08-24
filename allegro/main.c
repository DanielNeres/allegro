#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>    

#define SCREEN_W 1800 // tamanho da janela em largura
#define SCREEN_H 1200 // tamanho da janela em altura
#define SPR_NAVE_T_W 41   // largura de um frame da sprite
#define SPR_NAVE_T_H 32   // altura de um frame
#define FRAME_COUNT 5 // qtd de frames no spritespr_nave
#define SPEED 7     // velocidade no eixo x
#define SPR_BALA_T_W 13   // largura de um frame da sprite da bala
#define SPR_BALA_T_H 5   // altura de um frame da sprite da bala
#define BALA_SPEED 14 // velocidade da bala
#define TEMPO_ENTRE_BALAS 0.2 // tempo minimo entre tiros em segundos
#define SPR_METEORO_G_T_W 48   // largura de um frame da sprite do meteoro grande
#define SPR_METEORO_G_T_H 48   // altura de um frame da sprite do meteoro grande
#define METEORO_G_SPEED 5 // velocidade do meteoro grande
#define METEORO_G_VIDA 3 // vida do meteoro grande
#define METEORO_G_VEL_ROTACAO 0.06 // velocidade de rotação do meteoro grande


typedef struct{
    float x, y;
    float angulo;
}Bala;

typedef struct{
    float x, y;
    float angulo;
    float direcao_x, direcao_y;
    float vel_rotacao;
    short vida;
    short tipo; // 1-grande, 2-medio, 3-pequeno
}Meteoro;

typedef struct no_bala{
    Bala bala;
    struct no_bala* prox;
}No_Bala;

typedef struct no_meteoro{
    Meteoro meteoro;
    struct no_meteoro* prox;
}No_Meteoro;


void insere_bala(No_Bala** lista, float x, float y, float angulo){
    No_Bala* novo = (No_Bala*)malloc(sizeof(No_Bala));
    novo->bala.x = x;
    novo->bala.y = y;
    novo->bala.angulo = angulo;
    novo->prox = *lista;
    *lista = novo;
}

void insere_meteoro(No_Bala** lista, float x, float y, float angulo, short tipo, short vida){
    No_Meteoro* novo = (No_Meteoro*)malloc(sizeof(No_Meteoro));
    novo->meteoro.x = x;
    novo->meteoro.y = y;
    novo->meteoro.angulo = angulo;
    novo->meteoro.tipo = tipo;
    novo->meteoro.vida = vida;
    // defini direcao com base em um angulo aleatorio
    float ang = (float)(rand() % 360) * (float)(ALLEGRO_PI / 180.0);
	printf("Angulo meteoro: %f\n", ang);
    novo->meteoro.direcao_x = cos(ang);
    novo->meteoro.direcao_y = sin(ang);
    // define uma velocidade de rotação aleatoria
	novo->meteoro.vel_rotacao = ((float)(rand() % 100) / 100.0) * METEORO_G_VEL_ROTACAO - METEORO_G_VEL_ROTACAO; // entre -METEORO_G_VEL_ROTACAO e METEORO_G_VEL_ROTACAO
	printf("Velocidade de rotacao meteoro: %f\n", novo->meteoro.vel_rotacao);
    novo->prox = *lista;
    *lista = novo;
}

enum KEYS { UP, LEFT, RIGHT, SPACE, KEY_COUNT };

int main() {

	// Inicializa Allegro e componentes (padrão)
    if (!al_init()) {
        printf("Erro ao inicializar Allegro!\n");
        return -1;
    }

	// Inicializa imagens para carregar sprites
    if (!al_init_image_addon()) {
        printf("Erro ao inicializar add-on de imagem!\n");
        return -1;
    }

	// Inicializa a instalação de eventos do teclado
    if (!al_install_keyboard()) {
        printf("Erro ao inicializar teclado!\n");
        return -1;
    }

	// Inicializa a instalação do audio
    if (!al_install_audio()) {
        printf("Erro ao inicializar áudio!\n");
        return -1;
    }

    if (!al_init_acodec_addon()) {
        printf("Erro ao inicializar codecs de áudio!\n");
        return -1;
    }

	if (!al_reserve_samples(16)) { // reserva 16 canais de som, ou seje, 16 sons podem ser reproduzidos ao mesmo tempo
        printf("Erro ao reservar samples!\n");
        return -1;
    }


	// Inicializa a tela (padrão)
    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_W, SCREEN_H);
    if (!display) {
        printf("Erro ao criar janela!\n");
        return -1;
    }

	// define os eventos que ser��o verificados e o timer (padrão)
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0); // 60 FPS
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();

	// Registra os eventos que ser��o verificados (padrão tirando o keyboard, so precisa se usar a biblioteca do teclado)
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_keyboard_event_source());

	// carrega som
    ALLEGRO_SAMPLE* som_tiro = al_load_sample("tutorial1/snd_projetil.wav");
    if (!som_tiro) {
        printf("Erro ao carregar som!\n");
        return -1;
	}

	// Carrega a sprites
    ALLEGRO_BITMAP* spr_nave_movendo = al_load_bitmap("tutorial1/spr_nave_movendo.png");
    if (!spr_nave_movendo) {
        printf("Erro ao carregar spritespr_nave!\n");
        return -1;
    }

    ALLEGRO_BITMAP* spr_nave_parada = al_load_bitmap("tutorial1/spr_nave_parada.png");
    if (!spr_nave_parada) {
        printf("Erro ao carregar spritespr_nave!\n");
        return -1;
    }

    ALLEGRO_BITMAP* spr_nave_projetil = al_load_bitmap("tutorial1/spr_nave_projetil.png");
    if (!spr_nave_projetil) {
        printf("Erro ao carregar spritespr_nave!\n");
        return -1;
    }

	ALLEGRO_BITMAP* spr_meteoro_g = al_load_bitmap("tutorial1/spr_asteroide_grande.png");
    if (!spr_meteoro_g) {
        printf("Erro ao carregar spritespr_meteoro_g!\n");
        return -1;
	}

    
    bool keys[KEY_COUNT] = { false }; // array para armazenar o estado das teclas
    float x = SCREEN_W/2, y = SCREEN_H/2; 
    int frame = 0; // frame atual da anima��o
    double frame_time = 0; // tempo acumulado desde a troca do frame
    double frame_delay = 0.25; // tempo entre frames da anima��o em segundos
    float time_bala = TEMPO_ENTRE_BALAS; // tempo desde o ultimo tiro
    float angulo = 0;
	int escala = 4;
    float rotacao_por_segundo = 4.0 / 60.0;
	No_Bala* lista_balas = NULL; // Lista encadeada para armazenar as balas
	No_Bala* lista_meteoros = NULL; // Lista encadeada para armazenar os meteoros


    al_start_timer(timer);

	// insere 2 meteoros grandes
	insere_meteoro(&lista_meteoros, 100, 100, 0, 1, METEORO_G_VIDA);
    insere_meteoro(&lista_meteoros, 400, 400, 0, 1, METEORO_G_VIDA);

    bool running = true;
    while (running) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = 0;
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            // logica para verificar quais teclas foram ou estão apertadas
            switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_ESCAPE: running = false; break;
            case ALLEGRO_KEY_UP:     keys[UP] = true; break;
            case ALLEGRO_KEY_LEFT:   keys[LEFT] = true; break;
            case ALLEGRO_KEY_RIGHT:  keys[RIGHT] = true; break;
            case ALLEGRO_KEY_SPACE:  keys[SPACE] = true; break;
            }
        } else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
            // logica para verificar quais teclas não estão mais apertadas
            switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_UP:     keys[UP] = false; break;
            case ALLEGRO_KEY_LEFT:   keys[LEFT] = false; break;
            case ALLEGRO_KEY_RIGHT:  keys[RIGHT] = false; break;
            case ALLEGRO_KEY_SPACE:  keys[SPACE] = false; break;
            }
        } 
        if (ev.type == ALLEGRO_EVENT_TIMER) {
            // Atualiza frame da sprite (anima��o da nave)
            frame_time += 1.0 / 60.0;
			time_bala += 1.0 / 60.0;
            if (frame_time >= frame_delay) {
                frame = (frame + 1) % FRAME_COUNT;
                frame_time = 0;
            } 

            // Limpa tela
            al_clear_to_color(al_map_rgb(255, 255, 255));

            
            if (keys[LEFT]) {
                angulo -= rotacao_por_segundo; // rotaciona

                if (angulo <= 0) angulo = 2 * ALLEGRO_PI;
            }
            if (keys[RIGHT]) {
                angulo += rotacao_por_segundo; // rotaciona

				if (angulo >= 2 * ALLEGRO_PI) angulo = 0;
            }
            if (keys[SPACE] && time_bala >= TEMPO_ENTRE_BALAS) {
                time_bala = 0;
				// inicia bala na lista
                insere_bala(&lista_balas, x + 30*cos(angulo), y + 30*sin(angulo), angulo);
				// toca som
				al_play_sample(som_tiro, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}

            // Atualiza e desenha balas
            No_Bala* ant = NULL;
            No_Bala* atual = lista_balas;
            while (atual != NULL) {
                // Atualiza posi����o da bala
                atual->bala.x += BALA_SPEED * cos(atual->bala.angulo);
                atual->bala.y += BALA_SPEED * sin(atual->bala.angulo);
                // Desenha a bala
                al_draw_tinted_scaled_rotated_bitmap_region(spr_nave_projetil, 0, 0, SPR_BALA_T_W, SPR_BALA_T_H,
                    al_map_rgb(255, 255, 255), SPR_BALA_T_W / 2, SPR_BALA_T_H / 2,
                    (int)atual->bala.x, (int)atual->bala.y, escala, escala, atual->bala.angulo, 0);
                // Remove a bala se sair da tela
                if (atual->bala.x < -SPR_BALA_T_W || atual->bala.x > SCREEN_W || atual->bala.y < -SPR_BALA_T_H || atual->bala.y > SCREEN_H) {
                    No_Bala* temp = atual;
                    if (ant == NULL) {
                        lista_balas = atual->prox;
                        atual = lista_balas;
                    } else {
                        ant->prox = atual->prox;
                        atual = ant->prox;
                    }
                    free(temp);
                }
                else {
                    ant = atual;
                    atual = atual->prox;
                }
            }

			// Atualiza e desenha meteoros grandes
			No_Meteoro* ant_m = NULL;
			No_Meteoro* atual_m = lista_meteoros;
            while (atual_m != NULL) {
                // Atualiza posi����o do meteoro
                atual_m->meteoro.x += METEORO_G_SPEED * atual_m->meteoro.direcao_x;
                atual_m->meteoro.y += METEORO_G_SPEED * atual_m->meteoro.direcao_y;
                atual_m->meteoro.angulo += atual_m->meteoro.vel_rotacao;
                // Mantem meteoro dentro da tela (teletransporte para o lado oposto)
                if (atual_m->meteoro.x > SCREEN_W) atual_m->meteoro.x = -SPR_METEORO_G_T_W;
                else if (atual_m->meteoro.x < -SPR_METEORO_G_T_W) atual_m->meteoro.x = SCREEN_W;
                if (atual_m->meteoro.y > SCREEN_H) atual_m->meteoro.y = -SPR_METEORO_G_T_H;
                else if (atual_m->meteoro.y < -SPR_METEORO_G_T_H) atual_m->meteoro.y = SCREEN_H;
                // Desenha o meteoro
                al_draw_tinted_scaled_rotated_bitmap_region(spr_meteoro_g, 0, 0, SPR_METEORO_G_T_W, SPR_METEORO_G_T_H,
                    al_map_rgb(255, 255, 255), SPR_METEORO_G_T_W / 2, SPR_METEORO_G_T_H / 2,
                    (int)atual_m->meteoro.x, (int)atual_m->meteoro.y, escala, escala, atual_m->meteoro.angulo, 0);
                ant_m = atual_m;
                atual_m = atual_m->prox;
            }

            if (keys[UP]) {
                // Atualiza posi��o (indo para frente na dire����o do angulo)
                x += SPEED * cos(angulo);
                y += SPEED * sin(angulo);


                // Mantem nave dentro da tela (teletransporte para o lado oposto)
                if (x > SCREEN_W) x = -SPR_NAVE_T_W;
				else if (x < -SPR_NAVE_T_W) x = SCREEN_W;
                if (y > SCREEN_H) y = -SPR_NAVE_T_H;
                else if (y < -SPR_NAVE_T_H) y = SCREEN_H;

                // Desenha nave em movimento
                al_draw_tinted_scaled_rotated_bitmap_region(spr_nave_movendo, 
				frame * SPR_NAVE_T_W, 0, SPR_NAVE_T_W, SPR_NAVE_T_H, // regi�o do sprite a ser desenhada 
                al_map_rgb(255, 255, 255), // cor de tintura (branco = sem altera��o)
                 SPR_NAVE_T_W/2, SPR_NAVE_T_H / 2, // ponto de origem para rota��o e escala (canto superior esquerdo da regi�o do sprite)
                 (int)x, (int)y, // posi��o na tela (centralizado na nave)
                 escala, escala, // escala em x e y
                 angulo, 0); // �ngulo de rota��o em radianos
            } else {
                // Desenha nave parada
                al_draw_tinted_scaled_rotated_bitmap_region(spr_nave_parada, 0, 0, SPR_NAVE_T_H, SPR_NAVE_T_H,
                al_map_rgb(255, 255, 255), SPR_NAVE_T_H/2 - 7, SPR_NAVE_T_H/2, (int)x, (int)y, escala, escala, angulo, 0); // o -7 �� para centralizar a imagem, que n��o est�� perfeitamente alinhada
            }

            // Desenha apenas o frame atual
            //al_draw_bitmap_region(spr_nave, frame * SPR_NAVE_T_W, 0, SPR_NAVE_T_W, SPR_NAVE_T_H, x, 10, 0);
			//al_draw_scaled_bitmap(spr_nave, frame * SPR_NAVE_T_W, 0, SPR_NAVE_T_W, SPR_NAVE_T_H, x, 50, SPR_NAVE_T_W * 4, SPR_NAVE_T_H * 4, 0);

			
            //angulo += rotacao_por_segundo;
            //angulo *= (float)(ALLEGRO_PI / 180.0);

            al_flip_display();
        }
    }

    // Libera os recursos criados (padrão) 
    al_destroy_bitmap(spr_nave_movendo);
    al_destroy_bitmap(spr_nave_parada);
	al_destroy_bitmap(spr_nave_projetil);
	al_destroy_bitmap(spr_meteoro_g);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}
