#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("please specify your executable");
    exit(1);
  }

  char *argv_exec[argc];
  memmove(argv_exec, argv + 1, (argc - 1) * sizeof(char*));
  char cmd_buf[20];
  char *q = cmd_buf;
  char temp;
  while(read(0, &temp, 1) > 0) {
    if (temp != '\n') {
      *q++ = temp;
      continue; 
    }
    *q = 0;
    if (fork() == 0) {
      argv_exec[argc - 1] = cmd_buf;
      exec(argv[1], argv_exec);
    }
    q = cmd_buf;
  }
  wait(0);
  exit(0);
}