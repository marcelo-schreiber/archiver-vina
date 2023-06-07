#include "unpack.h"
#include "pack.h"

int main(void)
{
    new_pack("vina.vpp", (char *[]){"./LEIAME.md", "./.gitignore"}, 2);
    append_to_pack("vina.vpp", (char *[]){"./Makefile"}, 1);
    // unpack("vina.vpp");

    return 0;
}