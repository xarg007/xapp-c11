#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int xlog_core(unsigned int ui_level, const char *fmt, va_list args)
{
  if (ui_level == 0)
  {
    return 0;
  }

  int iret = vprintf(fmt, args);
  fflush(stdout);
  return iret;
}

int xlog_info_hex(const char *fmt, ...)
{
  int iret = 0;

  int log_switch = 1;
  va_list args = {0};
  if (log_switch)
  {
    va_start(args, fmt);
    iret = xlog_core(1, fmt, args);
    va_end(args);
  }

  return iret;
}

int xlog_info(const char *fmt, ...)
{
  int iret = 0;

  // pthread_mutex_lock(&xlog_mutex);
  int log_switch = 1;
  va_list args = {0};
  if (log_switch)
  {

    va_start(args, fmt);
    iret = xlog_core(1, fmt, args);
    va_end(args);
  }

  // pthread_mutex_unlock(&xlog_mutex);
  return iret;
}

//#define xlog_info printf
//#define xlog_info_hex printf

void DumpHex(unsigned char *pData, unsigned int iLen)
{
  if (pData == NULL || iLen == 0)
  {
    return;
  }
  // pthread_mutex_lock(&xlog_mutex);
  xlog_info_hex("\n");
  xlog_info_hex("%016p", pData);
  xlog_info_hex("|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|\n");
  xlog_info_hex("      =============================================================================\n");

  unsigned int iRow = (iLen % 16 != 0 ? iLen / 16 + 1 : iLen / 16);
  for (unsigned int i = 0; i < iRow; i++) //逐行处理
  {

    //数据相对地址
    xlog_info_hex("      0x%08x|", i * 16);

    //十六进制数据
    // xlog_info("\e[32m");
    //当前行1~8列数据
    for (unsigned int j = 0; j < 8; j++)
    {
      if ((i * 16 + j) < iLen)
      {
        xlog_info_hex("%02x ", *(pData + i * 16 + j));
      }
      else
      {
        xlog_info_hex("** ");
      }
    }

    //在第8列与第9列中加空格列
    xlog_info_hex(" ");

    //当前行前9~16列数据
    for (unsigned int j = 8; j < 16; j++)
    {
      if ((i * 16 + j) < iLen)
      {
        if (j < 15)
          xlog_info_hex("%02x ", pData[i * 16 + j]);
        else
          xlog_info_hex("%02x", pData[i * 16 + j]);
      }
      else
      {
        if (j < 15)
          xlog_info_hex("** ");
        else
          xlog_info_hex("**");
      }
    }

    // xlog_info("\e[0m");

    //数据与字符边界
    xlog_info_hex("|");

    unsigned char test_char = '*';
    //显示字符
    for (unsigned int j = 0; j < 16; j++)
    {
      if ((i * 16 + j) < iLen)
      {
        test_char = *(pData + i * 16 + j);
        do
        {
          if (isalpha(test_char))
            break;
          if (isdigit(test_char))
            break;
          if (ispunct(test_char))
            break;
          if (test_char == 0x20)
            break;
          if (test_char == 0)
            break;

          test_char = '.';
        } while (0);

        if (test_char == 0x0)
        // if(!(test_char||0x0))
        {
          // xlog_info("\e[37m.\e[0m");
          xlog_info_hex(".");
        }
        else
        {
          xlog_info_hex("%c", test_char);
        }
      }
      else
      {
        xlog_info_hex("*");
      }
    }

    //行尾边界处理
    xlog_info_hex("|");
    //换下一行
    xlog_info_hex("\n");
  }
  xlog_info_hex("      =============================================================================\n");
  xlog_info_hex("\n");

  // pthread_mutex_unlock(&xlog_mutex);
  return;
}

int main_proc()
{
  xlog_info("  >> the main_proc() starting ... ...(pid=0x%x)\n", getpid());
  sleep(2);
  xlog_info("  >> the main_proc() exit.(pid=0x%x)\n", getpid());
  return 0;
}

