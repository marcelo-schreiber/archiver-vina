// https://wiki.inf.ufpr.br/maziero/doku.php?id=pua:operacoes_em_diretorios
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

typedef struct file
{
    char *name;
    uid_t uid;
    mode_t permissions;
    off_t size;
    time_t date;
    unsigned int order;
    char *location;

} file_metadata;

#define MAX_BUF_SIZE 1024

file_metadata *initialize_header();
void insert_header(file_metadata *header, char *location, char *filename, unsigned int order);
void print_header(file_metadata *header);
void free_header(file_metadata *header);