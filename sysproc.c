#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "stdio.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}
c
int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

char buffer[100] = "....................................................................................................
.XXxXxxxXx+xxx+xx++xx++xx+x+++;++++;+++;+;++++++++++++++++++++++++++++++++++++++++x+x+xxxx$xXxX$Xxx.
.x++xxXxxXxxxXx+xxxx+x+x;;+x;;x+;++;+;;;+;+++++++++++++++++++++++++++++++++++;;+;+;xX+x+++XXXxxxXXx.
.xXXX+Xx+xxx+;+Xxxx+xx+xxxx+xx++;x+;+++++++++++++++++++++++++++++++++++++++++++;x+;;++xx++XX$XXXXxX.
.Xx+xxxx+xxxxXx++x+++x++++;++++++;;++++++++++++++++++++++++++++++++++++++++++;+++;+;++x++;+$XXXxxXX.
.xXxXxxXXx+xx+x+x+;++++x+++;;++;;;+++++++++++++++++++++++++++++++++++++++++++;++++x+x++;Xx;XXXxXXXX.
.+xxxx+++xx+x+xx+xX+x+xx;;+++++;+++++++++++++++++++++++++++++++++;;;+++++++++++;++;;xx++x++$XXXx$X$.
.XxxxxXXx++xxx+x+x;+xx+x+x++;+;;;;;;;;;;++++++++++++++++++++;:....:.....:;+++++++;++x:+++++;XXxXx;x.
.x++XxxxxxxXxxxx+x++;;++;+x;x;::.::.......:++++++++++++++;...;XX&&&&&&&&+.:;++++;+++xxxx++x+$XxXX$X.
.xXxxxx++++xx++xx+xxxxx+;;+::.:X$&&&&&&&&&.:++++++++++++:.+&&&$$&&x$&Xx+X&$:;+++++++++;+xxx++$Xxxx$.
.$X++XxxXx++xXx+xxx++;+x+;:.+&&&&&&&&&&&&&&..;+++++++++:.&&$XX$&&XXx++$$+X;X:;+++++;;++x+++xx$&XX&x.
.+xxxxxxxxXxxxxxx+xx++;;:.;&&&&&;:......$&&&;.:++++++++.x&$$XX$++;XXx$XxX&;x+;++;;++xx++xXx;+$Xx$;x.
.+XXxxXx+xxxx;++x+;xxxx+:+&&$XX+:;;+;;;...$&&X.++++++++.&&$$x:::;x+xxx+x$$X$;:;+++;++++;;+xx++;:$XX.
.X+x+xxxXXXxxX+xxx++x;;:;&&XX$&x;;:::::++;.&&&.++++++++.&&$X$$&X::+X$&&&&&$x;;:;+;+;+xxxX++xxXX;xXX.
.xxX+XxXxx+xxxx++++xxx+:$&$$$$&$;:+XX;:&&&&&&&.++;::;++.:&$;$&$$$;xxxXx:.:&&&x:;++xx;x+++xxxxxx+xxX.
.xxxXxx$xxxxxxxXxx+xx+;:&$X$X&Xx&&&&$&::::...$.;;;&$;:..:&+;:+;$$.:X$$x::.X+$$+;+;;++x++xx++x+x&&&X.
.xxx+xxx+x++xx++++x++;:&&X$XX&$;:::::XX::x:;::$:;;:++x&&x&x:.:xX::::::.:;:..X+:;++;+++++x;;xx++x..&.
.XXxxXxXxXxxxXxXxx+;x+;&$$&&X+;+::::&XX:;::x::&.:;;;;:...&&;X+:+X++::;$;;.&&&x:+++;++++++xxX++xX:.&.
.xXxxX+xxxXXX++++x+x+:X&X$;;$$$+++;;::xx::&&:.&&+:::;;:.:x&..&&+::+X&&x;:;&:.:;+++;;++x+xx++xx+xx.&.
.XXXxXxX+x+x++$Xxx+++:&&X$X:+xXx:::&&&$$$Xx:.;&&+X&;:+&;&&+&:;;$X&$::::;:$+:;+++++++;;;++++x+xxxx..:
.XXxXxxxxxxxxx++++xx;+&$X$&&$:Xx::.:;;:::::.x&&&&..::..:.X&&X;;:.:::;;+;.$:......:;++;;x+xxxx+xx$;.:
.XXXXxxX$Xxx+xxXxx++;&&XXXX$$&&&&&X+:.:....&&&X$&.+&&&&&&&$$&XXXXx::;;;:.&&&&&&&x:...::;xx+x+XXxx+..
.+xXXXxxxx+xxXxxXxX+X&$XXXXXXXX$$X$&&&$x;&&&$XX$&.+&$&XXX$X$&&:.;xx:::..&&$X$X$&&&&&X:.:;++x++++XX:.
.+++++;++xxx+x+x;++&&$XXXXXXXXX$$+x+Xxx$X&$$XXXX&&&&X$XX$xXXX$&&&X;++$&&$$XX$X$$$$X$&&&;.:::;;XxxX&.
.&&&&&&&&&&&&$$$&&&$X$XXXXXXX$XX$&;;;;;$&$XXXXXXX$$$XXXXX$$XX$X$&&&&&&&X$XXXX$$XXX$$X$&&$X&&&&X+++X.
.XXXX$$$$$$XX$$$$X$X$XX$XXXXXXXXX$&&&&&&$XXXXXXXXXXX$$X$&$$XX$X$XXXXXxXXXX$$XXXXXXX$X$X$&X;:::x$&$$.
.X$XXXXXXX$$$$XXXXXXXXX$XXXXXXXXXXXXXXXXXX$XX$XXXXX$XX&Xxxx&$XX$X$X$XXX$XXxXXXXXXX$XXXXXXX&Xx+::$x$.
.XXXXXX$$XX$$XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX$XXX$X$.xX;X&$$XXXX&&$$$&&$&$XXX$xXXXXXX$$$$&&$;+XX.
.XX$$X$X$XXXXX$X$$$X$XXXXX$$$$$$$XX$XX$X$XXXXX$XXXXXX$&X:X++$;$$XX$$$+&Xx+&&&&&&&$$XXXXXXXXXXX&x$xx.
....................................................................................................";
sys_wolfie(char* buff, int usersize)
{
if (usersize = size)
{
  for(int i = 0; i < size; i++)
  buff[i] = buffer[i];
}
return 0;
else
return -1;
};
