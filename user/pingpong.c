#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  int p[2], q[2];
  pipe(p);
  pipe(q);
  if (fork() == 0) {
    close(0);
    dup(q[0]);
    int pid = getpid();
    char *buff = "1";
    if (read(0, buff, 1)) printf("%d received ping\n", pid);
    else {
      exit(1);
    }
    close(1);
    dup(p[1]);
    write(1, "1", 1);
  } else {
    int orig_w = dup(1);
    close(1);
    dup(q[1]);
    char *buf = "1";
    if (write(1, buf, 1) == 0) exit(1);
    int pid = getpid();
    close(0);
    dup(p[0]);
    if (read(0, buf, 1)) {
      close(1);
      dup(orig_w);
      printf("%d received pong\n", pid);
    }
  }
  exit(0);
}