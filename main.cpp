#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <algorithm>
#include <stack>
#include <vector>

using namespace std;

#define N 3

typedef struct {
    int matriz[N][N];
    int depth;
    int antigoX;
    int antigoY;
    int foraPosicao;
    int heuristica;
    int numFilhos;
    int nosExpBacktracking;
    int id;
} Npuzzle;

int numerosForaDePosicao(int matriz[N][N]) {
    int sol[N][N] = {1, 2, 3,
                     4, 5, 6,
                     7, 8, 0};

    int custo = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (matriz[i][j] == 0)
                continue;
            else if(matriz[i][j] != sol[i][j]) {
                custo++;
            }
        }
    }

    return custo;
}

int manhattan(int matriz[N][N]) {
    int sol[N][N] = {
            1, 2, 3,
            4, 5, 6,
            7, 8, 0
    };

    int distancia = 0, iterator = 1, x1, y1;
    int x2;
    int y2;

    while(iterator < N*N) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if(sol[i][j] == iterator){
                    x1 = i;
                    y1 = j;
                    break;
                }
            }
        }
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if(matriz[i][j] == iterator){
                    x2 = i;
                    y2 = j;
                    distancia += abs(x2 - x1) + abs(y2 - y1);
                    break;
                }
            }
        }
        iterator++;
    }

    return distancia;
}

bool isSafe(int x, int y) {
    if(x < N && x >= 0 && y < N && y >= 0)
        return true;
    return false;
}

bool isSolution(int matriz[N][N]){

    if(matriz[0][0] == 1 && matriz[0][1] == 2 && matriz[0][2] == 3 &&
       matriz[1][0] == 4 && matriz[1][1] == 5 && matriz[1][2] == 6 &&
       matriz[2][0] == 7 && matriz[2][1] == 8){
        return true;
    }
    return false;

}

void print(int matriz[N][N]){

    int i, j;
    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            cout << matriz[i][j] << " ";
        }
        cout << endl;
    }
}

int findXPosition(int matriz[N][N]){

    int i, j;
    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            if(matriz[i][j] == 0){
                return i;
            }
        }
    }
}

int findYPosition(int matriz[N][N]){

    int i, j;
    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            if(matriz[i][j] == 0){
                return j;
            }
        }
    }
}

bool found_in_stack(int matriz[N][N], stack<Npuzzle> stack) {
    int k = 0;
    while (!stack.empty()) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if(matriz[i][j] == stack.top().matriz[i][j]) {
                    k++;
                }
            }
        }
        if(k == 9) {
            return true;
        }
        stack.pop();
        k = 0;
    }
    return false;
}

void backtracking(Npuzzle npuzzle, int posInicialX, int posInicialY, int n, int depth) {

    //baixo, direita baixo, esquerda baixo, direita, esquerda, cima, cima direita, cima esquerda
    int moveX[8] = {1,0,0,-1};
    int moveY[8] = {0,1,-1,0};
    int temp, proximoX, proximoY;

    int posX = findXPosition(npuzzle.matriz);
    int posY = findYPosition(npuzzle.matriz);

    temp = npuzzle.matriz[posX][posY];
    npuzzle.matriz[posX][posY] = npuzzle.matriz[posInicialX][posInicialY];
    npuzzle.matriz[posInicialX][posInicialY] = temp;

    if(n >= 9) {

        if(isSolution(npuzzle.matriz)) {
            cout << "n puzzle resolvido por Backtracking: " << endl;
            print(npuzzle.matriz);
            cout << "profundidade = " << depth << endl;
            cout << "nos expandidos = " << npuzzle.nosExpBacktracking << endl;
            cout << endl;
        }

        return;
    }

    for (int i = 0; i < 4; i++) {
        proximoX = posX + moveX[i];
        proximoY = posY + moveY[i];
        if(isSafe(proximoX, proximoY)) {
            depth++;
            npuzzle.nosExpBacktracking++;
            backtracking(npuzzle, proximoX, proximoY, n+1, depth);
            if(isSolution(npuzzle.matriz)) {
                return;
            }
        }
    }

    temp = npuzzle.matriz[posX][posY];
    npuzzle.matriz[posX][posY] = npuzzle.matriz[posInicialX][posInicialY];
    npuzzle.matriz[posInicialX][posInicialY] = temp;

}

