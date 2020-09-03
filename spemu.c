#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <unistd.h>


const char * const RED = "\x1b[38;5;1m";
const char * const GREEN = "\x1b[38;5;1m";
const char * const YELLOW = "\x1b[38;5;3m";
const char * const BLUE = "\x1b[38;5;4m";
const char * const END = "\x1b[0m";

extern int errno;

int fd = -1;
char *buf = NULL;

void sigint_handler(int _)
{
  printf("\rGracefully quitting...\n");

  free(buf);
  if (fd > 0)
  {
    close(fd);
  }
  exit(0);
}

int main(void)
{
  signal(SIGINT, sigint_handler);

  fd = posix_openpt(O_RDWR | O_NOCTTY);
  if (fd < 0)
  {
    fprintf(
      stderr, "%sCouldn't open PTY%s: %s (%d)\n",
      RED, END, strerror(errno), errno
    );
    exit(1);
  }
  printf(
    "Opened PTY with %sfd%s=%s%d%s\n",
    BLUE, END, YELLOW, fd, END
  );

  int err = grantpt(fd);
  if (err)
  {
    fprintf(
      stderr, "%sFailed to grant access to the PTY device%s: %s (%d)\n",
      RED, END, strerror(errno), errno
    );
    exit(1);
  }
  printf("Granted access to the PTY device.\n");

  err = unlockpt(fd);
  if (err)
  {
    fprintf(
      stderr, "%sFailed to unlock the PTY device%s: %s (%d)\n",
      RED, END, strerror(errno), errno
    );
    exit(1);
  }
  printf("Unlocked the PTY device: %s%s%s\n", YELLOW, ptsname(fd), END);

  for (;;)
  {
    char *buf = NULL;
    size_t bufsz = 0;
    size_t charcnt = getline(&buf, &bufsz, stdin);
    write(fd, buf, charcnt);
  }

  return 0;
}
