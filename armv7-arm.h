#ifndef MYY_ARMV7_ARM_H
#define MYY_ARMV7_ARM_H 1

#include <stdint.h>
#include <sections/data.h>
#include <sections/text.h>

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
	inst_blx_register,
	inst_bx_register,
	inst_mov_immediate,
	inst_mov_register,
	inst_movt_immediate,
	inst_movw_immediate,
	inst_mvn_immediate,
	inst_pop_regmask,
	inst_push_regmask,
	inst_sub_immediate,
	inst_svc_immediate,
	n_known_instructions
};

enum argument_type {
	arg_invalid,
	arg_condition,
	arg_register,
	arg_immediate,
	arg_address,
	arg_data_symbol_address,
	arg_data_symbol_address_top16,
	arg_data_symbol_address_bottom16,
	arg_data_symbol_size,
	arg_frame_address,
	arg_frame_address_pc_relative,
};

struct parameters {
	enum argument_type type;
	uint32_t restriction;
};

#define MAX_ARGS 3

struct instruction_args_infos {
	enum argument_type type;
	int32_t value;
};

struct instruction_representation {
	enum known_instructions mnemonic_id;
	struct instruction_args_infos args[MAX_ARGS];
};

struct instructions {
	unsigned int n;
	struct instruction_representation * converted;
};

typedef unsigned int address;
typedef int32_t relative_address;
typedef int32_t immediate;

struct armv7_text_frame {
	struct text_frame_metadata metadata;
	struct instruction_representation * instructions;
};

struct armv7_text_section {
	uint32_t id;
	uint32_t n_frames_refs;
	uint32_t max_frames_refs;
	uint32_t base_address;
	struct armv7_text_frame ** frames_refs;
};

uint32_t op_add_immediate
(enum arm_register dest, enum arm_register op1, immediate op2);
uint32_t op_b_address(enum arm_conditions condition, relative_address imm24);
uint32_t op_bl_address(enum arm_conditions condition, relative_address addr24);
uint32_t op_blx_address(relative_address addr24);
uint32_t op_blx_register
(enum arm_conditions condition, enum arm_register addr_reg);
uint32_t op_mov_immediate(enum arm_register dest, immediate value);
uint32_t op_mov_register(enum arm_register dest, enum arm_register src);
uint32_t op_movt_immediate(enum arm_register dest, immediate value);
uint32_t op_movw_immediate(enum arm_register dest, immediate value);
uint32_t op_mvn_immediate(enum arm_register dest, immediate value);
uint32_t op_ldr_immediate(enum arm_register dest, immediate pc_offset);
uint32_t op_pop_immediate_list
(enum arm_conditions condition, uint32_t reglist);
uint32_t op_push_immediate_list
(enum arm_conditions condition, uint32_t reglist);
uint32_t op_sub_immediate
(enum arm_register dest, enum arm_register op1, immediate op2);
uint32_t op_svc_immediate(immediate value);

uint32_t add_instruction
(struct instructions * __restrict const instructions,
 enum known_instructions id, uint32_t const val0, uint32_t const val1,
 uint32_t const val2);

uint32_t instructions_size
(struct instructions const * __restrict const instructions);

uint32_t assemble_code
(struct data_section const * __restrict const data_infos,
 struct instructions const * __restrict const instructions,
 uint32_t * result_code);

struct armv7_text_frame * generate_armv7_text_frame
(uint32_t (*id_generator)());

struct armv7_add_instruction_status {
	unsigned int added;
	struct instruction_representation * address;
};

struct armv7_add_instruction_status frame_add_instruction
(struct armv7_text_frame * __restrict const frame);

void instruction_mnemonic_id
(struct instruction_representation * const instruction,
 enum known_instructions mnemonic_id);

void instruction_arg
(struct instruction_representation * const instruction,
 unsigned int const index,
 enum argument_type argument_type,
 uint32_t const value);

unsigned int armv7_frame_gen_machine_code
(struct armv7_text_frame const * __restrict const frame,
 struct armv7_text_section const * __restrict const section,
 struct data_section const * __restrict const data_infos,
 uint32_t * __restrict const result_code);


struct armv7_text_section * generate_armv7_text_section();

unsigned int armv7_text_section_add_frame
(struct armv7_text_section * __restrict const text_section,
 struct armv7_text_frame const * __restrict const frame);

void armv7_frame_set_address
(struct armv7_text_frame * __restrict const frame,
 uint32_t const address);

unsigned int armv7_frame_size
(struct armv7_text_frame const * __restrict const frame);

unsigned int armv7_text_section_size
(struct armv7_text_section const * __restrict const text_section);

void armv7_text_section_rebase_at
(struct armv7_text_section * __restrict const text_section,
 uint32_t const addr);

uint32_t text_section_frame_address
(struct armv7_text_section const * __restrict const text_section,
 unsigned int const frame_id);

void armv7_text_section_write_at
(struct armv7_text_section const * __restrict const text_section,
 struct data_section const * __restrict const data_section,
 uint8_t * __restrict const output);

#endif
