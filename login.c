#include "types.h"
#include "stat.h"
#include "user.h"

#define PASSWD "/passwd"
#define BUF_SIZE 512

void rlinebreak(char *l)
{
    while (*(l++) != '\n')
        ;
    *(--l) = '\0';
}

int verify(char *line, char *user, char *passwd)
{
    while (*line != ':' && *(line++) == *(user++))
        ;
    if (*(line) != ':' || *(line - 1) != *(user - 1))
        return 0;
    ++line;
    while (*(line++) != ':')
        ;
    while (*line != '\n' && *(line++) == *(passwd++))
        ;
    if (*(line) != '\n' || *(line - 1) != *(passwd - 1))
        return 0;
    return 1;
}

int readline(int fd, char *buf, int size)
{
    int n = 0;
    while (read(fd, buf, 1) > 0)
    {
        n++;
        if (n == size - 1)
        {
            *(++buf) = '\0';
            return -1;
        }
        if (*buf == '\n')
        {
            *(++buf) = '\0';
            return 1;
        }
        ++buf;
    }
    *buf = '\0';
    return -1;
}

int main()
{
    int fd, pid, wpid;
    char buf[BUF_SIZE];
    char user[BUF_SIZE], passwd[BUF_SIZE];
    char *args[2] = {"sh", 0};

    while (1)
    {
        fd = open(PASSWD, 0);

        printf(1, "login: ");
        read(0, user, sizeof(user));
        printf(1, "password: ");
        read(0, passwd, sizeof(passwd));
        rlinebreak(user);
        rlinebreak(passwd);
        printf(1, "%s:%s\n", user, passwd);
        while (readline(fd, buf, sizeof(buf)) > 0)
        {
            if (verify(buf, user, passwd))
            {
                printf(1, "Success.\n");
                close(fd);
                pid = fork();
                if (pid < 0)
                {
                    printf(1, "login: fork failed\n");
                    exit();
                }
                if (pid == 0)
                {
                    exec("sh", args);
                    printf(1, "login: exec sh failed\n");
                    exit();
                }
                while ((wpid = wait()) >= 0 && wpid != pid)
                    printf(1, "zombie!\n");
                exit();
            }
        }
        printf(1, "Failure.\n");
        close(fd);
    }
    exit();
}
