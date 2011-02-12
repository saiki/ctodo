// vim: foldmethod=syntax
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "todo.h"

char *get_path() {
	char *path;
	path = (char *)malloc(sizeof(char) * (strlen(getenv("HOME")) + strlen("/todo.txt")));
	path = strcat(getenv("HOME"), "/todo.txt");	
	return path;
}

int add(char const *value) {
	FILE *fp;
	fp = fopen(get_path(), "a+");
	if (fp == NULL) {
		printf("%s can not open.", get_path());
		return -1;
	}
	int priority = 0;
	int current = 0;
	int c;
	char *line = (char *)malloc(sizeof(char) * 2);
	char *tmp;
	int n = 0;
	printf("%d\t%d", '\r', '\n');
	fseek(fp, 0, SEEK_SET);
	while ((c = fgetc(fp)) != EOF) {
		if (c != '\n' && c != '\r') {
			tmp = (char *)calloc((strlen(line) + 1), sizeof(char));
			tmp = strcpy(tmp, line);
			free(line);
			line = (char *)calloc((strlen(tmp) + 2), sizeof(char));
			sprintf(line, "%s%c", tmp, c);
			free(tmp);
			continue;
		} else {
			printf("%d\t%c", c, c);
		}
		// if crlf skip
		if (strlen(line) > 0) {
			if ((n = sscanf(line, "%d\t%*s", &current)) == 1) {
				priority = (priority > current) ? priority : current;
			}
			printf("line(n=%d, current=%d, priority=%d)=%s\n", n, current, priority, line);
			free(line);
			line = (char *)malloc(sizeof(char) * 2);
		}
	}
	printf("priority = %d\n", priority);
	printf("%s\n", value);
	fprintf(fp, "%d\t%s\n", (priority + 1), value);
	free(line);
	fclose(fp);
	return priority + 1;
}

void delete(int priority) {
	
}

int list(char *value) {
	FILE *fp;
	fp = fopen(get_path(), "r");
	if (fp == NULL) {
		printf("%s can not open.", get_path());
		return -1;
	}
	int c;
	while ((c = fgetc(fp)) != EOF) {
		putchar(c);
	}
	fclose(fp);
	return 0;
}

void nice(int priority, int nice) {

}

void sort() {
	FILE *tmp = tmpfile();
	FILE *fp = fopen(get_path(), "r");
	fclose(tmp);
	fclose(fp);
}

void usage(char **argv) {
	int i = 0;
	printf("unknown command:%s and values...\n", *(argv+1));
	for (i = 1; *(argv+i) != '\0'; i++) {
		printf("%s\n", *(argv+i));
	}
}

Params *parse(int argc, char **argv) {
	Params *params = (Params *)malloc(sizeof(Params));
	int i = 0;
	if (argc <= 1) {
		printf("list");
		params->command = LIST;
		return params;
	}
	if (strcmp(*(argv+1), "add") == 0) {
		params->command = ADD;
		params->value = (char *)malloc(sizeof(char));
		char *tmp;
		for (i = 2; i < argc; i++) {
			tmp = (char *)malloc(strlen(params->value));
			tmp = strcpy(tmp, params->value);
			free(params->value);
			if (strlen(tmp) > 0) {
				params->value = (char *)calloc((strlen(tmp) + sizeof(char) + strlen(*(argv+i))), sizeof(char));
				sprintf(params->value, "%s %s", tmp, *(argv+i));
			} else {
				params->value = (char *)calloc(strlen(*(argv+i)), sizeof(char));
				sprintf(params->value, "%s", *(argv+i));
			}
			free(tmp);
		}
	} else if (strcmp(*(argv+1), "update") == 0) {
		params->command = UPDATE;
		params->value = *(argv+2);
	} else if (strcmp(*(argv+1), "delete") == 0) {
		params->command = DELETE;
		params->priority = atoi(*(argv+2));
		params->nice = atoi(*(argv+3));
	} else if (strcmp(*(argv+1), "list") == 0) {
		params->command = LIST;
		params->value = *(argv+2);
	} else if (strcmp(*(argv+1), "nice") == 0) {
		params->command = NICE;
		params->priority = atoi(*(argv+2));
		params->nice = atoi(*(argv+3));
	} else {
		params->command = USAGE;
	}
	return params;
}
