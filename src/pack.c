#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_metadata.h"

#define MAX_BUF_SIZE 1024

void read_files_contents_and_write_to_archiver(unsigned int files_count, char **file_names_with_path, FILE *archiver)
{
  for (unsigned int i = 0; i < files_count; i++)
  {
    FILE *file = fopen(file_names_with_path[i], "rb");

    if (file == NULL)
    {
      printf("Error opening file\n");
      exit(1);
    }

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

void new_pack(char *vina_filename, char *file_names_with_path[], unsigned int files_count)
{
  FILE *archiver = fopen(vina_filename, "wb+");

  if (archiver == NULL)
  {
    printf("Error creating file\n");
    exit(1);
  }

  fwrite(&files_count, sizeof(unsigned int), 1, archiver);

  file_metadata *header = initialize_header();

  for (unsigned int i = 0; i < files_count; i++)
  {
    char *curr_file_with_path = file_names_with_path[i];

    // ./pasta/new_LEIAME.md -> new_LEIAME.md
    char *current_file_name = strrchr(curr_file_with_path, '/') + 1;

    // ./pasta/new_LEIAME.md -> ./pasta/
    unsigned int only_path_size = strlen(curr_file_with_path) - strlen(current_file_name);

    char *path = malloc(sizeof(char) * (only_path_size + 1)); // +1 for '\0'
    memset(path, 0, sizeof(char) * (only_path_size + 1));

    strncpy(path, curr_file_with_path, only_path_size); // copy only path to path variable
    path[only_path_size] = '\0';                        // add '\0' to end of path variable

    insert_header(header, path, current_file_name, i + 1);
    fwrite(header, sizeof(file_metadata), 1, archiver);

    free(path);
  }

  read_files_contents_and_write_to_archiver(files_count, file_names_with_path, archiver);

  free(header);

  fclose(archiver);
}
