#include "symbols.h"
#include <stdbool.h>
int getCursorPositionOffset(){
    port_byte_out(REG_SCREEN_CTRL, 14); /* Requesting byte 14: high byte of cursor pos */
    /* Data is returned in VGA data register (0x3d5) */
    int position = port_byte_in(REG_SCREEN_DATA);
    position = position << 8; /* high byte */

    port_byte_out(REG_SCREEN_CTRL, 15); /* requesting low byte */
    position += port_byte_in(REG_SCREEN_DATA);
    int offset_from_vga = position * 2;
    return offset_from_vga;
}

void set_cursor_offset(int offset) {
    /* Similar to get_cursor_offset, but instead of reading we write data */
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

bool is_key_pressed()
{
    // Check the keyboard status port (0x64) to determine if a key is pressed
    // Bit 0 of the status port will be set if there is data available
    unsigned char status = port_byte_in(KEYBOARD_STATUS_PORT);
    return (status & 0x01);  // Check the least significant bit
}

unsigned char read_key()
{
    // Wait for a key press by continuously checking the keyboard status
    while (!is_key_pressed()) {
        for (unsigned int i = 0; i < 1000000; ++i) {
            asm("nop");
        }
    }

    // Read the key code from the data port (0x60)
    unsigned char key = port_byte_in(KEYBOARD_DATA_PORT);
    return key;
}

void wait_for_keypress(unsigned char keyCode)
{   
  
    unsigned char key = 0;
    do {
        for (unsigned int i = 0; i < 100000; ++i) {
            asm("nop"); 
        }
        key = read_key();
    } while (key != keyCode);
}

void wait(){
    wait_for_keypress(0x1C);
}

bool check_if_letter_key(unsigned char key){
    if(key >= 0x10 && key <= 0x19)
        return true;
    if(key >= 0x1e && key <= 0x26)
        return true;
    if(key >= 0x23 && key <= 0x32)
        return true;
    if(key == 0x39)
        return true;
    return false;
}