int new_proc()
{
  xlog_info("  >> the new_proc() starting ... ...(pid=0x%x)\n", getpid());
  sleep(5);
  xlog_info("  >> the new_proc() exit.(pid=0x%x)\n", getpid());
  return 0xca;
}

int main(int argc, char *argv[])
{
  xlog_info("  >> the app starting ... ...\n");
  xlog_info("  >> main(%d, %p)\n", argc, argv);

  //pid_t fpid_mproc = getpid();
  int iret = 0;
  pid_t fpid_new = {0};

  fpid_new = fork();

  if (fpid_new > 0)
  {
    //还在主进程，fpid是子进程的进号ID
    iret = main_proc();
  }
  else if (fpid_new == 0)
  {
    //开始执行子进程
    xlog_info("  >> new process continue run ...\n");
    iret = new_proc();
  }
  else
  {
    //还在主进程中，创建子进程失败，需要容错处理
    xlog_info("  >> main() create new process error.\n");
    iret = -1;
  }

  int  status = 0;
  pid_t fpid_qt = {0};
  xlog_info("  >> main() wait new process(0x%x) quit ... ...\n", fpid_new);
  fpid_qt = waitpid(fpid_new, &status, 0);
  if (fpid_new == fpid_qt)
  {
    xlog_info("  >> main() wait new process(0x%x) quit ok.(%x)\n", fpid_qt, status);
    xlog_info("  >> main() wait new process(0x%x) quit.(%s).\n", fpid_new, strerror(errno));
  }
  else if (fpid_qt == -1)
  {
    xlog_info("  >> main() wait new process(0x%x) quit error(%s).\n", fpid_new, strerror(errno));
  }
  else
  {
    xlog_info("  >> main() wait new process(0x%x) quit.(%s).\n", fpid_new, strerror(errno));
  }

  xlog_info("  >> the app(pid=0x%x) exit(iret=%x).\n", getpid(), iret);

  return 0;
}

// pid_t waitpid(pid_t pid,int *status,int options);
//===========================================================
// pid_t pid
//     pid<-1 等待进程组号为pid绝对值的任何子进程。
//     pid=-1 等待任何子进程，此时的waitpid()函数就退化成了普通的wait()函数。
//     pid=0  等待进程组号与目前进程相同的任何子进程，也就是说任何和调用waitpid()函数的进程在同一个进程组的进程。
//     pid>0  等待进程号为pid的子进程。
// int *status
//     这个参数将保存子进程的状态信息，有了这个信息父进程就可以了解子进程为什么会推出，是正常推出还是出了什么错误。如果status不是空指针，则状态信息将被写入
//     器指向的位置。当然，如果不关心子进程为什么推出的话，也可以传入空指针。
//     Linux提供了一些非常有用的宏来帮助解析这个状态信息，这些宏都定义在sys/wait.h头文件中。主要有以下几个：
//     宏                  说明
//     WIFEXITED(status)   如果子进程正常结束，它就返回真；否则返回假。
//     WEXITSTATUS(status) 如果WIFEXITED(status)为真，则可以用该宏取得子进程exit()返回的结束代码。
//     WIFSIGNALED(status) 如果子进程因为一个未捕获的信号而终止，它就返回真；否则返回假。
//     WTERMSIG(status)    如果WIFSIGNALED(status)为真，则可以用该宏获得导致子进程终止的信号代码。
//     WIFSTOPPED(status)  如果当前子进程被暂停了，则返回真；否则返回假。
//     WSTOPSIG(status)    如果WIFSTOPPED(status)为真，则可以使用该宏获得导致子进程暂停的信号代码。
// int options
//     参数options提供了一些另外的选项来控制waitpid()函数的行为。如果不想使用这些选项，则可以把这个参数设为0。
//     主要使用的有以下两个选项：
//     参数	说明
//     WNOHANG	如果pid指定的子进程没有结束，则waitpid()函数立即返回0，而不是阻塞在这个函数上等待；如果结束了，则返回该子进程的进程号。
//     WUNTRACED	如果子进程进入暂停状态，则马上返回。

// https://valgrind.org/docs/manual/manual.html
