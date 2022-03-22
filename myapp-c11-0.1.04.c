#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <time.h>
#include <threads.h>

//https://en.cppreference.com/w/c/thread
//05: C11 标准线程测试
//gcc -std=c11 -g -Wall -O0 myapp-c11-0.1.04.c -o myapp -pthread

mtx_t xlog_mutex;
mtx_t xhex_mutex;

//int mtx_lock( mtx_t* mutex );
int xlog_core(unsigned int ui_level, const char* fmt, va_list args)
{
	int iret = vprintf(fmt, args);
	fflush(stdout);
	return iret;
}

int xlog_info(const char* fmt, ...)
{
	mtx_lock(&xlog_mutex);
	
	int iret = 0, log_switch = 1;
	
	if (log_switch)
	{
		va_list args;
		va_start(args, fmt);
		iret = xlog_core(1, fmt, args);
		va_end(args);
	}
	
	mtx_unlock(&xlog_mutex);
	
	return iret;
}

void DumpHex(unsigned char* pData, unsigned int iLen)
{
	if (pData == NULL || iLen == 0) { return; }
	
	mtx_lock(&xhex_mutex);
	
	xlog_info("\n");
	xlog_info("%016p", pData);
	xlog_info("|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|\n");
	xlog_info("      =============================================================================\n");
	
	unsigned int iRow = (iLen % 16 != 0 ? iLen / 16 + 1 : iLen / 16);
	for (unsigned int i = 0; i < iRow; i++)//逐行处理
	{
		//数据相对地址
		xlog_info("      0x%08x|", i * 16);
		
		//十六进制数据
		//xlog_info("\e[32m");
		//当前行1~8列数据
		for (unsigned int j = 0; j < 8; j++)
		{
			if ((i * 16 + j) < iLen)
			{
				xlog_info("%02x ", pData[i * 16 + j]);
			}
			else
			{
				xlog_info("** " );
			}
		}
		
		//在第8列与第9列中加空格列
		xlog_info(" ");
		
		//当前行前9~16列数据
		for (unsigned int j = 8; j < 16; j++)
		{
			if ((i * 16 + j) < iLen)
			{
				if (j < 15) xlog_info("%02x ", pData[i * 16 + j]);
				else        xlog_info("%02x" , pData[i * 16 + j]);
			}
			else
			{
				if (j < 15) xlog_info("** ");
				else        xlog_info("**" );
			}
		}
		
		//xlog_info("\e[0m");
		
		//数据与字符边界
		xlog_info("|");
		
		//显示字符
		for (unsigned int j = 0; j < 16; j++)
		{
			if ((i * 16 + j) < iLen)
			{
				unsigned char test_char = pData[i * 16 + j];
				do
				{
					if(isalpha(test_char)) break;
					if(isdigit(test_char)) break;
					if(ispunct(test_char)) break;
					if(test_char == 0x20 ) break;
					if(test_char == 0x0  ) break;
					test_char = '.';
				}while(0);
				
				if(test_char == 0x0)
				{
					//xlog_info("\e[37m.\e[0m");
					xlog_info(".");
				}
				else
				{
					xlog_info("%c", test_char);
				}
			}
			else
			{
				xlog_info("*");
			}
		}
		
		//行尾边界处理
		xlog_info("|");
		//换下一行
		xlog_info("\n");
	}
	xlog_info("      =============================================================================\n");
	xlog_info("\n");
	mtx_unlock(&xhex_mutex);
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
	struct s_thrd_param_t* p_param = (struct s_thrd_param_t*)pvoid;

	xlog_info("      => struct s_thrd_param_t p_param=%p\n", pvoid);
	xlog_info("      => {                     \n");
	xlog_info("      =>      int i_test1=0x%x;  \n", p_param->i_test1);
	xlog_info("      =>      int i_test2=0x%x;  \n", p_param->i_test2);
	xlog_info("      => };                    \n");
	
	thrd_t thrd_this = thrd_current();
	DumpHex((unsigned char*)thrd_this, 16*5+10);
	
	xlog_info("      => Time: %s", ctime(&(time_t){time(NULL)}));
	thrd_sleep(&(struct timespec){.tv_sec=5}, NULL); // sleep 1 sec
	xlog_info("      => Time: %s", ctime(&(time_t){time(NULL)}));
	xlog_info("   ==> thrd_func() exit.\n");

	return 0xfe;
}

