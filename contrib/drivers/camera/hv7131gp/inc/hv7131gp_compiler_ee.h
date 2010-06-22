/** 
* @file hv7131gp_compiler_ee.h
* @brief Erika Enterprise Compiler support for HV7131GP.
* @author Marco Ghibaudi
* @date 2010-03-19
*
* Using the Erika Enterprise abstraction for the compiler.
*/

#ifndef __hv7131gp_compiler_ee_h__
#define __hv7131gp_compiler_ee_h__

#include <ee.h>
//#include "p32xxxx.h"
/** 
* @name Basic type representations.
* @{ */

#ifndef DEF_BOOL_TYPE
#define DEF_BOOL_TYPE
/* chris: FIXME: do we really need this!? */
typedef enum {
	TRUE 		= 0x01,	/**< True */
	FALSE		= 0x00, /**< False */
} BOOL;
typedef BOOL Boolean;
#endif /* DEF_BOOL_TYPE */

#ifndef UINT8_T
#define UINT8_T
typedef EE_UINT8 uint8_t;	/**< Unsigned 8 bit integer. */
#endif

#ifndef UINT16_T
#define UINT16_T
typedef EE_UINT16 uint16_t;	/**< Unsigned 16 bit integer. */
#endif

#ifndef UINT32_T
#define UINT32_T
typedef EE_UINT32 uint32_t;	/**< Unsigned 32 bit integer. */
#endif

// TODO: Create EE_UINT64?
#ifndef UINT64_T
#define UINT64_T
typedef unsigned long long uint64_t;	/**< Unsigned 64 bit integer. */
#endif

#ifndef INT8_T
#define INT8_T
typedef EE_INT8 int8_t;		/**< Signed 8 bit integer. */
#endif

#ifndef INT16_T
#define INT16_T
typedef EE_INT16 int16_t;	/**< Signed 16 bit integer. */
#endif

#ifndef INT32_T
#define INT32_T
typedef EE_INT32 int32_t;	/**< Signed 32 bit integer. */
#endif


/**  @} */

#ifndef COMPILER_INLINE 
#define COMPILER_INLINE __INLINE__
#endif

/* chris: TODO: move this in the HAL, see MRF24J40 solution! */
#ifndef COMPILER_ISR
#define COMPILER_ISR(func) ISR2(func)
#endif

#endif /* Header Protection */
