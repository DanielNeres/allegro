#include "funcoes.h"

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
}