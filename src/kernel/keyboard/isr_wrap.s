%macro isr_wrap 1
extern %1
extern wrap_%1
wrap_%1:
    pushad
    cld
    call %1
    popad
    iret
%endmacro

isr_wrap keyboard_handler
