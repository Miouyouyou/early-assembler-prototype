#include <sections/data.h>
#include <assert.h>

#include <stdio.h>
#include <string.h>

#define LOG(...) fprintf(stderr, __VA_ARGS__)

void assert_data_copy
(struct data_section const * __restrict const data_section,
 struct data_symbol const * __restrict const metadata,
 uint8_t const * __restrict const compared_string,
 uint32_t const original_data_size
)
{
	assert(
		memcmp(metadata->data, compared_string, original_data_size) == 0
	);
}
 
void assert_symbol_infos
(struct data_section const * __restrict const data_section,
 uint32_t const id,
 uint32_t const expected_alignment,
 uint32_t const expected_data_size,
 uint8_t const * __restrict const expected_name,
 uint8_t const * __restrict const expected_data)
{
	struct symbol_found metadata =
		get_data_symbol_infos(data_section, id);
	assert(metadata.found);
	struct data_symbol const * __restrict const symbol_metadata =
		metadata.address;
	
	assert(symbol_metadata->align == expected_alignment);
	assert(symbol_metadata->size  == expected_data_size);
	// We'll need UTF-8 strings with size informations very soon !
	assert(strcmp(symbol_metadata->name, expected_name) == 0);
	assert_data_copy(
		data_section, symbol_metadata, expected_data, expected_data_size
	);
}
 
void assert_symbol_data_with_id
(struct data_section * __restrict const data_section,
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
(struct data_section * __restrict const data_section,
 uint8_t const * __restrict const data,
 uint32_t data_size,
 uint8_t const * __restrict const name)
{

	uint32_t expected_count = data_section->stored + 1;
	
	struct data_section_symbol_added symbol = data_section_add(
		data_section, 4, data_size, name, data
	);
	
	assert(symbol.added);
	uint32_t id = symbol.id;
	
	struct symbol_found const result =
		get_data_symbol_infos(data_section, id);
	assert(result.found == 1);
	struct data_symbol const * __restrict const added_symbol =
		result.address;
	
	assert(data_section->stored == expected_count);
	assert(added_symbol->name == name);
	assert(added_symbol->size == data_size);
	assert_data_copy(data_section, added_symbol, data, data_size);

	
	return id;
}

void assert_symbol_still_there
(struct data_section const * __restrict const data_section,
 unsigned int const id)
{
	assert(get_data_symbol_infos(data_section, id).found);
}


void assert_symbol_not_there
(struct data_section const * __restrict const data_section,
 unsigned int const id)
{
	assert(!get_data_symbol_infos(data_section, id).found);
}

void test_add_data() {
	uint8_t test_string[] = "1234567890";
	uint8_t test_string_name[] = "123";
	
	uint8_t test_string2[] = "POUIPPOUIP POUIPPOUIP\n";
	uint8_t test_string2_name[] = "fezfzej";

	struct data_symbol symbols[10];
	struct data_section data_section = {
		.symbols = symbols,
		.stored = 0,
		.base_address = 0x1000,
		.max_symbols_before_realloc = 10,
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

	struct data_symbol symbols[10];
	struct data_section data_section = {
		.symbols = symbols,
		.stored = 0,
		.base_address = 0x1000,
		.next_id = 0,
		.max_symbols_before_realloc = 10
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
	
	assert_symbol_still_there(&data_section, 0);
	assert_symbol_still_there(&data_section, 1);
	assert_symbol_still_there(&data_section, 2);
	
	delete_data_symbol(&data_section, 1);
	
	assert_symbol_still_there(&data_section, 0);
	assert_symbol_not_there(&data_section, 1);
	assert_symbol_still_there(&data_section, 2);
	
	assert(data_section.stored == 2);
	
	assert_symbol_data_with_id(
		&data_section, 0, test_string, sizeof(test_string), test_string_name
	);
	
	assert_symbol_data_with_id(
		&data_section, 2, test_string3, sizeof(test_string3),
		test_string2_name
	);
	
	delete_data_symbol(&data_section, 0);
	
	assert_symbol_not_there(&data_section, 0);
	assert_symbol_not_there(&data_section, 1);
	assert_symbol_still_there(&data_section, 2);
	
	assert_symbol_data_with_id(
		&data_section, 2, test_string3, sizeof(test_string3),
		test_string2_name
	);
	
	assert(data_section.stored == 1);
	
	delete_data_symbol(&data_section, 2);
	
	assert_symbol_not_there(&data_section, 0);
	assert_symbol_not_there(&data_section, 1);
	assert_symbol_not_there(&data_section, 2);
	
	assert(data_section.stored == 0);
}

void test_exchange_data() {
	struct data_symbol symbols[10];
	struct data_section data_section = {
		.symbols = symbols,
		.stored = 0,
		.base_address = 0x1000,
		.next_id = 0,
		.max_symbols_before_realloc = 10
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
	
	uintptr_t id0_address_before = (uintptr_t) get_data_symbol_infos(
		&data_section, 0
	).address;
	uintptr_t id2_address_before = (uintptr_t) get_data_symbol_infos(
		&data_section, 2
	).address;
	
	exchange_symbols_order(&data_section, 2, 0);
	
	uintptr_t id0_address_after = (uintptr_t) get_data_symbol_infos(
		&data_section, 0
	).address;
	uintptr_t id2_address_after = (uintptr_t) get_data_symbol_infos(
		&data_section, 2
	).address;
	
	assert(id2_address_after == id0_address_before);
	assert(id0_address_after == id2_address_before);
	
	assert_symbol_data_with_id(
		&data_section, 0, test_string,
		sizeof(test_string), test_string_name
	);
	
	assert_symbol_data_with_id(
		&data_section, 1, test_string2,
		sizeof(test_string2), test_string2_name
	);
	
	assert_symbol_data_with_id(
		&data_section, 2, test_string3,
		sizeof(test_string3), test_string3_name
	);
	
}

void test_update_data_symbol() {
	struct data_symbol symbols[10];
	struct data_section data_section = {
		.symbols = symbols,
		.stored = 0,
		.base_address = 0x1000,
		.next_id = 0,
		.max_symbols_before_realloc = 10,
	};
	
	uint8_t test_string[] = "UPDATE TEST UPDATE TEST";
	uint8_t test_string2[] = "CRUD principles";
	uint8_t test_string3[] = "Well, next time I'll use a database !";
	uint8_t test_string_name[] = "header";
	uint8_t test_string2_name[] = "crud";
	uint8_t test_string3_name[] = "db";
	
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
	
	uint8_t new_test_string2[] = "Crusty principles !?";
	
	update_data_symbol(
		&data_section, 1, 4, sizeof(new_test_string2), test_string2_name,
		new_test_string2
	);
	
	assert(data_section.stored == 3);
	
	assert_symbol_infos(
		&data_section, 1, 4, sizeof(new_test_string2), test_string2_name,
		new_test_string2
	);
	
	uint8_t new_test_string2_name[] = "CRUST";
	
	update_data_symbol(
		&data_section, 1, 16, sizeof(new_test_string2),
		new_test_string2_name, new_test_string2
	);
	
	assert_symbol_infos(
		&data_section, 1, 16, sizeof(new_test_string2),
		new_test_string2_name, new_test_string2
	);
	
	assert_symbol_infos(
		&data_section, 0, 4, sizeof(test_string),
		test_string_name, test_string
	);
	
	assert_symbol_infos(
		&data_section, 2, 4, sizeof(test_string3),
		test_string3_name, test_string3
	);
	

}

int main() {
	test_add_data();
	test_delete_data();
	test_exchange_data();
	test_update_data_symbol();
	return 0;
}
