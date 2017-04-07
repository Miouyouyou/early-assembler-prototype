#ifndef MYY_SECTIONS_TEXT_H
#define MYY_SECTIONS_TEXT_H 1

#include <stdint.h>

struct text_frame_metadata {
	uint32_t const id;
	uint32_t base_address;
	uint32_t stored_instructions;
	uint32_t max_instructions;
	/* This will soon be removed and linked to the ID somewhere else.
	 * The rationale is that :
	 * - a frame can easily have multiple names.
	 * - Which and how names are handled depend on the binary format
	 *   used, and the user settings of course.
	 */
	uint8_t * name;  
};

struct text_frame_alloc_status {
	unsigned int allocated;
	void * addr;
};

struct text_frame * generate_text_frame();

void frame_set_name
(struct text_frame_metadata * __restrict const text_frame_metadata,
 uint8_t const * __restrict const name);



#endif
