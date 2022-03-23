#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>

//05: C11 标准线程测试
//gcc -std=c11 -g -Wall -O0 myapp-c11-0.1.04.c -o myapp -pthread

pthread_mutex_t xlog_mutex = {0};

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
	
  pthread_mutex_lock(&xlog_mutex);
	int log_switch = 1;
	va_list args = {0};	
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
#define xlog_info      printf
#define xlog_info_hex  printf
#endif

void DumpHex(unsigned char* pData, unsigned int iLen)
{
	if (pData == NULL || iLen == 0) { return; }
	pthread_mutex_lock(&xlog_mutex);
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

	pthread_mutex_unlock(&xlog_mutex);
	return;
}

/* Create a new thread, starting with execution of START-ROUTINE
   getting passed ARG.  Creation attributed come from ATTR.  The new
   handle is stored in *NEWTHREAD.  */
//extern int pthread_create (pthread_t *__restrict __newthread,
//			   const pthread_attr_t *__restrict __attr,
//			   void *(*__start_routine) (void *),
//			   void *__restrict __arg) __THROWNL __nonnull ((1, 3));

struct s_ptrd_param_t
{
	int i_test1;
	int i_test2;
};

void* thread_func_tst(void * pvoid)
{
  xlog_info("   >>> thread_func_tst(void * param=%p)\n", pvoid);
	if(pvoid == NULL)
		return NULL;

	struct s_ptrd_param_t* p_param = (struct s_ptrd_param_t*)pvoid;

	xlog_info("      => struct s_ptrd_param_t p_param=%p\n", p_param);
	xlog_info("      => {                     \n");
	xlog_info("      =>      int i_test1=0x%x;  \n", p_param->i_test1);
	xlog_info("      =>      int i_test2=0x%x;  \n", p_param->i_test2);
	xlog_info("      => };                    \n");
	
	pthread_t thrd_this = pthread_self();

	DumpHex((unsigned char*)thrd_this, 16*5+10);

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
	//typedef unsigned long int thrd_t;
	//thrd_t thrd_this = thrd_current();
	
	//DumpHex((unsigned char*)&thrd_this, 16*5+10);
	
	struct s_ptrd_param_t param =
      {
        .i_test1 = 0x11,
        .i_test2 = 0x22
      };

	pthread_t ptrd_hd = {0};
	
	do
	{
		xlog_info("  >> the app create new thread .\n");
    iret = pthread_create(&ptrd_hd, NULL, thread_func_tst, (void *)&param);
		if(iret != 0)
		{
			xlog_info("   >>> main() creat thread error(%d)\n", iret);
			return -1;
		}
		
		DumpHex((unsigned char*)&ptrd_hd, 16*5+10);
		xlog_info("  >> the app create new thread ok.\n");
	}while(0);
	
	while(getchar() != 'x')
	{
		xlog_info("  >> press 'x' exit the app.\n");
	}
	
	do
	{
		void* pret = 0;

		iret = pthread_join(ptrd_hd, &pret); //(since C11)
	
		if(iret != 0)
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
	}while(0);
	
	pthread_mutex_destroy(&xlog_mutex);

	xlog_info("  >> the app exit.\n");
}

//https://valgrind.org/docs/manual/manual.html

#if 0
xadmin@hw:~/xwks.git.1/xapp-c11$ valgrind --tool=memcheck --leak-check=full --track-origins=yes -s ./myapp
==256605== Memcheck, a memory error detector
==256605== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==256605== Using Valgrind-3.15.0 and LibVEX; rerun with -h for copyright info
==256605== Command: ./myapp
==256605== 
  >> the app starting ... ...
  >> main(1, 0x1ffefffee8)
  >> the app create new thread .

0x00001ffefffdc8|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|
      =============================================================================
      0x00000000|00 f7 67 05 00 00 00 00  ==256605== Use of uninitialised value of size 8
