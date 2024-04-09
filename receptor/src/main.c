/* Copyright 2024, Adan Lema <adanlema@hotmail.com> */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "al_client.h"
#include "al_mapping.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
int main() {

    volatile uint32_t * addr = mapping_initialize();
    printf("Mapeo de memoria realizo con exito...\n");

    int sock = client_initialize();
    if (sock < 0) {
        printf("Error al inicializar el cliente...\n");
        return -1;
    }

    uint32_t * buffer = malloc(FPGA_REG * sizeof(uint32_t));
    if (buffer == NULL) {
        perror("Error allocating memory for buffer");
        return -1;
    }

    while (1) {
        uint32_t value = *(addr + FPGA_OFFSET_VALID);
        if (value == 1) {
            memset(buffer, 0, FPGA_REG * sizeof(uint32_t));
            memcpy(buffer, (const void *)addr, FPGA_REG * sizeof(uint32_t));
            send(sock, buffer, FPGA_REG * sizeof(uint32_t), 0);
        }
    };
    // Desconexion
    client_disconnect(sock);
    mapping_finalize(addr);
    return 0;
}
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/