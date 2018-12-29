#ifndef MYY_ARMV8_ARM_H
#define MYY_ARMV8_ARM_H 1

#include <stdint.h>

/* TRM : The Reference Manual */
/* When TRM is cited, it takes the following form :
 * ARM DDI 0487C.a | C1-145
 * 
 * Where:
 * - "ARM DDI 0487C.a" is the reference manual ID.
 *   If you search this ID on the Web, you'll find the PDF
 *   pretty fast.
 * - C1-145 is the page number,
 *   as printed on the bottom right of the actual page.
 */

enum arm_register {
	/* The r syntax is only used when either W or X can be used
	 * indiscriminately.
	 */
	r0   =  0,
	r1   =  1,
	r2   =  2,
	r3   =  3,
	r4   =  4,
	r5   =  5,
	r6   =  6,
	r7   =  7,
	r8   =  8,
	r9   =  9,
	r10  = 10,
	r11  = 11,
	r12  = 12,
	r13  = 13,
	r14  = 14,
	r15  = 15,
	r16  = 16,
	r17  = 17,
	r18  = 18,
	r19  = 19,
	r20  = 20,
	r21  = 21,
	r22  = 22,
	r23  = 23,
	r24  = 24,
	r25  = 25,
	r26  = 26,
	r27  = 27,
	r28  = 28,
	r29  = 29,
	r30  = 30,
	reg_zr = 31,
	/* The W syntax is used when only accessing
	 * the lower 32-bits of a register */
	w0   =  0,
	w1   =  1,
	w2   =  2,
	w3   =  3,
	w4   =  4,
	w5   =  5,
	w6   =  6,
	w7   =  7,
	w8   =  8,
	w9   =  9,
	w10  = 10,
	w11  = 11,
	w12  = 12,
	w13  = 13,
	w14  = 14,
	w15  = 15,
	w16  = 16,
	w17  = 17,
	w18  = 18,
	w19  = 19,
	w20  = 20,
	w21  = 21,
	w22  = 22,
	w23  = 23,
	w24  = 24,
	w25  = 25,
	w26  = 26,
	w27  = 27,
	w28  = 28,
	w29  = 29,
	w30  = 30,
	w_zr = 31,
	/* The standard syntax : Xn */
	x0   =  0,
	x1   =  1,
	x2   =  2,
	x3   =  3,
	x4   =  4,
	x5   =  5,
	x6   =  6,
	x7   =  7,
	x8   =  8,
	x9   =  9,
	x10  = 10,
	x11  = 11,
	x12  = 12,
	x13  = 13,
	x14  = 14,
	x15  = 15,
	x16  = 16,
	x17  = 17,
	x18  = 18,
	x19  = 19,
	x20  = 20,
	x21  = 21,
	x22  = 22,
	x23  = 23,
	x24  = 24,
	x25  = 25,
	x26  = 26,
	x27  = 27,
	x28  = 28,
	x29  = 29,
	x30  = 30,
	x_zr = 31,
	/* The 128-bits vector units.
	 * Not used ATM
	 */
	v0   =  0,
	v1   =  1,
	v2   =  2,
	v3   =  3,
	v4   =  4,
	v5   =  5,
	v6   =  6,
	v7   =  7,
	v8   =  8,
	v9   =  9,
	v10  = 10,
	v11  = 11,
	v12  = 12,
	v13  = 13,
	v14  = 14,
	v15  = 15,
	v16  = 16,
	v17  = 17,
	v18  = 18,
	v19  = 19,
	v20  = 20,
	v21  = 21,
	v22  = 22,
	v23  = 23,
	v24  = 24,
	v25  = 25,
	v26  = 26,
	v27  = 27,
	v28  = 28,
	v29  = 29,
	v30  = 30,
	v31  = 31,
	/* Special register names. */

