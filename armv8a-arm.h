
enum arm_register {
	/* The r syntax is only used when either W or X can be used
	 * indiscriminately
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
	reg_lr = 30,
	reg_sp = 31,
	reg_wsp = 31,
	reg_discard = 31,
	reg_pc,
};
typedef enum arm_register arm_register_t;

#define REGISTER_MASK 0b11111

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

typedef instruction_encoding uint32_t;
typedef immediate_t uint32_t;

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

static instruction_encoding adc_all(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_result,
	setting_flag_t setting_flag)
{
	uint_fast32_t const sf           =
		(variant & 1)                << 31;
	uint_fast32_t const op           =
		0                            << 30;
	uint_fast32_t const s            =
		(setting_flag & 1)           << 29;
	uint_fast32_t const fixed1 =
		(0b1010000)                  << 21;
	uint_fast32_t const rm           =
		(reg_op2 & REGISTER_MASK)    << 16;
	uint_fast32_t const fixed2 =
		(0b000000)                   << 10;
	uint_fast32_t const rn           =
		(reg_op1 & REGISTER_MASK)    <<  5;
	uint_fast32_t const rd           =
		(reg_result & REGISTER_MASK) <<  0;

	return
		(sf | op | s | fixed1 | rm | fixed2 | rn | rd);
}

instruction_encoding adc(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_result)
{
	return adc_all(
		variant, reg_op1, reg_op2, reg_result, set_flag_off);
}

instruction_encoding adcs(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_result)
{
	return adc_all(
		variant, reg_op1, reg_op2, reg_result, set_flag_on);
}

static instruction_encoding add_extended_register_all(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	zero_extend_t zero_extend,
	uint_fast8_t left_shift,
	arm_register_t const reg_result,
	setting_flag_t setting_flag)
{
	uint_fast32_t const sf           =
		(variant & 1)                        << 31;
	uint_fast32_t const op                      =
		0                                    << 30;
	uint_fast32_t const s                       =
		(setting_flag & 1)                   << 29;
	uint_fast32_t const fixed              =
		(0b01011001)                         << 21;
	uint_fast32_t const rm                      =
		(reg_op2 & REGISTER_MASK)            << 16;
	/* ze_lsl means different things on different encodings */
	if (zero_extend == ze_lsl)
		option = 
			(variant == encoding_64_bits) ? ze_uxtx : ze_uxtw;
	else option = zero_extend;
	option = (option & 0b111)                << 13;
	uint_fast32_t const imm3         =
		((left_shift <= 4) ? left_shift : 0) << 10;
	uint_fast32_t const rn           =
		(reg_op1 & REGISTER_MASK)            <<  5;
	uint_fast32_t const rd           =
		(reg_result & REGISTER_MASK)         <<  0;


	
	return
		(sf | op | s | fixed | rm | option | imm3 | rn | rd);
}

instruction_encoding add_extended_register(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	zero_extend_t zero_extend,
	uint_fast8_t left_shift,
	arm_register_t const reg_result)
{
	return add_extended_register_all(
		variant,
		reg_op1,
		reg_op2, zero_extend, left_shift,
		reg_result,
		set_flag_off);
}


instruction_encoding adds_extended_register(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	zero_extend_t zero_extend,
	uint_fast8_t left_shift,
	arm_register_t const reg_result)
{
	return add_extended_register_all(
		variant,
		reg_op1,
		reg_op2, zero_extend, left_shift,
		reg_result,
		set_flag_on);
}

static instruction_encoding add_immediate_all(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	immediate_t immediate_value_12bits, 
	uint_fast8_t left_shift,
	arm_register_t const reg_result,
	setting_flag_t setting_flag)
{
	uint_fast32_t const sf         =
		(variant & 1)                             << 31;
	uint_fast32_t const op         =
		(0)                                       << 30;
	uint_fast32_t const s          =
		(setting_flag & 1)                        << 29;
	uint_fast32_t const fixed =
		(0b10001)                                 << 24;
	/* ARM DDI 0487C.a C6-532
	 * ADD (immediate)
	 * LSL #0  when shift = 00
	 * LSL #12 when shift = 01
	 */
	// Abusing the bool to int conversion.
	uint_fast32_t const shift      =
		(left_shift == 12)                        << 22;
	uint_fast32_t const imm12      =
		(immediate_value_12bits & 0b111111111111) << 10;
	uint_fast32_t const rn         =
		(reg_op1 & REGISTER_MASK)                 <<  5;
	uint_fast32_t const rd         =
		(reg_result & REGISTER_MASK)              <<  0;

	return
		(sf | op | s | fixed | shift | imm12 | rn | rd);
}

instruction_encoding add_immediate(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	immediate_t immediate_value_12bits, 
	uint_fast8_t left_shift,
	arm_register_t const reg_result)
{
	return add_immediate_all(
		variant,
		reg_op1,
		immediate_value_12bits, left_shift,
		reg_result,
		set_flag_off);
}

instruction_encoding adds_immediate(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	immediate_t immediate_value_12bits, 
	uint_fast8_t left_shift,
	arm_register_t const reg_result)
{
	return add_immediate_all(
		variant,
		reg_op1,
		immediate_value_12bits, left_shift,
		reg_result,
		set_flag_on);
}

static instruction_encoding add_shifted_register_all(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	uint_fast8_t shift_type,
	uint_fast8_t shift_amount,
	arm_register_t const reg_result,
	setting_flag_t const setting_flag)
{
	uint_fast32_t const sf =
		(variant & 1)                     << 31;
	uint_fast32_t const op =
		(0)                               << 30;
	uint_fast32_t const s  =
		(setting_flag & 1)                << 29;
	uint_fast32_t const fixed1 =
		(0b1011)                          << 24;
	uint_fast32_t const shift = (shift_type & 0b11);
	shift = ((shift != 0b11) ? shift : 0) << 22;
	uint_fast32_t const fixed2 =
		(0)                               << 21;
	uint_fast32_t const rm =
		(reg_op2 & REGISTER_MASK)         << 16;
	uint_fast32_t const imm6 =
		(shift_amount & 0b111111)         << 10;
	uint_fast32_t const rn =
		(reg_op1 & REGISTER_MASK)         <<  5;
	uint_fast32_t const rd =
		(reg_result & REGISTER_MASK)      <<  0;

	return
		(sf | op | s | fixed1 | shift | fixed2 | rm | imm6 | rn | rd);
}

instruction_encoding add_shifted_register(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	uint_fast8_t shift_type,
	uint_fast8_t shift_amount,
	arm_register_t const reg_result)
{
	return add_shifted_register(
		variant,
		reg_op1,
		reg_op2, shift_type, shift_amount,
		reg_result,
		set_flag_off);
}

instruction_encoding adds_shifted_register(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	uint_fast8_t shift_type,
	uint_fast8_t shift_amount,
	arm_register_t const reg_result)
{
	return adds_shifted_register(
		variant,
		reg_op1,
		reg_op2, shift_type, shift_amount,
		reg_result,
		set_flag_on);
}

