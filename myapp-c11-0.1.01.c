#include <stdio.h>
#include <stdarg.h>

//https://www.runoob.com/cprogramming/c-tutorial.html
//https://en.cppreference.com/w/
//https://zh.cppreference.com/w/c/language/main_function

// gcc -std=c11 -g -Wall -O0 myapp-c11-0.1.00.c -o myapp

//https://zh.cppreference.com/w/c/variadic
//int vprintf(const char *format, va_list arg);
//int printf ( const char * format, ... );

int xlog_core(unsigned int ui_level, const char* fmt, va_list args)
{
	int iret = vprintf(fmt, args);
	fflush(stdout);
	return iret;
}

int xlog_info(const char* fmt, ...)
{
	int iret = 0, log_switch = 1;
	
	if (log_switch)
	{
		va_list args;
		va_start(args, fmt);
		iret = xlog_core(1, fmt, args);
		va_end(args);
	}
	
	return iret;
}

int main(int argc, char *argv[])
{
	xlog_info("  >> the app starting ... ...\n\n");
	
	for(int i=0; i<argc; i++)
	{
		xlog_info("   >> argv[%02d] = {\"%s\"}\n", i, argv[i]);
	}
	
	xlog_info("\n");
	xlog_info("  >> the app exit.\n");
	
	return 0;
}

#if 0
xadmin@hw:~/xwks.git.1/xapp-c11$ ./myapp  gcc -std=c11 -g -Wall -O0 myapp-c11-0.1.01.c -o myapp
  >> the app starting ... ...

   >> argv[00] = {"./myapp"}
   >> argv[01] = {"gcc"}
   >> argv[02] = {"-std=c11"}
   >> argv[03] = {"-g"}
   >> argv[04] = {"-Wall"}
   >> argv[05] = {"-O0"}
   >> argv[06] = {"myapp-c11-0.1.01.c"}
   >> argv[07] = {"-o"}
   >> argv[08] = {"myapp"}

  >> the app exit.
xadmin@hw:~/xwks.git.1/xapp-c11$ 

#endif