	/* lr is dubious, however there are times X30 or W30 is used
	 * to store the return address.
	 */
	reg_lr = 30,
	/* The Zero register and SP register share the same encodings.
	 * The instruction interpret them differently depending on the
	 * context
	 * See :
	 * ARM DDI 0487C.a | C1-145
	 */
	reg_sp = 31,
	reg_wsp = 31,
	reg_discard = 31,
	reg_pc,
};
typedef enum arm_register arm_register_t;

#define REGISTER_MASK 0b11111

enum known_instructions {
	inst_adc,
	inst_adcs,
	inst_add_extended_register,
	inst_adds_extended_register,
	inst_add_immediate,
	inst_adds_immediate,
	inst_add_shifted_register,
	inst_adds_shifted_register,
	inst_adr,
	inst_adrp,
	inst_and_immediate,
	inst_ands_immediate,
	inst_asr_register,
	inst_b_cond,
	inst_b,
	inst_bl,
	inst_br,
	inst_blr,
	inst_ret,
	inst_cbnz,
	inst_cbz,
	inst_cmmn_immediate,
	inst_cmmn_register,
	inst_cmmp_immediate,
	inst_cmmp_register,
	inst_sdiv,
	inst_udiv,
	inst_lsl_register,
	inst_lsr_register,
	inst_ldr_register,
	inst_ldrb_register,
	inst_ldrh_register,
	inst_str_register,
	inst_strb_register,
	inst_ldr_literal,
	inst_ldr_immediate,
	inst_ldrh_immediate,
	inst_ldrb_immediate,
	inst_str_immediate,
	inst_strh_immediate,
	inst_strb_immediate,
	inst_ldr_unsigned_immediate,
	inst_ldrh_unsigned_immediate,
	inst_ldrb_unsigned_immediate,
	inst_str_unsigned_immediate,
	inst_strh_unsigned_immediate,
	inst_strb_unsigned_immediate,
	inst_stp,
	inst_ldp,
	inst_madd,
	inst_mul,
	inst_umaddl,
	inst_umull,
	inst_umulh,
	inst_smaddl,
	inst_smull,
	inst_smulh,
	inst_msub,
	inst_mneg,
	inst_umsubl,
	inst_umnegl,
	inst_smsubl,
	inst_snegl,
	inst_mov_register,
	inst_mvn_shifted_register,
	inst_movk,
	inst_movn,
	inst_movw,
	inst_movz,
	inst_neg,
	inst_negs,
	inst_ngc,
	inst_ngcs,
	inst_sub_extended_register,
	inst_subs_extended_register,
	inst_sub_immediate_register,
	inst_subs_immediate_register,
	inst_sub_shifted_register,
	inst_subs_shifted_register,
	inst_sbc,
	inst_sbcs,
	inst_svc,
	n_known_instructions
};


/**
 * cf : Conditional flag
 * mb : Mask bit
 */
enum pstate {
	pstate_cf_negative,
	pstate_cf_zero,
	pstate_cf_carry,
	pstate_cf_overflow,
	pstate_mb_debug,
	pstate_mb_serror_interrupt,
	pstate_mb_irq_interrupt,
	pstate_mb_fiq_interrupt
};

enum encoding_variant {
	encoding_32_bits,
	encoding_64_bits
};
typedef enum encoding_variant encoding_variant_t;

enum set_flag {
	set_flag_off,
	set_flag_on
};
typedef enum set_flag setting_flag_t;

typedef uint32_t instruction_encoding;
typedef int32_t immediate_t;

enum zero_extend {
	ze_uxtb,
	ze_uxth,
	ze_uxtw,
	ze_uxtx,
	ze_sxtb,
	ze_sxth,
	ze_sxtw,
	ze_sxtx,
	ze_lsl
};
typedef enum zero_extend zero_extend_t;

enum arm_condition_integer {
	cond_int_eq,
	cond_int_ne,
	cond_int_cs,
	cond_int_hs = cond_int_cs,
	cond_int_cc,
	cond_int_lo = cond_int_cc,
	cond_int_mi,
	cond_int_pl,
	cond_int_vs,
	cond_int_vc,
	cond_int_hi,
	cond_int_ls,
	cond_int_ge,
	cond_int_lt,
	cond_int_gt,
	cond_int_le,
	cond_int_al,
	cond_int_nv
};
typedef enum arm_condition_integer arm_condition_integer_t;

