#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_metadata.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX_BUF_SIZE 1024

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
    fread(header_read, sizeof(file_metadata), 1, archiver);
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

    free(buffer);
    free(new_file_name);
    fclose(output_file);

    free(curr_header->name);
    free(curr_header);
  }

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