#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>

// 05: C11 posix thread test
// gcc -std=c11 -g -Wall -O0 myapp-c11-0.1.04.c -o myapp -pthread

pthread_mutex_t xlog_mutex = { 0 };

#if 1
int xlog_core(unsigned int ui_level, const char* fmt, va_list args)
{
    if (ui_level == 0)
    {
        return 0;
    }

    int iret = vprintf(fmt, args);
    fflush(stdout);
    return iret;
}

int xlog_info_hex(const char* fmt, ...)
{
    int iret = 0;

    int log_switch = 1;
    va_list args = { 0 };
    if (log_switch)
    {
        va_start(args, fmt);
        iret = xlog_core(1, fmt, args);
        va_end(args);
    }

    return iret;
}

int xlog_info(const char* fmt, ...)
{
    int iret = 0;

    pthread_mutex_lock(&xlog_mutex);
    int log_switch = 1;
    va_list args = { 0 };
    if (log_switch)
    {

        va_start(args, fmt);
        iret = xlog_core(1, fmt, args);
        va_end(args);
    }

    pthread_mutex_unlock(&xlog_mutex);
    return iret;
}
#else
#define xlog_info printf
#define xlog_info_hex printf
#endif

void DumpHex(unsigned char* pData, unsigned int iLen)
{
    if (pData == NULL || iLen == 0)
    {
        return;
    }
    pthread_mutex_lock(&xlog_mutex);
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

    pthread_mutex_unlock(&xlog_mutex);
    return;
}

/* Create a new thread, starting with execution of START-ROUTINE
   getting passed ARG.  Creation attributed come from ATTR.  The new
   handle is stored in *NEWTHREAD.  */
   // extern int pthread_create (pthread_t *__restrict __newthread,
   //			   const pthread_attr_t *__restrict __attr,
   //			   void *(*__start_routine) (void *),
   //			   void *__restrict __arg) __THROWNL __nonnull ((1, 3));

struct s_ptrd_param_t
{
    int i_test1;
    int i_test2;
};

void* thread_func_tst(void* pvoid)
{
    xlog_info("   >>> thread_func_tst(void * param=%p)\n", pvoid);
    if (pvoid == NULL)
        return NULL;

    struct s_ptrd_param_t* p_param = (struct s_ptrd_param_t*)pvoid;

    xlog_info("      => struct s_ptrd_param_t p_param=%p\n", p_param);
    xlog_info("      => {                     \n");
    xlog_info("      =>      int i_test1=0x%x;  \n", p_param->i_test1);
    xlog_info("      =>      int i_test2=0x%x;  \n", p_param->i_test2);
    xlog_info("      => };                    \n");

    pthread_t thrd_this = pthread_self();

    DumpHex((unsigned char*)thrd_this, 16 * 5 + 10);

    sleep(2);

    xlog_info("   >>> thread_func_tst() exit.\n");

    p_param->i_test1 = 0x9;
    p_param->i_test2 = 0x8;

    return (void*)p_param;
}

int main(int argc, char* argv[])
{
    xlog_info("  >> the app starting ... ...\n");
    xlog_info("  >> main(%d, %p)\n", argc, argv);

    pthread_mutex_init(&xlog_mutex, NULL);

    int iret = 0;

    pthread_t thrd_this = pthread_self();

    DumpHex((unsigned char*)&thrd_this, 16 * 5 + 10);

    struct s_ptrd_param_t param =
    {
        .i_test1 = 0x11,
        .i_test2 = 0x22 };

    pthread_t ptrd_hd = { 0 };

    do
    {
        xlog_info("  >> the app create new thread .\n");
        iret = pthread_create(&ptrd_hd, NULL, thread_func_tst, (void*)&param);
        if (iret != 0)
        {
            xlog_info("   >>> main() creat thread error(%d)\n", iret);
            return -1;
        }

        DumpHex((unsigned char*)&ptrd_hd, 16 * 5 + 10);
        xlog_info("  >> the app create new thread ok.\n");
    } while (0);

    while (getchar() != 'x')
    {
        xlog_info("  >> press 'x' exit the app.\n");
    }

    do
    {
        void* pret = 0;

        iret = pthread_join(ptrd_hd, &pret); //(since C11)

        if (iret != 0)
        {
            xlog_info("   >>> main() join thread error.(thread_func() ret = %p)\n", pret);
            return -1;
        }
        else
        {
            xlog_info("  >>> main() join thread ok.(0x%x)\n", iret);
            struct s_ptrd_param_t* p_param = (struct s_ptrd_param_t*)pret;
            xlog_info("      struct s_ptrd_param_t p_param=%p\n", p_param);
            xlog_info("      {                     \n");
            xlog_info("            int i_test1=0x%x;  \n", p_param->i_test1);
            xlog_info("            int i_test2=0x%x;  \n", p_param->i_test2);
            xlog_info("      };                    \n");
        }
    } while (0);

    pthread_mutex_destroy(&xlog_mutex);

    xlog_info("  >> the app exit.\n");
    return 0;
}

// https://valgrind.org/docs/manual/manual.html
