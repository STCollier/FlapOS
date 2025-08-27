#include "util.h"
#include "idt.h"
#include "vga.h"


void kmain(void) {
    memset(VGA, 0x1, 320 * 200); // clear vga mem
    
    klog("Kernel loaded.");
    klog("Load IDT.");
    idt_init();
    
    klog("IDT loaded.");
    __asm__ volatile ("int $1");
    klog("Interrupt 1 test finished.");
    //klog("1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n");
    while(1){__asm__ volatile ("hlt");};

}
