#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

//https://www.runoob.com/cprogramming/c-tutorial.html
//https://en.cppreference.com/w/
//https://zh.cppreference.com/w/c/language/main_function

//02: 显示十六进制数据块；
//gcc -std=c11 -g -Wall -O0 myapp-c11-0.1.02.c -o myapp

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

void DumpHex(unsigned char* pData, unsigned int iLen)
{
	if (pData == NULL || iLen == 0) { return; }
	
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
	
	return;
}

unsigned char* getHexDataFromFile(const char* pFileName, unsigned int* piFileLen)
{
	unsigned char* pHexData = NULL;
	struct stat statbuf     = {0};
	
	stat(pFileName, &statbuf);
	
	unsigned int iLen = statbuf.st_size;
	if(iLen > 0 && iLen < 10*1024*1024) //文件目前最大设为10M
	{
		FILE* hFile = fopen(pFileName, "rb");
		if(hFile == NULL)
			return NULL;
		
		*piFileLen = iLen;
		pHexData = (unsigned char*)calloc(iLen, sizeof(unsigned char)+8);
		
		size_t size_readok = fread(pHexData, 1, iLen, hFile);
		fclose(hFile);
		
		if(size_readok != iLen)
		{
			free(pHexData);
			return NULL;
		}
		
		return pHexData;
	}

	return NULL;
}

int main(int argc, char *argv[])
{
	xlog_info("  >> the app starting ... ...\n\n");
	
	for(int i=0; i<argc; i++)
	{
		xlog_info("   >> argv[%02d] = {\"%s\"};\n", i, argv[i]);
	}
	
	xlog_info("\n");
#if 0
	char* pFileNameSelf   = argv[0];
	char* pFileNameTarget = argv[1];
	
	xlog_info("  >> func{\e[31m%s:(%05d)()\e[0m}. pFileNameSelf=%s, pFileNameTarget=%s.\n",
					__func__, __LINE__, pFileNameSelf, pFileNameTarget);

	if(argc != 2)
	{
		//err	
		xlog_info("  >> func{%s:(%05d)@(%s)(param tbc)} is error !!\n", __func__, __LINE__, __FILE__);
		return -1;
	}
#endif

	//取文件数据
	do
	{
		///////////////////////////////////////////
		//get data from target file
		char*           pFileName = argv[0];
		unsigned char*  pHexData  = NULL;
		unsigned int    iLen      = 0;
		pHexData = getHexDataFromFile(pFileName, &iLen);
		if(pHexData != NULL && iLen > 0)
		{
			xlog_info("  >> func{%s:(%05d)} is call, pHexData=\"%p\" .\n", __func__, __LINE__, pHexData);
			
			DumpHex((unsigned char*)argv, 16*10+9);
			
			DumpHex((unsigned char*)argv[0], 16*10+9);
			
			DumpHex(pHexData, 16*30+11);
			
			//"\056"  "\0 56" ?? 注意转义符；
			char szTest[] = "`   1234....\\0\0\0567890~!@#$%^&*()_+-=qwertyuiop[]\\asdfghjkl;'zxcvbnm,./QWERTYUIOP{}|ASDFGHJKL:\"ZXCVBNM<>?";
			DumpHex((unsigned char*)szTest, sizeof(szTest));
		}
		free(pHexData);
	}while(0);
	
	xlog_info("\n");
	xlog_info("  >> the app exit.\n");
	
	return 0;
}



#if 0

xadmin@hw:~/xwks.git.1/xapp-c11$ 
xadmin@hw:~/xwks.git.1/xapp-c11$ gcc -std=c11 -g -Wall -O0 myapp-c11-0.1.02.c -o myapp
xadmin@hw:~/xwks.git.1/xapp-c11$ ./myapp gcc -std=c11 -g -Wall -O0 myapp-c11-0.1.02.c -o myapp
  >> the app starting ... ...

   >> argv[00] = {"./myapp"};
   >> argv[01] = {"gcc"};
   >> argv[02] = {"-std=c11"};
   >> argv[03] = {"-g"};
   >> argv[04] = {"-Wall"};
   >> argv[05] = {"-O0"};
   >> argv[06] = {"myapp-c11-0.1.02.c"};
   >> argv[07] = {"-o"};
   >> argv[08] = {"myapp"};

  >> func{main:(00202)} is call, pHexData="0x55b227664890" .

