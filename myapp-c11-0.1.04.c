#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <time.h>
#include <threads.h>

// int mtx_lock( mtx_t* mutex );
// https://en.cppreference.com/w/c/thread
// 05: C11 标准线程测试
// gcc -std=c11 -g -Wall -O0 myapp-c11-0.1.04.c -o myapp -pthread

mtx_t xlog_mutex = { 0 };

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
    mtx_lock(&xlog_mutex);

    int log_switch = 1;
    va_list args = { 0 };
    if (log_switch)
    {

        va_start(args, fmt);
        iret = xlog_core(1, fmt, args);
        va_end(args);
    }

    mtx_unlock(&xlog_mutex);
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

    mtx_lock(&xlog_mutex);

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
    mtx_unlock(&xlog_mutex);

    return;
}

struct s_thrd_param_t
{
    int i_test1;
    int i_test2;
};

int thrd_func(void* pvoid)
{
    xlog_info("   ==> thrd_func(%p) entry.\n", pvoid);
    if (pvoid == NULL)
        return -1;
    struct s_thrd_param_t* p_param = (struct s_thrd_param_t*)pvoid;

    xlog_info("      => struct s_thrd_param_t p_param=%p\n", pvoid);
    xlog_info("      => {                     \n");
    xlog_info("      =>      int i_test1=0x%x;  \n", p_param->i_test1);
    xlog_info("      =>      int i_test2=0x%x;  \n", p_param->i_test2);
    xlog_info("      => };                    \n");

    thrd_t thrd_this = thrd_current();
    DumpHex((unsigned char*)thrd_this, 16 * 5 + 10);

    xlog_info("      => Time: %s", ctime(&(time_t) { time(NULL) }));
    thrd_sleep(&(struct timespec) { .tv_sec = 2 }, NULL); // sleep 1 sec
    xlog_info("      => Time: %s", ctime(&(time_t) { time(NULL) }));
    xlog_info("   ==> thrd_func() exit.\n");

    return 0xfe;
}

int main(int argc, char* argv[])
{
    xlog_info("  >> the app starting ... ...\n");
    xlog_info("  >> main(%d, %p)\n", argc, argv);

    mtx_init(&xlog_mutex, mtx_plain);

    int iret = 0;
    // typedef unsigned long int thrd_t;
    thrd_t thrd_this = thrd_current();

    DumpHex((unsigned char*)&thrd_this, 16 * 5 + 10);

    struct s_thrd_param_t param = { 0x11, 0x22 };
    thrd_t thrd_handle = { 0 };

    do
    {
        xlog_info("  >> the app create new thread .\n");
        iret = thrd_create(&thrd_handle, thrd_func, (void*)&param);
        if (iret != thrd_success)
        {
            xlog_info("  >>> main() creat thread error(%d)\n", iret);
            return -1;
        }

        DumpHex((unsigned char*)&thrd_handle, 16 * 5 + 10);
        xlog_info("  >> the app create new thread ok.\n");
    } while (0);

    while (getchar() != 'x')
    {
        xlog_info("  >> press 'x' exit the app.\n");
    }

    do
    {
        int ires = 0;

        iret = thrd_join(thrd_handle, &ires); //(since C11)

        if (iret != thrd_success)
        {
            xlog_info("  >>> main() join thread error.(%d)\n", iret);
            return -1;
        }

        xlog_info("  >>> main() join thread ok.(0x%x)\n", ires);
    } while (0);

    mtx_destroy(&xlog_mutex);
    xlog_info("  >> the app exit.\n");

    return 0;
}

// https://valgrind.org/docs/manual/manual.html

