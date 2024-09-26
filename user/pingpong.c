#include "kernel/types.h"
#include "user.h"

#define BUF_SIZE 20

int main(int argc, char *argv[]) {
  int p2c[2];  // 父进程to子进程管道
  int c2p[2];  // 子进程to父进程管道

  if (pipe(p2c) < 0 || pipe(c2p) < 0) {
    printf("pipe init faild\n");
  }

  if (fork() == 0) {
    close(p2c[1]);
    close(c2p[0]);

    char buf[BUF_SIZE];

    if (read(p2c[0], buf, 10) < 0) {
      printf("child read from parent faild\n");
    }

    int ppid = atoi(buf);
    int cpid = getpid();

    printf("%d: received ping from pid %d\n", cpid, ppid);

    itoa(cpid, buf);
    if (write(c2p[1], buf, 10) < 0) {
      printf("child write to parent faild\n");
    }

    close(p2c[0]);
    close(c2p[1]);
  } else {
    close(p2c[0]);
    close(c2p[1]);

    char buf[BUF_SIZE];
    int ppid = getpid();

    itoa(ppid, buf);

    if (write(p2c[1], buf, 10) < 0) {
      printf("parent write to child faild\n");
    }

    if (read(c2p[0], buf, 10) < 0) {
      printf("parent read from child faild\n");
    }

    int cpid = atoi(buf);
    printf("%d: received pong from pid %d\n", ppid, cpid);

    close(p2c[1]);
    close(c2p[0]);
  }

  exit(0);
}