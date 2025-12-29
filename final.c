#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define NUM_NUMEROS 5
#define NUM_ESTRELAS 2
#define MAX_NUMEROS 50
#define MAX_ESTRELAS 12
#define MAX_APOSTAS 5
#define NOME_ARQUIVO "euromilhoes.csv"
#define USERNAME "admin"
#define PASSWORD "admin123"

// Estruturas principais
typedef struct {
    int numeros[NUM_NUMEROS];
    int estrelas[NUM_ESTRELAS];
} Aposta;

typedef struct {
    Aposta aposta;
    Aposta sorteio;
    int acertos_numeros;
    int acertos_estrelas;
    int id_premio;
    char descricao_premio[100];
    float valor_premio;
    float premio_total_jogo;
} ApostaCompleta;

typedef struct {
    int id;
    int acertos_numeros;
    int acertos_estrelas;
    char descricao[100];
    float percentagem;
} CategoriaPremio;

// Prototipos
void limparBuffer();
void ordenar(int array[], int tamanho);
int numeroRepetido(int array[], int tamanho, int numero);
void mostrarAposta(int numeros[], int estrelas[], const char *titulo);
void mostrarApostaComIndices(int numeros[], int estrelas[]);
void mostrarMenuPrincipal();
void bannerMenuPrincipal();
void clearScreen();
float lerPremioTotal();
void lerApostaUsuario(int numeros[], int estrelas[]);
void corrigirAposta(int numeros[], int estrelas[]);
void gerarSorteio(int numeros[], int estrelas[]);
int contarAcertos(int aposta[], int sorteio[], int tamanho);
void inicializarCategoriasPremio(CategoriaPremio categorias[]);
int verificarCategoriaPremio(CategoriaPremio categorias[], int acertos_numeros, int acertos_estrelas);
float calcularValorPremio(int id_premio, float premio_total, CategoriaPremio categorias[]);
int fazerLogin();
void salvarApostaCompletaCSV(ApostaCompleta *ac);
void lerUltimasApostas();
void mostrarApostaCompleta(ApostaCompleta *ac, int numero_aposta);
void printNumeroBoard();
void printEstrelaBoard(); 

// Funções

void printNumeroBoard() {
    printf("   ===== TABELA DE NUMEROS ====\n");
    printf("   |   1  2  3  4  5  6  7    |\n");
    printf("   |   8  9  10 11 12 13 14   |\n");
    printf("   |   15 16 17 18 19 20 21   |\n");
    printf("   |   22 23 24 25 26 27 28   |\n");
    printf("   |   29 30 31 32 33 34 35   |\n");
    printf("   |   36 37 38 39 40 41 42   |\n");
    printf("   |   43 44 45 46 47 48 49   |\n");
    printf("   |   50                     |\n");
    printf("   ============================\n");
}

void printEstrelaBoard() {
    printf("   == TABELA DE ESTRELAS ==\n");
    printf("   |   1  2  3  4  5  6   |\n");
    printf("   |   7  8  9  10 11 12  |\n");
    printf("   ========================\n");
}

int fazerLogin() {
    char username[50];
    char password[50];

    printf("\n   === LOGIN NECESSARIO ===\n");
    printf("   Acesso restrito aos administradores.\n");

    printf("   Nome de utilizador: ");
    limparBuffer();
    if (fgets(username, sizeof(username), stdin) == NULL) {
        return 0;
    }
    username[strcspn(username, "\n")] = 0;

    printf("   Password: ");
    if (fgets(password, sizeof(password), stdin) == NULL) {
        return 0;
    }
    password[strcspn(password, "\n")] = 0;

    if (strcmp(username, USERNAME) == 0 && strcmp(password, PASSWORD) == 0) {
        printf("\n   Login bem-sucedido! Acesso concedido.\n");
        return 1;
    }

    printf("\n   Credenciais incorretas. Acesso negado.\n");
    return 0;
}

