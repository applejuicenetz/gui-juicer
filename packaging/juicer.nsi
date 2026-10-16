;--------------------------------
;Include Modern UI
    !include "MUI2.nsh"
    !include "x64.nsh"

;--------------------------------
;General
    Unicode true
    !define COMPANY "appleJuiceNETZ"
    !define LINK_ABOUT "https://applejuicenetz.github.io"
    !define LINK_HELP "https://applejuicenetz.github.io/faq/"
    !define PRODUCT "appleJuice Juicer GUI"
    !define PRODUCT_SHORT "Juicer"
    !define EXE_NAME "Juicer"
    !define REG_URL_NAME "${COMPANY}.URI.${PRODUCT_SHORT}"
    !define INSTALLSIZE 16672

    InstallDirRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "InstallLocation"
    InstallDir "$PROGRAMFILES64\${COMPANY}\${PRODUCT_SHORT}"
    Name "${PRODUCT}"
    OutFile "${OUTPUT}/juicer-1.3.1-windows-x86_64-setup.exe"
    SetCompressor lzma
    RequestExecutionLevel admin
    ShowInstDetails show

;--------------------------------
;Interface Settings
    !define MUI_ICON "${SOURCE}\src\resources\juicer.ico"
    !define MUI_UNICON "${SOURCE}\src\resources\juicer.ico"
    !define MUI_ABORTWARNING
    !define MUI_FINISHPAGE_NOAUTOCLOSE

;--------------------------------
;Installer Settings
    !insertmacro MUI_PAGE_WELCOME
    !insertmacro MUI_PAGE_DIRECTORY
    !insertmacro MUI_PAGE_INSTFILES
    !insertmacro MUI_PAGE_FINISH

;--------------------------------
;Uninstaller Settings
    !insertmacro MUI_UNPAGE_CONFIRM
    !insertmacro MUI_UNPAGE_INSTFILES
    !insertmacro MUI_UNPAGE_FINISH

;--------------------------------
; Section Descriptions
    !insertmacro MUI_LANGUAGE "German"

Function .onInit
    ${IfNot} ${RunningX64}
        MessageBox MB_ICONSTOP "Juicer benötigt ein 64-Bit-Windows (x64)."
        Abort
    ${EndIf}
    SetRegView 64
FunctionEnd

Function un.onInit
    SetRegView 64
FunctionEnd

;--------------------------------
;Install Section
Section ""
    AddSize ${INSTALLSIZE}

    SetOutPath "$INSTDIR"
    File /oname=${EXE_NAME}.exe "${OUTPUT}/juicer-1.3.1-windows-x86_64.exe"

    WriteUninstaller "uninstaller.exe"

    CreateDirectory "$SMPROGRAMS\${COMPANY}"
    CreateShortcut "$SMPROGRAMS\${COMPANY}\${PRODUCT}.lnk" "$INSTDIR\${EXE_NAME}.exe"
    CreateShortcut "$desktop\${PRODUCT}.lnk" "$INSTDIR\${EXE_NAME}.exe"

    # uninstaller keys
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "DisplayName" "${PRODUCT}"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "DisplayVersion" "1.3.1"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "UninstallString" '"$INSTDIR\uninstaller.exe"'
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "InstallLocation" "$INSTDIR"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "DisplayIcon" '"$INSTDIR\${EXE_NAME}.exe",0'
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "Publisher" "${COMPANY}"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "HelpLink" "${LINK_HELP}"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "UrlInfoAbout" "${LINK_ABOUT}"
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "EstimatedSize" ${INSTALLSIZE}
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "NoModify" 1
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "NoRepair" 1

    # create ajfsp protocol (legacy)
    WriteRegStr HKCR "ajfsp" "" "URL: ajfsp Protocol"
    WriteRegStr HKCR "ajfsp" "URL Protocol" ""

    # create unique ajfsp capabilities
    WriteRegStr HKLM "SOFTWARE\${COMPANY}\${PRODUCT_SHORT}\Capabilities" "ApplicationName" "${PRODUCT_SHORT}"
    WriteRegStr HKLM "SOFTWARE\${COMPANY}\${PRODUCT_SHORT}\Capabilities" "ApplicationIcon" '"$INSTDIR\${EXE_NAME}.exe",0'
    WriteRegStr HKLM "SOFTWARE\${COMPANY}\${PRODUCT_SHORT}\Capabilities" "ApplicationDescription" "appleJuice GUI"
    WriteRegStr HKLM "SOFTWARE\${COMPANY}\${PRODUCT_SHORT}\Capabilities\URLAssociations" "ajfsp" "${REG_URL_NAME}"
    WriteRegStr HKLM "SOFTWARE\RegisteredApplications" "${PRODUCT_SHORT}" "Software\${COMPANY}\${PRODUCT_SHORT}\Capabilities"

    # create unique ajfsp classes
    WriteRegStr HKLM "SOFTWARE\Classes\${REG_URL_NAME}" "" "URL:ajfsp"
    WriteRegStr HKLM "SOFTWARE\Classes\${REG_URL_NAME}" "URL Protocol" ""
    WriteRegStr HKLM "SOFTWARE\Classes\${REG_URL_NAME}\DefaultIcon" "" '"$INSTDIR\${EXE_NAME}.exe",0'
    WriteRegStr HKLM "SOFTWARE\Classes\${REG_URL_NAME}\shell\open\command" "" '"$INSTDIR\${EXE_NAME}.exe" "%1"'

    # create unique ajfsp handler
    WriteRegStr HKCR "${REG_URL_NAME}" "" "URL:ajfsp"
    WriteRegStr HKCR "${REG_URL_NAME}" "URL Protocol" ""
    WriteRegStr HKCR "${REG_URL_NAME}\DefaultIcon" "" '"$INSTDIR\${EXE_NAME}.exe",0'
    WriteRegStr HKCR "${REG_URL_NAME}\shell\open\command" "" '"$INSTDIR\${EXE_NAME}.exe" "%1"'
SectionEnd

;--------------------------------
; Uninstaller
Section "Uninstall"
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}"
    DeleteRegKey HKLM "SOFTWARE\Classes\${REG_URL_NAME}"
    DeleteRegKey HKLM "SOFTWARE\${COMPANY}\${PRODUCT_SHORT}\Capabilities"
    DeleteRegValue HKLM "SOFTWARE\RegisteredApplications" "${PRODUCT_SHORT}"
    DeleteRegKey HKCR "${REG_URL_NAME}"
    Delete "$desktop\${PRODUCT}.lnk"
    Delete "$SMPROGRAMS\${COMPANY}\${PRODUCT}.lnk"
    RMDir "$SMPROGRAMS\${COMPANY}"
    RMDir /r /REBOOTOK $INSTDIR
SectionEnd
