#include <armv7-arm.h>

static uint32_t to_positive(immediate value) {
	return ~value + 1;
}


static inline enum arm_conditions clamp_condition
(enum arm_conditions condition)
{
	return (condition & 0xf);
}

static inline enum arm_register clamp_standard_register
(enum arm_register reg)
{
	return (reg & 0xf);
}

static inline uint32_t clamp_reglist(uint32_t reglist)
{
	return (reglist & 0xffff);
}

static uint32_t branch_instruction
(enum arm_conditions condition, address addr24,
 uint32_t fixed_part_bits)
{
	uint32_t cond       = clamp_condition(condition) << 28;
	uint32_t fixed_part = (fixed_part_bits) << 24;
	uint32_t imm24      = addr24 & 0xffffff;
	
	return cond | fixed_part | imm24;
}


uint32_t op_b_address(enum arm_conditions condition, address addr24)
{
	return branch_instruction(condition, addr24, 0b1010);
}

uint32_t op_bl_address(enum arm_conditions condition, address addr24)
{
	return branch_instruction(condition, addr24, 0b1011);
}

uint32_t op_blx_address(address addr24)
{
	uint32_t fixed_part = 0b1111101 << 25;
	uint32_t h          = addr24 & 1 << 24;
	uint32_t imm24      = ((addr24 >> 1) & 0xffffff);

	return fixed_part | h | imm24;
}

uint32_t op_blx_register
(enum arm_conditions condition, enum arm_register addr_reg)
{
	uint32_t cond       = clamp_condition(condition) << 28;
	uint32_t fixed_part = 0b000100101111111111110011 << 4;
	uint32_t rm         = clamp_standard_register(addr_reg);
	
	return cond | fixed_part | rm;
}

uint32_t op_mov_immediate(enum arm_register dest, immediate value)
{
	if (value >= 0) {
		uint32_t const cond  = cond_al << 28;
		uint32_t const fixed_part = 0b001110100000 << 16;
		uint32_t const rd = dest << 12;
		uint32_t const imm12 = (value & 0xfff); // imm12 = 12 bits max
		return cond | fixed_part | rd | imm12;
	}
	else return op_mvn_immediate(dest, ~value);
}

uint32_t op_mvn_immediate(enum arm_register dest, immediate value)
{
	if (value >= 0) {
		uint32_t const cond  = cond_al << 28;
		uint32_t const fixed_part = 0b001111100000 << 16;
		uint32_t const rd = dest << 12;
		uint32_t const imm12 = (value & 0xfff); // imm12 = 12 bits max
		return cond | fixed_part | rd | imm12;
	}
	else return op_mov_immediate(dest, ~value);
}

uint32_t op_mov_register(enum arm_register dest, enum arm_register src)
{
	uint32_t const cond = cond_al << 28;
	uint32_t const fixed_part = 0b000110100000 << 16;
	uint32_t const rd = dest << 12;
	uint32_t const rm = src;
	return cond | fixed_part | rd | rm;
}

uint32_t op_movw_immediate(enum arm_register dest, immediate value)
{
	uint32_t const cond = cond_al << 28;
	uint32_t const fixed_part = 0b00110000 << 20;

	uint32_t const imm4 = (value & 0xf000) << 16;
	uint32_t const rd   = clamp_standard_register(dest) << 12;
	uint32_t const imm12 = value & 0xfff;
	
	return cond | fixed_part | imm4 | rd | imm12;
}

uint32_t op_movt_immediate(enum arm_register dest, immediate value)
{
	uint32_t const cond = cond_al << 28;
	uint32_t const fixed_part = 0b00110100 << 20;

	uint32_t const imm4 = (value & 0xf000) << 16;
	uint32_t const rd   = clamp_standard_register(dest) << 12;
	uint32_t const imm12 = value & 0xfff;
	
	return cond | fixed_part | imm4 | rd | imm12;
}

uint32_t op_ldr_register(enum arm_register dest, immediate pc_offset)
{
	uint32_t const cond = cond_al << 28;
	uint32_t const first_fixed_part = 0b0101 << 24;
	uint32_t const positive = (pc_offset > 0) << 23;
	uint32_t const second_fixed_part = 0b0011111 << 16;
	uint32_t const rt = dest << 12;
	
	uint32_t positive_offset = pc_offset;
	if (!positive) positive_offset = to_positive(pc_offset);
	
	uint32_t const imm12 = positive_offset & 0xfff; // imm12 = 12 bits max
	return cond | first_fixed_part | positive | second_fixed_part | rt | imm12;
}

uint32_t op_sub_immediate
(enum arm_register dest, enum arm_register op1, immediate op2)
{
	if (op2 >= 0) {
		uint32_t const cond = cond_al << 28;
		uint32_t const fixed_part = 0b00100100 << 20;
		uint32_t const rn = op1 << 16;
		uint32_t const rd = dest << 12;
		uint32_t const imm12 = op2 & 0xfff;
		return cond | fixed_part | rn | rd | imm12;
	}
	else return op_add_immediate(dest, op1, to_positive(op2));
}

