;Nmap Installer 
;Started by Bo Jiang @ 08/26/2005 06:07PM 
 
;-------------------------------- 
;Include Modern UI 
 
  !include "MUI.nsh" 
  !include "AddToPath.nsh" 
 
;-------------------------------- 
;General 
 
  ;Name and file 
  Name "Nmap" 
  OutFile "..\pack\Nmap_with_NmapSI4-svn_GUI.exe"

  ;Required for removing shortcuts
  RequestExecutionLevel admin

  ;Default installation folder 
  InstallDir "$PROGRAMFILES\Nmap" 
   
  ;Get installation folder from registry if available 
  InstallDirRegKey HKCU "Software\Nmap" "" 
 
  !define VERSION "4.68"
  VIProductVersion "4.68.0.0"
  VIAddVersionKey /LANG=1033 "FileVersion" "${VERSION}"
  VIAddVersionKey /LANG=1033 "ProductName" "Nmap" 
  VIAddVersionKey /LANG=1033 "CompanyName" "Insecure.org" 
  VIAddVersionKey /LANG=1033 "InternalName" "NmapInstaller.exe" 
  VIAddVersionKey /LANG=1033 "LegalCopyright" "Copyright (c) Insecure.Com LLC (fyodor@insecure.org)" 
  VIAddVersionKey /LANG=1033 "LegalTrademark" "NMAP" 
  VIAddVersionKey /LANG=1033 "FileDescription" "Nmap installer" 
   
;-------------------------------- 
;Interface Settings 
 
  !define MUI_ABORTWARNING 
 
;-------------------------------- 
;Pages 
 
  !insertmacro MUI_PAGE_LICENSE "..\COPYING.txt"
  !insertmacro MUI_PAGE_COMPONENTS 
  !insertmacro MUI_PAGE_DIRECTORY 
  !insertmacro MUI_PAGE_INSTFILES 
  !insertmacro MUI_UNPAGE_CONFIRM 
  !insertmacro MUI_UNPAGE_INSTFILES 
  Page custom shortcutsPage makeShortcuts
  Page custom finalPage doFinal
   
;-------------------------------- 
;Languages 
  
  !insertmacro MUI_LANGUAGE "English" 

;--------------------------------
;Variables

Var nmapsi4set

;--------------------------------
;Reserves

ReserveFile "shortcuts.ini"
ReserveFile "final.ini"
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS

;--------------------------------
;Functions

Function .onInit
  !insertmacro MUI_INSTALLOPTIONS_EXTRACT "shortcuts.ini"
  !insertmacro MUI_INSTALLOPTIONS_EXTRACT "final.ini"
FunctionEnd


Function shortcutsPage
  StrCmp $nmapsi4set "" skip

  !insertmacro MUI_HEADER_TEXT "Create Shortcuts" ""
  !insertmacro MUI_INSTALLOPTIONS_DISPLAY "shortcuts.ini"  

  skip:
FunctionEnd

Function makeShortcuts
  StrCmp $nmapsi4set "" skip

  SetOutPath "$INSTDIR\nmapsi4"

  ReadINIStr $0 "$PLUGINSDIR\shortcuts.ini" "Field 1" "State"
  StrCmp $0 "0" skipdesktop

  skipdesktop:

  ReadINIStr $0 "$PLUGINSDIR\shortcuts.ini" "Field 2" "State"
  StrCmp $0 "0" skipstartmenu
  ;CreateDirectory "$SMPROGRAMS\Nmap"

  skipstartmenu:

  skip:
FunctionEnd

Function finalPage
  ; diplay a page saying everything's finished
  !insertmacro MUI_HEADER_TEXT "Finished" "Thank you for installing Nmap"
  !insertmacro MUI_INSTALLOPTIONS_DISPLAY "final.ini"
FunctionEnd

Function doFinal
 ; don't need to do anything
FunctionEnd

;-------------------------------- 
;Installer Sections 
 
