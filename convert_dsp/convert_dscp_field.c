#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>


#define INIT_DSCP_FIELD_WITH_0(target) (target = 0)
#define INIT_DSCP_FIELD_WITH_1(target) (target = 0xFFFFFFFFFFFFFFFFLLU)
#define ENABLE_DSCP_FIELD_VALUE(target, val)   (target |=(( 1LLU << val)))
#define ENABLE_DSCP_FIELD_RANGE(target, val1, val2)   (target |= (((1LLU << (val2-val1+1))-1) << val1))
#define DISABLE_DSCP_FIELD_VALUE(target, val)   (target &=(~( 1LLU << val)))
#define DISABLE_DSCP_FIELD_RANGE(target, val1, val2)   (target &= (~(((1LLU << (val2-val1+1))-1)) << val1))
#define GET_DSCP_FIELD_VALUE(target, val)   ((target >> val) & 0x00000001)


int main(int argc, char *argv[]) {


	if (argc == 1) {
		fprintf(stderr, "Missing DSCP field\n");
		return EXIT_FAILURE;
	}

	if (strlen(argv[1]) != 16) {
		fprintf(stderr, "DSCP field bad size\n");
		return EXIT_FAILURE;
	}


	uint64_t dscp_field;
        INIT_DSCP_FIELD_WITH_0(dscp_field);

	dscp_field = strtoll(argv[1], NULL, 16);
	if (errno != 0) {
		fprintf(stderr, "%s", strerror(errno));
		return EXIT_FAILURE;
	}

	int i = 0;
	for (i = 0; i < 64; i++) {
		if (GET_DSCP_FIELD_VALUE(dscp_field, i) == 1) {
			printf("%d ", i);
		}
	}
	printf("\n");


	return EXIT_SUCCESS;


}