==256605==    at 0x48E566A: _itoa_word (_itoa.c:180)
==256605==    by 0x49015A4: __vfprintf_internal (vfprintf-internal.c:1687)
==256605==    by 0x1092FF: xlog_core (myapp-c11-0.1.05.c:20)
==256605==    by 0x10940B: xlog_info_hex (myapp-c11-0.1.05.c:34)
==256605==    by 0x1096EE: DumpHex (myapp-c11-0.1.05.c:103)
==256605==    by 0x109AC4: main (myapp-c11-0.1.05.c:238)
==256605==  Uninitialised value was created by a stack allocation
==256605==    at 0x1099E0: main (myapp-c11-0.1.05.c:208)
==256605== 
==256605== Conditional jump or move depends on uninitialised value(s)
==256605==    at 0x48E567C: _itoa_word (_itoa.c:180)
==256605==    by 0x49015A4: __vfprintf_internal (vfprintf-internal.c:1687)
==256605==    by 0x1092FF: xlog_core (myapp-c11-0.1.05.c:20)
==256605==    by 0x10940B: xlog_info_hex (myapp-c11-0.1.05.c:34)
==256605==    by 0x1096EE: DumpHex (myapp-c11-0.1.05.c:103)
==256605==    by 0x109AC4: main (myapp-c11-0.1.05.c:238)
==256605==  Uninitialised value was created by a stack allocation
==256605==    at 0x1099E0: main (myapp-c11-0.1.05.c:208)
==256605== 
==256605== Conditional jump or move depends on uninitialised value(s)
==256605==    at 0x4902258: __vfprintf_internal (vfprintf-internal.c:1687)
==256605==    by 0x1092FF: xlog_core (myapp-c11-0.1.05.c:20)
==256605==    by 0x10940B: xlog_info_hex (myapp-c11-0.1.05.c:34)
==256605==    by 0x1096EE: DumpHex (myapp-c11-0.1.05.c:103)
==256605==    by 0x109AC4: main (myapp-c11-0.1.05.c:238)
==256605==  Uninitialised value was created by a stack allocation
==256605==    at 0x1099E0: main (myapp-c11-0.1.05.c:208)
==256605== 
==256605== Conditional jump or move depends on uninitialised value(s)
==256605==    at 0x490171E: __vfprintf_internal (vfprintf-internal.c:1687)
==256605==    by 0x1092FF: xlog_core (myapp-c11-0.1.05.c:20)
==256605==    by 0x10940B: xlog_info_hex (myapp-c11-0.1.05.c:34)
==256605==    by 0x1096EE: DumpHex (myapp-c11-0.1.05.c:103)
==256605==    by 0x109AC4: main (myapp-c11-0.1.05.c:238)
==256605==  Uninitialised value was created by a stack allocation
==256605==    at 0x1099E0: main (myapp-c11-0.1.05.c:208)
==256605== 
00 00 00 00 00 00 00 ==256605== Conditional jump or move depends on uninitialised value(s)
==256605==    at 0x4902258: __vfprintf_internal (vfprintf-internal.c:1687)
==256605==    by 0x1092FF: xlog_core (myapp-c11-0.1.05.c:20)
==256605==    by 0x10940B: xlog_info_hex (myapp-c11-0.1.05.c:34)
==256605==    by 0x10971F: DumpHex (myapp-c11-0.1.05.c:104)
==256605==    by 0x109AC4: main (myapp-c11-0.1.05.c:238)
==256605==  Uninitialised value was created by a stack allocation
==256605==    at 0x1099E0: main (myapp-c11-0.1.05.c:208)
==256605== 
==256605== Conditional jump or move depends on uninitialised value(s)
==256605==    at 0x490171E: __vfprintf_internal (vfprintf-internal.c:1687)
==256605==    by 0x1092FF: xlog_core (myapp-c11-0.1.05.c:20)
==256605==    by 0x10940B: xlog_info_hex (myapp-c11-0.1.05.c:34)
==256605==    by 0x10971F: DumpHex (myapp-c11-0.1.05.c:104)
==256605==    by 0x109AC4: main (myapp-c11-0.1.05.c:238)
==256605==  Uninitialised value was created by a stack allocation
==256605==    at 0x1099E0: main (myapp-c11-0.1.05.c:208)
==256605== 
00|..g.....==256605== Use of uninitialised value of size 8
==256605==    at 0x1097BE: DumpHex (myapp-c11-0.1.05.c:127)
==256605==    by 0x109AC4: main (myapp-c11-0.1.05.c:238)
==256605==  Uninitialised value was created by a stack allocation
==256605==    at 0x1099E0: main (myapp-c11-0.1.05.c:208)
==256605== 
==256605== Use of uninitialised value of size 8
==256605==    at 0x1097DF: DumpHex (myapp-c11-0.1.05.c:128)
==256605==    by 0x109AC4: main (myapp-c11-0.1.05.c:238)
==256605==  Uninitialised value was created by a stack allocation
==256605==    at 0x1099E0: main (myapp-c11-0.1.05.c:208)
==256605== 
==256605== Use of uninitialised value of size 8
==256605==    at 0x109800: DumpHex (myapp-c11-0.1.05.c:129)
==256605==    by 0x109AC4: main (myapp-c11-0.1.05.c:238)
==256605==  Uninitialised value was created by a stack allocation
==256605==    at 0x1099E0: main (myapp-c11-0.1.05.c:208)
==256605== 
==256605== Conditional jump or move depends on uninitialised value(s)
==256605==    at 0x109811: DumpHex (myapp-c11-0.1.05.c:130)
==256605==    by 0x109AC4: main (myapp-c11-0.1.05.c:238)
==256605==  Uninitialised value was created by a stack allocation
==256605==    at 0x1099E0: main (myapp-c11-0.1.05.c:208)
==256605== 
==256605== Conditional jump or move depends on uninitialised value(s)
==256605==    at 0x109817: DumpHex (myapp-c11-0.1.05.c:131)
==256605==    by 0x109AC4: main (myapp-c11-0.1.05.c:238)
==256605==  Uninitialised value was created by a stack allocation
==256605==    at 0x1099E0: main (myapp-c11-0.1.05.c:208)
==256605== 
==256605== Conditional jump or move depends on uninitialised value(s)
==256605==    at 0x109830: DumpHex (myapp-c11-0.1.05.c:136)
==256605==    by 0x109AC4: main (myapp-c11-0.1.05.c:238)
==256605==  Uninitialised value was created by a stack allocation
==256605==    at 0x1099E0: main (myapp-c11-0.1.05.c:208)
==256605== 
........|
      0x00000010|==256605== Conditional jump or move depends on uninitialised value(s)