uint32_t op_add_immediate
(enum arm_register dest, enum arm_register op1, immediate op2)
{
	if (op2 >= 0) {
		uint32_t const cond = cond_al << 28;
		uint32_t const fixed_part = 0b00101000 << 20;
		uint32_t const rn = op1 << 16;
		uint32_t const rd = dest << 12;
		uint32_t const imm12 = op2 & 0xfff;
		return cond | fixed_part | rn | rd | imm12;
	}
	else return op_sub_immediate(dest, op1, to_positive(op2));
}

uint32_t op_push_immediate_list
(enum arm_conditions condition, uint32_t reglist)
{
	uint32_t const cond          = clamp_condition(condition) << 28;
	uint32_t const fixed_part    = 0b100100101101 << 16;
	uint32_t const register_list = clamp_reglist(reglist);
	
	return cond | fixed_part | register_list;
}

uint32_t op_pop_immediate_list
(enum arm_conditions condition, uint32_t reglist)
{
	uint32_t const cond = clamp_condition(condition) << 28;
	uint32_t const fixed_part = 0b100010111101 << 16;
	uint32_t const register_list = clamp_reglist(reglist);
	
	return cond | fixed_part | register_list;
}

uint32_t op_svc_immediate(immediate value)
{
	uint32_t const cond       = cond_al << 28;
	uint32_t const fixed_part = 0b1111 << 24;
	uint32_t const imm24      = value & 0xffffff;
	return cond | fixed_part | imm24;
}

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

#include <string.h> // memcpy
#include <stdio.h>

uint32_t round_to(uint32_t value, uint32_t multiple)
{
	uint32_t result = value;
	if (value % multiple) result += (multiple - value % multiple);
	return result;
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

#include <stdlib.h>

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


struct args_values { unsigned int val0, val1, val2; };

struct args_values get_values
(struct data_symbols const * __restrict const symbols,
 struct instruction_args_infos const * __restrict const args)
{
	
	uint32_t values[MAX_ARGS];
	for (unsigned int a = 0; a < MAX_ARGS; a++) {
		uint32_t set_value = args[a].value;
		switch(args[a].type) {
			case arg_register:
			case arg_immediate:
			case arg_address:
				values[a] = set_value;
				break;
			case arg_data_symbol_address:
				values[a] = data_address(symbols, set_value);
				break;
			case arg_data_symbol_address_top16:
				values[a] = data_address_upper16(symbols, set_value);
				break;
			case arg_data_symbol_address_bottom16:
				values[a] = data_address_lower16(symbols, set_value);
				break;
			case arg_data_symbol_size:
				values[a] = data_size(symbols, set_value);
				break;
		}
	}
	
	struct args_values vals = {
		.val0 = values[0],
		.val1 = values[1],
		.val2 = values[2]
	};
	
	return vals;
}

/*
 * enum known_instructions {
	inst_add_immediate,
	inst_b_address,
	inst_bl_address,
	inst_blx_address,
	inst_mov_immediate,
	inst_mov_register,
	inst_mvn_immediate,
	inst_pop_regmask,
	inst_push_regmask,
	inst_sub_immediate,
	inst_svc_immediate,
	n_known_instructions
};*/

uint32_t (*op_functions[n_known_instructions])() = {
	[inst_add_immediate] = op_add_immediate,
	[inst_b_address]     = op_b_address,
	[inst_bl_address]    = op_bl_address,
	[inst_blx_address]   = op_blx_address,
	[inst_mov_immediate] = op_mov_immediate,
	[inst_mov_register]  = op_mov_register,
	[inst_movt_immediate] = op_movt_immediate,
	[inst_movw_immediate] = op_movw_immediate,
	[inst_mvn_immediate] = op_mvn_immediate,
	[inst_pop_regmask]   = op_pop_immediate_list,
	[inst_push_regmask]  = op_push_immediate_list,
	[inst_sub_immediate] = op_sub_immediate,
	[inst_svc_immediate] = op_svc_immediate
};

uint32_t assemble_code
(struct data_symbols const * __restrict const data_infos,
 struct instructions const * __restrict const instructions,
 uint32_t * result_code)
{
	unsigned int n_instructions = instructions->n;
	struct instruction_representation * __restrict const internal_insts =
		instructions->converted;
	for (unsigned int i = 0; i < n_instructions; i++) {
		struct args_values values = 
			get_values(data_infos, internal_insts[i].args);
		result_code[i] = op_functions[internal_insts[i].id](
			values.val0, values.val1, values.val2
		);
	}
	return n_instructions * sizeof(uint32_t);
}

uint32_t add_instruction
(struct instructions * __restrict const instructions,
 enum known_instructions id, uint32_t const val0, uint32_t const val1,
 uint32_t const val2)
{
	uint32_t new_index = instructions->n;
	instructions->n++;
	instructions->converted[new_index].id = id;
	instructions->converted[new_index].args[0].value = val0;
	instructions->converted[new_index].args[1].value = val1;
	instructions->converted[new_index].args[2].value = val2;
	return new_index;
}

uint32_t instructions_size
(struct instructions const * __restrict const instructions)
{
	return instructions->n * 4;
}
