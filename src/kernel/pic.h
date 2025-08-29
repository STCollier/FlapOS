#ifndef PIC_H
#define PIC_H
#define PIC_MASTER_BASE 0x20
#define PIC_SLAVE_BASE 0xA0
#define PIC_MASTER_DATA (PIC_MASTER_BASE + 1)
#define PIC_SLAVE_DATA (PIC_SLAVE_BASE+1)
#define PIC_EOI 0x20
#define PIC_MASTER_OFFSET 0x20
#define PIC_SLAVE_OFFSET (PIC_MASTER_OFFSET+8)
// pic flags in command
#define ICW1_ICW4_ENABLE 0b1
#define ICW1_CASCADE 0b10
#define ICW1_LEVEL	0x08 //Level triggered (edge) mode
#define ICW1_INIT	0x10	

#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

#define PIT_MODE_CONTROL 0x43
#define PIT_CH0 0x40
#define PIT_CH1 0x41
#define PIT_CH2 0x42

void map_pic();
void init_pit(uint32_t freq);

//#define 
#endif
