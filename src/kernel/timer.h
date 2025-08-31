#ifndef TIMER_H
#define TIMER_H

#include "util.h"

// https://wiki.osdev.org/Programmable_Interval_Timer#I/O_Ports
#define PIT0     0x40
#define PIT1     0x41
#define PIT2     0x42
#define PIT_CMD  0x43

void timer_init();

#endif