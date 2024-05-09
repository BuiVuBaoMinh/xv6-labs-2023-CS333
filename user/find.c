#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char* target;

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memset(buf, 0, sizeof(buf));
  memmove(buf, p, strlen(p));
//   memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void
find_dir(char *path)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, O_RDONLY)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    // printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
    fprintf(2, "find: %s not a dir\n", path);
    return;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
      return;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("find: cannot stat %s\n", buf);
        continue;
      }
    //   printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
      if (st.type == T_FILE) {
        if (strcmp(fmtname(buf), target) == 0) {
          printf("%s\n", buf);
        } 
      } else if (st.type == T_DIR) {
        if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
          continue;
        find_dir(buf);
      }
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  if (argc < 2 || argc > 3) {
    fprintf(2, "Usage: find (path) [file]\n");
    exit(1);
  } else if (argc < 3) {
    target = argv[1];
    find_dir(".");
  } else {
    target = argv[2];
    find_dir(argv[1]);
  }

  exit(0);
}