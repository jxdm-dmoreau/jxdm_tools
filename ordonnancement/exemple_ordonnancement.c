
#include <errno.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/time.h>

int main(int argc, char* argv[])
{
	int scheduler;
	int pid;
	int prio;
        struct sched_param param;
        struct timespec interval;

        void syntax(char *name)
        {
            fprintf(stderr, "Syntax %s <pid>\n", name);
            exit(EXIT_FAILURE);
        }


	if (argc != 2 || sscanf(argv[1], "%d", &pid) != 1) {
                syntax(argv[0]);
	}

        if (scheduler = sched_getscheduler(pid) < 0) {
		perror("sched_getscheduler");
		return EXIT_FAILURE;
	}

        if (sched_getparam(pid, &param) < 0) {
		perror("sched_getparam");
		return EXIT_FAILURE;
        }

        if (scheduler == SCHED_RR) {
            if (sched_rr_get_interval(pid, &interval) < 0) {
		perror("sched_rr_get_interval");
		return EXIT_FAILURE;
            }
        } else if (scheduler == SCHED_OTHER) {
            errno = 0;
            if ((prio = getpriority(PRIO_PROCESS, pid) == -1) && errno != 0) {
                perror("getpriority");
                return EXIT_FAILURE;
            }
        }

        switch(scheduler) {
            case SCHED_RR:
                printf("SCHED_RR: prio=%d, interval=%ld %ld s\n", param.sched_priority, interval.tv_sec, interval.tv_nsec);
                break;
            case SCHED_FIFO:
                printf("SCHED_FIFO: prio=%d\n", param.sched_priority);
                break;
            case SCHED_OTHER:
                printf("SCHED_OTHER: sta=%d dyn=%d\n", param.sched_priority, prio);
                break;
            default:
                fprintf(stderr, "???\n");
        }
        return EXIT_SUCCESS;

}
