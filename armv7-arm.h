#ifndef MYY_ARMV7_ARM_H
#define MYY_ARMV7_ARM_H 1

#include <stdint.h>
#include <data_section.h>

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

enum parameter_type {
	arg_register,
	arg_immediate,
	arg_address,
	arg_data_symbol_address,
	arg_data_symbol_address_top16,
	arg_data_symbol_address_bottom16,
	arg_data_symbol_size,
};

struct parameters {
	enum parameter_type type;
	uint32_t restriction;
};

#define MAX_ARGS 3

struct instruction_args_infos {
	enum parameter_type type;
	uint32_t value;
};

struct instruction_representation {
	enum known_instructions id;
	struct instruction_args_infos args[MAX_ARGS];
};

struct instructions {
	unsigned int n;
	struct instruction_representation * converted;
};

typedef unsigned int address;
typedef int32_t immediate;

uint32_t op_add_immediate
(enum arm_register dest, enum arm_register op1, immediate op2);
uint32_t op_b_address(enum arm_conditions condition, address imm24);
uint32_t op_bl_address(enum arm_conditions condition, address addr24);
uint32_t op_blx_address(address addr24);
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
(struct data_symbols const * __restrict const data_infos,
 struct instructions const * __restrict const instructions,
 uint32_t * result_code);


#endif
