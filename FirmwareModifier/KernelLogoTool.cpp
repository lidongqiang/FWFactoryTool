// KernelLogoTool.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include "KernelLogoTool.h"

#include <iostream>
#include <string>
#include <shlwapi.h> 
#include "ximage.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment(lib,"shlwapi.lib")


/////////////////////////////////////////////////////////////////////////////
// The one and only application object

//CWinApp theApp;

/*
kernelLogoTool -i picIndex kernel.img logo.ppm   #import logo.ppm to kernel
kernelLogoTool -e picIndex kernel.img logo.ppm   #export logo.ppm from kernel
*/
/*
void usage()
{
	cout <<_T("---------------------Function Lists---------------------")<<endl;
	cout <<_T("kernelLogoTool -i [picIndex] kernel.img logo.ppm   #import logo.ppm to kernel")<<endl;
	cout <<_T("kernelLogoTool -e [picIndex] kernel.img logo.ppm   #export logo.ppm from kernel")<<endl;
	cout <<_T("The parameter in [] is optional,Not input picIndex means that operating first logo in kernel.")<<endl;
	cout <<_T("--------------------------------------------------------")<<endl;
	
}*/
//int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
#define MAX_LINUX_LOGO_COLORS	224
#define MAX_KERNEL_LOGO_NUM   20
#define Endian16_Swap(value)	(((((unsigned short)value)<< 8) & 0xFF00)|\
								((((unsigned short)value) >> 8) & 0x00FF))
#pragma pack(1)
struct color {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};
enum enum_logo_type{
	em_logo_pnm=0,
	em_logo_bmp
};

enum enum_resource_type{
	em_kernel_type=1,
	em_logo_type,
	em_logo_kernel_type
};

struct color logo_clut[MAX_LINUX_LOGO_COLORS];


struct picture {
	unsigned long dataAddr;
	unsigned long clutAddr;
	unsigned int width;
	unsigned int height;
	unsigned char clutsize;
	unsigned int lcd_max_width;
	unsigned int lcd_max_height;
	enum_logo_type logo_type;
};
struct picture k_Picture[MAX_KERNEL_LOGO_NUM];


BYTE bmphead[54]={
	0x42, 0x4D, 0x36, 0x94, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00,
	0x00, 0x00, 0x20, 0x03, 0x00, 0x00, 0xE0, 0x01, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x94, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

typedef struct _RKRGBQUAD {
	BYTE    rgbBlue;
	BYTE    rgbGreen;
	BYTE    rgbRed;
	BYTE    rgbReserved;
} RKRGBQUAD,*PRKRGBQUAD;

typedef struct _BMPFILEHEADER
{
	WORD	bfType;            		// 文件类型, 必须为 "BM" (0x4D42) 
	DWORD	bfSize;            		// 文件的大小(字节) 
	WORD	bfReserved1;       		// 保留, 必须为 0 
	WORD	bfReserved2;       		// 保留, 必须为 0 
	DWORD	bfOffBits;         		// 位图阵列相对于文件头的偏移量(字节) 
} BMPFILEHEADER,*PBMPFILEHEADER;					// 文件头结构 14bytes

typedef struct _BMPFILEINFOHEADER
{
	DWORD	biSize;               		// size of BITMAPINFOHEADER 
	long	biWidth;              		// 位图宽度(像素) 
	long	biHeight;            		 // 位图高度(像素) 
	WORD	biPlanes;            		 // 目标设备的位平面数, 必须置为1 
	WORD	biBitCount;					// 每个像素的位数, 1,4,8或24 
	DWORD	biCompression;				 // 位图阵列的压缩方法,0=不压缩 
	DWORD	biSizeImage;				 // 图像大小(字节) 
	long	biXPelsPerMeter;   			// 目标设备水平每米像素个数 
	long	biYPelsPerMeter;  			 // 目标设备垂直每米像素个数 
	DWORD	biClrUsed;         			// 位图实际使用的颜色表的颜色数 
	DWORD	biClrImportant;    			// 重要颜色索引的个数 
} BMPFILEINFOHEADER,*PBMPFILEINFOHEADER;						// 位图信息头结构 

#define MAX_INDEX_ENTRY_PATH_LEN    256
#define BLOCK_SIZE                  512

typedef struct resource_ptn_header{
	BYTE     magic[4];//tag, "RSCE"
	WORD resource_ptn_version;
	WORD index_tbl_version;
	BYTE  header_size;//blocks, size of ptn header.
	BYTE  tbl_offset;//blocks, offset of index table.
	BYTE  tbl_entry_size;//blocks, size of index table's entry.
	UINT tbl_entry_num;//numbers of index table's entry.
	BYTE reserve[497];
} RESOURCEPTNHEADER,*PRESOURCEPTNHEADER;

typedef struct index_tbl_entry {
	BYTE     tag[4];//tag, "ENTR"
	BYTE     path[MAX_INDEX_ENTRY_PATH_LEN];
	UINT content_offset;//blocks, offset of resource content.
	UINT content_size;//bytes, size of resource content.
	BYTE reserve[244];
} INDEX_TBL_ENTRY,*PINDEX_TBL_ENTRY;

typedef struct _BMPINFO
{ 
	BMPFILEINFOHEADER bmiHeader; 
	RKRGBQUAD	bmiColors[1]; 
} BMPINFO,*PBMPINFO;
#define ABS(x)		((x<0) ? -x : x)
#pragma pack()

extern ULONG CRC_32(PBYTE pData, ULONG ulSize);
BOOL CheckKernel(BYTE *pKernel);
UINT SearchLogo(BYTE *pKernel);
BOOL SearchSubStr(PBYTE &pStartPos,PBYTE pEndPos,PCHAR pSubStr);
BOOL ExportLogo(BYTE *pKernel,int logoIndex,CString strLogoFile);
BOOL ImportLogo(BYTE *pKernel,int logoIndex,struct color *pLogo,int logoWidth,int logoHeight);
BOOL ParseLogo(CString strLogoFile,struct color* &pLogoData,int &logoWidth,int &logoHeight);
int get_number(FILE *fp);
int get_number255(FILE *fp,int maxVal);
int IsColorEqual(struct color c1, struct color c2);
BOOL SaveKernel(BYTE *pKernel,UINT uiKernelSize,CString strKernelPath);
/////////////////////////////////////////////////////////////////
BOOL FolderExist(CString strPath)
{
     WIN32_FIND_DATA   wfd;
     BOOL rValue = FALSE;
     HANDLE hFind = FindFirstFile(strPath, &wfd);
    if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
              rValue = TRUE;   
     }
     FindClose(hFind);
    return rValue;
}
BOOL CreateFolder(CString strPath)
{
     SECURITY_ATTRIBUTES attrib;
     attrib.bInheritHandle = FALSE;
     attrib.lpSecurityDescriptor = NULL;
     attrib.nLength =sizeof(SECURITY_ATTRIBUTES);
    //上面定义的属性可以省略。 直接return ::CreateDirectory( path, NULL); 即可
     return ::CreateDirectory( strPath, &attrib);
} 
//删除文件夹以及包含的所有文件，参数str要创建的文件夹绝对路径
BOOL DeleteFolder(CString str)
{
	BOOL ret= false;
        CFileFind find;
        CString fdName=str;
        if(fdName.Right(1)!="\\")
                fdName+="\\*.*";
        else
                fdName+="*.*";

        BOOL isFind;
        isFind=find.FindFile(fdName);
        while(isFind)
        {
                isFind=find.FindNextFile();
                if (!find.IsDots())
                {
                        if(!find.IsDirectory())
                                ret = DeleteFile(find.GetFilePath());//删除包含的文件
                        else
                        {
                                ret = DeleteFolder(find.GetFilePath());//如果是文件夹，递归
                        }
                }
        }
        find.Close();
        RemoveDirectory(str);//删除该文件夹
		return ret;
}

//删除目录下的文件
void DeleteFolderFile(CString str)
{
        CFileFind find;
        CString fdName=str;
        if(fdName.Right(1)!="\\")
                fdName+="\\*.*";
        else
                fdName+="*.*";

        BOOL isFind;
        isFind=find.FindFile(fdName);
        while(isFind)
        {
                isFind=find.FindNextFile();
                if (!find.IsDots())
                {
                        if(!find.IsDirectory())
                                DeleteFile(find.GetFilePath());//删除包含的文件
                        else
                        {
                                DeleteFolder(find.GetFilePath());//如果是文件夹，递归
                        }
                }
        }
        find.Close();
}

