;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"
  !include "LogicLib.nsh"

  !define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\win-install.ico"
  !define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\win-uninstall.ico"  

;--------------------------------
;General

  BrandingText "xiangzhai83@gmail.com"

  ;Name and file
  Name "HelloQQ"
  OutFile "HelloQQ Setup 1.0.0.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\HelloQQ"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKLM "Software\HelloQQ" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel admin

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_DIRECTORY
 
  !insertmacro MUI_PAGE_INSTFILES

;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Var AppInitDlls

Section

  SetOutPath "$INSTDIR"
  
  ;Store installation folder
  WriteRegStr HKLM "Software\HelloQQ" "" $INSTDIR

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

  ReadRegStr $AppInitDlls HKLM "Software\Microsoft\Windows NT\CurrentVersion\Windows" "AppInit_DLLs"
 
  ${If} $AppInitDlls == ""
    WriteRegStr HKLM "Software\Microsoft\Windows NT\CurrentVersion\Windows" \
                    "AppInit_DLLs" "$INSTDIR\HelloQQ.dll"
  ${Else}
    WriteRegStr HKLM "Software\Microsoft\Windows NT\CurrentVersion\Windows" \
                    "AppInit_DLLs" "$AppInitDlls;$INSTDIR\HelloQQ.dll"
  ${EndIf}

  ReadRegStr $AppInitDlls HKLM "Software\Microsoft\Windows NT\CurrentVersion\Windows" "AppInit_DLLs"
  DetailPrint "AppInit_DLLs: $AppInitDlls"

  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\HelloQQ" \
                 "DisplayName" "HelloQQ 1.0.0"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\HelloQQ" \
                 "UninstallString" "$\"$INSTDIR\Uninstall.exe$\""

SectionEnd
 
;--------------------------------
;Uninstaller Section

Section "Uninstall"

  Delete "$INSTDIR\*.*"
  RMDir "$INSTDIR"
  
  DeleteRegValue HKLM "Software\Microsoft\Windows NT\CurrentVersion\Windows\AppInit_DLLs" \
                    "$INSTDIR\HelloQQ.dll"
  DeleteRegKey /ifempty HKLM "Software\HelloQQ"
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\HelloQQ"

SectionEnd
