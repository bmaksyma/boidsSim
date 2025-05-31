#ifndef FLUSH_FRAMEBUFFER_H
#define FLUSH_FRAMEBUFFER_H

#include <cstdint>

/**
 * @brief Flush the local framebuffer content to the LCD screen
 * 
 * Sends pixel data from the local framebuffer to the display 
 * 
 * @param parlcd_mem_base Pointer to memory-mapped parlcd registers
 * @param fb Pointer to the framebuffer
 */
void flushFramebuffer(unsigned char* parlcd_mem_base, unsigned short* fb);

#endif