void buscaLargura(Npuzzle npuzzle) {

    //baixo, direita, cima, esquerda
    int moveX[4] = {1, 0, -1, 0};
    int moveY[4] = {0, 1, 0, -1};

    int temp, proximoX, proximoY, posX, posY;
    bool fracasso = false, sucesso = false;

    int nosExp = 0, nosVis = 0, mediaFilhos = 0;

    deque<Npuzzle> filaFechados;
    deque<Npuzzle> filaAbertos;

    npuzzle.antigoX = -1;
    npuzzle.antigoY = -1;

    filaAbertos.push_back(npuzzle);

    while(!sucesso && !fracasso) {
        if (filaAbertos.empty()){
            fracasso = true;
            cout << "não houve soluçao" << endl;
        }
        else {
            Npuzzle estado = filaAbertos.front();
            nosVis++;
            filaAbertos.pop_front();

            posX = findXPosition(estado.matriz);
            posY = findYPosition(estado.matriz);

            if(isSolution(estado.matriz)) {
                cout << "n puzzle resolvido por Busca em Largura: " << endl;
                print(estado.matriz);
                cout << "profundidade: " << estado.depth << endl;
                cout << "nos expandidos: " << nosExp << endl;
                cout << "nos visitados: " << nosVis << endl;
                cout << "custo: nao calculado"  << endl;
                //cout << "fator de ramificacao medio: " << nosExp/mediaFilhos << endl;
                sucesso = true;
            }
            else {
                for (int i = 0; i < 4; ++i) {
                    proximoX = posX + moveX[i];
                    proximoY = posY + moveY[i];
                    Npuzzle filho = estado;
                    if(isSafe(proximoX, proximoY) && (proximoX != estado.antigoX || proximoY != estado.antigoY)) {
                        temp = filho.matriz[posX][posY];
                        filho.matriz[posX][posY] = filho.matriz[proximoX][proximoY];
                        filho.matriz[proximoX][proximoY] = temp;
                        filho.depth = estado.depth + 1;
                        filho.antigoX = posX;
                        filho.antigoY = posY;
                        estado.numFilhos++;

                        nosExp++;
                        filaAbertos.push_back(filho);
                    }
                }
                /*if(estado.numFilhos == 0)
                    mediaFilhos++;
                cout << mediaFilhos << " ";*/
                filaFechados.push_back(estado);
            }
        }
    }
}

void buscaProfundidade(Npuzzle npuzzle) {

    //baixo, direita, cima, esquerda
    int moveX[4] = {1, 0, -1, 0};
    int moveY[4] = {0, 1, 0, -1};

    int temp, proximoX, proximoY, posX, posY;
    bool fracasso = false, sucesso = false;

    int nosExp = 0, nosVis = 0;

    stack<Npuzzle> pilhaAbertos;
    stack<Npuzzle> pilhaFechados;

    npuzzle.antigoX = -1;
    npuzzle.antigoY = -1;

    pilhaAbertos.push(npuzzle);

    while(!sucesso && !fracasso) {
        if (pilhaAbertos.empty()){
            fracasso = true;
            cout << "não houve solucao" << endl;
        }
        else {
            if(pilhaAbertos.top().depth == 10) {
                pilhaAbertos.pop();
                continue;
            }
            Npuzzle estado = pilhaAbertos.top();
            nosVis++;
            pilhaAbertos.pop();

            posX = findXPosition(estado.matriz);
            posY = findYPosition(estado.matriz);

            if(isSolution(estado.matriz)) {
                cout << "n puzzle resolvido por Busca em Profundidade: " << endl;
                print(estado.matriz);
                cout << "profundidade: " << estado.depth << endl;
                cout << "nos expandidos: " << nosExp << endl;
                cout << "nos visitados: " << nosVis << endl;
                cout << "custo: nao calculado"  << endl;

                sucesso = true;
            }
            else {
                for (int i = 0; i < 4; ++i) {
                    proximoX = posX + moveX[i];
                    proximoY = posY + moveY[i];
                    Npuzzle filho = estado;
                    if(isSafe(proximoX, proximoY) && (proximoX != estado.antigoX || proximoY != estado.antigoY)) {
                        temp = filho.matriz[posX][posY];
                        filho.matriz[posX][posY] = filho.matriz[proximoX][proximoY];
                        filho.matriz[proximoX][proximoY] = temp;
                        filho.depth = estado.depth + 1;
                        filho.antigoX = posX;
                        filho.antigoY = posY;

                        nosExp++;
                        pilhaAbertos.push(filho);
                    }
                }
                while(found_in_stack(pilhaAbertos.top().matriz, pilhaFechados)) {
                    pilhaAbertos.pop();
                }

                pilhaFechados.push(estado);
            }
        }
    }
}

