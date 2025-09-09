#include "funcoes.h"
typedef struct{
    float x, y;
    float angulo;
    float vertices[8]; //retangulo
}Bala;

typedef struct{
    float x, y;
    float angulo;
    float direcao_x, direcao_y;
    float vel_rotacao;
	float vertives[8]; // numero de vertices do colisor do meteoro (retangulo)
    short vida;
    short tipo; // 1-grande, 2-medio, 3-pequeno
    short sprite; // 1, 2 ou 3 (para os meteoros médios e pequenos)
}Meteoro;

typedef struct no_bala{
    Bala bala;
    struct no_bala* prox;
}No_Bala;

typedef struct no_meteoro{
    Meteoro meteoro;
    struct no_meteoro* prox;
}No_Meteoro;

/*
// produto vetorial 2D
float cross(Ponto a, Ponto b, Ponto c) {
    return (b.x - a.x) * (c.y - a.y) -
		(b.y - a.y) * (c.x - a.x); 
}

// produto escalar
static inline float dot(float ax, float ay, float bx, float by) {
    return ax * bx + ay * by;
}

bool pontoDentroTriangulo(Ponto p, Ponto a, Ponto b, Ponto c) {
    float c1 = cross(a, b, p);
    float c2 = cross(b, c, p);
    float c3 = cross(c, a, p);
    
    bool tem_neg = (c1 < 0) || (c2 < 0) || (c3 < 0);
    bool tem_pos = (c1 > 0) || (c2 > 0) || (c3 > 0);

    return !(tem_neg && tem_pos);
}


bool pontoDentroQuadrado(Ponto p, Ponto q[4]) {
    // Divide o quadrado em 2 triângulos e testa
    return pontoDentroTriangulo(p, q[0], q[1], q[2]) ||
        pontoDentroTriangulo(p, q[0], q[2], q[3]);
}


bool segmentosCruzam(Ponto p1, Ponto p2, Ponto q1, Ponto q2) {
    float d1 = cross(p1, p2, q1);
    float d2 = cross(p1, p2, q2);
    float d3 = cross(q1, q2, p1);
    float d4 = cross(q1, q2, p2);

    return ((d1 * d2) <= 0) && ((d3 * d4) <= 0);
}


bool colide_nave_meteoro(Ponto nave[3], Ponto meteoro[4]) {
    // Caso 1: algum vértice do triângulo está dentro do quadrado
    for (int i = 0; i < 3; i++)
        if (pontoDentroQuadrado(nave[i], meteoro))
            return true;

    // Caso 2: algum vértice do quadrado está dentro do triângulo
    for (int i = 0; i < 4; i++)
        if (pontoDentroTriangulo(meteoro[i], nave[0], nave[1], nave[2]))
            return true;

    // Caso 3: interseção de arestas
    for (int i = 0; i < 3; i++) {
        Ponto n1 = nave[i];
        Ponto n2 = nave[(i + 1) % 3];
        for (int j = 0; j < 4; j++) {
            Ponto m1 = meteoro[j];
            Ponto m2 = meteoro[(j + 1) % 4];
            if (segmentosCruzam(n1, n2, m1, m2))
                return true;
        }
    }

    return false; // nada colidiu
}

// projeta os vértices num eixo e retorna min/max
void projecao_poligno(float *vertices, float axis_x, float axis_y, float *min, float *max) {
    *min = *max = dot(vertices[0], vertices[1], axis_x, axis_y);

    for (int i = 1; i < 4; i++) {
        float proj = dot(vertices[i*2], vertices[i*2+1], axis_x, axis_y);
        if (proj < *min) *min = proj;
        if (proj > *max) *max = proj;
    }
}

// verifica colisão entre 2 retângulos rotacionados
bool colide_retangulos(float *vertsA, float *vertsB) {
    float axes[4][2];
    int edges[4][2] = { {0,2}, {2,4}, {4,6}, {6,0} };

    // pega 2 arestas do retângulo A e 2 do B (são os possíveis eixos de separação)
    for (int shape = 0; shape < 2; shape++) {
        float *v = (shape == 0 ? vertsA : vertsB);
        for (int i = 0; i < 2; i++) {
            int i0 = edges[i][0];
            int i1 = edges[i][1];

            float edge_x = v[i1] - v[i0];
            float edge_y = v[i1+1] - v[i0+1];

            // eixo perpendicular (normal)
            axes[shape*2+i][0] = -edge_y;
            axes[shape*2+i][1] =  edge_x;
        }
    }

    // testa projeções em cada eixo
    for (int i = 0; i < 4; i++) {
        float axis_x = axes[i][0];
        float axis_y = axes[i][1];

        // normaliza eixo
        float len = sqrtf(axis_x*axis_x + axis_y*axis_y);
        axis_x /= len;
        axis_y /= len;

        float minA, maxA, minB, maxB;
        projecao_poligno(vertsA, axis_x, axis_y, &minA, &maxA);
        projecao_poligno(vertsB, axis_x, axis_y, &minB, &maxB);

        if (maxA < minB || maxB < minA) {
            // há separação → sem colisão
            return false;
        }
    }
    return true; // todas projeções se sobrepõem → colisão
}*/


