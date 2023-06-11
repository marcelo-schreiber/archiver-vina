#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "pack.h"
#include "unpack.h"

int main(int argc, char *argv[])
{
    int option;
    char *archive = NULL;
    char **members = NULL;
    int members_count = 0;

    while ((option = getopt(argc, argv, "iaxmrch:")) != -1)
    {
        switch (option)
        {
        case 'i':
            // Insert/accretion logic
            if (archive == NULL)
            {
                if (optind < argc)
                {
                    archive = argv[optind++];
                    members = &argv[optind];
                    members_count = argc - optind;
                    printf("Inserting members: ");
                    for (int i = 0; i < members_count; i++)
                    {
                        printf("%s ", members[i]);
                    }
                    printf("into %s\n", archive);
                    new_pack(archive, members, members_count);
                }
                else
                {
                    fprintf(stderr, "Error: Archive not specified\n");
                    return 1;
                }
            }
            break;
        case 'a':
            // Insert/accretion with check logic
            if (archive == NULL)
            {
                if (optind < argc)
                {
                    archive = argv[optind++];
                    members = &argv[optind];
                    members_count = argc - optind;
                    printf("Inserting members (with check): ");
                    for (int i = 0; i < members_count; i++)
                    {
                        printf("%s ", members[i]);
                    }
                    printf("into %s\n", archive);
                }
                else
                {
                    fprintf(stderr, "Error: Archive not specified\n");
                    return 1;
                }
            }
            break;
        case 'm':
            // Move logic
            if (archive == NULL)
            {
                fprintf(stderr, "Error: Archive not specified\n");
                return 1;
            }
            if (optind + 1 < argc)
            {
                const char *member = optarg;
                const char *target = argv[optind++];
                printf("Moving member: %s after %s in %s\n", member, target, archive);
            }
            else
            {
                fprintf(stderr, "Error: Target member not specified\n");
            }
            break;
        case 'x':
            if (optind + 1 < argc)
            {
                archive = argv[optind++];
                members = &argv[optind];
                members_count = argc - optind;
                printf("Extracting members: ");
                for (int i = 0; i < members_count; i++)
                {
                    printf("%s ", members[i]);
                }
                printf("from %s\n", archive);
            }
            else
            {
                members = NULL;
                members_count = 0;
                archive = argv[optind++];
                printf("Extracting all members from %s\n", archive);
                unpack(archive);
            }
            break;
        case 'r':
            // Removal logic
            if (archive == NULL)
            {
                fprintf(stderr, "Error: Archive not specified\n");
                return 1;
            }
            if (optind < argc)
            {
                members = &argv[optind];
                members_count = argc - optind;
                printf("Removing members: ");
                for (int i = 0; i < members_count; i++)
                {
                    printf("%s ", members[i]);
                }
                printf("from %s\n", archive);
            }
            else
            {
                fprintf(stderr, "Error: Members to remove not specified\n");
            }
            break;
        case 'c':
            // Listing content logic
            if (archive == NULL)
            {
                fprintf(stderr, "Error: Archive not specified\n");
                return 1;
            }
            printf("Listing content of %s\n", archive);
            break;
        case 'h':
            // Display help message
            printf("Usage: vina++ <option> <archive> [member1 member2 ...]\n");
            printf("Options:\n");
            printf("-i : insert/accrete one or more members into the archive\n");
            printf("-a : insert/accrete one or more members into the archive, replacing only if newer\n");
            printf("-m target : move the specified member after the target member in the archive\n");
            printf("-x : extract the specified members from the archive\n");
            printf("-r : remove the specified members from the archive\n");
            printf("-c : list the content of the archive\n");
            printf("-h : display this help message\n");
            break;
        default:
            fprintf(stderr, "Error: Unknown option\n");
            return 1;
        }
    }

    return 0;
}
