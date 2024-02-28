#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
  int fd[2];
  pipe(fd);
  for (char num = 2; num <= 35; num++) {
    write(fd[1], &num, 1);  // 将数字写入管道
  }
  close(fd[1]);
  char num;
  int first = 1;
  while (fork() == 0) {
    //printf("pid:%d start\n", getpid());
    int t = dup(fd[0]);
    close(fd[0]);
    pipe(fd);
    if (read(t, &num, 1) == 0) exit(0);
    first = num;
    printf("prime %d\n", num);
    while (read(t, &num, 1)) {
      if (num % first != 0) {
        write(fd[1], &num, 1);
        //printf("pid:%d write %d\n", getpid(), num);
      }
    }
    close(t);
    close(fd[1]);
  }
  close(fd[0]);
  //printf("pid:%d wait\n", getpid());
  wait(0);
  exit(0);
}