//0x007ffd3cbcc998|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|
//      =============================================================================
//      0x00000000|2f d6 bc 3c fd 7f 00 00  37 d6 bc 3c fd 7f 00 00|/..<....7..<....|
//      0x00000010|3b d6 bc 3c fd 7f 00 00  44 d6 bc 3c fd 7f 00 00|;..<....D..<....|
//      0x00000020|47 d6 bc 3c fd 7f 00 00  4d d6 bc 3c fd 7f 00 00|G..<....M..<....|
//      0x00000030|51 d6 bc 3c fd 7f 00 00  64 d6 bc 3c fd 7f 00 00|Q..<....d..<....|
//      0x00000040|67 d6 bc 3c fd 7f 00 00  00 00 00 00 00 00 00 00|g..<............|
//      0x00000050|6d d6 bc 3c fd 7f 00 00  7d d6 bc 3c fd 7f 00 00|m..<....}..<....|
//      0x00000060|95 d6 bc 3c fd 7f 00 00  ac d6 bc 3c fd 7f 00 00|...<.......<....|
//      0x00000070|c0 d6 bc 3c fd 7f 00 00  d8 d6 bc 3c fd 7f 00 00|...<.......<....|
//      0x00000080|fd d6 bc 3c fd 7f 00 00  0c d7 bc 3c fd 7f 00 00|...<.......<....|
//      0x00000090|21 d7 bc 3c fd 7f 00 00  30 d7 bc 3c fd 7f 00 00|!..<....0..<....|
//      0x000000a0|42 d7 bc 3c fd 7f 00 00  57 ** ** ** ** ** ** **|B..<....W*******|
//      =============================================================================
//
//
//0x007ffd3cbcd62f|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|
//      =============================================================================
//      0x00000000|2e 2f 6d 79 61 70 70 00  67 63 63 00 2d 73 74 64|./myapp.gcc.-std|
//      0x00000010|3d 63 31 31 00 2d 67 00  2d 57 61 6c 6c 00 2d 4f|=c11.-g.-Wall.-O|
//      0x00000020|30 00 6d 79 61 70 70 2d  63 31 31 2d 30 2e 31 2e|0.myapp-c11-0.1.|
//      0x00000030|30 32 2e 63 00 2d 6f 00  6d 79 61 70 70 00 53 48|02.c.-o.myapp.SH|
//      0x00000040|45 4c 4c 3d 2f 62 69 6e  2f 62 61 73 68 00 4c 41|ELL=/bin/bash.LA|
//      0x00000050|4e 47 55 41 47 45 3d 7a  68 5f 43 4e 3a 65 6e 5f|NGUAGE=zh_CN:en_|
//      0x00000060|55 53 3a 65 6e 00 4c 43  5f 41 44 44 52 45 53 53|US:en.LC_ADDRESS|
//      0x00000070|3d 7a 68 5f 43 4e 2e 55  54 46 2d 38 00 4c 43 5f|=zh_CN.UTF-8.LC_|
//      0x00000080|4e 41 4d 45 3d 7a 68 5f  43 4e 2e 55 54 46 2d 38|NAME=zh_CN.UTF-8|
//      0x00000090|00 4c 43 5f 4d 4f 4e 45  54 41 52 59 3d 7a 68 5f|.LC_MONETARY=zh_|
//      0x000000a0|43 4e 2e 55 54 46 2d 38  00 ** ** ** ** ** ** **|CN.UTF-8.*******|
//      =============================================================================
//
//
//0x0055b227664890|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|
//      =============================================================================
//      0x00000000|7f 45 4c 46 02 01 01 00  00 00 00 00 00 00 00 00|.ELF............|
//      0x00000010|03 00 3e 00 01 00 00 00  80 11 00 00 00 00 00 00|..>.............|
//      0x00000020|40 00 00 00 00 00 00 00  70 50 00 00 00 00 00 00|@.......pP......|
//      0x00000030|00 00 00 00 40 00 38 00  0d 00 40 00 24 00 23 00|....@.8...@.$.#.|
//      0x00000040|06 00 00 00 04 00 00 00  40 00 00 00 00 00 00 00|........@.......|
//      0x00000050|40 00 00 00 00 00 00 00  40 00 00 00 00 00 00 00|@.......@.......|
//      0x00000060|d8 02 00 00 00 00 00 00  d8 02 00 00 00 00 00 00|................|
//      0x00000070|08 00 00 00 00 00 00 00  03 00 00 00 04 00 00 00|................|
//      0x00000080|18 03 00 00 00 00 00 00  18 03 00 00 00 00 00 00|................|
//      0x00000090|18 03 00 00 00 00 00 00  1c 00 00 00 00 00 00 00|................|
//      0x000000a0|1c 00 00 00 00 00 00 00  01 00 00 00 00 00 00 00|................|
//      0x000000b0|01 00 00 00 04 00 00 00  00 00 00 00 00 00 00 00|................|
//      0x000000c0|00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00|................|
//      0x000000d0|78 08 00 00 00 00 00 00  78 08 00 00 00 00 00 00|x.......x.......|
//      0x000000e0|00 10 00 00 00 00 00 00  01 00 00 00 05 00 00 00|................|
//      0x000000f0|00 10 00 00 00 00 00 00  00 10 00 00 00 00 00 00|................|
//      0x00000100|00 10 00 00 00 00 00 00  71 0b 00 00 00 00 00 00|........q.......|
//      0x00000110|71 0b 00 00 00 00 00 00  00 10 00 00 00 00 00 00|q...............|
//      0x00000120|01 00 00 00 04 00 00 00  00 20 00 00 00 00 00 00|......... ......|
//      0x00000130|00 20 00 00 00 00 00 00  00 20 00 00 00 00 00 00|. ....... ......|
//      0x00000140|78 03 00 00 00 00 00 00  78 03 00 00 00 00 00 00|x.......x.......|
//      0x00000150|00 10 00 00 00 00 00 00  01 00 00 00 06 00 00 00|................|
//      0x00000160|70 2d 00 00 00 00 00 00  70 3d 00 00 00 00 00 00|p-......p=......|
//      0x00000170|70 3d 00 00 00 00 00 00  a0 02 00 00 00 00 00 00|p=..............|
//      0x00000180|b0 02 00 00 00 00 00 00  00 10 00 00 00 00 00 00|................|
//      0x00000190|02 00 00 00 06 00 00 00  80 2d 00 00 00 00 00 00|.........-......|
//      0x000001a0|80 3d 00 00 00 00 00 00  80 3d 00 00 00 00 00 00|.=.......=......|
//      0x000001b0|f0 01 00 00 00 00 00 00  f0 01 00 00 00 00 00 00|................|
//      0x000001c0|08 00 00 00 00 00 00 00  04 00 00 00 04 00 00 00|................|
//      0x000001d0|38 03 00 00 00 00 00 00  38 03 00 00 00 00 00 00|8.......8.......|
//      0x000001e0|38 03 00 00 00 00 00 00  20 00 00 ** ** ** ** **|8....... ..*****|
//      =============================================================================
//
//
//0x007ffd3cbcc830|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|
//      =============================================================================
//      0x00000000|60 20 20 20 31 32 33 34  2e 2e 2e 2e 5c 30 00 2e|`   1234....\0..|
//      0x00000010|37 38 39 30 7e 21 40 23  24 25 5e 26 2a 28 29 5f|7890~!@#$%^&*()_|
//      0x00000020|2b 2d 3d 71 77 65 72 74  79 75 69 6f 70 5b 5d 5c|+-=qwertyuiop[]\|
//      0x00000030|61 73 64 66 67 68 6a 6b  6c 3b 27 7a 78 63 76 62|asdfghjkl;'zxcvb|
//      0x00000040|6e 6d 2c 2e 2f 51 57 45  52 54 59 55 49 4f 50 7b|nm,./QWERTYUIOP{|
//      0x00000050|7d 7c 41 53 44 46 47 48  4a 4b 4c 3a 22 5a 58 43|}|ASDFGHJKL:"ZXC|
//      0x00000060|56 42 4e 4d 3c 3e 3f 00  ** ** ** ** ** ** ** **|VBNM<>?.********|
//      =============================================================================


  >> the app exit.
xadmin@hw:~/xwks.git.1/xapp-c11$ 

#endif
