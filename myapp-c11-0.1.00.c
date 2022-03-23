#include <stdio.h>

// https://www.runoob.com/cprogramming/c-tutorial.html
// https://en.cppreference.com/w/
// https://zh.cppreference.com/w/c/language/main_function

// gcc -std=c11 -g -Wall -O0 -o myapp myapp-c11-0.1.00.c
// gcc -std=c11 -g -Wall -O0 myapp-c11-0.1.00.c -o myapp
// gcc xapp-c11-0.1.00.c -o myapp

int main(int argc, char* argv[])
{
	printf("  >> the app starting ... ...\n");

	do
	{
		/* code */
		printf("   >>> do something ... ...\n");
	} while (0);

	printf("  >> the app exit.\n");

	return 0;
}

#if 0

#查看系统默认工具链
xadmin@hw:~/ xwks.git.1 / xapp - c11$ type gcc
gcc 已被录入哈希表(/ usr / bin / gcc)
xadmin@hw : ~/ xwks.git.1 / xapp - c11$ ll / usr / bin / gcc
lrwxrwxrwx 1 root root 5 3月  20  2020 / usr / bin / gcc->gcc - 9 *
xadmin@hw : ~/ xwks.git.1 / xapp - c11$ ll / usr / bin / gcc - 9
lrwxrwxrwx 1 root root 22 6月   1  2021 / usr / bin / gcc - 9->x86_64 - linux - gnu - gcc - 9 *
xadmin@hw : ~/ xwks.git.1 / xapp - c11$ ll / usr / bin / x86_64 - linux - gnu - gcc - 9
- rwxr - xr - x 1 root root 1158288 6月   1  2021 / usr / bin / x86_64 - linux - gnu - gcc - 9 *

xadmin@hw : ~/ xwks.git.1 / xapp - c11$ type objdump
objdump 已被录入哈希表(/ usr / bin / objdump)
xadmin@hw : ~/ xwks.git.1 / xapp - c11$ ll / usr / bin / objdump
lrwxrwxrwx 1 root root 24 10月 20 19 : 09 / usr / bin / objdump->x86_64 - linux - gnu - objdump *
xadmin@hw:~/ xwks.git.1 / xapp - c11$ ll / usr / bin / x86_64 - linux - gnu
x86_64 - linux - gnu - addr2line         x86_64 - linux - gnu - gcc - ranlib        x86_64 - linux - gnu - ld.gold           x86_64 - linux - gnux32 - gcc - ar
x86_64 - linux - gnu - ar                x86_64 - linux - gnu - gcc - ranlib - 10     x86_64 - linux - gnu - lto - dump - 10       x86_64 - linux - gnux32 - gcc - ar - 9
x86_64 - linux - gnu - as                x86_64 - linux - gnu - gcc - ranlib - 8      x86_64 - linux - gnu - nm                x86_64 - linux - gnux32 - gcc - nm
x86_64 - linux - gnu - c++filt           x86_64 - linux - gnu - gcc - ranlib - 9      x86_64 - linux - gnu - objcopy           x86_64 - linux - gnux32 - gcc - nm - 9
x86_64 - linux - gnu - cpp               x86_64 - linux - gnu - gcov              x86_64 - linux - gnu - objdump           x86_64 - linux - gnux32 - gcc - ranlib
x86_64 - linux - gnu - cpp - 10            x86_64 - linux - gnu - gcov - 10           x86_64 - linux - gnu - pkg - config        x86_64 - linux - gnux32 - gcc - ranlib - 9
x86_64 - linux - gnu - cpp - 8             x86_64 - linux - gnu - gcov - 8            x86_64 - linux - gnu - python3.8 - config  x86_64 - linux - gnux32 - gcov
x86_64 - linux - gnu - cpp - 9             x86_64 - linux - gnu - gcov - 9            x86_64 - linux - gnu - python3 - config    x86_64 - linux - gnux32 - gcov - 9
x86_64 - linux - gnu - dwp               x86_64 - linux - gnu - gcov - dump         x86_64 - linux - gnu - ranlib            x86_64 - linux - gnux32 - gcov - dump
x86_64 - linux - gnu - elfedit           x86_64 - linux - gnu - gcov - dump - 10      x86_64 - linux - gnu - readelf           x86_64 - linux - gnux32 - gcov - dump - 9
x86_64 - linux - gnu - g++               x86_64 - linux - gnu - gcov - dump - 8       x86_64 - linux - gnu - size              x86_64 - linux - gnux32 - gcov - tool
x86_64 - linux - gnu - g++ - 10            x86_64 - linux - gnu - gcov - dump - 9       x86_64 - linux - gnu - strings           x86_64 - linux - gnux32 - gcov - tool - 9
x86_64 - linux - gnu - g++ - 9             x86_64 - linux - gnu - gcov - tool         x86_64 - linux - gnu - strip             x86_64 - linux - gnux32 - gprof
x86_64 - linux - gnu - gcc               x86_64 - linux - gnu - gcov - tool - 10      x86_64 - linux - gnux32 - addr2line      x86_64 - linux - gnux32 - ld
x86_64 - linux - gnu - gcc - 10            x86_64 - linux - gnu - gcov - tool - 8       x86_64 - linux - gnux32 - ar             x86_64 - linux - gnux32 - ld.bfd
x86_64 - linux - gnu - gcc - 8             x86_64 - linux - gnu - gcov - tool - 9       x86_64 - linux - gnux32 - as             x86_64 - linux - gnux32 - ld.gold
x86_64 - linux - gnu - gcc - 9             x86_64 - linux - gnu - gdc               x86_64 - linux - gnux32 - c++filt        x86_64 - linux - gnux32 - nm
x86_64 - linux - gnu - gcc - ar            x86_64 - linux - gnu - gdc - 10            x86_64 - linux - gnux32 - cpp            x86_64 - linux - gnux32 - objcopy
x86_64 - linux - gnu - gcc - ar - 10         x86_64 - linux - gnu - gfortran          x86_64 - linux - gnux32 - cpp - 9          x86_64 - linux - gnux32 - objdump
x86_64 - linux - gnu - gcc - ar - 8          x86_64 - linux - gnu - gfortran - 8        x86_64 - linux - gnux32 - dwp            x86_64 - linux - gnux32 - ranlib
x86_64 - linux - gnu - gcc - ar - 9          x86_64 - linux - gnu - gfortran - 9        x86_64 - linux - gnux32 - elfedit        x86_64 - linux - gnux32 - readelf
x86_64 - linux - gnu - gcc - nm            x86_64 - linux - gnu - gold              x86_64 - linux - gnux32 - g++            x86_64 - linux - gnux32 - size
x86_64 - linux - gnu - gcc - nm - 10         x86_64 - linux - gnu - gprof             x86_64 - linux - gnux32 - g++ - 9          x86_64 - linux - gnux32 - strings
x86_64 - linux - gnu - gcc - nm - 8          x86_64 - linux - gnu - ld                x86_64 - linux - gnux32 - gcc            x86_64 - linux - gnux32 - strip
x86_64 - linux - gnu - gcc - nm - 9          x86_64 - linux - gnu - ld.bfd            x86_64 - linux - gnux32 - gcc - 9
xadmin@hw:~/ xwks.git.1 / xapp - c11$

