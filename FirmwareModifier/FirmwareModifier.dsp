# Microsoft Developer Studio Project File - Name="FirmwareModifier" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=FirmwareModifier - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FirmwareModifier.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FirmwareModifier.mak" CFG="FirmwareModifier - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FirmwareModifier - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "FirmwareModifier - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FirmwareModifier - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\FirmwarePlatformTool\bin\Plugin"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "cxImage\release\inc" /I "cmMfcLib" /I "Common\inc\\" /I "Device\inc\\" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_MBCS" /D "FWPLUGIN_EXPORTS" /D "_UNICODE" /D "UNICODE" /D "_WINDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 cmMfcLib\cmMfclib.lib /nologo /subsystem:windows /dll /machine:I386 /libpath:"cxImage\release\lib" /libpath:"cmMfcLib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "FirmwareModifier - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\FirmwarePlatformTool\bin\Plugin"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "cxImage\debug\inc" /I "cmMfcLib" /I "Common\inc" /I "Device\inc" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_MBCS" /D "FWPLUGIN_EXPORTS" /D "_UNICODE" /D "UNICODE" /D "_WINDLL" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept /libpath:"cxImage\debug\lib" /libpath:"cmMfcLib"

!ENDIF 

# Begin Target

# Name "FirmwareModifier - Win32 Release"
# Name "FirmwareModifier - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "EMCommon"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Device\cpp\EmAdbDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\Device\cpp\EmAdbScanner.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\cpp\EmBase64.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\cpp\EmCharSet.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\cpp\EmCs.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\cpp\EmDiskInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\cpp\EmDllRegister.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\cpp\EmFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\cpp\EmFileCache.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\cpp\EmFileInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\cpp\EmGuid.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\cpp\EmHandy.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\cpp\EmMd5.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\cpp\EmMemory.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\cpp\EmOsVersion.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\cpp\EmProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\cpp\EmRes.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\cpp\EmShell.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\cpp\EmStlStr.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\cpp\EmStr.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\cpp\EmSz.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\cpp\EmThread.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\cpp\EmTime.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\cpp\EmTstr.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\cpp\EmTsz.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\cpp\EmUrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Device\cpp\EmUsbDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\Device\cpp\EmUsbScanner.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\cpp\EmUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\cpp\EmVariantConvert.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\cpp\EmWstr.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\cpp\EmWsz.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\cpp\tinystr.cpp
# End Source File
# End Group
# Begin Group "imagelib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\imagelib\libfiasco.dll
# End Source File
# Begin Source File

SOURCE=.\imagelib\libjpeg.dll
# End Source File
# Begin Source File

SOURCE=.\imagelib\libpbm.dll
# End Source File
# Begin Source File

SOURCE=.\imagelib\libpgm.dll
# End Source File
# Begin Source File

SOURCE=.\imagelib\libpng.dll
# End Source File
# Begin Source File

SOURCE=.\imagelib\libpnm.dll
# End Source File
# Begin Source File

SOURCE=.\imagelib\libppm.dll
# End Source File
# End Group
# Begin Source File

SOURCE=.\AdbOperator.cpp
# End Source File
# Begin Source File

SOURCE=.\ChargeImage.cpp
# End Source File
# Begin Source File

SOURCE=.\CRC.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBatteryParam.cpp
# End Source File
# Begin Source File

SOURCE=.\FirmwareModifier.cpp
# End Source File
# Begin Source File

SOURCE=.\FirmwareModifier.def
# End Source File
# Begin Source File

SOURCE=.\FirmwareModifier.rc
# End Source File
# Begin Source File

SOURCE=.\KernelLogoTool.cpp
# End Source File
# Begin Source File

SOURCE=.\MySpawnConsumer.cpp
# End Source File
# Begin Source File

SOURCE=.\NotePadBase.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SysCfgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TestPluginRangeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\unzip.cpp
# End Source File
# Begin Source File

SOURCE=.\Util.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "EMCommonInc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Device\inc\EmAdbDevice.h
# End Source File
# Begin Source File

SOURCE=.\Device\inc\EmAdbScanner.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\EmBase.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\EmBase64.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\EmCharSet.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\EmConstants.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\EmCs.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\EmDiskInfo.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\EmDllRegister.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\EmErrorCode.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\EmFile.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\EmFileCache.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\EmFileInfo.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\EmGuid.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\EmHandy.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\EmMacros.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\EmMd5.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\EmMemory.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\EmOsVersion.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\EmProcess.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\EmRes.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\EmShell.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\EmStlStr.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\emstltypedef.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\EmStr.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\EmSz.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\EmThread.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\EmTime.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\EmTstr.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\EmTsz.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\EmTypes.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\EmUrl.h
# End Source File
# Begin Source File

SOURCE=.\Device\inc\EmUsbDevice.h
# End Source File
# Begin Source File

SOURCE=.\Device\inc\EmUsbScanner.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\EmUtil.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\EmVariantConvert.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\EmWstr.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\EmWsz.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\EtMfc.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\IEmNetStreamReceiver.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\IEmNetStreamSender.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\IEmStateNoticer.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\IEmTcpConnectCallback.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\IEmTcpServerAcceptCallback.h
# End Source File
# Begin Source File

SOURCE=.\Common\inc\tinystr.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\AdbOperator.h
# End Source File
# Begin Source File

SOURCE=.\ChargeImage.h
# End Source File
# Begin Source File

SOURCE=.\CommonDefine.h
# End Source File
# Begin Source File

SOURCE=.\DlgBatteryParam.h
# End Source File
# Begin Source File

SOURCE=.\FirmwareModifier.h
# End Source File
# Begin Source File

SOURCE=.\KernelLogoTool.h
# End Source File
# Begin Source File

SOURCE=.\MySpawnConsumer.h
# End Source File
# Begin Source File

SOURCE=.\NotePadBase.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SysCfgDlg.h
# End Source File
# Begin Source File

SOURCE=.\TestPluginRangeDlg.h
# End Source File
# Begin Source File

SOURCE=.\unzip.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\FirmwareModifier.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
