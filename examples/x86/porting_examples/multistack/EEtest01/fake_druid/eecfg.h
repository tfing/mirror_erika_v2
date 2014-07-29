#ifndef __EECFG_H__
#define __EECFG_H__


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
    #define EE_MAX_RESOURCE 0U

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


//////////////////////////////////////////////////////////////////////////////
//   Automatic options
/////////////////////////////////////////////////////////////////////////////
#define __FP_NO_RESOURCES__

#endif

#endif

