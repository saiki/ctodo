// vim: foldmethod=syntax
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>

// 実行コマンド用
enum COMMAND {
	ADD, 
	UPDATE, 
	DELETE,
	LIST,
	NICE,
	USAGE
};

// パラメータ解析
typedef struct {
	enum COMMAND command;
	char *value;
	int priority;
	int nice;
} Params;

char *path = "";

char *get_path() {
	if (strlen(path) <= 0) {
		path = strcat(getenv("HOME"), "/todo.txt");	
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
	int priority = 0;
	int current = 0;
	char *c = (char *)malloc(sizeof(char));
	char *line = (char *)malloc(sizeof(char) * 10);
	char *tmp_line = (char *)malloc(sizeof(char) * 10);
	fseek(fp, 0, SEEK_SET);
	while ((*c = fgetc(fp)) != EOF) {
		if (*c != '\n' && *c != '\r') {
			free(tmp_line);
			tmp_line = (char *)malloc(sizeof(char) * (strlen(line) + 1));
			if (strlen(line) > 0) {
				tmp_line = strcpy(tmp_line, line);
			}
			tmp_line = strcat(tmp_line, c);
			free(line);
			line = (char *)malloc(sizeof(char) * (strlen(line) + 1));
			line = strcpy(line, tmp_line);
			printf("%s\n", line);
			continue;
		}
		// if crlf skip
		if (strlen(line) > 0) {
			printf("%s\n", line);
			if (sscanf(line, "%d\t%*s", &current) == 1) {
				priority = (priority > current) ? priority : current;
			}
			line = strcpy(line, "");
		}
	}
	fprintf(fp, "%d\t%s\n", (priority + 1), value);
	free(line);
	free(tmp_line);
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
	int n = 0;
	int priority;
	char *task;
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
		for (i = 2; i < argc; i++) {
			if (strlen(params->value) > 0) {
				strcat(params->value, " ");
			}
			strcat(params->value, *(argv+i));
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