int kernel_modify_logo(CString KerenlPath,CString LogoPath)
{

	int nRetCode = 0;
	CString strKernelPath,strLogoPath;
	CString strOperate;
	int picIndex;
	BOOL bImportFlag;

		//strOperate.Format(_T("%s"),argv[1]);
		bImportFlag = TRUE;
		picIndex = 0;
		strKernelPath = KerenlPath;//.Format(_T("%s"),_T("kernel.img"));
		strLogoPath.Format(_T("%s"),LogoPath);

		if (bImportFlag)
		{
			if (!PathFileExists(strLogoPath))
			{
				cout<<_T("Error:")<<(LPCTSTR)strLogoPath<<_T(" is not existed!")<<endl;
				return -6;
			}
		}

	
	//1.check kernel file
	CFile kernelFile;
	UINT uiKernelSize;
	BOOL bRet;
	bRet = kernelFile.Open(strKernelPath,CFile::modeRead|CFile::typeBinary);
	if (!bRet)
	{
		cout<<_T("Error:open ")<<(LPCTSTR)strKernelPath<<_T(" failed!")<<endl;
		return -7;
	}
	uiKernelSize = kernelFile.GetLength();
	BYTE *pKernelBuffer=NULL;
	UINT uiRead;
	pKernelBuffer = new BYTE[uiKernelSize];
	if (!pKernelBuffer)
	{
		kernelFile.Close();
		cout<<_T("Error:new kernel memory failed!")<<endl;
		nRetCode = -8;
		goto Exit_Main;
	}
	uiRead = kernelFile.ReadHuge(pKernelBuffer,uiKernelSize);
	kernelFile.Close();
	if (uiRead!=uiKernelSize)
	{
		cout<<_T("Error:read ")<<(LPCTSTR)strKernelPath<<_T(" failed!")<<endl;
		nRetCode = -9;
		goto Exit_Main;
	}
	bRet = CheckKernel(pKernelBuffer);
	if (!bRet)
	{
		cout<<_T("Error:check kernel failed!")<<endl;
		nRetCode = -10;
		goto Exit_Main;
	}
	//2.search logo
	UINT uiLogoNumber;
	uiLogoNumber = SearchLogo(pKernelBuffer);
	if (uiLogoNumber==0)
	{
		nRetCode = -11;
		goto Exit_Main;
	}
	if (picIndex>=uiLogoNumber)
	{
		cout<<_T("Error:picIndex is large than logo amount!")<<endl;
		nRetCode = -12;
		goto Exit_Main;
	}
	if (!bImportFlag)
	{//export logo
		bRet = ExportLogo(pKernelBuffer,picIndex,strLogoPath);
		if (!bRet)
		{
			cout<<_T("Error:export logo failed!")<<endl;
			nRetCode = -13;
			goto Exit_Main;
		}
		cout<<_T("Export logo success.")<<endl;
	}
	else
	{//import logo
		struct color *pLogoData=NULL;
		int logoWidth,logoHeight;
		bRet = ParseLogo(strLogoPath,pLogoData,logoWidth,logoHeight);
		if (!bRet)
		{
			cout<<_T("Error:parse logo failed!")<<endl;
			nRetCode = -14;
			goto Exit_Main;
		}
		bRet = ImportLogo(pKernelBuffer,picIndex,pLogoData,logoWidth,logoHeight);
		if (pLogoData)
		{
			delete []pLogoData;
			pLogoData = NULL;
		}
		if (!bRet)
		{
			cout<<_T("Error:import logo failed!")<<endl;
			nRetCode = -15;
			goto Exit_Main;
		}
		bRet = SaveKernel(pKernelBuffer,uiKernelSize,strKernelPath);
		if (!bRet)
		{
			cout<<_T("Error:save new kernel failed!")<<endl;
			nRetCode = -16;
			goto Exit_Main;
		}
		cout<<_T("Import logo success.")<<endl;
	}

Exit_Main:
	if (pKernelBuffer)
	{
		delete pKernelBuffer;
		pKernelBuffer = NULL;
	}

	

	return nRetCode;
}
BOOL CheckKernel(BYTE *pKernel)
{
	DWORD *pKernelTag,*pCrcSize,*pOldCrc;
	DWORD dwNewCrc;
	pKernelTag = (DWORD *)pKernel;
	pCrcSize = (DWORD *)(pKernel+4);
	pOldCrc = (DWORD *)(pKernel+8+*pCrcSize);
	
	if (*pKernelTag!=0x4C4E524B)
	{
		return FALSE;
	}
	dwNewCrc = CRC_32(pKernel+8,*pCrcSize);
	if (dwNewCrc!=*pOldCrc)
	{
		return FALSE;
	}
	return TRUE;
	
}

int SearchTag(BYTE *pKernel)
{
    int iRet=-1;
    char tag_str[] = "ROCK_battery";
    PBYTE pStartPos,pEndPos;
    DWORD *pSize;
    pSize = (DWORD *)(pKernel+4);
    pStartPos = pKernel+8;
    pEndPos = pStartPos+*pSize;
    
    //search tag
    if (SearchSubStr(pStartPos,pEndPos,tag_str))
    {
        iRet = pStartPos - pKernel;
    }
    return iRet;
}

UINT SearchLogo(BYTE *pKernel)
{
	UINT uiRet=0;
	char i, j;
	char data_str[] = "logo_RKlogo_data";
	char clut_str[] = "logo_RKlogo_clut";
	PBYTE pStartPos,pEndPos;
	DWORD *pSize;
	pSize = (DWORD *)(pKernel+4);
	pStartPos = pKernel+8;
	pEndPos = pStartPos+*pSize;

	//search clut
	for (j = 0; j < MAX_KERNEL_LOGO_NUM; j++)
	{
		if (SearchSubStr(pStartPos,pEndPos,clut_str))
		{
			k_Picture[j].clutsize = *((PCHAR)pStartPos);
			pStartPos++;
			k_Picture[j].clutAddr = pStartPos-pKernel;
		}
		else
		{
			break;
		}
	}
	if (j == 0)
	{
		return uiRet;
	}
	
	pStartPos = pKernel+8;
	
	for (i = 0; i < MAX_KERNEL_LOGO_NUM; i++)
	{
		if (SearchSubStr(pStartPos,pEndPos,data_str))
		{
			pStartPos -= 20;
			
			k_Picture[i].width = Endian16_Swap(*((PUSHORT)pStartPos));
			pStartPos += 2;
			k_Picture[i].height = Endian16_Swap(*((PUSHORT)pStartPos));
			pStartPos += 2;
			
			pStartPos += 16;		
			k_Picture[i].lcd_max_width = Endian16_Swap(*((PUSHORT)pStartPos));
			pStartPos += 2;
			k_Picture[i].lcd_max_height = Endian16_Swap(*((PUSHORT)pStartPos));
			pStartPos += 2;
			k_Picture[i].dataAddr = pStartPos-pKernel;
		}
		else
		{
			break;
		}
	}
	if (i == 0)
	{
		return uiRet;
	}
	if (i!=j)
	{
		return uiRet;
	}
	uiRet = i;
	
	return uiRet;
}

BOOL SearchSubStr(PBYTE &pStartPos,PBYTE pEndPos,PCHAR pSubStr)
{
	char *p = pSubStr;
	char c = *((PCHAR)pStartPos);
	
	while(pStartPos < pEndPos)
	{
		if (c == *p)
		{
			while(c == *p && *p != '\0')
			{
				pStartPos++;
				c = *((PCHAR)pStartPos);
				p++;
			}
			if (*p == '\0')
			{
				return TRUE;
			}
		}
		else
		{
			pStartPos++;
			c = *((PCHAR)pStartPos);
		}
		
		p = pSubStr;
	}
	return FALSE;
}

