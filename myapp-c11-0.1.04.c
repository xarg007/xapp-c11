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
}

#if 0
xadmin@hw:~/xwks.git.1/xapp-c11$ gcc -std=c11 -g -Wall -O0 myapp-c11-0.1.04.c -o myapp -lpthread
xadmin@hw:~/xwks.git.1/xapp-c11$ valgrind --tool=memcheck --leak-check=full --track-origins=yes -s ./myapp
==252879== Memcheck, a memory error detector
==252879== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==252879== Using Valgrind-3.15.0 and LibVEX; rerun with -h for copyright info
==252879== Command: ./myapp
==252879== 
  >> the app starting ... ...
  >> main(1, 0x1ffefffee8)

0x00001ffefffdd0|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|
      =============================================================================
      0x00000000|40 c7 a7 04 00 00 00 00  ==252879== Use of uninitialised value of size 8
==252879==    at 0x48E566A: _itoa_word (_itoa.c:180)
==252879==    by 0x49015A4: __vfprintf_internal (vfprintf-internal.c:1687)
==252879==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==252879==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==252879==    by 0x10972E: DumpHex (myapp-c11-0.1.04.c:107)
==252879==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==252879== 
==252879== Conditional jump or move depends on uninitialised value(s)
==252879==    at 0x48E567C: _itoa_word (_itoa.c:180)
==252879==    by 0x49015A4: __vfprintf_internal (vfprintf-internal.c:1687)
==252879==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==252879==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==252879==    by 0x10972E: DumpHex (myapp-c11-0.1.04.c:107)
==252879==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==252879== 
==252879== Conditional jump or move depends on uninitialised value(s)
==252879==    at 0x4902258: __vfprintf_internal (vfprintf-internal.c:1687)
==252879==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==252879==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==252879==    by 0x10972E: DumpHex (myapp-c11-0.1.04.c:107)
==252879==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==252879== 
==252879== Conditional jump or move depends on uninitialised value(s)
==252879==    at 0x490171E: __vfprintf_internal (vfprintf-internal.c:1687)
==252879==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==252879==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==252879==    by 0x10972E: DumpHex (myapp-c11-0.1.04.c:107)
==252879==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==252879== 
20 92 10 00 00 00 00 ==252879== Conditional jump or move depends on uninitialised value(s)
==252879==    at 0x4902258: __vfprintf_internal (vfprintf-internal.c:1687)
==252879==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==252879==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==252879==    by 0x10975F: DumpHex (myapp-c11-0.1.04.c:108)
==252879==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==252879== 
==252879== Conditional jump or move depends on uninitialised value(s)
==252879==    at 0x490171E: __vfprintf_internal (vfprintf-internal.c:1687)
==252879==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==252879==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==252879==    by 0x10975F: DumpHex (myapp-c11-0.1.04.c:108)
==252879==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==252879== 
00|@.......==252879== Use of uninitialised value of size 8
==252879==    at 0x1097FE: DumpHex (myapp-c11-0.1.04.c:131)
==252879==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==252879== 
==252879== Use of uninitialised value of size 8
==252879==    at 0x10981F: DumpHex (myapp-c11-0.1.04.c:132)
==252879==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==252879== 
==252879== Use of uninitialised value of size 8
==252879==    at 0x109840: DumpHex (myapp-c11-0.1.04.c:133)
==252879==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==252879== 
==252879== Conditional jump or move depends on uninitialised value(s)
==252879==    at 0x109851: DumpHex (myapp-c11-0.1.04.c:134)
==252879==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==252879== 
==252879== Conditional jump or move depends on uninitialised value(s)
==252879==    at 0x109870: DumpHex (myapp-c11-0.1.04.c:140)
==252879==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==252879== 
==252879== Conditional jump or move depends on uninitialised value(s)
==252879==    at 0x491AE0D: _IO_file_overflow@@GLIBC_2.2.5 (fileops.c:783)
==252879==    by 0x490391D: __vfprintf_internal (vfprintf-internal.c:1688)
==252879==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==252879==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==252879==    by 0x10989B: DumpHex (myapp-c11-0.1.04.c:148)
==252879==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==252879== 
==252879== Syscall param write(buf) points to uninitialised byte(s)
==252879==    at 0x49980A7: write (write.c:26)
==252879==    by 0x4918EBC: _IO_file_write@@GLIBC_2.2.5 (fileops.c:1181)
==252879==    by 0x491A980: new_do_write (fileops.c:449)
==252879==    by 0x491A980: _IO_new_do_write (fileops.c:426)
==252879==    by 0x491A980: _IO_do_write@@GLIBC_2.2.5 (fileops.c:423)
==252879==    by 0x49184A7: _IO_file_sync@@GLIBC_2.2.5 (fileops.c:799)
==252879==    by 0x490C3F5: fflush (iofflush.c:40)
==252879==    by 0x109351: xlog_core (myapp-c11-0.1.04.c:23)
==252879==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==252879==    by 0x10989B: DumpHex (myapp-c11-0.1.04.c:148)
==252879==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==252879==  Address 0x4a7f040 is 0 bytes inside a block of size 1,024 alloc'd
==252879==    at 0x483B7F3: malloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==252879==    by 0x490BD33: _IO_file_doallocate (filedoalloc.c:101)
==252879==    by 0x491BEFF: _IO_doallocbuf (genops.c:347)
==252879==    by 0x491AF5F: _IO_file_overflow@@GLIBC_2.2.5 (fileops.c:745)
==252879==    by 0x49196E4: _IO_new_file_xsputn (fileops.c:1244)
==252879==    by 0x49196E4: _IO_file_xsputn@@GLIBC_2.2.5 (fileops.c:1197)
==252879==    by 0x49009A1: __vfprintf_internal (vfprintf-internal.c:1373)
==252879==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==252879==    by 0x109571: xlog_info (myapp-c11-0.1.04.c:54)
==252879==    by 0x109ADC: main (myapp-c11-0.1.04.c:202)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==252879== 
 ==252879== Conditional jump or move depends on uninitialised value(s)
