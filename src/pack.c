#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_metadata.h"

#define MAX_BUF_SIZE 1024

void write_file_metadata_to_archiver(file_metadata *header, FILE *archiver)
{
  // char *name;
  // uid_t uid;
  // mode_t permissions;
  // off_t size;
  // time_t date;
  // unsigned int order;
  // char *location;
  unsigned int name_length = strlen(header->name);
  unsigned int location_length = strlen(header->location);
  fwrite(&name_length, sizeof(unsigned int), 1, archiver);
  fwrite(header->name, sizeof(char), name_length, archiver);
  fwrite(&header->uid, sizeof(uid_t), 1, archiver);
  fwrite(&header->permissions, sizeof(mode_t), 1, archiver);
  fwrite(&header->size, sizeof(off_t), 1, archiver);
  fwrite(&header->date, sizeof(time_t), 1, archiver);
  fwrite(&header->order, sizeof(unsigned int), 1, archiver);
  fwrite(&location_length, sizeof(unsigned int), 1, archiver);
  fwrite(header->location, sizeof(char), strlen(header->location), archiver);
}

void read_file_metadata_from_archiver_test(file_metadata *header, FILE *archiver)
{
  unsigned int name_length = 0;
  unsigned int location_length = 0;

  fread(&name_length, sizeof(unsigned int), 1, archiver);
  header->name = malloc(sizeof(char) * name_length + 1);
  fread(header->name, sizeof(char), name_length, archiver);
  header->name[name_length] = '\0';
  fread(&header->uid, sizeof(uid_t), 1, archiver);
  fread(&header->permissions, sizeof(mode_t), 1, archiver);
  fread(&header->size, sizeof(off_t), 1, archiver);
  fread(&header->date, sizeof(time_t), 1, archiver);

  fread(&header->order, sizeof(unsigned int), 1, archiver);
  fread(&location_length, sizeof(unsigned int), 1, archiver);

  header->location = malloc(sizeof(char) * location_length + 1);
  fread(header->location, sizeof(char), location_length, archiver);
  header->location[location_length] = '\0';
}

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
    write_file_metadata_to_archiver(header, archiver);

    free(path);
    free(header->name);
    free(header->location);
  }

  read_files_contents_and_write_to_archiver(files_count, file_names_with_path, archiver);

  free(header);

  fclose(archiver);
}

void append_to_pack(char *vina_filename, char *file_names_with_path[], unsigned int files_count)
{
  FILE *archiver = fopen(vina_filename, "rb+");

  if (archiver == NULL)
  {
    printf("Error creating file\n");
    exit(1);
  }

  FILE *temp = fopen("temp.vpp", "wb+");

  if (temp == NULL)
  {
    printf("Error creating file\n");
    exit(1);
  }

  unsigned int current_files_count = 0;

  fread(&current_files_count, sizeof(unsigned int), 1, archiver);

  unsigned int new_files_count = current_files_count + files_count;

  fwrite(&new_files_count, sizeof(unsigned int), 1, temp);

  file_metadata *header = initialize_header();

  for (unsigned int i = 0; i < current_files_count; i++)
  {
    read_file_metadata_from_archiver_test(header, archiver);
    write_file_metadata_to_archiver(header, temp);
    free(header->name);
    free(header->location);
  }

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

    insert_header(header, path, current_file_name, i + 1 + current_files_count);
    write_file_metadata_to_archiver(header, temp);

    free(path);
    free(header->name);
    free(header->location);
  }

  // write files contents to temp.vpp
  char *buffer = malloc(sizeof(char) * MAX_BUF_SIZE);

  if (buffer == NULL)
  {
    printf("Error allocating memory for buffer\n");
    exit(1);
  }

  memset(buffer, 0, sizeof(char) * MAX_BUF_SIZE);

  size_t size = 0;

  while ((size = fread(buffer, sizeof(char), MAX_BUF_SIZE, archiver)) > 0)
    fwrite(buffer, sizeof(char), size, temp);

  read_files_contents_and_write_to_archiver(files_count, file_names_with_path, temp);

  free(header);

  fclose(archiver);
}