BOOL ExportBmpLogo(BYTE *pKernel,int logoIndex,CString strLogoFile,int ChipType)
{
	PBYTE pLogoData = pKernel + k_Picture[logoIndex].dataAddr;
	CFile	Bmpfile;
	BOOL bRet;
    BMPFILEHEADER BmpFileHead;
    BMPFILEINFOHEADER BmpInfoHead;
	int i,j,k,n;

	//由于31也采用了2928生成logo的方法，所以去掉这部分
	int	number;
    BYTE tmp;
	if(ChipType==RK30_BOARD)
	{
		for (i = k_Picture[logoIndex].height; i>0; i--)
		{	
			for (j = 0;j<k_Picture[logoIndex].width ; j++) 
			{
				number=4*(i-1)*k_Picture[logoIndex].width+4*j;
				tmp=*(pLogoData+number+2);
				*(pLogoData+number+2)=*(pLogoData+number);
				*(pLogoData+number) = tmp;				
			}
		}
	}
	
	bRet = Bmpfile.Open(strLogoFile,CFile::typeBinary|CFile::modeCreate|CFile::modeReadWrite);
	if (!bRet)
	{
		cout<<_T("Error:create ")<<(LPCTSTR)strLogoFile<<_T(" failed!")<<endl;
		return FALSE;
	}

	//write bmp info header
	BmpInfoHead.biSize = sizeof(BMPFILEINFOHEADER);
	BmpInfoHead.biWidth = k_Picture[logoIndex].width;
	BmpInfoHead.biHeight = k_Picture[logoIndex].height;
	BmpInfoHead.biPlanes = 1;
	
	BmpInfoHead.biBitCount = 24;
	BmpInfoHead.biCompression = 0;//BI_BITFIELDS;
	BmpInfoHead.biSizeImage = BmpInfoHead.biWidth * ABS(BmpInfoHead.biHeight) * 3;
	BmpInfoHead.biXPelsPerMeter = 0;
	BmpInfoHead.biYPelsPerMeter = 0;
	BmpInfoHead.biClrUsed = 0;
	BmpInfoHead.biClrImportant = 0;

	//write bmp file header
	BmpFileHead.bfType = 0x4d42;		//'BM'
	BmpFileHead.bfSize = sizeof(BMPFILEHEADER)+BmpInfoHead.biSize+BmpInfoHead.biSizeImage ;
	BmpFileHead.bfReserved1 = 0;
	BmpFileHead.bfReserved2 = 0;
	BmpFileHead.bfOffBits = sizeof(BMPFILEHEADER)+BmpInfoHead.biSize;
    
	PBYTE pBmpdata=new BYTE[BmpInfoHead.biSizeImage];
	PBYTE pStart;
	PBYTE pKernelStart;
	DWORD maxlcdwidth=k_Picture[logoIndex].lcd_max_width;	
	DWORD maxlcdheight=k_Picture[logoIndex].lcd_max_height;
	n=0;

	for(i=BmpInfoHead.biHeight-1;i>=0;i--)
	{
		pKernelStart=pLogoData+maxlcdwidth*n*4;
		n++;
		pStart=pBmpdata+BmpInfoHead.biWidth*i*3;
		for(j=0;j<BmpInfoHead.biWidth;j++)
		{
			for(k=0;k<3;k++)
			{
				*pStart=*pKernelStart;
				pKernelStart++;
				pStart++;
			}		
			pKernelStart++;
		}
	}

	try
	{
		
		Bmpfile.Write(&BmpFileHead,sizeof(BMPFILEHEADER));
		Bmpfile.Write(&BmpInfoHead,sizeof(BMPFILEINFOHEADER));
		
		//rgb888标记
		//bmpdata_buffer[0] = 0xFF;
		//bmpdata_buffer[1] = 0xFF00;
		//bmpdata_buffer[2] = 0xFF0000;
		
		//Bmpfile.Write(bmpdata_buffer,12);
/*
		for(i=0;i<BmpInfoHead.biHeight;i++)
			for(j=0;j<BmpInfoHead.biWidth;j++)
			{			
				Bmpfile.Write(pLogoData,sizeof(BYTE));
				pLogoData++;
				Bmpfile.Write(pLogoData,sizeof(BYTE));
				pLogoData++;
				Bmpfile.Write(pLogoData,sizeof(BYTE));
				pLogoData+=2;
			
			}
*/
		Bmpfile.WriteHuge(pBmpdata,BmpInfoHead.biSizeImage);
		Bmpfile.Close();
	}
	catch (...)
	{
		Bmpfile.Close();
		cout<<_T("Error:write ")<<(LPCTSTR)strLogoFile<<_T(" occur exception!")<<endl;
		return FALSE;
	}

	delete pBmpdata;

	return TRUE;
}
BOOL ExportLogo(BYTE *pKernel,int logoIndex,CString kernelLogopath)
{
	int type;
	int i,j;
	CString picturefile;
	picturefile=kernelLogopath;
	CString ext = picturefile.Right(picturefile.GetLength()-picturefile.ReverseFind('.') - 1);
	if (ext == "bmp" || ext == "BMP")
		type = CXIMAGE_FORMAT_BMP;
	else if (ext == "pnm" || ext == "pgm" || ext == "ppm" || ext == "PNM" || ext == "PGM" || ext == "PPM")
		type = CXIMAGE_FORMAT_PNM;
	else
		type = CXIMAGE_FORMAT_UNKNOWN;
	
	if(type==CXIMAGE_FORMAT_BMP)
	{
		FILE *pFile=NULL;
		pFile = _tfopen(picturefile,_T("wb"));
		if (!pFile)
		{
			//MessageBox(_T("Error:create ")+picturefile+_T(" failed!"));
			return FALSE;
		}

		struct color *pLogoClut = (struct color *)(pKernel+k_Picture[logoIndex].clutAddr);
		PBYTE pLogoData = pKernel + k_Picture[logoIndex].dataAddr;

        for(i=0;i<54;i++)
			fprintf(pFile,"%c",bmphead[i]);
		int number=0;
		for (i = k_Picture[logoIndex].height; i>0; i--)
		{	
			for (j = 0;j<k_Picture[logoIndex].width ; j++) 
			{
				number=4*(i-1)*k_Picture[logoIndex].width+4*j;
				fprintf(pFile,"%c",*(pLogoData+number+2));
				fprintf(pFile,"%c",*(pLogoData+number+1));
			    fprintf(pFile,"%c",*(pLogoData+number));			
			}
		}
		
		fclose(pFile);
	}
	else if(type = CXIMAGE_FORMAT_PNM)
	{	
		if (k_Picture[logoIndex].clutsize>MAX_LINUX_LOGO_COLORS)
		{
			return FALSE;
		}
		FILE *pFile=NULL;
		pFile = _tfopen(picturefile,_T("wb"));
		if (!pFile)
		{
			return FALSE;
		}
		struct color *pLogoClut = (struct color *)(pKernel+k_Picture[logoIndex].clutAddr);
		PBYTE pLogoData = pKernel + k_Picture[logoIndex].dataAddr;


		fputs("P3\n", pFile);
		fprintf(pFile, "%d %d\n", k_Picture[logoIndex].width, k_Picture[logoIndex].height);
		fputs("255\n", pFile);
		
		UINT uiLoopCount=0;
		int i,j;
		BYTE clutValue;
		for (i = 0; i < k_Picture[logoIndex].height; i++)
		{
			for (j = 0; j < k_Picture[logoIndex].width; j++) 
			{
				clutValue = *((PCHAR)pLogoData);
				pLogoData++;
				
				fprintf(pFile, "  %d %d %d", pLogoClut[clutValue - 32].red, pLogoClut[clutValue - 32].green, pLogoClut[clutValue - 32].blue);
				
				uiLoopCount++;
				if (uiLoopCount % 6 == 0)
				{
					fputs("\n",pFile);
					uiLoopCount = 0;
				}				
			}
		}
		fclose(pFile);
	}
	else
	{
		cout<<_T("Error:invalid picture type")<<endl;
	}
	return TRUE;	

}
int get_number(FILE *fp)
{
    int c, val=-1;
	CString str;
	
    /* Skip leading whitespace */
	do {
		c = fgetc(fp);
		if (c == EOF)
		{
			return val;
		}
		if (c == '#') {
			/* Ignore comments 'till end of line */
			do {
				c = fgetc(fp);
				if (c == EOF)
				{
					return val;
				}
			} while (c != '\n');
		}
	} while (isspace(c));
	
	/* Parse decimal number */
	val = 0;
	while (isdigit(c)) {
		val = 10*val+c-'0';
		c = fgetc(fp);
		if (c == EOF)
		{
			break;
		}
	}
	return val;
}
int get_number255(FILE *fp,int maxVal)
{
	int val;
	val = get_number(fp);
	if ((val==-1)||(val>maxVal))
	{
		cout<<_T("Error:invalid data=")<<val<<_T(",max=")<<maxVal<<endl;
		return -1;
	}
//	val = (255*val+maxVal/2)/maxVal;
	val = val*100;
	val = 255*(val/maxVal);
	val = val /100;
	return val;
}