==252879==    at 0x109857: DumpHex (myapp-c11-0.1.04.c:135)
==252879==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==252879== 
.......|
      0x00000010|==252879== Conditional jump or move depends on uninitialised value(s)
==252879==    at 0x4902258: __vfprintf_internal (vfprintf-internal.c:1687)
==252879==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==252879==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==252879==    by 0x1096AD: DumpHex (myapp-c11-0.1.04.c:91)
==252879==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==252879== 
==252879== Conditional jump or move depends on uninitialised value(s)
==252879==    at 0x490171E: __vfprintf_internal (vfprintf-internal.c:1687)
==252879==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==252879==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==252879==    by 0x1096AD: DumpHex (myapp-c11-0.1.04.c:91)
==252879==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==252879== 
e0 fe ff fe 1f 00 00 00  00 29 48 c2 d0 73 61 14|.........)H..sa.|
      0x00000020|00 00 00 00 00 00 00 00  b3 e0 8a 04 00 00 00 00|................|
      0x00000030|20 d6 02 04 00 00 00 00  e8 fe ff fe 1f 00 00 00| ...............|
      0x00000040|00 00 00 00 01 00 00 00  aa 9a 10 00 00 00 00 00|................|
      0x00000050|60 9c 10 00 00 00 00 00  7c f0 ** ** ** ** ** **|`.......|.******|
      =============================================================================

  >> the app create new thread .

0x00001ffefffdd8|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|
      =============================================================================
      0x00000000|00 f7 67 05 00 00 00 00  11 00 00 00 22 00 00 00|..g........."...|
      0x00000010|00 29 48 c2 d0 73 61 14  00 00 00 00 00 00 00 00|.)H..sa.........|
      0x00000020|b3 e0 8a 04 00 00 00 00  20 d6 02 04 00 00 00 00|........==252879== Use of uninitialised value of size 8
==252879==    at 0x1097FE: DumpHex (myapp-c11-0.1.04.c:131)
==252879==    by 0x109BA0: main (myapp-c11-0.1.04.c:226)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x48ADFD3: (below main) (libc-start.c:137)
==252879== 
==252879== Use of uninitialised value of size 8
==252879==    at 0x10981F: DumpHex (myapp-c11-0.1.04.c:132)
==252879==    by 0x109BA0: main (myapp-c11-0.1.04.c:226)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x48ADFD3: (below main) (libc-start.c:137)
==252879== 
==252879== Use of uninitialised value of size 8
==252879==    at 0x109840: DumpHex (myapp-c11-0.1.04.c:133)
==252879==    by 0x109BA0: main (myapp-c11-0.1.04.c:226)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x48ADFD3: (below main) (libc-start.c:137)
==252879== 
==252879== Conditional jump or move depends on uninitialised value(s)
==252879==    at 0x109851: DumpHex (myapp-c11-0.1.04.c:134)
==252879==    by 0x109BA0: main (myapp-c11-0.1.04.c:226)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x48ADFD3: (below main) (libc-start.c:137)
==252879== 
==252879== Conditional jump or move depends on uninitialised value(s)
==252879==    at 0x109870: DumpHex (myapp-c11-0.1.04.c:140)
==252879==    by 0x109BA0: main (myapp-c11-0.1.04.c:226)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x48ADFD3: (below main) (libc-start.c:137)
==252879== 
==252879== Syscall param write(buf) points to uninitialised byte(s)
==252879==    at 0x49980DF: __libc_write (write.c:26)
==252879==    by 0x49980DF: write (write.c:24)
==252879==    by 0x4918EBC: _IO_file_write@@GLIBC_2.2.5 (fileops.c:1181)
==252879==    by 0x491A980: new_do_write (fileops.c:449)
==252879==    by 0x491A980: _IO_new_do_write (fileops.c:426)
==252879==    by 0x491A980: _IO_do_write@@GLIBC_2.2.5 (fileops.c:423)
==252879==    by 0x49184A7: _IO_file_sync@@GLIBC_2.2.5 (fileops.c:799)
==252879==    by 0x490C3F5: fflush (iofflush.c:40)
==252879==    by 0x109351: xlog_core (myapp-c11-0.1.04.c:23)
==252879==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==252879==    by 0x10989B: DumpHex (myapp-c11-0.1.04.c:148)
==252879==    by 0x109BA0: main (myapp-c11-0.1.04.c:226)
==252879==  Address 0x4a7f040 is 0 bytes inside a block of size 1,024 alloc'd
==252879==    at 0x483B7F3: malloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==252879==    by 0x490BD33: _IO_file_doallocate (filedoalloc.c:101)
==252879==    by 0x491BEFF: _IO_doallocbuf (genops.c:347)
==252879==    by 0x491AF5F: _IO_file_overflow@@GLIBC_2.2.5 (fileops.c:745)
==252879==    by 0x49196E4: _IO_new_file_xsputn (fileops.c:1244)
==252879==    by 0x49196E4: _IO_file_xsputn@@GLIBC_2.2.5 (fileops.c:1197)
==252879==    by 0x49009A1: __vfprintf_internal (vfprintf-internal.c:1373)
==252879==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==252879==    by 0x109571: xlog_info (myapp-c11-0.1.04.c:54)
==252879==    by 0x109ADC: main (myapp-c11-0.1.04.c:202)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x48ADFD3: (below main) (libc-start.c:137)
==252879== 
 ==252879== Conditional jump or move depends on uninitialised value(s)
==252879==    at 0x109857: DumpHex (myapp-c11-0.1.04.c:135)
==252879==    by 0x109BA0: main (myapp-c11-0.1.04.c:226)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x48ADFD3: (below main) (libc-start.c:137)
==252879== 
.......|
      0x00000030|e8 fe ff fe 1f 00 00 00  00 00 00 00 01 00 00 00|................|
      0x00000040|aa 9a 10 00 00 00 00 00  60 9c 10 00 00 00 00 00|........`.......|
      0x00000050|7c f0 6a 7f fd 8f b7 70  20 92 ** ** ** ** ** **||.j....p .******|
      =============================================================================

  >> the app create new thread ok.
   ==> thrd_func(0x1ffefffde0) entry.
      => struct s_thrd_param_t p_param=0x1ffefffde0
      => {                     
      =>      int i_test1=0x11;  
      =>      int i_test2=0x22;  
      => };                    

