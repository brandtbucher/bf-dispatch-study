// Reusable bf instruction definitions:

TARGET(INCREMENT)
{
    tape[head] += ip->oparg;
    DISPATCH;
}

TARGET(DECREMENT)
{
    tape[head] -= ip->oparg;
    DISPATCH;
}

TARGET(MOVE_LEFT)
{
    while (head < ip->oparg) {
        // Slow path: tape needs to grow.
        if (grow_left(&tape, &head, &size)) {
            free(tape);
            return -1;
        }
    }
    head -= ip->oparg;
    DISPATCH;
}

TARGET(MOVE_RIGHT)
{
    while (size <= head + ip->oparg) {
        // Slow path: tape needs to be grow.
        if (grow_right(&tape, &head, &size)) {
            free(tape);
            return -1;
        }
    }
    head += ip->oparg;
    DISPATCH;
}

TARGET(LOOP_BEGIN)
{
    if (tape[head] == 0) {
        ip += ip->oparg;
    }
    DISPATCH;
}

TARGET(LOOP_END)
{
    if (tape[head] != 0) {
        ip -= ip->oparg;
    }
    DISPATCH;
}

TARGET(READ)
{
    int c = getchar();
    if (c != EOF) {
        tape[head] = c;
    }
    else if (ferror(stdin)) {
        free(tape);
        return -1;
    }
    DISPATCH;
}

TARGET(WRITE)
{
    if (putchar(tape[head]) == EOF) {
        free(tape);
        return -1;
    }
    DISPATCH;
}

TARGET(RETURN)
{
    free(tape);
    return 0;
}
