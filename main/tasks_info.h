/**
 * @file tasks_info.h
 * @brief Header file for task information in ESP32 SCIR project.
 *
 * @author Kamil Kośnik
 * @date   2025-03-07
 * 
 * @par License:
 * - This code is released under the MIT License.
 */

#ifndef TASKS_INFO_H_
#define TASKS_INFO_H_

/* Includes ------------------------------------------------------------------*/

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

/* Definitions ---------------------------------------------------------------*/

// Priorities
#define priorityNORMAL              ( 3U )

// Functional priorities
#define priorityIDLE                ( priorityNORMAL - 3U )  // 0
#define priorityLOW                 ( priorityNORMAL - 2U )  // 1
#define priorityBELOW_NORMAL        ( priorityNORMAL - 1U )  // 2

#define priorityABOVE_NORMAL        ( priorityNORMAL + 1U )  // 4
#define priorityHIGH                ( priorityNORMAL + 2U )  // 5
#define priorityREAL_TIME           ( priorityNORMAL + 3U )  // 6

#define TASK_PRIO_ADC               ( priorityNORMAL )
#define TASK_PRIO_BME280            ( priorityNORMAL )
#define TASK_PRIO_SHTC3             ( priorityNORMAL )

#if (configMAX_PRIORITIES < priorityIDLE)
    #error Idle task priority too low
#endif

#if (configMAX_PRIORITIES < priorityREAL_TIME)
    #error Highest priority task priority too high
#endif

// Sizes

// Functional task sizes
#define taskSizeMINIMAL             ( 1536U )
#define taskSizeNORMAL              ( 2048U )
#define taskSizeGREATER             ( 3072U )
#define taskSizeBIGGEST             ( 4096U )
#define taskSizeSTORAGE             ( 4096U * 2 )

#define TASK_SIZE_ADC               ( taskSizeNORMAL )
#define TASK_SIZE_BME280            ( taskSizeBIGGEST )
#define TASK_SIZE_SHTC3             ( taskSizeBIGGEST )


#if (configMINIMAL_STACK_SIZE > taskSizeMINIMAL)
    #error Minimal task size too small
#endif


// Task delays
#define taskDelay_ADC               ( 1000U )
#define taskDelay_Main              ( 1000U )
#define taskDelay_BME280            ( 100U )
#define taskDelay_SHTC3             ( 100U )

#endif /* TASKS_INFO_H_ */