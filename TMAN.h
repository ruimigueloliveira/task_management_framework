/* 
/* 
 * File:   TMANp.h
 * Author: Gabriel Saudade e Rui Oliveira
 *
 * Created on 4 de Janeiro de 2022, 16:23
 */

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../UART/uart.h"

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"


#ifndef TMANP_H
#define	TMANP_H

#define TMAN_TICKS(time) (time * 3)      // TMAN TICKS = 1 segundo
#define TMAN_PERIOD(time) ( time * 3  / portTICK_PERIOD_MS) 
#define TMAN_PRIORITY(p) (tskIDLE_PRIORITY + p)

typedef enum {false, true} bool;

bool TMAN_Init();
void TMAN_TickHandler();
bool TMAN_TaskRegisterAttributes(int periodicity, int phase, int deadline, int priority, char* precedence, char* name, int id);
void TMAN_TaskWaitPeriod(int id);
bool TMAN_TaskAdd(void (*func) (void *pvParam), int id);
void TMAN_TickHandler(void* pvParam);
void TMAN_TaskRelease();
void TMAN_Close();
void TMAN_TaskStats();

typedef struct {   
    TaskHandle_t xHandle;       // task handler
    
    char* name;         // name of a task
    int period;         // period of a task
    int phase;          // task phase
    int deadline;       // task deadline
    int priority;       // task priority
    int act;            // number of activations
    int tick;           // tick number
    
    int dmisses;        // number of deadline misses
    
    char* precedence;   // precedence constrains
}Task;


int counter = 0;        // internal counter
Task task[7];           // task manager

int tck;
int nact;

/**
 * Initializes API adding Tick handler to the scheduler
 * 
 * @return boolean value if the task was successfully created 
 */
bool TMAN_Init() {
    printf("TMAN_Init\n\r");
    nact = -1;
    malloc(sizeof task);
   
    TMAN_TaskRegisterAttributes(TMAN_PERIOD(1), 0, 0, 31, NULL, "TickHandler", 0);
    TMAN_TaskAdd(TMAN_TickHandler, 0);
    
    return (task != NULL);
}


/**
 * Register attributes to the task
 * 
 * @param int periodicity, int phase, int deadline, int priority, char* precedence, char* name
 * @return boolean on successfully attributed values
 */
bool TMAN_TaskRegisterAttributes(int periodicity, int phase, int deadline, int priority, char* precedence, char* name, int id) {
    printf("TMAN_TaskRegisterAttributes\n\r");
    task[id].period = TMAN_PERIOD(periodicity);
    task[id].phase = TMAN_TICKS(phase);
    task[id].deadline = TMAN_TICKS(deadline);
    task[id].priority = TMAN_PRIORITY(priority);
    task[id].name = name;
    task[id].act = 0;
    task[id].dmisses = 0;
    
    uint8_t mesg[80];
    if(precedence != NULL) {
        for(int i = 0; i < counter; i++) {
            if(task[i].name == precedence) {   
                task[id].period = task[i].period - task[i].phase;
            }
        }
    }
    counter++;
    return true;
}


/**
 * Waits for the completion of the task and then suspends it
 */
void TMAN_TaskWaitPeriod(int id) {
    vTaskSuspend( task[id].xHandle );
}

/**
 * 
 * @param void (*func) (void *pvParam)
 * @return true if the task was successfully created 
 */
bool TMAN_TaskAdd(void (*func) (void *pvParam), int id) {
    printf("TMAN_TaskAdd\n\r");
    BaseType_t xReturned;
    xReturned = xTaskCreate(func, task[id].name, configMINIMAL_STACK_SIZE, (void*) id, task[id].priority, &task[id].xHandle);
    
    return xReturned == pdPASS;
}

/**
 * Tick Handler is the body of the task that handles the TMAN tick
 * 
 * @param void* pvParam
 */
void TMAN_TickHandler(void* pvParam) {
    portTickType xLastWakeTime;
    int id;
    
    
    char str[16];
    
    int flag = 0;
    for(;;) {
        xLastWakeTime = xTaskGetTickCount();
        
        for(id = 1; id < counter; id++) {
            tck = (int) xLastWakeTime - task[id].phase;
            if(tck % task[id].period == 0) {
                task[id].tick = tck;
                task[id].act++;
                
                task[id].deadline += tck;
                
                
                flag = 1;
                vTaskResume( task[id].xHandle ); 
            }
            
            if ((tck > (task[id].deadline + task[id].tick)) && flag) {
                task[id].dmisses++;
                flag = 0;
            }
            
        }  
        
        nact++;
        vTaskDelay(task[0].period);
    } 
    
}

/**
 * Starts the scheduler 
 */
void TMAN_TaskRelease() {
    vTaskStartScheduler();
}

/**
 * Prints the statistics of each task
 */
void TMAN_TaskStats() {
    char str[4];
    
    printf("##########TASK STATISTICS##########\n\r");
    for(int i = 1; i < counter; i++) {
        printf("Task Name: %s\t", task[i].name);
        itoa(str, task[i].act, 10);
        printf("Task activations: %s\t", str);
        
        itoa(str, task[i].dmisses, 10);
        printf("Deadline misses: %s\n\r", str);
    }
    
}

/**
 * Closes the API freeing memory 
 */
void TMAN_Close() {
    for (int i = 0; i < counter; i++)
        if (task[i].xHandle != NULL) vTaskDelete(task[i].xHandle);
    vPortFree((void*)task);
}



#endif	/* TMANP_H */