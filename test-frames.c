#include <sections/text.h>
#include <armv7-arm.h>

#include <stddef.h> // NULL
#include <assert.h>
#include <string.h>

unsigned int id = 0;
uint32_t id_generator() {
	return id++; // Famous dice-roll selection system
};

struct instruction_representation * assert_add_inst
(struct armv7_text_frame * frame)
{
	unsigned int expected_instructions_after = 
		frame->metadata.stored_instructions + 1;
	struct armv7_add_instruction_status status =
		frame_add_instruction(frame);
	
	assert(status.added);
	assert(status.address != NULL);
	assert(
		expected_instructions_after == 
		frame->metadata.stored_instructions
	);
	
	return status.address;
}

void assert_mnemonic_id
(struct instruction_representation * __restrict const inst,
 enum known_instructions mnemonic_id)
{
	instruction_mnemonic_id(inst, mnemonic_id);
	assert(inst->mnemonic_id == mnemonic_id);
}

void assert_mnemonic_args
(struct instruction_representation * __restrict const inst,
 unsigned int const index,
 enum argument_type arg_type,
 uint32_t arg_value)
{
	instruction_arg(inst, index, arg_type, arg_value);
	assert(inst->args[index].type == arg_type);
	assert(inst->args[index].value == arg_value);
}

void test_generate_frame() {
	struct armv7_text_frame * __restrict const frame =
		generate_armv7_text_frame(id_generator);
	struct armv7_text_section * __restrict const section =
		generate_armv7_text_section();
	struct data_section * __restrict const data_section = 
		generate_data_section();
		
	assert(frame != NULL);
	assert(data_section != NULL);
	
	uint8_t const string[] = "Frame test !\n";
	struct data_section_symbol_added status = data_section_add(
		data_section, 4, sizeof(string), "ftest", string
	);
	assert(status.added);
	uint32_t string_id = status.id;
	
	uint32_t const stdout_n = 1;
	uint32_t const write_syscall_n = 4;
	uint32_t const exit_syscall_n  = 1;
	frame_set_name(&frame->metadata, "simple_proc");
	
	struct instruction_representation * inst =
		assert_add_inst(frame);
	
	instruction_mnemonic_id(inst, inst_mov_immediate);
	instruction_arg(inst, 0, arg_register, r0);
	instruction_arg(inst, 1, arg_immediate, stdout_n);

	inst = assert_add_inst(frame);
	instruction_mnemonic_id(inst, inst_mov_immediate);
	instruction_arg(inst, 0, arg_register, r1);
	instruction_arg(inst, 1, arg_data_symbol_address_bottom16, string_id);

	inst = assert_add_inst(frame);
	instruction_mnemonic_id(inst, inst_movt_immediate);
	instruction_arg(inst, 0, arg_register, r1);
	instruction_arg(inst, 1, arg_data_symbol_address_top16, string_id);

	inst = assert_add_inst(frame);
	instruction_mnemonic_id(inst, inst_mov_immediate);
	instruction_arg(inst, 0, arg_register, r2);
	instruction_arg(inst, 1, arg_data_symbol_size, string_id);
	
	inst = assert_add_inst(frame);
	instruction_mnemonic_id(inst, inst_mov_immediate);
	instruction_arg(inst, 0, arg_register, r7);
	instruction_arg(inst, 1, arg_immediate, write_syscall_n);
	
	inst = assert_add_inst(frame);
	instruction_mnemonic_id(inst, inst_svc_immediate);
	
	inst = assert_add_inst(frame);
	instruction_mnemonic_id(inst, inst_mov_immediate);
	instruction_arg(inst, 0, arg_register, r0);
	instruction_arg(inst, 1, arg_immediate, 0);
	
	inst = assert_add_inst(frame);
	instruction_mnemonic_id(inst, inst_mov_immediate);
	instruction_arg(inst, 0, arg_register, r7);
	instruction_arg(inst, 1, arg_immediate, exit_syscall_n);
	
	inst = assert_add_inst(frame);
	instruction_mnemonic_id(inst, inst_svc_immediate);

	data_section_set_base_address(data_section, 0x20098);
	
	uint32_t expected_code[9] = {
		0xe3a00001,
		0xe3a01098,
		0xe3401002,
		0xe3a0200e,
		0xe3a07004,
		0xef000000,
		0xe3a00000,
		0xe3a07001,
		0xef000000
	};
	
	uint32_t produced_code[9];
	
	assert(
		armv7_frame_gen_machine_code(frame, section, data_section, produced_code) ==
		sizeof(expected_code)
	);
	
	assert(
		memcmp(expected_code, produced_code, sizeof(expected_code)) == 0
	);
}