0x0000000567f700|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|
      =============================================================================
      0x00000000|00 f7 67 05 00 00 00 00  90 f4 a7 04 00 00 00 00|..g.............|
      0x00000010|00 f7 67 05 00 00 00 00  01 00 00 00 00 00 00 00|..g.............|
      0x00000020|00 00 00 00 00 00 00 00  00 29 48 c2 d0 73 61 14|.........)H..sa.|
      0x00000030|b5 bf fe c7 5b 38 3e 78  00 00 00 00 00 00 00 00|....[8>x........|
      0x00000040|00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00|................|
      0x00000050|00 00 00 00 00 00 00 00  00 00 ** ** ** ** ** **|..........******|
      =============================================================================

      => Time: Wed Mar 23 15:09:26 2022
      => Time: Wed Mar 23 15:09:28 2022
   ==> thrd_func() exit.
x
  >>> main() join thread ok.(0xfe)
  >> the app exit.
==252879== 
==252879== HEAP SUMMARY:
==252879==     in use at exit: 0 bytes in 0 blocks
==252879==   total heap usage: 12 allocs, 12 frees, 7,429 bytes allocated
==252879== 
==252879== All heap blocks were freed -- no leaks are possible
==252879== 
==252879== ERROR SUMMARY: 414 errors from 23 contexts (suppressed: 0 from 0)
==252879== 
==252879== 1 errors in context 1 of 23:
==252879== Syscall param write(buf) points to uninitialised byte(s)
==252879==    at 0x49980DF: __libc_write (write.c:26)
==252879==    by 0x49980DF: write (write.c:24)
==252879==    by 0x4918EBC: _IO_file_write@@GLIBC_2.2.5 (fileops.c:1181)
==252879==    by 0x491A980: new_do_write (fileops.c:449)
==252879==    by 0x491A980: _IO_new_do_write (fileops.c:426)
==252879==    by 0x491A980: _IO_do_write@@GLIBC_2.2.5 (fileops.c:423)
==252879==    by 0x49184A7: _IO_file_sync@@GLIBC_2.2.5 (fileops.c:799)
==252879==    by 0x490C3F5: fflush (iofflush.c:40)
==252879==    by 0x109351: xlog_core (myapp-c11-0.1.04.c:23)
==252879==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==252879==    by 0x10989B: DumpHex (myapp-c11-0.1.04.c:148)
==252879==    by 0x109BA0: main (myapp-c11-0.1.04.c:226)
==252879==  Address 0x4a7f040 is 0 bytes inside a block of size 1,024 alloc'd
==252879==    at 0x483B7F3: malloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==252879==    by 0x490BD33: _IO_file_doallocate (filedoalloc.c:101)
==252879==    by 0x491BEFF: _IO_doallocbuf (genops.c:347)
==252879==    by 0x491AF5F: _IO_file_overflow@@GLIBC_2.2.5 (fileops.c:745)
==252879==    by 0x49196E4: _IO_new_file_xsputn (fileops.c:1244)
==252879==    by 0x49196E4: _IO_file_xsputn@@GLIBC_2.2.5 (fileops.c:1197)
==252879==    by 0x49009A1: __vfprintf_internal (vfprintf-internal.c:1373)
==252879==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==252879==    by 0x109571: xlog_info (myapp-c11-0.1.04.c:54)
==252879==    by 0x109ADC: main (myapp-c11-0.1.04.c:202)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x48ADFD3: (below main) (libc-start.c:137)
==252879== 
==252879== 
==252879== 2 errors in context 2 of 23:
==252879== Conditional jump or move depends on uninitialised value(s)
==252879==    at 0x490171E: __vfprintf_internal (vfprintf-internal.c:1687)
==252879==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==252879==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==252879==    by 0x10975F: DumpHex (myapp-c11-0.1.04.c:108)
==252879==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==252879== 
==252879== 
==252879== 2 errors in context 3 of 23:
==252879== Conditional jump or move depends on uninitialised value(s)
==252879==    at 0x4902258: __vfprintf_internal (vfprintf-internal.c:1687)
==252879==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==252879==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==252879==    by 0x10975F: DumpHex (myapp-c11-0.1.04.c:108)
==252879==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==252879== 
==252879== 
==252879== 2 errors in context 4 of 23:
==252879== Syscall param write(buf) points to uninitialised byte(s)
==252879==    at 0x49980A7: write (write.c:26)
==252879==    by 0x4918EBC: _IO_file_write@@GLIBC_2.2.5 (fileops.c:1181)
==252879==    by 0x491A980: new_do_write (fileops.c:449)
==252879==    by 0x491A980: _IO_new_do_write (fileops.c:426)
==252879==    by 0x491A980: _IO_do_write@@GLIBC_2.2.5 (fileops.c:423)
==252879==    by 0x49184A7: _IO_file_sync@@GLIBC_2.2.5 (fileops.c:799)
==252879==    by 0x490C3F5: fflush (iofflush.c:40)
==252879==    by 0x109351: xlog_core (myapp-c11-0.1.04.c:23)
==252879==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==252879==    by 0x10989B: DumpHex (myapp-c11-0.1.04.c:148)
==252879==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==252879==  Address 0x4a7f040 is 0 bytes inside a block of size 1,024 alloc'd
==252879==    at 0x483B7F3: malloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==252879==    by 0x490BD33: _IO_file_doallocate (filedoalloc.c:101)
==252879==    by 0x491BEFF: _IO_doallocbuf (genops.c:347)
==252879==    by 0x491AF5F: _IO_file_overflow@@GLIBC_2.2.5 (fileops.c:745)
==252879==    by 0x49196E4: _IO_new_file_xsputn (fileops.c:1244)
==252879==    by 0x49196E4: _IO_file_xsputn@@GLIBC_2.2.5 (fileops.c:1197)
==252879==    by 0x49009A1: __vfprintf_internal (vfprintf-internal.c:1373)
==252879==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==252879==    by 0x109571: xlog_info (myapp-c11-0.1.04.c:54)
==252879==    by 0x109ADC: main (myapp-c11-0.1.04.c:202)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==252879== 
==252879== 
==252879== 3 errors in context 5 of 23:
==252879== Conditional jump or move depends on uninitialised value(s)
==252879==    at 0x491AE0D: _IO_file_overflow@@GLIBC_2.2.5 (fileops.c:783)
==252879==    by 0x490391D: __vfprintf_internal (vfprintf-internal.c:1688)
==252879==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==252879==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==252879==    by 0x10989B: DumpHex (myapp-c11-0.1.04.c:148)
==252879==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==252879== 
==252879== 
==252879== 9 errors in context 6 of 23:
==252879== Conditional jump or move depends on uninitialised value(s)
==252879==    at 0x109870: DumpHex (myapp-c11-0.1.04.c:140)
==252879==    by 0x109BA0: main (myapp-c11-0.1.04.c:226)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x48ADFD3: (below main) (libc-start.c:137)
==252879== 
==252879== 
==252879== 11 errors in context 7 of 23:
==252879== Conditional jump or move depends on uninitialised value(s)
==252879==    at 0x109857: DumpHex (myapp-c11-0.1.04.c:135)
==252879==    by 0x109BA0: main (myapp-c11-0.1.04.c:226)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x48ADFD3: (below main) (libc-start.c:137)
==252879== 
==252879== 
==252879== 12 errors in context 8 of 23:
==252879== Conditional jump or move depends on uninitialised value(s)
==252879==    at 0x109851: DumpHex (myapp-c11-0.1.04.c:134)
==252879==    by 0x109BA0: main (myapp-c11-0.1.04.c:226)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x48ADFD3: (below main) (libc-start.c:137)
==252879== 
==252879== 
==252879== 12 errors in context 9 of 23:
==252879== Use of uninitialised value of size 8
==252879==    at 0x109840: DumpHex (myapp-c11-0.1.04.c:133)
==252879==    by 0x109BA0: main (myapp-c11-0.1.04.c:226)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x48ADFD3: (below main) (libc-start.c:137)
==252879== 
==252879== 
==252879== 12 errors in context 10 of 23:
==252879== Use of uninitialised value of size 8
==252879==    at 0x10981F: DumpHex (myapp-c11-0.1.04.c:132)
==252879==    by 0x109BA0: main (myapp-c11-0.1.04.c:226)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x48ADFD3: (below main) (libc-start.c:137)
==252879== 
==252879== 
==252879== 12 errors in context 11 of 23:
==252879== Use of uninitialised value of size 8
==252879==    at 0x1097FE: DumpHex (myapp-c11-0.1.04.c:131)
==252879==    by 0x109BA0: main (myapp-c11-0.1.04.c:226)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x48ADFD3: (below main) (libc-start.c:137)
==252879== 
==252879== 
==252879== 18 errors in context 12 of 23:
==252879== Conditional jump or move depends on uninitialised value(s)
==252879==    at 0x490171E: __vfprintf_internal (vfprintf-internal.c:1687)
==252879==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==252879==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==252879==    by 0x10972E: DumpHex (myapp-c11-0.1.04.c:107)
==252879==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==252879== 
==252879== 
==252879== 18 errors in context 13 of 23:
==252879== Conditional jump or move depends on uninitialised value(s)
==252879==    at 0x4902258: __vfprintf_internal (vfprintf-internal.c:1687)
==252879==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==252879==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==252879==    by 0x10972E: DumpHex (myapp-c11-0.1.04.c:107)
==252879==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==252879== 
==252879== 
==252879== 18 errors in context 14 of 23:
==252879== Conditional jump or move depends on uninitialised value(s)
==252879==    at 0x109870: DumpHex (myapp-c11-0.1.04.c:140)
==252879==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==252879== 
==252879== 
==252879== 20 errors in context 15 of 23:
==252879== Conditional jump or move depends on uninitialised value(s)
==252879==    at 0x490171E: __vfprintf_internal (vfprintf-internal.c:1687)
==252879==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==252879==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==252879==    by 0x1096AD: DumpHex (myapp-c11-0.1.04.c:91)
==252879==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==252879== 
==252879== 
==252879== 20 errors in context 16 of 23:
==252879== Conditional jump or move depends on uninitialised value(s)
==252879==    at 0x4902258: __vfprintf_internal (vfprintf-internal.c:1687)
==252879==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==252879==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==252879==    by 0x1096AD: DumpHex (myapp-c11-0.1.04.c:91)
==252879==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==252879== 
==252879== 
==252879== 26 errors in context 17 of 23:
==252879== Conditional jump or move depends on uninitialised value(s)
==252879==    at 0x109857: DumpHex (myapp-c11-0.1.04.c:135)
==252879==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==252879== 
==252879== 
==252879== 28 errors in context 18 of 23:
==252879== Conditional jump or move depends on uninitialised value(s)
==252879==    at 0x109851: DumpHex (myapp-c11-0.1.04.c:134)
==252879==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==252879== 
==252879== 
==252879== 28 errors in context 19 of 23:
==252879== Use of uninitialised value of size 8
==252879==    at 0x109840: DumpHex (myapp-c11-0.1.04.c:133)
==252879==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==252879== 
==252879== 
==252879== 28 errors in context 20 of 23:
==252879== Use of uninitialised value of size 8
==252879==    at 0x10981F: DumpHex (myapp-c11-0.1.04.c:132)
==252879==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==252879== 
==252879== 
==252879== 28 errors in context 21 of 23:
==252879== Use of uninitialised value of size 8
==252879==    at 0x1097FE: DumpHex (myapp-c11-0.1.04.c:131)
==252879==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==252879== 
==252879== 
==252879== 52 errors in context 22 of 23:
==252879== Conditional jump or move depends on uninitialised value(s)
==252879==    at 0x48E567C: _itoa_word (_itoa.c:180)
==252879==    by 0x49015A4: __vfprintf_internal (vfprintf-internal.c:1687)
==252879==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==252879==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==252879==    by 0x10972E: DumpHex (myapp-c11-0.1.04.c:107)
==252879==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==252879== 
==252879== 
==252879== 52 errors in context 23 of 23:
==252879== Use of uninitialised value of size 8
==252879==    at 0x48E566A: _itoa_word (_itoa.c:180)
==252879==    by 0x49015A4: __vfprintf_internal (vfprintf-internal.c:1687)
==252879==    by 0x10933F: xlog_core (myapp-c11-0.1.04.c:22)
==252879==    by 0x10944B: xlog_info_hex (myapp-c11-0.1.04.c:36)
==252879==    by 0x10972E: DumpHex (myapp-c11-0.1.04.c:107)
==252879==    by 0x109B28: main (myapp-c11-0.1.04.c:211)
==252879==  Uninitialised value was created by a stack allocation
==252879==    at 0x109AAA: main (myapp-c11-0.1.04.c:201)
==252879== 
==252879== ERROR SUMMARY: 414 errors from 23 contexts (suppressed: 0 from 0)
xadmin@hw:~/xwks.git.1/xapp-c11$ 

#endif
