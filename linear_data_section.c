#include <data_section.h>
#include <armv7-arm.h>
#include <stdint.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define data_address_func_sig struct data_symbols const * __restrict const data_infos,\
 uint32_t const data_index

uint32_t data_address
(data_address_func_sig)
{
	return data_infos->base_address + data_infos->symbols[data_index].offset;
}

uint32_t data_address_upper16
(data_address_func_sig)
{
	return data_address(data_infos, data_index) >> 16;
}

uint32_t data_address_lower16
(data_address_func_sig)
{
	return data_address(data_infos, data_index) & 0xffff;
}

uint32_t add_data_symbol
(struct data_symbols * __restrict const data_infos,
 uint8_t const * __restrict const data,
 unsigned int const size,
 unsigned int const alignment,
 uint8_t * __restrict name)
{
	uint32_t const next_index = data_infos->stored;
	uint32_t const new_id = data_infos->next_id;
	uint32_t const new_data_offset =
		round_to(data_infos->global_size, alignment);

  memcpy(data_infos->data+new_data_offset, data, size);
	data_infos->symbols[next_index].id = new_id;
	data_infos->symbols[next_index].offset = new_data_offset;
	data_infos->symbols[next_index].size = size;
	data_infos->symbols[next_index].align = alignment;
	data_infos->symbols[next_index].name = name;
	data_infos->global_size = new_data_offset + size;
	data_infos->stored += 1;
	data_infos->next_id += 1;
	
	return new_id;
}

static struct uint32_result get_data_symbol_index
(struct data_symbols const * __restrict const data_infos,
 uint32_t id)
{
	unsigned int const n_symbols = data_infos->stored;
	struct data_symbol * __restrict const symbols = data_infos->symbols;
	struct uint32_result returned_result = {
		.found = 0,
		.value = 0
	};

	fprintf(stderr, "Symbols : %d, Requested id : %d\n", n_symbols, id);
	unsigned int s = 0;
	for (; s < n_symbols && symbols[s].id != id; s++) {
		fprintf(stderr, "[%d/%d] Symbol id : %d\n", s, n_symbols, symbols[s].id);
	}
	
	returned_result.found = (s != n_symbols);
	returned_result.value = s;
	
	return returned_result;
}

struct symbol_found get_data_symbol_infos
(struct data_symbols const * __restrict const data_infos,
 uint32_t id)
{

	struct uint32_result results = get_data_symbol_index(data_infos, id);
	struct symbol_found returned_results = {
		.found = results.found,
		.address = data_infos->symbols+results.value,
	};
	
	return returned_results;
}

void delete_data_symbol
(struct data_symbols * __restrict const data_section,
 uint32_t id)
{
	struct uint32_result const index =
		get_data_symbol_index(data_section, id);
	uint32_t const i = index.value;
	uint32_t const next_i = i + 1;
	
	if (index.found == 0) goto unknown_id;

	uint32_t const symbols_metadata_stored = data_section->stored;
	uint32_t const symbols_metadata_tomove =
		symbols_metadata_stored - next_i;
	uint32_t const next_data_offset =
		data_section->symbols[next_i].offset;
	uint32_t const offset_data_to_delete =
		data_section->symbols[i].offset;
	uint32_t const new_offsets_delta =
		next_data_offset - offset_data_to_delete;
	uint32_t const size_of_data_to_move =
		data_section->global_size - next_data_offset;
	uint32_t const stored_symbols_before = data_section->stored;
	
	uint32_t const temporary_buffer_size =
		sizeof(struct data_symbol) * symbols_metadata_tomove;

	struct data_symbols * __restrict const temporary_metadata_buffer =
		(struct data_symbols *) malloc(temporary_buffer_size);

	if (temporary_metadata_buffer == NULL)
		goto cant_allocate_metadata_buffer;

	uint8_t * __restrict const temporary_data_buffer =
		malloc(size_of_data_to_move);

	if (temporary_data_buffer == NULL)
		goto cant_allocate_data_buffer;

	memcpy(
		temporary_metadata_buffer, data_section->symbols+next_i,
		temporary_buffer_size
	);
	memcpy(
		data_section->symbols+i, temporary_metadata_buffer,
		temporary_buffer_size
	);
	
	memcpy(
		temporary_data_buffer, data_section->data+next_data_offset,
		size_of_data_to_move
	);
	memcpy(
		data_section->data+offset_data_to_delete, temporary_data_buffer,
		size_of_data_to_move
	);
	
	data_section->stored -= 1;
	data_section->global_size -= new_offsets_delta;
	
	for (unsigned int s = i; s < stored_symbols_before - 1; s++) {
		data_section->symbols[s].offset -= new_offsets_delta;
	}

	free(temporary_data_buffer);
cant_allocate_data_buffer:
	free(temporary_metadata_buffer);
cant_allocate_metadata_buffer:
unknown_id:
	return;
}

