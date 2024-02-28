#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(2, "Usage: sleep seconds");
    exit(1);
  }
  int time = atoi(argv[1]);
  //fprintf(1, "%d", time);
  if (!time) {
    fprintf(2, "input should be an integer > 0");
    exit(1);    
  }
  if (sleep(time) < 0) {
      fprintf(2, "sleep: failed\n");
      exit(1);
  }
  exit(0);
}