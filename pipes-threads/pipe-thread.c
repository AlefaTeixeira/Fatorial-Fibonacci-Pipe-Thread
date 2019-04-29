#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXBUFF 1024

void *fatorial(void *arguments);
int fibonacci(int num);
void *runner(void *param);

struct argumentos_thread {
    int inicio;
    int fim;
};

struct package {
    int menu;
    int num;
    long result;
};

int sum;

void client(readfd, writefd)

int readfd, writefd;

{

    struct package pacote;
    char c[2];
    pacote.menu = 0;

    do {

        if (pacote.menu == 0) {

            system("clear");
            printf("\n --------------------- Calculos PIPE/THREADS --------------------- \n");
            printf(" 1 - Fatorial \n");
            printf(" 2 - Sequencia Fibonnaci \n");
            printf(" 3 - Sair \n ");
            fflush(stdin);
            scanf("%d", &pacote.menu);

        }else if (pacote.menu == 1 || pacote.menu == 2) {

            read(readfd, &pacote, sizeof (pacote));

            printf(" Resultado recebido <- %ld \n", pacote.result);

            error_caracter:

            printf("\n Calcular novamente? (y/n) ");

	    fflush(stdin);
            fflush(stdout);
            scanf("%s", c);

            if (c[0] == 'n') {
		fflush(stdin);
		fflush(stdout);
		pacote.num = 0;
                pacote.menu = 0;
            } else if (c[0] != 'y') {
		fflush(stdin);
		fflush(stdout);
		pacote.num = 0;
                system("clear");
                printf("\n Caracter invalido\n");
                goto error_caracter;
            }
        }

        switch (pacote.menu) {

            case 1:
		fflush(stdin);
                system("clear");
                printf(" \n Fatorial-> ");
                scanf("%d", &pacote.num);
                pacote.menu = 1;

                break;

            case 2:
		fflush(stdin);
                system("clear");
                printf(" \n Digite a posição da série que deseja mostrar-> ");
                scanf("%d", &pacote.num);
                pacote.menu = 2;

                break;


            case 3:

	        system("clear");
                exit(1);

                break;

        }

	write(writefd, &pacote, sizeof (pacote));

    } while (pacote.menu != 3);
}

void server(readfd, writefd)

int readfd, writefd;

{
    struct package pacote;
    struct argumentos_thread args1, args2;
    pthread_t t1, t2;
    pthread_attr_t attr;
    int i;
    long result, result2;

    while (1) {

        read(readfd, &pacote, sizeof (pacote));

        switch (pacote.menu) {

            case 1:

    		args1.inicio = 1;
   	        args1.fim = (pacote.num / 2);
    		args2.inicio = (pacote.num / 2) + 1;
   	        args2.fim = pacote.num;

                pthread_create (&t1,NULL,fatorial,(void *)&args1);
                pthread_create (&t2, NULL,fatorial, (void *)&args2);
		pthread_join(t1, (void **)&result);
		pthread_join(t2, (void **)&result2);

                pacote.result = result * result2;

		break;

            case 2:

	    	if (pacote.num < 0){
	             system("clear");
		     printf("\n Fibonacci de um numero negativo nao eh possivel.\n");
		     pacote.result = 0;
	    	}else{

			pthread_attr_init(&attr);

			for(i=1;i<=pacote.num;i++){
				pthread_t t3;
				pthread_create(&t3,&attr,runner,(void *)i);
				pthread_join(t3,NULL);
			}

			pacote.result = sum;


	    	}

                break;

        }

        write(writefd, &pacote, sizeof (pacote));

    }
}

int main() {

    int descritor, pipe1[2], pipe2[2];

    if (pipe(pipe1) < 0 || pipe(pipe2) < 0) {
        printf("Erro na chamada PIPE");
        exit(0);
    }

    if ((descritor = fork()) < 0) {
        printf("Erro na chamada FORK");
        exit(0);
    } else if (descritor > 0) {
        close(pipe1[0]);
        close(pipe2[1]);

        client(pipe2[0], pipe1[1]);

        close(pipe1[1]);
        close(pipe2[0]);
        exit(0);
    } else {
        close(pipe1[1]);
        close(pipe2[0]);

        server(pipe1[0], pipe2[1]);

        close(pipe1[0]);
        close(pipe2[1]);
        exit(0);

    }
}

void *fatorial(void *arguments) {

    long result;
    struct argumentos_thread *args = arguments;

    for(result = (args -> inicio); (args -> fim) > (args -> inicio); (args -> fim) = (args -> fim) - 1)
	result = result * (args -> fim);

    pthread_exit((long *)result);

}

void *runner(void *param){

	sum = fibonacci((int)param);
	pthread_exit(0);

}

int fibonacci(int num){

    if (num <= 1)
	return 1;

    return(fibonacci(num - 1) + fibonacci(num - 2));
}
