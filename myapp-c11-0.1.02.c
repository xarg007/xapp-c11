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
                xlog_info("** ");
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
                else        xlog_info("%02x", pData[i * 16 + j]);
            }
            else
            {
                if (j < 15) xlog_info("** ");
                else        xlog_info("**");
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
                    if (isalpha(test_char)) break;
                    if (isdigit(test_char)) break;
                    if (ispunct(test_char)) break;
                    if (test_char == 0x20) break;
                    if (test_char == 0x0) break;
                    test_char = '.';
                } while (0);

                if (test_char == 0x0)
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
    struct stat statbuf = { 0 };

    stat(pFileName, &statbuf);

    unsigned int iLen = statbuf.st_size;
    if (iLen > 0 && iLen < 10 * 1024 * 1024) //文件目前最大设为10M
    {
        FILE* hFile = fopen(pFileName, "rb");
        if (hFile == NULL)
            return NULL;

        *piFileLen = iLen;
        pHexData = (unsigned char*)calloc(iLen, sizeof(unsigned char) + 8);

        size_t size_readok = fread(pHexData, 1, iLen, hFile);
        fclose(hFile);

        if (size_readok != iLen)
        {
            free(pHexData);
            return NULL;
        }

        return pHexData;
    }

    return NULL;
}

int main(int argc, char* argv[])
{
    xlog_info("  >> the app starting ... ...\n\n");

    for (int i = 0; i < argc; i++)
    {
        xlog_info("   >> argv[%02d] = {\"%s\"};\n", i, argv[i]);
    }

    xlog_info("\n");
#if 0
    char* pFileNameSelf = argv[0];
    char* pFileNameTarget = argv[1];

    xlog_info("  >> func{\e[31m%s:(%05d)()\e[0m}. pFileNameSelf=%s, pFileNameTarget=%s.\n",
        __func__, __LINE__, pFileNameSelf, pFileNameTarget);

    if (argc != 2)
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
        char* pFileName = argv[0];
        unsigned char* pHexData = NULL;
        unsigned int    iLen = 0;
        pHexData = getHexDataFromFile(pFileName, &iLen);
        if (pHexData != NULL && iLen > 0)
        {
            xlog_info("  >> func{%s:(%05d)} is call, pHexData=\"%p\" .\n", __func__, __LINE__, pHexData);

            DumpHex((unsigned char*)argv, 16 * 10 + 9);

            DumpHex((unsigned char*)argv[0], 16 * 10 + 9);

            DumpHex(pHexData, 16 * 30 + 11);

            //"\056"  "\0 56" ?? 注意转义符；
            char szTest[] = "`   1234....\\0\0\0567890~!@#$%^&*()_+-=qwertyuiop[]\\asdfghjkl;'zxcvbnm,./QWERTYUIOP{}|ASDFGHJKL:\"ZXCVBNM<>?";
            DumpHex((unsigned char*)szTest, sizeof(szTest));
        }
        free(pHexData);
    } while (0);

    xlog_info("\n");
    xlog_info("  >> the app exit.\n");

    return 0;
}



