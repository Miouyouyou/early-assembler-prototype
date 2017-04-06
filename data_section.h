#ifndef MYY_DATA_SECTION_H
#define MYY_DATA_SECTION_H 1
#include <stdint.h>

struct data_symbol {
	uint32_t id;
	uint32_t align;
	uint32_t size;
	uint8_t * name;
	uint8_t * data;
};

struct data_symbols {
	struct data_symbol * symbols;
	uint32_t stored;
	uint32_t next_id;
	uint32_t base_address;
	uint32_t max_symbols_before_realloc;
};

uint32_t add_data_symbol
(struct data_symbols * __restrict const data_section,
 unsigned int const alignment,
 unsigned int const size,
 uint8_t const * __restrict const name,
 uint8_t const * __restrict const data);

struct uint32_result {
	unsigned int found;
	uint32_t value;
};

struct symbol_found {
	unsigned int found;
	struct data_symbol * address;
};

struct symbol_found get_data_symbol_infos
(struct data_symbols const * __restrict const data_infos,
 uint32_t id);

void exchange_data_symbols
(struct data_symbols * __restrict const data_section,
 unsigned int const id1, unsigned int const id2);

uint32_t write_data_section_content
(struct data_symbols const * __restrict const symbols,
 uint8_t * __restrict const dest);

void delete_data_symbol
(struct data_symbols * __restrict const data_section,
 uint32_t id);

uint32_t data_section_size
(struct data_symbols const * __restrict const data_section);

#define data_address_func_sig struct data_symbols const * __restrict const data_section,\
 uint32_t const data_id
uint32_t data_address(data_address_func_sig);
uint32_t data_address_upper16(data_address_func_sig);
uint32_t data_address_lower16(data_address_func_sig);
uint32_t data_size(data_address_func_sig);

void update_data_symbol
(struct data_symbols * __restrict const data_section,
 uint32_t const id,
 uint32_t const align,
 uint32_t const data_size,
 uint8_t const * __restrict const name,
 uint8_t const * __restrict const data);

#endif
