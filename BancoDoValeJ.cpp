#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXIMOACC 100

typedef struct {
    int numACC;
    char nome[100];
    char cpf[12];
    float saldo;
    int status;  // 1: Ativa, 0: Inativa
} ContaBancaria;


ContaBancaria contas[MAXIMOACC];
int totalACC = 0;


int cpf_em_uso(const char* cpf) {
    for (int i = 0; i < totalACC; i++) {
        if (strcmp(contas[i].cpf, cpf) == 0) {
            return 1;  
        }
    }
    return 0;  
}


int validar_cpf(const char* cpf) {
    int i = 0;
    int count = 0;

    while (cpf[i] != '\0') {
        if (isdigit(cpf[i])) {
            count++;
        }
        i++;
    }

    return count == 11;
}

bool validar_nome(const char* nome){
    int i = 0;
    int count = 0;

    while (nome[i] != '\0'){
        if (iscntrl(nome[i]) || isdigit(nome[i]) || ispunct(nome[i])){
            printf("O nome deve conter somente letras.");
            return false;
        }else{
            count++;
        }
        i++;
    }
    return true;
}

void cadastrar_conta() {
    if (totalACC >= MAXIMOACC) {
        printf("Limite de contas alcançado.\n");
        return;
    }

    ContaBancaria newACC;
    printf("Digite o numero da conta: ");
    scanf("%d", &newACC.numACC);

    for (int i = 0; i < totalACC; i++) {
        if (contas[i].numACC == newACC.numACC) {
            printf("Numero de conta ja existe.\n");
            return;
        }
    }

    do {
    printf("Digite o nome do titular: ");
    getchar();  
    fgets(newACC.nome, 100, stdin);
    newACC.nome[strcspn(newACC.nome, "\n")] = '\0';  
    } while ( !validar_nome(newACC.nome));

    do {
        printf("Digite o CPF do titular (apenas numeros): ");
        fgets(newACC.cpf, 12, stdin);
        newACC.cpf[strcspn(newACC.cpf, "\n")] = '\0';  

        if (!validar_cpf(newACC.cpf)) {
            printf("CPF invalido. O CPF deve conter exatamente 11 digitos numericos.\n");
        } else if (cpf_em_uso(newACC.cpf)) {
            printf("CPF ja cadastrado. Tente novamente.\n");
        }

    } while (!validar_cpf(newACC.cpf) || cpf_em_uso(newACC.cpf));

    do {
        printf("Digite o saldo inicial (>= 0): ");
        scanf("%f", &newACC.saldo);
    } while (newACC.saldo < 0);

    newACC.status = 1;  
    contas[totalACC++] = newACC;
    printf("Conta criada com sucesso!\n");
}


void alterar_conta() {
    int numACC;
    printf("Digite o numero da conta para alterar: ");
    scanf("%d", &numACC);

    int i;
    for (i = 0; i < totalACC; i++) {
        if (contas[i].numACC == numACC) {
            printf("Digite o novo nome do titular: ");
            getchar();  
            fgets(contas[i].nome, 100, stdin);
            contas[i].nome[strcspn(contas[i].nome, "\n")] = '\0';  

            do {
                printf("Digite o novo CPF do titular (apenas números): ");
                fgets(contas[i].cpf, 12, stdin);
                contas[i].cpf[strcspn(contas[i].cpf, "\n")] = '\0';  

                if (!validar_cpf(contas[i].cpf)) {
                    printf("CPF invalido. O CPF deve conter exatamente 11 dígitos numéricos.\n");
                } else if (cpf_em_uso(contas[i].cpf)) {
                    printf("CPF ja cadastrado. Tente novamente.\n");
                }

            } while (!validar_cpf(contas[i].cpf) || cpf_em_uso(contas[i].cpf));

            printf("Informações alteradas com sucesso!\n");
            return;
        }
    }
    printf("Conta nao encontrada.\n");
}

void consultar_conta() {
    int numACC;
    printf("Digite o numero da conta para consultar: ");
    scanf("%d", &numACC);

    int i;
    for (i = 0; i < totalACC; i++) {
        if (contas[i].numACC == numACC) {
            printf("Numero da conta: %d\n", contas[i].numACC);
            printf("Nome do titular: %s\n", contas[i].nome);
            printf("CPF: %s\n", contas[i].cpf);
            printf("Saldo: %.2f\n", contas[i].saldo);
            printf("Status: %s\n", contas[i].status == 1 ? "Ativa" : "Inativa");
            return;
        }
    }
    printf("Conta nao encontrada.\n");
}


