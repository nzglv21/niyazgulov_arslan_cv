struct cmd_inf {
	char ** argv; // список из имени команды и аргументов
	char *infile; // переназначенный файл стандартного ввода
	char *outfile; // переназначенный файл стандартного вывода
	int doublegr; //>>
	int backgrnd; // =1, если команда подлежит выполнению в фоновом режиме cmd_inf* psubcmd; // команды для запуска в дочернем shell
	struct cmd_inf* pipe; // следующая команда после “|”
	struct cmd_inf* next; // следующая после “;” (или после “&&”)
};

typedef struct cmd_inf node;
typedef struct cmd_inf* tree;
extern char *plex;

void print_tree(tree, int);

tree make_cmd();

tree build_tree();

void clear_tree(tree t);