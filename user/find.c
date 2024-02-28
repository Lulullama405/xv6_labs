#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *pn, char *fn) {
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(pn, 0)) < 0) {
    fprintf(2, "ls: cannot open %s\n", pn);
    exit(1);
  }

  if (strlen(pn) + 1 + DIRSIZ + 1 > sizeof buf) {
    printf("find: path too long\n");
    exit(1);
  }
  strcpy(buf, pn);
  p = buf + strlen(buf);
  *p++ = '/';
  
  while (read(fd, &de, sizeof(de)) == sizeof(de)) {
    if (de.inum == 0) continue;
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;
    int fd;
    if ((fd = open(buf, 0)) < 0) {
      fprintf(2, "ls: cannot open %s\n", buf);
      exit(1);
    }
    if (fstat(fd, &st) < 0) {
      fprintf(2, "ls: cannot stat %s\n", buf);
      close(fd);
      return;
    }
    close(fd);
    if (strcmp(".", de.name) == 0 || strcmp("..", de.name) == 0) continue;
    if (st.type == T_FILE && strcmp(de.name, fn) == 0) printf("%s\n", buf);
    if (st.type == T_DIR) find(buf, fn);
  }
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("format: find dir filename");
    exit(1);
  }
  char *path_name = argv[1];
  char *file_name = argv[2];
  find(path_name, file_name);
  exit(0);
}