void test_frame_addresses_retrieving() {
	struct armv7_text_frame * __restrict const main_frame =
		generate_armv7_text_frame(id_generator);
	struct armv7_text_frame * __restrict const write_frame =
		generate_armv7_text_frame(id_generator);
	struct armv7_text_frame * __restrict const exit_frame =
		generate_armv7_text_frame(id_generator);
	struct armv7_text_section * __restrict const text_section =
		generate_armv7_text_section();
	struct data_section * __restrict const data_section = 
		generate_data_section();
		
	assert(main_frame != NULL);
	assert(write_frame != NULL);
	assert(exit_frame != NULL);
	assert(data_section != NULL);
	
	uint8_t const string[] = "Hadouken !\n";
	struct data_section_symbol_added status = data_section_add(
		data_section, 4, sizeof(string), "↓↘→+P", string
	);
	assert(status.added);
	uint32_t string_id = status.id;
	
	uint32_t const stdout_n = 1;
	uint32_t const write_syscall_n = 4;
	uint32_t const exit_syscall_n  = 1;
	frame_set_name(&main_frame->metadata, "meynh");
	frame_set_name(&write_frame->metadata, "uraiht");
	frame_set_name(&exit_frame->metadata, "eggzit");
	
	assert(armv7_text_section_add_frame(text_section, main_frame));
	assert(armv7_text_section_add_frame(text_section, write_frame));
	assert(armv7_text_section_add_frame(text_section, exit_frame));
	
	struct instruction_representation * inst =
		assert_add_inst(main_frame);
	instruction_mnemonic_id(inst, inst_bl_address);
	instruction_arg(inst, 0, arg_condition, cond_al);
	instruction_arg(inst, 1, arg_frame_address_pc_relative, write_frame->metadata.id);

	inst = assert_add_inst(main_frame);
	instruction_mnemonic_id(inst, inst_b_address);
	instruction_arg(inst, 0, arg_condition, cond_al);
	instruction_arg(inst, 1, arg_frame_address_pc_relative, exit_frame->metadata.id);

	inst = assert_add_inst(write_frame);
	instruction_mnemonic_id(inst, inst_mov_immediate);
	instruction_arg(inst, 0, arg_register, r0);
	instruction_arg(inst, 1, arg_immediate, stdout_n);
	
	inst = assert_add_inst(write_frame);
	instruction_mnemonic_id(inst, inst_mov_immediate);
	instruction_arg(inst, 0, arg_register, r1);
	instruction_arg(inst, 1, arg_data_symbol_address_bottom16, string_id);
	
	inst = assert_add_inst(write_frame);
	instruction_mnemonic_id(inst, inst_movt_immediate);
	instruction_arg(inst, 0, arg_register, r1);
	instruction_arg(inst, 1, arg_data_symbol_address_top16, string_id);

	inst = assert_add_inst(write_frame);
	instruction_mnemonic_id(inst, inst_mov_immediate);
	instruction_arg(inst, 0, arg_register, r2);
	instruction_arg(inst, 1, arg_data_symbol_size, string_id);
	
	inst = assert_add_inst(write_frame);
	instruction_mnemonic_id(inst, inst_mov_immediate);
	instruction_arg(inst, 0, arg_register, r7);
	instruction_arg(inst, 1, arg_immediate, write_syscall_n);
	
	inst = assert_add_inst(write_frame);
	instruction_mnemonic_id(inst, inst_svc_immediate);
	
	inst = assert_add_inst(write_frame);
	instruction_mnemonic_id(inst, inst_bx_register);
	instruction_arg(inst, 0, arg_condition, cond_al);
	instruction_arg(inst, 1, arg_register, reg_lr);
	
	inst = assert_add_inst(exit_frame);
	instruction_mnemonic_id(inst, inst_mov_immediate);
	instruction_arg(inst, 0, arg_register, r0);
	instruction_arg(inst, 1, arg_immediate, 0);
	
	inst = assert_add_inst(exit_frame);
	instruction_mnemonic_id(inst, inst_mov_immediate);
	instruction_arg(inst, 0, arg_register, r7);
	instruction_arg(inst, 1, arg_immediate, exit_syscall_n);
	
	inst = assert_add_inst(exit_frame);
	instruction_mnemonic_id(inst, inst_svc_immediate);

	data_section_set_base_address(data_section, 0x20098);
	armv7_frame_set_address(main_frame, 0x10070);
	armv7_frame_set_address(write_frame, 0x10080);
	armv7_frame_set_address(exit_frame, 0x11000);
	
	uint32_t expected_main_frame_code[2] = {
		0xeb000003,
		0xea0003e2
	};
	
	uint32_t expected_write_frame_code[7] = {
		0xe3a00001,
		0xe3a01098,
		0xe3401002,
		0xe3a0200c,
		0xe3a07004,
		0xef000000,
		0xe12fff1e
	};
	
	uint32_t expected_exit_frame_code[3] = {
		0xe3a00000,
		0xe3a07001,
		0xef000000
	};
	
	uint32_t produced_code[7];
	
	enum expected_code_test {
		expected_main_code, expected_write_code, expected_exit_code,
		n_expectations
	};
	struct expected_from {
		uint32_t * expected_code;
		size_t code_size;
		struct armv7_text_frame * code_frame;
	} expected_generation[3] = {
		[expected_main_code] = {
			.expected_code = expected_main_frame_code,
			.code_size = sizeof(expected_main_frame_code),
			.code_frame = main_frame
		},
		[expected_write_code] = {
			.expected_code = expected_write_frame_code,
			.code_size = sizeof(expected_write_frame_code),
			.code_frame = write_frame
		},
		[expected_exit_code] = {
			.expected_code = expected_exit_frame_code,
			.code_size = sizeof(expected_exit_frame_code),
			.code_frame = exit_frame
		}
	};
	

	for (enum expected_code_test e = 0; e < n_expectations; e++) {
		assert(
			armv7_frame_gen_machine_code(
				expected_generation[e].code_frame, text_section, data_section,
				produced_code
			) == expected_generation[e].code_size
		);
		assert(
			memcmp(
				expected_generation[e].expected_code,
				produced_code,
				expected_generation[e].code_size
			) == 0
		);
	}
	
	armv7_frame_set_address(write_frame, 0x10000);
	uint32_t expected_new_main_frame_code[2] = {
		0xebffffe3,
		0xea0003e2
	};
	
	assert(
		armv7_frame_gen_machine_code(main_frame, text_section, data_section, produced_code) ==
		sizeof(expected_new_main_frame_code)
	);
	
	assert(
		memcmp(
			expected_new_main_frame_code, produced_code,
			sizeof(expected_new_main_frame_code)
		) == 0
	);
}

int main() {
	test_generate_frame();
	test_frame_addresses_retrieving();
	return 0;
}
