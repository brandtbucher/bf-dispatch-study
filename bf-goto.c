// A bf interpreter, implemented using "labels as values".

#include "common.h"

#define TARGET(NAME) NAME:
#define DISPATCH     goto *labels[(++ip)->opcode];

int
eval(const instruction_t *ip, unsigned char tape[], size_t head, size_t size)
{
    static void *labels[] = {
        ['+'] = &&INCREMENT,
        ['-'] = &&DECREMENT,
        ['<'] = &&MOVE_LEFT,
        ['>'] = &&MOVE_RIGHT,
        ['['] = &&LOOP_BEGIN,
        [']'] = &&LOOP_END,
        [','] = &&READ,
        ['.'] = &&WRITE,
        ['\0'] = &&RETURN,
    };
    goto *labels[ip->opcode];
    #include "cases.h"
}
