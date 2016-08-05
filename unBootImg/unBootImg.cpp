// unBootImg.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#define BOOT_MAGIC "ANDROID!"
#define BOOT_MAGIC_SIZE 8
#define BOOT_NAME_SIZE 16
#define BOOT_ARGS_SIZE 512

typedef struct 
{
    unsigned char magic[BOOT_MAGIC_SIZE];
	
    unsigned kernel_size;  /* size in bytes */
    unsigned kernel_addr;  /* physical load addr */
	
    unsigned ramdisk_size; /* size in bytes */
    unsigned ramdisk_addr; /* physical load addr */
	
    unsigned second_size;  /* size in bytes */
    unsigned second_addr;  /* physical load addr */
	
    unsigned tags_addr;    /* physical addr for kernel tags */
    unsigned page_size;    /* flash page size we assume */
    unsigned unused[2];    /* future expansion: should be 0 */
	
    unsigned char name[BOOT_NAME_SIZE]; /* asciiz product name */
    
    unsigned char cmdline[BOOT_ARGS_SIZE];
	
    unsigned id[8]; /* timestamp / checksum / sha1 / etc */
}boot_img_hdr,*pboot_img_hdr;
BOOL SaveRamdisk(FILE *pFile);
BOOL SaveKernel(FILE *pFile);
BOOL SaveResource(FILE *pFile);
//only unpack ramdisk.img: unBootImg -r boot.img
//only unpack kernel.img: unBootImg -k  boot.img
//only unpack ramdisk.img and kernel.img: unBootImg -rk boot.img 
int main(int argc, char* argv[])
{
	char szBootPath[MAX_PATH];
	char szBootTag[BOOT_MAGIC_SIZE+1];
	FILE *pFile=NULL;
	DWORD dwRet,dwRead;
	printf("unBootimg start...\r\n");
	if (argc>=3)
	{
		strcpy(szBootPath,argv[2]);
	}
	else
		strcpy(szBootPath,"boot.img");
	dwRet = GetFileAttributes(szBootPath);
	if (dwRet==0xFFFFFFFF)
	{
		printf("Boot file is not existed.\r\n");
		return -1;
	}
	pFile = fopen(szBootPath,"rb");
	if (!pFile)
	{
		printf("Open boot file failed.\r\n");
		return -2;
	}
	dwRead = fread(szBootTag,1,BOOT_MAGIC_SIZE,pFile);
	if (dwRead!=BOOT_MAGIC_SIZE)
	{
		fclose(pFile);
		printf("Read boot tag failed.\r\n");
		return -3;
	}
	szBootTag[BOOT_MAGIC_SIZE] = 0;
	if (strcmp(szBootTag,BOOT_MAGIC)!=0)
	{
		fclose(pFile);
		printf("Check boot tag failed.\r\n");
		return -4;
	}

	if (!stricmp(argv[1],"-r"))
	{
		if (!SaveRamdisk(pFile))
		{
			fclose(pFile);
			printf("Save ramdisk failed.\r\n");
			return -5;
		}
	}
	else if (!stricmp(argv[1],"-k"))
	{
		if (!SaveKernel(pFile))
		{
			fclose(pFile);
			printf("Save kernel failed.\r\n");
			return -6;
		}
	}
	else if (!stricmp(argv[1],"-s"))
	{
		if (!SaveResource(pFile))
		{
			fclose(pFile);
			printf("Save kernel failed.\r\n");
			return -6;
		}
	}
	else if (!stricmp(argv[1],"-rk"))
	{
		if (!SaveRamdisk(pFile))
		{
			fclose(pFile);
			printf("Save ramdisk failed.\r\n");
			return -7;
		}
		if (!SaveKernel(pFile))
		{
			fclose(pFile);
			printf("Save kernel failed.\r\n");
			return -8;
		}
	}
	else
	{
		fclose(pFile);
		printf("Invalid parameter.\r\n");
		return -2;
	}
	fclose(pFile);
	printf("unBootimg success.\r\n");
	return 0;
}
BOOL SaveRamdisk(FILE *pFile)
{
	boot_img_hdr head;
	DWORD dwHeadSize,dwRet,dwBufferSize,dwOffsetSector,dwRemain,dwWriteOnce;
	BYTE buffer[16*1024];
	FILE *pDest=NULL;
	dwHeadSize = sizeof(boot_img_hdr);
	dwBufferSize = 16*1024;
	fseek(pFile,0,SEEK_SET);
	dwRet = fread(&head,1,dwHeadSize,pFile);
	if (dwRet!=dwHeadSize)
	{
		printf("Read head struct data failed.\r\n");
		return FALSE;
	}
	dwOffsetSector = (head.kernel_size+head.page_size-1)/head.page_size;
	dwOffsetSector++;//skip head
	
	fseek(pFile,dwOffsetSector*head.page_size,SEEK_SET);

	dwRemain = head.ramdisk_size;
	pDest = fopen("ramdisk.img","wb+");
	if (!pDest)
	{
		printf("Open ramdisk.img failed.\r\n");
		return FALSE;
	}

	while(dwRemain>0)
	{
		dwWriteOnce = (dwRemain>=dwBufferSize) ? dwBufferSize:dwRemain ;
		dwRet = fread(buffer,1,dwWriteOnce,pFile);
		if (dwRet!=dwWriteOnce)
		{
			fclose(pDest);
			printf("Read ramdisk.img data failed.\r\n");
			return FALSE;
		}
		dwRet = fwrite(buffer,1,dwWriteOnce,pDest);
		if (dwRet!=dwWriteOnce)
		{
			fclose(pDest);
			printf("Write ramdisk.img data failed.\r\n");
			return FALSE;
		}
		dwRemain -= dwWriteOnce;
	}
	fclose(pDest);

	return TRUE;
}
BOOL SaveKernel(FILE *pFile)
{
	boot_img_hdr head;
	DWORD dwHeadSize,dwRet,dwBufferSize,dwOffsetSector,dwRemain,dwWriteOnce;
	BYTE buffer[16*1024];
	FILE *pDest=NULL;
	dwHeadSize = sizeof(boot_img_hdr);
	dwBufferSize = 16*1024;
	fseek(pFile,0,SEEK_SET);
	dwRet = fread(&head,1,dwHeadSize,pFile);
	if (dwRet!=dwHeadSize)
	{
		printf("Read head struct data failed.\r\n");
		return FALSE;
	}
	dwOffsetSector = 1;
	
	fseek(pFile,dwOffsetSector*head.page_size,SEEK_SET);
	
	dwRemain = head.kernel_size;
	pDest = fopen("kernel.img","wb+");
	if (!pDest)
	{
		printf("Open kernel.img failed.\r\n");
		return FALSE;
	}
	
	while(dwRemain>0)
	{
		dwWriteOnce = (dwRemain>=dwBufferSize) ? dwBufferSize:dwRemain ;
		dwRet = fread(buffer,1,dwWriteOnce,pFile);
		if (dwRet!=dwWriteOnce)
		{
			fclose(pDest);
			printf("Read kernel.img data failed.\r\n");
			return FALSE;
		}
		dwRet = fwrite(buffer,1,dwWriteOnce,pDest);
		if (dwRet!=dwWriteOnce)
		{
			fclose(pDest);
			printf("Write kernel.img data failed.\r\n");
			return FALSE;
		}
		dwRemain -= dwWriteOnce;
	}
	fclose(pDest);
	
	return TRUE;
}

