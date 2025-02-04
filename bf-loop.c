// A bf interpreter, implemented using a switch statement inside of a loop.

#include "common.h"

#define TARGET(NAME) case NAME:
#define DISPATCH     break;

#define INCREMENT  '+'
#define DECREMENT  '-'
#define MOVE_LEFT  '<'
#define MOVE_RIGHT '>'
#define LOOP_BEGIN '['
#define LOOP_END   ']'
#define READ       ','
#define WRITE      '.'
#define RETURN    '\0'

int
eval(const instruction_t *ip, cell_t tape[], size_t head, size_t size)
{
    for (;; ip++) {
        switch (ip->opcode) {
            #include "cases.h"
        }
    }
}
