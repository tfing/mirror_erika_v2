#ifndef __compiler_h__
#define __compiler_h__
/** 
* @file compiler.h
* @brief Compiler definitions.
* @author Marinoni Mauro, Nastasi Christian
* @date 2008-11-03
*/

/** 
* @name Basic type representations.
* @{ */
#ifdef __ERIKA__

#include <ee.h>

#ifndef UINT8_T
#define UINT8_T
typedef EE_UINT8 uint8_t;	/**< Unsigned 8 bit intgerer. */
#endif

#ifndef UINT16_T
#define UINT16_T
typedef EE_UINT16 uint16_t;	/**< Unsigned 16 bit intgerer. */
#endif

#ifndef UINT32_T
#define UINT32_T
typedef EE_UINT32 uint32_t;	/**< Unsigned 32 bit intgerer. */
#endif

#ifndef INT8_T
#define INT8_T
typedef EE_INT8 int8_t;		/**< Signed 8 bit intgerer. */
#endif

#ifndef INT16_T
#define INT16_T
typedef EE_INT16 int16_t;	/**< Signed 16 bit intgerer. */
#endif

#ifndef INT32_T
#define INT32_T
typedef EE_INT32 int32_t;	/**< Signed 32 bit intgerer. */
#endif

#else /* __ERIKA__ */

#ifndef UINT8_T
#define UINT8_T
typedef unsigned char uint8_t;		/**< Unsigned 8 bit intgerer. */
#endif

#ifndef UINT16_T
#define UINT16_T
typedef unsigned short int uint16_t;	/**< Unsigned 16 bit intgerer. */
#endif

#ifndef UINT32_T
#define UINT32_T
typedef unsigned long int uint32_t;	/**< Unsigned 32 bit intgerer. */
#endif

#ifndef INT8_T
#define INT8_T
typedef signed char int8_t;		/**< Signed 8 bit intgerer. */
#endif

#ifndef INT16_T
#define INT16_T
typedef signed short int int16_t;	/**< Signed 16 bit intgerer. */
#endif

#ifndef INT32_T
#define INT32_T
typedef signed long int int32_t;	/**< Signed 32 bit intgerer. */
#endif

#endif /* __ERIKA__ */
/**  @} */

/** \brief Compiler inlining keywords. */
#ifndef COMPILER_INLINE 
#ifdef __COSMIC__
#define COMPILER_INLINE	static @inline
#else
#define COMPILER_INLINE	static inline	
#endif
#endif
#endif	/* __compiler_h__ */

