/* 
 * File:   main_a3.c
 * Author: saudade
 *
 * Created on 11 de Dezembro de 2021, 15:37
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
//#include "TMAN.h"
#include "TMAN.h"

/* App includes */
#include "../UART/uart.h"

#define SYSCLK  80000000L // System clock frequency, in Hz
#define PBCLOCK 40000000L // Peripheral Bus Clock frequency, in Hz

#define IMAXCOUNT 0
#define JMAXCOUNT 0


void pvF();
void pvE();
void pvD();
void pvB();
void pvA();
void pvC();
void computationalWork();

void pvF(void* pvParam) {
    uint8_t mesg[80];
    int id = (int) pvParam;
    
    for (;;) {
        sprintf(mesg,"%s, %d\n\r", task[id].name, nact);
        PrintStr(mesg); 
        
        computationalWork();
        TMAN_TaskWaitPeriod(id);
    }
}
void pvE(void* pvParam) {
    uint8_t mesg[80];
    int id = (int) pvParam;
    
    char str[16];
    for (;;) {
        sprintf(mesg,"%s, %d\n\r", task[id].name, nact);
        PrintStr(mesg); 
        
        computationalWork();
        TMAN_TaskWaitPeriod(id);
    }
}
void pvD(void* pvParam) {
    uint8_t mesg[80];
    int id = (int) pvParam;
    
    for (;;) {
        sprintf(mesg,"%s, %d\n\r", task[id].name, nact);
        PrintStr(mesg); 
        
        computationalWork();
        TMAN_TaskWaitPeriod(id);
    }
}
void pvC(void* pvParam) {
    uint8_t mesg[80];
    int id = (int) pvParam;
    
    for (;;) {
        sprintf(mesg,"%s, %d\n\r", task[id].name, nact);
        PrintStr(mesg); 
        
        computationalWork();
        TMAN_TaskWaitPeriod(id);
    }
}
void pvB(void* pvParam) {
    uint8_t mesg[80];
    int id = (int) pvParam;
    
    
    for (;;) {
        sprintf(mesg,"%s, %d\n\r", task[id].name, nact);
        PrintStr(mesg); 
        
        computationalWork();
        TMAN_TaskWaitPeriod(id);     
    }
}
void pvA(void* pvParam) {
    uint8_t mesg[80];
    int id = (int) pvParam;
    
    for (;;) {
        sprintf(mesg,"%s, %d\n\r", task[id].name, nact);
        PrintStr(mesg); 
        
        computationalWork();
        TMAN_TaskWaitPeriod(id);
    }
}

/* Create the application tasks then start the scheduler */
int main_final_project(void) {
    
    // Init UART and redirect tdin/stdot/stderr to UART
    if (UartInit(PBCLOCK, 115200) != UART_SUCCESS) {
        PORTAbits.RA3 = 1; // If Led active error initializing UART
        while (1);
    }

    __XC_UART = 1; /* Redirect stdin/stdout/stderr to UART1*/


    /* Welcome message*/
    printf("\n\n*********************************************\n\r");
    printf("Starting ...\n\r");
    printf("*********************************************\n\r");
    
    TMAN_Init();
    
    TMAN_TaskRegisterAttributes(1, 0, 0, 4, NULL,"A", 1);
    TMAN_TaskRegisterAttributes(1, 0, 0, 4, NULL,"B", 2);
    TMAN_TaskRegisterAttributes(2, 0, 0, 3, NULL, "C", 3);
    TMAN_TaskRegisterAttributes(2, 0, 0, 3, NULL,"D", 4);
    TMAN_TaskRegisterAttributes(5, 0, 0, 2, NULL,"E", 5);
    TMAN_TaskRegisterAttributes(10, 0, 0, 1, NULL,"F", 6);
    
    
    if(TMAN_TaskAdd(pvA, 1)) {
        printf("successfully created task\n\r");
    }

    if(TMAN_TaskAdd(pvB,2)) {
        printf("successfully created task\n\r");
    }
    
    if(TMAN_TaskAdd(pvC,3)) {
        printf("successfully created task\n\r");
    }
    
    if(TMAN_TaskAdd(pvD,4)) {
        printf("successfully created task\n\r");
    }
    
    if(TMAN_TaskAdd(pvE,5)) {
        printf("successfully created task\n\r");
    } 
    
    if(TMAN_TaskAdd(pvF,6)) {
        printf("successfully created task\n\r");
    }

    
    TMAN_TaskStats();
    TMAN_TaskRelease();
    TMAN_Close();

    return (EXIT_SUCCESS);
}


void computationalWork() {
    int i, j;
    int mult;
    for(i = 0; i < IMAXCOUNT; i++) {
        for(j = 0; j < JMAXCOUNT; j++) {
            mult = i * j;
        }
    }
}