void buscaOrdenada(Npuzzle npuzzle) {

    //baixo, direita, cima, esquerda
    int moveX[4] = {1, 0, -1, 0};
    int moveY[4] = {0, 1, 0, -1};

    int temp, proximoX, proximoY, posX, posY, iterator = 0, posicao = 0;
    bool fracasso = false, sucesso = false;

    int nosExp = 0, nosVis = 0, custo = 0;

    deque<Npuzzle> filaFechados;
    deque<Npuzzle> filaAbertos;

    npuzzle.antigoX = -1;
    npuzzle.antigoY = -1;

    filaAbertos.push_back(npuzzle);

    while(!sucesso && !fracasso) {
        if (filaAbertos.empty()){
            fracasso = true;
            cout << "não houve soluçao" << endl;
        }
        else {
            Npuzzle estado = filaAbertos.front();
            nosVis++;
            for (auto & filaAberto : filaAbertos) {
                if (estado.foraPosicao > filaAberto.foraPosicao) {
                    estado = filaAberto;
                    posicao = iterator;
                }
                iterator++;
            }
            iterator = 0;
            custo += estado.foraPosicao;

            if(filaAbertos.size() == 1)
                filaAbertos.pop_front();
            else
                filaAbertos.erase(filaAbertos.begin()+posicao);

            posX = findXPosition(estado.matriz);
            posY = findYPosition(estado.matriz);

            if(isSolution(estado.matriz)) {
                cout << "n puzzle resolvido em Busca Ordenada: " << endl;
                print(estado.matriz);
                cout << "profundidade: " << estado.depth << endl;
                cout << "nos expandidos: " << nosExp << endl;
                cout << "nos visitados: " << nosVis << endl;
                cout << "custo: " << custo << endl;
                cout << "caminho via id: " << endl;
                for (auto & filaFechado : filaFechados) {
                    cout << filaFechado.id << " ";
                }
                cout << estado.id << endl;
                sucesso = true;
            }
            else {
                for (int i = 0; i < 4; ++i) {
                    proximoX = posX + moveX[i];
                    proximoY = posY + moveY[i];
                    Npuzzle filho = estado;
                    if(isSafe(proximoX, proximoY) && (proximoX != estado.antigoX || proximoY != estado.antigoY)) {
                        temp = filho.matriz[posX][posY];
                        filho.matriz[posX][posY] = filho.matriz[proximoX][proximoY];
                        filho.matriz[proximoX][proximoY] = temp;
                        filho.depth = estado.depth + 1;
                        filho.antigoX = posX;
                        filho.antigoY = posY;
                        filho.foraPosicao = numerosForaDePosicao(filho.matriz);
                        filho.id = estado.id + i + 1;

                        nosExp++;
                        filaAbertos.push_back(filho);
                    }
                }
                int iFila = 0;
                for (auto & filaFechado : filaFechados) {
                    if (filaFechado.id == estado.id)
                        iFila++;
                }
                if (iFila == 0)
                    filaFechados.push_back(estado);

            }
        }
    }
}

void buscaGulosa(Npuzzle npuzzle) {

    //baixo, direita, cima, esquerda
    int moveX[4] = {1, 0, -1, 0};
    int moveY[4] = {0, 1, 0, -1};

    int temp, proximoX, proximoY, posX, posY, iterator = 0, posicao = 0;
    bool fracasso = false, sucesso = false;

    int nosExp = 0, nosVis = 0, custo = 0;

    deque<Npuzzle> filaFechados;
    deque<Npuzzle> filaAbertos;

    npuzzle.antigoX = -1;
    npuzzle.antigoY = -1;

    filaAbertos.push_back(npuzzle);

    while(!sucesso && !fracasso) {
        if (filaAbertos.empty()){
            fracasso = true;
            cout << "não houve soluçao" << endl;
        }
        else {
            Npuzzle estado = filaAbertos.front();
            nosVis++;
            for (auto & filaAberto : filaAbertos) {
                if (estado.heuristica > filaAberto.heuristica) {
                    estado = filaAberto;
                    posicao = iterator;
                }
                iterator++;
            }
            iterator = 0;
            custo += estado.heuristica;
            if(filaAbertos.size() == 1)
                filaAbertos.pop_front();
            else
                filaAbertos.erase(filaAbertos.begin()+posicao);

            posX = findXPosition(estado.matriz);
            posY = findYPosition(estado.matriz);

            if(isSolution(estado.matriz)) {
                cout << "n puzzle resolvido por Busca Gulosa: " << endl;
                print(estado.matriz);
                cout << "profundidade: " << estado.depth << endl;
                cout << "nos expandidos: " << nosExp << endl;
                cout << "nos visitados: " << nosVis << endl;
                cout << "custo: " << custo << endl;
                cout << "caminho via id: " << endl;
                for (auto & filaFechado : filaFechados) {
                    cout << filaFechado.id << " ";
                }
                cout << estado.id << endl;

                sucesso = true;
            }
            else {
                for (int i = 0; i < 4; ++i) {
                    proximoX = posX + moveX[i];
                    proximoY = posY + moveY[i];
                    Npuzzle filho = estado;
                    if(isSafe(proximoX, proximoY) && (proximoX != estado.antigoX || proximoY != estado.antigoY)) {
                        temp = filho.matriz[posX][posY];
                        filho.matriz[posX][posY] = filho.matriz[proximoX][proximoY];
                        filho.matriz[proximoX][proximoY] = temp;
                        filho.depth = estado.depth + 1;
                        filho.antigoX = posX;
                        filho.antigoY = posY;
                        filho.heuristica = manhattan(filho.matriz);
                        filho.id = estado.id + i + 1;

                        nosExp++;
                        filaAbertos.push_back(filho);
                    }
                }
                int iFila = 0;
                for (auto & filaFechado : filaFechados) {
                    if (filaFechado.id == estado.id)
                        iFila++;
                }
                if (iFila == 0)
                    filaFechados.push_back(estado);

            }
        }
    }
}