BOOL ParseLogo(CString strLogoFile,struct color* &pLogoData,int &logoWidth,int &logoHeight)
{
	FILE *fp=NULL;
	BOOL bRet=FALSE;
    int i, j;
    int magic;
    int maxval,val;
	PBYTE pSrcData=NULL;
	UINT uiuRead,uiSrcDataSize,uiLineBytes;
    /* open image file */
    fp = _tfopen(strLogoFile, _T("rb"));
	if (!fp) 
	{
		cout<<_T("Error:open ")<<strLogoFile<<_T(" failed!")<<endl;
		return FALSE;
	}

    /* check file type and read file header */
    magic = fgetc(fp);
	if (magic != 'P') 
	{
		cout<<_T("Error:invalid logo magic!")<<endl;
		goto Exit_ParseLogo;
	}
    magic = fgetc(fp);
    switch (magic) 
	{
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
		break;
	default:
		cout<<_T("Error:invalid logo format!")<<endl;
		goto Exit_ParseLogo;
    }

	logoWidth = get_number(fp);
	if (logoWidth==-1)
	{
		cout<<_T("Error:invalid logo width!")<<endl;
		goto Exit_ParseLogo;
	}
	logoHeight = get_number(fp);
	if (logoHeight==-1)
	{
		cout<<_T("Error:invalid logo height!")<<endl;
		goto Exit_ParseLogo;
	}

    /* allocate image data */
	pLogoData = NULL;
	pLogoData = new struct color[logoHeight*logoWidth];
	
	if (!pLogoData) 
	{
		cout<<_T("Error:new logo data failed!")<<endl;
		goto Exit_ParseLogo;
	}

	
    /* read image data */
	switch (magic) {
	case '1':
		/* Plain PBM */
		for (i = 0; i < logoHeight; i++)
			for (j = 0; j < logoWidth; j++)
			{
				val = get_number(fp);
				if ((val!=0)&&(val!=1))
				{
					cout<<_T("Error:(Plain PBM)invalid data=")<<val<<endl;
					goto Exit_ParseLogo;
				}
				
				pLogoData[i*logoWidth+j].red = pLogoData[i*logoWidth+j].green = pLogoData[i*logoWidth+j].blue = 255*(1-val);
			}
		break;

	case '2':
		/* Plain PGM */
		maxval = get_number(fp);
		if (maxval==-1)
		{
			cout<<_T("Error:invalid maximum color value!")<<endl;
			goto Exit_ParseLogo;
		}
		if (maxval>255)
		{
			cout<<_T("Error:maximum color value is large than 255!")<<endl;
			goto Exit_ParseLogo;
		}
		for (i = 0; i < logoHeight; i++)
			for (j = 0; j < logoWidth; j++)
			{
				val = get_number255(fp,maxval);
				if (val==-1)
				{
					cout<<_T("Error:(Plain PGM)invalid data!")<<endl;
					goto Exit_ParseLogo;
				}
				pLogoData[i*logoWidth+j].red = pLogoData[i*logoWidth+j].green = pLogoData[i*logoWidth+j].blue = val;
			}
		break;

	case '3':
		/* Plain PPM */
	    maxval = get_number(fp);
		if (maxval==-1)
		{
			cout<<_T("Error:invalid maximum color value!")<<endl;
			goto Exit_ParseLogo;
		}
		if (maxval>255)
		{
			cout<<_T("Error:maximum color value is large than 255!")<<endl;
			goto Exit_ParseLogo;
		}
		for (i = 0; i < logoHeight; i++) {
			for (j = 0; j < logoWidth; j++)
			{
				val = get_number255(fp,maxval);
				if (val==-1)
				{
					cout<<_T("Error:(Plain PPM)invalid data!")<<endl;
					goto Exit_ParseLogo;
				}
				pLogoData[i*logoWidth+j].red = val;
				val = get_number255(fp,maxval);
				if (val==-1)
				{
					cout<<_T("Error:(Plain PPM)invalid data!")<<endl;
					goto Exit_ParseLogo;
				}
				pLogoData[i*logoWidth+j].green = val;
				val = get_number255(fp,maxval);
				if (val==-1)
				{
					cout<<_T("Error:(Plain PPM)invalid data!")<<endl;
					goto Exit_ParseLogo;
				}
				pLogoData[i*logoWidth+j].blue = val;
			}
		}
		break;
	case '4':
		/* Bin PBM */
		uiLineBytes = (logoWidth-1)/8+1;
		uiSrcDataSize = logoHeight*uiLineBytes;
		pSrcData = new BYTE[uiSrcDataSize];
		if (!pSrcData)
		{
			cout<<_T("Error:(Bin PBM)new data memory failed!")<<endl;
			goto Exit_ParseLogo;
		}
		uiuRead = fread(pSrcData,1,uiSrcDataSize,fp);
		if (uiuRead!=uiSrcDataSize)
		{
			cout<<_T("Error:(Bin PBM)read data failed,total=")<<uiSrcDataSize<<_T(",read=")<<uiuRead<<endl;
			goto Exit_ParseLogo;
		}
		for (i = 0; i < logoHeight; i++)
			for (j = 0; j < logoWidth; j++)
			{
				val = pSrcData[i*uiLineBytes+j/8];
				val = val >> (j%8);
				val = val % 2;
				pLogoData[i*logoWidth+j].red = pLogoData[i*logoWidth+j].green = pLogoData[i*logoWidth+j].blue = 255*(1-val);
			}
		break;

	case '5':
		/* Bin PGM */
		maxval = get_number(fp);
		if (maxval==-1)
		{
			cout<<_T("Error:invalid maximum color value!")<<endl;
			goto Exit_ParseLogo;
		}
		if (maxval>255)
		{
			cout<<_T("Error:maximum color value is large than 255!")<<endl;
			goto Exit_ParseLogo;
		}
		uiSrcDataSize = logoHeight*logoWidth;
		pSrcData = new BYTE[uiSrcDataSize];
		if (!pSrcData)
		{
			cout<<_T("Error:(Bin PGM)new data memory failed!")<<endl;
			goto Exit_ParseLogo;
		}
		uiuRead = fread(pSrcData,1,uiSrcDataSize,fp);
		if (uiuRead!=uiSrcDataSize)
		{
			cout<<_T("Error:(Bin PGM)read data failed,total=")<<uiSrcDataSize<<_T(",read=")<<uiuRead<<endl;
			goto Exit_ParseLogo;
		}
		
		for (i = 0; i < logoHeight; i++) {
			for (j = 0; j < logoWidth; j++)
			{
				val = pSrcData[i*logoWidth+j];
				val = val*100;
				val = 255*(val/maxval);
				val = val /100;				
				pLogoData[i*logoWidth+j].red = val;
				
				pLogoData[i*logoWidth+j].green = val;
				
				pLogoData[i*logoWidth+j].blue = val;
			}
		}
		break;
	case '6':
		/* Bin PPM */
		maxval = get_number(fp);
		if (maxval==-1)
		{
			cout<<_T("Error:invalid maximum color value!")<<endl;
			goto Exit_ParseLogo;
		}
		if (maxval>255)
		{
			cout<<_T("Error:maximum color value is large than 255!")<<endl;
			goto Exit_ParseLogo;
		}
		uiSrcDataSize = logoHeight*logoWidth*3;
		pSrcData = new BYTE[uiSrcDataSize];
		if (!pSrcData)
		{
			cout<<_T("Error:(Bin PPM)new data memory failed!")<<endl;
			goto Exit_ParseLogo;
		}
		uiuRead = fread(pSrcData,1,uiSrcDataSize,fp);
		if (uiuRead!=uiSrcDataSize)
		{
			cout<<_T("Error:(Bin PPM)read data failed,total=")<<uiSrcDataSize<<_T(",read=")<<uiuRead<<endl;
			goto Exit_ParseLogo;
		}

		for (i = 0; i < logoHeight; i++) {
			for (j = 0; j < logoWidth; j++)
			{
				val = pSrcData[(i*logoWidth+j)*3];
				val = val*100;
				val = 255*(val/maxval);
				val = val /100;				
				pLogoData[i*logoWidth+j].red = val;

				val = pSrcData[(i*logoWidth+j)*3+1];
				val = val*100;
				val = 255*(val/maxval);
				val = val /100;
				pLogoData[i*logoWidth+j].green = val;
				
				val = pSrcData[(i*logoWidth+j)*3+2];
				val = val*100;
				val = 255*(val/maxval);
				val = val /100;
				pLogoData[i*logoWidth+j].blue = val;
			}
		}
		break;
	default:
		break;
	}
	bRet = TRUE;

Exit_ParseLogo:
	if (pSrcData)
	{
		delete []pSrcData;
		pSrcData = NULL;
	}
	if (!bRet)
	{
		delete []pLogoData;
		pLogoData = NULL;
	}
	/* close file */
	fclose(fp);
	return bRet;
}
BOOL ImportLogo(BYTE *pKernel,int logoIndex,struct color *pLogo,int logoWidth,int logoHeight)
{
	int width,heigth;
	BOOL bRet=FALSE;
	int clutSize=0;
	int i,j,k;
	PUSHORT pusWidth,pusHeight;
	PBYTE pDataAddr=pKernel+k_Picture[logoIndex].dataAddr;

	if (k_Picture[logoIndex].lcd_max_width<logoWidth)
	{
		cout<<_T("Error:LCD_width is smaller than picure_width,lcd=")<<k_Picture[logoIndex].lcd_max_width<<_T(",pic=")<<logoWidth<<endl;
		width = k_Picture[logoIndex].lcd_max_width;
	}
	else
		width = logoWidth;

	if (k_Picture[logoIndex].lcd_max_height<logoHeight)
	{
		cout<<_T("Error:LCD_height is smaller than picure_height,lcd=")<<k_Picture[logoIndex].lcd_max_height<<_T(",pic=")<<logoHeight<<endl;
		heigth = k_Picture[logoIndex].lcd_max_height;
	}
	else
		heigth = logoHeight;
	PBYTE pClutIndex = NULL;
	pClutIndex = new BYTE[heigth*width];
	if (!pClutIndex)
	{
		cout<<_T("Error:new clut index failed!")<<endl;
		return FALSE;
	}
	//generate clut 
	for (i = 0; i < heigth; i++)
	{
		for (j = 0; j < width; j++) 
		{
			for (k = 0; k < clutSize; k++) 
			{
				if (IsColorEqual(pLogo[i*logoWidth+j], logo_clut[k]))
				{
					break;
				}
			}
			pClutIndex[i*width+j] = k+32;
			if (k == clutSize) 
			{
				if (clutSize == MAX_LINUX_LOGO_COLORS)
				{
					cout<<_T("Error:clut size is larger than 224!")<<endl;
					goto Exit_ImportLogo;
				}
				logo_clut[clutSize++] = pLogo[i*logoWidth+j];
			}
		}
	}
	//import logo
	
	/* write logo clut */
	pKernel[k_Picture[logoIndex].clutAddr-1] = clutSize;

	memcpy(pKernel+k_Picture[logoIndex].clutAddr,(PBYTE)logo_clut,clutSize*sizeof(struct color));
	
	pusWidth = (PUSHORT)(pKernel+k_Picture[logoIndex].dataAddr-4);
	*pusWidth = Endian16_Swap(k_Picture[logoIndex].lcd_max_width);
	pusHeight = pusWidth+1;
	*pusHeight = Endian16_Swap(k_Picture[logoIndex].lcd_max_height);
	
	/* write logo data */	
	memset(pDataAddr,0,k_Picture[logoIndex].lcd_max_width*k_Picture[logoIndex].lcd_max_height);
	for (i = 0; i < heigth; i++)
	{
		for (j = 0; j < width; j++) 
		{
			*pDataAddr=pClutIndex[i*width+j];
			pDataAddr++;
		}
		pDataAddr+=k_Picture[logoIndex].lcd_max_width-width;
	}
	//memcpy(pKernel+k_Picture[logoIndex].dataAddr,pClutIndex,heigth*width);

	bRet = TRUE;
Exit_ImportLogo:
	if (pClutIndex)
	{
		delete []pClutIndex;
		pClutIndex = NULL;
	}
	return bRet;
}
int IsColorEqual(struct color c1, struct color c2)
{
	return (c1.red == c2.red) && (c1.green == c2.green) && (c1.blue == c2.blue);
}
BOOL ImportBmpLogo(BYTE *pKernel,int logoIndex,CString strLogoFile)
{
	PBYTE pLogoData = pKernel + k_Picture[logoIndex].dataAddr;
	CFile	Bmpfile;
	BOOL bRet;
    BMPFILEHEADER BmpFileHead;
    BMPFILEINFOHEADER BmpInfoHead;
    unsigned long	bmpdata_buffer[3];

	memset(pLogoData,0,k_Picture[logoIndex].lcd_max_height*k_Picture[logoIndex].lcd_max_width*4);
	
	bRet = Bmpfile.Open(strLogoFile,CFile::typeBinary|CFile::modeRead);
	if (!bRet)
	{
		cout<<_T("Error:open ")<<(LPCTSTR)strLogoFile<<_T(" failed!")<<endl;
		return FALSE;
	}
	UINT uiRead;
	uiRead = Bmpfile.Read(&BmpFileHead,sizeof(BMPFILEHEADER));
	if (uiRead!=sizeof(BMPFILEHEADER))
	{
		Bmpfile.Close();
		cout<<_T("Error:read BmpFileHeader failed!")<<endl;
		return FALSE;
	}
	uiRead = Bmpfile.Read(&BmpInfoHead,sizeof(BMPFILEINFOHEADER));
	if (uiRead!=sizeof(BMPFILEINFOHEADER))
	{
		Bmpfile.Close();
		cout<<_T("Error:read BmpFileInfoHeader failed!")<<endl;
		return FALSE;
	}
	if (BmpFileHead.bfType!=0x4d42)
	{
		Bmpfile.Close();
		cout<<_T("Error:file is not bmp!")<<endl;
		return FALSE;
	}
	if ((BmpInfoHead.biBitCount!=24)&&(BmpInfoHead.biBitCount!=32))
	{
		Bmpfile.Close();
		cout<<_T("Error:only support 24 and 32 bits bmp!")<<endl;
		return FALSE;
	}
	if ((BmpInfoHead.biCompression!=BI_BITFIELDS)&&(BmpInfoHead.biCompression!=BI_RGB))
	{
		Bmpfile.Close();
		cout<<_T("Error:not support bmp compression !")<<endl;
		return FALSE;
	}
	if (k_Picture[logoIndex].lcd_max_height<ABS(BmpInfoHead.biHeight))
	{
		Bmpfile.Close();
		cout<<_T("Error:lcd height is smaller than bmp !")<<endl;
		return FALSE;
	}
	if (k_Picture[logoIndex].lcd_max_width<BmpInfoHead.biWidth)
	{
		Bmpfile.Close();
		cout<<_T("Error:lcd width is smaller than bmp !")<<endl;
		return FALSE;
	}
	if (BmpInfoHead.biCompression==BI_BITFIELDS)
	{
		uiRead = Bmpfile.Read(bmpdata_buffer,12);
		if (uiRead!=12)
		{
			Bmpfile.Close();
			cout<<_T("Error:read color mask failed!")<<endl;
			return FALSE;
		}
	}
	PBYTE pbmpData = NULL;
	pbmpData = new BYTE[BmpInfoHead.biSizeImage];
	if (!pbmpData)
	{
		Bmpfile.Close();
		cout<<_T("Error:new memory for read bmp data failed!")<<endl;
		return FALSE;
	}
	Bmpfile.Seek(BmpFileHead.bfOffBits,CFile::begin);
	uiRead = Bmpfile.ReadHuge(pbmpData,BmpInfoHead.biSizeImage);
	if (uiRead!=BmpInfoHead.biSizeImage)
	{
		if (pbmpData)
		{
			delete []pbmpData;
			pbmpData = NULL;
		}
		Bmpfile.Close();
		cout<<_T("Error:read bmp data failed!")<<endl;
		return FALSE;
	}
	Bmpfile.Close();
	PUSHORT pusWidth,pusHeight;
	pusWidth = (PUSHORT)(pKernel+k_Picture[logoIndex].dataAddr-24);
	*pusWidth = Endian16_Swap(BmpInfoHead.biWidth);
	pusHeight = pusWidth+1;
	*pusHeight = Endian16_Swap(ABS(BmpInfoHead.biHeight));
	int i,j;
	DWORD bmpColor;
	PDWORD pKernelColor=(PDWORD)pLogoData;
	int offset;
	BYTE byteCount = BmpInfoHead.biBitCount/8;
	if (BmpInfoHead.biCompression==BI_BITFIELDS)
	{
		DWORD dwRed,dwGreen,dwBlue,dwValue;
		BYTE  redShift,greenShift,blueShift;
		dwValue = bmpdata_buffer[0];
		redShift = 0;
		do 
		{
			if (dwValue&1)
			{
				break;
			}				
			dwValue = dwValue>>1;
			redShift++;
		} while (dwValue!=0);
		dwValue = bmpdata_buffer[1];
		greenShift = 0;
		do 
		{
			if (dwValue&1)
			{
				break;
			}				
			dwValue = dwValue>>1;
			greenShift++;
		} while (dwValue!=0);
		dwValue = bmpdata_buffer[2];
		blueShift = 0;
		do 
		{
			if (dwValue&1)
			{
				break;
			}				
			dwValue = dwValue>>1;
			blueShift++;
		} while (dwValue!=0);
		
		if (BmpInfoHead.biHeight>0)
		{//lower-left
			for (i=BmpInfoHead.biHeight-1;i>=0;i--)
			{
				for(j=0;j<BmpInfoHead.biWidth;j++)
				{
					offset = (i*BmpInfoHead.biWidth*byteCount)+j*byteCount;
					bmpColor = pbmpData[offset+2];
					bmpColor = bmpColor<<8;
					bmpColor += pbmpData[offset+1];
					bmpColor = bmpColor<<8;
					bmpColor += pbmpData[offset];
					dwRed = (bmpColor & bmpdata_buffer[0])>>redShift;
					dwGreen = (bmpColor & bmpdata_buffer[1])>>greenShift;
					dwBlue = (bmpColor & bmpdata_buffer[2])>>blueShift;
					*pKernelColor = (dwBlue<<16)|(dwGreen<<8)|dwRed;
					pKernelColor++;
				}
				pKernelColor+=k_Picture[logoIndex].lcd_max_width-BmpInfoHead.biWidth;
			}
		}
		else
		{//upper-left
			for (i=0;i<ABS(BmpInfoHead.biHeight);i++)
			{
				for(j=0;j<BmpInfoHead.biWidth;j++)
				{
					offset = (i*BmpInfoHead.biWidth*byteCount)+j*byteCount;
					bmpColor = pbmpData[offset+2];
					bmpColor = bmpColor<<8;
					bmpColor += pbmpData[offset+1];
					bmpColor = bmpColor<<8;
					bmpColor += pbmpData[offset];
					dwRed = (bmpColor & bmpdata_buffer[0])>>redShift;
					dwGreen = (bmpColor & bmpdata_buffer[1])>>greenShift;
					dwBlue = (bmpColor & bmpdata_buffer[2])>>blueShift;
					*pKernelColor = (dwBlue<<16)|(dwGreen<<8)|dwRed;
					pKernelColor++;
				}
				pKernelColor+=k_Picture[logoIndex].lcd_max_width-BmpInfoHead.biWidth;
			}
		}
	
	}
	else
	{
		if (BmpInfoHead.biHeight>0)
		{//lower-left
			for (i=BmpInfoHead.biHeight-1;i>=0;i--)
			{
				for(j=0;j<BmpInfoHead.biWidth;j++)
				{
					offset = i*BmpInfoHead.biWidth*byteCount+j*byteCount;
					bmpColor = pbmpData[offset+2];
					bmpColor = bmpColor<<8;
					bmpColor += pbmpData[offset+1];
					bmpColor = bmpColor<<8;
					bmpColor += pbmpData[offset];
					*pKernelColor = bmpColor;
					pKernelColor++;
				}
				pKernelColor+=k_Picture[logoIndex].lcd_max_width-BmpInfoHead.biWidth;
			}
		}
		else
		{//upper-left
			for (i=0;i<ABS(BmpInfoHead.biHeight);i++)
			{
				for(j=0;j<BmpInfoHead.biWidth;j++)
				{
					offset = (i*BmpInfoHead.biWidth*byteCount)+j*byteCount;
					bmpColor = pbmpData[offset];
					bmpColor = bmpColor<<8;
					bmpColor += pbmpData[offset+1];
					bmpColor = bmpColor<<8;
					bmpColor += pbmpData[offset+2];
					*pKernelColor = bmpColor;
					pKernelColor++;
				}
				pKernelColor+=k_Picture[logoIndex].lcd_max_width-BmpInfoHead.biWidth;
			}
		}
	}
	if (pbmpData)
	{
		delete []pbmpData;
		pbmpData = NULL;
	}
	return TRUE;

}
BOOL SaveKernel(BYTE *pKernel,UINT uiKernelSize,CString strKernelPath)
{
	CString strTmpKernelFile;
	PUINT puiCrc;
	GetModuleFileName(NULL, strTmpKernelFile.GetBuffer(MAX_PATH), MAX_PATH);
	strTmpKernelFile.ReleaseBuffer();
	strTmpKernelFile = strTmpKernelFile.Left(strTmpKernelFile.ReverseFind(_T('\\'))+1);
	strTmpKernelFile += _T("tmp_file.bin");
	CFile file;
	BOOL bRet;
	bRet = file.Open(strTmpKernelFile,CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary);
	if (!bRet)
	{
		cout<<_T("Error:create new kernel failed!")<<endl;
		return FALSE;
	}
	try
	{
		puiCrc = (PUINT)(pKernel+uiKernelSize-4);
		*puiCrc = CRC_32(pKernel+8,uiKernelSize-12);
		file.WriteHuge(pKernel,uiKernelSize);
	}
	catch (...)
	{
		file.Close();
		cout<<_T("Error:write new kernel failed!")<<endl;
		return FALSE;
	}
	file.Close();
	bRet = CopyFile(strTmpKernelFile,strKernelPath,FALSE);
	if (bRet)
	{
		DeleteFile(strTmpKernelFile);
	}
	return bRet;
}

