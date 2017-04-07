#ifndef MYY_HELPERS_MEMORY_H
#define MYY_HELPERS_MEMORY_H 1

void * allocate_durable_memory(unsigned int const bytes_amount);
void * reallocate_durable_memory
(void * __restrict const addr, unsigned int const bytes_amount);
void free_durable_memory(void * __restrict const addr);

void * allocate_temporary_memory(unsigned int const bytes_amount);
void * reallocate_temporary_memory
(void * __restrict const addr, unsigned int const bytes_amount);
void free_temporary_memory(void * __restrict const addr);

#endif
