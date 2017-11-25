#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define PASSWD "/passwd"
#define LENGTH 100
#define NUMSIZE 5

char *
itoa(int value, char *result, int base)
{
  // check that the base if valid
  if (base < 2 || base > 36)
  {
    *result = '\0';
    return result;
  }

  char *ptr = result, *ptr1 = result, tmp_char;
  int tmp_value;

  do
  {
    tmp_value = value;
    value /= base;
    *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp_value - value * base)];
  } while (value);

  // Apply negative sign
  if (tmp_value < 0)
    *ptr++ = '-';
  *ptr-- = '\0';
  while (ptr1 < ptr)
  {
    tmp_char = *ptr;
    *ptr-- = *ptr1;
    *ptr1++ = tmp_char;
  }
  return result;
}

int
contains(char *str, char *pat)
{
  int i = 0, j = 0;
  int sStr = strlen(str);
  int sPat = strlen(pat);

  while(i < sPat && j < sStr && pat[i] != str[j])
    j++;

  if(j == sStr)
    return 0;

  while(i < sPat && j < sStr && pat[i] == str[j]) {
    i++;
    j++;
  }

  if(i == sPat)
    return 1;
  else
    return 0;
}

int
readline(int fd, char *buf, int size)
{
  int n = 0, i = 0;
  char c;
  while(read(fd, &c, 1) > 0) {
    n++;
    if(c != '\n') {
      buf[i] = c;
      i++;
    }
    else
      break;
  }

  buf[i] = '\0';
  return n;
}

int
getid(char *record)
{
  int i = 0;
  char holder[NUMSIZE];
  while (i < strlen(record) && record[i] != ':')
    i++;

  i++;
  int j = 0;
  while (j < NUMSIZE && record[i] != ':')
  {
    holder[j] = record[i];
    i++;
    j++;
  }
  holder[i] = '\0';
  return atoi(holder) + 1;
}

int
main(int argc, char **argv)
{
  if (argc != 3)
  {
    printf(2, "Usage: useradd username password\n");
    exit();
  }

  int fd, n, id = 0;
  char record[LENGTH];

  fd = open(PASSWD, O_RDWR);
  while ((n = readline(fd, record, sizeof(record))) > 0)
  {
    id = getid(record);
    if (contains(record, argv[1]))
    {
      printf(2, "User already exists.\n");
      close(fd);
      exit();
    }
  }

  int len_uname;
  int len_paswd;
  len_uname = strlen(argv[1]);
  len_paswd = strlen(argv[2]);
  char colon = ':';
  char idbuf[NUMSIZE];
  char newline = '\n';
  itoa(id, idbuf, 10);
  write(fd, argv[1], len_uname);
  write(fd, &colon, 1);
  write(fd, idbuf, strlen(idbuf));
  write(fd, &colon, 1);
  write(fd, argv[2], len_paswd);
  write(fd, &newline, 1);
  close(fd);
  exit();
}
