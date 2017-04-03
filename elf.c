#include <stdint.h>
#include <elf.h>

#include <string.h>

#define INSTRUCTION_SIZE 9*4
#define DATA_SIZE 16

#define CODE_BASE_ADDR 0x10000
#define DATA_BASE_ADDR 0x20000

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


uint8_t data[]          = { "Hellow Meow !\n" };
uint8_t section_names[] = { "\0.text\0.data\0.shstrtab\0" };

uint32_t instructions[] = {
	0xe3a00001,
	0xe3001098,
	0xe3401002,
	0xe3a0200e,
	0xe3a07004,
	0xef000000,
	0xe3a00000,
	0xe3a07001,
	0xef000000
};

struct a {
  Elf32_Word	p_type;			/* Segment type */
  Elf32_Off	p_offset;		/* Segment file offset */
  Elf32_Addr	p_vaddr;		/* Segment virtual address */
  Elf32_Addr	p_paddr;		/* Segment physical address */
  Elf32_Word	p_filesz;		/* Segment size in file */
  Elf32_Word	p_memsz;		/* Segment size in memory */
  Elf32_Word	p_flags;		/* Segment flags */
  Elf32_Word	p_align;		/* Segment alignment */
};


Elf32_Phdr text_header = {
	.p_type   = PT_LOAD,
	.p_offset = 0,
	.p_vaddr  = CODE_BASE_ADDR,
	.p_paddr  = CODE_BASE_ADDR,
	.p_filesz = 0x74+INSTRUCTION_SIZE,
	.p_memsz  = 0x74+INSTRUCTION_SIZE,
	.p_flags  = PF_X | PF_R,
	.p_align  = 0x1000
};

Elf32_Phdr data_header = {
	.p_type   = PT_LOAD,
	.p_offset = 0,
	.p_vaddr  = DATA_BASE_ADDR,
	.p_paddr  = 0,
	.p_filesz = DATA_SIZE,
	.p_memsz  = DATA_SIZE,
	.p_flags  = PF_W | PF_R,
	.p_align  = 0x1000
};


struct shdr
{
  Elf32_Word	sh_name;		/* Section name (string tbl index) */
  Elf32_Word	sh_type;		/* Section type */
  Elf32_Word	sh_flags;		/* Section flags */
  Elf32_Addr	sh_addr;		/* Section virtual addr at execution */
  Elf32_Off	sh_offset;		/* Section file offset */
  Elf32_Word	sh_size;		/* Section size in bytes */
  Elf32_Word	sh_link;		/* Link to another section */
  Elf32_Word	sh_info;		/* Additional section information */
  Elf32_Word	sh_addralign;		/* Section alignment */
  Elf32_Word	sh_entsize;		/* Entry size if section holds table */
};

Elf32_Shdr empty_section = {0};
Elf32_Shdr text_section = {
	.sh_name  = 1,
	.sh_type  = SHT_PROGBITS,
	.sh_flags = SHF_ALLOC | SHF_EXECINSTR,
	.sh_addr  = 0,
	.sh_offset = 0x74,
	.sh_size   = INSTRUCTION_SIZE,
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
	.sh_size      = DATA_SIZE,
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

uint32_t write_data
(uint8_t * __restrict const storage,
 uint32_t storage_offset,
 void const * __restrict const data,
 uint32_t data_size)
{
	memcpy(storage+storage_offset, data, data_size);
	return storage_offset + data_size;
}

uint32_t offsets[n_elements] = {0};

uint32_t add_binary_data
(enum program_elements element,
 uint32_t storage_offset,
 void const * __restrict const data,
 uint32_t data_size)
{
	offsets[element] = storage_offset;
	return write_data(scratch_space, storage_offset, data, data_size);
}

void build_program() {
	memset(&empty_section, 0, sizeof(Elf32_Shdr));
	uint32_t bytes_written = 0;
	bytes_written = add_binary_data(element_elf_header, bytes_written, &program_header, sizeof(program_header));
	bytes_written = add_binary_data(element_text_phdr, bytes_written, &text_header, sizeof(text_header));
	bytes_written = add_binary_data(element_data_phdr, bytes_written, &data_header, sizeof(data_header));
	bytes_written = add_binary_data(element_text_data, bytes_written, &instructions, INSTRUCTION_SIZE);
	bytes_written = add_binary_data(element_data_data, bytes_written, &data, DATA_SIZE);
	bytes_written = add_binary_data(element_empty_shdr, bytes_written, &empty_section, sizeof(empty_section));
	bytes_written = add_binary_data(element_text_shdr, bytes_written, &text_section, sizeof(text_section));
	bytes_written = add_binary_data(element_data_shdr, bytes_written, &data_section, sizeof(data_section));
	bytes_written = add_binary_data(element_shstrtab_shdr, bytes_written, &shstrtab_section, sizeof(shstrtab_section));
	bytes_written = add_binary_data(element_shstrtab_data, bytes_written, &section_names, sizeof(section_names));
	
	Elf32_Ehdr * header = (Elf32_Ehdr *) scratch_space;
	header->e_shoff = offsets[element_empty_shdr];
	header->e_entry = CODE_BASE_ADDR+offsets[element_text_data];
	
	printf("th offset : %d\n", offsets[element_text_phdr]);
	Elf32_Phdr * th = (Elf32_Phdr *) (scratch_space+offsets[element_text_phdr]);
	th->p_paddr = CODE_BASE_ADDR;
	th->p_vaddr = CODE_BASE_ADDR;
	
	Elf32_Shdr * tsh = (Elf32_Shdr *) (scratch_space+offsets[element_text_shdr]);
	tsh->sh_addr = CODE_BASE_ADDR+offsets[element_text_data];
	tsh->sh_offset = offsets[element_text_data];
	
	Elf32_Phdr * dh = (Elf32_Phdr *) (scratch_space+offsets[element_data_phdr]);
	dh->p_offset = offsets[element_data_data];
	dh->p_memsz  = DATA_SIZE;
	dh->p_vaddr  = DATA_BASE_ADDR+offsets[element_data_data];
	dh->p_paddr  = DATA_BASE_ADDR+offsets[element_data_data];
	
	Elf32_Shdr * dsh = (Elf32_Shdr *) (scratch_space+offsets[element_data_shdr]);
	dsh->sh_offset = offsets[element_data_data];
	dsh->sh_addr   = DATA_BASE_ADDR+offsets[element_data_data];
	dsh->sh_size = DATA_SIZE;
	
	Elf32_Shdr * shstrtab_shdr = (Elf32_Shdr *) (scratch_space+offsets[element_shstrtab_shdr]);
	shstrtab_shdr->sh_size    = sizeof(section_names);
	shstrtab_shdr->sh_offset  = offsets[element_shstrtab_data];
	
	int fd = open("executable", O_WRONLY|O_CREAT|O_TRUNC, 00755);
	if (fd != -1) {
		write(fd, scratch_space, bytes_written);
		close(fd);
	}
}

int main() {
	build_program();
	for (enum program_elements element = element_elf_header;
	     element < n_elements; element++) {
		printf("%x\n", offsets[element]);
	}
	return 0;
}
