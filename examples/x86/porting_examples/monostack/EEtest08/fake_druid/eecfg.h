#ifndef __EECFG_H__
#define __EECFG_H__


#define RTDRUID_CONFIGURATOR_NUMBER 1275



//////////////////////////////////////////////////////////////////////////////
//
//   Common defines ( CPU 0 )
//
/////////////////////////////////////////////////////////////////////////////

    // TASK definition
    #define EE_MAX_TASK 2
    #define Task1 0
    #define Task2 1

    // MUTEX definition
    #define EE_MAX_RESOURCE 2U
    #define RES_SCHEDULER 0U
    #define Resource 1U

    // ALARM definition
    #define EE_MAX_ALARM 0

    // COUNTER definition
    #define EE_MAX_COUNTER 0

    // APPMODE definition
    #define EE_MAX_APPMODE 0U

    // CPUs
    #define EE_MAX_CPU 1
    #define EE_CURRENTCPU 0

#ifndef __DISABLE_EEOPT_DEFINES__


//////////////////////////////////////////////////////////////////////////////
//
//   User options
//
/////////////////////////////////////////////////////////////////////////////
#define DEBUG
#define __ASSERT__


//////////////////////////////////////////////////////////////////////////////
//   Automatic options
/////////////////////////////////////////////////////////////////////////////
#define __X86__
#define __USE_UART__
#define __FP__
#define __MONO__

#endif

#endif

