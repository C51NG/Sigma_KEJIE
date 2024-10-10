# Microsoft Developer Studio Project File - Name="Sys" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Sys - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Sys.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Sys.mak" CFG="Sys - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Sys - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Sys - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Sys - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Sys - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Sys - Win32 Release"
# Name "Sys - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Others.cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CfgCtrlBar.cpp
# End Source File
# Begin Source File

SOURCE=.\CfgTabCtrlBar.cpp
# End Source File
# Begin Source File

SOURCE=.\MdiClient.cpp
# End Source File
# Begin Source File

SOURCE=.\MTabWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\PopupMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\TabCtrlBarDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\TabCtrlDoc.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ADDigitView.cpp
# End Source File
# Begin Source File

SOURCE=.\ADDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ADFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ADHistDigitView.cpp
# End Source File
# Begin Source File

SOURCE=.\ADHistDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ADHistFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ADHistWaveView.cpp
# End Source File
# Begin Source File

SOURCE=.\ADScopeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ADStatusView.cpp
# End Source File
# Begin Source File

SOURCE=.\ADThread.cpp
# End Source File
# Begin Source File

SOURCE=.\ADWaveView.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCalibration.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ParaCfgView.cpp
# End Source File
# Begin Source File

SOURCE=.\QSortData.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Sys.cpp
# End Source File
# Begin Source File

SOURCE=.\Sys.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Others .h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CfgCtrlBar.h
# End Source File
# Begin Source File

SOURCE=.\CfgTabCtrlBar.h
# End Source File
# Begin Source File

SOURCE=.\MdiClient.h
# End Source File
# Begin Source File

SOURCE=.\MTabWnd.h
# End Source File
# Begin Source File

SOURCE=.\PopupMenu.h
# End Source File
# Begin Source File

SOURCE=.\TabCtrlBarDoc.h
# End Source File
# Begin Source File

SOURCE=.\TabCtrlDoc.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ADDigitView.h
# End Source File
# Begin Source File

SOURCE=.\ADDoc.h
# End Source File
# Begin Source File

SOURCE=.\ADFrm.h
# End Source File
# Begin Source File

SOURCE=.\ADHistDigitView.h
# End Source File
# Begin Source File

SOURCE=.\ADHistDoc.h
# End Source File
# Begin Source File

SOURCE=.\ADHistFrm.h
# End Source File
# Begin Source File

SOURCE=.\ADHistWaveView.h
# End Source File
# Begin Source File

SOURCE=.\ADScopeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ADStatusView.h
# End Source File
# Begin Source File

SOURCE=.\ADThread.h
# End Source File
# Begin Source File

SOURCE=.\ADWaveView.h
# End Source File
# Begin Source File

SOURCE=.\DlgCalibration.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\ParaCfgView.h
# End Source File
# Begin Source File

SOURCE=.\PCI8532.h
# End Source File
# Begin Source File

SOURCE=.\PXI8502.h
# End Source File
# Begin Source File

SOURCE=.\QSortData.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Sys.h
# End Source File
# Begin Source File

SOURCE=.\UserDef.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE="C:\Documents and Settings\LYB\桌面\Advanced\RES\AD.ICO"
# End Source File
# Begin Source File

SOURCE=..\..\Advanced\Res\AD.ICO
# End Source File
# Begin Source File

SOURCE=.\Res\AD.ico
# End Source File
# Begin Source File

SOURCE=.\Res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bitmap6.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bitmap7.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bitmap8.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\cursor1.cur
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\LYB\桌面\Advanced\RES\Hist.ico"
# End Source File
# Begin Source File

SOURCE=..\..\Advanced\Res\Hist.ico
# End Source File
# Begin Source File

SOURCE=.\Res\Hist.ico
# End Source File
# Begin Source File

SOURCE=.\Res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\Res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\Res\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\LYB\桌面\Advanced\RES\icon2.ico"
# End Source File
# Begin Source File

SOURCE=.\Res\icon2.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\LYB\桌面\Advanced\RES\idr_ad1.ico"
# End Source File
# Begin Source File

SOURCE=.\Res\idr_adst.ico
# End Source File
# Begin Source File

SOURCE=..\..\Advanced\Res\IDR_DIO.ICO
# End Source File
# Begin Source File

SOURCE=.\Res\idr_dio.ico
# End Source File
# Begin Source File

SOURCE=.\Res\litebulb.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\LYB\桌面\Advanced\RES\MainFrm.ico"
# End Source File
# Begin Source File

SOURCE=..\..\Advanced\Res\MainFrm.ico
# End Source File
# Begin Source File

SOURCE=.\Res\MainFrm.ico
# End Source File
# Begin Source File

SOURCE=.\Res\pci.ico
# End Source File
# Begin Source File

SOURCE=.\res\ShowDataDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Sys.ico
# End Source File
# Begin Source File

SOURCE=.\res\Sys.rc2
# End Source File
# Begin Source File

SOURCE=.\Res\TabImage.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\tabview.bmp
# End Source File
# Begin Source File

SOURCE=..\test\res\test.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=".\Res\USB图标(大).bmp"
# End Source File
# Begin Source File

SOURCE=".\Res\设备图标(大).bmp"
# End Source File
# Begin Source File

SOURCE=".\Res\设备图标(小).bmp"
# End Source File
# Begin Source File

SOURCE=".\Res\设备图标1(大).bmp"
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\Sys.reg
# End Source File
# End Target
# End Project
# Section Sys : {72ADFD6C-2C39-11D0-9903-00A0C91BC942}
# 	1:24:IDR_POPUP_HISTDIGIT_VIEW:120
# End Section
# Section Sys : {72ADFD54-2C39-11D0-9903-00A0C91BC942}
# 	1:17:CG_IDS_DIDYOUKNOW:125
# 	1:22:CG_IDS_TIPOFTHEDAYMENU:124
# 	1:18:CG_IDS_TIPOFTHEDAY:123
# 	1:22:CG_IDS_TIPOFTHEDAYHELP:130
# 	1:19:CG_IDP_FILE_CORRUPT:127
# 	1:7:IDD_TIP:122
# 	1:13:IDB_LIGHTBULB:121
# 	1:18:CG_IDS_FILE_ABSENT:126
# 	2:17:CG_IDS_DIDYOUKNOW:CG_IDS_DIDYOUKNOW
# 	2:7:CTipDlg:CTipDlg
# 	2:22:CG_IDS_TIPOFTHEDAYMENU:CG_IDS_TIPOFTHEDAYMENU
# 	2:18:CG_IDS_TIPOFTHEDAY:CG_IDS_TIPOFTHEDAY
# 	2:12:CTIP_Written:OK
# 	2:22:CG_IDS_TIPOFTHEDAYHELP:CG_IDS_TIPOFTHEDAYHELP
# 	2:2:BH:
# 	2:19:CG_IDP_FILE_CORRUPT:CG_IDP_FILE_CORRUPT
# 	2:7:IDD_TIP:IDD_TIP
# 	2:8:TipDlg.h:TipDlg.h
# 	2:13:IDB_LIGHTBULB:IDB_LIGHTBULB
# 	2:18:CG_IDS_FILE_ABSENT:CG_IDS_FILE_ABSENT
# 	2:10:TipDlg.cpp:TipDlg.cpp
# End Section
