// https://wiki.inf.ufpr.br/maziero/doku.php?id=pua:operacoes_em_diretorios
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
// Uma abordagem interessante consiste em colocar a área de diretório no final do archive, mantendo no início do deste apenas um inteiro indicando a posição do archive onde inicia essa área.
// Essa organização torna mais simples as operações de alteração do conteúdo do arquivo

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

file_metadata *initialize_header();
file_metadata *insert_header(char *location, char *filename, unsigned int order);
void print_header(file_metadata *header);
void free_header(file_metadata *header);