int main(int argc, char* argv[])
{
	xlog_info("  >> the app starting ... ...\n");
	
	mtx_init(&xhex_mutex, mtx_plain);
	mtx_init(&xlog_mutex, mtx_plain);
	
	int iret = 0;
	
	thrd_t thrd_this = thrd_current();
	
	DumpHex((unsigned char*)thrd_this, 16*5+10);
	
	struct s_thrd_param_t param = {0x11, 0x22};
	thrd_t thrd_handle = {0};
	
	do
	{
		xlog_info("  >> the app create new thread .\n");
		iret = thrd_create(&thrd_handle, thrd_func, (void *)&param);
		if(iret != thrd_success)
		{
			xlog_info("  >>> main() creat thread error(%d)\n", iret);
			return -1;
		}
		
		DumpHex((unsigned char*)&thrd_handle, 16*5+10);
		xlog_info("  >> the app create new thread ok.\n");
	}while(0);
	
	while(getchar() != 'x')
	{
		xlog_info("  >> press 'x' exit the app.\n");
	}
	
	do
	{
		int ires = 0;

		iret = thrd_join(thrd_handle, &ires); //(since C11)
	
		if(iret != thrd_success)
		{
			xlog_info("  >>> main() join thread error.(%d)\n", iret);
			return -1;
		}
		
		xlog_info("  >>> main() join thread ok.(0x%x)\n", ires);
	}while(0);
	
	mtx_destroy(&xlog_mutex);
	mtx_destroy(&xhex_mutex);
	xlog_info("  >> the app exit.\n");
}

#if 0
xadmin@hw:~/xwks.git.1/xapp-c11$ gcc -std=c11 -g -Wall -O0 myapp-c11-0.1.04.c -o myapp -pthread
xadmin@hw:~/xwks.git.1/xapp-c11$ ./myapp
  >> the app starting ... ...

//0x007fc0cbe8e740|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|
//      =============================================================================
//      0x00000000|40 e7 e8 cb c0 7f 00 00  a0 f0 e8 cb c0 7f 00 00|@...............|
//      0x00000010|40 e7 e8 cb c0 7f 00 00  00 00 00 00 00 00 00 00|@...............|
//      0x00000020|00 00 00 00 00 00 00 00  00 6c c2 e1 44 4f df 08|.........l..DO..|
//      0x00000030|84 b8 67 a0 5b f3 85 d3  00 00 00 00 00 00 00 00|..g.[...........|
//      0x00000040|00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00|................|
//      0x00000050|00 00 00 00 00 00 00 00  00 00 ** ** ** ** ** **|..........******|
//      =============================================================================

  >> the app create new thread .
  >> the app create new thread ok.
   ==> thrd_func(0x7ffe10ceba30) entry.
      => struct s_thrd_param_t p_param=0x7ffe10ceba30
      => {                     
      =>      int i_test1=0x11;  
      =>      int i_test2=0x22;  
      => };                    

//0x007fc0cbe8d700|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|
//      =============================================================================
//      0x00000000|00 d7 e8 cb c0 7f 00 00  c0 26 50 62 26 56 00 00|.........&Pb&V..|
//      0x00000010|00 d7 e8 cb c0 7f 00 00  01 00 00 00 00 00 00 00|................|
//      0x00000020|00 00 00 00 00 00 00 00  00 6c c2 e1 44 4f df 08|.........l..DO..|
//      0x00000030|84 b8 67 a0 5b f3 85 d3  00 00 00 00 00 00 00 00|..g.[...........|
//      0x00000040|00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00|................|
//      0x00000050|00 00 00 00 00 00 00 00  00 00 ** ** ** ** ** **|..........******|
//      =============================================================================

      => Time: Wed Mar 23 00:33:17 2022
      => Time: Wed Mar 23 00:33:22 2022
   ==> thrd_func() exit.
g
  >> press 'x' exit the app.
  >> press 'x' exit the app.
x

//0x007ffe10ceba20|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|
//      =============================================================================
//      0x00000000|00 d7 e8 cb c0 7f 00 00  40 e7 e8 cb c0 7f 00 00|........@.......|
//      0x00000010|11 00 00 00 22 00 00 00  00 6c c2 e1 44 4f df 08|...."....l..DO..|
//      0x00000020|00 00 00 00 00 00 00 00  b3 50 eb cb c0 7f 00 00|.........P......|
//      0x00000030|00 00 00 00 00 00 00 00  38 bb ce 10 fe 7f 00 00|........8.......|
//      0x00000040|a0 97 07 cc 01 00 00 00  9f 38 62 60 26 56 00 00|.........8b`&V..|
//      0x00000050|20 3a 62 60 26 56 00 00  0b a7 ** ** ** ** ** **| :b`&V....******|
//      =============================================================================

  >>> main() join thread ok.(0xfe)
  >> the app exit.
xadmin@hw:~/xwks.git.1/xapp-c11$ 

#endif
