#include <armv7-arm.h>
#include <sections/data.h>
#include <helpers/numeric.h>
#include <helpers/memory.h>

#include <stddef.h> // offsetof
#include <string.h> // memcpy

static struct instruction_representation
instructions_defaults[n_known_instructions] = {
	[inst_add_immediate] = {
		.mnemonic_id = inst_add_immediate,
		.args = {
			[0] = {
				.type = arg_register,
				.value = r0
			},
			[1] = {
				.type = arg_register,
				.value = r0
			},
			[2] = {
				.type = arg_immediate,
				.value = 0
			}
		}
	},
	[inst_b_address] = {
		.mnemonic_id = inst_b_address,
		.args = {
			[0] = {
				.type = arg_condition,
				.value = cond_al
			},
			[1] = {
				.type = arg_frame_address_pc_relative,
				.value = 0
			},
			[2] = {
				.type = arg_invalid,
				.value = 0
			}
		}
	},
	[inst_bl_address] = {
		.mnemonic_id = inst_bl_address,
		.args = {
			[0] = {
				.type = arg_condition,
				.value = cond_al
			},
			[1] = {
				.type = arg_frame_address_pc_relative,
				.value = 0
			},
			[2] = {
				.type = arg_invalid,
				.value = 0
			}
		}
	},
	[inst_blx_address] = {
		.mnemonic_id = inst_blx_address,
		.args = {
			[0] = {
				.type = arg_condition,
				.value = cond_al
			},
			[1] = {
				.type = arg_frame_address_pc_relative,
				.value = 0
			},
			[2] = {
				.type = arg_invalid,
				.value = 0
			}
		}
	},
	[inst_blx_register] = {
		.mnemonic_id = inst_blx_register,
		.args = {
			[0] = {
				.type = arg_condition,
				.value = cond_al
			},
			[1] = {
				.type = arg_register,
				.value = reg_lr
			},
			[2] = {
				.type = arg_invalid,
				.value = 0
			}
		}
	},
	[inst_bx_register] = {
		.mnemonic_id = inst_bx_register,
		.args = {
			[0] = {
				.type = arg_condition,
				.value = cond_al
			},
			[1] = {
				.type = arg_register,
				.value = reg_lr
			},
			[2] = {
				.type = arg_invalid,
				.value = 0
			}
		}
	},
	[inst_mov_immediate] = {
		.mnemonic_id = inst_mov_immediate,
		.args = {
			[0] = {
				.type = arg_register,
				.value = r0
			},
			[1] = {
				.type = arg_immediate,
				.value = 0
			},
			[2] = {
				.type = arg_invalid,
				.value = 0
			}
		}
	},
	[inst_mov_register] = {
		.mnemonic_id = inst_mov_register,
		.args = {
			[0] = {
				.type = arg_register,
				.value = r4
			},
			[1] = {
				.type = arg_register,
				.value = r0
			},
			[2] = {
				.type = arg_invalid,
				.value = 0
			}
		}
	},
	[inst_movt_immediate] = {
		.mnemonic_id = inst_movt_immediate,
		.args = {
			[0] = {
				.type = arg_register,
				.value = r0
			},
			[1] = {
				.type = arg_data_symbol_address_top16,
				.value = 0
			},
			[2] = {
				.type = arg_invalid,
				.value = 0
			}
		}
	},
	[inst_movw_immediate] = {
		.mnemonic_id = inst_movw_immediate,
		.args = {
			[0] = {
				.type = arg_register,
				.value = r0
			},
			[1] = {
				.type = arg_data_symbol_address_bottom16,
				.value = 0
			},
			[2] = {
				.type = arg_invalid,
				.value = 0
			}
		}
	},
	[inst_mvn_immediate] = {
		.mnemonic_id = inst_mvn_immediate,
		.args = {
			[0] = {
				.type = arg_register,
				.value = r0
			},
			[1] = {
				.type = arg_immediate,
				.value = 1
			},
			[2] = {
				.type = arg_invalid,
				.value = 0
			}
		}
	},
	[inst_pop_regmask] = {
		.mnemonic_id = inst_pop_regmask,
		.args = {
			[0] = {
				.type = arg_regmask,
				.value = 0b1000000011110000
			},
			[1] = {
				.type = arg_invalid,
				.value = 0
			},
			[2] = {
				.type = arg_invalid,
				.value = 0
			}
		}
	},
	[inst_push_regmask] = {
		.mnemonic_id = inst_push_regmask,
		.args = {
			[0] = {
				.type = arg_regmask,
				.value = 0b0100000011110000
			},
			[1] = {
				.type = arg_invalid,
				.value = 0
			},
			[2] = {
				.type = arg_invalid,
				.value = 0
			}
		}
	},
	[inst_sub_immediate] = {
		.mnemonic_id = inst_sub_immediate,
		.args = {
			[0] = {
				.type = arg_register,
				.value = r0,
			},
			[1] = {
				.type = arg_register,
				.value = r0,
			},
			[2] = {
				.type = arg_immediate,
				.value = 0
			}
		}
	},
	[inst_svc_immediate] = {
		.mnemonic_id = inst_svc_immediate,
		.args = {
			[0] = {
				.type = arg_immediate,
				.value = 0,
			},
			[1] = {
				.type = arg_invalid,
				.value = 0
			},
			[2] = {
				.type = arg_invalid,
				.value = 0
			}
		}
	}
};

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
(enum arm_conditions condition, relative_address addr24,
 uint32_t fixed_part_bits)
{
	uint32_t cond       = clamp_condition(condition) << 28;
	uint32_t fixed_part = (fixed_part_bits) << 24;
	uint32_t imm24      = (addr24 >> 2) & 0xffffff;
	
	return cond | fixed_part | imm24;
}