#if 0
//xadmin@hw:~/xwks.git.1/xapp-c11$ ./myapp
//  >> the app starting ... ...
//
//   >> argv[00] = {"./myapp"};
//
//  >> func{main:(00202)} is call, pHexData="0x7fd7329ad010" .
//
//0x007ffcad715948|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|
//      =============================================================================
//      0x00000000|66 76 71 ad fc 7f 00 00  00 00 00 00 00 00 00 00|fvq.............|
//      0x00000010|6e 76 71 ad fc 7f 00 00  7e 76 71 ad fc 7f 00 00|nvq.....~vq.....|
//      0x00000020|96 76 71 ad fc 7f 00 00  ad 76 71 ad fc 7f 00 00|.vq......vq.....|
//      0x00000030|c1 76 71 ad fc 7f 00 00  d9 76 71 ad fc 7f 00 00|.vq......vq.....|
//      0x00000040|fe 76 71 ad fc 7f 00 00  0d 77 71 ad fc 7f 00 00|.vq......wq.....|
//      0x00000050|22 77 71 ad fc 7f 00 00  31 77 71 ad fc 7f 00 00|"wq.....1wq.....|
//      0x00000060|43 77 71 ad fc 7f 00 00  58 77 71 ad fc 7f 00 00|Cwq.....Xwq.....|
//      0x00000070|69 77 71 ad fc 7f 00 00  4b 7d 71 ad fc 7f 00 00|iwq.....K}q.....|
//      0x00000080|80 7d 71 ad fc 7f 00 00  a2 7d 71 ad fc 7f 00 00|.}q......}q.....|
//      0x00000090|b9 7d 71 ad fc 7f 00 00  d7 7d 71 ad fc 7f 00 00|.}q......}q.....|
//      0x000000a0|e2 7d 71 ad fc 7f 00 00  02 ** ** ** ** ** ** **|.}q......*******|
//      =============================================================================
//
//
//0x007ffcad717666|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|
//      =============================================================================
//      0x00000000|2e 2f 6d 79 61 70 70 00  53 48 45 4c 4c 3d 2f 62|./myapp.SHELL=/b|
//      0x00000010|69 6e 2f 62 61 73 68 00  4c 41 4e 47 55 41 47 45|in/bash.LANGUAGE|
//      0x00000020|3d 7a 68 5f 43 4e 3a 65  6e 5f 55 53 3a 65 6e 00|=zh_CN:en_US:en.|
//      0x00000030|4c 43 5f 41 44 44 52 45  53 53 3d 7a 68 5f 43 4e|LC_ADDRESS=zh_CN|
//      0x00000040|2e 55 54 46 2d 38 00 4c  43 5f 4e 41 4d 45 3d 7a|.UTF-8.LC_NAME=z|
//      0x00000050|68 5f 43 4e 2e 55 54 46  2d 38 00 4c 43 5f 4d 4f|h_CN.UTF-8.LC_MO|
//      0x00000060|4e 45 54 41 52 59 3d 7a  68 5f 43 4e 2e 55 54 46|NETARY=zh_CN.UTF|
//      0x00000070|2d 38 00 50 57 44 3d 2f  68 6f 6d 65 2f 78 61 64|-8.PWD=/home/xad|
//      0x00000080|6d 69 6e 2f 78 77 6b 73  2e 67 69 74 2e 31 2f 78|min/xwks.git.1/x|
//      0x00000090|61 70 70 2d 63 31 31 00  4c 4f 47 4e 41 4d 45 3d|app-c11.LOGNAME=|
//      0x000000a0|78 61 64 6d 69 6e 00 58  44 ** ** ** ** ** ** **|xadmin.XD*******|
//      =============================================================================
//
//
//0x007fd7329ad010|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|
//      =============================================================================
//      0x00000000|7f 45 4c 46 02 01 01 00  00 00 00 00 00 00 00 00|.ELF............|
//      0x00000010|03 00 3e 00 01 00 00 00  80 11 00 00 00 00 00 00|..>.............|
//      0x00000020|40 00 00 00 00 00 00 00  60 50 00 00 00 00 00 00|@.......`P......|
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
//      0x00000100|00 10 00 00 00 00 00 00  81 0b 00 00 00 00 00 00|................|
//      0x00000110|81 0b 00 00 00 00 00 00  00 10 00 00 00 00 00 00|................|
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
//0x007ffcad7157e0|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|
//      =============================================================================
//      0x00000000|60 20 20 20 31 32 33 34  2e 2e 2e 2e 5c 30 00 2e|`   1234....\0..|
//      0x00000010|37 38 39 30 7e 21 40 23  24 25 5e 26 2a 28 29 5f|7890~!@#$%^&*()_|
//      0x00000020|2b 2d 3d 71 77 65 72 74  79 75 69 6f 70 5b 5d 5c|+-=qwertyuiop[]\|
//      0x00000030|61 73 64 66 67 68 6a 6b  6c 3b 27 7a 78 63 76 62|asdfghjkl;'zxcvb|
//      0x00000040|6e 6d 2c 2e 2f 51 57 45  52 54 59 55 49 4f 50 7b|nm,./QWERTYUIOP{|
//      0x00000050|7d 7c 41 53 44 46 47 48  4a 4b 4c 3a 22 5a 58 43|}|ASDFGHJKL:"ZXC|
//      0x00000060|56 42 4e 4d 3c 3e 3f 00  ** ** ** ** ** ** ** **|VBNM<>?.********|
//      =============================================================================
//
//
//  >> the app exit.
//xadmin@hw:~/xwks.git.1/xapp-c11$ 
//
//xadmin@hw:~/xwks.git.1/xapp-c11$ valgrind --tool=memcheck --leak-check=full --track-origins=yes -s ./myapp
//==29692== Memcheck, a memory error detector
//==29692== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
//==29692== Using Valgrind-3.15.0 and LibVEX; rerun with -h for copyright info
//==29692== Command: ./myapp
//==29692== 
//  >> the app starting ... ...
//
//   >> argv[00] = {"./myapp"};
//
//  >> func{main:(00202)} is call, pHexData="0x4a5c6a0" .
//
//0x00001fff0002d8|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|
//      =============================================================================
//      0x00000000|73 05 00 ff 1f 00 00 00  00 00 00 00 00 00 00 00|s...............|
//      0x00000010|7b 05 00 ff 1f 00 00 00  9b 05 00 ff 1f 00 00 00|{...............|
//      0x00000020|b3 05 00 ff 1f 00 00 00  bf 05 00 ff 1f 00 00 00|................|
//      0x00000030|e2 05 00 ff 1f 00 00 00  f6 05 00 ff 1f 00 00 00|................|
//      0x00000040|0b 06 00 ff 1f 00 00 00  13 06 00 ff 1f 00 00 00|................|
//      0x00000050|32 06 00 ff 1f 00 00 00  41 06 00 ff 1f 00 00 00|2.......A.......|
//      0x00000060|53 06 00 ff 1f 00 00 00  72 06 00 ff 1f 00 00 00|S.......r.......|
//      0x00000070|85 06 00 ff 1f 00 00 00  9d 06 00 ff 1f 00 00 00|................|
//      0x00000080|d3 06 00 ff 1f 00 00 00  e2 06 00 ff 1f 00 00 00|................|
//      0x00000090|f6 06 00 ff 1f 00 00 00  0d 07 00 ff 1f 00 00 00|................|
//      0x000000a0|18 07 00 ff 1f 00 00 00  29 ** ** ** ** ** ** **|........)*******|
//      =============================================================================
//
//
//0x00001fff000573|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|
//      =============================================================================
//      0x00000000|2e 2f 6d 79 61 70 70 00  4c 45 53 53 4f 50 45 4e|./myapp.LESSOPEN|
//      0x00000010|3d 7c 20 2f 75 73 72 2f  62 69 6e 2f 6c 65 73 73|=| /usr/bin/less|
//      0x00000020|70 69 70 65 20 25 73 00  4c 41 4e 47 55 41 47 45|pipe %s.LANGUAGE|
//      0x00000030|3d 7a 68 5f 43 4e 3a 65  6e 5f 55 53 3a 65 6e 00|=zh_CN:en_US:en.|
//      0x00000040|55 53 45 52 3d 78 61 64  6d 69 6e 00 53 53 48 5f|USER=xadmin.SSH_|
//      0x00000050|43 4c 49 45 4e 54 3d 31  39 32 2e 31 36 38 2e 33|CLIENT=192.168.3|
//      0x00000060|31 2e 31 39 36 20 36 31  37 38 34 20 32 32 00 4c|1.196 61784 22.L|
//      0x00000070|43 5f 54 49 4d 45 3d 7a  68 5f 43 4e 2e 55 54 46|C_TIME=zh_CN.UTF|
//      0x00000080|2d 38 00 58 44 47 5f 53  45 53 53 49 4f 4e 5f 54|-8.XDG_SESSION_T|
//      0x00000090|59 50 45 3d 74 74 79 00  53 48 4c 56 4c 3d 31 00|YPE=tty.SHLVL=1.|
//      0x000000a0|4c 44 5f 4c 49 42 52 41  52 ** ** ** ** ** ** **|LD_LIBRAR*******|
//      =============================================================================
//
//
//0x00000004a5c6a0|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|
//      =============================================================================
//      0x00000000|7f 45 4c 46 02 01 01 00  00 00 00 00 00 00 00 00|.ELF............|
//      0x00000010|03 00 3e 00 01 00 00 00  80 11 00 00 00 00 00 00|..>.............|
//      0x00000020|40 00 00 00 00 00 00 00  60 50 00 00 00 00 00 00|@.......`P......|
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
//      0x00000100|00 10 00 00 00 00 00 00  81 0b 00 00 00 00 00 00|................|
//      0x00000110|81 0b 00 00 00 00 00 00  00 10 00 00 00 00 00 00|................|
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
//0x00001fff000170|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|
//      =============================================================================
//      0x00000000|60 20 20 20 31 32 33 34  2e 2e 2e 2e 5c 30 00 2e|`   1234....\0..|
//      0x00000010|37 38 39 30 7e 21 40 23  24 25 5e 26 2a 28 29 5f|7890~!@#$%^&*()_|
//      0x00000020|2b 2d 3d 71 77 65 72 74  79 75 69 6f 70 5b 5d 5c|+-=qwertyuiop[]\|
//      0x00000030|61 73 64 66 67 68 6a 6b  6c 3b 27 7a 78 63 76 62|asdfghjkl;'zxcvb|
//      0x00000040|6e 6d 2c 2e 2f 51 57 45  52 54 59 55 49 4f 50 7b|nm,./QWERTYUIOP{|
//      0x00000050|7d 7c 41 53 44 46 47 48  4a 4b 4c 3a 22 5a 58 43|}|ASDFGHJKL:"ZXC|
//      0x00000060|56 42 4e 4d 3c 3e 3f 00  ** ** ** ** ** ** ** **|VBNM<>?.********|
//      =============================================================================
//
//
//  >> the app exit.
//==29692== 
//==29692== HEAP SUMMARY:
//==29692==     in use at exit: 0 bytes in 0 blocks
//==29692==   total heap usage: 4 allocs, 4 frees, 211,512 bytes allocated
//==29692== 
//==29692== All heap blocks were freed -- no leaks are possible
//==29692== 
//==29692== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
//xadmin@hw:~/xwks.git.1/xapp-c11$ 
//
#endif
