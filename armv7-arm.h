#ifndef MYY_ARMV7_ARM_H
#define MYY_ARMV7_ARM_H 1

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

#endif
