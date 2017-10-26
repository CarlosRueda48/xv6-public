#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main( int argc, char *argv[] )
{
    int fd_original;                        // File descriptor: apuntador del archivo
    int fd_new;
    char buf[512];
    int n;

    // Check arguments
    if ( argc != 3 ) {
        printf( 2, "Usage: cp original new");
        exit();
    }

    // Open files
    fd_original = open( argv[1], 0 );         // '0' son los permisos
    fd_new = open( argv[2], O_CREATE|O_WRONLY );

    // Iterate over original file and write it to new file
    while ( (n = read( fd_original, buf, sizeof(buf) )) > 0 )
    {
        write( fd_new, buf, n );
    }

    // Close files
    close( fd_original );
    close( fd_new );

    exit();
};
