#include <stdio.h>
#include <stdlib.h>
#include "file_header.h"

#define MAX_BUF_SIZE 1024

void encode(char *vina_filename)
{
  // create archiver file
  FILE *archiver = fopen(vina_filename, "wb+");

  char *file_names[2] = {"LEIAME.md", ".gitignore"}; // TODO: read from command line

  unsigned int files_count = 2;

  fwrite(&files_count, sizeof(int), 1, archiver);

  for (unsigned int i = 0; i < files_count; i++)
  {
    file_metadata *header = initialize_header();
    header = insert_header("./", file_names[i], i + 1); // TODO: get file path from command line
    print_header(header);
    fwrite(header, sizeof(file_metadata), 1, archiver);
  }

  // write the content of each file
  for (unsigned int i = 0; i < files_count; i++)
  {
    FILE *file = fopen(file_names[i], "rb");
    char *buffer = malloc(sizeof(char) * MAX_BUF_SIZE);
    int size = 0;
    while ((size = fread(buffer, sizeof(char), MAX_BUF_SIZE, file)) > 0)
      fwrite(buffer, sizeof(char), size, archiver);

    fclose(file);
  }

  fclose(archiver);
}