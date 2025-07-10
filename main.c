#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PESSOAS 100
#define TAM_STR_NOME 50
#define TAM_STR_DATA 11 // Formato YYYY-MM-DD

typedef struct {
    char cpf[12]; // CPF como string para evitar problemas com zeros à esquerda
    char nome[TAM_STR_NOME];
    char data_nascimento[TAM_STR_DATA]; // Formato YYYY-MM-DD
} Pessoa;

// Arrays globais
Pessoa pessoas[MAX_PESSOAS];
int contador_pessoas = 0;

// Função para adicionar uma pessoa
void incluir_pessoa(const char* cpf, const char* nome, const char* data_nascimento) {
    if (contador_pessoas >= MAX_PESSOAS) {
        printf("Número máximo de pessoas atingido.\n");
        return;
    }
    strncpy(pessoas[contador_pessoas].cpf, cpf, sizeof(pessoas[contador_pessoas].cpf) - 1);
    strncpy(pessoas[contador_pessoas].nome, nome, TAM_STR_NOME - 1);
    strncpy(pessoas[contador_pessoas].data_nascimento, data_nascimento, TAM_STR_DATA - 1);
    contador_pessoas++;
    printf("Pessoa adicionada: CPF: %s, Nome: %s, Data de Nascimento: %s\n", cpf, nome, data_nascimento);
}

// Função para remover uma pessoa pelo CPF
void remover_pessoa(const char* cpf) {
    for (int i = 0; i < contador_pessoas; i++) {
        if (strcmp(pessoas[i].cpf, cpf) == 0) {
            for (int j = i; j < contador_pessoas - 1; j++) {
                pessoas[j] = pessoas[j + 1];
            }
            contador_pessoas--;
            printf("Pessoa com CPF %s removida.\n", cpf);
            return;
        }
    }
    printf("Pessoa com CPF %s não encontrada.\n", cpf);
}

// Função para listar todas as pessoas
void listar_pessoas() {
    if (contador_pessoas == 0) {
        printf("Nenhuma pessoa cadastrada.\n");
        return;
    }
    printf("Lista de Pessoas:\n");
    printf("CPF         | Nome                          | Data de Nascimento\n");
    printf("---------------------------------------------------------------\n");
    for (int i = 0; i < contador_pessoas; i++) {
        printf("%-12s | %-30s | %s\n", pessoas[i].cpf, pessoas[i].nome, pessoas[i].data_nascimento);
    }
}

// Função para salvar pessoas em um arquivo CSV
void salvar_pessoas_csv(const char *nome_arquivo) {
    FILE *f = fopen(nome_arquivo, "w");
    if (!f) {
        perror("Erro ao abrir arquivo para salvar pessoas CSV");
        return;
    }
    fprintf(f, "cpf,nome,data_nascimento\n");
    for (int i = 0; i < contador_pessoas; i++) {
        fprintf(f, "%s,%s,%s\n", pessoas[i].cpf, pessoas[i].nome, pessoas[i].data_nascimento);
    }
    fclose(f);
    printf("Pessoas salvas em arquivo CSV '%s'\n", nome_arquivo);
}

// Função para carregar pessoas de um arquivo CSV
void carregar_pessoas_csv(const char *nome_arquivo) {
    FILE *f = fopen(nome_arquivo, "r");
    if (!f) {
        perror("Erro ao abrir arquivo para carregar pessoas CSV");
        return;
    }
    contador_pessoas = 0;
    char linha[200];
    fgets(linha, sizeof(linha), f); // pular cabeçalho
    while (fgets(linha, sizeof(linha), f) && contador_pessoas < MAX_PESSOAS) {
        Pessoa p;
        char *token = strtok(linha, ",");
        if (!token) continue;
        strncpy(p.cpf, token, sizeof(p.cpf) - 1);
        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(p.nome, token, TAM_STR_NOME - 1);
        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(p.data_nascimento, token, TAM_STR_DATA - 1);
        pessoas[contador_pessoas++] = p;
    }
    fclose(f);
    printf("Pessoas carregadas do arquivo CSV '%s'\n", nome_arquivo);
}

