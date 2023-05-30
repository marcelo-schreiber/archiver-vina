#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "insert.h"

#define MAX_BUF_SIZE 1024

file_metadata **read_headers(FILE *archiver, int num_of_files)
{
    file_metadata **headers = malloc(sizeof(file_metadata *) * num_of_files);

    for (int i = 0; i < num_of_files; i++)
    {
        file_metadata *header_read = initialize_header();
        fread(header_read, sizeof(file_metadata), 1, archiver);
        headers[i] = header_read;
        print_header(headers[i]);
    }

    return headers;
}

void extract_vpp(FILE *archiver)
{
    rewind(archiver);

    // read archiver file
    int num_of_files = 0;
    fread(&num_of_files, sizeof(int), 1, archiver);

    // store into array of files_metada
    file_metadata **headers = read_headers(archiver, num_of_files);

    // extract files
    for (int i = 0; i < num_of_files; i++)
    {
        file_metadata *header = headers[i];
        char *new_file_name = malloc(sizeof(char) * strlen(header->name) + 4);
        strcpy(new_file_name, "new");
        strcat(new_file_name, header->name);

        FILE *file = fopen(new_file_name, "wb+");
        int size = header->size;

        char *buffer = malloc(sizeof(char) * MAX_BUF_SIZE);

        while (size > 0)
        {
            if (size < MAX_BUF_SIZE)
            {
                fread(buffer, sizeof(char), size, archiver);
                fwrite(buffer, sizeof(char), size, file);
                break;
            }
            else
            {
                fread(buffer, sizeof(char), MAX_BUF_SIZE, archiver);
                fwrite(buffer, sizeof(char), MAX_BUF_SIZE, file);
            }
            size -= MAX_BUF_SIZE;
        }
    }
}

int main(void)
{
    // create archiver file
    FILE *archiver = fopen("vina.vpp", "wb+");

    char **file_names = malloc(sizeof(char *) * 2);
    file_names[0] = "LEIAME.md";
    file_names[1] = ".gitignore";
    int i = 2;
    fwrite(&i, sizeof(int), 1, archiver);
    for (i = 0; i < 2; i++)
    {
        file_metadata *header = initialize_header();
        header = insert_header("./", file_names[i], i + 1);
        print_header(header);
        fwrite(header, sizeof(file_metadata), 1, archiver);
    }

    // write the content of each file
    for (i = 0; i < 2; i++)
    {
        FILE *file = fopen(file_names[i], "rb");
        char *buffer = malloc(sizeof(char) * 1024);
        int size = 0;
        while ((size = fread(buffer, sizeof(char), 1024, file)) > 0)
        {
            fwrite(buffer, sizeof(char), size, archiver);
        }
        fclose(file);
    }

    extract_vpp(archiver);
    fclose(archiver);

    return 0;
}
