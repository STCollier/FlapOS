void test() {
    while(1);
    //return;
}
void kmain() {
    //*((int*)0xb8000)=0x07690748;
    __asm__("int $0x13");
    while(1); // dont know how to do rainbow stuff but i would otherwise

}