#生成可执行文件
xadmin@hw:~/ xwks.git.1 / xapp - c11$ gcc - v - std = c11 - g - Wall - O0 xapp - c11 - 0.1.00.c - o myapp
Using built - in specs.
COLLECT_GCC = gcc
COLLECT_LTO_WRAPPER = / usr / lib / gcc / x86_64 - linux - gnu / 9 / lto - wrapper
OFFLOAD_TARGET_NAMES = nvptx - none : hsa
OFFLOAD_TARGET_DEFAULT = 1
Target : x86_64 - linux - gnu
Configured with : .. / src / configure - v --with - pkgversion = 'Ubuntu 9.4.0-1ubuntu1~20.04' --with - bugurl = file :///usr/share/doc/gcc-9/README.Bugs --enable-languages=c,ada,c++,go,brig,d,fortran,objc,obj-c++,gm2 --prefix=/usr --with-gcc-major-version-only --program-suffix=-9 --program-prefix=x86_64-linux-gnu- --enable-shared --enable-linker-build-id --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --libdir=/usr/lib --enable-nls --enable-clocale=gnu --enable-libstdcxx-debug --enable-libstdcxx-time=yes --with-default-libstdcxx-abi=new --enable-gnu-unique-object --disable-vtable-verify --enable-plugin --enable-default-pie --with-system-zlib --with-target-system-zlib=auto --enable-objc-gc=auto --enable-multiarch --disable-werror --with-arch-32=i686 --with-abi=m64 --with-multilib-list=m32,m64,mx32 --enable-multilib --with-tune=generic --enable-offload-targets=nvptx-none=/build/gcc-9-yTrUTS/gcc-9-9.4.0/debian/tmp-nvptx/usr,hsa --without-cuda-driver --enable-checking=release --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=x86_64-linux-gnu
	Thread model : posix
	gcc version 9.4.0 (Ubuntu 9.4.0 - 1ubuntu1~20.04)
	COLLECT_GCC_OPTIONS = '-v' '-std=c11' '-g' '-Wall' '-O0' '-o' 'myapp' '-mtune=generic' '-march=x86-64'
	/ usr / lib / gcc / x86_64 - linux - gnu / 9 / cc1 - quiet - v - imultiarch x86_64 - linux - gnu xapp - c11 - 0.1.00.c - quiet - dumpbase xapp - c11 - 0.1.00.c - mtune = generic - march = x86 - 64 - auxbase xapp - c11 - 0.1.00 - g - O0 - Wall - std = c11 - version - fasynchronous - unwind - tables - fstack - protector - strong - Wformat - security - fstack - clash - protection - fcf - protection - o / tmp / ccjZ3pko.s
	GNU C11(Ubuntu 9.4.0 - 1ubuntu1~20.04) version 9.4.0 (x86_64 - linux - gnu)
	compiled by GNU C version 9.4.0, GMP version 6.2.0, MPFR version 4.0.2, MPC version 1.1.0, isl version isl - 0.22.1 - GMP

	GGC heuristics : --param ggc - min - expand = 100 --param ggc - min - heapsize = 131072
	ignoring nonexistent directory "/usr/local/include/x86_64-linux-gnu"
	ignoring nonexistent directory "/usr/lib/gcc/x86_64-linux-gnu/9/include-fixed"
	ignoring nonexistent directory "/usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include"
