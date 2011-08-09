/*
 * kbc.c
 *
 *  Created on: 9 Aug 2011
 *      Author: Chris Stones
 */

#include <arch/arch.h>

struct kbc_buffer_struct
{
	struct ticket_lock lock;
	uint8_t buffer[32];
	size_t   buffersize;
	uint16_t writepos;
	uint16_t readpos;
};

/*** todo - put this on the heap? ***/
static struct kbc_buffer_struct _kbc_buffer;
static struct kbc_buffer_struct *kbc_buffer = &_kbc_buffer;

void kcb_initialise() {

	kbc_buffer = &_kbc_buffer;
	memset(kbc_buffer,0, sizeof *kbc_buffer);
	kbc_buffer->buffersize = sizeof(kbc_buffer->buffer);
}


static size_t _kbc_bytes_in_buffer() {

	if(kbc_buffer->writepos >= kbc_buffer->readpos)
		return kbc_buffer->writepos - kbc_buffer->readpos;

	return kbc_buffer->writepos + (kbc_buffer->buffersize - kbc_buffer->readpos);
}

size_t kbc_bytes_in_buffer() {

	ticket_lock_wait(kbc_buffer->lock);

	size_t result = _kbc_bytes_in_buffer();

	ticket_lock_signal(kbc_buffer->lock);

	return (result);
}

static sint32_t _kbc_readchar() {

	if(!_kbc_bytes_in_buffer())
		return -1;

	sint32_t ret = (sint32_t)(kbc_buffer->buffer[kbc_buffer->readpos]);

	kbc_buffer->readpos ++;
	kbc_buffer->readpos %= kbc_buffer->buffersize;

	return (ret);
}

sint32_t kbc_readchar() {

	ticket_lock_wait(kbc_buffer->lock);

	sint32_t result = _kbc_readchar();

	ticket_lock_signal(kbc_buffer->lock);

	return (result);
}

sint32_t _kbc_read(void* _dst, size_t size) {

	sint32_t c = 0;
	uint8_t *dst = (uint8_t*)_dst;
	sint32_t i = 0;

	while(size-- && ((c = _kbc_readchar) >= 0))
		dst[i++] = (uint8_t)c;

	return (i);
}

sint32_t kbc_read(void* dst, size_t size) {

	ticket_lock_wait(kbc_buffer->lock);

	sint32_t result = _kbc_read(dst,size);

	ticket_lock_signal(kbc_buffer->lock);

	return (result);
}

void kbc_irq() {

	ticket_lock_wait(kbc_buffer->lock);

	uint8_t b = port_inb(0x60);

	kbc_buffer->buffer[kbc_buffer->writepos] = b;

	if(kbc_buffer->readpos == kbc_buffer->writepos) {

		/*** BUFFER UNDERFLOW! - LOST A CHARACTER! ***/

		kbc_buffer->readpos ++;
		kbc_buffer->readpos %= kbc_buffer->buffersize;
	}

	kbc_buffer->writepos ++;
	kbc_buffer->writepos %= kbc_buffer->buffersize;

	ticket_lock_signal(kbc_buffer->lock);
}


