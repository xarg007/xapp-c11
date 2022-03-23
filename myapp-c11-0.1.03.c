#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

// https://www.runoob.com/cprogramming/c-tutorial.html
// https://en.cppreference.com/w/
// https://zh.cppreference.com/w/c/language/main_function

// 03: 多种场景显示十六进制数据块；
// gcc -std=c11 -g -Wall -O0 myapp-c11-0.1.03.c -o myapp

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
    if (pData == NULL || iLen == 0)
    {
        return;
    }

    xlog_info("\n");
    xlog_info("%016p", pData);
    xlog_info("|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|\n");
    xlog_info("      =============================================================================\n");

    unsigned int iRow = (iLen % 16 != 0 ? iLen / 16 + 1 : iLen / 16);
    for (unsigned int i = 0; i < iRow; i++) //逐行处理
    {
        //数据相对地址
        xlog_info("      0x%08x|", i * 16);

        //十六进制数据
        // xlog_info("\e[32m");
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
                if (j < 15)
                    xlog_info("%02x ", pData[i * 16 + j]);
                else
                    xlog_info("%02x", pData[i * 16 + j]);
            }
            else
            {
                if (j < 15)
                    xlog_info("** ");
                else
                    xlog_info("**");
            }
        }

        // xlog_info("\e[0m");

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
                    if (isalpha(test_char))
                        break;
                    if (isdigit(test_char))
                        break;
                    if (ispunct(test_char))
                        break;
                    if (test_char == 0x20)
                        break;
                    if (test_char == 0x0)
                        break;
                    test_char = '.';
                } while (0);

                if (test_char == 0x0)
                {
                    // xlog_info("\e[37m.\e[0m");
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

// http://c.biancheng.net/ref/ctype_h/
//测试输出指定地址的格式化的十六进制数据块
void DumpHexA(unsigned char* pData, unsigned int iLen)
{
    // xlog_info("  >> func{%s:(%05d)} is call.{pData=%p, iLen=%d}.\n", __func__, __LINE__, pData, iLen);

    if (pData == NULL || iLen == 0)
    {
        return;
    }

    xlog_info("\n");
    xlog_info("%016p", pData);
    xlog_info("|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|\n");
    xlog_info("      =============================================================================\n");

    //行数 //如果条件为真 ? 则值为 X : 否则值为 Y
    unsigned int iRow = (iLen % 16 != 0 ? iLen / 16 + 1 : iLen / 16);
    for (unsigned int i = 0; i < iRow; i++) //逐行处理
    {
        //数据相对地址
        xlog_info("      0x%08x|", i * 16);

        //十六进制数据
        // xlog_info("\e[1m");

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
                if (j < 15)
                    xlog_info("%02x ", pData[i * 16 + j]);
                else
                    xlog_info("%02x", pData[i * 16 + j]);
            }
            else
            {
                if (j < 15)
                    xlog_info("** ");
                else
                    xlog_info("**");
            }
        }

        // xlog_info("\e[0m");

        //数据与字符边界
        xlog_info("|");

        //显示字符
        for (unsigned int j = 0; j < 16; j++)
        {
            if ((i * 16 + j) < iLen)
            {
                char test_char = pData[i * 16 + j];
                if (!isalpha(test_char)		//不是字母
                    && !isdigit(test_char)	//不是数字
                    && !ispunct(test_char)	//
                    && !(test_char == 0x20) //
                                            //&& !(test_char==0x0)  //
                    )
                {
                    test_char = '.';
                }

                xlog_info("%c", test_char);
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

// http://c.biancheng.net/ref/ctype_h/
//测试输出指定地址的格式化的十六进制数据块
void DumpHexB(unsigned char* pData, unsigned int iLen)
{
    // xlog_info("  >> func{%s:(%05d)} is call.{pData=%p, iLen=%d}.\n", __func__, __LINE__, pData, iLen);

    if (pData == NULL || iLen == 0)
    {
        return;
    }

    xlog_info("\n");
    xlog_info("%016p", pData);
    xlog_info("|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|\n");
    xlog_info("      =============================================================================\n");

    //行数 //如果条件为真 ? 则值为 X : 否则值为 Y
    unsigned int iRow = (iLen % 16 != 0 ? iLen / 16 + 1 : iLen / 16);
    for (unsigned int i = 0; i < iRow; i++) //逐行处理
    {
        //数据相对地址
        xlog_info("      0x%08x|", i * 16);

        //十六进制数据

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
                if (j < 15)
                    xlog_info("%02x ", pData[i * 16 + j]);
                else
                    xlog_info("%02x", pData[i * 16 + j]);
            }
            else
            {
                if (j < 15)
                    xlog_info("** ");
                else
                    xlog_info("**");
            }
        }

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
                    if (isalpha(test_char))
                        break;
                    if (isdigit(test_char))
                        break;
                    if (ispunct(test_char))
                        break;
                    if (test_char == 0x20)
                        break;
                    if (test_char == 0x0)
                        break;
                    test_char = '.';
                } while (0);

                if (test_char == 0x0)
                {
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

//测试输出，指定地址的数据以指针的方式显示
void DumpPtr(unsigned char* pDataStart, unsigned int iPtrCnt)
{
    void** ppVoidPtr = (void**)pDataStart;
    xlog_info("      ===========================================================\n");

    for (unsigned int i = 0; i < iPtrCnt; i++)
    {
        xlog_info("      >> ptr[%03d] = %016p;\n", i, *(ppVoidPtr + i));
    }

    xlog_info("      ===========================================================\n");
    xlog_info("\n");

    return;
}

//测试输出，指定地址的数据以指针的方式显示
void DumpPtr2Str(unsigned char* pDataStart, unsigned int iPtrCnt)
{
    xlog_info("      ===========================================================\n");
    for (unsigned int i = 0; i < iPtrCnt; i++)
    {
        while (*(char*)pDataStart == '\0')
        {
            pDataStart++;
        }

        xlog_info("      >> ptr[%03d]=%016p; str={\"%s\"};\n", i, pDataStart, (char*)pDataStart);

        while (*(char*)pDataStart != '\0')
        {
            pDataStart++;
        }
    }

    xlog_info("      ===========================================================\n");
    xlog_info("\n");

    return;
}

void DumpHex2Char(unsigned char* pData, unsigned int iLen)
{
    xlog_info("  >> func{%s:(%05d)} is call.{pData=%p, iLen=%d}.\n",
        __func__, __LINE__, pData, iLen);

    if (pData == NULL || iLen == 0)
    {
        return;
    }

    xlog_info("\n");
    xlog_info("      0x????????|00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F|\n");
    xlog_info("      ===========================================================\n");

    unsigned int iRow = (iLen % 16 != 0 ? iLen / 16 + 1 : iLen / 16);
    for (unsigned int i = 0; i < iRow; i++)
    {
        xlog_info("      0x%08x|", i * 16);
        for (unsigned int j = 0; j < 16; j++)
        {
            if ((i * 16 + j) < iLen)
            {
                char test_char = pData[i * 16 + j];
                if (!isalpha(test_char)		//不是字母
                    && !isdigit(test_char)	//不是数字
                    && !ispunct(test_char)	//
                    && !(test_char == 0x20) //
                    && !(test_char == '\0'))
                {
                    test_char = '.';
                }

                if (test_char == '\0')
                    test_char = ' ';

                if (j < 15)
                    xlog_info(" %c ", test_char);
                else
                    xlog_info(" %c", test_char);
            }
            else
            {
                if (j < 15)
                    xlog_info("** ");
                else
                    xlog_info("**");
            }
        }
        xlog_info("|\n");
    }

    xlog_info("\n");

    return;
}

void DumpHex2CharB(unsigned char* pData, unsigned int iLen)
{
    xlog_info("  >> func{%s:(%05d)} is call.{pData=%p, iLen=%d}.\n",
        __func__, __LINE__, pData, iLen);

    if (pData == NULL || iLen == 0)
    {
        return;
    }

    xlog_info("\n");
    xlog_info("      0x????????|0123456789ABCDEF|\n");
    xlog_info("      ===========================================================\n");

    unsigned int iRow = (iLen % 16 != 0 ? iLen / 16 + 1 : iLen / 16);
    for (unsigned int i = 0; i < iRow; i++)
    {
        xlog_info("      0x%08x|", i * 16);
        for (unsigned int j = 0; j < 16; j++)
        {
            if ((i * 16 + j) < iLen)
            {
                char test_char = pData[i * 16 + j];
                if (!isalpha(test_char)		//不是字母
                    && !isdigit(test_char)	//不是数字
                    && !ispunct(test_char)	//
                    && !(test_char == 0x20) //
                    && !(test_char == '\0'))
                {
                    test_char = '.';
                }

                if (test_char == '\0')
                    test_char = ' ';

                if (j < 15)
                    xlog_info("%c", test_char);
                else
                    xlog_info("%c", test_char);
            }
            else
            {
                if (j < 15)
                    xlog_info("*");
                else
                    xlog_info("*");
            }
        }
        xlog_info("|\n");
    }

    xlog_info("\n");

    return;
}

void DumpHex2CharA(unsigned char* pData, unsigned int iLen)
{
    xlog_info("  >> func{%s:(%05d)} is call.{pData=%p, iLen=%d}.\n",
        __func__, __LINE__, pData, iLen);

    if (pData == NULL || iLen == 0)
    {
        return;
    }

    xlog_info("\n");
    xlog_info("      0x????????|00    01    02    03    04    05    06    07    08    09    0A    0B    0C    0D    0E    0F   |\n");
    xlog_info("      ===========================================================================================================\n");

    unsigned int iRow = (iLen % 16 != 0 ? iLen / 16 + 1 : iLen / 16);
    for (unsigned int i = 0; i < iRow; i++)
    {
        xlog_info("      0x%08x|", i * 16);
        for (unsigned int j = 0; j < 16; j++)
        {
            if ((i * 16 + j) < iLen)
            {
                char test_char = pData[i * 16 + j];

                if (!isalpha(test_char)		//不是字母
                    && !isdigit(test_char)	//不是数字
                    && !ispunct(test_char)	//
                    && !(test_char == 0x20) //
                    )
                {
                    test_char = '.';
                }

                if (test_char == '\0')
                    test_char = ' ';

                if (j < 15)
                    xlog_info("%c(%02x) ", test_char, test_char);
                else
                    xlog_info("%c(%02x)", test_char, test_char);
            }
            else
            {
                if (j < 15)
                    xlog_info("**    ");
                else
                    xlog_info("**   ");
            }
        }
        xlog_info("|\n");
    }
    xlog_info("      ===========================================================================================================\n");
    xlog_info("\n");

    return;
}

unsigned char* getDataFromFile(const char* pFileName, int* piFileLen)
{
    printf("  >> func{%s:(%05d)} is call ,pFileName=\"%s\".\n",
        __func__, __LINE__, pFileName);

    unsigned char* pData = NULL;

    struct stat statbuf = { 0 };
    stat(pFileName, &statbuf);
    int iLen = statbuf.st_size;
    if (iLen > 0 && iLen < 10 * 1024 * 1024) //文件目前最大设为10M
    {
        FILE* hFile = fopen(pFileName, "rb");
        if (hFile == NULL)
        {
            return NULL;
        }
        *piFileLen = iLen;
        pData = calloc(iLen, sizeof(unsigned char));

        size_t size_readok = fread(pData, 1, iLen, hFile);
        fclose(hFile);
        if (size_readok != iLen)
        {
            free(pData);
            return NULL;
        }
        return pData;
    }

    return NULL;
}

int main(int argc, char* argv[])
{
    printf("  >> func:%s(%d, %p) is called. (@file:%s,line:%04d).\n", __func__, argc, argv, __FILE__, __LINE__);
    xlog_info("  >> func:%s(%d, %p) is called. (@file:%s,line:%04d).\n", __func__, argc, argv, __FILE__, __LINE__);

    char* pFileNameSelf = argv[0];
    char* pFileNameTarget = argv[1];

    xlog_info("  >> func{%s:(%05d)()}. pFileNameSelf=%s, pFileNameTarget=%s.\n",
        __func__, __LINE__, pFileNameSelf, pFileNameTarget);

    if (argc != 2)
    {
        // err
        xlog_info("  >> func{%s:(%05d)@(%s)(param tbc)} is error !!\n", __func__, __LINE__, __FILE__);
        return -1;
    }

    //取文件数据
    do
    {
        ///////////////////////////////////////////
        // get data from target file
        char* pFileName = pFileNameTarget;
        unsigned char* pHexData = NULL;
        int iLen = 0;
        pHexData = getDataFromFile(pFileName, &iLen);
        if (pHexData != NULL && iLen > 0)
        {
            xlog_info("  >> func{%s:(%05d)} is call, pHexData=\"%p\" .\n", __func__, __LINE__, pHexData);

            DumpHex((unsigned char*)argv, 16 * 10 + 9);
            DumpHexA((unsigned char*)argv, 16 * 10 + 9);

            DumpPtr((unsigned char*)argv, argc + 1);
            DumpPtr2Str((unsigned char*)argv[0], argc + 5);

            DumpHex2Char((unsigned char*)argv[0], 16 * 10 + 9);
            DumpHex2CharA((unsigned char*)argv[0], 16 * 10 + 9);
            DumpHex2CharB((unsigned char*)argv[0], 16 * 10 + 9);

            DumpHex((unsigned char*)argv[0], 16 * 10 + 9);
            DumpHexA((unsigned char*)argv[0], 16 * 10 + 9);
            DumpHexB((unsigned char*)argv[0], 16 * 10 + 9);

            DumpHexA(pHexData, 16 * 30 + 11);
            DumpHexB(pHexData, 16 * 30 + 11);
            //"\056"  "\0 56" ?? 注意转义符；
            char szTest[] = "`   1234....\\0\0\0567890~!@#$%^&*()_+-=qwertyuiop[]\\asdfghjkl;'zxcvbnm,./QWERTYUIOP{}|ASDFGHJKL:\"ZXCVBNM<>?";
            DumpHexA((unsigned char*)szTest, sizeof(szTest));
            DumpHexB((unsigned char*)szTest, sizeof(szTest));
        }
        free(pHexData);
    } while (0);

    xlog_info("  >> func:%s() is called. (@file:%s,line:%04d).\n", __func__, __FILE__, __LINE__);

    return 0;
}

#if 0
xadmin@hw:~/ xwks.git.1 / xapp - c11$ gcc - std = c11 - g - Wall - O0 myapp - c11 - 0.1.03.c - o myapp
xadmin@hw : ~/ xwks.git.1 / xapp - c11$ . / myapp myapp
>> func : main(2, 0x7ffe64147428) is called. (@file:myapp - c11 - 0.1.03.c, line : 0550).
>> func : main(2, 0x7ffe64147428) is called. (@file:myapp - c11 - 0.1.03.c, line : 0551).
>> func{ main:(00557)() }. pFileNameSelf = . / myapp, pFileNameTarget = myapp.
>> func{ getDataFromFile:(00521) } is call, pFileName = "myapp".
>> func{ main:(00577) } is call, pHexData = "0x56479e8c3890" .

//0x007ffe64147428|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|
//      =============================================================================
//      0x00000000|5f 86 14 64 fe 7f 00 00  67 86 14 64 fe 7f 00 00|_..d....g..d....|
//      0x00000010|00 00 00 00 00 00 00 00  6d 86 14 64 fe 7f 00 00|........m..d....|
//      0x00000020|7d 86 14 64 fe 7f 00 00  95 86 14 64 fe 7f 00 00|}..d.......d....|
//      0x00000030|ac 86 14 64 fe 7f 00 00  c0 86 14 64 fe 7f 00 00|...d.......d....|
//      0x00000040|d8 86 14 64 fe 7f 00 00  fd 86 14 64 fe 7f 00 00|...d.......d....|
//      0x00000050|0c 87 14 64 fe 7f 00 00  21 87 14 64 fe 7f 00 00|...d....!..d....|
//      0x00000060|30 87 14 64 fe 7f 00 00  42 87 14 64 fe 7f 00 00|0..d....B..d....|
//      0x00000070|57 87 14 64 fe 7f 00 00  68 87 14 64 fe 7f 00 00|W..d....h..d....|
//      0x00000080|4a 8d 14 64 fe 7f 00 00  7f 8d 14 64 fe 7f 00 00|J..d.......d....|
//      0x00000090|a1 8d 14 64 fe 7f 00 00  b8 8d 14 64 fe 7f 00 00|...d.......d....|
//      0x000000a0|d6 8d 14 64 fe 7f 00 00  e1 ** ** ** ** ** ** **|...d.....*******|
//      =============================================================================
//
//
//0x007ffe64147428|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|
//      =============================================================================
//      0x00000000|5f 86 14 64 fe 7f 00 00  67 86 14 64 fe 7f 00 00|_..d....g..d....|
//      0x00000010|00 00 00 00 00 00 00 00  6d 86 14 64 fe 7f 00 00|........m..d....|
//      0x00000020|7d 86 14 64 fe 7f 00 00  95 86 14 64 fe 7f 00 00|}..d.......d....|
//      0x00000030|ac 86 14 64 fe 7f 00 00  c0 86 14 64 fe 7f 00 00|...d.......d....|
//      0x00000040|d8 86 14 64 fe 7f 00 00  fd 86 14 64 fe 7f 00 00|...d.......d....|
//      0x00000050|0c 87 14 64 fe 7f 00 00  21 87 14 64 fe 7f 00 00|...d....!..d....|
//      0x00000060|30 87 14 64 fe 7f 00 00  42 87 14 64 fe 7f 00 00|0..d....B..d....|
//      0x00000070|57 87 14 64 fe 7f 00 00  68 87 14 64 fe 7f 00 00|W..d....h..d....|
//      0x00000080|4a 8d 14 64 fe 7f 00 00  7f 8d 14 64 fe 7f 00 00|J..d.......d....|
//      0x00000090|a1 8d 14 64 fe 7f 00 00  b8 8d 14 64 fe 7f 00 00|...d.......d....|
//      0x000000a0|d6 8d 14 64 fe 7f 00 00  e1 ** ** ** ** ** ** **|...d.....*******|
//      =============================================================================
//
//      ===========================================================
//      >> ptr[000] = 0x007ffe6414865f;
//      >> ptr[001] = 0x007ffe64148667;
//      >> ptr[002] =            (nil);
//      ===========================================================
//
//      ===========================================================
//      >> ptr[000]=0x007ffe6414865f; str={"./myapp"};
//      >> ptr[001]=0x007ffe64148667; str={"myapp"};
//      >> ptr[002]=0x007ffe6414866d; str={"SHELL=/bin/bash"};
//      >> ptr[003]=0x007ffe6414867d; str={"LANGUAGE=zh_CN:en_US:en"};
//      >> ptr[004]=0x007ffe64148695; str={"LC_ADDRESS=zh_CN.UTF-8"};
//      >> ptr[005]=0x007ffe641486ac; str={"LC_NAME=zh_CN.UTF-8"};
//      >> ptr[006]=0x007ffe641486c0; str={"LC_MONETARY=zh_CN.UTF-8"};
//      ===========================================================
//
//  >> func{DumpHex2Char:(00371)} is call.{pData=0x7ffe6414865f, iLen=169}.
//
//      0x????????|00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F|
//      ===========================================================
//      0x00000000| .  /  m  y  a  p  p     m  y  a  p  p     S  H|
//      0x00000010| E  L  L  =  /  b  i  n  /  b  a  s  h     L  A|
//      0x00000020| N  G  U  A  G  E  =  z  h  _  C  N  :  e  n  _|
//      0x00000030| U  S  :  e  n     L  C  _  A  D  D  R  E  S  S|
//      0x00000040| =  z  h  _  C  N  .  U  T  F  -  8     L  C  _|
//      0x00000050| N  A  M  E  =  z  h  _  C  N  .  U  T  F  -  8|
//      0x00000060|    L  C  _  M  O  N  E  T  A  R  Y  =  z  h  _|
//      0x00000070| C  N  .  U  T  F  -  8     P  W  D  =  /  h  o|
//      0x00000080| m  e  /  x  a  d  m  i  n  /  x  w  k  s  .  g|
//      0x00000090| i  t  .  1  /  x  a  p  p  -  c  1  1     L  O|
//      0x000000a0| G  N  A  M  E  =  x  a  d ** ** ** ** ** ** **|
//
//  >> func{DumpHex2CharA:(00471)} is call.{pData=0x7ffe6414865f, iLen=169}.
//
//      0x????????|00    01    02    03    04    05    06    07    08    09    0A    0B    0C    0D    0E    0F   |
//      ===========================================================================================================
//      0x00000000|.(2e) /(2f) m(6d) y(79) a(61) p(70) p(70) .(2e) m(6d) y(79) a(61) p(70) p(70) .(2e) S(53) H(48)|
//      0x00000010|E(45) L(4c) L(4c) =(3d) /(2f) b(62) i(69) n(6e) /(2f) b(62) a(61) s(73) h(68) .(2e) L(4c) A(41)|
//      0x00000020|N(4e) G(47) U(55) A(41) G(47) E(45) =(3d) z(7a) h(68) _(5f) C(43) N(4e) :(3a) e(65) n(6e) _(5f)|
//      0x00000030|U(55) S(53) :(3a) e(65) n(6e) .(2e) L(4c) C(43) _(5f) A(41) D(44) D(44) R(52) E(45) S(53) S(53)|
//      0x00000040|=(3d) z(7a) h(68) _(5f) C(43) N(4e) .(2e) U(55) T(54) F(46) -(2d) 8(38) .(2e) L(4c) C(43) _(5f)|
//      0x00000050|N(4e) A(41) M(4d) E(45) =(3d) z(7a) h(68) _(5f) C(43) N(4e) .(2e) U(55) T(54) F(46) -(2d) 8(38)|
//      0x00000060|.(2e) L(4c) C(43) _(5f) M(4d) O(4f) N(4e) E(45) T(54) A(41) R(52) Y(59) =(3d) z(7a) h(68) _(5f)|
//      0x00000070|C(43) N(4e) .(2e) U(55) T(54) F(46) -(2d) 8(38) .(2e) P(50) W(57) D(44) =(3d) /(2f) h(68) o(6f)|
//      0x00000080|m(6d) e(65) /(2f) x(78) a(61) d(64) m(6d) i(69) n(6e) /(2f) x(78) w(77) k(6b) s(73) .(2e) g(67)|
//      0x00000090|i(69) t(74) .(2e) 1(31) /(2f) x(78) a(61) p(70) p(70) -(2d) c(63) 1(31) 1(31) .(2e) L(4c) O(4f)|
//      0x000000a0|G(47) N(4e) A(41) M(4d) E(45) =(3d) x(78) a(61) d(64) **    **    **    **    **    **    **   |
//      ===========================================================================================================
//
//  >> func{DumpHex2CharB:(00421)} is call.{pData=0x7ffe6414865f, iLen=169}.
//
//      0x????????|0123456789ABCDEF|
//      ===========================================================
//      0x00000000|./myapp myapp SH|
//      0x00000010|ELL=/bin/bash LA|
//      0x00000020|NGUAGE=zh_CN:en_|
//      0x00000030|US:en LC_ADDRESS|
//      0x00000040|=zh_CN.UTF-8 LC_|
//      0x00000050|NAME=zh_CN.UTF-8|
//      0x00000060| LC_MONETARY=zh_|
//      0x00000070|CN.UTF-8 PWD=/ho|
//      0x00000080|me/xadmin/xwks.g|
//      0x00000090|it.1/xapp-c11 LO|
//      0x000000a0|GNAME=xad*******|
//
//
//0x007ffe6414865f|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|
//      =============================================================================
//      0x00000000|2e 2f 6d 79 61 70 70 00  6d 79 61 70 70 00 53 48|./myapp.myapp.SH|
//      0x00000010|45 4c 4c 3d 2f 62 69 6e  2f 62 61 73 68 00 4c 41|ELL=/bin/bash.LA|
//      0x00000020|4e 47 55 41 47 45 3d 7a  68 5f 43 4e 3a 65 6e 5f|NGUAGE=zh_CN:en_|
//      0x00000030|55 53 3a 65 6e 00 4c 43  5f 41 44 44 52 45 53 53|US:en.LC_ADDRESS|
//      0x00000040|3d 7a 68 5f 43 4e 2e 55  54 46 2d 38 00 4c 43 5f|=zh_CN.UTF-8.LC_|
//      0x00000050|4e 41 4d 45 3d 7a 68 5f  43 4e 2e 55 54 46 2d 38|NAME=zh_CN.UTF-8|
//      0x00000060|00 4c 43 5f 4d 4f 4e 45  54 41 52 59 3d 7a 68 5f|.LC_MONETARY=zh_|
//      0x00000070|43 4e 2e 55 54 46 2d 38  00 50 57 44 3d 2f 68 6f|CN.UTF-8.PWD=/ho|
//      0x00000080|6d 65 2f 78 61 64 6d 69  6e 2f 78 77 6b 73 2e 67|me/xadmin/xwks.g|
//      0x00000090|69 74 2e 31 2f 78 61 70  70 2d 63 31 31 00 4c 4f|it.1/xapp-c11.LO|
//      0x000000a0|47 4e 41 4d 45 3d 78 61  64 ** ** ** ** ** ** **|GNAME=xad*******|
//      =============================================================================
//
//
//0x007ffe6414865f|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|
//      =============================================================================
//      0x00000000|2e 2f 6d 79 61 70 70 00  6d 79 61 70 70 00 53 48|./myapp.myapp.SH|
//      0x00000010|45 4c 4c 3d 2f 62 69 6e  2f 62 61 73 68 00 4c 41|ELL=/bin/bash.LA|
//      0x00000020|4e 47 55 41 47 45 3d 7a  68 5f 43 4e 3a 65 6e 5f|NGUAGE=zh_CN:en_|
//      0x00000030|55 53 3a 65 6e 00 4c 43  5f 41 44 44 52 45 53 53|US:en.LC_ADDRESS|
//      0x00000040|3d 7a 68 5f 43 4e 2e 55  54 46 2d 38 00 4c 43 5f|=zh_CN.UTF-8.LC_|
//      0x00000050|4e 41 4d 45 3d 7a 68 5f  43 4e 2e 55 54 46 2d 38|NAME=zh_CN.UTF-8|
//      0x00000060|00 4c 43 5f 4d 4f 4e 45  54 41 52 59 3d 7a 68 5f|.LC_MONETARY=zh_|
//      0x00000070|43 4e 2e 55 54 46 2d 38  00 50 57 44 3d 2f 68 6f|CN.UTF-8.PWD=/ho|
//      0x00000080|6d 65 2f 78 61 64 6d 69  6e 2f 78 77 6b 73 2e 67|me/xadmin/xwks.g|
//      0x00000090|69 74 2e 31 2f 78 61 70  70 2d 63 31 31 00 4c 4f|it.1/xapp-c11.LO|
//      0x000000a0|47 4e 41 4d 45 3d 78 61  64 ** ** ** ** ** ** **|GNAME=xad*******|
//      =============================================================================
//
//
//0x007ffe6414865f|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|
//      =============================================================================
//      0x00000000|2e 2f 6d 79 61 70 70 00  6d 79 61 70 70 00 53 48|./myapp.myapp.SH|
//      0x00000010|45 4c 4c 3d 2f 62 69 6e  2f 62 61 73 68 00 4c 41|ELL=/bin/bash.LA|
//      0x00000020|4e 47 55 41 47 45 3d 7a  68 5f 43 4e 3a 65 6e 5f|NGUAGE=zh_CN:en_|
//      0x00000030|55 53 3a 65 6e 00 4c 43  5f 41 44 44 52 45 53 53|US:en.LC_ADDRESS|
//      0x00000040|3d 7a 68 5f 43 4e 2e 55  54 46 2d 38 00 4c 43 5f|=zh_CN.UTF-8.LC_|
//      0x00000050|4e 41 4d 45 3d 7a 68 5f  43 4e 2e 55 54 46 2d 38|NAME=zh_CN.UTF-8|
//      0x00000060|00 4c 43 5f 4d 4f 4e 45  54 41 52 59 3d 7a 68 5f|.LC_MONETARY=zh_|
//      0x00000070|43 4e 2e 55 54 46 2d 38  00 50 57 44 3d 2f 68 6f|CN.UTF-8.PWD=/ho|
//      0x00000080|6d 65 2f 78 61 64 6d 69  6e 2f 78 77 6b 73 2e 67|me/xadmin/xwks.g|
//      0x00000090|69 74 2e 31 2f 78 61 70  70 2d 63 31 31 00 4c 4f|it.1/xapp-c11.LO|
//      0x000000a0|47 4e 41 4d 45 3d 78 61  64 ** ** ** ** ** ** **|GNAME=xad*******|
//      =============================================================================
//
//
//0x0056479e8c3890|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|
//      =============================================================================
//      0x00000000|7f 45 4c 46 02 01 01 00  00 00 00 00 00 00 00 00|.ELF............|
//      0x00000010|03 00 3e 00 01 00 00 00  a0 11 00 00 00 00 00 00|..>.............|
//      0x00000020|40 00 00 00 00 00 00 00  38 6e 00 00 00 00 00 00|@.......8n......|
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
//      0x000000d0|b0 08 00 00 00 00 00 00  b0 08 00 00 00 00 00 00|................|
//      0x000000e0|00 10 00 00 00 00 00 00  01 00 00 00 05 00 00 00|................|
//      0x000000f0|00 10 00 00 00 00 00 00  00 10 00 00 00 00 00 00|................|
//      0x00000100|00 10 00 00 00 00 00 00  81 1b 00 00 00 00 00 00|................|
//      0x00000110|81 1b 00 00 00 00 00 00  00 10 00 00 00 00 00 00|................|
//      0x00000120|01 00 00 00 04 00 00 00  00 30 00 00 00 00 00 00|.........0......|
//      0x00000130|00 30 00 00 00 00 00 00  00 30 00 00 00 00 00 00|.0.......0......|
//      0x00000140|00 08 00 00 00 00 00 00  00 08 00 00 00 00 00 00|................|
//      0x00000150|00 10 00 00 00 00 00 00  01 00 00 00 06 00 00 00|................|
//      0x00000160|68 3d 00 00 00 00 00 00  68 4d 00 00 00 00 00 00|h=......hM......|
//      0x00000170|68 4d 00 00 00 00 00 00  a8 02 00 00 00 00 00 00|hM..............|
//      0x00000180|b8 02 00 00 00 00 00 00  00 10 00 00 00 00 00 00|................|
//      0x00000190|02 00 00 00 06 00 00 00  78 3d 00 00 00 00 00 00|........x=......|
//      0x000001a0|78 4d 00 00 00 00 00 00  78 4d 00 00 00 00 00 00|xM......xM......|
//      0x000001b0|f0 01 00 00 00 00 00 00  f0 01 00 00 00 00 00 00|................|
//      0x000001c0|08 00 00 00 00 00 00 00  04 00 00 00 04 00 00 00|................|
//      0x000001d0|38 03 00 00 00 00 00 00  38 03 00 00 00 00 00 00|8.......8.......|
//      0x000001e0|38 03 00 00 00 00 00 00  20 00 00 ** ** ** ** **|8....... ..*****|
//      =============================================================================
//
//
//0x0056479e8c3890|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|
//      =============================================================================
//      0x00000000|7f 45 4c 46 02 01 01 00  00 00 00 00 00 00 00 00|.ELF............|
//      0x00000010|03 00 3e 00 01 00 00 00  a0 11 00 00 00 00 00 00|..>.............|
//      0x00000020|40 00 00 00 00 00 00 00  38 6e 00 00 00 00 00 00|@.......8n......|
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
//      0x000000d0|b0 08 00 00 00 00 00 00  b0 08 00 00 00 00 00 00|................|
//      0x000000e0|00 10 00 00 00 00 00 00  01 00 00 00 05 00 00 00|................|
//      0x000000f0|00 10 00 00 00 00 00 00  00 10 00 00 00 00 00 00|................|
//      0x00000100|00 10 00 00 00 00 00 00  81 1b 00 00 00 00 00 00|................|
//      0x00000110|81 1b 00 00 00 00 00 00  00 10 00 00 00 00 00 00|................|
//      0x00000120|01 00 00 00 04 00 00 00  00 30 00 00 00 00 00 00|.........0......|
//      0x00000130|00 30 00 00 00 00 00 00  00 30 00 00 00 00 00 00|.0.......0......|
//      0x00000140|00 08 00 00 00 00 00 00  00 08 00 00 00 00 00 00|................|
//      0x00000150|00 10 00 00 00 00 00 00  01 00 00 00 06 00 00 00|................|
//      0x00000160|68 3d 00 00 00 00 00 00  68 4d 00 00 00 00 00 00|h=......hM......|
//      0x00000170|68 4d 00 00 00 00 00 00  a8 02 00 00 00 00 00 00|hM..............|
//      0x00000180|b8 02 00 00 00 00 00 00  00 10 00 00 00 00 00 00|................|
//      0x00000190|02 00 00 00 06 00 00 00  78 3d 00 00 00 00 00 00|........x=......|
//      0x000001a0|78 4d 00 00 00 00 00 00  78 4d 00 00 00 00 00 00|xM......xM......|
//      0x000001b0|f0 01 00 00 00 00 00 00  f0 01 00 00 00 00 00 00|................|
//      0x000001c0|08 00 00 00 00 00 00 00  04 00 00 00 04 00 00 00|................|
//      0x000001d0|38 03 00 00 00 00 00 00  38 03 00 00 00 00 00 00|8.......8.......|
//      0x000001e0|38 03 00 00 00 00 00 00  20 00 00 ** ** ** ** **|8....... ..*****|
//      =============================================================================
//
//
//0x007ffe641472c0|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|
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
//0x007ffe641472c0|00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F|0123456789ABCDEF|
//      =============================================================================
//      0x00000000|60 20 20 20 31 32 33 34  2e 2e 2e 2e 5c 30 00 2e|`   1234....\0..|
//      0x00000010|37 38 39 30 7e 21 40 23  24 25 5e 26 2a 28 29 5f|7890~!@#$%^&*()_|
//      0x00000020|2b 2d 3d 71 77 65 72 74  79 75 69 6f 70 5b 5d 5c|+-=qwertyuiop[]\|
//      0x00000030|61 73 64 66 67 68 6a 6b  6c 3b 27 7a 78 63 76 62|asdfghjkl;'zxcvb|
//      0x00000040|6e 6d 2c 2e 2f 51 57 45  52 54 59 55 49 4f 50 7b|nm,./QWERTYUIOP{|
//      0x00000050|7d 7c 41 53 44 46 47 48  4a 4b 4c 3a 22 5a 58 43|}|ASDFGHJKL:"ZXC|
//      0x00000060|56 42 4e 4d 3c 3e 3f 00  ** ** ** ** ** ** ** **|VBNM<>?.********|
//      =============================================================================

>> func:main() is called. (@file:myapp - c11 - 0.1.03.c, line : 0602).
xadmin@hw : ~/ xwks.git.1 / xapp - c11$

#endif

#if 0
xadmin@hw:~/ xwks.git.1 / xapp - c11$ gcc - std = c11 - g - Wall - O0 myapp - c11 - 0.1.03.c - o myapp - lpthread
xadmin@hw : ~/ xwks.git.1 / xapp - c11$ valgrind --tool = memcheck --leak - check = full --track - origins = yes - s . / myapp


#endif