#include "..." search starts here:
#include <...> search starts here:
	/ usr / lib / gcc / x86_64 - linux - gnu / 9 / include
	/ usr / local / include
	/ usr / include / x86_64 - linux - gnu
	/ usr / include
	End of search list.
	GNU C11(Ubuntu 9.4.0 - 1ubuntu1~20.04) version 9.4.0 (x86_64 - linux - gnu)
	compiled by GNU C version 9.4.0, GMP version 6.2.0, MPFR version 4.0.2, MPC version 1.1.0, isl version isl - 0.22.1 - GMP

	GGC heuristics : --param ggc - min - expand = 100 --param ggc - min - heapsize = 131072
	Compiler executable checksum : 0fa2a01b1ef16377fce6a0bcb60fb82d
	COLLECT_GCC_OPTIONS = '-v' '-std=c11' '-g' '-Wall' '-O0' '-o' 'myapp' '-mtune=generic' '-march=x86-64'
	as - v--64 - o / tmp / ccWW6o0q.o / tmp / ccjZ3pko.s
	GNU汇编版本 2.34 (x86_64 - linux - gnu) 使用BFD版本(GNU Binutils for Ubuntu) 2.34
	COMPILER_PATH = / usr / lib / gcc / x86_64 - linux - gnu / 9 / : / usr / lib / gcc / x86_64 - linux - gnu / 9 / : / usr / lib / gcc / x86_64 - linux - gnu / : / usr / lib / gcc / x86_64 - linux - gnu / 9 / : / usr / lib / gcc / x86_64 - linux - gnu /
	LIBRARY_PATH = / usr / lib / gcc / x86_64 - linux - gnu / 9 / : / usr / lib / gcc / x86_64 - linux - gnu / 9 / .. / .. / .. / x86_64 - linux - gnu / : / usr / lib / gcc / x86_64 - linux - gnu / 9 / .. / .. / .. / .. / lib / : / lib / x86_64 - linux - gnu / : / lib / .. / lib / : / usr / lib / x86_64 - linux - gnu / : / usr / lib / .. / lib / : / usr / lib / gcc / x86_64 - linux - gnu / 9 / .. / .. / .. / : / lib / : / usr / lib /
	COLLECT_GCC_OPTIONS = '-v' '-std=c11' '-g' '-Wall' '-O0' '-o' 'myapp' '-mtune=generic' '-march=x86-64'
	/ usr / lib / gcc / x86_64 - linux - gnu / 9 / collect2 - plugin / usr / lib / gcc / x86_64 - linux - gnu / 9 / liblto_plugin.so - plugin - opt = / usr / lib / gcc / x86_64 - linux - gnu / 9 / lto - wrapper - plugin - opt = -fresolution = / tmp / ccj27fHp.res - plugin - opt = -pass - through = -lgcc - plugin - opt = -pass - through = -lgcc_s - plugin - opt = -pass - through = -lc - plugin - opt = -pass - through = -lgcc - plugin - opt = -pass - through = -lgcc_s --build - id --eh - frame - hdr - m elf_x86_64 --hash - style = gnu --as - needed - dynamic - linker / lib64 / ld - linux - x86 - 64.so.2 - pie - z now - z relro - o myapp / usr / lib / gcc / x86_64 - linux - gnu / 9 / .. / .. / .. / x86_64 - linux - gnu / Scrt1.o / usr / lib / gcc / x86_64 - linux - gnu / 9 / .. / .. / .. / x86_64 - linux - gnu / crti.o / usr / lib / gcc / x86_64 - linux - gnu / 9 / crtbeginS.o - L / usr / lib / gcc / x86_64 - linux - gnu / 9 - L / usr / lib / gcc / x86_64 - linux - gnu / 9 / .. / .. / .. / x86_64 - linux - gnu - L / usr / lib / gcc / x86_64 - linux - gnu / 9 / .. / .. / .. / .. / lib - L / lib / x86_64 - linux - gnu - L / lib / .. / lib - L / usr / lib / x86_64 - linux - gnu - L / usr / lib / .. / lib - L / usr / lib / gcc / x86_64 - linux - gnu / 9 / .. / .. / .. / tmp / ccWW6o0q.o - lgcc --push - state --as - needed - lgcc_s --pop - state - lc - lgcc --push - state --as - needed - lgcc_s --pop - state / usr / lib / gcc / x86_64 - linux - gnu / 9 / crtendS.o / usr / lib / gcc / x86_64 - linux - gnu / 9 / .. / .. / .. / x86_64 - linux - gnu / crtn.o
	COLLECT_GCC_OPTIONS = '-v' '-std=c11' '-g' '-Wall' '-O0' '-o' 'myapp' '-mtune=generic' '-march=x86-64'
	xadmin@hw : ~/ xwks.git.1 / xapp - c11$

	#执行程序
	xadmin@hw:~/ xwks.git.1 / xapp - c11$ . / myapp
	>> the app starting ... ...
	>> > do something ... ...
	>> the app exit.
	xadmin@hw:~/ xwks.git.1 / xapp - c11$

	#确认文件类型
	xadmin@hw:~/ xwks.git.1 / xapp - c11$ file xapp - c11 - 0.1.00.c myapp
	xapp - c11 - 0.1.00.c : C source, ASCII text
	myapp : ELF 64 - bit LSB shared object, x86 - 64, version 1 (SYSV), dynamically linked, interpreter / lib64 / ld - linux - x86 - 64.so.2, BuildID[sha1] = 2dcac66781e880d102663e64f697e54a1b5ad164, for GNU / Linux 3.2.0, with debug_info, not stripped
	xadmin@hw : ~/ xwks.git.1 / xapp - c11$

	#查看可执行文件依赖的动态链接库
	xadmin@hw:~/ xwks.git.1 / xapp - c11$ ldd myapp
	linux - vdso.so.1 (0x00007ffd47726000)
	libc.so.6 = > / lib / x86_64 - linux - gnu / libc.so.6 (0x00007fa05b48c000)
	/ lib64 / ld - linux - x86 - 64.so.2 (0x00007fa05b6a2000)

#== == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == =
	xadmin@hw:~/ xwks.git.1 / xapp - c11$ ll / lib64 / ld - linux - x86 - 64.so.2
	lrwxrwxrwx 1 root root 32 2月  25 03 : 42 / lib64 / ld - linux - x86 - 64.so.2 -> / lib / x86_64 - linux - gnu / ld - 2.31.so *
	xadmin@hw : ~/ xwks.git.1 / xapp - c11$ ll / lib / x86_64 - linux - gnu / ld - 2.31.so
	- rwxr - xr - x 1 root root 191472 2月  25 03 : 42 / lib / x86_64 - linux - gnu / ld - 2.31.so *
	xadmin@hw : ~/ xwks.git.1 / xapp - c11$ file / lib / x86_64 - linux - gnu / ld - 2.31.so
	/ lib / x86_64 - linux - gnu / ld - 2.31.so : ELF 64 - bit LSB shared object, x86 - 64, version 1 (SYSV), dynamically linked, BuildID[sha1] = 39bd4a6852f6b9962ad57117d11b6f130d32f7fd, stripped

#== == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == =
	xadmin@hw : ~/ xwks.git.1 / xapp - c11$ ll / lib / x86_64 - linux - gnu / libc.so.6
	lrwxrwxrwx 1 root root 12 2月  25 03 : 42 / lib / x86_64 - linux - gnu / libc.so.6->libc - 2.31.so *
	xadmin@hw : ~/ xwks.git.1 / xapp - c11$ ll / lib / x86_64 - linux - gnu / libc - 2.31.so
	- rwxr - xr - x 1 root root 2029560 2月  25 03 : 42 / lib / x86_64 - linux - gnu / libc - 2.31.so *
	xadmin@hw : ~/ xwks.git.1 / xapp - c11$ file / lib / x86_64 - linux - gnu / libc - 2.31.so
	/ lib / x86_64 - linux - gnu / libc - 2.31.so : ELF 64 - bit LSB shared object, x86 - 64, version 1 (GNU / Linux), dynamically linked, interpreter / lib64 / ld - linux - x86 - 64.so.2, BuildID[sha1] = 9fdb74e7b217d06c93172a8243f8547f947ee6d1, for GNU / Linux 3.2.0, stripped
	xadmin@hw : ~/ xwks.git.1 / xapp - c11$ ldd / lib / x86_64 - linux - gnu / libc - 2.31.so *
	/ lib64 / ld - linux - x86 - 64.so.2 (0x00007f6e49c0e000)
	linux - vdso.so.1 (0x00007fffc21a0000)
	xadmin@hw : ~/ xwks.git.1 / xapp - c11$ ldd / lib64 / ld - linux - x86 - 64.so.2
	statically linked
#== == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == =

	#分析可执行文件ELF头信息
	xadmin@hw:~/ xwks.git.1 / xapp - c11$ readelf - h myapp
	ELF 头：
	Magic：   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00
	类别 : ELF64
	数据 : 2 补码，小端序(little endian)
	Version : 1 (current)
	OS / ABI : UNIX - System V
	ABI 版本 : 0
	类型 : DYN(共享目标文件)
	系统架构 : Advanced Micro Devices X86 - 64
	版本 : 0x1
	入口点地址：               0x1060
	程序头起点：          64 (bytes into file)
	Start of section headers : 16880 (bytes into file)
	标志：             0x0
	Size of this header : 64 (bytes)
	Size of program headers : 56 (bytes)
	Number of program headers : 13
	Size of section headers : 64 (bytes)
	Number of section headers : 36
	Section header string table index : 35
	xadmin@hw : ~/ xwks.git.1 / xapp - c11$

	#查看可执行文件的反汇编
	xadmin@hw:~/ xwks.git.1 / xapp - c11$ objdump - d - Mintel myapp

	myapp：     文件格式 elf64 - x86 - 64

	Disassembly of section.init :

	0000000000001000 < _init > :
	1000 : f3 0f 1e fa             endbr64
	1004 : 48 83 ec 08             sub    rsp, 0x8
	1008:       48 8b 05 d9 2f 00 00    mov    rax, QWORD PTR[rip + 0x2fd9]        # 3fe8 < __gmon_start__>
	100f:       48 85 c0                test   rax, rax
	1012 : 74 02                   je     1016 < _init + 0x16 >
	1014 : ff d0                   call   rax
	1016 : 48 83 c4 08             add    rsp, 0x8
	101a:       c3                      ret

	Disassembly of section.plt :

	0000000000001020 < .plt > :
	1020 : ff 35 9a 2f 00 00       push   QWORD PTR[rip + 0x2f9a]        # 3fc0 < _GLOBAL_OFFSET_TABLE_ + 0x8>
	1026:       f2 ff 25 9b 2f 00 00    bnd jmp QWORD PTR[rip + 0x2f9b]        # 3fc8 < _GLOBAL_OFFSET_TABLE_ + 0x10>
	102d:       0f 1f 00                nop    DWORD PTR[rax]
	1030 : f3 0f 1e fa             endbr64
	1034 : 68 00 00 00 00          push   0x0
	1039 : f2 e9 e1 ff ff ff       bnd jmp 1020 < .plt >
	103f : 90                      nop

	Disassembly of section.plt.got :

	0000000000001040 < __cxa_finalize@plt > :
	1040 : f3 0f 1e fa             endbr64
	1044 : f2 ff 25 ad 2f 00 00    bnd jmp QWORD PTR[rip + 0x2fad]        # 3ff8 < __cxa_finalize@GLIBC_2.2.5>
	104b:       0f 1f 44 00 00          nop    DWORD PTR[rax + rax * 1 + 0x0]

	Disassembly of section.plt.sec :

	0000000000001050 < puts@plt > :
	1050 : f3 0f 1e fa             endbr64
	1054 : f2 ff 25 75 2f 00 00    bnd jmp QWORD PTR[rip + 0x2f75]        # 3fd0 < puts@GLIBC_2.2.5>
	105b:       0f 1f 44 00 00          nop    DWORD PTR[rax + rax * 1 + 0x0]

	Disassembly of section.text :

	0000000000001060 < _start > :
	1060 : f3 0f 1e fa             endbr64
	1064 : 31 ed xor ebp, ebp
	1066 : 49 89 d1                mov    r9, rdx
	1069 : 5e                      pop    rsi
	106a : 48 89 e2                mov    rdx, rsp
	106d : 48 83 e4 f0 and rsp, 0xfffffffffffffff0
	1071 : 50                      push   rax
	1072 : 54                      push   rsp
	1073 : 4c 8d 05 86 01 00 00    lea    r8, [rip + 0x186]        # 1200 < __libc_csu_fini >
	107a:       48 8d 0d 0f 01 00 00    lea    rcx, [rip + 0x10f]        # 1190 < __libc_csu_init >
	1081:       48 8d 3d c1 00 00 00    lea    rdi, [rip + 0xc1]        # 1149 < main >
	1088:       ff 15 52 2f 00 00       call   QWORD PTR[rip + 0x2f52]        # 3fe0 < __libc_start_main@GLIBC_2.2.5>
	108e:       f4                      hlt
	108f : 90                      nop

	0000000000001090 <deregister_tm_clones> :
	1090 : 48 8d 3d 79 2f 00 00    lea    rdi, [rip + 0x2f79]        # 4010 < __TMC_END__ >
	1097:       48 8d 05 72 2f 00 00    lea    rax, [rip + 0x2f72]        # 4010 < __TMC_END__ >
	109e:       48 39 f8                cmp    rax, rdi
	10a1 : 74 15                   je     10b8 < deregister_tm_clones + 0x28>
	10a3 : 48 8b 05 2e 2f 00 00    mov    rax, QWORD PTR[rip + 0x2f2e]        # 3fd8 < _ITM_deregisterTMCloneTable>
	10aa:       48 85 c0                test   rax, rax
	10ad : 74 09                   je     10b8 < deregister_tm_clones + 0x28>
	10af:       ff e0                   jmp    rax
	10b1 : 0f 1f 80 00 00 00 00    nop    DWORD PTR[rax + 0x0]
	10b8 : c3                      ret
	10b9 : 0f 1f 80 00 00 00 00    nop    DWORD PTR[rax + 0x0]

	00000000000010c0 <register_tm_clones> :
	10c0 : 48 8d 3d 49 2f 00 00    lea    rdi, [rip + 0x2f49]        # 4010 < __TMC_END__ >
	10c7:       48 8d 35 42 2f 00 00    lea    rsi, [rip + 0x2f42]        # 4010 < __TMC_END__ >
	10ce:       48 29 fe                sub    rsi, rdi
	10d1 : 48 89 f0                mov    rax, rsi
	10d4 : 48 c1 ee 3f             shr    rsi, 0x3f
	10d8 : 48 c1 f8 03             sar    rax, 0x3
	10dc : 48 01 c6                add    rsi, rax
	10df : 48 d1 fe                sar    rsi, 1
	10e2 : 74 14                   je     10f8 < register_tm_clones + 0x38>
	10e4 : 48 8b 05 05 2f 00 00    mov    rax, QWORD PTR[rip + 0x2f05]        # 3ff0 < _ITM_registerTMCloneTable>
	10eb:       48 85 c0                test   rax, rax
	10ee : 74 08                   je     10f8 < register_tm_clones + 0x38>
	10f0:       ff e0                   jmp    rax
	10f2 : 66 0f 1f 44 00 00       nop    WORD PTR[rax + rax * 1 + 0x0]
	10f8 : c3                      ret
	10f9 : 0f 1f 80 00 00 00 00    nop    DWORD PTR[rax + 0x0]

	0000000000001100 < __do_global_dtors_aux > :
	1100 : f3 0f 1e fa             endbr64
	1104 : 80 3d 05 2f 00 00 00    cmp    BYTE PTR[rip + 0x2f05], 0x0        # 4010 < __TMC_END__ >
	110b:       75 2b                   jne    1138 < __do_global_dtors_aux + 0x38 >
	110d : 55                      push   rbp
	110e : 48 83 3d e2 2e 00 00    cmp    QWORD PTR[rip + 0x2ee2], 0x0        # 3ff8 < __cxa_finalize@GLIBC_2.2.5>
	1115:       00
	1116 : 48 89 e5                mov    rbp, rsp
	1119 : 74 0c                   je     1127 < __do_global_dtors_aux + 0x27 >
	111b : 48 8b 3d e6 2e 00 00    mov    rdi, QWORD PTR[rip + 0x2ee6]        # 4008 < __dso_handle >
	1122:       e8 19 ff ff ff          call   1040 < __cxa_finalize@plt >
	1127 : e8 64 ff ff ff          call   1090 < deregister_tm_clones >
	112c : c6 05 dd 2e 00 00 01    mov    BYTE PTR[rip + 0x2edd], 0x1        # 4010 < __TMC_END__ >
	1133:       5d                      pop    rbp
	1134 : c3                      ret
	1135 : 0f 1f 00                nop    DWORD PTR[rax]
	1138 : c3                      ret
	1139 : 0f 1f 80 00 00 00 00    nop    DWORD PTR[rax + 0x0]

	0000000000001140 < frame_dummy > :
	1140 : f3 0f 1e fa             endbr64
	1144 : e9 77 ff ff ff          jmp    10c0 <register_tm_clones>

	0000000000001149 <main> :
	1149 : f3 0f 1e fa             endbr64
	114d : 55                      push   rbp
	114e : 48 89 e5                mov    rbp, rsp
	1151 : 48 83 ec 10             sub    rsp, 0x10
	1155 : 89 7d fc                mov    DWORD PTR[rbp - 0x4], edi
	1158 : 48 89 75 f0             mov    QWORD PTR[rbp - 0x10], rsi
	115c : 48 8d 3d a1 0e 00 00    lea    rdi, [rip + 0xea1]        # 2004 < _IO_stdin_used + 0x4 >
	1163:       e8 e8 fe ff ff          call   1050 < puts@plt >
	1168 : 48 8d 3d b3 0e 00 00    lea    rdi, [rip + 0xeb3]        # 2022 < _IO_stdin_used + 0x22 >
	116f:       e8 dc fe ff ff          call   1050 < puts@plt >
	1174 : 48 8d 3d c3 0e 00 00    lea    rdi, [rip + 0xec3]        # 203e < _IO_stdin_used + 0x3e>
	117b:       e8 d0 fe ff ff          call   1050 < puts@plt >
	1180 : b8 00 00 00 00          mov    eax, 0x0
	1185 : c9                      leave
	1186 : c3                      ret
	1187 : 66 0f 1f 84 00 00 00    nop    WORD PTR[rax + rax * 1 + 0x0]
	118e : 00 00

	0000000000001190 <__libc_csu_init> :
	1190 : f3 0f 1e fa             endbr64
	1194 : 41 57                   push   r15
	1196 : 4c 8d 3d 1b 2c 00 00    lea    r15, [rip + 0x2c1b]        # 3db8 < __frame_dummy_init_array_entry>
	119d:       41 56                   push   r14
	119f : 49 89 d6                mov    r14, rdx
	11a2 : 41 55                   push   r13
	11a4 : 49 89 f5                mov    r13, rsi
	11a7 : 41 54                   push   r12
	11a9 : 41 89 fc                mov    r12d, edi
	11ac : 55                      push   rbp
	11ad : 48 8d 2d 0c 2c 00 00    lea    rbp, [rip + 0x2c0c]        # 3dc0 < __do_global_dtors_aux_fini_array_entry>
	11b4:       53                      push   rbx
	11b5 : 4c 29 fd                sub    rbp, r15
	11b8 : 48 83 ec 08             sub    rsp, 0x8
	11bc:       e8 3f fe ff ff          call   1000 < _init >
	11c1 : 48 c1 fd 03             sar    rbp, 0x3
	11c5 : 74 1f                   je     11e6 < __libc_csu_init + 0x56 >
	11c7 : 31 db xor ebx, ebx
	11c9 : 0f 1f 80 00 00 00 00    nop    DWORD PTR[rax + 0x0]
	11d0 : 4c 89 f2                mov    rdx, r14
	11d3 : 4c 89 ee                mov    rsi, r13
	11d6 : 44 89 e7                mov    edi, r12d
	11d9 : 41 ff 14 df             call   QWORD PTR[r15 + rbx * 8]
	11dd : 48 83 c3 01             add    rbx, 0x1
	11e1 : 48 39 dd                cmp    rbp, rbx
	11e4 : 75 ea                   jne    11d0 < __libc_csu_init + 0x40>
	11e6 : 48 83 c4 08             add    rsp, 0x8
	11ea:       5b                      pop    rbx
	11eb : 5d                      pop    rbp
	11ec : 41 5c                   pop    r12
	11ee : 41 5d                   pop    r13
	11f0 : 41 5e                   pop    r14
	11f2 : 41 5f                   pop    r15
	11f4 : c3                      ret
	11f5 : 66 66 2e 0f 1f 84 00    data16 nop WORD PTR cs : [rax + rax * 1 + 0x0]
	11fc : 00 00 00 00

	0000000000001200 < __libc_csu_fini > :
	1200 : f3 0f 1e fa             endbr64
	1204 : c3                      ret

	Disassembly of section.fini :

	0000000000001208 <_fini> :
	1208 : f3 0f 1e fa             endbr64
	120c : 48 83 ec 08             sub    rsp, 0x8
	1210:       48 83 c4 08             add    rsp, 0x8
	1214:       c3                      ret
	xadmin@hw : ~/ xwks.git.1 / xapp - c11$
	//xadmin@hw:~/xwks.git.1/xapp-c11$ hexdump -C myapp
	//00000000  7f 45 4c 46 02 01 01 00  00 00 00 00 00 00 00 00  |.ELF............|
	//00000010  03 00 3e 00 01 00 00 00  60 10 00 00 00 00 00 00  |..>.....`.......|
	//00000020  40 00 00 00 00 00 00 00  f0 41 00 00 00 00 00 00  |@........A......|
	//00000030  00 00 00 00 40 00 38 00  0d 00 40 00 24 00 23 00  |....@.8...@.$.#.|
	//00000040  06 00 00 00 04 00 00 00  40 00 00 00 00 00 00 00  |........@.......|
	//00000050  40 00 00 00 00 00 00 00  40 00 00 00 00 00 00 00  |@.......@.......|
	//00000060  d8 02 00 00 00 00 00 00  d8 02 00 00 00 00 00 00  |................|
	//00000070  08 00 00 00 00 00 00 00  03 00 00 00 04 00 00 00  |................|
	//00000080  18 03 00 00 00 00 00 00  18 03 00 00 00 00 00 00  |................|
	//00000090  18 03 00 00 00 00 00 00  1c 00 00 00 00 00 00 00  |................|
	//000000a0  1c 00 00 00 00 00 00 00  01 00 00 00 00 00 00 00  |................|
	//000000b0  01 00 00 00 04 00 00 00  00 00 00 00 00 00 00 00  |................|
	//000000c0  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
	//000000d0  f8 05 00 00 00 00 00 00  f8 05 00 00 00 00 00 00  |................|
	//000000e0  00 10 00 00 00 00 00 00  01 00 00 00 05 00 00 00  |................|
	//000000f0  00 10 00 00 00 00 00 00  00 10 00 00 00 00 00 00  |................|
	//00000100  00 10 00 00 00 00 00 00  15 02 00 00 00 00 00 00  |................|
	//00000110  15 02 00 00 00 00 00 00  00 10 00 00 00 00 00 00  |................|
	//00000120  01 00 00 00 04 00 00 00  00 20 00 00 00 00 00 00  |......... ......|
	//00000130  00 20 00 00 00 00 00 00  00 20 00 00 00 00 00 00  |. ....... ......|
	//00000140  a0 01 00 00 00 00 00 00  a0 01 00 00 00 00 00 00  |................|
	//00000150  00 10 00 00 00 00 00 00  01 00 00 00 06 00 00 00  |................|
	//00000160  b8 2d 00 00 00 00 00 00  b8 3d 00 00 00 00 00 00  |.-.......=......|
	//00000170  b8 3d 00 00 00 00 00 00  58 02 00 00 00 00 00 00  |.=......X.......|
	//00000180  60 02 00 00 00 00 00 00  00 10 00 00 00 00 00 00  |`...............|
	//00000190  02 00 00 00 06 00 00 00  c8 2d 00 00 00 00 00 00  |.........-......|
	//000001a0  c8 3d 00 00 00 00 00 00  c8 3d 00 00 00 00 00 00  |.=.......=......|
	//000001b0  f0 01 00 00 00 00 00 00  f0 01 00 00 00 00 00 00  |................|
	//000001c0  08 00 00 00 00 00 00 00  04 00 00 00 04 00 00 00  |................|
	//000001d0  38 03 00 00 00 00 00 00  38 03 00 00 00 00 00 00  |8.......8.......|
	//000001e0  38 03 00 00 00 00 00 00  20 00 00 00 00 00 00 00  |8....... .......|
	//000001f0  20 00 00 00 00 00 00 00  08 00 00 00 00 00 00 00  | ...............|
	//00000200  04 00 00 00 04 00 00 00  58 03 00 00 00 00 00 00  |........X.......|
	//00000210  58 03 00 00 00 00 00 00  58 03 00 00 00 00 00 00  |X.......X.......|
	//00000220  44 00 00 00 00 00 00 00  44 00 00 00 00 00 00 00  |D.......D.......|
	//00000230  04 00 00 00 00 00 00 00  53 e5 74 64 04 00 00 00  |........S.td....|
	//00000240  38 03 00 00 00 00 00 00  38 03 00 00 00 00 00 00  |8.......8.......|
	//00000250  38 03 00 00 00 00 00 00  20 00 00 00 00 00 00 00  |8....... .......|
	//00000260  20 00 00 00 00 00 00 00  08 00 00 00 00 00 00 00  | ...............|
	//00000270  50 e5 74 64 04 00 00 00  54 20 00 00 00 00 00 00  |P.td....T ......|
	//00000280  54 20 00 00 00 00 00 00  54 20 00 00 00 00 00 00  |T ......T ......|
	//00000290  44 00 00 00 00 00 00 00  44 00 00 00 00 00 00 00  |D.......D.......|
	//000002a0  04 00 00 00 00 00 00 00  51 e5 74 64 06 00 00 00  |........Q.td....|
	//000002b0  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
	//... ...
	//... ...
	//000002d0  00 00 00 00 00 00 00 00  10 00 00 00 00 00 00 00  |................|
	//000002e0  52 e5 74 64 04 00 00 00  b8 2d 00 00 00 00 00 00  |R.td.....-......|
	//000002f0  b8 3d 00 00 00 00 00 00  b8 3d 00 00 00 00 00 00  |.=.......=......|
	//00000300  48 02 00 00 00 00 00 00  48 02 00 00 00 00 00 00  |H.......H.......|
	//00000310  01 00 00 00 00 00 00 00  2f 6c 69 62 36 34 2f 6c  |......../lib64/l|
	//00000320  64 2d 6c 69 6e 75 78 2d  78 38 36 2d 36 34 2e 73  |d-linux-x86-64.s|
	//00000330  6f 2e 32 00 00 00 00 00  04 00 00 00 10 00 00 00  |o.2.............|
	//00000340  05 00 00 00 47 4e 55 00  02 00 00 c0 04 00 00 00  |....GNU.........|
	//00000350  03 00 00 00 00 00 00 00  04 00 00 00 14 00 00 00  |................|
	//00000360  03 00 00 00 47 4e 55 00  93 c0 67 9a 20 fb 4b 8c  |....GNU...g. .K.|
	//00000370  b7 74 4d f2 d3 6f ac 9c  bf ed 10 81 04 00 00 00  |.tM..o..........|
	//00000380  10 00 00 00 01 00 00 00  47 4e 55 00 00 00 00 00  |........GNU.....|
	//00000390  03 00 00 00 02 00 00 00  00 00 00 00 00 00 00 00  |................|
	//000003a0  02 00 00 00 06 00 00 00  01 00 00 00 06 00 00 00  |................|
	//000003b0  00 00 81 00 00 00 00 00  06 00 00 00 00 00 00 00  |................|
	//000003c0  d1 65 ce 6d 00 00 00 00  00 00 00 00 00 00 00 00  |.e.m............|
	//000003d0  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
	//000003e0  3d 00 00 00 20 00 00 00  00 00 00 00 00 00 00 00  |=... ...........|
	//000003f0  00 00 00 00 00 00 00 00  0b 00 00 00 12 00 00 00  |................|
	//00000400  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
	//00000410  1f 00 00 00 12 00 00 00  00 00 00 00 00 00 00 00  |................|
	//00000420  00 00 00 00 00 00 00 00  59 00 00 00 20 00 00 00  |........Y... ...|
	//00000430  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
	//00000440  68 00 00 00 20 00 00 00  00 00 00 00 00 00 00 00  |h... ...........|
	//00000450  00 00 00 00 00 00 00 00  10 00 00 00 22 00 00 00  |............"...|
	//00000460  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
	//00000470  00 6c 69 62 63 2e 73 6f  2e 36 00 70 75 74 73 00  |.libc.so.6.puts.|
	//00000480  5f 5f 63 78 61 5f 66 69  6e 61 6c 69 7a 65 00 5f  |__cxa_finalize._|
	//00000490  5f 6c 69 62 63 5f 73 74  61 72 74 5f 6d 61 69 6e  |_libc_start_main|
	//000004a0  00 47 4c 49 42 43 5f 32  2e 32 2e 35 00 5f 49 54  |.GLIBC_2.2.5._IT|
	//000004b0  4d 5f 64 65 72 65 67 69  73 74 65 72 54 4d 43 6c  |M_deregisterTMCl|
	//000004c0  6f 6e 65 54 61 62 6c 65  00 5f 5f 67 6d 6f 6e 5f  |oneTable.__gmon_|
	//000004d0  73 74 61 72 74 5f 5f 00  5f 49 54 4d 5f 72 65 67  |start__._ITM_reg|
	//000004e0  69 73 74 65 72 54 4d 43  6c 6f 6e 65 54 61 62 6c  |isterTMCloneTabl|
	//000004f0  65 00 00 00 00 00 02 00  02 00 00 00 00 00 02 00  |e...............|
	//00000500  01 00 01 00 01 00 00 00  10 00 00 00 00 00 00 00  |................|
	//00000510  75 1a 69 09 00 00 02 00  31 00 00 00 00 00 00 00  |u.i.....1.......|
	//00000520  b8 3d 00 00 00 00 00 00  08 00 00 00 00 00 00 00  |.=..............|
	//00000530  40 11 00 00 00 00 00 00  c0 3d 00 00 00 00 00 00  |@........=......|
	//00000540  08 00 00 00 00 00 00 00  00 11 00 00 00 00 00 00  |................|
	//00000550  08 40 00 00 00 00 00 00  08 00 00 00 00 00 00 00  |.@..............|
	//00000560  08 40 00 00 00 00 00 00  d8 3f 00 00 00 00 00 00  |.@.......?......|
	//*
	//00004a50  90 06 00 00 00 00 00 00  22 00 00 00 33 00 00 00  |........"...3...|
	//00004a60  08 00 00 00 00 00 00 00  18 00 00 00 00 00 00 00  |................|
	//00004a70  09 00 00 00 03 00 00 00  00 00 00 00 00 00 00 00  |................|
	//00004a80  00 00 00 00 00 00 00 00  88 3e 00 00 00 00 00 00  |.........>......|
	//00004a90  0d 02 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
	//00004aa0  01 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
	//00004ab0  11 00 00 00 03 00 00 00  00 00 00 00 00 00 00 00  |................|
	//00004ac0  00 00 00 00 00 00 00 00  95 40 00 00 00 00 00 00  |.........@......|
	//00004ad0  5a 01 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |Z...............|
	//00004ae0  01 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
	//00004af0
	xadmin@hw:~/ xwks.git.1 / xapp - c11$
	xadmin@hw : ~/ xwks.git.1 / xapp - c11$ strace . / myapp
	execve("./myapp", ["./myapp"], 0x7ffcf4947bc0 /* 35 vars */) = 0
	brk(NULL) = 0x559849a13000
	arch_prctl(0x3001 /* ARCH_??? */, 0x7ffc32ae7450) = -1 EINVAL(无效的参数)
	access("/etc/ld.so.preload", R_OK) = -1 ENOENT(没有那个文件或目录)
	openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY | O_CLOEXEC) = 3
	fstat(3, { st_mode = S_IFREG | 0644, st_size = 116436, ... }) = 0
	mmap(NULL, 116436, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7f2b2010f000
	close(3) = 0
	openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY | O_CLOEXEC) = 3
	read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\360A\2\0\0\0\0\0"..., 832) = 832
	pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
	pread64(3, "\4\0\0\0\20\0\0\0\5\0\0\0GNU\0\2\0\0\300\4\0\0\0\3\0\0\0\0\0\0\0", 32, 848) = 32
	pread64(3, "\4\0\0\0\24\0\0\0\3\0\0\0GNU\0\237\333t\347\262\27\320l\223\27*\202C\370T\177"..., 68, 880) = 68
	fstat(3, { st_mode = S_IFREG | 0755, st_size = 2029560, ... }) = 0
	mmap(NULL, 8192, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0) = 0x7f2b2010d000
	pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
	pread64(3, "\4\0\0\0\20\0\0\0\5\0\0\0GNU\0\2\0\0\300\4\0\0\0\3\0\0\0\0\0\0\0", 32, 848) = 32
	pread64(3, "\4\0\0\0\24\0\0\0\3\0\0\0GNU\0\237\333t\347\262\27\320l\223\27*\202C\370T\177"..., 68, 880) = 68
	mmap(NULL, 2037344, PROT_READ, MAP_PRIVATE | MAP_DENYWRITE, 3, 0) = 0x7f2b1ff1b000
	mmap(0x7f2b1ff3d000, 1540096, PROT_READ | PROT_EXEC, MAP_PRIVATE | MAP_FIXED | MAP_DENYWRITE, 3, 0x22000) = 0x7f2b1ff3d000
	mmap(0x7f2b200b5000, 319488, PROT_READ, MAP_PRIVATE | MAP_FIXED | MAP_DENYWRITE, 3, 0x19a000) = 0x7f2b200b5000
	mmap(0x7f2b20103000, 24576, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_FIXED | MAP_DENYWRITE, 3, 0x1e7000) = 0x7f2b20103000
	mmap(0x7f2b20109000, 13920, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_FIXED | MAP_ANONYMOUS, -1, 0) = 0x7f2b20109000
	close(3) = 0
	arch_prctl(ARCH_SET_FS, 0x7f2b2010e540) = 0
	mprotect(0x7f2b20103000, 16384, PROT_READ) = 0
	mprotect(0x55984946a000, 4096, PROT_READ) = 0
	mprotect(0x7f2b20159000, 4096, PROT_READ) = 0
	munmap(0x7f2b2010f000, 116436) = 0
	fstat(1, { st_mode = S_IFCHR | 0620, st_rdev = makedev(0x88, 0x3), ... }) = 0
	brk(NULL) = 0x559849a13000
	brk(0x559849a34000) = 0x559849a34000
	write(1, "  >> the app starting ... ...\n", 30 >> the app starting ... ...
	) = 30
	write(1, "   >>> do something ... ...\n", 28 >> > do something ... ...
	) = 28
	write(1, "  >> the app exit.\n", 19 >> the app exit.
	) = 19
	exit_group(0) = ?
	++ + exited with 0 ++ +
	xadmin@hw : ~/ xwks.git.1 / xapp - c11$