void insere_bala(No_Bala** lista, float x, float y, float angulo){
    No_Bala* novo = (No_Bala*)malloc(sizeof(No_Bala));
    novo->bala.x = x;
    novo->bala.y = y;
    novo->bala.angulo = angulo;
    
    float half_w = SPR_BALA_T_W * ESCALA / 2.0;
    float half_h = SPR_BALA_T_H * ESCALA / 2.0;

    float cosA = cos(angulo);
    float sinA = sin(angulo);

    float vx[4] = { -half_w, half_w, half_w , -half_w}; 
    float vy[4] = { -half_h, -half_h, half_h, half_h};

    // aplica rotação
    for (int i = 0; i < 4; i++) {
        float rx = vx[i] * cosA - vy[i] * sinA;
        float ry = vx[i] * sinA + vy[i] * cosA;

        novo->bala.vertices[i * 2]     = rx + x;
        novo->bala.vertices[i * 2 + 1] = ry + y;
    }


    novo->prox = *lista;
    *lista = novo;
}

void insere_meteoro(No_Meteoro** lista, float x, float y, float angulo, short tipo, short vida){
    No_Meteoro* novo = (No_Meteoro*)malloc(sizeof(No_Meteoro));
    novo->meteoro.x = x;
    novo->meteoro.y = y;
    novo->meteoro.angulo = angulo;
    novo->meteoro.tipo = tipo;
    novo->meteoro.vida = vida;
    // defini direcao com base em um angulo aleatorio
    float ang = (float)(rand() % 360) * (float)(ALLEGRO_PI / 180.0);
	//printf("Angulo meteoro: %f\n", ang);
    novo->meteoro.direcao_x = cos(ang);
    novo->meteoro.direcao_y = sin(ang);
    // define uma velocidade de rotação aleatoria
    if (tipo == 1){
        novo->meteoro.vel_rotacao = ((float)(rand() % 100) / 100.0) * METEORO_G_VEL_ROTACAO - METEORO_G_VEL_ROTACAO; // entre -METEORO_G_VEL_ROTACAO e METEORO_G_VEL_ROTACAO
	    //printf("Velocidade de rotacao meteoro: %f\n", novo->meteoro.vel_rotacao);

        // calculo dos vertices do colisor do meteoro grande
        float half_w = SPR_METEORO_G_T_W * ESCALA / 2.8;
        float half_h = SPR_METEORO_G_T_H * ESCALA / 4.8;

        float angle = 3.0 * ALLEGRO_PI / 4.0; // 135 graus
        float cosA = cos(angle);
        float sinA = sin(angle);
        float vx[4] = { -half_w, half_w, half_w , -half_w};
        float vy[4] = { -half_h, -half_h, half_h, half_h};
        // aplica rotação
        for (int i = 0; i < 4; i++) {
            float rx = vx[i] * cosA - vy[i] * sinA;
            float ry = vx[i] * sinA + vy[i] * cosA;

            novo->meteoro.vertives[i * 2]     = rx + x;
            novo->meteoro.vertives[i * 2 + 1] = ry + y;
        }

    } else if (tipo == 2){
        // escolhe um sprite aleatorio para o meteoro médio
        novo->meteoro.sprite = (rand() % 3) + 1; // 1, 2 ou 3
        novo->meteoro.vel_rotacao = ((float)(rand() % 100) / 100.0) * METEORO_M_VEL_ROTACAO - METEORO_M_VEL_ROTACAO; // entre -METEORO_M_VEL_ROTACAO e METEORO_M_VEL_ROTACAO
        //printf("Velocidade de rotacao meteoro: %f\n", novo->meteoro.vel_rotacao);

        // calculo dos vertices do colisor do meteoro médio
        float half_w = SPR_METEORO_M_T_W * ESCALA * 0.35;
        float half_h = SPR_METEORO_M_T_H * ESCALA * 0.35;

        float vx[4] = { -half_w, half_w, half_w , -half_w};
        float vy[4] = { -half_h, -half_h, half_h, half_h};

        for(int i = 0; i < 4; i++){
            novo->meteoro.vertives[i * 2]     = vx[i] + x;
            novo->meteoro.vertives[i * 2 + 1] = vy[i] + y;
        }
        

    } else{
        // escolhe um sprite aleatorio para o meteoro pequeno
        novo->meteoro.sprite = (rand() % 3) + 1; // 1, 2 ou 3
        novo->meteoro.vel_rotacao = ((float)(rand() % 100) / 100.0) * METEORO_P_VEL_ROTACAO - METEORO_P_VEL_ROTACAO; // entre -METEORO_P_VEL_ROTACAO e METEORO_P_VEL_ROTACAO
        //printf("Velocidade de rotacao meteoro: %f\n", novo->meteoro.vel_rotacao);

        // calculo dos vertices do colisor do meteoro pequeno
        float half_w = SPR_METEORO_P_T_W * ESCALA * 0.35;
        float half_h = SPR_METEORO_P_T_H * ESCALA * 0.35;

        float vx[4] = { -half_w, half_w, half_w , -half_w};
        float vy[4] = { -half_h, -half_h, half_h, half_h};

        for(int i = 0; i < 4; i++){
            novo->meteoro.vertives[i * 2]     = vx[i] + x;
            novo->meteoro.vertives[i * 2 + 1] = vy[i] + y;
        }
    }
    
	
    novo->prox = *lista;
    *lista = novo;
}

void salva_pontuacao(int pontos) {
    FILE *file = fopen("pontuacao.txt", "a+");
    if (file == NULL) {
        printf("Erro ao abrir arquivo para salvar pontuação!\n");
        return;
    }

    // Primeiro conta quantos jogos já existem
    int numero_jogos = 0;
    char linha[100];

    rewind(file); // volta para o início
    while (fgets(linha, sizeof(linha), file)) {
        numero_jogos++;
    }

    // Escreve a nova pontuação
    fprintf(file, "jogo %d = %d pontos\n", numero_jogos, pontos);

    fclose(file);
}