int kernel_importLogo_logo(CString KerenlPath,CString strLogoPath)
{

	int nRetCode = 0;
	CString strKernelPath;
	CString strOperate;
	int picIndex=0;
	//1.check kernel file
	CFile kernelFile;
	UINT uiKernelSize;
	BOOL bRet;
	bRet = kernelFile.Open(KerenlPath,CFile::modeRead|CFile::typeBinary);
	if (!bRet)
	{
		return -7;
		goto Exit_MAIN;
	}
	uiKernelSize = kernelFile.GetLength();
	BYTE *pKernelBuffer=NULL;
	UINT uiRead;
	pKernelBuffer = new BYTE[uiKernelSize];
	if (!pKernelBuffer)
	{
		kernelFile.Close();
		nRetCode = -8;
		goto Exit_MAIN;
	}
	uiRead = kernelFile.ReadHuge(pKernelBuffer,uiKernelSize);
	kernelFile.Close();
	if (uiRead!=uiKernelSize)
	{
		nRetCode = -9;
		goto Exit_MAIN;
	}
	bRet = CheckKernel(pKernelBuffer);
	if (!bRet)
	{
		nRetCode = -10;
		goto Exit_MAIN;
		
	}
	//2.search logo
	UINT uiLogoNumber;
	uiLogoNumber = SearchLogo(pKernelBuffer);
	if (uiLogoNumber==0)
	{
		nRetCode = -11;
		goto Exit_MAIN;
	}
	if (picIndex>=uiLogoNumber)
	{		
		nRetCode = -12;
		goto Exit_MAIN;
	}

	strLogoPath.MakeLower();
	if (strLogoPath.Find(_T(".bmp"))!=-1)
	{
		bRet = ImportBmpLogo(pKernelBuffer,picIndex,strLogoPath);
		if (!bRet)
		{
			cout<<_T("Error:import bmp logo failed!")<<endl;
			nRetCode = -15;
			goto Exit_MAIN;
		}
		bRet = SaveKernel(pKernelBuffer,uiKernelSize,KerenlPath);
		if (!bRet)
		{
			cout<<_T("Error:save new kernel failed!")<<endl;
			nRetCode = -16;
			goto Exit_MAIN;
		}
	}
	else
	{
		struct color *pLogoData=NULL;
		int logoWidth,logoHeight;
		bRet = ParseLogo(strLogoPath,pLogoData,logoWidth,logoHeight);
		if (!bRet)
		{
			cout<<_T("Error:parse logo failed!")<<endl;
			nRetCode = -14;
			goto Exit_MAIN;
		}
		bRet = ImportLogo(pKernelBuffer,picIndex,pLogoData,logoWidth,logoHeight);
		if (pLogoData)
		{
			delete []pLogoData;
			pLogoData = NULL;
		}
		if (!bRet)
		{
			cout<<_T("Error:import logo failed!")<<endl;
			nRetCode = -15;
			goto Exit_MAIN;

		}
		bRet = SaveKernel(pKernelBuffer,uiKernelSize,KerenlPath);
		if (!bRet)
		{
			cout<<_T("Error:save new kernel failed!")<<endl;
			nRetCode = -16;
			goto Exit_MAIN;

		}
	}
Exit_MAIN:
	if (pKernelBuffer)
	{
		delete pKernelBuffer;
		pKernelBuffer = NULL;
	}

	return nRetCode;
}

