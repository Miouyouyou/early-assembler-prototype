#include <armv7-arm.h>
#include <data_section.h>
#include <helpers/numeric.h>

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
