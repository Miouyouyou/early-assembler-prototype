#include <sections/text.h>
#include <helpers/memory.h>
#include <stddef.h> // offsetof

uint32_t id_counter = 0;

uint32_t generate_unique_id() {
	uint32_t id = id_counter;
	id_counter++;
	return id;
};

void frame_set_name
(struct text_frame_metadata * __restrict const text_frame_metadata,
 uint8_t const * __restrict const name)
{
	text_frame_metadata->name = name;
}
