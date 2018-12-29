#include <stdint.h>
#include <elf.h>
#include <sections/data.h>

#include <string.h>

#include <assert.h>

#define INSTRUCTION_SIZE 9*4
#define DATA_SIZE 16

#define CODE_BASE_ADDR 0x10000
#define DATA_BASE_ADDR 0x20000

#include "armv8a-arm.h"

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

Elf64_Ehdr program_header = {
	.e_ident     = {ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	.e_type      = ET_EXEC,
	.e_machine   = EM_AARCH64,
	.e_version   = 1,
	.e_entry     = 0,
	.e_phoff     = sizeof(Elf64_Ehdr),
	.e_shoff     = 0,
	.e_flags     = 0,
	.e_ehsize    = sizeof(Elf64_Ehdr),
	.e_phentsize = sizeof(Elf64_Phdr),
	.e_phnum     = 2, // .data and .text
	.e_shentsize = sizeof(Elf64_Shdr),
	.e_shnum     = 4, // none, .data, .text, .symtab
	.e_shstrndx  = 3, // [0],  [1],   [2],   [3]
};

Elf64_Phdr text_header = {
	.p_type   = PT_LOAD,
	.p_offset = 0,
	.p_vaddr  = 0,
	.p_paddr  = 0,
	.p_filesz = 0,
	.p_memsz  = 0,
	.p_flags  = PF_X | PF_R,
	.p_align  = 0x1000
};

Elf64_Phdr data_header = {
	.p_type   = PT_LOAD,
	.p_offset = 0,
	.p_vaddr  = 0,
	.p_paddr  = 0,
	.p_filesz = 0,
	.p_memsz  = 0,
	.p_flags  = PF_W | PF_R,
	.p_align  = 0x1000
};


uint32_t dot_text[9] = { 0 };
uint8_t dot_data[] = "Meow!";

Elf64_Shdr empty_section = {0};
Elf64_Shdr text_shdr = {
	.sh_name      = 1,
	.sh_type      = SHT_PROGBITS,
	.sh_flags     = SHF_ALLOC | SHF_EXECINSTR,
	.sh_addr      = 0,
	.sh_offset    = 0,
	.sh_size      = 0,
	.sh_info      = 0,
	.sh_addralign = 4,
	.sh_entsize   = 0
};
Elf64_Shdr data_shdr = {
	.sh_name      = 7,  // \0.text\0.data\0 -> [7] -> .data\0
	.sh_type      = SHT_PROGBITS,
	.sh_flags     = SHF_ALLOC | SHF_WRITE,
	.sh_addr      = 0,
	.sh_offset    = 0,
	.sh_size      = 0,
	.sh_info      = 0,
	.sh_addralign = 1,
	.sh_entsize   = 0
};
Elf64_Shdr shstrtab_section = {
	.sh_name = 13, // \0.text\0.data\0.shrstrtab\0 -> [13] -> .shrstrtab
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

uint8_t test_data_string[] =
	"My hamster is rich and can do kung-fu !\n";

typedef uint32_t offset;
offset glbl_offsets[n_elements] = {0};

uint32_t add_binary_data
(enum program_elements element,
 uint32_t storage_offset,
 void const * __restrict const data,
 uint32_t data_size)
{
	glbl_offsets[element] = storage_offset;
	return write_data(
		scratch_space, storage_offset, data, data_size
	);
}

static void setup_text_sections(
	uint8_t * __restrict const elf_binary_data,
	offset const * __restrict const offsets,
	uint32_t const text_base_addr)
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
	unsigned int text_size = sizeof(dot_text);
	offset const physical_text_offset = offsets[element_text_data];
	offset const virtual_text_offset  =
		text_base_addr + physical_text_offset;
	offset const text_end_offset = physical_text_offset+text_size;
		
	Elf64_Phdr * text_phdr =
		(Elf64_Phdr *) (elf_binary_data+offsets[element_text_phdr]);
	text_phdr->p_paddr  = text_base_addr;
	text_phdr->p_vaddr  = text_base_addr;
	text_phdr->p_memsz  = text_end_offset;
	text_phdr->p_filesz = text_end_offset;
	
	Elf64_Shdr * text_shdr =
		(Elf64_Shdr *) (elf_binary_data+offsets[element_text_shdr]);
	text_shdr->sh_addr = virtual_text_offset;
	text_shdr->sh_offset = physical_text_offset;
	text_shdr->sh_size   = text_size;
}

static void setup_data_sections
(uint8_t * __restrict const elf_binary_data,
 offset const * __restrict const offsets,
 uint32_t const data_base_addr)
{
	offset const physical_data_offset = offsets[element_data_data];
	offset const virtual_data_offset =
		data_base_addr+physical_data_offset;
	uint32_t data_size = sizeof(dot_data);
	
	Elf64_Phdr * dh =
		(Elf64_Phdr *) (elf_binary_data+offsets[element_data_phdr]);
	dh->p_offset = physical_data_offset;
	dh->p_vaddr  = virtual_data_offset;
	dh->p_paddr  = virtual_data_offset;
	dh->p_memsz  = data_size;
	dh->p_filesz = data_size;
	
	Elf64_Shdr * dsh =
		(Elf64_Shdr *) (elf_binary_data+offsets[element_data_shdr]);
	dsh->sh_offset = physical_data_offset;
	dsh->sh_addr   = virtual_data_offset;
	dsh->sh_size = data_size;
}

void build_program(void)
{

	memset(&empty_section, 0, sizeof(Elf64_Shdr));
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
	bytes_written = add_binary_data(
		element_text_data, bytes_written,
		dot_text, sizeof(dot_text)
	);
	bytes_written = add_binary_data(
		element_data_data, bytes_written,
		dot_data, sizeof(dot_data)
	);
	bytes_written = add_binary_data(
		element_empty_shdr, bytes_written,
		&empty_section, sizeof(empty_section)
	);
	bytes_written = add_binary_data(
		element_text_shdr, bytes_written,
		&text_shdr, sizeof(text_shdr)
	);
	bytes_written = add_binary_data(
		element_data_shdr, bytes_written,
		&data_shdr, sizeof(data_shdr)
	);
	bytes_written = add_binary_data(
		element_shstrtab_shdr, bytes_written,
		&shstrtab_section, sizeof(shstrtab_section)
	);
	bytes_written = add_binary_data(
		element_shstrtab_data, bytes_written,
		&section_names, sizeof(section_names)
	);
	
	Elf64_Ehdr * header = (Elf64_Ehdr *) scratch_space;
	header->e_shoff = glbl_offsets[element_empty_shdr];
	header->e_entry = CODE_BASE_ADDR+glbl_offsets[element_text_data];
	
	setup_text_sections(
		scratch_space, glbl_offsets,
		CODE_BASE_ADDR
	);
	setup_data_sections(
		scratch_space, glbl_offsets, DATA_BASE_ADDR
	);

	
	Elf64_Shdr * shstrtab_shdr =
		(Elf64_Shdr *) (scratch_space+glbl_offsets[element_shstrtab_shdr]);
	shstrtab_shdr->sh_size    = sizeof(section_names);
	shstrtab_shdr->sh_offset  = glbl_offsets[element_shstrtab_data];

	int fd = open("executable", O_WRONLY|O_CREAT|O_TRUNC, 00755);
	if (fd != -1) {
		write(fd, scratch_space, bytes_written);
		close(fd);
	}
}

uint32_t dot_text_model[] = {
	0x90000081, // adrp x1, PC+(0b10000 * 4096)
	0xd2800482, // mov  x2, #0x24
	0xd2800020, // mov  x0, #0x1
	0x91035021, // add  x1, x1, #0xd4
	0xd2800808, // mov  x8, #0x40
	0xd4000001, // svc  #0x0
	0xd2800000, // mov  x0, #0x0
	0xd2800ba8, // mov  x8, #0x5d
	0xd4000001, // svc  #0x0
};

void print_buffer_32(
	uint32_t const * __restrict const buffer,
	size_t buffer_size,
	char const * __restrict const buffer_name)
{
	char const * __restrict const printed_name =
		(buffer_name != NULL) ? buffer_name : "__anon_buffer";
	printf("uint32_t %s[%zu] = {", printed_name, buffer_size);
	for (size_t i = 0; i < buffer_size; i++) {
		printf("\n\t0x%08x,", buffer[i]);
	}
	printf("\n};\n");
}
int main() {
	dot_text[0] = adrp(16 /* x 4096 */,  x1);
	dot_text[1] = movz(encoding_64_bits, 0x24, 0, x2);
	dot_text[2] = movz(encoding_64_bits, 1,    0, x0);
	dot_text[3] = add_immediate(encoding_64_bits, x1, 0xd4, 0, x1);
	dot_text[4] = movz(encoding_64_bits, 0x40, 0, x8);
	dot_text[5] = svc(0);
	dot_text[6] = movz(encoding_64_bits, 0,    0, x0);
	dot_text[7] = movz(encoding_64_bits, 0x5d, 0, x8);
	dot_text[8] = svc(0);
	print_buffer_32(dot_text, sizeof(dot_text)/sizeof(uint32_t), "dot_text");
	build_program();
	for (enum program_elements element = element_elf_header;
	     element < n_elements; element++) {
		printf("%x\n", glbl_offsets[element]);
	}
	return 0;
}