enum adr_mode {
	adr_mode_direct,
	adr_mode_page
};
typedef enum adr_mode adr_mode_t;

enum and_shift_type {
	and_shift_lsl,
	and_shift_lsr,
	and_shift_asr,
	and_shift_ror
};
typedef enum and_shift_type and_shift_type_t;

enum arithmetic_mode {
	arithmetic_standard,
	arithmetic_long,
	arithmetic_high
};
typedef enum arithmetic_mode arithmetic_mode_t;

enum arithmetic_sign {
	arithmetic_signed,
	arithmetic_unsigned
};
typedef enum arithmetic_sign arithmetic_sign_t;

enum branch_should_link {
	branch_without_link,
	branch_with_link
};
typedef enum branch_should_link branch_should_link_t;

enum branch_type {
	branch_type_jmp  = 0b00,
	branch_type_call = 0b01,
	branch_type_ret  = 0b11
};
typedef enum branch_type branch_type_t;

enum cb_type {
	cb_type_zero,
	cb_type_nonzero
};
typedef enum cb_type cb_type_t;

enum ccm_type {
	ccm_type_compare_negative,
	ccm_type_compare_positive
};
typedef enum ccm_type ccm_type_t;

enum ccm_argument {
	ccm_argument_register,
	ccm_argument_immediate
};
typedef enum ccm_argument ccm_arg_type_t;

enum condition_flag {
	cond_flag_n = 0b0001,
	cond_flag_z = 0b0010,
	cond_flag_c = 0b0100,
	cond_flag_v = 0b1000
};
typedef enum condition_flag condition_flags_t;

enum element_sizes {
	element_size_byte,
	element_size_halfword,
	element_size_word,
	element_size_doubleword
};
typedef enum element_sizes element_size_t;


enum logical_shift_direction {
	logical_shift_left,
	logical_shift_right
};
typedef enum logical_shift_direction logical_shift_direction_t;

enum memory_access_type {
	memory_access_store,
	memory_access_load
};
typedef enum memory_access_type memory_access_type_t;

enum indexing_mode {
	indexing_preindex,
	indexing_postindex
};
typedef enum indexing_mode indexing_mode_t;

enum mov_special_type {
	mt_movn = 0b00,
	mt_mov  = 0b01,
	mt_movz = 0b10,
	mt_movw = 0b10,
	mt_movk = 0b11,
};
typedef enum mov_special_type mov_special_type_t;

enum multiply_post_op {
	multiply_and_add,
	multiply_and_subtract
};
typedef enum multiply_post_op multiply_post_op_t;

enum negate_or_not {
	negate_not_set,
	negate_set
};
typedef enum negate_or_not negate_or_not_t;

enum write_back_mode {
	write_back_disabled,
	write_back_enabled
};
typedef enum write_back_mode write_back_mode_t;

/** Encodings **/

instruction_encoding adc(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_result);

instruction_encoding adcs(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_result);


instruction_encoding add_extended_register(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	zero_extend_t const zero_extend,
	uint_fast8_t const left_shift,
	arm_register_t const reg_result);

instruction_encoding adds_extended_register(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	zero_extend_t const zero_extend,
	uint_fast8_t const left_shift,
	arm_register_t const reg_result);

instruction_encoding add_immediate(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	immediate_t const immediate_value_12bits, 
	uint_fast8_t const left_shift,
	arm_register_t const reg_result);

instruction_encoding adds_immediate(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	immediate_t const immediate_value_12bits, 
	uint_fast8_t const left_shift,
	arm_register_t const reg_result);

instruction_encoding add_shifted_register(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	uint_fast8_t const shift_type,
	uint_fast8_t const shift_amount,
	arm_register_t const reg_result);