void aEstrela(Npuzzle npuzzle) {

    //baixo, direita, cima, esquerda
    int moveX[4] = {1, 0, -1, 0};
    int moveY[4] = {0, 1, 0, -1};

    int temp, proximoX, proximoY, posX, posY, iterator = 0, posicao = 0;
    bool fracasso = false, sucesso = false;

    int nosExp = 0, nosVis = 0, custo = 0;

    deque<Npuzzle> filaFechados;
    deque<Npuzzle> filaAbertos;

    npuzzle.antigoX = -1;
    npuzzle.antigoY = -1;

    filaAbertos.push_back(npuzzle);

    while(!sucesso && !fracasso) {
        if (filaAbertos.empty()){
            fracasso = true;
            cout << "não houve soluçao" << endl;
        }
        else {
            Npuzzle estado = filaAbertos.front();
            nosVis++;
            for (auto & filaAberto : filaAbertos) {
                if ((estado.heuristica + estado.depth) > (filaAberto.heuristica + filaAberto.depth)) {
                    estado = filaAberto;
                    posicao = iterator;
                }
                iterator++;
            }
            iterator = 0;
            if(filaAbertos.size() == 1)
                filaAbertos.pop_front();
            else
                filaAbertos.erase(filaAbertos.begin()+posicao);

            posX = findXPosition(estado.matriz);
            posY = findYPosition(estado.matriz);

            if(isSolution(estado.matriz)) {
                cout << "n puzzle resolvido por A*: " << endl;
                print(estado.matriz);
                cout << "profundidade: " << estado.depth << endl;
                cout << "nos expandidos: " << nosExp << endl;
                cout << "nos visitados: " << nosVis << endl;
                cout << "custo: " << estado.heuristica + estado.depth << endl;
                cout << "caminho via id: ";
                for (auto & filaFechado : filaFechados) {
                    cout << filaFechado.id << " ";
                }
                cout << estado.id << endl;
                sucesso = true;
            }
            else {
                for (int i = 0; i < 4; ++i) {
                    proximoX = posX + moveX[i];
                    proximoY = posY + moveY[i];
                    Npuzzle filho = estado;
                    if(isSafe(proximoX, proximoY) && (proximoX != estado.antigoX || proximoY != estado.antigoY)) {
                        temp = filho.matriz[posX][posY];
                        filho.matriz[posX][posY] = filho.matriz[proximoX][proximoY];
                        filho.matriz[proximoX][proximoY] = temp;
                        filho.depth = estado.depth + 1;
                        filho.antigoX = posX;
                        filho.antigoY = posY;
                        filho.heuristica = manhattan(filho.matriz);
                        filho.foraPosicao = numerosForaDePosicao(filho.matriz);
                        filho.id = estado.id + i + 1;

                        nosExp++;
                        filaAbertos.push_back(filho);
                    }
                }
                int iFila = 0;
                for (auto & filaFechado : filaFechados) {
                    if (filaFechado.id == estado.id)
                        iFila++;
                }
                if (iFila == 0)
                    filaFechados.push_back(estado);

            }
        }
    }
}

