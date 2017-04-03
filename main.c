#include <stdio.h>
#include <stdint.h>

#include <string.h> // memset
// Stub of a dumb mnemonics to machine code library

enum arm_register {
	r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15,
	reg_ip = 12, reg_sp = 13, reg_lr = 14, reg_pc = 15
};
enum arm_conditions {
	cond_eq, cond_ne, cond_cs, cond_cc, cond_mi, cond_pl,
	cond_vs, cond_vc, cond_hi, cond_ls, cond_ge, cond_lt,
	cond_gt, cond_le, cond_al
};
enum known_instructions {
	inst_add_immediate,
	inst_b_address,
	inst_bl_address,
	inst_blx_address,
	inst_ldr_immediate,
	inst_mov_immediate,
	inst_mov_register,
	inst_mvn_immediate,
	inst_pop_regmask,
	inst_push_regmask,
	inst_sub_immediate,
	inst_svc_immediate,
	n_known_instructions
};


enum parameter_type {
	arg_register,
	arg_immediate,
	arg_address
};

struct parameters {
	enum parameter_type type;
	uint32_t restriction;
};

uint8_t msg[] = "Hellow Meow !\n";
uint32_t msg_size = sizeof(msg);
#define MAX_ARGS 3
struct instruction_parameters {
	struct parameters params[MAX_ARGS];
}
instruction_params[n_known_instructions] = {
	[inst_add_immediate] = {
		.params = {
			{.type = arg_register},
			{.type = arg_register},
			{.type = arg_immediate, .restriction = 12}
		},
	},
	[inst_ldr_immediate] = {
		.params = {
			{.type = arg_register},
			{.type = arg_address}
		},
	},
	[inst_mov_immediate] = {
		.params = {
			{.type = arg_register},
			{.type = arg_immediate, .restriction = 12}
		},
	},
	[inst_mvn_immediate] = {
		.params = {
			{.type = arg_register},
			{.type = arg_immediate, .restriction = 12}
		},
	},
	[inst_sub_immediate] = {
		.params = {
			{.type = arg_register},
			{.type = arg_immediate, .restriction = 12}
		},
	},
	[inst_svc_immediate] = {
		.params = {
			{.type = arg_register},
			{.type = arg_immediate, .restriction = 24}
		}
	}
};

/* Frame doit contenir
 * - Noms de symboles (Pointeurs)
 * - Code interprété (bytecode)
 * - Données locales
 * - Noms de symboles externes référencés
 */

/* Manque :
 * B, BL, BLX
 * PUSH, POP
 */

struct instruction_args_infos {
	uint32_t value;
};




struct instruction_representation {
	enum known_instructions id;
	struct instruction_args_infos args[MAX_ARGS];
};

char * register_names[] = {
	[r0]  = "r0",
	[r1]  = "r1",
	[r2]  = "r2",
	[r3]  = "r3",
	[r4]  = "r4",
	[r5]  = "r5",
	[r6]  = "r6",
	[r7]  = "r7",
	[r8]  = "r8",
	[r9]  = "r9",
	[r10] = "r10",
	[r11] = "r11",
	[r12] = "ip",
	[r13] = "sp",
	[r14] = "lr",
	[r15] = "pc"
};

#define tostring_func_sig char const * __restrict const format,\
 char * __restrict const string,\
 size_t const max_length,\
 struct instruction_args_infos const * __restrict const args

size_t one_reg_one_immediate(tostring_func_sig)
{
	return snprintf(
		string, max_length, format,
		register_names[args[0].value], args[1].value
	);
}

size_t two_regs_one_immediate(tostring_func_sig) {
	return snprintf(
		string, max_length, format,
		register_names[args[0].value], register_names[args[1].value],
		args[2].value
	);
}

size_t two_regs(tostring_func_sig) {
	return snprintf(
		string, max_length, format,
		register_names[args[0].value], register_names[args[1].value]
	);
}

size_t one_immediate(tostring_func_sig) {
	return snprintf(string, max_length, format, args[0].value);
}

