#include <stdio.h>
#include <stdint.h>

#include <string.h> // memset

#include <armv7-arm.h>
#include <sections/data.h>
// Stub of a dumb mnemonics to machine code library



uint8_t msg[] = "Hellow Meow !\n";
uint32_t msg_size = sizeof(msg);

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
	[inst_mov_immediate] = {one_reg_one_immediate,  "mov %s, #%d\n"},
	[inst_mov_register]  = {two_regs,               "mov %s, %s\n"},
	[inst_movt_immediate] = {one_reg_one_immediate, "movt %s, #%d\n"},
	[inst_movw_immediate] = {one_reg_one_immediate, "movw %s, #%d\n"},
	[inst_mvn_immediate] = {one_reg_one_immediate,  "mvn %s, %d\n"},
	[inst_sub_immediate] = {two_regs_one_immediate, "sub %s, %s, #%d\n"},
	[inst_svc_immediate] = {one_immediate,          "svc #%d\n"}
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
			instructions_string_conversions[current_instruction.mnemonic_id];

		stored_chars += to_string_infos.tostring_func(
			to_string_infos.format,
			output+stored_chars,
			output_max_size - stored_chars,
			current_instruction.args
		);
	}
	return stored_chars;
}

#define N_TESTS_INST 6

uint8_t test_data[1000] = {0};

static struct data_symbol test_symbols[10] = {0};
static struct data_section test_data_section = {
	.symbols = test_symbols,
	.stored = 0,
	.base_address = 0x20094,
};

static uint8_t test_data_string[] = "Meow world !\n";
static uint8_t test_data_string_name[] = "meow";

void test_instructions_to_string() {
	/*struct instruction_representation converted[N_TESTS_INST] = {0};
	struct instructions insts = {
		.n = 0,
		.converted = converted
	};
	
	printf("Got there !\n");
	
	add_instruction(&insts, inst_mov_immediate, r0, 1,      0);
	add_instruction(&insts, inst_movw_immediate, r1, 0, 0);
	add_instruction(&insts, inst_movt_immediate, r1, 0, 0);
	add_instruction(&insts, inst_mov_immediate, r2, 10, 0);
	add_instruction(&insts, inst_mov_immediate, r7, 4, 0);
	add_instruction(&insts, inst_svc_immediate, 0,0,0);
	
	uint32_t data_index = add_data_symbol(
		&test_data_section, test_data_string, sizeof(test_data_string),
		test_data_string_name
	);
	
	converted[1].args[1].type = arg_data_symbol_address_bottom16;
	converted[1].args[1].value = data_index;
	converted[2].args[1].type = arg_data_symbol_address_top16;
	converted[2].args[1].value = data_index;
	converted[3].args[1].type = arg_data_symbol_size;
	converted[3].args[1].value = data_index;
	
	char string[200];
	memset(string, 0, 200);
	
	instructions_to_string(converted, N_TESTS_INST, string, 200);
	printf("%s\n", string);
	
	uint32_t machine_code[N_TESTS_INST] = {0};
	
	assemble_code(&test_data_section, converted, N_TESTS_INST, machine_code);
	for (unsigned int i = 0; i < N_TESTS_INST; i++) {
		printf("%x\n", machine_code[i]);
	}
	printf("%d\n", instructions_size(&insts));*/
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
