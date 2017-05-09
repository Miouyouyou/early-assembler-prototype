#ifndef MYY_ASSEMBLER_DUMBELFLIB_H
#define MYY_ASSEMBLER_DUMBELFLIB_H 1

#include <armv7-arm.h>
#include <sections/data.h>

void dumbelflib_build_armv7_program
(struct data_section * __restrict const data_section,
 struct armv7_text_section * __restrict const text_section,
 char const * __restrict const filepath);

#endif
