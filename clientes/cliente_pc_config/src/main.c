/* Copyright 2024, Adan Lema <adanlema@hotmail.com> & Carcamo Mariano <mgcarcamo98@gmail.com> */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <json-c/json.h>
#include <pthread.h>

#include "al_params.h"
#include "al_client.h"
/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/
pthread_t thread_recv, thread_send;
/*==================[internal functions declaration]=========================*/
void * recive_msg(void * args);
void * send_msg(void * args);
void   conection_server(client_t client, thread_args_t th);
/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
void * recive_msg(void * args) {
    thread_args_t arg    = args;
    char *        buff   = arg->r_buff;
    int           soc_d  = *(arg->soc);
    params_t      params = arg->params;

    while (1) {
        memset(buff, 0, BUFTCP_SIZE);
        recv(soc_d, buff, BUFTCP_SIZE, 0);
        paramsUpdate(buff, params);
        printf("\n[server] %s", buff);
    }
}

void * send_msg(void * args) {
    thread_args_t arg    = args;
    char *        buff   = arg->s_buff;
    params_t      params = arg->params;
    int           soc_d  = *(arg->soc);

    while (1) {
        memset(buff, 0, BUFTCP_SIZE);
        scanf("%s", buff);
        if (!strcmp(buff, "EXIT")) {
            break;
        }
        if (paramsStrtoJson(buff, params) == -1) {
            printf("\nERROR: Se requiere una estructura JSON para enviar los datos\n");
        } else {
            memset(buff, 0, BUFTCP_SIZE);
            sprintf(buff,
                    "{\"prf\":%d, \"freq\":%d, \"ab\":%d, \"code\":%d, \"code-num\":%d, "
                    "\"start\":%d}\n",
                    params->prf, params->freq, params->ab, params->code, params->code_num,
                    params->start);
            send(soc_d, buff, BUFTCP_SIZE, 0);
        }
    }
}
void conection_server(client_t client, thread_args_t th) {
    int err;
    if (clientConnect(client) != 0) {
        printf("\n\nError al conectar el cliente...\n");
        return;
    }
    printf("\n\n[+] Cliente conectado con exito...\n[+] IP: %s\n[+] PORT: %u\n[+] Escriba "
           "EXIT "
           "para salir\n\n",
           clientGetIP(client), clientGetPort(client));

    err = pthread_create(&thread_recv, NULL, recive_msg, th);
    if (err) {
        printf("An error occured: %d\n", err);
        return;
    }
    err = pthread_create(&thread_send, NULL, send_msg, th);
    if (err) {
        printf("An error occured: %d\n", err);
        return;
    }
    while (1) {
        if (!strcmp(th->s_buff, "EXIT")) {
            printf("Cerrando conexion...\n");
            clientDisconnect(client);
            pthread_cancel(thread_recv);
            pthread_cancel(thread_send);
            printf("Conexion cerrada con exito...\n");
            break;
        }
    }
}

/*==================[external functions definition]==========================*/

int main() {
    int eleccion;

    client_t client_tx = clientCreate(PORT_TX, IP_TX);
    client_t client_rx = clientCreate(PORT_RX, IP_RX);

    params_t      params_tx = paramsCreate();
    thread_args_t thread_tx = threadCreate(clientGetDirSock(client_tx), params_tx);
    params_t      params_rx = paramsCreate();
    thread_args_t thread_rx = threadCreate(clientGetDirSock(client_rx), params_rx);

    while (1) {
        printf("\n\n Seleccione numericamente el comando que desea realizar:\n\
        [1] Conectar TX\n\
        [2] Conectar RX\n\
        [3] Salir\n\n");
        scanf("%d", &eleccion);
        switch (eleccion) {
            case 1:
                conection_server(client_tx, thread_tx);
                break;
            case 2:
                conection_server(client_rx, thread_rx);
                break;
            case 3:
                printf("Saliendo del programa...\n\n");
                threadFinalize(thread_tx);
                threadFinalize(thread_rx);
                return 0;
            default:
                printf("Comando invalido, vuelve a intentarlo...\n");
                break;
        }
    }

    return 0;
}

/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/