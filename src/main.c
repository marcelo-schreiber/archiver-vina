#include <stdio.h>
#include "insert.h"

int main(void)
{
    header_t *header = insert_header("./", "LEIAME.md", 1);
    print_header(header);
    return 0;
}