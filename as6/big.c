#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define DEFAULT_FILE_NAME "bigfile.txt"

int
main(int argc, char *argv[])
{
  char buf[512] = {0};
  int fd, i, sectors;
  char fname[32] = DEFAULT_FILE_NAME;
  int max_sectors = MAXINT;

  if (argc > 1) {
    memset(fname, 0, 32);
    strcpy(fname, argv[1]);
  }
  if (argc > 2) {
      max_sectors = atoi(argv[2]);
  }

  fd = open(fname, O_CREATE | O_WRONLY);
  if(fd < 0){
    printf(2, "big: cannot open %s for writing\n", fname);
    exit();
  }

  printf(2, "creating %s\n", fname);
  sectors = 0;
  while (sectors < max_sectors) {
    *(int *) buf = sectors;
    int cc = write(fd, buf, sizeof(buf));
    if (cc <= 0) {
      break;
    }
    sectors++;
    if (sectors % 100 == 0) {
      printf(2, ".");
    }
    if (sectors % 1000 == 0) {
      printf(2, " %d\n", sectors);
    }
  }

  printf(1, "\nwrote %d blocks\n", sectors);

  close(fd);
  fd = open(fname, O_RDONLY);
  if(fd < 0){
    printf(2, "big: cannot re-open %s for reading\n", fname);
    exit();
  }
  for(i = 0; i < sectors; i++){
    int cc = read(fd, buf, sizeof(buf));
    if(cc <= 0){
      printf(2, "big: read error at block %d\n", i);
      exit();
    }
    if(*(int*)buf != i){
      printf(2, "big: read the wrong data (%d) for block %d\n",
             *(int*)buf, i);
      exit();
    }
  }

  printf(1, "done; ok\n"); 

  exit();
}
