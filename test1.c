#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_NUMEROS 5
#define NUM_ESTRELAS 2
#define MAX_NUMEROS 50
#define MAX_ESTRELAS 12

void ordenar(int array[], int tamanho) {
  int i, j, temp;
  for (i = 0; i < tamanho - 1; i++) {
    for (j = 0; j < tamanho - i - 1; j++) {
      if (array[j] > array[j + 1]) {
        temp = array[j];
        array[j] = array[j + 1];
        array[j + 1] = temp;
      }
    }
  }
}

int numeroRepetido(int array[], int tamanho, int numero) {
  int i;
  for (i = 0; i < tamanho; i++) {
    if (array[i] == numero) {
      return 1;
    }
  }
  return 0;
}

void gerarAposta(int numeros[], int estrelas[]) {
  int i, num, est;

  // Gerar números principais
  for (i = 0; i < NUM_NUMEROS; i++) {
    do {
      num = rand() % MAX_NUMEROS + 1;
    } while (numeroRepetido(numeros, i, num));
    numeros[i] = num;
  }

  // Gerar estrelas
  for (i = 0; i < NUM_ESTRELAS; i++) {
    do {
      est = rand() % MAX_ESTRELAS + 1;
    } while (numeroRepetido(estrelas, i, est));
    estrelas[i] = est;
  }

  ordenar(numeros, NUM_NUMEROS);
  ordenar(estrelas, NUM_ESTRELAS);
}

void mostrarAposta(int numeros[], int estrelas[], char *titulo) {
  int i;
  printf("%s\n", titulo);
  printf("Numeros: ");
  for (i = 0; i < NUM_NUMEROS; i++) {
    printf("%02d ", numeros[i]);
  }
  printf("\nEstrelas: ");
  for (i = 0; i < NUM_ESTRELAS; i++) {
    printf("%02d ", estrelas[i]);
  }
  printf("\n\n");
}

int contarAcertos(int aposta[], int sorteio[], int tamanho) {
  int i, j, acertos = 0;
  for (i = 0; i < tamanho; i++) {
    for (j = 0; j < tamanho; j++) {
      if (aposta[i] == sorteio[j]) {
        acertos++;
        break;
      }
    }
  }
  return acertos;
}

void lerApostaUsuario(int numeros[], int estrelas[]) {
  int i;
  printf("=== INSIRA SUA APOSTA ===\n");

  // Ler números
  printf("Insira %d numeros (1-50):\n", NUM_NUMEROS);
  for (i = 0; i < NUM_NUMEROS; i++) {
    do {
      printf("Numero %d: ", i + 1);
      scanf("%d", &numeros[i]);
      if (numeros[i] < 1 || numeros[i] > MAX_NUMEROS) {
        printf("Numero invalido! Deve ser entre 1 e 50.\n");
      } else if (numeroRepetido(numeros, i, numeros[i])) {
        printf("Numero repetido! Escolha outro.\n");
        numeros[i] = 0; // Reset para repetir
      }
    } while (numeros[i] < 1 || numeros[i] > MAX_NUMEROS);
  }

  // Ler estrelas
  printf("\nInsira %d estrelas (1-12):\n", NUM_ESTRELAS);
  for (i = 0; i < NUM_ESTRELAS; i++) {
    do {
      printf("Estrela %d: ", i + 1);
      scanf("%d", &estrelas[i]);
      if (estrelas[i] < 1 || estrelas[i] > MAX_ESTRELAS) {
        printf("Estrela invalida! Deve ser entre 1 e 12.\n");
      } else if (numeroRepetido(estrelas, i, estrelas[i])) {
        printf("Estrela repetida! Escolha outra.\n");
        estrelas[i] = 0; // Reset para repetir
      }
    } while (estrelas[i] < 1 || estrelas[i] > MAX_ESTRELAS);
  }

  ordenar(numeros, NUM_NUMEROS);
  ordenar(estrelas, NUM_ESTRELAS);
}

void limparBuffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

int main() {
  int numerosSorteio[NUM_NUMEROS];
  int estrelasSorteio[NUM_ESTRELAS];
  int numerosAposta[NUM_NUMEROS];
  int estrelasAposta[NUM_ESTRELAS];
  int opcao, acertosNumeros, acertosEstrelas;

  srand(time(NULL));

  do {
    printf("=======================\n");
    printf("    EURO MILHOES\n");
    printf("=======================\n");
    printf("1. Gerar aposta aleatoria\n");
    printf("2. Inserir minha aposta\n");
    printf("3. Sair\n");
    printf("Escolha: ");

    if (scanf("%d", &opcao) != 1) {
      printf("Opção invalida!\n");
      limparBuffer();
      continue;
    }

    switch (opcao) {
    case 1:
      gerarAposta(numerosAposta, estrelasAposta);
      mostrarAposta(numerosAposta, estrelasAposta, "=== SUA APOSTA ===");
      break;

    case 2:
      lerApostaUsuario(numerosAposta, estrelasAposta);
      mostrarAposta(numerosAposta, estrelasAposta, "=== SUA APOSTA ===");
      break;

    case 3:
      printf("Adeus!\n");
      return 0;

    default:
      printf("Opção invalida!\n");
      continue;
    }

    // Realizar o sorteio
    printf("\n=== SORTEIO ===\n");
    gerarAposta(numerosSorteio, estrelasSorteio);
    mostrarAposta(numerosSorteio, estrelasSorteio, "Números sorteados:");

    // Verificar resultados
    acertosNumeros = contarAcertos(numerosAposta, numerosSorteio, NUM_NUMEROS);
    acertosEstrelas =
        contarAcertos(estrelasAposta, estrelasSorteio, NUM_ESTRELAS);

    printf("=== RESULTADOS ===\n");
    printf("Acertos nos numeros: %d/%d\n", acertosNumeros, NUM_NUMEROS);
    printf("Acertos nas estrelas: %d/%d\n", acertosEstrelas, NUM_ESTRELAS);

    // Mostrar prêmio aproximado
    printf("\n=== PREMIO ===\n");
    if (acertosNumeros == 5 && acertosEstrelas == 2) {
      printf("1º PREMIO! Jackpot!\n");
    } else if (acertosNumeros == 5 && acertosEstrelas == 1) {
      printf("2º PREMIO!\n");
    } else if (acertosNumeros == 5 && acertosEstrelas == 0) {
      printf("3º PREMIO!\n");
    } else if (acertosNumeros == 4 && acertosEstrelas == 2) {
      printf("4º PREMIO!\n");
    } else if (acertosNumeros == 4 && acertosEstrelas == 1) {
      printf("5º PREMIO!\n");
    } else if (acertosNumeros == 4 && acertosEstrelas == 0) {
      printf("6º PREMIO!\n");
    } else if (acertosNumeros == 3 && acertosEstrelas == 2) {
      printf("7º PREMIO!\n");
    } else if (acertosNumeros == 2 && acertosEstrelas == 2) {
      printf("8º PREMIO!\n");
    } else if (acertosNumeros == 3 && acertosEstrelas == 1) {
      printf("9º PREMIO!\n");
    } else if (acertosNumeros == 3 && acertosEstrelas == 0) {
      printf("10º PREMIO!\n");
    } else if (acertosNumeros == 1 && acertosEstrelas == 2) {
      printf("11º PREMIO!\n");
    } else if (acertosNumeros == 2 && acertosEstrelas == 1) {
      printf("12º PREMIO!\n");
    } else if (acertosNumeros == 2 && acertosEstrelas == 0) {
      printf("13º PREMIO!\n");
    } else {
      printf("Nao ganhou nenhum premio.\n");
    }

    printf("\nPressione Enter para continuar...");
    limparBuffer();
    getchar(); // Espera pelo Enter

  } while (opcao != 3);

  return 0;
}
