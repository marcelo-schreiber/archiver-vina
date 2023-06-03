#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "file_metadata.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX_BUF_SIZE 1024
#define CURRENT_DIRECTORY "./"

void read_file_metadata_from_archiver(file_metadata *header, FILE *archiver)
{
  // char *name;
  // uid_t uid;
  // mode_t permissions;
  // off_t size;
  // time_t date;
  // unsigned int order;
  // char *location;
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

  print_header(header);
}

file_metadata **create_header_array(FILE *archiver, int num_of_files)
{
  file_metadata **headers = malloc(sizeof(file_metadata *) * num_of_files);

  if (headers == NULL)
  {
    printf("Error allocating memory for headers\n");
    exit(1);
  }

  for (int i = 0; i < num_of_files; i++)
  {
    file_metadata *header_read = initialize_header();
    read_file_metadata_from_archiver(header_read, archiver);
    headers[i] = header_read;

    print_header(headers[i]);
  }

  return headers;
}

void extract_vpp(FILE *archiver)
{
  rewind(archiver); // set file pointer to the beginning of the file (just in case)

  unsigned int num_of_files = 0;
  fread(&num_of_files, sizeof(unsigned int), 1, archiver);

  file_metadata **headers = create_header_array(archiver, num_of_files);

  char *root_of_vpp_directory = getcwd(NULL, 0);
  // extract files
  for (unsigned int i = 0; i < num_of_files; i++)
  {
    file_metadata *curr_header = headers[i];

    char *new_file_name = malloc(sizeof(char) * (strlen(curr_header->name) + 5 + 1));

    if (new_file_name == NULL)
    {
      printf("Error allocating memory for new file name\n");
      exit(1);
    }

    // TODO: remove this
    strncpy(new_file_name, "new_", 5);        // add "new_" to the beginning of the file name
    strcat(new_file_name, curr_header->name); // concatenate the original file name to the new file name

    char *file_path = curr_header->location;

    if (file_path == NULL)
      file_path = CURRENT_DIRECTORY;

    mkdir(file_path, curr_header->permissions);

    if (chdir(file_path) != 0)
    {
      printf("Error changing directory\n");
      exit(1);
    }

    FILE *output_file = fopen(new_file_name, "wb+");

    if (output_file == NULL)
    {
      printf("Error creating file\n");
      exit(1);
    }

    // set the permission of file
    if (chmod(new_file_name, curr_header->permissions) != 0)
    {
      printf("Error setting file permissions\n");
      exit(1);
    }

    int header_size = curr_header->size;

    char *buffer = malloc(sizeof(char) * MAX_BUF_SIZE);

    if (buffer == NULL)
    {
      printf("Error allocating memory for buffer\n");
      exit(1);
    }

    while (header_size > 0) // read the file in chunks of MAX_BUF_SIZE
    {
      int curr_buffer_size = MIN(header_size, MAX_BUF_SIZE);

      fread(buffer, sizeof(char), curr_buffer_size, archiver);

      fwrite(buffer, sizeof(char), curr_buffer_size, output_file);

      header_size -= MAX_BUF_SIZE;
    }

    // change directory back to the original directory
    if (chdir(root_of_vpp_directory) != 0)
    {
      printf("Error changing directory back to original\n");
      exit(1);
    }

    free(buffer);
    free(new_file_name);
    fclose(output_file);

    free(curr_header->name);
    free(curr_header);
  }

  free(root_of_vpp_directory);
  free(headers);
}

void unpack(char *vina_filename)
{
  FILE *archiver = fopen(vina_filename, "rb");

  if (archiver == NULL)
  {
    printf("Error opening file\n");
    exit(1);
  }

  extract_vpp(archiver);
  fclose(archiver);
}