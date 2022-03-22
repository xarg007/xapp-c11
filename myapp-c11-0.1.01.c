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

int main_bak(int argc, char *argv[])
{
	xlog_info("  >> the app starting ... ...\n\n");
	
	for(int i=0; i<argc; i++)
	{
		xlog_info("   >> argv[%02d] = {\"%s\"};\n", i, argv[i]);
	}
	
	xlog_info("\n");
	xlog_info("  >> the app exit.\n");
	
	return 0;
}

int main(int argc, char *argv[], char** penv)
{
	xlog_info("  >> the app starting ... ...\n\n");
	
	for(int i=0; i<argc; i++)
	{
		xlog_info("   >>> argv[%02d] = {\"%s\"};\n", i, argv[i]);
	}
	
	xlog_info("\n");
	
	for(int i=0; (*(penv+i) != NULL); i++)
	{
		xlog_info("   >>> penv[%02d] = {\"%s\"};\n", i, penv[i]);
	}
	
	xlog_info("\n");
	
	xlog_info("  >> the app exit.\n");
	
	return 0;
}

#if 0
xadmin@hw:~/xwks.git.1/xapp-c11$ ./myapp  gcc -std=c11 -g -Wall -O0 myapp-c11-0.1.01.c -o myapp
  >> the app starting ... ...

   >>> argv[00] = {"./myapp"};
   >>> argv[01] = {"gcc"};
   >>> argv[02] = {"-std=c11"};
   >>> argv[03] = {"-g"};
   >>> argv[04] = {"-Wall"};
   >>> argv[05] = {"-O0"};
   >>> argv[06] = {"myapp-c11-0.1.01.c"};
   >>> argv[07] = {"-o"};
   >>> argv[08] = {"myapp"};

   >>> penv[00] = {"SHELL=/bin/bash"};
   >>> penv[01] = {"LANGUAGE=zh_CN:en_US:en"};
   >>> penv[02] = {"LC_ADDRESS=zh_CN.UTF-8"};
   >>> penv[03] = {"LC_NAME=zh_CN.UTF-8"};
   >>> penv[04] = {"LC_MONETARY=zh_CN.UTF-8"};
   >>> penv[05] = {"PWD=/home/xadmin/xwks.git.1/xapp-c11"};
   >>> penv[06] = {"LOGNAME=xadmin"};
   >>> penv[07] = {"XDG_SESSION_TYPE=tty"};
   >>> penv[08] = {"MOTD_SHOWN=pam"};
   >>> penv[09] = {"HOME=/home/xadmin"};
   >>> penv[10] = {"LC_PAPER=zh_CN.UTF-8"};
   >>> penv[11] = {"LANG=zh_CN.UTF-8"};
   >>> penv[12] = {"LS_COLORS=rs=0:di=01;34:ln=01;36:mh=00:pi=40;33:so=01;35:do=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:mi=00:su=37;41:sg=30;43:ca=30;41:tw=30;42:ow=34;42:st=37;44:ex=01;32:*.tar=01;31:*.tgz=01;31:*.arc=01;31:*.arj=01;31:*.taz=01;31:*.lha=01;31:*.lz4=01;31:*.lzh=01;31:*.lzma=01;31:*.tlz=01;31:*.txz=01;31:*.tzo=01;31:*.t7z=01;31:*.zip=01;31:*.z=01;31:*.dz=01;31:*.gz=01;31:*.lrz=01;31:*.lz=01;31:*.lzo=01;31:*.xz=01;31:*.zst=01;31:*.tzst=01;31:*.bz2=01;31:*.bz=01;31:*.tbz=01;31:*.tbz2=01;31:*.tz=01;31:*.deb=01;31:*.rpm=01;31:*.jar=01;31:*.war=01;31:*.ear=01;31:*.sar=01;31:*.rar=01;31:*.alz=01;31:*.ace=01;31:*.zoo=01;31:*.cpio=01;31:*.7z=01;31:*.rz=01;31:*.cab=01;31:*.wim=01;31:*.swm=01;31:*.dwm=01;31:*.esd=01;31:*.jpg=01;35:*.jpeg=01;35:*.mjpg=01;35:*.mjpeg=01;35:*.gif=01;35:*.bmp=01;35:*.pbm=01;35:*.pgm=01;35:*.ppm=01;35:*.tga=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:*.tiff=01;35:*.png=01;35:*.svg=01;35:*.svgz=01;35:*.mng=01;35:*.pcx=01;35:*.mov=01;35:*.mpg=01;35:*.mpeg=01;35:*.m2v=01;35:*.mkv=01;35:*.webm=01;35:*.ogm=01;35:*.mp4=01;35:*.m4v=01;35:*.mp4v=01;35:*.vob=01;35:*.qt=01;35:*.nuv=01;35:*.wmv=01;35:*.asf=01;35:*.rm=01;35:*.rmvb=01;35:*.flc=01;35:*.avi=01;35:*.fli=01;35:*.flv=01;35:*.gl=01;35:*.dl=01;35:*.xcf=01;35:*.xwd=01;35:*.yuv=01;35:*.cgm=01;35:*.emf=01;35:*.ogv=01;35:*.ogx=01;35:*.aac=00;36:*.au=00;36:*.flac=00;36:*.m4a=00;36:*.mid=00;36:*.midi=00;36:*.mka=00;36:*.mp3=00;36:*.mpc=00;36:*.ogg=00;36:*.ra=00;36:*.wav=00;36:*.oga=00;36:*.opus=00;36:*.spx=00;36:*.xspf=00;36:"};
   >>> penv[13] = {"SSH_CONNECTION=192.168.31.196 63078 192.168.31.64 22"};
   >>> penv[14] = {"LESSCLOSE=/usr/bin/lesspipe %s %s"};
   >>> penv[15] = {"XDG_SESSION_CLASS=user"};
   >>> penv[16] = {"LC_IDENTIFICATION=zh_CN.UTF-8"};
   >>> penv[17] = {"TERM=xterm"};
   >>> penv[18] = {"LESSOPEN=| /usr/bin/lesspipe %s"};
   >>> penv[19] = {"USER=xadmin"};
   >>> penv[20] = {"SHLVL=1"};
   >>> penv[21] = {"LC_TELEPHONE=zh_CN.UTF-8"};
   >>> penv[22] = {"LC_MEASUREMENT=zh_CN.UTF-8"};
   >>> penv[23] = {"XDG_SESSION_ID=15"};
   >>> penv[24] = {"PAPERSIZE=a4"};
   >>> penv[25] = {"XDG_RUNTIME_DIR=/run/user/1000"};
   >>> penv[26] = {"SSH_CLIENT=192.168.31.196 63078 22"};
   >>> penv[27] = {"LC_TIME=zh_CN.UTF-8"};
   >>> penv[28] = {"XDG_DATA_DIRS=/usr/local/share:/usr/share:/var/lib/snapd/desktop"};
   >>> penv[29] = {"PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin"};
   >>> penv[30] = {"DBUS_SESSION_BUS_ADDRESS=unix:path=/run/user/1000/bus"};
   >>> penv[31] = {"SSH_TTY=/dev/pts/1"};
   >>> penv[32] = {"LC_NUMERIC=zh_CN.UTF-8"};
   >>> penv[33] = {"_=./myapp"};
   >>> penv[34] = {"OLDPWD=/home/xadmin/xwks.git.1"};

  >> the app exit.
xadmin@hw:~/xwks.git.1/xapp-c11$ 


#endif
