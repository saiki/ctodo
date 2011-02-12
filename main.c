// vim: foldmethod=syntax
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include "todo.h"

int main(int argc, char *argv[]) {

	Params *params = parse(argc, argv);
	switch (params->command) {
		case ADD:
			if ((params->priority = add(params->value)) != -1) {
				printf("add priority %d\n", params->priority);
			} else {
				printf("error \"%s\" can not add.", params->value);
			}
			break;
		case DELETE:
			delete(params->priority);
			break;
		case LIST:
			list(params->value);
			break;
		case NICE:
			nice(params->priority, params->nice);
			break;
		case USAGE:
			usage(argv);
			exit(1);
			break;
		default:
			usage(argv);
			exit(1);
			break;
	}
	free(params->value);
	free(params);
	return 0;
}