BOOL SaveResource(FILE *pFile)
{
	boot_img_hdr head;
	DWORD dwHeadSize,dwRet,dwBufferSize,dwOffsetSector,dwRemain,dwWriteOnce;
	BYTE buffer[16*1024];
	FILE *pDest=NULL;
	dwHeadSize = sizeof(boot_img_hdr);
	dwBufferSize = 16*1024;
	fseek(pFile,0,SEEK_SET);
	dwRet = fread(&head,1,dwHeadSize,pFile);
	if (dwRet!=dwHeadSize)
	{
		printf("Read head struct data failed.\r\n");
		return FALSE;
	}
	dwOffsetSector = (head.ramdisk_size+head.kernel_size+head.page_size-1)/head.page_size;
	dwOffsetSector++;//skip head
	
	fseek(pFile,dwOffsetSector*head.page_size,SEEK_SET);
	
	dwRemain = head.second_size;
	pDest = fopen("resource.img","wb+");
	if (!pDest)
	{
		printf("Open resource.img failed.\r\n");
		return FALSE;
	}
	
	while(dwRemain>0)
	{
		dwWriteOnce = (dwRemain>=dwBufferSize) ? dwBufferSize:dwRemain ;
		dwRet = fread(buffer,1,dwWriteOnce,pFile);
		if (dwRet!=dwWriteOnce)
		{
			fclose(pDest);
			printf("Read resource.img data failed.\r\n");
			return FALSE;
		}
		dwRet = fwrite(buffer,1,dwWriteOnce,pDest);
		if (dwRet!=dwWriteOnce)
		{
			fclose(pDest);
			printf("Write resource.img data failed.\r\n");
			return FALSE;
		}
		dwRemain -= dwWriteOnce;
	}
	fclose(pDest);
	
	return TRUE;
}