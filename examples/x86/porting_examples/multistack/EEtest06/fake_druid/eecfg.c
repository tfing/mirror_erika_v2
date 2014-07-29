#include "ee.h"



//////////////////////////////////////////////////////////////////////////////
//
//   Stack definition for MSP430
//
/////////////////////////////////////////////////////////////////////////////

    #define STACK_1_SIZE 256
    #define STACK_2_SIZE 256
    #define STACK_3_SIZE 256

    #define STACK_OFFSET 16

    int EE_x86_stack_1[STACK_1_SIZE];	// Task 0 (Task1)
    int EE_x86_stack_2[STACK_2_SIZE];	// Task 1 (Task2)
    int EE_x86_stack_3[STACK_3_SIZE];	// IRQ stack

    EE_UREG EE_std_thread_tos[EE_MAX_TASK+1] = {
        0U,	 /* dummy*/
        1U,	 /* Task1*/
        2U 	 /* Task2*/
    };

    struct EE_TOS EE_std_system_tos[3] = {
       {0},	// Task   (dummy)
       {(EE_ADDR)(&EE_x86_stack_1[STACK_1_SIZE-STACK_OFFSET])},// Task 0 (Task1)
       {(EE_ADDR)(&EE_x86_stack_2[STACK_2_SIZE-STACK_OFFSET])} // Task 1 (Task2)
    };

    EE_UREG EE_x86_active_tos = 0; /* dummy */

    /* stack used only by IRQ handlers */
    struct EE_TOS EE_x86_IRQ_tos = {
        (EE_ADDR)(&EE_x86_stack_3[STACK_3_SIZE - 4])
    };

/***************************************************************************
 *
 * Kernel ( CPU 0 )
 *
 **************************************************************************/
    /* Definition of task's body */
    DeclareTask(Task1);
    DeclareTask(Task2);

    const EE_FADDR EE_hal_thread_body[EE_MAX_TASK] = {
        (EE_FADDR)FuncTask1, 		// thread Task1
     	(EE_FADDR)FuncTask2         // thread Task2
    };

    /* ready priority */
    const EE_TYPEPRIO EE_th_ready_prio[EE_MAX_TASK] = {
        0x1U, 		/* thread Task1 */
        0x2U 		/* thread Task2 */
    };

    /* dispatch priority */
    const EE_TYPEPRIO EE_th_dispatch_prio[EE_MAX_TASK] = {
        0x1U, 		/* thread Task1 */
        0x2U 		/* thread Task2 */
    };
    /* thread status */
    #if defined(__MULTI__) || defined(__WITH_STATUS__)
        EE_TYPESTATUS EE_th_status[EE_MAX_TASK] = {
            EE_READY,
            EE_READY
        };
    #endif

    /* next thread */
    EE_TID EE_th_next[EE_MAX_TASK] = {
        EE_NIL,
        EE_NIL
    };

    EE_TYPEPRIO EE_th_nact[EE_MAX_TASK];
    /* The first stacked task */
    EE_TID EE_stkfirst = EE_NIL;

    /* The first task into the ready queue */
    EE_TID EE_rqfirst  = EE_NIL;

    /* system ceiling */
    EE_TYPEPRIO EE_sys_ceiling= 0x0000U;


