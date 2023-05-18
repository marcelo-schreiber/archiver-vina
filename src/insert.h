
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

typedef struct header
{
    char *name;
    uid_t uid;
    mode_t permissions;
    off_t size;
    time_t date;
    unsigned int order; // ordem no arquivo
    char *location;
} header_t;

header_t *insert_header(char *location, char *filename, unsigned int order);
void print_header(header_t *header);
