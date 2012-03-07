// vim: foldmethod=syntax
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "todo.h"

static char *path;

char *get_path() {
	if (path == NULL) {
		path = (char *)malloc(sizeof(char) * (strlen(getenv("HOME")) + strlen("/todo.txt")));
		path = strcpy(path, getenv("HOME"));
		path = strcat(path, "/todo.txt");	
	}
	return path;
}

int add(char const *value) {
	FILE *fp;
	fp = fopen(get_path(), "a+");
	if (fp == NULL) {
		printf("%s can not open.", get_path());
		return -1;
	}
	fprintf(fp, "%s\n", value);
	fclose(fp);
	free(path);
	return 0;
}

int list(char *value) {
	FILE *fp;
	fp = fopen(get_path(), "r");
	if (fp == NULL) {
		printf("%s can not open.", get_path());
		return -1;
	}
	int c = EOF;
	int line = 1;
	printf("%d\t", line++);
	int putline = 0;
	while ((c = fgetc(fp)) != EOF) {
		if (putline != 0) {
			printf("%d\t", line++);
			putline = 0;
		}
		putchar(c);
		putline = (c == '\n' || c == '\n');
	}
	fclose(fp);
	free(path);
	return 0;
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
	params->value = (char *)malloc(sizeof(char));
	int i = 0;
	// command not specified.
	if (argc <= 1) {
		params->command = LIST;
		return params;
	}
	if (strcmp(*(argv+1), "add") == 0) {
		params->command = ADD;
		char *tmp;
		for (i = 2; i < argc; i++) {
			tmp = (char *)calloc(strlen(params->value) + 1, sizeof(char));
			tmp = strcpy(tmp, params->value);
			free(params->value);
			if (strlen(tmp) > 0) {
				params->value = (char *)calloc((strlen(tmp) + 1 + strlen(*(argv+i))), sizeof(char));
				sprintf(params->value, "%s %s", tmp, *(argv+i));
			} else {
				params->value = (char *)calloc(strlen(*(argv+i)) + 1, sizeof(char));
				sprintf(params->value, "%s", *(argv+i));
			}
			free(tmp);
		}
	} else if (strcmp(*(argv+1), "delete") == 0) {
		params->command = DELETE;
		params->line = atoi(*(argv+2));
	} else if (strcmp(*(argv+1), "list") == 0) {
		params->command = LIST;
		params->value = *(argv+2);
	} else if (strcmp(*(argv+1), "nice") == 0) {
		params->command = NICE;
		params->line = atoi(*(argv+2));
		params->nice = atoi(*(argv+3));
	} else {
		params->command = USAGE;
	}
	return params;
}

int delete(int line) {
	FILE *fp;
	FILE *tmp;
	fp = fopen(get_path(), "r");
	if (fp == NULL) {
		printf("%s can not open.", get_path());
		return -1;
	}
	tmp = tmpfile();
	if (tmp == NULL) {
		printf("tmp file can not open.");
		if (fp != NULL) {
			fclose(fp);
		}
		return -1;
	}
	int c = EOF;
	int i = 1;
	while ((c = fgetc(fp)) != EOF) {
		if (c == '\n' || c == '\r') {
			i++;
		}
		if (i != line) {
			fputc(c, tmp);
		}
	}
	fclose(fp);
	fseek(tmp, 0, SEEK_SET);
	fp = fopen(get_path(), "w");
	if (fp == NULL) {
		printf("%s can not open.", get_path());
		return -1;
	}
	while ((c = fgetc(tmp)) != EOF) {
		fputc(c, fp);
	}
	fclose(tmp);
	free(path);
	return 0;
}

int nice(int line, int nice) {
	FILE *fp;
	FILE *tmp;
	fp = fopen(get_path(), "r");
	if (fp == NULL) {
		printf("%s can not open.", get_path());
		return -1;
	}
	tmp = tmpfile();
	if (tmp == NULL) {
		printf("tmp file can not open.");
		if (fp != NULL) {
			fclose(fp);
		}
		return -1;
	}
	int c = EOF;
	int i = 1;
	char *value = (char *)calloc(2, sizeof(char));
	char *vtmp;
	while ((c = fgetc(fp)) != EOF) {
		if (c == '\n' || c == '\r') {
			i++;
		}
		if (i != line) {
			fputc(c, tmp);
		} else {
			if (c != '\n' && c != '\r') {
				vtmp = (char *)calloc(strlen(value) + 1, sizeof(char));
				vtmp = strcpy(vtmp, value);
				free(value);
				value = (char *)calloc(strlen(vtmp) + 2, sizeof(char));
				sprintf(value, "%s%c", vtmp, c);
				free(vtmp);
			}
		}
	}
	fclose(fp);
	fseek(tmp, 0, SEEK_SET);
	fp = fopen(get_path(), "w");
	if (fp == NULL) {
		printf("%s can not open.", get_path());
		return -1;
	}
	i = 1;
	printf("%s\n", value);
	if (i == nice) {
		fprintf(fp, "%s\n", value);
		i++;
	}
	while ((c = fgetc(tmp)) != EOF) {
		if (c == '\n' || c == '\r') {
			i++;
			if (i == nice) {
				fputc(c, fp);
				fprintf(fp, "%s\n", value);
				i++;
				continue;
			}
		}
		fputc(c, fp);
	}
	fclose(tmp);
	free(path);
	free(value);
	return 0;
}