instruction_encoding adds_shifted_register(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	uint_fast8_t const shift_type,
	uint_fast8_t const shift_amount,
	arm_register_t const reg_result);

instruction_encoding adr(
	immediate_t const immediate_value_21bits,
	arm_register_t const reg_result);

instruction_encoding adrp(
	immediate_t const immediate_value_21bits,
	arm_register_t const reg_result);

instruction_encoding and_immediate(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	and_shift_type_t const shift_type,
	immediate_t const shift_amount_6bits,
	arm_register_t const reg_result);

instruction_encoding ands_immediate(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	and_shift_type_t const shift_type,
	immediate_t const shift_amount_6bits,
	arm_register_t const reg_result);

instruction_encoding asr_register(
	encoding_variant_t const variant,
	arm_register_t const reg_value,
	arm_register_t const reg_shift,
	arm_register_t const reg_result);

instruction_encoding b_cond(
	immediate_t const pc_relative_offset_19bits,
	arm_condition_integer_t const condition);

instruction_encoding b(
	immediate_t const pc_relative_offset_26bits);

instruction_encoding bl(
	immediate_t const pc_relative_offset_26bits);

instruction_encoding br(
	arm_register_t const reg_containing_address);

instruction_encoding blr(
	arm_register_t const reg_containing_address);

instruction_encoding ret(
	arm_register_t const reg_containing_address);

instruction_encoding cbnz(
	encoding_variant_t const variant,
	immediate_t const pc_relative_offset_19bits,
	arm_register_t const reg_compared);

instruction_encoding cbz(
	encoding_variant_t const variant,
	immediate_t const pc_relative_offset_19bits,
	arm_register_t const reg_compared);

instruction_encoding ccmn_immediate(
	encoding_variant_t const variant,
	arm_register_t const reg_compared,
	immediate_t const comparaison_value_5bits,
	arm_condition_integer_t const condition,
	condition_flags_t const flags_set_otherwise);

instruction_encoding ccmn_register(
	encoding_variant_t const variant,
	arm_register_t const reg_compared,
	arm_register_t const reg_comparaison_value,
	arm_condition_integer_t const condition,
	condition_flags_t const flags_set_otherwise);

instruction_encoding ccmp_immediate(
	encoding_variant_t const variant,
	arm_register_t const reg_compared,
	immediate_t const comparaison_value_5bits,
	arm_condition_integer_t const condition,
	condition_flags_t const flags_set_otherwise);

instruction_encoding ccmp_register(
	encoding_variant_t const variant,
	arm_register_t const reg_compared,
	arm_register_t const reg_comparaison_value,
	arm_condition_integer_t const condition,
	condition_flags_t const flags_set_otherwise);

instruction_encoding sdiv(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_result);

instruction_encoding udiv(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_result);

instruction_encoding lsl_register(
	encoding_variant_t const variant,
	arm_register_t const reg_shifted,
	arm_register_t const reg_shift_amount,
	arm_register_t const reg_result);

instruction_encoding lsr_register(
	encoding_variant_t const variant,
	arm_register_t const reg_shifted,
	arm_register_t const reg_shift_amount,
	arm_register_t const reg_result);

instruction_encoding ldr_register(
	arm_register_t const reg_address,
	arm_register_t const reg_offset,
	zero_extend_t const offset_extend_option,
	immediate_t const offset_shift_by_ptr_size_1bit,
	arm_register_t const reg_dst,
	encoding_variant_t const variant);

instruction_encoding ldrb_register(
	arm_register_t const reg_address,
	arm_register_t const reg_offset,
	zero_extend_t const offset_extend_option,
	immediate_t const offset_shift_by_ptr_size_1bit,
	arm_register_t const reg_dst);

instruction_encoding ldrh_register(
	arm_register_t const reg_address,
	arm_register_t const reg_offset,
	zero_extend_t const offset_extend_option,
	immediate_t const offset_shift_by_ptr_size_1bit,
	arm_register_t const reg_dst);

