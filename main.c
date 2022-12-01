#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define MANHA "MANHA "
#define NOITE "NOITE "
#define ATIVO "ATIVO "
#define VAZIO "      "
#define INATIVO "INATIVO"

#define PHP "  PHP "
#define PHP_MANHA 210.00
#define PHP_NOITE 260.00

#define JAVA " JAVA "
#define JAVA_MANHA 320.00
#define JAVA_NOITE 390.00

#define PHYTON "PHYTON"
#define PHYTON_MANHA 390.00
#define PHYTON_NOITE 310.00

struct TAluno {
    int   matricula;
    char  nome[30];
    char  sexo;
    char  data_nascimento[10];
    char  turno[2];
    char  curso[2];
    float mensalidade;
    char  status;
};

struct TAluno Aluno, AlunoAux, AlunoNulo;


struct TData{
    int dia;
    int mes;
    int ano;
};

struct TData DataInicio, DataAtual;

int dias_mes[2][13] = {{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
                       {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};

int bissexto (int ano) {
return (ano % 4 == 0) && ((ano % 100 != 0) || (ano % 400 == 0));
}

void PegarDataAtual(){
    time_t mytime;
    mytime = time(NULL);
    struct tm tm = *localtime(&mytime);
    DataAtual.dia = tm.tm_mday;
    DataAtual.mes = tm.tm_mon  + 1;
    DataAtual.ano = tm.tm_year + 1900;
}

int a;

FILE *PAluno;

void AbrirArquivo(){
    PAluno = fopen("aluno.txt", "r+b");
    if (PAluno == NULL) PAluno = fopen("aluno.txt", "w+b");
}

void GravarArquivo(){
    fseek(PAluno,0,SEEK_END);
    fwrite(&Aluno,sizeof(Aluno),1,PAluno);
}

void RescreverRegistro(int pos){
    fseek(PAluno,pos*sizeof(Aluno),SEEK_SET);
    fwrite(&Aluno,sizeof(Aluno),1,PAluno);
}

void FecharArquivo(){
    fclose(PAluno);
}

void Cabecalho(){
    system("cls");
    printf("***************************\n");
    printf("*         TI CURSOS       *\n");
    printf("*    ESCOLHA UMA OPCAO    *\n");
    printf("* 1 - CADASTRAR ALUNO     *\n");
    printf("* 2 - EDITAR ALUNO        *\n");
    printf("* 3 - REMOVER ALUNO       *\n");
    printf("* 4 - LISTAGEM GERAL      *\n");
    printf("* 5 - LISTAGEM POR CURSO  *\n");
    printf("* 6 - LISTAGEM POR SEXO   *\n");
    printf("* 0 - SAIR                *\n");
    printf("***************************\n");
}

void CadastrarAlunos(){
    int opc;
        printf("\n***************************");
        printf("\n*    CADASTRO DE ALUNO    *");
        printf("\n*                         *");
        printf("\n* MATRICULA: ");
        scanf ("%d", &Aluno.matricula);
        fflush(stdin);
        printf("* NOME: ");
        scanf ("%[^\n]", &Aluno.nome);
        fflush(stdin);
        printf("* SEXO(F-FEM/M-MASC): ");
        scanf ("%c", &Aluno.sexo);
        fflush(stdin);
        printf("* DATA NASCIMENTO(EX: 01/01/1900): ");
        scanf ("%s", &Aluno.data_nascimento);
        fflush(stdin);
        printf("* TURNO(1-MANHA/2-NOITE): ");
        scanf ("%c", &Aluno.turno[1]);
        fflush(stdin);
        printf("* CURSO(1-PHP/2-JAVA/3-PYTHON): ");
        scanf ("%c", &Aluno.curso[1]);
        fflush(stdin);
        if(ValidarCamposCadastro()==0){
            Aluno.status = 'A';
            printf("* DESEJA CADASTRAR UM OUTRO CURSO(1-SIM/2-NAO): ");
            scanf ("%d", &opc);
            fflush(stdin);
            if(opc == 1){
                printf("* ESCOLHA O OUTRO CURSO   \n");
                printf("* TURNO(1-MANHA/2-NOITE): ");
                scanf ("%c", &Aluno.turno[2]);
                fflush(stdin);
                printf("* CURSO(1-PHP/2-JAVA/3-PYTHON): ");
                scanf ("%c", &Aluno.curso[2]);
                fflush(stdin);
                if(ValidarCursoExtra()==0){
                    CalculoDaMensalidade('2');
                    GravarArquivo();
                }
            }else {
                CalculoDaMensalidade('1');
                GravarArquivo();

            }
            printf("* ALUNO CADASTRADO COM SUCESSO!\n");
            system("pause");
        }
        Aluno  = AlunoNulo;
}

int ValidarCamposCadastro(){
    if(ValidarMatricula() != 0) return 1;
    if(ValidarNome() != 0) return 1;
    if(ValidarSexo() != 0) return 1;
    if(ValidarDataNascimento() != 0) return 1;
    if(ValidarCurso() != 0) return 1;

    return 0;
}

int ValidarMatricula(){
    if(Aluno.matricula <= 0){
        printf("\n* Campo MATRICULA nao pode ser 0!\n");
        system("pause");
        return 1;
    }
    if(ProcurarMatricula(Aluno.matricula) != -1){
        printf("\n* O numero da MATRICULA ja existe!\n");
        system("pause");
        return 1;
    }
    return 0;
}

int ValidarNome(){
    int tamanhoNome = strlen(Aluno.nome);
    if(tamanhoNome < 3){
        printf("* NOME muito curto para existir!\n");
        system("pause");
        return 1;
    }
    for(a=0;a<30; a++){
        if(Aluno.nome[a] > '0' && Aluno.nome[a] < '9'){
            printf("* Nao pode ter numeros no NOME!\n");
            system("pause");
            return 1;
        }
    }
    return 0;
}

int ValidarSexo(){
    if((Aluno.sexo != 'f' && Aluno.sexo != 'm') &&
       (Aluno.sexo != 'F' && Aluno.sexo != 'M')) {
        printf("* Favor digite uma opcao valida para SEXO!\n");
        system("pause");
        return 1;
       }
    return 0;
}

int ValidarDataNascimento(){
    if(Aluno.data_nascimento[2] != '/' && Aluno.data_nascimento[5] != '/'){
        printf("* Formato da DATA DE NASCIMENTO incorreto!\n");
        system("pause");
        return 1;
    }

    char dia[3] = {Aluno.data_nascimento[0],Aluno.data_nascimento[1],'\0'};
    char mes[3] = {Aluno.data_nascimento[3],Aluno.data_nascimento[4],'\0'};
    char ano[5] = {Aluno.data_nascimento[6],Aluno.data_nascimento[7],
                   Aluno.data_nascimento[8],Aluno.data_nascimento[9],'\0'};

    int ddia = strtol(dia, NULL, 10);
    int mmes = strtol(mes, NULL, 10);
    int aano = strtol(ano, NULL, 10);
    int bbisexto = bissexto(aano);

    if(aano >= DataAtual.ano){
        printf("* ANO de nascimento invalido! Esse aluno ainda nao nasceu!?\n");
        system("pause");
        return 1;
    }
    if(mmes > 12 || mmes <= 0){
        printf("* MES de nascimento invalido!\n");
        system("pause");
        return 1;
    }

    if(ddia > dias_mes[bbisexto][mmes] || ddia <= 0){
        printf("* DIA de nascimento Invalido!\n");
        system("pause");
        return 1;
    }

    int idade = CalcularIdade();

    if(idade < 15){
        printf("* Aluno com uma IDADE muito baixa para o curso!\n");
        system("pause");
        return 1;
    }
    if(idade > 100){
        printf("* Aluno com uma IDADE muito avancada para o curso!\n");
        system("pause");
        return 1;
    }
    return 0;
}

int ValidarCurso(){
    if(Aluno.turno[1] != '1' && Aluno.turno[1] != '2'){
        printf("* Favor digitar TURNO valido!\n");
        system("pause");
        return 1;
    }
    if(Aluno.turno[1] == Aluno.turno[2]){
        printf("* Ja tem um CURSO em andamento neste TURNO!\n");
        system("pause");
        return 1;
    }
    if(Aluno.curso[1] != '1' && Aluno.curso[1] != '2' && Aluno.curso[1] != '3'){
        printf("* Favor digitar CURSO valido!\n");
        system("pause");
        return 1;
    }
    if(Aluno.curso[1] == Aluno.curso[2]){
        printf("* O Aluno ja tem este CURSO matriculado!\n");
        system("pause");
        return 1;
    }
    return 0;
}

int ValidarCursoExtra(){
    if(Aluno.turno[2] != '1' && Aluno.turno[2] != '2'){
        printf("* Favor digitar TURNO valido!\n");
        system("pause");
        return 1;
    }
    if(Aluno.turno[2] == Aluno.turno[1]){
        printf("* Ja tem um CURSO em andamento neste TURNO!\n");
        system("pause");
        return 1;
    }
    if(Aluno.curso[2] != '1' && Aluno.curso[2] != '2' && Aluno.curso[2] != '3'){
        printf("* Favor digitar CURSO valido!\n");
        system("pause");
        return 1;
    }
    if(Aluno.curso[2] == Aluno.curso[1]){
        printf("* O Aluno ja tem este CURSO matriculado!\n");
        system("pause");
        return 1;
    }
    return 0;
}

int ProcurarMatricula(int mat){
    AlunoAux = AlunoNulo;
    int position;
    rewind(PAluno);
    fread(&AlunoAux,sizeof(AlunoAux),1,PAluno);
    while(feof(PAluno) == 0){
        if(AlunoAux.matricula == mat){
            return position;
        }
        fread(&AlunoAux,sizeof(AlunoAux),1,PAluno);
        position++;
    }
    AlunoAux = AlunoNulo;
    return -1;
}

void CalculoDaMensalidade(char qtdCurso){

    if(qtdCurso == '1' || qtdCurso == '2'){
        switch(Aluno.curso[1]){
        case '1':
            if(Aluno.turno[1] == '1')
                Aluno.mensalidade = PHP_MANHA;
            if(Aluno.turno[1] == '2')
                Aluno.mensalidade = PHP_NOITE;
            break;
        case '2':
            if(Aluno.turno[1] == '1')
                Aluno.mensalidade = JAVA_MANHA;
            if(Aluno.turno[1] == '2')
                Aluno.mensalidade = JAVA_NOITE;
            break;
        case '3':
            if(Aluno.turno[1] == '1')
                Aluno.mensalidade = PHYTON_MANHA;
            if(Aluno.turno[1] == '2')
                Aluno.mensalidade = PHYTON_NOITE;
            break;
        }
    }
    if(qtdCurso == '2'){
        switch(Aluno.curso[2]){
        case '1':
            if(Aluno.turno[2] == '1')
                Aluno.mensalidade = Aluno.mensalidade + PHP_MANHA;
            if(Aluno.turno[2] == '2')
                Aluno.mensalidade = Aluno.mensalidade + PHP_NOITE;
            break;
        case '2':
            if(Aluno.turno[2] == '1')
                Aluno.mensalidade = Aluno.mensalidade + JAVA_MANHA;
            if(Aluno.turno[2] == '2')
                Aluno.mensalidade = Aluno.mensalidade + JAVA_NOITE;
            break;
        case '3':
            if(Aluno.turno[2] == '1')
                Aluno.mensalidade = Aluno.mensalidade + PHYTON_MANHA;
            if(Aluno.turno[2] == '2')
                Aluno.mensalidade = Aluno.mensalidade + PHYTON_NOITE;
            break;
        }
    }
    if(qtdCurso == '2'){
        Aluno.mensalidade = Aluno.mensalidade - (Aluno.mensalidade * 0.30);
    }else{
        if(CalcularIdade() > 45){
            Aluno.mensalidade = Aluno.mensalidade - (Aluno.mensalidade * 0.15);
        }
    }
}

int CalcularIdade(){
    char dia[3] = {Aluno.data_nascimento[0],Aluno.data_nascimento[1],'\0'};
    char mes[3] = {Aluno.data_nascimento[3],Aluno.data_nascimento[4],'\0'};
    char ano[5] = {Aluno.data_nascimento[6],Aluno.data_nascimento[7],
                   Aluno.data_nascimento[8],Aluno.data_nascimento[9],'\0'};

    DataInicio.dia = strtol(dia, NULL, 10);
    DataInicio.mes = strtol(mes, NULL, 10);
    DataInicio.ano = strtol(ano, NULL, 10);

    int idade;
    idade = DataAtual.ano - DataInicio.ano;
    if(DataInicio.mes > DataAtual.mes){
        idade -= 1;
    }else{
        if(DataInicio.mes == DataAtual.mes){
            if(DataInicio.dia > DataAtual.dia){
                idade -= 1;
            }
        }
    }
    return idade;
}

void EditarAlunos(){
    int opc;
    printf("\n***************************");
    printf("\n*      EDITAR  ALUNO      *");
    printf("\n* MATRICULA: ");
    scanf ("%d", &Aluno.matricula);
    fflush(stdin);
    int posicaoRegistro = ProcurarMatricula(Aluno.matricula);
    Aluno = AlunoAux;
    if(posicaoRegistro != -1){
            printf("\n\n");
        CabecalhoListagem();
        MostrarEmTela();
        printf("\n* DESEJA EDITAR ESSE ALUNO?(1-SIM/2-NAO):");
        scanf("%d",&opc);
        fflush(stdin);
        if(opc == 1){
            if(Aluno.status == 'A'){
                do{
                    printf("\n\n");
                    printf("***************************\n");
                    printf("*      EDITAR ALUNO       *\n");
                    printf("* ESCOLHA UMA OPCAO PARA  *\n");
                    printf("*        EDITAR!          *\n");
                    printf("*                         *\n");
                    printf("* 1 - NOME                *\n");
                    printf("* 2 - SEXO                *\n");
                    printf("* 3 - DATA DE NASCIMENTO  *\n");
                    printf("* 4 - TURNO 1             *\n");
                    printf("* 5 - CURSO 1             *\n");
                    printf("* 6 - TURNO 2             *\n");
                    printf("* 7 - CURSO 2             *\n");
                    printf("*                         *\n");
                    printf("* DIGITE UMA OPCAO:");
                    scanf ("%d", &opc);
                    fflush(stdin);
                    switch(opc){
                    case 1:
                        printf("\n* NOME:");
                        scanf ("%s", &Aluno.nome);
                        break;
                    case 2:
                        printf("\n* SEXO(F-FEM/M-MASC):");
                        scanf ("%c", &Aluno.sexo);
                        break;
                    case 3:
                        printf("\n* DATA DE NASCIMENTO(EX: 01/01/1900):");
                        scanf ("%s", &Aluno.data_nascimento);
                        break;
                    case 4:
                        printf("\n* TURNO 1(1-MANHA/2-NOITE):");
                        scanf ("%c", &Aluno.turno[1]);
                        break;
                    case 5:
                        printf("\n* CURSO 1(1-PHP/2-JAVA/3-PYTHON):");
                        scanf ("%c", &Aluno.curso[1]);
                        break;
                    case 6:
                        printf("\n* TURNO 2(1-MANHA/2-NOITE):");
                        scanf ("%c", &Aluno.turno[2]);
                        break;
                    case 7:
                        printf("\n* CURSO 2(1-PHP/2-JAVA/3-PYTHON):");
                        scanf ("%c", &Aluno.curso[2]);
                        break;
                    default:
                        printf("* FAVOR DIGITAR OPCAO VALIDA!\N");
                        system("pause");
                    }
                    fflush(stdin);
                    printf("* DESEJA CONTINUAR EDITANDO?(1-SIM/2-NAO):");
                    scanf ("%d",&opc);
                    fflush(stdin);
                }while(opc != 2);
                if(ValidarCamposEditar()==0){
                    if(Aluno.curso[2] != NULL){
                        CalculoDaMensalidade('2');
                    }else{
                        CalculoDaMensalidade('1');
                    }
                    RescreverRegistro(posicaoRegistro);
                    printf("* ALUNO foi atualizado com sucesso!\n");
                    system("pause");
                }
            }else{
                printf("* MATRICULA ESTA DESATIVADA!\n");
                printf("* DESEJA REATIVAR A MATRICULA?(1-SIM/2-NAO):");
                scanf("%d", &opc);
                fflush(stdin);
                if(opc == 1){
                    Aluno.status = 'A';
                    RescreverRegistro(posicaoRegistro);
                    printf("* ALUNO foi reativado com sucesso!\n");
                    system("pause");
                }else if(opc != 2){
                    printf("* OPCAO INVALIDA!\n");
                    system("pause");
                }
            }
        }else if(opc != 2){
            printf("* OPCAO INVALIDA!\n");
            system("pause");
        }
    }else{
        printf("* MATRICULA nao encontrada!\n");
        system("pause");
    }
    Aluno    = AlunoNulo;
    AlunoAux = AlunoNulo;
}

int ValidarCamposEditar(){
    if(ValidarNome() != 0) return 1;
    if(ValidarSexo() != 0) return 1;
    if(ValidarDataNascimento() != 0) return 1;
    if(ValidarCurso() != 0) return 1;
    if(Aluno.curso[2] != NULL){
        if(ValidarCursoExtra() != 0) return 1;
    }

    return 0;
}

void RemoverAlunos(){
    int opc;
    printf("\n***************************");
    printf("\n*      REMOVER ALUNO      *");
    printf("\n*                         *");
    printf("\n* MATRICULA: ");
    scanf ("%d", &Aluno.matricula);
    int posicaoRegistro = ProcurarMatricula(Aluno.matricula);
    Aluno = AlunoAux;
    if(posicaoRegistro != -1){
        printf("\n\n");
        CabecalhoListagem();
        MostrarEmTela();
        printf("\n* DESEJA DESATIVAR ESSE ALUNO?(1-SIM/2-NAO):");
        scanf("%d",&opc);
        if(opc == 1){
           if(Aluno.status == 'A'){
                Aluno.status = 'I';
                RescreverRegistro(posicaoRegistro);
                printf("* ALUNO foi desativado com sucesso!\n");
                system("pause");
           }else{
                printf("* ALUNO ja esta desativado!");
                system("pause");
           }
        }
    }else{
        printf("* MATRICULA nao encontrada!\n");
        system("pause");
    }
    Aluno    = AlunoNulo;
    AlunoAux = AlunoNulo;
}

void ListarGeral(){
    int opc;
    printf("*******************\n");
    printf("* DIGITE UM OPCAO *\n");
    printf("*   DE LISTAGEM   *\n");
    printf("* 1 - ATIVOS      *\n");
    printf("* 2 - DESATIVADOS *\n");
    printf("* 3 - TODOS       *\n");
    printf("*******************\n\n");
    printf("* DIGITE UMA OPCAO:");
    scanf("%d",&opc);

    if(opc != 1 && opc != 2 && opc != 3){
        printf("* OPCAO INVALIDA!\n");
    }else{
        printf("\n\t\t\t\t\t\t..*** | Listagem Geral | ***..\n\n");
        CabecalhoListagem();
        rewind(PAluno);
        fread(&Aluno,sizeof(Aluno),1,PAluno);
        while(feof(PAluno) == 0){
            if(opc == 1){
                if(Aluno.status == 'A')MostrarEmTela();
            }else if(opc == 2){
                if(Aluno.status == 'I')MostrarEmTela();
            }else{
                MostrarEmTela();
            }
            fread(&Aluno,sizeof(Aluno),1,PAluno);
        }
    }
    system("pause");
}

void CabecalhoListagem(){
    printf("----------.-------------------------------.------.--------------------.---------.---------.---------.---------.-------------.--------\n");
    printf("Matricula | Nome                          | Sexo | Data de Nascimento | Turno 1 | Curso 1 | Turno 2 | Curso 2 | Mensalidade | Status \n");
    printf("----------|-------------------------------|------|--------------------|---------|---------|---------|---------|-------------|--------\n");
}

void MostrarEmTela(){
    char sexo;
    char turno[6];
    char curso[7];
    char turnoExtra[6];
    char cursoExtra[7];
    char status[5];
    char nomeAux[30];
    char nome[30];
    char tamanhoNome = strlen(Aluno.nome);
    strcpy(nomeAux, Aluno.nome);

    for(a=0; a < tamanhoNome; a++){
        nome[a] = nomeAux[a];
    }

    for(a=tamanhoNome; a < 29; a++){
        nome[a] = ' ';
    }

    nome[30] = '\0';


    if(Aluno.sexo == 'f' || Aluno.sexo == 'F')
        sexo = 'F';
    else if(Aluno.sexo == 'm' || Aluno.sexo == 'M')
        sexo = 'M';

    if(Aluno.turno[1] == '1'){
        strcpy(turno, MANHA);
    }else{
        strcpy(turno, NOITE);
    }

    switch(Aluno.curso[1]){
    case '1': strcpy(curso, PHP);   break;
    case '2': strcpy(curso, JAVA);  break;
    case '3': strcpy(curso, PHYTON);break;
    }

    printf("%9.0d | %s | %c    | %s         | %s  | %s  ",
           Aluno.matricula, nome, sexo, Aluno.data_nascimento, turno, curso);

    if(Aluno.turno[2] == NULL){
        strcpy(turnoExtra, VAZIO);
    }else{
        if(Aluno.turno[2] == '1'){
            strcpy(turnoExtra, MANHA);
        }else if(Aluno.turno[2] == '2'){
            strcpy(turnoExtra, NOITE);
        }
    }

    printf("| %s  ", turnoExtra);

    switch(Aluno.curso[2]){
    case '1': strcpy(cursoExtra, PHP);   break;
    case '2': strcpy(cursoExtra, JAVA);  break;
    case '3': strcpy(cursoExtra, PHYTON);break;
    default : strcpy(cursoExtra, VAZIO);
    }

    printf("| %s  ", cursoExtra);


    if(Aluno.status == 'A'){
        strcpy(status, ATIVO);
    }else{
        strcpy(status, INATIVO);
    }

    printf("| R$ %0.2f   | %s     \n",
            Aluno.mensalidade, status);
}

void ListarPorCurso(){
    int opc;
    printf("*******************\n");
    printf("* DIGITE UM OPCAO *\n");
    printf("*  DE CURSO PARA  *\n");
    printf("*    LISTAGEM     *\n");
    printf("* 1 - PHP         *\n");
    printf("* 2 - JAVA        *\n");
    printf("* 3 - PYTHON      *\n");
    printf("*******************\n\n");
    printf("* DIGITE UMA OPCAO:");
    scanf("%d",&opc);

    if(opc != 1 && opc != 2 && opc != 3){
        printf("* OPCAO INVALIDA!\n");
    }else{
        printf("\n\t\t\t\t\t\t..*** | Listagem por Curso | ***..\n\n");
        CabecalhoListagem();
        rewind(PAluno);
        fread(&Aluno,sizeof(Aluno),1,PAluno);
        while(feof(PAluno) == 0){
            if(opc == 1){
                if((Aluno.curso[1] == '1' || Aluno.curso[2] == '1') && Aluno.status == 'A')
                    MostrarEmTela();
            }else if(opc == 2){
                if((Aluno.curso[1] == '2' || Aluno.curso[2] == '2') && Aluno.status == 'A')
                    MostrarEmTela();
            }else{
                if((Aluno.curso[1] == '3' || Aluno.curso[2] == '3') && Aluno.status == 'A')
                    MostrarEmTela();
            }
            fread(&Aluno,sizeof(Aluno),1,PAluno);
        }
    }
    system("pause");
}

void ListarPorSexo(){
    int opc;
    printf("*******************\n");
    printf("* DIGITE UM OPCAO *\n");
    printf("*   DE SEXO PARA  *\n");
    printf("*     LISTAGEM    *\n");
    printf("* 1 - FEMININO    *\n");
    printf("* 2 - MASCULINO   *\n");
    printf("*******************\n\n");
    printf("* DIGITE UMA OPCAO:");
    scanf("%d",&opc);

    if(opc != 1 && opc != 2){
        printf("* OPCAO INVALIDA!\n");
    }else{
        printf("\n\t\t\t\t\t\t..*** | Listagem por Sexo | ***..\n\n");
        CabecalhoListagem();
        rewind(PAluno);
        fread(&Aluno,sizeof(Aluno),1,PAluno);
        while(feof(PAluno) == 0){
            if(opc == 1){
                if((Aluno.sexo == 'f' || Aluno.sexo == 'F') && Aluno.status == 'A')
                    MostrarEmTela();
            }else if(opc == 2){
                if((Aluno.sexo == 'm' || Aluno.sexo == 'M') && Aluno.status == 'A')
                    MostrarEmTela();
            }
            fread(&Aluno,sizeof(Aluno),1,PAluno);
        }
    }
    system("pause");
}

int main() {

    PegarDataAtual();

    int opcao;
    AbrirArquivo();

    do {
        Cabecalho();

        printf("Digite a opcao:");
        scanf("%d", &opcao);
        printf("\n");

        switch (opcao){
        case  1: CadastrarAlunos(); break;
        case  2: EditarAlunos();    break;
        case  3: RemoverAlunos();   break;
        case  4: ListarGeral();     break;
        case  5: ListarPorCurso();  break;
        case  6: ListarPorSexo();   break;
        case  0: printf("Obrigado por usar nosso programa!\n"); FecharArquivo(); break;
        default: printf("Digite uma opcao valida!\n"); system("pause");
        }
    } while(opcao != 0);

    return 0;
}