uint32_t op_b_address(enum arm_conditions condition, relative_address addr24)
{
	return branch_instruction(condition, addr24, 0b1010);
}

uint32_t op_bl_address(enum arm_conditions condition, relative_address addr24)
{
	return branch_instruction(condition, addr24, 0b1011);
}

uint32_t op_blx_address(relative_address addr24)
{
	uint32_t fixed_part = 0b1111101 << 25;
	uint32_t h          = addr24 & 1 << 24;
	uint32_t imm24      = ((addr24 >> 1) & 0xffffff) >> 1;

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

uint32_t op_bx_register
(enum arm_conditions condition, enum arm_register addr_reg)
{
	uint32_t cond       = clamp_condition(condition) << 28;
	uint32_t fixed_part = 0b000100101111111111110001 << 4;
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
(struct data_section const * __restrict const symbols,
 struct armv7_text_section const * __restrict const text_section,
 struct instruction_args_infos const * __restrict const args,
 unsigned int const pc)
{
	
	uint32_t values[MAX_ARGS];
	for (unsigned int a = 0; a < MAX_ARGS; a++) {
		uint32_t set_value = args[a].value;
		switch(args[a].type) {
			case arg_invalid:
				values[a] = 0;
				break;
			case arg_condition:
				values[a] = clamp_condition(set_value);
				break;
			case arg_register:
				values[a] = clamp_standard_register(set_value);
				break;
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
			case arg_frame_address:
				values[a] = text_section_frame_address(text_section, set_value);
				break;
			case arg_frame_address_pc_relative: {
					uint32_t address = 
						text_section_frame_address(text_section, set_value);
					// We currently only support ARM mode.
					values[a] = address - pc - 8;
				}
				break;
			case arg_regmask:
				values[a] = set_value;
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

uint32_t (*op_functions[n_known_instructions])() = {
	[inst_add_immediate] = op_add_immediate,
	[inst_b_address]     = op_b_address,
	[inst_bl_address]    = op_bl_address,
	[inst_blx_address]   = op_blx_address,
	[inst_blx_register]  = op_blx_register,
	[inst_bx_register]   = op_bx_register,
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
(struct data_section const * __restrict const data_infos,
 struct instructions const * __restrict const instructions,
 uint32_t * result_code)
{
	unsigned int n_instructions = instructions->n;
	struct instruction_representation * __restrict const internal_insts =
		instructions->converted;
	for (unsigned int i = 0; i < n_instructions; i++) {
		struct args_values values = 
			get_values(data_infos, NULL, internal_insts[i].args, 0);
		result_code[i] = op_functions[internal_insts[i].mnemonic_id](
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
	instructions->converted[new_index].mnemonic_id = id;
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

struct armv7_text_frame * generate_armv7_text_frame
(uint32_t (*id_generator)())
{
	struct armv7_text_frame * __restrict text_frame = NULL;

	unsigned int const n_instructions_default = 128;
	unsigned int const instructions_array_size =
		n_instructions_default * sizeof(struct instruction_representation);
	struct instruction_representation * instructions =
		allocate_durable_memory(instructions_array_size);
		
	if (instructions == NULL) goto cant_allocate_instructions_array;
	
	struct armv7_text_frame const frame_data = {
		.metadata = {
			.id = id_generator(),
			.base_address = 0,
			.stored_instructions = 0,
			.max_instructions = n_instructions_default
		},
		.instructions = instructions
	};
	
	text_frame = allocate_durable_memory(sizeof(struct armv7_text_frame));
	
	if (text_frame != NULL) {
		memcpy(text_frame, &frame_data, sizeof(struct armv7_text_frame));
		memset(instructions, 0, instructions_array_size);
	}
	else free_durable_memory(instructions);

cant_allocate_instructions_array:
	return text_frame;
}

static unsigned int need_more_space_for_instructions_in
(struct armv7_text_frame * __restrict const frame)
{
	return (frame->metadata.stored_instructions ==
	        frame->metadata.max_instructions);
}

static unsigned int allocate_more_space_for_instructions_in
(struct armv7_text_frame * __restrict const frame)
{

	unsigned int current_instructions_space =
		frame->metadata.max_instructions *
		sizeof(struct instruction_representation);
	unsigned int new_instructions_space =
		current_instructions_space * 2;
	unsigned int delta = 
		new_instructions_space - current_instructions_space;
	
	struct instruction_representation * __restrict const new_addr =
		reallocate_durable_memory(
			frame->instructions, new_instructions_space
		);
	
	unsigned int allocated = (new_addr != NULL);
	
	if (allocated) {
		memset(new_addr+current_instructions_space, 0, delta);
		frame->instructions = new_addr;
	}
	
	return allocated;
}

struct armv7_add_instruction_status frame_add_instruction
(struct armv7_text_frame * __restrict const frame)
{
	struct armv7_add_instruction_status status = {
		.added = 0,
		.address = NULL
	};

	if (need_more_space_for_instructions_in(frame))
		if (!allocate_more_space_for_instructions_in(frame))
			goto no_more_space_for_instructions;

	unsigned int new_index = frame->metadata.stored_instructions;
	
	struct instruction_representation * instruction_addr =
		frame->instructions+new_index;
	frame->metadata.stored_instructions += 1;

	status.added = 1;
	status.address = instruction_addr;

no_more_space_for_instructions:
	return status;
}

void instruction_mnemonic_id
(struct instruction_representation * instruction,
 enum known_instructions mnemonic_id)
{
	if (instruction->mnemonic_id != mnemonic_id)
		*instruction = instructions_defaults[mnemonic_id];

}

void instruction_arg
(struct instruction_representation * const instruction,
 unsigned int const index,
 enum argument_type argument_type,
 uint32_t const value)
{
	instruction->args[index].type = argument_type;
	instruction->args[index].value = value;
}

unsigned int armv7_frame_gen_machine_code
(struct armv7_text_frame const * __restrict const frame,
 struct armv7_text_section const * __restrict const section,
 struct data_section const * __restrict const data_infos,
 uint32_t * __restrict const result_code)
{
	unsigned int n_instructions = frame->metadata.stored_instructions;
	struct instruction_representation * __restrict const instructions =
		frame->instructions;

	for (unsigned int i = 0, pc = frame->metadata.base_address;
	     i < n_instructions;
	     i++, pc += 4) {
		struct args_values values = 
			get_values(data_infos, section, instructions[i].args, pc);
		result_code[i] = op_functions[instructions[i].mnemonic_id](
			values.val0, values.val1, values.val2
		);
	}
	return n_instructions * sizeof(uint32_t);
}

uint32_t text_section_frame_address
(struct armv7_text_section const * __restrict const text_section,
 unsigned int const frame_id)
{
	uint32_t address = 0;
	
	unsigned int n_frames = text_section->n_frames_refs;
	struct armv7_text_frame const * const * __restrict const frames =
		text_section->frames_refs;
	
	unsigned int f = 0;
	while(frames[f]->metadata.id != frame_id && f < n_frames) f++;
	
	if (f < n_frames) address = frames[f]->metadata.base_address;
	
	return address;
}

static unsigned int expand_frame_space_of
(struct armv7_text_section * __restrict const text_section)
{
	unsigned int current_refs_size = 
		text_section->max_frames_refs * sizeof(struct armv7_text_frame *);
	unsigned int new_refs_size = current_refs_size * 2;
	
	struct armv7_text_frame ** const new_refs_addr =
		reallocate_durable_memory(text_section->frames_refs, new_refs_size);
		
	unsigned int expanded = (new_refs_addr != NULL);
	
	return expanded;
}

static unsigned int not_enough_frame_space_in
(struct armv7_text_section * __restrict const text_section)
{
	return (text_section->n_frames_refs == text_section->max_frames_refs);
}

struct armv7_text_section * generate_armv7_text_section()
{
	struct armv7_text_section * text_section = NULL;
	unsigned int const n_frames_refs_default = 512;
	
	unsigned int const frames_refs_size =
		n_frames_refs_default * sizeof(struct armv7_text_frame *);
	
	
	struct armv7_text_frame ** const frames_refs =
		allocate_durable_memory(frames_refs_size);
	
	if (frames_refs == NULL) goto cant_allocate_frames_refs_space;
	
	struct armv7_text_section const section_infos = {
		.id = 0,
		.n_frames_refs = 0,
		.max_frames_refs = n_frames_refs_default,
		.base_address = 0,
		.frames_refs = frames_refs
	};
	
	
	text_section = 
		allocate_durable_memory(sizeof(struct armv7_text_section));
	
	if (text_section == NULL) free_durable_memory(frames_refs);
	else memcpy(
		text_section, &section_infos, sizeof(struct armv7_text_section)
	);
	
cant_allocate_frames_refs_space:
	return text_section;
}

void armv7_frame_set_address
(struct armv7_text_frame * __restrict const frame,
 uint32_t const address)
{
	frame->metadata.base_address = address;
}

unsigned int armv7_text_section_add_frame
(struct armv7_text_section * __restrict const text_section,
 struct armv7_text_frame const * __restrict const frame)
{
	unsigned int added = 0;
	if (not_enough_frame_space_in(text_section))
		if (!expand_frame_space_of(text_section))
			goto not_enough_memory_for_new_frame_reference;
	
	unsigned int new_index = text_section->n_frames_refs;
	text_section->frames_refs[new_index] = frame;
	text_section->n_frames_refs = new_index + 1;
	added = 1;
	
not_enough_memory_for_new_frame_reference:
	return added;
}

unsigned int armv7_frame_size
(struct armv7_text_frame const * __restrict const frame)
{
	return frame->metadata.stored_instructions * sizeof(uint32_t);
}

unsigned int armv7_text_section_size
(struct armv7_text_section const * __restrict const text_section)
{
	unsigned int size = 0;
	
	for (unsigned int f = 0; f < text_section->n_frames_refs; f++)
		size += armv7_frame_size(text_section->frames_refs[f]);
	
	return size;
}

void armv7_text_section_rebase_at
(struct armv7_text_section * __restrict const text_section,
 uint32_t const base)
{	
	text_section->base_address = base;

	unsigned int addr = base;
	for (unsigned int f = 0; f < text_section->n_frames_refs; f++) {
		struct armv7_text_frame * __restrict const current_frame =
			text_section->frames_refs[f];
		armv7_frame_set_address(current_frame, addr);
		addr += armv7_frame_size(current_frame);
	}
}

void armv7_text_section_write_at
(struct armv7_text_section const * __restrict const text_section,
 struct data_section const * __restrict const data_section,
 uint8_t * __restrict const output)
{
	unsigned int const base_address = text_section->base_address;
	unsigned int output_cursor = 0;
	
	for (unsigned int f = 0; f < text_section->n_frames_refs; f++) {
		struct armv7_text_frame * __restrict const current_frame =
			text_section->frames_refs[f];
		uint32_t const frame_address = current_frame->metadata.base_address;
		output_cursor = frame_address - base_address;
		armv7_frame_gen_machine_code(
			current_frame, text_section,
			data_section, (uint32_t *) (output+output_cursor)
		);
	}
	
}