static void exchange_adjacent_symbols
(struct data_symbols * __restrict const data_section,
 unsigned int const current_first_index,
 unsigned int const current_second_index)
{
	unsigned int const alignment = 4;
	struct data_symbol current_first =
		data_section->symbols[current_first_index];
		
	struct data_symbol current_second =
		data_section->symbols[current_second_index];

	unsigned int
		realignment_needed = (current_second.size % alignment),
		realignment_delta = (realignment_needed ? 
			(alignment - realignment_needed) :
			realignment_needed),
		new_second_offset = current_second.offset + realignment_delta,
		copy_begin = current_first.offset,
		copy_end   = current_second.offset + current_second.size,
		temp_buffer_size = copy_end - copy_begin;
	
	uint8_t * __restrict const temp_data_buffer =
		malloc(temp_buffer_size);
	if (temp_data_buffer == NULL) goto cant_allocate_temp_data_buffer;
	uint8_t * __restrict const current_data = data_section->data;
	
	memcpy(
		temp_data_buffer, current_data+copy_begin, temp_buffer_size
	);
	
	memset(current_data+copy_begin, 0, temp_buffer_size);
	
	memcpy(
		current_data+current_first.offset,
		temp_data_buffer+current_second.offset,
		current_second.size
	);

	memcpy(
		current_data+new_second_offset,
		temp_data_buffer+current_first.offset,
		current_first.size
	);
	
	free(temp_data_buffer);
	
	data_section->global_size += realignment_delta;
	current_second.offset = new_second_offset;
	
	struct data_symbol new_adjacent_symbols[] = {
		current_second, current_first
	};
	
	memcpy(
		data_section->symbols+current_first_index, new_adjacent_symbols,
		2*sizeof(struct data_symbol)
	);
	
cant_allocate_temp_data_buffer:
	return;
}

static unsigned int realign_symbols_and_data_between
(struct data_symbols * __restrict const data_section,
 uint8_t const * __restrict const temp_data,
 unsigned int const temp_data_offset_difference,
 unsigned int data_cursor,
 unsigned int const start_index,
 unsigned int const end_index)
{
	for (unsigned int s = start_index; s < end_index; s++) {
		struct data_symbol sym_metadata = data_section->symbols[s];
		unsigned int temp_data_offset =
			sym_metadata.offset - temp_data_offset_difference;
		data_cursor = round_to(data_cursor, sym_metadata.align);
		memcpy(
			data_section->data+data_cursor,
			temp_data+temp_data_offset,
			sym_metadata.size
		);
		data_section->symbols[s].offset = data_cursor;
		data_cursor += sym_metadata.size;
	}
	return data_cursor;
}

static void exchange_separated_symbols
(struct data_symbols * __restrict const data_section,
 unsigned int const current_first_index,
 unsigned int const current_second_index)
{
	
	struct data_symbol const first = 
		data_section->symbols[current_first_index];
	struct data_symbol const second = 
		data_section->symbols[current_second_index];
	
	unsigned int const 
		copy_begin = first.offset,
		copy_end   = second.offset + second.size,
		temp_data_buffer_size = copy_end - copy_begin;
		
	uint8_t * __restrict const temp_data_buffer =
		malloc(temp_data_buffer_size);
	
	if (temp_data_buffer == NULL) goto cant_allocate_temp_data_buffer;
	
	uint8_t * __restrict const current_data = data_section->data;
	
	memcpy(
		temp_data_buffer, current_data+copy_begin,
		temp_data_buffer_size
	);
	
	unsigned int new_data_cursor = copy_begin;
	
	memset(current_data+new_data_cursor, 0, temp_data_buffer_size);
	
	memcpy(
		current_data+new_data_cursor, temp_data_buffer+second.offset,
		second.size
	);
	
	new_data_cursor += second.size;
	
	new_data_cursor = realign_symbols_and_data_between(
		data_section,
		temp_data_buffer,
		copy_begin,
		new_data_cursor,
		current_first_index+1,
		current_second_index
	);
	
	new_data_cursor = round_to(new_data_cursor,first.align);
	uint32_t second_element_offset = new_data_cursor;
	memcpy(
		current_data+new_data_cursor,
		temp_data_buffer+first.offset,
		first.size
	);
	
	new_data_cursor += first.size;
	
	free(temp_data_buffer);
	
	data_section->global_size = new_data_cursor;
	data_section->symbols[current_first_index] = second;
	data_section->symbols[current_second_index] = first;
	data_section->symbols[current_first_index].offset = copy_begin;
	data_section->symbols[current_second_index].offset = second_element_offset;
	
cant_allocate_temp_data_buffer:
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

	uint32_t begin_i;
	uint32_t end_i;
	
	if (symbols_metadata[first_index.value].offset < 
			symbols_metadata[second_index.value].offset) {
		begin_i = first_index.value;
		end_i = second_index.value;
	}
	else {
		begin_i = second_index.value;
		end_i = first_index.value;
	}
	
	exchange_separated_symbols(data_section, begin_i, end_i);

id_not_found:
nothing_to_do:
	return;
}

uint32_t data_size
(data_address_func_sig)
{
	return data_infos->global_size;
}

uint32_t write_data_section_content
(struct data_symbols const * __restrict const symbols,
 uint8_t * __restrict const dest)
{
	memcpy(dest, symbols->data, symbols->global_size);
	return symbols->global_size;
}
