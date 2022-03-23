#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <time.h>
#include <threads.h>

//int mtx_lock( mtx_t* mutex );
//https://en.cppreference.com/w/c/thread
//05: C11 标准线程测试
//gcc -std=c11 -g -Wall -O0 myapp-c11-0.1.04.c -o myapp -pthread

mtx_t xlog_mutex = {0};

#if 1
int xlog_core(unsigned int ui_level, const char* fmt, va_list args)
{
	if(ui_level == 0)
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
	va_list args = {0};	
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
	va_list args = {0};	
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
#define xlog_info      printf
#define xlog_info_hex  printf
#endif

void DumpHex(unsigned char* pData, unsigned int iLen)
{
	if (pData == NULL || iLen == 0) { return; }

	mtx_lock(&xlog_mutex);
	
	xlog_info_hex("\n");
	xlog_info_hex("%016p", pData);
	xlog_info_hex("|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|\n");
	xlog_info_hex("      =============================================================================\n");
	
	unsigned int iRow = (iLen % 16 != 0 ? iLen / 16 + 1 : iLen / 16);
	for (unsigned int i = 0; i < iRow; i++)//逐行处理
	{

		//数据相对地址
		xlog_info_hex("      0x%08x|", i * 16);
		
		//十六进制数据
		//xlog_info("\e[32m");
		//当前行1~8列数据
		for (unsigned int j = 0; j < 8; j++)
		{
			if ((i * 16 + j) < iLen)
			{
				xlog_info_hex("%02x ", *(pData + i*16 + j));
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
				if (j < 15) xlog_info_hex("%02x ", pData[i * 16 + j]);
				else        xlog_info_hex("%02x" , pData[i * 16 + j]);
			}
			else
			{
				if (j < 15) xlog_info_hex("** ");
				else        xlog_info_hex("**" );
			}
		}
		
		//xlog_info("\e[0m");
		
		//数据与字符边界
		xlog_info_hex("|");
	
		unsigned char test_char = '*';
		//显示字符
		for (unsigned int j = 0; j < 16; j++)
		{
			if ((i * 16 + j) < iLen)
			{
				test_char = *(pData + i*16 + j);
				do
				{
					if(isalpha(test_char)) break;
					if(isdigit(test_char)) break;
					if(ispunct(test_char)) break;
					if(test_char == 0x20 ) break;
					if(test_char == 0  ) break;

					test_char = '.';
				}while(0);
				
				if(test_char == 0x0)
				//if(!(test_char||0x0))
				{
					//xlog_info("\e[37m.\e[0m");
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
	if(pvoid == NULL)
		return -1;
	struct s_thrd_param_t* p_param = (struct s_thrd_param_t*)pvoid;

	xlog_info("      => struct s_thrd_param_t p_param=%p\n", pvoid);
	xlog_info("      => {                     \n");
	xlog_info("      =>      int i_test1=0x%x;  \n", p_param->i_test1);
	xlog_info("      =>      int i_test2=0x%x;  \n", p_param->i_test2);
	xlog_info("      => };                    \n");
	
	thrd_t thrd_this = thrd_current();
	DumpHex((unsigned char*)thrd_this, 16*5+10);
	
	xlog_info("      => Time: %s", ctime(&(time_t){time(NULL)}));
	thrd_sleep(&(struct timespec){.tv_sec=2}, NULL); // sleep 1 sec
	xlog_info("      => Time: %s", ctime(&(time_t){time(NULL)}));
	xlog_info("   ==> thrd_func() exit.\n");

	return 0xfe;
}

int main(int argc, char* argv[])
{
	xlog_info("  >> the app starting ... ...\n");
	xlog_info("  >> main(%d, %p)\n", argc, argv);
	
	mtx_init(&xlog_mutex, mtx_plain);
	
	int iret = 0;
	//typedef unsigned long int thrd_t;
	thrd_t thrd_this = thrd_current();
	
	DumpHex((unsigned char*)&thrd_this, 16*5+10);
	
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
	xlog_info("  >> the app exit.\n");

  return 0;
}

//https://valgrind.org/docs/manual/manual.html

#if 0

xadmin@hw:~/xwks.git.1/xapp-c11$ gcc -std=c11 -g -Wall -O0 myapp-c11-0.1.04.c -o myapp -lpthread
myapp-c11-0.1.04.c:258:48: warning: missing terminating ' character
  258 | ==252879== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
      |                                                ^
myapp-c11-0.1.04.c:375:76: warning: missing terminating ' character
  375 | ==252879==  Address 0x4a7f040 is 0 bytes inside a block of size 1,024 alloc'd
      |                                                                            ^
myapp-c11-0.1.04.c:425:79: warning: missing terminating " character
  425 |       0x00000000|00 f7 67 05 00 00 00 00  11 00 00 00 22 00 00 00|..g........."...|
      |                                                                               ^
myapp-c11-0.1.04.c:470:76: warning: missing terminating ' character
  470 | ==252879==  Address 0x4a7f040 is 0 bytes inside a block of size 1,024 alloc'd
      |                                                                            ^
myapp-c11-0.1.04.c:543:76: warning: missing terminating ' character
  543 | ==252879==  Address 0x4a7f040 is 0 bytes inside a block of size 1,024 alloc'd
      |                                                                            ^
myapp-c11-0.1.04.c:593:76: warning: missing terminating ' character
  593 | ==252879==  Address 0x4a7f040 is 0 bytes inside a block of size 1,024 alloc'd
      |                                                                            ^
xadmin@hw:~/xwks.git.1/xapp-c11$ valgrind --tool=memcheck --leak-check=full --track-origins=yes -s ./myapp myapp
==253532== Memcheck, a memory error detector
==253532== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==253532== Using Valgrind-3.15.0 and LibVEX; rerun with -h for copyright info
==253532== Command: ./myapp myapp
==253532== 
  >> the app starting ... ...
  >> main(2, 0x1ffefffed8)

0x00001ffefffdc0|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|
      =============================================================================
      0x00000000|40 c7 a7 04 00 00 00 00  ==253532== Use of uninitialised value of size 8
==253532==    at 0x48E566A: _itoa_word (_itoa.c:180)
==253532==    by 0x49015A4: __vfprintf_internal (vfprintf-internal.c:1687)
==253532==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==253532==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==253532==    by 0x10972E: DumpHex (myapp-c11-0.1.04.c:107)
==253532==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==253532== 
==253532== Conditional jump or move depends on uninitialised value(s)
==253532==    at 0x48E567C: _itoa_word (_itoa.c:180)
==253532==    by 0x49015A4: __vfprintf_internal (vfprintf-internal.c:1687)
==253532==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==253532==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==253532==    by 0x10972E: DumpHex (myapp-c11-0.1.04.c:107)
==253532==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==253532== 
==253532== Conditional jump or move depends on uninitialised value(s)
==253532==    at 0x4902258: __vfprintf_internal (vfprintf-internal.c:1687)
==253532==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==253532==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==253532==    by 0x10972E: DumpHex (myapp-c11-0.1.04.c:107)
==253532==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==253532== 
==253532== Conditional jump or move depends on uninitialised value(s)
==253532==    at 0x490171E: __vfprintf_internal (vfprintf-internal.c:1687)
==253532==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==253532==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==253532==    by 0x10972E: DumpHex (myapp-c11-0.1.04.c:107)
==253532==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==253532== 
20 92 10 00 00 00 00 ==253532== Conditional jump or move depends on uninitialised value(s)
==253532==    at 0x4902258: __vfprintf_internal (vfprintf-internal.c:1687)
==253532==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==253532==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==253532==    by 0x10975F: DumpHex (myapp-c11-0.1.04.c:108)
==253532==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==253532== 
==253532== Conditional jump or move depends on uninitialised value(s)
==253532==    at 0x490171E: __vfprintf_internal (vfprintf-internal.c:1687)
==253532==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==253532==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==253532==    by 0x10975F: DumpHex (myapp-c11-0.1.04.c:108)
==253532==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==253532== 
00|@.......==253532== Use of uninitialised value of size 8
==253532==    at 0x1097FE: DumpHex (myapp-c11-0.1.04.c:131)
==253532==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==253532== 
==253532== Use of uninitialised value of size 8
==253532==    at 0x10981F: DumpHex (myapp-c11-0.1.04.c:132)
==253532==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==253532== 
==253532== Use of uninitialised value of size 8
==253532==    at 0x109840: DumpHex (myapp-c11-0.1.04.c:133)
==253532==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==253532== 
==253532== Conditional jump or move depends on uninitialised value(s)
==253532==    at 0x109851: DumpHex (myapp-c11-0.1.04.c:134)
==253532==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==253532== 
==253532== Conditional jump or move depends on uninitialised value(s)
==253532==    at 0x109870: DumpHex (myapp-c11-0.1.04.c:140)
==253532==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==253532== 
==253532== Conditional jump or move depends on uninitialised value(s)
==253532==    at 0x491AE0D: _IO_file_overflow@@GLIBC_2.2.5 (fileops.c:783)
==253532==    by 0x490391D: __vfprintf_internal (vfprintf-internal.c:1688)
==253532==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==253532==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==253532==    by 0x10989B: DumpHex (myapp-c11-0.1.04.c:148)
==253532==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==253532== 
==253532== Syscall param write(buf) points to uninitialised byte(s)
==253532==    at 0x49980A7: write (write.c:26)
==253532==    by 0x4918EBC: _IO_file_write@@GLIBC_2.2.5 (fileops.c:1181)
==253532==    by 0x491A980: new_do_write (fileops.c:449)
==253532==    by 0x491A980: _IO_new_do_write (fileops.c:426)
==253532==    by 0x491A980: _IO_do_write@@GLIBC_2.2.5 (fileops.c:423)
==253532==    by 0x49184A7: _IO_file_sync@@GLIBC_2.2.5 (fileops.c:799)
==253532==    by 0x490C3F5: fflush (iofflush.c:40)
==253532==    by 0x109351: xlog_core (myapp-c11-0.1.04.c:23)
==253532==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==253532==    by 0x10989B: DumpHex (myapp-c11-0.1.04.c:148)
==253532==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==253532==  Address 0x4a7f040 is 0 bytes inside a block of size 1,024 alloc'd
==253532==    at 0x483B7F3: malloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==253532==    by 0x490BD33: _IO_file_doallocate (filedoalloc.c:101)
==253532==    by 0x491BEFF: _IO_doallocbuf (genops.c:347)
==253532==    by 0x491AF5F: _IO_file_overflow@@GLIBC_2.2.5 (fileops.c:745)
==253532==    by 0x49196E4: _IO_new_file_xsputn (fileops.c:1244)
==253532==    by 0x49196E4: _IO_file_xsputn@@GLIBC_2.2.5 (fileops.c:1197)
==253532==    by 0x49009A1: __vfprintf_internal (vfprintf-internal.c:1373)
==253532==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==253532==    by 0x109571: xlog_info (myapp-c11-0.1.04.c:54)
==253532==    by 0x109ADC: main (myapp-c11-0.1.04.c:202)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==253532== 
 ==253532== Conditional jump or move depends on uninitialised value(s)
==253532==    at 0x109857: DumpHex (myapp-c11-0.1.04.c:135)
==253532==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==253532== 
.......|
      0x00000010|==253532== Conditional jump or move depends on uninitialised value(s)
==253532==    at 0x4902258: __vfprintf_internal (vfprintf-internal.c:1687)
==253532==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==253532==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==253532==    by 0x1096AD: DumpHex (myapp-c11-0.1.04.c:91)
==253532==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==253532== 
==253532== Conditional jump or move depends on uninitialised value(s)
==253532==    at 0x490171E: __vfprintf_internal (vfprintf-internal.c:1687)
==253532==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==253532==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==253532==    by 0x1096AD: DumpHex (myapp-c11-0.1.04.c:91)
==253532==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==253532== 
d0 fe ff fe 1f 00 00 00  00 66 b3 28 92 7b f6 06|.........f.(.{..|
      0x00000020|00 00 00 00 00 00 00 00  b3 e0 8a 04 00 00 00 00|................|
      0x00000030|20 d6 02 04 00 00 00 00  d8 fe ff fe 1f 00 00 00| ...............|
      0x00000040|00 00 00 00 02 00 00 00  aa 9a 10 00 00 00 00 00|................|
      0x00000050|60 9c 10 00 00 00 00 00  e0 9f ** ** ** ** ** **|`.........******|
      =============================================================================

  >> the app create new thread .

0x00001ffefffdc8|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|
      =============================================================================
      0x00000000|00 f7 67 05 00 00 00 00  11 00 00 00 22 00 00 00|..g........."...|
      0x00000010|00 66 b3 28 92 7b f6 06  00 00 00 00 00 00 00 00|.f.(.{..........|
      0x00000020|b3 e0 8a 04 00 00 00 00  20 d6 02 04 00 00 00 00|........==253532== Use of uninitialised value of size 8
==253532==    at 0x1097FE: DumpHex (myapp-c11-0.1.04.c:131)
==253532==    by 0x109BA0: main (myapp-c11-0.1.04.c:226)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x48ADFD3: (below main) (libc-start.c:137)
==253532== 
==253532== Use of uninitialised value of size 8
==253532==    at 0x10981F: DumpHex (myapp-c11-0.1.04.c:132)
==253532==    by 0x109BA0: main (myapp-c11-0.1.04.c:226)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x48ADFD3: (below main) (libc-start.c:137)
==253532== 
==253532== Use of uninitialised value of size 8
==253532==    at 0x109840: DumpHex (myapp-c11-0.1.04.c:133)
==253532==    by 0x109BA0: main (myapp-c11-0.1.04.c:226)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x48ADFD3: (below main) (libc-start.c:137)
==253532== 
==253532== Conditional jump or move depends on uninitialised value(s)
==253532==    at 0x109851: DumpHex (myapp-c11-0.1.04.c:134)
==253532==    by 0x109BA0: main (myapp-c11-0.1.04.c:226)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x48ADFD3: (below main) (libc-start.c:137)
==253532== 
==253532== Conditional jump or move depends on uninitialised value(s)
==253532==    at 0x109870: DumpHex (myapp-c11-0.1.04.c:140)
==253532==    by 0x109BA0: main (myapp-c11-0.1.04.c:226)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x48ADFD3: (below main) (libc-start.c:137)
==253532== 
==253532== Syscall param write(buf) points to uninitialised byte(s)
==253532==    at 0x49980DF: __libc_write (write.c:26)
==253532==    by 0x49980DF: write (write.c:24)
==253532==    by 0x4918EBC: _IO_file_write@@GLIBC_2.2.5 (fileops.c:1181)
==253532==    by 0x491A980: new_do_write (fileops.c:449)
==253532==    by 0x491A980: _IO_new_do_write (fileops.c:426)
==253532==    by 0x491A980: _IO_do_write@@GLIBC_2.2.5 (fileops.c:423)
==253532==    by 0x49184A7: _IO_file_sync@@GLIBC_2.2.5 (fileops.c:799)
==253532==    by 0x490C3F5: fflush (iofflush.c:40)
==253532==    by 0x109351: xlog_core (myapp-c11-0.1.04.c:23)
==253532==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==253532==    by 0x10989B: DumpHex (myapp-c11-0.1.04.c:148)
==253532==    by 0x109BA0: main (myapp-c11-0.1.04.c:226)
==253532==  Address 0x4a7f040 is 0 bytes inside a block of size 1,024 alloc'd
==253532==    at 0x483B7F3: malloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==253532==    by 0x490BD33: _IO_file_doallocate (filedoalloc.c:101)
==253532==    by 0x491BEFF: _IO_doallocbuf (genops.c:347)
==253532==    by 0x491AF5F: _IO_file_overflow@@GLIBC_2.2.5 (fileops.c:745)
==253532==    by 0x49196E4: _IO_new_file_xsputn (fileops.c:1244)
==253532==    by 0x49196E4: _IO_file_xsputn@@GLIBC_2.2.5 (fileops.c:1197)
==253532==    by 0x49009A1: __vfprintf_internal (vfprintf-internal.c:1373)
==253532==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==253532==    by 0x109571: xlog_info (myapp-c11-0.1.04.c:54)
==253532==    by 0x109ADC: main (myapp-c11-0.1.04.c:202)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x48ADFD3: (below main) (libc-start.c:137)
==253532== 
 ==253532== Conditional jump or move depends on uninitialised value(s)
==253532==    at 0x109857: DumpHex (myapp-c11-0.1.04.c:135)
==253532==    by 0x109BA0: main (myapp-c11-0.1.04.c:226)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x48ADFD3: (below main) (libc-start.c:137)
==253532== 
.......|
      0x00000030|d8 fe ff fe 1f 00 00 00  00 00 00 00 02 00 00 00|................|
      0x00000040|aa 9a 10 00 00 00 00 00  60 9c 10 00 00 00 00 00|........`.......|
      0x00000050|e0 9f 0f 83 9c ca fc e2  20 92 ** ** ** ** ** **|........ .******|
      =============================================================================

  >> the app create new thread ok.
   ==> thrd_func(0x1ffefffdd0) entry.
      => struct s_thrd_param_t p_param=0x1ffefffdd0
      => {                     
      =>      int i_test1=0x11;  
      =>      int i_test2=0x22;  
      => };                    

0x0000000567f700|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|
      =============================================================================
      0x00000000|00 f7 67 05 00 00 00 00  90 f4 a7 04 00 00 00 00|..g.............|
      0x00000010|00 f7 67 05 00 00 00 00  01 00 00 00 00 00 00 00|..g.............|
      0x00000020|00 00 00 00 00 00 00 00  00 66 b3 28 92 7b f6 06|.........f.(.{..|
      0x00000030|87 41 4e 65 7e 71 f0 cf  00 00 00 00 00 00 00 00|.ANe~q..........|
      0x00000040|00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00|................|
      0x00000050|00 00 00 00 00 00 00 00  00 00 ** ** ** ** ** **|..........******|
      =============================================================================

      => Time: Wed Mar 23 15:20:24 2022
x
      => Time: Wed Mar 23 15:20:26 2022
   ==> thrd_func() exit.
  >>> main() join thread ok.(0xfe)
  >> the app exit.
==253532== 
==253532== HEAP SUMMARY:
==253532==     in use at exit: 0 bytes in 0 blocks
==253532==   total heap usage: 12 allocs, 12 frees, 7,429 bytes allocated
==253532== 
==253532== All heap blocks were freed -- no leaks are possible
==253532== 
==253532== ERROR SUMMARY: 414 errors from 23 contexts (suppressed: 0 from 0)
==253532== 
==253532== 1 errors in context 1 of 23:
==253532== Syscall param write(buf) points to uninitialised byte(s)
==253532==    at 0x49980DF: __libc_write (write.c:26)
==253532==    by 0x49980DF: write (write.c:24)
==253532==    by 0x4918EBC: _IO_file_write@@GLIBC_2.2.5 (fileops.c:1181)
==253532==    by 0x491A980: new_do_write (fileops.c:449)
==253532==    by 0x491A980: _IO_new_do_write (fileops.c:426)
==253532==    by 0x491A980: _IO_do_write@@GLIBC_2.2.5 (fileops.c:423)
==253532==    by 0x49184A7: _IO_file_sync@@GLIBC_2.2.5 (fileops.c:799)
==253532==    by 0x490C3F5: fflush (iofflush.c:40)
==253532==    by 0x109351: xlog_core (myapp-c11-0.1.04.c:23)
==253532==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==253532==    by 0x10989B: DumpHex (myapp-c11-0.1.04.c:148)
==253532==    by 0x109BA0: main (myapp-c11-0.1.04.c:226)
==253532==  Address 0x4a7f040 is 0 bytes inside a block of size 1,024 alloc'd
==253532==    at 0x483B7F3: malloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==253532==    by 0x490BD33: _IO_file_doallocate (filedoalloc.c:101)
==253532==    by 0x491BEFF: _IO_doallocbuf (genops.c:347)
==253532==    by 0x491AF5F: _IO_file_overflow@@GLIBC_2.2.5 (fileops.c:745)
==253532==    by 0x49196E4: _IO_new_file_xsputn (fileops.c:1244)
==253532==    by 0x49196E4: _IO_file_xsputn@@GLIBC_2.2.5 (fileops.c:1197)
==253532==    by 0x49009A1: __vfprintf_internal (vfprintf-internal.c:1373)
==253532==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==253532==    by 0x109571: xlog_info (myapp-c11-0.1.04.c:54)
==253532==    by 0x109ADC: main (myapp-c11-0.1.04.c:202)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x48ADFD3: (below main) (libc-start.c:137)
==253532== 
==253532== 
==253532== 2 errors in context 2 of 23:
==253532== Conditional jump or move depends on uninitialised value(s)
==253532==    at 0x490171E: __vfprintf_internal (vfprintf-internal.c:1687)
==253532==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==253532==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==253532==    by 0x10975F: DumpHex (myapp-c11-0.1.04.c:108)
==253532==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==253532== 
==253532== 
==253532== 2 errors in context 3 of 23:
==253532== Conditional jump or move depends on uninitialised value(s)
==253532==    at 0x4902258: __vfprintf_internal (vfprintf-internal.c:1687)
==253532==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==253532==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==253532==    by 0x10975F: DumpHex (myapp-c11-0.1.04.c:108)
==253532==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==253532== 
==253532== 
==253532== 2 errors in context 4 of 23:
==253532== Syscall param write(buf) points to uninitialised byte(s)
==253532==    at 0x49980A7: write (write.c:26)
==253532==    by 0x4918EBC: _IO_file_write@@GLIBC_2.2.5 (fileops.c:1181)
==253532==    by 0x491A980: new_do_write (fileops.c:449)
==253532==    by 0x491A980: _IO_new_do_write (fileops.c:426)
==253532==    by 0x491A980: _IO_do_write@@GLIBC_2.2.5 (fileops.c:423)
==253532==    by 0x49184A7: _IO_file_sync@@GLIBC_2.2.5 (fileops.c:799)
==253532==    by 0x490C3F5: fflush (iofflush.c:40)
==253532==    by 0x109351: xlog_core (myapp-c11-0.1.04.c:23)
==253532==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==253532==    by 0x10989B: DumpHex (myapp-c11-0.1.04.c:148)
==253532==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==253532==  Address 0x4a7f040 is 0 bytes inside a block of size 1,024 alloc'd
==253532==    at 0x483B7F3: malloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==253532==    by 0x490BD33: _IO_file_doallocate (filedoalloc.c:101)
==253532==    by 0x491BEFF: _IO_doallocbuf (genops.c:347)
==253532==    by 0x491AF5F: _IO_file_overflow@@GLIBC_2.2.5 (fileops.c:745)
==253532==    by 0x49196E4: _IO_new_file_xsputn (fileops.c:1244)
==253532==    by 0x49196E4: _IO_file_xsputn@@GLIBC_2.2.5 (fileops.c:1197)
==253532==    by 0x49009A1: __vfprintf_internal (vfprintf-internal.c:1373)
==253532==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==253532==    by 0x109571: xlog_info (myapp-c11-0.1.04.c:54)
==253532==    by 0x109ADC: main (myapp-c11-0.1.04.c:202)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==253532== 
==253532== 
==253532== 3 errors in context 5 of 23:
==253532== Conditional jump or move depends on uninitialised value(s)
==253532==    at 0x491AE0D: _IO_file_overflow@@GLIBC_2.2.5 (fileops.c:783)
==253532==    by 0x490391D: __vfprintf_internal (vfprintf-internal.c:1688)
==253532==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==253532==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==253532==    by 0x10989B: DumpHex (myapp-c11-0.1.04.c:148)
==253532==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==253532== 
==253532== 
==253532== 9 errors in context 6 of 23:
==253532== Conditional jump or move depends on uninitialised value(s)
==253532==    at 0x109870: DumpHex (myapp-c11-0.1.04.c:140)
==253532==    by 0x109BA0: main (myapp-c11-0.1.04.c:226)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x48ADFD3: (below main) (libc-start.c:137)
==253532== 
==253532== 
==253532== 11 errors in context 7 of 23:
==253532== Conditional jump or move depends on uninitialised value(s)
==253532==    at 0x109857: DumpHex (myapp-c11-0.1.04.c:135)
==253532==    by 0x109BA0: main (myapp-c11-0.1.04.c:226)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x48ADFD3: (below main) (libc-start.c:137)
==253532== 
==253532== 
==253532== 12 errors in context 8 of 23:
==253532== Conditional jump or move depends on uninitialised value(s)
==253532==    at 0x109851: DumpHex (myapp-c11-0.1.04.c:134)
==253532==    by 0x109BA0: main (myapp-c11-0.1.04.c:226)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x48ADFD3: (below main) (libc-start.c:137)
==253532== 
==253532== 
==253532== 12 errors in context 9 of 23:
==253532== Use of uninitialised value of size 8
==253532==    at 0x109840: DumpHex (myapp-c11-0.1.04.c:133)
==253532==    by 0x109BA0: main (myapp-c11-0.1.04.c:226)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x48ADFD3: (below main) (libc-start.c:137)
==253532== 
==253532== 
==253532== 12 errors in context 10 of 23:
==253532== Use of uninitialised value of size 8
==253532==    at 0x10981F: DumpHex (myapp-c11-0.1.04.c:132)
==253532==    by 0x109BA0: main (myapp-c11-0.1.04.c:226)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x48ADFD3: (below main) (libc-start.c:137)
==253532== 
==253532== 
==253532== 12 errors in context 11 of 23:
==253532== Use of uninitialised value of size 8
==253532==    at 0x1097FE: DumpHex (myapp-c11-0.1.04.c:131)
==253532==    by 0x109BA0: main (myapp-c11-0.1.04.c:226)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x48ADFD3: (below main) (libc-start.c:137)
==253532== 
==253532== 
==253532== 18 errors in context 12 of 23:
==253532== Conditional jump or move depends on uninitialised value(s)
==253532==    at 0x490171E: __vfprintf_internal (vfprintf-internal.c:1687)
==253532==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==253532==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==253532==    by 0x10972E: DumpHex (myapp-c11-0.1.04.c:107)
==253532==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==253532== 
==253532== 
==253532== 18 errors in context 13 of 23:
==253532== Conditional jump or move depends on uninitialised value(s)
==253532==    at 0x4902258: __vfprintf_internal (vfprintf-internal.c:1687)
==253532==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==253532==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==253532==    by 0x10972E: DumpHex (myapp-c11-0.1.04.c:107)
==253532==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==253532== 
==253532== 
==253532== 18 errors in context 14 of 23:
==253532== Conditional jump or move depends on uninitialised value(s)
==253532==    at 0x109870: DumpHex (myapp-c11-0.1.04.c:140)
==253532==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==253532== 
==253532== 
==253532== 20 errors in context 15 of 23:
==253532== Conditional jump or move depends on uninitialised value(s)
==253532==    at 0x490171E: __vfprintf_internal (vfprintf-internal.c:1687)
==253532==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==253532==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==253532==    by 0x1096AD: DumpHex (myapp-c11-0.1.04.c:91)
==253532==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==253532== 
==253532== 
==253532== 20 errors in context 16 of 23:
==253532== Conditional jump or move depends on uninitialised value(s)
==253532==    at 0x4902258: __vfprintf_internal (vfprintf-internal.c:1687)
==253532==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==253532==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==253532==    by 0x1096AD: DumpHex (myapp-c11-0.1.04.c:91)
==253532==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==253532== 
==253532== 
==253532== 26 errors in context 17 of 23:
==253532== Conditional jump or move depends on uninitialised value(s)
==253532==    at 0x109857: DumpHex (myapp-c11-0.1.04.c:135)
==253532==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==253532== 
==253532== 
==253532== 28 errors in context 18 of 23:
==253532== Conditional jump or move depends on uninitialised value(s)
==253532==    at 0x109851: DumpHex (myapp-c11-0.1.04.c:134)
==253532==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==253532== 
==253532== 
==253532== 28 errors in context 19 of 23:
==253532== Use of uninitialised value of size 8
==253532==    at 0x109840: DumpHex (myapp-c11-0.1.04.c:133)
==253532==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==253532== 
==253532== 
==253532== 28 errors in context 20 of 23:
==253532== Use of uninitialised value of size 8
==253532==    at 0x10981F: DumpHex (myapp-c11-0.1.04.c:132)
==253532==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==253532== 
==253532== 
==253532== 28 errors in context 21 of 23:
==253532== Use of uninitialised value of size 8
==253532==    at 0x1097FE: DumpHex (myapp-c11-0.1.04.c:131)
==253532==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==253532== 
==253532== 
==253532== 52 errors in context 22 of 23:
==253532== Conditional jump or move depends on uninitialised value(s)
==253532==    at 0x48E567C: _itoa_word (_itoa.c:180)
==253532==    by 0x49015A4: __vfprintf_internal (vfprintf-internal.c:1687)
==253532==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==253532==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==253532==    by 0x10972E: DumpHex (myapp-c11-0.1.04.c:107)
==253532==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==253532== 
==253532== 
==253532== 52 errors in context 23 of 23:
==253532== Use of uninitialised value of size 8
==253532==    at 0x48E566A: _itoa_word (_itoa.c:180)
==253532==    by 0x49015A4: __vfprintf_internal (vfprintf-internal.c:1687)
==253532==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==253532==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==253532==    by 0x10972E: DumpHex (myapp-c11-0.1.04.c:107)
==253532==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==253532==  Uninitialised value was created by a stack allocation
==253532==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==253532== 
==253532== ERROR SUMMARY: 414 errors from 23 contexts (suppressed: 0 from 0)
xadmin@hw:~/xwks.git.1/xapp-c11$ 

#endif

#if 0
xadmin@hw:~/xwks.git.1/xapp-c11$ valgrind --help
usage: valgrind [options] prog-and-args

  tool-selection option, with default in [ ]:
    --tool=<name>             use the Valgrind tool named <name> [memcheck]

  basic user options for all Valgrind tools, with defaults in [ ]:
    -h --help                 show this message
    --help-debug              show this message, plus debugging options
    --version                 show version
    -q --quiet                run silently; only print error msgs
    -v --verbose              be more verbose -- show misc extra info
    --trace-children=no|yes   Valgrind-ise child processes (follow execve)? [no]
    --trace-children-skip=patt1,patt2,...    specifies a list of executables
                              that --trace-children=yes should not trace into
    --trace-children-skip-by-arg=patt1,patt2,...   same as --trace-children-skip=
                              but check the argv[] entries for children, rather
                              than the exe name, to make a follow/no-follow decision
    --child-silent-after-fork=no|yes omit child output between fork & exec? [no]
    --vgdb=no|yes|full        activate gdbserver? [yes]
                              full is slower but provides precise watchpoint/step
    --vgdb-error=<number>     invoke gdbserver after <number> errors [999999999]
                              to get started quickly, use --vgdb-error=0
                              and follow the on-screen directions
    --vgdb-stop-at=event1,event2,... invoke gdbserver for given events [none]
         where event is one of:
           startup exit valgrindabexit all none
    --track-fds=no|yes        track open file descriptors? [no]
    --time-stamp=no|yes       add timestamps to log messages? [no]
    --log-fd=<number>         log messages to file descriptor [2=stderr]
    --log-file=<file>         log messages to <file>
    --log-socket=ipaddr:port  log messages to socket ipaddr:port

  user options for Valgrind tools that report errors:
    --xml=yes                 emit error output in XML (some tools only)
    --xml-fd=<number>         XML output to file descriptor
    --xml-file=<file>         XML output to <file>
    --xml-socket=ipaddr:port  XML output to socket ipaddr:port
    --xml-user-comment=STR    copy STR verbatim into XML output
    --demangle=no|yes         automatically demangle C++ names? [yes]
    --num-callers=<number>    show <number> callers in stack traces [12]
    --error-limit=no|yes      stop showing new errors if too many? [yes]
    --exit-on-first-error=no|yes exit code on the first error found? [no]
    --error-exitcode=<number> exit code to return if errors found [0=disable]
    --error-markers=<begin>,<end> add lines with begin/end markers before/after
                              each error output in plain text mode [none]
    --show-error-list=no|yes  show detected errors list and
                              suppression counts at exit [no]
    -s                        same as --show-error-list=yes
    --keep-debuginfo=no|yes   Keep symbols etc for unloaded code [no]
                              This allows saved stack traces (e.g. memory leaks)
                              to include file/line info for code that has been
                              dlclose'd (or similar)
    --show-below-main=no|yes  continue stack traces below main() [no]
    --default-suppressions=yes|no
                              load default suppressions [yes]
    --suppressions=<filename> suppress errors described in <filename>
    --gen-suppressions=no|yes|all    print suppressions for errors? [no]
    --input-fd=<number>       file descriptor for input [0=stdin]
    --dsymutil=no|yes         run dsymutil on Mac OS X when helpful? [yes]
    --max-stackframe=<number> assume stack switch for SP changes larger
                              than <number> bytes [2000000]
    --main-stacksize=<number> set size of main thread's stack (in bytes)
                              [min(max(current 'ulimit' value,1MB),16MB)]

  user options for Valgrind tools that replace malloc:
    --alignment=<number>      set minimum alignment of heap allocations [16]
    --redzone-size=<number>   set minimum size of redzones added before/after
                              heap blocks (in bytes). [16]
    --xtree-memory=none|allocs|full   profile heap memory in an xtree [none]
                              and produces a report at the end of the execution
                              none: no profiling, allocs: current allocated
                              size/blocks, full: profile current and cumulative
                              allocated size/blocks and freed size/blocks.
    --xtree-memory-file=<file>   xtree memory report file [xtmemory.kcg.%p]

  uncommon user options for all Valgrind tools:
    --fullpath-after=         (with nothing after the '=')
                              show full source paths in call stacks
    --fullpath-after=string   like --fullpath-after=, but only show the
                              part of the path after 'string'.  Allows removal
                              of path prefixes.  Use this flag multiple times
                              to specify a set of prefixes to remove.
    --extra-debuginfo-path=path    absolute path to search for additional
                              debug symbols, in addition to existing default
                              well known search paths.
    --debuginfo-server=ipaddr:port    also query this server
                              (valgrind-di-server) for debug symbols
    --allow-mismatched-debuginfo=no|yes  [no]
                              for the above two flags only, accept debuginfo
                              objects that don't "match" the main object
    --smc-check=none|stack|all|all-non-file [all-non-file]
                              checks for self-modifying code: none, only for
                              code found in stacks, for all code, or for all
                              code except that from file-backed mappings
    --read-inline-info=yes|no read debug info about inlined function calls
                              and use it to do better stack traces.
                              [yes] on Linux/Android/Solaris for the tools
                              Memcheck/Massif/Helgrind/DRD only.
                              [no] for all other tools and platforms.
    --read-var-info=yes|no    read debug info on stack and global variables
                              and use it to print better error messages in
                              tools that make use of it (Memcheck, Helgrind,
                              DRD) [no]
    --vgdb-poll=<number>      gdbserver poll max every <number> basic blocks [5000] 
    --vgdb-shadow-registers=no|yes   let gdb see the shadow registers [no]
    --vgdb-prefix=<prefix>    prefix for vgdb FIFOs [/tmp/vgdb-pipe]
    --run-libc-freeres=no|yes free up glibc memory at exit on Linux? [yes]
    --run-cxx-freeres=no|yes  free up libstdc++ memory at exit on Linux
                              and Solaris? [yes]
    --sim-hints=hint1,hint2,...  activate unusual sim behaviours [none] 
         where hint is one of:
           lax-ioctls lax-doors fuse-compatible enable-outer
           no-inner-prefix no-nptl-pthread-stackcache fallback-llsc none
    --fair-sched=no|yes|try   schedule threads fairly on multicore systems [no]
    --kernel-variant=variant1,variant2,...
         handle non-standard kernel variants [none]
         where variant is one of:
           bproc android-no-hw-tls
           android-gpu-sgx5xx android-gpu-adreno3xx none
    --merge-recursive-frames=<number>  merge frames between identical
           program counters in max <number> frames) [0]
    --num-transtab-sectors=<number> size of translated code cache [32]
           more sectors may increase performance, but use more memory.
    --avg-transtab-entry-size=<number> avg size in bytes of a translated
           basic block [0, meaning use tool provided default]
    --aspace-minaddr=0xPP     avoid mapping memory below 0xPP [guessed]
    --valgrind-stacksize=<number> size of valgrind (host) thread's stack
                               (in bytes) [1048576]
    --show-emwarns=no|yes     show warnings about emulation limits? [no]
    --require-text-symbol=:sonamepattern:symbolpattern    abort run if the
                              stated shared object doesn't have the stated
                              text symbol.  Patterns can contain ? and *.
    --soname-synonyms=syn1=pattern1,syn2=pattern2,... synonym soname
              specify patterns for function wrapping or replacement.
              To use a non-libc malloc library that is
                  in the main exe:  --soname-synonyms=somalloc=NONE
                  in libxyzzy.so:   --soname-synonyms=somalloc=libxyzzy.so
    --sigill-diagnostics=yes|no  warn about illegal instructions? [yes]
    --unw-stack-scan-thresh=<number>   Enable stack-scan unwind if fewer
                  than <number> good frames found  [0, meaning "disabled"]
                  NOTE: stack scanning is only available on arm-linux.
    --unw-stack-scan-frames=<number>   Max number of frames that can be
                  recovered by stack scanning [5]
    --resync-filter=no|yes|verbose [yes on MacOS, no on other OSes]
              attempt to avoid expensive address-space-resync operations
    --max-threads=<number>    maximum number of threads that valgrind can
                              handle [500]

  user options for Memcheck:
    --leak-check=no|summary|full     search for memory leaks at exit?  [summary]
    --leak-resolution=low|med|high   differentiation of leak stack traces [high]
    --show-leak-kinds=kind1,kind2,.. which leak kinds to show?
                                            [definite,possible]
    --errors-for-leak-kinds=kind1,kind2,..  which leak kinds are errors?
                                            [definite,possible]
        where kind is one of:
          definite indirect possible reachable all none
    --leak-check-heuristics=heur1,heur2,... which heuristics to use for
        improving leak search false positive [all]
        where heur is one of:
          stdstring length64 newarray multipleinheritance all none
    --show-reachable=yes             same as --show-leak-kinds=all
    --show-reachable=no --show-possibly-lost=yes
                                     same as --show-leak-kinds=definite,possible
    --show-reachable=no --show-possibly-lost=no
                                     same as --show-leak-kinds=definite
    --xtree-leak=no|yes              output leak result in xtree format? [no]
    --xtree-leak-file=<file>         xtree leak report file [xtleak.kcg.%p]
    --undef-value-errors=no|yes      check for undefined value errors [yes]
    --track-origins=no|yes           show origins of undefined values? [no]
    --partial-loads-ok=no|yes        too hard to explain here; see manual [yes]
    --expensive-definedness-checks=no|auto|yes
                                     Use extra-precise definedness tracking [auto]
    --freelist-vol=<number>          volume of freed blocks queue     [20000000]
    --freelist-big-blocks=<number>   releases first blocks with size>= [1000000]
    --workaround-gcc296-bugs=no|yes  self explanatory [no].  Deprecated.
                                     Use --ignore-range-below-sp instead.
    --ignore-ranges=0xPP-0xQQ[,0xRR-0xSS]   assume given addresses are OK
    --ignore-range-below-sp=<number>-<number>  do not report errors for
                                     accesses at the given offsets below SP
    --malloc-fill=<hexnumber>        fill malloc'd areas with given value
    --free-fill=<hexnumber>          fill free'd areas with given value
    --keep-stacktraces=alloc|free|alloc-and-free|alloc-then-free|none
        stack trace(s) to keep for malloc'd/free'd areas       [alloc-and-free]
    --show-mismatched-frees=no|yes   show frees that don't match the allocator? [yes]

  Extra options read from ~/.valgrindrc, $VALGRIND_OPTS, ./.valgrindrc

  Memcheck is Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
  Valgrind is Copyright (C) 2000-2017, and GNU GPL'd, by Julian Seward et al.
  LibVEX is Copyright (C) 2004-2017, and GNU GPL'd, by OpenWorks LLP et al.

  Bug reports, feedback, admiration, abuse, etc, to: www.valgrind.org.

xadmin@hw:~/xwks.git.1/xapp-c11$ 


#endif