void sacar() {
    int numACC;
    float valor;

    printf("Digite o numero da conta para sacar: ");
    scanf("%d", &numACC);
    printf("Digite o valor do saque: ");
    scanf("%f", &valor);

    int i;
    for (i = 0; i < totalACC; i++) {
        if (contas[i].numACC == numACC) {
            if (contas[i].status == 0) {
                printf("Conta inativa. Nao pode realizar o saque.\n");
                return;
            }
            if (contas[i].saldo < valor) {
                printf("Saldo insuficiente.\n");
                return;
            }
            contas[i].saldo -= valor;
            printf("Saque realizado com sucesso!\n");
            return;
        }
    }
    printf("Conta nao encontrada.\n");
}


void depositar() {
    int numACC;
    float valor;

    printf("Digite o numero da conta para depositar: ");
    scanf("%d", &numACC);
    printf("Digite o valor do deposito: ");
    scanf("%f", &valor);

    int i;
    for (i = 0; i < totalACC; i++) {
        if (contas[i].numACC == numACC) {
            if (contas[i].status == 0) {
                printf("Conta inativa. Nao pode realizar o deposito.\n");
                return;
            }
            contas[i].saldo += valor;
            printf("Deposito realizado com sucesso!\n");
            return;
        }
    }
    printf("Conta nao encontrada.\n");
}


void transferir() {
    int numACC_origem, numACC_destino;
    float valor;

    printf("Digite o numero da conta de origem: ");
    scanf("%d", &numACC_origem);
    printf("Digite o numero da conta de destino: ");
    scanf("%d", &numACC_destino);
    printf("Digite o valor da transferencia: ");
    scanf("%f", &valor);

    int i, j;
    for (i = 0; i < totalACC; i++) {
        if (contas[i].numACC == numACC_origem) {
            if (contas[i].status == 0) {
                printf("Conta de origem inativa. Nao pode realizar a transferencia.\n");
                return;
            }
            if (contas[i].saldo < valor) {
                printf("Saldo insuficiente na conta de origem.\n");
                return;
            }

            for (j = 0; j < totalACC; j++) {
                if (contas[j].numACC == numACC_destino) {
                    if (contas[j].status == 0) {
                        printf("Conta de destino inativa. Nao pode realizar a transferencia.\n");
                        return;
                    }
                    contas[i].saldo -= valor;
                    contas[j].saldo += valor;
                    printf("Transferencia realizada com sucesso!\n");
                    return;
                }
            }
            printf("Conta de destino nao encontrada.\n");
            return;
        }
    }
    printf("Conta de origem nao encontrada.\n");
}


void encerrar_conta() {
    int numACC;
    printf("Digite o numero da conta para encerrar: ");
    scanf("%d", &numACC);

    int i;
    for (i = 0; i < totalACC; i++) {
        if (contas[i].numACC == numACC) {
            if (contas[i].saldo != 0) {
                printf("Conta nao pode ser encerrada com saldo diferente de zero.\n");
                return;
            }
            contas[i].status = 0;  
            printf("Conta encerrada com sucesso!\n");
            return;
        }
    }
    printf("Conta nao encontrada.\n");
}

void desativar_conta(){
    int numACC;
    printf("Digite o numero da conta que deseja desativar:");
    scanf("%d",&numACC);
    for (int i=0; i < totalACC; i++){
        if (contas[i].numACC == numACC){
            contas[i].status = 0;
            printf("conta desativada com sucesso!");
            return;
        }
    }
}

void ativar_conta(){
    int numACC;
    printf("Digite o numero da conta que deseja ativar:");
    scanf("%d",&numACC);
    for (int i=0; i < totalACC; i++){
        if (contas[i].numACC == numACC){
            contas[i].status = 1;
            printf("conta ativada com sucesso!");
            return;
        }
    }
}



int main() {
    int opcao;
    do {
        printf("\nMenu:\n");
        printf("1. Cadastrar conta\n");
        printf("2. Alterar conta\n");
        printf("3. Consultar conta\n");
        printf("4. Sacar\n");
        printf("5. Depositar\n");
        printf("6. Transferir\n");
        printf("7. Encerrar conta\n");
        printf("8. Desativar conta\n");
        printf("9. Ativar conta\n");
        printf("10. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: cadastrar_conta(); break;
            case 2: alterar_conta(); break;
            case 3: consultar_conta(); break;
            case 4: sacar(); break;
            case 5: depositar(); break;
            case 6: transferir(); break;
            case 7: encerrar_conta(); break;
            case 8: desativar_conta(); break;
            case 9: ativar_conta(); break;
            case 10: printf("Saindo...\n"); break;
            default: printf("Opcao invalida!\n"); break;
        }
    } while (opcao != 10);

    return 0;
}
