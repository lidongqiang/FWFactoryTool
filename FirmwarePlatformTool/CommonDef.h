#ifndef COMMONDEF_HEADER
#define COMMONDEF_HEADER

#define  IMAGE_RESERVED_SIZE 61
typedef enum
{
		RKNONE_DEVICE=0,
		RK27_DEVICE=0x10,
		RKCAYMAN_DEVICE,
		RK28_DEVICE=0x20,
		RK281X_DEVICE,
		RKPANDA_DEVICE,
		RKNANO_DEVICE=0x30,
		RKSMART_DEVICE,
		RKCROWN_DEVICE=0x40,
		RK29_DEVICE=0x50,
		RK292X_DEVICE,
		RK30_DEVICE=0x60,
		RK31_DEVICE=0x70,
		RK32_DEVICE=0x80,
		RK33_DEVICE=0x90,
}ENUM_RKDEVICE_TYPE;

#pragma pack(1)
typedef struct  
{
	USHORT usYear;
	UCHAR  ucMonth;
	UCHAR  ucDay;
	UCHAR  ucHour;
	UCHAR  ucMinute;
	UCHAR  ucSecond;
}STRUCT_RKTIME,*PSTRUCT_RKTIME;

typedef struct  
{
	UINT uiTag;
	USHORT usSize;
	DWORD  dwVersion;
	DWORD  dwMergeVersion;
	STRUCT_RKTIME stReleaseTime;
	ENUM_RKDEVICE_TYPE emSupportChip;
	DWORD  dwBootOffset;
	DWORD  dwBootSize;
	DWORD  dwFWOffset;
	DWORD  dwFWSize;
	BYTE   reserved[IMAGE_RESERVED_SIZE];
}STRUCT_RKIMAGE_HEAD,*PSTRUCT_RKIMAGE_HEAD;

typedef struct 
{
	char name[32];// 分区名称
	char file[60];// 相对路径名
}STRUCT_PACKAGE_ITEM,*PSTRUCT_PACKAGE_ITEM;

#pragma pack()

typedef vector<STRUCT_PACKAGE_ITEM> VECTOR_PACKAGE_SET;
#define PARTNAME_PARAMETER		"parameter"
#define PARTNAME_MISC			"misc"
#define PARTNAME_KERNEL			"kernel"
#define PARTNAME_BOOT			"boot"
#define PARTNAME_RECOVERY		"recovery"
#define PARTNAME_SYSTEM			"system"
#define PARTNAME_BACKUP			"backup"

typedef list<CString> STRING_SET;
typedef STRING_SET::iterator string_list_iter;

typedef void (_stdcall *SetMsgHandle)(DWORD msgHwnd);
typedef void  (_stdcall *SetWindowLanguage)(int emLang);
typedef bool  (_stdcall *GetTabTitle)(char *szTitle);
typedef DWORD  (_stdcall *CreateChildWindow)(DWORD parentHwnd);
typedef bool  (_stdcall *DestroyChildWindow)(DWORD hwnd);

typedef struct
{
	HMODULE hDll;
	CString strDllName;
	CString strTabTitle;
	DWORD   childHwnd;
	SetMsgHandle pSetMsgHandle;
	SetWindowLanguage pSetWindowLanguage;
	GetTabTitle pGetTabTitle;
	CreateChildWindow pCreateChildWindow;
	DestroyChildWindow pDestroyChildWindow;
}STRUCT_EXTENSION;
typedef vector<STRUCT_EXTENSION> VECTOR_EXTENSION;
enum ENUM_MSG_KEY{EmMsg_Boot_Key=0,EmMsg_Recovery_Key,EmMsg_System_key,EmMsg_Root_key};
#define WM_TEST_RUNNING WM_USER+88
#define WM_GET_DIR WM_USER+89
#define WM_UNPACK_FINISH WM_USER+90
#define WM_PACK_FINISH WM_USER+91
#define WM_START_PACK  WM_USER+92
#endif