Section "Nmap Core Files" SecCore 

  StrCpy $R0 $INSTDIR "" -2
  StrCmp $R0 ":\" bad_key_install
  StrCpy $R0 $INSTDIR "" -14
  StrCmp $R0 "\Program Files" bad_key_install
  StrCpy $R0 $INSTDIR "" -8
  StrCmp $R0 "\Windows" bad_key_install
  StrCpy $R0 $INSTDIR "" -6
  StrCmp $R0 "\WinNT" bad_key_install
  StrCpy $R0 $INSTDIR "" -9
  StrCmp $R0 "\system32" bad_key_install
  StrCpy $R0 $INSTDIR "" -8
  StrCmp $R0 "\Desktop" bad_key_install
  StrCpy $R0 $INSTDIR "" -22
  StrCmp $R0 "\Documents and Settings" bad_key_install
  StrCpy $R0 $INSTDIR "" -13
  StrCmp $R0 "\My Documents" bad_key_install probably_safe_key_install
  bad_key_install:
    MessageBox MB_YESNO "It may not be safe to uninstall the previous installation of Nmap from the directory '$INSTDIR'.$\r$\nContinue anyway (not recommended)?" IDYES probably_safe_key_install 
    Abort "Install aborted by user" 
  probably_safe_key_install:

  ;Delete specific subfolders (NB: custom scripts in scripts folder will be lost)
  RMDir /r "$INSTDIR\nselib"
  RMDir /r "$INSTDIR\nselib-bin"
  RMDir /r "$INSTDIR\scripts"
  RMDir /r "$INSTDIR\NmapSI4"

  SetOutPath "$INSTDIR" 

  SetOverwrite on 
  File ..\CHANGELOG
  File ..\COPYING.txt
  File ..\nmapsi4.ico
  File ..\nmap-mac-prefixes
  File ..\nmap-os-db
  File ..\nmap-protocols
  File ..\nmap-rpc
  File ..\nmap-service-probes
  File ..\nmap-services
  File ..\nmap.exe
  File ..\nmap.xsl
  File ..\nmap_performance.reg 
  File ..\README-WIN32
  File ..\libeay32.dll
  File ..\ssleay32.dll
  File /r /x mswin32 /x .svn ..\scripts
  File /r /x mswin32 /x .svn ..\nselib
  File /r /x mswin32 /x .svn ..\nselib-bin
  
  ;Store installation folder 
  WriteRegStr HKCU "Software\Nmap" "" $INSTDIR 

  ;Silent install of Visual C++ 2008 runtime components
  File ..\vcredist_x86.exe
  ExecWait '"$INSTDIR\vcredist_x86.exe" /q:a /c:$TEMP\"msiexec /i vcredist.msi /qn "'
  ;ExecWait '"$INSTDIR\vcredist_x86.exe" /q'
  Delete "$INSTDIR\vcredist_x86.exe" 

  ;Create uninstaller 
  WriteUninstaller "$INSTDIR\Uninstall.exe" 
   
  ; Register Nmap with add/remove programs 
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Nmap" "DisplayName" "Nmap ${VERSION}" 
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Nmap" "UninstallString" '"$INSTDIR\uninstall.exe"' 
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Nmap" "DisplayIcon" '"$INSTDIR\nmapsi4.ico"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Nmap" "NoModify" 1 
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Nmap" "NoRepair" 1 
  
SectionEnd 
 
Section "Register Nmap Path" SecRegisterPath 
  PUSH $INSTDIR 
  Call AddToPath 
SectionEnd 
 
Section "WinPcap 4.02" SecWinPcap 
  SetOutPath "$INSTDIR" 
  SetOverwrite on 
  File ..\winpcap-nmap-4.02.exe 
  ; If the Nmap installer was launched using /S then pass some arguments to WinPcap
  IfSilent winpcap_silent winpcap_loud
  winpcap_silent:
    ExecWait '"$INSTDIR\winpcap-nmap-4.02.exe" /S /D=$\""$PROGRAMFILES\WinPcap\"$\"' 
  Goto delete_winpcap
  winpcap_loud:
    ExecWait '"$INSTDIR\winpcap-nmap-4.02.exe"' 
  delete_winpcap:
  Delete "$INSTDIR\winpcap-nmap-4.02.exe" 
SectionEnd 

Section "Network Performance Improvements" SecPerfRegistryMods 
  SetOutPath "$INSTDIR" 
  SetOverwrite on 
  File ..\nmap_performance.reg 
  Exec 'regedt32 /S "$INSTDIR\nmap_performance.reg"' 
SectionEnd 

Section "NmapSI4 (GUI Frontend)" SecNmapSI4
  SetOutPath "$INSTDIR" 
  SetOverwrite on 
  File ..\pack\NmapSI4-svn.exe
  ExecWait '"$INSTDIR\NmapSI4-svn.exe"'
  Delete "$INSTDIR\NmapSI4-svn.exe"
  StrCpy $nmapsi4set "true"
SectionEnd
 