// Função para ordenar pessoas por nome
int cmp_nome(const void *a, const void *b) {
    return strcmp(((Pessoa *)a)->nome, ((Pessoa *)b)->nome);
}

// Função para ordenar pessoas por CPF
int cmp_cpf(const void *a, const void *b) {
    return strcmp(((Pessoa *)a)->cpf, ((Pessoa *)b)->cpf);
}

// Função para calcular a idade a partir da data de nascimento
int calcular_idade(const char *data_nascimento) {
    struct tm tm_nasc = {0};
    sscanf(data_nascimento, "%4d-%2d-%2d", &tm_nasc.tm_year, &tm_nasc.tm_mon, &tm_nasc.tm_mday);
    tm_nasc.tm_year -= 1900; // Ajustar para o ano desde 1900
    tm_nasc.tm_mon -= 1; // Ajustar para o mês de 0 a 11
    time_t agora = time(NULL);
    struct tm *tm_atual = localtime(&agora);
    int idade = tm_atual->tm_year - tm_nasc.tm_year;
    if (tm_atual->tm_mon < tm_nasc.tm_mon || (tm_atual->tm_mon == tm_nasc.tm_mon && tm_atual->tm_mday < tm_nasc.tm_mday)) {
        idade--;
    }
    return idade;
}

// Função para ordenar pessoas por idade
int cmp_idade(const void *a, const void *b) {
    int idade_a = calcular_idade(((Pessoa *)a)->data_nascimento);
    int idade_b = calcular_idade(((Pessoa *)b)->data_nascimento);
    return idade_a - idade_b;
}

// Função para buscar pessoa por nome
void buscar_por_nome(const char *nome) {
    for (int i = 0; i < contador_pessoas; i++) {
        if (strstr(pessoas[i].nome, nome) != NULL) {
            printf("Encontrada: CPF: %s, Nome: %s, Data de Nascimento: %s\n", pessoas[i].cpf, pessoas[i].nome, pessoas[i].data_nascimento);
        }
    }
}

// Função para buscar pessoa por CPF
void buscar_por_cpf(const char *cpf) {
    for (int i = 0; i < contador_pessoas; i++) {
        if (strcmp(pessoas[i].cpf, cpf) == 0) {
            printf("Encontrada: CPF: %s, Nome: %s, Data de Nascimento: %s\n", pessoas[i].cpf, pessoas[i].nome, pessoas[i].data_nascimento);
            return;
        }
    }
    printf("Pessoa com CPF %s não encontrada.\n", cpf);
}

// Função para buscar pessoa por idade
void buscar_por_idade(int idade) {
    for (int i = 0; i < contador_pessoas; i++) {
        if (calcular_idade(pessoas[i].data_nascimento) == idade) {
            printf("Encontrada: CPF: %s, Nome: %s, Data de Nascimento: %s\n", pessoas[i].cpf, pessoas[i].nome, pessoas[i].data_nascimento);
        }
    }
}