int CovertNormalBmp(CString srcPath,CString desPath)
{
	DWORD srcSize=0;
	int bRet=0,mOffsetHeadInfo=0;	
	CFile srcBmpFile,desBmpFile;
	BMPFILEHEADER BmpFileHead;
    BMPFILEINFOHEADER BmpInfoHead;
	PRKRGBQUAD pBmpColorTable=NULL;
	PBYTE pSrcBmpdata=NULL;

	bRet = srcBmpFile.Open(srcPath,CFile::typeBinary|CFile::modeRead);
	if (!bRet)
	{
		if(srcBmpFile!=NULL)
			srcBmpFile.Close();
		cout<<_T("Error:create ")<<(LPCTSTR)srcPath<<_T(" failed!")<<endl;
		return FALSE;
	}

	bRet = desBmpFile.Open(desPath,CFile::typeBinary|CFile::modeWrite|CFile::modeCreate);
	if (!bRet)
	{
		if(desBmpFile!=NULL)
			desBmpFile.Close();
		cout<<_T("Error:create ")<<(LPCTSTR)desPath<<_T(" failed!")<<endl;
		return FALSE;
	}

	srcSize=srcBmpFile.GetLength();
	if(srcSize>0)
	{
		PBYTE pTemp=NULL;
		
		srcBmpFile.Read(&BmpFileHead,sizeof(BMPFILEHEADER));	
		srcBmpFile.Read(&BmpInfoHead,sizeof(BMPFILEINFOHEADER));
	

		if(BmpInfoHead.biCompression!=1){
			goto End;			
		}

		pBmpColorTable=new RKRGBQUAD[256]; 

		srcBmpFile.Seek(sizeof(BMPFILEHEADER)+BmpInfoHead.biSize,CFile::begin);
		srcBmpFile.Read(pBmpColorTable,256*sizeof(RKRGBQUAD));

		pSrcBmpdata=new BYTE[BmpFileHead.bfSize-BmpFileHead.bfOffBits];
		srcBmpFile.Read(pSrcBmpdata,BmpFileHead.bfSize-BmpFileHead.bfOffBits);

		if(BmpInfoHead.biSize!=0x28){
			mOffsetHeadInfo=BmpInfoHead.biSize-0x28;
			BmpInfoHead.biSize=0x28;
			BmpFileHead.bfSize=BmpFileHead.bfSize-mOffsetHeadInfo;
			BmpFileHead.bfOffBits=BmpFileHead.bfOffBits-mOffsetHeadInfo;
		}


		desBmpFile.Write(&BmpFileHead,sizeof(BMPFILEHEADER));
		desBmpFile.Write(&BmpInfoHead,sizeof(BMPFILEINFOHEADER));
		desBmpFile.Write(pBmpColorTable,256*sizeof(RKRGBQUAD));
		desBmpFile.Write(pSrcBmpdata,BmpFileHead.bfSize-BmpFileHead.bfOffBits);
	}
	
End:
	srcBmpFile.Close();
	desBmpFile.Close();
	if(pSrcBmpdata!=NULL)
		delete [] pSrcBmpdata;

	if(pBmpColorTable!=NULL)
		delete [] pBmpColorTable;
	return 0;
}

int resource_importLogo(CString ResourcePath,CString LogoPath)
{
	BOOL bRet;
	CFile logoFile;	
	CFile resourceFile;
	UINT logoSize,size;
	UINT FileSize=0;
	INDEX_TBL_ENTRY bmpEntry;

	bRet = logoFile.Open(LogoPath,CFile::modeRead|CFile::typeBinary);
	if (!bRet)
	{
		return -7;
	}
	size=logoFile.GetLength();
	if(size%512==0)
		logoSize=size;
	else
		logoSize=size+size%512;

	BYTE *pLogoBuffer=NULL;
	UINT uiRead;
	pLogoBuffer = new BYTE[logoSize];
	
	memset(pLogoBuffer,0,logoSize);
	
	uiRead = logoFile.Read(pLogoBuffer,size);
	logoFile.Close();
	if (uiRead!=size)
	{	
		if(pLogoBuffer)
			delete [] pLogoBuffer;
		return -9;
	}
	
	TRACE1("%s",ResourcePath);
	bRet = resourceFile.Open(ResourcePath,CFile::modeReadWrite|CFile::typeBinary);
	if (!bRet)
	{
		if(pLogoBuffer)
			delete [] pLogoBuffer;
		return -7;
	}
	
	resourceFile.Seek(2*BLOCK_SIZE,CFile::begin);

	resourceFile.Read(&bmpEntry,sizeof(INDEX_TBL_ENTRY));

	bmpEntry.content_size=size;

	resourceFile.Seek(2*BLOCK_SIZE,CFile::begin);

	resourceFile.Write(&bmpEntry,sizeof(INDEX_TBL_ENTRY));

	resourceFile.Seek(bmpEntry.content_offset*512,CFile::begin);

	resourceFile.Write(pLogoBuffer,logoSize);	

	resourceFile.Close();

	if(pLogoBuffer)
		delete [] pLogoBuffer;

	return 0;
}

