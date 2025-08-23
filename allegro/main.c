#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>    

#define SCREEN_W 1800 // tamanho da janela em largura
#define SCREEN_H 1200 // tamanho da janela em altura
#define SPR_NAVE_T_W 41   // largura de um frame da sprite
#define SPR_NAVE_T_H 32   // altura de um frame
#define FRAME_COUNT 5 // qtd de frames no spritespr_nave
#define SPEED 7     // velocidade no eixo x


enum KEYS { UP, LEFT, RIGHT, SPACE, KEY_COUNT };

int main() {
    if (!al_init()) {
        printf("Erro ao inicializar Allegro!\n");
        return -1;
    }

    if (!al_init_image_addon()) {
        printf("Erro ao inicializar add-on de imagem!\n");
        return -1;
    }

    if (!al_install_keyboard()) {
        printf("Erro ao inicializar teclado!\n");
        return -1;
    }

    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_W, SCREEN_H);
    if (!display) {
        printf("Erro ao criar janela!\n");
        return -1;
    }

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0); // 60 FPS
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_keyboard_event_source());

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
    
    bool keys[KEY_COUNT] = { false };
    float x = SCREEN_W/2, y = SCREEN_H/2;
    int frame = 0;
    double frame_time = 0;
    double frame_delay = 0.25; // tempo entre frames da anima��o em segundos
    float angulo = 0;
	int escala = 4;
    float rotacao_por_segundo = 4.0 / 60.0;

    al_start_timer(timer);

    bool running = true;
    while (running) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = 0;
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_ESCAPE: running = false; break;
            case ALLEGRO_KEY_UP:     keys[UP] = true; break;
            case ALLEGRO_KEY_LEFT:   keys[LEFT] = true; break;
            case ALLEGRO_KEY_RIGHT:  keys[RIGHT] = true; break;
            case ALLEGRO_KEY_SPACE:  keys[SPACE] = true; break;
            }
        } else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
            switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_UP:     keys[UP] = false; break;
            case ALLEGRO_KEY_LEFT:   keys[LEFT] = false; break;
            case ALLEGRO_KEY_RIGHT:  keys[RIGHT] = false; break;
            case ALLEGRO_KEY_SPACE:  keys[SPACE] = false; break;
            }
        } 
        if (ev.type == ALLEGRO_EVENT_TIMER) {
            // Atualiza frame da sprite (anima��o)
            frame_time += 1.0 / 60.0;
            if (frame_time >= frame_delay) {
                frame = (frame + 1) % FRAME_COUNT;
                frame_time = 0;
            } 

            // Limpa tela
            al_clear_to_color(al_map_rgb(0, 0, 0));

            
            if (keys[LEFT]) {
                angulo -= rotacao_por_segundo; // rotaciona

                if (angulo <= 0) angulo = 2 * ALLEGRO_PI;
            }
            if (keys[RIGHT]) {
                angulo += rotacao_por_segundo; // rotaciona

				if (angulo >= 2 * ALLEGRO_PI) angulo = 0;
            }
            if (keys[UP]) {
                // Atualiza posi��o
                x += SPEED * cos(angulo);
                y += SPEED * sin(angulo);

                if (x > SCREEN_W) x = -SPR_NAVE_T_W;
				else if (x < -SPR_NAVE_T_W) x = SCREEN_W;
                if (y > SCREEN_H) y = -SPR_NAVE_T_H;
                else if (y < -SPR_NAVE_T_H) y = SCREEN_H;

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
                al_map_rgb(255, 255, 255), SPR_NAVE_T_H/2 - 7, SPR_NAVE_T_H/2, (int)x, (int)y, escala, escala, angulo, 0);
            }

            // Desenha apenas o frame atual
            //al_draw_bitmap_region(spr_nave, frame * SPR_NAVE_T_W, 0, SPR_NAVE_T_W, SPR_NAVE_T_H, x, 10, 0);
			//al_draw_scaled_bitmap(spr_nave, frame * SPR_NAVE_T_W, 0, SPR_NAVE_T_W, SPR_NAVE_T_H, x, 50, SPR_NAVE_T_W * 4, SPR_NAVE_T_H * 4, 0);

			
            //angulo += rotacao_por_segundo;
            //angulo *= (float)(ALLEGRO_PI / 180.0);

            al_flip_display();
        }
    }

    al_destroy_bitmap(spr_nave_movendo);
    al_destroy_bitmap(spr_nave_parada);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}
