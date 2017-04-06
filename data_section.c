#include <data_section.h>
#include <armv7-arm.h>
#include <stdint.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <helpers/numeric.h>

struct uint32_result get_data_symbol_index
(struct data_symbols const * __restrict const data_section,
 uint32_t id)
{
	unsigned int const n_symbols = data_section->stored;
	struct data_symbol * __restrict const symbols = data_section->symbols;
	struct uint32_result returned_result = {
		.found = 0,
		.value = 0
	};

	unsigned int s = 0;
	while(s < n_symbols && symbols[s].id != id) s++;
	
	returned_result.found = (s != n_symbols);
	returned_result.value = s;
	
	return returned_result;
}

uint32_t data_address_upper16
(data_address_func_sig)
{
	return data_address(data_section, data_id) >> 16;
}

uint32_t data_address_lower16
(data_address_func_sig)
{
	return data_address(data_section, data_id) & 0xffff;
}

uint32_t add_data_symbol
(struct data_symbols * __restrict const data_section,
 unsigned int const alignment,
 unsigned int const size,
 uint8_t const * __restrict const name,
 uint8_t const * __restrict const data)
{
	uint32_t const next_index = data_section->stored;
	uint32_t const new_id = data_section->next_id;

	data_section->symbols[next_index].id = new_id;
	data_section->symbols[next_index].align = alignment;
	data_section->symbols[next_index].size = size;
	data_section->symbols[next_index].name = name;
	data_section->symbols[next_index].data = data;

	data_section->stored += 1;
	data_section->next_id += 1;
	
	return new_id;
}

struct symbol_found get_data_symbol_infos
(struct data_symbols const * __restrict const data_section,
 uint32_t id)
{

	struct uint32_result results = get_data_symbol_index(data_section, id);
	struct symbol_found returned_results = {
		.found = results.found,
		.address = data_section->symbols+results.value,
	};
	
	return returned_results;
}

void update_data_symbol
(struct data_symbols * __restrict const data_section,
 uint32_t const id,
 uint32_t const align,
 uint32_t const data_size,
 uint8_t const * __restrict const name,
 uint8_t const * __restrict const data)
{
	struct symbol_found metadata =
		get_data_symbol_infos(data_section, id);
	
	if (metadata.found) {
		struct data_symbol * __restrict const symbol = metadata.address;
		symbol->align = align;
		symbol->size = data_size;
		symbol->name = name;
		symbol->data = data;
	}
}

void delete_data_symbol
(struct data_symbols * __restrict const data_section,
 uint32_t id)
{
	struct uint32_result const index =
		get_data_symbol_index(data_section, id);
	if (!index.found) goto id_not_found;

	uint32_t const symbols_stored = data_section->stored;
	uint32_t const next_index = index.value + 1;
	uint32_t const remaining_indices_after = symbols_stored - next_index;
	uint32_t const remaining_metadata_size =
		remaining_indices_after * sizeof(struct data_symbol);
	
	struct data_symbol * __restrict const temp_symbols_buffer =
		malloc(remaining_metadata_size);
	
	if (temp_symbols_buffer == NULL)
		goto cant_allocate_temp_symbols_buffer;
		
	memcpy(
		temp_symbols_buffer, data_section->symbols+next_index,
		remaining_metadata_size
	);
	
	memcpy(
		data_section->symbols+index.value, temp_symbols_buffer,
		remaining_metadata_size
	);
	
	free(temp_symbols_buffer);
	
	data_section->stored -= 1;
	
cant_allocate_temp_symbols_buffer:
id_not_found:
	return;
}

void exchange_data_symbols
(struct data_symbols * __restrict const data_section,
 unsigned int const id1, unsigned int const id2)
{

	if (id1 == id2) goto nothing_to_do;
	struct uint32_result const first_index = get_data_symbol_index(
		data_section, id1
	);
	struct uint32_result const second_index = get_data_symbol_index(
		data_section, id2
	);
	if ((first_index.found & second_index.found) == 0)
		goto id_not_found;
	
	struct data_symbol * __restrict const symbols_metadata =
		data_section->symbols;

	struct data_symbol temp = symbols_metadata[first_index.value];
	symbols_metadata[first_index.value] = 
		symbols_metadata[second_index.value];
	symbols_metadata[second_index.value] = temp;
	
id_not_found:
nothing_to_do:
	return;
}

static uint32_t data_size_up_to
(struct data_symbols const * __restrict const data_section,
 uint32_t const symbol_index)
{
	uint32_t global_size = data_section->base_address;
	
	for (unsigned int s = 0; s < symbol_index; s++) {
		struct data_symbol const * __restrict const symbol =
			data_section->symbols+s;
		global_size = round_to(global_size, symbol->align);
		global_size += symbol->size;
	}
	return global_size - data_section->base_address;
}

uint32_t data_address
(data_address_func_sig)
{
	struct uint32_result const index = 
		get_data_symbol_index(data_section, data_id);
	uint32_t address = 0;
	if (index.found) {
		address += data_section->base_address;
		
		address += data_size_up_to(data_section, index.value);
		
		address = 
			round_to(address, data_section->symbols[index.value].align);
	}
	return address;
}

uint32_t data_size
(data_address_func_sig)
{
	uint32_t size = 0;
	
	struct symbol_found symbol = 
		get_data_symbol_infos(data_section, data_id);
	if (symbol.found) size = symbol.address->size;
	return size;
}

uint32_t data_section_size
(struct data_symbols const * __restrict const data_section)
{
	return data_size_up_to(data_section, data_section->stored);
}


uint32_t write_data_section_content
(struct data_symbols const * __restrict const data_section,
 uint8_t * __restrict const dest)
{
	unsigned int cursor = 0;
	for (unsigned int s = 0; s < data_section->stored; s++) {
		struct data_symbol const * __restrict const symbol =
			data_section->symbols+s;
		cursor = round_to(cursor, symbol->align);
		memcpy(dest+cursor, symbol->data, symbol->size);
		cursor += symbol->size;
	}

	return cursor;
}
