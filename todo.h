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
	int line;
	int nice;
} Params;

char *get_path(void);

int add(const char *);

void delete(int);

void nice(int, int);

void usage(char **);

Params *parse(int, char **);
