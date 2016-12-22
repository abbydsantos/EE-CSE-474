#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char** argv) {
  int fd;
  char *myfifo = "mypipe";
  mkfifo(myfifo, 0666);

  fd = open(myfifo, O_WRONLY);
  if (fd == -1) {
    return 1;
  }
  
  char buf[1000];
  while (1) {
    scanf("%s", buf);
    write(fd, buf, sizeof(buf));
  }
  close(fd);
  unlink(myfifo);
  return 0;
}
