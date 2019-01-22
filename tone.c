#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/io.h>
#include <time.h>
#include <pthread.h>

/* Base parallel port address. May be different on different systems*/
#define BASE 0x378

#define set_lpt_val(val) (outb (val, BASE))

static void *tone_thread_f (void *arg) {
    unsigned char state = 0;
    struct timespec *delay = arg;
    for (;;) {
        set_lpt_val (state ^= 1); nanosleep (delay, NULL);
        set_lpt_val (state ^= 1); nanosleep (delay, NULL);
    }
    return NULL;
}

void tone (int freq, double duration) {
    long half_period = 500000000 / freq;
    pthread_t tone_thread;

    struct timespec *freq_delay = malloc (sizeof *freq_delay);
    freq_delay->tv_sec = 0;
    freq_delay->tv_nsec = half_period;

    struct timespec  tempo_delay;
    tempo_delay.tv_sec = (int) duration;
    tempo_delay.tv_nsec = (duration - tempo_delay.tv_sec) * 1e9;

    pthread_create (&tone_thread, NULL, tone_thread_f, freq_delay);

    nanosleep (&tempo_delay, NULL);

    pthread_cancel (tone_thread);

    free (freq_delay);
}

int main (int ac, char *av[]) {
    int freq = 20000;
    double duration = 1;

    switch (ac) {
    case 1:
        break;
    default:
    case 3:
        sscanf (av[2], "%lf", &duration);
    case 2:
        sscanf (av[1], "%d", &freq);
        break;
    }

    if (ioperm (BASE, 1, 1)) fprintf (stderr, "Access denied\n"), exit (1);

    tone (freq, duration);

    return 0;
}