struct instruction_to_string {
	size_t (* const tostring_func)(tostring_func_sig);
	char const * const format;
}
instructions_string_conversions[n_known_instructions] = {
	[inst_add_immediate] = {two_regs_one_immediate, "add %s, %s, #%d\n"},
	[inst_ldr_immediate] = {one_reg_one_immediate,  "ldr %s, #%d\n"},
	[inst_mov_immediate] = {one_reg_one_immediate,  "mov %s, #%d\n"},
	[inst_mov_register]  = {two_regs,               "mov %s, %s\n"},
	[inst_mvn_immediate] = {one_reg_one_immediate,  "mvn %s, %d\n"},
	[inst_sub_immediate] = {two_regs_one_immediate, "sub %s, %s, #%d\n"},
	[inst_svc_immediate] = {one_immediate,          "svc #%d\n"}
};


struct instructions {
	unsigned int n_instructions;
	struct instruction_representation converted[];
};

size_t instructions_to_string
(struct instruction_representation const * __restrict const instructions,
 unsigned int n_instructions,
 char * __restrict const output, size_t const output_max_size)
{
	unsigned int stored_chars = 0;
	for (unsigned int i = 0;
	     i < n_instructions && stored_chars < output_max_size; i++) {
		struct instruction_representation const current_instruction =
			instructions[i];
		
		struct instruction_to_string to_string_infos =
			instructions_string_conversions[current_instruction.id];

		stored_chars += to_string_infos.tostring_func(
			to_string_infos.format,
			output+stored_chars,
			output_max_size - stored_chars,
			current_instruction.args
		);
	}
	return stored_chars;
}

void set_instruction
(struct instruction_representation * __restrict const instruction,
 enum known_instructions id, uint32_t const val0, uint32_t const val1,
 uint32_t const val2)
{
	instruction->id = id;
	instruction->args[0].value = val0;
	instruction->args[1].value = val1;
	instruction->args[2].value = val2;
}

void test_instructions_to_string() {
	struct instruction_representation instructions[5] = {0};
	
	printf("Got there !\n");
	
	set_instruction(instructions+0, inst_mov_immediate, r0, 1,      0);
	set_instruction(instructions+1, inst_ldr_immediate, r1, 0x1000, 0);
	set_instruction(instructions+2, inst_mov_immediate, r2, 10, 0);
	set_instruction(instructions+3, inst_mov_immediate, r7, 4, 0);
	set_instruction(instructions+4, inst_svc_immediate, 0,0,0);
	
	char string[200];
	memset(string, 0, 200);
	
	instructions_to_string(instructions, 5, string, 200);
	printf("%s\n", string);
}

typedef int32_t immediate;

static uint32_t to_positive(immediate value) {
	return ~value + 1;
}

typedef unsigned int address;

uint32_t op_add_immediate
(enum arm_register dest, enum arm_register op1, immediate op2);
uint32_t op_b_address(enum arm_conditions condition, address imm24);
uint32_t op_bl_address(enum arm_conditions condition, address addr24);
uint32_t op_blx_address(address addr24);
uint32_t op_blx_register
(enum arm_conditions condition, enum arm_register addr_reg);
uint32_t op_mov_immediate(enum arm_register dest, immediate value);
uint32_t op_mov_register(enum arm_register dest, enum arm_register src);
uint32_t op_mvn_immediate(enum arm_register dest, immediate value);
uint32_t op_ldr_immediate(enum arm_register dest, immediate pc_offset);
uint32_t op_pop_immediate_list
(enum arm_conditions condition, uint16_t reglist);
uint32_t op_push_immediate_list
(enum arm_conditions condition, uint16_t reglist);
uint32_t op_sub_immediate
(enum arm_register dest, enum arm_register op1, immediate op2);

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
(enum arm_conditions condition, uint16_t reglist)
{
	uint32_t const cond          = clamp_condition(condition) << 28;
	uint32_t const fixed_part    = 0b100100101101 << 16;
	uint32_t const register_list = clamp_reglist(reglist);
	
	return cond | fixed_part | register_list;
}

uint32_t op_pop_immediate_list
(enum arm_conditions condition, uint16_t reglist)
{
	uint32_t const cond = clamp_condition(condition) << 28;
	uint32_t const fixed_part = 0b100010111101 << 16;
	uint32_t const register_list = clamp_reglist(reglist);
	
	return cond | fixed_part | register_list;
}

static address get_global_data_address
(unsigned const int data_id)
{
	return 0x2000;
}

void generate_machine_code
(struct instructions const * __restrict const instructions)
{
	
}

int main()
{
	printf("%x\n", op_movt_immediate(r1, 2));
	
	test_instructions_to_string();
  return 0;
}