void desenha_estrelas(ALLEGRO_BITMAP* spr_estrela_pequena, ALLEGRO_BITMAP* spr_estrela_grande, Ponto* estrelas_p, Ponto* estrelas_g, float escala_das_estrelas){
    for (int i = 0; i < NUM_ESTRELAS_P; i++){
        al_draw_scaled_bitmap(spr_estrela_pequena, 0, 0, SPR_ESTRELAS_P_T_W, SPR_ESTRELAS_P_T_H,
            estrelas_p[i].x, estrelas_p[i].y, escala_das_estrelas * SPR_ESTRELAS_P_T_W, escala_das_estrelas * SPR_ESTRELAS_P_T_H, 0);
    }
    for (int i = 0; i < NUM_ESTRELAS_G; i++){
        al_draw_scaled_bitmap(spr_estrela_grande, 0, 0, SPR_ESTRELAS_G_T_W, SPR_ESTRELAS_G_T_H,
            estrelas_g[i].x, estrelas_g[i].y,  escala_das_estrelas * SPR_ESTRELAS_G_T_W,  escala_das_estrelas * SPR_ESTRELAS_G_T_H, 0);
    }
}



enum KEYS { UP, LEFT, RIGHT, SPACE, KEY_COUNT, PAUSE_P };

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

    if (!al_init_primitives_addon()) {
        printf("Erro ao inicializar primitives!\n");
        return -1;
    }

    if (!al_init_font_addon()) {
        printf("Erro ao inicializar font!\n");
        return -1;
    }

    if(!al_init_ttf_addon()) {
        printf("Erro ao inicializar ttf font!\n");
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


    // carrega a fonte
    ALLEGRO_FONT *fonte = al_load_ttf_font("Pixellettersfull-BnJ5.ttf", 64, 0);
    if (!fonte) {
        fprintf(stderr, "Erro ao carregar fonte!\n");
        return -1;
    }

	// carrega som
    ALLEGRO_SAMPLE* som_tiro = al_load_sample("tutorial1/snd_projetil.wav");
    if (!som_tiro) {
        printf("Erro ao carregar som!\n");
        return -1;
	}

    ALLEGRO_SAMPLE* som_hit = al_load_sample("tutorial1/snd_hit.wav");
    if (!som_hit) {
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

    ALLEGRO_BITMAP* spr_meteoro_m_1 = al_load_bitmap("tutorial1/spr_asteroide_medio_1.png");
    if (!spr_meteoro_m_1) {
        printf("Erro ao carregar spritespr_meteoro_m_1!\n");
        return -1;
    }

    ALLEGRO_BITMAP* spr_meteoro_m_2 = al_load_bitmap("tutorial1/spr_asteroide_medio_2.png");
    if (!spr_meteoro_m_2) {
        printf("Erro ao carregar spritespr_meteoro_m_2!\n");
        return -1;
    }

    ALLEGRO_BITMAP* spr_meteoro_m_3 = al_load_bitmap("tutorial1/spr_asteroide_medio_3.png");
    if (!spr_meteoro_m_3) {
        printf("Erro ao carregar spritespr_meteoro_m_3!\n");
        return -1;
    }

    ALLEGRO_BITMAP* spr_meteoro_p_1 = al_load_bitmap("tutorial1/spr_asteroide_pequeno_1.png");
    if (!spr_meteoro_p_1) {
        printf("Erro ao carregar spritespr_meteoro_p_1!\n");
        return -1;
    }

    ALLEGRO_BITMAP* spr_meteoro_p_2 = al_load_bitmap("tutorial1/spr_asteroide_pequeno_2.png");
    if (!spr_meteoro_p_2) {
        printf("Erro ao carregar spritespr_meteoro_p_2!\n");
        return -1;
    }

    ALLEGRO_BITMAP* spr_meteoro_p_3 = al_load_bitmap("tutorial1/spr_asteroide_pequeno_3.png");
    if (!spr_meteoro_p_3) {
        printf("Erro ao carregar spritespr_meteoro_p_3!\n");
        return -1;
    }

    ALLEGRO_BITMAP* spr_coracao = al_load_bitmap("tutorial1/spr_coracao.png");
    if (!spr_coracao) {
        printf("Erro ao carregar spritespr_coracao!\n");
        return -1;
    }

    ALLEGRO_BITMAP* spr_estrela_grande = al_load_bitmap("tutorial1/spr_estrela_grande.png");
    if (!spr_estrela_grande) {
        printf("Erro ao carregar spritespr_estrela_grande!\n");
        return -1;
    }

    ALLEGRO_BITMAP* spr_estrela_pequena = al_load_bitmap("tutorial1/spr_estrela_pequena.png");
    if (!spr_estrela_pequena) {
        printf("Erro ao carregar spritespr_estrela_pequena!\n");
        return -1;
    }

    
    bool keys[KEY_COUNT] = { false }; // array para armazenar o estado das teclas
    float x = SCREEN_W/2, y = SCREEN_H/2; 
    int frame = 0; // frame atual da anima��o
    double frame_time = 0; // tempo acumulado desde a troca do frame
    double frame_delay = 0.25; // tempo entre frames da anima��o em segundos
    float time_bala = TEMPO_ENTRE_BALAS; // tempo desde o ultimo tiro
    float time_colisions = TEMPO_ENTRE_COLISIONS; // tempo desde o ultimo tiro
    float time_novo_meteoro = 0; // tempo desde o ultimo meteoro
    float porcentagem_do_tempo_meteoros = 1;
    float angulo = 0;
    short vida = 3;
    int pontos = 0;
    short escala_das_estrelas = 2.3;
    ALLEGRO_COLOR color = al_map_rgb(255, 255, 255); // cor branca (padrão)
    float rotacao_por_segundo = 4.0 / 60.0;
	No_Bala* lista_balas = NULL; // Lista encadeada para armazenar as balas
	No_Meteoro* lista_meteoros = NULL; // Lista encadeada para armazenar os meteoros
    // posição das estrelas de fundo
    Ponto estrelas_g[NUM_ESTRELAS_G];
    Ponto estrelas_p[NUM_ESTRELAS_P];
    for (int i = 0; i < NUM_ESTRELAS_G; i++){
        estrelas_g[i].x = rand()%SCREEN_W - SPR_ESTRELAS_G_T_W;
        estrelas_g[i].y = rand()%SCREEN_H - SPR_ESTRELAS_G_T_H;
    }
    for (int i = 0; i < NUM_ESTRELAS_P; i++){
        estrelas_p[i].x = rand()%SCREEN_W - SPR_ESTRELAS_P_T_W;
        estrelas_p[i].y = rand()%SCREEN_H - SPR_ESTRELAS_P_T_H;
    }

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
            case ALLEGRO_KEY_P:      keys[PAUSE_P] = true; /*printf("estou aqui %d\n", keys[PAUSE_P]); debug*/ break;
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
            time_colisions += 1.0 / 60.0;
            time_novo_meteoro += 1.0 / 60.0;
            if (frame_time >= frame_delay) {
                frame = (frame + 1) % FRAME_COUNT;
                frame_time = 0;
            } 

            // Limpa tela
            al_clear_to_color(al_map_rgb(0, 0, 0));

            // Desenha estrelas de fundo
            desenha_estrelas(spr_estrela_pequena, spr_estrela_grande, estrelas_p, estrelas_g, escala_das_estrelas);

            
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

                // atualiza os vertices de colisäo da bala
                for (int i = 0; i < 4; i++) {
                    atual->bala.vertices[i * 2] += BALA_SPEED * cos(atual->bala.angulo);;
                    atual->bala.vertices[i * 2 + 1] += BALA_SPEED * sin(atual->bala.angulo);
                }

                // Desenha a bala
                al_draw_tinted_scaled_rotated_bitmap_region(spr_nave_projetil, 0, 0, SPR_BALA_T_W, SPR_BALA_T_H,
                    al_map_rgb(255, 255, 255), SPR_BALA_T_W / 2, SPR_BALA_T_H / 2,
                    (int)atual->bala.x, (int)atual->bala.y, ESCALA, ESCALA, atual->bala.angulo, 0);

                // debug desenha o colisor da bala
                //al_draw_line(atual->bala.vertices[0], atual->bala.vertices[1], atual->bala.vertices[2], atual->bala.vertices[3], al_map_rgb(255, 0, 255), 2);
                //al_draw_line(atual->bala.vertices[2], atual->bala.vertices[3], atual->bala.vertices[4], atual->bala.vertices[5], al_map_rgb(255, 0, 255), 2);
                //al_draw_line(atual->bala.vertices[4], atual->bala.vertices[5], atual->bala.vertices[6], atual->bala.vertices[7], al_map_rgb(255, 0, 255), 2);
                //al_draw_line(atual->bala.vertices[6], atual->bala.vertices[7], atual->bala.vertices[0], atual->bala.vertices[1], al_map_rgb(255, 0, 255), 2);
                
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
                if (atual_m->meteoro.tipo == 1){
                    atual_m->meteoro.x += METEORO_G_SPEED * atual_m->meteoro.direcao_x;
                    atual_m->meteoro.y += METEORO_G_SPEED * atual_m->meteoro.direcao_y;
                    atual_m->meteoro.angulo += atual_m->meteoro.vel_rotacao;

                    if(atual_m->meteoro.angulo >= 2 * ALLEGRO_PI) atual_m->meteoro.angulo = 0;
                    else if (atual_m->meteoro.angulo < 0) atual_m->meteoro.angulo = 2 * ALLEGRO_PI;

                    float half_w = SPR_METEORO_G_T_W * ESCALA / 2.8;
                    float half_h = SPR_METEORO_G_T_H * ESCALA / 4.8;

                    float angle = 3.0 * ALLEGRO_PI / 4.0; // 135 graus
                    float cosA = cos(angle);
                    float sinA = sin(angle);

                    float vx[4] = { -half_w, half_w, half_w , -half_w}; 
                    float vy[4] = { -half_h, -half_h, half_h, half_h};

                    // aplica rotação
                    float vxr[4], vyr[4];
                    vxr[0] = vx[0] * cosA - vy[0] * sinA;
                    vyr[0] = vx[0] * sinA + vy[0] * cosA;

                    vxr[1] = vx[1] * cosA - vy[1] * sinA;
                    vyr[1] = vx[1] * sinA + vy[1] * cosA;

                    vxr[2] = vx[2] * cosA - vy[2] * sinA;
                    vyr[2] = vx[2] * sinA + vy[2] * cosA;

                    vxr[3] = vx[3] * cosA - vy[3] * sinA;
                    vyr[3] = vx[3] * sinA + vy[3] * cosA;


                    // aplica rotação do meteoro e desloca para posição real
                    for (int i = 0; i < 4; i++) {
                        float xr = vxr[i] * cos(atual_m->meteoro.angulo) - vyr[i] * sin(atual_m->meteoro.angulo);
                        float yr = vxr[i] * sin(atual_m->meteoro.angulo) + vyr[i] * cos(atual_m->meteoro.angulo);
                        atual_m->meteoro.vertives[i * 2] = atual_m->meteoro.x + xr;
                        atual_m->meteoro.vertives[i * 2 + 1] = atual_m->meteoro.y + yr;
                    }

                    // debug desenha o colisor do meteoro
                    /*al_draw_line(atual_m->meteoro.vertives[0], atual_m->meteoro.vertives[1], atual_m->meteoro.vertives[2], atual_m->meteoro.vertives[3], al_map_rgb(0, 255, 255), 2);
                    al_draw_line(atual_m->meteoro.vertives[2], atual_m->meteoro.vertives[3], atual_m->meteoro.vertives[4], atual_m->meteoro.vertives[5], al_map_rgb(0, 255, 255), 2);
                    al_draw_line(atual_m->meteoro.vertives[4], atual_m->meteoro.vertives[5], atual_m->meteoro.vertives[6], atual_m->meteoro.vertives[7], al_map_rgb(0, 255, 255), 2);
                    al_draw_line(atual_m->meteoro.vertives[6], atual_m->meteoro.vertives[7], atual_m->meteoro.vertives[0], atual_m->meteoro.vertives[1], al_map_rgb(0, 255, 255), 2);*/
                    
                    // Mantem meteoro dentro da tela (teletransporte para o lado oposto)
                    if (atual_m->meteoro.x > SCREEN_W) atual_m->meteoro.x = -SPR_METEORO_G_T_W;
                    else if (atual_m->meteoro.x < -SPR_METEORO_G_T_W) atual_m->meteoro.x = SCREEN_W;
                    if (atual_m->meteoro.y > SCREEN_H) atual_m->meteoro.y = -SPR_METEORO_G_T_H;
                    else if (atual_m->meteoro.y < -SPR_METEORO_G_T_H) atual_m->meteoro.y = SCREEN_H;

                    // Desenha o meteoro
                    al_draw_tinted_scaled_rotated_bitmap_region(spr_meteoro_g, 0, 0, SPR_METEORO_G_T_W, SPR_METEORO_G_T_H,
                    al_map_rgb(255, 255, 255), SPR_METEORO_G_T_W / 2, SPR_METEORO_G_T_H / 2,
                    (int)atual_m->meteoro.x, (int)atual_m->meteoro.y, ESCALA, ESCALA, atual_m->meteoro.angulo, 0);

                } else if (atual_m->meteoro.tipo == 2){
                    atual_m->meteoro.x += METEORO_M_SPEED * atual_m->meteoro.direcao_x;
                    atual_m->meteoro.y += METEORO_M_SPEED * atual_m->meteoro.direcao_y;
                    atual_m->meteoro.angulo += atual_m->meteoro.vel_rotacao;

                    if(atual_m->meteoro.angulo >= 2 * ALLEGRO_PI) atual_m->meteoro.angulo = 0;
                    else if (atual_m->meteoro.angulo < 0) atual_m->meteoro.angulo = 2 * ALLEGRO_PI;

                    float half_w = SPR_METEORO_M_T_W * ESCALA * 0.35;
                    float half_h = SPR_METEORO_M_T_H * ESCALA * 0.35;

                    float vx[4] = { -half_w, half_w, half_w , -half_w}; 
                    float vy[4] = { -half_h, -half_h, half_h, half_h};


                    for (int i = 0; i < 4; i++) {
                        float xr = vx[i] * cos(atual_m->meteoro.angulo) - vy[i] * sin(atual_m->meteoro.angulo);
                        float yr = vx[i] * sin(atual_m->meteoro.angulo) + vy[i] * cos(atual_m->meteoro.angulo);
                        atual_m->meteoro.vertives[i * 2] = atual_m->meteoro.x + xr;
                        atual_m->meteoro.vertives[i * 2 + 1] = atual_m->meteoro.y + yr;
                    }

                    if (atual_m->meteoro.x > SCREEN_W) atual_m->meteoro.x = -SPR_METEORO_M_T_W;
                    else if (atual_m->meteoro.x < -SPR_METEORO_M_T_W) atual_m->meteoro.x = SCREEN_W;
                    if (atual_m->meteoro.y > SCREEN_H) atual_m->meteoro.y = -SPR_METEORO_M_T_H;
                    else if (atual_m->meteoro.y < -SPR_METEORO_M_T_H) atual_m->meteoro.y = SCREEN_H;

                    // debug desenha o colisor do meteoro
                    /*al_draw_line(atual_m->meteoro.vertives[0], atual_m->meteoro.vertives[1], atual_m->meteoro.vertives[2], atual_m->meteoro.vertives[3], al_map_rgb(0, 255, 255), 2);
                    al_draw_line(atual_m->meteoro.vertives[2], atual_m->meteoro.vertives[3], atual_m->meteoro.vertives[4], atual_m->meteoro.vertives[5], al_map_rgb(0, 255, 255), 2);
                    al_draw_line(atual_m->meteoro.vertives[4], atual_m->meteoro.vertives[5], atual_m->meteoro.vertives[6], atual_m->meteoro.vertives[7], al_map_rgb(0, 255, 255), 2);
                    al_draw_line(atual_m->meteoro.vertives[6], atual_m->meteoro.vertives[7], atual_m->meteoro.vertives[0], atual_m->meteoro.vertives[1], al_map_rgb(0, 255, 255), 2);*/


                    // debug pausa aqui para ver o colisor
                    /*while (true){
                        al_wait_for_event(queue, &ev);
                        if (ALLEGRO_EVENT_KEY_DOWN){
                            break;
                        }
                        //al_flip_display();
                        
                    }*/
                    

                    if (atual_m->meteoro.sprite == 1){
                        al_draw_tinted_scaled_rotated_bitmap_region(spr_meteoro_m_1, 0, 0, SPR_METEORO_M_T_W, SPR_METEORO_M_T_H,
                        al_map_rgb(255, 255, 255), SPR_METEORO_M_T_W / 2, SPR_METEORO_M_T_H / 2,
                        (int)atual_m->meteoro.x, (int)atual_m->meteoro.y, ESCALA, ESCALA, atual_m->meteoro.angulo, 0);
                    } else if (atual_m->meteoro.sprite == 2){
                        al_draw_tinted_scaled_rotated_bitmap_region(spr_meteoro_m_2, 0, 0, SPR_METEORO_M_T_W, SPR_METEORO_M_T_H,
                        al_map_rgb(255, 255, 255), SPR_METEORO_M_T_W / 2, SPR_METEORO_M_T_H / 2,
                        (int)atual_m->meteoro.x, (int)atual_m->meteoro.y, ESCALA, ESCALA, atual_m->meteoro.angulo, 0);
                    } else{
                        al_draw_tinted_scaled_rotated_bitmap_region(spr_meteoro_m_3, 0, 0, SPR_METEORO_M_T_W, SPR_METEORO_M_T_H,
                        al_map_rgb(255, 255, 255), SPR_METEORO_M_T_W / 2, SPR_METEORO_M_T_H / 2,
                        (int)atual_m->meteoro.x, (int)atual_m->meteoro.y, ESCALA, ESCALA, atual_m->meteoro.angulo, 0);
                    }
                
                } else{
                    atual_m->meteoro.x += METEORO_P_SPEED * atual_m->meteoro.direcao_x;
                    atual_m->meteoro.y += METEORO_P_SPEED * atual_m->meteoro.direcao_y;
                    atual_m->meteoro.angulo += atual_m->meteoro.vel_rotacao;

                    if(atual_m->meteoro.angulo >= 2 * ALLEGRO_PI) atual_m->meteoro.angulo = 0;
                    else if (atual_m->meteoro.angulo < 0) atual_m->meteoro.angulo = 2 * ALLEGRO_PI;

                    float half_w = SPR_METEORO_P_T_W * ESCALA * 0.3;
                    float half_h = SPR_METEORO_P_T_H * ESCALA * 0.3;

                    float vx[4] = { -half_w, half_w, half_w , -half_w}; 
                    float vy[4] = { -half_h, -half_h, half_h, half_h};

                    for (int i = 0; i < 4; i++) {
                        float xr = vx[i] * cos(atual_m->meteoro.angulo) - vy[i] * sin(atual_m->meteoro.angulo);
                        float yr = vx[i] * sin(atual_m->meteoro.angulo) + vy[i] * cos(atual_m->meteoro.angulo);
                        atual_m->meteoro.vertives[i * 2] = atual_m->meteoro.x + xr;
                        atual_m->meteoro.vertives[i * 2 + 1] = atual_m->meteoro.y + yr;
                    }

                    if (atual_m->meteoro.x > SCREEN_W) atual_m->meteoro.x = -SPR_METEORO_P_T_W;
                    else if (atual_m->meteoro.x < -SPR_METEORO_P_T_W) atual_m->meteoro.x = SCREEN_W;

                    if (atual_m->meteoro.y > SCREEN_H) atual_m->meteoro.y = -SPR_METEORO_P_T_H;
                    else if (atual_m->meteoro.y < -SPR_METEORO_P_T_H) atual_m->meteoro.y = SCREEN_H;

                    // debug desenha o colisor do meteoro
                    /*al_draw_line(atual_m->meteoro.vertives[0], atual_m->meteoro.vertives[1], atual_m->meteoro.vertives[2], atual_m->meteoro.vertives[3], al_map_rgb(0, 255, 255), 2);
                    al_draw_line(atual_m->meteoro.vertives[2], atual_m->meteoro.vertives[3], atual_m->meteoro.vertives[4], atual_m->meteoro.vertives[5], al_map_rgb(0, 255, 255), 2);
                    al_draw_line(atual_m->meteoro.vertives[4], atual_m->meteoro.vertives[5], atual_m->meteoro.vertives[6], atual_m->meteoro.vertives[7], al_map_rgb(0, 255, 255), 2);
                    al_draw_line(atual_m->meteoro.vertives[6], atual_m->meteoro.vertives[7], atual_m->meteoro.vertives[0], atual_m->meteoro.vertives[1], al_map_rgb(0, 255, 255), 2);*/

                    if (atual_m->meteoro.sprite == 1){
                        al_draw_tinted_scaled_rotated_bitmap_region(spr_meteoro_p_1, 0, 0, SPR_METEORO_P_T_W, SPR_METEORO_P_T_H,
                        al_map_rgb(255, 255, 255), SPR_METEORO_P_T_W / 2, SPR_METEORO_P_T_H / 2,
                        (int)atual_m->meteoro.x, (int)atual_m->meteoro.y, ESCALA, ESCALA, atual_m->meteoro.angulo, 0);
                    } else if (atual_m->meteoro.sprite == 2){
                        al_draw_tinted_scaled_rotated_bitmap_region(spr_meteoro_p_2, 0, 0, SPR_METEORO_P_T_W, SPR_METEORO_P_T_H,
                        al_map_rgb(255, 255, 255), SPR_METEORO_P_T_W / 2, SPR_METEORO_P_T_H / 2,
                        (int)atual_m->meteoro.x, (int)atual_m->meteoro.y, ESCALA, ESCALA, atual_m->meteoro.angulo, 0);
                    } else{
                        al_draw_tinted_scaled_rotated_bitmap_region(spr_meteoro_p_3, 0, 0, SPR_METEORO_P_T_W, SPR_METEORO_P_T_H,
                        al_map_rgb(255, 255, 255), SPR_METEORO_P_T_W / 2, SPR_METEORO_P_T_H / 2,
                        (int)atual_m->meteoro.x, (int)atual_m->meteoro.y, ESCALA, ESCALA, atual_m->meteoro.angulo, 0);
                    }
                }
                
                
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
                color, // cor de tintura (branco = sem altera��o)
                 SPR_NAVE_T_W/2, SPR_NAVE_T_H / 2, // ponto de origem para rota��o e ESCALA (canto superior esquerdo da regi�o do sprite)
                 (int)x, (int)y, // posi��o na tela (centralizado na nave)
                 ESCALA, ESCALA, // ESCALA em x e y
                 angulo, 0); // �ngulo de rota��o em radianos
            } else {
                // Desenha nave parada
                al_draw_tinted_scaled_rotated_bitmap_region(spr_nave_parada, 0, 0, SPR_NAVE_T_H, SPR_NAVE_T_H,
                color, SPR_NAVE_T_H/2 - 7, SPR_NAVE_T_H/2, (int)x, (int)y, ESCALA, ESCALA, angulo, 0); // o -7 �� para centralizar a imagem, que n��o est�� perfeitamente alinhada
            }

            // Desenha apenas o frame atual
            //al_draw_bitmap_region(spr_nave, frame * SPR_NAVE_T_W, 0, SPR_NAVE_T_W, SPR_NAVE_T_H, x, 10, 0);
			//al_draw_scaled_bitmap(spr_nave, frame * SPR_NAVE_T_W, 0, SPR_NAVE_T_W, SPR_NAVE_T_H, x, 50, SPR_NAVE_T_W * 4, SPR_NAVE_T_H * 4, 0);

			
            //angulo += rotacao_por_segundo;
            //angulo *= (float)(ALLEGRO_PI / 180.0);

            // --- calcula triângulo da nave ---
            float nave_tri[6];
            nave_tri[0] = x + cos(angulo) * 80;  // ponta da frente
            nave_tri[1] = y + sin(angulo) * 80;

            nave_tri[2] = x + cos(angulo + 2) * 60;  // asa esquerda
            nave_tri[3] = y + sin(angulo + 2) * 60;

            nave_tri[4] = x + cos(angulo - 2) * 60;  // asa direita
            nave_tri[5] = y + sin(angulo - 2) * 60;


			//printf("Triangulo da nave: (%f, %f), (%f, %f), (%f, %f)\n", nave_tri[0], nave_tri[1], nave_tri[2], nave_tri[3], nave_tri[4], nave_tri[5]);


            
            // Nave em forma de triângulo
            Ponto nave[3] = {
                { nave_tri[0], nave_tri[1] },
                { nave_tri[2], nave_tri[3] },
                { nave_tri[4], nave_tri[5] }
            };

            // --- Colisão nave x meteoro e bala x meteoro, com remoção segura usando ponteiro-para-ponteiro ---
            for (No_Meteoro** pm = &lista_meteoros; *pm; ) {
                No_Meteoro* m = *pm;

                // 1) Colisão NAVE x METEORO (use os vértices já atualizados de m->meteoro.vertives)
                Ponto mpts[4] = {
                    { m->meteoro.vertives[0], m->meteoro.vertives[1] },
                    { m->meteoro.vertives[2], m->meteoro.vertives[3] },
                    { m->meteoro.vertives[4], m->meteoro.vertives[5] },
                    { m->meteoro.vertives[6], m->meteoro.vertives[7] }
                };

                if (colide_nave_meteoro(nave, mpts) && time_colisions >= TEMPO_ENTRE_COLISIONS) {
                    vida--;
                    time_colisions = 0;
                    if (vida <= 0) {
                        running = false; // fim de jogo
                    }
                }

                // 2) Colisão BALAS x METEORO (pode remover bala e até remover o meteoro)
                bool meteor_removed = false;
                for (No_Bala** pb = &lista_balas; *pb; ) {
                    No_Bala* b = *pb;

                    if (colide_retangulos(b->bala.vertices, m->meteoro.vertives)) {
                        // acerta: toca som, remove bala, aplica dano
                        al_play_sample(som_hit, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

                        // remove bala atual
                        *pb = b->prox;
                        free(b);

                        // aplica dano e verifica remoção do meteoro
                        if (--m->meteoro.vida <= 0) {

                            // remove meteoro atual
                            *pm = m->prox;   // desvincula o nó
                            free(m);
                            
                            meteor_removed = true;

							// meteoro cria novos meteoros menores ao ser destruído
                            if (m->meteoro.tipo == 1) {
                                // se for meteoro grande, cria 2 médios
                                insere_meteoro(&lista_meteoros, m->meteoro.x, m->meteoro.y, 0, 2, METEORO_M_VIDA);
                                insere_meteoro(&lista_meteoros, m->meteoro.x, m->meteoro.y, 0, 2, METEORO_M_VIDA);
                                insere_meteoro(&lista_meteoros, m->meteoro.x, m->meteoro.y, 0, 2, METEORO_M_VIDA);
                                pontos += 10;
                            } else if (m->meteoro.tipo == 2) {
                                // se for meteoro médio, cria 3 pequenos
                                insere_meteoro(&lista_meteoros, m->meteoro.x, m->meteoro.y, 0, 3, METEORO_P_VIDA);
                                insere_meteoro(&lista_meteoros, m->meteoro.x, m->meteoro.y, 0, 3, METEORO_P_VIDA);
                                insere_meteoro(&lista_meteoros, m->meteoro.x, m->meteoro.y, 0, 3, METEORO_P_VIDA);
                                pontos += 5;
                            } else{
                               pontos += 2;
                            }
                            
                            break;           // para de checar balas: meteoro já não existe
                        }
                        // se o meteoro ainda vive, 'pb' já aponta para o próximo (por causa de *pb = b->prox)
                        // então apenas continua sem avançar manualmente
                    }
                    else {
                        // sem colisão: avança o iterador de balas
                        pb = &(*pb)->prox;
                    }
                }

                if (meteor_removed) {
                    // *pm já aponta para o próximo meteoro; não use 'm' aqui (foi liberado)
                    continue; // recomeça o loop com o novo *pm
                }
                else {
                    // meteoro não foi removido: avance normalmente
                    pm = &m->prox;
                }
            }


            // nave piscando depois de colidir
            if (time_colisions < TEMPO_ENTRE_COLISIONS){
                if (((int)(time_colisions * 10) % 2) == 0) // pisca a cada 0.1s
                    color = al_map_rgb(255, 0, 0); // vermelho
                else
                    color = al_map_rgb(255, 255, 255); // branco
            } else {
                color = al_map_rgb(255, 255, 255); // branco
            }

            // desenha o texto
            al_draw_textf(fonte, al_map_rgb(255, 255, 255), 50, 50, 0, "Pontos: %d", pontos);

            // desenha corações de vida 3 e a escala
            for (int i = 0; i < vida; i++) {
                al_draw_scaled_bitmap(spr_coracao, 0, 0, SPR_CORACAO_T_W, SPR_CORACAO_T_H,
                    SCREEN_W - (i + 1) * (SPR_CORACAO_T_W * 3 + 10), 20,
                    SPR_CORACAO_T_W * 3, SPR_CORACAO_T_H * 3, 0);
            }

            //espaunar meteoros nos recantos
            if(time_novo_meteoro > TEMPO_ENTRE_NOVOS_METEOROS*porcentagem_do_tempo_meteoros) {
                time_novo_meteoro = 0;
                if (porcentagem_do_tempo_meteoros > 0.1){
                    porcentagem_do_tempo_meteoros -= 0.03; // aumenta a frequencia de meteoros com o tempo
                }
                
                int edge = rand()%4; // 0=topo, 1=direita, 2=baixo, 3=esquerda
                int posicao = rand()%SCREEN_W; // posição aleatória na borda
                if (edge == 0){ // topo
                    insere_meteoro(&lista_meteoros, posicao, -SPR_METEORO_G_T_H, 0, 1, METEORO_G_VIDA);
                } else if (edge == 1){ // direita
                    insere_meteoro(&lista_meteoros, SCREEN_W + SPR_METEORO_G_T_W, posicao, 0, 1, METEORO_G_VIDA);
                } else if (edge == 2){ // baixo
                    insere_meteoro(&lista_meteoros, posicao, SCREEN_H + SPR_METEORO_G_T_H, 0, 1, METEORO_G_VIDA);
                } else{ // esquerda
                    insere_meteoro(&lista_meteoros, -SPR_METEORO_G_T_W, posicao, 0, 1, METEORO_G_VIDA);
                }
            }

            // depois eu faço
            /*if(keys[PAUSE_P]) {
                // desenha umm retangulo semi-transparente sobre a tela e o texto "PAUSADO"
                
                al_draw_rectangle(0, 0, SCREEN_W, SCREEN_H, al_map_rgba(0, 0, 0, 128), 0);
                al_draw_textf(fonte, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2, 0, "PAUSADO");
                printf("Pausado\n");
            }*/


            al_flip_display();
        }
    }

    // Fim do jogo
    salva_pontuacao(pontos);

    // Libera os recursos criados (padrão) 
    al_destroy_bitmap(spr_nave_movendo);
    al_destroy_bitmap(spr_nave_parada);
	al_destroy_bitmap(spr_nave_projetil);
	al_destroy_bitmap(spr_meteoro_g);
    al_destroy_bitmap(spr_meteoro_m_1);
    al_destroy_bitmap(spr_meteoro_m_2);
    al_destroy_bitmap(spr_meteoro_m_3);
    al_destroy_bitmap(spr_meteoro_p_1);
    al_destroy_bitmap(spr_meteoro_p_2);
    al_destroy_bitmap(spr_meteoro_p_3);
    al_destroy_bitmap(spr_coracao);
    al_destroy_bitmap(spr_estrela_grande);
    al_destroy_bitmap(spr_estrela_pequena);
    al_destroy_sample(som_hit);
    al_destroy_sample(som_tiro);
    al_destroy_font(fonte);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_shutdown_primitives_addon();

    return 0;
}