==256605==    at 0x4902258: __vfprintf_internal (vfprintf-internal.c:1687)
==256605==    by 0x1092FF: xlog_core (myapp-c11-0.1.05.c:20)
==256605==    by 0x10940B: xlog_info_hex (myapp-c11-0.1.05.c:34)
==256605==    by 0x10966D: DumpHex (myapp-c11-0.1.05.c:87)
==256605==    by 0x109AC4: main (myapp-c11-0.1.05.c:238)
==256605==  Uninitialised value was created by a stack allocation
==256605==    at 0x1099E0: main (myapp-c11-0.1.05.c:208)
==256605== 
==256605== Conditional jump or move depends on uninitialised value(s)
==256605==    at 0x490171E: __vfprintf_internal (vfprintf-internal.c:1687)
==256605==    by 0x1092FF: xlog_core (myapp-c11-0.1.05.c:20)
==256605==    by 0x10940B: xlog_info_hex (myapp-c11-0.1.05.c:34)
==256605==    by 0x10966D: DumpHex (myapp-c11-0.1.05.c:87)
==256605==    by 0x109AC4: main (myapp-c11-0.1.05.c:238)
==256605==  Uninitialised value was created by a stack allocation
==256605==    at 0x1099E0: main (myapp-c11-0.1.05.c:208)
==256605== 
e0 91 10 00 00 00 00 00  11 00 00 00 22 00 00 00|............"...|
      0x00000020|00 21 d9 5d d8 29 2c 1b  00 00 00 00 00 00 00 00|.!.].),.........|
      0x00000030|b3 e0 8a 04 00 00 00 00  20 d6 02 04 00 00 00 00|........==256605== Conditional jump or move depends on uninitialised value(s)
