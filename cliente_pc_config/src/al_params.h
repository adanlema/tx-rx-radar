/* Copyright 2024, Adan Lema <adanlema@hotmail.com> & Carcamo Mariano <mgcarcamo98@gmail.com> */

#ifndef AL_PARAMS_H
#define AL_PARAMS_H

/*==================[inclusions]=============================================*/
#include <stdint.h>
#include <stdio.h>

#include "al_mapping.h"
/*==================[macros]=================================================*/
#define BARKER7_CODE  0x72
#define BARKER7_NUM   0x7
#define BARKER11_CODE 0x0712
#define BARKER11_NUM  0xB
#define BARKER13_CODE 0x1F35
#define BARKER13_NUM  0xD
/*==================[typedef]================================================*/
typedef struct params_s {
    uint32_t prf;
    uint32_t ab;
    uint32_t freq;
    uint32_t code;
    uint32_t code_num;
    uint32_t start;
} * params_t;

typedef struct {
    int *    soc;
    params_t params;
    char *   r_buff;
    char *   s_buff;
} thread_args;

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

int      paramsStrtoJson(char * str, params_t params);
void     paramsCodeManager(params_t config, const char * code);
void     paramsSetConfig(addrs_t mem_p, params_t config);
void     paramsSaveConfig(params_t params);
void     paramsRestoreDefault(addrs_t addrs, params_t params);
params_t paramsCreate();

/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/
#endif