void ordenar(int array[], int tamanho) {
    for (int i = 0; i < tamanho - 1; i++) {
        for (int j = 0; j < tamanho - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

int numeroRepetido(int array[], int tamanho, int numero) {
    for (int i = 0; i < tamanho; i++) {
        if (array[i] == numero) {
            return 1;
        }
    }
    return 0;
}

void mostrarAposta(int numeros[], int estrelas[], const char *titulo) {
    if (titulo != NULL && strlen(titulo) > 0) {
        printf("%s\n", titulo);
    }
    printf("   Numeros: ");
    for (int i = 0; i < NUM_NUMEROS; i++) {
        printf("%02d ", numeros[i]);
    }
    printf("\n   Estrelas: ");
    for (int i = 0; i < NUM_ESTRELAS; i++) {
        printf("%02d ", estrelas[i]);
    }
    printf("\n");
}

void mostrarApostaComIndices(int numeros[], int estrelas[]) {
    printf("   Numeros:\n");
    for (int i = 0; i < NUM_NUMEROS; i++) {
        printf("  [%d] %02d\n", i + 1, numeros[i]);
    }
    printf("\n   Estrelas:\n");
    for (int i = 0; i < NUM_ESTRELAS; i++) {
        printf("  [%d] %02d\n", i + 1, estrelas[i]);
    }
}

float lerPremioTotal() {
    float premio;
    printf("   Qual o valor total do premio em jogo? (ex: 180000000): ");
    while (1) {
        if (scanf("%f", &premio) != 1) {
            printf("   Valor invalido! Digite um valor numerico: ");
            limparBuffer();
        } else if (premio <= 0) {
            printf("   Valor invalido! O premio deve ser maior que 0: ");
        } else {
            break;
        }
    }
    return premio;
}

void lerApostaUsuario(int numeros[], int estrelas[]) {
    for (int i = 0; i < NUM_NUMEROS; i++) {
        numeros[i] = 0;
    }
    for (int i = 0; i < NUM_ESTRELAS; i++) {
        estrelas[i] = 0;
    }

    printf("   Insira %d numeros (1-50):\n", NUM_NUMEROS);
    for (int i = 0; i < NUM_NUMEROS; i++) {
        int valor;
        while (1) {
            printNumeroBoard();
            printf("   Numero %d: ", i + 1);
            if (scanf("%d", &valor) != 1) {
                printf("   Entrada invalida. Tente novamente.\n");
                limparBuffer();
                continue;
            }
            if (valor < 1 || valor > MAX_NUMEROS) {
                printf("   Numero invalido! Deve ser entre 1 e 50.\n");
                continue;
            }
            if (numeroRepetido(numeros, i, valor)) {
                printf("   Numero repetido! Escolha outro.\n");
                continue;
            }
            numeros[i] = valor;
            break;
        }
    }

    ordenar(numeros, NUM_NUMEROS);

    char resposta;
    while (1) {
        printf("\nNumeros escolhidos (ordenados): ");
        for (int i = 0; i < NUM_NUMEROS; i++) {
            printf("%02d ", numeros[i]);
        }
        printf("\nPretende trocar algum numero? (S/N): ");
        limparBuffer();
        if (scanf("%c", &resposta) != 1) {
            resposta = 'N';
        }
        resposta = (char)toupper((unsigned char)resposta);

        if (resposta == 'N') {
            break;
        }
        if (resposta == 'S') {
            int pos, novo_num;
            do {
                printf("Indique a posicao a alterar (1-%d): ", NUM_NUMEROS);
                if (scanf("%d", &pos) != 1) {
                    printf("Entrada invalida.\n");
                    limparBuffer();
                    pos = 0;
                    continue;
                }
            } while (pos < 1 || pos > NUM_NUMEROS);

            do {
                printf("Novo numero: ");
                if (scanf("%d", &novo_num) != 1) {
                    printf("Entrada invalida.\n");
                    limparBuffer();
                    novo_num = -1;
                    continue;
                }
            } while (novo_num < 1 || novo_num > MAX_NUMEROS || numeroRepetido(numeros, NUM_NUMEROS, novo_num));

            numeros[pos - 1] = novo_num;
            ordenar(numeros, NUM_NUMEROS);
        }
    }

    clearScreen();

    printf("\nInsira %d estrelas (1-12):\n", NUM_ESTRELAS);
    for (int i = 0; i < NUM_ESTRELAS; i++) {
        int valor;
        while (1) {
            printEstrelaBoard();
            printf("Estrela %d: ", i + 1);
            if (scanf("%d", &valor) != 1) {
                printf("Entrada invalida. Tente novamente.\n");
                limparBuffer();
                continue;
            }
            if (valor < 1 || valor > MAX_ESTRELAS) {
                printf("Estrela invalida! Deve ser entre 1 e 12.\n");
                continue;
            }
            if (numeroRepetido(estrelas, i, valor)) {
                printf("Estrela repetida! Escolha outra.\n");
                continue;
            }
            estrelas[i] = valor;
            break;
        }
    }

    ordenar(numeros, NUM_NUMEROS);
    ordenar(estrelas, NUM_ESTRELAS);
}

void corrigirAposta(int numeros[], int estrelas[]) {
    int opcao;
    printf("\n   === CORRIGIR APOSTA ===\n");
    printf("   1. Corrigir um numero\n");
    printf("   2. Corrigir uma estrela\n");
    printf("   Escolha a opcao: ");
    if (scanf("%d", &opcao) != 1) {
        limparBuffer();
        return;
    }

    if (opcao == 1) {
        int indice, novo_valor;
        printf("   Qual numero deseja corrigir? (1 a %d): ", NUM_NUMEROS);
        if (scanf("%d", &indice) != 1) {
            limparBuffer();
            return;
        }
        if (indice < 1 || indice > NUM_NUMEROS) {
            printf("   Indice invalido!\n");
            return;
        }
        indice--;
        while (1) {
            printf("   Digite o novo numero (1-50): ");
            if (scanf("%d", &novo_valor) != 1) {
                printf("   Entrada invalida.\n");
                limparBuffer();
                continue;
            }
            if (novo_valor < 1 || novo_valor > MAX_NUMEROS) {
                printf("   Numero invalido! Deve ser entre 1 e 50.\n");
                continue;
            }
            int repetido = 0;
            for (int i = 0; i < NUM_NUMEROS; i++) {
                if (i != indice && numeros[i] == novo_valor) {
                    repetido = 1;
                    break;
                }
            }
            if (repetido) {
                printf("   Numero repetido! Escolha outro.\n");
                continue;
            }
            numeros[indice] = novo_valor;
            ordenar(numeros, NUM_NUMEROS);
            break;
        }

    } else if (opcao == 2) {
        int indice, novo_valor;
        printf("   Qual estrela deseja corrigir? (1 a %d): ", NUM_ESTRELAS);
        if (scanf("%d", &indice) != 1) {
            limparBuffer();
            return;
        }
        if (indice < 1 || indice > NUM_ESTRELAS) {
            printf("   Indice invalido!\n");
            return;
        }
        indice--;
        while (1) {
            printf("   Digite a nova estrela (1-12): ");
            if (scanf("%d", &novo_valor) != 1) {
                printf("   Entrada invalida.\n");
                limparBuffer();
                continue;
            }
            if (novo_valor < 1 || novo_valor > MAX_ESTRELAS) {
                printf("   Estrela invalida! Deve ser entre 1 e 12.\n");
                continue;
            }
            int repetido = 0;
            for (int i = 0; i < NUM_ESTRELAS; i++) {
                if (i != indice && estrelas[i] == novo_valor) {
                    repetido = 1;
                    break;
                }
            }
            if (repetido) {
                printf("   Estrela repetida! Escolha outra.\n");
                continue;
            }
            estrelas[indice] = novo_valor;
            ordenar(estrelas, NUM_ESTRELAS);
            break;
        }

    } else {
        printf("   Opcao invalida!\n");
    }
}

void gerarSorteio(int numeros[], int estrelas[]) {
    for (int i = 0; i < NUM_NUMEROS; i++) {
        int num;
        do {
            num = rand() % MAX_NUMEROS + 1;
        } while (numeroRepetido(numeros, i, num));
        numeros[i] = num;
    }

    for (int i = 0; i < NUM_ESTRELAS; i++) {
        int est;
        do {
            est = rand() % MAX_ESTRELAS + 1;
        } while (numeroRepetido(estrelas, i, est));
        estrelas[i] = est;
    }

    ordenar(numeros, NUM_NUMEROS);
    ordenar(estrelas, NUM_ESTRELAS);
}

int contarAcertos(int aposta[], int sorteio[], int tamanho) {
    int acertos = 0;
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            if (aposta[i] == sorteio[j]) {
                acertos++;
                break;
            }
        }
    }
    return acertos;
}

void inicializarCategoriasPremio(CategoriaPremio categorias[]) {
    categorias[0] = (CategoriaPremio){1, 5, 2, "5 numeros + 2 estrelas", 0.50f};
    categorias[1] = (CategoriaPremio){2, 5, 1, "5 numeros + 1 estrela", 0.15f};
    categorias[2] = (CategoriaPremio){3, 5, 0, "5 numeros + 0 estrelas", 0.10f};
    categorias[3] = (CategoriaPremio){4, 4, 2, "4 numeros + 2 estrelas", 0.07f};
    categorias[4] = (CategoriaPremio){5, 4, 1, "4 numeros + 1 estrela", 0.05f};
    categorias[5] = (CategoriaPremio){6, 4, 0, "4 numeros + 0 estrelas", 0.03f};
    categorias[6] = (CategoriaPremio){7, 3, 2, "3 numeros + 2 estrelas", 0.02f};
    categorias[7] = (CategoriaPremio){8, 3, 1, "3 numeros + 1 estrela", 0.015f};
    categorias[8] = (CategoriaPremio){9, 3, 0, "3 numeros + 0 estrelas", 0.010f};
    categorias[9] = (CategoriaPremio){10, 2, 2, "2 numeros + 2 estrelas", 0.008f};
    categorias[10] = (CategoriaPremio){11, 2, 1, "2 numeros + 1 estrela", 0.006f};
    categorias[11] = (CategoriaPremio){12, 1, 2, "1 numero + 2 estrelas", 0.004f};
    categorias[12] = (CategoriaPremio){13, 0, 2, "0 numeros + 2 estrelas", 0.002f};
}

int verificarCategoriaPremio(CategoriaPremio categorias[], int acertos_numeros, int acertos_estrelas) {
    for (int i = 0; i < 13; i++) {
        if (acertos_numeros == categorias[i].acertos_numeros && acertos_estrelas == categorias[i].acertos_estrelas) {
            return categorias[i].id;
        }
    }
    return 0;
}

float calcularValorPremio(int id_premio, float premio_total, CategoriaPremio categorias[]) {
    if (id_premio == 0) {
        return 0.0f;
    }
    return premio_total * categorias[id_premio - 1].percentagem;
}

void bannerMenuPrincipal() {
    printf("   ==========================================================================================\n");
    printf("   ||  #####  ##   ##  #####   ####    ##   ##  ##  ##      ##   ##  ####    #####  #####  ||\n");
    printf("   ||  ##     ##   ##  ##  ##  ##  ##  ### ###  ##  ##      ##   ##  ##  ##  ##     ##     ||\n");
    printf("   ||  ####   ##   ##  #####   ##  ##  ## # ##  ##  ##      #######  ##  ##  ####   #####  ||\n");
    printf("   ||  ##     ##   ##  ##  ##  ##  ##  ##   ##  ##  ##      ##   ##  ##  ##  ##        ##  ||\n");
    printf("   ||  #####   #####   ##  ##   ####   ##   ##  ##  ######  ##   ##   ####   #####  #####  ||\n");
    printf("   ==========================================================================================\n");
    printf("    by Afonso Santos e Afonso Marques\n");
}

void clearScreen() {
    system("cls || clear");
    bannerMenuPrincipal();
}

void mostrarMenuPrincipal() {
    clearScreen();
    printf("    ===================================\n");
    printf("    1. Inserir a minha aposta\n");
    printf("    2. Admin\n");
    printf("    3. Sair\n");
    printf("    ===================================\n");
    printf("    Escolha: ");
}

void mostrarMenuAdmin() {
    clearScreen();
    printf("\n");
    printf("    ===================================\n");
    printf("    ||           MENU ADMIN          ||\n");
    printf("    ===================================\n");
    printf("    1. Ver ultimas apostas\n");
    printf("    2. Inserir valor do premio em jogo\n");
    printf("    ===================================\n");
    printf("    Escolha: ");
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

void salvarApostaCompletaCSV(ApostaCompleta *ac) {
    FILE *arquivo = fopen(NOME_ARQUIVO, "a");
    if (arquivo == NULL) {
        printf("   Erro ao abrir o arquivo para salvar a aposta.\n");
        return;
    }

    for (int i = 0; i < NUM_NUMEROS; i++) {
        fprintf(arquivo, "%02d", ac->aposta.numeros[i]);
        if (i < NUM_NUMEROS - 1) fprintf(arquivo, " ");
    }
    fprintf(arquivo, ";");

    for (int i = 0; i < NUM_ESTRELAS; i++) {
        fprintf(arquivo, "%02d", ac->aposta.estrelas[i]);
        if (i < NUM_ESTRELAS - 1) fprintf(arquivo, " ");
    }
    fprintf(arquivo, ";");

    for (int i = 0; i < NUM_NUMEROS; i++) {
        fprintf(arquivo, "%02d", ac->sorteio.numeros[i]);
        if (i < NUM_NUMEROS - 1) fprintf(arquivo, " ");
    }
    fprintf(arquivo, ";");

    for (int i = 0; i < NUM_ESTRELAS; i++) {
        fprintf(arquivo, "%02d", ac->sorteio.estrelas[i]);
        if (i < NUM_ESTRELAS - 1) fprintf(arquivo, " ");
    }
    fprintf(arquivo, ";");

    fprintf(arquivo, "%d;%d;%d;%s;%.2f;%.2f\n",
            ac->acertos_numeros,
            ac->acertos_estrelas,
            ac->id_premio,
            ac->descricao_premio,
            ac->valor_premio,
            ac->premio_total_jogo);

    fclose(arquivo);
}

void lerUltimasApostas() {
    FILE *arquivo = fopen(NOME_ARQUIVO, "r");
    if (arquivo == NULL) {
        printf("   Nenhuma aposta registrada ainda.\n");
        return;
    }

    char linhas[MAX_APOSTAS][500];
    int total_linhas = 0;
    char linha[500];

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        linha[strcspn(linha, "\n")] = 0;
        if (total_linhas < MAX_APOSTAS) {
            strcpy(linhas[total_linhas], linha);
            total_linhas++;
        } else {
            for (int i = 0; i < MAX_APOSTAS - 1; i++) {
                strcpy(linhas[i], linhas[i + 1]);
            }
            strcpy(linhas[MAX_APOSTAS - 1], linha);
        }
    }

    fclose(arquivo);

    if (total_linhas == 0) {
        printf("   Nenhuma aposta registrada ainda.\n");
        return;
    }

    printf("\n");
    for (int i = total_linhas - 1; i >= 0; i--) {
        ApostaCompleta ac;
        char *token;
        char *rest = linhas[i];

        token = strtok_r(rest, ";", &rest);
        sscanf(token, "%d %d %d %d %d",
               &ac.aposta.numeros[0], &ac.aposta.numeros[1], &ac.aposta.numeros[2],
               &ac.aposta.numeros[3], &ac.aposta.numeros[4]);

        token = strtok_r(rest, ";", &rest);
        sscanf(token, "%d %d", &ac.aposta.estrelas[0], &ac.aposta.estrelas[1]);

        token = strtok_r(rest, ";", &rest);
        sscanf(token, "%d %d %d %d %d",
               &ac.sorteio.numeros[0], &ac.sorteio.numeros[1], &ac.sorteio.numeros[2],
               &ac.sorteio.numeros[3], &ac.sorteio.numeros[4]);

        token = strtok_r(rest, ";", &rest);
        sscanf(token, "%d %d", &ac.sorteio.estrelas[0], &ac.sorteio.estrelas[1]);

        token = strtok_r(rest, ";", &rest);
        ac.acertos_numeros = atoi(token);

        token = strtok_r(rest, ";", &rest);
        ac.acertos_estrelas = atoi(token);

        token = strtok_r(rest, ";", &rest);
        ac.id_premio = atoi(token);

        token = strtok_r(rest, ";", &rest);
        strcpy(ac.descricao_premio, token);

        token = strtok_r(rest, ";", &rest);
        ac.valor_premio = (float)atof(token);

        token = strtok_r(rest, ";", &rest);
        ac.premio_total_jogo = (float)atof(token);

        mostrarApostaCompleta(&ac, total_linhas - i);

        if (i > 0) {
            printf("\n   ==========================================================\n");
        }
    }
}

void mostrarApostaCompleta(ApostaCompleta *ac, int numero_aposta) {
    printf("   APOSTA %d:\n", numero_aposta);
    printf("   ==========================================================\n");

    printf("   Sua Aposta:\n");
    mostrarAposta(ac->aposta.numeros, ac->aposta.estrelas, "");

    printf("\n   Numeros Sorteados:\n");
    mostrarAposta(ac->sorteio.numeros, ac->sorteio.estrelas, "");

    printf("\n   Resultado:\n");
    printf("  Acertos nos numeros: %d/%d\n", ac->acertos_numeros, NUM_NUMEROS);
    printf("  Acertos nas estrelas: %d/%d\n", ac->acertos_estrelas, NUM_ESTRELAS);

    printf("\n   Premio:\n");
    if (ac->id_premio == 0) {
        printf("  Nenhum premio ganho\n");
    } else {
        printf("  Categoria: %d Premio\n", ac->id_premio);
        printf("  Descricao: %s\n", ac->descricao_premio);
        printf("  Valor do premio: %.2f\n", ac->valor_premio);
        printf("  Premio total do jogo: %.2f\n", ac->premio_total_jogo);
    }
}

int main() {
    ApostaCompleta ac;
    int opcao;
    char correcao;

    srand((unsigned int)time(NULL));

    do {
        mostrarMenuPrincipal();

        if (scanf("%d", &opcao) != 1) {
            printf("   Opcao invalida!\n");
            limparBuffer();
            continue;
        }

        clearScreen();

        switch (opcao) {
        case 1:
            printf("\n   === INSERIR APOSTA ===\n");

            lerApostaUsuario(ac.aposta.numeros, ac.aposta.estrelas);

            clearScreen();

            do {
                printf("\n   === SUA APOSTA ATUAL ===\n");
                mostrarApostaComIndices(ac.aposta.numeros, ac.aposta.estrelas);

                printf("\n   Deseja corrigir algum numero ou estrela? (S/N): ");
                limparBuffer();
                if (scanf("%c", &correcao) != 1) {
                    correcao = 'N';
                }

                if (correcao == 'S' || correcao == 's') {
                    corrigirAposta(ac.aposta.numeros, ac.aposta.estrelas);
                }
            } while (correcao == 'S' || correcao == 's');

            clearScreen();

            printf("\n   === APOSTA FINAL CONFIRMADA ===\n");
            mostrarAposta(ac.aposta.numeros, ac.aposta.estrelas, "");

            printf("\n   === SORTEIO ===\n");
            gerarSorteio(ac.sorteio.numeros, ac.sorteio.estrelas);
            mostrarAposta(ac.sorteio.numeros, ac.sorteio.estrelas, "");

            ac.acertos_numeros = contarAcertos(ac.aposta.numeros, ac.sorteio.numeros, NUM_NUMEROS);
            ac.acertos_estrelas = contarAcertos(ac.aposta.estrelas, ac.sorteio.estrelas, NUM_ESTRELAS);

            CategoriaPremio categorias[13];
            inicializarCategoriasPremio(categorias);
            ac.id_premio = verificarCategoriaPremio(categorias, ac.acertos_numeros, ac.acertos_estrelas);
            ac.valor_premio = calcularValorPremio(ac.id_premio, ac.premio_total_jogo, categorias);

            if (ac.id_premio > 0) {
                strcpy(ac.descricao_premio, categorias[ac.id_premio - 1].descricao);
            } else {
                strcpy(ac.descricao_premio, "Sem premio");
            }

            printf("\n   === PREMIO ===\n");
            if (ac.id_premio == 0) {
                printf("   Infelizmente nao ganhou nenhum premio. Tente novamente!\n");
            } else {
                printf("   Categoria: %d Premio\n", ac.id_premio);
                printf("   Descricao: %s\n", ac.descricao_premio);
                printf("   Valor do premio: %.2f\n", ac.valor_premio);
            }

            salvarApostaCompletaCSV(&ac);
            printf("\n   Aposta salva!\n");

            printf("\n   Pressione Enter para continuar...");
            limparBuffer();
            getchar();
            break;

        case 2:
            if (fazerLogin()) {
                mostrarMenuAdmin();
                int opcao_admin;
                if (scanf("%d", &opcao_admin) != 1) {
                    printf("   Opcao invalida!\n");
                    limparBuffer();
                    break;
                }
                clearScreen();
                switch (opcao_admin) {
                    case 1:
                        lerUltimasApostas();
                        break;
                    case 2:
                        ac.premio_total_jogo = lerPremioTotal();
                        printf("   Valor do premio em jogo atualizado para: %.2f\n", ac.premio_total_jogo);
                        break;
                }
            }

            printf("\n   Pressione Enter para continuar...");
            limparBuffer();
            getchar();
            break;

        case 3:
            printf("\n   Adeus! Obrigado por usar o programa.\n");
            break;

        default:
            printf("   Opcao invalida! Tente novamente.\n");
            break;
        }

    } while (opcao != 3);

    return 0;
}