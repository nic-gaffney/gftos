extern keyboard_handler
extern wrap_keyboard_handler
wrap_keyboard_handler:
    pushad
    cld
    call keyboard_handler
    popad
    iret
