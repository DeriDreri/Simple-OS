unsigned char port_byte_in(unsigned short port){
    unsigned char result;
    __asm__("in %%dx, %%al" : : "a" (result), "d" (port));
    return result;
}

void port_byte_out()