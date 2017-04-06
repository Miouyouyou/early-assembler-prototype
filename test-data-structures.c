#include <armv7-arm.h>
#include <assert.h>

#include <stdio.h>
#include <string.h>

#define LOG(...) fprintf(stderr, __VA_ARGS__)

void assert_data_copy
(struct data_symbols const * __restrict const data_section,
 struct data_symbol const * __restrict const metadata,
 uint8_t const * __restrict const compared_string,
 uint32_t const original_data_size
)
{
	assert(
		memcmp(
			data_section->data+metadata->offset, compared_string,
			original_data_size
		) == 0
	);
}
 
void assert_symbol_data_with_id
(struct data_symbols * __restrict const data_section,
 unsigned int id,
 uint8_t const * __restrict const data,
 uint32_t data_size,
 uint8_t const * __restrict const name)
{
	struct symbol_found metadata_infos = get_data_symbol_infos(
		data_section, id
	);
	assert(metadata_infos.found);
	struct data_symbol const * const symbol_metadata =
		metadata_infos.address;
	
	assert(symbol_metadata->size == data_size);
	assert_data_copy(
		data_section, symbol_metadata, data, data_size
	);
	
}
uint32_t assert_add_symbol
(struct data_symbols * __restrict const data_section,
 uint8_t const * __restrict const data,
 uint32_t data_size,
 uint8_t const * __restrict const name)
{
	uint32_t expected_data_offset =
		round_to(data_section->global_size, 4);
	uint32_t expected_global_size =
		expected_data_offset + data_size;
	uint32_t expected_count = data_section->stored + 1;
	
	uint32_t id = add_data_symbol(
		data_section, data, data_size, 4, name
	);
	
	struct symbol_found const result =
		get_data_symbol_infos(data_section, id);
	assert(result.found == 1);
	struct data_symbol const * __restrict const added_symbol =
		result.address;
	
	assert(data_section->global_size == expected_global_size);
	assert(data_section->stored == expected_count);
	assert(added_symbol->name == name);
	assert(added_symbol->size == data_size);
	assert(added_symbol->offset == expected_data_offset);
	assert_data_copy(data_section, added_symbol, data, data_size);

	
	return id;
}

void test_add_data() {
	uint8_t test_string[] = "1234567890";
	uint8_t test_string_name[] = "123";
	
	uint8_t test_string2[] = "POUIPPOUIP POUIPPOUIP\n";
	uint8_t test_string2_name[] = "fezfzej";
	uint8_t test_data[100];
	struct data_symbol symbols[10];
	struct data_symbols data_section = {
		.data = test_data,
		.symbols = symbols,
		.stored = 0,
		.base_address = 0x1000,
		.global_size = 0,
		.max_size = 100,
		.max_symbols = 10,
		.next_id = 0,
	};
	
	uint8_t id = assert_add_symbol(
		&data_section, test_string, sizeof(test_string), test_string_name
	);
	assert(id == 0);
		
	id = assert_add_symbol(
		&data_section, test_string2, sizeof(test_string2),
		test_string2_name
	);
	
	assert(id == 1);

}
 

void test_delete_data() {
	
	uint8_t test_data[100];
	struct data_symbol symbols[10];
	struct data_symbols data_section = {
		.data = test_data,
		.symbols = symbols,
		.stored = 0,
		.base_address = 0x1000,
		.global_size = 0,
		.max_size = sizeof(test_data),
		.max_symbols = sizeof(symbols)/sizeof(struct data_symbol),
		.next_id = 0,
	};

	uint8_t test_string[] = "DELETE TEST DELETE TEST";
	uint8_t test_string2[] = "TOAST TOAST TOAST";
	uint8_t test_string3[] = "BURNING !!!";
	uint8_t test_string_name[] = "header";
	uint8_t test_string2_name[] = "toast";
	uint8_t test_string3_name[] = "burning";
	
	uint32_t id = assert_add_symbol(
		&data_section, test_string, sizeof(test_string), test_string_name
	);
	assert(id == 0);
	id = assert_add_symbol(
		&data_section, test_string2, sizeof(test_string2), test_string2_name
	);
	assert(id == 1);
	id = assert_add_symbol(
		&data_section, test_string3, sizeof(test_string3), test_string3_name
	);
	assert(id == 2);
	
	assert(data_section.stored == 3);
	
	delete_data_symbol(&data_section, 1);
	
	assert(data_section.stored == 2);
	
	assert_symbol_data_with_id(
		&data_section, 0, test_string, sizeof(test_string), test_string_name
	);
	
	assert_symbol_data_with_id(
		&data_section, 2, test_string3, sizeof(test_string3),
		test_string2_name
	);
	
}

void test_exchange_data() {
	uint8_t test_data[100];
	struct data_symbol symbols[10];
	struct data_symbols data_section = {
		.data = test_data,
		.symbols = symbols,
		.stored = 0,
		.base_address = 0x1000,
		.global_size = 0,
		.max_size = sizeof(test_data),
		.max_symbols = sizeof(symbols)/sizeof(struct data_symbol),
		.next_id = 0,
	};
	
	uint8_t test_string[] = "EXCHANGE TEST EXCHANGE TEST";
	uint8_t test_string2[] = "SWAPPY Burdh??";
	uint8_t test_string3[] = "And remember kids, don't do drugs !";
	uint8_t test_string_name[] = "header";
	uint8_t test_string2_name[] = "swappy";
	uint8_t test_string3_name[] = "warning";
	
	uint32_t id = assert_add_symbol(
		&data_section, test_string, sizeof(test_string), test_string_name
	);
	assert(id == 0);
	id = assert_add_symbol(
		&data_section, test_string2, sizeof(test_string2), test_string2_name
	);
	assert(id == 1);
	id = assert_add_symbol(
		&data_section, test_string3, sizeof(test_string3), test_string3_name
	);
	assert(id == 2);
	
	uint32_t id0_offset_before = get_data_symbol_infos(
		&data_section, 0
	).address->offset;
	uint32_t id2_offset_before = get_data_symbol_infos(
		&data_section, 2
	).address->offset;
	
	exchange_data_symbols(&data_section, 2, 0);
	
	uint32_t id0_offset_after = get_data_symbol_infos(
		&data_section, 0
	).address->offset;
	uint32_t id2_offset_after = get_data_symbol_infos(
		&data_section, 2
	).address->offset;
	
	assert(id0_offset_after != id0_offset_before);
	assert(id2_offset_after != id2_offset_before);
	
	assert_symbol_data_with_id(
		&data_section, 0, test_string, sizeof(test_string), test_string_name
	);
	
	assert_symbol_data_with_id(
		&data_section, 1, test_string2, sizeof(test_string2), test_string2_name
	);
	
	assert_symbol_data_with_id(
		&data_section, 2, test_string3, sizeof(test_string3), test_string3_name
	);
	
	
}

int main() {
	test_add_data();
	test_delete_data();
	test_exchange_data();
	return 0;
}