#endif

#if 0 // valgrind test
	xadmin@hw:~/ xwks.git.1 / xapp - c11$ gcc - std = c11 - g - Wall - O0 myapp - c11 - 0.1.00.c - o myapp - lpthread
	xadmin@hw : ~/ xwks.git.1 / xapp - c11$ valgrind --tool = memcheck --leak - check = full --track - origins = yes - s . / myapp
	== 253148 == Memcheck, a memory error detector
	== 253148 == Copyright(C) 2002 - 2017, and GNU GPL'd, by Julian Seward et al.
	== 253148 == Using Valgrind - 3.15.0 and LibVEX; rerun with - h for copyright info
	== 253148 == Command: . / myapp
	== 253148==
	>> the app starting ... ...
	>> > do something ... ...
	>> the app exit.
	== 253148==
	==253148 == HEAP SUMMARY :
== 253148 == in use at exit : 0 bytes in 0 blocks
== 253148 == total heap usage : 1 allocs, 1 frees, 1, 024 bytes allocated
== 253148==
==253148 == All heap blocks were freed -- no leaks are possible
== 253148==
==253148 == ERROR SUMMARY : 0 errors from 0 contexts(suppressed : 0 from 0)
xadmin@hw : ~/ xwks.git.1 / xapp - c11$

#endif
