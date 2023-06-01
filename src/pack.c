#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_metadata.h"

void create_files(unsigned int files_count, char **file_names, FILE *archiver)
{
  for (unsigned int i = 0; i < files_count; i++)
  {
    FILE *file = fopen(file_names[i], "rb");
    char *buffer = malloc(sizeof(char) * MAX_BUF_SIZE);

    if (buffer == NULL)
    {
      printf("Error allocating memory for buffer\n");
      exit(1);
    }

    memset(buffer, 0, sizeof(char) * MAX_BUF_SIZE);

    int size = 0;

    while ((size = fread(buffer, sizeof(char), MAX_BUF_SIZE, file)) > 0)
      fwrite(buffer, sizeof(char), size, archiver);

    fclose(file);
    free(buffer);
  }
}

void pack(char *vina_filename)
{
  FILE *archiver = fopen(vina_filename, "wb+");

  if (archiver == NULL)
  {
    printf("Error creating file\n");
    exit(1);
  }

  char *file_names[2] = {"LEIAME.md", ".gitignore"}; // TODO: read from command line

  unsigned int files_count = 2; // TODO: read from command line

  fwrite(&files_count, sizeof(int), 1, archiver);

  file_metadata *header = initialize_header();

  for (unsigned int i = 0; i < files_count; i++)
  {
    insert_header(header, "./", file_names[i], i + 1); // TODO: get file path from command line
    fwrite(header, sizeof(file_metadata), 1, archiver);
  }

  create_files(files_count, file_names, archiver);

  free(header);

  fclose(archiver);
}
