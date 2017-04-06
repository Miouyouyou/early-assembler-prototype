#ifndef MYY_HELPERS_NUMERIC_H
#define MYY_HELPERS_NUMERIC_H 1

#include <stdint.h>

static uint32_t round_to(uint32_t value, uint32_t multiple) {
	uint32_t result = value;
	if (value % multiple) result += (multiple - value % multiple);
	return result;
}

#endif
