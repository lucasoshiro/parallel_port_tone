#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/io.h>
#include <time.h>

/* Base parallel port address. May be different on different systems*/
#define BASE 0x378

#define set_lpt_val(val) (outb (val, BASE))

void tone (int freq, double duration) {
    unsigned char state = 0;
    double start;
    long period = 1000000000 / freq;

    struct timespec delay = {0, period / 2};

    start = (double) clock () / CLOCKS_PER_SEC;

    while (((double) clock () / CLOCKS_PER_SEC - start) < duration) {
        set_lpt_val (state ^= 1);
        nanosleep (&delay, NULL);
        set_lpt_val (state ^= 1);
        nanosleep (&delay, NULL);
    }
}

int main (int ac, char *av[]) {
    int freq = 440, duration = 1;

    switch (ac) {
    case 1:
        break;
    case 2:
        sscanf (av[1], "%d", &freq);
    case 3:
    default:
        sscanf (av[2], "%d", &duration);
        break;
    }

    if (ioperm (BASE, 1, 1)) fprintf (stderr, "Access denied\n"), exit (1);

    tone (freq, duration);

    return 0;
}
