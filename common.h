#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned char opcode;
    size_t oparg;
} instruction_t;

// Provided by the different bf implementations:
int eval(const instruction_t *ip, unsigned char tape[], size_t head, size_t size);

int grow_left(unsigned char *tape_p[], size_t *head_p, size_t *size_p);
int grow_right(unsigned char *tape_p[], size_t *head_p, size_t *size_p);
