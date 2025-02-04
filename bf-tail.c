// A bf interpreter, implemented using tail calls.

#include "common.h"

#define TARGET(NAME) \
    __attribute__((preserve_none)) static int \
    NAME(const instruction_t *ip, cell_t tape[], size_t head, size_t size)

#define DISPATCH \
    __attribute__((musttail)) \
    return handlers[(ip + 1)->opcode](ip + 1, tape, head, size);

typedef __attribute__((preserve_none)) int
handler_t(const instruction_t *ip, cell_t tape[], size_t head, size_t size);

static handler_t *handlers[];

#include "cases.h"

static handler_t *handlers[] = {
    ['+']  = INCREMENT,
    ['-']  = DECREMENT,
    ['<']  = MOVE_LEFT,
    ['>']  = MOVE_RIGHT,
    ['[']  = LOOP_BEGIN,
    [']']  = LOOP_END,
    [',']  = READ,
    ['.']  = WRITE,
    ['\0'] = RETURN,
};

int
eval(const instruction_t *ip, cell_t tape[], size_t head, size_t size)
{
    return handlers[ip->opcode](ip, tape, head, size);
}
