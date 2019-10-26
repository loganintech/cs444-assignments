#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "rand.h"

int sys_fork(void)
{
  return fork();
}

#ifdef GETPPID
int sys_getppid(void)
{
  int ppid = 1;

  if (myproc()->parent)
  {
    ppid = myproc()->parent->pid;
  }
  return ppid;
}
#endif

int sys_rand(void)
{
  int r = rand();
  return r;
}

int sys_srand(void)
{
  int seed;

  if (argint(0, &seed) < 0)
  {
    return -1;
  }
  srand(seed);
  return 0;
}

int sys_renice(void)
{
  int nice;
  int pid;

  if (argint(0, &nice) < 0)
  {
    return -1;
  }
  if (nice < MIN_NICE_VALUE || nice > MAX_NICE_VALUE)
  {
    return 1;
  }
  if (argint(1, &pid) < 0)
  {
    return -1;
  }

  int r = renice(nice, pid);
  return r;
}

int sys_exit(void)
{
  exit();
  return 0; // not reached
}

int sys_wait(void)
{
  return wait();
}

int sys_kill(void)
{
  int pid;

  if (argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int sys_getpid(void)
{
  return myproc()->pid;
}

int sys_sbrk(void)
{
  int addr;
  int n;

  if (argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

int sys_sleep(void)
{
  int n;
  uint ticks0;

  if (argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n)
  {
    if (myproc()->killed)
    {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

int sys_kdebug(void)
{
  extern uint debugState;

  int tof = FALSE;

  if (argint(0, &tof) < 0)
    return -1;
  debugState = tof;
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int sys_halt(void)
{
  outb(0xf4, 0x00);
  return 0;
}
