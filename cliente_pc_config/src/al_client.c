/* Copyright 2024, Adan Lema <adanlema@hotmail.com> */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "al_client.h"
/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/
struct client_s cliente = {0};
/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
static int client_create_socket() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("[-]Socket error:");
        return -1;
    }
    printf("[+]TCP client socket created.\n");
    return sock;
}

/*==================[external functions definition]==========================*/
client_t clientCreate(uint32_t port, char * ip) {
    cliente.sock = client_create_socket();
    cliente.port = port;
    strcpy(cliente.ip, ip);

    memset(&client.addr, '\0', sizeof(client.addr));
    client.addr.sin_family      = AF_INET;
    client.addr.sin_port        = htons(port);
    client.addr.sin_addr.s_addr = inet_addr(ip);
    return &cliente;
}

int clientConnect(client_t client) {
    if (connect(client->sock, (struct sockaddr *)&client->addr, sizeof(client->addr)) != 0) {
        perror("Error al conectar el cliente con el servidor: ");
        return -1;
    }
    printf("Cliente conectado con exito...\n");
    return 0;
}

void clientDisconnect(client_t client) {
    close(client->sock);
    printf("Desconexion del cliente del server...\n");
}

void clientRemove(client_t client) {
    if (client != NULL) {
        close(client->sock);
        free(client);
    }
}

/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/