/*void idaEstrela(Npuzzle npuzzle) {

    //baixo, direita, cima, esquerda
    int moveX[4] = {1, 0, -1, 0};
    int moveY[4] = {0, 1, 0, -1};

    int temp, proximoX, proximoY, posX, posY, iterator = 0, posicao = 0;
    bool fracasso = false, sucesso = false;

    int patamar = npuzzle.heuristica + npuzzle.foraPosicao;
    int patamar_old = -1;

    deque<Npuzzle> descartados;
    deque<Npuzzle> abertos;

    npuzzle.antigoX = -1;
    npuzzle.antigoY = -1;

    Npuzzle estado = npuzzle;

    while(!sucesso && !fracasso) {

        posX = findXPosition(estado.matriz);
        posY = findYPosition(estado.matriz);

        if (patamar_old == patamar) {
            fracasso = true;
            cout << "não houve soluçao" << endl;
        }
        else {
            if (isSolution(estado.matriz) && (estado.heuristica + estado.foraPosicao) <= patamar){
                cout << "acabei" << endl;
                cout << "depth: " << estado.depth << endl;
                print(estado.matriz);
                sucesso = true;
            }
            else {
                if ((estado.heuristica + estado.foraPosicao) > patamar) {
                    descartados.push_back(estado);
                }
                for (int i = 0; i < 4; ++i) {
                    proximoX = posX + moveX[i];
                    proximoY = posY + moveY[i];
                    Npuzzle filho = estado;
                    if (isSafe(proximoX, proximoY) && (proximoX != estado.antigoX || proximoY != estado.antigoY)) {
                        temp = filho.matriz[posX][posY];
                        filho.matriz[posX][posY] = filho.matriz[proximoX][proximoY];
                        filho.matriz[proximoX][proximoY] = temp;
                        filho.depth = estado.depth + 1;
                        filho.antigoX = posX;
                        filho.antigoY = posY;
                        filho.heuristica = manhattan(filho.matriz);
                        filho.foraPosicao = numerosForaDePosicao(filho.matriz);

                        abertos.push_back(filho);
                        break;
                    }
                }

            }
        }
    }
}*/

int main() {
    clock_t time;

    /*int matriz[N][N] =
            {1, 8, 2,
             0, 4, 3,
             7, 6, 5
            };*/

    /*int matriz[N][N] =
            {0, 1, 3,
             4, 2, 5,
             7, 8, 6
            };*/

    int matriz[N][N] =
            {1, 0, 3,
             4, 2, 5,
             7, 8, 6
            };

    /*int matriz[N][N] =
            {1, 2, 3,
             0, 4, 6,
             7, 5, 8
            };*/

    Npuzzle npuzzle;

    for(int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            npuzzle.matriz[i][j] = matriz[i][j];
        }
    }

    npuzzle.depth = 0;
    npuzzle.heuristica = manhattan(npuzzle.matriz);
    npuzzle.foraPosicao = numerosForaDePosicao(npuzzle.matriz);
    npuzzle.numFilhos = 0;
    npuzzle.nosExpBacktracking = 0;
    npuzzle.id = 1;

    int posX = findXPosition(matriz);
    int posY = findYPosition(matriz);

    cout << endl;


    cout << "SOLUCOES DO BACKTRACKING: " << endl;
    time = clock();
    backtracking(npuzzle, posX, posY, 0, 0);
    time = clock() - time;
    cout << "tempo = " << time << "ms ou " << ((float)time)/CLOCKS_PER_SEC << " segundos" <<endl;

    cout << "SOLUCOES DOS OUTROS ALGORITMOS: " << endl;

    time = clock();
    buscaLargura(npuzzle);
    time = clock() - time;
    cout << "tempo = " << time << "ms ou " << ((float)time)/CLOCKS_PER_SEC << "s"<< endl << endl;

    time = clock();
    buscaProfundidade(npuzzle);
    time = clock() - time;
    cout << "tempo = " << time << "ms ou " << ((float)time)/CLOCKS_PER_SEC << "s"<< endl << endl;

    time = clock();
    buscaOrdenada(npuzzle);
    time = clock() - time;
    cout << "tempo = " << time << "ms ou " << ((float)time)/CLOCKS_PER_SEC << "s"<< endl << endl;

    time = clock();
    buscaGulosa(npuzzle);
    time = clock() - time;
    cout << "tempo = " << time << "ms ou " << ((float)time)/CLOCKS_PER_SEC << "s"<< endl << endl;

    time = clock();
    aEstrela(npuzzle);
    time = clock() - time;
    cout << "tempo = " << time << "ms ou " << ((float)time)/CLOCKS_PER_SEC << "s"<< endl << endl;

    return 0;
}