enum adr_mode {
	adr_mode_direct,
	adr_mode_page
};
typedef enum adr_mode adr_mode_t;

static instruction_encoding adr_all(
	immediate_t const immediate_value_21bits,
	arm_register_t const reg_result,
	adr_mode_t const mode)
{
	uint_fast32_t const op =
		(mode & 1) << 31;
	uint_fast32_t const immlo =
		(immediate_value_21bits & 0b11) << 29;
	uint_fast32_t const fixed =
		0b10000 << 24;
	uint_fast32_t const immhi =
		(((immediate_value_21bits >> 2) 
		 & 0b1111111111111111111)
		<< 5;
	uint_fast32_t const rd =
		reg_result & REGISTER_MASK;

	return
		(op | immlo | fixed | immhi | rd);
}

instruction_encoding adr(
	immediate_t const immediate_value_21bits,
	arm_register_t const reg_result)
{
	return adr_all(
		immediate_value_21bits,
		reg_result,
		adr_mode_direct);
}

instruction_encoding adrp(
	immediate_t const immediate_value_21bits,
	arm_register_t const reg_result)
{
	return adr_all(
		immediate_value_21bits,
		reg_result,
		adr_mode_page);
}

static instruction_encoding and_immediate_all(
	encoding_variant_t const variant,
	setting_flag_t const setting_flag)
{
	uint_fast32_t const sf =
		(variant & 1) << 31;
	uint_fast32_t const opc =
		((setting_flag & 1) | (setting_flag & 1)) << 29;
	uint_fast32_t const fixed =
		0b100100;
	/// ???? Bitmasks ????
}

instruction_encoding and_immediate(
	)
{
	return and_immediate_all(
		set_flag_off);
}

instruction_encoding ands_immediate(
	)
{
	return and_immediate_all(
		set_flag_on);
}

enum and_shift_type {
	and_shift_lsl,
	and_shift_lsr,
	and_shift_asr,
	and_shift_ror
};
typedef enum and_shift_type and_shift_type_t;

static instruction_encoding and_shifted_register_all(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	and_shift_type_t const shift_type,
	immediate_t const shift_amount_6bits,
	arm_register_t const reg_result,
	setting_flag_t const setting_flag)
{
	uint_fast32_t const sf =
		(variant & 1) << 31;
	uint_fast32_t const opc =
		((setting_flag & 1) | (setting_flag & 1)) << 29;
	uint_fast32_t const fixed =
		(0b01010) << 24;
	uint_fast32_t const shift =
		(shift_type & 0b11) << 22;
	uint_fast32_t const n =
		0 << 21;
	uint_fast32_t const rm =
		(reg_op2 & REGISTER_MASK) << 16;
	uint_fast32_t const imm6 =
		(shift_amount_6bits & 0b111111) << 10;
	uint_fast32_t const rn =
		(reg_op1 & REGISTER_MASK) << 5;
	uint_fast32_t const rd =
		(reg_result & REGISTER_MASK) << 0;

	return
		(sf | opc | fixed | shift | n | rm | imm6 | rn | rd);
}

instruction_encoding and_immediate(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	and_shift_type_t const shift_type,
	immediate_t const shift_amount_6bits,
	arm_register_t const reg_result)
{
	return and_shifted_register_all(
		variant,
		reg_op1,
		reg_op2, shift_type, shift_amount_6bits,
		reg_result,
		set_flag_off);
}

instruction_encoding ands_immediate(
	)
{
	return and_shifted_register_all(
		variant,
		reg_op1,
		reg_op2, shift_type, shift_amount_6bits,
		reg_result,
		set_flag_on);
}

instruction_encoding asr_register(
	encoding_variant_t const variant,
	arm_register_t const reg_value,
	arm_register_t const reg_shift,
	arm_register_t const reg_result)
{
	uint_fast32_t const sf =
		(variant & 1) << 31;
	uint_fast32_t const fixed1 =
		(0b0011010110) << 21;
	uint_fast32_t const rm =
		(reg_shift & REGISTER_MASK) << 16;
	uint_fast32_t const fixed2 =
		(0b001010) << 10;
	uint_fast32_t const rn =
		(reg_value & REGISTER_MASK) << 5;
	uint_fast32_t const rd =
		(reg_result & REGISTER_MASK) << 0;

	return
		(sf | fixed1 | rm | fixed2 | rn | rd);
}

instruction_encoding b_cond(
	immediate_t pc_relative_offset_19bits,
	arm_condition_integer_t condition)
{
	uint_fast32_t const fixed1 =
		(0b01010100) << 24;
	uint_fast32_t const imm19 =
		(pc_relative_offset_19bits & 0b1111111111111111111) << 5;
	uint_fast32_t const fixed2 =
		(0)                                                 << 4;
	uint_fast32_t const cond =
		(condition & 0b1111)                                << 0;

	return
		(fixed1 | imm19 | fixed2 | cond);
}

enum branch_should_link {
	branch_without_link,
	branch_with_link
};
typedef enum branch_should_link branch_should_link_t;

static instruction_encoding b_or_bl(
	immediate_t pc_relative_offset_26bits,
	branch_should_link_t should_link)
{
	uint_fast32_t const op =
		(should_link & 1) << 31;
	uint_fast32_t const fixed =
		0b00101 << 26;
	uint_fast32_t const imm26 =
		(pc_relative_offset_26bits
		 & 0b11111111111111111111111111)
		<< 0;

	return
		(op | fixed | imm26);
}

instruction_encoding b(
	immediate_t pc_relative_offset_26bits)
{
	return b_or_bl(pc_relative_offset_26bits, branch_without_link);
}

instruction_encoding bl(
	immediate_t pc_relative_offset_26bits)
{
	return b_or_bl(pc_relative_offset_26bits, branch_with_link);
}

enum branch_type {
	branch_type_jmp  = 0b00,
	branch_type_call = 0b01,
	branch_type_ret  = 0b11
};
typedef enum branch_type branch_type_t;

static instruction_encoding b_register_all(
	arm_register_t const reg_containing_address,
	branch_type_t const b_type)
{
	uint_fast32_t const fixed1 =
		0b1101011 << 25;
	uint_fast32_t const z =
		0 << 24;
	uint_fast32_t const fixed2 =
		0 << 23;
	uint_fast32_t const op =
		(branch_type & 0b11) << 21;
	uint_fast32_t const fixed3 =
		0b111110000 << 12;
	uint_fast32_t const a =
		0 << 11;
	uint_fast32_t const m =
		0 << 10;
	uint_fast32_t const rn =
		(reg_containing_address & REGISTER_MASK) << 5;
	uint_fast32_t const rm =
		0b00000;

	return
		(fixed1 | z | fixed2 | op | fixed3 | a | m | rn | rm);
}

instruction_encoding br(
	arm_register_t const reg_containing_address)
{
	return b_register_all(reg_containing_address, branch_type_jmp);
}

instruction_encoding blr(
	arm_register_t const reg_containing_address)
{
	return b_register_all(reg_containing_address, branch_type_call);
}

instruction_encoding ret(
	arm_register_t const reg_containing_address)
{
	return b_register_all(reg_containing_address, branch_type_ret);
}

enum cb_type {
	cb_type_zero,
	cb_type_nonzero
};
typedef enum cb_type cb_type_t;

static instruction_encoding cb_all(
	encoding_variant_t const variant,
	immediate_t pc_relative_offset_19bits,
	arm_register_t const reg_compared,
	cb_type_t const branch_condition)
{
	uint_fast32_t const sf =
		(variant & 1) << 31;
	uint_fast32_t const fixed =
		(0b011010) << 25;
	uint_fast32_t const op =
		(branch_condition & 1) << 24;
	uint_fast32_t const imm19 =
		(pc_relative_offset_19bits & 0b1111111111111111111 << 5;
	uint_fast32_t const rt =
		(reg_compared & REGISTER_MASK) << 0;

	return
		(sf | fixed | op | imm19 | rt);
}

instruction_encoding cbnz(
	)
{
	return cb_all(
		cb_type_nonzero);
}

instruction_encoding cbz(
	)
{
	return cb_all(
		cb_type_zero);
}

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

static instruction_encoding ccm_all(
	encoding_variant_t variant,
	arm_register_t reg_compared,
	uint8_t comparaison_value_or_register,
	arm_condition_integer_t condition,
	condition_flags_t flags_set_otherwise,
	ccm_type_t comparaison,
	ccm_arg_type_t select_imm5_or_rm)
{
	uint_fast32_t const sf =
		(variant & 1) << 31;
	uint_fast32_t const op =
		(comparaison & 1) << 30;:
	uint_fast32_t const fixed1 =
		(0b111010010) << 21;
	uint_fast32_t const imm5_or_rm =
		(comparaison_value_or_register) << 16;
	uint_fast32_t const cond =
		(condition & 0b1111) << 12;
	uint_fast32_t const selection =
		(select_imm5_or_rm == ccm_argument_immediate) << 11;
	uint_fast32_t const fixed2 =
		(0) << 10;
	uint_fast32_t const rn =
		(reg_compared << 5);
	uint_fast32_t const fixed3 =
		(0) << 4;
	uint_fast32_t const nzcv =
		(flags_set_otherwise & 0b1111) << 0;

	return
		(sf | op | fixed1 | imm5_or_rm | cond 
		 | selection | fixed2 | rn | fixed3 | nzcv);
}

instruction_encoding ccmn_immediate(
	encoding_variant_t variant,
	arm_register_t reg_compared,
	immediate_t comparaison_value_5bits,
	arm_condition_integer_t condition,
	condition_flags_t flags_set_otherwise)
{
	return ccm_all(
		variant,
		reg_compared,
		(uint8_t) comparaison_value_5bits,
		condition,
		flags_set_otherwise,
		ccm_type_compare_negative,
		ccm_argument_immediate);
}

instruction_encoding ccmn_register(
	encoding_variant_t variant,
	arm_register_t reg_compared,
	arm_register_t reg_comparaison_value,
	arm_condition_integer_t condition,
	condition_flags_t flags_set_otherwise)
{
	return ccm_all(
		variant,
		reg_compared,
		reg_comparaison_value,
		condition,
		flags_set_otherwise,
		ccm_type_compare_negative,
		ccm_argument_register);
}

instruction_encoding ccmp_immediate(
	encoding_variant_t variant,
	arm_register_t reg_compared,
	immediate_t comparaison_value_5bits,
	arm_condition_integer_t condition,
	condition_flags_t flags_set_otherwise)
{
	return ccm_all(
		variant,
		reg_compared,
		(uint8_t) comparaison_value_5bits,
		condition,
		flags_set_otherwise,
		ccm_type_compare_positive,
		ccm_argument_immediate);
}

instruction_encoding ccmp_register(
	encoding_variant_t variant,
	arm_register_t reg_compared,
	arm_register_t reg_comparaison_value,
	arm_condition_integer_t condition,
	condition_flags_t flags_set_otherwise)
{
	return ccm_all(
		variant,
		reg_compared,
		reg_comparaison_value,
		condition,
		flags_set_otherwise,
		ccm_type_compare_positive,
		ccm_argument_register);
}

enum logical_shift_direction {
	logical_shift_left,
	logical_shift_right
};
typedef enum logical_shift_direction logical_shift_direction_t;

static instruction_encoding div_all(
	encoding_variant_t variant,
	arm_register_t reg_op1,
	arm_register_t reg_op2,
	arm_register_t reg_result,
	arithmetic_sign_t sign)
{
	uint_fast32_t const sf =
		(variant & 1) << 31;
	uint_fast32_t const fixed1 =
		(0b0011010110) << 21;
	uint_fast32_t const rm =
		(reg_op2 & REGISTER_MASK) << 16;
	uint_fast32_t const fixed2 =
		(0b00001) << 11;
	uint_fast32_t const s =
		((~sign) & 1) << 10;
	uint_fast32_t const rn =
		(reg_op1 & REGISTER_MASK) << 5;
	uint_fast32_t const rd =
		(reg_result & REGISTER_MASK) << 0;

	return
		(sf | fixed1 | rm | fixed2 | s | rn | rd);
}

instruction_encoding sdiv(
	encoding_variant_t variant,
	arm_register_t reg_op1,
	arm_register_t reg_op2,
	arm_register_t reg_result)
{
	return div_all(
		variant,
		reg_op1,
		reg_op2,
		reg_result,
		arithmetic_signed);
}

instruction_encoding udiv(
	encoding_variant_t variant,
	arm_register_t reg_op1,
	arm_register_t reg_op2,
	arm_register_t reg_result)
{
	return div_all(
		variant,
		reg_op1,
		reg_op2,
		reg_result,
		arithmetic_unsigned);
}

static instruction_encoding logical_shift_register_all(
	encoding_variant_t variant,
	arm_register_t reg_shifted,
	arm_register_t reg_shift_amount,
	arm_register_t reg_result,
	logical_shift_direction_t shift_direction)
{
	uint_fast32_t const sf =
		(variant & 1) << 31;
	uint_fast32_t const fixed1 =
		(0b0011010110) << 21;
	uint_fast32_t const rm =
		(reg_shift_amount & REGISTER_MASK) << 16;
	uint_fast32_t const fixed2 =
		(0b0010) << 12;
	uint_fast32_t const op2 =
		(shift_direction & 0b01) << 10;
	uint_fast32_t const rn =
		(reg_shifted & REGISTER_MASK) << 5;
	uint_fast32_t const rd =
		(reg_result & REGISTER_MASK) << 0;

	return
		(sf | fixed1 | rm | fixed2 | op2 | rn | rd);
}

instruction_encoding lsl_register(
	encoding_variant_t variant,
	arm_register_t reg_shifted,
	arm_register_t reg_shift_amount,
	arm_register_t reg_result)
{
	return logical_shift_register_all(
		variant,
		reg_shifted,
		reg_shift_amount,
		reg_result,
		logical_shift_left);
}

instruction_encoding lsr_register(
	encoding_variant_t variant,
	arm_register_t reg_shifted,
	arm_register_t reg_shift_amount,
	arm_register_t reg_result)
{
	return logical_shift_register_all(
		variant,
		reg_shifted,
		reg_shift_amount,
		reg_result,
		logical_shift_right);
}

enum element_sizes {
	element_size_byte,
	element_size_halfword,
	element_size_word,
	element_size_doubleword
};
typedef enum element_sizes element_size_t;

enum memory_access_type {
	memory_access_store,
	memory_access_load
};
typedef enum memory_access_type memory_access_type_t;

static instruction_encoding ldr_str_register_all(
	arm_register_t const reg_address,
	arm_register_t const reg_offset,
	zero_extend_t const offset_extend_option,
	immediate_t const offset_shift_by_ptr_size_1bit,
	arm_register_t const reg_dst_or_src,
	element_size_t const element_size,
	memory_access_type_t memory_access)
{
	uint_fast32_t const size =
		(element_size & 0b11) << 30;
	uint_fast32_t const fixed1 =
		(0b111000) << 24;
	uint_fast32_t const opc =
		(memory_access & 0b01) << 22;
	uint_fast32_t const fixed2 =
		(1) << 21;
	uint_fast32_t const rm =
		(reg_offset & REGISTER_MASK) << 16;
	uint_fast32_t const option =
		(offset_extend_option & 0b111) << 13;
	uint_fast32_t const s =
		(offset_shift_by_ptr_size_1bit & 1) << 12;
	uint_fast32_t const fixed3 =
		(0b10) << 10;
	uint_fast32_t const rn =
		(reg_address & REGISTER_MASK) << 5;
	uint_fast32_t const rt =
		(reg_dst_or_src & REGISTER_MASK) << 0;

	return
		(size | fixed1 | opc | fixed2 | rm | option | s | fixed3 | rn | rt);
}

instruction_encoding ldr_register(
	arm_register_t const reg_address,
	arm_register_t const reg_offset,
	zero_extend_t const offset_extend_option,
	immediate_t const offset_shift_by_ptr_size_1bit,
	arm_register_t const reg_dst,
	encoding_variant_t variant)
{
	return ldr_str_register_all(
		reg_address,
		reg_offset,
		offset_extend_option,
		offset_shift_by_ptr_size_1bit,
		reg_dst,
		(variant == encoding_64_bits)
		? element_size_word
		: element_size_doubleword,
		memory_access_load);
}

instruction_encoding ldrb_register(
	arm_register_t const reg_address,
	arm_register_t const reg_offset,
	zero_extend_t const offset_extend_option,
	immediate_t const offset_shift_by_ptr_size_1bit,
	arm_register_t const reg_dst)
{
	return ldr_str_register_all(
		reg_address,
		reg_offset,
		offset_extend_option,
		offset_shift_by_ptr_size_1bit,
		reg_dst,
		element_size_byte,
		memory_access_load);
}

instruction_encoding ldrh_register(
	arm_register_t const reg_address,
	arm_register_t const reg_offset,
	zero_extend_t const offset_extend_option,
	immediate_t const offset_shift_by_ptr_size_1bit,
	arm_register_t const reg_dst)
{
	return ldr_str_register_all(
		reg_address,
		reg_offset,
		offset_extend_option,
		offset_shift_by_ptr_size_1bit,
		reg_dst,
		element_size_halfword,
		memory_access_load);
}


instruction_encoding str_register(
	arm_register_t const reg_address,
	arm_register_t const reg_offset,
	zero_extend_t const offset_extend_option,
	immediate_t const offset_shift_by_ptr_size_1bit,
	arm_register_t const reg_from,
	encoding_variant_t variant)
{
	return ldr_str_register_all(
		reg_address,
		reg_offset,
		offset_extend_option,
		offset_shift_by_ptr_size_1bit,
		reg_from,
		(variant == encoding_64_bits)
		? element_size_word
		: element_size_doubleword,
		memory_access_store);
}

instruction_encoding strb_register(
	arm_register_t const reg_address,
	arm_register_t const reg_offset,
	zero_extend_t const offset_extend_option,
	immediate_t const offset_shift_by_ptr_size_1bit,
	arm_register_t const reg_from)
{
	return ldr_str_register_all(
		reg_address,
		reg_offset,
		offset_extend_option,
		offset_shift_by_ptr_size_1bit,
		reg_from,
		element_size_byte,
		memory_access_store);
}

instruction_encoding strh_register(
	arm_register_t const reg_address,
	arm_register_t const reg_offset,
	zero_extend_t const offset_extend_option,
	immediate_t const offset_shift_by_ptr_size_1bit,
	arm_register_t const reg_from)
{
	return ldr_str_register_all(
		reg_address,
		reg_offset,
		offset_extend_option,
		offset_shift_by_ptr_size_1bit,
		reg_from,
		element_size_halfword,
		memory_access_store);
}

instruction_encoding ldr_literal(
	encoding_variant_t const variant,
	immediate_t const pc_relative_offset_19bits,
	arm_register_t const reg_loaded_value)
{
	uint_fast32_t const opc =
		(0b00 | (variant & 1)) << 30;
	uint_fast32_t const fixed =
		(0b011000) << 24;
	uint_fast32_t const imm19 =
		(pc_relative_offset_19bits & 0b1111111111111111111) << 5;
	uint_fast32_t const rt =
		(reg_loaded_value & REGISTER_MASK) << 0;

	return
		(opc | fixed | imm19 | rt);
}

enum indexing_mode {
	indexing_preindex,
	indexing_postindex
};
typedef enum indexing_mode indexing_mode_t;

static instruction_encoding ldr_str_immediate_all(
	arm_register_t const reg_address,
	immediate_t const pc_relative_offset_9bits,
	indexing_mode_t const indexing,
	arm_register_t const reg_dst_or_src,
	element_size_t const element_size,
	memory_access_type_t const memory_access)
{
	uint_fast32_t const size =
		(element_size & 0b11) << 30;
	uint_fast32_t const fixed1 =
		(0b111000) << 24;
	uint_fast32_t const opc =
		(memory_access & 0b00) << 22;
	uint_fast32_t const fixed2 =
		(0) << 21;
	uint_fast32_t const imm9 =
		(pc_relative_offset_9bits & 0b111111111) << 12;
	uint_fast32_t const i =
		(indexing & 1) << 11;
	uint_fast32_t const fixed3 =
		(0) << 10;
	uint_fast32_t const rn =
		(reg_address & REGISTER_MASK) << 5;
	uint_fast32_t const rt =
		(reg_dst_or_src & REGISTER_MASK) << 0;

	return
		(size | fixed1 | opc | fixed2 | imm9 | i | fixed3 | rn | rt);
}

instruction_encoding ldr_immediate(
	arm_register_t const reg_address,
	immediate_t const pc_relative_offset_9bits,
	indexing_mode_t const indexing_mode,
	arm_register_t const reg_loaded_value,
	encoding_variant_t variant)
{
	return ldr_str_immediate_all(
		reg_address,
		pc_relative_offset_9bits,
		indexing_mode,
		reg_loaded_value,
		(variant == encoding_64_bits)
		? element_size_word
		: element_size_doubleword,
		memory_access_load);
}

instruction_encoding ldrh_immediate(
	arm_register_t const reg_address,
	immediate_t const pc_relative_offset_9bits,
	indexing_mode_t const indexing_mode,
	arm_register_t const reg_loaded_value)
{
	return ldr_str_immediate_all(
		reg_address,
		pc_relative_offset_9bits,
		indexing_mode,
		reg_loaded_value,
		element_size_halfword,
		memory_access_load);
}

instruction_encoding ldrb_immediate(
	arm_register_t const reg_address,
	immediate_t const pc_relative_offset_9bits,
	indexing_mode_t const indexing_mode,
	arm_register_t const reg_loaded_value)
{
	return ldr_str_immediate_all(
		reg_address,
		pc_relative_offset_9bits,
		indexing_mode,
		reg_loaded_value,
		element_size_byte,
		memory_access_load);
}

instruction_encoding str_immediate(
	arm_register_t const reg_address,
	immediate_t const pc_relative_offset_9bits,
	indexing_mode_t const indexing_mode,
	arm_register_t const reg_value_to_store,
	encoding_variant_t variant)
{
	return ldr_str_immediate_all(
		reg_address,
		pc_relative_offset_9bits,
		indexing_mode,
		reg_value_to_store,
		(variant == encoding_64_bits)
		? element_size_word
		: element_size_doubleword,
		memory_access_store);
}

instruction_encoding strh_immediate(
	arm_register_t const reg_address,
	immediate_t const pc_relative_offset_9bits,
	indexing_mode_t const indexing_mode,
	arm_register_t const reg_value_to_store)
{
	return ldr_str_immediate_all(
		reg_address,
		pc_relative_offset_9bits,
		indexing_mode,
		reg_value_to_store,
		element_size_halfword,
		memory_access_store);
}

instruction_encoding strb_immediate(
	arm_register_t const reg_address,
	immediate_t const pc_relative_offset_9bits,
	indexing_mode_t const indexing_mode,
	arm_register_t const reg_value_to_store)
{
	return ldr_str_immediate_all(
		reg_address,
		pc_relative_offset_9bits,
		indexing_mode,
		reg_value_to_store,
		element_size_byte,
		memory_access_store);
}

static instruction_encoding ldr_str_unsigned_immediate_all(
	arm_register_t const reg_address,
	immediate_t const pc_relative_offset_12bits,
	arm_register_t const reg_dst_or_src,
	element_size_t const element_size,
	memory_access_type_t const memory_access)
{
	uint_fast32_t const size =
		(element_size & 0b11)                        << 30;
	uint_fast32_t const fixed =
		(0b111001)                                   << 24;
	uint_fast32_t const opc =
		(memory_access & 0b00)                       << 22;
	uint_fast32_t const imm12 =
		(pc_relative_offset_12bits & 0b111111111111) << 10;
	uint_fast32_t const rn =
		(reg_address & REGISTER_MASK)                <<  5;
	uint_fast32_t const rt =
		(reg_dst_or_src & REGISTER_MASK)             <<  0;

	return
		(size | fixed | opc | imm12 | rn | rt);
}

instruction_encoding ldr_unsigned_immediate(
	arm_register_t const reg_address,
	immediate_t const pc_relative_offset_12bits,
	arm_register_t const reg_loaded_value,
	encoding_variant_t variant)
{
	return ldr_str_unsigned_immediate_all(
		reg_address,
		pc_relative_offset_12bits,
		reg_loaded_value,
		(variant == encoding_64_bits)
		? element_size_doubleword
		: element_size_word,
		memory_access_load);
}

instruction_encoding ldrh_unsigned_immediate(
	arm_register_t const reg_address,
	immediate_t const pc_relative_offset_12bits,
	arm_register_t const reg_loaded_value)
{
	return ldr_str_unsigned_immediate_all(
		reg_address,
		pc_relative_offset_12bits,
		reg_loaded_value,
		element_size_halfword,
		memory_access_load);
}

instruction_encoding ldrb_unsigned_immediate(
	arm_register_t const reg_address,
	immediate_t const pc_relative_offset_12bits,
	arm_register_t const reg_loaded_value)
{
	return ldr_str_unsigned_immediate_all(
		reg_address,
		pc_relative_offset_12bits,
		reg_loaded_value,
		element_size_byte,
		memory_access_load);
}

instruction_encoding str_unsigned_immediate(
	arm_register_t const reg_address,
	immediate_t const pc_relative_offset_12bits,
	arm_register_t const reg_value_to_store,
	encoding_variant_t variant)
{
	return ldr_str_unsigned_immediate_all(
		reg_address,
		pc_relative_offset_12bits,
		reg_value_to_store,
		(variant == encoding_64_bits)
		? element_size_doubleword
		: element_size_word,
		memory_access_store);
}

instruction_encoding strh_unsigned_immediate(
	arm_register_t const reg_address,
	immediate_t const pc_relative_offset_12bits,
	arm_register_t const reg_value_to_store)
{
	return ldr_str_unsigned_immediate_all(
		reg_address,
		pc_relative_offset_12bits,
		reg_value_to_store,
		element_size_halfword,
		memory_access_store);
}

instruction_encoding strb_unsigned_immediate(
	arm_register_t const reg_address,
	immediate_t const pc_relative_offset_12bits,
	arm_register_t const reg_value_to_store)
{
	return ldr_str_unsigned_immediate_all(
		reg_address,
		pc_relative_offset_12bits,
		reg_value_to_store,
		element_size_byte,
		memory_access_store);
}

enum write_back_mode {
	write_back_disabled,
	write_back_enabled
};
typedef enum write_back_mode write_back_mode_t;

/* TODO : Is the "no-writeback" mode THAT useful ?
 * It's basically added to deal with TRM
 * "Signed offset" encoding, which is terribly named.
 * All encodings of LDP, STP use a "signed offset".
 * So that encoding just disable the writeback and use
 * a pre-index mode.
 * (Of course, since there ain't any use of post-indexing
 *  if you don't use the offset and don't write the offseted
 *  address back).
 */
static instruction_encoding stp_ldp_all(
	encoding_variant_t const variant,
	arm_register_t const reg_1,
	arm_register_t const reg_2,
	arm_regisetr_t const reg_base_address,
	immediate_t const pc_relative_offset_7bits,
	indexing_mode_t indexing,
	write_back_mode_t write_back,
	memory_access_type_t access_type)
{
	// Check this before calling this function
	// if (!write_back) indexing = indexing_preindex;
	uint_fast32_t const opc_h =
		(variant & 1)                          << 31;
	uint_fast32_t const opc_l =
		(0)                                    << 30;
	uint_fast32_t const fixed =
		(0b10100)                              << 25;
	uint_fast32_t const wb =
		(write_back & 1)                       << 24;
	uint_fast32_t const i =
		(indexing & 1)                         << 23;
	uint_fast32_t const l =
		(access_type & 1)                      << 22;
	uint_fast32_t const imm7 =
		(pc_relative_offset_7bits & 0b1111111) << 15;
	uint_fast32_t const rt2  =
		(reg_2 & REGISTER_MASK)                << 10;
	uint_fast32_t const rn =
		(reg_base_address & REGISTER_MASK)     <<  5;
	uint_fast32_t const rt =
		(reg_1 & REGISTER_MASK)                <<  0;

	return
		(opc_h | opc_l | fixed | wb | i | l | imm7 | rt2 | rn | rt);
}

instruction_encoding stp(
	encoding_variant_t const variant,
	arm_register_t const reg_1,
	arm_register_t const reg_2,
	arm_regisetr_t const reg_base_address,
	immediate_t const pc_relative_offset_7bits,
	indexing_mode_t indexing,
	write_back_mode_t write_back)
{
	return stp_ldp_all(
		variant,
		reg_1,
		reg_2,
		reg_base_address,
		pc_relative_offset_7bits,
		indexing,
		write_back,
		memory_access_store);
}

instruction_encoding ldp(
	encoding_variant_t const variant,
	arm_register_t const reg_1,
	arm_register_t const reg_2,
	arm_regisetr_t const reg_base_address,
	immediate_t const pc_relative_offset_7bits,
	indexing_mode_t indexing,
	write_back_mode_t write_back)
{
	return stp_ldp_all(
		variant,
		reg_1,
		reg_2,
		reg_base_address,
		pc_relative_offset_7bits,
		indexing,
		write_back,
		memory_access_load);
}

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

enum multiply_post_op {
	multiply_and_add,
	multiply_and_subtract
};
typedef enum multiply_post_op multiply_post_op_t;

static instruction_encoding mul_all(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_post_multiply_offset,
	arm_register_t const reg_final_result,
	arithmetic_mode_t const mode,
	arithmetic_sign_t const use_sign,
	multiply_post_op_t const post_operation)
{
	uint_fast32_t const sf =
		(variant & 1) << 31;
	uint_fast32_t const fixed =
		(0b0011011) << 24;
	uint_fast32_t const u =
		(use_sign & 1) << 23;
	uint_fast32_t const size =
		(mode & 0b11) << 21;
	uint_fast32_t const rm =
		(reg_op2 & REGISTER_MASK) << 16;
	uint_fast32_t const o0 =
		(post_operation & 1) << 15;
	uint_fast32_t const ra =
		(reg_post_multiply_offset & REGISTER_MASK) << 10;
	uint_fast32_t const rn =
		(reg_op1 & REGISTER_MASK) << 5;
	uint_fast32_t const rd =
		(reg_final_result & REGISTER_MASK) << 0;

	return
		(sf | fixed | u | size | rm | o0 | ra | rn | rd);
}

instruction_encoding madd(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_added,
	arm_register_t const reg_result)
{
	return mul_all(
		variant,
		reg_op1,
		reg_op2,
		reg_added,
		reg_result,
		arithmetic_standard,
		arithmetic_signed,
		multiply_and_add);
}

instruction_encoding mul(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_result)
{
	return madd(variant, reg_op1, reg_op2, x_zr, reg_result);
}

instruction_encoding umaddl(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_added,
	arm_register_t const reg_result)
{
	return mul_all(
		variant,
		reg_op1,
		reg_op2,
		reg_added,
		reg_result,
		arithmetic_long,
		arithmetic_unsigned,
		multiply_and_add);
}

instruction_encoding umull(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_result)
{
	return umaddl(variant, reg_op1, reg_op2, x_zr, reg_result);
}

instruction_encoding umulh(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_result)
{
	return mul_all(
		variant,
		reg_op1,
		reg_op2,
		x_zr,
		reg_result,
		arithmetic_high,
		arithmetic_unsigned,
		multiply_and_add);
}

instruction_encoding smaddl(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_added,
	arm_register_t const reg_result)
{
	return mul_all(
		variant,
		reg_op1,
		reg_op2,
		reg_added,
		reg_result,
		arithmetic_long,
		arithmetic_signed,
		multiply_and_add);
}

instruction_encoding smull(
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_result)
{
	return smaddl(variant, reg_op1, reg_op2, x_zr, reg_result);
}

instruction_encoding smulh(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_result)
{
	return mul_all(
		variant,
		reg_op1,
		reg_op2,
		x_zr,
		reg_result,
		arithmetic_high,
		arithmetic_signed,
		multiply_and_add);
}

instruction_encoding msub(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_subtracted,
	arm_register_t const reg_result)
{
	return mul_all(
		variant,
		reg_op1,
		reg_op2,
		reg_subtracted,
		reg_result,
		arithmetic_standard,
		arithmetic_signed,
		multiply_and_subtract);
}

instruction_encoding mneg(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_result)
{
	return msub(variant, reg_op1, reg_op2, x_zr, reg_result);
}

instruction_encoding umsubl(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_subtracted,
	arm_register_t const reg_result)
{
	return mul_all(
		variant,
		reg_op1,
		reg_op2,
		reg_subtracted,
		reg_result,
		arithmetic_long,
		arithmetic_unsigned,
		multiply_and_subtract);
}

instruction_encoding umnegl(
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_result)
{
	return umsubl(variant, reg_op1, reg_op2, x_zr, reg_result);
}

instruction_encoding smsubl(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_subtracted,
	arm_register_t const reg_result)
{
	return mul_all(
		variant,
		reg_op1,
		reg_op2,
		reg_subtracted,
		reg_result,
		arithmetic_long,
		arithmetic_signed,
		multiply_and_subtract);
}

instruction_encoding snegl(
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	arm_register_t const reg_result)
{
	return smsubl(variant, reg_op1, reg_op2, x_zr, reg_result);
}



static instruction_encoding neg_all(
	encoding_variant_t variant,
	arm_register_t reg_negated,
	and_shift_type_t shift_type,
	immediate_t shift_amount_6bits,
	arm_register_t reg_result,
	setting_flag_t const setting_flag)
{
	uint_fast32_t const sf =
		(variant & 1) << 31;
	uint_fast32_t const op =
		(1) << 30;
	uint_fast32_t const s  =
		(setting_flag & 1) << 29;
	uint_fast32_t const fixed1 =
		(0b01011) << 24;
	uint_fast32_t const shift =
		(shift_type & 0b11) << 22;
	uint_fast32_t const fixed2 =
		(0) << 21;
	uint_fast32_t const rm =
		(reg_negated & REGISTER_MASK) << 16;
	uint_fast32_t const imm6 =
		(shift_amount_6bits & 0b111111) << 10;
	uint_fast32_t const rn =
		(0b11111) << 5;
	uint_fast32_t const rd =
		(reg_result & REGISTER_MASK) << 0;

	return
		(sf | op | s | fixed1 | shift | fixed2 | rm | imm6 | rn | rd);
}

instruction_encoding neg(
	encoding_variant_t variant,
	arm_register_t reg_negated,
	and_shift_type_t shift_type,
	immediate_t shift_amount_6bits,
	arm_register_t reg_result)
{
	return neg_all(
		variant,
		reg_negated, shift_type, shift_amount_6bits,
		set_flag_off);
}

instruction_encoding negs(
	encoding_variant_t variant,
	arm_register_t reg_negated,
	and_shift_type_t shift_type,
	immediate_t shift_amount_6bits,
	arm_register_t reg_result)
{
	return neg_all(
		variant,
		reg_negated, shift_type, shift_amount_6bits,
		set_flag_on);
}

enum negate_or_not {
	negate_not_set,
	negate_set
};
typedef enum negate_or_not negate_or_not_t;

instruction_encoding orr_shifted_register_all(
	encoding_variant_t variant,
	arm_register_t reg_op1,
	arm_register_t reg_op2,
	and_shift_type_t shift_type,
	immediate_t shift_amount_6bits,
	arm_register_t reg_result,
	negate_or_not_t negate_state)
{
	uint_fast32_t const sf =
		(variant & 1)                   << 31;
	uint_fast32_t const opc =
		(0b01)                          << 29;
	uint_fast32_t const fixed1 =
		(0b01010)                       << 24;
	uint_fast32_t const shift =
		(shift_type & 0b11)             << 22;
	uint_fast32_t const n =
		(negate_state & 1)              << 21;
	uint_fast32_t const rm =
		(reg_op2 & REGISTER_MASK)       << 16;
	uint_fast32_t const imm6 =
		(shift_amount_6bits & 0b111111) << 10;
	uint_fast32_t const rn =
		(reg_op1 & REGISTER_MASK)       <<  5;
	uint_fast32_t const rd =
		(reg_result & REGISTER_MASK)    <<  0;

	return
		(sf | opc | fixed1 | shift | n | rm | imm6 | rn | rd);
}

instruction_encoding mov_register(
	encoding_variant_t variant,
	arm_register_t reg_src,
	arm_register_t reg_dst)
{
	/* ARM DDI 0487C.a - C6-796
	 * MOV (register)
	 * 
	 * [...]
	 * 
	 * MOV <Wd>, <Wm>
	 * 
	 * is equivalent to
	 * 
	 * ORR <Wd>, WZR, <Wm>
	 * 
	 * [...]
	 * 
	 * MOV <Xd>, <Xm>
	 * 
	 * is equivalent to
	 * 
	 * ORR <Xd>, XZR, <Xm>
	 */
	return orr_shifted_register_all(
		variant,
		reg_zr,
		reg_src,
		0b00, 0,
		reg_dst,
		negate_not_set);
}

instruction_encoding mvn_shifted_register(
	encoding_variant_t variant,
	arm_register_t reg_src,
	and_shift_type_t src_shift_type,
	immediate_t src_shift_amount_6bits,
	arm_register_t reg_dst)
{
	return orr_shifted_register_all(
		variant,
		reg_zr,
		reg_src, src_shift_type, src_shift_amount_6bits,
		reg_dst);
}

enum mov_special_type {
	mt_movn,
	mt_movz,
	mt_movw,
	mt_movk,
};

static instruction_encoding mov_special(
	encoding_variant_t variant,
	immediate_t src_value_16bits,
	immediate_t src_shift_by_16,
	arm_register_t reg_dst,
	mov_special_type type)
{
	uint_fast32_t const sf = 
		(variant & 1) << 31;
	uint_fast32_t const opc =
		(mov_special_type & 0b11) << 29;
	uint_fast32_t const fixed =
		(0b100101) << 23;
	/* The following check is not performed :
	 * 'if (!sf && (hw & 0b10)) error();'
	 */
	uint_fast32_t const hw =
		(src_shift_by_16 & 0b11) << 21;
	uint_fast32_t const imm16 =
		(src_value_16bits & 0b1111111111111111) << 5;
	uint_fast32_t const rd =
		(reg_dst & REGISTER_MASK) << 0;

	return
		(sf | opc | fixed | hw | imm16 | rd);
}

instruction_encoding movk(
	encoding_variant_t variant,
	immediate_t src_value_16bits,
	immediate_t src_shift_by_16,
	arm_register_t reg_dst)
{
	return mov_special(
		variant,
		src_value_16bits,
		src_shift_by_16,
		reg_dst,
		mt_movk);
}

instruction_encoding movn(
	encoding_variant_t variant,
	immediate_t src_value_16bits,
	immediate_t src_shift_by_16,
	arm_register_t reg_dst)
{
	return mov_special(
		variant,
		src_value_16bits,
		src_shift_by_16,
		reg_dst,
		mt_movn);
}

instruction_encoding movw(
	encoding_variant_t variant,
	immediate_t src_value_16bits,
	immediate_t src_shift_by_16,
	arm_register_t reg_dst)
{
	return mov_special(
		variant,
		src_value_16bits,
		src_shift_by_16,
		reg_dst,
		mt_movw);
}

instruction_encoding movz(
	encoding_variant_t variant,
	immediate_t src_value_16bits,
	immediate_t src_shift_by_16,
	arm_register_t reg_dst)
{
	return mov_special(
		variant,
		src_value_16bits,
		src_shift_by_16,
		reg_dst,
		mt_movz);
}

instruction_encoding neg(
	)
{
	return sub_shifted_register_all(
		set_flag_off);
}

instruction_encoding negs(
	)
{
	return sub_shifted_register_all(
		set_flag_on);
}

instruction_encoding ngc(
	)
{
	return sbc_with_carry_all(
		set_flag_off);
}

instruction_encoding ngcs(
	)
{
	return sbc_with_carry_all(
		set_flag_on);
}

static instruction_encoding sub_extended_register_all(
	encoding_variant_t variant,
	arm_register_t reg_op1,
	arm_register_t reg_op2,
	zero_extend_t extend_option,
	immediate_t left_shift_amount,
	arm_register_t reg_result,
	setting_flag_t const setting_flag)
{
	uint_fast32_t const sf =
		(variant & 1) << 31;
	uint_fast32_t const op =
		(1) << 30;
	uint_fast32_t const s  =
		(setting_flag & 1) << 29;
	uint_fast32_t const fixed =
		(0b01011001) << 21;
	uint_fast32_t const rm =
		(reg_op2 & REGISTER_MASK) << 16;
	uint_fast32_t const option = extend_option;
	if (option == ze_lsl)
		option = (variant == encoding_64_bits) ? ze_uxtx : ze_uxtw;
	option = (option & 0b111) << 13;
	uint_fast32_t const imm3 =
		(left_shift_amount & 0b111) << 10;
	uint_fast32_t const rn =
		(reg_op1 & REGISTER_MASK) << 5;
	uint_fast32_t const rd =
		(reg_result & REGISTER_MASK) << 0;

	return
		(sf | op | s | fixed | rm | option | imm3 | rn | rd);
}

instruction_encoding sub_extended_register(
	encoding_variant_t variant,
	arm_register_t reg_op1,
	arm_register_t reg_op2,
	zero_extend_t extend_option,
	immediate_t left_shift_amount,
	arm_register_t reg_result)
{
	return sub_extended_register_all(
		variant,
		reg_op1,
		reg_op2, extend_option, left_shift_amount,
		reg_result,
		set_flag_off);
}

instruction_encoding subs_extended_register(
	encoding_variant_t variant,
	arm_register_t reg_op1,
	arm_register_t reg_op2,
	zero_extend_t extend_option,
	immediate_t left_shift_amount,
	arm_register_t reg_result)
{
	return sub_extended_register_all(
		variant,
		reg_op1,
		reg_op2, extend_option, left_shift_amount,
		reg_result,
		set_flag_on);
}

static instruction_encoding sub_immediate_register_all(
	encoding_variant_t variant,
	arm_register_t reg_op1,
	immediate_t immediate_op2_12bits,
	immediate_t left_shift_by_12,
	arm_regisetr_t reg_result,
	setting_flag_t const setting_flag)
{
	uint_fast32_t const sf =
		(variant & 1) << 31;
	uint_fast32_t const op =
		(1) << 30;
	uint_fast32_t const s  =
		(setting_flag & 1) << 29;
	uint_fast32_t const fixed =
		(0b10001) << 24;
	uint_fast32_t const shift =
		(left_shift_by_12 & 0b01) << 22;
	uint_fast32_t const imm12 =
		(immediate_op2_12bits & 0b111111111111) << 10;
	uint_fast32_t const rn =
		(reg_op1 & REGISTER_MASK) << 5;
	uint_fast32_t const rd =
		(reg_result & REGISTER_MASK) << 0;

	return
		(sf | op | s | fixed | shift | imm12 | rn | rd);
}

instruction_encoding sub_immediate_register(
	encoding_variant_t variant,
	arm_register_t reg_op1,
	immediate_t immediate_op2_12bits,
	immediate_t left_shift_by_12,
	arm_regisetr_t reg_result)
{
	return sub_immediate_register_all(
		variant,
		reg_op1,
		immediate_op2_12bits,
		left_shift_by_12,
		reg_result,
		set_flag_off);
}

instruction_encoding subs_immediate_register(
	encoding_variant_t variant,
	arm_register_t reg_op1,
	immediate_t immediate_op2_12bits,
	immediate_t left_shift_by_12,
	arm_regisetr_t reg_result)
{
	return sub_immediate_register_all(
		variant,
		reg_op1,
		immediate_op2_12bits,
		left_shift_by_12,
		reg_result,
		set_flag_on);
}


static instruction_encoding sub_shifted_register_all(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	and_shift_type_t const shift_type,
	immediate_t const shift_amount_6bits,
	arm_register_t const reg_dst,
	setting_flag_t const setting_flag)
{
	uint_fast32_t const sf =
		(variant & 1) << 31;
	uint_fast32_t const op =
		(1) << 30;
	uint_fast32_t const s  =
		(setting_flag & 1) << 29;
	uint_fast32_t const fixed1 =
		(0b01011) << 24;
	uint_fast32_t shift = shift_type;
	shift = ((shift < 0b11) ? shift : and_shift_lsl) << 22;

	uint_fast32_t const fixed2 =
		(0) << 21;
	uint_fast32_t const rm =
		(reg_op2 & REGISTER_MASK) << 16;
	uint_fast32_t const imm6 =
		(shift_amount_6bits & 0b111111) << 10;
	uint_fast32_t const rn =
		(reg_op1 & REGISTER_MASK) << 5;
	uint_fast32_t const rd =
		(reg_result & REGISTER_MASK) << 0;

	return
		(sf | op | s | fixed1 | shift | fixed2 | rm | imm6 | rn | rd);
}

instruction_encoding sub_shifted_register(
	encoding_variant_t const variant,
	arm_register_t const reg_op1,
	arm_register_t const reg_op2,
	and_shift_type_t const shift_type,
	immediate_t const shift_amount_6bits,
	arm_register_t const reg_dst)
{
	return sub_shifted_register_all(
		variant,
		reg_op1,
		reg_op2,
		shift_type,
		shift_amount_6bits,
		reg_dst,
		set_flag_off);
}

instruction_encoding subs_shifted_register(
	encoding_variant_t variant,
	arm_register_t reg_op1,
	arm_register_t reg_op2,
	and_shift_type_t shift_type,
	immediate_t shift_amount_6bits,
	arm_register_t reg_dst)
{
	return sub_shifted_register_all(
		variant,
		reg_op1,
		reg_op2,
		shift_type,
		shift_amount_6bits,
		reg_dst,
		set_flag_on);
}


static instruction_encoding sbc_all(
	encoding_variant_t const variant,
	arm_register_t reg_op1,
	arm_register_t reg_op2,
	arm_register_t reg_result,
	setting_flag_t const setting_flag)
{
	uint_fast32_t const sf =
		(variant & 1) << 31;
	uint_fast32_t const op =
		(1) << 30;
	uint_fast32_t const s  =
		(setting_flag & 1) << 29;
	uint_fast32_t const fixed1 =
		(0b11010000) << 21;
	uint_fast32_t const rm =
		(reg_op2 & REGISTER_MASK) << 16;
	uint_fast32_t const fixed2 =
		(0b000000) << 10;
	uint_fast32_t const rn =
		(reg_op1 & REGISTER_MASK) << 5;
	uint_fast32_t const rd =
		(reg_result & REGISTER_MASK) << 0;

	return
		(sf | op | s | fixed1 | rm | fixed2 | rn | rd);
}

instruction_encoding sbc(
	encoding_variant_t const variant,
	arm_register_t reg_op1,
	arm_register_t reg_op2,
	arm_register_t reg_result)
{
	return sbc_all(
		variant,
		reg_op1,
		reg_op2,
		reg_result,
		set_flag_off);
}

instruction_encoding sbcs(
	encoding_variant_t const variant,
	arm_register_t reg_op1,
	arm_register_t reg_op2,
	arm_register_t reg_result)
{
	return sbc_all(
		variant,
		reg_op1,
		reg_op2,
		reg_result,
		set_flag_on);
}

instruction_encoding svc(
	immediate_t system_call_number)
{
	uint_fast32_t fixed1 =
		0b11010100000 << 21;
	uint_fast32_t imm16 =
		(system_call_number & 0b111111111111) << 5;
	uint_fast32_t fixed2 =
		0b00001 << 0;

	return
		(fixed1 | imm16 | fixed2);
}