instruction_encoding str_register(
	arm_register_t const reg_address,
	arm_register_t const reg_offset,
	zero_extend_t const offset_extend_option,
	immediate_t const offset_shift_by_ptr_size_1bit,
	arm_register_t const reg_from,
	encoding_variant_t variant);

instruction_encoding strb_register(
	arm_register_t const reg_address,
	arm_register_t const reg_offset,
	zero_extend_t const offset_extend_option,
	immediate_t const offset_shift_by_ptr_size_1bit,
	arm_register_t const reg_from);

instruction_encoding strh_register(
	arm_register_t const reg_address,
	arm_register_t const reg_offset,
	zero_extend_t const offset_extend_option,
	immediate_t const offset_shift_by_ptr_size_1bit,
	arm_register_t const reg_from);

instruction_encoding ldr_literal(
	encoding_variant_t const variant,
	immediate_t const pc_relative_offset_19bits,
	arm_register_t const reg_loaded_value);

instruction_encoding ldr_immediate(
	arm_register_t const reg_address,
	immediate_t const pc_relative_offset_9bits,
	indexing_mode_t const indexing_mode,
	arm_register_t const reg_loaded_value,
	encoding_variant_t const variant);

instruction_encoding ldrh_immediate(
	arm_register_t const reg_address,
	immediate_t const pc_relative_offset_9bits,
	indexing_mode_t const indexing_mode,
	arm_register_t const reg_loaded_value);

instruction_encoding ldrb_immediate(
	arm_register_t const reg_address,
	immediate_t const pc_relative_offset_9bits,
	indexing_mode_t const indexing_mode,
	arm_register_t const reg_loaded_value);

instruction_encoding str_immediate(
	arm_register_t const reg_address,
	immediate_t const pc_relative_offset_9bits,
	indexing_mode_t const indexing_mode,
	arm_register_t const reg_value_to_store,
	encoding_variant_t const variant);

instruction_encoding strh_immediate(
	arm_register_t const reg_address,
	immediate_t const pc_relative_offset_9bits,
	indexing_mode_t const indexing_mode,
	arm_register_t const reg_value_to_store);

instruction_encoding strb_immediate(
	arm_register_t const reg_address,
	immediate_t const pc_relative_offset_9bits,
	indexing_mode_t const indexing_mode,
	arm_register_t const reg_value_to_store);

instruction_encoding ldr_unsigned_immediate(
	arm_register_t const reg_address,
	immediate_t const pc_relative_offset_12bits,
	arm_register_t const reg_loaded_value,
	encoding_variant_t const variant);

instruction_encoding ldrh_unsigned_immediate(
	arm_register_t const reg_address,
	immediate_t const pc_relative_offset_12bits,
	arm_register_t const reg_loaded_value);

instruction_encoding ldrb_unsigned_immediate(
	arm_register_t const reg_address,
	immediate_t const pc_relative_offset_12bits,
	arm_register_t const reg_loaded_value);

instruction_encoding str_unsigned_immediate(
	arm_register_t const reg_address,
	immediate_t const pc_relative_offset_12bits,
	arm_register_t const reg_value_to_store,
	encoding_variant_t const variant);

instruction_encoding strh_unsigned_immediate(
	arm_register_t const reg_address,
	immediate_t const pc_relative_offset_12bits,
	arm_register_t const reg_value_to_store);

instruction_encoding strb_unsigned_immediate(
	arm_register_t const reg_address,
	immediate_t const pc_relative_offset_12bits,
	arm_register_t const reg_value_to_store);


instruction_encoding stp(
	encoding_variant_t const variant,
	arm_register_t const reg_1,
	arm_register_t const reg_2,
	arm_register_t const reg_base_address,
	immediate_t const pc_relative_offset_7bits,
	indexing_mode_t const indexing,
	write_back_mode_t const write_back);

instruction_encoding ldp(
	encoding_variant_t const variant,
	arm_register_t const reg_1,
	arm_register_t const reg_2,
	arm_register_t const reg_base_address,
	immediate_t const pc_relative_offset_7bits,
	indexing_mode_t const indexing,
	write_back_mode_t const write_back);

