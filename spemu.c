#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <unistd.h>


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
      stderr,
      "\x1b[38;5;1m"
      "Couldn't open PTY"
      "\x1b[0m"
      ": %s (%d)\n",
      strerror(errno),
      errno
    );
    exit(1);
  }
  printf(
    "Opened PTY with "
    "\x1b[38;5;3m"
    "fd="
    "\x1b[38;5;4m"
    "%d"
    "\x1b[0m\n",
    fd
  );

  int err = grantpt(fd);
  if (err)
  {
    fprintf(
      stderr,
      "\x1b[38;5;1m"
      "Failed to grant access to the PTY device"
      "\x1b[0m"
      ": %s (%d)\n",
      strerror(errno),
      errno
    );
    exit(1);
  }
  printf("Granted access to the PTY device.\n");

  err = unlockpt(fd);
  if (err)
  {
    fprintf(
      stderr,
      "Failed to unlock the PTY device: %s (%d)\n",
      strerror(errno),
      errno
    );
    exit(1);
  }
  printf(
    "Unlocked the PTY device: "
    "\x1b[38;5;4m"
    "%s"
    "\x1b[0m\n",
    ptsname(fd)
  );

  for (;;)
  {
    char *buf = NULL;
    size_t bufsz = 0;
    size_t charcnt = getline(&buf, &bufsz, stdin);
    write(fd, buf, charcnt);
  }

  return 0;
}