int resource_importDtb(CString ResourcePath,CString dtbPath)
{
	BOOL bRet;
	CFile dtbFile;
	UINT dtbSize,dtbrealsize;
	UINT FileSize=0;
	bRet = dtbFile.Open(dtbPath,CFile::modeRead|CFile::typeBinary);
	if (!bRet)
	{
		return -1;
	}
	dtbrealsize=dtbFile.GetLength();
	if(dtbrealsize%512==0)
		dtbSize=dtbrealsize;
	else
		dtbSize=(dtbrealsize/BLOCK_SIZE+1)*BLOCK_SIZE;
	
	BYTE *pdtbBuffer=NULL;
	UINT uiRead;
	pdtbBuffer = new BYTE[dtbSize];
	
	memset(pdtbBuffer,0,dtbSize);
	
	uiRead = dtbFile.Read(pdtbBuffer,dtbrealsize);
	dtbFile.Close();
	if (uiRead!=dtbrealsize)
	{
		if(pdtbBuffer)
			delete [] pdtbBuffer;
		return -2;
	}

	
	INDEX_TBL_ENTRY bmpEntry;
	INDEX_TBL_ENTRY kernelEntry;
	CFile resourceFile;
	TRACE1("%s",ResourcePath);
	bRet = resourceFile.Open(ResourcePath,CFile::modeReadWrite|CFile::typeBinary);
	if (!bRet)
	{
		if(pdtbBuffer)
			delete [] pdtbBuffer;
		return -3;
	}

	resourceFile.Seek(BLOCK_SIZE,CFile::begin);
	resourceFile.Read(&kernelEntry,sizeof(INDEX_TBL_ENTRY));
	resourceFile.Read(&bmpEntry,sizeof(INDEX_TBL_ENTRY));

	BYTE *pbmpBuffer=NULL;
	UINT bmpsize;
	if((bmpEntry.content_size%BLOCK_SIZE)==0)
		bmpsize=bmpEntry.content_size;
	else
		bmpsize=(bmpEntry.content_size/BLOCK_SIZE+1)*BLOCK_SIZE;

	pbmpBuffer = new BYTE[bmpsize];
	resourceFile.Seek(bmpEntry.content_offset*BLOCK_SIZE,CFile::begin);
	resourceFile.Read(pbmpBuffer,bmpsize);

	resourceFile.Seek(BLOCK_SIZE,CFile::begin);
	kernelEntry.content_size=dtbrealsize;
	resourceFile.Write(&kernelEntry,sizeof(INDEX_TBL_ENTRY));

	bmpEntry.content_offset=(kernelEntry.content_offset*BLOCK_SIZE+dtbSize)/BLOCK_SIZE;
	resourceFile.Write(&bmpEntry,sizeof(INDEX_TBL_ENTRY));
	
	resourceFile.Write(pdtbBuffer,dtbSize);

	resourceFile.Write(pbmpBuffer,bmpsize);
	
	resourceFile.Close();
	if(pbmpBuffer)
		delete [] pbmpBuffer;

	if(pdtbBuffer)
		delete [] pdtbBuffer;

	return 0;
}

int kernel_export_logo(CString KerenlPath,CString FilePath,int ChipType)
{

	int nRetCode = 0;
	CString strKernelPath,strLogoPath;
	CString strOperate;
	int picIndex=0;
	//1.check kernel file
	CFile kernelFile;
	UINT uiKernelSize;
	BOOL bRet;
	bRet = kernelFile.Open(KerenlPath,CFile::modeRead|CFile::typeBinary);
	if (!bRet)
	{
		//MessageBox(_T("Error:open ")+KerenlPath+_T(" failed!"));
		return -7;
	}
	uiKernelSize = kernelFile.GetLength();
	BYTE *pKernelBuffer=NULL;
	UINT uiRead;
	pKernelBuffer = new BYTE[uiKernelSize];
	if (!pKernelBuffer)
	{
		kernelFile.Close();
		//MessageBox(_T("Error:new kernel memory failed!"));
		nRetCode = -8;
		
	}
	uiRead = kernelFile.ReadHuge(pKernelBuffer,uiKernelSize);
	kernelFile.Close();
	if (uiRead!=uiKernelSize)
	{
		//MessageBox(_T("Error:read ")+KerenlPath+_T(" failed!"));
		nRetCode = -9;
		
	}
	bRet = CheckKernel(pKernelBuffer);
	if (!bRet)
	{
		//MessageBox(_T("Error:check kernel failed!"));
		nRetCode = -10;
	
	}
	//2.search logo
	UINT uiLogoNumber;
	uiLogoNumber = SearchLogo(pKernelBuffer);
	if (uiLogoNumber==0)
	{
		nRetCode = -11;
		
	}

	FilePath.MakeLower();
	if (FilePath.Find(_T(".bmp"))!=-1)
	{
		bRet = ExportBmpLogo(pKernelBuffer,picIndex,FilePath,ChipType);
		if (!bRet)
		{
			cout<<_T("Error:export logo failed!")<<endl;
			nRetCode = -13;
		}
	}
	else
	{
		bRet = ExportLogo(pKernelBuffer,picIndex,FilePath);
		if (!bRet)
		{
			cout<<_T("Error:export logo failed!")<<endl;
			nRetCode = -13;
		}
	}
	//ExportLogo(pKernelBuffer,picIndex,FilePath);

	
	if (pKernelBuffer)
	{
		delete pKernelBuffer;
		pKernelBuffer = NULL;
	}

	return nRetCode;
}

int convert_nocompression_logo(CString srcLogo,CString desLogo)
{
	CFile	srcBmpfile;
	CFile	desBmpfile;
	int srcSize=0;
	BOOL bRet;
    BMPFILEHEADER BmpFileHead;
    BMPFILEINFOHEADER BmpInfoHead;
	int i,j,k,num,index,size=0;
	

	bRet = srcBmpfile.Open(srcLogo,CFile::typeBinary|CFile::modeRead);
	if (!bRet)
	{
		cout<<_T("Error:create ")<<(LPCTSTR)srcLogo<<_T(" failed!")<<endl;
		return FALSE;
	}

	bRet = desBmpfile.Open(desLogo,CFile::typeBinary|CFile::modeWrite|CFile::modeCreate);
	if (!bRet)
	{
		if(srcBmpfile!=NULL)
			srcBmpfile.Close();
		cout<<_T("Error:create ")<<(LPCTSTR)srcLogo<<_T(" failed!")<<endl;
		return FALSE;
	}

	srcSize=srcBmpfile.GetLength();
	if(srcSize>0)
	{
		PBYTE pTemp=NULL;
		
		srcBmpfile.Read(&BmpFileHead,sizeof(BMPFILEHEADER));	
		srcBmpfile.Read(&BmpInfoHead,sizeof(BMPFILEINFOHEADER));
	

		if(BmpInfoHead.biCompression!=1){
			return -1;		
		}

		PRKRGBQUAD pBmpColorTable=new RKRGBQUAD[256]; 
		srcBmpfile.Read(pBmpColorTable,256*sizeof(RKRGBQUAD));

		PBYTE pSrcBmpdata=new BYTE[BmpFileHead.bfSize-BmpFileHead.bfOffBits];
		srcBmpfile.Read(pSrcBmpdata,BmpFileHead.bfSize-BmpFileHead.bfOffBits);


		PUINT pDesBmpdata=new UINT[BmpInfoHead.biHeight*BmpInfoHead.biWidth];

		desBmpfile.Write(&BmpFileHead,sizeof(BMPFILEHEADER));
		desBmpfile.Write(&BmpFileHead,sizeof(BMPFILEHEADER));
		for(i=0;i<BmpInfoHead.biHeight;i++)
			for(j=0;j<BmpInfoHead.biWidth;j=j+2){
				index=pSrcBmpdata[i*BmpInfoHead.biWidth+j];
				num=pSrcBmpdata[i*BmpInfoHead.biWidth+j+1];
				if(num>=0){
					for(k=0;k<num;k++)
						pDesBmpdata[k+size]=*(UINT *)(pBmpColorTable+index);
					size=size+k;
				}
			}
				
		desBmpfile.Write(&pDesBmpdata,BmpInfoHead.biHeight*BmpInfoHead.biWidth*sizeof(UINT));
	
	}

	srcBmpfile.Close();
	desBmpfile.Close();

	return 0;
}

int resource_export_kernel_logo(CString ResourcePath,CString FilePath)
{
	
	int mFilesize=0;
	int nRetCode = 0;
	CFile resourceFile;
	CFile LogoFile;
	BOOL bRet;
	INDEX_TBL_ENTRY bmpEntry;

	bRet = resourceFile.Open(ResourcePath,CFile::modeRead|CFile::typeBinary);
	if (!bRet)
	{
		return -1;
	}
	
	resourceFile.Seek(em_logo_kernel_type*BLOCK_SIZE,0);

	resourceFile.Read(&bmpEntry,sizeof(INDEX_TBL_ENTRY));

	BYTE *pResourceBuffer=NULL;
	pResourceBuffer = new BYTE[bmpEntry.content_size];

	resourceFile.Seek(bmpEntry.content_offset*BLOCK_SIZE,0);

	mFilesize=resourceFile.Read(pResourceBuffer,bmpEntry.content_size);
	if(mFilesize!=bmpEntry.content_size){
		resourceFile.Close();
		return -2;
	}

	resourceFile.Close();


	bRet = LogoFile.Open(FilePath,CFile::modeWrite|CFile::modeCreate);
	if (!bRet)
	{
		return -3;
	}

	LogoFile.Write(pResourceBuffer,mFilesize);

	LogoFile.Close();

	if(pResourceBuffer!=NULL){
		delete[] pResourceBuffer;
		pResourceBuffer=NULL;
	}

	return 0;
}

