#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
    if(argc != 3){
        printf(2, "Usage: rename old_name new_name\n");
        exit();
    }

    if(link(argv[1], argv[2]) < 0) {
        printf(2, "rename %s %s: failed\n", argv[1], argv[2]);
        exit();
    }

    if(unlink(argv[1]) < 0){
        printf(2, "rename: %s failed to rename\n", argv[1]);
        exit();
    }

  exit();
}
