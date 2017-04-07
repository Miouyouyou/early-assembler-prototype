#include <stdlib.h>
#include <stdint.h>

#include <helpers/numeric.h>

void * allocate_durable_memory(unsigned int const bytes_amount)
{
	return malloc(bytes_amount);
}

void * reallocate_durable_memory
(void * __restrict const addr, unsigned int const bytes_amount)
{
	return realloc(addr, bytes_amount);
}

void free_durable_memory(void * __restrict const addr)
{
	free(addr);
	return;
}


void * allocate_temporary_memory(unsigned int const bytes_amount)
{
	return malloc(bytes_amount);
}

void * reallocate_temporary_memory
(void * __restrict const addr, unsigned int const bytes_amount)
{
	return realloc(addr, bytes_amount);
}

void free_temporary_memory(void * __restrict const addr)
{
	free(addr);
	return;
}