instruction_encoding madd(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_added,
	arm_register_t const reg_result);

instruction_encoding mul(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_result);

instruction_encoding umaddl(
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_added,
	arm_register_t const reg_result);

instruction_encoding umull(
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_result);

instruction_encoding umulh(
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_result);

instruction_encoding smaddl(
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_added,
	arm_register_t const reg_result);

instruction_encoding smull(
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_result);

instruction_encoding smulh(
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_result);

instruction_encoding msub(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_subtracted,
	arm_register_t const reg_result);

instruction_encoding mneg(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_result);

instruction_encoding umsubl(
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_subtracted,
	arm_register_t const reg_result);

instruction_encoding umnegl(
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_result);

instruction_encoding smsubl(
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_subtracted,
	arm_register_t const reg_result);

instruction_encoding snegl(
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_result);

/*instruction_encoding neg(
	encoding_variant_t const variant,
	arm_register_t const reg_negated,
	and_shift_type_t const shift_type,
	immediate_t const shift_amount_6bits,
	arm_register_t const reg_result);

instruction_encoding negs(
	encoding_variant_t const variant,
	arm_register_t const reg_negated,
	and_shift_type_t const shift_type,
	immediate_t const shift_amount_6bits,
	arm_register_t const reg_result);*/

instruction_encoding mov_register(
	encoding_variant_t const variant,
	arm_register_t const reg_src,
	arm_register_t const reg_dst);

instruction_encoding mvn_shifted_register(
	encoding_variant_t const variant,
	arm_register_t const reg_src,
	and_shift_type_t const src_shift_type,
	immediate_t const src_shift_amount_6bits,
	arm_register_t const reg_dst);

instruction_encoding movk(
	encoding_variant_t const variant,
	immediate_t const src_value_16bits,
	immediate_t const src_shift_by_16,
	arm_register_t const reg_dst);

instruction_encoding movn(
	encoding_variant_t const variant,
	immediate_t const src_value_16bits,
	immediate_t const src_shift_by_16,
	arm_register_t const reg_dst);

instruction_encoding movw(
	encoding_variant_t const variant,
	immediate_t const src_value_16bits,
	immediate_t const src_shift_by_16,
	arm_register_t const reg_dst);

instruction_encoding movz(
	encoding_variant_t const variant,
	immediate_t const src_value_16bits,
	immediate_t const src_shift_by_16,
	arm_register_t const reg_dst);

instruction_encoding neg(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	and_shift_type_t const shift_type,
	immediate_t const shift_amount_6bits,
	arm_register_t const reg_dst);

instruction_encoding negs(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	and_shift_type_t const shift_type,
	immediate_t const shift_amount_6bits,
	arm_register_t const reg_dst);

instruction_encoding ngc(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_result);

instruction_encoding ngcs(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_result);

instruction_encoding sub_extended_register(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	zero_extend_t const extend_option,
	immediate_t const left_shift_amount,
	arm_register_t const reg_result);

instruction_encoding subs_extended_register(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	zero_extend_t const extend_option,
	immediate_t const left_shift_amount,
	arm_register_t const reg_result);


instruction_encoding sub_immediate(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	immediate_t const immediate_op2_12bits,
	immediate_t const left_shift_by_12,
	arm_register_t const reg_result);

instruction_encoding subs_immediate(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	immediate_t const immediate_op2_12bits,
	immediate_t const left_shift_by_12,
	arm_register_t const reg_result);


instruction_encoding sub_shifted_register(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	and_shift_type_t const shift_type,
	immediate_t const shift_amount_6bits,
	arm_register_t const reg_dst);

instruction_encoding subs_shifted_register(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	and_shift_type_t const shift_type,
	immediate_t const shift_amount_6bits,
	arm_register_t const reg_dst);

instruction_encoding sbc(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_result);

instruction_encoding sbcs(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_result);

instruction_encoding svc(
	immediate_t const system_call_number);

#endif