==256605==    at 0x491AE0D: _IO_file_overflow@@GLIBC_2.2.5 (fileops.c:783)
==256605==    by 0x490391D: __vfprintf_internal (vfprintf-internal.c:1688)
==256605==    by 0x1092FF: xlog_core (myapp-c11-0.1.05.c:20)
==256605==    by 0x10940B: xlog_info_hex (myapp-c11-0.1.05.c:34)
==256605==    by 0x10985B: DumpHex (myapp-c11-0.1.05.c:144)
==256605==    by 0x109AC4: main (myapp-c11-0.1.05.c:238)
==256605==  Uninitialised value was created by a stack allocation
==256605==    at 0x48ADFD3: (below main) (libc-start.c:137)
==256605== 
==256605== Syscall param write(buf) points to uninitialised byte(s)
==256605==    at 0x49980DF: __libc_write (write.c:26)
==256605==    by 0x49980DF: write (write.c:24)
==256605==    by 0x4918EBC: _IO_file_write@@GLIBC_2.2.5 (fileops.c:1181)
==256605==    by 0x491A980: new_do_write (fileops.c:449)
==256605==    by 0x491A980: _IO_new_do_write (fileops.c:426)
==256605==    by 0x491A980: _IO_do_write@@GLIBC_2.2.5 (fileops.c:423)
==256605==    by 0x49184A7: _IO_file_sync@@GLIBC_2.2.5 (fileops.c:799)
==256605==    by 0x490C3F5: fflush (iofflush.c:40)
==256605==    by 0x109311: xlog_core (myapp-c11-0.1.05.c:21)
==256605==    by 0x10940B: xlog_info_hex (myapp-c11-0.1.05.c:34)
==256605==    by 0x10985B: DumpHex (myapp-c11-0.1.05.c:144)
==256605==    by 0x109AC4: main (myapp-c11-0.1.05.c:238)
==256605==  Address 0x4a7f040 is 0 bytes inside a block of size 1,024 alloc'd
==256605==    at 0x483B7F3: malloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==256605==    by 0x490BD33: _IO_file_doallocate (filedoalloc.c:101)
==256605==    by 0x491BEFF: _IO_doallocbuf (genops.c:347)
==256605==    by 0x491AF5F: _IO_file_overflow@@GLIBC_2.2.5 (fileops.c:745)
==256605==    by 0x49196E4: _IO_new_file_xsputn (fileops.c:1244)
==256605==    by 0x49196E4: _IO_file_xsputn@@GLIBC_2.2.5 (fileops.c:1197)
==256605==    by 0x49009A1: __vfprintf_internal (vfprintf-internal.c:1373)
==256605==    by 0x1092FF: xlog_core (myapp-c11-0.1.05.c:20)
==256605==    by 0x109531: xlog_info (myapp-c11-0.1.05.c:52)
==256605==    by 0x109A12: main (myapp-c11-0.1.05.c:209)
==256605==  Uninitialised value was created by a stack allocation
==256605==    at 0x48ADFD3: (below main) (libc-start.c:137)
==256605== 
 .......|
      0x00000040|e8 fe ff fe 1f 00 00 00  00 00 00 00 01 00 00 00|................|
      0x00000050|e0 99 10 00 00 00 00 00  00 9c ** ** ** ** ** **|..........******|
      =============================================================================

  >> the app create new thread ok.
   >>> thread_func_tst(void * param=0x1ffefffde0)
      => struct s_ptrd_param_t p_param=0x1ffefffde0
      => {                     
      =>      int i_test1=0x11;  
      =>      int i_test2=0x22;  
      => };                    

0x0000000567f700|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|
      =============================================================================
      0x00000000|00 f7 67 05 00 00 00 00  90 f4 a7 04 00 00 00 00|..g.............|
      0x00000010|00 f7 67 05 00 00 00 00  01 00 00 00 00 00 00 00|..g.............|
      0x00000020|00 00 00 00 00 00 00 00  00 21 d9 5d d8 29 2c 1b|.........!.].),.|
      0x00000030|56 ea 7c 48 d7 50 53 99  00 00 00 00 00 00 00 00|V.|H.PS.........|
      0x00000040|00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00|................|
      0x00000050|00 00 00 00 00 00 00 00  00 00 ** ** ** ** ** **|..........******|
      =============================================================================

   >>> thread_func_tst() exit.
x
  >>> main() join thread ok.(0x0)
      struct s_ptrd_param_t p_param=0x1ffefffde0
      {                     
            int i_test1=0x9;  
            int i_test2=0x8;  
      };                    
  >> the app exit.
