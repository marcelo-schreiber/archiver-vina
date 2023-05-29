#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "insert.h"
// insert into executable archiver

// área de diretório contém informações sobre os membros do archive
// Todas as informações sobre os membros, necessárias para a manipulação do archive, devem estar armazenadas única e exclusivamente na área de diretório. A parte restante deve conter apenas os dados dos membros.
// Para cada membro, devem ser armazenadas as seguintes informações: nome (sem espaços), UID (user ID), permissões, tamanho, data de modificação, ordem no arquivo e localização.
// A ordem de um membro no arquivo é dada pela ordem de inserção, e pode ser alterada pela opção -m.
// O conteúdo da área de diretório pode ser manipulado em memória RAM ou em disco, a critério do aluno.
// O conteúdo dos membros do archive deve ser manipulado diretamente em disco, não sendo permitida a alocação de mais de 1.024 bytes de memória para manipulação dos conteúdos dos membros.

file_metadata *initialize_header()
{
    file_metadata *header = malloc(sizeof(file_metadata));

    header->name = NULL;
    header->uid = 0;
    header->permissions = 0;
    header->size = 0;
    header->date = 0;
    header->order = 0;
    header->location = NULL;

    return header;
}

void print_header(file_metadata *header)
{
    // pretty print like ls -l

    // print header permission like --w--w--w-
    printf("%s", (header->permissions & S_IRUSR) ? "r" : "-");
    printf("%s", (header->permissions & S_IWUSR) ? "w" : "-");
    printf("%s", (header->permissions & S_IXUSR) ? "x" : "-");
    printf("%s", (header->permissions & S_IRGRP) ? "r" : "-");
    printf("%s", (header->permissions & S_IWGRP) ? "w" : "-");
    printf("%s", (header->permissions & S_IXGRP) ? "x" : "-");
    printf("%s", (header->permissions & S_IROTH) ? "r" : "-");
    printf("%s", (header->permissions & S_IWOTH) ? "w" : "-");
    printf("%s", (header->permissions & S_IXOTH) ? "x" : "-");

    // print header uid
    printf(" %d", header->uid);

    // print header size
    printf(" %ld", header->size);

    // UTC time
    time_t time = header->date - 19800; // from local time to UTC time
    struct tm ts = *localtime(&time);
    char buf[80];
    strftime(buf, sizeof(buf), "%m %d %H:%M", &ts);
    printf(" %s", buf);

    // print header name
    printf(" %s", header->name);

    // print header order
    printf(" %d\n", header->order);
}

file_metadata *insert_header(char *location, char *filename, unsigned int order)
{
    file_metadata *header = initialize_header();

    struct stat file_stat;
    stat(filename, &file_stat);

    char *name = malloc(sizeof(char) * strlen(filename));
    strcpy(name, filename);
    header->name = name;
    header->uid = file_stat.st_uid;
    header->permissions = file_stat.st_mode;
    header->size = file_stat.st_size;
    header->date = file_stat.st_mtime;
    header->order = order;
    header->location = location;

    return header;
}
