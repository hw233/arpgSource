#ifndef _SVRCORE_UTIL_H_
#define _SVRCORE_UTIL_H_

#include <svrcore-internal.h>

/* Return a random number in the range min..max; (max-min) must be smaller than 32768. */
TEA_SVRCORE_API int32 irand(int32 min,int32 max);

/* Return a random number in the range min..max (inclusive). For reliable results, the difference
* between max and min should be less than RAND32_MAX. */
TEA_SVRCORE_SPEC uint32 urand(uint32 min,uint32 max);

/* Return a random number in the range 0 .. RAND32_MAX. */
TEA_SVRCORE_SPEC int32 rand32();

/* Return a random double from 0.0 to 1.0 (exclusive). Floats support only 7 valid decimal digits.
* A double supports up to 15 valid decimal digits and is used internally (RAND32_MAX has 10 digits).
* With an FPU, there is usually no difference in performance between float and double. */
TEA_SVRCORE_SPEC double rand_norm(void);

/* Return a random double from 0.0 to 99.9999999999999. Floats support only 7 valid decimal digits.
* A double supports up to 15 valid decimal digits and is used internaly (RAND32_MAX has 10 digits).
* With an FPU, there is usually no difference in performance between float and double. */
TEA_SVRCORE_SPEC double rand_chance(void);

#endif