;-------------------------------- 
;Descriptions 
 
  ;Component strings 
  LangString DESC_SecCore ${LANG_ENGLISH} "Installs Nmap executable, NSE scripts and Visual C++ 2008 runtime components"
  LangString DESC_SecRegisterPath ${LANG_ENGLISH} "Registers Nmap path to System path so you can execute it from any directory" 
  LangString DESC_SecWinPcap ${LANG_ENGLISH} "Installs WinPcap 4.0 (required for most Nmap scans unless it is already installed)" 
  LangString DESC_SecPerfRegistryMods ${LANG_ENGLISH} "Modifies Windows registry values to improve TCP connect scan performance.  Recommended." 
  LangString DESC_SecNmapSI4 ${LANG_ENGLISH} "Installs NmapSI4, the Qt4 graphical user interface.  Recommended."

  ;Assign language strings to sections 
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN 
    !insertmacro MUI_DESCRIPTION_TEXT ${SecCore} $(DESC_SecCore) 
    !insertmacro MUI_DESCRIPTION_TEXT ${SecWinPcap} $(DESC_SecWinPcap) 
    !insertmacro MUI_DESCRIPTION_TEXT ${SecRegisterPath} $(DESC_SecRegisterPath) 
    !insertmacro MUI_DESCRIPTION_TEXT ${SecPerfRegistryMods} $(DESC_SecPerfRegistryMods) 
    !insertmacro MUI_DESCRIPTION_TEXT ${SecNmapSI4} $(DESC_SecNmapSI4)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END 
;-------------------------------- 
;Uninstaller Section 
 
Section "Uninstall" 

  StrCpy $R0 $INSTDIR "" -2
  StrCmp $R0 ":\" bad_key_uninstall
  StrCpy $R0 $INSTDIR "" -14
  StrCmp $R0 "\Program Files" bad_key_uninstall
  StrCpy $R0 $INSTDIR "" -8
  StrCmp $R0 "\Windows" bad_key_uninstall
  StrCpy $R0 $INSTDIR "" -6
  StrCmp $R0 "\WinNT" bad_key_uninstall
  StrCpy $R0 $INSTDIR "" -9
  StrCmp $R0 "\system32" bad_key_uninstall
  StrCpy $R0 $INSTDIR "" -8
  StrCmp $R0 "\Desktop" bad_key_uninstall
  StrCpy $R0 $INSTDIR "" -22
  StrCmp $R0 "\Documents and Settings" bad_key_uninstall
  StrCpy $R0 $INSTDIR "" -13
  StrCmp $R0 "\My Documents" bad_key_uninstall probably_safe_key_uninstall
  bad_key_uninstall:
    MessageBox MB_YESNO "It may not be safe to uninstall Nmap from the directory '$INSTDIR'.$\r$\nContinue anyway (not recommended)?" IDYES probably_safe_key_uninstall 
    Abort "Uninstall aborted by user" 
  probably_safe_key_uninstall:

  IfFileExists $INSTDIR\nmap.exe nmap_installed 
    MessageBox MB_YESNO "It does not appear that Nmap is installed in the directory '$INSTDIR'.$\r$\nContinue anyway (not recommended)?" IDYES nmap_installed 
    Abort "Uninstall aborted by user" 

  SetDetailsPrint textonly 
  DetailPrint "Uninstalling Files..." 
  SetDetailsPrint listonly 
   
  nmap_installed: 
  Delete "$INSTDIR\CHANGELOG" 
  Delete "$INSTDIR\COPYING.txt"
  Delete "$INSTDIR\nmap-mac-prefixes" 
  Delete "$INSTDIR\nmap-os-db" 
  Delete "$INSTDIR\nmap-protocols" 
  Delete "$INSTDIR\nmap-rpc" 
  Delete "$INSTDIR\nmap-service-probes" 
  Delete "$INSTDIR\nmap-services" 
  Delete "$INSTDIR\nmap.exe" 
  Delete "$INSTDIR\nmap.xsl" 
  Delete "$INSTDIR\nmap_performance.reg" 
  Delete "$INSTDIR\README-WIN32" 
  Delete "$INSTDIR\nmapsi4.ico"
  Delete "$INSTDIR\libeay32.dll"
  Delete "$INSTDIR\ssleay32.dll"
  Delete "$INSTDIR\winpcap-nmap*.exe"
  ;Delete specific subfolders (NB: custom scripts in scripts folder will be lost)
  RMDir /r "$INSTDIR\nselib"
  RMDir /r "$INSTDIR\nselib-bin"
  RMDir /r "$INSTDIR\scripts"
  RMDir /r "$INSTDIR\nmapsi4"
 
  Delete "$INSTDIR\Uninstall.exe" 

  ;Removes folder if it's now empty
  RMDir "$INSTDIR"
 
  SetDetailsPrint textonly 
  DetailPrint "Deleting Registry Keys..." 
  SetDetailsPrint listonly 
  DeleteRegKey /ifempty HKCU "Software\Nmap" 
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Nmap" 
  SetDetailsPrint textonly 
  DetailPrint "Unregistering Nmap Path..." 
  Push $INSTDIR 
  Call un.RemoveFromPath 

  Delete "$DESKTOP\Nmap - NmapSI4 GUI.lnk"
  Delete "$SMPROGRAMS\Nmap\Nmap - NmapSI4 GUI.lnk"
  RMDIR "$SMPROGRAMS\Nmap"

  SetDetailsPrint both 
SectionEnd 
