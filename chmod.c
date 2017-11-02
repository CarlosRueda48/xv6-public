#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  char permissions[4];
  if( argc != 3 )
  {
    printf( 2, "Usage: chmod permissions file\n" );
    exit();
  }

  // Normalize ASCII value to actual number. Cast to integer.
  permissions[0] = argv[1][0] - '0';
  permissions[1] = argv[1][1] - '0';
  permissions[2] = argv[1][2] - '0';
  permissions[3] = 0;

  if( chmod( permissions, argv[2] ) < 0 )
    printf(2, "chmod %s %s: failed\n", argv[1], argv[2]);

  exit();
}
