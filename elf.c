#include <stdint.h>
#include <elf.h>
#include <armv7-arm.h>
#include <sections/data.h>

#include <string.h>

#define INSTRUCTION_SIZE 9*4
#define DATA_SIZE 16

#define CODE_BASE_ADDR 0x10000
#define DATA_BASE_ADDR 0x20000

enum program_elements {
	element_elf_header,
	element_text_phdr,
	element_data_phdr,
	element_text_data,
	element_data_data,
	element_empty_shdr,
	element_text_shdr,
	element_data_shdr,
	element_shstrtab_shdr,
	element_shstrtab_data,
	n_elements,
	element_start_phdrs = element_text_phdr,
	element_start_text  = element_text_data,
	element_end_text    = element_data_data,
	element_start_shdrs = element_empty_shdr
};

Elf32_Ehdr program_header = {
	.e_ident     = {127, 69, 76, 70, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	.e_type      = ET_EXEC,
	.e_machine   = EM_ARM,
	.e_version   = 1,
	.e_entry     = 0,
	.e_phoff     = sizeof(Elf32_Ehdr),
	.e_shoff     = 0,
	.e_flags     = 0x5000202,
	.e_ehsize    = sizeof(Elf32_Ehdr),
	.e_phentsize = sizeof(Elf32_Phdr),
	.e_phnum     = 2,
	.e_shentsize = sizeof(Elf32_Shdr),
	.e_shnum     = 4,
	.e_shstrndx  = 3,
};

Elf32_Phdr text_header = {
	.p_type   = PT_LOAD,
	.p_offset = 0,
	.p_vaddr  = 0,
	.p_paddr  = 0,
	.p_filesz = 0,
	.p_memsz  = 0,
	.p_flags  = PF_X | PF_R,
	.p_align  = 0x1000
};

Elf32_Phdr data_header = {
	.p_type   = PT_LOAD,
	.p_offset = 0,
	.p_vaddr  = 0,
	.p_paddr  = 0,
	.p_filesz = 0,
	.p_memsz  = 0,
	.p_flags  = PF_W | PF_R,
	.p_align  = 0x1000
};

Elf32_Shdr empty_section = {0};
Elf32_Shdr text_section = {
	.sh_name  = 1,
	.sh_type  = SHT_PROGBITS,
	.sh_flags = SHF_ALLOC | SHF_EXECINSTR,
	.sh_addr  = 0,
	.sh_offset = 0,
	.sh_size   = 0,
	.sh_info   = 0,
	.sh_addralign = 4,
	.sh_entsize = 0
};
Elf32_Shdr data_section = {
	.sh_name      = 7,
	.sh_type      = SHT_PROGBITS,
	.sh_flags     = SHF_ALLOC | SHF_WRITE,
	.sh_addr      = 0,
	.sh_offset    = 0,
	.sh_size      = 0,
	.sh_info      = 0,
	.sh_addralign = 1,
	.sh_entsize   = 0
};
Elf32_Shdr shstrtab_section = {
	.sh_name = 13,
	.sh_type = SHT_STRTAB,
	.sh_addralign = 1,
};

static uint8_t scratch_space[10000];

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define N_TEST_INSTS 10
uint32_t test_machine_code[50] = {0};
uint8_t test_data[1000] = {0};
static struct data_symbol test_symbols[10] = {0};
static struct data_section test_data_section = {
	.symbols = test_symbols,
	.stored = 0,
	.base_address = 0x20000,
	.max_symbols_before_realloc = 10
};
static uint8_t test_data_string[] = "My hamster is rich and can do kung-fu !\n";
static uint8_t test_data_string_name[] = "meow";

uint8_t section_names[] = { "\0.text\0.data\0.shstrtab\0" };

uint32_t write_data
(uint8_t * __restrict const storage,
 uint32_t storage_offset,
 void const * __restrict const data,
 uint32_t data_size)
{
	memcpy(storage+storage_offset, data, data_size);
	return storage_offset + data_size;
}

void prepare_test_code_and_data
(struct instructions * __restrict const insts,
 struct data_section * __restrict const data_section)
{
	struct instruction_representation * __restrict const converted =
		insts->converted;
	
	uint32_t data_index = data_section_add(
		data_section, 4, sizeof(test_data_string),
		test_data_string_name, test_data_string
	).id;
	
	add_instruction(insts, inst_mov_immediate, r0,  1, 0);
	add_instruction(insts, inst_movw_immediate, r1, data_index, 0);
	add_instruction(insts, inst_movt_immediate, r1, data_index, 0);
	add_instruction(insts, inst_mov_immediate, r2,  data_index, 0);
	add_instruction(insts, inst_mov_immediate, r7,  4, 0);
	add_instruction(insts, inst_svc_immediate, 0,   0,0);
	add_instruction(insts, inst_mov_immediate, r0, 0, 0);
	add_instruction(insts, inst_mov_immediate, r7, 1, 0);
	add_instruction(insts, inst_svc_immediate, 0, 0, 0);
	
	converted[1].args[1].type = arg_data_symbol_address_bottom16;
	converted[2].args[1].type = arg_data_symbol_address_top16;
	converted[3].args[1].type = arg_data_symbol_size;
}

typedef uint32_t offset;

offset glbl_offsets[n_elements] = {0};

uint32_t add_binary_data
(enum program_elements element,
 uint32_t storage_offset,
 void const * __restrict const data,
 uint32_t data_size)
{
	glbl_offsets[element] = storage_offset;
	return write_data(scratch_space, storage_offset, data, data_size);
}

uint32_t prepare_machine_code_section
(enum program_elements element,
 uint32_t storage_offset,
 struct instructions const * __restrict const frame_instructions)
{
	glbl_offsets[element] = storage_offset;
	uint32_t bytes_written = instructions_size(frame_instructions);
	memset(scratch_space+storage_offset, 0, bytes_written);
	return storage_offset + bytes_written;
}

uint32_t write_data_section
(enum program_elements element,
 uint32_t storage_offset,
 struct data_section const * __restrict const data_section)
{
	glbl_offsets[element] = storage_offset;
	uint32_t bytes_written = write_data_section_content(
		data_section, scratch_space+storage_offset
	);
	return storage_offset+bytes_written;
}

static void setup_text_sections
(uint8_t * __restrict const elf_binary_data,
 offset const * __restrict const offsets,
 uint32_t const text_base_addr,
 uint32_t const text_size)
{

	/* It seems conventional that the size defined in the Text Program 
	 * Header, defines where the .text section data stop in the binary
	 * file, instead of the amount of bytes these data really takes in
	 * the binary file.
	 * Also, the offset stays at 0 when this convention applies.
	 * 
	 * Program Headers defining data sections do not seem to follow such
	 * convention.
	 * 
	 * I have no idea if ignoring this convention could break the program
	 * execution, so I'll follow such convention for now.
	 */
	offset const physical_text_offset = offsets[element_text_data];
	offset const virtual_text_offset  =
		text_base_addr + physical_text_offset;
	offset const text_end_offset = physical_text_offset+text_size;
		
	Elf32_Phdr * text_phdr =
		(Elf32_Phdr *) (elf_binary_data+offsets[element_text_phdr]);
	text_phdr->p_paddr  = text_base_addr;
	text_phdr->p_vaddr  = text_base_addr;
	text_phdr->p_memsz  = text_end_offset;
	text_phdr->p_filesz = text_end_offset;
	
	Elf32_Shdr * text_shdr =
		(Elf32_Shdr *) (elf_binary_data+offsets[element_text_shdr]);
	text_shdr->sh_addr = virtual_text_offset;
	text_shdr->sh_offset = physical_text_offset;
	text_shdr->sh_size   = text_size;
}

static void setup_data_sections
(uint8_t * __restrict const elf_binary_data,
 offset const * __restrict const offsets,
 uint32_t const data_base_addr,
 struct data_section * __restrict const data_infos)
{
	offset const physical_data_offset = offsets[element_data_data];
	offset const virtual_data_offset =
		data_base_addr+physical_data_offset;
	uint32_t data_size = data_section_size(data_infos);
	data_infos->base_address = virtual_data_offset;
	
	Elf32_Phdr * dh =
		(Elf32_Phdr *) (elf_binary_data+offsets[element_data_phdr]);
	dh->p_offset = physical_data_offset;
	dh->p_vaddr  = virtual_data_offset;
	dh->p_paddr  = virtual_data_offset;
	dh->p_memsz  = data_size;
	dh->p_filesz = data_size;
	
	Elf32_Shdr * dsh =
		(Elf32_Shdr *) (elf_binary_data+offsets[element_data_shdr]);
	dsh->sh_offset = physical_data_offset;
	dsh->sh_addr   = virtual_data_offset;
	dsh->sh_size = data_size;
}

void build_program
(struct instructions const * __restrict const insts,
 struct data_section * __restrict const data_infos)
{

	memset(&empty_section, 0, sizeof(Elf32_Shdr));
	uint32_t bytes_written = 0;
	bytes_written = add_binary_data(
		element_elf_header, bytes_written, &program_header,
		sizeof(program_header)
	);
	bytes_written = add_binary_data(
		element_text_phdr, bytes_written,
		&text_header, sizeof(text_header)
	);
	bytes_written = add_binary_data(
		element_data_phdr, bytes_written,
		&data_header, sizeof(data_header)
	);
	bytes_written = prepare_machine_code_section(
		element_text_data, bytes_written, insts
	);
	bytes_written = write_data_section(
		element_data_data, bytes_written, data_infos
	);
	bytes_written = add_binary_data(
		element_empty_shdr, bytes_written,
		&empty_section, sizeof(empty_section)
	);
	bytes_written = add_binary_data(
		element_text_shdr, bytes_written,
		&text_section, sizeof(text_section)
	);
	bytes_written = add_binary_data(
		element_data_shdr, bytes_written,
		&data_section, sizeof(data_section)
	);
	bytes_written = add_binary_data(
		element_shstrtab_shdr, bytes_written,
		&shstrtab_section, sizeof(shstrtab_section)
	);
	bytes_written = add_binary_data(
		element_shstrtab_data, bytes_written,
		&section_names, sizeof(section_names)
	);
	
	Elf32_Ehdr * header = (Elf32_Ehdr *) scratch_space;
	header->e_shoff = glbl_offsets[element_empty_shdr];
	header->e_entry = CODE_BASE_ADDR+glbl_offsets[element_text_data];
	
	setup_text_sections(
		scratch_space, glbl_offsets, CODE_BASE_ADDR, instructions_size(insts)
	);
	setup_data_sections(
		scratch_space, glbl_offsets, DATA_BASE_ADDR, data_infos
	);
	
	Elf32_Shdr * shstrtab_shdr = (Elf32_Shdr *) (scratch_space+glbl_offsets[element_shstrtab_shdr]);
	shstrtab_shdr->sh_size    = sizeof(section_names);
	shstrtab_shdr->sh_offset  = glbl_offsets[element_shstrtab_data];
	
	assemble_code(
		data_infos, insts,
		(uint32_t *) (scratch_space+glbl_offsets[element_text_data])
	);
	
	int fd = open("executable", O_WRONLY|O_CREAT|O_TRUNC, 00755);
	if (fd != -1) {
		write(fd, scratch_space, bytes_written);
		close(fd);
	}
}

int main() {
	struct instruction_representation converted[N_TEST_INSTS] = {0};
	struct instructions frame_instructions = {
		.n = 0,
		.converted = converted
	};
	prepare_test_code_and_data(&frame_instructions, &test_data_section);
	build_program(&frame_instructions, &test_data_section);
	for (enum program_elements element = element_elf_header;
	     element < n_elements; element++) {
		printf("%x\n", glbl_offsets[element]);
	}
	return 0;
}
