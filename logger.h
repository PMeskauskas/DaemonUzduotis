int open_log();
int close_log();
void logger(int level, char *msg);

#define INFO 1
#define WARNING 2
#define ERROR 3