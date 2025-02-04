// Utilites and main driver for different bf interpreters.

#include "common.h"

// [HEAD ...] -> [... HEAD ...]
int
grow_left(cell_t *tape_p[], size_t *head_p, size_t *size_p)
{
    size_t size = *size_p;
    cell_t *tape = malloc(2 * size * sizeof(cell_t));
    if (tape == NULL) {
        return -1;
    }
    memset(tape, 0, size);
    memcpy(tape + size, *tape_p, size * sizeof(cell_t));
    free(*tape_p);
    *tape_p = tape;
    *head_p += size;
    *size_p += size;
    return 0;
}

// [... HEAD] -> [... HEAD ...]
int
grow_right(cell_t *tape_p[], size_t *head_p, size_t *size_p)
{
    size_t size = *size_p;
    cell_t *tape = realloc(*tape_p, 2 * size * sizeof(cell_t));
    if (tape == NULL) {
        return -1;
    }
    memset(tape + size, 0, size * sizeof(cell_t));
    *tape_p = tape;
    *size_p += size;
    return 0;
}

static const char opposites[CHAR_MAX] = {
    ['+'] = '-',
    ['-'] = '+',
    ['<'] = '>',
    ['>'] = '<',
};

// Parse the code in file into an array of instruction_ts.
static instruction_t *
parse(FILE *file)
{
    // Keep a linked list of unmatched ['s by offset:
    size_t loop = SIZE_MAX;
    size_t i = 0, size = 1;
    instruction_t *code = malloc(size * sizeof(instruction_t));
    if (code == NULL) {
        return NULL;
    }
    for (int c = fgetc(file); c != EOF; c = fgetc(file)) {
        switch (c) {
            case '+':
            case '-':
            case '<':
            case '>':
                // oparg is the number of repetitions.
                if (i) {
                    instruction_t *previous = &code[i - 1];
                    if (previous->opcode == c) {
                        previous->oparg++;
                        continue;
                    }
                    // Peephole opposite instructions:
                    if (previous->opcode == opposites[c]) {
                        if (--previous->oparg == 0) {
                            i--;
                        }
                        continue;
                    }
                }
                code[i++] = (instruction_t){c, 1};
                break;
            case '[':
                // oparg is the offset of the matching ']'. Push to linked list:
                code[i++] = (instruction_t){c, loop};
                loop = i - 1;
                break;
            case ']':
                // oparg is the offset of the matching '['. Pop from linked list:
                if (loop == SIZE_MAX) {
                    free(code);
                    return NULL;
                }
                size_t oparg = i - loop;
                code[i++] = (instruction_t){c, oparg};
                instruction_t *matching = &code[loop];
                loop = matching->oparg;
                matching->oparg = oparg;
                break;
            case ',':
            case '.':
                // oparg is unused.
                code[i++] = (instruction_t){c, 0};
                break;
            default:
                continue;
        }
        if (i == size) {
            // Grow the code:
            instruction_t *new = realloc(code, 2 * size * sizeof(instruction_t));
            if (new == NULL) {
                free(code);
                return NULL;
            }
            memset(new + size, 0, size);
            size += size;
            code = new;
        }
    }
    code[i++] = (instruction_t){'\0', 0};
    // Check for unmatched ['s:
    if (loop != SIZE_MAX) {
        free(code);
        return NULL;
    }
    return code;
}

int
main(int argc, char *argv[])
{
    if (argc != 2) {
        return -1;
    }
    char *filename = argv[1];
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return -1;
    }
    instruction_t *code = parse(file);
    fclose(file);
    if (code == NULL) {
        return -1;
    }
    size_t head = 0, size = 1;
    cell_t *tape = calloc(size, sizeof(cell_t));
    if (tape == NULL) {
        free(code);
        return -1;
    }
    // tape is stolen by eval, since it may need to grow:
    int error = eval(code, tape, head, size);
    free(code);
    return error;
}
