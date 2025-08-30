#include "util.h"
#include "pic.h"

void map_pic() {
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
  /*
  outb(PIC_MASTER_BASE, ICW1_INIT | ICW1_ICW4_ENABLE);
  io_wait();
  outb(PIC_SLAVE_BASE, ICW1_INIT | ICW1_ICW4_ENABLE);
  io_wait();
  outb(PIC_MASTER_DATA, PIC_MASTER_OFFSET);
  io_wait(); 
  outb(PIC_SLAVE_DATA, PIC_SLAVE_OFFSET);
  io_wait(); 
  outb(PIC_MASTER_DATA, 1 << 2);
  io_wait();
  outb(PIC_SLAVE_DATA, 2);
  io_wait();
  outb(PIC_MASTER_DATA, ICW4_8086);
  io_wait();
  outb(PIC_SLAVE_DATA,  ICW4_8086);
  io_wait();
  outb(PIC_MASTER_DATA, 0);
  io_wait();
  outb(PIC_SLAVE_DATA, 0)*/;
}
void init_pit(uint32_t freq) {
  uint8_t low = (1193180/freq) & 0xff;
  uint8_t high= ((1193180/freq)>>8)&0xff;
  /*
Bits         Usage

7 and 6      Select channel :
                0 0 = Channel 0
                0 1 = Channel 1
                1 0 = Channel 2
                1 1 = Read-back command (8254 only)
5 and 4      Access mode :
                0 0 = Latch count value command
                0 1 = Access mode: lobyte only
                1 0 = Access mode: hibyte only
                1 1 = Access mode: lobyte/hibyte
3 to 1       Operating mode :
                0 0 0 = Mode 0 (interrupt on terminal count)
                0 0 1 = Mode 1 (hardware re-triggerable one-shot)
                0 1 0 = Mode 2 (rate generator)
                0 1 1 = Mode 3 (square wave generator)
                1 0 0 = Mode 4 (software triggered strobe)
                1 0 1 = Mode 5 (hardware triggered strobe)
                1 1 0 = Mode 2 (rate generator, same as 010b)
                1 1 1 = Mode 3 (square wave generator, same as 011b)
0            BCD/Binary mode: 0 = 16-bit binary, 1 = four-digit BCD
   * */
  outb(PIT_MODE_CONTROL, 0b00110110);
  outb(PIT_CH0, low);
  outb(PIT_CH0, high);

}