int main() {
    int opcao = -1;

    while (opcao != 0) {
        printf("\n=== Sistema de Controle de Pessoas ===\n");
        printf("1. Incluir pessoa\n");
        printf("2. Remover pessoa\n");
        printf("3. Listar pessoas\n");
        printf("4. Ordenar pessoas\n");
        printf("5. Buscar pessoa\n");
        printf("6. Salvar dados (CSV)\n");
        printf("7. Carregar dados (CSV)\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida.\n");
            while(getchar() != '\n'); // limpar buffer
            continue;
        }
        while(getchar() != '\n'); // limpar buffer

        if (opcao == 1) {
            char cpf[12], nome[TAM_STR_NOME], data_nascimento[TAM_STR_DATA];
            printf("Digite o CPF: ");
            fgets(cpf, sizeof(cpf), stdin);
            cpf[strcspn(cpf, "\n")] = 0; // remover \n

            printf("Digite o nome: ");
            fgets(nome, sizeof(nome), stdin);
            nome[strcspn(nome, "\n")] = 0; // remover \n

            printf("Digite a data de nascimento (YYYY-MM-DD): ");
            fgets(data_nascimento, sizeof(data_nascimento), stdin);
            data_nascimento[strcspn(data_nascimento, "\n")] = 0; // remover \n

            incluir_pessoa(cpf, nome, data_nascimento);

        } else if (opcao == 2) {
            char cpf[12];
            printf("Digite o CPF da pessoa a remover: ");
            fgets(cpf, sizeof(cpf), stdin);
            cpf[strcspn(cpf, "\n")] = 0; // remover \n
            remover_pessoa(cpf);

        } else if (opcao == 3) {
            listar_pessoas();

        } else if (opcao == 4) {
    		int tipo_ordem;
   			 printf("Escolha o critério de ordenação:\n");
    		 printf("1. Nome\n");
    		 printf("2. CPF\n");
    		 printf("3. Idade\n");
    		 printf("Escolha uma opção: ");
    	  if (scanf("%d", &tipo_ordem) != 1 || tipo_ordem < 1 || tipo_ordem > 3) {
        	printf("Entrada inválida.\n");
        while(getchar() != '\n'); // limpar buffer
        continue;
    }
    while(getchar() != '\n'); // limpar buffer

    if (contador_pessoas == 0) {
        printf("Nenhuma pessoa cadastrada para ordenar.\n");
        continue;
    }

    switch(tipo_ordem) {
        case 1: 
            qsort(pessoas, contador_pessoas, sizeof(Pessoa), cmp_nome);
            printf("Pessoas ordenadas por nome:\n");
            break;
        case 2:
            qsort(pessoas, contador_pessoas, sizeof(Pessoa), cmp_cpf);
            printf("Pessoas ordenadas por CPF:\n");
            break;
        case 3:
            qsort(pessoas, contador_pessoas, sizeof(Pessoa), cmp_idade);
            printf("Pessoas ordenadas por idade:\n");
            break;
    }

    printf("---------------------------------------------------------------\n");
    for (int i = 0; i < contador_pessoas; i++) {
        if (tipo_ordem == 3) { // Mostrar idade quando ordenado por idade
            int idade = calcular_idade(pessoas[i].data_nascimento);
            printf("%-12s | %-30s | %s | Idade: %d\n", 
                   pessoas[i].cpf, pessoas[i].nome, 
                   pessoas[i].data_nascimento, idade);
        } else {
            printf("%-12s | %-30s | %s\n", 
                   pessoas[i].cpf, pessoas[i].nome, 
                   pessoas[i].data_nascimento);
        }
    }


        } else if (opcao == 5) {
            int tipo_busca;
            printf("Escolha o critério de busca:\n");
            printf("1. Nome\n");
            printf("2. CPF\n");
            printf("3. Idade\n");
            printf("Escolha uma opção: ");
            if (scanf("%d", &tipo_busca) != 1) {
                printf("Entrada inválida.\n");
                while(getchar() != '\n'); // limpar buffer
                continue;
            }
            while(getchar() != '\n'); // limpar buffer

            if (tipo_busca == 1) {
                char nome[TAM_STR_NOME];
                printf("Digite o nome a buscar: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = 0; // remover \n
                buscar_por_nome(nome);
            } else if (tipo_busca == 2) {
                char cpf[12];
                printf("Digite o CPF a buscar: ");
                fgets(cpf, sizeof(cpf), stdin);
                cpf[strcspn(cpf, "\n")] = 0; // remover \n
                buscar_por_cpf(cpf);
            } else if (tipo_busca == 3) {
                int idade;
                printf("Digite a idade a buscar: ");
                if (scanf("%d", &idade) != 1) {
                    printf("Entrada inválida.\n");
                    while(getchar() != '\n'); // limpar buffer
                    continue;
                }
                buscar_por_idade(idade);
            } else {
                printf("Opção inválida.\n");
            }

        } else if (opcao == 6) {
            salvar_pessoas_csv("pessoas.csv");

        } else if (opcao == 7) {
            carregar_pessoas_csv("pessoas.csv");

        } else if (opcao == 0) {
            printf("Saindo do sistema.\n");
        } else {
            printf("Opção inválida.\n");
        }
    }

    return 0;
}