int resource_export_uboot_logo(CString ResourcePath,CString FilePath)
{
	
	int mFilesize=0;
	int nRetCode = 0;
	CFile resourceFile;
	CFile LogoFile;
	BOOL bRet;
	INDEX_TBL_ENTRY bmpEntry;
	
	bRet = resourceFile.Open(ResourcePath,CFile::modeRead|CFile::typeBinary);
	if (!bRet)
	{
		return -1;
	}
	
	resourceFile.Seek(em_logo_type*BLOCK_SIZE,0);
	
	resourceFile.Read(&bmpEntry,sizeof(INDEX_TBL_ENTRY));
	
	BYTE *pResourceBuffer=NULL;
	pResourceBuffer = new BYTE[bmpEntry.content_size];
	
	resourceFile.Seek(bmpEntry.content_offset*BLOCK_SIZE,0);
	
	mFilesize=resourceFile.Read(pResourceBuffer,bmpEntry.content_size);
	if(mFilesize!=bmpEntry.content_size){
		resourceFile.Close();
		return -2;
	}
	
	resourceFile.Close();
	
	
	bRet = LogoFile.Open(FilePath,CFile::modeWrite|CFile::modeCreate);
	if (!bRet)
	{
		return -3;
	}
	
	LogoFile.Write(pResourceBuffer,mFilesize);
	
	LogoFile.Close();
	
	if(pResourceBuffer!=NULL){
		delete[] pResourceBuffer;
		pResourceBuffer=NULL;
	}
	
	return 0;
}

int resource_export_dtb(CString ResourcePath,CString FilePath)
{
	
	int mFilesize=0;
	int nRetCode = 0;
	CFile resourceFile;
	CFile DtbFile;
	BOOL bRet;
	INDEX_TBL_ENTRY dtbEntry;
	
	bRet = resourceFile.Open(ResourcePath,CFile::modeRead|CFile::typeBinary);
	if (!bRet)
	{
		return -1;
	}
	
	resourceFile.Seek(em_kernel_type*BLOCK_SIZE,0);
	
	resourceFile.Read(&dtbEntry,sizeof(INDEX_TBL_ENTRY));
	
	BYTE *pResourceBuffer=NULL;
	pResourceBuffer = new BYTE[dtbEntry.content_size];
	
	resourceFile.Seek(dtbEntry.content_offset*BLOCK_SIZE,0);
	
	mFilesize=resourceFile.Read(pResourceBuffer,dtbEntry.content_size);
	if(mFilesize!=dtbEntry.content_size){
		resourceFile.Close();
		return -2;
	}
	
	resourceFile.Close();
	
	
	bRet = DtbFile.Open(FilePath,CFile::modeWrite|CFile::modeCreate);
	if (!bRet)
	{
		return -3;
	}
	
	DtbFile.Write(pResourceBuffer,mFilesize);
	
	DtbFile.Close();
	
	if(pResourceBuffer!=NULL){
		delete[] pResourceBuffer;
		pResourceBuffer=NULL;
	}
	
	return 0;
}


int GetKernelLogoSize(CString KerenlPath,WORD *width,WORD *height,WORD *maxwidth,WORD *maxheight,int index)
{
	int nRetCode = 0;
	CString strKernelPath,strLogoPath;
	CString strOperate;
	int picIndex=0;
	//1.check kernel file
	CFile kernelFile;
	UINT uiKernelSize;
	BOOL bRet;
	char j;
	int uiRet=0;
	char data_str[] = "logo_RKlogo_data";
	
	bRet = kernelFile.Open(KerenlPath,CFile::modeRead|CFile::typeBinary);
	if (!bRet)
	{
		//MessageBox(_T("Error:open ")+KerenlPath+_T(" failed!"));
		return -7;
	}
	uiKernelSize = kernelFile.GetLength();
	BYTE *pKernelBuffer=NULL;
	UINT uiRead;
	pKernelBuffer = new BYTE[uiKernelSize];
	if (!pKernelBuffer)
	{
		kernelFile.Close();
		//MessageBox(_T("Error:new kernel memory failed!"));
		nRetCode = -8;
		
	}
	uiRead = kernelFile.ReadHuge(pKernelBuffer,uiKernelSize);
	kernelFile.Close();
	if (uiRead!=uiKernelSize)
	{
        if (pKernelBuffer)
        {
            delete pKernelBuffer;
            pKernelBuffer = NULL;
		}
		nRetCode = -9;
		
	}

	DWORD *pSize;
	PBYTE pStartPos,pEndPos;
	pSize = (DWORD *)(pKernelBuffer+4);
	pStartPos = pKernelBuffer+8;
	pEndPos = pStartPos+*pSize;

	//search clut
	for (j = 0; j < MAX_KERNEL_LOGO_NUM; j++)
	{
		if (SearchSubStr(pStartPos,pEndPos,data_str))
		{			
			*maxwidth=Endian16_Swap(*((PUSHORT)(pStartPos)));
			*maxheight=Endian16_Swap(*((PUSHORT)(pStartPos+2)));

			*width=Endian16_Swap(*((PUSHORT)(pStartPos-sizeof(data_str)-3)));
			*height=Endian16_Swap(*((PUSHORT)(pStartPos-sizeof(data_str)-1)));
            if (pKernelBuffer)
            {
                delete pKernelBuffer;
                pKernelBuffer = NULL;
		    }
			return 0;
		}
		else
		{
            if (pKernelBuffer)
            {
                delete pKernelBuffer;
                pKernelBuffer = NULL;
		    }
			return -1;
		}
	}
    if (pKernelBuffer)
    {
        delete pKernelBuffer;
        pKernelBuffer = NULL;
    }
	return -1;

}

int GetResourceLogoSize(CString ResourcePath,WORD *width,WORD *height)
{
	int nRetCode = 0;
	CString strResourcePath,strLogoPath;
	CString strOperate;
	int picIndex=0;
	//1.check kernel file
	CFile resourceFile;
	UINT uiResourceSize;
	BYTE *pbuf=NULL;
	BOOL bRet;
	int uiRet=0;
	INDEX_TBL_ENTRY bmpEntry;
	BMPFILEINFOHEADER bmpInfo;
	
	bRet = resourceFile.Open(ResourcePath,CFile::modeRead|CFile::typeBinary);
	if (!bRet)
	{
		return -7;
	}
	uiResourceSize = resourceFile.GetLength();

	resourceFile.Seek(512,CFile::begin);
	resourceFile.Read(&bmpEntry,sizeof(INDEX_TBL_ENTRY));

	resourceFile.Seek(bmpEntry.content_offset*512+sizeof(BMPFILEHEADER),CFile::begin);
	resourceFile.Read(&bmpInfo,sizeof(BMPFILEINFOHEADER));

	*width=bmpInfo.biWidth;
	
	*height=bmpInfo.biHeight;

	if(resourceFile)
		resourceFile.Close();

	return nRetCode;

}

int JudgeKernelLogoType(CString KerenlPath)
{
	int nRetCode = 0;
	CString strKernelPath,strLogoPath;
	CString strOperate;
	int picIndex=0;
	//1.check kernel file
	CFile kernelFile;
	UINT uiKernelSize;
	BOOL bRet;
	char i, j;
	int uiRet=0;
	char clut_str[] = "logo_RKlogo_clut";
	
	bRet = kernelFile.Open(KerenlPath,CFile::modeRead|CFile::typeBinary);
	if (!bRet)
	{
		//MessageBox(_T("Error:open ")+KerenlPath+_T(" failed!"));
		return -7;
	}
	uiKernelSize = kernelFile.GetLength();
	BYTE *pKernelBuffer=NULL;
	UINT uiRead;
	pKernelBuffer = new BYTE[uiKernelSize];
	if (!pKernelBuffer)
	{
		kernelFile.Close();
		//MessageBox(_T("Error:new kernel memory failed!"));
		nRetCode = -8;
		
	}
	uiRead = kernelFile.ReadHuge(pKernelBuffer,uiKernelSize);
	kernelFile.Close();
	if (uiRead!=uiKernelSize)
	{
		if (pKernelBuffer)
		{
            delete pKernelBuffer;
            pKernelBuffer = NULL;
		}
		nRetCode = -9;
	}

	DWORD *pSize;
	char logotye[3];
	PBYTE pStartPos,pEndPos;
	pSize = (DWORD *)(pKernelBuffer+4);
	pStartPos = pKernelBuffer+8;
	pEndPos = pStartPos+*pSize;

	//search clut
	for (j = 0; j < MAX_KERNEL_LOGO_NUM; j++)
	{
		if (SearchSubStr(pStartPos,pEndPos,clut_str))
		{
			for(i=0;i<3;i++)
				logotye[i]= *((PCHAR)(pStartPos-sizeof(clut_str)-2+i));
		
			//MessageBox(CString(logotye,3));
			if(!strncmp(logotye,"bmp",3))
			{
                if (pKernelBuffer)
                {
                    delete pKernelBuffer;
                    pKernelBuffer = NULL;
		        }
				return CXIMAGE_FORMAT_BMP;
			}
			else
			{
                if (pKernelBuffer)
                {
                    delete pKernelBuffer;
                    pKernelBuffer = NULL;
		        }
				return CXIMAGE_FORMAT_PNM;
			}
			pStartPos++;
			
		}
		else
		{
            if (pKernelBuffer)
            {
                delete pKernelBuffer;
                pKernelBuffer = NULL;
		    }
			return -1;
		}
	}

    if (pKernelBuffer)
    {
        delete pKernelBuffer;
        pKernelBuffer = NULL;
		}
	return -1;

}


