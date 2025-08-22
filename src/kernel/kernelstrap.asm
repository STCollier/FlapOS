global _start
[extern kmain]
_start:
[bits 32]
; for some reason this is still just sliding past and going to the test case? idk this stinks
call kmain
jmp $ ; idk why the kernel would do this but whatevera