==256605== 
==256605== HEAP SUMMARY:
==256605==     in use at exit: 0 bytes in 0 blocks
==256605==   total heap usage: 3 allocs, 3 frees, 2,320 bytes allocated
==256605== 
==256605== All heap blocks were freed -- no leaks are possible
==256605== 
==256605== ERROR SUMMARY: 285 errors from 16 contexts (suppressed: 0 from 0)
==256605== 
==256605== 1 errors in context 1 of 16:
==256605== Syscall param write(buf) points to uninitialised byte(s)
==256605==    at 0x49980DF: __libc_write (write.c:26)
==256605==    by 0x49980DF: write (write.c:24)
==256605==    by 0x4918EBC: _IO_file_write@@GLIBC_2.2.5 (fileops.c:1181)
==256605==    by 0x491A980: new_do_write (fileops.c:449)
==256605==    by 0x491A980: _IO_new_do_write (fileops.c:426)
==256605==    by 0x491A980: _IO_do_write@@GLIBC_2.2.5 (fileops.c:423)
==256605==    by 0x49184A7: _IO_file_sync@@GLIBC_2.2.5 (fileops.c:799)
==256605==    by 0x490C3F5: fflush (iofflush.c:40)
==256605==    by 0x109311: xlog_core (myapp-c11-0.1.05.c:21)
==256605==    by 0x10940B: xlog_info_hex (myapp-c11-0.1.05.c:34)
==256605==    by 0x10985B: DumpHex (myapp-c11-0.1.05.c:144)
==256605==    by 0x109AC4: main (myapp-c11-0.1.05.c:238)
==256605==  Address 0x4a7f040 is 0 bytes inside a block of size 1,024 alloc'd
==256605==    at 0x483B7F3: malloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==256605==    by 0x490BD33: _IO_file_doallocate (filedoalloc.c:101)
==256605==    by 0x491BEFF: _IO_doallocbuf (genops.c:347)
==256605==    by 0x491AF5F: _IO_file_overflow@@GLIBC_2.2.5 (fileops.c:745)
==256605==    by 0x49196E4: _IO_new_file_xsputn (fileops.c:1244)
==256605==    by 0x49196E4: _IO_file_xsputn@@GLIBC_2.2.5 (fileops.c:1197)
==256605==    by 0x49009A1: __vfprintf_internal (vfprintf-internal.c:1373)
==256605==    by 0x1092FF: xlog_core (myapp-c11-0.1.05.c:20)
==256605==    by 0x109531: xlog_info (myapp-c11-0.1.05.c:52)
==256605==    by 0x109A12: main (myapp-c11-0.1.05.c:209)
==256605==  Uninitialised value was created by a stack allocation
==256605==    at 0x48ADFD3: (below main) (libc-start.c:137)
==256605== 
==256605== 
==256605== 1 errors in context 2 of 16:
==256605== Conditional jump or move depends on uninitialised value(s)
==256605==    at 0x491AE0D: _IO_file_overflow@@GLIBC_2.2.5 (fileops.c:783)
==256605==    by 0x490391D: __vfprintf_internal (vfprintf-internal.c:1688)
==256605==    by 0x1092FF: xlog_core (myapp-c11-0.1.05.c:20)
==256605==    by 0x10940B: xlog_info_hex (myapp-c11-0.1.05.c:34)
==256605==    by 0x10985B: DumpHex (myapp-c11-0.1.05.c:144)
==256605==    by 0x109AC4: main (myapp-c11-0.1.05.c:238)
==256605==  Uninitialised value was created by a stack allocation
==256605==    at 0x48ADFD3: (below main) (libc-start.c:137)
==256605== 
==256605== 
==256605== 2 errors in context 3 of 16:
==256605== Conditional jump or move depends on uninitialised value(s)
==256605==    at 0x490171E: __vfprintf_internal (vfprintf-internal.c:1687)
==256605==    by 0x1092FF: xlog_core (myapp-c11-0.1.05.c:20)
==256605==    by 0x10940B: xlog_info_hex (myapp-c11-0.1.05.c:34)
==256605==    by 0x10971F: DumpHex (myapp-c11-0.1.05.c:104)
==256605==    by 0x109AC4: main (myapp-c11-0.1.05.c:238)
==256605==  Uninitialised value was created by a stack allocation
==256605==    at 0x1099E0: main (myapp-c11-0.1.05.c:208)
==256605== 
==256605== 
==256605== 2 errors in context 4 of 16:
==256605== Conditional jump or move depends on uninitialised value(s)
==256605==    at 0x4902258: __vfprintf_internal (vfprintf-internal.c:1687)
==256605==    by 0x1092FF: xlog_core (myapp-c11-0.1.05.c:20)
==256605==    by 0x10940B: xlog_info_hex (myapp-c11-0.1.05.c:34)
==256605==    by 0x10971F: DumpHex (myapp-c11-0.1.05.c:104)
==256605==    by 0x109AC4: main (myapp-c11-0.1.05.c:238)
==256605==  Uninitialised value was created by a stack allocation
==256605==    at 0x1099E0: main (myapp-c11-0.1.05.c:208)
==256605== 
==256605== 
==256605== 8 errors in context 5 of 16:
==256605== Conditional jump or move depends on uninitialised value(s)
==256605==    at 0x490171E: __vfprintf_internal (vfprintf-internal.c:1687)
==256605==    by 0x1092FF: xlog_core (myapp-c11-0.1.05.c:20)
==256605==    by 0x10940B: xlog_info_hex (myapp-c11-0.1.05.c:34)
==256605==    by 0x10966D: DumpHex (myapp-c11-0.1.05.c:87)
==256605==    by 0x109AC4: main (myapp-c11-0.1.05.c:238)
==256605==  Uninitialised value was created by a stack allocation
==256605==    at 0x1099E0: main (myapp-c11-0.1.05.c:208)
==256605== 
==256605== 
==256605== 8 errors in context 6 of 16:
==256605== Conditional jump or move depends on uninitialised value(s)
==256605==    at 0x4902258: __vfprintf_internal (vfprintf-internal.c:1687)
==256605==    by 0x1092FF: xlog_core (myapp-c11-0.1.05.c:20)
==256605==    by 0x10940B: xlog_info_hex (myapp-c11-0.1.05.c:34)
==256605==    by 0x10966D: DumpHex (myapp-c11-0.1.05.c:87)
==256605==    by 0x109AC4: main (myapp-c11-0.1.05.c:238)
==256605==  Uninitialised value was created by a stack allocation
==256605==    at 0x1099E0: main (myapp-c11-0.1.05.c:208)
==256605== 
==256605== 
==256605== 18 errors in context 7 of 16:
==256605== Conditional jump or move depends on uninitialised value(s)
==256605==    at 0x490171E: __vfprintf_internal (vfprintf-internal.c:1687)
==256605==    by 0x1092FF: xlog_core (myapp-c11-0.1.05.c:20)
==256605==    by 0x10940B: xlog_info_hex (myapp-c11-0.1.05.c:34)
==256605==    by 0x1096EE: DumpHex (myapp-c11-0.1.05.c:103)
==256605==    by 0x109AC4: main (myapp-c11-0.1.05.c:238)
==256605==  Uninitialised value was created by a stack allocation
==256605==    at 0x1099E0: main (myapp-c11-0.1.05.c:208)
==256605== 
==256605== 
==256605== 18 errors in context 8 of 16:
==256605== Conditional jump or move depends on uninitialised value(s)
==256605==    at 0x4902258: __vfprintf_internal (vfprintf-internal.c:1687)
==256605==    by 0x1092FF: xlog_core (myapp-c11-0.1.05.c:20)
==256605==    by 0x10940B: xlog_info_hex (myapp-c11-0.1.05.c:34)
==256605==    by 0x1096EE: DumpHex (myapp-c11-0.1.05.c:103)
==256605==    by 0x109AC4: main (myapp-c11-0.1.05.c:238)
==256605==  Uninitialised value was created by a stack allocation
==256605==    at 0x1099E0: main (myapp-c11-0.1.05.c:208)
==256605== 
==256605== 
==256605== 22 errors in context 9 of 16:
==256605== Conditional jump or move depends on uninitialised value(s)
==256605==    at 0x109830: DumpHex (myapp-c11-0.1.05.c:136)
==256605==    by 0x109AC4: main (myapp-c11-0.1.05.c:238)
==256605==  Uninitialised value was created by a stack allocation
==256605==    at 0x1099E0: main (myapp-c11-0.1.05.c:208)
==256605== 
==256605== 
==256605== 27 errors in context 10 of 16:
==256605== Conditional jump or move depends on uninitialised value(s)
==256605==    at 0x109817: DumpHex (myapp-c11-0.1.05.c:131)
==256605==    by 0x109AC4: main (myapp-c11-0.1.05.c:238)
==256605==  Uninitialised value was created by a stack allocation
==256605==    at 0x1099E0: main (myapp-c11-0.1.05.c:208)
==256605== 
==256605== 
==256605== 28 errors in context 11 of 16:
==256605== Conditional jump or move depends on uninitialised value(s)
==256605==    at 0x109811: DumpHex (myapp-c11-0.1.05.c:130)
==256605==    by 0x109AC4: main (myapp-c11-0.1.05.c:238)
==256605==  Uninitialised value was created by a stack allocation
==256605==    at 0x1099E0: main (myapp-c11-0.1.05.c:208)
==256605== 
==256605== 
==256605== 28 errors in context 12 of 16:
==256605== Use of uninitialised value of size 8
==256605==    at 0x109800: DumpHex (myapp-c11-0.1.05.c:129)
==256605==    by 0x109AC4: main (myapp-c11-0.1.05.c:238)
==256605==  Uninitialised value was created by a stack allocation
==256605==    at 0x1099E0: main (myapp-c11-0.1.05.c:208)
==256605== 
==256605== 
==256605== 28 errors in context 13 of 16:
==256605== Use of uninitialised value of size 8
==256605==    at 0x1097DF: DumpHex (myapp-c11-0.1.05.c:128)
==256605==    by 0x109AC4: main (myapp-c11-0.1.05.c:238)
==256605==  Uninitialised value was created by a stack allocation
==256605==    at 0x1099E0: main (myapp-c11-0.1.05.c:208)
==256605== 
==256605== 
==256605== 28 errors in context 14 of 16:
==256605== Use of uninitialised value of size 8
==256605==    at 0x1097BE: DumpHex (myapp-c11-0.1.05.c:127)
==256605==    by 0x109AC4: main (myapp-c11-0.1.05.c:238)
==256605==  Uninitialised value was created by a stack allocation
==256605==    at 0x1099E0: main (myapp-c11-0.1.05.c:208)
==256605== 
==256605== 
==256605== 33 errors in context 15 of 16:
==256605== Conditional jump or move depends on uninitialised value(s)
==256605==    at 0x48E567C: _itoa_word (_itoa.c:180)
==256605==    by 0x49015A4: __vfprintf_internal (vfprintf-internal.c:1687)
==256605==    by 0x1092FF: xlog_core (myapp-c11-0.1.05.c:20)
==256605==    by 0x10940B: xlog_info_hex (myapp-c11-0.1.05.c:34)
==256605==    by 0x1096EE: DumpHex (myapp-c11-0.1.05.c:103)
==256605==    by 0x109AC4: main (myapp-c11-0.1.05.c:238)
==256605==  Uninitialised value was created by a stack allocation
==256605==    at 0x1099E0: main (myapp-c11-0.1.05.c:208)
==256605== 
==256605== 
==256605== 33 errors in context 16 of 16:
==256605== Use of uninitialised value of size 8
==256605==    at 0x48E566A: _itoa_word (_itoa.c:180)
==256605==    by 0x49015A4: __vfprintf_internal (vfprintf-internal.c:1687)
==256605==    by 0x1092FF: xlog_core (myapp-c11-0.1.05.c:20)
==256605==    by 0x10940B: xlog_info_hex (myapp-c11-0.1.05.c:34)
==256605==    by 0x1096EE: DumpHex (myapp-c11-0.1.05.c:103)
==256605==    by 0x109AC4: main (myapp-c11-0.1.05.c:238)
==256605==  Uninitialised value was created by a stack allocation
==256605==    at 0x1099E0: main (myapp-c11-0.1.05.c:208)
==256605== 
==256605== ERROR SUMMARY: 285 errors from 16 contexts (suppressed: 0 from 0)
xadmin@hw:~/xwks.git.1/xapp-c11$ 

#endif
