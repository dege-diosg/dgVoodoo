; #########################################################################
;
;      WINDOWS.INC for 32 bit MASM (Version 1.25a)  22 October 2001
;
;      This file contains the equates and structures for
;      writing 32 bit MASM assembler language programs.
;
;      This include file is a joint project maintained by
;      Iczelion < Iczelion@galaxycorp.com >
;      hutch < hutch@movsd.com > 
;
;      Iczelion's homepage: http://win32asm.cjb.net
;      hutch's homepage: http://masm32.cjb.net
;
; #########################################################################

;   This macro is specifically for programmers who use the
;   different format include files created by l2extia.exe.
;   NOTE : windows.inc MUST be included before and of the
;   include file created with l2extia.exe as they require
;   this macro to expand the parameters.

; #########################################################################

    ArgCount MACRO number
      LOCAL txt
      txt equ <typedef PROTO :DWORD>
        REPEAT number - 1
          txt CATSTR txt,<,:DWORD>
        ENDM
      EXITM <txt>
    ENDM

    pr0  typedef PROTO
    pr1  ArgCount(1)
    pr2  ArgCount(2)
    pr3  ArgCount(3)
    pr4  ArgCount(4)
    pr5  ArgCount(5)
    pr6  ArgCount(6)
    pr7  ArgCount(7)
    pr8  ArgCount(8)
    pr9  ArgCount(9)
    pr10 ArgCount(10)
    pr11 ArgCount(11)
    pr12 ArgCount(12)
    pr13 ArgCount(13)
    pr14 ArgCount(14)
    pr15 ArgCount(15)
    pr16 ArgCount(16)
    pr17 ArgCount(17)
    pr18 ArgCount(18)
    pr19 ArgCount(19)
    pr20 ArgCount(20)
    pr21 ArgCount(21)
    pr22 ArgCount(22)
    pr23 ArgCount(23)
    pr24 ArgCount(24)
    pr25 ArgCount(25)

; @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

wsprintfA PROTO C :ptr BYTE, :ptr BYTE, :VARARG
wsprintf equ <wsprintfA>

TRUE                                 equ 1
FALSE                                equ 0
NULL                                 equ 0
Normal                               equ 000000h
ReadOnly                             equ 000001h
Hidden                               equ 000010h
System                               equ 000100h
vLabel                               equ 001000h
SubDir                               equ 010000h
Archive                              equ 100000h
Black                                equ 000000h
Blue                                 equ 0FF0000h
Green                                equ 00FF00h
Cyan                                 equ 0FFFF00h
Red                                  equ 0000FFh
Magenta                              equ 0FF00FFh
Yellow                               equ 00FFFFh
White                                equ 0FFFFFFh
Gray                                 equ 080808h
MAXIMUM_SUPPORTED_EXTENSION          equ 512
ANYSIZE_ARRAY                        equ 1
INVALID_HANDLE_VALUE                 equ -1
DELETE                               equ 10000h
READ_CONTROL                         equ 20000h
WRITE_DAC                            equ 40000h
WRITE_OWNER                          equ 80000h
SYNCHRONIZE                          equ 100000h
STANDARD_RIGHTS_READ                 equ READ_CONTROL
STANDARD_RIGHTS_WRITE                equ READ_CONTROL
STANDARD_RIGHTS_EXECUTE              equ READ_CONTROL
STANDARD_RIGHTS_REQUIRED             equ 0F0000h
STANDARD_RIGHTS_ALL                  equ 1F0000h
SPECIFIC_RIGHTS_ALL                  equ 0FFFFh
SID_REVISION                         equ 1
SID_MAX_SUB_AUTHORITIES              equ 15
SID_RECOMMENDED_SUB_AUTHORITIES      equ 1
SidTypeUser                          equ 1
SidTypeGroup                         equ 2
SidTypeDomain                        equ 3
SidTypeAlias                         equ 4
SidTypeWellKnownGroup                equ 5
SidTypeDeletedAccount                equ 6
SidTypeInvalid                       equ 7
SidTypeUnknown                       equ 8
SECURITY_NULL_RID                    equ 0h
SECURITY_WORLD_RID                   equ 0h
SECURITY_LOCAL_RID                   equ 0h
SECURITY_CREATOR_OWNER_RID           equ 0h
SECURITY_CREATOR_GROUP_RID           equ 1h
SECURITY_DIALUP_RID                  equ 1h
SECURITY_NETWORK_RID                 equ 2h
SECURITY_BATCH_RID                   equ 3h
SECURITY_INTERACTIVE_RID             equ 4h
SECURITY_SERVICE_RID                 equ 6h
SECURITY_ANONYMOUS_LOGON_RID         equ 7h
SECURITY_LOGON_IDS_RID               equ 5h
SECURITY_LOCAL_SYSTEM_RID            equ 12h
SECURITY_NT_NON_UNIQUE               equ 15h
SECURITY_BUILTIN_DOMAIN_RID          equ 20h
DOMAIN_USER_RID_ADMIN                equ 1F4h
DOMAIN_USER_RID_GUEST                equ 1F5h
DOMAIN_GROUP_RID_ADMINS              equ 200h
DOMAIN_GROUP_RID_USERS               equ 201h
DOMAIN_GROUP_RID_GUESTS              equ 202h
DOMAIN_ALIAS_RID_ADMINS              equ 220h
DOMAIN_ALIAS_RID_USERS               equ 221h
DOMAIN_ALIAS_RID_GUESTS              equ 222h
DOMAIN_ALIAS_RID_POWER_USERS         equ 223h
DOMAIN_ALIAS_RID_ACCOUNT_OPS         equ 224h
DOMAIN_ALIAS_RID_SYSTEM_OPS          equ 225h
DOMAIN_ALIAS_RID_PRINT_OPS           equ 226h
DOMAIN_ALIAS_RID_BACKUP_OPS          equ 227h
DOMAIN_ALIAS_RID_REPLICATOR          equ 228h
SE_GROUP_MANDATORY                   equ 1h
SE_GROUP_ENABLED_BY_DEFAULT          equ 2h
SE_GROUP_ENABLED                     equ 4h
SE_GROUP_OWNER                       equ 8h
SE_GROUP_LOGON_ID                    equ 0C0000000h
FILE_BEGIN                           equ 0
FILE_CURRENT                         equ 1
FILE_END                             equ 2
FILE_FLAG_WRITE_THROUGH              equ 80000000h
FILE_FLAG_OVERLAPPED                 equ 40000000h
FILE_FLAG_NO_BUFFERING               equ 20000000h
FILE_FLAG_RANDOM_ACCESS              equ 10000000h
FILE_FLAG_SEQUENTIAL_SCAN            equ 8000000h
FILE_FLAG_DELETE_ON_CLOSE            equ 4000000h
FILE_FLAG_BACKUP_SEMANTICS           equ 2000000h
FILE_FLAG_POSIX_SEMANTICS            equ 1000000h
CREATE_NEW                           equ 1
CREATE_ALWAYS                        equ 2
OPEN_EXISTING                        equ 3
OPEN_ALWAYS                          equ 4
TRUNCATE_EXISTING                    equ 5
PIPE_ACCESS_INBOUND                  equ 1h
PIPE_ACCESS_OUTBOUND                 equ 2h
PIPE_ACCESS_DUPLEX                   equ 3h
PIPE_CLIENT_END                      equ 0h
PIPE_SERVER_END                      equ 1h
PIPE_WAIT                            equ 0h
PIPE_NOWAIT                          equ 1h
PIPE_READMODE_BYTE                   equ 0h
PIPE_READMODE_MESSAGE                equ 2h
PIPE_TYPE_BYTE                       equ 0h
PIPE_TYPE_MESSAGE                    equ 4h
PIPE_UNLIMITED_INSTANCES             equ 255
SECURITY_CONTEXT_TRACKING            equ 40000h
SECURITY_EFFECTIVE_ONLY              equ 80000h
SECURITY_SQOS_PRESENT                equ 100000h
SECURITY_VALID_SQOS_FLAGS            equ 1F0000h
SP_SERIALCOMM                        equ 1h
PST_UNSPECIFIED                      equ 0h
PST_RS232                            equ 1h
PST_PARALLELPORT                     equ 2h
PST_RS422                            equ 3h
PST_RS423                            equ 4h
PST_RS449                            equ 5h
PST_FAX                              equ 21h
PST_SCANNER                          equ 22h
PST_NETWORK_BRIDGE                   equ 100h
PST_LAT                              equ 101h
PST_TCPIP_TELNET                     equ 102h
PST_X25                              equ 103h
PCF_DTRDSR                           equ 1h
PCF_RTSCTS                           equ 2h
PCF_RLSD                             equ 4h
PCF_PARITY_CHECK                     equ 8h
PCF_XONXOFF                          equ 10h
PCF_SETXCHAR                         equ 20h
PCF_TOTALTIMEOUTS                    equ 40h
PCF_INTTIMEOUTS                      equ 80h
PCF_SPECIALCHARS                     equ 100h
PCF_16BITMODE                        equ 200h
DLL_PROCESS_DETACH                   equ 0
DLL_PROCESS_ATTACH                   equ 1
DLL_THREAD_ATTACH                    equ 2
DLL_THREAD_DETACH                    equ 3
SP_PARITY                            equ 1h
SP_BAUD                              equ 2h
SP_DATABITS                          equ 4h
SP_STOPBITS                          equ 8h
SP_HANDSHAKING                       equ 10h
SP_PARITY_CHECK                      equ 20h
SP_RLSD                              equ 40h
BAUD_075                             equ 1h
BAUD_110                             equ 2h
BAUD_134_5                           equ 4h
BAUD_150                             equ 8h
BAUD_300                             equ 10h
BAUD_600                             equ 20h
BAUD_1200                            equ 40h
BAUD_1800                            equ 80h
BAUD_2400                            equ 100h
BAUD_4800                            equ 200h
BAUD_7200                            equ 400h
BAUD_9600                            equ 800h
BAUD_14400                           equ 1000h
BAUD_19200                           equ 2000h
BAUD_38400                           equ 4000h
BAUD_56K                             equ 8000h
BAUD_128K                            equ 10000h
BAUD_115200                          equ 20000h
BAUD_57600                           equ 40000h
BAUD_USER                            equ 10000000h
DATABITS_5                           equ 1h
DATABITS_6                           equ 2h
DATABITS_7                           equ 4h
DATABITS_8                           equ 8h
DATABITS_16                          equ 10h
DATABITS_16X                         equ 20h
STOPBITS_10                          equ 1h
STOPBITS_15                          equ 2h
STOPBITS_20                          equ 4h
PARITY_NONE                          equ 100h
PARITY_ODD                           equ 200h
PARITY_EVEN                          equ 400h
PARITY_MARK                          equ 800h
PARITY_SPACE                         equ 1000h
DTR_CONTROL_DISABLE                  equ 0h
DTR_CONTROL_ENABLE                   equ 1h
DTR_CONTROL_HANDSHAKE                equ 2h
RTS_CONTROL_DISABLE                  equ 0h
RTS_CONTROL_ENABLE                   equ 1h
RTS_CONTROL_HANDSHAKE                equ 2h
RTS_CONTROL_TOGGLE                   equ 3h
GMEM_FIXED                           equ 0h
GMEM_MOVEABLE                        equ 2h
GMEM_NOCOMPACT                       equ 10h
GMEM_NODISCARD                       equ 20h
GMEM_ZEROINIT                        equ 40h
GMEM_MODIFY                          equ 80h
GMEM_DISCARDABLE                     equ 100h
GMEM_NOT_BANKED                      equ 1000h
GMEM_SHARE                           equ 2000h
GMEM_DDESHARE                        equ 2000h
GMEM_NOTIFY                          equ 4000h
GMEM_LOWER                           equ GMEM_NOT_BANKED
GMEM_VALID_FLAGS                     equ 7F72h
GMEM_INVALID_HANDLE                  equ 8000h
GMEM_DISCARDED                       equ 4000h
GMEM_LOCKCOUNT                       equ 0FFh
GHND                                 equ GMEM_MOVEABLE OR GMEM_ZEROINIT
GPTR                                 equ GMEM_FIXED OR GMEM_ZEROINIT
LMEM_FIXED                           equ 0h
LMEM_MOVEABLE                        equ 2h
LMEM_NOCOMPACT                       equ 10h
LMEM_NODISCARD                       equ 20h
LMEM_ZEROINIT                        equ 40h
LMEM_MODIFY                          equ 80h
LMEM_DISCARDABLE                     equ 0F00h
LMEM_VALID_FLAGS                     equ 0F72h
LMEM_INVALID_HANDLE                  equ 8000h
LHND                                 equ LMEM_MOVEABLE + LMEM_ZEROINIT
LPTR                                 equ LMEM_FIXED + LMEM_ZEROINIT
NONZEROLHND                          equ LMEM_MOVEABLE
NONZEROLPTR                          equ LMEM_FIXED
LMEM_DISCARDED                       equ 4000h
LMEM_LOCKCOUNT                       equ 0FFh
DEBUG_PROCESS                        equ 1h
DEBUG_ONLY_THIS_PROCESS              equ 2h
CREATE_SUSPENDED                     equ 4h
CREATE_SEPARATE_WOW_VDM              equ 800h
DETACHED_PROCESS                     equ 8h
CREATE_NEW_CONSOLE                   equ 10h
NORMAL_PRIORITY_CLASS                equ 20h
IDLE_PRIORITY_CLASS                  equ 40h
HIGH_PRIORITY_CLASS                  equ 80h
REALTIME_PRIORITY_CLASS              equ 100h
CREATE_NEW_PROCESS_GROUP             equ 200h
CREATE_NO_WINDOW                     equ 8000000h
PROFILE_USER                         equ 10000000h
PROFILE_KERNEL                       equ 20000000h
PROFILE_SERVER                       equ 40000000h
MAXLONG                              equ 7FFFFFFFh
THREAD_BASE_PRIORITY_MIN             equ -2
THREAD_BASE_PRIORITY_MAX             equ 2
THREAD_BASE_PRIORITY_LOWRT           equ 15
THREAD_BASE_PRIORITY_IDLE            equ -15
THREAD_PRIORITY_LOWEST               equ THREAD_BASE_PRIORITY_MIN
THREAD_PRIORITY_BELOW_NORMAL         equ THREAD_PRIORITY_LOWEST + 1
THREAD_PRIORITY_NORMAL               equ 0
THREAD_PRIORITY_HIGHEST              equ THREAD_BASE_PRIORITY_MAX
THREAD_PRIORITY_ABOVE_NORMAL         equ THREAD_PRIORITY_HIGHEST - 1
THREAD_PRIORITY_ERROR_RETURN         equ MAXLONG
THREAD_PRIORITY_TIME_CRITICAL        equ THREAD_BASE_PRIORITY_LOWRT
THREAD_PRIORITY_IDLE                 equ THREAD_BASE_PRIORITY_IDLE
APPLICATION_ERROR_MASK               equ 20000000h
ERROR_SEVERITY_SUCCESS               equ 0h
ERROR_SEVERITY_INFORMATIONAL         equ 40000000h
ERROR_SEVERITY_WARNING               equ 80000000h
ERROR_SEVERITY_ERROR                 equ 0C0000000h
MINCHAR                              equ 80h
MAXCHAR                              equ 7Fh
MINSHORT                             equ 8000h
MAXSHORT                             equ 7FFFh
MINLONG                              equ 80000000h
MAXBYTE                              equ 0FFh
MAXWORD                              equ 0FFFFh
MAXDWORD                             equ 0FFFFFFFFh
LANG_NEUTRAL                         equ 0h
LANG_BULGARIAN                       equ 2h
LANG_CHINESE                         equ 4h
LANG_CROATIAN                        equ 1Ah
LANG_CZECH                           equ 5h
LANG_DANISH                          equ 6h
LANG_DUTCH                           equ 13h
LANG_ENGLISH                         equ 9h
LANG_FINNISH                         equ 0Bh
LANG_FRENCH                          equ 0Ch
LANG_GERMAN                          equ 7h
LANG_GREEK                           equ 8h
LANG_HUNGARIAN                       equ 0Eh
LANG_ICELANDIC                       equ 0Fh
LANG_ITALIAN                         equ 10h
LANG_JAPANESE                        equ 11h
LANG_KOREAN                          equ 12h
LANG_NORWEGIAN                       equ 14h
LANG_POLISH                          equ 15h
LANG_PORTUGUESE                      equ 16h
LANG_ROMANIAN                        equ 18h
LANG_RUSSIAN                         equ 19h
LANG_SLOVAK                          equ 1Bh
LANG_SLOVENIAN                       equ 24h
LANG_SPANISH                         equ 0Ah
LANG_SWEDISH                         equ 1Dh
LANG_TURKISH                         equ 1Fh
SUBLANG_NEUTRAL                      equ 0h
SUBLANG_DEFAULT                      equ 1h
SUBLANG_SYS_DEFAULT                  equ 2h
SUBLANG_CHINESE_TRADITIONAL          equ 1h
SUBLANG_CHINESE_SIMPLIFIED           equ 2h
SUBLANG_CHINESE_HONGKONG             equ 3h
SUBLANG_CHINESE_SINGAPORE            equ 4h
SUBLANG_DUTCH                        equ 1h
SUBLANG_DUTCH_BELGIAN                equ 2h
SUBLANG_ENGLISH_US                   equ 1h
SUBLANG_ENGLISH_UK                   equ 2h
SUBLANG_ENGLISH_AUS                  equ 3h
SUBLANG_ENGLISH_CAN                  equ 4h
SUBLANG_ENGLISH_NZ                   equ 5h
SUBLANG_ENGLISH_EIRE                 equ 6h
SUBLANG_FRENCH                       equ 1h
SUBLANG_FRENCH_BELGIAN               equ 2h
SUBLANG_FRENCH_CANADIAN              equ 3h
SUBLANG_FRENCH_SWISS                 equ 4h
SUBLANG_GERMAN                       equ 1h
SUBLANG_GERMAN_SWISS                 equ 2h
SUBLANG_GERMAN_AUSTRIAN              equ 3h
SUBLANG_ITALIAN                      equ 1h
SUBLANG_ITALIAN_SWISS                equ 2h
SUBLANG_NORWEGIAN_BOKMAL             equ 1h
SUBLANG_NORWEGIAN_NYNORSK            equ 2h
SUBLANG_PORTUGUESE                   equ 2h
SUBLANG_PORTUGUESE_BRAZILIAN         equ 1h
SUBLANG_SPANISH                      equ 1h
SUBLANG_SPANISH_MEXICAN              equ 2h
SUBLANG_SPANISH_MODERN               equ 3h
SORT_DEFAULT                         equ 0h
SORT_JAPANESE_XJIS                   equ 0h
SORT_JAPANESE_UNICODE                equ 1h
SORT_CHINESE_BIG5                    equ 0h
SORT_CHINESE_UNICODE                 equ 1h
SORT_KOREAN_KSC                      equ 0h
SORT_KOREAN_UNICODE                  equ 1h
FILE_READ_DATA                       equ 1h
FILE_LIST_DIRECTORY                  equ 1h
FILE_WRITE_DATA                      equ 2h
FILE_ADD_FILE                        equ 2h
FILE_APPEND_DATA                     equ 4h
FILE_ADD_SUBDIRECTORY                equ 4h
FILE_CREATE_PIPE_INSTANCE            equ 4h
FILE_READ_EA                         equ 8h
FILE_READ_PROPERTIES                 equ FILE_READ_EA
FILE_WRITE_EA                        equ 10h
FILE_WRITE_PROPERTIES                equ FILE_WRITE_EA
FILE_EXECUTE                         equ 20h
FILE_TRAVERSE                        equ 20h
FILE_DELETE_CHILD                    equ 40h
FILE_READ_ATTRIBUTES                 equ 80h
FILE_WRITE_ATTRIBUTES                equ 100h
FILE_ALL_ACCESS                      equ STANDARD_RIGHTS_REQUIRED OR SYNCHRONIZE OR 1FFh
FILE_GENERIC_READ                    equ STANDARD_RIGHTS_READ OR FILE_READ_DATA OR FILE_READ_ATTRIBUTES OR FILE_READ_EA OR SYNCHRONIZE
FILE_GENERIC_WRITE                   equ STANDARD_RIGHTS_WRITE OR FILE_WRITE_DATA OR FILE_WRITE_ATTRIBUTES OR FILE_WRITE_EA OR FILE_APPEND_DATA OR SYNCHRONIZE
FILE_GENERIC_EXECUTE                 equ STANDARD_RIGHTS_EXECUTE OR FILE_READ_ATTRIBUTES OR FILE_EXECUTE OR SYNCHRONIZE
FILE_SHARE_READ                      equ 1h
FILE_SHARE_WRITE                     equ 2h
FILE_ATTRIBUTE_READONLY              equ 1h
FILE_ATTRIBUTE_HIDDEN                equ 2h
FILE_ATTRIBUTE_SYSTEM                equ 4h
FILE_ATTRIBUTE_DIRECTORY             equ 10h
FILE_ATTRIBUTE_ARCHIVE               equ 20h
FILE_ATTRIBUTE_NORMAL                equ 80h
FILE_ATTRIBUTE_TEMPORARY             equ 100h
FILE_ATTRIBUTE_COMPRESSED            equ 800h
FILE_NOTIFY_CHANGE_FILE_NAME         equ 1h
FILE_NOTIFY_CHANGE_DIR_NAME          equ 2h
FILE_NOTIFY_CHANGE_ATTRIBUTES        equ 4h
FILE_NOTIFY_CHANGE_SIZE              equ 8h
FILE_NOTIFY_CHANGE_LAST_WRITE        equ 10h
FILE_NOTIFY_CHANGE_SECURITY          equ 100h
MAILSLOT_NO_MESSAGE                  equ -1
MAILSLOT_WAIT_FOREVER                equ -1
FILE_CASE_SENSITIVE_SEARCH           equ 1h
FILE_CASE_PRESERVED_NAMES            equ 2h
FILE_UNICODE_ON_DISK                 equ 4h
FILE_PERSISTENT_ACLS                 equ 8h
FILE_FILE_COMPRESSION                equ 10h
FILE_VOLUME_IS_COMPRESSED            equ 8000h
IO_COMPLETION_MODIFY_STATE           equ 2h
IO_COMPLETION_ALL_ACCESS             equ STANDARD_RIGHTS_REQUIRED OR SYNCHRONIZE OR 3h
DUPLICATE_CLOSE_SOURCE               equ 1h
DUPLICATE_SAME_ACCESS                equ 2h
ACCESS_SYSTEM_SECURITY               equ 1000000h
MAXIMUM_ALLOWED                      equ 2000000h
GENERIC_READ                         equ 80000000h
GENERIC_WRITE                        equ 40000000h
GENERIC_EXECUTE                      equ 20000000h
GENERIC_ALL                          equ 10000000h
ACL_REVISION                         equ 2
ACL_REVISION1                        equ 1
ACL_REVISION2                        equ 2
ACCESS_ALLOWED_ACE_TYPE              equ 0h
ACCESS_DENIED_ACE_TYPE               equ 1h
SYSTEM_AUDIT_ACE_TYPE                equ 2h
SYSTEM_ALARM_ACE_TYPE                equ 3h
HELPINFO_WINDOW                      equ 1
HELPINFO_MENUITEM                    equ 2
OBJECT_INHERIT_ACE                   equ 1h
CONTAINER_INHERIT_ACE                equ 2h
NO_PROPAGATE_INHERIT_ACE             equ 4h
INHERIT_ONLY_ACE                     equ 8h
VALID_INHERIT_FLAGS                  equ 0Fh
SUCCESSFUL_ACCESS_ACE_FLAG           equ 40h
FAILED_ACCESS_ACE_FLAG               equ 80h
AclRevisionInformation               equ 1
AclSizeInformation                   equ 2
SECURITY_DESCRIPTOR_REVISION         equ 1
SECURITY_DESCRIPTOR_REVISION1        equ 1
SECURITY_DESCRIPTOR_MIN_LENGTH       equ 20
SE_OWNER_DEFAULTED                   equ 1h
SE_GROUP_DEFAULTED                   equ 2h
SE_DACL_PRESENT                      equ 4h
SE_DACL_DEFAULTED                    equ 8h
SE_SACL_PRESENT                      equ 10h
SE_SACL_DEFAULTED                    equ 20h
SE_SELF_RELATIVE                     equ 8000h
SE_PRIVILEGE_ENABLED_BY_DEFAULT      equ 1h
SE_PRIVILEGE_ENABLED                 equ 2h
SE_PRIVILEGE_USED_FOR_ACCESS         equ 80000000h
PRIVILEGE_SET_ALL_NECESSARY          equ 1
SecurityAnonymous                    equ 1
SecurityIdentification               equ 2
REG_OPTION_RESERVED                  equ 0
REG_OPTION_NON_VOLATILE              equ 0
REG_OPTION_VOLATILE                  equ 1
REG_OPTION_CREATE_LINK               equ 2
REG_OPTION_BACKUP_RESTORE            equ 4
REG_NONE                             equ 0
REG_SZ                               equ 1
REG_EXPAND_SZ                        equ 2
REG_BINARY                           equ 3
REG_DWORD                            equ 4
REG_DWORD_LITTLE_ENDIAN              equ 4
REG_DWORD_BIG_ENDIAN                 equ 5
REG_LINK                             equ 6
REG_MULTI_SZ                         equ 7
REG_RESOURCE_LIST                    equ 8
REG_FULL_RESOURCE_DESCRIPTOR         equ 9
REG_RESOURCE_REQUIREMENTS_LIST       equ 10
REG_CREATED_NEW_KEY                  equ 1h
REG_OPENED_EXISTING_KEY              equ 2h
REG_WHOLE_HIVE_VOLATILE              equ 1h
REG_REFRESH_HIVE                     equ 2h
REG_NOTIFY_CHANGE_NAME               equ 1h
REG_NOTIFY_CHANGE_ATTRIBUTES         equ 2h
REG_NOTIFY_CHANGE_LAST_SET           equ 4h
REG_NOTIFY_CHANGE_SECURITY           equ 8h
REG_LEGAL_CHANGE_FILTER              equ REG_NOTIFY_CHANGE_NAME OR REG_NOTIFY_CHANGE_ATTRIBUTES OR REG_NOTIFY_CHANGE_LAST_SET OR REG_NOTIFY_CHANGE_SECURITY
REG_LEGAL_OPTION                     equ REG_OPTION_RESERVED OR REG_OPTION_NON_VOLATILE OR REG_OPTION_VOLATILE OR REG_OPTION_CREATE_LINK OR REG_OPTION_BACKUP_RESTORE
KEY_QUERY_VALUE                      equ 1h
KEY_SET_VALUE                        equ 2h
KEY_CREATE_SUB_KEY                   equ 4h
KEY_ENUMERATE_SUB_KEYS               equ 8h
KEY_NOTIFY                           equ 10h
KEY_CREATE_LINK                      equ 20h
KEY_READ                             equ (STANDARD_RIGHTS_READ OR KEY_QUERY_VALUE OR KEY_ENUMERATE_SUB_KEYS OR KEY_NOTIFY) AND NOT SYNCHRONIZE
KEY_WRITE                            equ (STANDARD_RIGHTS_WRITE OR KEY_SET_VALUE OR KEY_CREATE_SUB_KEY) AND NOT SYNCHRONIZE
KEY_EXECUTE                          equ KEY_READ
KEY_ALL_ACCESS                       equ (STANDARD_RIGHTS_ALL OR KEY_QUERY_VALUE OR KEY_SET_VALUE OR KEY_CREATE_SUB_KEY OR KEY_ENUMERATE_SUB_KEYS OR KEY_NOTIFY OR KEY_CREATE_LINK) AND NOT SYNCHRONIZE
EXCEPTION_DEBUG_EVENT                equ 1
CREATE_THREAD_DEBUG_EVENT            equ 2
CREATE_PROCESS_DEBUG_EVENT           equ 3
EXIT_THREAD_DEBUG_EVENT              equ 4
EXIT_PROCESS_DEBUG_EVENT             equ 5
LOAD_DLL_DEBUG_EVENT                 equ 6
UNLOAD_DLL_DEBUG_EVENT               equ 7
OUTPUT_DEBUG_STRING_EVENT            equ 8
RIP_EVENT                            equ 9
EXCEPTION_MAXIMUM_PARAMETERS         equ 15
DRIVE_REMOVABLE                      equ 2
DRIVE_FIXED                          equ 3
DRIVE_REMOTE                         equ 4
DRIVE_CDROM                          equ 5
DRIVE_RAMDISK                        equ 6
FILE_TYPE_UNKNOWN                    equ 0h
FILE_TYPE_DISK                       equ 1h
FILE_TYPE_CHAR                       equ 2h
FILE_TYPE_PIPE                       equ 3h
FILE_TYPE_REMOTE                     equ 8000h
STD_INPUT_HANDLE                     equ -10
STD_OUTPUT_HANDLE                    equ -11
STD_ERROR_HANDLE                     equ -12
NOPARITY                             equ 0
ODDPARITY                            equ 1
EVENPARITY                           equ 2
MARKPARITY                           equ 3
SPACEPARITY                          equ 4
ONESTOPBIT                           equ 0
ONE5STOPBITS                         equ 1
TWOSTOPBITS                          equ 2
IGNORE                               equ 0
INFINITE                             equ -1
CBR_110                              equ 110
CBR_300                              equ 300
CBR_600                              equ 600
CBR_1200                             equ 1200
CBR_2400                             equ 2400
CBR_4800                             equ 4800
CBR_9600                             equ 9600
CBR_14400                            equ 14400
CBR_19200                            equ 19200
CBR_38400                            equ 38400
CBR_56000                            equ 56000
CBR_57600                            equ 57600
CBR_115200                           equ 115200
CBR_128000                           equ 128000
CBR_256000                           equ 256000
CE_RXOVER                            equ 1h
CE_OVERRUN                           equ 2h
CE_RXPARITY                          equ 4h
CE_FRAME                             equ 8h
CE_BREAK                             equ 10h
CE_TXFULL                            equ 100h
CE_PTO                               equ 200h
CE_IOE                               equ 400h
CE_DNS                               equ 800h
CE_OOP                               equ 1000h
CE_MODE                              equ 8000h
IE_BADID                             equ -1
IE_OPEN                              equ -2
IE_NOPEN                             equ -3
IE_MEMORY                            equ -4
IE_DEFAULT                           equ -5
IE_HARDWARE                          equ -10
IE_BYTESIZE                          equ -11
IE_BAUDRATE                          equ -12
EV_RXCHAR                            equ 1h
EV_RXFLAG                            equ 2h
EV_TXEMPTY                           equ 4h
EV_CTS                               equ 8h
EV_DSR                               equ 10h
EV_RLSD                              equ 20h
EV_BREAK                             equ 40h
EV_ERR                               equ 80h
EV_RING                              equ 100h
EV_PERR                              equ 200h
EV_RX80FULL                          equ 400h
EV_EVENT1                            equ 800h
EV_EVENT2                            equ 1000h
SETXOFF                              equ 1
SETXON                               equ 2
SETRTS                               equ 3
CLRRTS                               equ 4
SETDTR                               equ 5
CLRDTR                               equ 6
RESETDEV                             equ 7
SETBREAK                             equ 8
CLRBREAK                             equ 9
PURGE_TXABORT                        equ 1h
PURGE_RXABORT                        equ 2h
PURGE_TXCLEAR                        equ 4h
PURGE_RXCLEAR                        equ 8h
LPTx                                 equ 80h
MS_CTS_ON                            equ 10h
MS_DSR_ON                            equ 20h
MS_RING_ON                           equ 40h
MS_RLSD_ON                           equ 80h
S_QUEUEEMPTY                         equ 0
S_THRESHOLD                          equ 1
S_ALLTHRESHOLD                       equ 2
S_NORMAL                             equ 0
S_LEGATO                             equ 1
S_STACCATO                           equ 2
S_PERIOD512                          equ 0
S_PERIOD1024                         equ 1
S_PERIOD2048                         equ 2
S_PERIODVOICE                        equ 3
S_WHITE512                           equ 4
S_WHITE1024                          equ 5
S_WHITE2048                          equ 6
S_WHITEVOICE                         equ 7
S_SERDVNA                            equ -1
S_SEROFM                             equ -2
S_SERMACT                            equ -3
S_SERQFUL                            equ -4
S_SERBDNT                            equ -5
S_SERDLN                             equ -6
S_SERDCC                             equ -7
S_SERDTP                             equ -8
S_SERDVL                             equ -9
S_SERDMD                             equ -10
S_SERDSH                             equ -11
S_SERDPT                             equ -12
S_SERDFQ                             equ -13
S_SERDDR                             equ -14
S_SERDSR                             equ -15
S_SERDST                             equ -16
NMPWAIT_WAIT_FOREVER                 equ 0FFFFh
NMPWAIT_NOWAIT                       equ 1h
NMPWAIT_USE_DEFAULT_WAIT             equ 0h
FS_CASE_IS_PRESERVED                 equ FILE_CASE_PRESERVED_NAMES
FS_CASE_SENSITIVE                    equ FILE_CASE_SENSITIVE_SEARCH
FS_UNICODE_STORED_ON_DISK            equ FILE_UNICODE_ON_DISK
FS_PERSISTENT_ACLS                   equ FILE_PERSISTENT_ACLS
SECTION_QUERY                        equ 1h
SECTION_MAP_WRITE                    equ 2h
SECTION_MAP_READ                     equ 4h
SECTION_MAP_EXECUTE                  equ 8h
SECTION_EXTEND_SIZE                  equ 10h
SECTION_ALL_ACCESS                   equ STANDARD_RIGHTS_REQUIRED OR SECTION_QUERY OR SECTION_MAP_WRITE OR SECTION_MAP_READ OR SECTION_MAP_EXECUTE OR SECTION_EXTEND_SIZE
FILE_MAP_COPY                        equ SECTION_QUERY
FILE_MAP_WRITE                       equ SECTION_MAP_WRITE
FILE_MAP_READ                        equ SECTION_MAP_READ
FILE_MAP_ALL_ACCESS                  equ SECTION_ALL_ACCESS
OF_READ                              equ 0h
OF_WRITE                             equ 1h
OF_READWRITE                         equ 2h
OF_SHARE_COMPAT                      equ 0h
OF_SHARE_EXCLUSIVE                   equ 10h
OF_SHARE_DENY_WRITE                  equ 20h
OF_SHARE_DENY_READ                   equ 30h
OF_SHARE_DENY_NONE                   equ 40h
OF_PARSE                             equ 100h
OF_DELETE                            equ 200h
OF_VERIFY                            equ 400h
OF_CANCEL                            equ 800h
OF_CREATE                            equ 1000h
OF_PROMPT                            equ 2000h
OF_EXIST                             equ 4000h
OF_REOPEN                            equ 8000h
OFS_MAXPATHNAME                      equ 128
DONT_RESOLVE_DLL_REFERENCES          equ 1h
TC_NORMAL                            equ 0
TC_HARDERR                           equ 1
TC_GP_TRAP                           equ 2
TC_SIGNAL                            equ 3
MAX_LEADBYTES                        equ 12
MB_PRECOMPOSED                       equ 1h
MB_COMPOSITE                         equ 2h
MB_USEGLYPHCHARS                     equ 4h
WC_DEFAULTCHECK                      equ 100h
WC_COMPOSITECHECK                    equ 200h
WC_DISCARDNS                         equ 10h
WC_SEPCHARS                          equ 20h
WC_DEFAULTCHAR                       equ 40h
CT_CTYPE1                            equ 1h
CT_CTYPE2                            equ 2h
CT_CTYPE3                            equ 4h
C1_UPPER                             equ 1h
C1_LOWER                             equ 2h
C1_DIGIT                             equ 4h
C1_SPACE                             equ 8h
C1_PUNCT                             equ 10h
C1_CNTRL                             equ 20h
C1_BLANK                             equ 40h
C1_XDIGIT                            equ 80h
C1_ALPHA                             equ 100h
C2_LEFTTORIGHT                       equ 1h
C2_RIGHTTOLEFT                       equ 2h
C2_EUROPENUMBER                      equ 3h
C2_EUROPESEPARATOR                   equ 4h
C2_EUROPETERMINATOR                  equ 5h
C2_ARABICNUMBER                      equ 6h
C2_COMMONSEPARATOR                   equ 7h
C2_BLOCKSEPARATOR                    equ 8h
C2_SEGMENTSEPARATOR                  equ 9h
C2_WHITESPACE                        equ 0Ah
C2_OTHERNEUTRAL                      equ 0Bh
C2_NOTAPPLICABLE                     equ 0h
C3_NONSPACING                        equ 1h
C3_DIACRITIC                         equ 2h
C3_VOWELMARK                         equ 4h
C3_SYMBOL                            equ 8h
C3_NOTAPPLICABLE                     equ 0h
NORM_IGNORECASE                      equ 1h
NORM_IGNORENONSPACE                  equ 2h
NORM_IGNORESYMBOLS                   equ 4h
MAP_FOLDCZONE                        equ 10h
MAP_PRECOMPOSED                      equ 20h
MAP_COMPOSITE                        equ 40h
MAP_FOLDDIGITS                       equ 80h
LCMAP_LOWERCASE                      equ 100h
LCMAP_UPPERCASE                      equ 200h
LCMAP_SORTKEY                        equ 400h
LCMAP_BYTEREV                        equ 800h
SORT_STRINGSORT                      equ 1000h
CP_ACP                               equ 0
CP_OEMCP                             equ 1
CTRY_DEFAULT                         equ 0
CTRY_AUSTRALIA                       equ 61
CTRY_AUSTRIA                         equ 43
CTRY_BELGIUM                         equ 32
CTRY_BRAZIL                          equ 55
CTRY_CANADA                          equ 2
CTRY_DENMARK                         equ 45
CTRY_FINLAND                         equ 358
CTRY_FRANCE                          equ 33
CTRY_GERMANY                         equ 49
CTRY_ICELAND                         equ 354
CTRY_IRELAND                         equ 353
CTRY_ITALY                           equ 39
CTRY_JAPAN                           equ 81
CTRY_MEXICO                          equ 52
CTRY_NETHERLANDS                     equ 31
CTRY_NEW_ZEALAND                     equ 64
CTRY_NORWAY                          equ 47
CTRY_PORTUGAL                        equ 351
CTRY_PRCHINA                         equ 86
CTRY_SOUTH_KOREA                     equ 82
CTRY_SPAIN                           equ 34
CTRY_SWEDEN                          equ 46
CTRY_SWITZERLAND                     equ 41
CTRY_TAIWAN                          equ 886
CTRY_UNITED_KINGDOM                  equ 44
CTRY_UNITED_STATES                   equ 1
LOCALE_NOUSEROVERRIDE                equ 80000000h
LOCALE_USER_DEFAULT                  equ 0400h
LOCALE_ILANGUAGE                     equ 1h
LOCALE_SLANGUAGE                     equ 2h
LOCALE_SENGLANGUAGE                  equ 1001h
LOCALE_SABBREVLANGNAME               equ 3h
LOCALE_SNATIVELANGNAME               equ 4h
LOCALE_ICOUNTRY                      equ 5h
LOCALE_SCOUNTRY                      equ 6h
LOCALE_SENGCOUNTRY                   equ 1002h
LOCALE_SABBREVCTRYNAME               equ 7h
LOCALE_SNATIVECTRYNAME               equ 8h
LOCALE_IDEFAULTLANGUAGE              equ 9h
LOCALE_IDEFAULTCOUNTRY               equ 0Ah
LOCALE_IDEFAULTCODEPAGE              equ 0Bh
LOCALE_SLIST                         equ 0Ch
LOCALE_IMEASURE                      equ 0Dh
LOCALE_SDECIMAL                      equ 0Eh
LOCALE_STHOUSAND                     equ 0Fh
LOCALE_SGROUPING                     equ 10h
LOCALE_IDIGITS                       equ 11h
LOCALE_ILZERO                        equ 12h
LOCALE_SNATIVEDIGITS                 equ 13h
LOCALE_SCURRENCY                     equ 14h
LOCALE_SINTLSYMBOL                   equ 15h
LOCALE_SMONDECIMALSEP                equ 16h
LOCALE_SMONTHOUSANDSEP               equ 17h
LOCALE_SMONGROUPING                  equ 18h
LOCALE_ICURRDIGITS                   equ 19h
LOCALE_IINTLCURRDIGITS               equ 1Ah
LOCALE_ICURRENCY                     equ 1Bh
LOCALE_INEGCURR                      equ 1Ch
LOCALE_SDATE                         equ 1Dh
LOCALE_STIME                         equ 1Eh
LOCALE_SSHORTDATE                    equ 1Fh
LOCALE_SLONGDATE                     equ 20h
LOCALE_STIMEFORMAT                   equ 1003h
LOCALE_IDATE                         equ 21h
LOCALE_ILDATE                        equ 22h
LOCALE_ITIME                         equ 23h
LOCALE_ICENTURY                      equ 24h
LOCALE_ITLZERO                       equ 25h
LOCALE_IDAYLZERO                     equ 26h
LOCALE_IMONLZERO                     equ 27h
LOCALE_S1159                         equ 28h
LOCALE_S2359                         equ 29h
LOCALE_SDAYNAME1                     equ 2Ah
LOCALE_SDAYNAME2                     equ 2Bh
LOCALE_SDAYNAME3                     equ 2Ch
LOCALE_SDAYNAME4                     equ 2Dh
LOCALE_SDAYNAME5                     equ 2Eh
LOCALE_SDAYNAME6                     equ 2Fh
LOCALE_SDAYNAME7                     equ 30h
LOCALE_SABBREVDAYNAME1               equ 31h
LOCALE_SABBREVDAYNAME2               equ 32h
LOCALE_SABBREVDAYNAME3               equ 33h
LOCALE_SABBREVDAYNAME4               equ 34h
LOCALE_SABBREVDAYNAME5               equ 35h
LOCALE_SABBREVDAYNAME6               equ 36h
LOCALE_SABBREVDAYNAME7               equ 37h
LOCALE_SMONTHNAME1                   equ 38h
LOCALE_SMONTHNAME2                   equ 39h
LOCALE_SMONTHNAME3                   equ 3Ah
LOCALE_SMONTHNAME4                   equ 3Bh
LOCALE_SMONTHNAME5                   equ 3Ch
LOCALE_SMONTHNAME6                   equ 3Dh
LOCALE_SMONTHNAME7                   equ 3Eh
LOCALE_SMONTHNAME8                   equ 3Fh
LOCALE_SMONTHNAME9                   equ 40h
LOCALE_SMONTHNAME10                  equ 41h
LOCALE_SMONTHNAME11                  equ 42h
LOCALE_SMONTHNAME12                  equ 43h
LOCALE_SABBREVMONTHNAME1             equ 44h
LOCALE_SABBREVMONTHNAME2             equ 45h
LOCALE_SABBREVMONTHNAME3             equ 46h
LOCALE_SABBREVMONTHNAME4             equ 47h
LOCALE_SABBREVMONTHNAME5             equ 48h
LOCALE_SABBREVMONTHNAME6             equ 49h
LOCALE_SABBREVMONTHNAME7             equ 4Ah
LOCALE_SABBREVMONTHNAME8             equ 4Bh
LOCALE_SABBREVMONTHNAME9             equ 4Ch
LOCALE_SABBREVMONTHNAME10            equ 4Dh
LOCALE_SABBREVMONTHNAME11            equ 4Eh
LOCALE_SABBREVMONTHNAME12            equ 4Fh
LOCALE_SABBREVMONTHNAME13            equ 100Fh
LOCALE_SPOSITIVESIGN                 equ 50h
LOCALE_SNEGATIVESIGN                 equ 51h
LOCALE_IPOSSIGNPOSN                  equ 52h
LOCALE_INEGSIGNPOSN                  equ 53h
LOCALE_IPOSSYMPRECEDES               equ 54h
LOCALE_IPOSSEPBYSPACE                equ 55h
LOCALE_INEGSYMPRECEDES               equ 56h
LOCALE_INEGSEPBYSPACE                equ 57h
TIME_NOMINUTESORSECONDS              equ 1h
TIME_NOSECONDS                       equ 2h
TIME_NOTIMEMARKER                    equ 4h
TIME_FORCE24HOURFORMAT               equ 8h
DATE_SHORTDATE                       equ 1h
DATE_LONGDATE                        equ 2h
TF_FORCEDRIVE                        equ 80h
LOCKFILE_FAIL_IMMEDIATELY            equ 1h
LOCKFILE_EXCLUSIVE_LOCK              equ 2h
LNOTIFY_OUTOFMEM                     equ 0
LNOTIFY_MOVE                         equ 1
LNOTIFY_DISCARD                      equ 2
SLE_ERROR                            equ 1h
SLE_MINORERROR                       equ 2h
SLE_WARNING                          equ 3h
SEM_FAILCRITICALERRORS               equ 1h
SEM_NOGPFAULTERRORBOX                equ 2h
SEM_NOOPENFILEERRORBOX               equ 8000h
RT_CURSOR                            equ 1
RT_BITMAP                            equ 2
RT_ICON                              equ 3
RT_MENU                              equ 4
RT_DIALOG                            equ 5
RT_STRING                            equ 6
RT_FONTDIR                           equ 7
RT_FONT                              equ 8
RT_ACCELERATOR                       equ 9
RT_RCDATA                            equ 10
DFC_CAPTION                          equ 1
DFC_MENU                             equ 2
DFC_SCROLL                           equ 3
DFC_BUTTON                           equ 4
DFC_POPUPMENU           			  equ 5
DFCS_CAPTIONCLOSE                    equ 0000h
DFCS_CAPTIONMIN                      equ 0001h
DFCS_CAPTIONMAX                      equ 0002h
DFCS_CAPTIONRESTORE                  equ 0003h
DFCS_CAPTIONHELP                     equ 0004h
DFCS_MENUARROW                       equ 0000h
DFCS_MENUCHECK                       equ 0001h
DFCS_MENUBULLET                      equ 0002h
DFCS_MENUARROWRIGHT                  equ 0004h
DFCS_SCROLLUP                        equ 0000h
DFCS_SCROLLDOWN                      equ 0001h
DFCS_SCROLLLEFT                      equ 0002h
DFCS_SCROLLRIGHT                     equ 0003h
DFCS_SCROLLCOMBOBOX                  equ 0005h
DFCS_SCROLLSIZEGRIP                  equ 0008h
DFCS_SCROLLSIZEGRIPRIGHT             equ 0010h
DFCS_BUTTONCHECK                     equ 0000h
DFCS_BUTTONRADIOIMAGE                equ 0001h
DFCS_BUTTONRADIOMASK                 equ 0002h
DFCS_BUTTONRADIO                     equ 0004h
DFCS_BUTTON3STATE                    equ 0008h
DFCS_BUTTONPUSH                      equ 0010h
DFCS_INACTIVE                        equ 0100h
DFCS_PUSHED                          equ 0200h
DFCS_CHECKED                         equ 0400h
DFCS_TRANSPARENT        			  equ 0800h
DFCS_HOT		                	  equ 1000h
DFCS_ADJUSTRECT                      equ 2000h
DFCS_FLAT                            equ 4000h
DFCS_MONO                            equ 8000h
DDD_RAW_TARGET_PATH                  equ 1h
DDD_REMOVE_DEFINITION                equ 2h
DDD_EXACT_MATCH_ON_REMOVE            equ 4h
MAX_PATH                             equ 260
MOVEFILE_REPLACE_EXISTING            equ 1h
MOVEFILE_COPY_ALLOWED                equ 2h
MOVEFILE_DELAY_UNTIL_REBOOT          equ 4h
TokenUser                            equ 1
TokenGroups                          equ 2
TokenPrivileges                      equ 3
TokenOwner                           equ 4
TokenPrimaryGroup                    equ 5
TokenDefaultDacl                     equ 6
TokenSource                          equ 7
TokenType                            equ 8
TokenImpersonationLevel              equ 9
TokenStatistics                      equ 10
TOKEN_ADJUST_PRIVILEGES              equ 20h
GET_TAPE_MEDIA_INFORMATION           equ 0
GET_TAPE_DRIVE_INFORMATION           equ 1
SET_TAPE_MEDIA_INFORMATION           equ 0
SET_TAPE_DRIVE_INFORMATION           equ 1
FORMAT_MESSAGE_ALLOCATE_BUFFER       equ 100h
FORMAT_MESSAGE_IGNORE_INSERTS        equ 200h
FORMAT_MESSAGE_FROM_STRING           equ 400h
FORMAT_MESSAGE_FROM_HMODULE          equ 800h
FORMAT_MESSAGE_FROM_SYSTEM           equ 1000h
FORMAT_MESSAGE_ARGUMENT_ARRAY        equ 2000h
FORMAT_MESSAGE_MAX_WIDTH_MASK        equ 0FFh
TLS_OUT_OF_INDEXES                   equ 0FFFFh
BACKUP_DATA                          equ 1h
BACKUP_EA_DATA                       equ 2h
BACKUP_SECURITY_DATA                 equ 3h
BACKUP_ALTERNATE_DATA                equ 4h
BACKUP_LINK                          equ 5h
STREAM_MODIFIED_WHEN_READ            equ 1h
STREAM_CONTAINS_SECURITY             equ 2h
STARTF_USESHOWWINDOW                 equ 1h
STARTF_USESIZE                       equ 2h
STARTF_USEPOSITION                   equ 4h
STARTF_USECOUNTCHARS                 equ 8h
STARTF_USEFILLATTRIBUTE              equ 10h
STARTF_RUNFULLSCREEN                 equ 20h
STARTF_FORCEONFEEDBACK               equ 40h
STARTF_FORCEOFFFEEDBACK              equ 80h
STARTF_USESTDHANDLES                 equ 100h
SHUTDOWN_NORETRY                     equ 1h
MAX_DEFAULTCHAR                      equ 2
MAX_LEADBYTES                        equ 12
CAL_ICALINTVALUE                     equ 1h
CAL_SCALNAME                         equ 2h
CAL_IYEAROFFSETRANGE                 equ 3h
CAL_SERASTRING                       equ 4h
CAL_SSHORTDATE                       equ 5h
CAL_SLONGDATE                        equ 6h
CAL_SDAYNAME1                        equ 7h
CAL_SDAYNAME2                        equ 8h
CAL_SDAYNAME3                        equ 9h
CAL_SDAYNAME4                        equ 0Ah
CAL_SDAYNAME5                        equ 0Bh
CAL_SDAYNAME6                        equ 0Ch
CAL_SDAYNAME7                        equ 0Dh
CAL_SABBREVDAYNAME1                  equ 0Eh
CAL_SABBREVDAYNAME2                  equ 0Fh
CAL_SABBREVDAYNAME3                  equ 10h
CAL_SABBREVDAYNAME4                  equ 11h
CAL_SABBREVDAYNAME5                  equ 12h
CAL_SABBREVDAYNAME6                  equ 13h
CAL_SABBREVDAYNAME7                  equ 14h
CAL_SMONTHNAME1                      equ 15h
CAL_SMONTHNAME2                      equ 16h
CAL_SMONTHNAME3                      equ 17h
CAL_SMONTHNAME4                      equ 18h
CAL_SMONTHNAME5                      equ 19h
CAL_SMONTHNAME6                      equ 1Ah
CAL_SMONTHNAME7                      equ 1Bh
CAL_SMONTHNAME8                      equ 1Ch
CAL_SMONTHNAME9                      equ 1Dh
CAL_SMONTHNAME10                     equ 1Eh
CAL_SMONTHNAME11                     equ 1Fh
CAL_SMONTHNAME12                     equ 20h
CAL_SMONTHNAME13                     equ 21h
CAL_SABBREVMONTHNAME1                equ 22h
CAL_SABBREVMONTHNAME2                equ 23h
CAL_SABBREVMONTHNAME3                equ 24h
CAL_SABBREVMONTHNAME4                equ 25h
CAL_SABBREVMONTHNAME5                equ 26h
CAL_SABBREVMONTHNAME6                equ 27h
CAL_SABBREVMONTHNAME7                equ 28h
CAL_SABBREVMONTHNAME8                equ 29h
CAL_SABBREVMONTHNAME9                equ 2Ah
CAL_SABBREVMONTHNAME10               equ 2Bh
CAL_SABBREVMONTHNAME11               equ 2Ch
CAL_SABBREVMONTHNAME12               equ 2Dh
CAL_SABBREVMONTHNAME13               equ 2Eh
ENUM_ALL_CALENDARS                   equ 0FFFFh
CAL_GREGORIAN                        equ 1
CAL_GREGORIAN_US                     equ 2
CAL_JAPAN                            equ 3
CAL_TAIWAN                           equ 4
CAL_KOREA                            equ 5
RIGHT_ALT_PRESSED                    equ 1h
LEFT_ALT_PRESSED                     equ 2h
RIGHT_CTRL_PRESSED                   equ 4h
LEFT_CTRL_PRESSED                    equ 8h
SHIFT_PRESSED                        equ 10h
NUMLOCK_ON                           equ 20h
SCROLLLOCK_ON                        equ 40h
CAPSLOCK_ON                          equ 80h
ENHANCED_KEY                         equ 100h
FROM_LEFT_1ST_BUTTON_PRESSED         equ 1h
RIGHTMOST_BUTTON_PRESSED             equ 2h
FROM_LEFT_2ND_BUTTON_PRESSED         equ 4h
FROM_LEFT_3RD_BUTTON_PRESSED         equ 8h
FROM_LEFT_4TH_BUTTON_PRESSED         equ 10h
MOUSE_MOVED                          equ 1h
DOUBLE_CLICK                         equ 2h
KEY_EVENT                            equ 1h
mouse_eventC                         equ 2h
WINDOW_BUFFER_SIZE_EVENT             equ 4h
MENU_EVENT                           equ 8h
FOCUS_EVENT                          equ 10h
FOREGROUND_BLUE                      equ 1h
FOREGROUND_GREEN                     equ 2h
FOREGROUND_RED                       equ 4h
FOREGROUND_INTENSITY                 equ 8h
BACKGROUND_BLUE                      equ 10h
BACKGROUND_GREEN                     equ 20h
BACKGROUND_RED                       equ 40h
BACKGROUND_INTENSITY                 equ 80h
CTRL_C_EVENT                         equ 0
CTRL_BREAK_EVENT                     equ 1
CTRL_CLOSE_EVENT                     equ 2
CTRL_LOGOFF_EVENT                    equ 5
CTRL_SHUTDOWN_EVENT                  equ 6
ENABLE_PROCESSED_INPUT               equ 1h
ENABLE_LINE_INPUT                    equ 2h
ENABLE_ECHO_INPUT                    equ 4h
ENABLE_WINDOW_INPUT                  equ 8h
ENABLE_MOUSE_INPUT                   equ 10h
ENABLE_PROCESSED_OUTPUT              equ 1h
ENABLE_WRAP_AT_EOL_OUTPUT            equ 2h
CONSOLE_TEXTMODE_BUFFER              equ 1
R2_BLACK                             equ 1
R2_NOTMERGEPEN                       equ 2
R2_MASKNOTPEN                        equ 3
R2_NOTCOPYPEN                        equ 4
R2_MASKPENNOT                        equ 5
R2_NOT                               equ 6
R2_XORPEN                            equ 7
R2_NOTMASKPEN                        equ 8
R2_MASKPEN                           equ 9
R2_NOTXORPEN                         equ 10
R2_NOP                               equ 11
R2_MERGENOTPEN                       equ 12
R2_COPYPEN                           equ 13
R2_MERGEPENNOT                       equ 14
R2_MERGEPEN                          equ 15
R2_WHITE                             equ 16
R2_LAST                              equ 16
SRCCOPY                              equ 0CC0020h
SRCPAINT                             equ 0EE0086h
SRCAND                               equ 8800C6h
SRCINVERT                            equ 660046h
SRCERASE                             equ 440328h
NOTSRCCOPY                           equ 330008h
NOTSRCERASE                          equ 1100A6h
MERGECOPY                            equ 0C000CAh
MERGEPAINT                           equ 0BB0226h
PATCOPY                              equ 0F00021h
PATPAINT                             equ 0FB0A09h
PATINVERT                            equ 5A0049h
DSTINVERT                            equ 550009h
BLACKNESS                            equ 42h
WHITENESS                            equ 0FF0062h
GDI_ERROR                            equ 0FFFFh
HGDI_ERROR                           equ 0FFFFh
ERRORAPI                             equ 0
NULLREGION                           equ 1
SIMPLEREGION                         equ 2
COMPLEXREGION                        equ 3
RGN_AND                              equ 1
RGN_OR                               equ 2
RGN_XOR                              equ 3
RGN_DIFF                             equ 4
RGN_COPY                             equ 5
RGN_MIN                              equ RGN_AND
RGN_MAX                              equ RGN_COPY
BLACKONWHITE                         equ 1
WHITEONBLACK                         equ 2
COLORONCOLOR                         equ 3
HALFTONE                             equ 4
MAXSTRETCHBLTMODE                    equ 4
ALTERNATE                            equ 1
WINDING                              equ 2
POLYFILL_LAST                        equ 2
TA_NOUPDATECP                        equ 0
TA_UPDATECP                          equ 1
TA_LEFT                              equ 0
TA_RIGHT                             equ 2
TA_CENTER                            equ 6
TA_TOP                               equ 0
TA_BOTTOM                            equ 8
TA_BASELINE                          equ 24
TA_MASK                              equ TA_BASELINE + TA_CENTER + TA_UPDATECP
VTA_BASELINE                         equ TA_BASELINE
VTA_LEFT                             equ TA_BOTTOM
VTA_RIGHT                            equ TA_TOP
VTA_CENTER                           equ TA_CENTER
VTA_BOTTOM                           equ TA_RIGHT
VTA_TOP                              equ TA_LEFT
ETO_GRAYED                           equ 1
ETO_OPAQUE                           equ 2
ETO_CLIPPED                          equ 4
ASPECT_FILTERING                     equ 1h
DCB_RESET                            equ 1h
DCB_ACCUMULATE                       equ 2h
DCB_DIRTY                            equ DCB_ACCUMULATE
DCB_SET                              equ DCB_RESET OR DCB_ACCUMULATE
DCB_ENABLE                           equ 4h
DCB_DISABLE                          equ 8h
META_SETBKCOLOR                      equ 201h
META_SETBKMODE                       equ 102h
META_SETMAPMODE                      equ 103h
META_SETROP2                         equ 104h
META_SETRELABS                       equ 105h
META_SETPOLYFILLMODE                 equ 106h
META_SETSTRETCHBLTMODE               equ 107h
META_SETTEXTCHAREXTRA                equ 108h
META_SETTEXTCOLOR                    equ 209h
META_SETTEXTJUSTIFICATION            equ 20Ah
META_SETWINDOWORG                    equ 20Bh
META_SETWINDOWEXT                    equ 20Ch
META_SETVIEWPORTORG                  equ 20Dh
META_SETVIEWPORTEXT                  equ 20Eh
META_OFFSETWINDOWORG                 equ 20Fh
META_SCALEWINDOWEXT                  equ 410h
META_OFFSETVIEWPORTORG               equ 211h
META_SCALEVIEWPORTEXT                equ 412h
META_LINETO                          equ 213h
META_MOVETO                          equ 214h
META_EXCLUDECLIPRECT                 equ 415h
META_INTERSECTCLIPRECT               equ 416h
META_ARC                             equ 817h
META_ELLIPSE                         equ 418h
META_FLOODFILL                       equ 419h
META_PIE                             equ 81Ah
META_RECTANGLE                       equ 41Bh
META_ROUNDRECT                       equ 61Ch
META_PATBLT                          equ 61Dh
META_SAVEDC                          equ 1Eh
META_SETPIXEL                        equ 41Fh
META_OFFSETCLIPRGN                   equ 220h
META_TEXTOUT                         equ 521h
META_BITBLT                          equ 922h
META_STRETCHBLT                      equ 0B23h
META_POLYGON                         equ 324h
META_POLYLINE                        equ 325h
META_ESCAPE                          equ 626h
META_RESTOREDC                       equ 127h
META_FILLREGION                      equ 228h
META_FRAMEREGION                     equ 429h
META_INVERTREGION                    equ 12Ah
META_PAINTREGION                     equ 12Bh
META_SELECTCLIPREGION                equ 12Ch
META_SELECTOBJECT                    equ 12Dh
META_SETTEXTALIGN                    equ 12Eh
META_CHORD                           equ 830h
META_SETMAPPERFLAGS                  equ 231h
META_EXTTEXTOUT                      equ 0A32h
META_SETDIBTODEV                     equ 0D33h
META_SELECTPALETTE                   equ 234h
META_REALIZEPALETTE                  equ 35h
META_ANIMATEPALETTE                  equ 436h
META_SETPALENTRIES                   equ 37h
META_POLYPOLYGON                     equ 538h
META_RESIZEPALETTE                   equ 139h
META_DIBBITBLT                       equ 940h
META_DIBSTRETCHBLT                   equ 0B41h
META_DIBCREATEPATTERNBRUSH           equ 142h
META_STRETCHDIB                      equ 0F43h
META_EXTFLOODFILL                    equ 548h
META_DELETEOBJECT                    equ 1F0h
META_CREATEPALETTE                   equ 0F7h
META_CREATEPATTERNBRUSH              equ 1F9h
META_CREATEPENINDIRECT               equ 2FAh
META_CREATEFONTINDIRECT              equ 2FBh
META_CREATEBRUSHINDIRECT             equ 2FCh
META_CREATEREGION                    equ 6FFh
NEWFRAME                             equ 1
AbortDocC                            equ 2
NEXTBAND                             equ 3
SETCOLORTABLE                        equ 4
GETCOLORTABLE                        equ 5
FLUSHOUTPUT                          equ 6
DRAFTMODE                            equ 7
QUERYESCSUPPORT                      equ 8
SETABORTPROC                         equ 9
StartDocC                            equ 10
EndDocC                              equ 11
GETPHYSPAGESIZE                      equ 12
GETPRINTINGOFFSET                    equ 13
GETSCALINGFACTOR                     equ 14
MFCOMMENT                            equ 15
GETPENWIDTH                          equ 16
SETCOPYCOUNT                         equ 17
SELECTPAPERSOURCE                    equ 18
DEVICEDATA                           equ 19
PASSTHROUGH                          equ 19
GETTECHNOLGY                         equ 20
GETTECHNOLOGY                        equ 20
SETLINECAP                           equ 21
SETLINEJOIN                          equ 22
SetMiterLimitC                       equ 23
BANDINFO                             equ 24
DRAWPATTERNRECT                      equ 25
GETVECTORPENSIZE                     equ 26
GETVECTORBRUSHSIZE                   equ 27
ENABLEDUPLEX                         equ 28
GETSETPAPERBINS                      equ 29
GETSETPRINTORIENT                    equ 30
ENUMPAPERBINS                        equ 31
SETDIBSCALING                        equ 32
EPSPRINTING                          equ 33
ENUMPAPERMETRICS                     equ 34
GETSETPAPERMETRICS                   equ 35
POSTSCRIPT_DATA                      equ 37
POSTSCRIPT_IGNORE                    equ 38
MOUSETRAILS                          equ 39
GETDEVICEUNITS                       equ 42
GETEXTENDEDTEXTMETRICS               equ 256
GETEXTENTTABLE                       equ 257
GETPAIRKERNTABLE                     equ 258
GETTRACKKERNTABLE                    equ 259
ExtTextOutC                          equ 512
GETFACENAME                          equ 513
DOWNLOADFACE                         equ 514
ENABLERELATIVEWIDTHS                 equ 768
ENABLEPAIRKERNING                    equ 769
SETKERNTRACK                         equ 770
SETALLJUSTVALUES                     equ 771
SETCHARSET                           equ 772
StretchBltC                          equ 2048
GETSETSCREENPARAMS                   equ 3072
BEGIN_PATH                           equ 4096
CLIP_TO_PATH                         equ 4097
END_PATH                             equ 4098
EXT_DEVICE_CAPS                      equ 4099
RESTORE_CTM                          equ 4100
SAVE_CTM                             equ 4101
SET_ARC_DIRECTION                    equ 4102
SET_BACKGROUND_COLOR                 equ 4103
SET_POLY_MODE                        equ 4104
SET_SCREEN_ANGLE                     equ 4105
SET_SPREAD                           equ 4106
TRANSFORM_CTM                        equ 4107
SET_CLIP_BOX                         equ 4108
SET_BOUNDS                           equ 4109
SET_MIRROR_MODE                      equ 4110
OPENCHANNEL                          equ 4110
DOWNLOADHEADER                       equ 4111
CLOSECHANNEL                         equ 4112
POSTSCRIPT_PASSTHROUGH               equ 4115
ENCAPSULATED_POSTSCRIPT              equ 4116
SP_NOTREPORTED                       equ 4000h
SP_ERROR                             equ -1
SP_APPABORT                          equ -2
SP_USERABORT                         equ -3
SP_OUTOFDISK                         equ -4
SP_OUTOFMEMORY                       equ -5
PR_JOBSTATUS                         equ 0h
OBJ_PEN                              equ 1
OBJ_BRUSH                            equ 2
OBJ_DC                               equ 3
OBJ_METADC                           equ 4
OBJ_PAL                              equ 5
OBJ_FONT                             equ 6
OBJ_BITMAP                           equ 7
OBJ_REGION                           equ 8
OBJ_METAFILE                         equ 9
OBJ_MEMDC                            equ 10
OBJ_EXTPEN                           equ 11
OBJ_ENHMETADC                        equ 12
OBJ_ENHMETAFILE                      equ 13
MWT_IDENTITY                         equ 1
MWT_LEFTMULTIPLY                     equ 2
MWT_RIGHTMULTIPLY                    equ 3
MWT_MIN                              equ MWT_IDENTITY
MWT_MAX                              equ MWT_RIGHTMULTIPLY
BI_RGB                               equ 0
BI_RLE8                              equ 1
BI_RLE4                              equ 2
BI_bitfields                         equ 3
NTM_REGULAR                          equ 40h
NTM_BOLD                             equ 20h
NTM_ITALIC                           equ 1h
TMPF_FIXED_PITCH                     equ 1h
TMPF_VECTOR                          equ 2h
TMPF_DEVICE                          equ 8h
TMPF_TRUETYPE                        equ 4h
LF_FACESIZE                          equ 32
LF_FULLFACESIZE                      equ 64
OUT_DEFAULT_PRECIS                   equ 0
OUT_STRING_PRECIS                    equ 1
OUT_CHARACTER_PRECIS                 equ 2
OUT_STROKE_PRECIS                    equ 3
OUT_TT_PRECIS                        equ 4
OUT_DEVICE_PRECIS                    equ 5
OUT_RASTER_PRECIS                    equ 6
OUT_TT_ONLY_PRECIS                   equ 7
OUT_OUTLINE_PRECIS                   equ 8
CLIP_DEFAULT_PRECIS                  equ 0
CLIP_CHARACTER_PRECIS                equ 1
CLIP_STROKE_PRECIS                   equ 2
CLIP_MASK                            equ 0Fh
CLIP_LH_ANGLES                       equ 16
CLIP_TT_ALWAYS                       equ 32
CLIP_EMBEDDED                        equ 128
DEFAULT_QUALITY                      equ 0
DRAFT_QUALITY                        equ 1
PROOF_QUALITY                        equ 2
DEFAULT_PITCH                        equ 0
FIXED_PITCH                          equ 1
VARIABLE_PITCH                       equ 2
ANSI_CHARSET                         equ 0
DEFAULT_CHARSET                      equ 1
SYMBOL_CHARSET                       equ 2
SHIFTJIS_CHARSET                     equ 128
GB2312_CHARSET         equ 134
HANGEUL_CHARSET                      equ 129
CHINESEBIG5_CHARSET                  equ 136
OEM_CHARSET                          equ 255
FF_DONTCARE                          equ 0
FF_ROMAN                             equ 16
FF_SWISS                             equ 32
FF_MODERN                            equ 48
FF_SCRIPT                            equ 64
FF_DECORATIVE                        equ 80
FW_DONTCARE                          equ 0
FW_THIN                              equ 100
FW_EXTRALIGHT                        equ 200
FW_LIGHT                             equ 300
FW_NORMAL                            equ 400
FW_MEDIUM                            equ 500
FW_SEMIBOLD                          equ 600
FW_BOLD                              equ 700
FW_EXTRABOLD                         equ 800
FW_HEAVY                             equ 900
FW_ULTRALIGHT                        equ FW_EXTRALIGHT
FW_REGULAR                           equ FW_NORMAL
FW_DEMIBOLD                          equ FW_SEMIBOLD
FW_ULTRABOLD                         equ FW_EXTRABOLD
FW_BLACK                             equ FW_HEAVY
PANOSE_COUNT                         equ 10
PAN_FAMILYTYPE_INDEX                 equ 0
PAN_SERIFSTYLE_INDEX                 equ 1
PAN_WEIGHT_INDEX                     equ 2
PAN_PROPORTION_INDEX                 equ 3
PAN_CONTRAST_INDEX                   equ 4
PAN_STROKEVARIATION_INDEX            equ 5
PAN_ARMSTYLE_INDEX                   equ 6
PAN_LETTERFORM_INDEX                 equ 7
PAN_MIDLINE_INDEX                    equ 8
PAN_XHEIGHT_INDEX                    equ 9
PAN_CULTURE_LATIN                    equ 0
PAN_ANY                              equ 0
PAN_NO_FIT                           equ 1
PAN_FAMILY_TEXT_DISPLAY              equ 2
PAN_FAMILY_SCRIPT                    equ 3
PAN_FAMILY_DECORATIVE                equ 4
PAN_FAMILY_PICTORIAL                 equ 5
PAN_SERIF_COVE                       equ 2
PAN_SERIF_OBTUSE_COVE                equ 3
PAN_SERIF_SQUARE_COVE                equ 4
PAN_SERIF_OBTUSE_SQUARE_COVE         equ 5
PAN_SERIF_SQUARE                     equ 6
PAN_SERIF_THIN                       equ 7
PAN_SERIF_BONE                       equ 8
PAN_SERIF_EXAGGERATED                equ 9
PAN_SERIF_TRIANGLE                   equ 10
PAN_SERIF_NORMAL_SANS                equ 11
PAN_SERIF_OBTUSE_SANS                equ 12
PAN_SERIF_PERP_SANS                  equ 13
PAN_SERIF_FLARED                     equ 14
PAN_SERIF_ROUNDED                    equ 15
PAN_WEIGHT_VERY_LIGHT                equ 2
PAN_WEIGHT_LIGHT                     equ 3
PAN_WEIGHT_THIN                      equ 4
PAN_WEIGHT_BOOK                      equ 5
PAN_WEIGHT_MEDIUM                    equ 6
PAN_WEIGHT_DEMI                      equ 7
PAN_WEIGHT_BOLD                      equ 8
PAN_WEIGHT_HEAVY                     equ 9
PAN_WEIGHT_BLACK                     equ 10
PAN_WEIGHT_NORD                      equ 11
PAN_PROP_OLD_STYLE                   equ 2
PAN_PROP_MODERN                      equ 3
PAN_PROP_EVEN_WIDTH                  equ 4
PAN_PROP_EXPANDED                    equ 5
PAN_PROP_CONDENSED                   equ 6
PAN_PROP_VERY_EXPANDED               equ 7
PAN_PROP_VERY_CONDENSED              equ 8
PAN_PROP_MONOSPACED                  equ 9
PAN_CONTRAST_NONE                    equ 2
PAN_CONTRAST_VERY_LOW                equ 3
PAN_CONTRAST_LOW                     equ 4
PAN_CONTRAST_MEDIUM_LOW              equ 5
PAN_CONTRAST_MEDIUM                  equ 6
PAN_CONTRAST_MEDIUM_HIGH             equ 7
PAN_CONTRAST_HIGH                    equ 8
PAN_CONTRAST_VERY_HIGH               equ 9
PAN_STROKE_GRADUAL_DIAG              equ 2
PAN_STROKE_GRADUAL_TRAN              equ 3
PAN_STROKE_GRADUAL_VERT              equ 4
PAN_STROKE_GRADUAL_HORZ              equ 5
PAN_STROKE_RAPID_VERT                equ 6
PAN_STROKE_RAPID_HORZ                equ 7
PAN_STROKE_INSTANT_VERT              equ 8
PAN_STRAIGHT_ARMS_HORZ               equ 2
PAN_STRAIGHT_ARMS_WEDGE              equ 3
PAN_STRAIGHT_ARMS_VERT               equ 4
PAN_STRAIGHT_ARMS_SINGLE_SERIF       equ 5
PAN_STRAIGHT_ARMS_DOUBLE_SERIF       equ 6
PAN_BENT_ARMS_HORZ                   equ 7
PAN_BENT_ARMS_WEDGE                  equ 8
PAN_BENT_ARMS_VERT                   equ 9
PAN_BENT_ARMS_SINGLE_SERIF           equ 10
PAN_BENT_ARMS_DOUBLE_SERIF           equ 11
PAN_LETT_NORMAL_CONTACT              equ 2
PAN_LETT_NORMAL_WEIGHTED             equ 3
PAN_LETT_NORMAL_BOXED                equ 4
PAN_LETT_NORMAL_FLATTENED            equ 5
PAN_LETT_NORMAL_ROUNDED              equ 6
PAN_LETT_NORMAL_OFF_CENTER           equ 7
PAN_LETT_NORMAL_SQUARE               equ 8
PAN_LETT_OBLIQUE_CONTACT             equ 9
PAN_LETT_OBLIQUE_WEIGHTED            equ 10
PAN_LETT_OBLIQUE_BOXED               equ 11
PAN_LETT_OBLIQUE_FLATTENED           equ 12
PAN_LETT_OBLIQUE_ROUNDED             equ 13
PAN_LETT_OBLIQUE_OFF_CENTER          equ 14
PAN_LETT_OBLIQUE_SQUARE              equ 15
PAN_MIDLINE_STANDARD_TRIMMED         equ 2
PAN_MIDLINE_STANDARD_POINTED         equ 3
PAN_MIDLINE_STANDARD_SERIFED         equ 4
PAN_MIDLINE_HIGH_TRIMMED             equ 5
PAN_MIDLINE_HIGH_POINTED             equ 6
PAN_MIDLINE_HIGH_SERIFED             equ 7
PAN_MIDLINE_CONSTANT_TRIMMED         equ 8
PAN_MIDLINE_CONSTANT_POINTED         equ 9
PAN_MIDLINE_CONSTANT_SERIFED         equ 10
PAN_MIDLINE_LOW_TRIMMED              equ 11
PAN_MIDLINE_LOW_POINTED              equ 12
PAN_MIDLINE_LOW_SERIFED              equ 13
PAN_XHEIGHT_CONSTANT_SMALL           equ 2
PAN_XHEIGHT_CONSTANT_STD             equ 3
PAN_XHEIGHT_CONSTANT_LARGE           equ 4
PAN_XHEIGHT_DUCKING_SMALL            equ 5
PAN_XHEIGHT_DUCKING_STD              equ 6
PAN_XHEIGHT_DUCKING_LARGE            equ 7
ELF_VENDOR_SIZE                      equ 4
ELF_VERSION                          equ 0
ELF_CULTURE_LATIN                    equ 0
RASTER_FONTTYPE                      equ 1h
DEVICE_FONTTYPE                      equ 2h
TRUETYPE_FONTTYPE                    equ 4h
PC_RESERVED                          equ 1h
PC_EXPLICIT                          equ 2h
PC_NOCOLLAPSE                        equ 4h
TRANSPARENT                          equ 1
OPAQUE                               equ 2
BKMODE_LAST                          equ 2
GM_COMPATIBLE                        equ 1
GM_ADVANCED                          equ 2
GM_LAST                              equ 2
PT_CLOSEFIGURE                       equ 1h
PT_LINETO                            equ 2h
PT_BEZIERTO                          equ 4h
PT_MOVETO                            equ 6h
MM_TEXT                              equ 1
MM_LOMETRIC                          equ 2
MM_HIMETRIC                          equ 3
MM_LOENGLISH                         equ 4
MM_HIENGLISH                         equ 5
MM_TWIPS                             equ 6
MM_ISOTROPIC                         equ 7
MM_ANISOTROPIC                       equ 8
MM_MIN                               equ MM_TEXT
MM_MAX                               equ MM_ANISOTROPIC
MM_MAX_FIXEDSCALE                    equ MM_TWIPS
ABSOLUTE                             equ 1
RELATIVE                             equ 2
WHITE_BRUSH                          equ 0
LTGRAY_BRUSH                         equ 1
GRAY_BRUSH                           equ 2
DKGRAY_BRUSH                         equ 3
BLACK_BRUSH                          equ 4
NULL_BRUSH                           equ 5
HOLLOW_BRUSH                         equ NULL_BRUSH
WHITE_PEN                            equ 6
BLACK_PEN                            equ 7
NULL_PEN                             equ 8
OEM_FIXED_FONT                       equ 10
ANSI_FIXED_FONT                      equ 11
ANSI_VAR_FONT                        equ 12
SYSTEM_FONT                          equ 13
DEVICE_DEFAULT_FONT                  equ 14
DEFAULT_PALETTE                      equ 15
SYSTEM_FIXED_FONT                    equ 16
STOCK_LAST                           equ 16
CLR_INVALID                          equ 0FFFFh
BS_SOLID                             equ 0
BS_NULL                              equ 1
BS_HOLLOW                            equ 0BS_NULL
BS_HATCHED                           equ 2
BS_PATTERN                           equ 3
BS_INDEXED                           equ 4
BS_DIBPATTERN                        equ 5
BS_DIBPATTERNPT                      equ 6
BS_PATTERN8X8                        equ 7
BS_DIBPATTERN8X8                     equ 8
HS_HORIZONTAL                        equ 0
HS_VERTICAL                          equ 1
HS_FDIAGONAL                         equ 2
HS_BDIAGONAL                         equ 3
HS_CROSS                             equ 4
HS_DIAGCROSS                         equ 5
HS_FDIAGONAL1                        equ 6
HS_BDIAGONAL1                        equ 7
HS_SOLID                             equ 8
HS_DENSE1                            equ 9
HS_DENSE2                            equ 10
HS_DENSE3                            equ 11
HS_DENSE4                            equ 12
HS_DENSE5                            equ 13
HS_DENSE6                            equ 14
HS_DENSE7                            equ 15
HS_DENSE8                            equ 16
HS_NOSHADE                           equ 17
HS_HALFTONE                          equ 18
HS_SOLIDCLR                          equ 19
HS_DITHEREDCLR                       equ 20
HS_SOLIDTEXTCLR                      equ 21
HS_DITHEREDTEXTCLR                   equ 22
HS_SOLIDBKCLR                        equ 23
HS_DITHEREDBKCLR                     equ 24
HS_API_MAX                           equ 25
PS_SOLID                             equ 0
PS_DASH                              equ 1
PS_DOT                               equ 2
PS_DASHDOT                           equ 3
PS_DASHDOTDOT                        equ 4
PS_NULL                              equ 5
PS_INSIDEFRAME                       equ 6
PS_USERSTYLE                         equ 7
PS_ALTERNATE                         equ 8
PS_STYLE_MASK                        equ 0Fh
PS_ENDCAP_ROUND                      equ 0h
PS_ENDCAP_SQUARE                     equ 100h
PS_ENDCAP_FLAT                       equ 200h
PS_ENDCAP_MASK                       equ 0F00h
PS_JOIN_ROUND                        equ 0h
PS_JOIN_BEVEL                        equ 1000h
PS_JOIN_MITER                        equ 2000h
PS_JOIN_MASK                         equ 0F000h
PS_COSMETIC                          equ 0h
PS_GEOMETRIC                         equ 10000h
PS_TYPE_MASK                         equ F0000h
AD_COUNTERCLOCKWISE                  equ 1
AD_CLOCKWISE                         equ 2
PRF_CHECKVISIBLE                     equ 00000001h
PRF_NONCLIENT                        equ 00000002h
PRF_CLIENT                           equ 00000004h
PRF_ERASEBKGND                       equ 00000008h
PRF_CHILDREN                         equ 00000010h
PRF_OWNED                            equ 00000020h
BDR_RAISEDOUTER                      equ 0001h
BDR_SUNKENOUTER                      equ 0002h
BDR_RAISEDINNER                      equ 0004h
BDR_SUNKENINNER                      equ 0008h
BDR_OUTER                            equ 0003h
BDR_INNER                            equ 000Ch
BDR_RAISED                           equ 0005h
BDR_SUNKEN                           equ 000Ah
EDGE_RAISED                          equ BDR_RAISEDOUTER OR BDR_RAISEDINNER
EDGE_SUNKEN                          equ BDR_SUNKENOUTER OR BDR_SUNKENINNER
EDGE_ETCHED                          equ BDR_SUNKENOUTER OR BDR_RAISEDINNER
EDGE_BUMP                            equ BDR_RAISEDOUTER OR BDR_SUNKENINNER
BF_LEFT                              equ 0001h
BF_TOP                               equ 0002h
BF_RIGHT                             equ 0004h
BF_BOTTOM                            equ 0008h
BF_TOPLEFT                           equ BF_TOP OR BF_LEFT
BF_TOPRIGHT                          equ BF_TOP OR BF_RIGHT
BF_BOTTOMLEFT                        equ BF_BOTTOM OR BF_LEFT
BF_BOTTOMRIGHT                       equ BF_BOTTOM OR BF_RIGHT
BF_RECT                              equ BF_LEFT OR BF_TOP OR BF_RIGHT OR BF_BOTTOM
BF_DIAGONAL                          equ 0010h
BF_DIAGONAL_ENDTOPRIGHT              equ BF_DIAGONAL OR BF_TOP OR BF_RIGHT
BF_DIAGONAL_ENDTOPLEFT               equ BF_DIAGONAL OR BF_TOP OR BF_LEFT
BF_DIAGONAL_ENDBOTTOMLEFT            equ BF_DIAGONAL OR BF_BOTTOM OR BF_LEFT
BF_DIAGONAL_ENDBOTTOMRIGHT           equ BF_DIAGONAL OR BF_BOTTOM OR BF_RIGHT
BF_MIDDLE                            equ 0800h
BF_SOFT                              equ 1000h
BF_ADJUST                            equ 2000h
BF_FLAT                              equ 4000h
BF_MONO                              equ 8000h
DRIVERVERSION                        equ 0
TECHNOLOGY                           equ 2
HORZSIZE                             equ 4
VERTSIZE                             equ 6
HORZRES                              equ 8
VERTRES                              equ 10
BITSPIXEL                            equ 12
PLANES                               equ 14
NUMBRUSHES                           equ 16
NUMPENS                              equ 18
NUMMARKERS                           equ 20
NUMFONTS                             equ 22
NUMCOLORS                            equ 24
PDEVICESIZE                          equ 26
CURVECAPS                            equ 28
LINECAPS                             equ 30
POLYGONALCAPS                        equ 32
TEXTCAPS                             equ 34
CLIPCAPS                             equ 36
RASTERCAPS                           equ 38
ASPECTX                              equ 40
ASPECTY                              equ 42
ASPECTXY                             equ 44
LOGPIXELSX                           equ 88
LOGPIXELSY                           equ 90
SIZEPALETTE                          equ 104
NUMRESERVED                          equ 106
COLORRES                             equ 108
PHYSICALWIDTH                        equ 110
PHYSICALHEIGHT                       equ 111
PHYSICALOFFSETX                      equ 112
PHYSICALOFFSETY                      equ 113
SCALINGFACTORX                       equ 114
SCALINGFACTORY                       equ 115
DT_PLOTTER                           equ 0
DT_RASDISPLAY                        equ 1
DT_RASPRINTER                        equ 2
DT_RASCAMERA                         equ 3
DT_CHARSTREAM                        equ 4
DT_METAFILE                          equ 5
DT_DISPFILE                          equ 6
CC_NONE                              equ 0
CC_CIRCLES                           equ 1
CC_PIE                               equ 2
CC_CHORD                             equ 4
CC_ELLIPSES                          equ 8
CC_WIDE                              equ 16
CC_STYLED                            equ 32
CC_WIDESTYLED                        equ 64
CC_INTERIORS                         equ 128
CC_ROUNDRECT                         equ 256
LC_NONE                              equ 0
LC_POLYLINE                          equ 2
LC_MARKER                            equ 4
LC_POLYMARKER                        equ 8
LC_WIDE                              equ 16
LC_STYLED                            equ 32
LC_WIDESTYLED                        equ 64
LC_INTERIORS                         equ 128
PC_NONE                              equ 0
PC_POLYGON                           equ 1
PC_RECTANGLE                         equ 2
PC_WINDPOLYGON                       equ 4
PC_TRAPEZOID                         equ 4
PC_SCANLINE                          equ 8
PC_WIDE                              equ 16
PC_STYLED                            equ 32
PC_WIDESTYLED                        equ 64
PC_INTERIORS                         equ 128
CP_NONE                              equ 0
CP_RECTANGLE                         equ 1
CP_REGION                            equ 2
TC_OP_CHARACTER                      equ 1h
TC_OP_STROKE                         equ 2h
TC_CP_STROKE                         equ 4h
TC_CR_90                             equ 8h
TC_CR_ANY                            equ 10h
TC_SF_X_YINDEP                       equ 20h
TC_SA_DOUBLE                         equ 40h
TC_SA_INTEGER                        equ 80h
TC_SA_CONTIN                         equ 100h
TC_EA_DOUBLE                         equ 200h
TC_IA_ABLE                           equ 400h
TC_UA_ABLE                           equ 800h
TC_SO_ABLE                           equ 1000h
TC_RA_ABLE                           equ 2000h
TC_VA_ABLE                           equ 4000h
TC_RESERVED                          equ 8000h
TC_SCROLLBLT                         equ 10000h
RC_NONE                              equ 0
RC_BITBLT                            equ 1
RC_BANDING                           equ 2
RC_SCALING                           equ 4
RC_BITMAP64                          equ 8
RC_GDI20_OUTPUT                      equ 10h
RC_GDI20_STATE                       equ 20h
RC_SAVEBITMAP                        equ 40h
RC_DI_BITMAP                         equ 80h
RC_PALETTE                           equ 100h
RC_DIBTODEV                          equ 200h
RC_BIGFONT                           equ 400h
RC_STRETCHBLT                        equ 800h
RC_FLOODFILL                         equ 1000h
RC_STRETCHDIB                        equ 2000h
RC_OP_DX_OUTPUT                      equ 4000h
RC_DEVBITS                           equ 8000h
DIB_RGB_COLORS                       equ 0
DIB_PAL_COLORS                       equ 1
DIB_PAL_INDICES                      equ 2
DIB_PAL_PHYSINDICES                  equ 2
DIB_PAL_LOGINDICES                   equ 4
SYSPAL_ERROR                         equ 0
SYSPAL_STATIC                        equ 1
SYSPAL_NOSTATIC                      equ 2
CBM_CREATEDIB                        equ 2h
CBM_INIT                             equ 4h
FLOODFILLBORDER                      equ 0
FLOODFILLSURFACE                     equ 1
CCHDEVICENAME                        equ 32
CCHFORMNAME                          equ 32
DM_SPECVERSION                       equ 320h
DM_ORIENTATION                       equ 1h
DM_PAPERSIZE                         equ 2h
DM_PAPERLENGTH                       equ 4h
DM_PAPERWIDTH                        equ 8h
DM_SCALE                             equ 10h
DM_COPIES                            equ 100h
DM_DEFAULTSOURCE                     equ 200h
DM_PRINTQUALITY                      equ 400h
DM_COLOR                             equ 800h
DM_DUPLEX                            equ 1000h
DM_YRESOLUTION                       equ 2000h
DM_TTOPTION                          equ 4000h
DM_COLLATE                           equ 8000h
DM_FORMNAME                          equ 10000h
DMORIENT_PORTRAIT                    equ 1
DMORIENT_LANDSCAPE                   equ 2
DMPAPER_LETTER                       equ 1
DMPAPER_FIRST                        equ DMPAPER_LETTER
DMPAPER_LETTERSMALL                  equ 2
DMPAPER_TABLOID                      equ 3
DMPAPER_LEDGER                       equ 4
DMPAPER_LEGAL                        equ 5
DMPAPER_STATEMENT                    equ 6
DMPAPER_EXECUTIVE                    equ 7
DMPAPER_A3                           equ 8
DMPAPER_A4                           equ 9
DMPAPER_A4SMALL                      equ 10
DMPAPER_A5                           equ 11
DMPAPER_B4                           equ 12
DMPAPER_B5                           equ 13
DMPAPER_FOLIO                        equ 14
DMPAPER_QUARTO                       equ 15
DMPAPER_10X14                        equ 16
DMPAPER_11X17                        equ 17
DMPAPER_NOTE                         equ 18
DMPAPER_ENV_9                        equ 19
DMPAPER_ENV_10                       equ 20
DMPAPER_ENV_11                       equ 21
DMPAPER_ENV_12                       equ 22
DMPAPER_ENV_14                       equ 23
DMPAPER_CSHEET                       equ 24
DMPAPER_DSHEET                       equ 25
DMPAPER_ESHEET                       equ 26
DMPAPER_ENV_DL                       equ 27
DMPAPER_ENV_C5                       equ 28
DMPAPER_ENV_C3                       equ 29
DMPAPER_ENV_C4                       equ 30
DMPAPER_ENV_C6                       equ 31
DMPAPER_ENV_C65                      equ 32
DMPAPER_ENV_B4                       equ 33
DMPAPER_ENV_B5                       equ 34
DMPAPER_ENV_B6                       equ 35
DMPAPER_ENV_ITALY                    equ 36
DMPAPER_ENV_MONARCH                  equ 37
DMPAPER_ENV_PERSONAL                 equ 38
DMPAPER_FANFOLD_US                   equ 39
DMPAPER_FANFOLD_STD_GERMAN           equ 40
DMPAPER_FANFOLD_LGL_GERMAN           equ 41
DMPAPER_LAST                         equ DMPAPER_FANFOLD_LGL_GERMAN
DMPAPER_USER                         equ 256
DMBIN_UPPER                          equ 1
DMBIN_FIRST                          equ DMBIN_UPPER
DMBIN_ONLYONE                        equ 1
DMBIN_LOWER                          equ 2
DMBIN_MIDDLE                         equ 3
DMBIN_MANUAL                         equ 4
DMBIN_ENVELOPE                       equ 5
DMBIN_ENVMANUAL                      equ 6
DMBIN_AUTO                           equ 7
DMBIN_TRACTOR                        equ 8
DMBIN_SMALLFMT                       equ 9
DMBIN_LARGEFMT                       equ 10
DMBIN_LARGECAPACITY                  equ 11
DMBIN_CASSETTE                       equ 14
DMBIN_LAST                           equ DMBIN_CASSETTE
DMBIN_USER                           equ 256
DMRES_DRAFT                          equ -1
DMRES_LOW                            equ -2
DMRES_MEDIUM                         equ -3
DMRES_HIGH                           equ -4
DMCOLOR_MONOCHROME                   equ 1
DMCOLOR_COLOR                        equ 2
DMDUP_SIMPLEX                        equ 1
DMDUP_VERTICAL                       equ 2
DMDUP_HORIZONTAL                     equ 3
DMTT_BITMAP                          equ 1
DMTT_DOWNLOAD                        equ 2
DMTT_SUBDEV                          equ 3
DMCOLLATE_FALSE                      equ 0
DMCOLLATE_TRUE                       equ 1
DM_GRAYSCALE                         equ 1h
DM_INTERLACED                        equ 2h
RDH_RECTANGLES                       equ 1
GGO_METRICS                          equ 0
GGO_BITMAP                           equ 1
GGO_NATIVE                           equ 2
TT_POLYGON_TYPE                      equ 24
TT_PRIM_LINE                         equ 1
TT_PRIM_QSPLINE                      equ 2
TT_AVAILABLE                         equ 1h
TT_ENABLED                           equ 2h
DM_UPDATE                            equ 1
DM_COPY                              equ 2
DM_PROMPT                            equ 4
DM_MODIFY                            equ 8
DM_IN_BUFFER                         equ DM_MODIFY
DM_IN_PROMPT                         equ DM_PROMPT
DM_OUT_BUFFER                        equ DM_COPY
DM_OUT_DEFAULT                       equ DM_UPDATE
DC_FIELDS                            equ 1
DC_PAPERS                            equ 2
DC_PAPERSIZE                         equ 3
DC_MINEXTENT                         equ 4
DC_MAXEXTENT                         equ 5
DC_BINS                              equ 6
DC_DUPLEX                            equ 7
DC_SIZE                              equ 8
DC_EXTRA                             equ 9
DC_VERSION                           equ 10
DC_DRIVER                            equ 11
DC_BINNAMES                          equ 12
DC_ENUMRESOLUTIONS                   equ 13
DC_FILEDEPENDENCIES                  equ 14
DC_TRUETYPE                          equ 15
DC_PAPERNAMES                        equ 16
DC_ORIENTATION                       equ 17
DC_COPIES                            equ 18
DCTT_BITMAP                          equ 1h
DCTT_DOWNLOAD                        equ 2h
DCTT_SUBDEV                          equ 4h
CA_NEGATIVE                          equ 1h
CA_LOG_FILTER                        equ 2h
ILLUMINANT_DEVICE_DEFAULT            equ 0
ILLUMINANT_A                         equ 1
ILLUMINANT_B                         equ 2
ILLUMINANT_C                         equ 3
ILLUMINANT_D50                       equ 4
ILLUMINANT_D55                       equ 5
ILLUMINANT_D65                       equ 6
ILLUMINANT_D75                       equ 7
ILLUMINANT_F2                        equ 8
ILLUMINANT_MAX_INDEX                 equ ILLUMINANT_F2
ILLUMINANT_TUNGSTEN                  equ ILLUMINANT_A
ILLUMINANT_DAYLIGHT                  equ ILLUMINANT_C
ILLUMINANT_FLUORESCENT               equ ILLUMINANT_F2
ILLUMINANT_NTSC                      equ ILLUMINANT_C
RGB_GAMMA_MIN                        equ 2500
RGB_GAMMA_MAX                        equ 65000
REFERENCE_WHITE_MIN                  equ 6000
REFERENCE_WHITE_MAX                  equ 10000
REFERENCE_BLACK_MIN                  equ 0
REFERENCE_BLACK_MAX                  equ 4000
COLOR_ADJ_MIN                        equ -100
COLOR_ADJ_MAX                        equ 100
FONTMAPPER_MAX                       equ 10
ENHMETA_SIGNATURE                    equ 464D4520h
ENHMETA_STOCK_OBJECT                 equ 80000000h
EMR_HEADER                           equ 1
EMR_POLYBEZIER                       equ 2
EMR_POLYGON                          equ 3
EMR_POLYLINE                         equ 4
EMR_POLYBEZIERTO                     equ 5
EMR_POLYLINETO                       equ 6
EMR_POLYPOLYLINE                     equ 7
EMR_POLYPOLYGON                      equ 8
EMR_SETWINDOWEXTEX                   equ 9
EMR_SETWINDOWORGEX                   equ 10
EMR_SETVIEWPORTEXTEX                 equ 11
EMR_SETVIEWPORTORGEX                 equ 12
EMR_SETBRUSHORGEX                    equ 13
EMR_EOF                              equ 14
EMR_SETPIXELV                        equ 15
EMR_SETMAPPERFLAGS                   equ 16
EMR_SETMAPMODE                       equ 17
EMR_SETBKMODE                        equ 18
EMR_SETPOLYFILLMODE                  equ 19
EMR_SETROP2                          equ 20
EMR_SETSTRETCHBLTMODE                equ 21
EMR_SETTEXTALIGN                     equ 22
EMR_SETCOLORADJUSTMENT               equ 23
EMR_SETTEXTCOLOR                     equ 24
EMR_SETBKCOLOR                       equ 25
EMR_OFFSETCLIPRGN                    equ 26
EMR_MOVETOEX                         equ 27
EMR_SETMETARGN                       equ 28
EMR_EXCLUDECLIPRECT                  equ 29
EMR_INTERSECTCLIPRECT                equ 30
EMR_SCALEVIEWPORTEXTEX               equ 31
EMR_SCALEWINDOWEXTEX                 equ 32
EMR_SAVEDC                           equ 33
EMR_RESTOREDC                        equ 34
EMR_SETWORLDTRANSFORM                equ 35
EMR_MODIFYWORLDTRANSFORM             equ 36
EMR_SELECTOBJECT                     equ 37
EMR_CREATEPEN                        equ 38
EMR_CREATEBRUSHINDIRECT              equ 39
EMR_DELETEOBJECT                     equ 40
EMR_ANGLEARC                         equ 41
EMR_ELLIPSE                          equ 42
EMR_RECTANGLE                        equ 43
EMR_ROUNDRECT                        equ 44
EMR_ARC                              equ 45
EMR_CHORD                            equ 46
EMR_PIE                              equ 47
EMR_SELECTPALETTE                    equ 48
EMR_CREATEPALETTE                    equ 49
EMR_SETPALETTEENTRIES                equ 50
EMR_RESIZEPALETTE                    equ 51
EMR_REALIZEPALETTE                   equ 52
EMR_EXTFLOODFILL                     equ 53
EMR_LINETO                           equ 54
EMR_ARCTO                            equ 55
EMR_POLYDRAW                         equ 56
EMR_SETARCDIRECTION                  equ 57
EMR_SETMITERLIMIT                    equ 58
EMR_BEGINPATH                        equ 59
EMR_ENDPATH                          equ 60
EMR_CLOSEFIGURE                      equ 61
EMR_FILLPATH                         equ 62
EMR_STROKEANDFILLPATH                equ 63
EMR_STROKEPATH                       equ 64
EMR_FLATTENPATH                      equ 65
EMR_WIDENPATH                        equ 66
EMR_SELECTCLIPPATH                   equ 67
EMR_ABORTPATH                        equ 68
EMR_GDICOMMENT                       equ 70
EMR_FILLRGN                          equ 71
EMR_FRAMERGN                         equ 72
EMR_INVERTRGN                        equ 73
EMR_PAINTRGN                         equ 74
EMR_EXTSELECTCLIPRGN                 equ 75
EMR_BITBLT                           equ 76
EMR_STRETCHBLT                       equ 77
EMR_MASKBLT                          equ 78
EMR_PLGBLT                           equ 79
EMR_SETDIBITSTODEVICE                equ 80
EMR_STRETCHDIBITS                    equ 81
EMR_EXTCREATEFONTINDIRECTW           equ 82
EMR_EXTTEXTOUTA                      equ 83
EMR_EXTTEXTOUTW                      equ 84
EMR_POLYBEZIER16                     equ 85
EMR_POLYGON16                        equ 86
EMR_POLYLINE16                       equ 87
EMR_POLYBEZIERTO16                   equ 88
EMR_POLYLINETO16                     equ 89
EMR_POLYPOLYLINE16                   equ 90
EMR_POLYPOLYGON16                    equ 91
EMR_POLYDRAW16                       equ 92
EMR_CREATEMONOBRUSH                  equ 93
EMR_CREATEDIBPATTERNBRUSHPT          equ 94
EMR_EXTCREATEPEN                     equ 95
EMR_POLYTEXTOUTA                     equ 96
EMR_POLYTEXTOUTW                     equ 97
EMR_MIN                              equ 1
EMR_MAX                              equ 97
STRETCH_ANDSCANS                     equ 1
STRETCH_ORSCANS                      equ 2
STRETCH_DELETESCANS                  equ 3
STRETCH_HALFTONE                     equ 4
TCI_SRCCHARSET                       equ 1
TCI_SRCCODEPAGE                      equ 2
TCI_SRCFONTSIG                       equ 3
MONO_FONT                            equ 8
JOHAB_CHARSET                        equ 130
HEBREW_CHARSET                       equ 177
ARABIC_CHARSET                       equ 178
GREEK_CHARSET                        equ 161
TURKISH_CHARSET                      equ 162
THAI_CHARSET                         equ 222
EASTEUROPE_CHARSET                   equ 238
RUSSIAN_CHARSET                      equ 204
MAC_CHARSET                          equ 77
BALTIC_CHARSET                       equ 186
FS_LATIN1                            equ 1h
FS_LATIN2                            equ 2h
FS_CYRILLIC                          equ 4h
FS_GREEK                             equ 8h
FS_TURKISH                           equ 10h
FS_HEBREW                            equ 20h
FS_ARABIC                            equ 40h
FS_BALTIC                            equ 80h
FS_THAI                              equ 10000h
FS_JISJAPAN                          equ 20000h
FS_CHINESESIMP                       equ 40000h
FS_WANSUNG                           equ 80000h
FS_CHINESETRAD                       equ 100000h
FS_JOHAB                             equ 200000h
FS_SYMBOL                            equ 80000000h
DEFAULT_GUI_FONT                     equ 17
DM_RESERVED1                         equ 800000h
DM_RESERVED2                         equ 1000000h
DM_ICMMETHOD                         equ 2000000h
DM_ICMINTENT                         equ 4000000h
DM_MEDIATYPE                         equ 8000000h
DM_DITHERTYPE                        equ 10000000h
DMPAPER_ISO_B4                       equ 42
DMPAPER_JAPANESE_POSTCARD            equ 43
DMPAPER_9X11                         equ 44
DMPAPER_10X11                        equ 45
DMPAPER_15X11                        equ 46
DMPAPER_ENV_INVITE                   equ 47
DMPAPER_RESERVED_48                  equ 48
DMPAPER_RESERVED_49                  equ 49
DMPAPER_LETTER_EXTRA                 equ 50
DMPAPER_LEGAL_EXTRA                  equ 51
DMPAPER_TABLOID_EXTRA                equ 52
DMPAPER_A4_EXTRA                     equ 53
DMPAPER_LETTER_TRANSVERSE            equ 54
DMPAPER_A4_TRANSVERSE                equ 55
DMPAPER_LETTER_EXTRA_TRANSVERSE      equ 56
DMPAPER_A_PLUS                       equ 57
DMPAPER_B_PLUS                       equ 58
DMPAPER_LETTER_PLUS                  equ 59
DMPAPER_A4_PLUS                      equ 60
DMPAPER_A5_TRANSVERSE                equ 61
DMPAPER_B5_TRANSVERSE                equ 62
DMPAPER_A3_EXTRA                     equ 63
DMPAPER_A5_EXTRA                     equ 64
DMPAPER_B5_EXTRA                     equ 65
DMPAPER_A2                           equ 66
DMPAPER_A3_TRANSVERSE                equ 67
DMPAPER_A3_EXTRA_TRANSVERSE          equ 68
DMTT_DOWNLOAD_OUTLINE                equ 4
DMICMMETHOD_NONE                     equ 1
DMICMMETHOD_SYSTEM                   equ 2
DMICMMETHOD_DRIVER                   equ 3
DMICMMETHOD_DEVICE                   equ 4
DMICMMETHOD_USER                     equ 256
DMICM_SATURATE                       equ 1
DMICM_CONTRAST                       equ 2
DMICM_COLORMETRIC                    equ 3
DMICM_USER                           equ 256
DMMEDIA_STANDARD                     equ 1
DMMEDIA_GLOSSY                       equ 2
DMMEDIA_TRANSPARENCY                 equ 3
DMMEDIA_USER                         equ 256
DMDITHER_NONE                        equ 1
DMDITHER_COARSE                      equ 2
DMDITHER_FINE                        equ 3
DMDITHER_LINEART                     equ 4
DMDITHER_GRAYSCALE                   equ 5
DMDITHER_USER                        equ 256
GGO_GRAY2_BITMAP                     equ 4
GGO_GRAY4_BITMAP                     equ 5
GGO_GRAY8_BITMAP                     equ 6
GGO_GLYPH_INDEX                      equ 80h
GCP_DBCS                             equ 1h
GCP_REORDER                          equ 2h
GCP_USEKERNING                       equ 8h
GCP_GLYPHSHAPE                       equ 10h
GCP_LIGATE                           equ 20h
GCP_DIACRITIC                        equ 100h
GCP_KASHIDA                          equ 400h
GCP_ERROR                            equ 8000h
FLI_MASK                             equ 103Bh
GCP_JUSTIFY                          equ 10000h
GCP_NODIACRITICS                     equ 20000h
FLI_GLYPHS                           equ 40000h
GCP_CLASSIN                          equ 80000h
GCP_MAXEXTENT                        equ 100000h
GCP_JUSTIFYIN                        equ 200000h
GCP_DISPLAYZWG                       equ 400000h
GCP_SYMSWAPOFF                       equ 800000h
GCP_NUMERICOVERRIDE                  equ 1000000h
GCP_NEUTRALOVERRIDE                  equ 2000000h
GCP_NUMERICSLATIN                    equ 4000000h
GCP_NUMERICSLOCAL                    equ 8000000h
GCPCLASS_LATIN                       equ 1
GCPCLASS_HEBREW                      equ 2
GCPCLASS_ARABIC                      equ 2
GCPCLASS_NEUTRAL                     equ 3
GCPCLASS_LOCALNUMBER                 equ 4
GCPCLASS_LATINNUMBER                 equ 5
GCPCLASS_LATINNUMERICTERMINATOR      equ 6
GCPCLASS_LATINNUMERICSEPARATOR       equ 7
GCPCLASS_NUMERICSEPARATOR            equ 8
GCPCLASS_PREBOUNDRTL                 equ 80h
GCPCLASS_PREBOUNDLTR                 equ 40h
DC_BINADJUST                         equ 19
DC_EMF_COMPLIANT                     equ 20
DC_DATATYPE_PRODUCED                 equ 21
DC_COLLATE                           equ 22
DCTT_DOWNLOAD_OUTLINE                equ 8h
DCBA_FACEUPNONE                      equ 0h
DCBA_FACEUPCENTER                    equ 1h
DCBA_FACEUPLEFT                      equ 2h
DCBA_FACEUPRIGHT                     equ 3h
DCBA_FACEDOWNNONE                    equ 100h
DCBA_FACEDOWNCENTER                  equ 101h
DCBA_FACEDOWNLEFT                    equ 102h
DCBA_FACEDOWNRIGHT                   equ 103h
ICM_OFF                              equ 1
ICM_ON                               equ 2
ICM_QUERY                            equ 3
EMR_SETICMMODE                       equ 98
EMR_CREATECOLORSPACE                 equ 99
EMR_SETCOLORSPACE                    equ 100
EMR_DELETECOLORSPACE                 equ 101
SB_HORZ                              equ 0
SB_VERT                              equ 1
SB_CTL                               equ 2
SB_BOTH                              equ 3
SB_LINEUP                            equ 0
SB_LINELEFT                          equ 0
SB_LINEDOWN                          equ 1
SB_LINERIGHT                         equ 1
SB_PAGEUP                            equ 2
SB_PAGELEFT                          equ 2
SB_PAGEDOWN                          equ 3
SB_PAGERIGHT                         equ 3
SB_THUMBPOSITION                     equ 4
SB_THUMBTRACK                        equ 5
SB_TOP                               equ 6
SB_LEFT                              equ 6
SB_BOTTOM                            equ 7
SB_RIGHT                             equ 7
SB_ENDSCROLL                         equ 8
SBM_SETSCROLLINFO                    equ 00E9h
SBM_GETSCROLLINFO                    equ 00EAh
SIF_RANGE                            equ 0001h
SIF_PAGE                             equ 0002h
SIF_POS                              equ 0004h
SIF_DISABLENOSCROLL                  equ 0008h
SIF_TRACKPOS                         equ 0010h
SIF_ALL                              equ SIF_RANGE OR SIF_PAGE OR SIF_POS OR SIF_TRACKPOS
SW_HIDE                              equ 0
SW_SHOWNORMAL                        equ 1
SW_NORMAL                            equ 1
SW_SHOWMINIMIZED                     equ 2
SW_SHOWMAXIMIZED                     equ 3
SW_MAXIMIZE                          equ 3
SW_SHOWNOACTIVATE                    equ 4
SW_SHOW                              equ 5
SW_MINIMIZE                          equ 6
SW_SHOWMINNOACTIVE                   equ 7
SW_SHOWNA                            equ 8
SW_RESTORE                           equ 9
SW_SHOWDEFAULT                       equ 10
SW_MAX                               equ 10
HIDE_WINDOW                          equ 0
SHOW_OPENWINDOW                      equ 1
SHOW_ICONWINDOW                      equ 2
SHOW_FULLSCREEN                      equ 3
SHOW_OPENNOACTIVATE                  equ 4
SW_PARENTCLOSING                     equ 1
SW_OTHERZOOM                         equ 2
SW_PARENTOPENING                     equ 3
SW_OTHERUNZOOM                       equ 4
KF_EXTENDED                          equ 100h
KF_DLGMODE                           equ 800h
KF_MENUMODE                          equ 1000h
KF_ALTDOWN                           equ 2000h
KF_REPEAT                            equ 4000h
KF_UP                                equ 8000h
VK_BACK                              equ 8h
VK_CANCEL                            equ 3h
VK_CAPITAL                           equ 14h
VK_CLEAR                             equ 0Ch
VK_CONTROL                           equ 11h
VK_DELETE                            equ 2Eh
VK_DOWN                              equ 28h
VK_END                               equ 23h
VK_ESCAPE                            equ 1Bh
VK_EXECUTE                           equ 2Bh
VK_HELP                              equ 2Fh
VK_HOME                              equ 24h
VK_INSERT                            equ 2Dh
VK_LBUTTON                           equ 1h
VK_LEFT                              equ 25h
VK_MBUTTON                           equ 4h
VK_MENU                              equ 12h
VK_NEXT                              equ 22h
VK_PAUSE                             equ 13h
VK_PGDN                              equ 22h
VK_PGUP                              equ 21h
VK_PRINT                             equ 2Ah
VK_PRIOR                             equ 21h
VK_RBUTTON                           equ 2h
VK_RETURN                            equ 0Dh
VK_RIGHT                             equ 27h
VK_SELECT                            equ 29h
VK_SHIFT                             equ 10h
VK_SNAPSHOT                          equ 2Ch
VK_SPACE                             equ 20h
VK_TAB                               equ 9h
VK_UP                                equ 26h
VK_NUMPAD0                           equ 60h
VK_NUMPAD1                           equ 61h
VK_NUMPAD2                           equ 62h
VK_NUMPAD3                           equ 63h
VK_NUMPAD4                           equ 64h
VK_NUMPAD5                           equ 65h
VK_NUMPAD6                           equ 66h
VK_NUMPAD7                           equ 67h
VK_NUMPAD8                           equ 68h
VK_NUMPAD9                           equ 69h
VK_MULTIPLY                          equ 6Ah
VK_ADD                               equ 6Bh
VK_SEPARATOR                         equ 6Ch
VK_SUBTRACT                          equ 6Dh
VK_DECIMAL                           equ 6Eh
VK_DIVIDE                            equ 6Fh
VK_F1                                equ 70h
VK_F2                                equ 71h
VK_F3                                equ 72h
VK_F4                                equ 73h
VK_F5                                equ 74h
VK_F6                                equ 75h
VK_F7                                equ 76h
VK_F8                                equ 77h
VK_F9                                equ 78h
VK_F10                               equ 79h
VK_F11                               equ 7Ah
VK_F12                               equ 7Bh
VK_F13                               equ 7Ch
VK_F14                               equ 7Dh
VK_F15                               equ 7Eh
VK_F16                               equ 7Fh
VK_F17                               equ 80h
VK_F18                               equ 81h
VK_F19                               equ 82h
VK_F20                               equ 83h
VK_F21                               equ 84h
VK_F22                               equ 85h
VK_F23                               equ 86h
VK_F24                               equ 87h
VK_NUMLOCK                           equ 90h
VK_SCROLL                            equ 91h
VK_LSHIFT                            equ 0A0h
VK_RSHIFT                            equ 0A1h
VK_LCONTROL                          equ 0A2h
VK_RCONTROL                          equ 0A3h
VK_LMENU                             equ 0A4h
VK_RMENU                             equ 0A5h
VK_ATTN                              equ 0F6h
VK_CRSEL                             equ 0F7h
VK_EXSEL                             equ 0F8h
VK_EREOF                             equ 0F9h
VK_PLAY                              equ 0FAh
VK_ZOOM                              equ 0FBh
VK_NONAME                            equ 0FCh
VK_PA1                               equ 0FDh
VK_OEM_CLEAR                         equ 0FEh
VK_0						 equ 30h
VK_1						 equ 31h
VK_2						 equ 32h
VK_3						 equ 33h
VK_4						 equ 34h
VK_5						 equ 35h
VK_6						 equ 36h
VK_7						 equ 37h
VK_8						 equ 38h
VK_9						 equ 39h
VK_A						 equ 41h
VK_B						 equ 42h
VK_C						 equ 43h
VK_D						 equ 44h
VK_E						 equ 45h
VK_F						 equ 46h
VK_G						 equ 47h
VK_H						 equ 48h
VK_I						 equ 49h
VK_J						 equ 4Ah
VK_K						 equ 4Bh
VK_L						 equ 4Ch
VK_M						 equ 4Dh
VK_N						 equ 4Eh
VK_O						 equ 4Fh
VK_P						 equ 50h
VK_Q						 equ 51h
VK_R						 equ 52h
VK_S						 equ 53h
VK_T						 equ 54h
VK_U						 equ 55h
VK_V						 equ 56h
VK_W						 equ 57h
VK_X						 equ 58h
VK_Y						 equ 59h
VK_Z						 equ 5Ah
WH_MIN                               equ -1
WH_MSGFILTER                         equ -1
WH_JOURNALRECORD                     equ 0
WH_JOURNALPLAYBACK                   equ 1
WH_KEYBOARD                          equ 2
WH_GETMESSAGE                        equ 3
WH_CALLWNDPROC                       equ 4
WH_CBT                               equ 5
WH_SYSMSGFILTER                      equ 6
WH_MOUSE                             equ 7
WH_HARDWARE                          equ 8
WH_DEBUG                             equ 9
WH_SHELL                             equ 10
WH_FOREGROUNDIDLE                    equ 11
WH_MAX                               equ 11
HC_ACTION                            equ 0
HC_GETNEXT                           equ 1
HC_SKIP                              equ 2
HC_NOREMOVE                          equ 3
HC_NOREM                             equ HC_NOREMOVE
HC_SYSMODALON                        equ 4
HC_SYSMODALOFF                       equ 5
HCBT_MOVESIZE                        equ 0
HCBT_MINMAX                          equ 1
HCBT_QS                              equ 2
HCBT_CREATEWND                       equ 3
HCBT_DESTROYWND                      equ 4
HCBT_ACTIVATE                        equ 5
HCBT_CLICKSKIPPED                    equ 6
HCBT_KEYSKIPPED                      equ 7
HCBT_SYSCOMMAND                      equ 8
HCBT_SETFOCUS                        equ 9
HSHELL_WINDOWCREATED                 equ 1
HSHELL_WINDOWDESTROYED               equ 2
HSHELL_ACTIVATESHELLWINDOW           equ 3
HKL_PREV                             equ 0
HKL_NEXT                             equ 1
KLF_ACTIVATE                         equ 1h
KLF_SUBSTITUTE_OK                    equ 2h
KLF_UNLOADPREVIOUS                   equ 4h
KLF_REORDER                          equ 8h
KL_NAMELENGTH                        equ 9
DESKTOP_READOBJECTS                  equ 1h
DESKTOP_CREATEWINDOW                 equ 2h
DESKTOP_CREATEMENU                   equ 4h
DESKTOP_HOOKCONTROL                  equ 8h
DESKTOP_JOURNALRECORD                equ 10h
DESKTOP_JOURNALPLAYBACK              equ 20h
DESKTOP_ENUMERATE                    equ 40h
DESKTOP_WRITEOBJECTS                 equ 80h
WINSTA_ENUMDESKTOPS                  equ 1h
WINSTA_READATTRIBUTES                equ 2h
WINSTA_ACCESSCLIPBOARD               equ 4h
WINSTA_CREATEDESKTOP                 equ 8h
WINSTA_WRITEATTRIBUTES               equ 10h
WINSTA_ACCESSGLOBALATOMS             equ 20h
WINSTA_EXITWINDOWS                   equ 40h
WINSTA_ENUMERATE                     equ 100h
WINSTA_READSCREEN                    equ 200h
GWL_WNDPROC                          equ -4
GWL_HINSTANCE                        equ -6
GWL_HWNDPARENT                       equ -8
GWL_STYLE                            equ -16
GWL_EXSTYLE                          equ -20
GWL_USERDATA                         equ -21
GWL_ID                               equ -12
GCL_MENUNAME                         equ -8
GCL_HBRBACKGROUND                    equ -10
GCL_HCURSOR                          equ -12
GCL_HICON                            equ -14
GCL_HMODULE                          equ -16
GCL_CBWNDEXTRA                       equ -18
GCL_CBCLSEXTRA                       equ -20
GCL_WNDPROC                          equ -24
GCL_STYLE                            equ -26
GCW_ATOM                             equ -32
WM_USER                              equ 400h
WM_NULL                              equ 0h
WM_CREATE                            equ 1h
WM_DESTROY                           equ 2h
WM_MOVE                              equ 3h
WM_SIZE                              equ 5h
WM_ACTIVATE                          equ 6h
WA_INACTIVE                          equ 0
WA_ACTIVE                            equ 1
WA_CLICKACTIVE                       equ 2
WM_SETFOCUS                          equ 7h
WM_KILLFOCUS                         equ 08h
WM_ENABLE                            equ 0Ah
WM_SETREDRAW                         equ 0Bh
WM_SETTEXT                           equ 0Ch
WM_GETTEXT                           equ 0Dh
WM_GETTEXTLENGTH                     equ 0Eh
WM_PAINT                             equ 0Fh
WM_CLOSE                             equ 10h
WM_QUERYENDSESSION                   equ 11h
WM_QUIT                              equ 12h
WM_QUERYOPEN                         equ 13h
WM_ERASEBKGND                        equ 14h
WM_SYSCOLORCHANGE                    equ 15h
WM_ENDSESSION                        equ 16h
WM_SHOWWINDOW                        equ 18h
WM_WININICHANGE                      equ 1Ah
WM_SETTINGCHANGE                	  equ WM_WININICHANGE
WM_DEVMODECHANGE                     equ 1Bh
WM_ACTIVATEAPP                       equ 1Ch
WM_FONTCHANGE                        equ 1Dh
WM_TIMECHANGE                        equ 1Eh
WM_CANCELMODE                        equ 1Fh
WM_SETCURSOR                         equ 20h
WM_MOUSEACTIVATE                     equ 21h
WM_CHILDACTIVATE                     equ 22h
WM_QUEUESYNC                         equ 23h
WM_GETMINMAXINFO                     equ 24h
WM_PAINTICON                         equ 26h
WM_ICONERASEBKGND                    equ 27h
WM_NEXTDLGCTL                        equ 28h
WM_SPOOLERSTATUS                     equ 2Ah
WM_DRAWITEM                          equ 2Bh
WM_MEASUREITEM                       equ 2Ch
WM_DELETEITEM                        equ 2Dh
WM_VKEYTOITEM                        equ 2Eh
WM_CHARTOITEM                        equ 2Fh
WM_SETFONT                           equ 30h
WM_GETFONT                           equ 31h
WM_SETHOTKEY                         equ 32h
WM_GETHOTKEY                         equ 33h
WM_QUERYDRAGICON                     equ 37h
WM_COMPAREITEM                       equ 39h
WM_GETOBJECT                    	  equ 3Dh
WM_COMPACTING                        equ 41h
WM_OTHERWINDOWCREATED                equ 42h
WM_OTHERWINDOWDESTROYED              equ 43h
WM_COMMNOTIFY                        equ 44h
CN_RECEIVE                           equ 1h
CN_TRANSMIT                          equ 2h
CN_EVENT                             equ 4h
WM_WINDOWPOSCHANGING                 equ 46h
WM_WINDOWPOSCHANGED                  equ 47h
WM_POWER                             equ 48h
PWR_OK                               equ 1
PWR_FAIL                             equ -1
PWR_SUSPENDREQUEST                   equ 1
PWR_SUSPENDRESUME                    equ 2
PWR_CRITICALRESUME                   equ 3
WM_COPYDATA                          equ 4Ah
WM_CANCELJOURNAL                     equ 4Bh
WM_NOTIFY                            equ 4Eh
WM_INPUTLANGCHANGEREQUEST            equ 50h
WM_INPUTLANGCHANGE                   equ 51h
WM_TCARD                             equ 52h
WM_HELP                              equ 53h
WM_USERCHANGED                       equ 54h
WM_NOTIFYFORMAT                      equ 55h
WM_CONTEXTMENU                       equ 7Bh
WM_STYLECHANGING                     equ 7Ch
WM_STYLECHANGED                      equ 7Dh
WM_DISPLAYCHANGE                     equ 7Eh
WM_GETICON                           equ 7Fh
WM_SETICON                           equ 80h
WM_NCCREATE                          equ 81h
WM_NCDESTROY                         equ 82h
WM_NCCALCSIZE                        equ 83h
WM_NCHITTEST                         equ 84h
WM_NCPAINT                           equ 85h
WM_NCACTIVATE                        equ 86h
WM_GETDLGCODE                        equ 87h
WM_SYNCPAINT                    	  equ 88h
WM_NCMOUSEMOVE                       equ 0A0h
WM_NCLBUTTONDOWN                     equ 0A1h
WM_NCLBUTTONUP                       equ 0A2h
WM_NCLBUTTONDBLCLK                   equ 0A3h
WM_NCRBUTTONDOWN                     equ 0A4h
WM_NCRBUTTONUP                       equ 0A5h
WM_NCRBUTTONDBLCLK                   equ 0A6h
WM_NCMBUTTONDOWN                     equ 0A7h
WM_NCMBUTTONUP                       equ 0A8h
WM_NCMBUTTONDBLCLK                   equ 0A9h
WM_KEYFIRST                          equ 100h
WM_KEYDOWN                           equ 100h
WM_KEYUP                             equ 101h
WM_CHAR                              equ 102h
WM_DEADCHAR                          equ 103h
WM_SYSKEYDOWN                        equ 104h
WM_SYSKEYUP                          equ 105h
WM_SYSCHAR                           equ 106h
WM_SYSDEADCHAR                       equ 107h
WM_KEYLAST                           equ 108h
WM_INITDIALOG                        equ 110h
WM_COMMAND                           equ 111h
WM_SYSCOMMAND                        equ 112h
WM_TIMER                             equ 113h
WM_HSCROLL                           equ 114h
WM_VSCROLL                           equ 115h
WM_INITMENU                          equ 116h
WM_INITMENUPOPUP                     equ 117h
WM_MENUSELECT                        equ 11Fh
WM_MENUCHAR                          equ 120h
WM_ENTERIDLE                         equ 121h
WM_CTLCOLORMSGBOX                    equ 132h
WM_CTLCOLOREDIT                      equ 133h
WM_CTLCOLORLISTBOX                   equ 134h
WM_CTLCOLORBTN                       equ 135h
WM_CTLCOLORDLG                       equ 136h
WM_CTLCOLORSCROLLBAR                 equ 137h
WM_CTLCOLORSTATIC                    equ 138h
WM_MOUSEFIRST                        equ 200h
WM_MOUSEMOVE                         equ 200h
WM_LBUTTONDOWN                       equ 201h
WM_LBUTTONUP                         equ 202h
WM_LBUTTONDBLCLK                     equ 203h
WM_RBUTTONDOWN                       equ 204h
WM_RBUTTONUP                         equ 205h
WM_RBUTTONDBLCLK                     equ 206h
WM_MBUTTONDOWN                       equ 207h
WM_MBUTTONUP                         equ 208h
WM_MBUTTONDBLCLK                     equ 209h
WM_MOUSELAST                         equ 209h
WM_PARENTNOTIFY                      equ 210h
WM_ENTERMENULOOP                     equ 211h
WM_EXITMENULOOP                      equ 212h
WM_MDICREATE                         equ 220h
WM_MDIDESTROY                        equ 221h
WM_MDIACTIVATE                       equ 222h
WM_MDIRESTORE                        equ 223h
WM_MDINEXT                           equ 224h
WM_MDIMAXIMIZE                       equ 225h
WM_MDITILE                           equ 226h
WM_MDICASCADE                        equ 227h
WM_MDIICONARRANGE                    equ 228h
WM_MDIGETACTIVE                      equ 229h
WM_MDISETMENU                        equ 230h
WM_DROPFILES                         equ 233h
WM_MDIREFRESHMENU                    equ 234h
WM_CUT                               equ 300h
WM_COPY                              equ 301h
WM_PASTE                             equ 302h
WM_CLEAR                             equ 303h
WM_UNDO                              equ 304h
WM_RENDERFORMAT                      equ 305h
WM_RENDERALLFORMATS                  equ 306h
WM_DESTROYCLIPBOARD                  equ 307h
WM_DRAWCLIPBOARD                     equ 308h
WM_PAINTCLIPBOARD                    equ 309h
WM_VSCROLLCLIPBOARD                  equ 30Ah
WM_SIZECLIPBOARD                     equ 30Bh
WM_ASKCBFORMATNAME                   equ 30Ch
WM_CHANGECBCHAIN                     equ 30Dh
WM_HSCROLLCLIPBOARD                  equ 30Eh
WM_QUERYNEWPALETTE                   equ 30Fh
WM_PALETTEISCHANGING                 equ 310h
WM_PALETTECHANGED                    equ 311h
WM_HOTKEY                            equ 312h
WM_PRINT                        	  equ 317h
WM_PRINTCLIENT                       equ 318h
WM_PENWINFIRST                       equ 380h
WM_PENWINLAST                        equ 38Fh
ST_BEGINSWP                          equ 0
ST_ENDSWP                            equ 1
HTERROR                              equ -2
HTTRANSPARENT                        equ -1
HTNOWHERE                            equ 0
HTCLIENT                             equ 1
HTCAPTION                            equ 2
HTSYSMENU                            equ 3
HTGROWBOX                            equ 4
HTSIZE                               equ HTGROWBOX
HTMENU                               equ 5
HTHSCROLL                            equ 6
HTVSCROLL                            equ 7
HTMINBUTTON                          equ 8
HTMAXBUTTON                          equ 9
HTLEFT                               equ 10
HTRIGHT                              equ 11
HTTOP                                equ 12
HTTOPLEFT                            equ 13
HTTOPRIGHT                           equ 14
HTBOTTOM                             equ 15
HTBOTTOMLEFT                         equ 16
HTBOTTOMRIGHT                        equ 17
HTBORDER                             equ 18
HTREDUCE                             equ HTMINBUTTON
HTZOOM                               equ HTMAXBUTTON
HTSIZEFIRST                          equ HTLEFT
HTSIZELAST                           equ HTBOTTOMRIGHT
HTOBJECT            			  equ 19
HTCLOSE             			  equ 20
HTHELP			                 equ 21
SMTO_NORMAL                          equ 0h
SMTO_BLOCK                           equ 1h
SMTO_ABORTIFHUNG                     equ 2h
SMTO_NOTIMEOUTIFNOTHUNG 			  equ 8
MA_ACTIVATE                          equ 1
MA_ACTIVATEANDEAT                    equ 2
MA_NOACTIVATE                        equ 3
MA_NOACTIVATEANDEAT                  equ 4
SIZE_RESTORED                        equ 0
SIZE_MINIMIZED                       equ 1
SIZE_MAXIMIZED                       equ 2
SIZE_MAXSHOW                         equ 3
SIZE_MAXHIDE                         equ 4
SIZENORMAL                           equ SIZE_RESTORED
SIZEICONIC                           equ SIZE_MINIMIZED
SIZEFULLSCREEN                       equ SIZE_MAXIMIZED
SIZEZOOMSHOW                         equ SIZE_MAXSHOW
SIZEZOOMHIDE                         equ SIZE_MAXHIDE
WVR_ALIGNTOP                         equ 10h
WVR_ALIGNLEFT                        equ 20h
WVR_ALIGNBOTTOM                      equ 40h
WVR_ALIGNRIGHT                       equ 80h
WVR_HREDRAW                          equ 100h
WVR_VREDRAW                          equ 200h
WVR_REDRAW                           equ WVR_HREDRAW OR WVR_VREDRAW
WVR_VALIDRECTS                       equ 400h
MK_LBUTTON                           equ 1h
MK_RBUTTON                           equ 2h
MK_SHIFT                             equ 4h
MK_CONTROL                           equ 8h
MK_MBUTTON                           equ 10h
WS_OVERLAPPED                        equ 0h
WS_POPUP                             equ 80000000h
WS_CHILD                             equ 40000000h
WS_MINIMIZE                          equ 20000000h
WS_VISIBLE                           equ 10000000h
WS_DISABLED                          equ 8000000h
WS_CLIPSIBLINGS                      equ 4000000h
WS_CLIPCHILDREN                      equ 2000000h
WS_MAXIMIZE                          equ 1000000h
WS_CAPTION                           equ 0C00000h
WS_BORDER                            equ 800000h
WS_DLGFRAME                          equ 400000h
WS_VSCROLL                           equ 200000h
WS_HSCROLL                           equ 100000h
WS_SYSMENU                           equ 80000h
WS_THICKFRAME                        equ 40000h
WS_GROUP                             equ 20000h
WS_TABSTOP                           equ 10000h
WS_MINIMIZEBOX                       equ 20000h
WS_MAXIMIZEBOX                       equ 10000h
WS_TILED                             equ WS_OVERLAPPED
WS_ICONIC                            equ WS_MINIMIZE
WS_SIZEBOX                           equ WS_THICKFRAME
WS_OVERLAPPEDWINDOW                  equ WS_OVERLAPPED OR WS_CAPTION OR WS_SYSMENU OR WS_THICKFRAME OR WS_MINIMIZEBOX OR WS_MAXIMIZEBOX
WS_TILEDWINDOW                       equ WS_OVERLAPPEDWINDOW
WS_POPUPWINDOW                       equ WS_POPUP OR WS_BORDER OR WS_SYSMENU
WS_CHILDWINDOW                       equ WS_CHILD
WS_EX_DLGMODALFRAME                  equ 1h
WS_EX_NOPARENTNOTIFY                 equ 4h
WS_EX_TOPMOST                        equ 8h
WS_EX_ACCEPTFILES                    equ 10h
WS_EX_TRANSPARENT                    equ 20h
WS_EX_MDICHILD                       equ 00000040h
WS_EX_TOOLWINDOW                     equ 00000080h
WS_EX_WINDOWEDGE                     equ 00000100h
WS_EX_CLIENTEDGE                     equ 00000200h
WS_EX_CONTEXTHELP                    equ 00000400h
WS_EX_RIGHT                          equ 00001000h
WS_EX_LEFT                           equ 00000000h
WS_EX_RTLREADING                     equ 00002000h
WS_EX_LTRREADING                     equ 00000000h
WS_EX_LEFTSCROLLBAR                  equ 00004000h
WS_EX_RIGHTSCROLLBAR                 equ 00000000h
WS_EX_CONTROLPARENT                  equ 00010000h
WS_EX_STATICEDGE                     equ 00020000h
WS_EX_APPWINDOW                      equ 00040000h
WS_EX_OVERLAPPEDWINDOW               equ WS_EX_WINDOWEDGE OR WS_EX_CLIENTEDGE
WS_EX_PALETTEWINDOW                  equ WS_EX_WINDOWEDGE OR WS_EX_TOOLWINDOW OR WS_EX_TOPMOST
CS_VREDRAW                           equ 1h
CS_HREDRAW                           equ 2h
CS_KEYCVTWINDOW                      equ 4h
CS_DBLCLKS                           equ 8h
CS_OWNDC                             equ 20h
CS_CLASSDC                           equ 40h
CS_PARENTDC                          equ 80h
CS_NOKEYCVT                          equ 100h
CS_NOCLOSE                           equ 200h
CS_SAVEBITS                          equ 800h
CS_BYTEALIGNCLIENT                   equ 1000h
CS_BYTEALIGNWINDOW                   equ 2000h
CS_PUBLICCLASS                       equ 4000h
CS_GLOBALCLASS                       equ CS_PUBLICCLASS
CS_IME              			  equ 10000h
CF_TEXT                              equ  1
CF_BITMAP                            equ  2
CF_METAFILEPICT                      equ  3
CF_SYLK                              equ  4
CF_DIF                               equ  5
CF_TIFF                              equ  6
CF_OEMTEXT                           equ  7
CF_DIB                               equ  8
CF_PALETTE                           equ  9
CF_PENDATA                           equ 10
CF_RIFF                              equ 11
CF_WAVE                              equ 12
CF_UNICODETEXT      			  equ 13
CF_ENHMETAFILE      			  equ 14
CF_HDROP            			  equ 15
CF_LOCALE       				  equ 16
CF_MAX              			  equ 17
CF_OWNERDISPLAY                      equ 80h
CF_DSPTEXT                           equ 81h
CF_DSPBITMAP                         equ 82h
CF_DSPMETAFILEPICT                   equ 83h
CF_DSPENHMETAFILE                    equ 8Eh
CF_PRIVATEFIRST                      equ 200h
CF_PRIVATELAST                       equ 2FFh
CF_GDIOBJFIRST                       equ 300h
CF_GDIOBJLAST                        equ 3FFh
FVIRTKEY                             equ 1h
FNOINVERT                            equ 2h
FSHIFT                               equ 4h
FCONTROL                             equ 8h
FALT                                 equ 10h
WPF_SETMINPOSITION                   equ 1h
WPF_RESTORETOMAXIMIZED               equ 2h
ODT_MENU                             equ 1
ODT_LISTBOX                          equ 2
ODT_COMBOBOX                         equ 3
ODT_BUTTON                           equ 4
ODT_STATIC					  equ 5
ODA_DRAWENTIRE                       equ 1h
ODA_SELECT                           equ 2h
ODA_FOCUS                            equ 4h
ODS_SELECTED                         equ 1h
ODS_GRAYED                           equ 2h
ODS_DISABLED                         equ 4h
ODS_CHECKED                          equ 8h
ODS_FOCUS                            equ 10h
ODS_DEFAULT         			  equ 20h
ODS_COMBOBOXEDIT    			  equ 1000h
ODS_HOTLIGHT        			  equ 40h
ODS_INACTIVE        			  equ 80h
PM_NOREMOVE                          equ 0h
PM_REMOVE                            equ 1h
PM_NOYIELD                           equ 2h
MOD_ALT                              equ 1h
MOD_CONTROL                          equ 2h
MOD_SHIFT                            equ 4h
MOD_WIN						  equ 8h
IDHOT_SNAPWINDOW                     equ -1
IDHOT_SNAPDESKTOP                    equ -2
EWX_LOGOFF                           equ 0
EWX_SHUTDOWN                         equ 1
EWX_REBOOT                           equ 2
EWX_FORCE                            equ 4
EWX_POWEROFF                         equ 8
EWX_FORCEIFHUNG      			  equ 10h
EW_RESTARTWINDOWS                    equ 42h
READAPI                              equ 0


WRITEAPI                             equ 1
READ_WRITE                           equ 2
HWND_BROADCAST                       equ 0FFFFh
CW_USEDEFAULT                        equ 80000000h
HWND_MESSAGE					  equ -3
HWND_DESKTOP                         equ 0
SWP_NOSIZE                           equ 1h
SWP_NOMOVE                           equ 2h
SWP_NOZORDER                         equ 4h
SWP_NOREDRAW                         equ 8h
SWP_NOACTIVATE                       equ 10h
SWP_FRAMECHANGED                     equ 20h
SWP_SHOWWINDOW                       equ 40h
SWP_HIDEWINDOW                       equ 80h
SWP_NOCOPYBITS                       equ 100h
SWP_NOOWNERZORDER                    equ 200h
SWP_DRAWFRAME                        equ SWP_FRAMECHANGED
SWP_NOREPOSITION                     equ SWP_NOOWNERZORDER
SWP_DEFERERASE      			  equ 2000h
SWP_ASYNCWINDOWPOS  			  equ 4000h
HWND_TOP                             equ 0
HWND_BOTTOM                          equ 1
HWND_TOPMOST                         equ -1
HWND_NOTOPMOST                       equ -2
DLGWINDOWEXTRA                       equ 30
KEYEVENTF_EXTENDEDKEY                equ 1h
KEYEVENTF_KEYUP                      equ 2h
MOUSEEVENTF_MOVE                     equ 1h
MOUSEEVENTF_LEFTDOWN                 equ 2h
MOUSEEVENTF_LEFTUP                   equ 4h
MOUSEEVENTF_RIGHTDOWN                equ 8h
MOUSEEVENTF_RIGHTUP                  equ 10h
MOUSEEVENTF_MIDDLEDOWN               equ 20h
MOUSEEVENTF_MIDDLEUP                 equ 40h
MOUSEEVENTF_WHEEL       			  equ 800h
MOUSEEVENTF_ABSOLUTE                 equ 8000h
QS_KEY                               equ 1h
QS_MOUSEMOVE                         equ 2h
QS_MOUSEBUTTON                       equ 4h
QS_POSTMESSAGE                       equ 8h
QS_TIMER                             equ 10h
QS_PAINT                             equ 20h
QS_SENDMESSAGE                       equ 40h
QS_HOTKEY                            equ 80h
QS_ALLPOSTMESSAGE   			  equ 100h
QS_MOUSE                             equ QS_MOUSEMOVE OR QS_MOUSEBUTTON
QS_INPUT                             equ QS_MOUSE OR QS_KEY
QS_ALLEVENTS                         equ QS_INPUT OR QS_POSTMESSAGE OR QS_TIMER OR QS_PAINT OR QS_HOTKEY
QS_ALLINPUT                          equ QS_SENDMESSAGE OR QS_PAINT OR QS_TIMER OR QS_POSTMESSAGE OR QS_MOUSEBUTTON OR QS_MOUSEMOVE OR QS_HOTKEY OR QS_KEY
SM_CXSCREEN                          equ 0
SM_CYSCREEN                          equ 1
SM_CXVSCROLL                         equ 2
SM_CYHSCROLL                         equ 3
SM_CYCAPTION                         equ 4
SM_CXBORDER                          equ 5
SM_CYBORDER                          equ 6
SM_CXDLGFRAME                        equ 7
SM_CYDLGFRAME                        equ 8
SM_CYVTHUMB                          equ 9
SM_CXHTHUMB                          equ 10
SM_CXICON                            equ 11
SM_CYICON                            equ 12
SM_CXCURSOR                          equ 13
SM_CYCURSOR                          equ 14
SM_CYMENU                            equ 15
SM_CXFULLSCREEN                      equ 16
SM_CYFULLSCREEN                      equ 17
SM_CYKANJIWINDOW                     equ 18
SM_MOUSEPRESENT                      equ 19
SM_CYVSCROLL                         equ 20
SM_CXHSCROLL                         equ 21
SM_DEBUG                             equ 22
SM_SWAPBUTTON                        equ 23
SM_RESERVED1                         equ 24
SM_RESERVED2                         equ 25
SM_RESERVED3                         equ 26
SM_RESERVED4                         equ 27
SM_CXMIN                             equ 28
SM_CYMIN                             equ 29
SM_CXSIZE                            equ 30
SM_CYSIZE                            equ 31
SM_CXFRAME                           equ 32
SM_CYFRAME                           equ 33
SM_CXMINTRACK                        equ 34
SM_CYMINTRACK                        equ 35
SM_CXDOUBLECLK                       equ 36
SM_CYDOUBLECLK                       equ 37
SM_CXICONSPACING                     equ 38
SM_CYICONSPACING                     equ 39
SM_MENUDROPALIGNMENT                 equ 40
SM_PENWINDOWS                        equ 41
SM_DBCSENABLED                       equ 42
SM_CMOUSEBUTTONS                     equ 43
SM_CXFIXEDFRAME                      equ SM_CXDLGFRAME
SM_CYFIXEDFRAME                      equ SM_CYDLGFRAME
SM_CXSIZEFRAME                       equ SM_CXFRAME
SM_CYSIZEFRAME                       equ SM_CYFRAME
SM_SECURE                            equ 44
SM_CXEDGE                            equ 45
SM_CYEDGE                            equ 46
SM_CXMINSPACING                      equ 47
SM_CYMINSPACING                      equ 48
SM_CXSMICON                          equ 49
SM_CYSMICON                          equ 50
SM_CYSMCAPTION                       equ 51
SM_CXSMSIZE                          equ 52
SM_CYSMSIZE                          equ 53
SM_CXMENUSIZE                        equ 54
SM_CYMENUSIZE                        equ 55
SM_ARRANGE                           equ 56
SM_CXMINIMIZED                       equ 57
SM_CYMINIMIZED                       equ 58
SM_CXMAXTRACK                        equ 59
SM_CYMAXTRACK                        equ 60
SM_CXMAXIMIZED                       equ 61
SM_CYMAXIMIZED                       equ 62
SM_NETWORK                           equ 63
SM_CLEANBOOT                         equ 67
SM_CXDRAG                            equ 68
SM_CYDRAG                            equ 69
SM_SHOWSOUNDS                        equ 70
SM_CXMENUCHECK                       equ 71
SM_CYMENUCHECK                       equ 72
SM_SLOWMACHINE                       equ 73
SM_MIDEASTENABLED                    equ 74
SM_MOUSEWHEELPRESENT    			  equ 75
SM_CMETRICS                          equ 75
SM_XVIRTUALSCREEN       			  equ 76
SM_YVIRTUALSCREEN       			  equ 77
SM_CXVIRTUALSCREEN      			  equ 78
SM_CYVIRTUALSCREEN      			  equ 79
SM_CMONITORS            			  equ 80
SM_SAMEDISPLAYFORMAT    			  equ 81
TPM_LEFTBUTTON                       equ 0h
TPM_RIGHTBUTTON                      equ 2h
TPM_LEFTALIGN                        equ 0h
TPM_CENTERALIGN                      equ 4h
TPM_RIGHTALIGN                       equ 8h
TPM_TOPALIGN        			  equ 0
TPM_VCENTERALIGN    			  equ 10h
TPM_BOTTOMALIGN     			  equ 20h
TPM_HORIZONTAL      			  equ 0
TPM_VERTICAL        			  equ 40h
TPM_NONOTIFY        			  equ 80h
TPM_RETURNCMD       			  equ 100h
TPM_RECURSE         			  equ 1
DT_TOP                               equ 0h
DT_LEFT                              equ 0h
DT_CENTER                            equ 1h
DT_RIGHT                             equ 2h
DT_VCENTER                           equ 4h
DT_BOTTOM                            equ 8h
DT_WORDBREAK                         equ 10h
DT_SINGLELINE                        equ 20h
DT_EXPANDTABS                        equ 40h
DT_TABSTOP                           equ 80h
DT_NOCLIP                            equ 100h
DT_EXTERNALLEADING                   equ 200h
DT_CALCRECT                          equ 400h
DT_NOPREFIX                          equ 800h
DT_INTERNAL                          equ 1000h
DT_EDITCONTROL      			  equ 2000h
DT_PATH_ELLIPSIS    			  equ 4000h
DT_END_ELLIPSIS     			  equ 8000h
DT_MODIFYSTRING     			  equ 10000h
DT_RTLREADING       			  equ 20000h
DT_WORD_ELLIPSIS    			  equ 40000h
DCX_WINDOW                           equ 1h
DCX_CACHE                            equ 2h
DCX_NORESETATTRS                     equ 4h
DCX_CLIPCHILDREN                     equ 8h
DCX_CLIPSIBLINGS                     equ 10h
DCX_PARENTCLIP                       equ 20h
DCX_EXCLUDERGN                       equ 40h
DCX_INTERSECTRGN                     equ 80h
DCX_EXCLUDEUPDATE                    equ 100h
DCX_INTERSECTUPDATE                  equ 200h
DCX_LOCKWINDOWUPDATE                 equ 400h
DCX_NORECOMPUTE                      equ 100000h
DCX_VALIDATE                         equ 200000h
RDW_INVALIDATE                       equ 1h
RDW_INTERNALPAINT                    equ 2h
RDW_ERASE                            equ 4h
RDW_VALIDATE                         equ 8h
RDW_NOINTERNALPAINT                  equ 10h
RDW_NOERASE                          equ 20h
RDW_NOCHILDREN                       equ 40h
RDW_ALLCHILDREN                      equ 80h
RDW_UPDATENOW                        equ 100h
RDW_ERASENOW                         equ 200h
RDW_FRAME                            equ 400h
RDW_NOFRAME                          equ 800h
SW_SCROLLCHILDREN                    equ 1h
SW_INVALIDATE                        equ 2h
SW_ERASE                             equ 4h
SW_SMOOTHSCROLL     			  equ 10h
ESB_ENABLE_BOTH                      equ 0h
ESB_DISABLE_BOTH                     equ 3h
ESB_DISABLE_LEFT                     equ 1h
ESB_DISABLE_RIGHT                    equ 2h
ESB_DISABLE_UP                       equ 1h
ESB_DISABLE_DOWN                     equ 2h
ESB_DISABLE_LTUP                     equ ESB_DISABLE_LEFT
ESB_DISABLE_RTDN                     equ ESB_DISABLE_RIGHT
MB_OK                                equ 0h
MB_OKCANCEL                          equ 1h
MB_ABORTRETRYIGNORE                  equ 2h
MB_YESNOCANCEL                       equ 3h
MB_YESNO                             equ 4h
MB_RETRYCANCEL                       equ 5h
MB_ICONHAND                          equ 10h
MB_ICONQUESTION                      equ 20h
MB_ICONEXCLAMATION                   equ 30h
MB_ICONASTERISK                      equ 40h
MB_USERICON					  equ 80h
MB_ICONERROR                         equ MB_ICONHAND
MB_ICONINFORMATION                   equ MB_ICONASTERISK
MB_ICONSTOP                          equ MB_ICONHAND
MB_ICONWARNING                       equ MB_ICONEXCLAMATION
MB_DEFBUTTON1                        equ 0h
MB_DEFBUTTON2                        equ 100h
MB_DEFBUTTON3                        equ 200h
MB_DEFBUTTON4               		  equ 300h
MB_APPLMODAL                         equ 0h
MB_SYSTEMMODAL                       equ 1000h
MB_TASKMODAL                         equ 2000h
MB_HELP						  equ 4000h
MB_NOFOCUS                           equ 8000h
MB_SETFOREGROUND                     equ 10000h
MB_DEFAULT_DESKTOP_ONLY              equ 20000h
MB_TYPEMASK                          equ 0Fh
MB_ICONMASK                          equ 0F0h
MB_DEFMASK                           equ 0F00h
MB_MODEMASK                          equ 3000h
MB_MISCMASK                          equ 0C000h
MB_TOPMOST                  		  equ 40000h
MB_RIGHT                    		  equ 80000h
MB_RTLREADING               		  equ 100000h
MB_SERVICE_NOTIFICATION          	  equ 200000h
CTLCOLOR_MSGBOX                      equ 0
CTLCOLOR_EDIT                        equ 1
CTLCOLOR_LISTBOX                     equ 2
CTLCOLOR_BTN                         equ 3
CTLCOLOR_DLG                         equ 4
CTLCOLOR_SCROLLBAR                   equ 5
CTLCOLOR_STATIC                      equ 6
CTLCOLOR_MAX                         equ 8
COLOR_SCROLLBAR                      equ 0
COLOR_BACKGROUND                     equ 1
COLOR_ACTIVECAPTION                  equ 2
COLOR_INACTIVECAPTION                equ 3
COLOR_MENU                           equ 4
COLOR_WINDOW                         equ 5
COLOR_WINDOWFRAME                    equ 6
COLOR_MENUTEXT                       equ 7
COLOR_WINDOWTEXT                     equ 8
COLOR_CAPTIONTEXT                    equ 9
COLOR_ACTIVEBORDER                   equ 10
COLOR_INACTIVEBORDER                 equ 11
COLOR_APPWORKSPACE                   equ 12
COLOR_HIGHLIGHT                      equ 13
COLOR_HIGHLIGHTTEXT                  equ 14
COLOR_BTNFACE                        equ 15
COLOR_BTNSHADOW                      equ 16
COLOR_GRAYTEXT                       equ 17
COLOR_BTNTEXT                        equ 18
COLOR_INACTIVECAPTIONTEXT            equ 19
COLOR_BTNHIGHLIGHT                   equ 20
COLOR_3DDKSHADOW                     equ 21
COLOR_3DLIGHT                        equ 22
COLOR_INFOTEXT                       equ 23
COLOR_INFOBK                         equ 24
COLOR_DESKTOP                        equ COLOR_BACKGROUND
COLOR_3DFACE                         equ COLOR_BTNFACE
COLOR_3DSHADOW                       equ COLOR_BTNSHADOW
COLOR_3DHIGHLIGHT                    equ COLOR_BTNHIGHLIGHT
COLOR_3DHILIGHT                      equ COLOR_BTNHIGHLIGHT
COLOR_BTNHILIGHT                     equ COLOR_BTNHIGHLIGHT
COLOR_HOTLIGHT                  	  equ 26
COLOR_GRADIENTACTIVECAPTION     	  equ 27
COLOR_GRADIENTINACTIVECAPTION   	  equ 28
GW_HWNDFIRST                         equ 0
GW_HWNDLAST                          equ 1
GW_HWNDNEXT                          equ 2
GW_HWNDPREV                          equ 3
GW_OWNER                             equ 4
GW_CHILD                             equ 5
GW_MAX                               equ 5
GW_ENABLEDPOPUP     			  equ 6
MF_INSERT                            equ 0h
MF_CHANGE                            equ 80h
MF_APPEND                            equ 100h
MF_DELETE                            equ 200h
MF_REMOVE                            equ 1000h
MF_BYCOMMAND                         equ 0h
MF_BYPOSITION                        equ 400h
MF_SEPARATOR                         equ 800h
MF_ENABLED                           equ 0h
MF_GRAYED                            equ 1h
MF_DISABLED                          equ 2h
MF_UNCHECKED                         equ 0h
MF_CHECKED                           equ 8h
MF_USECHECKBITMAPS                   equ 200h
MF_STRING                            equ 0h
MF_BITMAP                            equ 4h
MF_OWNERDRAW                         equ 100h
MF_POPUP                             equ 10h
MF_MENUBARBREAK                      equ 20h
MF_MENUBREAK                         equ 40h
MF_UNHILITE                          equ 0h
MF_HILITE                            equ 80h
MF_SYSMENU                           equ 2000h
MF_HELP                              equ 4000h
MF_MOUSESELECT                       equ 8000h
MF_END                               equ 80h
MF_DEFAULT          			  equ 1000h
MF_RIGHTJUSTIFY     			  equ 4000h
SC_SIZE                              equ 0F000h
SC_MOVE                              equ 0F010h
SC_MINIMIZE                          equ 0F020h
SC_MAXIMIZE                          equ 0F030h
SC_NEXTWINDOW                        equ 0F040h
SC_PREVWINDOW                        equ 0F050h
SC_CLOSE                             equ 0F060h
SC_VSCROLL                           equ 0F070h
SC_HSCROLL                           equ 0F080h
SC_MOUSEMENU                         equ 0F090h
SC_KEYMENU                           equ 0F100h
SC_ARRANGE                           equ 0F110h
SC_RESTORE                           equ 0F120h
SC_TASKLIST                          equ 0F130h
SC_SCREENSAVE                        equ 0F140h
SC_HOTKEY                            equ 0F150h
SC_ICON                              equ SC_MINIMIZE
SC_ZOOM                              equ SC_MAXIMIZE
SC_DEFAULT      				  equ 0F160h
SC_MONITORPOWER 				  equ 0F170h
SC_CONTEXTHELP  				  equ 0F180h
SC_SEPARATOR    				  equ 0F00Fh
IDC_ARROW                            equ 32512
IDC_IBEAM                            equ 32513
IDC_WAIT                             equ 32514
IDC_CROSS                            equ 32515
IDC_UPARROW                          equ 32516
IDC_SIZE                             equ 32640
IDC_ICON                             equ 32641
IDC_SIZENWSE                         equ 32642
IDC_SIZENESW                         equ 32643
IDC_SIZEWE                           equ 32644
IDC_SIZENS                           equ 32645
IDC_SIZEALL                          equ 32646
IDC_NO                               equ 32648
IDC_HAND            			  equ 32649
IDC_APPSTARTING                      equ 32650
IDC_HELP            			  equ 32651
OBM_CLOSE                            equ 32754
OBM_UPARROW                          equ 32753
OBM_DNARROW                          equ 32752
OBM_RGARROW                          equ 32751
OBM_LFARROW                          equ 32750
OBM_REDUCE                           equ 32749
OBM_ZOOM                             equ 32748
OBM_RESTORE                          equ 32747
OBM_REDUCED                          equ 32746
OBM_ZOOMD                            equ 32745
OBM_RESTORED                         equ 32744
OBM_UPARROWD                         equ 32743
OBM_DNARROWD                         equ 32742
OBM_RGARROWD                         equ 32741
OBM_LFARROWD                         equ 32740
OBM_MNARROW                          equ 32739
OBM_COMBO                            equ 32738
OBM_UPARROWI                         equ 32737
OBM_DNARROWI                         equ 32736
OBM_RGARROWI                         equ 32735
OBM_LFARROWI                         equ 32734
OBM_OLD_CLOSE                        equ 32767
OBM_SIZE                             equ 32766
OBM_OLD_UPARROW                      equ 32765
OBM_OLD_DNARROW                      equ 32764
OBM_OLD_RGARROW                      equ 32763
OBM_OLD_LFARROW                      equ 32762
OBM_BTSIZE                           equ 32761
OBM_CHECK                            equ 32760
OBM_CHECKBOXES                       equ 32759
OBM_BTNCORNERS                       equ 32758
OBM_OLD_REDUCE                       equ 32757
OBM_OLD_ZOOM                         equ 32756
OBM_OLD_RESTORE                      equ 32755
OCR_NORMAL                           equ 32512
OCR_IBEAM                            equ 32513
OCR_WAIT                             equ 32514
OCR_CROSS                            equ 32515
OCR_UP                               equ 32516
OCR_SIZE                             equ 32640
OCR_ICON                             equ 32641
OCR_SIZENWSE                         equ 32642
OCR_SIZENESW                         equ 32643
OCR_SIZEWE                           equ 32644
OCR_SIZENS                           equ 32645
OCR_SIZEALL                          equ 32646
OCR_ICOCUR                           equ 32647
OCR_NO                               equ 32648
OCR_HAND            			  equ 32649
OCR_APPSTARTING     			  equ 32650
OIC_SAMPLE                           equ 32512
OIC_HAND                             equ 32513
OIC_QUES                             equ 32514
OIC_BANG                             equ 32515
OIC_NOTE                             equ 32516
OIC_WINLOGO         			  equ 32517
OIC_WARNING         			  equ OIC_BANG
OIC_ERROR           			  equ OIC_HAND
OIC_INFORMATION     			  equ OIC_NOTE
ORD_LANGDRIVER                       equ 1
IDI_APPLICATION                      equ 32512
IDI_HAND                             equ 32513
IDI_QUESTION                         equ 32514
IDI_EXCLAMATION                      equ 32515
IDI_ASTERISK                         equ 32516
IDI_WINLOGO         			  equ 32517
IDI_WARNING     				  equ IDI_EXCLAMATION
IDI_ERROR       				  equ IDI_HAND
IDI_INFORMATION 				  equ IDI_ASTERISK
IDOK                                 equ 1
IDCANCEL                             equ 2
IDABORT                              equ 3
IDRETRY                              equ 4
IDIGNORE                             equ 5
IDYES                                equ 6
IDNO                                 equ 7
IDCLOSE         				  equ 8
IDHELP          				  equ 9
ES_LEFT                              equ 0h
ES_CENTER                            equ 1h
ES_RIGHT                             equ 2h
ES_MULTILINE                         equ 4h
ES_UPPERCASE                         equ 8h
ES_LOWERCASE                         equ 10h
ES_PASSWORD                          equ 20h
ES_AUTOVSCROLL                       equ 40h
ES_AUTOHSCROLL                       equ 80h
ES_NOHIDESEL                         equ 100h
ES_OEMCONVERT                        equ 400h
ES_READONLY                          equ 800h
ES_WANTRETURN                        equ 1000h
ES_NUMBER           			 equ 2000h
EN_SETFOCUS                          equ 100h
EN_KILLFOCUS                         equ 200h
EN_CHANGE                            equ 300h
EN_UPDATE                            equ 400h
EN_ERRSPACE                          equ 500h
EN_MAXTEXT                           equ 501h
EN_HSCROLL                           equ 601h
EN_VSCROLL                           equ 602h


EM_GETSEL                            equ 0B0h
EM_SETSEL                            equ 0B1h
EM_GETRECT                           equ 0B2h
EM_SETRECT                           equ 0B3h
EM_SETRECTNP                         equ 0B4h
EM_SCROLL                            equ 0B5h
EM_LINESCROLL                        equ 0B6h
EM_SCROLLCARET                       equ 0B7h
EM_GETMODIFY                         equ 0B8h
EM_SETMODIFY                         equ 0B9h
EM_GETLINECOUNT                      equ 0BAh
EM_LINEINDEX                         equ 0BBh
EM_SETHANDLE                         equ 0BCh
EM_GETHANDLE                         equ 0BDh
EM_GETTHUMB                          equ 0BEh
EM_LINELENGTH                        equ 0C1h
EM_REPLACESEL                        equ 0C2h
EM_GETLINE                           equ 0C4h
EM_LIMITTEXT                         equ 0C5h
EM_CANUNDO                           equ 0C6h
EM_UNDO                              equ 0C7h
EM_FMTLINES                          equ 0C8h
EM_LINEFROMCHAR                      equ 0C9h
EM_SETTABSTOPS                       equ 0CBh
EM_SETPASSWORDCHAR                   equ 0CCh
EM_EMPTYUNDOBUFFER                   equ 0CDh
EM_GETFIRSTVISIBLELINE               equ 0CEh
EM_SETREADONLY                       equ 0CFh
EM_SETWORDBREAKPROC                  equ 0D0h
EM_GETWORDBREAKPROC                  equ 0D1h
EM_GETPASSWORDCHAR                   equ 0D2h
EM_SETMARGINS                        equ 0D3h
EM_GETMARGINS                        equ 0D4h
EM_SETLIMITTEXT                      equ EM_LIMITTEXT
EM_GETLIMITTEXT                      equ 0D5h
EM_POSFROMCHAR                       equ 0D6h
EM_CHARFROMPOS                       equ 0D7h
EM_SETIMESTATUS                      equ 0D8h
EM_GETIMESTATUS                      equ 0D9h





WB_LEFT                              equ 0
WB_RIGHT                             equ 1
WB_ISDELIMITER                       equ 2
BS_PUSHBUTTON                        equ 0h
BS_DEFPUSHBUTTON                     equ 1h
BS_CHECKBOX                          equ 2h
BS_AUTOCHECKBOX                      equ 3h
BS_RADIOBUTTON                       equ 4h
BS_3STATE                            equ 5h
BS_AUTO3STATE                        equ 6h
BS_GROUPBOX                          equ 7h
BS_USERBUTTON                        equ 8h
BS_AUTORADIOBUTTON                   equ 9h
BS_OWNERDRAW                         equ 0Bh
BS_LEFTTEXT                          equ 20h
BS_BITMAP                            equ 80h
BS_ICON                              equ 40h
BS_TEXT             			  equ 0
BS_ICON             			  equ 40h
BS_BITMAP           			  equ 80h
BS_LEFT             			  equ 100h
BS_RIGHT           			  equ 200h
BS_CENTER           			  equ 300h
BS_TOP              			  equ 400h
BS_BOTTOM           			  equ 800h
BS_VCENTER          			  equ 0C00h
BS_PUSHLIKE         			  equ 1000h
BS_MULTILINE        			  equ 2000h
BS_NOTIFY           			  equ 4000h
BS_FLAT             			  equ 8000h
BS_RIGHTBUTTON      			  equ BS_LEFTTEXT
BN_CLICKED                           equ 0
BN_PAINT                             equ 1
BN_HILITE                            equ 2
BN_UNHILITE                          equ 3
BN_DISABLE                           equ 4
BN_DOUBLECLICKED                     equ 5
BN_SETFOCUS                          equ 6
BN_KILLFOCUS                         equ 7
BN_PUSHED           			  equ BN_HILITE
BN_UNPUSHED         			  equ BN_UNHILITE
BN_DBLCLK           			  equ BN_DOUBLECLICKED
BST_UNCHECKED                        equ 00h
BST_CHECKED                          equ 1
BST_INDETERMINATE                    equ 2
BST_PUSHED                           equ 4
BST_FOCUS          			 equ 8
BM_GETCHECK                          equ 0F0h
BM_SETCHECK                          equ 0F1h
BM_GETSTATE                          equ 0F2h
BM_SETSTATE                          equ 0F3h
BM_SETSTYLE                          equ 0F4h
BM_CLICK                             equ 0F5h
BM_GETIMAGE                          equ 0F6h
BM_SETIMAGE                          equ 0F7h
SS_LEFT                              equ 0h
SS_CENTER                            equ 1h
SS_RIGHT                             equ 2h
SS_ICON                              equ 3h
SS_BLACKRECT                         equ 4h
SS_GRAYRECT                          equ 5h
SS_WHITERECT                         equ 6h
SS_BLACKFRAME                        equ 7h
SS_GRAYFRAME                         equ 8h
SS_WHITEFRAME                        equ 9h
SS_USERITEM                          equ 0Ah
SS_SIMPLE                            equ 0Bh
SS_LEFTNOWORDWRAP                    equ 0Ch
SS_NOPREFIX                          equ 80h
STM_SETICON                          equ 170h
STM_GETICON                          equ 171h
STM_MSGMAX                           equ 172h
WC_DIALOG                            equ 8002h
DWL_MSGRESULT                        equ 0
DWL_DLGPROC                          equ 4
DWL_USER                             equ 8
DDL_READWRITE                        equ 0h
DDL_READONLY                         equ 1h
DDL_HIDDEN                           equ 2h
DDL_SYSTEM                           equ 4h
DDL_DIRECTORY                        equ 10h
DDL_ARCHIVE                          equ 20h
DDL_POSTMSGS                         equ 2000h
DDL_DRIVES                           equ 4000h
DDL_EXCLUSIVE                        equ 8000h
DS_ABSALIGN                          equ 0001h
DS_SYSMODAL                          equ 0002h
DS_3DLOOK                            equ 0004h
DS_FIXEDSYS                          equ 0008h
DS_NOFAILCREATE                      equ 0010h
DS_LOCALEDIT                         equ 0020h
DS_SETFONT                           equ 0040h
DS_MODALFRAME                        equ 0080h
DS_NOIDLEMSG                         equ 0100h
DS_SETFOREGROUND                     equ 0200h
DS_CONTROL                           equ 0400h
DS_CENTER                            equ 0800h
DS_CENTERMOUSE                       equ 1000h
DS_CONTEXTHELP                       equ 2000h
DM_GETDEFID                          equ WM_USER + 0
DM_SETDEFID                          equ WM_USER + 1
DC_HASDEFID                          equ 534h
DLGC_WANTARROWS                      equ 1h
DLGC_WANTTAB                         equ 2h
DLGC_WANTALLKEYS                     equ 4h
DLGC_WANTMESSAGE                     equ 4h
DLGC_HASSETSEL                       equ 8h
DLGC_DEFPUSHBUTTON                   equ 10h
DLGC_UNDEFPUSHBUTTON                 equ 20h
DLGC_RADIOBUTTON                     equ 40h
DLGC_WANTCHARS                       equ 80h
DLGC_STATIC                          equ 100h
DLGC_BUTTON                          equ 2000h
LB_CTLCODE                           equ 0
LB_OKAY                              equ 0
LB_ERR                               equ -1
LB_ERRSPACE                          equ -2
LBN_ERRSPACE                         equ -2
LBN_SELCHANGE                        equ 1
LBN_DBLCLK                           equ 2
LBN_SELCANCEL                        equ 3
LBN_SETFOCUS                         equ 4
LBN_KILLFOCUS                        equ 5
LB_ADDSTRING                         equ 180h
LB_INSERTSTRING                      equ 181h
LB_DELETESTRING                      equ 182h
LB_SELITEMRANGEEX                    equ 183h
LB_RESETCONTENT                      equ 184h
LB_SETSEL                            equ 185h
LB_SETCURSEL                         equ 186h
LB_GETSEL                            equ 187h
LB_GETCURSEL                         equ 188h
LB_GETTEXT                           equ 189h
LB_GETTEXTLEN                        equ 18Ah
LB_GETCOUNT                          equ 18Bh
LB_SELECTSTRING                      equ 18Ch
LB_DIR                               equ 18Dh
LB_GETTOPINDEX                       equ 18Eh
LB_FINDSTRING                        equ 18Fh
LB_GETSELCOUNT                       equ 190h
LB_GETSELITEMS                       equ 191h
LB_SETTABSTOPS                       equ 192h
LB_GETHORIZONTALEXTENT               equ 193h
LB_SETHORIZONTALEXTENT               equ 194h
LB_SETCOLUMNWIDTH                    equ 195h
LB_ADDFILE                           equ 196h
LB_SETTOPINDEX                       equ 197h
LB_GETITEMRECT                       equ 198h
LB_GETITEMDATA                       equ 199h
LB_SETITEMDATA                       equ 19Ah
LB_SELITEMRANGE                      equ 19Bh
LB_SETANCHORINDEX                    equ 19Ch
LB_GETANCHORINDEX                    equ 19Dh
LB_SETCARETINDEX                     equ 19Eh
LB_GETCARETINDEX                     equ 19Fh
LB_SETITEMHEIGHT                     equ 1A0h
LB_GETITEMHEIGHT                     equ 1A1h
LB_FINDSTRINGEXACT                   equ 1A2h
LB_SETLOCALE                         equ 1A5h
LB_GETLOCALE                         equ 1A6h
LB_SETCOUNT                          equ 1A7h
LB_MSGMAX                            equ 1A8h
LBS_NOTIFY                           equ 1h
LBS_SORT                             equ 2h
LBS_NOREDRAW                         equ 4h
LBS_MULTIPLESEL                      equ 8h
LBS_OWNERDRAWFIXED                   equ 10h
LBS_OWNERDRAWVARIABLE                equ 20h
LBS_HASSTRINGS                       equ 40h
LBS_USETABSTOPS                      equ 80h
LBS_NOINTEGRALHEIGHT                 equ 100h
LBS_MULTICOLUMN                      equ 200h
LBS_WANTKEYBOARDINPUT                equ 400h
LBS_EXTENDEDSEL                      equ 800h
LBS_DISABLENOSCROLL                  equ 1000h
LBS_NODATA                           equ 2000h
LBS_STANDARD                         equ LBS_NOTIFY OR LBS_SORT OR WS_VSCROLL OR WS_BORDER
CB_OKAY                              equ 0
CB_ERR                               equ -1
CB_ERRSPACE                          equ -2
CBN_ERRSPACE                         equ -1
CBN_SELCHANGE                        equ 1
CBN_DBLCLK                           equ 2
CBN_SETFOCUS                         equ 3
CBN_KILLFOCUS                        equ 4
CBN_EDITCHANGE                       equ 5
CBN_EDITUPDATE                       equ 6
CBN_DROPDOWN                         equ 7
CBN_CLOSEUP                          equ 8
CBN_SELENDOK                         equ 9
CBN_SELENDCANCEL                     equ 10
CBS_SIMPLE                           equ 1h
CBS_DROPDOWN                         equ 2h
CBS_DROPDOWNLIST                     equ 3h
CBS_OWNERDRAWFIXED                   equ 10h
CBS_OWNERDRAWVARIABLE                equ 20h
CBS_AUTOHSCROLL                      equ 40h
CBS_OEMCONVERT                       equ 80h
CBS_SORT                             equ 100h
CBS_HASSTRINGS                       equ 200h
CBS_NOINTEGRALHEIGHT                 equ 400h
CBS_DISABLENOSCROLL                  equ 800h
CB_GETEDITSEL                        equ 140h
CB_LIMITTEXT                         equ 141h
CB_SETEDITSEL                        equ 142h
CB_ADDSTRING                         equ 143h
CB_DELETESTRING                      equ 144h
CB_DIR                               equ 145h
CB_GETCOUNT                          equ 146h
CB_GETCURSEL                         equ 147h
CB_GETLBTEXT                         equ 148h
CB_GETLBTEXTLEN                      equ 149h
CB_INSERTSTRING                      equ 14Ah
CB_RESETCONTENT                      equ 14Bh
CB_FINDSTRING                        equ 14Ch
CB_SELECTSTRING                      equ 14Dh
CB_SETCURSEL                         equ 14Eh
CB_SHOWDROPDOWN                      equ 14Fh
CB_GETITEMDATA                       equ 150h
CB_SETITEMDATA                       equ 151h
CB_GETDROPPEDCONTROLRECT             equ 152h
CB_SETITEMHEIGHT                     equ 153h
CB_GETITEMHEIGHT                     equ 154h
CB_SETEXTENDEDUI                     equ 155h
CB_GETEXTENDEDUI                     equ 156h
CB_GETDROPPEDSTATE                   equ 157h
CB_FINDSTRINGEXACT                   equ 158h
CB_SETLOCALE                         equ 159h
CB_GETLOCALE                         equ 15Ah
CB_GETTOPINDEX                       equ 15Bh
CB_SETTOPINDEX                       equ 15Ch
CB_GETHORIZONTALEXTENT               equ 15Dh
CB_SETHORIZONTALEXTENT               equ 15Eh
CB_GETDROPPEDWIDTH                   equ 15Fh
CB_SETDROPPEDWIDTH                   equ 160h
CB_INITSTORAGE                       equ 161h
CB_MSGMAX                            equ 162h
SBS_HORZ                             equ 0h
SBS_VERT                             equ 1h
SBS_TOPALIGN                         equ 2h
SBS_LEFTALIGN                        equ 2h
SBS_BOTTOMALIGN                      equ 4h
SBS_RIGHTALIGN                       equ 4h
SBS_SIZEBOXTOPLEFTALIGN              equ 2h
SBS_SIZEBOXBOTTOMRIGHTALIGN          equ 4h
SBS_SIZEBOX                          equ 8h
SBS_SIZEGRIP                         equ 10h
SBM_SETPOS                           equ 0E0h
SBM_GETPOS                           equ 0E1h
SBM_SETRANGE                         equ 0E2h
SBM_SETRANGEREDRAW                   equ 0E6h
SBM_GETRANGE                         equ 0E3h
SBM_ENABLE_ARROWS                    equ 0E4h
MDIS_ALLCHILDSTYLES                  equ 1h
MDITILE_VERTICAL                     equ 0h
MDITILE_HORIZONTAL                   equ 1h
MDITILE_SKIPDISABLED                 equ 2h
HELP_CONTEXT                         equ 1h
HELP_QUIT                            equ 2h
HELP_INDEX                           equ 3h
HELP_CONTENTS                        equ 3h
HELP_HELPONHELP                      equ 4h
HELP_SETINDEX                        equ 5h
HELP_SETCONTENTS                     equ 5h
HELP_CONTEXTPOPUP                    equ 8h
HELP_FORCEFILE                       equ 9h
HELP_KEY                             equ 101h
HELP_COMMAND                         equ 102h
HELP_PARTIALKEY                      equ 105h
HELP_MULTIKEY                        equ 201h
HELP_SETWINPOS                       equ 203h
HELP_CONTEXTMENU                     equ 000Ah
HELP_FINDER                          equ 000Bh
HELP_WM_HELP                         equ 000Ch
HELP_SETPOPUP_POS                    equ 000Dh
HELP_TCARD                           equ 8000h
HELP_TCARD_DATA                      equ 0010h
HELP_TCARD_OTHER_CALLER              equ 0011h
IDH_NO_HELP                          equ 28440
IDH_MISSING_CONTEXT                  equ 28441
IDH_GENERIC_HELP_BUTTON              equ 28442
IDH_OK                               equ 28443
IDH_CANCEL                           equ 28444
IDH_HELP                             equ 28445
SPI_GETBEEP                          equ 1
SPI_SETBEEP                          equ 2
SPI_GETMOUSE                         equ 3
SPI_SETMOUSE                         equ 4
SPI_GETBORDER                        equ 5
SPI_SETBORDER                        equ 6
SPI_GETKEYBOARDSPEED                 equ 10
SPI_SETKEYBOARDSPEED                 equ 11
SPI_LANGDRIVER                       equ 12
SPI_ICONHORIZONTALSPACING            equ 13
SPI_GETSCREENSAVETIMEOUT             equ 14
SPI_SETSCREENSAVETIMEOUT             equ 15
SPI_GETSCREENSAVEACTIVE              equ 16
SPI_SETSCREENSAVEACTIVE              equ 17
SPI_GETGRIDGRANULARITY               equ 18
SPI_SETGRIDGRANULARITY               equ 19
SPI_SETDESKWALLPAPER                 equ 20
SPI_SETDESKPATTERN                   equ 21
SPI_GETKEYBOARDDELAY                 equ 22
SPI_SETKEYBOARDDELAY                 equ 23
SPI_ICONVERTICALSPACING              equ 24
SPI_GETICONTITLEWRAP                 equ 25
SPI_SETICONTITLEWRAP                 equ 26
SPI_GETMENUDROPALIGNMENT             equ 27
SPI_SETMENUDROPALIGNMENT             equ 28
SPI_SETDOUBLECLKWIDTH                equ 29
SPI_SETDOUBLECLKHEIGHT               equ 30
SPI_GETICONTITLELOGFONT              equ 31
SPI_SETDOUBLECLICKTIME               equ 32
SPI_SETMOUSEBUTTONSWAP               equ 33
SPI_SETICONTITLELOGFONT              equ 34
SPI_GETFASTTASKSWITCH                equ 35
SPI_SETFASTTASKSWITCH                equ 36
SPI_SETDRAGFULLWINDOWS               equ 37
SPI_GETDRAGFULLWINDOWS               equ 38
SPI_GETNONCLIENTMETRICS              equ 41
SPI_SETNONCLIENTMETRICS              equ 42
SPI_GETMINIMIZEDMETRICS              equ 43
SPI_SETMINIMIZEDMETRICS              equ 44
SPI_GETICONMETRICS                   equ 45
SPI_SETICONMETRICS                   equ 46
SPI_SETWORKAREA                      equ 47
SPI_GETWORKAREA                      equ 48
SPI_SETPENWINDOWS                    equ 49
SPI_GETFILTERKEYS                    equ 50
SPI_SETFILTERKEYS                    equ 51
SPI_GETTOGGLEKEYS                    equ 52
SPI_SETTOGGLEKEYS                    equ 53
SPI_GETMOUSEKEYS                     equ 54
SPI_SETMOUSEKEYS                     equ 55
SPI_GETSHOWSOUNDS                    equ 56
SPI_SETSHOWSOUNDS                    equ 57
SPI_GETSTICKYKEYS                    equ 58
SPI_SETSTICKYKEYS                    equ 59
SPI_GETACCESSTIMEOUT                 equ 60
SPI_SETACCESSTIMEOUT                 equ 61
SPI_GETSERIALKEYS                    equ 62
SPI_SETSERIALKEYS                    equ 63
SPI_GETSOUNDSENTRY                   equ 64
SPI_SETSOUNDSENTRY                   equ 65
SPI_GETHIGHCONTRAST                  equ 66
SPI_SETHIGHCONTRAST                  equ 67
SPI_GETKEYBOARDPREF                  equ 68
SPI_SETKEYBOARDPREF                  equ 69
SPI_GETSCREENREADER                  equ 70
SPI_SETSCREENREADER                  equ 71
SPI_GETANIMATION                     equ 72
SPI_SETANIMATION                     equ 73
SPI_GETFONTSMOOTHING                 equ 74
SPI_SETFONTSMOOTHING                 equ 75
SPI_SETDRAGWIDTH                     equ 76
SPI_SETDRAGHEIGHT                    equ 77
SPI_SETHANDHELD                      equ 78
SPI_GETLOWPOWERTIMEOUT               equ 79
SPI_GETPOWEROFFTIMEOUT               equ 80
SPI_SETLOWPOWERTIMEOUT               equ 81
SPI_SETPOWEROFFTIMEOUT               equ 82
SPI_GETLOWPOWERACTIVE                equ 83
SPI_GETPOWEROFFACTIVE                equ 84
SPI_SETLOWPOWERACTIVE                equ 85
SPI_SETPOWEROFFACTIVE                equ 86
SPI_SETCURSORS                       equ 87
SPI_SETICONS                         equ 88
SPI_GETDEFAULTINPUTLANG              equ 89
SPI_SETDEFAULTINPUTLANG              equ 90
SPI_SETLANGTOGGLE                    equ 91
SPI_GETWINDOWSEXTENSION              equ 92
SPI_SETMOUSETRAILS                   equ 93
SPI_GETMOUSETRAILS                   equ 94
SPI_SCREENSAVERRUNNING               equ 97
SPIF_UPDATEINIFILE                   equ 1h
SPIF_SENDWININICHANGE                equ 2h
WM_DDE_FIRST                         equ 3E0h
WM_DDE_INITIATE                      equ WM_DDE_FIRST
WM_DDE_TERMINATE                     equ WM_DDE_FIRST + 1
WM_DDE_ADVISE                        equ WM_DDE_FIRST + 2
WM_DDE_UNADVISE                      equ WM_DDE_FIRST + 3
WM_DDE_ACK                           equ WM_DDE_FIRST + 4
WM_DDE_DATA                          equ WM_DDE_FIRST + 5
WM_DDE_REQUEST                       equ WM_DDE_FIRST + 6
WM_DDE_POKE                          equ WM_DDE_FIRST + 7
WM_DDE_EXECUTE                       equ WM_DDE_FIRST + 8
WM_DDE_LAST                          equ WM_DDE_FIRST + 8
XST_NULL                             equ 0
XST_INCOMPLETE                       equ 1
XST_CONNECTED                        equ 2
XST_INIT1                            equ 3
XST_INIT2                            equ 4
XST_REQSENT                          equ 5
XST_DATARCVD                         equ 6
XST_POKESENT                         equ 7
XST_POKEACKRCVD                      equ 8
XST_EXECSENT                         equ 9
XST_EXECACKRCVD                      equ 10
XST_ADVSENT                          equ 11
XST_UNADVSENT                        equ 12
XST_ADVACKRCVD                       equ 13
XST_UNADVACKRCVD                     equ 14
XST_ADVDATASENT                      equ 15
XST_ADVDATAACKRCVD                   equ 16
CADV_LATEACK                         equ 0FFFFh
ST_CONNECTED                         equ 1h
ST_ADVISE                            equ 2h
ST_ISLOCAL                           equ 4h
ST_BLOCKED                           equ 8h
ST_CLIENT                            equ 10h
ST_TERMINATED                        equ 20h
ST_INLIST                            equ 40h
ST_BLOCKNEXT                         equ 80h
ST_ISSELF                            equ 100h
DDE_FACK                             equ 8000h
DDE_FBUSY                            equ 4000h
DDE_FDEFERUPD                        equ 4000h
DDE_FACKREQ                          equ 8000h
DDE_FRELEASE                         equ 2000h
DDE_FREQUESTED                       equ 1000h
DDE_FAPPSTATUS                       equ 0FFh
DDE_FNOTPROCESSED                    equ 0h
DDE_FACKRESERVED                     equ NOT DDE_FACK OR DDE_FBUSY OR DDE_FAPPSTATUS
DDE_FADVRESERVED                     equ NOT DDE_FACKREQ OR DDE_FDEFERUPD
DDE_FDATRESERVED                     equ NOT DDE_FACKREQ OR DDE_FRELEASE OR DDE_FREQUESTED
DDE_FPOKRESERVED                     equ NOT DDE_FRELEASE
CP_WINANSI                           equ 1004
CP_WINUNICODE                        equ 1200
XTYPF_NOBLOCK                        equ 2h
XTYPF_NODATA                         equ 4h
XTYPF_ACKREQ                         equ 8h
XCLASS_MASK                          equ 0FC00h
XCLASS_BOOL                          equ 1000h
XCLASS_DATA                          equ 2000h
XCLASS_FLAGS                         equ 4000h
XCLASS_NOTIFICATION                  equ 8000h
XTYP_ERROR                           equ 0h OR XCLASS_NOTIFICATION OR XTYPF_NOBLOCK
XTYP_ADVDATA                         equ 10h OR XCLASS_FLAGS
XTYP_ADVREQ                          equ 20h OR XCLASS_DATA OR XTYPF_NOBLOCK
XTYP_ADVSTART                        equ 30h OR XCLASS_BOOL
XTYP_ADVSTOP                         equ 40h OR XCLASS_NOTIFICATION
XTYP_EXECUTE                         equ 50h OR XCLASS_FLAGS
XTYP_CONNECT                         equ 60h OR XCLASS_BOOL OR XTYPF_NOBLOCK
XTYP_CONNECT_CONFIRM                 equ 70h OR XCLASS_NOTIFICATION OR XTYPF_NOBLOCK
XTYP_XACT_COMPLETE                   equ 80h OR XCLASS_NOTIFICATION
XTYP_POKE                            equ 90h OR XCLASS_FLAGS
XTYP_REGISTER                        equ 0A0h OR XCLASS_NOTIFICATION OR XTYPF_NOBLOCK
XTYP_REQUEST                         equ 0B0h OR XCLASS_DATA
XTYP_DISCONNECT                      equ 0C0h OR XCLASS_NOTIFICATION OR XTYPF_NOBLOCK
XTYP_UNREGISTER                      equ 0D0h OR XCLASS_NOTIFICATION OR XTYPF_NOBLOCK
XTYP_WILDCONNECT                     equ 0E0h OR XCLASS_DATA OR XTYPF_NOBLOCK
XTYP_MASK                            equ 0F0h
XTYP_SHIFT                           equ 4
TIMEOUT_ASYNC                        equ 0FFFFh
QID_SYNC                             equ 0FFFFh
CBR_BLOCK                            equ 0FFFFh
CBF_FAIL_SELFCONNECTIONS             equ 1000h
CBF_FAIL_CONNECTIONS                 equ 2000h
CBF_FAIL_ADVISES                     equ 4000h
CBF_FAIL_EXECUTES                    equ 8000h
CBF_FAIL_POKES                       equ 10000h
CBF_FAIL_REQUESTS                    equ 20000h
CBF_FAIL_ALLSVRXACTIONS              equ 3F000h
CBF_SKIP_CONNECT_CONFIRMS            equ 40000h
CBF_SKIP_REGISTRATIONS               equ 80000h
CBF_SKIP_UNREGISTRATIONS             equ 100000h
CBF_SKIP_DISCONNECTS                 equ 200000h
CBF_SKIP_ALLNOTIFICATIONS            equ 3C0000h
APPCMD_CLIENTONLY                    equ 10h
APPCMD_FILTERINITS                   equ 20h
APPCMD_MASK                          equ 0FF0h
APPCLASS_STANDARD                    equ 0h
APPCLASS_MASK                        equ 0Fh
EC_ENABLEALL                         equ 0
EC_ENABLEONE                         equ ST_BLOCKNEXT
EC_DISABLE                           equ ST_BLOCKED
EC_QUERYWAITING                      equ 2
DNS_REGISTER                         equ 1h
DNS_UNREGISTER                       equ 2h
DNS_FILTERON                         equ 4h
DNS_FILTEROFF                        equ 8h
HDATA_APPOWNED                       equ 1h
DMLERR_NO_ERROR                      equ 0
DMLERR_FIRST                         equ 4000h
DMLERR_ADVACKTIMEOUT                 equ 4000h
DMLERR_BUSY                          equ 4001h
DMLERR_DATAACKTIMEOUT                equ 4002h
DMLERR_DLL_NOT_INITIALIZED           equ 4003h
DMLERR_DLL_USAGE                     equ 4004h
DMLERR_EXECACKTIMEOUT                equ 4005h
DMLERR_INVALIDPARAMETER              equ 4006h
DMLERR_LOW_MEMORY                    equ 4007h
DMLERR_MEMORY_ERROR                  equ 4008h
DMLERR_NOTPROCESSED                  equ 4009h
DMLERR_NO_CONV_ESTABLISHED           equ 400Ah
DMLERR_POKEACKTIMEOUT                equ 400Bh
DMLERR_POSTMSG_FAILED                equ 400Ch
DMLERR_REENTRANCY                    equ 400Dh
DMLERR_SERVER_DIED                   equ 400Eh
DMLERR_SYS_ERROR                     equ 400Fh
DMLERR_UNADVACKTIMEOUT               equ 4010h
DMLERR_UNFOUND_QUEUE_ID              equ 4011h
DMLERR_LAST                          equ 4011h
MH_CREATE                            equ 1
MH_KEEP                              equ 2
MH_DELETE                            equ 3
MH_CLEANUP                           equ 4
MAX_MONITORS                         equ 4
APPCLASS_MONITOR                     equ 1h
XTYP_MONITOR                         equ 0F0h OR XCLASS_NOTIFICATION OR XTYPF_NOBLOCK
MF_HSZ_INFO                          equ 1000000h
MF_SENDMSGS                          equ 2000000h
MF_POSTMSGS                          equ 4000000h
MF_CALLBACKS                         equ 8000000h
MF_ERRORS                            equ 10000000h
MF_LINKS                             equ 20000000h
MF_CONV                              equ 40000000h
MF_MASK                              equ 0FF000000h
NO_ERROR                             equ 0
ERROR_SUCCESS                        equ 0
ERROR_INVALID_FUNCTION               equ 1
ERROR_FILE_NOT_FOUND                 equ 2
ERROR_PATH_NOT_FOUND                 equ 3
ERROR_TOO_MANY_OPEN_FILES            equ 4
ERROR_ACCESS_DENIED                  equ 5
ERROR_INVALID_HANDLE                 equ 6
ERROR_ARENA_TRASHED                  equ 7
ERROR_NOT_ENOUGH_MEMORY              equ 8
ERROR_INVALID_BLOCK                  equ 9
ERROR_BAD_ENVIRONMENT                equ 10
ERROR_BAD_FORMAT                     equ 11
ERROR_INVALID_ACCESS                 equ 12
ERROR_INVALID_DATA                   equ 13
ERROR_OUTOFMEMORY                    equ 14
ERROR_INVALID_DRIVE                  equ 15
ERROR_CURRENT_DIRECTORY              equ 16
ERROR_NOT_SAME_DEVICE                equ 17
ERROR_NO_MORE_FILES                  equ 18
ERROR_WRITE_PROTECT                  equ 19
ERROR_BAD_UNIT                       equ 20
ERROR_NOT_READY                      equ 21
ERROR_BAD_COMMAND                    equ 22
ERROR_CRC                            equ 23
ERROR_BAD_LENGTH                     equ 24
ERROR_SEEK                           equ 25
ERROR_NOT_DOS_DISK                   equ 26
ERROR_SECTOR_NOT_FOUND               equ 27
ERROR_OUT_OF_PAPER                   equ 28
ERROR_WRITE_FAULT                    equ 29
ERROR_READ_FAULT                     equ 30
ERROR_GEN_FAILURE                    equ 31
ERROR_SHARING_VIOLATION              equ 32
ERROR_LOCK_VIOLATION                 equ 33
ERROR_WRONG_DISK                     equ 34
ERROR_SHARING_BUFFER_EXCEEDED        equ 36
ERROR_HANDLE_EOF                     equ 38
ERROR_HANDLE_DISK_FULL               equ 39
ERROR_NOT_SUPPORTED                  equ 50
ERROR_REM_NOT_LIST                   equ 51
ERROR_DUP_NAME                       equ 52
ERROR_BAD_NETPATH                    equ 53
ERROR_NETWORK_BUSY                   equ 54
ERROR_DEV_NOT_EXIST                  equ 55
ERROR_TOO_MANY_CMDS                  equ 56
ERROR_ADAP_HDW_ERR                   equ 57
ERROR_BAD_NET_RESP                   equ 58
ERROR_UNEXP_NET_ERR                  equ 59
ERROR_BAD_REM_ADAP                   equ 60
ERROR_PRINTQ_FULL                    equ 61
ERROR_NO_SPOOL_SPACE                 equ 62
ERROR_PRINT_CANCELLED                equ 63
ERROR_NETNAME_DELETED                equ 64
ERROR_NETWORK_ACCESS_DENIED          equ 65
ERROR_BAD_DEV_TYPE                   equ 66
ERROR_BAD_NET_NAME                   equ 67
ERROR_TOO_MANY_NAMES                 equ 68
ERROR_TOO_MANY_SESS                  equ 69
ERROR_SHARING_PAUSED                 equ 70
ERROR_REQ_NOT_ACCEP                  equ 71
ERROR_REDIR_PAUSED                   equ 72
ERROR_FILE_EXISTS                    equ 80
ERROR_CANNOT_MAKE                    equ 82
ERROR_FAIL_I24                       equ 83
ERROR_OUT_OF_STRUCTURES              equ 84
ERROR_ALREADY_ASSIGNED               equ 85
ERROR_INVALID_PASSWORD               equ 86
ERROR_INVALID_PARAMETER              equ 87
ERROR_NET_WRITE_FAULT                equ 88
ERROR_NO_PROC_SLOTS                  equ 89
ERROR_TOO_MANY_SEMAPHORES            equ 100
ERROR_EXCL_SEM_ALREADY_OWNED         equ 101
ERROR_SEM_IS_SET                     equ 102
ERROR_TOO_MANY_SEM_REQUESTS          equ 103
ERROR_INVALID_AT_INTERRUPT_TIME      equ 104
ERROR_SEM_OWNER_DIED                 equ 105
ERROR_SEM_USER_LIMIT                 equ 106
ERROR_DISK_CHANGE                    equ 107
ERROR_DRIVE_LOCKED                   equ 108
ERROR_BROKEN_PIPE                    equ 109
ERROR_OPEN_FAILED                    equ 110
ERROR_BUFFER_OVERFLOW                equ 111
ERROR_DISK_FULL                      equ 112
ERROR_NO_MORE_SEARCH_HANDLES         equ 113
ERROR_INVALID_TARGET_HANDLE          equ 114
ERROR_INVALID_CATEGORY               equ 117
ERROR_INVALID_VERIFY_SWITCH          equ 118
ERROR_BAD_DRIVER_LEVEL               equ 119
ERROR_CALL_NOT_IMPLEMENTED           equ 120
ERROR_SEM_TIMEOUT                    equ 121
ERROR_INSUFFICIENT_BUFFER            equ 122
ERROR_INVALID_NAME                   equ 123
ERROR_INVALID_LEVEL                  equ 124
ERROR_NO_VOLUME_LABEL                equ 125
ERROR_MOD_NOT_FOUND                  equ 126
ERROR_PROC_NOT_FOUND                 equ 127
ERROR_WAIT_NO_CHILDREN               equ 128
ERROR_CHILD_NOT_COMPLETE             equ 129
ERROR_DIRECT_ACCESS_HANDLE           equ 130
ERROR_NEGATIVE_SEEK                  equ 131
ERROR_SEEK_ON_DEVICE                 equ 132
ERROR_IS_JOIN_TARGET                 equ 133
ERROR_IS_JOINED                      equ 134
ERROR_IS_SUBSTED                     equ 135
ERROR_NOT_JOINED                     equ 136
ERROR_NOT_SUBSTED                    equ 137
ERROR_JOIN_TO_JOIN                   equ 138
ERROR_SUBST_TO_SUBST                 equ 139
ERROR_JOIN_TO_SUBST                  equ 140
ERROR_SUBST_TO_JOIN                  equ 141
ERROR_BUSY_DRIVE                     equ 142
ERROR_SAME_DRIVE                     equ 143
ERROR_DIR_NOT_ROOT                   equ 144
ERROR_DIR_NOT_EMPTY                  equ 145
ERROR_IS_SUBST_PATH                  equ 146
ERROR_IS_JOIN_PATH                   equ 147
ERROR_PATH_BUSY                      equ 148
ERROR_IS_SUBST_TARGET                equ 149
ERROR_SYSTEM_TRACE                   equ 150
ERROR_INVALID_EVENT_COUNT            equ 151
ERROR_TOO_MANY_MUXWAITERS            equ 152
ERROR_INVALID_LIST_FORMAT            equ 153
ERROR_LABEL_TOO_LONG                 equ 154
ERROR_TOO_MANY_TCBS                  equ 155
ERROR_SIGNAL_REFUSED                 equ 156
ERROR_DISCARDED                      equ 157
ERROR_NOT_LOCKED                     equ 158
ERROR_BAD_THREADID_ADDR              equ 159
ERROR_BAD_ARGUMENTS                  equ 160
ERROR_BAD_PATHNAME                   equ 161
ERROR_SIGNAL_PENDING                 equ 162
ERROR_MAX_THRDS_REACHED              equ 164
ERROR_LOCK_FAILED                    equ 167
ERROR_BUSY                           equ 170
ERROR_CANCEL_VIOLATION               equ 173
ERROR_ATOMIC_LOCKS_NOT_SUPPORTED     equ 174
ERROR_INVALID_SEGMENT_NUMBER         equ 180
ERROR_INVALID_ORDINAL                equ 182
ERROR_ALREADY_EXISTS                 equ 183
ERROR_INVALID_FLAG_NUMBER            equ 186
ERROR_SEM_NOT_FOUND                  equ 187
ERROR_INVALID_STARTING_CODESEG       equ 188
ERROR_INVALID_STACKSEG               equ 189
ERROR_INVALID_MODULETYPE             equ 190
ERROR_INVALID_EXE_SIGNATURE          equ 191
ERROR_EXE_MARKED_INVALID             equ 192
ERROR_BAD_EXE_FORMAT                 equ 193
ERROR_ITERATED_DATA_EXCEEDS_64k      equ 194
ERROR_INVALID_MINALLOCSIZE           equ 195
ERROR_DYNLINK_FROM_INVALID_RING      equ 196
ERROR_IOPL_NOT_ENABLED               equ 197
ERROR_INVALID_SEGDPL                 equ 198
ERROR_AUTODATASEG_EXCEEDS_64k        equ 199
ERROR_RING2SEG_MUST_BE_MOVABLE       equ 200
ERROR_RELOC_CHAIN_XEEDS_SEGLIM       equ 201
ERROR_INFLOOP_IN_RELOC_CHAIN         equ 202
ERROR_ENVVAR_NOT_FOUND               equ 203
ERROR_NO_SIGNAL_SENT                 equ 205
ERROR_FILENAME_EXCED_RANGE           equ 206
ERROR_RING2_STACK_IN_USE             equ 207
ERROR_META_EXPANSION_TOO_LONG        equ 208
ERROR_INVALID_SIGNAL_NUMBER          equ 209
ERROR_THREAD_1_INACTIVE              equ 210
ERROR_LOCKED                         equ 212
ERROR_TOO_MANY_MODULES               equ 214
ERROR_NESTING_NOT_ALLOWED            equ 215
ERROR_BAD_PIPE                       equ 230
ERROR_PIPE_BUSY                      equ 231
ERROR_NO_DATA                        equ 232
ERROR_PIPE_NOT_CONNECTED             equ 233
ERROR_MORE_DATA                      equ 234
ERROR_VC_DISCONNECTED                equ 240
ERROR_INVALID_EA_NAME                equ 254
ERROR_EA_LIST_INCONSISTENT           equ 255
ERROR_NO_MORE_ITEMS                  equ 259
ERROR_CANNOT_COPY                    equ 266
ERROR_DIRECTORY                      equ 267
ERROR_EAS_DIDNT_FIT                  equ 275
ERROR_EA_FILE_CORRUPT                equ 276
ERROR_EA_TABLE_FULL                  equ 277
ERROR_INVALID_EA_HANDLE              equ 278
ERROR_EAS_NOT_SUPPORTED              equ 282
ERROR_NOT_OWNER                      equ 288
ERROR_TOO_MANY_POSTS                 equ 298
ERROR_MR_MID_NOT_FOUND               equ 317
ERROR_INVALID_ADDRESS                equ 487
ERROR_ARITHMETIC_OVERFLOW            equ 534
ERROR_PIPE_CONNECTED                 equ 535
ERROR_PIPE_LISTENING                 equ 536
ERROR_EA_ACCESS_DENIED               equ 994
ERROR_OPERATION_ABORTED              equ 995
ERROR_IO_INCOMPLETE                  equ 996
ERROR_IO_PENDING                     equ 997
ERROR_NOACCESS                       equ 998
ERROR_SWAPERROR                      equ 999
ERROR_STACK_OVERFLOW                 equ 1001
ERROR_INVALID_MESSAGE                equ 1002
ERROR_CAN_NOT_COMPLETE               equ 1003
ERROR_INVALID_FLAGS                  equ 1004
ERROR_UNRECOGNIZED_VOLUME            equ 1005
ERROR_FILE_INVALID                   equ 1006
ERROR_FULLSCREEN_MODE                equ 1007
ERROR_NO_TOKEN                       equ 1008
ERROR_BADDB                          equ 1009
ERROR_BADKEY                         equ 1010
ERROR_CANTOPEN                       equ 1011
ERROR_CANTREAD                       equ 1012
ERROR_CANTWRITE                      equ 1013
ERROR_REGISTRY_RECOVERED             equ 1014
ERROR_REGISTRY_CORRUPT               equ 1015
ERROR_REGISTRY_IO_FAILED             equ 1016
ERROR_NOT_REGISTRY_FILE              equ 1017
ERROR_KEY_DELETED                    equ 1018
ERROR_NO_LOG_SPACE                   equ 1019
ERROR_KEY_HAS_CHILDREN               equ 1020
ERROR_CHILD_MUST_BE_VOLATILE         equ 1021
ERROR_NOTIFY_ENUM_DIR                equ 1022
ERROR_DEPENDENT_SERVICES_RUNNING     equ 1051
ERROR_INVALID_SERVICE_CONTROL        equ 1052
ERROR_SERVICE_REQUEST_TIMEOUT        equ 1053
ERROR_SERVICE_NO_THREAD              equ 1054
ERROR_SERVICE_DATABASE_LOCKED        equ 1055
ERROR_SERVICE_ALREADY_RUNNING        equ 1056
ERROR_INVALID_SERVICE_ACCOUNT        equ 1057
ERROR_SERVICE_DISABLED               equ 1058
ERROR_CIRCULAR_DEPENDENCY            equ 1059
ERROR_SERVICE_DOES_NOT_EXIST         equ 1060
ERROR_SERVICE_CANNOT_ACCEPT_CTRL     equ 1061
ERROR_SERVICE_NOT_ACTIVE             equ 1062
ERROR_FAILED_SERVICE_CONTROLLER_CONNECT              equ 1063
ERROR_EXCEPTION_IN_SERVICE           equ 1064
ERROR_DATABASE_DOES_NOT_EXIST        equ 1065
ERROR_SERVICE_SPECIFIC_ERROR         equ 1066
ERROR_PROCESS_ABORTED                equ 1067
ERROR_SERVICE_DEPENDENCY_FAIL        equ 1068
ERROR_SERVICE_LOGON_FAILED           equ 1069
ERROR_SERVICE_START_HANG             equ 1070
ERROR_INVALID_SERVICE_LOCK           equ 1071
ERROR_SERVICE_MARKED_FOR_DELETE      equ 1072
ERROR_SERVICE_EXISTS                 equ 1073
ERROR_ALREADY_RUNNING_LKG            equ 1074
ERROR_SERVICE_DEPENDENCY_DELETED     equ 1075
ERROR_BOOT_ALREADY_ACCEPTED          equ 1076
ERROR_SERVICE_NEVER_STARTED          equ 1077
ERROR_DUPLICATE_SERVICE_NAME         equ 1078
ERROR_END_OF_MEDIA                   equ 1100
ERROR_FILEMARK_DETECTED              equ 1101
ERROR_BEGINNING_OF_MEDIA             equ 1102
ERROR_SETMARK_DETECTED               equ 1103
ERROR_NO_DATA_DETECTED               equ 1104
ERROR_PARTITION_FAILURE              equ 1105
ERROR_INVALID_BLOCK_LENGTH           equ 1106
ERROR_DEVICE_NOT_PARTITIONED         equ 1107
ERROR_UNABLE_TO_LOCK_MEDIA           equ 1108
ERROR_UNABLE_TO_UNLOAD_MEDIA         equ 1109
ERROR_MEDIA_CHANGED                  equ 1110
ERROR_BUS_RESET                      equ 1111
ERROR_NO_MEDIA_IN_DRIVE              equ 1112
ERROR_NO_UNICODE_TRANSLATION         equ 1113
ERROR_DLL_INIT_FAILED                equ 1114
ERROR_SHUTDOWN_IN_PROGRESS           equ 1115
ERROR_NO_SHUTDOWN_IN_PROGRESS        equ 1116
ERROR_IO_DEVICE                      equ 1117
ERROR_SERIAL_NO_DEVICE               equ 1118
ERROR_IRQ_BUSY                       equ 1119
ERROR_MORE_WRITES                    equ 1120
ERROR_COUNTER_TIMEOUT                equ 1121
ERROR_FLOPPY_ID_MARK_NOT_FOUND       equ 1122
ERROR_FLOPPY_WRONG_CYLINDER          equ 1123
ERROR_FLOPPY_UNKNOWN_ERROR           equ 1124
ERROR_FLOPPY_BAD_REGISTERS           equ 1125
ERROR_DISK_RECALIBRATE_FAILED        equ 1126
ERROR_DISK_OPERATION_FAILED          equ 1127
ERROR_DISK_RESET_FAILED              equ 1128
ERROR_EOM_OVERFLOW                   equ 1129
ERROR_NOT_ENOUGH_SERVER_MEMORY       equ 1130
ERROR_POSSIBLE_DEADLOCK              equ 1131
ERROR_MAPPED_ALIGNMENT               equ 1132
ERROR_INVALID_PIXEL_FORMAT           equ 2000
ERROR_BAD_DRIVER                     equ 2001
ERROR_INVALID_WINDOW_STYLE           equ 2002
ERROR_METAFILE_NOT_SUPPORTED         equ 2003
ERROR_TRANSFORM_NOT_SUPPORTED        equ 2004
ERROR_CLIPPING_NOT_SUPPORTED         equ 2005
ERROR_UNKNOWN_PRINT_MONITOR          equ 3000
ERROR_PRINTER_DRIVER_IN_USE          equ 3001
ERROR_SPOOL_FILE_NOT_FOUND           equ 3002
ERROR_SPL_NO_STARTDOC                equ 3003
ERROR_SPL_NO_ADDJOB                  equ 3004
ERROR_PRINT_PROCESSOR_ALREADY_INSTALLED              equ 3005
ERROR_PRINT_MONITOR_ALREADY_INSTALLED                equ 3006
ERROR_WINS_INTERNAL                  equ 4000
ERROR_CAN_NOT_DEL_LOCAL_WINS         equ 4001
ERROR_STATIC_INIT                    equ 4002
ERROR_INC_BACKUP                     equ 4003
ERROR_FULL_BACKUP                    equ 4004
ERROR_REC_NON_EXISTENT               equ 4005
ERROR_RPL_NOT_ALLOWED                equ 4006
SEVERITY_SUCCESS                     equ 0
SEVERITY_ERROR                       equ 1
FACILITY_NT_BIT                      equ 10000000h
NOERROR                              equ 0
E_UNEXPECTED                         equ 8000FFFFh
E_NOTIMPL                            equ 80004001h
E_OUTOFMEMORY                        equ 8007000Eh
E_INVALIDARG                         equ 80070057h
E_NOINTERFACE                        equ 80004002h
E_POINTER                            equ 80004003h
E_HANDLE                             equ 80070006h
E_ABORT                              equ 80004004h
E_FAIL                               equ 80004005h
E_ACCESSDENIED                       equ 80070005h
CO_E_INIT_TLS                        equ 80004006h
CO_E_INIT_SHARED_ALLOCATOR           equ 80004007h
CO_E_INIT_MEMORY_ALLOCATOR           equ 80004008h
CO_E_INIT_CLASS_CACHE                equ 80004009h
CO_E_INIT_RPC_CHANNEL                equ 8000400Ah
CO_E_INIT_TLS_SET_CHANNEL_CONTROL    equ 8000400Bh
CO_E_INIT_TLS_CHANNEL_CONTROL        equ 8000400Ch
CO_E_INIT_UNACCEPTED_USER_ALLOCATOR  equ 8000400Dh
CO_E_INIT_SCM_MUTEX_EXISTS           equ 8000400Eh
CO_E_INIT_SCM_FILE_MAPPING_EXISTS    equ 8000400Fh
CO_E_INIT_SCM_MAP_VIEW_OF_FILE       equ 80004010h
CO_E_INIT_SCM_EXEC_FAILURE           equ 80004011h
CO_E_INIT_ONLY_SINGLE_THREADED       equ 80004012h
S_OK                                 equ 0h
S_FALSE                              equ 1h
OLE_E_FIRST                          equ 80040000h
OLE_E_LAST                           equ 800400FFh
OLE_S_FIRST                          equ 40000h
OLE_S_LAST                           equ 400FFh
OLE_E_OLEVERB                        equ 80040000h
OLE_E_ADVF                           equ 80040001h
OLE_E_ENUM_NOMORE                    equ 80040002h
OLE_E_ADVISENOTSUPPORTED             equ 80040003h
OLE_E_NOCONNECTION                   equ 80040004h
OLE_E_NOTRUNNING                     equ 80040005h
OLE_E_NOCACHE                        equ 80040006h
OLE_E_BLANK                          equ 80040007h
OLE_E_CLASSDIFF                      equ 80040008h
OLE_E_CANT_GETMONIKER                equ 80040009h
OLE_E_CANT_BINDTOSOURCE              equ 8004000Ah
OLE_E_STATIC                         equ 8004000Bh
OLE_E_PROMPTSAVECANCELLED            equ 8004000Ch
OLE_E_INVALIDRECT                    equ 8004000Dh
OLE_E_WRONGCOMPOBJ                   equ 8004000Eh
OLE_E_INVALIDHWND                    equ 8004000Fh
OLE_E_NOT_INPLACEACTIVE              equ 80040010h
OLE_E_CANTCONVERT                    equ 80040011h
OLE_E_NOSTORAGE                      equ 80040012h
DV_E_FORMATETC                       equ 80040064h
DV_E_DVTARGETDEVICE                  equ 80040065h
DV_E_STGMEDIUM                       equ 80040066h
DV_E_STATDATA                        equ 80040067h
DV_E_LINDEX                          equ 80040068h
DV_E_TYMED                           equ 80040069h
DV_E_CLIPFORMAT                      equ 8004006Ah
DV_E_DVASPECT                        equ 8004006Bh
DV_E_DVTARGETDEVICE_SIZE             equ 8004006Ch
DV_E_NOIVIEWOBJECT                   equ 8004006Dh
DRAGDROP_E_FIRST                     equ 80040100h
DRAGDROP_E_LAST                      equ 8004010Fh
DRAGDROP_S_FIRST                     equ 40100h
DRAGDROP_S_LAST                      equ 4010Fh
DRAGDROP_E_NOTREGISTERED             equ 80040100h
DRAGDROP_E_ALREADYREGISTERED         equ 80040101h
DRAGDROP_E_INVALIDHWND               equ 80040102h
CLASSFACTORY_E_FIRST                 equ 80040110h
CLASSFACTORY_E_LAST                  equ 8004011Fh
CLASSFACTORY_S_FIRST                 equ 40110h
CLASSFACTORY_S_LAST                  equ 4011Fh
CLASS_E_NOAGGREGATION                equ 80040110h
CLASS_E_CLASSNOTAVAILABLE            equ 80040111h
MARSHAL_E_FIRST                      equ 80040120h
MARSHAL_E_LAST                       equ 8004012Fh
MARSHAL_S_FIRST                      equ 40120h
MARSHAL_S_LAST                       equ 4012Fh
DATA_E_FIRST                         equ 80040130h
DATA_E_LAST                          equ 8004013Fh
DATA_S_FIRST                         equ 40130h
DATA_S_LAST                          equ 4013Fh
VIEW_E_FIRST                         equ 80040140h
VIEW_E_LAST                          equ 8004014Fh
VIEW_S_FIRST                         equ 40140h
VIEW_S_LAST                          equ 4014Fh
VIEW_E_DRAW                          equ 80040140h
REGDB_E_FIRST                        equ 80040150h
REGDB_E_LAST                         equ 8004015Fh
REGDB_S_FIRST                        equ 40150h
REGDB_S_LAST                         equ 4015Fh
REGDB_E_READREGDB                    equ 80040150h
REGDB_E_WRITEREGDB                   equ 80040151h
REGDB_E_KEYMISSING                   equ 80040152h
REGDB_E_INVALIDVALUE                 equ 80040153h
REGDB_E_CLASSNOTREG                  equ 80040154h
REGDB_E_IIDNOTREG                    equ 80040155h
CACHE_E_FIRST                        equ 80040170h
CACHE_E_LAST                         equ 8004017Fh
CACHE_S_FIRST                        equ 40170h
CACHE_S_LAST                         equ 4017Fh
CACHE_E_NOCACHE_UPDATED              equ 80040170h
OLEOBJ_E_FIRST                       equ 80040180h
OLEOBJ_E_LAST                        equ 8004018Fh
OLEOBJ_S_FIRST                       equ 40180h
OLEOBJ_S_LAST                        equ 4018Fh
OLEOBJ_E_NOVERBS                     equ 80040180h
OLEOBJ_E_INVALIDVERB                 equ 80040181h
CLIENTSITE_E_FIRST                   equ 80040190h
CLIENTSITE_E_LAST                    equ 8004019Fh
CLIENTSITE_S_FIRST                   equ 40190h
CLIENTSITE_S_LAST                    equ 4019Fh
INPLACE_E_NOTUNDOABLE                equ 800401A0h
INPLACE_E_NOTOOLSPACE                equ 800401A1h
INPLACE_E_FIRST                      equ 800401A0h
INPLACE_E_LAST                       equ 800401AFh
INPLACE_S_FIRST                      equ 401A0h
INPLACE_S_LAST                       equ 401AFh
ENUM_E_FIRST                         equ 800401B0h
ENUM_E_LAST                          equ 800401BFh
ENUM_S_FIRST                         equ 401B0h
ENUM_S_LAST                          equ 401BFh
CONVERT10_E_FIRST                    equ 800401C0h
CONVERT10_E_LAST                     equ 800401CFh
CONVERT10_S_FIRST                    equ 401C0h
CONVERT10_S_LAST                     equ 401CFh
CONVERT10_E_OLESTREAM_GET            equ 800401C0h
CONVERT10_E_OLESTREAM_PUT            equ 800401C1h
CONVERT10_E_OLESTREAM_FMT            equ 800401C2h
CONVERT10_E_OLESTREAM_BITMAP_TO_DIB  equ 800401C3h
CONVERT10_E_STG_FMT                  equ 800401C4h
CONVERT10_E_STG_NO_STD_STREAM        equ 800401C5h
CONVERT10_E_STG_DIB_TO_BITMAP        equ 800401C6h
CLIPBRD_E_FIRST                      equ 800401D0h
CLIPBRD_E_LAST                       equ 800401DFh
CLIPBRD_S_FIRST                      equ 401D0h
CLIPBRD_S_LAST                       equ 401DFh
CLIPBRD_E_CANT_OPEN                  equ 800401D0h
CLIPBRD_E_CANT_EMPTY                 equ 800401D1h
CLIPBRD_E_CANT_SET                   equ 800401D2h
CLIPBRD_E_BAD_DATA                   equ 800401D3h
CLIPBRD_E_CANT_CLOSE                 equ 800401D4h
MK_E_FIRST                           equ 800401E0h
MK_E_LAST                            equ 800401EFh
MK_S_FIRST                           equ 401E0h
MK_S_LAST                            equ 401EFh
MK_E_CONNECTMANUALLY                 equ 800401E0h
MK_E_EXCEEDEDDEADLINE                equ 800401E1h
MK_E_NEEDGENERIC                     equ 800401E2h
MK_E_UNAVAILABLE                     equ 800401E3h
MK_E_SYNTAX                          equ 800401E4h
MK_E_NOOBJECT                        equ 800401E5h
MK_E_INVALIDEXTENSION                equ 800401E6h
MK_E_INTERMEDIATEINTERFACENOTSUPPORTED               equ 800401E7h
MK_E_NOTBINDABLE                     equ 800401E8h
MK_E_NOTBOUND                        equ 800401E9h
MK_E_CANTOPENFILE                    equ 800401EAh
MK_E_MUSTBOTHERUSER                  equ 800401EBh
MK_E_NOINVERSE                       equ 800401ECh
MK_E_NOSTORAGE                       equ 800401EDh
MK_E_NOPREFIX                        equ 800401EEh
MK_E_ENUMERATION_FAILED              equ 800401EFh
CO_E_FIRST                           equ 800401F0h
CO_E_LAST                            equ 800401FFh
CO_S_FIRST                           equ 401F0h
CO_S_LAST                            equ 401FFh
CO_E_NOTINITIALIZED                  equ 800401F0h
CO_E_ALREADYINITIALIZED              equ 800401F1h
CO_E_CANTDETERMINECLASS              equ 800401F2h
CO_E_CLASSSTRING                     equ 800401F3h
CO_E_IIDSTRING                       equ 800401F4h
CO_E_APPNOTFOUND                     equ 800401F5h
CO_E_APPSINGLEUSE                    equ 800401F6h
CO_E_ERRORINAPP                      equ 800401F7h
CO_E_DLLNOTFOUND                     equ 800401F8h
CO_E_ERRORINDLL                      equ 800401F9h
CO_E_WRONGOSFORAPP                   equ 800401FAh
CO_E_OBJNOTREG                       equ 800401FBh
CO_E_OBJISREG                        equ 800401FCh
CO_E_OBJNOTCONNECTED                 equ 800401FDh
CO_E_APPDIDNTREG                     equ 800401FEh
CO_E_RELEASED                        equ 800401FFh
OLE_S_USEREG                         equ 40000h
OLE_S_STATIC                         equ 40001h
OLE_S_MAC_CLIPFORMAT                 equ 40002h
DRAGDROP_S_DROP                      equ 40100h
DRAGDROP_S_CANCEL                    equ 40101h
DRAGDROP_S_USEDEFAULTCURSORS         equ 40102h
DATA_S_SAMEFORMATETC                 equ 40130h
VIEW_S_ALREADY_FROZEN                equ 40140h
CACHE_S_FORMATETC_NOTSUPPORTED       equ 40170h
CACHE_S_SAMECACHE                    equ 40171h
CACHE_S_SOMECACHES_NOTUPDATED        equ 40172h
OLEOBJ_S_INVALIDVERB                 equ 40180h
OLEOBJ_S_CANNOT_DOVERB_NOW           equ 40181h
OLEOBJ_S_INVALIDHWND                 equ 40182h
INPLACE_S_TRUNCATED                  equ 401A0h
CONVERT10_S_NO_PRESENTATION          equ 401C0h
MK_S_REDUCED_TO_SELF                 equ 401E2h
MK_S_ME                              equ 401E4h
MK_S_HIM                             equ 401E5h
MK_S_US                              equ 401E6h
MK_S_MONIKERALREADYREGISTERED        equ 401E7h
CO_E_CLASS_CREATE_FAILED             equ 80080001h
CO_E_SCM_ERROR                       equ 80080002h
CO_E_SCM_RPC_FAILURE                 equ 80080003h
CO_E_BAD_PATH                        equ 80080004h
CO_E_SERVER_EXEC_FAILURE             equ 80080005h
CO_E_OBJSRV_RPC_FAILURE              equ 80080006h
MK_E_NO_NORMALIZED                   equ 80080007h
CO_E_SERVER_STOPPING                 equ 80080008h
MEM_E_INVALID_ROOT                   equ 80080009h
MEM_E_INVALID_LINK                   equ 80080010h
MEM_E_INVALID_SIZE                   equ 80080011h
DISP_E_UNKNOWNINTERFACE              equ 80020001h
DISP_E_MEMBERNOTFOUND                equ 80020003h
DISP_E_PARAMNOTFOUND                 equ 80020004h
DISP_E_TYPEMISMATCH                  equ 80020005h
DISP_E_UNKNOWNNAME                   equ 80020006h
DISP_E_NONAMEDARGS                   equ 80020007h
DISP_E_BADVARTYPE                    equ 80020008h
DISP_E_EXCEPTION                     equ 80020009h
DISP_E_OVERFLOW                      equ 8002000Ah
DISP_E_BADINDEX                      equ 8002000Bh
DISP_E_UNKNOWNLCID                   equ 8002000Ch
DISP_E_ARRAYISLOCKED                 equ 8002000Dh
DISP_E_BADPARAMCOUNT                 equ 8002000Eh
DISP_E_PARAMNOTOPTIONAL              equ 8002000Fh
DISP_E_BADCALLEE                     equ 80020010h
DISP_E_NOTACOLLECTION                equ 80020011h
TYPE_E_BUFFERTOOSMALL                equ 80028016h
TYPE_E_INVDATAREAD                   equ 80028018h
TYPE_E_UNSUPFORMAT                   equ 80028019h
TYPE_E_REGISTRYACCESS                equ 8002801Ch
TYPE_E_LIBNOTREGISTERED              equ 8002801Dh
TYPE_E_UNDEFINEDTYPE                 equ 80028027h
TYPE_E_QUALIFIEDNAMEDISALLOWED       equ 80028028h
TYPE_E_INVALIDSTATE                  equ 80028029h
TYPE_E_WRONGTYPEKIND                 equ 8002802Ah
TYPE_E_ELEMENTNOTFOUND               equ 8002802Bh
TYPE_E_AMBIGUOUSNAME                 equ 8002802Ch
TYPE_E_NAMECONFLICT                  equ 8002802Dh
TYPE_E_UNKNOWNLCID                   equ 8002802Eh
TYPE_E_DLLFUNCTIONNOTFOUND           equ 8002802Fh
TYPE_E_BADMODULEKIND                 equ 800288BDh
TYPE_E_SIZETOOBIG                    equ 800288C5h
TYPE_E_DUPLICATEID                   equ 800288C6h
TYPE_E_INVALIDID                     equ 800288CFh
TYPE_E_TYPEMISMATCH                  equ 80028CA0h
TYPE_E_OUTOFBOUNDS                   equ 80028CA1h
TYPE_E_IOERROR                       equ 80028CA2h
TYPE_E_CANTCREATETMPFILE             equ 80028CA3h
TYPE_E_CANTLOADLIBRARY               equ 80029C4Ah
TYPE_E_INCONSISTENTPROPFUNCS         equ 80029C83h
TYPE_E_CIRCULARTYPE                  equ 80029C84h
STG_E_INVALIDFUNCTION                equ 80030001h
STG_E_FILENOTFOUND                   equ 80030002h
STG_E_PATHNOTFOUND                   equ 80030003h
STG_E_TOOMANYOPENFILES               equ 80030004h
STG_E_ACCESSDENIED                   equ 80030005h
STG_E_INVALIDHANDLE                  equ 80030006h
STG_E_INSUFFICIENTMEMORY             equ 80030008h
STG_E_INVALIDPOINTER                 equ 80030009h
STG_E_NOMOREFILES                    equ 80030012h
STG_E_DISKISWRITEPROTECTED           equ 80030013h
STG_E_SEEKERROR                      equ 80030019h
STG_E_WRITEFAULT                     equ 8003001Dh
STG_E_READFAULT                      equ 8003001Eh
STG_E_SHAREVIOLATION                 equ 80030020h
STG_E_LOCKVIOLATION                  equ 80030021h
STG_E_FILEALREADYEXISTS              equ 80030050h
STG_E_INVALIDPARAMETER               equ 80030057h
STG_E_MEDIUMFULL                     equ 80030070h
STG_E_ABNORMALAPIEXIT                equ 800300FAh
STG_E_INVALIDHEADER                  equ 800300FBh
STG_E_INVALIDNAME                    equ 800300FCh
STG_E_UNKNOWN                        equ 800300FDh
STG_E_UNIMPLEMENTEDFUNCTION          equ 800300FEh
STG_E_INVALIDFLAG                    equ 800300FFh
STG_E_INUSE                          equ 80030100h
STG_E_NOTCURRENT                     equ 80030101h
STG_E_REVERTED                       equ 80030102h
STG_E_CANTSAVE                       equ 80030103h
STG_E_OLDFORMAT                      equ 80030104h
STG_E_OLDDLL                         equ 80030105h
STG_E_SHAREREQUIRED                  equ 80030106h
STG_E_NOTFILEBASEDSTORAGE            equ 80030107h
STG_E_EXTANTMARSHALLINGS             equ 80030108h
STG_S_CONVERTED                      equ 30200h
RPC_E_CALL_REJECTED                  equ 80010001h
RPC_E_CALL_CANCELED                  equ 80010002h
RPC_E_CANTPOST_INSENDCALL            equ 80010003h
RPC_E_CANTCALLOUT_INASYNCCALL        equ 80010004h
RPC_E_CANTCALLOUT_INEXTERNALCALL     equ 80010005h
RPC_E_CONNECTION_TERMINATED          equ 80010006h
RPC_E_SERVER_DIED                    equ 80010007h
RPC_E_CLIENT_DIED                    equ 80010008h
RPC_E_INVALID_DATAPACKET             equ 80010009h
RPC_E_CANTTRANSMIT_CALL              equ 8001000Ah
RPC_E_CLIENT_CANTMARSHAL_DATA        equ 8001000Bh
RPC_E_CLIENT_CANTUNMARSHAL_DATA      equ 8001000Ch
RPC_E_SERVER_CANTMARSHAL_DATA        equ 8001000Dh
RPC_E_SERVER_CANTUNMARSHAL_DATA      equ 8001000Eh
RPC_E_INVALID_DATA                   equ 8001000Fh
RPC_E_INVALID_PARAMETER              equ 80010010h
RPC_E_CANTCALLOUT_AGAIN              equ 80010011h
RPC_E_SERVER_DIED_DNE                equ 80010012h
RPC_E_SYS_CALL_FAILED                equ 80010100h
RPC_E_OUT_OF_RESOURCES               equ 80010101h
RPC_E_ATTEMPTED_MULTITHREAD          equ 80010102h
RPC_E_NOT_REGISTERED                 equ 80010103h
RPC_E_FAULT                          equ 80010104h
RPC_E_SERVERFAULT                    equ 80010105h
RPC_E_CHANGED_MODE                   equ 80010106h
RPC_E_INVALIDMETHOD                  equ 80010107h
RPC_E_DISCONNECTED                   equ 80010108h
RPC_E_RETRY                          equ 80010109h
RPC_E_SERVERCALL_RETRYLATER          equ 8001010Ah
RPC_E_SERVERCALL_REJECTED            equ 8001010Bh
RPC_E_INVALID_CALLDATA               equ 8001010Ch
RPC_E_CANTCALLOUT_ININPUTSYNCCALL    equ 8001010Dh
RPC_E_WRONG_THREAD                   equ 8001010Eh
RPC_E_THREAD_NOT_INIT                equ 8001010Fh
RPC_E_UNEXPECTED                     equ 8001FFFFh
ERROR_BAD_USERNAME                   equ 2202
ERROR_NOT_CONNECTED                  equ 2250
ERROR_OPEN_FILES                     equ 2401
ERROR_DEVICE_IN_USE                  equ 2404
ERROR_BAD_DEVICE                     equ 1200
ERROR_CONNECTION_UNAVAIL             equ 1201
ERROR_DEVICE_ALREADY_REMEMBERED      equ 1202
ERROR_NO_NET_OR_BAD_PATH             equ 1203
ERROR_BAD_PROVIDER                   equ 1204
ERROR_CANNOT_OPEN_PROFILE            equ 1205
ERROR_BAD_PROFILE                    equ 1206
ERROR_NOT_CONTAINER                  equ 1207
ERROR_EXTENDED_ERROR                 equ 1208
ERROR_INVALID_GROUPNAME              equ 1209
ERROR_INVALID_COMPUTERNAME           equ 1210
ERROR_INVALID_EVENTNAME              equ 1211
ERROR_INVALID_DOMAINNAME             equ 1212
ERROR_INVALID_SERVICENAME            equ 1213
ERROR_INVALID_NETNAME                equ 1214
ERROR_INVALID_SHARENAME              equ 1215
ERROR_INVALID_PASSWORDNAME           equ 1216
ERROR_INVALID_MESSAGENAME            equ 1217
ERROR_INVALID_MESSAGEDEST            equ 1218
ERROR_SESSION_CREDENTIAL_CONFLICT    equ 1219
ERROR_REMOTE_SESSION_LIMIT_EXCEEDED  equ 1220
ERROR_DUP_DOMAINNAME                 equ 1221
ERROR_NO_NETWORK                     equ 1222
ERROR_NOT_ALL_ASSIGNED               equ 1300
ERROR_SOME_NOT_MAPPED                equ 1301
ERROR_NO_QUOTAS_FOR_ACCOUNT          equ 1302
ERROR_LOCAL_USER_SESSION_KEY         equ 1303
ERROR_NULL_LM_PASSWORD               equ 1304
ERROR_UNKNOWN_REVISION               equ 1305
ERROR_REVISION_MISMATCH              equ 1306
ERROR_INVALID_OWNER                  equ 1307
ERROR_INVALID_PRIMARY_GROUP          equ 1308
ERROR_NO_IMPERSONATION_TOKEN         equ 1309
ERROR_CANT_DISABLE_MANDATORY         equ 1310
ERROR_NO_LOGON_SERVERS               equ 1311
ERROR_NO_SUCH_LOGON_SESSION          equ 1312
ERROR_NO_SUCH_PRIVILEGE              equ 1313
ERROR_PRIVILEGE_NOT_HELD             equ 1314
ERROR_INVALID_ACCOUNT_NAME           equ 1315
ERROR_USER_EXISTS                    equ 1316
ERROR_NO_SUCH_USER                   equ 1317
ERROR_GROUP_EXISTS                   equ 1318
ERROR_NO_SUCH_GROUP                  equ 1319
ERROR_MEMBER_IN_GROUP                equ 1320
ERROR_MEMBER_NOT_IN_GROUP            equ 1321
ERROR_LAST_ADMIN                     equ 1322
ERROR_WRONG_PASSWORD                 equ 1323
ERROR_ILL_FORMED_PASSWORD            equ 1324
ERROR_PASSWORD_RESTRICTION           equ 1325
ERROR_LOGON_FAILURE                  equ 1326
ERROR_ACCOUNT_RESTRICTION            equ 1327
ERROR_INVALID_LOGON_HOURS            equ 1328
ERROR_INVALID_WORKSTATION            equ 1329
ERROR_PASSWORD_EXPIRED               equ 1330
ERROR_ACCOUNT_DISABLED               equ 1331
ERROR_NONE_MAPPED                    equ 1332
ERROR_TOO_MANY_LUIDS_REQUESTED       equ 1333
ERROR_LUIDS_EXHAUSTED                equ 1334
ERROR_INVALID_SUB_AUTHORITY          equ 1335
ERROR_INVALID_ACL                    equ 1336
ERROR_INVALID_SID                    equ 1337
ERROR_INVALID_SECURITY_DESCR         equ 1338
ERROR_BAD_INHERITANCE_ACL            equ 1340
ERROR_SERVER_DISABLED                equ 1341
ERROR_SERVER_NOT_DISABLED            equ 1342
ERROR_INVALID_ID_AUTHORITY           equ 1343
ERROR_ALLOTTED_SPACE_EXCEEDED        equ 1344
ERROR_INVALID_GROUP_ATTRIBUTES       equ 1345
ERROR_BAD_IMPERSONATION_LEVEL        equ 1346
ERROR_CANT_OPEN_ANONYMOUS            equ 1347
ERROR_BAD_VALIDATION_CLASS           equ 1348
ERROR_BAD_TOKEN_TYPE                 equ 1349
ERROR_NO_SECURITY_ON_OBJECT          equ 1350
ERROR_CANT_ACCESS_DOMAIN_INFO        equ 1351
ERROR_INVALID_SERVER_STATE           equ 1352
ERROR_INVALID_DOMAIN_STATE           equ 1353
ERROR_INVALID_DOMAIN_ROLE            equ 1354
ERROR_NO_SUCH_DOMAIN                 equ 1355
ERROR_DOMAIN_EXISTS                  equ 1356
ERROR_DOMAIN_LIMIT_EXCEEDED          equ 1357
ERROR_INTERNAL_DB_CORRUPTION         equ 1358
ERROR_INTERNAL_ERROR                 equ 1359
ERROR_GENERIC_NOT_MAPPED             equ 1360
ERROR_BAD_DESCRIPTOR_FORMAT          equ 1361
ERROR_NOT_LOGON_PROCESS              equ 1362
ERROR_LOGON_SESSION_EXISTS           equ 1363
ERROR_NO_SUCH_PACKAGE                equ 1364
ERROR_BAD_LOGON_SESSION_STATE        equ 1365
ERROR_LOGON_SESSION_COLLISION        equ 1366
ERROR_INVALID_LOGON_TYPE             equ 1367
ERROR_CANNOT_IMPERSONATE             equ 1368
ERROR_RXACT_INVALID_STATE            equ 1369
ERROR_RXACT_COMMIT_FAILURE           equ 1370
ERROR_SPECIAL_ACCOUNT                equ 1371
ERROR_SPECIAL_GROUP                  equ 1372
ERROR_SPECIAL_USER                   equ 1373
ERROR_MEMBERS_PRIMARY_GROUP          equ 1374
ERROR_TOKEN_ALREADY_IN_USE           equ 1375
ERROR_NO_SUCH_ALIAS                  equ 1376
ERROR_MEMBER_NOT_IN_ALIAS            equ 1377
ERROR_MEMBER_IN_ALIAS                equ 1378
ERROR_ALIAS_EXISTS                   equ 1379
ERROR_LOGON_NOT_GRANTED              equ 1380
ERROR_TOO_MANY_SECRETS               equ 1381
ERROR_SECRET_TOO_LONG                equ 1382
ERROR_INTERNAL_DB_ERROR              equ 1383
ERROR_TOO_MANY_CONTEXT_IDS           equ 1384
ERROR_LOGON_TYPE_NOT_GRANTED         equ 1385
ERROR_NT_CROSS_ENCRYPTION_REQUIRED   equ 1386
ERROR_NO_SUCH_MEMBER                 equ 1387
ERROR_INVALID_MEMBER                 equ 1388
ERROR_TOO_MANY_SIDS                  equ 1389
ERROR_LM_CROSS_ENCRYPTION_REQUIRED   equ 1390
ERROR_NO_INHERITANCE                 equ 1391
ERROR_FILE_CORRUPT                   equ 1392
ERROR_DISK_CORRUPT                   equ 1393
ERROR_NO_USER_SESSION_KEY            equ 1394
ERROR_INVALID_WINDOW_HANDLE          equ 1400
ERROR_INVALID_MENU_HANDLE            equ 1401
ERROR_INVALID_CURSOR_HANDLE          equ 1402
ERROR_INVALID_ACCEL_HANDLE           equ 1403
ERROR_INVALID_HOOK_HANDLE            equ 1404
ERROR_INVALID_DWP_HANDLE             equ 1405
ERROR_TLW_WITH_WSCHILD               equ 1406
ERROR_CANNOT_FIND_WND_CLASS          equ 1407
ERROR_WINDOW_OF_OTHER_THREAD         equ 1408
ERROR_HOTKEY_ALREADY_REGISTERED      equ 1409
ERROR_CLASS_ALREADY_EXISTS           equ 1410
ERROR_CLASS_DOES_NOT_EXIST           equ 1411
ERROR_CLASS_HAS_WINDOWS              equ 1412
ERROR_INVALID_INDEX                  equ 1413
ERROR_INVALID_ICON_HANDLE            equ 1414
ERROR_PRIVATE_DIALOG_INDEX           equ 1415
ERROR_LISTBOX_ID_NOT_FOUND           equ 1416
ERROR_NO_WILDCARD_CHARACTERS         equ 1417
ERROR_CLIPBOARD_NOT_OPEN             equ 1418
ERROR_HOTKEY_NOT_REGISTERED          equ 1419
ERROR_WINDOW_NOT_DIALOG              equ 1420
ERROR_CONTROL_ID_NOT_FOUND           equ 1421
ERROR_INVALID_COMBOBOX_MESSAGE       equ 1422
ERROR_WINDOW_NOT_COMBOBOX            equ 1423
ERROR_INVALID_EDIT_HEIGHT            equ 1424
ERROR_DC_NOT_FOUND                   equ 1425
ERROR_INVALID_HOOK_FILTER            equ 1426
ERROR_INVALID_FILTER_PROC            equ 1427
ERROR_HOOK_NEEDS_HMOD                equ 1428
ERROR_PUBLIC_ONLY_HOOK               equ 1429
ERROR_JOURNAL_HOOK_SET               equ 1430
ERROR_HOOK_NOT_INSTALLED             equ 1431
ERROR_INVALID_LB_MESSAGE             equ 1432
ERROR_SETCOUNT_ON_BAD_LB             equ 1433
ERROR_LB_WITHOUT_TABSTOPS            equ 1434
ERROR_DESTROY_OBJECT_OF_OTHER_THREAD equ 1435
ERROR_CHILD_WINDOW_MENU              equ 1436
ERROR_NO_SYSTEM_MENU                 equ 1437
ERROR_INVALID_MSGBOX_STYLE           equ 1438
ERROR_INVALID_SPI_VALUE              equ 1439
ERROR_SCREEN_ALREADY_LOCKED          equ 1440
ERROR_HWNDS_HAVE_DIFF_PARENT         equ 1441
ERROR_NOT_CHILD_WINDOW               equ 1442
ERROR_INVALID_GW_COMMAND             equ 1443
ERROR_INVALID_THREAD_ID              equ 1444
ERROR_NON_MDICHILD_WINDOW            equ 1445
ERROR_POPUP_ALREADY_ACTIVE           equ 1446
ERROR_NO_SCROLLBARS                  equ 1447
ERROR_INVALID_SCROLLBAR_RANGE        equ 1448
ERROR_INVALID_SHOWWIN_COMMAND        equ 1449
ERROR_EVENTLOG_FILE_CORRUPT          equ 1500
ERROR_EVENTLOG_CANT_START            equ 1501
ERROR_LOG_FILE_FULL                  equ 1502
ERROR_EVENTLOG_FILE_CHANGED          equ 1503
RPC_S_INVALID_STRING_BINDING         equ 1700
RPC_S_WRONG_KIND_OF_BINDING          equ 1701
RPC_S_INVALID_BINDING                equ 1702
RPC_S_PROTSEQ_NOT_SUPPORTED          equ 1703
RPC_S_INVALID_RPC_PROTSEQ            equ 1704
RPC_S_INVALID_STRING_UUID            equ 1705
RPC_S_INVALID_ENDPOINT_FORMAT        equ 1706
RPC_S_INVALID_NET_ADDR               equ 1707
RPC_S_NO_ENDPOINT_FOUND              equ 1708
RPC_S_INVALID_TIMEOUT                equ 1709
RPC_S_OBJECT_NOT_FOUND               equ 1710
RPC_S_ALREADY_REGISTERED             equ 1711
RPC_S_TYPE_ALREADY_REGISTERED        equ 1712
RPC_S_ALREADY_LISTENING              equ 1713
RPC_S_NO_PROTSEQS_REGISTERED         equ 1714
RPC_S_NOT_LISTENING                  equ 1715
RPC_S_UNKNOWN_MGR_TYPE               equ 1716
RPC_S_UNKNOWN_IF                     equ 1717
RPC_S_NO_BINDINGS                    equ 1718
RPC_S_NO_PROTSEQS                    equ 1719
RPC_S_CANT_CREATE_ENDPOINT           equ 1720
RPC_S_OUT_OF_RESOURCES               equ 1721
RPC_S_SERVER_UNAVAILABLE             equ 1722
RPC_S_SERVER_TOO_BUSY                equ 1723
RPC_S_INVALID_NETWORK_OPTIONS        equ 1724
RPC_S_NO_CALL_ACTIVE                 equ 1725
RPC_S_CALL_FAILED                    equ 1726
RPC_S_CALL_FAILED_DNE                equ 1727
RPC_S_PROTOCOL_ERROR                 equ 1728
RPC_S_UNSUPPORTED_TRANS_SYN          equ 1730
RPC_S_UNSUPPORTED_TYPE               equ 1732
RPC_S_INVALID_TAG                    equ 1733
RPC_S_INVALID_BOUND                  equ 1734
RPC_S_NO_ENTRY_NAME                  equ 1735
RPC_S_INVALID_NAME_SYNTAX            equ 1736
RPC_S_UNSUPPORTED_NAME_SYNTAX        equ 1737
RPC_S_UUID_NO_ADDRESS                equ 1739
RPC_S_DUPLICATE_ENDPOINT             equ 1740
RPC_S_UNKNOWN_AUTHN_TYPE             equ 1741
RPC_S_MAX_CALLS_TOO_SMALL            equ 1742
RPC_S_STRING_TOO_LONG                equ 1743
RPC_S_PROTSEQ_NOT_FOUND              equ 1744
RPC_S_PROCNUM_OUT_OF_RANGE           equ 1745
RPC_S_BINDING_HAS_NO_AUTH            equ 1746
RPC_S_UNKNOWN_AUTHN_SERVICE          equ 1747

RPC_S_UNKNOWN_AUTHN_LEVEL            equ 1748
RPC_S_INVALID_AUTH_IDENTITY          equ 1749
RPC_S_UNKNOWN_AUTHZ_SERVICE          equ 1750
EPT_S_INVALID_ENTRY                  equ 1751
EPT_S_CANT_PERFORM_OP                equ 1752
EPT_S_NOT_REGISTERED                 equ 1753
RPC_S_NOTHING_TO_EXPORT              equ 1754
RPC_S_INCOMPLETE_NAME                equ 1755
RPC_S_INVALID_VERS_OPTION            equ 1756
RPC_S_NO_MORE_MEMBERS                equ 1757
RPC_S_NOT_ALL_OBJS_UNEXPORTED        equ 1758
RPC_S_INTERFACE_NOT_FOUND            equ 1759
RPC_S_ENTRY_ALREADY_EXISTS           equ 1760
RPC_S_ENTRY_NOT_FOUND                equ 1761
RPC_S_NAME_SERVICE_UNAVAILABLE       equ 1762
RPC_S_INVALID_NAF_ID                 equ 1763
RPC_S_CANNOT_SUPPORT                 equ 1764
RPC_S_NO_CONTEXT_AVAILABLE           equ 1765
RPC_S_INTERNAL_ERROR                 equ 1766
RPC_S_ZERO_DIVIDE                    equ 1767
RPC_S_ADDRESS_ERROR                  equ 1768
RPC_S_FP_DIV_ZERO                    equ 1769
RPC_S_FP_UNDERFLOW                   equ 1770
RPC_S_FP_OVERFLOW                    equ 1771
RPC_X_NO_MORE_ENTRIES                equ 1772
RPC_X_SS_CHAR_TRANS_OPEN_FAIL        equ 1773
RPC_X_SS_CHAR_TRANS_SHORT_FILE       equ 1774
RPC_X_SS_IN_NULL_CONTEXT             equ 1775
RPC_X_SS_CONTEXT_DAMAGED             equ 1777
RPC_X_SS_HANDLES_MISMATCH            equ 1778
RPC_X_SS_CANNOT_GET_CALL_HANDLE      equ 1779
RPC_X_NULL_REF_POINTER               equ 1780
RPC_X_ENUM_VALUE_OUT_OF_RANGE        equ 1781
RPC_X_BYTE_COUNT_TOO_SMALL           equ 1782
RPC_X_BAD_STUB_DATA                  equ 1783
ERROR_INVALID_USER_BUFFER            equ 1784
ERROR_UNRECOGNIZED_MEDIA             equ 1785
ERROR_NO_TRUST_LSA_SECRET            equ 1786
ERROR_NO_TRUST_SAM_ACCOUNT           equ 1787
ERROR_TRUSTED_DOMAIN_FAILURE         equ 1788
ERROR_TRUSTED_RELATIONSHIP_FAILURE   equ 1789
ERROR_TRUST_FAILURE                  equ 1790
RPC_S_CALL_IN_PROGRESS               equ 1791
ERROR_NETLOGON_NOT_STARTED           equ 1792
ERROR_ACCOUNT_EXPIRED                equ 1793
ERROR_REDIRECTOR_HAS_OPEN_HANDLES    equ 1794
ERROR_PRINTER_DRIVER_ALREADY_INSTALLED               equ 1795
ERROR_UNKNOWN_PORT                   equ 1796
ERROR_UNKNOWN_PRINTER_DRIVER         equ 1797
ERROR_UNKNOWN_PRINTPROCESSOR         equ 1798
ERROR_INVALID_SEPARATOR_FILE         equ 1799
ERROR_INVALID_PRIORITY               equ 1800
ERROR_INVALID_PRINTER_NAME           equ 1801
ERROR_PRINTER_ALREADY_EXISTS         equ 1802
ERROR_INVALID_PRINTER_COMMAND        equ 1803
ERROR_INVALID_DATATYPE               equ 1804
ERROR_INVALID_ENVIRONMENT            equ 1805
RPC_S_NO_MORE_BINDINGS               equ 1806
ERROR_NOLOGON_INTERDOMAIN_TRUST_ACCOUNT              equ 1807
ERROR_NOLOGON_WORKSTATION_TRUST_ACCOUNT              equ 1808
ERROR_NOLOGON_SERVER_TRUST_ACCOUNT   equ 1809
ERROR_DOMAIN_TRUST_INCONSISTENT      equ 1810
ERROR_SERVER_HAS_OPEN_HANDLES        equ 1811
ERROR_RESOURCE_DATA_NOT_FOUND        equ 1812
ERROR_RESOURCE_TYPE_NOT_FOUND        equ 1813
ERROR_RESOURCE_NAME_NOT_FOUND        equ 1814
ERROR_RESOURCE_LANG_NOT_FOUND        equ 1815
ERROR_NOT_ENOUGH_QUOTA               equ 1816
RPC_S_GROUP_MEMBER_NOT_FOUND         equ 1898
EPT_S_CANT_CREATE                    equ 1899
RPC_S_INVALID_OBJECT                 equ 1900
ERROR_INVALID_TIME                   equ 1901
ERROR_INVALID_FORM_NAME              equ 1902
ERROR_INVALID_FORM_SIZE              equ 1903
ERROR_ALREADY_WAITING                equ 1904
ERROR_PRINTER_DELETED                equ 1905
ERROR_INVALID_PRINTER_STATE          equ 1906
ERROR_NO_BROWSER_SERVERS_FOUND       equ 6118
MAXPNAMELEN                          equ 32
MAXERRORLENGTH                       equ 128
TIME_MS                              equ 1h
TIME_SAMPLES                         equ 2h
TIME_BYTES                           equ 4h
TIME_SMPTE                           equ 8h
TIME_MIDI                            equ 10h
TIME_TICKS      				 equ 20h
MM_JOY1MOVE                          equ 3A0h
MM_JOY2MOVE                          equ 3A1h
MM_JOY1ZMOVE                         equ 3A2h
MM_JOY2ZMOVE                         equ 3A3h
MM_JOY1BUTTONDOWN                    equ 3B5h
MM_JOY2BUTTONDOWN                    equ 3B6h
MM_JOY1BUTTONUP                      equ 3B7h
MM_JOY2BUTTONUP                      equ 3B8h
MM_MCINOTIFY                         equ 3B9h
MM_MCISYSTEM_STRING                  equ 3CAh
MM_WOM_OPEN                          equ 3BBh
MM_WOM_CLOSE                         equ 3BCh
MM_WOM_DONE                          equ 3BDh
MM_WIM_OPEN                          equ 3BEh
MM_WIM_CLOSE                         equ 3BFh
MM_WIM_DATA                          equ 3C0h
MM_MIM_OPEN                          equ 3C1h
MM_MIM_CLOSE                         equ 3C2h
MM_MIM_DATA                          equ 3C3h
MM_MIM_LONGDATA                      equ 3C4h
MM_MIM_ERROR                         equ 3C5h
MM_MIM_LONGERROR                     equ 3C6h
MM_MOM_OPEN                          equ 3C7h
MM_MOM_CLOSE                         equ 3C8h
MM_MOM_DONE                          equ 3C9h
MMSYSERR_BASE                        equ 0
WAVERR_BASE                          equ 32
MIDIERR_BASE                         equ 64
TIMERR_BASE                          equ 96
JOYERR_BASE                          equ 160
MCIERR_BASE                          equ 256
MIXERR_BASE            			 equ 1024
MCI_STRING_OFFSET                    equ 512
MCI_VD_OFFSET                        equ 1024
MCI_CD_OFFSET                        equ 1088
MCI_WAVE_OFFSET                      equ 1152
MCI_SEQ_OFFSET                       equ 1216
MMSYSERR_NOERROR                     equ 0
MMSYSERR_ERROR                       equ MMSYSERR_BASE + 1
MMSYSERR_BADDEVICEID                 equ MMSYSERR_BASE + 2
MMSYSERR_NOTENABLED                  equ MMSYSERR_BASE + 3
MMSYSERR_ALLOCATED                   equ MMSYSERR_BASE + 4
MMSYSERR_INVALHANDLE                 equ MMSYSERR_BASE + 5
MMSYSERR_NODRIVER                    equ MMSYSERR_BASE + 6
MMSYSERR_NOMEM                       equ MMSYSERR_BASE + 7
MMSYSERR_NOTSUPPORTED                equ MMSYSERR_BASE + 8
MMSYSERR_BADERRNUM                   equ MMSYSERR_BASE + 9
MMSYSERR_INVALFLAG                   equ MMSYSERR_BASE + 10
MMSYSERR_INVALPARAM                  equ MMSYSERR_BASE + 11
MMSYSERR_HANDLEBUSY                  equ MMSYSERR_BASE + 12
MMSYSERR_INVALIDALIAS                equ MMSYSERR_BASE + 13
MMSYSERR_LASTERROR                   equ MMSYSERR_BASE + 13
MM_MOM_POSITIONCB                    equ 3CAh
MM_MCISIGNAL                         equ 3CBh
MM_MIM_MOREDATA                      equ 3CCh
MIDICAPS_STREAM                      equ 8h
MEVT_F_SHORT                         equ 0h
MEVT_F_LONG                          equ 80000000h
MEVT_F_CALLBACK                      equ 40000000h
MIDISTRM_ERROR                       equ -2
MIDIPROP_SET                         equ 80000000h
MIDIPROP_GET                         equ 40000000h
MIDIPROP_TIMEDIV                     equ 1h
MIDIPROP_TEMPO                       equ 2h
MIXER_SHORT_NAME_CHARS               equ 16
MIXER_LONG_NAME_CHARS                equ 64
MIXERR_BASE                          equ 1024
MIXERR_INVALLINE                     equ MIXERR_BASE + 0
MIXERR_INVALCONTROL                  equ MIXERR_BASE + 1
MIXERR_INVALVALUE                    equ MIXERR_BASE + 2
MIXERR_LASTERROR                     equ MIXERR_BASE + 2
MIXER_OBJECTF_HANDLE                 equ 80000000h
MIXER_OBJECTF_MIXER                  equ 0h
MIXER_OBJECTF_HMIXER                 equ MIXER_OBJECTF_HANDLE OR MIXER_OBJECTF_MIXER
MIXER_OBJECTF_WAVEOUT                equ 10000000h
MIXER_OBJECTF_HWAVEOUT               equ MIXER_OBJECTF_HANDLE OR MIXER_OBJECTF_WAVEOUT
MIXER_OBJECTF_WAVEIN                 equ 20000000h
MIXER_OBJECTF_HWAVEIN                equ MIXER_OBJECTF_HANDLE OR MIXER_OBJECTF_WAVEIN
MIXER_OBJECTF_MIDIOUT                equ 30000000h
MIXER_OBJECTF_HMIDIOUT               equ MIXER_OBJECTF_HANDLE OR MIXER_OBJECTF_MIDIOUT
MIXER_OBJECTF_MIDIIN                 equ 40000000h
MIXER_OBJECTF_HMIDIIN                equ MIXER_OBJECTF_HANDLE OR MIXER_OBJECTF_MIDIIN
MIXER_OBJECTF_AUX                    equ 50000000h
MIXERLINE_LINEF_ACTIVE               equ 1h
MIXERLINE_LINEF_DISCONNECTED         equ 8000h
MIXERLINE_LINEF_SOURCE               equ 80000000h
MIXERLINE_COMPONENTTYPE_DST_FIRST    equ 0h
MIXERLINE_COMPONENTTYPE_DST_UNDEFINED                equ MIXERLINE_COMPONENTTYPE_DST_FIRST + 0
MIXERLINE_COMPONENTTYPE_DST_DIGITAL  equ MIXERLINE_COMPONENTTYPE_DST_FIRST + 1
MIXERLINE_COMPONENTTYPE_DST_LINE     equ MIXERLINE_COMPONENTTYPE_DST_FIRST + 2
MIXERLINE_COMPONENTTYPE_DST_MONITOR  equ MIXERLINE_COMPONENTTYPE_DST_FIRST + 3
MIXERLINE_COMPONENTTYPE_DST_SPEAKERS equ MIXERLINE_COMPONENTTYPE_DST_FIRST + 4
MIXERLINE_COMPONENTTYPE_DST_HEADPHONES               equ MIXERLINE_COMPONENTTYPE_DST_FIRST + 5
MIXERLINE_COMPONENTTYPE_DST_TELEPHONE                equ MIXERLINE_COMPONENTTYPE_DST_FIRST + 6
MIXERLINE_COMPONENTTYPE_DST_WAVEIN   equ MIXERLINE_COMPONENTTYPE_DST_FIRST + 7
MIXERLINE_COMPONENTTYPE_DST_VOICEIN  equ MIXERLINE_COMPONENTTYPE_DST_FIRST + 8
MIXERLINE_COMPONENTTYPE_DST_LAST     equ MIXERLINE_COMPONENTTYPE_DST_FIRST + 8
MIXERLINE_COMPONENTTYPE_SRC_FIRST    equ 1000h
MIXERLINE_COMPONENTTYPE_SRC_UNDEFINED                equ MIXERLINE_COMPONENTTYPE_SRC_FIRST + 0
MIXERLINE_COMPONENTTYPE_SRC_DIGITAL  equ MIXERLINE_COMPONENTTYPE_SRC_FIRST + 1
MIXERLINE_COMPONENTTYPE_SRC_LINE     equ MIXERLINE_COMPONENTTYPE_SRC_FIRST + 2
MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE               equ MIXERLINE_COMPONENTTYPE_SRC_FIRST + 3
MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER              equ MIXERLINE_COMPONENTTYPE_SRC_FIRST + 4
MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC              equ MIXERLINE_COMPONENTTYPE_SRC_FIRST + 5
MIXERLINE_COMPONENTTYPE_SRC_TELEPHONE                equ MIXERLINE_COMPONENTTYPE_SRC_FIRST + 6
MIXERLINE_COMPONENTTYPE_SRC_PCSPEAKER                equ MIXERLINE_COMPONENTTYPE_SRC_FIRST + 7
MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT  equ MIXERLINE_COMPONENTTYPE_SRC_FIRST + 8
MIXERLINE_COMPONENTTYPE_SRC_AUXILIARY                equ MIXERLINE_COMPONENTTYPE_SRC_FIRST + 9
MIXERLINE_COMPONENTTYPE_SRC_ANALOG   equ MIXERLINE_COMPONENTTYPE_SRC_FIRST + 10
MIXERLINE_COMPONENTTYPE_SRC_LAST     equ MIXERLINE_COMPONENTTYPE_SRC_FIRST + 10
MIXERLINE_TARGETTYPE_UNDEFINED       equ 0
MIXERLINE_TARGETTYPE_WAVEOUT         equ 1
MIXERLINE_TARGETTYPE_WAVEIN          equ 2
MIXERLINE_TARGETTYPE_MIDIOUT         equ 3
MIXERLINE_TARGETTYPE_MIDIIN          equ 4
MIXERLINE_TARGETTYPE_AUX             equ 5
MIXER_GETLINEINFOF_DESTINATION       equ 0h
MIXER_GETLINEINFOF_SOURCE            equ 1h
MIXER_GETLINEINFOF_LINEID            equ 2h
MIXER_GETLINEINFOF_COMPONENTTYPE     equ 3h
MIXER_GETLINEINFOF_TARGETTYPE        equ 4h
MIXER_GETLINEINFOF_QUERYMASK         equ 0Fh
MIXERCONTROL_CONTROLF_UNIFORM        equ 1h
MIXERCONTROL_CONTROLF_MULTIPLE       equ 2h
MIXERCONTROL_CONTROLF_DISABLED       equ 80000000h
MIXERCONTROL_CT_CLASS_MASK           equ 0F0000000h
MIXERCONTROL_CT_CLASS_CUSTOM         equ 0h
MIXERCONTROL_CT_CLASS_METER          equ 10000000h
MIXERCONTROL_CT_CLASS_SWITCH         equ 20000000h
MIXERCONTROL_CT_CLASS_NUMBER         equ 30000000h
MIXERCONTROL_CT_CLASS_SLIDER         equ 40000000h
MIXERCONTROL_CT_CLASS_FADER          equ 50000000h
MIXERCONTROL_CT_CLASS_TIME           equ 60000000h
MIXERCONTROL_CT_CLASS_LIST           equ 70000000h
MIXERCONTROL_CT_SUBCLASS_MASK        equ 0F000000h
MIXERCONTROL_CT_SC_SWITCH_BOOLEAN    equ 0h
MIXERCONTROL_CT_SC_SWITCH_BUTTON     equ 1000000h
MIXERCONTROL_CT_SC_METER_POLLED      equ 0h
MIXERCONTROL_CT_SC_TIME_MICROSECS    equ 0h
MIXERCONTROL_CT_SC_TIME_MILLISECS    equ 1000000h
MIXERCONTROL_CT_SC_LIST_SINGLE       equ 0h
MIXERCONTROL_CT_SC_LIST_MULTIPLE     equ 1000000h
MIXERCONTROL_CT_UNITS_MASK           equ 0FF0000h
MIXERCONTROL_CT_UNITS_CUSTOM         equ 0h
MIXERCONTROL_CT_UNITS_BOOLEAN        equ 10000h
MIXERCONTROL_CT_UNITS_SIGNED         equ 20000h
MIXERCONTROL_CT_UNITS_UNSIGNED       equ 30000h
MIXERCONTROL_CT_UNITS_DECIBELS       equ 40000h
MIXERCONTROL_CT_UNITS_PERCENT        equ 50000h
MIXERCONTROL_CONTROLTYPE_CUSTOM      equ MIXERCONTROL_CT_CLASS_CUSTOM OR MIXERCONTROL_CT_UNITS_CUSTOM
MIXERCONTROL_CONTROLTYPE_BOOLEANMETER                equ MIXERCONTROL_CT_CLASS_METER OR MIXERCONTROL_CT_SC_METER_POLLED OR MIXERCONTROL_CT_UNITS_BOOLEAN
MIXERCONTROL_CONTROLTYPE_SIGNEDMETER equ MIXERCONTROL_CT_CLASS_METER OR MIXERCONTROL_CT_SC_METER_POLLED OR MIXERCONTROL_CT_UNITS_SIGNED
MIXERCONTROL_CONTROLTYPE_PEAKMETER   equ MIXERCONTROL_CONTROLTYPE_SIGNEDMETER + 1
MIXERCONTROL_CONTROLTYPE_UNSIGNEDMETER               equ MIXERCONTROL_CT_CLASS_METER OR MIXERCONTROL_CT_SC_METER_POLLED OR MIXERCONTROL_CT_UNITS_UNSIGNED
MIXERCONTROL_CONTROLTYPE_BOOLEAN     equ MIXERCONTROL_CT_CLASS_SWITCH OR MIXERCONTROL_CT_SC_SWITCH_BOOLEAN OR MIXERCONTROL_CT_UNITS_BOOLEAN
MIXERCONTROL_CONTROLTYPE_ONOFF       equ MIXERCONTROL_CONTROLTYPE_BOOLEAN + 1
MIXERCONTROL_CONTROLTYPE_MUTE        equ MIXERCONTROL_CONTROLTYPE_BOOLEAN + 2
MIXERCONTROL_CONTROLTYPE_MONO        equ MIXERCONTROL_CONTROLTYPE_BOOLEAN + 3
MIXERCONTROL_CONTROLTYPE_LOUDNESS    equ MIXERCONTROL_CONTROLTYPE_BOOLEAN + 4
MIXERCONTROL_CONTROLTYPE_STEREOENH   equ MIXERCONTROL_CONTROLTYPE_BOOLEAN + 5
MIXERCONTROL_CONTROLTYPE_BUTTON      equ MIXERCONTROL_CT_CLASS_SWITCH OR MIXERCONTROL_CT_SC_SWITCH_BUTTON OR MIXERCONTROL_CT_UNITS_BOOLEAN
MIXERCONTROL_CONTROLTYPE_DECIBELS    equ MIXERCONTROL_CT_CLASS_NUMBER OR MIXERCONTROL_CT_UNITS_DECIBELS
MIXERCONTROL_CONTROLTYPE_SIGNED      equ MIXERCONTROL_CT_CLASS_NUMBER OR MIXERCONTROL_CT_UNITS_SIGNED
MIXERCONTROL_CONTROLTYPE_UNSIGNED    equ MIXERCONTROL_CT_CLASS_NUMBER OR MIXERCONTROL_CT_UNITS_UNSIGNED
MIXERCONTROL_CONTROLTYPE_PERCENT     equ MIXERCONTROL_CT_CLASS_NUMBER OR MIXERCONTROL_CT_UNITS_PERCENT
MIXERCONTROL_CONTROLTYPE_SLIDER      equ MIXERCONTROL_CT_CLASS_SLIDER OR MIXERCONTROL_CT_UNITS_SIGNED
MIXERCONTROL_CONTROLTYPE_PAN         equ MIXERCONTROL_CONTROLTYPE_SLIDER + 1


MIXERCONTROL_CONTROLTYPE_QSOUNDPAN   equ MIXERCONTROL_CONTROLTYPE_SLIDER + 2
MIXERCONTROL_CONTROLTYPE_FADER       equ MIXERCONTROL_CT_CLASS_FADER OR MIXERCONTROL_CT_UNITS_UNSIGNED
MIXERCONTROL_CONTROLTYPE_VOLUME      equ MIXERCONTROL_CONTROLTYPE_FADER + 1
MIXERCONTROL_CONTROLTYPE_BASS        equ MIXERCONTROL_CONTROLTYPE_FADER + 2
MIXERCONTROL_CONTROLTYPE_TREBLE      equ MIXERCONTROL_CONTROLTYPE_FADER + 3
MIXERCONTROL_CONTROLTYPE_EQUALIZER   equ MIXERCONTROL_CONTROLTYPE_FADER + 4
MIXERCONTROL_CONTROLTYPE_SINGLESELECT                equ MIXERCONTROL_CT_CLASS_LIST OR MIXERCONTROL_CT_SC_LIST_SINGLE OR MIXERCONTROL_CT_UNITS_BOOLEAN
MIXERCONTROL_CONTROLTYPE_MUX         equ MIXERCONTROL_CONTROLTYPE_SINGLESELECT + 1
MIXERCONTROL_CONTROLTYPE_MULTIPLESELECT              equ MIXERCONTROL_CT_CLASS_LIST OR MIXERCONTROL_CT_SC_LIST_MULTIPLE OR MIXERCONTROL_CT_UNITS_BOOLEAN
MIXERCONTROL_CONTROLTYPE_MIXER       equ MIXERCONTROL_CONTROLTYPE_MULTIPLESELECT + 1
MIXERCONTROL_CONTROLTYPE_MICROTIME   equ MIXERCONTROL_CT_CLASS_TIME OR MIXERCONTROL_CT_SC_TIME_MICROSECS OR MIXERCONTROL_CT_UNITS_UNSIGNED
MIXERCONTROL_CONTROLTYPE_MILLITIME   equ MIXERCONTROL_CT_CLASS_TIME OR MIXERCONTROL_CT_SC_TIME_MILLISECS OR MIXERCONTROL_CT_UNITS_UNSIGNED
MIXER_GETLINECONTROLSF_ALL           equ 0h
MIXER_GETLINECONTROLSF_ONEBYID       equ 1h
MIXER_GETLINECONTROLSF_ONEBYTYPE     equ 2h
MIXER_GETLINECONTROLSF_QUERYMASK     equ 0Fh
MIXER_GETCONTROLDETAILSF_VALUE       equ 0h
MIXER_GETCONTROLDETAILSF_LISTTEXT    equ 1h
MIXER_GETCONTROLDETAILSF_QUERYMASK   equ 0Fh
MIXER_SETCONTROLDETAILSF_VALUE       equ 0h
MIXER_SETCONTROLDETAILSF_CUSTOM      equ 1h
MIXER_SETCONTROLDETAILSF_QUERYMASK   equ 0Fh
JOY_BUTTON5                          equ 10h
JOY_BUTTON6                          equ 20h
JOY_BUTTON7                          equ 40h
JOY_BUTTON8                          equ 80h
JOY_BUTTON9                          equ 100h
JOY_BUTTON10                         equ 200h
JOY_BUTTON11                         equ 400h
JOY_BUTTON12                         equ 800h
JOY_BUTTON13                         equ 1000h
JOY_BUTTON14                         equ 2000h
JOY_BUTTON15                         equ 4000h
JOY_BUTTON16                         equ 8000h
JOY_BUTTON17                         equ 10000h
JOY_BUTTON18                         equ 20000h
JOY_BUTTON19                         equ 40000h
JOY_BUTTON20                         equ 80000h
JOY_BUTTON21                         equ 100000h
JOY_BUTTON22                         equ 200000h
JOY_BUTTON23                         equ 400000h
JOY_BUTTON24                         equ 800000h
JOY_BUTTON25                         equ 1000000h
JOY_BUTTON26                         equ 2000000h
JOY_BUTTON27                         equ 4000000h
JOY_BUTTON28                         equ 8000000h
JOY_BUTTON29                         equ 10000000h
JOY_BUTTON30                         equ 20000000h
JOY_BUTTON31                         equ 40000000h
JOY_BUTTON32                         equ 80000000h
JOY_POVCENTERED                      equ -1
JOY_POVFORWARD                       equ 0
JOY_POVRIGHT                         equ 9000
JOY_POVBACKWARD                      equ 18000
JOY_POVLEFT                          equ 27000
JOY_RETURNX                          equ 1h
JOY_RETURNY                          equ 2h
JOY_RETURNZ                          equ 4h
JOY_RETURNR                          equ 8h
JOY_RETURNU                          equ 10h
JOY_RETURNV                          equ 20h
JOY_RETURNPOV                        equ 40h
JOY_RETURNBUTTONS                    equ 80h
JOY_RETURNRAWDATA                    equ 100h
JOY_RETURNPOVCTS                     equ 200h
JOY_RETURNCENTERED                   equ 400h
JOY_USEDEADZONE                      equ 800h
JOY_RETURNALL                        equ JOY_RETURNX OR JOY_RETURNY OR JOY_RETURNZ OR JOY_RETURNR OR JOY_RETURNU OR JOY_RETURNV OR JOY_RETURNPOV OR JOY_RETURNBUTTONS
JOY_CAL_READALWAYS                   equ 10000h
JOY_CAL_READXYONLY                   equ 20000h
JOY_CAL_READ3                        equ 40000h
JOY_CAL_READ4                        equ 80000h
JOY_CAL_READXONLY                    equ 100000h
JOY_CAL_READYONLY                    equ 200000h
JOY_CAL_READ5                        equ 400000h
JOY_CAL_READ6                        equ 800000h
JOY_CAL_READZONLY                    equ 1000000h
JOY_CAL_READRONLY                    equ 2000000h
JOY_CAL_READUONLY                    equ 4000000h
JOY_CAL_READVONLY                    equ 8000000h
WAVE_FORMAT_QUERY                    equ 1h
SND_PURGE                            equ 40h
SND_APPLICATION                      equ 80h
WAVE_MAPPED                          equ 4h
WAVE_FORMAT_DIRECT                   equ 8h
WAVE_FORMAT_DIRECT_QUERY             equ WAVE_FORMAT_QUERY OR WAVE_FORMAT_DIRECT
MIM_MOREDATA                         equ MM_MIM_MOREDATA
MOM_POSITIONCB                       equ MM_MOM_POSITIONCB
MIDI_IO_STATUS                       equ 20h
DRV_LOAD                             equ 1h
DRV_ENABLE                           equ 2h
DRV_OPEN                             equ 3h
DRV_CLOSE                            equ 4h
DRV_DISABLE                          equ 5h
DRV_FREE                             equ 6h
DRV_CONFIGURE                        equ 7h
DRV_QUERYCONFIGURE                   equ 8h
DRV_INSTALL                          equ 9h
DRV_REMOVE                           equ 0Ah
DRV_EXITSESSION                      equ 0Bh
DRV_POWER                            equ 0Fh
DRV_RESERVED                         equ 800h
DRV_USER                             equ 4000h
DRVCNF_CANCEL                        equ 0h
DRVCNF_OK                            equ 1h
DRVCNF_RESTART                       equ 2h
DRV_CANCEL                           equ DRVCNF_CANCEL
DRV_OK                               equ DRVCNF_OK
DRV_RESTART                          equ DRVCNF_RESTART
DRV_MCI_FIRST                        equ DRV_RESERVED
DRV_MCI_LAST                         equ DRV_RESERVED + FFFh
CALLBACK_TYPEMASK                    equ 70000h
CALLBACK_NULL                        equ 0h
CALLBACK_WINDOW                      equ 10000h
CALLBACK_TASK                        equ 20000h
CALLBACK_FUNCTION                    equ 30000h
MM_MICROSOFT                         equ 1
MM_MIDI_MAPPER                       equ 1
MM_WAVE_MAPPER                       equ 2
MM_SNDBLST_MIDIOUT                   equ 3
MM_SNDBLST_MIDIIN                    equ 4
MM_SNDBLST_SYNTH                     equ 5
MM_SNDBLST_WAVEOUT                   equ 6
MM_SNDBLST_WAVEIN                    equ 7
MM_ADLIB                             equ 9
MM_MPU401_MIDIOUT                    equ 10
MM_MPU401_MIDIIN                     equ 11
MM_PC_JOYSTICK                       equ 12
SND_SYNC                             equ 0h
SND_ASYNC                            equ 1h
SND_NODEFAULT                        equ 2h
SND_MEMORY                           equ 4h
SND_ALIAS                            equ 10000h
SND_FILENAME                         equ 20000h
SND_RESOURCE                         equ 40004h
SND_ALIAS_ID                         equ 110000h
SND_ALIAS_START                      equ 0
SND_LOOP                             equ 8h
SND_NOSTOP                           equ 10h
SND_VALID                            equ 1Fh
SND_NOWAIT                           equ 2000h
SND_VALIDFLAGS                       equ 17201Fh
SND_RESERVED                         equ 0FF000000h
SND_TYPE_MASK                        equ 170007h
WAVERR_BADFORMAT                     equ WAVERR_BASE + 0
WAVERR_STILLPLAYING                  equ WAVERR_BASE + 1
WAVERR_UNPREPARED                    equ WAVERR_BASE + 2
WAVERR_SYNC                          equ WAVERR_BASE + 3
WAVERR_LASTERROR                     equ WAVERR_BASE + 3
WOM_OPEN                             equ MM_WOM_OPEN
WOM_CLOSE                            equ MM_WOM_CLOSE
WOM_DONE                             equ MM_WOM_DONE
WIM_OPEN                             equ MM_WIM_OPEN
WIM_CLOSE                            equ MM_WIM_CLOSE
WIM_DATA                             equ MM_WIM_DATA
WAVE_MAPPER                          equ -1
WAVE_ALLOWSYNC                       equ 2h
WAVE_VALID                           equ 3h
WHDR_DONE                            equ 1h
WHDR_PREPARED                        equ 2h
WHDR_BEGINLOOP                       equ 4h
WHDR_ENDLOOP                         equ 8h
WHDR_INQUEUE                         equ 10h
WHDR_VALID                           equ 1Fh
WAVECAPS_PITCH                       equ 1h
WAVECAPS_PLAYBACKRATE                equ 2h
WAVECAPS_VOLUME                      equ 4h
WAVECAPS_LRVOLUME                    equ 8h
WAVECAPS_SYNC                        equ 10h
WAVE_INVALIDFORMAT                   equ 0h
WAVE_FORMAT_1M08                     equ 1h
WAVE_FORMAT_1S08                     equ 2h
WAVE_FORMAT_1M16                     equ 4h
WAVE_FORMAT_1S16                     equ 8h
WAVE_FORMAT_2M08                     equ 10h
WAVE_FORMAT_2S08                     equ 20h
WAVE_FORMAT_2M16                     equ 40h
WAVE_FORMAT_2S16                     equ 80h
WAVE_FORMAT_4M08                     equ 100h
WAVE_FORMAT_4S08                     equ 200h
WAVE_FORMAT_4M16                     equ 400h
WAVE_FORMAT_4S16                     equ 800h
WAVE_FORMAT_PCM                      equ 1
MIDIERR_UNPREPARED                   equ MIDIERR_BASE + 0
MIDIERR_STILLPLAYING                 equ MIDIERR_BASE + 1
MIDIERR_NOMAP                        equ MIDIERR_BASE + 2
MIDIERR_NOTREADY                     equ MIDIERR_BASE + 3
MIDIERR_NODEVICE                     equ MIDIERR_BASE + 4
MIDIERR_INVALIDSETUP                 equ MIDIERR_BASE + 5
MIDIERR_LASTERROR                    equ MIDIERR_BASE + 5
MIM_OPEN                             equ MM_MIM_OPEN
MIM_CLOSE                            equ MM_MIM_CLOSE
MIM_DATA                             equ MM_MIM_DATA
MIM_LONGDATA                         equ MM_MIM_LONGDATA
MIM_ERROR                            equ MM_MIM_ERROR
MIM_LONGERROR                        equ MM_MIM_LONGERROR
MOM_OPEN                             equ MM_MOM_OPEN
MOM_CLOSE                            equ MM_MOM_CLOSE
MOM_DONE                             equ MM_MOM_DONE
MIDIMAPPER                           equ -1
MIDI_MAPPER                          equ -1
MIDI_CACHE_ALL                       equ 1
MIDI_CACHE_BESTFIT                   equ 2
MIDI_CACHE_QUERY                     equ 3
MIDI_UNCACHE                         equ 4
MIDI_CACHE_VALID                     equ MIDI_CACHE_ALL OR MIDI_CACHE_BESTFIT OR MIDI_CACHE_QUERY OR MIDI_UNCACHE
MOD_MIDIPORT                         equ 1
MOD_SYNTH                            equ 2
MOD_SQSYNTH                          equ 3
MOD_FMSYNTH                          equ 4
MOD_MAPPER                           equ 5
MIDICAPS_VOLUME                      equ 1
MIDICAPS_LRVOLUME                    equ 2
MIDICAPS_CACHE                       equ 4
MIDICAPS_STREAM          		 equ 8
MHDR_DONE                            equ 1
MHDR_PREPARED                        equ 2
MHDR_INQUEUE                         equ 4
MHDR_VALID                           equ 7
MHDR_ISSTRM     				 equ 8
AUX_MAPPER                           equ -1
AUXCAPS_CDAUDIO                      equ 1
AUXCAPS_AUXIN                        equ 2
AUXCAPS_VOLUME                       equ 1h
AUXCAPS_LRVOLUME                     equ 2h
TIMERR_NOERROR                       equ 0
TIMERR_NOCANDO                       equ TIMERR_BASE + 1
TIMERR_STRUCT                        equ TIMERR_BASE + 33
TIME_ONESHOT                         equ 0
TIME_PERIODIC                        equ 1
JOYERR_NOERROR                       equ 0
JOYERR_PARMS                         equ JOYERR_BASE + 5
JOYERR_NOCANDO                       equ JOYERR_BASE + 6
JOYERR_UNPLUGGED                     equ JOYERR_BASE + 7
JOY_BUTTON1                          equ 1h
JOY_BUTTON2                          equ 2h
JOY_BUTTON3                          equ 4h
JOY_BUTTON4                          equ 8h
JOY_BUTTON1CHG                       equ 100h
JOY_BUTTON2CHG                       equ 200h
JOY_BUTTON3CHG                       equ 400h
JOY_BUTTON4CHG                       equ 800h
JOYSTICKID1                          equ 0
JOYSTICKID2                          equ 1
MMIOERR_BASE                         equ 256
MMIOERR_FILENOTFOUND                 equ MMIOERR_BASE + 1
MMIOERR_OUTOFMEMORY                  equ MMIOERR_BASE + 2
MMIOERR_CANNOTOPEN                   equ MMIOERR_BASE + 3
MMIOERR_CANNOTCLOSE                  equ MMIOERR_BASE + 4
MMIOERR_CANNOTREAD                   equ MMIOERR_BASE + 5
MMIOERR_CANNOTWRITE                  equ MMIOERR_BASE + 6
MMIOERR_CANNOTSEEK                   equ MMIOERR_BASE + 7
MMIOERR_CANNOTEXPAND                 equ MMIOERR_BASE + 8
MMIOERR_CHUNKNOTFOUND                equ MMIOERR_BASE + 9
MMIOERR_UNBUFFERED                   equ MMIOERR_BASE + 10
MMIO_RWMODE                          equ 3h
MMIO_SHAREMODE                       equ 70h
MMIO_CREATE                          equ 1000h
MMIO_PARSE                           equ 100h
MMIO_DELETE                          equ 200h
MMIO_EXIST                           equ 4000h
MMIO_ALLOCBUF                        equ 10000h
MMIO_GETTEMP                         equ 20000h
MMIO_DIRTY                           equ 10000000h
MMIO_OPEN_VALID                      equ 3FFFFh
MMIO_READ                            equ 0h
MMIO_WRITE                           equ 1h
MMIO_READWRITE                       equ 2h
MMIO_COMPAT                          equ 0h
MMIO_EXCLUSIVE                       equ 10h
MMIO_DENYWRITE                       equ 20h
MMIO_DENYREAD                        equ 30h
MMIO_DENYNONE                        equ 40h
MMIO_FHOPEN                          equ 10h
MMIO_EMPTYBUF                        equ 10h
MMIO_TOUPPER                         equ 10h
MMIO_INSTALLPROC                     equ 10000h
MMIO_GLOBALPROC                      equ 10000000h
MMIO_UNICODEPROC                     equ 1000000h
MMIO_REMOVEPROC                      equ 20000h
MMIO_FINDPROC                        equ 40000h
MMIO_FINDCHUNK                       equ 10h
MMIO_FINDRIFF                        equ 20h
MMIO_FINDLIST                        equ 40h
MMIO_CREATERIFF                      equ 20h
MMIO_CREATELIST                      equ 40h
MMIO_VALIDPROC                       equ 11070000h
MMIOM_READ                           equ MMIO_READ
MMIOM_WRITE                          equ MMIO_WRITE
MMIOM_SEEK                           equ 2
MMIOM_OPEN                           equ 3
MMIOM_CLOSE                          equ 4
MMIOM_WRITEFLUSH                     equ 5
MMIOM_RENAME                         equ 6
MMIOM_USER                           equ 8000h
SEEK_SET                             equ 0
SEEK_CUR                             equ 1
SEEK_END                             equ 2
MMIO_DEFAULTBUFFER                   equ 8192
MCIERR_INVALID_DEVICE_ID             equ MCIERR_BASE + 1
MCIERR_UNRECOGNIZED_KEYWORD          equ MCIERR_BASE + 3
MCIERR_UNRECOGNIZED_COMMAND          equ MCIERR_BASE + 5
MCIERR_HARDWARE                      equ MCIERR_BASE + 6
MCIERR_INVALID_DEVICE_NAME           equ MCIERR_BASE + 7
MCIERR_OUT_OF_MEMORY                 equ MCIERR_BASE + 8
MCIERR_DEVICE_OPEN                   equ MCIERR_BASE + 9
MCIERR_CANNOT_LOAD_DRIVER            equ MCIERR_BASE + 10
MCIERR_MISSING_COMMAND_STRING        equ MCIERR_BASE + 11
MCIERR_PARAM_OVERFLOW                equ MCIERR_BASE + 12
MCIERR_MISSING_STRING_ARGUMENT       equ MCIERR_BASE + 13
MCIERR_BAD_INTEGER                   equ MCIERR_BASE + 14
MCIERR_PARSER_INTERNAL               equ MCIERR_BASE + 15
MCIERR_DRIVER_INTERNAL               equ MCIERR_BASE + 16
MCIERR_MISSING_PARAMETER             equ MCIERR_BASE + 17
MCIERR_UNSUPPORTED_FUNCTION          equ MCIERR_BASE + 18
MCIERR_FILE_NOT_FOUND                equ MCIERR_BASE + 19
MCIERR_DEVICE_NOT_READY              equ MCIERR_BASE + 20
MCIERR_INTERNAL                      equ MCIERR_BASE + 21
MCIERR_DRIVER                        equ MCIERR_BASE + 22
MCIERR_CANNOT_USE_ALL                equ MCIERR_BASE + 23
MCIERR_MULTIPLE                      equ MCIERR_BASE + 24
MCIERR_EXTENSION_NOT_FOUND           equ MCIERR_BASE + 25
MCIERR_OUTOFRANGE                    equ MCIERR_BASE + 26
MCIERR_FLAGS_NOT_COMPATIBLE          equ MCIERR_BASE + 28
MCIERR_FILE_NOT_SAVED                equ MCIERR_BASE + 30
MCIERR_DEVICE_TYPE_REQUIRED          equ MCIERR_BASE + 31
MCIERR_DEVICE_LOCKED                 equ MCIERR_BASE + 32
MCIERR_DUPLICATE_ALIAS               equ MCIERR_BASE + 33
MCIERR_BAD_CONSTANT                  equ MCIERR_BASE + 34
MCIERR_MUST_USE_SHAREABLE            equ MCIERR_BASE + 35
MCIERR_MISSING_DEVICE_NAME           equ MCIERR_BASE + 36
MCIERR_BAD_TIME_FORMAT               equ MCIERR_BASE + 37
MCIERR_NO_CLOSING_QUOTE              equ MCIERR_BASE + 38
MCIERR_DUPLICATE_FLAGS               equ MCIERR_BASE + 39
MCIERR_INVALID_FILE                  equ MCIERR_BASE + 40
MCIERR_NULL_PARAMETER_BLOCK          equ MCIERR_BASE + 41
MCIERR_UNNAMED_RESOURCE              equ MCIERR_BASE + 42
MCIERR_NEW_REQUIRES_ALIAS            equ MCIERR_BASE + 43
MCIERR_NOTIFY_ON_AUTO_OPEN           equ MCIERR_BASE + 44
MCIERR_NO_ELEMENT_ALLOWED            equ MCIERR_BASE + 45
MCIERR_NONAPPLICABLE_FUNCTION        equ MCIERR_BASE + 46
MCIERR_ILLEGAL_FOR_AUTO_OPEN         equ MCIERR_BASE + 47
MCIERR_FILENAME_REQUIRED             equ MCIERR_BASE + 48
MCIERR_EXTRA_CHARACTERS              equ MCIERR_BASE + 49
MCIERR_DEVICE_NOT_INSTALLED          equ MCIERR_BASE + 50
MCIERR_GET_CD                        equ MCIERR_BASE + 51
MCIERR_SET_CD                        equ MCIERR_BASE + 52
MCIERR_SET_DRIVE                     equ MCIERR_BASE + 53
MCIERR_DEVICE_LENGTH                 equ MCIERR_BASE + 54
MCIERR_DEVICE_ORD_LENGTH             equ MCIERR_BASE + 55
MCIERR_NO_INTEGER                    equ MCIERR_BASE + 56
MCIERR_WAVE_OUTPUTSINUSE             equ MCIERR_BASE + 64
MCIERR_WAVE_SETOUTPUTINUSE           equ MCIERR_BASE + 65
MCIERR_WAVE_INPUTSINUSE              equ MCIERR_BASE + 66
MCIERR_WAVE_SETINPUTINUSE            equ MCIERR_BASE + 67
MCIERR_WAVE_OUTPUTUNSPECIFIED        equ MCIERR_BASE + 68
MCIERR_WAVE_INPUTUNSPECIFIED         equ MCIERR_BASE + 69
MCIERR_WAVE_OUTPUTSUNSUITABLE        equ MCIERR_BASE + 70
MCIERR_WAVE_SETOUTPUTUNSUITABLE      equ MCIERR_BASE + 71
MCIERR_WAVE_INPUTSUNSUITABLE         equ MCIERR_BASE + 72
MCIERR_WAVE_SETINPUTUNSUITABLE       equ MCIERR_BASE + 73
MCIERR_SEQ_DIV_INCOMPATIBLE          equ MCIERR_BASE + 80
MCIERR_SEQ_PORT_INUSE                equ MCIERR_BASE + 81
MCIERR_SEQ_PORT_NONEXISTENT          equ MCIERR_BASE + 82
MCIERR_SEQ_PORT_MAPNODEVICE          equ MCIERR_BASE + 83
MCIERR_SEQ_PORT_MISCERROR            equ MCIERR_BASE + 84
MCIERR_SEQ_TIMER                     equ MCIERR_BASE + 85
MCIERR_SEQ_PORTUNSPECIFIED           equ MCIERR_BASE + 86
MCIERR_SEQ_NOMIDIPRESENT             equ MCIERR_BASE + 87
MCIERR_NO_WINDOW                     equ MCIERR_BASE + 90
MCIERR_CREATEWINDOW                  equ MCIERR_BASE + 91
MCIERR_FILE_READ                     equ MCIERR_BASE + 92
MCIERR_FILE_WRITE                    equ MCIERR_BASE + 93
MCIERR_CUSTOM_DRIVER_BASE            equ MCIERR_BASE + 256
MCI_FIRST                            equ 800h
MCI_OPEN                             equ 803h
MCI_CLOSE                            equ 804h
MCI_ESCAPE                           equ 805h
MCI_PLAY                             equ 806h
MCI_SEEK                             equ 807h
MCI_STOP                             equ 808h
MCI_PAUSE                            equ 809h
MCI_INFO                             equ 80Ah
MCI_GETDEVCAPS                       equ 80Bh
MCI_SPIN                             equ 80Ch
MCI_SET                              equ 80Dh
MCI_STEP                             equ 80Eh
MCI_RECORD                           equ 80Fh
MCI_SYSINFO                          equ 810h
MCI_BREAK                            equ 811h
MCI_SOUND                            equ 812h
MCI_SAVE                             equ 813h
MCI_STATUS                           equ 814h
MCI_CUE                              equ 830h
MCI_REALIZE                          equ 840h
MCI_WINDOW                           equ 841h
MCI_PUT                              equ 842h
MCI_WHERE                            equ 843h
MCI_FREEZE                           equ 844h
MCI_UNFREEZE                         equ 845h
MCI_LOAD                             equ 850h
MCI_CUT                              equ 851h
MCI_COPY                             equ 852h
MCI_PASTE                            equ 853h
MCI_UPDATE                           equ 854h
MCI_RESUME                           equ 855h
MCI_DELETE                           equ 856h
MCI_LAST                             equ 0FFFh
MCI_USER_MESSAGES                    equ 400h + MCI_FIRST
MCI_ALL_DEVICE_ID                    equ -1
MCI_DEVTYPE_VCR                      equ 513
MCI_DEVTYPE_VIDEODISC                equ 514
MCI_DEVTYPE_OVERLAY                  equ 515
MCI_DEVTYPE_CD_AUDIO                 equ 516
MCI_DEVTYPE_DAT                      equ 517
MCI_DEVTYPE_SCANNER                  equ 518
MCI_DEVTYPE_ANIMATION                equ 519
MCI_DEVTYPE_DIGITAL_VIDEO            equ 520
MCI_DEVTYPE_OTHER                    equ 521
MCI_DEVTYPE_WAVEFORM_AUDIO           equ 522
MCI_DEVTYPE_SEQUENCER                equ 523
MCI_DEVTYPE_FIRST                    equ MCI_DEVTYPE_VCR
MCI_DEVTYPE_LAST                     equ MCI_DEVTYPE_SEQUENCER
MCI_DEVTYPE_FIRST_USER               equ 1000h
MCI_MODE_NOT_READY                   equ MCI_STRING_OFFSET + 12
MCI_MODE_STOP                        equ MCI_STRING_OFFSET + 13
MCI_MODE_PLAY                        equ MCI_STRING_OFFSET + 14
MCI_MODE_RECORD                      equ MCI_STRING_OFFSET + 15
MCI_MODE_SEEK                        equ MCI_STRING_OFFSET + 16
MCI_MODE_PAUSE                       equ MCI_STRING_OFFSET + 17
MCI_MODE_OPEN                        equ MCI_STRING_OFFSET + 18
MCI_FORMAT_MILLISECONDS              equ 0
MCI_FORMAT_HMS                       equ 1
MCI_FORMAT_MSF                       equ 2
MCI_FORMAT_FRAMES                    equ 3
MCI_FORMAT_SMPTE_24                  equ 4
MCI_FORMAT_SMPTE_25                  equ 5
MCI_FORMAT_SMPTE_30                  equ 6
MCI_FORMAT_SMPTE_30DROP              equ 7
MCI_FORMAT_BYTES                     equ 8
MCI_FORMAT_SAMPLES                   equ 9
MCI_FORMAT_TMSF                      equ 10
MCI_NOTIFY_SUCCESSFUL                equ 1h
MCI_NOTIFY_SUPERSEDED                equ 2h
MCI_NOTIFY_ABORTED                   equ 4h
MCI_NOTIFY_FAILURE                   equ 8h
MCI_NOTIFY                           equ 1h
MCI_WAIT                             equ 2h
MCI_FROM                             equ 4h
MCI_TO                               equ 8h
MCI_TRACK                            equ 10h
MCI_OPEN_SHAREABLE                   equ 100h
MCI_OPEN_ELEMENT                     equ 200h
MCI_OPEN_ALIAS                       equ 400h
MCI_OPEN_ELEMENT_ID                  equ 800h
MCI_OPEN_TYPE_ID                     equ 1000h
MCI_OPEN_TYPE                        equ 2000h
MCI_SEEK_TO_START                    equ 100h
MCI_SEEK_TO_END                      equ 200h
MCI_STATUS_ITEM                      equ 100h
MCI_STATUS_START                     equ 200h
MCI_STATUS_LENGTH                    equ 1h
MCI_STATUS_POSITION                  equ 2h
MCI_STATUS_NUMBER_OF_TRACKS          equ 3h
MCI_STATUS_MODE                      equ 4h
MCI_STATUS_MEDIA_PRESENT             equ 5h
MCI_STATUS_TIME_FORMAT               equ 6h
MCI_STATUS_READY                     equ 7h
MCI_STATUS_CURRENT_TRACK             equ 8h
MCI_INFO_PRODUCT                     equ 100h
MCI_INFO_FILE                        equ 200h
MCI_GETDEVCAPS_ITEM                  equ 100h
MCI_GETDEVCAPS_CAN_RECORD            equ 1h
MCI_GETDEVCAPS_HAS_AUDIO             equ 2h
MCI_GETDEVCAPS_HAS_VIDEO             equ 3h
MCI_GETDEVCAPS_DEVICE_TYPE           equ 4h
MCI_GETDEVCAPS_USES_FILES            equ 5h
MCI_GETDEVCAPS_COMPOUND_DEVICE       equ 6h
MCI_GETDEVCAPS_CAN_EJECT             equ 7h
MCI_GETDEVCAPS_CAN_PLAY              equ 8h
MCI_GETDEVCAPS_CAN_SAVE              equ 9h
MCI_SYSINFO_QUANTITY                 equ 100h
MCI_SYSINFO_OPEN                     equ 200h
MCI_SYSINFO_NAME                     equ 400h
MCI_SYSINFO_INSTALLNAME              equ 800h
MCI_SET_DOOR_OPEN                    equ 100h
MCI_SET_DOOR_CLOSED                  equ 200h
MCI_SET_TIME_FORMAT                  equ 400h
MCI_SET_AUDIO                        equ 800h
MCI_SET_VIDEO                        equ 1000h
MCI_SET_ON                           equ 2000h
MCI_SET_OFF                          equ 4000h
MCI_SET_AUDIO_ALL                    equ 4001h
MCI_SET_AUDIO_LEFT                   equ 4002h
MCI_SET_AUDIO_RIGHT                  equ 4003h
MCI_BREAK_KEY                        equ 100h
MCI_BREAK_HWND                       equ 200h
MCI_BREAK_OFF                        equ 400h
MCI_RECORD_INSERT                    equ 100h
MCI_RECORD_OVERWRITE                 equ 200h
MCI_SOUND_NAME                       equ 100h
MCI_SAVE_FILE                        equ 100h
MCI_LOAD_FILE                        equ 100h
MCI_VD_MODE_PARK                     equ MCI_VD_OFFSET + 1
MCI_VD_MEDIA_CLV                     equ MCI_VD_OFFSET + 2
MCI_VD_MEDIA_CAV                     equ MCI_VD_OFFSET + 3
MCI_VD_MEDIA_OTHER                   equ MCI_VD_OFFSET + 4
MCI_VD_FORMAT_TRACK                  equ 4001h
MCI_VD_PLAY_REVERSE                  equ 10000h
MCI_VD_PLAY_FAST                     equ 20000h
MCI_VD_PLAY_SPEED                    equ 40000h
MCI_VD_PLAY_SCAN                     equ 80000h
MCI_VD_PLAY_SLOW                     equ 100000h
MCI_VD_SEEK_REVERSE                  equ 10000h
MCI_VD_STATUS_SPEED                  equ 4002h
MCI_VD_STATUS_FORWARD                equ 4003h
MCI_VD_STATUS_MEDIA_TYPE             equ 4004h
MCI_VD_STATUS_SIDE                   equ 4005h
MCI_VD_STATUS_DISC_SIZE              equ 4006h
MCI_VD_GETDEVCAPS_CLV                equ 10000h
MCI_VD_GETDEVCAPS_CAV                equ 20000h
MCI_VD_SPIN_UP                       equ 10000h
MCI_VD_SPIN_DOWN                     equ 20000h
MCI_VD_GETDEVCAPS_CAN_REVERSE        equ 4002h
MCI_VD_GETDEVCAPS_FAST_RATE          equ 4003h
MCI_VD_GETDEVCAPS_SLOW_RATE          equ 4004h
MCI_VD_GETDEVCAPS_NORMAL_RATE        equ 4005h
MCI_VD_STEP_FRAMES                   equ 10000h
MCI_VD_STEP_REVERSE                  equ 20000h
MCI_VD_ESCAPE_STRING                 equ 100h
MCI_WAVE_PCM                         equ MCI_WAVE_OFFSET + 0
MCI_WAVE_MAPPER                      equ MCI_WAVE_OFFSET + 1
MCI_WAVE_OPEN_BUFFER                 equ 10000h
MCI_WAVE_SET_FORMATTAG               equ 10000h
MCI_WAVE_SET_CHANNELS                equ 20000h
MCI_WAVE_SET_SAMPLESPERSEC           equ 40000h
MCI_WAVE_SET_AVGBYTESPERSEC          equ 80000h
MCI_WAVE_SET_BLOCKALIGN              equ 100000h
MCI_WAVE_SET_BITSPERSAMPLE           equ 200000h
MCI_WAVE_INPUT                       equ 400000h
MCI_WAVE_OUTPUT                      equ 800000h
MCI_WAVE_STATUS_FORMATTAG            equ 4001h
MCI_WAVE_STATUS_CHANNELS             equ 4002h
MCI_WAVE_STATUS_SAMPLESPERSEC        equ 4003h
MCI_WAVE_STATUS_AVGBYTESPERSEC       equ 4004h
MCI_WAVE_STATUS_BLOCKALIGN           equ 4005h
MCI_WAVE_STATUS_BITSPERSAMPLE        equ 4006h
MCI_WAVE_STATUS_LEVEL                equ 4007h
MCI_WAVE_SET_ANYINPUT                equ 4000000h
MCI_WAVE_SET_ANYOUTPUT               equ 8000000h
MCI_WAVE_GETDEVCAPS_INPUTS           equ 4001h
MCI_WAVE_GETDEVCAPS_OUTPUTS          equ 4002h
MCI_SEQ_DIV_PPQN                     equ 0 + MCI_SEQ_OFFSET
MCI_SEQ_DIV_SMPTE_24                 equ 1 + MCI_SEQ_OFFSET
MCI_SEQ_DIV_SMPTE_25                 equ 2 + MCI_SEQ_OFFSET
MCI_SEQ_DIV_SMPTE_30DROP             equ 3 + MCI_SEQ_OFFSET
MCI_SEQ_DIV_SMPTE_30                 equ 4 + MCI_SEQ_OFFSET
MCI_SEQ_FORMAT_SONGPTR               equ 4001h
MCI_SEQ_FILE                         equ 4002h
MCI_SEQ_MIDI                         equ 4003h
MCI_SEQ_SMPTE                        equ 4004h
MCI_SEQ_NONE                         equ 65533
MCI_SEQ_MAPPER                       equ 65535
MCI_SEQ_STATUS_TEMPO                 equ 4002h
MCI_SEQ_STATUS_PORT                  equ 4003h
MCI_SEQ_STATUS_SLAVE                 equ 4007h
MCI_SEQ_STATUS_MASTER                equ 4008h
MCI_SEQ_STATUS_OFFSET                equ 4009h
MCI_SEQ_STATUS_DIVTYPE               equ 400Ah
MCI_SEQ_SET_TEMPO                    equ 10000h
MCI_SEQ_SET_PORT                     equ 20000h
MCI_SEQ_SET_SLAVE                    equ 40000h
MCI_SEQ_SET_MASTER                   equ 80000h
MCI_SEQ_SET_OFFSET                   equ 1000000h
MCI_ANIM_OPEN_WS                     equ 10000h
MCI_ANIM_OPEN_PARENT                 equ 20000h
MCI_ANIM_OPEN_NOSTATIC               equ 40000h
MCI_ANIM_PLAY_SPEED                  equ 10000h
MCI_ANIM_PLAY_REVERSE                equ 20000h
MCI_ANIM_PLAY_FAST                   equ 40000h
MCI_ANIM_PLAY_SLOW                   equ 80000h
MCI_ANIM_PLAY_SCAN                   equ 100000h
MCI_ANIM_STEP_REVERSE                equ 10000h
MCI_ANIM_STEP_FRAMES                 equ 20000h
MCI_ANIM_STATUS_SPEED                equ 4001h
MCI_ANIM_STATUS_FORWARD              equ 4002h
MCI_ANIM_STATUS_HWND                 equ 4003h
MCI_ANIM_STATUS_HPAL                 equ 4004h
MCI_ANIM_STATUS_STRETCH              equ 4005h
MCI_ANIM_INFO_TEXT                   equ 10000h
MCI_ANIM_GETDEVCAPS_CAN_REVERSE      equ 4001h
MCI_ANIM_GETDEVCAPS_FAST_RATE        equ 4002h
MCI_ANIM_GETDEVCAPS_SLOW_RATE        equ 4003h
MCI_ANIM_GETDEVCAPS_NORMAL_RATE      equ 4004h
MCI_ANIM_GETDEVCAPS_PALETTES         equ 4006h
MCI_ANIM_GETDEVCAPS_CAN_STRETCH      equ 4007h
MCI_ANIM_GETDEVCAPS_MAX_WINDOWS      equ 4008h
MCI_ANIM_REALIZE_NORM                equ 10000h
MCI_ANIM_REALIZE_BKGD                equ 20000h
MCI_ANIM_WINDOW_HWND                 equ 10000h
MCI_ANIM_WINDOW_STATE                equ 40000h
MCI_ANIM_WINDOW_TEXT                 equ 80000h
MCI_ANIM_WINDOW_ENABLE_STRETCH       equ 100000h
MCI_ANIM_WINDOW_DISABLE_STRETCH      equ 200000h
MCI_ANIM_WINDOW_DEFAULT              equ 0h
MCI_ANIM_RECT                        equ 10000h
MCI_ANIM_PUT_SOURCE                  equ 20000h
MCI_ANIM_PUT_DESTINATION             equ 40000h
MCI_ANIM_WHERE_SOURCE                equ 20000h
MCI_ANIM_WHERE_DESTINATION           equ 40000h
MCI_ANIM_UPDATE_HDC                  equ 20000h
MCI_OVLY_OPEN_WS                     equ 10000h
MCI_OVLY_OPEN_PARENT                 equ 20000h
MCI_OVLY_STATUS_HWND                 equ 4001h
MCI_OVLY_STATUS_STRETCH              equ 4002h
MCI_OVLY_INFO_TEXT                   equ 10000h
MCI_OVLY_GETDEVCAPS_CAN_STRETCH      equ 4001h
MCI_OVLY_GETDEVCAPS_CAN_FREEZE       equ 4002h
MCI_OVLY_GETDEVCAPS_MAX_WINDOWS      equ 4003h
MCI_OVLY_WINDOW_HWND                 equ 10000h
MCI_OVLY_WINDOW_STATE                equ 40000h
MCI_OVLY_WINDOW_TEXT                 equ 80000h
MCI_OVLY_WINDOW_ENABLE_STRETCH       equ 100000h
MCI_OVLY_WINDOW_DISABLE_STRETCH      equ 200000h
MCI_OVLY_WINDOW_DEFAULT              equ 0h
MCI_OVLY_RECT                        equ 10000h
MCI_OVLY_PUT_SOURCE                  equ 20000h
MCI_OVLY_PUT_DESTINATION             equ 40000h
MCI_OVLY_PUT_FRAME                   equ 80000h
MCI_OVLY_PUT_VIDEO                   equ 100000h
MCI_OVLY_WHERE_SOURCE                equ 20000h
MCI_OVLY_WHERE_DESTINATION           equ 40000h
MCI_OVLY_WHERE_FRAME                 equ 80000h
MCI_OVLY_WHERE_VIDEO                 equ 100000h
CAPS1                                equ 94
C1_TRANSPARENT                       equ 1h
NEWTRANSPARENT                       equ 3
QUERYROPSUPPORT                      equ 40
SELECTDIB                            equ 41
SE_ERR_SHARE                         equ 26
SE_ERR_ASSOCINCOMPLETE               equ 27
SE_ERR_DDETIMEOUT                    equ 28
SE_ERR_DDEFAIL                       equ 29
SE_ERR_DDEBUSY                       equ 30
SE_ERR_NOASSOC                       equ 31
PRINTER_CONTROL_PAUSE                equ 1
PRINTER_CONTROL_RESUME               equ 2
PRINTER_CONTROL_PURGE                equ 3
PRINTER_STATUS_PAUSED                equ 1h
PRINTER_STATUS_ERROR                 equ 2h
PRINTER_STATUS_PENDING_DELETION      equ 4h
PRINTER_STATUS_PAPER_JAM             equ 8h
PRINTER_STATUS_PAPER_OUT             equ 10h
PRINTER_STATUS_MANUAL_FEED           equ 20h
PRINTER_STATUS_PAPER_PROBLEM         equ 40h
PRINTER_STATUS_OFFLINE               equ 80h
PRINTER_STATUS_IO_ACTIVE             equ 100h
PRINTER_STATUS_BUSY                  equ 200h
PRINTER_STATUS_PRINTING              equ 400h
PRINTER_STATUS_OUTPUT_BIN_FULL       equ 800h
PRINTER_STATUS_NOT_AVAILABLE         equ 1000h
PRINTER_STATUS_WAITING               equ 2000h
PRINTER_STATUS_PROCESSING            equ 4000h
PRINTER_STATUS_INITIALIZING          equ 8000h
PRINTER_STATUS_WARMING_UP            equ 10000h
PRINTER_STATUS_TONER_LOW             equ 20000h
PRINTER_STATUS_NO_TONER              equ 40000h
PRINTER_STATUS_PAGE_PUNT             equ 80000h
PRINTER_STATUS_USER_INTERVENTION     equ 100000h
PRINTER_STATUS_OUT_OF_MEMORY         equ 200000h
PRINTER_STATUS_DOOR_OPEN             equ 400000h
PRINTER_ATTRIBUTE_QUEUED             equ 1h
PRINTER_ATTRIBUTE_DIRECT             equ 2h
PRINTER_ATTRIBUTE_DEFAULT            equ 4h
PRINTER_ATTRIBUTE_SHARED             equ 8h
PRINTER_ATTRIBUTE_NETWORK            equ 10h
PRINTER_ATTRIBUTE_HIDDEN             equ 20h
PRINTER_ATTRIBUTE_LOCAL              equ 40h
NO_PRIORITY                          equ 0
MAX_PRIORITY                         equ 99
MIN_PRIORITY                         equ 1
DEF_PRIORITY                         equ 1
JOB_CONTROL_PAUSE                    equ 1
JOB_CONTROL_RESUME                   equ 2
JOB_CONTROL_CANCEL                   equ 3
JOB_CONTROL_RESTART                  equ 4
JOB_STATUS_PAUSED                    equ 1h
JOB_STATUS_ERROR                     equ 2h
JOB_STATUS_DELETING                  equ 4h
JOB_STATUS_SPOOLING                  equ 8h
JOB_STATUS_PRINTING                  equ 10h
JOB_STATUS_OFFLINE                   equ 20h
JOB_STATUS_PAPEROUT                  equ 40h
JOB_STATUS_PRINTED                   equ 80h
JOB_POSITION_UNSPECIFIED             equ 0
FORM_BUILTIN                         equ 1h
PRINTER_CONTROL_SET_STATUS           equ 4
PRINTER_ATTRIBUTE_WORK_OFFLINE       equ 400h
PRINTER_ATTRIBUTE_ENABLE_BIDI        equ 800h
JOB_CONTROL_DELETE                   equ 5
JOB_STATUS_USER_INTERVENTION         equ 10000h
DI_CHANNEL                           equ 1
DI_READ_SPOOL_JOB                    equ 3
PORT_TYPE_WRITE                      equ 1h
PORT_TYPE_READ                       equ 2h
PORT_TYPE_REDIRECTED                 equ 4h
PORT_TYPE_NET_ATTACHED               equ 8h
PRINTER_ENUM_DEFAULT                 equ 1h
PRINTER_ENUM_LOCAL                   equ 2h
PRINTER_ENUM_CONNECTIONS             equ 4h
PRINTER_ENUM_FAVORITE                equ 4h
PRINTER_ENUM_NAME                    equ 8h
PRINTER_ENUM_REMOTE                  equ 10h
PRINTER_ENUM_SHARED                  equ 20h
PRINTER_ENUM_NETWORK                 equ 40h
PRINTER_ENUM_EXPAND                  equ 4000h
PRINTER_ENUM_CONTAINER               equ 8000h
PRINTER_ENUM_ICONMASK                equ 0FF0000h
PRINTER_ENUM_ICON1                   equ 10000h
PRINTER_ENUM_ICON2                   equ 20000h
PRINTER_ENUM_ICON3                   equ 40000h
PRINTER_ENUM_ICON4                   equ 80000h
PRINTER_ENUM_ICON5                   equ 100000h
PRINTER_ENUM_ICON6                   equ 200000h
PRINTER_ENUM_ICON7                   equ 400000h
PRINTER_ENUM_ICON8                   equ 800000h
PRINTER_CHANGE_ADD_PRINTER           equ 1h
PRINTER_CHANGE_SET_PRINTER           equ 2h
PRINTER_CHANGE_DELETE_PRINTER        equ 4h
PRINTER_CHANGE_PRINTER               equ 0FFh
PRINTER_CHANGE_ADD_JOB               equ 100h
PRINTER_CHANGE_SET_JOB               equ 200h
PRINTER_CHANGE_DELETE_JOB            equ 400h
PRINTER_CHANGE_WRITE_JOB             equ 800h
PRINTER_CHANGE_JOB                   equ 0FF00h
PRINTER_CHANGE_ADD_FORM              equ 10000h
PRINTER_CHANGE_SET_FORM              equ 20000h
PRINTER_CHANGE_DELETE_FORM           equ 40000h
PRINTER_CHANGE_FORM                  equ 70000h
PRINTER_CHANGE_ADD_PORT              equ 100000h
PRINTER_CHANGE_CONFIGURE_PORT        equ 200000h
PRINTER_CHANGE_DELETE_PORT           equ 400000h
PRINTER_CHANGE_PORT                  equ 700000h
PRINTER_CHANGE_ADD_PRINT_PROCESSOR   equ 1000000h
PRINTER_CHANGE_DELETE_PRINT_PROCESSOR                equ 4000000h
PRINTER_CHANGE_PRINT_PROCESSOR       equ 7000000h
PRINTER_CHANGE_ADD_PRINTER_DRIVER    equ 10000000h
PRINTER_CHANGE_DELETE_PRINTER_DRIVER equ 40000000h
PRINTER_CHANGE_PRINTER_DRIVER        equ 70000000h
PRINTER_CHANGE_TIMEOUT               equ 80000000h
PRINTER_CHANGE_ALL                   equ 7777FFFFh
PRINTER_ERROR_INFORMATION            equ 80000000h
PRINTER_ERROR_WARNING                equ 40000000h
PRINTER_ERROR_SEVERE                 equ 20000000h
PRINTER_ERROR_OUTOFPAPER             equ 1h
PRINTER_ERROR_JAM                    equ 2h
PRINTER_ERROR_OUTOFTONER             equ 4h
SERVER_ACCESS_ADMINISTER             equ 1h
SERVER_ACCESS_ENUMERATE              equ 2h
PRINTER_ACCESS_ADMINISTER            equ 4h
PRINTER_ACCESS_USE                   equ 8h
JOB_ACCESS_ADMINISTER                equ 10h
SERVER_ALL_ACCESS                    equ STANDARD_RIGHTS_REQUIRED OR SERVER_ACCESS_ADMINISTER OR SERVER_ACCESS_ENUMERATE
SERVER_READ                          equ STANDARD_RIGHTS_READ OR SERVER_ACCESS_ENUMERATE
SERVER_WRITE                         equ STANDARD_RIGHTS_WRITE OR SERVER_ACCESS_ADMINISTER OR SERVER_ACCESS_ENUMERATE
SERVER_EXECUTE                       equ STANDARD_RIGHTS_EXECUTE OR SERVER_ACCESS_ENUMERATE
PRINTER_ALL_ACCESS                   equ STANDARD_RIGHTS_REQUIRED OR PRINTER_ACCESS_ADMINISTER OR PRINTER_ACCESS_USE
PRINTER_READ                         equ STANDARD_RIGHTS_READ OR PRINTER_ACCESS_USE
PRINTER_WRITE                        equ STANDARD_RIGHTS_WRITE OR PRINTER_ACCESS_USE
PRINTER_EXECUTE                      equ STANDARD_RIGHTS_EXECUTE OR PRINTER_ACCESS_USE
JOB_ALL_ACCESS                       equ STANDARD_RIGHTS_REQUIRED OR JOB_ACCESS_ADMINISTER
JOB_READ                             equ STANDARD_RIGHTS_READ OR JOB_ACCESS_ADMINISTER
JOB_WRITE                            equ STANDARD_RIGHTS_WRITE OR JOB_ACCESS_ADMINISTER
JOB_EXECUTE                          equ STANDARD_RIGHTS_EXECUTE OR JOB_ACCESS_ADMINISTER
RESOURCE_CONNECTED                   equ 1h
RESOURCE_PUBLICNET                   equ 2h
RESOURCE_GLOBALNET                   equ 2h
RESOURCE_REMEMBERED                  equ 3h
RESOURCE_RECENT                      equ 4h
RESOURCE_CONTEXT                     equ 5h
RESOURCETYPE_ANY                     equ 0h
RESOURCETYPE_DISK                    equ 1h
RESOURCETYPE_PRINT                   equ 2h
RESOURCETYPE_UNKNOWN                 equ 0FFFFh
RESOURCEUSAGE_CONNECTABLE            equ 1h
RESOURCEUSAGE_CONTAINER              equ 2h
RESOURCEUSAGE_RESERVED               equ 80000000h
RESOURCEDISPLAYTYPE_GENERIC          equ 0h
RESOURCEDISPLAYTYPE_DOMAIN           equ 1h
RESOURCEDISPLAYTYPE_SERVER           equ 2h
RESOURCEDISPLAYTYPE_SHARE            equ 3h
RESOURCEDISPLAYTYPE_FILE             equ 4h
RESOURCEDISPLAYTYPE_GROUP            equ 5h
CONNECT_UPDATE_PROFILE               equ 1h
WN_SUCCESS                           equ NO_ERROR
WN_NOT_SUPPORTED                     equ ERROR_NOT_SUPPORTED
WN_NET_ERROR                         equ ERROR_UNEXP_NET_ERR
WN_MORE_DATA                         equ ERROR_MORE_DATA
WN_BAD_POINTER                       equ ERROR_INVALID_ADDRESS
WN_BAD_VALUE                         equ ERROR_INVALID_PARAMETER
WN_BAD_PASSWORD                      equ ERROR_INVALID_PASSWORD
WN_ACCESS_DENIED                     equ ERROR_ACCESS_DENIED
WN_FUNCTION_BUSY                     equ ERROR_BUSY
WN_WINDOWS_ERROR                     equ ERROR_UNEXP_NET_ERR
WN_BAD_USER                          equ ERROR_BAD_USERNAME
WN_OUT_OF_MEMORY                     equ ERROR_NOT_ENOUGH_MEMORY
WN_NO_NETWORK                        equ ERROR_NO_NETWORK
WN_EXTENDED_ERROR                    equ ERROR_EXTENDED_ERROR
WN_NOT_CONNECTED                     equ ERROR_NOT_CONNECTED
WN_OPEN_FILES                        equ ERROR_OPEN_FILES
WN_DEVICE_IN_USE                     equ ERROR_DEVICE_IN_USE
WN_BAD_NETNAME                       equ ERROR_BAD_NET_NAME
WN_BAD_LOCALNAME                     equ ERROR_BAD_DEVICE
WN_ALREADY_CONNECTED                 equ ERROR_ALREADY_ASSIGNED
WN_DEVICE_ERROR                      equ ERROR_GEN_FAILURE
WN_CONNECTION_CLOSED                 equ ERROR_CONNECTION_UNAVAIL
WN_NO_NET_OR_BAD_PATH                equ ERROR_NO_NET_OR_BAD_PATH
WN_BAD_PROVIDER                      equ ERROR_BAD_PROVIDER
WN_CANNOT_OPEN_PROFILE               equ ERROR_CANNOT_OPEN_PROFILE
WN_BAD_PROFILE                       equ ERROR_BAD_PROFILE
WN_BAD_HANDLE                        equ ERROR_INVALID_HANDLE
WN_NO_MORE_ENTRIES                   equ ERROR_NO_MORE_ITEMS
WN_NOT_CONTAINER                     equ ERROR_NOT_CONTAINER
WN_NO_ERROR                          equ NO_ERROR
NCBNAMSZ                             equ 16
MAX_LANA                             equ 254
NAME_FLAGS_MASK                      equ 87h
GROUP_NAME                           equ 80h
UNIQUE_NAME                          equ 0h
REGISTERING                          equ 0h
REGISTERED                           equ 4h
DEREGISTERED                         equ 5h
DUPLICATE                            equ 6h
DUPLICATE_DEREG                      equ 7h
LISTEN_OUTSTANDING                   equ 1h
CALL_PENDING                         equ 2h
SESSION_ESTABLISHED                  equ 3h
HANGUP_PENDING                       equ 4h
HANGUP_COMPLETE                      equ 5h
SESSION_ABORTED                      equ 6h
NCBCALL                              equ 10h
NCBLISTEN                            equ 11h
NCBHANGUP                            equ 12h
NCBSEND                              equ 14h
NCBRECV                              equ 15h
NCBRECVANY                           equ 16h
NCBCHAINSEND                         equ 17h
NCBDGSEND                            equ 20h
NCBDGRECV                            equ 21h
NCBDGSENDBC                          equ 22h
NCBDGRECVBC                          equ 23h
NCBADDNAME                           equ 30h
NCBDELNAME                           equ 31h
NCBRESET                             equ 32h
NCBASTAT                             equ 33h
NCBSSTAT                             equ 34h
NCBCANCEL                            equ 35h
NCBADDGRNAME                         equ 36h
NCBENUM                              equ 37h
NCBUNLINK                            equ 70h
NCBSENDNA                            equ 71h
NCBCHAINSENDNA                       equ 72h
NCBLANSTALERT                        equ 73h
NCBACTION                            equ 77h
NCBFINDNAME                          equ 78h
NCBTRACE                             equ 79h
ASYNCH                               equ 80h
NRC_GOODRET                          equ 0h
NRC_BUFLEN                           equ 1h
NRC_ILLCMD                           equ 3h
NRC_CMDTMO                           equ 5h
NRC_INCOMP                           equ 6h
NRC_BADDR                            equ 7h
NRC_SNUMOUT                          equ 8h
NRC_NORES                            equ 9h
NRC_SCLOSED                          equ 0Ah
NRC_CMDCAN                           equ 0Bh
NRC_DUPNAME                          equ 0Dh
NRC_NAMTFUL                          equ 0Eh
NRC_ACTSES                           equ 0Fh
NRC_LOCTFUL                          equ 11h
NRC_REMTFUL                          equ 12h
NRC_ILLNN                            equ 13h
NRC_NOCALL                           equ 14h
NRC_NOWILD                           equ 15h
NRC_INUSE                            equ 16h
NRC_NAMERR                           equ 17h
NRC_SABORT                           equ 18h
NRC_NAMCONF                          equ 19h
NRC_IFBUSY                           equ 21h
NRC_TOOMANY                          equ 22h
NRC_BRIDGE                           equ 23h
NRC_CANOCCR                          equ 24h
NRC_CANCEL                           equ 26h
NRC_DUPENV                           equ 30h
NRC_ENVNOTDEF                        equ 34h
NRC_OSRESNOTAV                       equ 35h
NRC_MAXAPPS                          equ 36h
NRC_NOSAPS                           equ 37h
NRC_NORESOURCES                      equ 38h
NRC_INVADDRESS                       equ 39h
NRC_INVDDID                          equ 3Bh
NRC_LOCKFAIL                         equ 3Ch
NRC_OPENERR                          equ 3Fh
NRC_SYSTEM                           equ 40h
NRC_PENDING                          equ 0FFh
EXCEPTION_EXECUTE_HANDLER            equ 1
EXCEPTION_CONTINUE_SEARCH            equ 0
EXCEPTION_CONTINUE_EXECUTION         equ -1
ctlFirst                             equ 400h
ctlLast                              equ 4FFh
psh1                                 equ 400h
psh2                                 equ 401h
psh3                                 equ 402h
psh4                                 equ 403h
psh5                                 equ 404h
psh6                                 equ 405h
psh7                                 equ 406h
psh8                                 equ 407h
psh9                                 equ 408h
psh10                                equ 409h
psh11                                equ 40Ah
psh12                                equ 40Bh
psh13                                equ 40Ch
psh14                                equ 40Dh
psh15                                equ 40Eh
pshHelp                              equ psh15
psh16                                equ 40Fh
chx1                                 equ 410h
chx2                                 equ 411h
chx3                                 equ 412h
chx4                                 equ 413h
chx5                                 equ 414h
chx6                                 equ 415h
chx7                                 equ 416h
chx8                                 equ 417h
chx9                                 equ 418h
chx10                                equ 419h
chx11                                equ 41Ah
chx12                                equ 41Bh
chx13                                equ 41Ch
chx14                                equ 41Dh
chx15                                equ 41Eh
chx16                                equ 41Dh
rad1                                 equ 420h
rad2                                 equ 421h
rad3                                 equ 422h
rad4                                 equ 423h
rad5                                 equ 424h
rad6                                 equ 425h
rad7                                 equ 426h
rad8                                 equ 427h
rad9                                 equ 428h
rad10                                equ 429h
rad11                                equ 42Ah
rad12                                equ 42Bh
rad13                                equ 42Ch
rad14                                equ 42Dh
rad15                                equ 42Eh
rad16                                equ 42Fh
grp1                                 equ 430h
grp2                                 equ 431h
grp3                                 equ 432h
grp4                                 equ 433h
frm1                                 equ 434h
frm2                                 equ 435h
frm3                                 equ 436h
frm4                                 equ 437h
rct1                                 equ 438h
rct2                                 equ 439h
rct3                                 equ 43Ah
rct4                                 equ 43Bh
ico1                                 equ 43Ch
ico2                                 equ 43Dh
ico3                                 equ 43Eh
ico4                                 equ 43Fh
stc1                                 equ 440h
stc2                                 equ 441h
stc3                                 equ 442h
stc4                                 equ 443h
stc5                                 equ 444h
stc6                                 equ 445h
stc7                                 equ 446h
stc8                                 equ 447h
stc9                                 equ 448h
stc10                                equ 449h
stc11                                equ 44Ah
stc12                                equ 44Bh
stc13                                equ 44Ch
stc14                                equ 44Dh
stc15                                equ 44Eh
stc16                                equ 44Fh
stc17                                equ 450h
stc18                                equ 451h
stc19                                equ 452h
stc20                                equ 453h
stc21                                equ 454h
stc22                                equ 455h
stc23                                equ 456h
stc24                                equ 457h
stc25                                equ 458h
stc26                                equ 459h
stc27                                equ 45Ah
stc28                                equ 45Bh
stc29                                equ 45Ch
stc30                                equ 45Dh
stc31                                equ 45Eh
stc32                                equ 45Fh
lst1                                 equ 460h
lst2                                 equ 461h
lst3                                 equ 462h
lst4                                 equ 463h
lst5                                 equ 464h
lst6                                 equ 465h
lst7                                 equ 466h
lst8                                 equ 467h
lst9                                 equ 468h
lst10                                equ 469h
lst11                                equ 46Ah
lst12                                equ 46Bh
lst13                                equ 46Ch
lst14                                equ 46Dh
lst15                                equ 46Eh
lst16                                equ 46Fh
cmb1                                 equ 470h
cmb2                                 equ 471h
cmb3                                 equ 472h
cmb4                                 equ 473h
cmb5                                 equ 474h
cmb6                                 equ 475h
cmb7                                 equ 476h
cmb8                                 equ 477h
cmb9                                 equ 478h
cmb10                                equ 479h
cmb11                                equ 47Ah
cmb12                                equ 47Bh
cmb13                                equ 47Ch
cmb14                                equ 47Dh
cmb15                                equ 47Eh
cmb16                                equ 47Fh
edt1                                 equ 480h
edt2                                 equ 481h
edt3                                 equ 482h
edt4                                 equ 483h
edt5                                 equ 484h
edt6                                 equ 485h
edt7                                 equ 486h
edt8                                 equ 487h
edt9                                 equ 488h
edt10                                equ 489h
edt11                                equ 48Ah
edt12                                equ 48Bh
edt13                                equ 48Ch
edt14                                equ 48Dh
edt15                                equ 48Eh
edt16                                equ 48Fh
scr1                                 equ 490h
scr2                                 equ 491h
scr3                                 equ 492h
scr4                                 equ 493h
scr5                                 equ 494h
scr6                                 equ 495h
scr7                                 equ 496h
scr8                                 equ 497h
FILEOPENORD                          equ 1536
MULTIFILEOPENORD                     equ 1537
PRINTDLGORD                          equ 1538
PRNSETUPDLGORD                       equ 1539
FINDDLGORD                           equ 1540
REPLACEDLGORD                        equ 1541
FONTDLGORD                           equ 1542
FORMATDLGORD31                       equ 1543
FORMATDLGORD30                       equ 1544
HKEY_CLASSES_ROOT                    equ 80000000h
HKEY_CURRENT_USER                    equ 80000001h
HKEY_LOCAL_MACHINE                   equ 80000002h
HKEY_USERS                           equ 80000003h
HKEY_PERFORMANCE_DATA                equ 80000004h
HKEY_CURRENT_CONFIG                  equ 80000005h
HKEY_DYN_DATA                        equ 80000006h
SERVICE_NO_CHANGE                    equ 0FFFFFFFFh
SERVICE_ACTIVE                       equ 1h
SERVICE_INACTIVE                     equ 2h
SERVICE_STATE_ALL                    equ SERVICE_ACTIVE OR SERVICE_INACTIVE
SERVICE_CONTROL_STOP                 equ 1h
SERVICE_CONTROL_PAUSE                equ 2h
SERVICE_CONTROL_CONTINUE             equ 3h
SERVICE_CONTROL_INTERROGATE          equ 4h
SERVICE_CONTROL_SHUTDOWN             equ 5h
SERVICE_STOPPED                      equ 1h
SERVICE_START_PENDING                equ 2h
SERVICE_STOP_PENDING                 equ 3h
SERVICE_RUNNING                      equ 4h
SERVICE_CONTINUE_PENDING             equ 5h
SERVICE_PAUSE_PENDING                equ 6h
SERVICE_PAUSED                       equ 7h
SERVICE_ACCEPT_STOP                  equ 1h
SERVICE_ACCEPT_PAUSE_CONTINUE        equ 2h
SERVICE_ACCEPT_SHUTDOWN              equ 4h
SC_MANAGER_CONNECT                   equ 1h
SC_MANAGER_CREATE_SERVICE            equ 2h
SC_MANAGER_ENUMERATE_SERVICE         equ 4h
SC_MANAGER_LOCK                      equ 8h
SC_MANAGER_QUERY_LOCK_STATUS         equ 10h
SC_MANAGER_MODIFY_BOOT_CONFIG        equ 20h
SC_MANAGER_ALL_ACCESS                equ STANDARD_RIGHTS_REQUIRED OR SC_MANAGER_CONNECT OR SC_MANAGER_CREATE_SERVICE OR SC_MANAGER_ENUMERATE_SERVICE OR SC_MANAGER_LOCK or SC_MANAGER_QUERY_LOCK_STATUS or SC_MANAGER_MODIFY_BOOT_CONFIG
SERVICE_QUERY_CONFIG                 equ 1h
SERVICE_CHANGE_CONFIG                equ 2h
SERVICE_QUERY_STATUS                 equ 4h
SERVICE_ENUMERATE_DEPENDENTS         equ 8h
SERVICE_START                        equ 10h
SERVICE_STOP                         equ 20h
SERVICE_PAUSE_CONTINUE               equ 40h
SERVICE_INTERROGATE                  equ 80h
SERVICE_USER_DEFINED_CONTROL         equ 100h
SERVICE_ALL_ACCESS                   equ STANDARD_RIGHTS_REQUIRED OR SERVICE_QUERY_CONFIG OR SERVICE_CHANGE_CONFIG OR SERVICE_QUERY_STATUS or SERVICE_ENUMERATE_DEPENDENTS or SERVICE_START or SERVICE_STOP or SERVICE_PAUSE_CONTINUE or SERVICE_INTERROGATE or SERVICE_USER_DEFINED_CONTROL
PERF_DATA_VERSION                    equ 1
PERF_DATA_REVISION                   equ 1
PERF_NO_INSTANCES                    equ -1
PERF_SIZE_DWORD                      equ 0h
PERF_SIZE_LARGE                      equ 100h
PERF_SIZE_ZERO                       equ 200h
PERF_SIZE_VARIABLE_LEN               equ 300h
PERF_TYPE_NUMBER                     equ 0h
PERF_TYPE_COUNTER                    equ 400h
PERF_TYPE_TEXT                       equ 800h
PERF_TYPE_ZERO                       equ 0C00h
PERF_NUMBER_HEX                      equ 0h
PERF_NUMBER_DECIMAL                  equ 10000h
PERF_NUMBER_DEC_1000                 equ 20000h
PERF_COUNTER_VALUE                   equ 0h
PERF_COUNTER_RATE                    equ 10000h
PERF_COUNTER_FRACTION                equ 20000h
PERF_COUNTER_BASE                    equ 30000h
PERF_COUNTER_ELAPSED                 equ 40000h
PERF_COUNTER_QUEUELEN                equ 50000h
PERF_COUNTER_HISTOGRAM               equ 60000h
PERF_TEXT_UNICODE                    equ 0h
PERF_TEXT_ASCII                      equ 10000h
PERF_TIMER_TICK                      equ 0h
PERF_TIMER_100NS                     equ 100000h
PERF_OBJECT_TIMER                    equ 200000h
PERF_DELTA_COUNTER                   equ 400000h
PERF_DELTA_BASE                      equ 800000h
PERF_INVERSE_COUNTER                 equ 1000000h
PERF_MULTI_COUNTER                   equ 2000000h
PERF_DISPLAY_NO_SUFFIX               equ 0h
PERF_DISPLAY_PER_SEC                 equ 10000000h
PERF_DISPLAY_PERCENT                 equ 20000000h
PERF_DISPLAY_SECONDS                 equ 30000000h
PERF_DISPLAY_NOSHOW                  equ 40000000h
PERF_COUNTER_COUNTER                 equ PERF_SIZE_DWORD OR PERF_TYPE_COUNTER OR PERF_COUNTER_RATE OR PERF_TIMER_TICK OR PERF_DELTA_COUNTER OR PERF_DISPLAY_PER_SEC
PERF_COUNTER_TIMER                   equ PERF_SIZE_LARGE OR PERF_TYPE_COUNTER OR PERF_COUNTER_RATE OR PERF_TIMER_TICK OR PERF_DELTA_COUNTER OR PERF_DISPLAY_PERCENT
PERF_COUNTER_QUEUELEN_TYPE           equ PERF_SIZE_DWORD OR PERF_TYPE_COUNTER OR PERF_COUNTER_QUEUELEN OR PERF_TIMER_TICK OR PERF_DELTA_COUNTER OR PERF_DISPLAY_NO_SUFFIX
PERF_COUNTER_BULK_COUNT              equ PERF_SIZE_LARGE OR PERF_TYPE_COUNTER OR PERF_COUNTER_RATE OR PERF_TIMER_TICK OR PERF_DELTA_COUNTER OR PERF_DISPLAY_PER_SEC
PERF_COUNTER_TEXT                    equ PERF_SIZE_VARIABLE_LEN OR PERF_TYPE_TEXT OR PERF_TEXT_UNICODE OR PERF_DISPLAY_NO_SUFFIX
PERF_COUNTER_RAWCOUNT                equ PERF_SIZE_DWORD OR PERF_TYPE_NUMBER OR PERF_NUMBER_DECIMAL OR PERF_DISPLAY_NO_SUFFIX
PERF_SAMPLE_FRACTION                 equ PERF_SIZE_DWORD OR PERF_TYPE_COUNTER OR PERF_COUNTER_FRACTION OR PERF_DELTA_COUNTER OR PERF_DELTA_BASE OR PERF_DISPLAY_PERCENT
PERF_SAMPLE_COUNTER                  equ PERF_SIZE_DWORD OR PERF_TYPE_COUNTER OR PERF_COUNTER_RATE OR PERF_TIMER_TICK OR PERF_DELTA_COUNTER OR PERF_DISPLAY_NO_SUFFIX
PERF_COUNTER_NODATA                  equ PERF_SIZE_ZERO OR PERF_DISPLAY_NOSHOW
PERF_COUNTER_TIMER_INV               equ PERF_SIZE_LARGE OR PERF_TYPE_COUNTER OR PERF_COUNTER_RATE OR PERF_TIMER_TICK OR PERF_DELTA_COUNTER OR PERF_INVERSE_COUNTER OR PERF_DISPLAY_PERCENT
PERF_SAMPLE_BASE                     equ PERF_SIZE_DWORD OR PERF_TYPE_COUNTER OR PERF_COUNTER_BASE OR PERF_DISPLAY_NOSHOW OR 1h
PERF_AVERAGE_TIMER                   equ PERF_SIZE_DWORD OR PERF_TYPE_COUNTER OR PERF_COUNTER_FRACTION OR PERF_DISPLAY_SECONDS
PERF_AVERAGE_BASE                    equ PERF_SIZE_DWORD OR PERF_TYPE_COUNTER OR PERF_COUNTER_BASE OR PERF_DISPLAY_NOSHOW OR 2h
PERF_AVERAGE_BULK                    equ PERF_SIZE_LARGE OR PERF_TYPE_COUNTER OR PERF_COUNTER_FRACTION OR PERF_DISPLAY_NOSHOW
PERF_100NSEC_TIMER                   equ PERF_SIZE_LARGE OR PERF_TYPE_COUNTER OR PERF_COUNTER_RATE OR PERF_TIMER_100NS OR PERF_DELTA_COUNTER OR PERF_DISPLAY_PERCENT
PERF_100NSEC_TIMER_INV               equ PERF_SIZE_LARGE OR PERF_TYPE_COUNTER OR PERF_COUNTER_RATE OR PERF_TIMER_100NS OR PERF_DELTA_COUNTER OR PERF_INVERSE_COUNTER OR PERF_DISPLAY_PERCENT
PERF_COUNTER_MULTI_TIMER             equ PERF_SIZE_LARGE OR PERF_TYPE_COUNTER OR PERF_COUNTER_RATE OR PERF_DELTA_COUNTER OR PERF_TIMER_TICK OR PERF_MULTI_COUNTER OR PERF_DISPLAY_PERCENT
PERF_COUNTER_MULTI_TIMER_INV         equ PERF_SIZE_LARGE OR PERF_TYPE_COUNTER OR PERF_COUNTER_RATE OR PERF_DELTA_COUNTER OR PERF_MULTI_COUNTER OR PERF_TIMER_TICK OR PERF_INVERSE_COUNTER OR PERF_DISPLAY_PERCENT
PERF_COUNTER_MULTI_BASE              equ PERF_SIZE_LARGE OR PERF_TYPE_COUNTER OR PERF_COUNTER_BASE OR PERF_MULTI_COUNTER OR PERF_DISPLAY_NOSHOW
PERF_100NSEC_MULTI_TIMER             equ PERF_SIZE_LARGE OR PERF_TYPE_COUNTER OR PERF_DELTA_COUNTER OR PERF_COUNTER_RATE OR PERF_TIMER_100NS OR PERF_MULTI_COUNTER OR PERF_DISPLAY_PERCENT
PERF_100NSEC_MULTI_TIMER_INV         equ PERF_SIZE_LARGE OR PERF_TYPE_COUNTER OR PERF_DELTA_COUNTER OR PERF_COUNTER_RATE OR PERF_TIMER_100NS OR PERF_MULTI_COUNTER OR PERF_INVERSE_COUNTER OR PERF_DISPLAY_PERCENT
PERF_RAW_FRACTION                    equ PERF_SIZE_DWORD OR PERF_TYPE_COUNTER OR PERF_COUNTER_FRACTION OR PERF_DISPLAY_PERCENT
PERF_RAW_BASE                        equ PERF_SIZE_DWORD OR PERF_TYPE_COUNTER OR PERF_COUNTER_BASE OR PERF_DISPLAY_NOSHOW OR 3h
PERF_ELAPSED_TIME                    equ PERF_SIZE_LARGE OR PERF_TYPE_COUNTER OR PERF_COUNTER_ELAPSED OR PERF_OBJECT_TIMER OR PERF_DISPLAY_SECONDS
PERF_COUNTER_HISTOGRAM_TYPE          equ 80000000h
PERF_DETAIL_NOVICE                   equ 100
PERF_DETAIL_ADVANCED                 equ 200
PERF_DETAIL_EXPERT                   equ 300
PERF_DETAIL_WIZARD                   equ 400
PERF_NO_UNIQUE_ID                    equ -1
LZERROR_BADINHANDLE                  equ -1
LZERROR_BADOUTHANDLE                 equ -2
LZERROR_READ                         equ -3
LZERROR_WRITE                        equ -4
LZERROR_PUBLICLOC                    equ -5
LZERROR_GLOBLOCK                     equ -6
LZERROR_BADVALUE                     equ -7
LZERROR_UNKNOWNALG                   equ -8
VK_PROCESSKEY                        equ 0E5h
STYLE_DESCRIPTION_SIZE               equ 32
WM_CONVERTREQUESTEX                  equ 108h

WM_IME_REQUEST                  	  equ 288h
WM_IME_STARTCOMPOSITION              equ 10Dh
WM_IME_ENDCOMPOSITION                equ 10Eh
WM_IME_COMPOSITION                   equ 10Fh
WM_IME_KEYLAST                       equ 10Fh
WM_IME_SETCONTEXT                    equ 281h
WM_IME_NOTIFY                        equ 282h
WM_IME_CONTROL                       equ 283h
WM_IME_COMPOSITIONFULL               equ 284h
WM_IME_SELECT                        equ 285h
WM_IME_CHAR                          equ 286h
WM_IME_KEYDOWN                       equ 290h
WM_IME_KEYUP                         equ 291h
IMC_GETCANDIDATEPOS                  equ 7h
IMC_SETCANDIDATEPOS                  equ 8h
IMC_GETCOMPOSITIONFONT               equ 9h
IMC_SETCOMPOSITIONFONT               equ 0Ah
IMC_GETCOMPOSITIONWINDOW             equ 0Bh
IMC_SETCOMPOSITIONWINDOW             equ 0Ch
IMC_GETSTATUSWINDOWPOS               equ 0Fh
IMC_SETSTATUSWINDOWPOS               equ 10h
IMC_CLOSESTATUSWINDOW                equ 21h
IMC_OPENSTATUSWINDOW                 equ 22h
NI_OPENCANDIDATE                     equ 10h
NI_CLOSECANDIDATE                    equ 11h
NI_SELECTCANDIDATESTR                equ 12h
NI_CHANGECANDIDATELIST               equ 13h
NI_FINALIZECONVERSIONRESULT          equ 14h
NI_COMPOSITIONSTR                    equ 15h
NI_SETCANDIDATE_PAGESTART            equ 16h
NI_SETCANDIDATE_PAGESIZE             equ 17h
ISC_SHOWUICANDIDATEWINDOW            equ 1h
ISC_SHOWUICOMPOSITIONWINDOW          equ 80000000h
ISC_SHOWUIGUIDELINE                  equ 40000000h
ISC_SHOWUIALLCANDIDATEWINDOW         equ 0Fh
ISC_SHOWUIALL                        equ 0C000000Fh
CPS_COMPLETE                         equ 1h
CPS_CONVERT                          equ 2h
CPS_REVERT                           equ 3h
CPS_CANCEL                           equ 4h
IME_CHOTKEY_IME_NONIME_TOGGLE        equ 10h
IME_CHOTKEY_SHAPE_TOGGLE             equ 11h
IME_CHOTKEY_SYMBOL_TOGGLE            equ 12h
IME_JHOTKEY_CLOSE_OPEN               equ 30h
IME_KHOTKEY_SHAPE_TOGGLE             equ 50h
IME_KHOTKEY_HANJACONVERT             equ 51h
IME_KHOTKEY_ENGLISH                  equ 52h
IME_THOTKEY_IME_NONIME_TOGGLE        equ 70h
IME_THOTKEY_SHAPE_TOGGLE             equ 71h
IME_THOTKEY_SYMBOL_TOGGLE            equ 72h
IME_HOTKEY_DSWITCH_FIRST             equ 100h
IME_HOTKEY_DSWITCH_LAST              equ 11Fh
IME_ITHOTKEY_RESEND_RESULTSTR        equ 200h
IME_ITHOTKEY_PREVIOUS_COMPOSITION    equ 201h
IME_ITHOTKEY_UISTYLE_TOGGLE          equ 202h
GCS_COMPREADSTR                      equ 1h
GCS_COMPREADATTR                     equ 2h
GCS_COMPREADCLAUSE                   equ 4h
GCS_COMPSTR                          equ 8h
GCS_COMPATTR                         equ 10h
GCS_COMPCLAUSE                       equ 20h
GCS_CURSORPOS                        equ 80h
GCS_DELTASTART                       equ 100h
GCS_RESULTREADSTR                    equ 200h
GCS_RESULTREADCLAUSE                 equ 400h
GCS_RESULTSTR                        equ 800h
GCS_RESULTCLAUSE                     equ 1000h
CS_INSERTCHAR                        equ 2000h
CS_NOMOVECARET                       equ 4000h
IME_PROP_AT_CARET                    equ 10000h
IME_PROP_SPECIAL_UI                  equ 20000h
IME_PROP_CANDLIST_START_FROM_1       equ 40000h
IME_PROP_UNICODE                     equ 80000h
UI_CAP_2700                          equ 1h
UI_CAP_ROT90                         equ 2h
UI_CAP_ROTANY                        equ 4h
SCS_CAP_COMPSTR                      equ 1h
SCS_CAP_MAKEREAD                     equ 2h
SELECT_CAP_CONVERSION                equ 1h
SELECT_CAP_SENTENCE                  equ 2h
GGL_LEVEL                            equ 1h
GGL_INDEX                            equ 2h
GGL_STRING                           equ 3h
GGL_PRIVATE                          equ 4h
GL_LEVEL_NOGUIDELINE                 equ 0h
GL_LEVEL_FATAL                       equ 1h
GL_LEVEL_ERROR                       equ 2h
GL_LEVEL_WARNING                     equ 3h
GL_LEVEL_INFORMATION                 equ 4h
GL_ID_UNKNOWN                        equ 0h
GL_ID_NOMODULE                       equ 1h
GL_ID_NODICTIONARY                   equ 10h
GL_ID_CANNOTSAVE                     equ 11h
GL_ID_NOCONVERT                      equ 20h
GL_ID_TYPINGERROR                    equ 21h
GL_ID_TOOMANYSTROKE                  equ 22h
GL_ID_READINGCONFLICT                equ 23h
GL_ID_INPUTREADING                   equ 24h
GL_ID_INPUTRADICAL                   equ 25h
GL_ID_INPUTCODE                      equ 26h
GL_ID_INPUTSYMBOL                    equ 27h
GL_ID_CHOOSECANDIDATE                equ 28h
GL_ID_REVERSECONVERSION              equ 29h
GL_ID_PRIVATE_FIRST                  equ 8000h
GL_ID_PRIVATE_LAST                   equ 0FFFFh
IGP_PROPERTY                         equ 4h
IGP_CONVERSION                       equ 8h
IGP_SENTENCE                         equ 0Ch
IGP_UI                               equ 10h
IGP_SETCOMPSTR                       equ 14h
IGP_SELECT                           equ 18h
SCS_SETSTR                           equ GCS_COMPREADSTR OR GCS_COMPSTR
SCS_CHANGEATTR                       equ GCS_COMPREADATTR OR GCS_COMPATTR
SCS_CHANGECLAUSE                     equ GCS_COMPREADCLAUSE OR GCS_COMPCLAUSE
ATTR_INPUT                           equ 0h
ATTR_TARGET_CONVERTED                equ 1h
ATTR_CONVERTED                       equ 2h
ATTR_TARGET_NOTCONVERTED             equ 3h
ATTR_INPUT_ERROR                     equ 4h
CFS_DEFAULT                          equ 0h
CFS_RECT                             equ 1h
CFS_POINT                            equ 2h
CFS_SCREEN                           equ 4h
CFS_FORCE_POSITION                   equ 20h
CFS_CANDIDATEPOS                     equ 40h
CFS_EXCLUDE                          equ 80h
GCL_CONVERSION                       equ 1h
GCL_REVERSECONVERSION                equ 2h
GCL_REVERSE_LENGTH                   equ 3h
IME_CMODE_ALPHANUMERIC               equ 0h
IME_CMODE_NATIVE                     equ 1h
IME_CMODE_CHINESE                    equ IME_CMODE_NATIVE
IME_CMODE_HANGEUL                    equ IME_CMODE_NATIVE
IME_CMODE_JAPANESE                   equ IME_CMODE_NATIVE
IME_CMODE_KATAKANA                   equ 2h
IME_CMODE_LANGUAGE                   equ 3h
IME_CMODE_FULLSHAPE                  equ 8h
IME_CMODE_ROMAN                      equ 10h
IME_CMODE_CHARCODE                   equ 20h
IME_CMODE_HANJACONVERT               equ 40h
IME_CMODE_SOFTKBD                    equ 80h
IME_CMODE_NOCONVERSION               equ 100h
IME_CMODE_EUDC                       equ 200h
IME_CMODE_SYMBOL                     equ 400h
IME_SMODE_NONE                       equ 0h
IME_SMODE_PLAURALCLAUSE              equ 1h
IME_SMODE_SINGLECONVERT              equ 2h
IME_SMODE_AUTOMATIC                  equ 4h
IME_SMODE_PHRASEPREDICT              equ 8h
IME_CAND_UNKNOWN                     equ 0h
IME_CAND_READ                        equ 1h
IME_CAND_CODE                        equ 2h
IME_CAND_MEANING                     equ 3h
IME_CAND_RADICAL                     equ 4h
IME_CAND_STROKE                      equ 5h
IMN_CLOSESTATUSWINDOW                equ 1h
IMN_OPENSTATUSWINDOW                 equ 2h
IMN_CHANGECANDIDATE                  equ 3h
IMN_CLOSECANDIDATE                   equ 4h
IMN_OPENCANDIDATE                    equ 5h
IMN_SETCONVERSIONMODE                equ 6h
IMN_SETSENTENCEMODE                  equ 7h
IMN_SETOPENSTATUS                    equ 8h
IMN_SETCANDIDATEPOS                  equ 9h
IMN_SETCOMPOSITIONFONT               equ 0Ah
IMN_SETCOMPOSITIONWINDOW             equ 0Bh
IMN_SETSTATUSWINDOWPOS               equ 0Ch
IMN_GUIDELINE                        equ 0Dh
IMN_PRIVATE                          equ 0Eh
IMM_ERROR_NODATA                     equ -1
IMM_ERROR_GENERAL                    equ -2
IME_CONFIG_GENERAL                   equ 1
IME_CONFIG_REGISTERWORD              equ 2
IME_CONFIG_SELECTDICTIONARY          equ 3
IME_ESC_QUERY_SUPPORT                equ 3h
IME_ESC_RESERVED_FIRST               equ 4h
IME_ESC_RESERVED_LAST                equ 7FFh
IME_ESC_PRIVATE_FIRST                equ 800h
IME_ESC_PRIVATE_LAST                 equ 0FFFh
IME_ESC_SEQUENCE_TO_INTERNAL         equ 1001h
IME_ESC_GET_EUDC_DICTIONARY          equ 1003h
IME_ESC_SET_EUDC_DICTIONARY          equ 1004h
IME_ESC_MAX_KEY                      equ 1005h
IME_ESC_IME_NAME                     equ 1006h
IME_ESC_SYNC_HOTKEY                  equ 1007h
IME_ESC_HANJA_MODE                   equ 1008h
IME_REGWORD_STYLE_EUDC               equ 1h
IME_REGWORD_STYLE_USER_FIRST         equ 80000000h
IME_REGWORD_STYLE_USER_LAST          equ 0FFFFh
SOFTKEYBOARD_TYPE_T1                 equ 1h
SOFTKEYBOARD_TYPE_C1                 equ 2h
DIALOPTION_BILLING                   equ 40h
DIALOPTION_QUIET                     equ 80h
DIALOPTION_DIALTONE                  equ 100h
MDMVOLFLAG_LOW                       equ 1h
MDMVOLFLAG_MEDIUM                    equ 2h
MDMVOLFLAG_HIGH                      equ 4h
MDMVOL_LOW                           equ 0h
MDMVOL_MEDIUM                        equ 1h
MDMVOL_HIGH                          equ 2h
MDMSPKRFLAG_OFF                      equ 1h
MDMSPKRFLAG_DIAL                     equ 2h
MDMSPKRFLAG_ON                       equ 4h
MDMSPKRFLAG_CALLSETUP                equ 8h
MDMSPKR_OFF                          equ 0h
MDMSPKR_DIAL                         equ 1h
MDMSPKR_ON                           equ 2h
MDMSPKR_CALLSETUP                    equ 3h
MDM_COMPRESSION                      equ 1h
MDM_ERROR_CONTROL                    equ 2h
MDM_FORCED_EC                        equ 4h
MDM_CELLULAR                         equ 8h
MDM_FLOWCONTROL_HARD                 equ 10h
MDM_FLOWCONTROL_SOFT                 equ 20h
MDM_CCITT_OVERRIDE                   equ 40h
MDM_SPEED_ADJUST                     equ 80h
MDM_TONE_DIAL                        equ 100h
MDM_BLIND_DIAL                       equ 200h
MDM_V23_OVERRIDE                     equ 400h
ABM_NEW                              equ 0h
ABM_REMOVE                           equ 1h
ABM_QUERYPOS                         equ 2h
ABM_SETPOS                           equ 3h
ABM_GETSTATE                         equ 4h
ABM_GETTASKBARPOS                    equ 5h
ABM_ACTIVATE                         equ 6h
ABM_GETAUTOHIDEBAR                   equ 7h
ABM_SETAUTOHIDEBAR                   equ 8h
ABM_WINDOWPOSCHANGED                 equ 9h
ABN_STATECHANGE                      equ 0h
ABN_POSCHANGED                       equ 1h
ABN_FULLSCREENAPP                    equ 2h
ABN_WINDOWARRANGE                    equ 3h
ABS_AUTOHIDE                         equ 1h
ABS_ALWAYSONTOP                      equ 2h
ABE_LEFT                             equ 0
ABE_TOP                              equ 1
ABE_RIGHT                            equ 2
ABE_BOTTOM                           equ 3
EIRESID                              equ -1
FO_MOVE                              equ 1h
FO_COPY                              equ 2h
FO_DELETE                            equ 3h
FO_RENAME                            equ 4h
FOF_MULTIDESTFILES                   equ 1h
FOF_CONFIRMMOUSE                     equ 2h
FOF_SILENT                           equ 4h
FOF_RENAMEONCOLLISION                equ 8h
FOF_NOCONFIRMATION                   equ 10h
FOF_WANTMAPPINGHANDLE                equ 20h
FOF_ALLOWUNDO                        equ 40h
FOF_FILESONLY                        equ 80h
FOF_SIMPLEPROGRESS                   equ 100h
FOF_NOCONFIRMMKDIR                   equ 200h
PO_DELETE                            equ 13h
PO_RENAME                            equ 14h
PO_PORTCHANGE                        equ 20h
PO_REN_PORT                          equ 34h
SE_ERR_FNF                           equ 2
SE_ERR_PNF                           equ 3
SE_ERR_ACCESSDENIED                  equ 5
SE_ERR_OOM                           equ 8
SE_ERR_DLLNOTFOUND                   equ 32
SEE_MASK_CLASSNAME                   equ 1h
SEE_MASK_CLASSKEY                    equ 3h
SEE_MASK_IDLIST                      equ 4h
SEE_MASK_INVOKEIDLIST                equ 0Ch
SEE_MASK_ICON                        equ 10h
SEE_MASK_HOTKEY                      equ 20h
SEE_MASK_NOCLOSEPROCESS              equ 40h
SEE_MASK_CONNECTNETDRV               equ 80h
SEE_MASK_FLAG_DDEWAIT                equ 100h
SEE_MASK_DOENVSUBST                  equ 200h
SEE_MASK_FLAG_NO_UI                  equ 400h
NIM_ADD                              equ 0h
NIM_MODIFY                           equ 1h
NIM_DELETE                           equ 2h
NIF_MESSAGE                          equ 1h
NIF_ICON                             equ 2h
NIF_TIP                              equ 4h
SHGFI_ICON                           equ 100h
SHGFI_DISPLAYNAME                    equ 200h
SHGFI_TYPENAME                       equ 400h
SHGFI_ATTRIBUTES                     equ 800h
SHGFI_ICONLOCATION                   equ 1000h
SHGFI_EXETYPE                        equ 2000h
SHGFI_SYSICONINDEX                   equ 4000h
SHGFI_LINKOVERLAY                    equ 8000h
SHGFI_SELECTED                       equ 10000h
SHGFI_LARGEICON                      equ 0h
SHGFI_SMALLICON                      equ 1h
SHGFI_OPENICON                       equ 2h
SHGFI_SHELLICONSIZE                  equ 4h
SHGFI_PIDL                           equ 8h
SHGFI_USEFILEATTRIBUTES              equ 10h
SHGNLI_PIDL                          equ 1h
SHGNLI_PREFIXNAME                    equ 2h
VS_VERSION_INFO                      equ 1
VS_USER_DEFINED                      equ 100
VS_FFI_SIGNATURE                     equ 0FEEF04BDh
VS_FFI_STRUCVERSION                  equ 10000h
VS_FFI_FILEFLAGSMASK                 equ 3Fh
VS_FF_DEBUG                          equ 1h
VS_FF_PRERELEASE                     equ 2h
VS_FF_PATCHED                        equ 4h
VS_FF_PRIVATEBUILD                   equ 8h
VS_FF_INFOINFERRED                   equ 10h
VS_FF_SPECIALBUILD                   equ 20h
VOS_UNKNOWN                          equ 0h
VOS_DOS                              equ 10000h
VOS_OS216                            equ 20000h
VOS_OS232                            equ 30000h
VOS_NT                               equ 40000h
VOS__BASE                            equ 0h
VOS__WINDOWS16                       equ 1h
VOS__PM16                            equ 2h
VOS__PM32                            equ 3h
VOS__WINDOWS32                       equ 4h
VOS_DOS_WINDOWS16                    equ 10001h
VOS_DOS_WINDOWS32                    equ 10004h
VOS_OS216_PM16                       equ 20002h
VOS_OS232_PM32                       equ 30003h
VOS_NT_WINDOWS32                     equ 40004h
VFT_UNKNOWN                          equ 0h
VFT_APP                              equ 1h
VFT_DLL                              equ 2h
VFT_DRV                              equ 3h
VFT_FONT                             equ 4h
VFT_VXD                              equ 5h
VFT_STATIC_LIB                       equ 7h
VFT2_UNKNOWN                         equ 0h
VFT2_DRV_PRINTER                     equ 1h
VFT2_DRV_KEYBOARD                    equ 2h
VFT2_DRV_LANGUAGE                    equ 3h
VFT2_DRV_DISPLAY                     equ 4h
VFT2_DRV_MOUSE                       equ 5h
VFT2_DRV_NETWORK                     equ 6h
VFT2_DRV_SYSTEM                      equ 7h
VFT2_DRV_INSTALLABLE                 equ 8h
VFT2_DRV_SOUND                       equ 9h
VFT2_DRV_COMM                        equ 0Ah
VFT2_DRV_INPUTMETHOD                 equ 0Bh
VFT2_FONT_RASTER                     equ 1h
VFT2_FONT_VECTOR                     equ 2h
VFT2_FONT_TRUETYPE                   equ 3h
VFFF_ISSHAREDFILE                    equ 1h
VFF_CURNEDEST                        equ 1h
VFF_FILEINUSE                        equ 2h
VFF_BUFFTOOSMALL                     equ 4h
VIFF_FORCEINSTALL                    equ 1h
VIFF_DONTDELETEOLD                   equ 2h
VIF_TEMPFILE                         equ 1h
VIF_MISMATCH                         equ 2h
VIF_SRCOLD                           equ 4h
VIF_DIFFLANG                         equ 8h
VIF_DIFFCODEPG                       equ 10h
VIF_DIFFTYPE                         equ 20h
VIF_WRITEPROT                        equ 40h
VIF_FILEINUSE                        equ 80h
VIF_OUTOFSPACE                       equ 100h
VIF_ACCESSVIOLATION                  equ 200h
VIF_SHARINGVIOLATION                 equ 400h
VIF_CANNOTCREATE                     equ 800h
VIF_CANNOTDELETE                     equ 1000h
VIF_CANNOTRENAME                     equ 2000h
VIF_CANNOTDELETECUR                  equ 4000h
VIF_OUTOFMEMORY                      equ 8000h
VIF_CANNOTREADSRC                    equ 10000h
VIF_CANNOTREADDST                    equ 20000h
VIF_BUFFTOOSMALL                     equ 40000h
PROCESS_HEAP_REGION                  equ 1h
PROCESS_HEAP_UNCOMMITTED_RANGE       equ 2h
PROCESS_HEAP_ENTRY_BUSY              equ 4h
PROCESS_HEAP_ENTRY_MOVEABLE          equ 10h
PROCESS_HEAP_ENTRY_DDESHARE          equ 20h
SCS_32BIT_BINARY                     equ 0
SCS_DOS_BINARY                       equ 1
SCS_WOW_BINARY                       equ 2
SCS_PIF_BINARY                       equ 3
SCS_POSIX_BINARY                     equ 4
SCS_OS216_BINARY                     equ 5
LOGON32_LOGON_INTERACTIVE            equ 2
LOGON32_LOGON_BATCH                  equ 4
LOGON32_LOGON_SERVICE                equ 5
LOGON32_PROVIDER_DEFAULT             equ 0
LOGON32_PROVIDER_WINNT35             equ 1
VER_PLATFORM_WIN32s                  equ 0
VER_PLATFORM_WIN32_WINDOWS           equ 1
VER_PLATFORM_WIN32_NT                equ 2
AC_LINE_OFFLINE                      equ 0h
AC_LINE_ONLINE                       equ 1h
AC_LINE_BACKUP_POWER                 equ 2h
AC_LINE_UNKNOWN                      equ 0FFh
BATTERY_FLAG_HIGH                    equ 1h
BATTERY_FLAG_LOW                     equ 2h
BATTERY_FLAG_CRITICAL                equ 4h
BATTERY_FLAG_CHARGING                equ 8h
BATTERY_FLAG_NO_BATTERY              equ 80h
BATTERY_FLAG_UNKNOWN                 equ 0FFh
BATTERY_PERCENTAGE_UNKNOWN           equ 0FFh
BATTERY_LIFE_UNKNOWN                 equ 0FFFFh
CDM_FIRST                            equ WM_USER + 100
CDM_LAST                             equ WM_USER + 200
CDM_GETSPEC                          equ CDM_FIRST + 0h
CDM_GETFILEPATH                      equ CDM_FIRST + 1h
CDM_GETFOLDERPATH                    equ CDM_FIRST + 2h
CDM_GETFOLDERIDLIST                  equ CDM_FIRST + 3h
CDM_SETCONTROLTEXT                   equ CDM_FIRST + 4h
CDM_HIDECONTROL                      equ CDM_FIRST + 5h
CDM_SETDEFEXT                        equ CDM_FIRST + 6h
SIMULATED_FONTTYPE                   equ 8000h
PRINTER_FONTTYPE                     equ 4000h
SCREEN_FONTTYPE                      equ 2000h
BOLD_FONTTYPE                        equ 100h
ITALIC_FONTTYPE                      equ 200h
REGULAR_FONTTYPE                     equ 400h
WM_PSD_PAGESETUPDLG                  equ WM_USER
WM_PSD_FULLPAGERECT                  equ WM_USER + 1
WM_PSD_MINMARGINRECT                 equ WM_USER + 2
WM_PSD_MARGINRECT                    equ WM_USER + 3
WM_PSD_GREEKTEXTRECT                 equ WM_USER + 4
WM_PSD_ENVSTAMPRECT                  equ WM_USER + 5
WM_PSD_YAFULLPAGERECT                equ WM_USER + 6
PSD_DEFAULTMINMARGINS                equ 0h
PSD_INWININIINTLMEASURE              equ 0h
PSD_MINMARGINS                       equ 1h
PSD_MARGINS                          equ 2h
PSD_INTHOUSANDTHSOFINCHES            equ 4h
PSD_INHUNDREDTHSOFMILLIMETERS        equ 8h
PSD_DISABLEMARGINS                   equ 10h
PSD_DISABLEPRINTER                   equ 20h
PSD_NOWARNING                        equ 80h
PSD_DISABLEORIENTATION               equ 100h
PSD_RETURNDEFAULT                    equ 400h
PSD_DISABLEPAPER                     equ 200h
PSD_SHOWHELP                         equ 800h
PSD_ENABLEPAGESETUPHOOK              equ 2000h
PSD_ENABLEPAGESETUPTEMPLATE          equ 8000h
PSD_ENABLEPAGESETUPTEMPLATEHANDLE    equ 20000h
PSD_ENABLEPAGEPAINTHOOK              equ 40000h
PSD_DISABLEPAGEPAINTING              equ 80000h
NM_FIRST                             equ 0-  0
NM_LAST                              equ 0-99
DBG_CONTINUE                         equ 00010002h
DBG_TERMINATE_THREAD                 equ 40010003h
DBG_TERMINATE_PROCESS                equ 40010004h
DBG_CONTROL_C                        equ 40010005h
DBG_CONTROL_BREAK                    equ 40010008h
DBG_EXCEPTION_NOT_HANDLED            equ 80010001h
SIZE_OF_80387_REGISTERS   equ 80
STATUS_WAIT_0             equ 00000000h
STATUS_ABANDONED_WAIT_0   equ 00000080h
STATUS_USER_APC           equ 000000C0h
STATUS_TIMEOUT            equ 00000102h
STATUS_PENDING            equ 00000103h
STATUS_DATATYPE_MISALIGNMENT              equ 80000002h
STATUS_BREAKPOINT         equ 80000003h
STATUS_SINGLE_STEP        equ 80000004h
STATUS_ACCESS_VIOLATION   equ 0C0000005h
STATUS_IN_PAGE_ERROR      equ 0C0000006h
STATUS_NO_MEMORY          equ 0C0000017h
STATUS_ILLEGAL_INSTRUCTION                equ 0C000001Dh
STATUS_NONCONTINUABLE_EXCEPTION           equ 0C0000025h
STATUS_INVALID_DISPOSITION                equ 0C0000026h
STATUS_ARRAY_BOUNDS_EXCEEDED              equ 0C000008Ch
STATUS_FLOAT_DENORMAL_OPERAND             equ 0C000008Dh
STATUS_FLOAT_DIVIDE_BY_ZERO               equ 0C000008Eh
STATUS_FLOAT_INEXACT_RESULT               equ 0C000008Fh
STATUS_FLOAT_INVALID_OPERATION            equ 0C0000090h
STATUS_FLOAT_OVERFLOW     equ 0C0000091h
STATUS_FLOAT_STACK_CHECK  equ 0C0000092h
STATUS_FLOAT_UNDERFLOW    equ 0C0000093h
STATUS_INTEGER_DIVIDE_BY_ZERO             equ 0C0000094h
STATUS_INTEGER_OVERFLOW   equ 0C0000095h
STATUS_PRIVILEGED_INSTRUCTION             equ 0C0000096h
STATUS_STACK_OVERFLOW     equ 0C00000FDh
STATUS_CONTROL_C_EXIT     equ 0C000013Ah
WAIT_FAILED               equ -1
WAIT_OBJECT_0             equ STATUS_WAIT_0
WAIT_ABANDONED            equ STATUS_ABANDONED_WAIT_0
WAIT_ABANDONED_0          equ STATUS_ABANDONED_WAIT_0
WAIT_TIMEOUT              equ STATUS_TIMEOUT
WAIT_IO_COMPLETION        equ STATUS_USER_APC
STILL_ACTIVE              equ STATUS_PENDING

EXCEPTION_CONTINUABLE     equ 0
EXCEPTION_NONCONTINUABLE  equ 1h
EXCEPTION_ACCESS_VIOLATION                equ STATUS_ACCESS_VIOLATION
EXCEPTION_DATATYPE_MISALIGNMENT           equ STATUS_DATATYPE_MISALIGNMENT
EXCEPTION_BREAKPOINT      equ STATUS_BREAKPOINT
EXCEPTION_SINGLE_STEP     equ STATUS_SINGLE_STEP
EXCEPTION_ARRAY_BOUNDS_EXCEEDED           equ STATUS_ARRAY_BOUNDS_EXCEEDED
EXCEPTION_FLT_DENORMAL_OPERAND            equ STATUS_FLOAT_DENORMAL_OPERAND
EXCEPTION_FLT_DIVIDE_BY_ZERO              equ STATUS_FLOAT_DIVIDE_BY_ZERO
EXCEPTION_FLT_INEXACT_RESULT              equ STATUS_FLOAT_INEXACT_RESULT
EXCEPTION_FLT_INVALID_OPERATION           equ STATUS_FLOAT_INVALID_OPERATION
EXCEPTION_FLT_OVERFLOW    equ STATUS_FLOAT_OVERFLOW
EXCEPTION_FLT_STACK_CHECK equ STATUS_FLOAT_STACK_CHECK
EXCEPTION_FLT_UNDERFLOW   equ STATUS_FLOAT_UNDERFLOW

EXCEPTION_INT_DIVIDE_BY_ZERO              equ STATUS_INTEGER_DIVIDE_BY_ZERO
EXCEPTION_INT_OVERFLOW    equ STATUS_INTEGER_OVERFLOW
EXCEPTION_ILLEGAL_INSTRUCTION     equ  STATUS_ILLEGAL_INSTRUCTION
EXCEPTION_PRIV_INSTRUCTION                equ STATUS_PRIVILEGED_INSTRUCTION
EXCEPTION_IN_PAGE_ERROR   equ STATUS_IN_PAGE_ERROR
CONTEXT_i386              equ 00010000h
CONTEXT_i486              equ 00010000h
CONTEXT_CONTROL           equ CONTEXT_i386 OR 00000001h
CONTEXT_INTEGER           equ CONTEXT_i386 OR 00000002h
CONTEXT_SEGMENTS          equ CONTEXT_i386 OR 00000004h
CONTEXT_FLOATING_POINT    equ CONTEXT_i386 OR 00000008h
CONTEXT_DEBUG_REGISTERS   equ CONTEXT_i386 OR 00000010h
CONTEXT_FULL              equ CONTEXT_CONTROL OR CONTEXT_INTEGER OR CONTEXT_SEGMENTS
PAGE_NOACCESS             equ 1
PAGE_READONLY             equ 2
PAGE_READWRITE            equ 4
PAGE_WRITECOPY            equ 8
PAGE_EXECUTE              equ 10h
PAGE_EXECUTE_READ         equ 20h
PAGE_EXECUTE_READWRITE    equ 40h
PAGE_EXECUTE_WRITECOPY    equ 80h
PAGE_GUARD                equ 100h
PAGE_NOCACHE              equ 200h
MEM_COMMIT                equ 1000h
MEM_RESERVE               equ 2000h
MEM_DECOMMIT              equ 4000h
MEM_RELEASE               equ 8000h
MEM_FREE                  equ 10000h
MEM_PRIVATE               equ 20000h
MEM_MAPPED                equ 40000h
MEM_RESET                 equ 80000h     
MEM_TOP_DOWN              equ 100000h
SEC_FILE                  equ 800000h
SEC_IMAGE                 equ 1000000h
SEC_RESERVE               equ 4000000h
SEC_COMMIT                equ 8000000h
SEC_NOCACHE               equ 10000000h
MEM_IMAGE        equ   <SEC_IMAGE>
IMAGE_DIRECTORY_ENTRY_EXPORT                equ  0
IMAGE_DIRECTORY_ENTRY_IMPORT                equ  1
IMAGE_DIRECTORY_ENTRY_RESOURCE              equ  2
IMAGE_DIRECTORY_ENTRY_EXCEPTION             equ  3
IMAGE_DIRECTORY_ENTRY_SECURITY              equ  4
IMAGE_DIRECTORY_ENTRY_BASERELOC             equ  5
IMAGE_DIRECTORY_ENTRY_DEBUG equ  6
IMAGE_DIRECTORY_ENTRY_COPYRIGHT             equ  7
IMAGE_DIRECTORY_ENTRY_GLOBALPTR             equ  8
IMAGE_DIRECTORY_ENTRY_TLS   equ  9
IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG           equ 10
IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT          equ 11
IMAGE_DIRECTORY_ENTRY_IAT   equ 12
IMAGE_NUMBEROF_DIRECTORY_ENTRIES            equ 16
IMAGE_BITMAP                equ 0
IMAGE_ICON                  equ 1
IMAGE_CURSOR                equ 2
IMAGE_ENHMETAFILE           equ 3
PROCESSOR_INTEL_386         equ 386
PROCESSOR_INTEL_486         equ 486
PROCESSOR_INTEL_PENTIUM     equ 586
PROCESSOR_MIPS_R4000        equ 4000
PROCESSOR_ALPHA_21064       equ 21064

CALLBACK        TYPEDEF PROTO STDCALL
WINAPI          TYPEDEF PROTO STDCALL

BOOL            TYPEDEF DWORD
BOOLEAN         TYPEDEF BYTE
LPBYTE          TYPEDEF PTR BYTE
LPDWORD         TYPEDEF PTR DWORD
LPVOID          TYPEDEF PTR
LPCVOID         TYPEDEF PTR
LPSTR           TYPEDEF ptr byte		;changed 14.7.2002 japheth
LPCSTR          TYPEDEF ptr byte		;changed 14.7.2002 japheth

UINT            TYPEDEF DWORD
LONG            TYPEDEF DWORD
SCODE		    TYPEDEF LONG

WPARAM          TYPEDEF UINT
LPARAM          TYPEDEF DWORD

HANDLE          TYPEDEF DWORD
HWND            TYPEDEF DWORD
HGLOBAL         TYPEDEF DWORD
HGDIOBJ         TYPEDEF DWORD
HACCEL          TYPEDEF DWORD
HBITMAP         TYPEDEF DWORD
HBRUSH          TYPEDEF DWORD
HDC             TYPEDEF DWORD
HFONT           TYPEDEF DWORD
HICON           TYPEDEF DWORD
HMENU           TYPEDEF DWORD
HINSTANCE       TYPEDEF DWORD
HRGN            TYPEDEF DWORD
HRSRC           TYPEDEF DWORD
HCURSOR         TYPEDEF DWORD
COLORREF        TYPEDEF DWORD

; --------------------------- comctl equates -------------------------------

ODT_HEADER          equ 100
ODT_TAB             equ 101
ODT_LISTVIEW        equ 102
LVM_FIRST           equ 1000h
TV_FIRST            equ 1100h
HDM_FIRST           equ 1200h
NM_OUTOFMEMORY      equ NM_FIRST-1
NM_CLICK            equ NM_FIRST-2
NM_DBLCLK           equ NM_FIRST-3
NM_RETURN           equ NM_FIRST-4
NM_RCLICK           equ NM_FIRST-5
NM_RDBLCLK          equ NM_FIRST-6
NM_SETFOCUS         equ NM_FIRST-7
NM_KILLFOCUS        equ NM_FIRST-8
CCS_TOP             equ 00000001h
CCS_NOMOVEY         equ 00000002h
CCS_BOTTOM          equ 00000003h
CCS_NORESIZE        equ 00000004h
CCS_NOPARENTALIGN   equ 00000008h
CCS_ADJUSTABLE      equ 00000020h
CCS_NODIVIDER       equ 00000040h
CCM_FIRST           equ 2000h
CCM_LAST            equ    CCM_FIRST + 200h
CCM_SETBKCOLOR          equ CCM_FIRST + 1
CCM_SETCOLORSCHEME      equ CCM_FIRST + 2
CCM_GETCOLORSCHEME      equ CCM_FIRST + 3
CCM_GETDROPTARGET       equ CCM_FIRST + 4
CCM_SETUNICODEFORMAT    equ CCM_FIRST + 5
CCM_GETUNICODEFORMAT    equ CCM_FIRST + 6
LVN_FIRST        equ 0-100
LVN_LAST         equ 0-199
HDN_FIRST        equ 0-300
HDN_LAST         equ 0-399
TVN_FIRST        equ 0-400
TVN_LAST         equ 0-499
TTN_FIRST        equ 0-520
TTN_LAST         equ 0-549
TCN_FIRST        equ 0-550
TCN_LAST         equ 0-580
CDN_FIRST        equ 0-601
CDN_LAST         equ 0-699
TBN_FIRST        equ 0-700
TBN_LAST         equ 0-720
UDN_FIRST        equ 0-721
UDN_LAST         equ 0-740
MCN_FIRST        equ 0-750
MCN_LAST         equ 0-759
DTN_FIRST        equ 0-760
DTN_LAST         equ 0-799
CBEN_FIRST       equ 0-800
CBEN_LAST        equ 0-830
RBN_FIRST        equ 0-831
RBN_LAST         equ 0-859
IPN_FIRST        equ 0-860
IPN_LAST         equ 0-879
SBN_FIRST        equ 0-880
SBN_LAST         equ 0-899
PGN_FIRST        equ 0-900
PGN_LAST         equ 0-950
PGM_FIRST               equ 1400h
MSGF_COMMCTRL_BEGINDRAG            equ 4200h
MSGF_COMMCTRL_SIZEHEADER           equ 4201h
MSGF_COMMCTRL_DRAGSELECT           equ 4202h
MSGF_COMMCTRL_TOOLBARCUST          equ 4203h
ICC_LISTVIEW_CLASSES             equ 00000001h
ICC_TREEVIEW_CLASSES             equ 00000002h
ICC_BAR_CLASSES  equ 00000004h
ICC_TAB_CLASSES  equ 00000008h
ICC_UPDOWN_CLASS equ 00000010h
ICC_PROGRESS_CLASS               equ 00000020h
ICC_HOTKEY_CLASS equ 00000040h
ICC_ANIMATE_CLASS                equ 00000080h
ICC_WIN95_CLASSES                equ 000000FFh
ICC_DATE_CLASSES equ 00000100h
ICC_USEREX_CLASSES               equ 00000200h
ICC_COOL_CLASSES equ 00000400h
ICC_INTERNET_CLASSES             equ 00000800h
ICC_PAGESCROLLER_CLASS           equ 00001000h
ICC_NATIVEFNTCTL_CLASS           equ 00002000h
RBIM_IMAGELIST equ 1
RBS_TOOLTIPS   equ 100h
RBS_VARHEIGHT  equ 200h
RBS_BANDBORDERS                equ 400h
RBS_FIXEDORDER equ 800h
RBS_REGISTERDROP               equ 1000h
RBS_AUTOSIZE   equ 2000h
RBS_VERTICALGRIPPER            equ 4000h
RBS_DBLCLKTOGGLE               equ 8000h
RBBS_BREAK     equ 1
RBBS_FIXEDSIZE equ 2
RBBS_CHILDEDGE equ 4
RBBS_HIDDEN    equ 8
RBBS_NOVERT    equ 10h
RBBS_FIXEDBMP  equ 20h
RBBS_VARIABLEHEIGHT            equ 40h
RBBS_GRIPPERALWAYS             equ 80h
RBBS_NOGRIPPER equ 100h
RBBIM_STYLE    equ 1
RBBIM_COLORS   equ 2
RBBIM_TEXT     equ 4
RBBIM_IMAGE    equ 8
RBBIM_CHILD    equ 10h
RBBIM_CHILDSIZE equ 20h
RBBIM_SIZE     equ 40h
RBBIM_BACKGROUND               equ 80h
RBBIM_ID       equ 100h
RBBIM_IDEALSIZE                equ 00000200h
RBBIM_LPARAM   equ 00000400h
RBBIM_HEADERSIZE               equ 00000800h
RB_INSERTBAND  equ WM_USER +  1
RB_DELETEBAND  equ WM_USER +  2
RB_GETBARINFO  equ WM_USER +  3
RB_SETBARINFO  equ WM_USER +  4
RB_GETBANDINFO equ WM_USER +  29
RB_SETBANDINFO equ WM_USER +  6
RB_SETPARENT   equ WM_USER +  7
RB_HITTEST     equ WM_USER +  8
RB_GETRECT     equ WM_USER +  9
RB_GETBANDCOUNT                equ WM_USER +  12
RB_GETROWCOUNT equ WM_USER +  13
RB_GETROWHEIGHT                equ WM_USER +  14
RB_IDTOINDEX   equ WM_USER +  16
RB_GETTOOLTIPS equ WM_USER +  17
RB_SETTOOLTIPS equ WM_USER +  18
RB_SETBKCOLOR  equ WM_USER +  19
RB_GETBKCOLOR  equ WM_USER +  20
RB_SETTEXTCOLOR                equ WM_USER +  21
RB_GETTEXTCOLOR                equ WM_USER +  22
RB_SIZETORECT  equ WM_USER +  23
RB_SETCOLORSCHEME              equ CCM_SETCOLORSCHEME
RB_GETCOLORSCHEME              equ CCM_GETCOLORSCHEME
RB_BEGINDRAG   equ WM_USER + 24
RB_ENDDRAG     equ WM_USER + 25
RB_DRAGMOVE    equ WM_USER + 26
RB_GETBARHEIGHT                equ WM_USER + 27
RB_MINIMIZEBAND                equ WM_USER + 30
RB_MAXIMIZEBAND                equ WM_USER + 31
RB_GETDROPTARGET               equ CCM_GETDROPTARGET
RB_GETBANDBORDERS              equ WM_USER + 34
RB_SHOWBAND    equ WM_USER + 35
RB_SETPALETTE  equ WM_USER + 37
RB_GETPALETTE  equ WM_USER + 38
RB_MOVEBAND    equ WM_USER + 39
RB_SETUNICODEFORMAT            equ CCM_SETUNICODEFORMAT
RB_GETUNICODEFORMAT            equ CCM_GETUNICODEFORMAT
RBN_HEIGHTCHANGE               equ RBN_FIRST - 0
RBN_GETOBJECT  equ RBN_FIRST - 1
RBN_LAYOUTCHANGED              equ RBN_FIRST - 2
RBN_AUTOSIZE   equ RBN_FIRST - 3
RBN_BEGINDRAG  equ RBN_FIRST - 4
RBN_ENDDRAG    equ RBN_FIRST - 5
RBN_DELETINGBAND               equ RBN_FIRST - 6
RBN_DELETEDBAND                equ RBN_FIRST - 7
RBN_CHILDSIZE  equ RBN_FIRST - 8
RBNM_ID        equ 00000001h
RBNM_STYLE     equ 00000002h
RBNM_LPARAM    equ 00000004h
RBHT_NOWHERE   equ 0001h
RBHT_CAPTION   equ 0002h
RBHT_CLIENT    equ 0003h
RBHT_GRABBER   equ 0004h
CLR_NONE         equ 0FFFFFFFFh
CLR_DEFAULT      equ 0FF000000h
ILC_MASK         equ 0001h
ILC_COLOR        equ 0000h
ILC_COLORDDB     equ 00FEh
ILC_COLOR4       equ 0004h
ILC_COLOR8       equ 0008h
ILC_COLOR16      equ 0010h
ILC_COLOR24      equ 0018h
ILC_COLOR32      equ 0020h
ILC_PALETTE      equ 0800h
ILD_NORMAL       equ 0000h
ILD_TRANSPARENT  equ 0001h
ILD_MASK         equ 0010h
ILD_IMAGE        equ 0020h
ILD_BLEND25      equ 0002h
ILD_BLEND50      equ 0004h
ILD_OVERLAYMASK  equ 0F00h
ILD_SELECTED     equ ILD_BLEND50
ILD_FOCUS        equ ILD_BLEND25
ILD_BLEND        equ ILD_BLEND50
CLR_HILIGHT      equ CLR_DEFAULT
HDS_HORZ         equ 00000000h
HDS_BUTTONS      equ 00000002h
HDS_HIDDEN       equ 00000008h
HDI_WIDTH        equ 0001h
HDI_HEIGHT       equ HDI_WIDTH
HDI_TEXT         equ 0002h
HDI_FORMAT       equ 0004h
HDI_LPARAM       equ 0008h
HDI_BITMAP       equ 0010h
HDF_LEFT         equ 0
HDF_RIGHT        equ 1
HDF_CENTER       equ 2
HDF_JUSTIFYMASK  equ 0003h
HDF_RTLREADING   equ 4
HDF_OWNERDRAW    equ 8000h
HDF_STRING       equ 4000h
HDF_BITMAP       equ 2000h
HDM_GETITEMCOUNT equ HDM_FIRST + 0
HDM_INSERTITEM   equ  HDM_FIRST + 1
HDM_INSERTITEMW  equ  HDM_FIRST + 10
HDM_DELETEITEM  equ  HDM_FIRST + 2
HDM_GETITEM      equ  HDM_FIRST + 3
HDM_GETITEMW     equ  HDM_FIRST + 11
HDM_SETITEM      equ HDM_FIRST + 4
HDM_SETITEMW     equ HDM_FIRST + 12
HDM_LAYOUT       equ HDM_FIRST + 5
HHT_NOWHERE      equ 0001h
HHT_ONHEADER     equ 0002h
HHT_ONDIVIDER    equ 0004h
HHT_ONDIVOPEN    equ 0008h
HHT_ABOVE        equ 0100h
HHT_BELOW        equ 0200h
HHT_TORIGHT      equ 0400h
HHT_TOLEFT       equ 0800h
HDM_HITTEST      equ HDM_FIRST + 6
HDN_ITEMCHANGING equ HDN_FIRST - 0
HDN_ITEMCHANGINGW                equ HDN_FIRST - 20
HDN_ITEMCHANGED  equ HDN_FIRST - 1
HDN_ITEMCHANGEDW equ HDN_FIRST - 21
HDN_ITEMCLICK    equ HDN_FIRST - 2
HDN_ITEMCLICKW   equ HDN_FIRST - 22
HDN_ITEMDBLCLICK equ HDN_FIRST - 3
HDN_ITEMDBLCLICKW                equ HDN_FIRST - 23
HDN_DIVIDERDBLCLICK              equ HDN_FIRST - 5
HDN_DIVIDERDBLCLICKW             equ HDN_FIRST - 25
HDN_BEGINTRACK   equ HDN_FIRST - 6
HDN_BEGINTRACKW  equ HDN_FIRST - 26
HDN_ENDTRACK     equ HDN_FIRST - 7
HDN_ENDTRACKW    equ HDN_FIRST - 27
HDN_TRACK        equ HDN_FIRST - 8
HDN_TRACKW       equ HDN_FIRST - 28
CMB_MASKED      equ 02h
TBSTATE_CHECKED equ 01h
TBSTATE_PRESSED equ 02h
TBSTATE_ENABLED equ 04h
TBSTATE_HIDDEN  equ 08h
TBSTATE_INDETERMINATE           equ 10h
TBSTATE_WRAP    equ 20h
TBSTYLE_BUTTON  equ 00h
TBSTYLE_SEP     equ 01h
TBSTYLE_CHECK   equ 02h
TBSTYLE_GROUP   equ 04h
TBSTYLE_CHECKGROUP              equ TBSTYLE_GROUP OR TBSTYLE_CHECK
TBSTYLE_TOOLTIPS equ 0100h
TBSTYLE_WRAPABLE equ 0200h
TBSTYLE_ALTDRAG  equ 0400h
TBSTYLE_FLAT     equ 0800h
TBSTYLE_LIST     equ 1000h
TBSTYLE_CUSTOMERASE              equ 2000h
TBSTYLE_REGISTERDROP             equ 4000h
TBSTYLE_TRANSPARENT              equ 8000h
TB_ENABLEBUTTON  equ WM_USER + 1
TB_CHECKBUTTON   equ WM_USER + 2
TB_PRESSBUTTON   equ WM_USER + 3
TB_HIDEBUTTON    equ WM_USER + 4
TB_INDETERMINATE equ WM_USER + 5
TB_ISBUTTONENABLED               equ WM_USER + 9
TB_ISBUTTONCHECKED               equ WM_USER + 10
TB_ISBUTTONPRESSED               equ WM_USER + 11
TB_ISBUTTONHIDDEN                equ WM_USER + 12
TB_ISBUTTONINDETERMINATE          equ WM_USER + 13
TB_SETSTATE      equ WM_USER + 17
TB_GETSTATE      equ WM_USER + 18
TB_ADDBITMAP     equ WM_USER + 19
TB_SETSTYLE      equ WM_USER + 56
TB_GETSTYLE      equ WM_USER + 57
HINST_COMMCTRL   equ -1
IDB_STD_SMALL_COLOR              equ 0
IDB_STD_LARGE_COLOR              equ 1
IDB_VIEW_SMALL_COLOR             equ 4
IDB_VIEW_LARGE_COLOR             equ 5
STD_CUT          equ 0
STD_COPY         equ 1
STD_PASTE        equ 2
STD_UNDO         equ 3
STD_REDOW        equ 4
STD_DELETE       equ 5
STD_FILENEW      equ 6
STD_FILEOPEN     equ 7
STD_FILESAVE     equ 8
STD_PRINTPRE     equ 9
STD_PROPERTIES   equ 10
STD_HELP         equ 11
STD_FIND         equ 12
STD_REPLACE      equ 13
STD_PRINT        equ 14
VIEW_LARGEICONS  equ 0
VIEW_SMALLICONS  equ 1
VIEW_LIST        equ 2
VIEW_DETAILS     equ 3
VIEW_SORTNAME    equ 4
VIEW_SORTSIZE    equ 5
VIEW_SORTDATE    equ 6
VIEW_SORTTYPE    equ 7
VIEW_PARENTFOLDER                equ 8
VIEW_NETCONNECT  equ 9
VIEW_NETDISCONNECT               equ 10
VIEW_NEWFOLDER   equ 11
TB_ADDBUTTONS    equ WM_USER + 20
TB_INSERTBUTTON  equ WM_USER + 21
TB_DELETEBUTTON  equ WM_USER + 22
TB_GETBUTTON     equ WM_USER + 23
TB_BUTTONCOUNT   equ WM_USER + 24
TB_COMMANDTOINDEX                equ WM_USER + 25
TB_SAVERESTORE   equ WM_USER + 26
TB_CUSTOMIZE     equ WM_USER + 27
TB_ADDSTRING     equ WM_USER + 28
TB_GETITEMRECT   equ WM_USER + 29
TB_BUTTONSTRUCTSIZE              equ WM_USER + 30

TB_SETBUTTONSIZE equ WM_USER + 31
TB_SETBITMAPSIZE equ WM_USER + 32
TB_AUTOSIZE      equ WM_USER + 33
TB_GETTOOLTIPS   equ WM_USER + 35
TB_SETTOOLTIPS   equ WM_USER + 36
TB_SETPARENT     equ WM_USER + 37
TB_SETROWS       equ WM_USER + 39
TB_GETROWS       equ WM_USER + 40
TB_SETCMDID      equ WM_USER + 42
TB_CHANGEBITMAP  equ WM_USER + 43
TB_GETBITMAP     equ WM_USER + 44
TB_GETBUTTONTEXT equ WM_USER + 45
TB_REPLACEBITMAP equ WM_USER + 46
TBBF_LARGE       equ 1
TB_GETBITMAPFLAGS                equ WM_USER + 41
TBN_GETBUTTONINFO                equ TBN_FIRST
TBN_BEGINDRAG    equ TBN_FIRST-1
TBN_ENDDRAG      equ TBN_FIRST-2
TBN_BEGINADJUST  equ TBN_FIRST-3
TBN_ENDADJUST    equ TBN_FIRST-4
TBN_RESET        equ TBN_FIRST-5
TBN_QUERYINSERT  equ TBN_FIRST-6
TBN_QUERYDELETE  equ TBN_FIRST-7
TBN_TOOLBARCHANGE                equ TBN_FIRST-8
TBN_CUSTHELP     equ TBN_FIRST-9
TTS_ALWAYSTIP    equ 01h
TTS_NOPREFIX     equ 02h
TTF_IDISHWND     equ 01h
TTF_CENTERTIP    equ 02h
TTF_RTLREADING   equ 04h
TTF_SUBCLASS     equ 10h
TTDT_AUTOMATIC   equ 0
TTDT_RESHOW      equ 1
TTDT_AUTOPOP     equ 2
TTDT_INITIAL     equ 3
TTM_ACTIVATE     equ WM_USER + 1
TTM_SETDELAYTIME equ WM_USER + 3
TTM_ADDTOOL      equ WM_USER + 4
TTM_ADDTOOLW     equ WM_USER + 50
TTM_DELTOOL      equ WM_USER + 5
TTM_DELTOOLW     equ WM_USER + 51
TTM_NEWTOOLRECT  equ WM_USER + 6
TTM_NEWTOOLRECTW equ WM_USER + 52
TTM_RELAYEVENT   equ WM_USER + 7
TTM_GETTOOLINFO  equ WM_USER + 8
TTM_GETTOOLINFOW equ WM_USER + 53
TTM_SETTOOLINFO  equ WM_USER + 9
TTM_SETTOOLINFOW equ WM_USER + 54
TTM_HITTEST      equ WM_USER +10
TTM_HITTESTW     equ WM_USER +55
TTM_GETTEXT      equ WM_USER +11
TTM_GETTEXTW     equ WM_USER +56
TTM_UPDATETIPTEXT                equ WM_USER +12
TTM_UPDATETIPTEXTW               equ WM_USER +57
TTM_GETTOOLCOUNT equ WM_USER +13
TTM_ENUMTOOLS    equ WM_USER +14
TTM_ENUMTOOLSW   equ WM_USER +58
TTM_GETCURRENTTOOL               equ WM_USER + 15
TTM_GETCURRENTTOOLW              equ WM_USER + 59
TTM_WINDOWFROMPOINT              equ WM_USER + 16
TTN_NEEDTEXT     equ TTN_FIRST - 0
TTN_NEEDTEXTW    equ TTN_FIRST - 10
TTN_SHOW         equ TTN_FIRST - 1
TTN_POP          equ TTN_FIRST - 2
SBARS_SIZEGRIP   equ 0100h
SB_SETTEXT      equ WM_USER+1
SB_SETTEXTW     equ WM_USER+11
SB_GETTEXT      equ WM_USER+2
SB_GETTEXTW     equ WM_USER+13
SB_GETTEXTLENGTH                equ WM_USER+3
SB_GETTEXTLENGTHW               equ WM_USER+12
SB_SETPARTS      equ WM_USER+4
SB_GETPARTS      equ WM_USER+6
SB_GETBORDERS    equ WM_USER+7
SB_SETMINHEIGHT  equ WM_USER+8
SB_SIMPLE        equ WM_USER+9
SB_GETRECT       equ WM_USER+10
SBT_OWNERDRAW     equ 1000h
SBT_NOBORDERS     equ 0100h
SBT_POPOUT        equ 0200h
SBT_RTLREADING    equ 0400h
MINSYSCOMMAND           equ SC_SIZE
TBS_AUTOTICKS   equ 0001h
TBS_VERT        equ 0002h
TBS_HORZ        equ 0000h
TBS_TOP         equ 0004h
TBS_BOTTOM      equ 0000h
TBS_LEFT        equ 0004h
TBS_RIGHT       equ 0000h
TBS_BOTH        equ 0008h
TBS_NOTICKS     equ 0010h
TBS_ENABLESELRANGE              equ 0020h
TBS_FIXEDLENGTH equ 0040h
TBS_NOTHUMB     equ 0080h
TBM_GETPOS      equ WM_USER
TBM_GETRANGEMIN equ WM_USER+1
TBM_GETRANGEMAX equ WM_USER+2
TBM_GETTIC      equ WM_USER+3
TBM_SETTIC      equ WM_USER+4
TBM_SETPOS      equ WM_USER+5
TBM_SETRANGE    equ WM_USER+6
TBM_SETRANGEMIN equ WM_USER+7
TBM_SETRANGEMAX equ WM_USER+8
TBM_CLEARTICS   equ WM_USER+9
TBM_SETSEL      equ WM_USER+10
TBM_SETSELSTART equ WM_USER+11
TBM_SETSELEND   equ WM_USER+12
TBM_GETPTICS    equ WM_USER+14
TBM_GETTICPOS   equ WM_USER+15
TBM_GETNUMTICS  equ WM_USER+16
TBM_GETSELSTART equ WM_USER+17
TBM_GETSELEND   equ WM_USER+18
TBM_CLEARSEL    equ WM_USER+19
TBM_SETTICFREQ  equ WM_USER+20
TBM_SETPAGESIZE equ WM_USER+21
TBM_GETPAGESIZE equ WM_USER+22
TBM_SETLINESIZE equ WM_USER+23
TBM_GETLINESIZE equ WM_USER+24
TBM_GETTHUMBRECT                equ WM_USER+25
TBM_GETCHANNELRECT              equ WM_USER+26
TBM_SETTHUMBLENGTH              equ WM_USER+27
TBM_GETTHUMBLENGTH              equ WM_USER+28
TB_LINEUP       equ 0
TB_LINEDOWN     equ 1
TB_PAGEUP       equ 2
TB_PAGEDOWN     equ 3
TB_THUMBPOSITION                equ 4
TB_THUMBTRACK   equ 5
TB_TOP          equ 6
TB_BOTTOM       equ 7
TB_ENDTRACK     equ 8
DL_BEGINDRAG    equ WM_USER+133
DL_DRAGGING     equ WM_USER+134
DL_DROPPED      equ WM_USER+135
DL_CANCELDRAG   equ WM_USER+136
DL_CURSORSET    equ 0
DL_STOPCURSOR   equ 1
DL_COPYCURSOR   equ 2
DL_MOVECURSOR   equ 3
UD_MAXVAL       equ 7FFFh
UD_MINVAL       equ - UD_MAXVAL
UDS_WRAP        equ 0001h
UDS_SETBUDDYINT equ 0002h
UDS_ALIGNRIGHT  equ 0004h
UDS_ALIGNLEFT   equ 0008h
UDS_AUTOBUDDY   equ 0010h
UDS_ARROWKEYS   equ 0020h
UDS_HORZ        equ 0040h
UDS_NOTHOUSANDS equ 0080h
UDM_SETRANGE    equ WM_USER+101
UDM_GETRANGE    equ WM_USER+102
UDM_SETPOS      equ WM_USER+103
UDM_GETPOS      equ WM_USER+104
UDM_SETBUDDY    equ WM_USER+105
UDM_GETBUDDY    equ WM_USER+106
UDM_SETACCEL    equ WM_USER+107
UDM_GETACCEL    equ WM_USER+108
UDM_SETBASE     equ WM_USER+109
UDM_GETBASE     equ WM_USER+110
UDN_DELTAPOS          equ UDN_FIRST - 1
PBM_SETRANGE    equ WM_USER+1
PBM_SETPOS      equ WM_USER+2
PBM_DELTAPOS    equ WM_USER+3
PBM_SETSTEP     equ WM_USER+4
PBM_STEPIT      equ WM_USER+5
HOTKEYF_SHIFT   equ 01h
HOTKEYF_CONTROL equ 02h
HOTKEYF_ALT     equ 04h
HOTKEYF_EXT     equ 08h
HKCOMB_NONE     equ 0001h
HKCOMB_S        equ 0002h
HKCOMB_C        equ 0004h
HKCOMB_A        equ 0008h
HKCOMB_SC       equ 0010h
HKCOMB_SA       equ 0020h
HKCOMB_CA       equ 0040h
HKCOMB_SCA      equ 0080h
HKM_SETHOTKEY   equ WM_USER+1
HKM_GETHOTKEY   equ WM_USER+2
HKM_SETRULES    equ WM_USER+3
LVS_ICON        equ 0000h
LVS_REPORT      equ 0001h
LVS_SMALLICON   equ 0002h
LVS_LIST        equ 0003h
LVS_TYPEMASK    equ 0003h
LVS_SINGLESEL   equ 0004h
LVS_SHOWSELALWAYS               equ 0008h
LVS_SORTASCENDING               equ 0010h
LVS_SORTDESCENDING              equ 0020h
LVS_SHAREIMAGELISTS             equ 0040h
LVS_NOLABELWRAP equ 0080h
LVS_AUTOARRANGE equ 0100h
LVS_EDITLABELS  equ 0200h
LVS_NOSCROLL    equ 2000h
LVS_TYPESTYLEMASK               equ 0fc00h
LVS_ALIGNTOP    equ 0000h
LVS_ALIGNLEFT   equ 0800h
LVS_ALIGNMASK   equ 0c00h
LVS_OWNERDRAWFIXED              equ 0400h
LVS_NOCOLUMNHEADER              equ 4000h
LVS_NOSORTHEADER                equ 8000h
LVM_GETBKCOLOR  equ LVM_FIRST + 0
LVM_SETBKCOLOR  equ LVM_FIRST + 1
LVM_GETIMAGELIST                equ LVM_FIRST + 2
LVM_SUBITEMHITTEST              equ LVM_FIRST + 57
LVSIL_NORMAL    equ 0
LVSIL_SMALL     equ 1
LVSIL_STATE     equ 2
LVM_SETIMAGELIST                equ LVM_FIRST + 3
LVM_GETITEMCOUNT                equ LVM_FIRST + 4
LVIF_TEXT       equ 0001h
LVIF_IMAGE      equ 0002h
LVIF_PARAM      equ 0004h
LVIF_STATE      equ 0008h
LVIS_FOCUSED    equ 0001h
LVIS_SELECTED   equ 0002h
LVIS_CUT        equ 0004h
LVIS_DROPHILITED                equ 0008h
LVIS_OVERLAYMASK                equ 0F00h
LVIS_STATEIMAGEMASK             equ 0F000h
LPSTR_TEXTCALLBACKW             equ -1
LPSTR_TEXTCALLBACK              equ -1
I_IMAGECALLBACK equ -1
LVM_GETITEM     equ LVM_FIRST + 5
LVM_GETITEMW    equ LVM_FIRST + 75
LVM_SETITEM     equ LVM_FIRST + 6
LVM_SETITEMW    equ LVM_FIRST + 76
LVM_INSERTITEM  equ LVM_FIRST + 7
LVM_INSERTITEMW equ LVM_FIRST + 77
LVM_DELETEITEM  equ LVM_FIRST + 8
LVM_DELETEALLITEMS              equ LVM_FIRST + 9
LVM_GETCALLBACKMASK             equ LVM_FIRST + 10
LVM_SETCALLBACKMASK             equ LVM_FIRST + 11
LVNI_ALL        equ 0000h
LVNI_FOCUSED    equ 0001h
LVNI_SELECTED   equ 0002h
LVNI_CUT        equ 0004h
LVNI_DROPHILITED                equ 0008h
LVNI_ABOVE      equ 0100h
LVNI_BELOW      equ 0200h
LVNI_TOLEFT     equ 0400h
LVNI_TORIGHT    equ 0800h
LVM_GETNEXTITEM equ LVM_FIRST + 12
LVFI_PARAM      equ 0001h
LVFI_STRING     equ 0002h
LVFI_PARTIAL    equ 0008h
LVFI_WRAP       equ 0020h
LVFI_NEARESTXY  equ 0040h
LVM_FINDITEM    equ LVM_FIRST + 13
LVM_FINDITEMW   equ LVM_FIRST + 83
LVIR_BOUNDS     equ 0
LVIR_ICON       equ 1
LVIR_LABEL      equ 2
LVIR_SELECTBOUNDS               equ 3
LVM_GETITEMRECT equ LVM_FIRST + 14
LVM_SETITEMPOSITION             equ LVM_FIRST + 15
LVM_GETITEMPOSITION             equ LVM_FIRST + 16
LVM_GETSTRINGWIDTH              equ LVM_FIRST + 17
LVM_GETSTRINGWIDTHW             equ LVM_FIRST + 87
LVHT_NOWHERE    equ 0001h
LVHT_ONITEMICON equ 0002h
LVHT_ONITEMLABEL                equ 0004h
LVHT_ONITEMSTATEICON            equ 0008h
LVHT_ONITEM     equ LVHT_ONITEMICON OR LVHT_ONITEMLABEL OR LVHT_ONITEMSTATEICON
LVHT_ABOVE      equ 0008h
LVHT_BELOW      equ 0010h
LVHT_TORIGHT    equ 0020h
LVHT_TOLEFT     equ 0040h
LVM_HITTEST     equ LVM_FIRST + 18
LVM_ENSUREVISIBLE               equ LVM_FIRST + 19
LVM_SCROLL      equ LVM_FIRST + 20
LVM_REDRAWITEMS equ LVM_FIRST + 21
LVA_DEFAULT     equ 0000h
LVA_ALIGNLEFT   equ 0001h
LVA_ALIGNTOP    equ 0002h
LVA_SNAPTOGRID  equ 0005h
LVM_ARRANGE     equ LVM_FIRST + 22
LVM_EDITLABEL   equ LVM_FIRST + 23
LVM_EDITLABELW  equ LVM_FIRST + 118
LVM_GETEDITCONTROL              equ LVM_FIRST + 24
LVCF_FMT        equ 0001h
LVCF_WIDTH      equ 0002h
LVCF_TEXT       equ 0004h
LVCF_SUBITEM    equ 0008h
LVCFMT_LEFT     equ 0000h
LVCFMT_RIGHT    equ 0001h
LVCFMT_CENTER   equ 0002h
LVCFMT_JUSTIFYMASK              equ 0003h
LVM_GETCOLUMN   equ LVM_FIRST + 25
LVM_GETCOLUMNW  equ LVM_FIRST + 95
LVM_SETCOLUMN   equ LVM_FIRST + 26
LVM_SETCOLUMNW  equ LVM_FIRST + 96
LVM_INSERTCOLUMN                equ LVM_FIRST + 27
LVM_INSERTCOLUMNW               equ LVM_FIRST + 97
LVM_DELETECOLUMN                equ LVM_FIRST + 28
LVM_GETCOLUMNWIDTH              equ LVM_FIRST + 29
LVSCW_AUTOSIZE      equ -1
LVSCW_AUTOSIZE_USEHEADER            equ -2
LVM_SETCOLUMNWIDTH  equ LVM_FIRST + 30
LVM_CREATEDRAGIMAGE             equ LVM_FIRST + 33
LVM_GETVIEWRECT equ LVM_FIRST + 34
LVM_GETTEXTCOLOR                equ LVM_FIRST + 35
LVM_SETTEXTCOLOR                equ LVM_FIRST + 36
LVM_GETTEXTBKCOLOR              equ LVM_FIRST + 37
LVM_SETTEXTBKCOLOR              equ LVM_FIRST + 38
LVM_GETTOPINDEX equ LVM_FIRST + 39
LVM_GETCOUNTPERPAGE             equ LVM_FIRST + 40
LVM_GETORIGIN   equ LVM_FIRST + 41
LVM_UPDATE      equ LVM_FIRST + 42
LVM_SETITEMSTATE                equ LVM_FIRST + 43
LVM_GETITEMSTATE                equ LVM_FIRST + 44
LVM_GETITEMTEXT equ LVM_FIRST + 45
LVM_GETITEMTEXTW                equ LVM_FIRST + 115
LVM_SETITEMTEXT equ LVM_FIRST + 46
LVM_SETITEMTEXTW                equ LVM_FIRST + 116
LVM_SETITEMCOUNT                equ LVM_FIRST + 47
LVM_SORTITEMS   equ LVM_FIRST + 48
LVM_SETITEMPOSITION32           equ LVM_FIRST + 49
LVM_GETSELECTEDCOUNT            equ LVM_FIRST + 50
LVM_GETITEMSPACING              equ LVM_FIRST + 51
LVM_GETISEARCHSTRING            equ LVM_FIRST + 52
LVM_GETISEARCHSTRINGW           equ LVM_FIRST + 117
LVN_ITEMCHANGING                equ LVN_FIRST-0
LVN_ITEMCHANGED equ LVN_FIRST-1
LVN_INSERTITEM  equ LVN_FIRST-2
LVN_DELETEITEM  equ LVN_FIRST-3
LVN_DELETEALLITEMS              equ LVN_FIRST-4
LVN_BEGINLABELEDIT              equ LVN_FIRST-5
LVN_BEGINLABELEDITW             equ LVN_FIRST-75
LVN_ENDLABELEDIT                equ LVN_FIRST-6
LVN_ENDLABELEDITW               equ LVN_FIRST-76
LVN_COLUMNCLICK equ LVN_FIRST-8
LVN_BEGINDRAG   equ LVN_FIRST-9
LVN_BEGINRDRAG  equ LVN_FIRST-11
LVN_GETDISPINFO equ LVN_FIRST-50
LVN_GETDISPINFOW                equ LVN_FIRST-77
LVN_SETDISPINFO equ LVN_FIRST-51
LVN_SETDISPINFOW                equ LVN_FIRST-78
LVIF_DI_SETITEM equ 1000h
LVN_KEYDOWN     equ LVN_FIRST-55
TVS_HASBUTTONS  equ 0001h
TVS_HASLINES    equ 0002h
TVS_LINESATROOT equ 0004h
TVS_EDITLABELS  equ 0008h
TVS_DISABLEDRAGDROP             equ 0010h
TVS_SHOWSELALWAYS               equ 0020h
TVIF_TEXT       equ 0001h
TVIF_IMAGE      equ 0002h
TVIF_PARAM      equ 0004h
TVIF_STATE      equ 0008h
TVIF_HANDLE     equ 0010h
TVIF_SELECTEDIMAGE              equ 0020h
TVIF_CHILDREN   equ 0040h
TVIS_FOCUSED    equ 0001h
TVIS_SELECTED   equ 0002h
TVIS_CUT        equ 0004h
TVIS_DROPHILITED                equ 0008h

TVIS_BOLD       equ 0010h
TVIS_EXPANDED   equ 0020h
TVIS_EXPANDEDONCE               equ 0040h
TVIS_OVERLAYMASK                equ 0F00h
TVIS_STATEIMAGEMASK             equ 0F000h
TVIS_USERMASK   equ 0F000h
I_CHILDRENCALLBACK              equ -1
TVI_ROOT        equ 0FFFF0000h
TVI_FIRST       equ 0FFFF0001h
TVI_LAST        equ 0FFFF0002h
TVI_SORT        equ 0FFFF0003h
TVM_INSERTITEM  equ TV_FIRST + 0
TVM_INSERTITEMW equ TV_FIRST + 50
TVM_DELETEITEM  equ TV_FIRST + 1
TVM_EXPAND      equ TV_FIRST + 2
TVE_COLLAPSE    equ 0001h
TVE_EXPAND      equ 0002h
TVE_TOGGLE      equ 0003h
TVE_COLLAPSERESET               equ 8000h
TVM_GETITEMRECT equ TV_FIRST + 4
TVM_GETCOUNT    equ TV_FIRST + 5
TVM_GETINDENT   equ TV_FIRST + 6
TVM_SETINDENT   equ TV_FIRST + 7
TVM_GETIMAGELIST                equ TV_FIRST + 8
TVSIL_NORMAL    equ 0
TVSIL_STATE     equ 2
TVM_SETIMAGELIST                equ TV_FIRST + 9
TVM_GETNEXTITEM equ TV_FIRST + 10
TVGN_ROOT       equ 0000h
TVGN_NEXT       equ 0001h
TVGN_PREVIOUS   equ 0002h
TVGN_PARENT     equ 0003h
TVGN_CHILD      equ 0004h
TVGN_FIRSTVISIBLE               equ 0005h
TVGN_NEXTVISIBLE                equ 0006h
TVGN_PREVIOUSVISIBLE            equ 0007h
TVGN_DROPHILITE equ 0008h
TVGN_CARET      equ 0009h
TVM_SELECTITEM  equ TV_FIRST + 11
TVM_GETITEM     equ TV_FIRST + 12
TVM_GETITEMW    equ TV_FIRST + 62
TVM_SETITEM     equ TV_FIRST + 13
TVM_SETITEMW    equ TV_FIRST + 63
TVM_EDITLABEL   equ TV_FIRST + 14
TVM_EDITLABELW  equ TV_FIRST + 65
TVM_GETEDITCONTROL              equ TV_FIRST + 15
TVM_GETVISIBLECOUNT             equ TV_FIRST + 16
TVM_HITTEST     equ TV_FIRST + 17
TVHT_NOWHERE    equ 0001h
TVHT_ONITEMICON equ 0002h
TVHT_ONITEMLABEL                equ 0004h
TVHT_ONITEMSTATEICON            equ 0040h
TVHT_ONITEM     equ TVHT_ONITEMICON OR TVHT_ONITEMLABEL OR TVHT_ONITEMSTATEICON
TVHT_ONITEMINDENT               equ 0008h
TVHT_ONITEMBUTTON               equ 0010h
TVHT_ONITEMRIGHT                equ 0020h
TVHT_ABOVE      equ 0100h
TVHT_BELOW      equ 0200h
TVHT_TORIGHT    equ 0400h
TVHT_TOLEFT     equ 0800h
TVM_CREATEDRAGIMAGE             equ TV_FIRST + 18
TVM_SORTCHILDREN                equ TV_FIRST + 19
TVM_ENSUREVISIBLE               equ TV_FIRST + 20
TVM_SORTCHILDRENCB              equ TV_FIRST + 21
TVM_GETISEARCHSTRING            equ TV_FIRST + 23
TVN_SELCHANGING                equ TVN_FIRST-1
TVN_SELCHANGED equ TVN_FIRST-2
TVC_UNKNOWN     equ 0000h
TVC_BYMOUSE     equ 0001h
TVC_BYKEYBOARD  equ 0002h
TVN_GETDISPINFO                equ TVN_FIRST-3
TVN_SETDISPINFO                equ TVN_FIRST-4
TVIF_DI_SETITEM equ 1000h
TVN_ITEMEXPANDING               equ TVN_FIRST-5
TVN_ITEMEXPANDED                equ TVN_FIRST-6
TVN_BEGINDRAG   equ TVN_FIRST-7
TVN_BEGINRDRAG  equ TVN_FIRST-8
TVN_DELETEITEM  equ TVN_FIRST-9
TVN_BEGINLABELEDIT              equ TVN_FIRST-10
TVN_ENDLABELEDIT                equ TVN_FIRST-11
TVN_KEYDOWN     equ TVN_FIRST-12
TCS_FORCEICONLEFT                equ 0010h
TCS_FORCELABELLEFT               equ 0020h
TCS_TABS         equ 0000h
TCS_BUTTONS      equ 0100h
TCS_SINGLELINE   equ 0000h
TCS_MULTILINE    equ 0200h
TCS_RIGHTJUSTIFY equ 0000h
TCS_FIXEDWIDTH   equ 0400h
TCS_RAGGEDRIGHT  equ 0800h
TCS_FOCUSONBUTTONDOWN            equ 1000h
TCS_OWNERDRAWFIXED               equ 2000h
TCS_TOOLTIPS     equ 4000h
TCS_FOCUSNEVER   equ 8000h
TCM_FIRST        equ 1300h
TCM_GETIMAGELIST equ TCM_FIRST + 2
TCM_SETIMAGELIST                equ TCM_FIRST + 3
TCM_GETITEMCOUNT equ TCM_FIRST + 4
TCIF_TEXT        equ 0001h
TCIF_IMAGE       equ 0002h
TCIF_RTLREADING  equ 0004h
TCIF_PARAM       equ 0008h
TCM_GETITEM      equ TCM_FIRST + 5
TCM_SETITEM     equ TCM_FIRST + 6
TCM_SETITEMW    equ TCM_FIRST + 61
TCM_INSERTITEM  equ TCM_FIRST + 7
TCM_INSERTITEMW equ TCM_FIRST + 62
TCM_DELETEITEM   equ TCM_FIRST + 8
TCM_DELETEALLITEMS               equ TCM_FIRST + 9
TCM_GETITEMRECT  equ TCM_FIRST + 10
TCM_GETCURSEL    equ TCM_FIRST + 11
TCM_SETCURSEL    equ TCM_FIRST + 12
TCHT_NOWHERE     equ 0001h
TCHT_ONITEMICON  equ 0002h
TCHT_ONITEMLABEL equ 0004h
TCHT_ONITEM      equ TCHT_ONITEMICON OR TCHT_ONITEMLABEL
TCM_HITTEST      equ TCM_FIRST + 13
TCM_SETITEMEXTRA equ TCM_FIRST + 14
TCM_ADJUSTRECT   equ TCM_FIRST + 40
TCM_SETITEMSIZE  equ TCM_FIRST + 41
TCM_REMOVEIMAGE  equ TCM_FIRST + 42
TCM_SETPADDING   equ TCM_FIRST + 43
TCM_GETROWCOUNT  equ TCM_FIRST + 44
TCM_GETTOOLTIPS  equ TCM_FIRST + 45
TCM_SETTOOLTIPS  equ TCM_FIRST + 46
TCM_GETCURFOCUS  equ TCM_FIRST + 47
TCM_SETCURFOCUS  equ TCM_FIRST + 48
TCN_KEYDOWN      equ TCN_FIRST - 0
TCN_SELCHANGE    equ TCN_FIRST - 1
TCN_SELCHANGING  equ TCN_FIRST - 2
ACS_CENTER      equ 0001h
ACS_TRANSPARENT equ 0002h
ACS_AUTOPLAY    equ 0004h
ACM_OPEN        equ WM_USER+100
ACM_OPENW       equ WM_USER+103
ACM_PLAY        equ WM_USER+101
ACM_STOP        equ WM_USER+102
ACN_START       equ 1
ACN_STOP        equ 2

FXPT2DOT30      equ  <DWORD>
LCSCSTYPE       equ  <DWORD>
LCSGAMUTMATCH   equ  <DWORD>
MMVERSION       equ  <DWORD>
SECURITY_CONTEXT_TRACKING_MODE  equ  <BOOLEAN>
MAX_JOYSTICKOEMVXDNAME equ 260
FOURCC          equ  <DWORD>
MCIDEVICEID TYPEDEF DWORD
ACCESS_MASK TYPEDEF DWORD
UCHAR TYPEDEF BYTE
FILEOP_FLAGS TYPEDEF WORD
WCHAR TYPEDEF WORD

DONT_RESOLVE_DLL_REFERENCES     equ 1
LOAD_LIBRARY_AS_DATAFILE        equ 2
LOAD_WITH_ALTERED_SEARCH_PATH   equ 8

EVENT_MODIFY_STATE      equ 2
;-------------------------- wsock32 equates -------------------------------

WSADESCRIPTION_LEN          equ 256
WSASYS_STATUS_LEN           equ 128
IPPROTO_IP          equ             0
IPPROTO_ICMP          equ           1
IPPROTO_GGP          equ            2
IPPROTO_TCP          equ            6
IPPROTO_PUP          equ            12
IPPROTO_UDP          equ            17
IPPROTO_IDP          equ            22
IPPROTO_ND          equ             77
IPPROTO_RAW          equ            255
IPPROTO_MAX          equ            256
IOCPARM_MASK           equ 7Fh
IOC_VOID               equ 20000000h
IOC_OUT                equ 40000000h
IOC_IN equ 80000000h
IOC_INOUT              equ IOC_IN OR IOC_OUT
FIONBIO                equ 8004667Eh
FIONSYNC               equ 8004667Dh
FIONREAD               equ 4004667Fh
IPPORT_ECHO          equ            7
IPPORT_DISCARD          equ         9
IPPORT_SYSTAT          equ          11
IPPORT_DAYTIME          equ         13
IPPORT_NETSTAT          equ         15
IPPORT_FTP          equ             21
IPPORT_TELNET          equ          23
IPPORT_SMTP          equ            25
IPPORT_TIMESERVER          equ      37
IPPORT_NAMESERVER          equ      42
IPPORT_WHOIS          equ           43
IPPORT_MTP          equ             57
IPPORT_TFTP          equ            69
IPPORT_RJE          equ             77
IPPORT_FINGER          equ          79
IPPORT_TTYLINK          equ         87
IPPORT_SUPDUP          equ          95
IPPORT_EXECSERVER          equ      512
IPPORT_LOGINSERVER          equ     513
IPPORT_CMDSERVER          equ       514
IPPORT_EFSSERVER          equ       520
IPPORT_BIFFUDP          equ         512
IPPORT_WHOSERVER          equ       513
IPPORT_ROUTESERVER          equ     520
IPPORT_RESERVED          equ        1024
IMPLINK_IP          equ             155
IMPLINK_LOWEXPER          equ       156
IMPLINK_HIGHEXPER          equ      158
IN_CLASSA_NET          equ          0FF000000h
IN_CLASSA_NSHIFT          equ       24
IN_CLASSA_HOST          equ         000FFFFFFh
IN_CLASSA_MAX          equ          128
IN_CLASSB_NET          equ          0FFFF0000h
IN_CLASSB_NSHIFT          equ       16
IN_CLASSB_HOST          equ         00000FFFFh
IN_CLASSB_MAX          equ          65536
IN_CLASSC_NET          equ          0FFFFFF00h
IN_CLASSC_NSHIFT          equ       8
IN_CLASSC_HOST          equ         0000000FFh
INADDR_ANY          equ             000000000h
INADDR_LOOPBACK          equ        07F000001h
INADDR_BROADCAST          equ       0FFFFFFFFh
INADDR_NONE          equ            0FFFFFFFFh
SOCK_STREAM          equ    1
SOCK_DGRAM          equ     2

SOCK_RAW          equ       3
SOCK_RDM          equ       4
SOCK_SEQPACKET          equ 5
SO_DEBUG          equ       00001h
SO_ACCEPTCONN          equ  00002h
SO_REUSEADDR          equ   00004h
SO_KEEPALIVE          equ   00008h
SO_DONTROUTE          equ   00010h
SO_BROADCAST          equ   00020h
SO_USELOOPBACK          equ 00040h
SO_LINGER          equ      00080h
SO_OOBINLINE          equ   00100h
SOL_SOCKET            equ   0FFFFh
SO_DONTLINGER          equ  NOT SO_LINGER
SO_SNDBUF          equ      01001h
SO_RCVBUF          equ      01002h
SO_SNDLOWAT          equ    01003h
SO_RCVLOWAT          equ    01004h
SO_SNDTIMEO          equ    01005h
SO_RCVTIMEO          equ    01006h
SO_ERROR          equ       01007h
SO_TYPE          equ        01008h
TCP_NODELAY          equ    00001h
AF_UNSPEC          equ      0
AF_UNIX          equ        1
AF_INET          equ        2
AF_IMPLINK          equ     3
AF_PUP          equ         4
AF_CHAOS          equ       5
AF_NS          equ          6
AF_IPX          equ         6
AF_ISO          equ         7
AF_OSI          equ         AF_ISO
AF_ECMA          equ        8
AF_DATAKIT          equ     9
AF_CCITT          equ       10
AF_SNA          equ         11
AF_DECnet          equ      12
AF_DLI          equ         13
AF_LAT          equ         14
AF_HYLINK          equ      15
AF_APPLETALK          equ   16
AF_NETBIOS          equ     17
AF_MAX          equ         18
PF_UNSPEC          equ      AF_UNSPEC
PF_UNIX          equ        AF_UNIX
PF_INET          equ        AF_INET
PF_IMPLINK          equ     AF_IMPLINK
PF_PUP          equ         AF_PUP
PF_CHAOS          equ       AF_CHAOS
PF_NS          equ          AF_NS
PF_IPX          equ         AF_IPX
PF_ISO          equ         AF_ISO
PF_OSI          equ         AF_OSI
PF_ECMA          equ        AF_ECMA
PF_DATAKIT          equ     AF_DATAKIT
PF_CCITT          equ       AF_CCITT
PF_SNA          equ         AF_SNA
PF_DECnet          equ      AF_DECnet
PF_DLI          equ         AF_DLI
PF_LAT          equ         AF_LAT
PF_HYLINK          equ      AF_HYLINK
PF_APPLETALK          equ   AF_APPLETALK
PF_MAX          equ         AF_MAX
SOMAXCONN          equ      5
MSG_OOB          equ        01h
MSG_PEEK          equ   02h
MSG_DONTROUTE          equ  04h
MSG_MAXIOVLEN          equ  16
MAXGETHOSTSTRUCT          equ       1024
FD_READ          equ        001h
FD_WRITE          equ       002h
FD_OOB          equ         004h
FD_ACCEPT          equ      008h
FD_CONNECT          equ     010h
FD_CLOSE          equ       020h
WSABASEERR          equ             10000
WSAEINTR          equ               WSABASEERR+4
WSAEBADF          equ               WSABASEERR+9
WSAEACCES          equ              WSABASEERR+13
WSAEFAULT          equ              WSABASEERR+14
WSAEINVAL          equ              WSABASEERR+22
WSAEMFILE          equ              WSABASEERR+24
WSAEWOULDBLOCK          equ         WSABASEERR + 35
WSAEINPROGRESS          equ         WSABASEERR + 36
WSAEALREADY          equ            WSABASEERR + 37
WSAENOTSOCK          equ            WSABASEERR + 38
WSAEDESTADDRREQ          equ        WSABASEERR + 39
WSAEMSGSIZE          equ            WSABASEERR + 40
WSAEPROTOTYPE          equ          WSABASEERR + 41
WSAENOPROTOOPT          equ         WSABASEERR + 42
WSAEPROTONOSUPPORT          equ     WSABASEERR + 43
WSAESOCKTNOSUPPORT          equ     WSABASEERR + 44
WSAEOPNOTSUPP          equ          WSABASEERR + 45
WSAEPFNOSUPPORT          equ        WSABASEERR + 46
WSAEAFNOSUPPORT          equ        WSABASEERR + 47
WSAEADDRINUSE          equ          WSABASEERR + 48
WSAEADDRNOTAVAIL          equ       WSABASEERR + 49
WSAENETDOWN          equ            WSABASEERR + 50
WSAENETUNREACH          equ         WSABASEERR + 51
WSAENETRESET          equ           WSABASEERR + 52
WSAECONNABORTED          equ        WSABASEERR + 53
WSAECONNRESET          equ          WSABASEERR + 54
WSAENOBUFS          equ             WSABASEERR + 55
WSAEISCONN          equ             WSABASEERR + 56
WSAENOTCONN          equ            WSABASEERR + 57
WSAESHUTDOWN          equ           WSABASEERR + 58
WSAETOOMANYREFS          equ        WSABASEERR + 59
WSAETIMEDOUT          equ           WSABASEERR + 60
WSAECONNREFUSED          equ        WSABASEERR + 61
WSAELOOP          equ               WSABASEERR + 62
WSAENAMETOOLONG          equ        WSABASEERR + 63
WSAEHOSTDOWN          equ           WSABASEERR + 64
WSAEHOSTUNREACH          equ        WSABASEERR + 65
WSAENOTEMPTY          equ           WSABASEERR + 66
WSAEPROCLIM          equ            WSABASEERR + 67
WSAEUSERS          equ              WSABASEERR + 68
WSAEDQUOT          equ              WSABASEERR + 69
WSAESTALE          equ              WSABASEERR + 70
WSAEREMOTE          equ             WSABASEERR + 71
WSASYSNOTREADY          equ         WSABASEERR+91
WSAVERNOTSUPPORTED          equ     WSABASEERR+92
WSANOTINITIALISED          equ      WSABASEERR+93
WSAHOST_NOT_FOUND          equ      WSABASEERR + 1001
HOST_NOT_FOUND          equ         WSAHOST_NOT_FOUND
WSATRY_AGAIN          equ           WSABASEERR + 1002
TRY_AGAIN          equ              WSATRY_AGAIN
WSANO_RECOVERY          equ         WSABASEERR + 1003
NO_RECOVERY          equ            WSANO_RECOVERY
WSANO_DATA          equ             WSABASEERR + 1004
NO_DATA          equ                WSANO_DATA
WSANO_ADDRESS          equ          WSANO_DATA
NO_ADDRESS          equ             WSANO_ADDRESS
EWOULDBLOCK          equ            WSAEWOULDBLOCK
EINPROGRESS          equ            WSAEINPROGRESS
EALREADY          equ               WSAEALREADY
ENOTSOCK          equ               WSAENOTSOCK
EDESTADDRREQ          equ           WSAEDESTADDRREQ
EMSGSIZE          equ               WSAEMSGSIZE
EPROTOTYPE          equ             WSAEPROTOTYPE
ENOPROTOOPT          equ            WSAENOPROTOOPT
EPROTONOSUPPORT          equ        WSAEPROTONOSUPPORT
ESOCKTNOSUPPORT          equ        WSAESOCKTNOSUPPORT

EOPNOTSUPP          equ             WSAEOPNOTSUPP
EPFNOSUPPORT          equ           WSAEPFNOSUPPORT
EAFNOSUPPORT          equ           WSAEAFNOSUPPORT
EADDRINUSE          equ             WSAEADDRINUSE
EADDRNOTAVAIL          equ          WSAEADDRNOTAVAIL
ENETDOWN          equ               WSAENETDOWN
ENETUNREACH          equ            WSAENETUNREACH
ENETRESET          equ              WSAENETRESET
ECONNABORTED          equ           WSAECONNABORTED
ECONNRESET          equ             WSAECONNRESET
ENOBUFS          equ                WSAENOBUFS
EISCONN          equ                WSAEISCONN
ENOTCONN          equ               WSAENOTCONN
ESHUTDOWN          equ              WSAESHUTDOWN
ETOOMANYREFS          equ           WSAETOOMANYREFS
ETIMEDOUT          equ              WSAETIMEDOUT
ECONNREFUSED          equ           WSAECONNREFUSED
ELOOP          equ                  WSAELOOP
ENAMETOOLONG          equ           WSAENAMETOOLONG
EHOSTDOWN          equ              WSAEHOSTDOWN
EHOSTUNREACH          equ           WSAEHOSTUNREACH
ENOTEMPTY          equ              WSAENOTEMPTY
EPROCLIM          equ               WSAEPROCLIM
EUSERS          equ                 WSAEUSERS
EDQUOT          equ                 WSAEDQUOT
ESTALE          equ                 WSAESTALE
EREMOTE          equ                WSAEREMOTE
FD_SETSIZE  equ 64
INVALID_SOCKET              equ NOT 0
SOCKET_ERROR                equ -1
SOCKET_BUFFER_SIZE          equ 512
ICMP_ECHOREPLY          equ   0
ICMP_ECHOREQ            equ   8
SOCKET TYPEDEF DWORD
IMAGE_SIZEOF_SECTION_HEADER equ 40
;----------------------------------- wtypes.h-------------------------
HDWP TYPEDEF HANDLE
HFILE TYPEDEF DWORD
CHAR TYPEDEF BYTE
TCHAR TYPEDEF WCHAR
OLECHAR TYPEDEF CHAR
LPOLESTR TYPEDEF LPSTR
LPCOLESTR TYPEDEF LPCSTR
FLOAT TYPEDEF DWORD
USHORT TYPEDEF WORD
ULONG TYPEDEF DWORD
DOUBLE TYPEDEF QWORD

; ------------------------- comctl structures ------------------------------

POINT STRUCT
  x  DWORD ?
  y  DWORD ?

POINT ENDS

POINTS STRUCT
  x WORD ?
  y WORD ?
POINTS ENDS

POINTL  equ  <POINT>

RECT STRUCT
  left    dd      ?
  top     dd      ?
  right   dd      ?
  bottom  dd      ?
RECT ENDS

INITCOMMONCONTROLSEX STRUCT
  dwSize  dd      ?
  dwICC   dd      ?
INITCOMMONCONTROLSEX ENDS

REBARINFO STRUCT
  cbSize    dd      ?
  fMask     dd     ?
  himl      dd      ?
REBARINFO ENDS

REBARBANDINFO STRUCT
  cbSize        DWORD      ?
  fMask         DWORD      ?
  fStyle        DWORD      ?
  clrFore       DWORD      ?
  clrBack       DWORD      ?
  lpText        DWORD      ?
  cch           DWORD      ?
  iImage        DWORD      ?
  hwndChild     DWORD      ?
  cxMinChild    DWORD      ?
  cyMinChild    DWORD      ?
  lx            DWORD      ?
  hbmBack       DWORD      ?
  wID           DWORD      ?
  cyChild       DWORD      ?
  cyMaxChild    DWORD      ?
  cyIntegral    DWORD      ?
  cxIdeal       DWORD      ?
  lParam        DWORD      ?
  cxHeader      DWORD      ?
REBARBANDINFO ENDS

NMHDR STRUCT
    hwndFrom    DWORD ?
    idFrom      DWORD ?
    code        DWORD ?
NMHDR ends

NMREBARCHILDSIZE STRUCT
  hdr       NMHDR       <>
  uBand     DWORD       ?
  wID       DWORD       ?
  rcChild   RECT        <>
  rcBand    RECT        <>
NMREBARCHILDSIZE ENDS

NMREBAR STRUCT
  hdr       NMHDR     <>
  dwMask    DWORD      ?
  uBand     DWORD      ?
  fStyle    DWORD      ?
  wID       DWORD      ?
  lParam    DWORD      ?
NMREBAR ENDS

NMRBAUTOSIZE STRUCT
  hdr  NMHDR      <>
  fChanged  DWORD      ?
  rcTarget  RECT      <>
  rcActual  RECT      <>
NMRBAUTOSIZE ENDS

RBHITTESTINFO STRUCT
  pt     POINT      <>
  flags  DWORD      ?
  iBand  DWORD      ?
RBHITTESTINFO ENDS

IMAGEINFO STRUCT
  hbmImage   HBITMAP      ?
  hbmMask    HBITMAP      ?
  Unused1    DWORD        ?
  Unused2    DWORD        ?
  rcImage    RECT         <>
IMAGEINFO ENDS

HD_ITEM STRUCT
  imask          DWORD      ?
  lxy            DWORD      ?

  pszText        DWORD      ?
  hbm            DWORD      ?
  cchTextMax     DWORD      ?
  fmt            DWORD      ?
  lParam         DWORD      ?
  iImage         DWORD ?
  iOrder         DWORD ?
  itype dd ?
  pvFilter dd ?
HD_ITEM ENDS

HD_LAYOUT STRUCT
  prc    DWORD      ?
  pwpos  DWORD      ?
HD_LAYOUT ENDS

HD_HITTESTINFO STRUCT
  pt         POINT      <>
  flags      DWORD      ?
  iItem      DWORD      ?
HD_HITTESTINFO ENDS

NMHEADER STRUCT
    hdr NMHDR   <>
    iItem DWORD ?
    iButton DWORD ?
    pitem DWORD ?
NMHEADER ends

HD_NOTIFY  equ  <NMHEADER>

TBBUTTON STRUCT
  iBitmap           DWORD      ?
  idCommand         DWORD      ?
  fsState           BYTE       ?
  fsStyle           BYTE       ?
  _wPad1            WORD       ?
  dwData            DWORD      ?
  iString           DWORD      ?
TBBUTTON ENDS

COLORMAP STRUCT
  From       DWORD      ?
  To         DWORD      ?
COLORMAP ENDS

TBADDBITMAP STRUCT
  hInst          DWORD      ?
  nID            DWORD      ?
TBADDBITMAP ENDS

TBSAVEPARAMS STRUCT
  hkr  DWORD      ?
  pszSubKey  DWORD      ?
  pszValueName  DWORD      ?
TBSAVEPARAMS ENDS

TBREPLACEBITMAP STRUCT
  hInstOld           DWORD      ?
  nIdOld             DWORD      ?
  hInstNew           DWORD      ?
  nIdNew             DWORD      ?
  nButtons           DWORD      ?
TBREPLACEBITMAP ENDS


NMTOOLBAR STRUCT
  hdr            NMHDR      <>
  iItem          DWORD      ?
  tbButton       TBBUTTON   <>
  cchText        DWORD      ?
  pszText        DWORD      ?
NMTOOLBAR ENDS

TBNOTIFY  equ  <NMTOOLBAR>

TOOLINFO STRUCT
  cbSize             DWORD      ?
  uFlags             DWORD      ?
  hWnd               DWORD      ?
  uId                DWORD      ?
  rect               RECT      <>
  hInst              DWORD      ?
  lpszText           DWORD      ?
  lParam             LPARAM     ?
TOOLINFO ENDS

TT_HITTESTINFO STRUCT
  hWnd          DWORD      ?
  pt            POINT      <>
  ti            TOOLINFO   <>
TT_HITTESTINFO ENDS

NMTTDISPINFO STRUCT
  hdr           NMHDR      <>
  lpszText      DWORD      ?
  szText        BYTE 80 dup (?)
  union
  	hInst  dd  ?
  	hinst dd ?
  ends
  uFlags        DWORD      ?
  lParam        LPARAM ?
NMTTDISPINFO ENDS

TOOLTIPTEXT  equ  <NMTTDISPINFO>

DRAGLISTINFO STRUCT
  uNotification     DWORD      ?
  hWnd              DWORD      ?
  ptCursor          POINT  <>
DRAGLISTINFO ENDS

UDACCEL STRUCT
  nSec  DWORD      ?
  nInc  DWORD      ?
UDACCEL ENDS

NM_UPDOWN STRUCT
  hdr       NMHDR      <>
  iPos      DWORD      ?
  iDelta    DWORD      ?
NM_UPDOWN ENDS

NMUPDOWN  equ  <NM_UPDOWN>

LV_ITEM STRUCT
  imask         DWORD      ?
  iItem         DWORD      ?
  iSubItem      DWORD      ?
  state         DWORD      ?
  stateMask     DWORD      ?
  pszText       DWORD      ?
  cchTextMax    DWORD      ?
  iImage        DWORD      ?
  lParam        DWORD      ?
  iIndent       DWORD        ?
LV_ITEM ENDS

LVITEM  equ  <LV_ITEM>

LV_FINDINFO STRUCT
  flags         DWORD      ?
  psz           DWORD      ?
  lParam        DWORD      ?
  pt            POINT      <>
  vkDirection   DWORD      ?
LV_FINDINFO ENDS

LVFINDINFO  equ  <LV_FINDINFO>

LV_HITTESTINFO STRUCT
  pt        POINT      <>
  flags     DWORD      ?
  iItem     DWORD      ?
  iSubItem  DWORD  ?
LV_HITTESTINFO ENDS

LVHITTESTINFO  equ  <LV_HITTESTINFO>

LV_COLUMN STRUCT
  imask         DWORD      ?
  fmt           DWORD      ?
  lx            DWORD      ?
  pszText       DWORD      ?
  cchTextMax    DWORD      ?
  iSubItem      DWORD      ?
  iImage        DWORD ?
  iOrder        DWORD ?
LV_COLUMN ENDS

LVCOLUMN  equ  <LV_COLUMN>

NM_LISTVIEW STRUCT
  hdr           NMHDR      <>
  iItem         DWORD      ?
  iSubItem      DWORD      ?
  uNewState     DWORD      ?
  uOldState     DWORD      ?
  uChanged      DWORD      ?
  ptAction      POINT      <>
  lParam        DWORD      ?
NM_LISTVIEW ENDS

NMLISTVIEW  equ  <NM_LISTVIEW>

LV_DISPINFO STRUCT
  hdr   NMHDR      <>
  item  LVITEM     <>
LV_DISPINFO ENDS

LV_KEYDOWN STRUCT
  hdr       NMHDR      <>
  wVKey     WORD      ?
  flags     DWORD      ?
LV_KEYDOWN ENDS

NMLVKEYDOWN  equ  <LV_KEYDOWN>

TV_ITEM STRUCT
  imask             DWORD      ?
  hItem             DWORD      ?
  state             DWORD      ?
  stateMask         DWORD      ?
  pszText           DWORD      ?
  cchTextMax        DWORD      ?
  iImage            DWORD      ?
  iSelectedImage    DWORD      ?
  cChildren         DWORD      ?
  lParam            DWORD      ?
TV_ITEM ENDS

TVITEM  equ  <TV_ITEM>

TV_ITEMEX STRUCT
  imask             DWORD      ?
  hItem             DWORD      ?
  state             DWORD      ?
  stateMask         DWORD      ?
  pszText           DWORD      ?
  cchTextMax        DWORD      ?
  iImage            DWORD      ?
  iSelectedImage    DWORD      ?
  cChildren         DWORD      ?
  lParam            DWORD      ?
  iIntegral         DWORD      ?
TV_ITEMEX ENDS

TVITEMEX  equ  <TV_ITEMEX>

ITEMTYPE UNION
        itemex TVITEMEX <>
        item TVITEM <>
ITEMTYPE ENDS

TV_INSERTSTRUCT STRUCT
  hParent       DWORD      ?  
  hInsertAfter  DWORD ?
               ITEMTYPE <>
TV_INSERTSTRUCT ENDS

TV_HITTESTINFO STRUCT
  pt        POINT      <>
  flags     DWORD      ?
  hItem     DWORD      ?
TV_HITTESTINFO ENDS

TVHITTESTINFO  equ  <TV_HITTESTINFO>

TV_SORTCB STRUCT
  hParent       DWORD      ?
  lpfnCompare   DWORD      ?
  lParam        DWORD      ?
TV_SORTCB ENDS

TVSORTCB  equ  <TV_SORTCB>

NM_TREEVIEW STRUCT
  hdr       NMHDR      <>
  action    DWORD      ?
  itemOld   TVITEM      <>
  itemNew   TVITEM      <>
  ptDrag    POINT      <>
NM_TREEVIEW ENDS

NMTREEVIEW  equ  <NM_TREEVIEW>

TV_DISPINFO STRUCT
  hdr   NMHDR      <>
  item  TVITEM <>
TV_DISPINFO ENDS

NMTVDISPINFO  equ  <TV_DISPINFO>

TV_KEYDOWN STRUCT
  hdr       NMHDR      <>
  wVKey     WORD      ?
  flags     DWORD      ?
TV_KEYDOWN ENDS

NMTVKEYDOWN  equ  <TV_KEYDOWN>

TC_ITEMHEADER STRUCT
  imask             DWORD      ?
  lpReserved1       DWORD      ?
  lpReserved2       DWORD      ?
  pszText           DWORD      ?
  cchTextMax        DWORD      ?
  iImage            DWORD      ?
TC_ITEMHEADER ENDS

TCITEMHEADER  equ  <TC_ITEMHEADER>

TC_ITEM STRUCT
  imask             DWORD      ?
  lpReserved1       DWORD      ?
  lpReserved2       DWORD      ?
  pszText           DWORD      ?
  cchTextMax        DWORD      ?
  iImage            DWORD      ?
  lParam            DWORD      ?
TC_ITEM ENDS

TCITEM  equ  <TC_ITEM>

TC_HITTESTINFO STRUCT
  pt        POINT      <>
  flags     DWORD      ?
TC_HITTESTINFO ENDS

TCHITTESTINFO  equ  <TC_HITTESTINFO>

TC_KEYDOWN STRUCT
  hdr            NMHDR      <>
  wVKey          WORD       ?
  flags          DWORD      ?
TC_KEYDOWN ENDS

NMTCKEYDOWN  equ  <TC_KEYDOWN>

; -------------------------- comdlg equates -------------------------------

CDERR_GENERALCODES      equ 0000h
CDERR_STRUCTSIZE        equ 0001h
CDERR_INITIALIZATION    equ 0002h
CDERR_NOTEMPLATE        equ 0003h
CDERR_NOHINSTANCE       equ 0004h
CDERR_LOADSTRFAILURE    equ 0005h
CDERR_FINDRESFAILURE    equ 0006h
CDERR_LOADRESFAILURE    equ 0007h
CDERR_LOCKRESFAILURE    equ 0008h
CDERR_MEMALLOCFAILURE   equ 0009h
CDERR_MEMLOCKFAILURE    equ 000Ah
CDERR_NOHOOK            equ 000Bh
CDERR_REGISTERMSGFAIL   equ 000Ch
CC_RGBINIT              equ 00000001h
CC_FULLOPEN             equ 00000002h
CC_PREVENTFULLOPEN      equ 00000004h
CC_SHOWHELP             equ 00000008h
CC_ENABLEHOOK           equ 00000010h
CC_ENABLETEMPLATE       equ 00000020h
CC_ENABLETEMPLATEHANDLE equ 00000040h
CCERR_CHOOSECOLORCODES  equ 5000h
FR_DOWN                 equ 00000001h
FR_WHOLEWORD            equ 00000002h
FR_MATCHCASE            equ 00000004h
FR_FINDNEXT             equ 00000008h
FR_REPLACE              equ 00000010h
FR_REPLACEALL           equ 00000020h
FR_DIALOGTERM           equ 00000040h
FR_SHOWHELP             equ 00000080h
FR_ENABLEHOOK           equ 00000100h
FR_ENABLETEMPLATE       equ 00000200h
FR_NOUPDOWN             equ 00000400h
FR_NOMATCHCASE          equ 00000800h
FR_NOWHOLEWORD          equ 00001000h
FR_ENABLETEMPLATEHANDLE equ 00002000h
FR_HIDEUPDOWN           equ 00004000h
FR_HIDEMATCHCASE        equ 00008000h
FR_HIDEWHOLEWORD        equ 00010000h
FRERR_FINDREPLACECODES  equ 4000h
FRERR_BUFFERLENGTHZERO  equ 4001h
CF_SCREENFONTS          equ 00000001h
CF_PRINTERFONTS         equ 00000002h
CF_BOTH                 equ CF_SCREENFONTS + CF_PRINTERFONTS
CF_SHOWHELP             equ 00000004h
CF_ENABLEHOOK           equ 00000008h
CF_ENABLETEMPLATE       equ 00000010h
CF_ENABLETEMPLATEHANDLE equ 00000020h
CF_INITTOLOGFONTSTRUCT  equ 00000040h
CF_USESTYLE             equ 00000080h
CF_EFFECTS              equ 00000100h
CF_APPLY                equ 00000200h
CF_ANSIONLY             equ 00000400h
CF_NOVECTORFONTS        equ 00000800h
CF_NOOEMFONTS           equ CF_NOVECTORFONTS
CF_NOSIMULATIONS        equ 00001000h
CF_LIMITSIZE            equ 00002000h
CF_FIXEDPITCHONLY       equ 00004000h
CF_WYSIWYG              equ 00008000h
CF_FORCEFONTEXIST       equ 00010000h
CF_SCALABLEONLY         equ 00020000h
CF_TTONLY               equ 00040000h
CF_NOFACESEL            equ 00080000h
CF_NOSTYLESEL           equ 00100000h
CF_NOSIZESEL            equ 00200000h
CFERR_CHOOSEFONTCODES   equ 2000h
CFERR_NOFONTS           equ 2001h
CFERR_MAXLESSTHANMIN    equ 2002h
WM_CHOOSEFONT_GETLOGFONT equ WM_USER + 1
CD_LBSELNOITEMS         equ -1
CD_LBSELCHANGE          equ  0
CD_LBSELSUB             equ  1
CD_LBSELADD             equ  2
PD_ALLPAGES             equ 00000000h
PD_SELECTION            equ 00000001h
PD_PAGENUMS             equ 00000002h
PD_NOSELECTION          equ 00000004h
PD_NOPAGENUMS           equ 00000008h
PD_COLLATE              equ 00000010h
PD_PRINTTOFILE          equ 00000020h
PD_PRINTSETUP           equ 00000040h
PD_NOWARNING            equ 00000080h
PD_RETURNDC             equ 00000100h
PD_RETURNIC             equ 00000200h
PD_RETURNDEFAULT        equ 00000400h
PD_SHOWHELP             equ 00000800h
PD_ENABLEPRINTHOOK      equ 00001000h
PD_ENABLESETUPHOOK      equ 00002000h
PD_ENABLEPRINTTEMPLATE  equ 00004000h
PD_ENABLESETUPTEMPLATE  equ 00008000h
PD_ENABLEPRINTTEMPLATEHANDLE            equ 00010000h
PD_ENABLESETUPTEMPLATEHANDLE            equ 00020000h
PD_USEDEVMODECOPIES     equ 00040000h
PD_DISABLEPRINTTOFILE   equ 00080000h
PD_HIDEPRINTTOFILE      equ 00100000h
PDERR_PRINTERCODES      equ 1000h
PDERR_SETUPFAILURE      equ 1001h
PDERR_PARSEFAILURE      equ 1002h
PDERR_RETDEFFAILURE     equ 1003h
PDERR_LOADDRVFAILURE    equ 1004h
PDERR_GETDEVMODEFAIL    equ 1005h
PDERR_INITFAILURE       equ 1006h
PDERR_NODEVICES         equ 1007h
PDERR_NODEFAULTPRN      equ 1008h
PDERR_DNDMMISMATCH      equ 1009h
PDERR_CREATEICFAILURE   equ 100Ah
PDERR_PRINTERNOTFOUND   equ 100Bh
PDERR_DEFAULTDIFFERENT  equ 100Ch
DN_DEFAULTPRN           equ 0001h
OFN_ALLOWMULTISELECT    equ 00000200h
OFN_CREATEPROMPT        equ 00002000h
OFN_ENABLEHOOK          equ 00000020h
OFN_ENABLETEMPLATE      equ 00000040h
OFN_ENABLETEMPLATEHANDLE                equ 00000080h
OFN_EXPLORER            equ 00080000h
OFN_EXTENSIONDIFFERENT  equ 00000400h
OFN_FILEMUSTEXIST       equ 00001000h
OFN_HIDEREADONLY        equ 00000004h
OFN_LONGNAMES           equ 00200000h
OFN_NOCHANGEDIR         equ 00000008h
OFN_NODEREFERENCELINKS  equ 00100000h
OFN_NOLONGNAMES         equ 00040000h
OFN_NONETWORKBUTTON     equ 00020000h
OFN_NOREADONLYRETURN    equ 00008000h
OFN_NOTESTFILECREATE    equ 00010000h
OFN_NOVALIDATE          equ 00000100h
OFN_OVERWRITEPROMPT     equ 00000002h
OFN_PATHMUSTEXIST       equ 00000800h
OFN_READONLY            equ 00000001h
OFN_SHAREAWARE          equ 00004000h
OFN_SHOWHELP            equ 00000010h
OFN_SHAREFALLTHROUGH    equ 2
OFN_SHARENOWARN         equ 1
OFN_SHAREWARN           equ 0
CDERR_DIALOGFAILURE     equ 0FFFFh
FNERR_FILENAMECODES     equ 3000h
FNERR_SUBCLASSFAILURE   equ 3001h
FNERR_INVALIDFILENAME   equ 3002h
FNERR_BUFFERTOOSMALL    equ 3003h
; -------------------------- comdlg structures ----------------------------

CHOOSECOLORA STRUCT
  lStructSize     DWORD      ?
  hwndOwner       DWORD      ?
  hInstance       DWORD      ?
  rgbResult       DWORD      ?
  lpCustColors    DWORD      ?
  Flags           DWORD      ?
  lCustData       DWORD      ?
  lpfnHook        DWORD      ?
  lpTemplateName  DWORD      ?
CHOOSECOLORA ENDS

CHOOSECOLOR  equ  <CHOOSECOLORA>

FINDREPLACEA STRUCT
  lStructSize       DWORD      ?
  hwndOwner         DWORD      ?
  hInstance         DWORD      ?
  Flags             DWORD      ?
  lpstrFindWhat     DWORD      ?
  lpstrReplaceWith  DWORD      ?
  wFindWhatLen       WORD      ?
  wReplaceWithLen    WORD      ?
  lCustData         DWORD      ?
  lpfnHook          DWORD      ?
  lpTemplateName    DWORD      ?
FINDREPLACEA ENDS

FINDREPLACE  equ  <FINDREPLACEA>

CHOOSEFONTA STRUCT
  lStructSize     DWORD      ?
  hwndOwner       DWORD      ?
  hDC             DWORD      ?
  lpLogFont       DWORD      ?
  iPointSize      DWORD      ?
  Flags           DWORD      ?
  rgbColors       DWORD      ?
  lCustData       DWORD      ?
  lpfnHook        DWORD      ?
  lpTemplateName  DWORD      ?
  hInstance       DWORD      ?
  lpszStyle       DWORD      ?
  nFontType        WORD      ?
  Alignment        WORD      ?
  nSizeMin        DWORD      ?
  nSizeMax        DWORD      ?
CHOOSEFONTA ENDS

CHOOSEFONT  equ  <CHOOSEFONTA>

DEVNAMES STRUCT
  wDriverOffset  WORD      ?
  wDeviceOffset  WORD      ?
  wOutputOffset  WORD      ?
  wDefault       WORD      ?
DEVNAMES ENDS

PRINTDLGA STRUCT
  lStructSize               DWORD      ?
  hwndOwner                 DWORD      ?
  hDevMode                  DWORD      ?
  hDevNames                 DWORD      ?
  hDC                       DWORD      ?
  Flags                     DWORD      ?
  nFromPage                  WORD      ?
  nToPage                    WORD      ?
  nMinPage                   WORD      ?
  nMaxPage                   WORD      ?
  nCopies                    WORD      ?
  hInstance                 DWORD      ?
  lCustData                 DWORD      ?
  lpfnPrintHook             DWORD      ?
  lpfnSetupHook             DWORD      ?
  lpPrintTemplateName       DWORD      ?
  lpPrintSetupTemplateName  DWORD      ?
  hPrintTemplate            DWORD      ?
  hSetupTemplate            DWORD      ?
PRINTDLGA ENDS

PRINTDLG  equ  <PRINTDLGA>
            
OPENFILENAMEA STRUCT
  lStructSize        DWORD      ?
  hwndOwner          DWORD      ?
  hInstance          DWORD      ?
  lpstrFilter        DWORD      ?
  lpstrCustomFilter  DWORD      ?
  nMaxCustFilter     DWORD      ?
  nFilterIndex       DWORD      ?
  lpstrFile          DWORD      ?
  nMaxFile           DWORD      ?
  lpstrFileTitle     DWORD      ?
  nMaxFileTitle      DWORD      ?
  lpstrInitialDir    DWORD      ?
  lpstrTitle         DWORD      ?
  Flags              DWORD      ?
  nFileOffset         WORD      ?
  nFileExtension      WORD      ?
  lpstrDefExt        DWORD      ?
  lCustData          DWORD      ?
  lpfnHook           DWORD      ?
  lpTemplateName     DWORD      ?
OPENFILENAMEA ENDS

OPENFILENAME  equ  <OPENFILENAMEA>

; -------------------------- riched equates -------------------------------

cchTextLimitDefault         equ 32767

EM_CANPASTE                 equ WM_USER + 50
EM_DISPLAYBAND              equ WM_USER + 51
EM_EXGETSEL                 equ WM_USER + 52
EM_EXLIMITTEXT              equ WM_USER + 53
EM_EXLINEFROMCHAR           equ WM_USER + 54
EM_EXSETSEL                 equ WM_USER + 55
EM_FINDTEXT                 equ WM_USER + 56
EM_FORMATRANGE              equ WM_USER + 57
EM_GETCHARFORMAT            equ WM_USER + 58
EM_GETEVENTMASK             equ WM_USER + 59
EM_GETOLEINTERFACE          equ WM_USER + 60
EM_GETPARAFORMAT            equ WM_USER + 61
EM_GETSELTEXT               equ WM_USER + 62
EM_HIDESELECTION            equ WM_USER + 63
EM_PASTESPECIAL             equ WM_USER + 64
EM_REQUESTRESIZE            equ WM_USER + 65
EM_SELECTIONTYPE            equ WM_USER + 66
EM_SETBKGNDCOLOR            equ WM_USER + 67
EM_SETCHARFORMAT            equ WM_USER + 68
EM_SETEVENTMASK             equ WM_USER + 69
EM_SETOLECALLBACK           equ WM_USER + 70
EM_SETPARAFORMAT            equ WM_USER + 71
EM_SETTARGETDEVICE          equ WM_USER + 72
EM_STREAMIN                 equ WM_USER + 73
EM_STREAMOUT                equ WM_USER + 74
EM_GETTEXTRANGE             equ WM_USER + 75
EM_FINDWORDBREAK            equ WM_USER + 76
EM_SETOPTIONS               equ WM_USER + 77
EM_GETOPTIONS               equ WM_USER + 78
EM_FINDTEXTEX               equ WM_USER + 79
EM_GETWORDBREAKPROCEX       equ WM_USER + 80
EM_SETWORDBREAKPROCEX       equ WM_USER + 81
EM_SETUNDOLIMIT             equ WM_USER + 82
EM_REDO                     equ WM_USER + 84
EM_CANREDO                  equ WM_USER + 85
EM_GETUNDONAME              equ WM_USER + 86
EM_GETREDONAME              equ WM_USER + 87
EM_STOPGROUPTYPING          equ WM_USER + 88
EM_SETTEXTMODE              equ WM_USER + 89
EM_GETTEXTMODE              equ WM_USER + 90
EM_AUTOURLDETECT            equ WM_USER + 91
EM_GETAUTOURLDETECT         equ WM_USER + 92
EM_SETPALETTE               equ WM_USER + 93
EM_GETTEXTEX                equ WM_USER + 94
EM_GETTEXTLENGTHEX          equ WM_USER + 95
EM_SHOWSCROLLBAR            equ WM_USER + 96

EM_SETPUNCTUATION           equ WM_USER + 100
EM_GETPUNCTUATION           equ WM_USER + 101
EM_SETWORDWRAPMODE          equ WM_USER + 102
EM_GETWORDWRAPMODE          equ WM_USER + 103
EM_SETIMECOLOR              equ WM_USER + 104
EM_GETIMECOLOR              equ WM_USER + 105
EM_SETIMEOPTIONS            equ WM_USER + 106
EM_GETIMEOPTIONS            equ WM_USER + 107
EM_CONVPOSITION             equ WM_USER + 108
EM_SETLANGOPTIONS           equ WM_USER + 120
EM_GETLANGOPTIONS           equ WM_USER + 121
EM_GETIMECOMPMODE           equ WM_USER + 122
EM_FINDTEXTW                equ WM_USER + 123
EM_FINDTEXTEXW              equ WM_USER + 124
EM_RECONVERSION             equ WM_USER + 125
EM_SETIMEMODEBIAS           equ WM_USER + 126  
EM_GETIMEMODEBIAS           equ WM_USER + 127
EM_SETBIDIOPTIONS           equ WM_USER + 200
EM_GETBIDIOPTIONS           equ WM_USER + 201
EM_SETTYPOGRAPHYOPTIONS     equ WM_USER + 202
EM_GETTYPOGRAPHYOPTIONS     equ WM_USER + 203
EM_SETEDITSTYLE             equ WM_USER + 204
EM_GETEDITSTYLE             equ WM_USER + 205

EM_OUTLINE                  equ WM_USER + 220
EM_GETSCROLLPOS             equ WM_USER + 221
EM_SETSCROLLPOS             equ WM_USER + 222
EM_SETFONTSIZE              equ WM_USER + 223
EM_GETZOOM                  equ WM_USER + 224
EM_SETZOOM                  equ WM_USER + 225

SES_EMULATESYSEDIT          equ 1
SES_BEEPONMAXTEXT           equ 2
SES_EXTENDBACKCOLOR         equ 4
SES_MAPCPS                  equ 8
SES_EMULATE10               equ 16
SES_USECRLF                 equ 32
SES_USEAIMM                 equ 64
SES_ALLOWBEEPS              equ 256

EN_MSGFILTER                equ 0700h
EN_REQUESTRESIZE            equ 0701h
EN_SELCHANGE                equ 0702h
EN_DROPFILES                equ 0703h
EN_PROTECTED                equ 0704h
EN_CORRECTTEXT              equ 0705h
EN_STOPNOUNDO               equ 0706h
EN_IMECHANGE                equ 0707h
EN_SAVECLIPBOARD            equ 0708h
EN_OLEOPFAILED              equ 0709h
ENM_NONE                    equ 00000000h
ENM_CHANGE                  equ 00000001h
ENM_UPDATE                  equ 00000002h
ENM_SCROLL                  equ 00000004h
ENM_KEYEVENTS               equ 00010000h
ENM_MOUSEEVENTS             equ 00020000h
ENM_REQUESTRESIZE           equ 00040000h
ENM_SELCHANGE               equ 00080000h
ENM_DROPFILES               equ 00100000h
ENM_PROTECTED               equ 00200000h
ENM_CORRECTTEXT             equ 00400000h
ENM_IMECHANGE               equ 00800000h
ES_SAVESEL                  equ 00008000h
ES_SUNKEN                   equ 00004000h
ES_DISABLENOSCROLL          equ 00002000h
ES_SELECTIONBAR             equ 01000000h
ES_EX_NOCALLOLEINIT         equ 01000000h
ES_VERTICAL                 equ 00400000h
ES_NOIME                    equ 00080000h
ES_SELFIME                  equ 00040000h
ECO_AUTOWORDSELECTION       equ 00000001h
ECO_AUTOVSCROLL             equ 00000040h
ECO_AUTOHSCROLL             equ 00000080h
ECO_NOHIDESEL               equ 00000100h
ECO_READONLY                equ 00000800h
ECO_WANTRETURN              equ 00001000h
ECO_SAVESEL                 equ 00008000h
ECO_SELECTIONBAR            equ 01000000h
ECO_VERTICAL                equ 00400000h
ECOOP_SET                   equ 0001h
ECOOP_OR                    equ 0002h
ECOOP_AND                   equ 0003h
ECOOP_XOR                   equ 0004h
WB_CLASSIFY                 equ 3
WB_MOVEWORDLEFT             equ 4
WB_MOVEWORDRIGHT            equ 5
WB_LEFTBREAK                equ 6
WB_RIGHTBREAK               equ 7
WB_MOVEWORDPREV             equ 4
WB_MOVEWORDNEXT             equ 5
WB_PREVBREAK                equ 6
WB_NEXTBREAK                equ 7
PC_FOLLOWING                equ 1
PC_LEADING                  equ 2
PC_OVERFLOW                 equ 3
PC_DELIMITER                equ 4
WBF_WORDWRAP                equ 010h
WBF_WORDBREAK               equ 020h
WBF_OVERFLOW                equ 040h
WBF_LEVEL1                  equ 080h
WBF_LEVEL2                  equ 100h
WBF_CUSTOM                  equ 200h

IMF_FORCENONE                  equ 0001h
IMF_FORCEENABLE                equ 0002h
IMF_FORCEDISABLE               equ 0004h
IMF_CLOSESTATUSWINDOW          equ 0008h
IMF_VERTICAL   equ 0020h
IMF_FORCEACTIVE                equ 0040h
IMF_FORCEINACTIVE              equ 0080h
IMF_FORCEREMEMBER              equ 0100h
WBF_CLASS               equ 0Fh
WBF_ISWHITE             equ 10h
WBF_BREAKLINE           equ 20h
WBF_BREAKAFTER          equ 40h
CFM_BOLD               equ 00000001h
CFM_ITALIC             equ 00000002h
CFM_UNDERLINE          equ 00000004h
CFM_STRIKEOUT          equ 00000008h
CFM_PROTECTED          equ 00000010h
CFM_SIZE               equ 80000000h
CFM_COLOR              equ 40000000h
CFM_FACE               equ 20000000h
CFM_OFFSET             equ 10000000h
CFM_CHARSET            equ 08000000h
CFE_BOLD               equ 0001h
CFE_ITALIC             equ 0002h
CFE_UNDERLINE          equ 0004h
CFE_STRIKEOUT          equ 0008h
CFE_PROTECTED          equ 0010h
CFE_AUTOCOLOR          equ 40000000h
yHeightCharPtsMost          equ 1638
SCF_SELECTION          equ 0001h
SCF_WORD               equ 0002h
SF_TEXT               equ 0001h
SF_RTF                equ 0002h
SF_RTFNOOBJS          equ 0003h
SF_TEXTIZED           equ 0004h
SFF_SELECTION          equ 8000h
SFF_PLAINRTF          equ 4000h
MAX_TAB_STOPS          equ 32
lDefaultTab            equ 720
PFM_STARTINDENT             equ 00000001h
PFM_RIGHTINDENT             equ 00000002h
PFM_OFFSET  equ 00000004h
PFM_ALIGNMENT               equ 00000008h
PFM_TABSTOPS                equ 00000010h
PFM_NUMBERING               equ 00000020h
PFM_OFFSETINDENT            equ 80000000h
PFN_BULLET  equ 0001h
PFA_LEFT    equ 0001h
PFA_RIGHT   equ 0002h
PFA_CENTER  equ 0003h
SEL_EMPTY                equ 0000h
SEL_TEXT equ 0001h
SEL_OBJECT               equ 0002h
SEL_MULTICHAR            equ 0004h
SEL_MULTIOBJECT          equ 0008h
OLEOP_DOVERB           equ 1
; -------------------------- riched structures -----------------------------

CHARFORMATA STRUCT
  cbSize            DWORD      ?
  dwMask            DWORD      ?
  dwEffects         DWORD      ?
  yHeight           DWORD      ?
  yOffset           DWORD      ?
  crTextColor       COLORREF   ?
  bCharSet          BYTE       ?
  bPitchAndFamily   BYTE       ?
  szFaceName        BYTE LF_FACESIZE dup(?)
  _wPad2            WORD       ?
CHARFORMATA ENDS

CHARFORMAT  equ  <CHARFORMATA>


CHARRANGE STRUCT
  cpMin  DWORD      ?
  cpMax  DWORD      ?
CHARRANGE ENDS

TEXTRANGEA STRUCT
  chrg          CHARRANGE <>
  lpstrText     DWORD      ?
TEXTRANGEA ENDS

TEXTRANGE  equ  <TEXTRANGEA>

EDITSTREAM STRUCT
  dwCookie      DWORD      ?
  dwError       DWORD      ?
  pfnCallback   DWORD      ?
EDITSTREAM ENDS

FINDTEXTA STRUCT
  chrg          CHARRANGE  <>
  lpstrText     DWORD      ?
FINDTEXTA ENDS

FINDTEXT  equ  <FINDTEXTA>

FINDTEXTEXA STRUCT
  chrg          CHARRANGE <>
  lpstrText     DWORD      ?
  chrgText      CHARRANGE <>
FINDTEXTEXA ENDS

FINDTEXTEX  equ  <FINDTEXTEXA>

FORMATRANGE STRUCT
  hdc           DWORD      ?
  hdcTarget     DWORD      ?
  rc            RECT <>
  rcPage        RECT <>
  chrg          CHARRANGE <>
FORMATRANGE ENDS

PARAFORMAT STRUCT
  cbSize            DWORD     ?
  dwMask            DWORD     ?
  wNumbering        WORD      ?
  wEffects          WORD      ?
  dxStartIndent     DWORD     ?
  dxRightIndent     DWORD     ?
  dxOffset          DWORD     ?
  wAlignment        WORD      ?
  cTabCount         WORD      ?
  rgxTabs           DWORD MAX_TAB_STOPS dup(?)
PARAFORMAT ENDS

PARAFORMAT2 STRUCT
  cbSize            DWORD    ?
  dwMask            DWORD    ?
  wNumbering        WORD     ?
  wEffects          WORD     ?
  dxStartIndent     DWORD    ?
  dxRightIndent     DWORD    ?
  dxOffset          DWORD    ?
  wAlignment        WORD     ?
  cTabCount         WORD     ?
  rgxTabs           DWORD MAX_TAB_STOPS dup(?)
  dySpaceBefore     DWORD    ?
  dySpaceAfter      DWORD    ?
  dyLineSpacing     DWORD    ?
  sStyle            WORD     ?
  bLineSpacingRule  BYTE     ?
  bOutlineLevel     BYTE     ?
  wShadingWeight    WORD     ?
  wShadingStyle     WORD     ?
  wNumberingStart   WORD     ?
  wNumberingStyle   WORD     ?
  wNumberingTab     WORD     ?
  wBorderSpace      WORD     ?
  wBorderWidth      WORD     ?
  wBorders          WORD     ?
PARAFORMAT2 ENDS

MSGFILTER STRUCT
  nmhdr     NMHDR <>
  msg       DWORD      ?
  wParam    DWORD      ?
  lParam    DWORD      ?
MSGFILTER ENDS

REQRESIZE STRUCT
  nmhdr  NMHDR <>
  rc     RECT <>
REQRESIZE ENDS

SELCHANGE STRUCT
  nmhdr     NMHDR <>
  chrg      CHARRANGE <>
  seltyp    WORD      ?
SELCHANGE ENDS

ENDROPFILES STRUCT
  nmhdr         NMHDR <>
  hDrop         DWORD      ?
  cp            DWORD      ?
  fProtected    DWORD      ?
ENDROPFILES ENDS

ENPROTECTED STRUCT
  nmhdr     NMHDR <>
  msg       DWORD      ?
  wPad1     DWORD ?
  wParam    DWORD      ?
  wPad2     DWORD ?
  lParam    DWORD      ?
  chrg      CHARRANGE <>
ENPROTECTED ENDS

ENSAVECLIPBOARD STRUCT
  nmhdr         NMHDR <>
  cObjectCount  DWORD      ?
  cch           DWORD      ?
ENSAVECLIPBOARD ENDS

ENOLEOPFAILED STRUCT
  nmhdr     NMHDR <>
  iob       DWORD      ?
  lOper     DWORD      ?
  hr        DWORD      ?
ENOLEOPFAILED ENDS

ENCORRECTTEXT STRUCT
  nmhdr     NMHDR <>
  chrg      CHARRANGE <>
  seltyp    WORD      ?
ENCORRECTTEXT ENDS

PUNCTUATION STRUCT
  iSize         DWORD      ?
  szPunctuation DWORD      ?
PUNCTUATION ENDS

COMPCOLOR STRUCT
  crText        DWORD      ?
  crBackground  DWORD      ?
  dwEffects     DWORD      ?
COMPCOLOR ENDS

REPASTESPECIAL STRUCT
  dwAspect  DWORD      ?
  dwParam   DWORD      ?
REPASTESPECIAL ENDS


; ----------------------- win32api structures -----------------------------

SIZEL STRUCT
  x  DWORD      ?
  y  DWORD      ?
SIZEL ENDS

_SIZE  equ  <SIZEL>

MSG STRUCT
  hwnd      DWORD      ?
  message   DWORD      ?
  wParam    DWORD      ?
  lParam    DWORD      ?
  time      DWORD      ?
  pt        POINT      <>
MSG ENDS

SID_AND_ATTRIBUTES STRUCT
  Sid           DWORD      ?
  Attributes    DWORD      ?
SID_AND_ATTRIBUTES ENDS

SID_IDENTIFIER_AUTHORITY STRUCT
  Value  BYTE     6 dup(?)
SID_IDENTIFIER_AUTHORITY ENDS

OVERLAPPED STRUCT
  Internal      DWORD      ?
  InternalHigh  DWORD      ?
  loffset       DWORD      ?
  OffsetHigh    DWORD      ?
  hEvent        DWORD      ?
OVERLAPPED ENDS

SECURITY_ATTRIBUTES STRUCT
  nLength              DWORD      ?
  lpSecurityDescriptor  DWORD      ?
  bInheritHandle        DWORD      ?
SECURITY_ATTRIBUTES ENDS

PROCESS_INFORMATION STRUCT
  hProcess      DWORD      ?
  hThread       DWORD      ?
  dwProcessId   DWORD      ?
  dwThreadId    DWORD      ?
PROCESS_INFORMATION ENDS

FILETIME STRUCT
  dwLowDateTime     DWORD      ?
  dwHighDateTime    DWORD      ?
FILETIME ENDS

SYSTEMTIME STRUCT
  wYear             WORD      ?
  wMonth            WORD      ?
  wDayOfWeek        WORD      ?
  wDay              WORD      ?
  wHour             WORD      ?
  wMinute           WORD      ?
  wSecond           WORD      ?
  wMilliseconds     WORD      ?
SYSTEMTIME ENDS

COMMPROP STRUCT
  wPacketiLength        WORD      ?
  wPacketVersion        WORD      ?
  dwServiceMask         DWORD      ?
  dwReserved1           DWORD      ?
  dwMaxTxQueue          DWORD      ?
  dwMaxRxQueue          DWORD      ?
  dwMaxBaud             DWORD      ?
  dwProvSubType         DWORD      ?
  dwProvCapabilities    DWORD      ?
  dwSettableParams      DWORD      ?
  dwSettableBaud        DWORD      ?
  wSettableData         WORD      ?
  wSettableStopParity   WORD      ?
  dwCurrentTxQueue      DWORD      ?
  dwCurrentRxQueue      DWORD      ?
  dwProvSpec1           DWORD      ?
  dwProvSpec2           DWORD      ?
  wcProvChar            WORD      ?
COMMPROP ENDS


COMRECORD RECORD fCtsHold:1, fDsrHold:1,fRlsdHold:1,fXoffHold:1,fXoffSent:1,fEof:1,fTxim:1,fReserved:25

COMSTAT STRUCT
  comrec    COMRECORD     <>
  cbInQue   DWORD       ?
  cbOutQue  DWORD      ?
COMSTAT ENDS

BITRECORD RECORD fBinary:1,fParity:1,fOutxCtsFlow:1,fOutxDsrFlow:1,fDtrControl:2,fDsrSensitivity:1,fTXContinueOnXoff:1,fOutX:1,fInX:1,fErrorChar:1,fNull:1,fRtsControl:2,fAbortOnError:1,fDummy2:17

DCB STRUCT
  DCBlength     DWORD      ?
  BaudRate      DWORD       ?
  fbits         BITRECORD      <>
  wReserved     WORD       ?
  XonLim        WORD          ?
  XoffLim       WORD         ?
  ByteSize      BYTE        ?
  Parity        BYTE          ?
  StopBits      BYTE        ?
  XonChar       BYTE         ?
  XoffChar      BYTE        ?
  ErrorChar     BYTE       ?
  EofChar       BYTE         ?
  EvtChar       BYTE         ?
  wReserved1    WORD       ?
DCB ENDS

COMMTIMEOUTS STRUCT
  ReadIntervalTimeout           DWORD      ?
  ReadTotalTimeoutMultiplier    DWORD      ?
  ReadTotalTimeoutConstant      DWORD      ?
  WriteTotalTimeoutMultiplier   DWORD      ?
  WriteTotalTimeoutConstant     DWORD      ?
COMMTIMEOUTS ENDS

SYSTEM_INFO STRUCT
  wProcessorArchitecture        WORD       ?
  wReserved                     WORD       ?
  dwPageSize                    DWORD      ?
  lpMinimumApplicationAddress   DWORD      ?
  lpMaximumApplicationAddress   DWORD      ?
  dwActiveProcessorMask         DWORD      ?
  dwNumberOfProcessors         DWORD      ?
  dwProcessorType               DWORD      ?
  dwAllocationGranularity       DWORD      ?
  wProcessorLevel               WORD       ?
  wProcessorRevision            WORD       ?
SYSTEM_INFO ENDS

MEMORYSTATUS STRUCT
  dwLength          DWORD      ?
  dwMemoryLoad      DWORD      ?
  dwTotalPhys       DWORD      ?
  dwAvailPhys       DWORD      ?
  dwTotalPageFile   DWORD      ?
  dwAvailPageFile   DWORD      ?
  dwTotalVirtual    DWORD      ?
  dwAvailVirtual    DWORD      ?
MEMORYSTATUS ENDS

TPMPARAMS STRUCT
  cbSize        DWORD      ?
  rcExclude     RECT       <>
TPMPARAMS ENDS


GENERIC_MAPPING STRUCT
  GenericRead       DWORD      ?
  GenericWrite      DWORD      ?
  GenericExecute    DWORD      ?
  GenericAll        DWORD      ?
GENERIC_MAPPING ENDS

LUID STRUCT
  LowPart   DWORD      ?
  HighPart  DWORD      ?
LUID ENDS

ACL STRUCT
  AclRevision   BYTE      ?
  Sbz1          BYTE      ?
  AclSize       WORD      ?
  AceCount      WORD      ?
  Sbz2          WORD      ?
ACL ENDS

ACE_HEADER STRUCT
    AceType db ?
    AceFlags db ?
    AceSize dw ?
ACE_HEADER ENDS

ACCESS_ALLOWED_ACE STRUCT
  Header    ACE_HEADER <>
  imask     DWORD      ?
  SidStart  DWORD      ?
ACCESS_ALLOWED_ACE ENDS

ACCESS_DENIED_ACE STRUCT
  Header        ACE_HEADER <>
  imask         DWORD      ?
  SidStart      DWORD      ?
ACCESS_DENIED_ACE ENDS

SYSTEM_AUDIT_ACE STRUCT
  Header    ACE_HEADER <>
  imask     DWORD      ?
  SidStart  DWORD      ?
SYSTEM_AUDIT_ACE ENDS

SYSTEM_ALARM_ACE STRUCT
  Header    ACE_HEADER <>
  imask     DWORD      ?
  SidStart  DWORD      ?
SYSTEM_ALARM_ACE ENDS

ACL_REVISION_INFORMATION STRUCT
  AclRevision  DWORD      ?
ACL_REVISION_INFORMATION ENDS

ACL_SIZE_INFORMATION STRUCT
  AceCount          DWORD      ?
  AclBytesInUse     DWORD      ?
  AclBytesFree      DWORD      ?
ACL_SIZE_INFORMATION ENDS

SECURITY_DESCRIPTOR STRUCT
  Revision  BYTE      ?
  Sbz1      BYTE      ?
  Control   WORD      ?
  Owner     DWORD      ?
  lGroup    DWORD      ?
  Sacl      DWORD      ?
  Dacl      DWORD      ?
SECURITY_DESCRIPTOR ENDS

LUID_AND_ATTRIBUTES STRUCT
    Luid LUID <>
    Attributes dd ?
LUID_AND_ATTRIBUTES ENDS

PRIVILEGE_SET STRUCT
  PrivilegeCount    DWORD      ?
  Control           DWORD      ?
  Privilege  LUID_AND_ATTRIBUTES ANYSIZE_ARRAY dup(<>)
PRIVILEGE_SET ENDS

EXCEPTION_RECORD STRUCT
  ExceptionCode         DWORD      ?
  ExceptionFlags        DWORD      ?
  pExceptionRecord      DWORD      ?
  ExceptionAddress      DWORD      ?
  NumberParameters      DWORD      ?
  ExceptionInformation  DWORD EXCEPTION_MAXIMUM_PARAMETERS dup(?)
EXCEPTION_RECORD ENDS

EXCEPTION_DEBUG_INFO STRUCT
  pExceptionRecord  EXCEPTION_RECORD <?,?,?,?,?,EXCEPTION_MAXIMUM_PARAMETERS dup(?)>
  dwFirstChance     DWORD      ?
EXCEPTION_DEBUG_INFO ENDS

CREATE_THREAD_DEBUG_INFO STRUCT
  hThread               DWORD      ?
  lpThreadLocalBase     DWORD      ?
  lpStartAddress        DWORD      ?
CREATE_THREAD_DEBUG_INFO ENDS

CREATE_PROCESS_DEBUG_INFO STRUCT
  hFile                 DWORD      ?
  hProcess              DWORD      ?
  hThread               DWORD      ?
  lpBaseOfImage         DWORD      ?
  dwDebugInfoFileOffset DWORD      ?
  nDebugInfoSize        DWORD      ?
  lpThreadLocalBase     DWORD      ?
  lpStartAddress        DWORD      ?
  lpImageName           DWORD      ?
  fUnicode              WORD      ?
CREATE_PROCESS_DEBUG_INFO ENDS

EXIT_THREAD_DEBUG_INFO STRUCT
  dwExitCode  DWORD      ?
EXIT_THREAD_DEBUG_INFO ENDS

EXIT_PROCESS_DEBUG_INFO STRUCT
  dwExitCode  DWORD      ?
EXIT_PROCESS_DEBUG_INFO ENDS

LOAD_DLL_DEBUG_INFO STRUCT
  hFile                     DWORD      ?
  lpBaseOfDll               DWORD      ?
  dwDebugInfoFileOffset     DWORD      ?
  nDebugInfoSize            DWORD      ?
  lpImageName               DWORD      ?
  fUnicode                  WORD       ?
LOAD_DLL_DEBUG_INFO ENDS

UNLOAD_DLL_DEBUG_INFO STRUCT
  lpBaseOfDll  DWORD      ?
UNLOAD_DLL_DEBUG_INFO ENDS

OUTPUT_DEBUG_STRING_INFO STRUCT
  lpDebugStringData     DWORD      ?
  fUnicode              WORD      ?
  nDebugStringiLength   WORD      ?
OUTPUT_DEBUG_STRING_INFO ENDS

RIP_INFO STRUCT
  dwError   DWORD      ?
  dwType    DWORD      ?
RIP_INFO ENDS

OFSTRUCT STRUCT
  cBytes        BYTE      ?
  fFixedDisk    BYTE      ?
  nErrCode      WORD      ?
  Reserved1     WORD      ?
  Reserved2     WORD      ?
  szPathName    BYTE  OFS_MAXPATHNAME dup(?)
OFSTRUCT ENDS

WNDCLASSEX STRUCT
  cbSize            DWORD      ?
  style             DWORD      ?
  lpfnWndProc       DWORD      ?
  cbClsExtra        DWORD      ?
  cbWndExtra        DWORD      ?
  hInstance         DWORD      ?
  hIcon             DWORD      ?
  hCursor           DWORD      ?
  hbrBackground     DWORD      ?
  lpszMenuName      DWORD      ?
  lpszClassName     DWORD      ?
  hIconSm           DWORD      ?
WNDCLASSEX ENDS

WNDCLASSA STRUCT
  style             DWORD      ?
  lpfnWndProc       DWORD      ?
  cbClsExtra        DWORD      ?
  cbWndExtra        DWORD      ?
  hInstance         DWORD      ?
  hIcon             DWORD      ?
  hCursor           DWORD      ?
  hbrBackground     DWORD      ?
  lpszMenuName      DWORD      ?
  lpszClassName     DWORD      ?
WNDCLASSA ENDS

WNDCLASS  equ  <WNDCLASSA>

RTL_CRITICAL_SECTION STRUCT
    DebugInfo       DWORD ?
    LockCount       LONG ?
    RecursionCount  LONG ?
    OwningThread    HANDLE ?
    LockSemaphore   HANDLE ?
    SpinCount       DWORD ?
RTL_CRITICAL_SECTION ENDS

CRITICAL_SECTION  equ  <RTL_CRITICAL_SECTION>

BY_HANDLE_FILE_INFORMATION STRUCT
  dwFileAttributes      DWORD      ?
  ftCreationTime        FILETIME <>
  ftLastAccessTime      FILETIME <>
  ftLastWriteTime       FILETIME <>
  dwVolumeSerialNumber  DWORD      ?
  nFileSizeHigh         DWORD      ?
  nFileSizeLow          DWORD      ?
  nNumberOfLinks        DWORD      ?
  nFileIndexHigh        DWORD      ?
  nFileIndexLow         DWORD      ?
BY_HANDLE_FILE_INFORMATION ENDS

MEMORY_BASIC_INFORMATION STRUCT
  BaseAddress       DWORD      ?
  AllocationBase    DWORD      ?
  AllocationProtect DWORD      ?
  RegionSize        DWORD      ?
  State             DWORD      ?
  Protect           DWORD      ?
  lType             DWORD      ?
MEMORY_BASIC_INFORMATION ENDS

EVENTLOGRECORD STRUCT
  iLength               DWORD      ?
  Reserved              DWORD      ?
  RecordNumber          DWORD      ?
  TimeGenerated         DWORD      ?
  TimeWritten           DWORD      ?
  EventID               DWORD      ?
  EventType             WORD      ?
  NumStrings            WORD      ?
  EventCategory         WORD      ?
  ReservedFlags         WORD      ?
  ClosingRecordNumber   DWORD      ?
  StringOffset          DWORD      ?
  UserSidiLength        DWORD      ?
  UserSidOffset         DWORD      ?
  DataiLength           DWORD      ?
  DataOffset            DWORD      ?
EVENTLOGRECORD ENDS

TOKEN_GROUPS STRUCT
  GroupCount    DWORD      ?
  Groups        SID_AND_ATTRIBUTES ANYSIZE_ARRAY dup(<>)
TOKEN_GROUPS ENDS

TOKEN_PRIVILEGES STRUCT
  PrivilegeCount    DWORD      ?
  Privileges        LUID_AND_ATTRIBUTES ANYSIZE_ARRAY dup(<>)
TOKEN_PRIVILEGES ENDS

FLOATING_SAVE_AREA STRUCT
  ControlWord   DWORD      ?
  StatusWord    DWORD      ?
  TagWord       DWORD      ?
  ErrorOffset   DWORD      ?
  ErrorSelector DWORD      ?
  DataOffset    DWORD      ?
  DataSelector  DWORD      ?
  RegisterArea  BYTE  SIZE_OF_80387_REGISTERS dup(?)
  Cr0NpxState   DWORD      ?
FLOATING_SAVE_AREA ENDS

CONTEXT STRUCT
  ContextFlags  DWORD      ?
  iDr0          DWORD      ?
  iDr1          DWORD      ?
  iDr2          DWORD      ?
  iDr3          DWORD      ?
  iDr6          DWORD      ?
  iDr7          DWORD      ?
  FloatSave     FLOATING_SAVE_AREA <>
  regGs         DWORD      ?
  regFs         DWORD      ?
  regEs         DWORD      ?
  regDs         DWORD      ?
  regEdi        DWORD      ?
  regEsi        DWORD      ?
  regEbx        DWORD      ?
  regEdx        DWORD      ?
  regEcx        DWORD      ?
  regEax        DWORD      ?
  regEbp        DWORD      ?
  regEip        DWORD      ?
  regCs         DWORD      ?
  regFlag       DWORD      ?
  regEsp        DWORD      ?
  regSs         DWORD      ?
  ExtendedRegisters db MAXIMUM_SUPPORTED_EXTENSION dup(?)
CONTEXT ENDS

EXCEPTION_POINTERS STRUCT
  pExceptionRecord  DWORD      ?
  ContextRecord     DWORD      ?
EXCEPTION_POINTERS ENDS

LDT_BYTES STRUCT
  BaseMid  BYTE      ?
  Flags1   BYTE      ?
  Flags2   BYTE      ?
  BaseHi   BYTE      ?
LDT_BYTES ENDS

TIME_ZONE_INFORMATION STRUCT
  Bias          DWORD       ?
  StandardName  WORD        32 dup(?)
  StandardDate  SYSTEMTIME  <>
  StandardBias  DWORD       ?
  DaylightName  WORD        32 dup(?)
  DaylightDate  SYSTEMTIME  <>
  DaylightBias  DWORD       ?
TIME_ZONE_INFORMATION ENDS

LARGE_INTEGER STRUCT
        QuadPart QWORD ?
LARGE_INTEGER ENDS

WIN32_STREAM_ID STRUCT
  dwStreamID            DWORD      ?
  dwStreamAttributes    DWORD      ?
  dwSize                LARGE_INTEGER <>
  dwStreamNameSize      DWORD      ?
  cStreamName           BYTE  ANYSIZE_ARRAY dup(?)
WIN32_STREAM_ID ENDS

STARTUPINFO STRUCT
  cb                DWORD      ?
  lpReserved        DWORD      ?
  lpDesktop         DWORD      ?
  lpTitle           DWORD      ?
  dwX               DWORD      ?
  dwY               DWORD      ?
  dwXSize           DWORD      ?
  dwYSize           DWORD      ?
  dwXCountChars     DWORD      ?
  dwYCountChars     DWORD      ?
  dwFillAttribute   DWORD      ?
  dwFlags           DWORD      ?
  wShowWindow       WORD       ?
  cbReserved2       WORD       ?
  lpReserved2       DWORD      ?
  hStdInput         DWORD      ?
  hStdOutput        DWORD      ?
  hStdError         DWORD      ?
STARTUPINFO ENDS

WIN32_FIND_DATA STRUCT
  dwFileAttributes      DWORD      ?
  ftCreationTime        FILETIME <>
  ftLastAccessTime      FILETIME <>
  ftLastWriteTime       FILETIME <>
  nFileSizeHigh         DWORD      ?
  nFileSizeLow          DWORD      ?
  dwReserved0           DWORD      ?
  dwReserved1           DWORD      ?
  cFileName             BYTE MAX_PATH dup(?)
  cAlternate            BYTE 14 dup(?)
WIN32_FIND_DATA ENDS

CPINFO STRUCT
  MaxCharSize   DWORD      ?
  DefaultChar   BYTE   MAX_DEFAULTCHAR dup(?)
  LeadByte      BYTE MAX_LEADBYTES dup(?)
CPINFO ENDS

NUMBERFMTA STRUCT
  NumDigits         DWORD      ?
  LeadingZero       DWORD      ?
  Grouping          DWORD      ?
  lpDecimalSep      DWORD      ?
  lpThousandSep     DWORD      ?
  NegativeOrder     DWORD      ?
NUMBERFMTA ENDS

NUMBERFMT  equ  <NUMBERFMTA>

CURRENCYFMTA STRUCT
  NumDigits         DWORD      ?
  LeadingZero       DWORD      ?
  Grouping          DWORD      ?
  lpDecimalSep      DWORD      ?
  lpThousandSep     DWORD      ?
  NegativeOrder     DWORD      ?
  PositiveOrder     DWORD      ?
  lpCurrencySymbol  DWORD      ?
CURRENCYFMTA ENDS

CURRENCYFMT  equ  <CURRENCYFMTA>

COORD STRUCT
  x  WORD      ?
  y  WORD      ?
COORD ENDS

SMALL_RECT STRUCT
  Left      WORD      ?
  Top       WORD      ?
  Right     WORD      ?
  Bottom    WORD      ?
SMALL_RECT ENDS

CHARTYPE UNION
 UnicodeChar    WORD ?
 AsciiChar      db ?
CHARTYPE ENDS

KEY_EVENT_RECORD STRUCT
  bKeyDown          DWORD      ?
  wRepeatCount      WORD      ?
  wVirtualKeyCode   WORD      ?
  wVirtualScanCode  WORD      ?
  uChar             CHARTYPE <>
  dwControlKeyState DWORD      ?
KEY_EVENT_RECORD ENDS

MOUSE_EVENT_RECORD STRUCT
  dwMousePosition       COORD <>
  dwButtonState         DWORD      ?
  dwControlKeyState     DWORD      ?
  dwEventFlags          DWORD      ?
MOUSE_EVENT_RECORD ENDS

WINDOW_BUFFER_SIZE_RECORD STRUCT
  dwSize  COORD <>
WINDOW_BUFFER_SIZE_RECORD ENDS

MENU_EVENT_RECORD STRUCT
  dwCommandId  DWORD      ?
MENU_EVENT_RECORD ENDS

FOCUS_EVENT_RECORD STRUCT
  bSetFocus  DWORD      ?
FOCUS_EVENT_RECORD ENDS

CHAR_INFO STRUCT
  Char          CHARTYPE <>
  Attributes    WORD      ?
CHAR_INFO ENDS

CONSOLE_SCREEN_BUFFER_INFO STRUCT
  dwSize                COORD <>
  dwCursorPosition      COORD <>
  wAttributes           WORD      ?
  srWindow              SMALL_RECT <>
  dwMaximumWindowSize   COORD <>
CONSOLE_SCREEN_BUFFER_INFO ENDS

CONSOLE_CURSOR_INFO STRUCT
  dwSize    DWORD      ?
  bVisible  DWORD      ?
CONSOLE_CURSOR_INFO ENDS

XFORM STRUCT
  eM11  DWORD      ?
  eM12  DWORD      ?
  eM21  DWORD      ?
  eM22  DWORD      ?
  ex    DWORD      ?
  ey    DWORD      ?
XFORM ENDS

BITMAP STRUCT
  bmType        DWORD       ?
  bmWidth       DWORD       ?
  bmHeight      DWORD       ?
  bmWidthBytes  DWORD       ?
  bmPlanes      WORD        ?
  bmBitsPixel   WORD        ?
  bmBits        DWORD       ?
BITMAP ENDS

RGBTRIPLE STRUCT
  rgbtBlue      BYTE      ?
  rgbtGreen     BYTE      ?
  rgbtRed       BYTE      ?
RGBTRIPLE ENDS

RGBQUAD STRUCT
  rgbBlue       BYTE      ?
  rgbGreen      BYTE      ?
  rgbRed        BYTE      ?
  rgbReserved   BYTE      ?
RGBQUAD ENDS

BITMAPCOREHEADER STRUCT
  bcSize        DWORD      ?
  bcWidth       WORD      ?
  bcHeight      WORD      ?
  bcPlanes      WORD      ?
  bcBitCount    WORD      ?
BITMAPCOREHEADER ENDS

BITMAPINFOHEADER STRUCT
  biSize            DWORD      ?
  biWidth           DWORD      ?
  biHeight          DWORD      ?
  biPlanes          WORD       ?
  biBitCount        WORD       ?
  biCompression     DWORD      ?
  biSizeImage       DWORD      ?
  biXPelsPerMeter   DWORD      ?
  biYPelsPerMeter   DWORD      ?
  biClrUsed         DWORD      ?
  biClrImportant    DWORD      ?
BITMAPINFOHEADER ENDS

BITMAPINFO STRUCT
  bmiHeader  BITMAPINFOHEADER <>
  bmiColors  RGBQUAD <>
BITMAPINFO ENDS

BITMAPCOREINFO STRUCT
  bmciHeader  BITMAPCOREHEADER <>
  bmciColors  RGBTRIPLE <>
BITMAPCOREINFO ENDS

BITMAPFILEHEADER STRUCT
  bfType        WORD      ?
  bfSize        DWORD      ?
  bfReserved1   WORD      ?
  bfReserved2   WORD      ?
  bfOffBits     DWORD      ?
BITMAPFILEHEADER ENDS

HANDLETABLE STRUCT
  objectHandle  DWORD      ?
HANDLETABLE ENDS

METARECORD STRUCT
  rdSize        DWORD      ?
  rdFunction    WORD      ?
  rdParm1       WORD      ?
METARECORD ENDS

METAFILEPICT STRUCT
  imm   DWORD      ?
  xExt  DWORD      ?
  yExt  DWORD      ?
  hMF   DWORD      ?
METAFILEPICT ENDS

METAHEADER STRUCT
  mtType            WORD      ?
  mtHeaderSize      WORD      ?
  mtVersion         WORD      ?
  mtSize            DWORD     ?
  mtNoObjects       WORD      ?
  mtMaxRecord       DWORD     ?
  mtNoParameters    WORD      ?
METAHEADER ENDS

ENHMETARECORD STRUCT
  iType   DWORD      ?
  nSize   DWORD      ?
  dParm1  DWORD      ?
ENHMETARECORD ENDS

RECTL STRUCT
   left     LONG ?
   top      LONG ?
   right    LONG ?
   bottom   LONG ?
RECTL ENDS

ENHMETAHEADER STRUCT
  iType             DWORD      ?
  nSize             DWORD      ?
  rclBounds         RECTL <>
  rclFrame          RECTL <>
  dSignature        DWORD      ?
  nVersion          DWORD      ?
  nBytes            DWORD      ?
  nRecords          DWORD      ?
  nHandles          WORD      ?
  sReserved         WORD      ?
  nDescription      DWORD      ?
  offDescription    DWORD      ?
  nPalEntries       DWORD ?
  szlDevice         SIZEL <>
  szlMillimeters    SIZEL <>
  cbPixelFormat     DWORD ?
  offPixelFormat    DWORD ?
  bOpenGL           DWORD ?
ENHMETAHEADER ENDS

TEXTMETRICA STRUCT
  tmHeight              DWORD      ?
  tmAscent              DWORD      ?
  tmDescent             DWORD      ?
  tmInternalLeading     DWORD      ?
  tmExternalLeading     DWORD      ?
  tmAveCharWidth        DWORD      ?
  tmMaxCharWidth        DWORD      ?
  tmWeight              DWORD      ?
  tmOverhang            DWORD      ?
  tmDigitizedAspectX    DWORD      ?
  tmDigitizedAspectY    DWORD      ?
  tmFirstChar           BYTE      ?
  tmLastChar            BYTE      ?
  tmDefaultChar         BYTE      ?
  tmBreakChar           BYTE      ?
  tmItalic              BYTE      ?
  tmUnderlined          BYTE      ?
  tmStruckOut           BYTE      ?
  tmPitchAndFamily      BYTE      ?
  tmCharSet             BYTE      ?
TEXTMETRICA ENDS

TEXTMETRIC  equ  <TEXTMETRICA>

NEWTEXTMETRICA STRUCT
  tmHeight              DWORD      ?
  tmAscent              DWORD      ?
  tmDescent             DWORD      ?
  tmInternalLeading     DWORD      ?
  tmExternalLeading     DWORD      ?
  tmAveCharWidth        DWORD      ?
  tmMaxCharWidth        DWORD      ?
  tmWeight              DWORD      ?
  tmOverhang            DWORD      ?
  tmDigitizedAspectX    DWORD      ?
  tmDigitizedAspectY    DWORD      ?
  tmFirstChar           BYTE      ?
  tmLastChar            BYTE      ?
  tmDefaultChar         BYTE      ?
  tmBreakChar           BYTE      ?
  tmItalic              BYTE      ?
  tmUnderlined          BYTE      ?
  tmStruckOut           BYTE      ?

  tmPitchAndFamily      BYTE      ?
  tmCharSet             BYTE      ?
  ntmFlags              DWORD      ?
  ntmSizeEM             DWORD      ?
  ntmCellHeight         DWORD      ?
  ntmAveWidth           DWORD      ?
NEWTEXTMETRICA ENDS

NEWTEXTMETRIC  equ  <NEWTEXTMETRICA>

PELARRAY STRUCT
  paXCount  DWORD      ?
  paYCount  DWORD      ?
  paXExt    DWORD      ?
  paYExt    DWORD      ?
  paRGBs    BYTE      ?
PELARRAY ENDS

LOGBRUSH STRUCT
  lbStyle  DWORD      ?
  lbColor  DWORD      ?
  lbHatch  DWORD      ?
LOGBRUSH ENDS

LOGPEN STRUCT
  lopnStyle  DWORD      ?
  lopnWidth  POINT <>
  lopnColor  DWORD      ?
LOGPEN ENDS

EXTLOGPEN STRUCT
  elpPenStyle       DWORD      ?
  elpWidth          DWORD      ?
  elpBrushStyle     DWORD      ?
  elpColor          DWORD      ?
  elpHatch          DWORD      ?
  elpNumEntries     DWORD      ?
  elpStyleEntry     DWORD      ?
EXTLOGPEN ENDS

PALETTEENTRY STRUCT
  peRed     BYTE      ?
  peGreen   BYTE      ?
  peBlue    BYTE      ?
  peFlags   BYTE      ?
PALETTEENTRY ENDS

LOGPALETTE STRUCT
  palVersion        WORD      ?
  palNumEntries     WORD      ?
  palPalEntry       PALETTEENTRY <>
LOGPALETTE ENDS

LOGFONTA STRUCT
  lfHeight          DWORD      ?
  lfWidth           DWORD      ?
  lfEscapement      DWORD      ?
  lfOrientation     DWORD      ?
  lfWeight          DWORD      ?
  lfItalic          BYTE      ?
  lfUnderline       BYTE      ?
  lfStrikeOut       BYTE      ?
  lfCharSet         BYTE      ?
  lfOutPrecision    BYTE      ?
  lfClipPrecision   BYTE      ?
  lfQuality         BYTE      ?
  lfPitchAndFamily  BYTE      ?
  lfFaceName        BYTE LF_FACESIZE dup(?)
LOGFONTA ENDS

LOGFONT  equ  <LOGFONTA>

NONCLIENTMETRICSA STRUCT
  cbSize            DWORD      ?
  iBorderWidth      DWORD      ?
  iScrollWidth      DWORD      ?
  iScrollHeight     DWORD      ?
  iCaptionWidth     DWORD      ?
  iCaptionHeight    DWORD      ?
  lfCaptionFont     LOGFONTA <>
  iSMCaptionWidth   DWORD      ?
  iSMCaptionHeight  DWORD      ?
  lfSMCaptionFont   LOGFONTA <>
  iMenuWidth        DWORD      ?
  iMenuHeight       DWORD      ?
  lfMenuFont        LOGFONTA <>
  lfStatusFont      LOGFONTA <>
  lfMessageFont     LOGFONTA <>
NONCLIENTMETRICSA ENDS

NONCLIENTMETRICS  equ  <NONCLIENTMETRICSA>

ENUMLOGFONTA STRUCT
  elfLogFont    LOGFONTA  <>
  elfFullName   BYTE      LF_FULLFACESIZE dup(?)
  elfStyle      BYTE      LF_FACESIZE dup(?)
ENUMLOGFONTA ENDS

ENUMLOGFONT  equ  <ENUMLOGFONTA>

PANOSE STRUCT
  bFamilyType   BYTE      ?
  bSerifStyle   BYTE      ?
  bWeight       BYTE      ?
  bProportion   BYTE      ?
  bContrast     BYTE      ?
  bStrokeVariation  BYTE      ?
  bArmStyle     BYTE      ?
  bLetterform   BYTE      ?
  bMidline      BYTE      ?
  bXHeight      BYTE      ?
PANOSE ENDS

EXTLOGFONTA STRUCT
  elfLogFont    LOGFONTA <>
  elfFullName   BYTE     LF_FULLFACESIZE dup(?)
  elfStyle      BYTE     LF_FACESIZE dup(?)
  elfVersion    DWORD      ?
  elfStyleSize  DWORD      ?
  elfMatch      DWORD      ?
  elfReserved   DWORD      ?
  elfVendorId   BYTE      ELF_VENDOR_SIZE dup(?)
  elfCulture    DWORD      ?
  elfPanose     PANOSE     <>
EXTLOGFONTA ENDS

EXTLOGFONT  equ  <EXTLOGFONTA>

DEVMODE STRUCT
  dmDeviceName      BYTE   CCHDEVICENAME dup(?)
  dmSpecVersion     WORD      ?
  dmDriverVersion   WORD      ?
  dmSize            WORD      ?
  dmDriverExtra     WORD      ?
  dmFields          DWORD      ?
  union
     struct
        dmOrientation dw ?
        dmPaperSize dw ?
        dmPaperLength dw ?
        dmPaperWidth dw ?
     ends
      dmPosition POINTL <>
  ends
  dmScale           WORD      ?
  dmCopies          WORD      ?
  dmDefaultSource   WORD      ?
  dmPrintQuality    WORD      ?
  dmColor           WORD      ?
  dmDuplex          WORD      ?
  dmYResolution     WORD      ?
  dmTTOption        WORD      ?
  dmCollate         WORD      ?
  dmFormName        BYTE CCHFORMNAME dup (?)
  dmLogPixels       WORD      ?
  dmBitsPerPel      DWORD      ?
  dmPelsWidth       DWORD      ?
  dmPelsHeight      DWORD      ?
  dmDisplayFlags    DWORD      ?
  dmDisplayFrequency  DWORD      ?
  dmICMMethod       DWORD ?
  dmICMIntent       DWORD ?
  dmMediaType       DWORD ?
  dmDitherType      DWORD ?
  dmReserved1       DWORD ?
  dmReserved2       DWORD ?
DEVMODE ENDS

RGNDATAHEADER STRUCT
  dwSize    DWORD      ?
  iType     DWORD      ?
  nCount    DWORD      ?
  nRgnSize  DWORD      ?
  rcBound   RECT <>
RGNDATAHEADER ENDS

RGNDATA STRUCT
  rdh       RGNDATAHEADER <>
  Buffer    BYTE      ?
RGNDATA ENDS

ABC STRUCT
  abcA  DWORD      ?
  abcB  DWORD      ?
  abcC  DWORD      ?
ABC ENDS

ABCFLOAT STRUCT
  abcfA  DWORD      ?
  abcfB  DWORD      ?
  abcfC  DWORD      ?
ABCFLOAT ENDS

OUTLINETEXTMETRICA STRUCT
  otmSize                   DWORD      ?
  otmTextMetrics            TEXTMETRICA <>
  otmFiller                 BYTE       ?
  otmPanoseNumber           PANOSE     <>
  otmfsSelection            DWORD      ?
  otmfsType                 DWORD      ?
  otmsCharSlopeRise         DWORD      ?
  otmsCharSlopeRun          DWORD      ?
  otmItalicAngle            DWORD      ?
  otmEMSquare               DWORD      ?
  otmAscent                 DWORD      ?
  otmDescent                DWORD      ?
  otmLineGap                DWORD      ?
  otmsCapEmHeight           DWORD      ?
  otmsXHeight               DWORD      ?
  otmrcFontBox              RECT       <>
  otmMacAscent              DWORD      ?
  otmMacDescent             DWORD      ?
  otmMacLineGap             DWORD      ?
  otmusMinimumPPEM          DWORD      ?
  otmptSubscriptSize        POINT      <>
  otmptSubscriptOffset      POINT      <>
  otmptSuperscriptSize      POINT      <>
  otmptSuperscriptOffset    POINT      <>
  otmsStrikeoutSize         DWORD      ?
  otmsStrikeoutPosition     DWORD      ?
  otmsUnderscoreSize        DWORD      ?
  otmsUnderscorePosition    DWORD      ?
  otmpFamilyName            DWORD      ?
  otmpFaceName              DWORD      ?
  otmpStyleName             DWORD      ?
  otmpFullName              DWORD      ?
OUTLINETEXTMETRICA ENDS

OUTLINETEXTMETRIC  equ  <OUTLINETEXTMETRICA>

POLYTEXTA STRUCT
  x         DWORD       ?
  y         DWORD       ?
  n         DWORD       ?
  lpStr     DWORD       ?
  uiFlags   DWORD       ?
  icl       RECT        <>
  pdx       DWORD       ?
POLYTEXTA ENDS

POLYTEXT  equ  <POLYTEXTA>

FIXED STRUCT
  fract  WORD      ?
  Value  WORD      ?
FIXED ENDS

MAT2 STRUCT
  eM11  FIXED <>
  eM12  FIXED <>
  eM21  FIXED <>
  eM22  FIXED <>
MAT2 ENDS

GLYPHMETRICS STRUCT
  gmBlackBoxX       DWORD       ?
  gmBlackBoxY       DWORD       ?
  gmptGlyphOrigin   POINT       <>
  gmCellIncX        WORD        ?
  gmCellIncY        WORD        ?
GLYPHMETRICS ENDS

POINTFX STRUCT
  x  FIXED <>
  y  FIXED <>
POINTFX ENDS

TTPOLYCURVE STRUCT
  wType     WORD      ?
  cpfx      WORD      ?
  apfx      POINTFX <>
TTPOLYCURVE ENDS

TTPOLYGONHEADER STRUCT
  cb        DWORD      ?
  dwType    DWORD      ?
  pfxStart  POINTFX <>
TTPOLYGONHEADER ENDS

RASTERIZER_STATUS STRUCT
  nSize         WORD      ?
  wFlags        WORD      ?
  nLanguageID   WORD      ?
RASTERIZER_STATUS ENDS

COLORADJUSTMENT STRUCT
  caSize            WORD      ?
  caFlags           WORD      ?
  caIlluminantIndex WORD      ?
  caRedGamma        WORD      ?
  caGreenGamma      WORD      ?
  caBlueGamma       WORD      ?
  caReferenceBlack  WORD      ?
  caReferenceWhite  WORD      ?
  caContrast        WORD      ?
  caBrightness      WORD      ?
  caColorfulness    WORD      ?
  caRedGreenTint    WORD      ?
COLORADJUSTMENT ENDS

DOCINFOA STRUCT
  cbSize        DWORD       ?
  lpszDocName   DWORD       ?
  lpszOutput    DWORD       ?
  lpszDatatype  DWORD       ?
  fwType        DWORD       ?
DOCINFOA ENDS

DOCINFO  equ  <DOCINFOA>

KERNINGPAIR STRUCT
  wFirst        WORD       ?
  wSecond       WORD       ?
  iKernAmount   DWORD      ?
KERNINGPAIR ENDS

EMR STRUCT
  iType  DWORD      ?
  nSize  DWORD      ?
EMR ENDS

EMRTEXT STRUCT
  ptlReference  POINTL      <>
  nchars        DWORD       ?
  offString     DWORD       ?
  fOptions      DWORD       ?
  ircl          RECTL       <>
  offDx         DWORD       ?
EMRTEXT ENDS

EMRABORTPATH STRUCT
  emr  EMR <>
EMRABORTPATH ENDS

EMRBEGINPATH        equ  <EMRABORTPATH>
EMRENDPATH          equ  <EMRABORTPATH>
EMRCLOSEFIGURE      equ  <EMRABORTPATH>
EMRFLATTENPATH      equ  <EMRABORTPATH>
EMRWIDENPATH        equ  <EMRABORTPATH>
EMRSETMETARGN       equ  <EMRABORTPATH>
EMREMRSAVEDC        equ  <EMRABORTPATH>
EMRREALIZEPALETTE   equ  <EMRABORTPATH>

EMRSELECTCLIPPATH STRUCT
  emr       EMR         <>
  iMode     DWORD       ?
EMRSELECTCLIPPATH ENDS

EMRSETBKMODE            equ  <EMRSELECTCLIPPATH>
EMRSETMAPMODE           equ  <EMRSELECTCLIPPATH>
EMRSETPOLYFILLMODE      equ  <EMRSELECTCLIPPATH>
EMRSETROP2              equ  <EMRSELECTCLIPPATH>
EMRSETSTRETCHBLTMODE    equ  <EMRSELECTCLIPPATH>
EMRSETTEXTALIGN         equ  <EMRSELECTCLIPPATH>

EMRSETMITERLIMIT STRUCT
   emr          EMR <>
   eMiterLimit  DWORD ?
EMRSETMITERLIMIT ENDS

EMRRESTOREDC STRUCT
   emr          EMR <>
   iRelative    DWORD ?
EMRRESTOREDC ENDS

EMRSETARCDIRECTION STRUCT
   emr              EMR <>
   iArcDirection    DWORD ?
EMRSETARCDIRECTION ENDS

EMRSETMAPPERFLAGS STRUCT
   emr      EMR <>
   dwFlags  DWORD ?
EMRSETMAPPERFLAGS ENDS

EMRSETTEXTCOLOR STRUCT
   emr      EMR <>
   crColor  COLORREF ?
EMRSETTEXTCOLOR ENDS

EMRSETBKCOLOR  equ  <EMRSETTEXTCOLOR>

EMRSELECTOBJECT STRUCT
  emr       EMR <>
  ihObject  DWORD      ?
EMRSELECTOBJECT ENDS

EMRDELETEOBJECT  equ  <EMRSELECTOBJECT>

EMRSELECTCOLORSPACE STRUCT
  emr   EMR <>
  ihCS  DWORD ?
EMRSELECTCOLORSPACE ENDS

EMRDELETECOLORSPACE  equ  <EMRSELECTCOLORSPACE>

EMRSELECTPALETTE STRUCT
  emr       EMR <>
  ihPal     DWORD      ?
EMRSELECTPALETTE ENDS

EMRRESIZEPALETTE STRUCT
  emr       EMR <>
  ihPal     DWORD      ?
  cEntries  DWORD      ?
EMRRESIZEPALETTE ENDS

EMRSETPALETTEENTRIES STRUCT
  emr           EMR <>
  ihPal         DWORD      ?
  iStart        DWORD      ?
  cEntries      DWORD      ?
  aPalEntries   PALETTEENTRY <>
EMRSETPALETTEENTRIES ENDS

EMRSETCOLORADJUSTMENT STRUCT
  emr               EMR <>
  ColorAdjustment   COLORADJUSTMENT <>
EMRSETCOLORADJUSTMENT ENDS

EMRGDICOMMENT STRUCT
  emr       EMR <>
  cbData    DWORD      ?
  xData1    BYTE ?
EMRGDICOMMENT ENDS

EMREOF STRUCT
  emr               EMR <>
  nPalEntries       DWORD      ?
  offPalEntries     DWORD      ?
  nSizeLast         DWORD      ?
EMREOF ENDS

EMRLINETO STRUCT
  emr   EMR <>
  ptl   POINTL <>
EMRLINETO ENDS

EMRMOVETOEX  equ  <EMRLINETO>

EMROFFSETCLIPRGN STRUCT
  emr           EMR <>
  ptlOffset     POINTL <>
EMROFFSETCLIPRGN ENDS

EMRFILLPATH STRUCT
  emr           EMR <>
  rclBounds     RECTL <>
EMRFILLPATH ENDS

EMRSTROKEANDFILLPATH  equ  <EMRFILLPATH>
EMRSTROKEPATH         equ  <EMRFILLPATH>

EMREXCLUDECLIPRECT STRUCT
  emr       EMR   <>
  rclClip   RECTL <>
EMREXCLUDECLIPRECT ENDS

EMRINTERSECTCLIPRECT  equ  <EMREXCLUDECLIPRECT>

EMRSETVIEWPORTORGEX STRUCT
  emr           EMR    <>
  ptlOrigin     POINTL <>
EMRSETVIEWPORTORGEX ENDS

EMRSETWINDOWORGEX  equ  <EMRSETVIEWPORTORGEX>
EMRSETBRUSHORGEX   equ  <EMRSETVIEWPORTORGEX>

EMRSETVIEWPORTEXTEX STRUCT
  emr           EMR <>
  szlExtent     SIZEL <>
EMRSETVIEWPORTEXTEX ENDS

EMRSETWINDOWEXTEX  equ  <EMRSETVIEWPORTEXTEX>

EMRSCALEVIEWPORTEXTEX STRUCT
  emr       EMR         <>
  xNum      DWORD       ?
  xDenom    DWORD       ?
  yNum      DWORD       ?
  yDemon    DWORD       ?
EMRSCALEVIEWPORTEXTEX ENDS

EMRSCALEWINDOWEXTEX  equ  <EMRSCALEVIEWPORTEXTEX>

EMRSETWORLDTRANSFORM STRUCT
  emr       EMR         <>
  xform     XFORM       <>
EMRSETWORLDTRANSFORM ENDS

EMRMODIFYWORLDTRANSFORM STRUCT
  emr       EMR         <>
  xform     XFORM       <>
  iMode     DWORD       ?
EMRMODIFYWORLDTRANSFORM ENDS

EMRSETPIXELV STRUCT
  emr       EMR         <>
  ptlPixel  POINTL      <>
  crColor   COLORREF    ?
EMRSETPIXELV ENDS

EMREXTFLOODFILL STRUCT
  emr       EMR         <>
  ptlStart  POINTL      <>
  crColor   COLORREF    ?
  iMode     DWORD       ?
EMREXTFLOODFILL ENDS

EMRELLIPSE STRUCT
  emr       EMR     <>
  rclBox    RECTL   <>
EMRELLIPSE ENDS

EMRRECTANGLE  equ  <EMRELLIPSE>

EMRROUNDRECT STRUCT
  emr           EMR     <>
  rclBox        RECTL   <>
  szlCorner     SIZEL   <>
EMRROUNDRECT ENDS

EMRARC STRUCT
  emr       EMR     <>
  rclBox    RECTL   <>
  ptlStart  POINTL  <>
  ptlEnd    POINTL  <>
EMRARC ENDS

EMRARCTO    equ  <EMRARC>
EMRCHORD    equ  <EMRARC>
EMRPIE      equ  <EMRARC>

EMRANGLEARC STRUCT
  emr           EMR         <>
  ptlCenter     POINTL      <>
  nRadius       DWORD       ?
  eStartAngle   DWORD       ?
  eSweepAngle   DWORD       ?
EMRANGLEARC ENDS

EMRPOLYLINE STRUCT
  emr           EMR <>
  rclBounds     RECTL <>
  cptl          DWORD      ?
  aptl          POINTL <>
EMRPOLYLINE ENDS

EMRPOLYBEZIER   equ  <EMRPOLYLINE>
EMRPOLYGON      equ  <EMRPOLYLINE>
EMRPOLYBEZIERTO equ  <EMRPOLYLINE>

EMRPOLYLINE16 STRUCT
  emr           EMR     <>
  rclBounds     RECTL   <>
  cpts          DWORD   ?
  apts          POINTS  <>
EMRPOLYLINE16 ENDS

EMRPOLYBEZIER16     equ  <EMRPOLYLINE16>
EMRPOLYGON16        equ  <EMRPOLYLINE16>
EMRPLOYBEZIERTO16   equ  <EMRPOLYLINE16>
EMRPOLYLINETO16     equ  <EMRPOLYLINE16>

EMRPOLYDRAW STRUCT
  emr           EMR     <>
  rclBounds     RECTL   <>
  cptl          DWORD   ?
  aptl          POINTL  <>
  abTypes1      BYTE    ?
EMRPOLYDRAW ENDS

EMRPOLYDRAW16 STRUCT
  emr       EMR     <>
  rclBounds RECTL   <>
  cpts      DWORD   ?
  apts      POINTS  <>
  abTypes   BYTE    ?
EMRPOLYDRAW16 ENDS

EMRPOLYPOLYLINE STRUCT
  emr           EMR         <>
  rclBounds     RECTL       <>
  nPolys        DWORD       ?
  cptl          DWORD       ?
  aPolyCounts   DWORD       ?
  aptl          POINTL      <>
EMRPOLYPOLYLINE ENDS

EMRPOLYPOLYGON  equ  <EMRPOLYPOLYLINE>

EMRPOLYPOLYLINE16 STRUCT
  emr           EMR         <>
  rclBounds     RECTL       <>
  nPolys        DWORD       ?
  cpts          DWORD       ?
  aPolyCounts   DWORD       ?
  apts          POINTS      <>
EMRPOLYPOLYLINE16 ENDS

EMRPOLYPOLYGON16  equ  <EMRPOLYPOLYLINE16>

EMRINVERTRGN STRUCT
  emr           EMR         <>
  rclBounds     RECTL       <>
  cbRgnData     DWORD       ?
  RgnData       BYTE        ?
EMRINVERTRGN ENDS

EMRPAINTRGN  equ  <EMRINVERTRGN>

EMRFILLRGN STRUCT
  emr           EMR       <>
  rclBounds     RECTL     <>
  cbRgnData     DWORD      ?
  ihBrush       DWORD      ?
  RgnData       BYTE       ?
EMRFILLRGN ENDS

EMRFRAMERGN STRUCT
  emr           EMR         <>
  rclBounds     RECTL       <>
  cbRgnData     DWORD       ?
  ihBrush       DWORD       ?
  szlStroke     SIZEL       <>
  RgnData       BYTE        ?
EMRFRAMERGN ENDS

EMREXTSELECTCLIPRGN STRUCT
  emr           EMR         <>
  cbRgnData     DWORD       ?
  iMode         DWORD       ?
  RgnData       BYTE        ?
EMREXTSELECTCLIPRGN ENDS

EMREXTTEXTOUTA STRUCT
  emr               EMR         <>
  rclBounds         RECTL       <>
  iGraphicsMode     DWORD       ?
  exScale           DWORD       ?
  eyScale           DWORD       ?
  emrtext           EMRTEXT     <>
EMREXTTEXTOUTA ENDS

EMREXTTEXTOUT  equ  <EMREXTTEXTOUTA>

EMRBITBLT STRUCT
  emr EMR                 <>
  rclBounds     RECTL     <>
  xDest         DWORD      ?
  yDest         DWORD      ?
  cxDest        DWORD      ?
  cyDest        DWORD      ?
  dwRop         DWORD      ?
  xSrc          DWORD      ?
  ySrc          DWORD      ?
  xformSrc      XFORM     <>
  crBkColorSrc  COLORREF   ?
  iUsageSrc     DWORD      ?
  offBmiSrc     DWORD      ?
  cbBmiSrc      DWORD      ?
  offBitsSrc    DWORD      ?
  cbBitsSrc     DWORD      ?
EMRBITBLT ENDS

EMRSTRETCHBLT STRUCT
  emr           EMR        <>
  rclBounds     RECTL      <>
  xDest         DWORD      ?
  yDest         DWORD      ?
  cxDest        DWORD      ?
  cyDest        DWORD      ?
  dwRop         DWORD      ?
  xSrc          DWORD      ?
  ySrc          DWORD      ?
  xformSrc      XFORM     <>
  crBkColorSrc  COLORREF   ?
  iUsageSrc     DWORD      ?
  offBmiSrc     DWORD      ?
  cbBmiSrc      DWORD      ?
  offBitsSrc    DWORD      ?
  cbBitsSrc     DWORD      ?
  cxSrc         DWORD      ?
  cySrc         DWORD      ?
EMRSTRETCHBLT ENDS

EMRMASKBLT STRUCT
  emr           EMR <>
  rclBounds     RECTL <>
  xDest         DWORD      ?
  yDest         DWORD      ?
  cxDest        DWORD      ?
  cyDest        DWORD      ?
  dwRop         DWORD      ?
  xSrc2         DWORD      ?
  cyDest2       DWORD      ?
  dwRop2        DWORD      ?
  xSrc          DWORD      ?
  ySrc          DWORD      ?
  xformSrc      XFORM      <>
  crBkColorSrc  COLORREF   ?
  iUsageSrc     DWORD      ?
  offBmiSrc     DWORD      ?
  cbBmiSrc      DWORD      ?
  offBitsSrc    DWORD      ?
  cbBitsSrc     DWORD      ?
  xMask         DWORD      ?
  yMask         DWORD      ?
  iUsageMask    DWORD      ?
  offBmiMask    DWORD      ?
  cbBmiMask     DWORD      ?
  offBitsMask   DWORD      ?
  cbBitsMask    DWORD      ?
EMRMASKBLT ENDS

EMRPLGBLT STRUCT
  emr           EMR <>
  rclBounds     RECTL <>
  aptlDest      POINTL 3 dup(<>)
  xSrc          DWORD      ?
  ySrc          DWORD      ?
  cxSrc         DWORD      ?
  cySrc         DWORD      ?
  xformSrc      XFORM <>
  crBkColorSrc  COLORREF ?
  iUsageSrc     DWORD      ?
  offBmiSrc     DWORD      ?
  cbBmiSrc      DWORD      ?
  offBitsSrc    DWORD      ?
  cbBitsSrc     DWORD      ?
  xMask         DWORD      ?
  yMask         DWORD      ?
  iUsageMask    DWORD      ?
  offBmiMask    DWORD      ?
  cbBmiMask     DWORD      ?
  offBitsMask   DWORD      ?
  cbBitsMask    DWORD      ?
EMRPLGBLT ENDS

EMRSETDIBITSTODEVICE STRUCT
  emr           EMR       <>
  rclBounds     RECTL     <>
  xDest         DWORD      ?
  yDest         DWORD      ?
  xSrc          DWORD      ?
  ySrc          DWORD      ?
  cxSrc         DWORD      ?
  cySrc         DWORD      ?
  offBmiSrc     DWORD      ?
  cbBmiSrc      DWORD      ?
  offBitsSrc    DWORD      ?
  cbBitsSrc     DWORD      ?
  iUsageSrc     DWORD      ?
  iStartScan    DWORD      ?
  cScans        DWORD      ?
EMRSETDIBITSTODEVICE ENDS

EMRSTRETCHDIBITS STRUCT
  emr           EMR        <>
  rclBounds     RECTL      <>
  xDest         DWORD      ?
  yDest         DWORD      ?
  xSrc          DWORD      ?
  ySrc          DWORD      ?
  cxSrc         DWORD      ?
  cySrc         DWORD      ?
  offBmiSrc     DWORD      ?
  cbBmiSrc      DWORD      ?
  offBitsSrc    DWORD      ?
  cbBitsSrc     DWORD      ?
  iUsageSrc     DWORD      ?
  dwRop         DWORD      ?
  cxDest        DWORD      ?
  cyDest        DWORD      ?
EMRSTRETCHDIBITS ENDS

EMRCREATEPALETTE STRUCT
  emr       EMR <>
  ihPal     DWORD       ?
  lgpl      LOGPALETTE  <>
EMRCREATEPALETTE ENDS

EMRCREATEPEN STRUCT
  emr       EMR <>
  ihPen     DWORD       ?
  lopn      LOGPEN      <>
EMRCREATEPEN ENDS

EMREXTCREATEPEN STRUCT
  emr       EMR <>
  ihPen     DWORD      ?
  offBmi    DWORD      ?
  cbBmi     DWORD      ?
  offBits   DWORD      ?
  cbBits    DWORD      ?
  elp       EXTLOGPEN <>
EMREXTCREATEPEN ENDS

EMRCREATEBRUSHINDIRECT STRUCT
  emr       EMR <>
  ihBrush   DWORD      ?
  lb        LOGBRUSH <>
EMRCREATEBRUSHINDIRECT ENDS

EMRCREATEMONOBRUSH STRUCT
  emr       EMR        <>
  ihBrush   DWORD      ?
  iUsage    DWORD      ?
  offBmi    DWORD      ?
  cbBmi     DWORD      ?
  offBits   DWORD      ?
  cbBits    DWORD      ?
EMRCREATEMONOBRUSH ENDS

EMRCREATEDIBPATTERNBRUSHPT STRUCT
  emr       EMR <>
  ihBursh   DWORD      ?
  iUsage    DWORD      ?
  offBmi    DWORD      ?
  cbBmi     DWORD      ?
  offBits   DWORD      ?
  cbBits    DWORD      ?
EMRCREATEDIBPATTERNBRUSHPT ENDS

CIEXYZ STRUCT
        ciexyzX FXPT2DOT30 ?
        ciexyzY FXPT2DOT30 ?
        ciexyzZ FXPT2DOT30 ?
CIEXYZ ENDS

CIEXYZTRIPLE STRUCT
        ciexyzRed   CIEXYZ <>
        ciexyzGreen CIEXYZ <>
        ciexyzBlue  CIEXYZ <>
CIEXYZTRIPLE ENDS

BITMAPV4HEADER STRUCT
  bV4Size           DWORD      ?
  bV4Width          DWORD      ?
  bV4Height         DWORD      ?
  bV4Planes         WORD       ?
  bV4BitCount       WORD       ?
  bV4V4Compression  DWORD      ?
  bV4SizeImage      DWORD      ?
  bV4XPelsPerMeter  DWORD      ?
  bV4YPelsPerMeter  DWORD      ?
  bV4ClrUsed        DWORD      ?
  bV4ClrImportant   DWORD      ?
  bV4RedMask        DWORD      ?
  bV4GreenMask      DWORD      ?
  bV4BlueMask       DWORD      ?
  bV4AlphaMask      DWORD      ?
  bV4CSType         DWORD      ?
  bV4Endpoints      CIEXYZTRIPLE <>
  bV4GammaRed       DWORD      ?
  bV4GammaGreen     DWORD      ?
  bV4GammaBlue      DWORD      ?
BITMAPV4HEADER ENDS

FONTSIGNATURE STRUCT
  fsUsb4  DWORD      4 dup(?)
  fsCsb2  DWORD      2 dup(?)
FONTSIGNATURE ENDS

CHARSETINFO STRUCT
  ciCharset     DWORD      ?
  ciACP         DWORD      ?
  xlfs          FONTSIGNATURE <>
CHARSETINFO ENDS

LOCALESIGNATURE STRUCT
  lsUsb4            DWORD     4 dup(?)
  lsCsbDefault      DWORD     2 dup(?)
  lsCsbSupported    DWORD     2 dup(?)
LOCALESIGNATURE ENDS

NEWTEXTMETRICEXA STRUCT
  ntmTm         NEWTEXTMETRICA  <>
  ntmFontSig    FONTSIGNATURE   <>
NEWTEXTMETRICEXA ENDS

NEWTEXTMETRICEX  equ  <NEWTEXTMETRICEXA>

ENUMLOGFONTEXA STRUCT
  elfLogFont        LOGFONTA    <>
  elfFullName       BYTE        LF_FULLFACESIZE dup(?)
  elfStyle          BYTE        LF_FACESIZE dup(?)
  elfScript         BYTE        LF_FACESIZE dup(?)
ENUMLOGFONTEXA ENDS

ENUMLOGFONTEX  equ  <ENUMLOGFONTEXA>

GCP_RESULTSA STRUCT
  lStructSize   DWORD      ?
  lpOutString   DWORD      ?
  lpOrder       DWORD      ?
  lpDX          DWORD      ?
  lpCaretPos    DWORD      ?
  lpClass       DWORD      ?
  lpGlyphs      DWORD      ?
  nGlyphs       DWORD      ?
  nMaxFit       DWORD      ?
GCP_RESULTSA ENDS

GCP_RESULTS  equ  <GCP_RESULTSA>

LOGCOLORSPACEA STRUCT
  lcsSignature      DWORD      ?
  lcsVersion        DWORD      ?
  lcsSize           DWORD      ?
  lcsCSType         LCSCSTYPE ?
  lcsIntent         LCSGAMUTMATCH ?
  lcsEndPoints      CIEXYZTRIPLE <>
  lcsGammaRed       DWORD      ?
  lcsGammaGreen     DWORD      ?
  lcsGammaBlue      DWORD      ?
  lcsFileName       BYTE MAX_PATH dup (?)
LOGCOLORSPACEA ENDS

LOGCOLORSPACEW STRUCT
    lcsSignature    DWORD ?
    lcsVersion      DWORD ?
    lcsSize         DWORD ?
    lcsCSType       LCSCSTYPE ?
    lcsIntent       LCSGAMUTMATCH ?
    lcsEndpoints    CIEXYZTRIPLE <>
    lcsGammaRed     DWORD ?
    lcsGammaGreen   DWORD ?
    lcsGammaBlue    DWORD ?
    lcsFilename     WORD MAX_PATH dup(?)
LOGCOLORSPACEW ENDS

EMRCREATECOLORSPACE STRUCT
  emr   EMR <>
  ihCS  DWORD      ?
  lcs   LOGCOLORSPACEW <>
EMRCREATECOLORSPACE ENDS

CBTACTIVATESTRUCT STRUCT
  fMouse        DWORD    ?
  hWndActive    DWORD    ?
CBTACTIVATESTRUCT ENDS

EVENTMSG STRUCT
  message   DWORD      ?
  paramL    DWORD      ?
  paramH    DWORD      ?
  time      DWORD      ?
  hwnd      DWORD      ?
EVENTMSG ENDS

CWPSTRUCT STRUCT
  lParam    DWORD      ?
  wParam    DWORD      ?
  message   DWORD      ?
  hwnd      DWORD      ?
CWPSTRUCT ENDS

DEBUGHOOKINFO STRUCT
  hModuleHook   DWORD      ?
  Reserved      DWORD      ?
  lParam        DWORD      ?
  wParam        DWORD      ?
  code          DWORD      ?
DEBUGHOOKINFO ENDS

MOUSEHOOKSTRUCT STRUCT
  pt            POINT      <>
  hwnd          DWORD      ?
  wHitTestCode  DWORD      ?
  dwExtraInfo   DWORD      ?
MOUSEHOOKSTRUCT ENDS

MINMAXINFO STRUCT
  ptReserved        POINT <>
  ptMaxSize         POINT <>
  ptMaxPosition     POINT <>
  ptMinTrackSize    POINT <>

  ptMaxTrackSize    POINT <>
MINMAXINFO ENDS

COPYDATASTRUCT STRUCT
  dwData  DWORD      ?
  cbData  DWORD      ?
  lpData  DWORD      ?
COPYDATASTRUCT ENDS

WINDOWPOS STRUCT
  hwnd              DWORD      ?
  hWndInsertAfter   DWORD      ?
  x                 DWORD      ?
  y                 DWORD      ?
  lx                DWORD      ?
  cy                DWORD      ?
  flags             DWORD      ?
WINDOWPOS ENDS

ACCEL STRUCT
  fVirt     BYTE      ?
  key       WORD      ?
  cmd       WORD      ?
ACCEL ENDS

PAINTSTRUCT STRUCT
  hdc           DWORD      ?
  fErase        DWORD      ?
  rcPaint       RECT       <>
  fRestore      DWORD      ?
  fIncUpdate    DWORD      ?
  rgbReserved   BYTE 32 dup(?)
PAINTSTRUCT ENDS

CREATESTRUCTA STRUCT
  lpCreateParams    DWORD      ?
  hInstance         DWORD      ?
  hMenu             DWORD      ?
  hWndParent        DWORD      ?
  ly                DWORD      ?
  lx                DWORD      ?
  y                 DWORD      ?
  x                 DWORD      ?
  style             DWORD      ?
  lpszName          DWORD      ?
  lpszClass         DWORD      ?
  ExStyle           DWORD      ?
CREATESTRUCTA ENDS

CREATESTRUCT  equ  <CREATESTRUCTA>

CBT_CREATEWNDA STRUCT
  lpcs              DWORD      ?
  hWndInsertAfter   DWORD      ?
CBT_CREATEWNDA ENDS

CBT_CREATEWND  equ  <CBT_CREATEWNDA>

WINDOWPLACEMENT STRUCT
  iLength           DWORD       ?
  flags             DWORD       ?
  showCmd           DWORD       ?
  ptMinPosition     POINT       <>
  ptMaxPosition     POINT       <>
  rcNormalPosition  RECT        <>
WINDOWPLACEMENT ENDS

MEASUREITEMSTRUCT STRUCT
  CtlType       DWORD      ?
  CtlID         DWORD      ?
  itemID        DWORD      ?
  itemWidth     DWORD      ?
  itemHeight    DWORD      ?
  itemData      DWORD      ?
MEASUREITEMSTRUCT ENDS

DRAWITEMSTRUCT STRUCT
  CtlType       DWORD      ?
  CtlID         DWORD      ?
  itemID        DWORD      ?
  itemAction    DWORD      ?

  itemState     DWORD      ?
  hwndItem      DWORD      ?
  hdc           DWORD      ?
  rcItem        RECT <>
  itemData      DWORD      ?
DRAWITEMSTRUCT ENDS

DELETEITEMSTRUCT STRUCT
  CtlType   DWORD      ?
  CtlID     DWORD      ?
  itemID    DWORD      ?
  hwndItem  DWORD      ?
  itemData  DWORD      ?
DELETEITEMSTRUCT ENDS

COMPAREITEMSTRUCT STRUCT
  CtlType       DWORD      ?
  CtlID         DWORD      ?
  hwndItem      DWORD      ?
  itemID1       DWORD      ?
  itemData1     DWORD      ?
  itemID2       DWORD      ?
  itemData2     DWORD      ?
  dwLocaleId    DWORD ?
COMPAREITEMSTRUCT ENDS

DLGTEMPLATE STRUCT
  style             DWORD      ?
  dwExtendedStyle   DWORD      ?
  cdit              WORD      ?
  x                 WORD      ?
  y                 WORD      ?
  lx                WORD      ?
  ly                WORD      ?
DLGTEMPLATE ENDS

DLGITEMTEMPLATE STRUCT
  style DWORD      ?
  dwExtendedStyle  DWORD      ?
  x     WORD      ?
  y     WORD      ?
  lx    WORD      ?
  cy    WORD      ?
  id    WORD      ?
DLGITEMTEMPLATE ENDS

MENUITEMTEMPLATEHEADER STRUCT
  versionNumber     WORD      ?
  loffset           WORD      ?
MENUITEMTEMPLATEHEADER ENDS

MENUITEMTEMPLATE STRUCT
  mtOption      WORD      ?
  mtID          WORD      ?
  mtString      WORD      ?
MENUITEMTEMPLATE ENDS

ICONINFO STRUCT
  fIcon     DWORD      ?
  xHotspot  DWORD      ?
  yHotspot  DWORD      ?
  hbmMask   DWORD      ?
  hbmColor  DWORD      ?
ICONINFO ENDS

MDICREATESTRUCTA STRUCT
  szClass   DWORD      ?
  szTitle   DWORD      ?
  hOwner    DWORD      ?
  x         DWORD      ?
  y         DWORD      ?
  lx        DWORD      ?
  ly        DWORD      ?
  style     DWORD      ?
  lParam    DWORD      ?
MDICREATESTRUCTA ENDS

MDICREATESTRUCT  equ  <MDICREATESTRUCTA>

CLIENTCREATESTRUCT STRUCT
  hWindowMenu   DWORD      ?
  idFirstChild  DWORD      ?
CLIENTCREATESTRUCT ENDS

MULTIKEYHELPA STRUCT
  mkSize        DWORD      ?
  mkKeylist     BYTE       ?
  szKeyphrase   BYTE       ?
MULTIKEYHELPA ENDS

MULTIKEYHELP EQU <MULTIKEYHELPA>

HELPWININFOA STRUCT
  wStructSize   DWORD      ?
  x             DWORD      ?
  y             DWORD      ?
  lx            DWORD      ?
  ly            DWORD      ?
  wMax          DWORD      ?
  rgchMember    BYTE 2 dup (?)
HELPWININFOA ENDS

HELPWININFO  equ  <HELPWININFOA>

DDEACK STRUCT
  dw ?
DDEACK ENDS

DDEADVISE STRUCT
  dd ?
DDEADVISE ENDS  

DDEDATA STRUCT
        dd ?
        db ?
DDEDATA ENDS

DDEPOKE STRUCT
        dd ?
        db ?
DDEPOKE ENDS

DDELN STRUCT
        dd ?
DDELN ENDS

DDEUP STRUCT
        dd ?
        db ?
DDEUP ENDS

HSZPAIR STRUCT
  hszSvc    DWORD      ?
  hszTopic  DWORD      ?
HSZPAIR ENDS

SECURITY_QUALITY_OF_SERVICE STRUCT
  iLength               DWORD      ?
  Impersonationlevel    DWORD      ?
  ContextTrackingMode   BYTE       ?
  EffectiveOnly         BYTE       ?
SECURITY_QUALITY_OF_SERVICE ENDS

CONVCONTEXT STRUCT
  cb            DWORD      ?
  wFlags        DWORD      ?
  wCountryID    DWORD      ?
  iCodePage     DWORD      ?
  dwLangID      DWORD      ?
  dwSecurity    DWORD      ?
  qos           SECURITY_QUALITY_OF_SERVICE <>
CONVCONTEXT ENDS

CONVINFO STRUCT
  cb            DWORD      ?
  hUser         DWORD      ?
  hConvPartner  DWORD      ?
  hszSvcPartner DWORD      ?
  hszServiceReq DWORD      ?
  hszTopic      DWORD      ?
  hszItem       DWORD      ?
  wFmt          DWORD      ?
  wType         DWORD      ?
  wStatus       DWORD      ?
  wConvst       DWORD      ?
  wLastError    DWORD      ?
  hConvList     DWORD      ?
  ConvCtxt      CONVCONTEXT <>
  hwnd          DWORD      ?
  hwndPartner   DWORD      ?
CONVINFO ENDS

DDEML_MSG_HOOK_DATA STRUCT
  uiLo      DWORD      ?
  uiHi      DWORD      ?
  cbData    DWORD      ?
  xData     DWORD   8 dup(?)
DDEML_MSG_HOOK_DATA ENDS

MONMSGSTRUCT STRUCT
  cb        DWORD      ?
  hwndTo    DWORD      ?
  dwTime    DWORD      ?
  htask     DWORD      ?
  wMsg      DWORD      ?
  wParam    DWORD      ?
  lParam    DWORD      ?
  dmhd      DDEML_MSG_HOOK_DATA <>
MONMSGSTRUCT ENDS

MONCBSTRUCT STRUCT
  cb        DWORD      ?
  dwTime    DWORD      ?
  htask     DWORD      ?
  dwRet     DWORD      ?
  wType     DWORD      ?
  wFmt      DWORD      ?
  hConv     DWORD      ?
  hsz1      DWORD      ?
  hsz2      DWORD      ?
  hData     DWORD      ?
  dwData1   DWORD      ?
  dwData2   DWORD      ?
  cc        CONVCONTEXT <>
  cbData    DWORD      ?
  xData8    DWORD      ?
MONCBSTRUCT ENDS

MONHSZSTRUCTA STRUCT
  cb        DWORD      ?
  fsAction  DWORD      ?
  dwTime    DWORD      ?
  hsz       DWORD      ?
  htask     DWORD      ?
  xstr      BYTE       ?
MONHSZSTRUCTA ENDS

MONHSZSTRUCT  equ  <MONHSZSTRUCTA>

MONERRSTRUCT STRUCT
  cb            DWORD      ?
  wLastError    DWORD      ?
  dwTime        DWORD      ?
  htask         DWORD      ?
MONERRSTRUCT ENDS

MONLINKSTRUCT STRUCT
  cb            DWORD      ?
  dwTime        DWORD      ?
  htask         DWORD      ?
  fEstablished  DWORD      ?
  fNoData       DWORD      ?
  hszSvc        DWORD      ?
  hszTopic      DWORD      ?
  hszItem       DWORD      ?
  wFmt          DWORD      ?
  fServer       DWORD      ?
  hConvServer   DWORD      ?
  hConvClient   DWORD      ?
MONLINKSTRUCT ENDS

MONCONVSTRUCT STRUCT
  cb            DWORD      ?
  fConnect      DWORD      ?
  dwTime        DWORD      ?
  htask         DWORD      ?
  hszSvc        DWORD      ?
  hszTopic      DWORD      ?
  hConvClient   DWORD      ?
  hConvServer   DWORD      ?
MONCONVSTRUCT ENDS

smpte STRUCT
  hour      BYTE      ?
  minute    BYTE      ?
  sec       BYTE      ?
  frame     BYTE      ?
  fps       BYTE      ?
  dummy     BYTE      ?
  pad       BYTE      2 dup(?)
smpte ENDS

midi STRUCT
  songptrpos  DWORD      ?
midi ENDS

MMUNION UNION
  ms        DWORD ?
  sample    DWORD ?
  cb        DWORD ?
  ticks     DWORD ?

  smp       smpte <>
  midi1     midi <>
MMUNION ENDS

MMTIME STRUCT
  wType     DWORD      ?
  u         MMUNION <>
MMTIME ENDS

MIDIEVENT STRUCT
  dwDeltaTime   DWORD      ?
  dwStreamID    DWORD      ?
  dwEvent       DWORD      ?
  dwParms       DWORD      ?
MIDIEVENT ENDS

MIDISTRMBUFFVER STRUCT
  dwVersion     DWORD      ?
  dwMid         DWORD      ?
  dwOEMVersion  DWORD      ?
MIDISTRMBUFFVER ENDS

MIDIPROPTIMEDIV STRUCT
  cbStruct      DWORD      ?
  dwTimeDiv     DWORD      ?
MIDIPROPTIMEDIV ENDS

MIDIPROPTEMPO STRUCT
  cbStruct      DWORD      ?
  dwTempo       DWORD      ?
MIDIPROPTEMPO ENDS

MIXERCAPSA STRUCT
  wMid              WORD        ?
  wPid              WORD        ?
  vDriverVersion    MMVERSION   ?
  szPname           BYTE MAXPNAMELEN dup (?)
  fdwSupport        DWORD       ?
  cDestinations     DWORD       ?
MIXERCAPSA ENDS

MIXERCAPS  equ  <MIXERCAPSA>

Target STRUCT
  dwType        DWORD           ?
  dwDeviceID    DWORD           ?
  wMid          WORD            ?
  wPid          WORD            ?
  vDriverVersion  MMVERSION     ?
  szPname       BYTE MAXPNAMELEN dup (?)
Target ENDS


MIXERLINECONTROLSA STRUCT
  cbStruct  DWORD      ?
  dwLineID  DWORD      ?
  union
	  dwControlID   DWORD      ?
	  dwControlType DWORD      ?
  ends
  cControls  DWORD      ?
  cbmxctrl  DWORD       ?
  pamxctrl  DWORD       ?
MIXERLINECONTROLSA ENDS

MIXERLINECONTROLS  equ  <MIXERLINECONTROLSA>

MIXERCONTROLDETAILS STRUCT
  cbStruct      DWORD      ?
  dwControlID   DWORD      ?
  cChannels     DWORD      ?
  union
	  hwndOwner         DWORD ?
	  cMultipleItems    DWORD ?
  ends
  cbDetails     DWORD      ?
  paDetails     DWORD      ?
MIXERCONTROLDETAILS ENDS

MIXERCONTROLDETAILS_BOOLEAN STRUCT
  fValue  DWORD      ?
MIXERCONTROLDETAILS_BOOLEAN ENDS

MIXERCONTROLDETAILS_SIGNED STRUCT
  lValue  DWORD      ?
MIXERCONTROLDETAILS_SIGNED ENDS

MIXERCONTROLDETAILS_UNSIGNED STRUCT
  dwValue  DWORD      ?
MIXERCONTROLDETAILS_UNSIGNED ENDS

JOYINFOEX STRUCT
  dwSize            DWORD      ?
  dwFlags           DWORD      ?
  dwXpos            DWORD      ?
  dwYpos            DWORD      ?
  dwZpos            DWORD      ?
  dwRpos            DWORD      ?
  dwUpos            DWORD      ?
  dwVpos            DWORD      ?
  dwButtons         DWORD      ?
  dwButtonNumber    DWORD      ?
  dwPOV             DWORD      ?
  dwReserved1       DWORD      ?
  dwReserved2       DWORD      ?
JOYINFOEX ENDS

DRVCONFIGINFOEX STRUCT
  dwDCISize             DWORD      ?
  lpszDCISectionName    DWORD      ?
  lpszDCIAliasName      DWORD      ?
  dnDevNode             DWORD      ?
DRVCONFIGINFOEX ENDS

DRVCONFIGINFO STRUCT
  dwDCISize             DWORD      ?
  lpszDCISectionName    DWORD      ?
  lpszDCIAliasName      DWORD      ?
DRVCONFIGINFO ENDS

WAVEHDR STRUCT
  lpData            DWORD      ?
  dwBufferLength    DWORD      ?
  dwBytesRecorded   DWORD      ?
  dwUser            DWORD      ?
  dwFlags           DWORD      ?
  dwLoops           DWORD      ?
  lpNext            DWORD      ?
  Reserved          DWORD      ?
WAVEHDR ENDS

WAVEOUTCAPSA STRUCT
  wMid              WORD        ?
  wPid              WORD        ?
  vDriverVersion    MMVERSION   ?
  szPname           BYTE MAXPNAMELEN dup (?)
  dwFormats         DWORD       ?
  wChannels         WORD        ?
  wReserved1        WORD        ?
  dwSupport         DWORD       ?
WAVEOUTCAPSA ENDS

WAVEOUTCAPS  equ  <WAVEOUTCAPSA>

WAVEINCAPSA STRUCT
  wMid              WORD        ?
  wPid              WORD        ?
  vDriverVersion    MMVERSION   ?
  szPname           BYTE MAXPNAMELEN dup (?)
  dwFormats         DWORD       ?
  wChannels         WORD        ?
  wReserved1        WORD        ?
WAVEINCAPSA ENDS

WAVEINCAPS  equ  <WAVEINCAPSA>

WAVEFORMAT STRUCT
  wFormatTag        WORD      ?
  nChannels         WORD      ?
  nSamplesPerSec    DWORD     ?
  nAvgBytesPerSec   DWORD     ?
  nBlockAlign       WORD      ?
WAVEFORMAT ENDS

PCMWAVEFORMAT STRUCT
  wf                WAVEFORMAT <>
  wBitsPerSample    WORD       ?
PCMWAVEFORMAT ENDS

MIDIOUTCAPSA STRUCT
  wMid              WORD      ?
  wPid              WORD      ?
  vDriverVersion    MMVERSION ?
  szPname           BYTE MAXPNAMELEN dup (?)
  wTechnology       WORD      ?
  wVoices           WORD      ?
  wNotes            WORD      ?
  wChannelMask      WORD      ?
  dwSupport         DWORD     ?
MIDIOUTCAPSA ENDS

MIDIOUTCAPS  equ  <MIDIOUTCAPSA>

MIDIINCAPSA STRUCT
  wMid              WORD            ?
  wPid              WORD            ?
  vDriverVersion    MMVERSION       ?
  szPname           BYTE MAXPNAMELEN dup (?)
  dwSupport         DWORD ?
MIDIINCAPSA ENDS

MIDIINCAPS  equ  <MIDIINCAPSA>

MIDIHDR STRUCT
  lpData            DWORD      ?
  dwBufferiLength   DWORD      ?
  dwBytesRecorded   DWORD      ?
  dwUser            DWORD      ?
  dwFlags           DWORD      ?
  lpNext            DWORD      ?
  Reserved          DWORD      ?
  dwOffset          DWORD      ?
  dwReserved        DWORD 8 dup(?)
MIDIHDR ENDS

AUXCAPSA STRUCT
  wMid              WORD            ?
  wPid              WORD            ?
  vDriverVersion    MMVERSION       ?
  szPname           BYTE MAXPNAMELEN dup (?)
  wTechnology       WORD            ?
  dwSupport         DWORD           ?
AUXCAPSA ENDS

AUXCAPS  equ  <AUXCAPSA>

TIMECAPS STRUCT
  wPeriodMin  DWORD      ?
  wPeriodMax  DWORD      ?
TIMECAPS ENDS

JOYCAPSA STRUCT
  wMid          WORD      ?
  wPid          WORD      ?
  szPname       BYTE MAXPNAMELEN dup (?)
  wXmin         WORD      ?
  wXmax         WORD      ?
  wYmin         WORD      ?
  wYmax         WORD      ?
  wZmin         WORD      ?
  wZmax         WORD      ?
  wNumButtons   WORD      ?
  wPeriodMin    WORD      ?
  wPeriodMax    WORD      ?
  wRmin         DWORD ?
  wRmax         DWORD ?
  wUmin         DWORD ?
  wUmax         DWORD ?
  wVmin         DWORD ?
  wVmax         DWORD ?
  wCaps         DWORD ?
  wMaxAxes      DWORD ?
  wNumAxes      DWORD ?
  wMaxButtons   DWORD ?
  szRegKey      BYTE MAXPNAMELEN dup(?)
  szOEMVxD      BYTE MAX_JOYSTICKOEMVXDNAME dup(?)
JOYCAPSA ENDS

JOYCAPS  equ  <JOYCAPSA>

JOYINFO STRUCT
  wXpos     WORD      ?
  wYpos     WORD      ?
  wZpos     WORD      ?
  wButtons  WORD      ?
JOYINFO ENDS

MMIOINFO STRUCT
  dwFlags       DWORD      ?
  fccIOProc     DWORD      ?
  pIOProc       DWORD      ?
  wErrorRet     DWORD      ?
  htask         DWORD      ?
  cchBuffer     DWORD      ?
  pchBuffer     DWORD      ?
  pchNext       DWORD      ?
  pchEndRead    DWORD      ?
  pchEndWrite   DWORD      ?
  lBufOffset    DWORD      ?
  lDiskOffset   DWORD      ?
  adwInfo       DWORD     3 dup(?)
  dwReserved1   DWORD      ?
  dwReserved2   DWORD      ?
  hmmio         DWORD      ?
MMIOINFO ENDS

MMCKINFO STRUCT
  ckid          DWORD      ?
  ckSize        DWORD      ?
  fccType       DWORD      ?
  dwDataOffset  DWORD      ?
  dwFlags       DWORD      ?
MMCKINFO ENDS

MCI_GENERIC_PARMS STRUCT
  dwCallback  DWORD      ?
MCI_GENERIC_PARMS ENDS

MCI_OPEN_PARMS STRUCT
  dwCallback        DWORD      ?
  wDeviceID         DWORD      ?
  lpstrDeviceType   DWORD      ?
  lpstrElementName  DWORD      ?
  lpstrAlias        DWORD      ?
MCI_OPEN_PARMS ENDS

MCI_PLAY_PARMS STRUCT
  dwCallback    DWORD      ?
  dwFrom        DWORD      ?
  dwTo          DWORD      ?
MCI_PLAY_PARMS ENDS

MCI_SEEK_PARMS STRUCT
  dwCallback    DWORD      ?
  dwTo          DWORD      ?
MCI_SEEK_PARMS ENDS

MCI_STATUS_PARMS STRUCT
  dwCallback    DWORD      ?
  dwReturn      DWORD      ?
  dwItem        DWORD      ?
  dwTrack       WORD       ?
MCI_STATUS_PARMS ENDS

MCI_INFO_PARMSA STRUCT
  dwCallback    DWORD      ?
  lpstrReturn   DWORD      ?
  dwRetSize     DWORD      ?
MCI_INFO_PARMSA ENDS

MCI_INFO_PARMS  equ  <MCI_INFO_PARMSA>

MCI_GETDEVCAPS_PARMS STRUCT
  dwCallback    DWORD      ?
  dwReturn      DWORD      ?
  dwIten        DWORD      ?
MCI_GETDEVCAPS_PARMS ENDS

MCI_SYSINFO_PARMSA STRUCT
  dwCallback    DWORD      ?
  lpstrReturn   DWORD      ?
  dwRetSize     DWORD      ?
  dwNumber      DWORD      ?
  wDeviceType   DWORD      ?
MCI_SYSINFO_PARMSA ENDS

MCI_SYSINFO_PARMS  equ  <MCI_SYSINFO_PARMSA>

MCI_SET_PARMS STRUCT
  dwCallback    DWORD      ?
  dwTimeFormat  DWORD      ?
  dwAudio       DWORD      ?
MCI_SET_PARMS ENDS

MCI_BREAK_PARMS STRUCT
  dwCallback    DWORD      ?
  nVirtKey      DWORD      ?
  hwndBreak     DWORD      ?
MCI_BREAK_PARMS ENDS

MCI_SOUND_PARMS STRUCT
  dwCallback        DWORD      ?
  lpstrSoundName    DWORD      ?
MCI_SOUND_PARMS ENDS

MCI_SAVE_PARMSA STRUCT
  dwCallback  DWORD      ?
  lpFileName  DWORD      ?
MCI_SAVE_PARMSA ENDS

MCI_SAVE_PARMS  equ  <MCI_SAVE_PARMSA>

MCI_LOAD_PARMSA STRUCT
  dwCallback  DWORD      ?
  lpFileName  DWORD      ?
MCI_LOAD_PARMSA ENDS

MCI_LOAD_PARMS  equ  <MCI_LOAD_PARMSA>

MCI_RECORD_PARMS STRUCT
  dwCallback    DWORD      ?
  dwFrom        DWORD      ?
  dwTo          DWORD      ?
MCI_RECORD_PARMS ENDS

MCI_VD_PLAY_PARMS STRUCT
  dwCallback    DWORD      ?
  dwFrom        DWORD      ?
  dwTo          DWORD      ?
  dwSpeed       DWORD      ?
MCI_VD_PLAY_PARMS ENDS

MCI_VD_STEP_PARMS STRUCT
  dwCallback    DWORD      ?
  dwFrames      DWORD      ?
MCI_VD_STEP_PARMS ENDS

MCI_VD_ESCAPE_PARMSA STRUCT
  dwCallback    DWORD      ?
  lpstrCommand  DWORD      ?
MCI_VD_ESCAPE_PARMSA ENDS

MCI_VD_ESCAPE_PARMS  equ  <MCI_VD_ESCAPE_PARMSA>

MCI_WAVE_OPEN_PARMSA STRUCT
  dwCallback        DWORD      ?
  wDeviceID         DWORD      ?
  lpstrDeviceType   DWORD      ?
  lpstrElementName  DWORD      ?
  lpstrAlias        DWORD      ?
  dwBufferSeconds   DWORD      ?
MCI_WAVE_OPEN_PARMSA ENDS

MCI_WAVE_OPEN_PARMS  equ  <MCI_WAVE_OPEN_PARMSA>

MCI_WAVE_DELETE_PARMS STRUCT
  dwCallback    DWORD      ?
  dwFrom        DWORD      ?
  dwTo          DWORD      ?
MCI_WAVE_DELETE_PARMS ENDS


MCI_WAVE_SET_PARMS STRUCT
  dwCallback        DWORD      ?
  dwTimeFormat      DWORD      ?
  dwAudio           DWORD      ?
  wInput            DWORD      ?
  wOutput           DWORD      ?
  wFormatTag        WORD       ?
  wReserved2        WORD       ?
  nChannels         WORD       ?
  wReserved3        WORD       ?
  nSamplesPerSec    DWORD      ?
  nAvgBytesPerSec   DWORD      ?
  nBlockAlign       WORD       ?
  wReserved4        WORD       ?
  wBitsPerSample    WORD       ?
  wReserved5        WORD       ?
MCI_WAVE_SET_PARMS ENDS

MCI_SEQ_SET_PARMS STRUCT
  dwCallback    DWORD      ?
  dwTimeFormat  DWORD      ?
  dwAudio       DWORD      ?
  dwTempo       DWORD      ?
  dwPort        DWORD      ?
  dwSlave       DWORD      ?
  dwMaster      DWORD      ?
  dwOffset      DWORD      ?
MCI_SEQ_SET_PARMS ENDS

MCI_ANIM_OPEN_PARMSA STRUCT
  dwCallback        DWORD      ?
  wDeviceID         DWORD      ?
  lpstrDeviceType   DWORD      ?
  lpstrElementName  DWORD      ?
  lpstrAlias        DWORD      ?
  dwStyle           DWORD      ?
  hWndParent        DWORD      ?
MCI_ANIM_OPEN_PARMSA ENDS

MCI_ANIM_OPEN_PARMS  equ  <MCI_ANIM_OPEN_PARMSA>

MCI_ANIM_PLAY_PARMS STRUCT
  dwCallback    DWORD      ?
  dwFrom        DWORD      ?
  dwTo          DWORD      ?
  dwSpeed       DWORD      ?
MCI_ANIM_PLAY_PARMS ENDS

MCI_ANIM_STEP_PARMS STRUCT
  dwCallback    DWORD      ?
  dwFrames      DWORD      ?
MCI_ANIM_STEP_PARMS ENDS

MCI_ANIM_WINDOW_PARMS STRUCT
  dwCallback    DWORD      ?
  hwnd          DWORD      ?
  nCmdShow      DWORD      ?
  lpstrText     DWORD      ?
MCI_ANIM_WINDOW_PARMS ENDS

MCI_ANIM_RECT_PARMS STRUCT
  dwCallback    DWORD      ?
  rc            RECT       <>
MCI_ANIM_RECT_PARMS ENDS

MCI_ANIM_UPDATE_PARMS STRUCT
  dwCallback    DWORD      ?
  rc            RECT       <>
  hdc           DWORD      ?
MCI_ANIM_UPDATE_PARMS ENDS

MCI_OVLY_OPEN_PARMSA STRUCT
  dwCallback        DWORD      ?
  wDeviceID         DWORD      ?
  lpstrDeviceType   DWORD      ?
  lpstrElementName  DWORD      ?
  lpstrAlias        DWORD      ?
  dwStyle           DWORD      ?
  hWndParent        DWORD      ?
MCI_OVLY_OPEN_PARMSA ENDS

MCI_OVLY_OPEN_PARMS  equ  <MCI_OVLY_OPEN_PARMSA>

MCI_OVLY_WINDOW_PARMSA STRUCT
  dwCallback    DWORD       ?
  hwnd          DWORD       ?
  nCmdShow      DWORD       ?
  lpstrText     DWORD       ?
MCI_OVLY_WINDOW_PARMSA ENDS

MCI_OVLY_WINDOW_PARMS  equ  <MCI_OVLY_WINDOW_PARMSA>

MCI_OVLY_RECT_PARMS STRUCT
  dwCallback    DWORD       ?
  rc            RECT        <>
MCI_OVLY_RECT_PARMS ENDS

MCI_OVLY_SAVE_PARMSA STRUCT
  dwCallback    DWORD      ?
  lpFileName    DWORD      ?
  rc            RECT       <>
MCI_OVLY_SAVE_PARMSA ENDS

MCI_OVLY_SAVE_PARMS  equ  <MCI_OVLY_SAVE_PARMSA>

MCI_OVLY_LOAD_PARMSA STRUCT
  dwCallback    DWORD   ?
  lpFileName    DWORD   ?
  rc            RECT    <>
MCI_OVLY_LOAD_PARMSA ENDS

MCI_OVLY_LOAD_PARMS  equ  <MCI_OVLY_LOAD_PARMSA>

PRINTER_INFO_1A STRUCT
  flags         DWORD      ?
  pDescription  DWORD      ?
  pName         DWORD      ?
  pComment      DWORD      ?
PRINTER_INFO_1A ENDS

PRINTER_INFO_1  equ  <PRINTER_INFO_1A>

PRINTER_INFO_2A STRUCT
  pServerName           DWORD      ?
  pPrinterName          DWORD      ?
  pShareName            DWORD      ?
  pPortName             DWORD      ?
  pDriverName           DWORD      ?
  pComment              DWORD      ?
  pLocation             DWORD      ?
  pDevMode              DWORD      ?
  pSepFile              DWORD      ?
  pPrintProcessor       DWORD      ?
  pDatatype             DWORD      ?
  pParameters           DWORD      ?
  pSecurityDescriptor   DWORD      ?
  Attributes            DWORD      ?
  Priority              DWORD      ?
  DefaultPriority       DWORD      ?
  StartTime             DWORD      ?
  UntilTime             DWORD      ?
  Status                DWORD      ?
  cJobs                 DWORD      ?
  AveragePPM            DWORD      ?
PRINTER_INFO_2A ENDS


PRINTER_INFO_2  equ  <PRINTER_INFO_2A>

PRINTER_INFO_3 STRUCT
  pSecurityDescriptor  DWORD      ?
PRINTER_INFO_3 ENDS

PRINTER_INFO_4A STRUCT
  pPrinterName  DWORD ?
  pServerName   DWORD ?
  Attributes    DWORD ?
PRINTER_INFO_4A ENDS

PRINTER_INFO_4  equ  <PRINTER_INFO_4A>

JOB_INFO_1A STRUCT
  JobId         DWORD      ?
  pPrinterName  DWORD      ?
  pMachineName  DWORD      ?
  pUserName     DWORD      ?
  pDocument     DWORD      ?
  pDatatype     DWORD      ?
  pStatus       DWORD      ?
  Status        DWORD      ?
  Priority      DWORD      ?
  Position      DWORD      ?
  TotalPages    DWORD      ?
  PagesPrinted  DWORD      ?
  Submitted     SYSTEMTIME <>
JOB_INFO_1A ENDS

JOB_INFO_1  equ  <JOB_INFO_1A>

JOB_INFO_2A STRUCT
  JobId                 DWORD      ?
  pPrinterName          DWORD      ?
  pMachineName          DWORD      ?
  pUserName             DWORD      ?
  pDocument             DWORD      ?
  pNotifyName           DWORD      ?
  pDatatype             DWORD      ?
  pPrintProcessor       DWORD      ?
  pParameters           DWORD      ?
  pDriverName           DWORD      ?
  pDevMode              DWORD      ?
  pStatus               DWORD      ?
  pSecurityDescriptor   DWORD      ?
  Status                DWORD      ?
  Priority              DWORD      ?
  Position              DWORD      ?
  StartTime             DWORD      ?
  UntilTime             DWORD      ?
  TotalPages            DWORD      ?
  isize                 DWORD      ?
  Submitted             SYSTEMTIME <>
  time                  DWORD      ?
  PagesPrinted          DWORD      ?
JOB_INFO_2A ENDS

JOB_INFO_2  equ  <JOB_INFO_2A>

ADDJOB_INFO_1A STRUCT
  Path      DWORD      ?
  JobId     DWORD      ?
ADDJOB_INFO_1A ENDS

ADDJOB_INFO_1  equ  <ADDJOB_INFO_1A>

DRIVER_INFO_1A STRUCT
  pName     DWORD      ?
DRIVER_INFO_1A ENDS

DRIVER_INFO_1  equ  <DRIVER_INFO_1A>

DRIVER_INFO_2A STRUCT
  cVersion      DWORD      ?
  pName         DWORD      ?
  pEnvironment  DWORD      ?
  pDriverPath   DWORD      ?
  pDataFile     DWORD      ?
  pConfigFile   DWORD      ?
DRIVER_INFO_2A ENDS

DRIVER_INFO_2  equ  <DRIVER_INFO_2A>

DOC_INFO_1A STRUCT
  pDocName      DWORD      ?
  pOutputFile   DWORD      ?
  pDatatype     DWORD      ?
DOC_INFO_1A ENDS

DOC_INFO_1  equ  <DOC_INFO_1A>

FORM_INFO_1A STRUCT
  Flags         DWORD       ?
  pName         DWORD       ?
  iSize         SIZEL       <>
  ImageableArea RECTL       <>
FORM_INFO_1A ENDS

FORM_INFO_1  equ  <FORM_INFO_1A>

PRINTPROCESSOR_INFO_1A STRUCT
  pName  DWORD      ?
PRINTPROCESSOR_INFO_1A ENDS

PRINTPROCESSOR_INFO_1  equ  <PRINTPROCESSOR_INFO_1A>

PORT_INFO_1A STRUCT
  pName  DWORD      ?
PORT_INFO_1A ENDS

PORT_INFO_1  equ  <PORT_INFO_1A>

MONITOR_INFO_1A STRUCT
  pName  DWORD      ?
MONITOR_INFO_1A ENDS

MONITOR_INFO_1  equ  <MONITOR_INFO_1A>

MONITOR_INFO_2A STRUCT
  pName         DWORD      ?
  pEnvironment  DWORD      ?
  pDLLName      DWORD      ?
MONITOR_INFO_2A ENDS

MONITOR_INFO_2  equ  <MONITOR_INFO_2A>

DATATYPES_INFO_1A STRUCT
  pName  DWORD      ?
DATATYPES_INFO_1A ENDS

DATATYPES_INFO_1  equ  <DATATYPES_INFO_1A>

PRINTER_DEFAULTSA STRUCT
  pDatatype         DWORD       ?
  pDevMode          DWORD       ?
  DesiredAccess     ACCESS_MASK ?
PRINTER_DEFAULTSA ENDS

PRINTER_DEFAULTS  equ  <PRINTER_DEFAULTSA>

PRINTER_INFO_4A STRUCT
  pPrinterName      DWORD      ?
  pServerName       DWORD      ?
  Attributes        DWORD      ?
PRINTER_INFO_4A ENDS

PRINTER_INFO_4  equ  <PRINTER_INFO_4A>

PRINTER_INFO_5A STRUCT
  pPrinterName              DWORD      ?
  pPortName                 DWORD      ?
  Attributes                DWORD      ?
  DeviceNotSelectedTimeout  DWORD      ?
  TransmissionRetryTimeout  DWORD      ?
PRINTER_INFO_5A ENDS

PRINTER_INFO_5  equ  <PRINTER_INFO_5A>

DRIVER_INFO_3A STRUCT
  cVersion          DWORD      ?
  pName             DWORD      ?
  pEnvironment      DWORD      ?
  pDriverPath       DWORD      ?
  pDataFile         DWORD      ?
  pConfigFile       DWORD      ?
  pHelpFile         DWORD      ?
  pDependentFiles   DWORD      ?
  pMonitorName      DWORD      ?
  pDefaultDataType  DWORD      ?
DRIVER_INFO_3A ENDS

DRIVER_INFO_3  equ  <DRIVER_INFO_3A>

DOC_INFO_2A STRUCT
  pDocName      DWORD      ?
  pOutputFile   DWORD      ?
  pDatatype     DWORD      ?
  dwMode        DWORD      ?
  JobId         DWORD      ?
DOC_INFO_2A ENDS

DOC_INFO_2  equ  <DOC_INFO_2A>

PORT_INFO_2A STRUCT
  pPortName     DWORD      ?
  pMonitorName  DWORD      ?
  pDescription  DWORD      ?
  fPortType     DWORD      ?
  Reserved      DWORD      ?
PORT_INFO_2A ENDS

PORT_INFO_2  equ  <PORT_INFO_2A>

PROVIDOR_INFO_1A STRUCT
  pName         DWORD      ?
  pEnvironment  DWORD      ?
  pDLLName      DWORD      ?
PROVIDOR_INFO_1A ENDS

PROVIDOR_INFO_1  equ  <PROVIDOR_INFO_1A>

NETRESOURCEA STRUCT
  dwScope           DWORD      ?
  dwType            DWORD      ?
  dwDisplayType     DWORD      ?
  dwUsage           DWORD      ?
  lpLocalName       DWORD      ?
  lpRemoteName      DWORD      ?
  lpComment         DWORD      ?
  lpProvider        DWORD      ?
NETRESOURCEA ENDS

NETRESOURCE  equ  <NETRESOURCEA>

NCB STRUCT
  ncb_command       BYTE    ?
  ncb_retcode       BYTE    ?
  ncb_lsn           BYTE    ?
  ncb_num           BYTE    ?
  ncb_buffer        DWORD   ?
  ncb_length        WORD    ?
  ncb_callname      BYTE NCBNAMSZ dup (?)
  ncb_name          BYTE NCBNAMSZ dup (?)
  ncb_rto           BYTE    ?
  ncb_sto           BYTE    ?
  ncb_post          DWORD   ?
  ncb_lana_num      BYTE    ?
  ncb_cmd_cplt      BYTE    ?
  ncb_reserve       BYTE    10 DUP(?)
  ncb_event         DWORD   ?
NCB ENDS

ADAPTER_STATUS STRUCT
  adapter_address   BYTE 6 dup (?)
  rev_major         BYTE      ?
  reserved0         BYTE      ?
  adapter_type      BYTE      ?
  rev_minor         BYTE      ?
  duration          WORD      ?
  frmr_recv         WORD      ?

  frmr_xmit         WORD      ?
  iframe_recv_err   WORD      ?
  xmit_aborts       WORD      ?
  xmit_success      DWORD     ?
  recv_success      DWORD     ?
  iframe_xmit_err   WORD      ?
  recv_buff_unavail WORD      ?
  t1_timeouts       WORD      ?
  ti_timeouts       WORD      ?
  rserved1          DWORD     ?
  free_ncbs         WORD      ?
  max_cfg_ncbs      WORD      ?
  max_ncbs          WORD      ?
  xmit_buf_unavail  WORD      ?
  max_dgram_isize   WORD      ?
  pending_sess      WORD      ?
  max_cfg_sess      WORD      ?
  max_sess          WORD      ?
  max_sess_pkt_isize WORD     ?
  name_count        WORD      ?
ADAPTER_STATUS ENDS

NAME_BUFFER STRUCT
  xname         BYTE NCBNAMSZ dup (?)
  name_num      BYTE      ?
  name_flags    BYTE      ?
NAME_BUFFER ENDS

SESSION_HEADER STRUCT
  sess_name             BYTE      ?
  num_sess              BYTE      ?
  rcv_dg_outstanding    BYTE      ?
  rcv_any_outstanding   BYTE      ?
SESSION_HEADER ENDS

SESSION_BUFFER STRUCT
  lsn                   BYTE      ?
  State                 BYTE      ?
  local_name            BYTE NCBNAMSZ dup (?)
  remote_name           BYTE NCBNAMSZ dup (?)
  rcvs_outstanding      BYTE      ?
  sends_outstanding     BYTE      ?
SESSION_BUFFER ENDS

LANA_ENUM STRUCT
  iLength   BYTE      ?
  lana      BYTE    MAX_LANA+1 DUP(?)
LANA_ENUM ENDS

FIND_NAME_HEADER STRUCT
  node_count    WORD      ?
  Reserved      BYTE      ?
  unique_group  BYTE      ?
FIND_NAME_HEADER ENDS

FIND_NAME_BUFFER STRUCT
  iLength           BYTE        ?
  access_control    BYTE        ?
  frame_control     BYTE        ?
  destination_addr  BYTE  6 DUP(?)
  source_addr       BYTE  6 DUP(?)
  routing_info      BYTE 18 DUP(?)
FIND_NAME_BUFFER ENDS

ACTION_HEADER STRUCT
  transport_id  DWORD      ?
  action_code   WORD      ?
  Reserved      WORD      ?
ACTION_HEADER ENDS

CRGB STRUCT
  bRed      BYTE      ?
  bGreen    BYTE      ?
  bBlue     BYTE      ?
  bExtra    BYTE      ?

CRGB ENDS

SERVICE_STATUS STRUCT
  dwServiceType             DWORD      ?
  dwCurrentState            DWORD      ?
  dwControlsAccepted        DWORD      ?
  dwWin32ExitCode           DWORD      ?
  dwServiceSpecificExitCode DWORD      ?
  dwCheckPoint              DWORD      ?
  dwWaitHint                DWORD      ?
SERVICE_STATUS ENDS

ENUM_SERVICE_STATUSA STRUCT
  lpServiceName  DWORD      ?
  lpDisplayName  DWORD      ?
  ServiceStatus  SERVICE_STATUS <>
ENUM_SERVICE_STATUSA ENDS

ENUM_SERVICE_STATUS  equ  <ENUM_SERVICE_STATUSA>

QUERY_SERVICE_LOCK_STATUSA STRUCT
  fIsLocked         DWORD      ?
  lpLockOwner       DWORD      ?
  dwLockDuration    DWORD      ?
QUERY_SERVICE_LOCK_STATUSA ENDS

QUERY_SERVICE_LOCK_STATUS  equ  <QUERY_SERVICE_LOCK_STATUSA>

QUERY_SERVICE_CONFIGA STRUCT
  dwServiceType         DWORD      ?
  dwStartType           DWORD      ?
  dwErrorControl        DWORD      ?
  lpBinaryPathName      DWORD      ?
  lpLoadOrderGroup      DWORD      ?
  dwTagId               DWORD      ?
  lpDependencies        DWORD      ?
  lpServiceStartName    DWORD      ?
  lpDisplayName         DWORD      ?
QUERY_SERVICE_CONFIGA ENDS

QUERY_SERVICE_CONFIG  equ  <QUERY_SERVICE_CONFIGA>

SERVICE_TABLE_ENTRYA STRUCT
  lpServiceName  DWORD      ?
  lpServiceProc  DWORD      ?
SERVICE_TABLE_ENTRYA ENDS

SERVICE_TABLE_ENTRY  equ  <SERVICE_TABLE_ENTRYA>

PERF_DATA_BLOCK STRUCT
  Signature             WORD 4 dup (?)
  LittleEndian          DWORD      ?
  Version               DWORD      ?
  Revision              DWORD      ?
  TotalByteiLength      DWORD      ?
  HeaderiLength         DWORD      ?
  NumObjectTypes        DWORD      ?
  DefaultObject         DWORD      ?
  SystemTime            SYSTEMTIME <>
  Padding               DWORD ?
  PerfTime              LARGE_INTEGER <>
  PerfFreq              LARGE_INTEGER <>
  PerTime100nSec        LARGE_INTEGER <>
  SystemNameiLength     DWORD      ?
  SystemNameOffset      DWORD      ?
PERF_DATA_BLOCK ENDS

PERF_OBJECT_TYPE STRUCT
  TotalByteiLength      DWORD      ?
  DefinitioniLength     DWORD      ?
  HeaderiLength         DWORD      ?
  ObjectNameTitleIndex  DWORD      ?
  ObjectNameTitle       DWORD      ?
  ObjectHelpTitleIndex  DWORD      ?
  ObjectHelpTitle       DWORD      ?
  DetailLevel           DWORD      ?
  NumCounters           DWORD      ?
  DefaultCounter        DWORD      ?
  NumInstances          DWORD      ?
  CodePage              DWORD      ?
  PerfTime              LARGE_INTEGER <>
  PerfFreq              LARGE_INTEGER <>
PERF_OBJECT_TYPE ENDS

PERF_COUNTER_DEFINITION STRUCT
  ByteiLength               DWORD      ?
  CounterNameTitleIndex     DWORD      ?
  CounterNameTitle          DWORD      ?
  CounterHelpTitleIndex     DWORD      ?
  CounterHelpTitle          DWORD      ?
  DefaultScale              DWORD      ?
  DetailLevel               DWORD      ?
  CounterType               DWORD      ?
  CounterSize               DWORD      ?
  CounterOffset             DWORD      ?
PERF_COUNTER_DEFINITION ENDS

PERF_INSTANCE_DEFINITION STRUCT
  ByteiLength               DWORD      ?
  ParentObjectTitleIndex    DWORD      ?
  ParentObjectInstance      DWORD      ?
  UniqueID                  DWORD      ?
  NameOffset                DWORD      ?
  NameiLength               DWORD      ?
PERF_INSTANCE_DEFINITION ENDS

PERF_COUNTER_BLOCK STRUCT
  ByteiLength  DWORD      ?
PERF_COUNTER_BLOCK ENDS

COMPOSITIONFORM STRUCT
  dwStyle       DWORD      ?
  ptCurrentPos  POINT     <>
  rcArea        RECT      <>
COMPOSITIONFORM ENDS

CANDIDATEFORM STRUCT
  dwIndex       DWORD      ?
  dwStyle       DWORD      ?
  ptCurrentPos  POINT     <>
  rcArea        RECT      <>
CANDIDATEFORM ENDS

CANDIDATELIST STRUCT
  dwSize        DWORD      ?
  dwStyle       DWORD      ?
  dwCount       DWORD      ?
  dwSelection   DWORD      ?
  dwPageStart   DWORD      ?
  dwPageSize    DWORD      ?
  dwOffset1     DWORD      ?
CANDIDATELIST ENDS

STYLEBUFA STRUCT

  dwStyle           DWORD      ?
  szDescription     BYTE STYLE_DESCRIPTION_SIZE dup (?)
STYLEBUFA ENDS

STYLEBUF  equ  <STYLEBUFA>

MODEMDEVCAPS STRUCT
  dwActualSize                  DWORD      ?
  dwRequiredSize                DWORD      ?
  dwDevSpecificOffset           DWORD      ?
  dwDevSpecificSize             DWORD      ?
  dwModemProviderVersion        DWORD      ?
  dwModemManufacturerOffset     DWORD      ?
  dwModemManufacturerSize       DWORD      ?
  dwModemModelOffset            DWORD      ?
  dwModemModelSize              DWORD      ?
  dwModemVersionOffset          DWORD      ?
  dwModemVersionSize            DWORD      ?
  dwDialOptions                 DWORD      ?
  dwCallSetupFailTimer          DWORD      ?
  dwInactivityTimeout           DWORD      ?
  dwSpeakerVolume               DWORD      ?
  dwSpeakerMode                 DWORD      ?
  dwModemOptions                DWORD      ?
  dwMaxDTERate                  DWORD      ?
  dwMaxDCERate                  DWORD      ?
  abVariablePortion             BYTE       ?
MODEMDEVCAPS ENDS

MODEMSETTINGS STRUCT
  dwActualSize              DWORD      ?
  dwRequiredSize            DWORD      ?
  dwDevSpecificOffset       DWORD      ?
  dwDevSpecificSize         DWORD      ?
  dwCallSetupFailTimer      DWORD      ?
  dwInactivityTimeout       DWORD      ?
  dwSpeakerVolume           DWORD      ?
  dwSpeakerMode             DWORD      ?
  dwPreferredModemOptions   DWORD      ?
  dwNegotiatedModemOptions  DWORD      ?
  dwNegotiatedDCERate       DWORD      ?
  abVariablePortion         BYTE       ?
MODEMSETTINGS ENDS

DRAGINFOA STRUCT
  uSize         DWORD      ?
  pt            POINT      <>
  fNC           DWORD      ?
  lpFileList    DWORD      ?
  grfKeyState   DWORD      ?
DRAGINFOA ENDS

DRAGINFO  equ  <DRAGINFOA>

APPBARDATA STRUCT
  cbSize            DWORD      ?
  hwnd              DWORD      ?
  uCallbackMessage  DWORD      ?
  uEdge             DWORD      ?
  rc                RECT       <>
  lParam            DWORD      ?
APPBARDATA ENDS

SHFILEOPSTRUCTA STRUCT
  hwnd                  DWORD      ?
  wFunc                 DWORD      ?
  pFrom                 DWORD      ?
  pTo                   DWORD      ?
  fFlags                FILEOP_FLAGS ?
  fAnyOperationsAborted DWORD      ?
  hNameMappings         DWORD      ?
  lpszProgressTitle     DWORD      ?
SHFILEOPSTRUCTA ENDS

SHFILEOPSTRUCT  equ  <SHFILEOPSTRUCTA>

SHNAMEMAPPINGA STRUCT
  pszOldPath  DWORD      ?
  pszNewPath  DWORD      ?
  cchOldPath  DWORD      ?
  cchNewPath  DWORD      ?
SHNAMEMAPPINGA ENDS

SHNAMEMAPPING  equ  <SHNAMEMAPPINGA>

SHELLEXECUTEINFOA STRUCT
  cbSize        DWORD       ?
  fMask         DWORD       ?
  hwnd          DWORD       ?
  lpVerb        DWORD       ?
  lpFile        DWORD       ?
  lpParameters  DWORD       ?
  lpDirectory   DWORD       ?
  nShow         DWORD       ?
  hInstApp      DWORD       ?
  lpIDList      DWORD       ?
  lpClass       DWORD       ?
  hkeyClass     DWORD       ?
  dwHotKey      DWORD       ?
  union
      hIcon     DWORD       ?
      hMonitor  DWORD       ?
  ends
  hProcess      DWORD       ?
SHELLEXECUTEINFOA ENDS

SHELLEXECUTEINFO  equ  <SHELLEXECUTEINFOA>

NOTIFYICONDATAA STRUCT
  cbSize            DWORD      ?
  hwnd              DWORD      ?
  uID               DWORD      ?
  uFlags            DWORD      ?
  uCallbackMessage  DWORD      ?
  hIcon             DWORD      ?
  szTip             BYTE 64 dup (?)
NOTIFYICONDATAA ENDS

NOTIFYICONDATA  equ  <NOTIFYICONDATAA>

SHFILEINFOA STRUCT
  hIcon             DWORD       ?
  iIcon             DWORD       ?
  dwAttributes      DWORD       ?
  szDisplayName     BYTE        MAX_PATH DUP(?)
  szTypeName        BYTE        80 dup(?)
SHFILEINFOA ENDS

SHFILEINFO  equ  <SHFILEINFOA>

VS_FIXEDFILEINFO STRUCT
  dwSignature           DWORD      ?
  dwStrucVersion        DWORD      ?
  dwFileVersionMS       DWORD      ?
  dwFileVersionLS       DWORD      ?
  dwProductVersionMS    DWORD      ?
  dwProductVersionLS    DWORD      ?
  dwFileFlagsMask       DWORD      ?
  dwFileFlags           DWORD      ?
  dwFileOS              DWORD      ?
  dwFileType            DWORD      ?
  dwFileSubtype         DWORD      ?
  dwFileDateMS          DWORD      ?
  dwFileDateLS          DWORD      ?
VS_FIXEDFILEINFO ENDS

ICONMETRICSA STRUCT
  cbSize        DWORD      ?
  iHorzSpacing  DWORD      ?
  iVertSpacing  DWORD      ?
  iTitleWrap    DWORD      ?
  lfFont        LOGFONTA   <>
ICONMETRICSA ENDS

ICONMETRICS  equ  <ICONMETRICSA>

HELPINFO STRUCT
  cbSize        DWORD      ?
  iContextType  DWORD      ?
  iCtrlId       DWORD      ?
  hItemHandle   DWORD      ?
  dwContextId   DWORD      ?
  MousePos      POINT     <>
HELPINFO ENDS

ANIMATIONINFO STRUCT
  cbSize        DWORD      ?
  iMinAnimate   DWORD      ?
ANIMATIONINFO ENDS

MINIMIZEDMETRICS STRUCT
  cbSize    DWORD      ?
  iWidth    DWORD      ?
  iHorzGap  DWORD      ?
  iVertGap  DWORD      ?
  iArrange  DWORD      ?
MINIMIZEDMETRICS ENDS

OSVERSIONINFOA STRUCT
  dwOSVersionInfoSize   DWORD      ?
  dwMajorVersion        DWORD      ?
  dwMinorVersion        DWORD      ?
  dwBuildNumber         DWORD      ?
  dwPlatformId          DWORD      ?
  szCSDVersion          BYTE 128 dup (?)
OSVERSIONINFOA ENDS

OSVERSIONINFO  equ  <OSVERSIONINFOA>

SYSTEM_POWER_STATUS STRUCT
  ACLineStatus          BYTE      ?
  BatteryFlag           BYTE      ?
  BatteryLifePercent    BYTE      ?
  Reserved1             BYTE      ?
  BatteryLifeTime       DWORD      ?
  BatteryFullLifeTime   DWORD      ?
SYSTEM_POWER_STATUS ENDS


DEVNAMES STRUCT
  wDriverOffset     WORD      ?
  wDeviceOffset     WORD      ?
  wOutputOffset     WORD      ?
  wDefault          WORD      ?
DEVNAMES ENDS

PAGESETUPDLGA STRUCT
  lStructSize               DWORD      ?
  hwndOwner                 DWORD      ?
  hDevMode                  DWORD      ?
  hDevNames                 DWORD      ?

  union
  	Flags                     DWORD      ?
  	flags dd ?
  ends
  ptPaperSize               POINT     <>
  rtMinMargin               RECT      <>
  rtMargin                  RECT      <>
  hInstance                 DWORD      ?
  lCustData                 DWORD      ?
  lpfnPageSetupHook         DWORD      ?
  lpfnPagePaintHook         DWORD      ?
  lpPageSetupTemplateName   DWORD      ?
  hPageSetupTemplate        DWORD      ?
PAGESETUPDLGA ENDS

PAGESETUPDLG  equ  <PAGESETUPDLGA>

COMMCONFIG STRUCT
  dwSize                DWORD      ?
  wVersion              WORD       ?
  wReserved             WORD       ?
  dcbx                  DCB        <>
  dwProviderSubType     DWORD      ?
  dwProviderOffset      DWORD      ?
  dwProviderSize        DWORD      ?
  wcProviderData        WORD       ?
COMMCONFIG ENDS

PIXELFORMATDESCRIPTOR STRUCT
  nSize             WORD      ?
  nVersion          WORD      ?
  dwFlags           DWORD      ?
  iPixelType        BYTE      ?
  cColorBits        BYTE      ?
  cRedBits          BYTE      ?
  cRedShift         BYTE      ?
  cGreenBits        BYTE      ?
  cGreenShift       BYTE      ?
  cBlueBits         BYTE      ?
  cBlueShift        BYTE      ?
  cAlphaBits        BYTE      ?
  cAlphaShift       BYTE      ?
  cAccumBits        BYTE      ?
  cAccumRedBits     BYTE      ?
  cAccumGreenBits   BYTE      ?
  cAccumBlueBits    BYTE      ?
  cAccumAlphaBits   BYTE      ?
  cDepthBits        BYTE      ?
  cStencilBits      BYTE      ?
  cAuxBuffers       BYTE      ?
  iLayerType        BYTE      ?
  bReserved         BYTE      ?
  dwLayerMask       DWORD      ?
  dwVisibleMask     DWORD      ?
  dwDamageMask      DWORD      ?
PIXELFORMATDESCRIPTOR ENDS

DRAWTEXTPARAMS STRUCT
  cbSize            DWORD      ?
  iTabiLength       DWORD      ?
  iLeftMargin       DWORD      ?
  iRightMargin      DWORD      ?
  uiiLengthDrawn    DWORD      ?
DRAWTEXTPARAMS ENDS

MENUITEMINFOA STRUCT
  cbSize        DWORD      ?
  fMask         DWORD      ?
  fType         DWORD      ?
  fState        DWORD      ?
  wID           DWORD      ?
  hSubMenu      DWORD      ?
  hbmpChecked   DWORD      ?
  hbmpUnchecked DWORD      ?
  dwItemData    DWORD      ?
  dwTypeData    DWORD      ?
  cch           DWORD      ?
MENUITEMINFOA ENDS

MENUITEMINFO  equ  <MENUITEMINFOA>

SCROLLINFO STRUCT
  cbSize        DWORD      ?
  fMask         DWORD      ?
  nMin          DWORD      ?
  nMax          DWORD      ?
  nPage         DWORD      ?
  nPos          DWORD      ?
  nTrackPos     DWORD      ?
SCROLLINFO ENDS

MSGBOXPARAMSA STRUCT
  cbSize                DWORD      ?
  hwndOwner             DWORD      ?
  hInstance             DWORD      ?
  lpszText              DWORD      ?
  lpszCaption           DWORD      ?
  dwStyle               DWORD      ?
  lpszIcon              DWORD      ?
  dwContextHelpId       DWORD      ?
  lpfnMsgBoxCallback    DWORD      ?
  dwLanguageId          DWORD      ?
MSGBOXPARAMSA ENDS

MSGBOXPARAMS EQU <MSGBOXPARAMSA>

CREATE_THREAD_DEBUG_INFO STRUCT
  hThread               DWORD      ?
  lpThreadLocalBase     DWORD      ?
  lpStartAddress        DWORD      ?
CREATE_THREAD_DEBUG_INFO ENDS

EXIT_THREAD_DEBUG_INFO STRUCT
  dwExitCode  DWORD      ?
EXIT_THREAD_DEBUG_INFO ENDS

EXIT_PROCESS_DEBUG_INFO STRUCT
  dwExitCode  DWORD      ?
EXIT_PROCESS_DEBUG_INFO ENDS

RIP_INFO STRUCT
  dwError  DWORD      ?
  dwType   DWORD      ?
RIP_INFO ENDS


DEBUGSTRUCT UNION
  Exception         EXCEPTION_DEBUG_INFO <{?,?,?,?,?,EXCEPTION_MAXIMUM_PARAMETERS dup(?)},?>
  CreateThread      CREATE_THREAD_DEBUG_INFO <?,?,?>
  CreateProcessInfo CREATE_PROCESS_DEBUG_INFO <?,?,?,?,?,?,?,?,?,?>
  ExitThread        EXIT_THREAD_DEBUG_INFO <?>
  ExitProcess       EXIT_PROCESS_DEBUG_INFO <?>
  LoadDll           LOAD_DLL_DEBUG_INFO <?,?,?,?,?,?>
  UnloadDll         UNLOAD_DLL_DEBUG_INFO <?>
  DebugString       OUTPUT_DEBUG_STRING_INFO <?,?,?>
  RipInfo           RIP_INFO <?,?>
DEBUGSTRUCT ENDS

DEBUG_EVENT STRUCT
  dwDebugEventCode  DWORD       ?
  dwProcessId       DWORD       ?
  dwThreadId        DWORD       ?
  u                 DEBUGSTRUCT <>
DEBUG_EVENT ENDS

IMAGE_DATA_DIRECTORY STRUCT
  VirtualAddress    DWORD      ?
  isize             DWORD      ?
IMAGE_DATA_DIRECTORY ENDS

IMAGE_OPTIONAL_HEADER32 STRUCT
  Magic                         WORD       ?
  MajorLinkerVersion            BYTE       ?
  MinorLinkerVersion            BYTE       ?
  SizeOfCode                    DWORD      ?
  SizeOfInitializedData         DWORD      ?
  SizeOfUninitializedData       DWORD      ?
  AddressOfEntryPoint           DWORD      ?
  BaseOfCode                    DWORD      ?
  BaseOfData                    DWORD      ?
  ImageBase                     DWORD      ?
  SectionAlignment              DWORD      ?
  FileAlignment                 DWORD      ?
  MajorOperatingSystemVersion   WORD       ?
  MinorOperatingSystemVersion   WORD       ?
  MajorImageVersion             WORD       ?
  MinorImageVersion             WORD       ?
  MajorSubsystemVersion         WORD       ?
  MinorSubsystemVersion         WORD       ?
  Win32VersionValue             DWORD      ?
  SizeOfImage                   DWORD      ?
  SizeOfHeaders                 DWORD      ?
  CheckSum                      DWORD      ?
  Subsystem                     WORD       ?
  DllCharacteristics            WORD       ?
  SizeOfStackReserve            DWORD      ?
  SizeOfStackCommit             DWORD      ?
  SizeOfHeapReserve             DWORD      ?
  SizeOfHeapCommit              DWORD      ?
  LoaderFlags                   DWORD      ?
  NumberOfRvaAndSizes           DWORD      ?
  DataDirectory                 IMAGE_DATA_DIRECTORY IMAGE_NUMBEROF_DIRECTORY_ENTRIES dup(<>)
IMAGE_OPTIONAL_HEADER32 ENDS

IMAGE_OPTIONAL_HEADER  equ  <IMAGE_OPTIONAL_HEADER32>

IMAGE_FILE_HEADER STRUCT
  Machine               WORD    ?
  NumberOfSections      WORD    ?
  TimeDateStamp         DWORD   ?
  PointerToSymbolTable  DWORD   ?
  NumberOfSymbols       DWORD   ?
  SizeOfOptionalHeader  WORD    ?
  Characteristics       WORD    ?
IMAGE_FILE_HEADER ENDS

IMAGE_NT_HEADERS STRUCT
  Signature         DWORD                   ?
  FileHeader        IMAGE_FILE_HEADER       <>
  OptionalHeader    IMAGE_OPTIONAL_HEADER32 <>
IMAGE_NT_HEADERS ENDS

IMAGE_EXPORT_DIRECTORY STRUCT
  Characteristics           DWORD      ?
  TimeDateStamp             DWORD      ?
  MajorVersion              WORD       ?
  MinorVersion              WORD       ?
  nName                     DWORD      ?
  nBase                     DWORD      ?
  NumberOfFunctions         DWORD      ?
  NumberOfNames             DWORD      ?
  AddressOfFunctions        DWORD      ?
  AddressOfNames            DWORD      ?
  AddressOfNameOrdinals     DWORD      ?
IMAGE_EXPORT_DIRECTORY ENDS

IMAGE_DOS_HEADER STRUCT
  e_magic           WORD      ?
  e_cblp            WORD      ?
  e_cp              WORD      ?
  e_crlc            WORD      ?
  e_cparhdr         WORD      ?
  e_minalloc        WORD      ?
  e_maxalloc        WORD      ?
  e_ss              WORD      ?
  e_sp              WORD      ?
  e_csum            WORD      ?
  e_ip              WORD      ?
  e_cs              WORD      ?
  e_lfarlc          WORD      ?
  e_ovno            WORD      ?
  e_res             WORD   4 dup(?)
  e_oemid           WORD      ?
  e_oeminfo         WORD      ?
  e_res2            WORD  10 dup(?)
  e_lfanew          DWORD      ?
IMAGE_DOS_HEADER ENDS

; ------------------------ wsock32 structures -----------------------------

fd_set STRUCT
  fd_count  DWORD      ?
  fd_array  SOCKET FD_SETSIZE dup(?)
fd_set ENDS

timeval STRUCT
  tv_sec   DWORD      ?
  tv_usec  DWORD      ?
timeval ENDS

S_UN_B STRUCT
  s_b1 BYTE ?
  s_b2 BYTE ?
  s_b3 BYTE ?
  s_b4 BYTE ?
S_UN_B ENDS

S_UN_W STRUCT 
  s_w1 WORD ?
  s_w2 WORD ?
S_UN_W ENDS

ADDRESS_UNION UNION 
   S_un_b S_UN_B <>
   S_un_w S_UN_W <>
   S_addr DWORD ?
ADDRESS_UNION ENDS

in_addr STRUCT
  S_un ADDRESS_UNION <>
in_addr ENDS

sockaddr_in STRUCT
  sin_family    WORD      ?
  sin_port      WORD      ?
  sin_addr      in_addr <>
  sin_zero      BYTE 8 dup (?)
sockaddr_in ENDS

sockaddr STRUCT
  sa_family     WORD      ?
  sa_data       BYTE 14 dup(?)
sockaddr ENDS

WSADATA STRUCT
  wVersion      WORD      ?
  wHighVersion  WORD      ?
  szDescription BYTE WSADESCRIPTION_LEN + 1 dup (?)
  szSystemStatus  BYTE WSASYS_STATUS_LEN + 1 dup (?)
  iMaxSockets   WORD      ?
  iMaxUdpDg     WORD      ?
  lpVendorInfo  DWORD      ?
WSADATA ENDS

sockproto STRUCT
  sp_family     WORD      ?
  sp_protocol   WORD      ?
sockproto ENDS

linger STRUCT
  l_onoff   WORD      ?
  l_linger  WORD      ?
linger ENDS

hostent STRUCT
  h_name      DWORD      ?
  h_alias     DWORD      ?
  h_addr      WORD       ?
  h_len       WORD       ?
  h_list      DWORD      ?
hostent ENDS

netent STRUCT
  n_name      DWORD      ?
  n_aliases   DWORD      ?
  n_addrtype  WORD       ?
  n_net       DWORD      ?
netent ENDS

servent STRUCT
  s_name     DWORD      ?
  s_aliases  DWORD      ?
  s_port     WORD       ?
  s_proto    DWORD      ?
servent ENDS

icmp_hdr STRUCT
  icmp_type   BYTE      ?
  icmp_code   BYTE      ?
  icmp_cksum  WORD      ?
  icmp_id     WORD      ?
  icmp_seq    WORD      ?
  icmp_data   BYTE      ?
icmp_hdr ENDS

ip_hdr STRUCT
  ip_hlv    BYTE      ?
  ip_tos    BYTE      ?
  ip_len    WORD      ?
  ip_id     WORD      ?
  ip_off    WORD      ?
  ip_ttl    BYTE      ?
  ip_p      BYTE      ?
  ip_cksum  WORD      ?
  ip_src    DWORD      ?
  ip_dest   DWORD      ?
ip_hdr ENDS

ICMP_OPTIONS STRUCT
  Ttl              BYTE      ?
  Tos              BYTE      ?
  Flags            BYTE      ?
  OptionsSize      BYTE      ?
  OptionsData      DWORD      ?
ICMP_OPTIONS ENDS

ICMP_ECHO_REPLY STRUCT
  Address          DWORD      ?
  Status           DWORD      ?
  RoundTripTime    DWORD      ?
  DataSize         WORD      ?
  Reserved         WORD      ?
  DataPointer      DWORD      ?
  Options          DWORD      ?
  zData            BYTE 250 dup (?)
ICMP_ECHO_REPLY ENDS
;------------------------------ ToolHelp 32 equates ---------------------
MAX_MODULE_NAME32  equ 255
TH32CS_SNAPHEAPLIST     equ 1
TH32CS_SNAPPROCESS      equ 2
TH32CS_SNAPTHREAD       equ 4
TH32CS_SNAPMODULE       equ 8
TH32CS_SNAPALL          equ TH32CS_SNAPHEAPLIST + TH32CS_SNAPPROCESS + TH32CS_SNAPTHREAD + TH32CS_SNAPMODULE
TH32CS_INHERIT          equ 80000000h
HF32_DEFAULT            equ 1 
HF32_SHARED             equ 2
LF32_FIXED              equ 1
LF32_FREE               equ 2
LF32_MOVEABLE           equ 4

HEAPLIST32 STRUCT
    dwSize              DWORD ?
    th32ProcessID       DWORD ?
    th32HeapID          DWORD ?
    dwFlags             DWORD ?
HEAPLIST32 ENDS

HEAPENTRY32 STRUCT
    dwSize          DWORD ?
    hHandle         DWORD ?
    dwAddress       DWORD ?
    dwBlockSize     DWORD ?
    dwFlags         DWORD ?
    dwLockCount     DWORD ?
    dwResvd         DWORD ?
    th32ProcessID   DWORD ?
    th32HeapID      DWORD ?
HEAPENTRY32 ENDS

PROCESSENTRY32 STRUCT
    dwSize              DWORD ?
    cntUsage            DWORD ?
    th32ProcessID       DWORD ?
    th32DefaultHeapID   DWORD ?
    th32ModuleID        DWORD ?
    cntThreads          DWORD ?
    th32ParentProcessID DWORD ?
    pcPriClassBase      DWORD ?
    dwFlags             DWORD ?
    szExeFile           db MAX_PATH dup(?)
PROCESSENTRY32 ENDS

THREADENTRY32 STRUCT
    dwSize              DWORD ?
    cntUsage            DWORD ?
    th32ThreadID        DWORD ?
    th32OwnerProcessID  DWORD ?
    tpBasePri           DWORD ?
    tpDeltaPri          DWORD ?
    dwFlags             DWORD ?
THREADENTRY32 ENDS

MODULEENTRY32 STRUCT
    dwSize          DWORD ?
    th32ModuleID    DWORD ?
    th32ProcessID   DWORD ?
    GlblcntUsage    DWORD ?
    ProccntUsage    DWORD ?
    modBaseAddr     DWORD ?
    modBaseSize     DWORD ?
    hModule         DWORD ?
    szModule        db MAX_MODULE_NAME32+1 dup(?)
    szExePath       db MAX_PATH dup(?)
MODULEENTRY32 ENDS

;--------- These are the equates & structures for property sheets ---------

MAXPROPPAGES            equ   100

LPFNPSPCALLBACK         equ   <LPFNPSPCALLBACKA>

PSP_DEFAULT             equ   00000000h
PSP_DLGINDIRECT         equ   00000001h
PSP_USEDWORD            equ   00000002h
PSP_USEICONID           equ   00000004h
PSP_USETITLE            equ   00000008h
PSP_RTLREADING          equ   00000010h

PSP_HASHELP             equ   00000020h
PSP_USEREFPARENT        equ   00000040h
PSP_USECALLBACK         equ   00000080h
PSP_PREMATURE           equ   00000400h

PSP_HIDEHEADER          equ   00000800h
PSP_USEHEADERTITLE      equ   00001000h
PSP_USEHEADERSUBTITLE   equ   00002000h

PSPCB_RELEASE           equ     1
PSPCB_CREATE            equ     2

PROPSHEETPAGEA STRUCT
        dwSize              DWORD ?
        dwFlags             DWORD ?
        hInstance           DWORD ?
          union 
            pszTemplate     DWORD ?
            pResource       DWORD ?
          ends
          union 
            hIcon           DWORD ?
            pszIcon         DWORD ?
          ends
        pszTitle            DWORD ?
        pfnDlgProc          DWORD ?        
        lParam              DWORD ?         
        pfnCallback         DWORD ?
        pcRefParent         DWORD ?
        pszHeaderTitle      DWORD ?
        pszHeaderSubTitle   DWORD ?
PROPSHEETPAGEA ENDS

PROPSHEETPAGE         equ   <PROPSHEETPAGEA>
LPPROPSHEETPAGE       equ   <LPPROPSHEETPAGEA>
LPCPROPSHEETPAGE      equ   <LPCPROPSHEETPAGEA>
PROPSHEETPAGE_V1_SIZE equ   <PROPSHEETPAGEA_V1_SIZE>

PSH_DEFAULT           equ   00000000h
PSH_PROPTITLE         equ   00000001h
PSH_USEDWORD          equ   00000002h
PSH_USEICONID         equ   00000004h
PSH_PROPSHEETPAGE     equ   00000008h
PSH_WIZARDHASFINISH   equ   00000010h
PSH_WIZARD            equ   00000020h
PSH_USEPSTARTPAGE     equ   00000040h
PSH_NOAPPLYNOW        equ   00000080h
PSH_USECALLBACK       equ   00000100h
PSH_HASHELP           equ   00000200h
PSH_MODELESS          equ   00000400h
PSH_RTLREADING        equ   00000800h
PSH_WIZARDCONTEXTHELP equ   00001000h
ifndef WIN32_IE5
	PSH_WIZARD97          equ   00002000h
else
	PSH_WIZARD97           equ 01000000h
endif
PSH_WATERMARK         equ   00008000h
PSH_USEHBMWATERMARK   equ   00010000h
PSH_USEHPLWATERMARK   equ   00020000h
PSH_STRETCHWATERMARK  equ   00040000h
PSH_HEADER            equ   00080000h
PSH_USEHBMHEADER      equ   00100000h
PSH_USEPAGELANG       equ   00200000h

PROPSHEETHEADERA STRUCT
        dwSize              DWORD ?
        dwFlags             DWORD ?          
        hwndParent          DWORD ?           
        hInstance           DWORD ?      
        union 
            hIcon           DWORD ?
            pszIcon         DWORD ?
        ends
        pszCaption          DWORD ? 
        nPages              DWORD ? 
          union 
            nStartPage      DWORD ?
            pStartPage      DWORD ?    
          ends
          union 
            ppsp            DWORD ?
            phpage          DWORD ?
          ends
        pfnCallback         DWORD ?
          union 
            hbmWatermark    DWORD ?
            pszbmWatermark  DWORD ?
          ends
        hplWatermark        DWORD ?
        union 
            hbmHeader       DWORD ?
            pszbmHeader     DWORD ?
        ends

PROPSHEETHEADERA ENDS

PROPSHEETHEADER         equ   <PROPSHEETHEADERA>
LPPROPSHEETHEADER       equ   <LPPROPSHEETHEADERA>

LPCPROPSHEETHEADER      equ   <LPCPROPSHEETHEADERA>
PROPSHEETHEADER_V1_SIZE equ   <PROPSHEETHEADERA_V1_SIZE>

PSCB_INITIALIZED        equ   1
PSCB_PRECREATE          equ   2

CreatePropertySheetPage equ   <CreatePropertySheetPageA>
PropertySheet           equ   <PropertySheetA>

PSHNOTIFY STRUCT
    hdr         NMHDR <>
    lParam      DWORD ?
PSHNOTIFY ENDS

PSN_FIRST               equ   0-200
PSN_LAST                equ   0-299

PSN_SETACTIVE           equ   PSN_FIRST-0
PSN_KILLACTIVE          equ   PSN_FIRST-1
PSN_APPLY               equ   PSN_FIRST-2
PSN_RESET               equ   PSN_FIRST-3
PSN_HELP                equ   PSN_FIRST-5
PSN_WIZBACK             equ   PSN_FIRST-6
PSN_WIZNEXT             equ   PSN_FIRST-7
PSN_WIZFINISH           equ   PSN_FIRST-8
PSN_QUERYCANCEL         equ   PSN_FIRST-9
PSN_GETOBJECT           equ   PSN_FIRST-10

PSNRET_NOERROR              equ   0
PSNRET_INVALID              equ   1
PSNRET_INVALID_NOCHANGEPAGE equ   2


PSM_SETCURSEL           equ   WM_USER + 101
PSM_REMOVEPAGE          equ   WM_USER + 102
PSM_ADDPAGE             equ   WM_USER + 103
PSM_CHANGED             equ   WM_USER + 104
PSM_RESTARTWINDOWS      equ   WM_USER + 105
PSM_REBOOTSYSTEM        equ   WM_USER + 106
PSM_CANCELTOCLOSE       equ   WM_USER + 107
PSM_QUERYSIBLINGS       equ   WM_USER + 108
PSM_UNCHANGED           equ   WM_USER + 109
PSM_APPLY               equ   WM_USER + 110
PSM_SETTITLEA           equ   WM_USER + 111

PSM_SETTITLE            equ   <PSM_SETTITLEA>
PSM_SETWIZBUTTONS       equ   WM_USER + 112

PSWIZB_BACK             equ   00000001h
PSWIZB_NEXT             equ   00000002h
PSWIZB_FINISH           equ   00000004h
PSWIZB_DISABLEDFINISH   equ   00000008h

PSM_PRESSBUTTON         equ   WM_USER + 113

PSBTN_BACK              equ   0
PSBTN_NEXT              equ   1
PSBTN_FINISH            equ   2
PSBTN_OK                equ   3
PSBTN_APPLYNOW          equ   4
PSBTN_CANCEL            equ   5
PSBTN_HELP              equ   6
PSBTN_MAX               equ   6

PSM_SETCURSELID         equ   WM_USER + 114

PSM_SETFINISHTEXTA      equ   WM_USER + 115

PSM_SETFINISHTEXT       equ   <PSM_SETFINISHTEXTA>

PSM_GETTABCONTROL       equ   WM_USER + 116
PSM_ISDIALOGMESSAGE     equ   WM_USER + 117
PSM_GETCURRENTPAGEDWORD equ   WM_USER + 118

ID_PSRESTARTWINDOWS     equ   2
ID_PSREBOOTSYSTEM       equ   <ID_PSRESTARTWINDOWS or 1>

WIZ_CXDLG               equ   276
WIZ_CYDLG               equ   140

WIZ_CXBMP               equ   80

WIZ_BODYX               equ   92
WIZ_BODYCX              equ   184

PROP_SM_CXDLG           equ   212
PROP_SM_CYDLG           equ   188

PROP_MED_CXDLG          equ   227
PROP_MED_CYDLG          equ   215

PROP_LG_CXDLG           equ   252
PROP_LG_CYDLG           equ   218

; --------------------- equates & structures for WINNT ---------------------

ANYSIZE_ARRAY equ 1       
MAX_NATURAL_ALIGNMENT           equ sizeof DWORD
APPLICATION_ERROR_MASK          equ 20000000h
ERROR_SEVERITY_SUCCESS          equ 00000000h
ERROR_SEVERITY_INFORMATIONAL    equ 40000000h
ERROR_SEVERITY_WARNING          equ 80000000h
ERROR_SEVERITY_ERROR            equ 0C0000000h
MINCHAR     equ 80h        
MAXCHAR     equ 7fh        
MINSHORT    equ 8000h      
MAXSHORT    equ 7fffh      
MINLONG     equ 80000000h  
MAXLONG     equ 7fffffffh  
MAXBYTE     equ 0ffh        
MAXWORD     equ 0ffffh      
MAXDWORD    equ 0ffffffffh  
LANG_NEUTRAL equ 00h
LANG_AFRIKAANS equ 36h
LANG_ALBANIAN equ 1ch
LANG_ARABIC equ 01h
LANG_ARMENIAN equ 2bh
LANG_ASSAMESE equ 4dh
LANG_AZERI equ 2ch
LANG_BASQUE equ 2dh
LANG_BELARUSIAN equ 23h
LANG_BENGALI equ 45h
LANG_BULGARIAN equ 02h
LANG_CATALAN equ 03h
LANG_CHINESE equ 04h
LANG_CROATIAN equ 1ah
LANG_CZECH equ 05h
LANG_DANISH equ 06h
LANG_DUTCH equ 13h
LANG_ENGLISH equ 09h
LANG_ESTONIAN equ 25h
LANG_FAEROESE equ 38h
LANG_FARSI equ 29h
LANG_FINNISH equ 0bh
LANG_FRENCH equ 0ch
LANG_GEORGIAN equ 37h
LANG_GERMAN equ 07h
LANG_GREEK equ 08h
LANG_GUJARATI equ 47h
LANG_HEBREW equ 0dh
LANG_HINDI equ 39h
LANG_HUNGARIAN equ 0eh
LANG_ICELANDIC equ 0fh
LANG_INDONESIAN equ 21h
LANG_ITALIAN equ 10h
LANG_JAPANESE equ 11h
LANG_KANNADA equ 4bh
LANG_KASHMIRI equ 60h
LANG_KAZAK equ 3fh
LANG_KONKANI equ 57h
LANG_KOREAN equ 12h
LANG_LATVIAN equ 26h
LANG_LITHUANIAN equ 27h
LANG_MACEDONIAN equ 2fh
LANG_MALAY equ 3eh
LANG_MALAYALAM equ 4ch
LANG_MANIPURI equ 58h
LANG_MARATHI equ 4eh
LANG_NEPALI equ 61h
LANG_NORWEGIAN equ 14h
LANG_ORIYA equ 48h
LANG_POLISH equ 15h
LANG_PORTUGUESE equ 16h
LANG_PUNJABI equ 46h
LANG_ROMANIAN equ 18h
LANG_RUSSIAN equ 19h
LANG_SANSKRIT equ 4fh
LANG_SERBIAN equ 1ah
LANG_SINDHI equ 59h
LANG_SLOVAK equ 1bh
LANG_SLOVENIAN equ 24h
LANG_SPANISH equ 0ah
LANG_SWAHILI equ 41h
LANG_SWEDISH equ 1dh
LANG_TAMIL equ 49h
LANG_TATAR equ 44h
LANG_TELUGU equ 4ah
LANG_THAI equ 1eh
LANG_TURKISH equ 1fh
LANG_UKRAINIAN equ 22h
LANG_URDU equ 20h
LANG_UZBEK equ 43h
LANG_VIETNAMESE equ 2ah
SUBLANG_NEUTRAL equ 00h    
SUBLANG_DEFAULT equ 01h    
SUBLANG_SYS_DEFAULT equ 02h    
SUBLANG_ARABIC_SAUDI_ARABIA equ 01h    
SUBLANG_ARABIC_IRAQ equ 02h    
SUBLANG_ARABIC_EGYPT equ 03h    
SUBLANG_ARABIC_LIBYA equ 04h    
SUBLANG_ARABIC_ALGERIA equ 05h    
SUBLANG_ARABIC_MOROCCO equ 06h    
SUBLANG_ARABIC_TUNISIA equ 07h    
SUBLANG_ARABIC_OMAN equ 08h    
SUBLANG_ARABIC_YEMEN equ 09h    
SUBLANG_ARABIC_SYRIA equ 0ah    
SUBLANG_ARABIC_JORDAN equ 0bh    
SUBLANG_ARABIC_LEBANON equ 0ch    
SUBLANG_ARABIC_KUWAIT equ 0dh    
SUBLANG_ARABIC_UAE equ 0eh    
SUBLANG_ARABIC_BAHRAIN equ 0fh    
SUBLANG_ARABIC_QATAR equ 10h    
SUBLANG_AZERI_LATIN equ 01h    
SUBLANG_AZERI_CYRILLIC equ 02h    
SUBLANG_CHINESE_TRADITIONAL equ 01h    
SUBLANG_CHINESE_SIMPLIFIED equ 02h    
SUBLANG_CHINESE_HONGKONG equ 03h    
SUBLANG_CHINESE_SINGAPORE equ 04h    
SUBLANG_CHINESE_MACAU equ 05h    
SUBLANG_DUTCH equ 01h    
SUBLANG_DUTCH_BELGIAN equ 02h    
SUBLANG_ENGLISH_US equ 01h    
SUBLANG_ENGLISH_UK equ 02h    
SUBLANG_ENGLISH_AUS equ 03h    
SUBLANG_ENGLISH_CAN equ 04h    
SUBLANG_ENGLISH_NZ equ 05h    
SUBLANG_ENGLISH_EIRE equ 06h    
SUBLANG_ENGLISH_SOUTH_AFRICA equ 07h    
SUBLANG_ENGLISH_JAMAICA equ 08h    
SUBLANG_ENGLISH_CARIBBEAN equ 09h    
SUBLANG_ENGLISH_BELIZE equ 0ah    
SUBLANG_ENGLISH_TRINIDAD equ 0bh    
SUBLANG_ENGLISH_ZIMBABWE equ 0ch    
SUBLANG_ENGLISH_PHILIPPINES equ 0dh    
SUBLANG_FRENCH equ 01h    
SUBLANG_FRENCH_BELGIAN equ 02h    
SUBLANG_FRENCH_CANADIAN equ 03h    
SUBLANG_FRENCH_SWISS equ 04h    
SUBLANG_FRENCH_LUXEMBOURG equ 05h    
SUBLANG_FRENCH_MONACO equ 06h    
SUBLANG_GERMAN equ 01h    
SUBLANG_GERMAN_SWISS equ 02h    
SUBLANG_GERMAN_AUSTRIAN equ 03h    
SUBLANG_GERMAN_LUXEMBOURG equ 04h    
SUBLANG_GERMAN_LIECHTENSTEIN equ 05h    
SUBLANG_ITALIAN equ 01h    
SUBLANG_ITALIAN_SWISS equ 02h    
SUBLANG_KASHMIRI_INDIA equ 02h    
SUBLANG_KOREAN equ 01h    
SUBLANG_LITHUANIAN equ 01h    
SUBLANG_LITHUANIAN_CLASSIC equ 02h    
SUBLANG_MALAY_MALAYSIA equ 01h    
SUBLANG_MALAY_BRUNEI_DARUSSALAM equ 02h    
SUBLANG_NEPALI_INDIA equ 02h    
SUBLANG_NORWEGIAN_BOKMAL equ 01h    
SUBLANG_NORWEGIAN_NYNORSK equ 02h    
SUBLANG_PORTUGUESE equ 02h    
SUBLANG_PORTUGUESE_BRAZILIAN equ 01h    
SUBLANG_SERBIAN_LATIN equ 02h    
SUBLANG_SERBIAN_CYRILLIC equ 03h    
SUBLANG_SPANISH equ 01h    
SUBLANG_SPANISH_MEXICAN equ 02h    
SUBLANG_SPANISH_MODERN equ 03h    
SUBLANG_SPANISH_GUATEMALA equ 04h    
SUBLANG_SPANISH_COSTA_RICA equ 05h    
SUBLANG_SPANISH_PANAMA equ 06h    
SUBLANG_SPANISH_DOMINICAN_REPUBLIC equ 07h  
SUBLANG_SPANISH_VENEZUELA equ 08h    
SUBLANG_SPANISH_COLOMBIA equ 09h    
SUBLANG_SPANISH_PERU equ 0ah    
SUBLANG_SPANISH_ARGENTINA equ 0bh    
SUBLANG_SPANISH_ECUADOR equ 0ch    
SUBLANG_SPANISH_CHILE equ 0dh    
SUBLANG_SPANISH_URUGUAY equ 0eh    
SUBLANG_SPANISH_PARAGUAY equ 0fh    
SUBLANG_SPANISH_BOLIVIA equ 10h    
SUBLANG_SPANISH_EL_SALVADOR equ 11h    
SUBLANG_SPANISH_HONDURAS equ 12h    
SUBLANG_SPANISH_NICARAGUA equ 13h    
SUBLANG_SPANISH_PUERTO_RICO equ 14h    
SUBLANG_SWEDISH equ 01h    
SUBLANG_SWEDISH_FINLAND equ 02h    
SUBLANG_URDU_PAKISTAN equ 01h    
SUBLANG_URDU_INDIA equ 02h    
SUBLANG_UZBEK_LATIN equ 01h    
SUBLANG_UZBEK_CYRILLIC equ 02h    
SORT_DEFAULT equ 0h     
SORT_JAPANESE_XJIS equ 0h     
SORT_JAPANESE_UNICODE equ 1h     
SORT_CHINESE_BIG5 equ 0h     
SORT_CHINESE_PRCP equ 0h     
SORT_CHINESE_UNICODE equ 1h     
SORT_CHINESE_PRC equ 2h     
SORT_CHINESE_BOPOMOFO equ 3h     
SORT_KOREAN_KSC equ 0h     
SORT_KOREAN_UNICODE equ 1h     
SORT_GERMAN_PHONE_BOOK equ 1h     
SORT_HUNGARIAN_DEFAULT equ 0h     
SORT_HUNGARIAN_TECHNICAL equ 1h     
SORT_GEORGIAN_TRADITIONAL equ 0h     
SORT_GEORGIAN_MODERN equ 1h     
NLS_VALID_LOCALE_MASK equ 000fffffh
STATUS_WAIT_0 equ 00000000h
STATUS_ABANDONED_WAIT_0 equ 00000080h
STATUS_USER_APC equ 000000C0h
STATUS_TIMEOUT equ 00000102h 
STATUS_PENDING equ 00000103h
STATUS_SEGMENT_NOTIFICATION equ 40000005h
STATUS_GUARD_PAGE_VIOLATION equ 80000001h
STATUS_DATATYPE_MISALIGNMENT equ 80000002h
STATUS_BREAKPOINT equ 80000003h    
STATUS_SINGLE_STEP equ 80000004h    
STATUS_ACCESS_VIOLATION equ 0C0000005h
STATUS_IN_PAGE_ERROR equ 0C0000006h    
STATUS_INVALID_HANDLE equ 0C0000008h
STATUS_NO_MEMORY equ 0C0000017h
STATUS_ILLEGAL_INSTRUCTION equ 0C000001Dh
STATUS_NONCONTINUABLE_EXCEPTION equ 0C0000025h
STATUS_INVALID_DISPOSITION equ 0C0000026h
STATUS_ARRAY_BOUNDS_EXCEEDED equ 0C000008Ch
STATUS_FLOAT_DENORMAL_OPERAND equ 0C000008Dh
STATUS_FLOAT_DIVIDE_BY_ZERO equ 0C000008Eh
STATUS_FLOAT_INEXACT_RESULT equ 0C000008Fh
STATUS_FLOAT_INVALID_OPERATION equ 0C0000090h
STATUS_FLOAT_OVERFLOW equ 0C0000091h
STATUS_FLOAT_STACK_CHECK equ 0C0000092h
STATUS_FLOAT_UNDERFLOW equ 0C0000093h
STATUS_INTEGER_DIVIDE_BY_ZERO equ 0C0000094h
STATUS_INTEGER_OVERFLOW equ 0C0000095h
STATUS_PRIVILEGED_INSTRUCTION equ 0C0000096h
STATUS_STACK_OVERFLOW equ 0C00000FDh
STATUS_CONTROL_C_EXIT equ 0C000013Ah
STATUS_FLOAT_MULTIPLE_FAULTS equ 0C00002B4h
STATUS_FLOAT_MULTIPLE_TRAPS equ 0C00002B5h
STATUS_ILLEGAL_VLM_REFERENCE equ 0C00002C0h     
MAXIMUM_WAIT_OBJECTS equ 64     
MAXIMUM_SUSPEND_COUNT equ MAXCHAR 
SIZE_OF_80387_REGISTERS equ 80
CONTEXT_i386 equ 00010000h    
CONTEXT_i486 equ 00010000h    
CONTEXT_CONTROL equ (CONTEXT_i386 + 00000001h) 
CONTEXT_INTEGER equ (CONTEXT_i386 + 00000002h) 
CONTEXT_SEGMENTS equ (CONTEXT_i386 + 00000004h) 
CONTEXT_FLOATING_POINT equ (CONTEXT_i386 + 00000008h) 
CONTEXT_DEBUG_REGISTERS equ (CONTEXT_i386 + 00000010h) 
CONTEXT_EXTENDED_REGISTERS equ (CONTEXT_i386 + 00000020h) 
MAXIMUM_SUPPORTED_EXTENSION equ 512
EXCEPTION_NONCONTINUABLE equ 1h    
EXCEPTION_MAXIMUM_PARAMETERS equ 15 
DELETE equ (00010000h)
READ_CONTROL equ (00020000h)
WRITE_DAC equ (00040000h)
WRITE_OWNER equ (00080000h)
SYNCHRONIZE equ (00100000h)
STANDARD_RIGHTS_REQUIRED equ (000F0000h)
STANDARD_RIGHTS_READ equ (READ_CONTROL)
STANDARD_RIGHTS_WRITE equ (READ_CONTROL)
STANDARD_RIGHTS_EXECUTE equ (READ_CONTROL)
STANDARD_RIGHTS_ALL equ (001F0000h)
SPECIFIC_RIGHTS_ALL equ (0000FFFFh)
ACCESS_SYSTEM_SECURITY equ (01000000h)
MAXIMUM_ALLOWED equ (02000000h)
GENERIC_READ equ (80000000h)
GENERIC_WRITE equ (40000000h)
GENERIC_EXECUTE equ (20000000h)
GENERIC_ALL equ (10000000h)
SID_REVISION equ (1)    
SID_MAX_SUB_AUTHORITIES equ (15)
SID_RECOMMENDED_SUB_AUTHORITIES equ (1)    
SECURITY_NULL_SID_AUTHORITY equ {0,0,0,0,0,0}
SECURITY_WORLD_SID_AUTHORITY equ {0,0,0,0,0,1}
SECURITY_LOCAL_SID_AUTHORITY equ {0,0,0,0,0,2}
SECURITY_CREATOR_SID_AUTHORITY equ {0,0,0,0,0,3}
SECURITY_NON_UNIQUE_AUTHORITY equ {0,0,0,0,0,4}
SECURITY_NULL_RID equ (00000000h)
SECURITY_WORLD_RID equ (00000000h)
SECURITY_LOCAL_RID equ (00000000h)
SECURITY_CREATOR_OWNER_RID equ (00000000h)
SECURITY_CREATOR_GROUP_RID equ (00000001h)
SECURITY_CREATOR_OWNER_SERVER_RID equ (00000002h)
SECURITY_CREATOR_GROUP_SERVER_RID equ (00000003h)
SECURITY_NT_AUTHORITY equ {0,0,0,0,0,5}   
SECURITY_DIALUP_RID equ (00000001h)
SECURITY_NETWORK_RID equ (00000002h)
SECURITY_BATCH_RID equ (00000003h)
SECURITY_INTERACTIVE_RID equ (00000004h)
SECURITY_SERVICE_RID equ (00000006h)
SECURITY_ANONYMOUS_LOGON_RID equ (00000007h)
SECURITY_PROXY_RID equ (00000008h)
SECURITY_ENTERPRISE_CONTROLLERS_RID equ (00000009h)
SECURITY_SERVER_LOGON_RID equ SECURITY_ENTERPRISE_CONTROLLERS_RID
SECURITY_PRINCIPAL_SELF_RID equ (0000000Ah)
SECURITY_AUTHENTICATED_USER_RID equ (0000000Bh)

SECURITY_RESTRICTED_CODE_RID equ (0000000Ch)
SECURITY_LOGON_IDS_RID equ (00000005h)
SECURITY_LOGON_IDS_RID_COUNT equ (3L)
SECURITY_LOCAL_SYSTEM_RID equ (00000012h)
SECURITY_NT_NON_UNIQUE equ (00000015h)
SECURITY_BUILTIN_DOMAIN_RID equ (00000020h)
DOMAIN_USER_RID_ADMIN equ (000001F4h)
DOMAIN_USER_RID_GUEST equ (000001F5h)
DOMAIN_USER_RID_KRBTGT equ (000001F6h)
DOMAIN_GROUP_RID_ADMINS equ (00000200h)
DOMAIN_GROUP_RID_USERS equ (00000201h)
DOMAIN_GROUP_RID_GUESTS equ (00000202h)
DOMAIN_GROUP_RID_COMPUTERS equ (00000203h)
DOMAIN_GROUP_RID_CONTROLLERS equ (00000204h)
DOMAIN_GROUP_RID_CERT_ADMINS equ (00000205h)
DOMAIN_GROUP_RID_SCHEMA_ADMINS equ (00000206h)
DOMAIN_ALIAS_RID_ADMINS equ (00000220h)
DOMAIN_ALIAS_RID_USERS equ (00000221h)
DOMAIN_ALIAS_RID_GUESTS equ (00000222h)
DOMAIN_ALIAS_RID_POWER_USERS equ (00000223h)
DOMAIN_ALIAS_RID_ACCOUNT_OPS equ (00000224h)
DOMAIN_ALIAS_RID_SYSTEM_OPS equ (00000225h)
DOMAIN_ALIAS_RID_PRINT_OPS equ (00000226h)
DOMAIN_ALIAS_RID_BACKUP_OPS equ (00000227h)
DOMAIN_ALIAS_RID_REPLICATOR equ (00000228h)
SYSTEM_LUID equ { 3E7h, 0h }
ANONYMOUS_LOGON_LUID equ { 3e6h, 0h }
SE_GROUP_MANDATORY equ (00000001h)
SE_GROUP_ENABLED_BY_DEFAULT equ (00000002h)
SE_GROUP_ENABLED equ (00000004h)
SE_GROUP_OWNER equ (00000008h)
SE_GROUP_USE_FOR_DENY_ONLY equ (00000010h)
SE_GROUP_LOGON_ID equ (0C0000000h)
ACL_REVISION equ (2)
ACL_REVISION_DS equ (4)
ACL_REVISION1 equ (1)
MIN_ACL_REVISION equ ACL_REVISION2
ACL_REVISION2 equ (2)
ACL_REVISION3 equ (3)
ACL_REVISION4 equ (4)
MAX_ACL_REVISION equ ACL_REVISION4
ACCESS_MIN_MS_ACE_TYPE equ (0h)
ACCESS_ALLOWED_ACE_TYPE equ (0h)
ACCESS_DENIED_ACE_TYPE equ (1h)
SYSTEM_AUDIT_ACE_TYPE equ (2h)
SYSTEM_ALARM_ACE_TYPE equ (3h)
ACCESS_MAX_MS_V2_ACE_TYPE equ (3h)
ACCESS_ALLOWED_COMPOUND_ACE_TYPE equ (4h)
ACCESS_MAX_MS_V3_ACE_TYPE equ (4h)
ACCESS_MIN_MS_OBJECT_ACE_TYPE equ (5h)
ACCESS_ALLOWED_OBJECT_ACE_TYPE equ (5h)
ACCESS_DENIED_OBJECT_ACE_TYPE equ (6h)
SYSTEM_AUDIT_OBJECT_ACE_TYPE equ (7h)
SYSTEM_ALARM_OBJECT_ACE_TYPE equ (8h)
ACCESS_MAX_MS_OBJECT_ACE_TYPE equ (8h)
ACCESS_MAX_MS_V4_ACE_TYPE equ (8h)
ACCESS_MAX_MS_ACE_TYPE equ (8h)
OBJECT_INHERIT_ACE equ (1h)
CONTAINER_INHERIT_ACE equ (2h)
NO_PROPAGATE_INHERIT_ACE equ (4h)
INHERIT_ONLY_ACE equ (8h)
INHERITED_ACE equ (10h)
SUCCESSFUL_ACCESS_ACE_FLAG equ (40h)
FAILED_ACCESS_ACE_FLAG equ (80h)
ACE_OBJECT_TYPE_PRESENT equ 1h
ACE_INHERITED_OBJECT_TYPE_PRESENT equ 2h
SECURITY_DESCRIPTOR_REVISION equ (1)
SECURITY_DESCRIPTOR_REVISION1 equ (1)
SECURITY_DESCRIPTOR_MIN_LENGTH equ sizeof SECURITY_DESCRIPTOR
SE_OWNER_DEFAULTED equ (0001h)
SE_GROUP_DEFAULTED equ (0002h)
SE_DACL_PRESENT equ (0004h)
SE_DACL_DEFAULTED equ (0008h)
SE_SACL_PRESENT equ (0010h)
SE_SACL_DEFAULTED equ (0020h)
SE_DACL_AUTO_INHERIT_REQ equ (0100h)
SE_SACL_AUTO_INHERIT_REQ equ (0200h)
SE_DACL_AUTO_INHERITED equ (0400h)
SE_SACL_AUTO_INHERITED equ (0800h)
SE_DACL_PROTECTED equ (1000h)
SE_SACL_PROTECTED equ (2000h)
SE_SELF_RELATIVE equ (8000h)
ACCESS_OBJECT_GUID equ 0
ACCESS_PROPERTY_SET_GUID equ 1
ACCESS_PROPERTY_GUID equ 2
ACCESS_MAX_LEVEL equ 4
AUDIT_ALLOW_NO_PRIVILEGE equ 1h
ACCESS_DS_SOURCE_A equ "DS"
ACCESS_DS_OBJECT_TYPE_NAME_A equ "Directory Service Object"
SE_PRIVILEGE_ENABLED_BY_DEFAULT equ (00000001h)
SE_PRIVILEGE_ENABLED equ (00000002h)
SE_PRIVILEGE_USED_FOR_ACCESS equ (80000000h)
PRIVILEGE_SET_ALL_NECESSARY equ (1)
SE_CREATE_TOKEN_NAME equ "SeCreateTokenPrivilege"
SE_ASSIGNPRIMARYTOKEN_NAME equ "SeAssignPrimaryTokenPrivilege"
SE_LOCK_MEMORY_NAME equ "SeLockMemoryPrivilege"
SE_INCREASE_QUOTA_NAME equ ("SeIncreaseQuotaPrivilege")
SE_UNSOLICITED_INPUT_NAME equ ("SeUnsolicitedInputPrivilege")
SE_MACHINE_ACCOUNT_NAME equ ("SeMachineAccountPrivilege")
SE_TCB_NAME equ ("SeTcbPrivilege")
SE_SECURITY_NAME equ ("SeSecurityPrivilege")
SE_TAKE_OWNERSHIP_NAME equ ("SeTakeOwnershipPrivilege")
SE_LOAD_DRIVER_NAME equ ("SeLoadDriverPrivilege")
SE_SYSTEM_PROFILE_NAME equ ("SeSystemProfilePrivilege")
SE_SYSTEMTIME_NAME equ ("SeSystemtimePrivilege")
SE_PROF_SINGLE_PROCESS_NAME equ ("SeProfileSingleProcessPrivilege")
SE_INC_BASE_PRIORITY_NAME equ ("SeIncreaseBasePriorityPrivilege")
SE_CREATE_PAGEFILE_NAME equ ("SeCreatePagefilePrivilege")
SE_CREATE_PERMANENT_NAME equ ("SeCreatePermanentPrivilege")
SE_BACKUP_NAME equ ("SeBackupPrivilege")
SE_RESTORE_NAME equ ("SeRestorePrivilege")
SE_SHUTDOWN_NAME equ ("SeShutdownPrivilege")
SE_DEBUG_NAME equ ("SeDebugPrivilege")
SE_AUDIT_NAME equ ("SeAuditPrivilege")
SE_SYSTEM_ENVIRONMENT_NAME equ ("SeSystemEnvironmentPrivilege")
SE_CHANGE_NOTIFY_NAME equ ("SeChangeNotifyPrivilege")
SE_REMOTE_SHUTDOWN_NAME equ ("SeRemoteShutdownPrivilege")
TOKEN_ASSIGN_PRIMARY equ (0001h)
TOKEN_DUPLICATE equ (0002h)
TOKEN_IMPERSONATE equ (0004h)
TOKEN_QUERY equ (0008h)
TOKEN_QUERY_SOURCE equ (0010h)
TOKEN_ADJUST_PRIVILEGES equ (0020h)
TOKEN_ADJUST_GROUPS equ (0040h)
TOKEN_ADJUST_DEFAULT equ (0080h)
TOKEN_ADJUST_SESSIONID equ (0100h)
TOKEN_ALL_ACCESS equ (STANDARD_RIGHTS_REQUIRED+TOKEN_ASSIGN_PRIMARY+TOKEN_DUPLICATE+ \
                          TOKEN_IMPERSONATE+TOKEN_QUERY+TOKEN_QUERY_SOURCE+ \
                          TOKEN_ADJUST_PRIVILEGES+TOKEN_ADJUST_GROUPS+TOKEN_ADJUST_SESSIONID+ \
                          TOKEN_ADJUST_DEFAULT)
TOKEN_READ equ (STANDARD_RIGHTS_READ+TOKEN_QUERY)
TOKEN_WRITE equ (STANDARD_RIGHTS_WRITE+TOKEN_ADJUST_PRIVILEGES+TOKEN_ADJUST_GROUPS+TOKEN_ADJUST_DEFAULT)
TOKEN_EXECUTE equ (STANDARD_RIGHTS_EXECUTE)
TOKEN_SOURCE_LENGTH equ 8
SECURITY_DYNAMIC_TRACKING equ TRUE
SECURITY_STATIC_TRACKING equ FALSE
OWNER_SECURITY_INFORMATION equ (00000001h)
GROUP_SECURITY_INFORMATION equ (00000002h)
DACL_SECURITY_INFORMATION equ (00000004h)
SACL_SECURITY_INFORMATION equ (00000008h)
PROCESS_TERMINATE equ (0001h)  
PROCESS_CREATE_THREAD equ (0002h)  
PROCESS_SET_SESSIONID equ (0004h)  
PROCESS_VM_OPERATION equ (0008h)  
PROCESS_VM_READ equ (0010h)  
PROCESS_VM_WRITE equ (0020h)  
PROCESS_DUP_HANDLE equ (0040h)  
PROCESS_CREATE_PROCESS equ (0080h)  
PROCESS_SET_QUOTA equ (0100h)  
PROCESS_SET_INFORMATION equ (0200h)  
PROCESS_QUERY_INFORMATION equ (0400h)  
PROCESS_ALL_ACCESS equ (STANDARD_RIGHTS_REQUIRED + SYNCHRONIZE + 0FFFh)
MAXIMUM_PROCESSORS equ 32
THREAD_TERMINATE equ (0001h)  
THREAD_SUSPEND_RESUME equ (0002h)  
THREAD_GET_CONTEXT equ (0008h)  
THREAD_SET_CONTEXT equ (0010h)  
THREAD_SET_INFORMATION equ (0020h)  
THREAD_QUERY_INFORMATION equ (0040h)  
THREAD_SET_THREAD_TOKEN equ (0080h)
THREAD_IMPERSONATE equ (0100h)
THREAD_DIRECT_IMPERSONATION equ (0200h)
THREAD_ALL_ACCESS equ (STANDARD_RIGHTS_REQUIRED + SYNCHRONIZE +  3FFh) 
                                  
JOB_OBJECT_ASSIGN_PROCESS equ (0001h)
JOB_OBJECT_SET_ATTRIBUTES equ (0002h)
JOB_OBJECT_QUERY equ (0004h)
JOB_OBJECT_TERMINATE equ (0008h)
JOB_OBJECT_SET_SECURITY_ATTRIBUTES equ (0010h)
JOB_OBJECT_ALL_ACCESS equ (STANDARD_RIGHTS_REQUIRED + SYNCHRONIZE +   1Fh )
                                      
TLS_MINIMUM_AVAILABLE equ 64    
THREAD_BASE_PRIORITY_LOWRT equ 15  
THREAD_BASE_PRIORITY_MAX equ 2   
THREAD_BASE_PRIORITY_MIN equ -2  
THREAD_BASE_PRIORITY_IDLE equ -15 
EVENT_MODIFY_STATE equ 0002h  
EVENT_ALL_ACCESS equ (STANDARD_RIGHTS_REQUIRED+SYNCHRONIZE+3h) 
MUTANT_QUERY_STATE equ 0001h
MUTANT_ALL_ACCESS equ (STANDARD_RIGHTS_REQUIRED+SYNCHRONIZE+ MUTANT_QUERY_STATE)
SEMAPHORE_MODIFY_STATE equ 0002h  
SEMAPHORE_ALL_ACCESS equ (STANDARD_RIGHTS_REQUIRED+SYNCHRONIZE+3h) 
TIMER_QUERY_STATE equ 0001h
TIMER_MODIFY_STATE equ 0002h
TIMER_ALL_ACCESS equ (STANDARD_RIGHTS_REQUIRED+SYNCHRONIZE+TIMER_QUERY_STATE+TIMER_MODIFY_STATE)
TIME_ZONE_ID_UNKNOWN equ 0
TIME_ZONE_ID_STANDARD equ 1
TIME_ZONE_ID_DAYLIGHT equ 2
PROCESSOR_INTEL_386 equ 386
PROCESSOR_INTEL_486 equ 486
PROCESSOR_INTEL_PENTIUM equ 586
PROCESSOR_MIPS_R4000 equ 4000    
PROCESSOR_ALPHA_21064 equ 21064
PROCESSOR_PPC_601 equ 601
PROCESSOR_PPC_603 equ 603
PROCESSOR_PPC_604 equ 604
PROCESSOR_PPC_620 equ 620
PROCESSOR_HITACHI_SH3 equ 10003   
PROCESSOR_HITACHI_SH3E equ 10004   
PROCESSOR_HITACHI_SH4 equ 10005   
PROCESSOR_MOTOROLA_821 equ 821     
PROCESSOR_SHx_SH3 equ 103     
PROCESSOR_SHx_SH4 equ 104     
PROCESSOR_STRONGARM equ 2577    
PROCESSOR_ARM720 equ 1824    
PROCESSOR_ARM820 equ 2080    
PROCESSOR_ARM920 equ 2336    
PROCESSOR_ARM_7TDMI equ 70001   
PROCESSOR_ARCHITECTURE_INTEL equ 0
PROCESSOR_ARCHITECTURE_MIPS equ 1
PROCESSOR_ARCHITECTURE_ALPHA equ 2
PROCESSOR_ARCHITECTURE_PPC equ 3
PROCESSOR_ARCHITECTURE_SHX equ 4
PROCESSOR_ARCHITECTURE_ARM equ 5
PROCESSOR_ARCHITECTURE_IA64 equ 6
PROCESSOR_ARCHITECTURE_ALPHA64 equ 7
PROCESSOR_ARCHITECTURE_UNKNOWN equ 0FFFFh
PF_FLOATING_POINT_PRECISION_ERRATA equ 0   
PF_FLOATING_POINT_EMULATED equ 1   
PF_COMPARE_EXCHANGE_DOUBLE equ 2   
PF_MMX_INSTRUCTIONS_AVAILABLE equ 3   
PF_PPC_MOVEMEM_64BIT_OK equ 4   
PF_ALPHA_BYTE_INSTRUCTIONS equ 5   
PF_XMMI_INSTRUCTIONS_AVAILABLE equ 6   
PF_AMD3D_INSTRUCTIONS_AVAILABLE equ 7   
SYSTEM_FLAG_REMOTE_BOOT_CLIENT equ 00000001h   
SYSTEM_FLAG_DISKLESS_CLIENT equ 00000002h   
SECTION_QUERY equ 0001h
SECTION_MAP_WRITE equ 0002h
SECTION_MAP_READ equ 0004h
SECTION_MAP_EXECUTE equ 0008h
SECTION_EXTEND_SIZE equ 0010h
SECTION_ALL_ACCESS equ (STANDARD_RIGHTS_REQUIRED+SECTION_QUERY+ \
                            SECTION_MAP_WRITE +       \
                            SECTION_MAP_READ +        \
                            SECTION_MAP_EXECUTE +     \
                            SECTION_EXTEND_SIZE)
PAGE_NOACCESS equ 01h     
PAGE_READONLY equ 02h     
PAGE_READWRITE equ 04h     
PAGE_WRITECOPY equ 08h     
PAGE_EXECUTE equ 10h     
PAGE_EXECUTE_READ equ 20h     
PAGE_EXECUTE_READWRITE equ 40h     
PAGE_EXECUTE_WRITECOPY equ 80h     
PAGE_GUARD equ 100h     
PAGE_NOCACHE equ 200h     
PAGE_WRITECOMBINE equ 400h     
MEM_COMMIT equ 1000h     
MEM_RESERVE equ 2000h     
MEM_DECOMMIT equ 4000h     
MEM_RELEASE equ 8000h     
MEM_FREE equ 10000h     
MEM_PRIVATE equ 20000h     
MEM_MAPPED equ 40000h     
MEM_RESET equ 80000h     
MEM_TOP_DOWN equ 100000h     
MEM_4MB_PAGES equ 80000000h     
SEC_FILE equ 800000h     
SEC_IMAGE equ 1000000h     
SEC_VLM equ 2000000h     
SEC_RESERVE equ 4000000h     
SEC_COMMIT equ 8000000h     
SEC_NOCACHE equ 10000000h     
MEM_IMAGE equ SEC_IMAGE     
FILE_READ_DATA equ ( 0001h )    
FILE_LIST_DIRECTORY equ ( 0001h )    
FILE_WRITE_DATA equ ( 0002h )    
FILE_ADD_FILE equ ( 0002h )    
FILE_APPEND_DATA equ ( 0004h )    
FILE_ADD_SUBDIRECTORY equ ( 0004h )    
FILE_CREATE_PIPE_INSTANCE equ ( 0004h )    
FILE_READ_EA equ ( 0008h )    
FILE_WRITE_EA equ ( 0010h )    
FILE_EXECUTE equ ( 0020h )    
FILE_TRAVERSE equ ( 0020h )    
FILE_DELETE_CHILD equ ( 0040h )    
FILE_READ_ATTRIBUTES equ ( 0080h )    
FILE_WRITE_ATTRIBUTES equ ( 0100h )    
FILE_GENERIC_READ equ (STANDARD_RIGHTS_READ     + \
                                   FILE_READ_DATA           + \
                                   FILE_READ_ATTRIBUTES     + \
                                   FILE_READ_EA             + \
                                   SYNCHRONIZE)
FILE_GENERIC_WRITE equ (STANDARD_RIGHTS_WRITE    + \
                                   FILE_WRITE_DATA          + \
                                   FILE_WRITE_ATTRIBUTES    + \
                                   FILE_WRITE_EA            + \
                                   FILE_APPEND_DATA         + \
                                   SYNCHRONIZE)
FILE_GENERIC_EXECUTE equ (STANDARD_RIGHTS_EXECUTE  + \
                                   FILE_READ_ATTRIBUTES     + \
                                   FILE_EXECUTE             + \
                                   SYNCHRONIZE)
FILE_SHARE_READ equ 00000001h  
FILE_SHARE_WRITE equ 00000002h  
FILE_SHARE_DELETE equ 00000004h  
FILE_ATTRIBUTE_READONLY equ 00000001h  
FILE_ATTRIBUTE_HIDDEN equ 00000002h  
FILE_ATTRIBUTE_SYSTEM equ 00000004h  
FILE_ATTRIBUTE_DIRECTORY equ 00000010h  
FILE_ATTRIBUTE_ARCHIVE equ 00000020h  
FILE_ATTRIBUTE_ENCRYPTED equ 00000040h  
FILE_ATTRIBUTE_NORMAL equ 00000080h  
FILE_ATTRIBUTE_TEMPORARY equ 00000100h  
FILE_ATTRIBUTE_SPARSE_FILE equ 00000200h  
FILE_ATTRIBUTE_REPARSE_POINT equ 00000400h  
FILE_ATTRIBUTE_COMPRESSED equ 00000800h  
FILE_ATTRIBUTE_OFFLINE equ 00001000h  
FILE_ATTRIBUTE_NOT_CONTENT_INDEXED equ 00002000h  
FILE_NOTIFY_CHANGE_FILE_NAME equ 00000001h   
FILE_NOTIFY_CHANGE_DIR_NAME equ 00000002h   
FILE_NOTIFY_CHANGE_ATTRIBUTES equ 00000004h   
FILE_NOTIFY_CHANGE_SIZE equ 00000008h   
FILE_NOTIFY_CHANGE_LAST_WRITE equ 00000010h   
FILE_NOTIFY_CHANGE_LAST_ACCESS equ 00000020h   
FILE_NOTIFY_CHANGE_CREATION equ 00000040h   
FILE_NOTIFY_CHANGE_SECURITY equ 00000100h   
FILE_ACTION_ADDED equ 00000001h   
FILE_ACTION_REMOVED equ 00000002h   
FILE_ACTION_MODIFIED equ 00000003h   
FILE_ACTION_RENAMED_OLD_NAME equ 00000004h   
FILE_ACTION_RENAMED_NEW_NAME equ 00000005h   
MAILSLOT_NO_MESSAGE equ (-1) 
MAILSLOT_WAIT_FOREVER equ (-1) 
FILE_CASE_SENSITIVE_SEARCH equ 00000001h  
FILE_CASE_PRESERVED_NAMES equ 00000002h  
FILE_UNICODE_ON_DISK equ 00000004h  
FILE_PERSISTENT_ACLS equ 00000008h  
FILE_FILE_COMPRESSION equ 00000010h  
FILE_VOLUME_QUOTAS equ 00000020h  
FILE_SUPPORTS_SPARSE_FILES equ 00000040h  
FILE_SUPPORTS_REPARSE_POINTS equ 00000080h  
FILE_SUPPORTS_REMOTE_STORAGE equ 00000100h  
FILE_VOLUME_IS_COMPRESSED equ 00008000h  
FILE_SUPPORTS_OBJECT_IDS equ 00010000h  
FILE_SUPPORTS_ENCRYPTION equ 00020000h  
MAXIMUM_REPARSE_DATA_BUFFER_SIZE equ ( 16 * 1024 )
IO_REPARSE_TAG_RESERVED_ZERO equ (0)
IO_REPARSE_TAG_RESERVED_ONE equ (1)
IO_REPARSE_TAG_RESERVED_RANGE equ IO_REPARSE_TAG_RESERVED_ONE
IO_REPARSE_TAG_VALID_VALUES equ (0E000FFFFh)
IO_REPARSE_TAG_SYMBOLIC_LINK equ IO_REPARSE_TAG_RESERVED_ZERO
IO_REPARSE_TAG_MOUNT_POINT equ (0A0000003h)
IO_REPARSE_TAG_HSM equ (0C0000004h)
IO_REPARSE_TAG_NSS equ (80000005h)
IO_REPARSE_TAG_NSSRECOVER equ (80000006h)
IO_REPARSE_TAG_SIS equ (80000007h)
IO_REPARSE_TAG_DFS equ (80000008h)
IO_COMPLETION_MODIFY_STATE equ 0002h  
IO_COMPLETION_ALL_ACCESS equ (STANDARD_RIGHTS_REQUIRED+SYNCHRONIZE+3h) 
DUPLICATE_CLOSE_SOURCE equ 00000001h  
DUPLICATE_SAME_ACCESS equ 00000002h  
ES_SYSTEM_REQUIRED equ (00000001h)
ES_DISPLAY_REQUIRED equ (00000002h)
ES_USER_PRESENT equ (00000004h)
ES_CONTINUOUS equ (80000000h)
IMAGE_DOS_SIGNATURE equ 5A4Dh      
IMAGE_OS2_SIGNATURE equ 454Eh      
IMAGE_OS2_SIGNATURE_LE equ 454Ch      
IMAGE_VXD_SIGNATURE equ 454Ch      
IMAGE_NT_SIGNATURE equ 00004550h  
IMAGE_SIZEOF_FILE_HEADER equ 20
IMAGE_FILE_RELOCS_STRIPPED equ 0001h  
IMAGE_FILE_EXECUTABLE_IMAGE equ 0002h  
IMAGE_FILE_LINE_NUMS_STRIPPED equ 0004h  
IMAGE_FILE_LOCAL_SYMS_STRIPPED equ 0008h  
IMAGE_FILE_AGGRESIVE_WS_TRIM equ 0010h  
IMAGE_FILE_LARGE_ADDRESS_AWARE equ 0020h  
IMAGE_FILE_BYTES_REVERSED_LO equ 0080h  
IMAGE_FILE_32BIT_MACHINE equ 0100h  
IMAGE_FILE_DEBUG_STRIPPED equ 0200h  
IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP equ 0400h  
IMAGE_FILE_NET_RUN_FROM_SWAP equ 0800h  
IMAGE_FILE_SYSTEM equ 1000h  
IMAGE_FILE_DLL equ 2000h  
IMAGE_FILE_UP_SYSTEM_ONLY equ 4000h  
IMAGE_FILE_BYTES_REVERSED_HI equ 8000h  
IMAGE_FILE_MACHINE_UNKNOWN equ 0
IMAGE_FILE_MACHINE_I386 equ 014ch  
IMAGE_FILE_MACHINE_R3000 equ 0162h  
IMAGE_FILE_MACHINE_R4000 equ 0166h  
IMAGE_FILE_MACHINE_R10000 equ 0168h  
IMAGE_FILE_MACHINE_WCEMIPSV2 equ 0169h  
IMAGE_FILE_MACHINE_ALPHA equ 0184h  
IMAGE_FILE_MACHINE_POWERPC equ 01F0h  
IMAGE_FILE_MACHINE_SH3 equ 01a2h  
IMAGE_FILE_MACHINE_SH3E equ 01a4h  
IMAGE_FILE_MACHINE_SH4 equ 01a6h  
IMAGE_FILE_MACHINE_ARM equ 01c0h  
IMAGE_FILE_MACHINE_THUMB equ 01c2h
IMAGE_FILE_MACHINE_IA64 equ 0200h  
IMAGE_FILE_MACHINE_MIPS16 equ 0266h  
IMAGE_FILE_MACHINE_MIPSFPU equ 0366h  
IMAGE_FILE_MACHINE_MIPSFPU16 equ 0466h  
IMAGE_FILE_MACHINE_ALPHA64 equ 0284h  
IMAGE_FILE_MACHINE_AXP64 equ IMAGE_FILE_MACHINE_ALPHA64
IMAGE_SIZEOF_ROM_OPTIONAL_HEADER equ 56
IMAGE_SIZEOF_STD_OPTIONAL_HEADER equ 28
IMAGE_SIZEOF_NT_OPTIONAL32_HEADER equ 224
IMAGE_SIZEOF_NT_OPTIONAL64_HEADER equ 240
IMAGE_NT_OPTIONAL_HDR32_MAGIC equ 10bh
IMAGE_NT_OPTIONAL_HDR64_MAGIC equ 20bh
IMAGE_ROM_OPTIONAL_HDR_MAGIC equ 107h
IMAGE_SUBSYSTEM_UNKNOWN equ 0   
IMAGE_SUBSYSTEM_NATIVE equ 1   
IMAGE_SUBSYSTEM_WINDOWS_GUI equ 2   
IMAGE_SUBSYSTEM_WINDOWS_CUI equ 3   
IMAGE_SUBSYSTEM_OS2_CUI equ 5   
IMAGE_SUBSYSTEM_POSIX_CUI equ 7   
IMAGE_SUBSYSTEM_NATIVE_WINDOWS equ 8   
IMAGE_SUBSYSTEM_WINDOWS_CE_GUI equ 9   
IMAGE_DLLCHARACTERISTICS_WDM_DRIVER equ 2000h     
IMAGE_DIRECTORY_ENTRY_EXPORT equ 0   
IMAGE_DIRECTORY_ENTRY_IMPORT equ 1   
IMAGE_DIRECTORY_ENTRY_RESOURCE equ 2   
IMAGE_DIRECTORY_ENTRY_EXCEPTION equ 3   
IMAGE_DIRECTORY_ENTRY_SECURITY equ 4   
IMAGE_DIRECTORY_ENTRY_BASERELOC equ 5   
IMAGE_DIRECTORY_ENTRY_DEBUG equ 6   
IMAGE_DIRECTORY_ENTRY_ARCHITECTURE equ 7   
IMAGE_DIRECTORY_ENTRY_GLOBALPTR equ 8   
IMAGE_DIRECTORY_ENTRY_TLS equ 9   
IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG equ 10   
IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT equ 11   
IMAGE_DIRECTORY_ENTRY_IAT equ 12   
IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT equ 13   
IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR equ 14   
IMAGE_SIZEOF_SHORT_NAME equ 8
IMAGE_SIZEOF_SECTION_HEADER equ 40
IMAGE_SCN_TYPE_NO_PAD equ 00000008h  
IMAGE_SCN_CNT_CODE equ 00000020h  
IMAGE_SCN_CNT_INITIALIZED_DATA equ 00000040h  
IMAGE_SCN_CNT_UNINITIALIZED_DATA equ 00000080h  
IMAGE_SCN_LNK_OTHER equ 00000100h  
IMAGE_SCN_LNK_INFO equ 00000200h  
IMAGE_SCN_LNK_REMOVE equ 00000800h  
IMAGE_SCN_LNK_COMDAT equ 00001000h  
IMAGE_SCN_NO_DEFER_SPEC_EXC equ 00004000h  
IMAGE_SCN_GPREL equ 00008000h  
IMAGE_SCN_MEM_FARDATA equ 00008000h
IMAGE_SCN_MEM_PURGEABLE equ 00020000h
IMAGE_SCN_MEM_16BIT equ 00020000h
IMAGE_SCN_MEM_LOCKED equ 00040000h
IMAGE_SCN_MEM_PRELOAD equ 00080000h
IMAGE_SCN_ALIGN_1BYTES equ 00100000h  
IMAGE_SCN_ALIGN_2BYTES equ 00200000h  
IMAGE_SCN_ALIGN_4BYTES equ 00300000h  
IMAGE_SCN_ALIGN_8BYTES equ 00400000h  
IMAGE_SCN_ALIGN_16BYTES equ 00500000h  
IMAGE_SCN_ALIGN_32BYTES equ 00600000h  
IMAGE_SCN_ALIGN_64BYTES equ 00700000h  
IMAGE_SCN_ALIGN_128BYTES equ 00800000h  
IMAGE_SCN_ALIGN_256BYTES equ 00900000h  
IMAGE_SCN_ALIGN_512BYTES equ 00A00000h  
IMAGE_SCN_ALIGN_1024BYTES equ 00B00000h  
IMAGE_SCN_ALIGN_2048BYTES equ 00C00000h  
IMAGE_SCN_ALIGN_4096BYTES equ 00D00000h  
IMAGE_SCN_ALIGN_8192BYTES equ 00E00000h  
IMAGE_SCN_LNK_NRELOC_OVFL equ 01000000h  
IMAGE_SCN_MEM_DISCARDABLE equ 02000000h  
IMAGE_SCN_MEM_NOT_CACHED equ 04000000h  
IMAGE_SCN_MEM_NOT_PAGED equ 08000000h  
IMAGE_SCN_MEM_SHARED equ 10000000h  
IMAGE_SCN_MEM_EXECUTE equ 20000000h  
IMAGE_SCN_MEM_READ equ 40000000h  
IMAGE_SCN_MEM_WRITE equ 80000000h  
IMAGE_SCN_SCALE_INDEX equ 00000001h  
IMAGE_SIZEOF_SYMBOL equ 18
IMAGE_SYM_UNDEFINED equ 0          
IMAGE_SYM_ABSOLUTE equ -1         
IMAGE_SYM_DEBUG equ -2         
IMAGE_SYM_TYPE_NULL equ 0000h  
IMAGE_SYM_TYPE_VOID equ 0001h  
IMAGE_SYM_TYPE_CHAR equ 0002h  
IMAGE_SYM_TYPE_SHORT equ 0003h  
IMAGE_SYM_TYPE_INT equ 0004h  
IMAGE_SYM_TYPE_LONG equ 0005h  
IMAGE_SYM_TYPE_FLOAT equ 0006h  
IMAGE_SYM_TYPE_DOUBLE equ 0007h  
IMAGE_SYM_TYPE_STRUCT equ 0008h  
IMAGE_SYM_TYPE_UNION equ 0009h  
IMAGE_SYM_TYPE_ENUM equ 000Ah  
IMAGE_SYM_TYPE_MOE equ 000Bh  
IMAGE_SYM_TYPE_BYTE equ 000Ch  
IMAGE_SYM_TYPE_WORD equ 000Dh  
IMAGE_SYM_TYPE_UINT equ 000Eh  
IMAGE_SYM_TYPE_DWORD equ 000Fh  
IMAGE_SYM_TYPE_PCODE equ 8000h  
IMAGE_SYM_DTYPE_NULL equ 0       
IMAGE_SYM_DTYPE_POINTER equ 1       
IMAGE_SYM_DTYPE_FUNCTION equ 2       
IMAGE_SYM_DTYPE_ARRAY equ 3       
IMAGE_SYM_CLASS_END_OF_FUNCTION equ -1
IMAGE_SYM_CLASS_NULL equ 0000h
IMAGE_SYM_CLASS_AUTOMATIC equ 0001h
IMAGE_SYM_CLASS_EXTERNAL equ 0002h
IMAGE_SYM_CLASS_STATIC equ 0003h
IMAGE_SYM_CLASS_REGISTER equ 0004h
IMAGE_SYM_CLASS_EXTERNAL_DEF equ 0005h
IMAGE_SYM_CLASS_LABEL equ 0006h
IMAGE_SYM_CLASS_UNDEFINED_LABEL equ 0007h
IMAGE_SYM_CLASS_MEMBER_OF_STRUCT equ 0008h
IMAGE_SYM_CLASS_ARGUMENT equ 0009h
IMAGE_SYM_CLASS_STRUCT_TAG equ 000Ah
IMAGE_SYM_CLASS_MEMBER_OF_UNION equ 000Bh
IMAGE_SYM_CLASS_UNION_TAG equ 000Ch
IMAGE_SYM_CLASS_TYPE_DEFINITION equ 000Dh
IMAGE_SYM_CLASS_UNDEFINED_STATIC equ 000Eh
IMAGE_SYM_CLASS_ENUM_TAG equ 000Fh
IMAGE_SYM_CLASS_MEMBER_OF_ENUM equ 0010h
IMAGE_SYM_CLASS_REGISTER_PARAM equ 0011h
IMAGE_SYM_CLASS_BIT_FIELD equ 0012h
IMAGE_SYM_CLASS_FAR_EXTERNAL equ 0044h  
IMAGE_SYM_CLASS_BLOCK equ 0064h
IMAGE_SYM_CLASS_FUNCTION equ 0065h
IMAGE_SYM_CLASS_END_OF_STRUCT equ 0066h
IMAGE_SYM_CLASS_FILE equ 0067h
IMAGE_SYM_CLASS_SECTION equ 0068h
IMAGE_SYM_CLASS_WEAK_EXTERNAL equ 0069h
N_BTMASK equ 000Fh
N_TMASK equ 0030h
N_TMASK1 equ 00C0h
N_TMASK2 equ 00F0h
N_BTSHFT equ 4
N_TSHIFT equ 2
IMAGE_SIZEOF_AUX_SYMBOL equ 18
IMAGE_COMDAT_SELECT_NODUPLICATES equ 1
IMAGE_COMDAT_SELECT_ANY equ 2
IMAGE_COMDAT_SELECT_SAME_SIZE equ 3
IMAGE_COMDAT_SELECT_EXACT_MATCH equ 4
IMAGE_COMDAT_SELECT_ASSOCIATIVE equ 5
IMAGE_COMDAT_SELECT_LARGEST equ 6
IMAGE_COMDAT_SELECT_NEWEST equ 7
IMAGE_WEAK_EXTERN_SEARCH_NOLIBRARY equ 1
IMAGE_WEAK_EXTERN_SEARCH_LIBRARY equ 2
IMAGE_WEAK_EXTERN_SEARCH_ALIAS equ 3
IMAGE_SIZEOF_RELOCATION equ 10
IMAGE_REL_I386_ABSOLUTE equ 0000h  
IMAGE_REL_I386_DIR16 equ 0001h  
IMAGE_REL_I386_REL16 equ 0002h  
IMAGE_REL_I386_DIR32 equ 0006h  
IMAGE_REL_I386_DIR32NB equ 0007h  
IMAGE_REL_I386_SEG12 equ 0009h  
IMAGE_REL_I386_SECTION equ 000Ah
IMAGE_REL_I386_SECREL equ 000Bh
IMAGE_REL_I386_REL32 equ 0014h  
IMAGE_SIZEOF_LINENUMBER equ 6
IMAGE_SIZEOF_BASE_RELOCATION equ 8
IMAGE_REL_BASED_ABSOLUTE equ 0
IMAGE_REL_BASED_HIGH equ 1
IMAGE_REL_BASED_LOW equ 2
IMAGE_REL_BASED_HIGHLOW equ 3
IMAGE_REL_BASED_HIGHADJ equ 4
IMAGE_REL_BASED_MIPS_JMPADDR equ 5
IMAGE_REL_BASED_SECTION equ 6
IMAGE_REL_BASED_REL32 equ 7
IMAGE_REL_BASED_MIPS_JMPADDR16 equ 9
IMAGE_REL_BASED_IA64_IMM64 equ 9
IMAGE_REL_BASED_DIR64 equ 10
IMAGE_REL_BASED_HIGH3ADJ equ 11
IMAGE_ARCHIVE_START_SIZE equ 8
IMAGE_ARCHIVE_START equ "!<arch>",10
IMAGE_ARCHIVE_END equ "`",10
IMAGE_ARCHIVE_PAD equ 10
IMAGE_ARCHIVE_LINKER_MEMBER equ "/               "
IMAGE_ARCHIVE_LONGNAMES_MEMBER equ "//              "
IMAGE_SIZEOF_ARCHIVE_MEMBER_HDR equ 60
IMAGE_ORDINAL_FLAG32 equ 80000000h
IMAGE_RESOURCE_NAME_IS_STRING equ 80000000h
IMAGE_RESOURCE_DATA_IS_DIRECTORY equ 80000000h
IMAGE_DEBUG_TYPE_UNKNOWN equ 0
IMAGE_DEBUG_TYPE_COFF equ 1
IMAGE_DEBUG_TYPE_CODEVIEW equ 2
IMAGE_DEBUG_TYPE_FPO equ 3
IMAGE_DEBUG_TYPE_MISC equ 4
IMAGE_DEBUG_TYPE_EXCEPTION equ 5
IMAGE_DEBUG_TYPE_FIXUP equ 6
IMAGE_DEBUG_TYPE_OMAP_TO_SRC equ 7
IMAGE_DEBUG_TYPE_OMAP_FROM_SRC equ 8
IMAGE_DEBUG_TYPE_BORLAND equ 9
IMAGE_DEBUG_TYPE_RESERVED10 equ 10
FRAME_FPO equ 0
FRAME_TRAP equ 1
FRAME_TSS equ 2
FRAME_NONFPO equ 3
SIZEOF_RFPO_DATA equ 16
IMAGE_DEBUG_MISC_EXENAME equ 1
IMAGE_SEPARATE_DEBUG_SIGNATURE equ 4944h
IMAGE_SEPARATE_DEBUG_FLAGS_MASK equ 8000h
IMAGE_SEPARATE_DEBUG_MISMATCH equ 8000h  
HEAP_NO_SERIALIZE equ 00000001h      
HEAP_GROWABLE equ 00000002h      
HEAP_GENERATE_EXCEPTIONS equ 00000004h      
HEAP_ZERO_MEMORY equ 00000008h      
HEAP_REALLOC_IN_PLACE_ONLY equ 00000010h      
HEAP_TAIL_CHECKING_ENABLED equ 00000020h      
HEAP_FREE_CHECKING_ENABLED equ 00000040h      
HEAP_DISABLE_COALESCE_ON_FREE equ 00000080h      
HEAP_CREATE_ALIGN_16 equ 00010000h      
HEAP_CREATE_ENABLE_TRACING equ 00020000h      
HEAP_MAXIMUM_TAG equ 0FFFh              
HEAP_PSEUDO_TAG_FLAG equ 8000h              
HEAP_TAG_SHIFT equ 18                  
IS_TEXT_UNICODE_ASCII16 equ 0001h
IS_TEXT_UNICODE_REVERSE_ASCII16 equ 0010h
IS_TEXT_UNICODE_STATISTICS equ 0002h
IS_TEXT_UNICODE_REVERSE_STATISTICS equ 0020h
IS_TEXT_UNICODE_CONTROLS equ 0004h
IS_TEXT_UNICODE_REVERSE_CONTROLS equ 0040h
IS_TEXT_UNICODE_SIGNATURE equ 0008h
IS_TEXT_UNICODE_REVERSE_SIGNATURE equ 0080h
IS_TEXT_UNICODE_ILLEGAL_CHARS equ 0100h
IS_TEXT_UNICODE_ODD_LENGTH equ 0200h
IS_TEXT_UNICODE_DBCS_LEADBYTE equ 0400h
IS_TEXT_UNICODE_NULL_BYTES equ 1000h
IS_TEXT_UNICODE_UNICODE_MASK equ 000Fh
IS_TEXT_UNICODE_REVERSE_MASK equ 00F0h
IS_TEXT_UNICODE_NOT_UNICODE_MASK equ 0F00h
IS_TEXT_UNICODE_NOT_ASCII_MASK equ 0F000h
COMPRESSION_FORMAT_NONE equ (0000h)   
COMPRESSION_FORMAT_DEFAULT equ (0001h)   
COMPRESSION_FORMAT_LZNT1 equ (0002h)   
COMPRESSION_ENGINE_STANDARD equ (0000h)   
COMPRESSION_ENGINE_MAXIMUM equ (0100h)   
MESSAGE_RESOURCE_UNICODE equ 0001h
RTL_CRITSECT_TYPE equ 0
RTL_RESOURCE_TYPE equ 1
SEF_DACL_AUTO_INHERIT equ 1h
SEF_SACL_AUTO_INHERIT equ 2h
SEF_DEFAULT_DESCRIPTOR_FOR_OBJECT equ 4h
SEF_AVOID_PRIVILEGE_CHECK equ 8h
WT_EXECUTEDEFAULT equ 00000000h                           
WT_EXECUTEINIOTHREAD equ 00000001h                           
WT_EXECUTEINUITHREAD equ 00000002h                           
WT_EXECUTEINWAITTHREAD equ 00000004h                           
WT_EXECUTEDELETEWAIT equ 00000008h                           
WT_EXECUTEINLONGTHREAD equ 00000010h                           
DLL_PROCESS_ATTACH equ 1    
DLL_THREAD_ATTACH equ 2    
DLL_THREAD_DETACH equ 3    
DLL_PROCESS_DETACH equ 0    
EVENTLOG_SEQUENTIAL_READ equ 0001h
EVENTLOG_SEEK_READ equ 0002h
EVENTLOG_FORWARDS_READ equ 0004h
EVENTLOG_BACKWARDS_READ equ 0008h
EVENTLOG_SUCCESS equ 0000h
EVENTLOG_ERROR_TYPE equ 0001h
EVENTLOG_WARNING_TYPE equ 0002h
EVENTLOG_INFORMATION_TYPE equ 0004h
EVENTLOG_AUDIT_SUCCESS equ 0008h
EVENTLOG_AUDIT_FAILURE equ 0010h
EVENTLOG_START_PAIRED_EVENT equ 0001h
EVENTLOG_END_PAIRED_EVENT equ 0002h
EVENTLOG_END_ALL_PAIRED_EVENTS equ 0004h
EVENTLOG_PAIRED_EVENT_ACTIVE equ 0008h
EVENTLOG_PAIRED_EVENT_INACTIVE equ 0010h
MAXLOGICALLOGNAMESIZE equ 256
DBG_CONTINUE equ (00010002h) 
DBG_TERMINATE_THREAD equ (40010003h) 
DBG_TERMINATE_PROCESS equ (40010004h) 
DBG_CONTROL_C equ (40010005h) 
DBG_CONTROL_BREAK equ (40010008h) 
DBG_EXCEPTION_NOT_HANDLED equ (80010001h) 
KEY_QUERY_VALUE equ (0001h)
KEY_SET_VALUE equ (0002h)
KEY_CREATE_SUB_KEY equ (0004h)
KEY_ENUMERATE_SUB_KEYS equ (0008h)
KEY_NOTIFY equ (0010h)
KEY_CREATE_LINK equ (0020h)
REG_OPTION_RESERVED equ (00000000h)   
REG_OPTION_NON_VOLATILE equ (00000000h)   
REG_OPTION_VOLATILE equ (00000001h)   
REG_OPTION_CREATE_LINK equ (00000002h)   
REG_OPTION_BACKUP_RESTORE equ (00000004h)   
REG_OPTION_OPEN_LINK equ (00000008h)   
REG_CREATED_NEW_KEY equ (00000001h)   
REG_OPENED_EXISTING_KEY equ (00000002h)   
REG_WHOLE_HIVE_VOLATILE equ (00000001h)   
REG_REFRESH_HIVE equ (00000002h)   
REG_NO_LAZY_FLUSH equ (00000004h)   
REG_NOTIFY_CHANGE_NAME equ (00000001h) 
REG_NOTIFY_CHANGE_ATTRIBUTES equ (00000002h)
REG_NOTIFY_CHANGE_LAST_SET equ (00000004h) 
REG_NOTIFY_CHANGE_SECURITY equ (00000008h)
REG_LEGAL_CHANGE_FILTER equ (REG_NOTIFY_CHANGE_NAME         + \
                             REG_NOTIFY_CHANGE_ATTRIBUTES   + \
                             REG_NOTIFY_CHANGE_LAST_SET     + \
                             REG_NOTIFY_CHANGE_SECURITY)
REG_NONE                        equ ( 0 )   
REG_SZ                          equ ( 1 )   
REG_EXPAND_SZ                   equ ( 2 )   
REG_BINARY                      equ ( 3 )   
REG_DWORD                       equ ( 4 )   
REG_DWORD_LITTLE_ENDIAN         equ ( 4 )   
REG_DWORD_BIG_ENDIAN            equ ( 5 )   
REG_LINK                        equ ( 6 )   
REG_MULTI_SZ                    equ ( 7 )   
REG_RESOURCE_LIST               equ ( 8 )   
REG_FULL_RESOURCE_DESCRIPTOR    equ ( 9 )  
REG_RESOURCE_REQUIREMENTS_LIST  equ (10 )
SERVICE_KERNEL_DRIVER equ 00000001h
SERVICE_FILE_SYSTEM_DRIVER equ 00000002h
SERVICE_ADAPTER equ 00000004h
SERVICE_RECOGNIZER_DRIVER equ 00000008h
SERVICE_DRIVER equ (SERVICE_KERNEL_DRIVER + SERVICE_FILE_SYSTEM_DRIVER + SERVICE_RECOGNIZER_DRIVER)
SERVICE_WIN32_OWN_PROCESS equ 00000010h
SERVICE_WIN32_SHARE_PROCESS equ 00000020h
SERVICE_WIN32 equ (SERVICE_WIN32_OWN_PROCESS +  SERVICE_WIN32_SHARE_PROCESS)
SERVICE_INTERACTIVE_PROCESS equ 00000100h
SERVICE_TYPE_ALL equ (SERVICE_WIN32+SERVICE_ADAPTER + SERVICE_DRIVER  + SERVICE_INTERACTIVE_PROCESS)
SERVICE_BOOT_START equ 00000000h
SERVICE_SYSTEM_START equ 00000001h
SERVICE_AUTO_START equ 00000002h
SERVICE_DEMAND_START equ 00000003h
SERVICE_DISABLED equ 00000004h
SERVICE_ERROR_IGNORE equ 00000000h
SERVICE_ERROR_NORMAL equ 00000001h
SERVICE_ERROR_SEVERE equ 00000002h
SERVICE_ERROR_CRITICAL equ 00000003h
TAPE_ERASE_SHORT equ 0
TAPE_ERASE_LONG equ 1
TAPE_LOAD equ 0
TAPE_UNLOAD equ 1
TAPE_TENSION equ 2
TAPE_LOCK equ 3
TAPE_UNLOCK equ 4
TAPE_FORMAT equ 5
TAPE_SETMARKS equ 0
TAPE_FILEMARKS equ 1
TAPE_SHORT_FILEMARKS equ 2
TAPE_LONG_FILEMARKS equ 3
TAPE_ABSOLUTE_POSITION equ 0
TAPE_LOGICAL_POSITION equ 1
TAPE_PSEUDO_LOGICAL_POSITION equ 2
TAPE_REWIND equ 0
TAPE_ABSOLUTE_BLOCK equ 1
TAPE_LOGICAL_BLOCK equ 2
TAPE_PSEUDO_LOGICAL_BLOCK equ 3
TAPE_SPACE_END_OF_DATA equ 4
TAPE_SPACE_RELATIVE_BLOCKS equ 5
TAPE_SPACE_FILEMARKS equ 6
TAPE_SPACE_SEQUENTIAL_FMKS equ 7
TAPE_SPACE_SETMARKS equ 8
TAPE_SPACE_SEQUENTIAL_SMKS equ 9
TAPE_DRIVE_FIXED equ 00000001h
TAPE_DRIVE_SELECT equ 00000002h
TAPE_DRIVE_INITIATOR equ 00000004h
TAPE_DRIVE_ERASE_SHORT equ 00000010h
TAPE_DRIVE_ERASE_LONG equ 00000020h
TAPE_DRIVE_ERASE_BOP_ONLY equ 00000040h
TAPE_DRIVE_ERASE_IMMEDIATE equ 00000080h
TAPE_DRIVE_TAPE_CAPACITY equ 00000100h
TAPE_DRIVE_TAPE_REMAINING equ 00000200h
TAPE_DRIVE_FIXED_BLOCK equ 00000400h
TAPE_DRIVE_VARIABLE_BLOCK equ 00000800h
TAPE_DRIVE_WRITE_PROTECT equ 00001000h
TAPE_DRIVE_EOT_WZ_SIZE equ 00002000h
TAPE_DRIVE_ECC equ 00010000h
TAPE_DRIVE_COMPRESSION equ 00020000h
TAPE_DRIVE_PADDING equ 00040000h
TAPE_DRIVE_REPORT_SMKS equ 00080000h
TAPE_DRIVE_GET_ABSOLUTE_BLK equ 00100000h
TAPE_DRIVE_GET_LOGICAL_BLK equ 00200000h
TAPE_DRIVE_SET_EOT_WZ_SIZE equ 00400000h
TAPE_DRIVE_EJECT_MEDIA equ 01000000h
TAPE_DRIVE_CLEAN_REQUESTS equ 02000000h
TAPE_DRIVE_SET_CMP_BOP_ONLY equ 04000000h
TAPE_DRIVE_RESERVED_BIT equ 80000000h  
TAPE_DRIVE_LOAD_UNLOAD equ 80000001h
TAPE_DRIVE_TENSION equ 80000002h
TAPE_DRIVE_LOCK_UNLOCK equ 80000004h
TAPE_DRIVE_REWIND_IMMEDIATE equ 80000008h
TAPE_DRIVE_SET_BLOCK_SIZE equ 80000010h
TAPE_DRIVE_LOAD_UNLD_IMMED equ 80000020h
TAPE_DRIVE_TENSION_IMMED equ 80000040h
TAPE_DRIVE_LOCK_UNLK_IMMED equ 80000080h
TAPE_DRIVE_SET_ECC equ 80000100h
TAPE_DRIVE_SET_COMPRESSION equ 80000200h
TAPE_DRIVE_SET_PADDING equ 80000400h
TAPE_DRIVE_SET_REPORT_SMKS equ 80000800h
TAPE_DRIVE_ABSOLUTE_BLK equ 80001000h
TAPE_DRIVE_ABS_BLK_IMMED equ 80002000h
TAPE_DRIVE_LOGICAL_BLK equ 80004000h
TAPE_DRIVE_LOG_BLK_IMMED equ 80008000h
TAPE_DRIVE_END_OF_DATA equ 80010000h
TAPE_DRIVE_RELATIVE_BLKS equ 80020000h
TAPE_DRIVE_FILEMARKS equ 80040000h
TAPE_DRIVE_SEQUENTIAL_FMKS equ 80080000h
TAPE_DRIVE_SETMARKS equ 80100000h
TAPE_DRIVE_SEQUENTIAL_SMKS equ 80200000h
TAPE_DRIVE_REVERSE_POSITION equ 80400000h
TAPE_DRIVE_SPACE_IMMEDIATE equ 80800000h
TAPE_DRIVE_WRITE_SETMARKS equ 81000000h
TAPE_DRIVE_WRITE_FILEMARKS equ 82000000h
TAPE_DRIVE_WRITE_SHORT_FMKS equ 84000000h
TAPE_DRIVE_WRITE_LONG_FMKS equ 88000000h
TAPE_DRIVE_WRITE_MARK_IMMED equ 090000000h
TAPE_DRIVE_FORMAT equ 0A0000000h
TAPE_DRIVE_FORMAT_IMMEDIATE equ 0C0000000h
TAPE_DRIVE_HIGH_FEATURES equ 80000000h  
IMAGE_NUMBEROF_DIRECTORY_ENTRIES equ 16
IMPORT_OBJECT_HDR_SIG2 equ 0ffffh
TAPE_FIXED_PARTITIONS equ 0
TAPE_SELECT_PARTITIONS equ 1
TAPE_INITIATOR_PARTITIONS equ 2

IMAGE_NUMBEROF_DIRECTORY_ENTRIES    equ 16

DriverType               equ SERVICE_KERNEL_DRIVER
FileSystemType           equ SERVICE_FILE_SYSTEM_DRIVER
Win32ServiceOwnProcess   equ SERVICE_WIN32_OWN_PROCESS
Win32ServiceShareProcess equ SERVICE_WIN32_SHARE_PROCESS
AdapterType              equ SERVICE_ADAPTER
RecognizerType           equ SERVICE_RECOGNIZER_DRIVER

BootLoad    equ SERVICE_BOOT_START
SystemLoad  equ SERVICE_SYSTEM_START
AutoLoad    equ SERVICE_AUTO_START
DemandLoad  equ SERVICE_DEMAND_START
DisableLoad equ SERVICE_DISABLED

IgnoreError   equ SERVICE_ERROR_IGNORE
NormalError   equ SERVICE_ERROR_NORMAL
SevereError   equ SERVICE_ERROR_SEVERE
CriticalError equ SERVICE_ERROR_CRITICAL
IMPORT_OBJECT_HDR_SIG2  equ 0ffffh

IMPORT_OBJECT_CODE      equ 0
IMPORT_OBJECT_DATA      equ 1
IMPORT_OBJECT_CONST     equ 2

IMPORT_OBJECT_ORDINAL   equ 0
IMPORT_OBJECT_NAME      equ 1
IMPORT_OBJECT_NAME_NO_PREFIX equ  2
IMPORT_OBJECT_NAME_UNDECORATE equ 3

EXECUTION_STATE         equ DWORD

LT_DONT_CARE            equ 0
LT_LOWEST_LATENCY       equ 1

TokenPrimary            equ 1
TokenImpersonation      equ 2

TokenUser               equ 1
TokenGroups             equ 2
TokenPrivileges         equ 3
TokenOwner              equ 4
TokenPrimaryGroup       equ 5
TokenDefaultDacl        equ 6
TokenSource             equ 7
TokenType               equ 8
TokenImpersonationLevel equ 9
TokenStatistics         equ 10
TokenRestrictedSids     equ 11
TokenSessionId          equ 12

AuditEventObjectAccess  equ 0
AuditEventDirectoryServiceAccess equ 1

AclRevisionInformation  equ 1
AclSizeInformation      equ 2

SidTypeUser             equ 1
SidTypeGroup            equ 2
SidTypeDomain           equ 3
SidTypeAlias            equ 4
SidTypeWellKnownGroup   equ 5
SidTypeDeletedAccount   equ 6
SidTypeInvalid          equ 7
SidTypeUnknown          equ 8
SidTypeComputer         equ 9

LIST_ENTRY STRUCT
    Flink DWORD ?
    Blink DWORD ?
LIST_ENTRY ENDS

SINGLE_LIST_ENTRY STRUCT
	Next DWORD ?
SINGLE_LIST_ENTRY ENDS

GUID	STRUCT
    Data1 	dd ?
    Data2 	dw ?
    Data3 	dw ?
    Data4 	db 8 dup(?)
GUID ENDS

OBJECTID STRUCT
    Lineage         GUID <>
    Uniquifier      dd ?
OBJECTID ENDS

LDTBits RECORD  rBaseHi:8, Granularity:1,Default_Big : 1,Reserved_0:1,Sys:1,LimitHi:4,Pres:1, Dpl:2, S_bit:1, Type1:4,rBaseMid:8

LDT_ENTRY STRUCT
    LimitLow	dw ?
    BaseLow		dw ?
    union HighWord1
        Bytes LDT_BYTES <>
        Bits LDTBits <>
    ends
LDT_ENTRY ENDS

SID_IDENTIFIER_AUTHORITY STRUCT
    Value db 6 dup(?)
SID_IDENTIFIER_AUTHORITY ENDS

SID STRUCT
   Revision db ?
   SubAuthorityCount db ?
   IdentifierAuthority SID_IDENTIFIER_AUTHORITY <>
   SubAuthority	dd ANYSIZE_ARRAY dup(?)
SID ENDS

ACL STRUCT
    AclRevision db  ?
    Sbz1 db         ?
    AclSize dw      ?
    AceCount dw     ?
    Sbz2 dw         ?
ACL ENDS

ACCESS_ALLOWED_OBJECT_ACE STRUCT
    Header ACE_HEADER   <>
    Mask1 dd            ?
    Flags dd            ?
    ObjectType GUID     <>
    InheritedObjectType GUID <>
    SidStart dd         ?
ACCESS_ALLOWED_OBJECT_ACE ENDS

ACCESS_DENIED_OBJECT_ACE STRUCT
    Header ACE_HEADER   <>
    Mask1 dd            ?
    Flags dd            ?
    ObjectType GUID     <>
    InheritedObjectType GUID <>
    SidStart dd         ?
ACCESS_DENIED_OBJECT_ACE ENDS

SYSTEM_AUDIT_OBJECT_ACE STRUCT
    Header ACE_HEADER   <>
    Mask1 dd            ?
    Flags dd            ?
    ObjectType GUID     <>
    InheritedObjectType GUID <>
    SidStart dd         ?
SYSTEM_AUDIT_OBJECT_ACE ENDS

SYSTEM_ALARM_OBJECT_ACE STRUCT
    Header ACE_HEADER <>
    Mask1 dd        ?
    Flags dd        ?
    ObjectType GUID <>
    InheritedObjectType GUID <>
    SidStart dd     ?
SYSTEM_ALARM_OBJECT_ACE ENDS

SECURITY_DESCRIPTOR_RELATIVE STRUCT
    Revision db     ?
    Sbz1 db         ?
    Control1 dw     ?
    Owner dd        ?
    Group1 dd       ?
    Sacl dd         ?
    Dacl dd         ?
SECURITY_DESCRIPTOR_RELATIVE ENDS

OBJECT_TYPE_LIST STRUCT
    Level dw        ?
    Sbz dw          ?
    ObjectType dd   ?
OBJECT_TYPE_LIST ENDS

TOKEN_USER STRUCT
    User SID_AND_ATTRIBUTES <>
TOKEN_USER ENDS

TOKEN_OWNER STRUCT
    Owner dd ?
TOKEN_OWNER ENDS

TOKEN_PRIMARY_GROUP STRUCT
    PrimaryGroup dd ?
TOKEN_PRIMARY_GROUP ENDS

TOKEN_DEFAULT_DACL STRUCT
    DefaultDacl dd ?
TOKEN_DEFAULT_DACL ENDS

TOKEN_SOURCE STRUCT
    SourceName db TOKEN_SOURCE_LENGTH dup(?)
    SourceIdentifier LUID <>
TOKEN_SOURCE ends

TOKEN_CONTROL STRUCT
    TokenId LUID                <>
    AuthenticationId LUID       <>
    ModifiedId LUID             <>
    TokenSource1 TOKEN_SOURCE   <>
TOKEN_CONTROL ENDS

NT_TIB STRUCT
    ExceptionList dd    ?
    StackBase dd        ?
    StackLimit dd       ?
    SubSystemTib dd     ?
    union
        FiberData dd    ?
        Version dd      ?
    ends
    ArbitraryUserPointer dd ?
    Self dd ?
NT_TIB ENDS

FILE_NOTIFY_INFORMATION STRUCT
    NextEntryOffset dd  ?
    Action dd           ?
    FileNameLength dd   ?
    FileName dw         ?
FILE_NOTIFY_INFORMATION ENDS


REPARSE_DATA_BUFFER STRUCT
    ReparseTag dd           ?
    ReparseDataLength dw    ?
    Reserved dw             ?
    union
        struct SymbolicLinkReparseBuffer
            SubstituteNameOffset dw ?
            SubstituteNameLength dw ?
            PrintNameOffset dw      ?
            PrintNameLength dw      ?
            PathBuffer dw           ?
		  ends
        struct MountPointReparseBuffer
            SubstituteNameOffset dw ?
            SubstituteNameLength dw ?
            PrintNameOffset dw      ?
            PrintNameLength dw      ?
            PathBuffer dw           ?
        ends
        struct GenericReparseBuffer
            DataBuffer db ?
        ends
    ends
REPARSE_DATA_BUFFER ENDS

REPARSE_GUID_DATA_BUFFER STRUCT
    ReparseTag dd           ?
    ReparseDataLength dw    ?
    Reserved dw             ?
    ReparseGuid GUID        <>
    struct GenericReparseBuffer
        DataBuffer db ?
    ends
REPARSE_GUID_DATA_BUFFER ENDS


REPARSE_POINT_INFORMATION STRUCT
    ReparseDataLength dw    ?
    UnparsedNameLength dw   ?
REPARSE_POINT_INFORMATION ENDS

POWER_DEVICE_TIMEOUTS STRUCT
    ConservationIdleTime dd ?
    PerformanceIdleTime dd  ?
POWER_DEVICE_TIMEOUTS ENDS

IMAGE_VXD_HEADER STRUCT
    e32_magic dw        ?
    e32_border db       ?
    e32_worder db       ?
    e32_level dd        ?
    e32_cpu dw          ?
    e32_os dw           ?
    e32_ver dd          ?
    e32_mflags dd       ?
    e32_mpages dd       ?
    e32_startobj dd     ?
    e32_eip dd          ?
    e32_stackobj dd     ?
    e32_esp dd          ?
    e32_pagesize dd     ?
    e32_lastpagesize dd ?
    e32_fixupsize dd    ?
    e32_fixupsum dd     ?
    e32_ldrsize dd      ?
    e32_ldrsum dd       ?
    e32_objtab dd       ?
    e32_objcnt dd       ?
    e32_objmap dd       ?
    e32_itermap dd      ?
    e32_rsrctab dd      ?
    e32_rsrccnt dd      ?
    e32_restab dd       ?
    e32_enttab dd       ?
    e32_dirtab dd       ?
    e32_dircnt dd       ?
    e32_fpagetab dd     ?
    e32_frectab dd      ?
    e32_impmod dd       ?
    e32_impmodcnt dd    ?
    e32_impproc dd      ?
    e32_pagesum dd      ?
    e32_datapage dd     ?
    e32_preload dd      ?
    e32_nrestab dd      ?
    e32_cbnrestab dd    ?
    e32_nressum dd      ?
    e32_autodata dd     ?
    e32_debuginfo dd    ?
    e32_debuglen dd     ?
    e32_instpreload dd  ?
    e32_instdemand dd   ?
    e32_heapsize dd     ?
    e32_res3 db 12    dup(?)
    e32_winresoff dd    ?
    e32_winreslen dd    ?
    e32_devid dw        ?
    e32_ddkver dw       ?
IMAGE_VXD_HEADER ENDS

IMAGE_ROM_OPTIONAL_HEADER STRUCT
    Magic dw                    ?
    MajorLinkerVersion db       ?
    MinorLinkerVersion db       ?
    SizeOfCode dd               ?
    SizeOfInitializedData dd    ?
    SizeOfUninitializedData dd  ?
    AddressOfEntryPoint dd      ?
    BaseOfCode dd               ?
    BaseOfData dd               ?
    BaseOfBss dd                ?
    GprMask dd                  ?
    CprMask dd              4 dup(?)
    GpValue dd                  ?
IMAGE_ROM_OPTIONAL_HEADER ENDS

IMAGE_ROM_HEADERS STRUCT
    FileHeader IMAGE_FILE_HEADER <>
    OptionalHeader IMAGE_ROM_OPTIONAL_HEADER <>
IMAGE_ROM_HEADERS ENDS

IMAGE_SECTION_HEADER STRUCT
    Name1 db IMAGE_SIZEOF_SHORT_NAME dup(?)
    union Misc
        PhysicalAddress dd  ?
        VirtualSize dd      ?
    ends
    VirtualAddress dd       ?
    SizeOfRawData dd        ?
    PointerToRawData dd     ?
    PointerToRelocations dd ?
    PointerToLinenumbers dd ?
    NumberOfRelocations dw  ?
    NumberOfLinenumbers dw  ?
    Characteristics dd      ?
IMAGE_SECTION_HEADER ENDS

IMAGE_SYMBOL STRUCT
    union N
        ShortName db 8 dup(?)
        struct Name1
            Short1 dd       ?
            Long1 dd        ?
        ends
        LongName dd 2 dup(?)
    ends
    Value dd                ?
    SectionNumber dw        ?
    Type1 dw                ?
    StorageClass db         ?
    NumberOfAuxSymbols db   ?
IMAGE_SYMBOL ENDS

IMAGE_AUX_SYMBOL STRUCT
    struct Sym
        TagIndex dd ?
        union Misc
            struct LnSz
                Linenumber dw ?
                Size1 dw ?
			   ends
           TotalSize dd ?
        ends
        union FcnAry
            struct Function
                PointerToLinenumber dd ?
                PointerToNextFunction dd ?
            ends
            struct Array
                Dimension dw 4 dup(?)
            ends
        ends
        TvIndex dw ?
	 ends
    struct File
        Name1 db IMAGE_SIZEOF_SYMBOL dup(?)
	 ends
    struct Section
        Length1 dd              ?
        NumberOfRelocations dw  ?
        NumberOfLinenumbers dw  ?
        CheckSum dd             ?
        Number dw               ?
        Selection db            ?
    ends
IMAGE_AUX_SYMBOL ENDS

IMAGE_RELOCATION STRUCT
    union 
        VirtualAddress dd   ?
        RelocCount dd       ?
    ends
    SymbolTableIndex dd     ?
    Type1 dw ?
IMAGE_RELOCATION ENDS

IMAGE_LINENUMBER STRUCT
    union Type1
        SymbolTableIndex dd ?
        VirtualAddress dd   ?
	 ends
    Linenumber dw ?
IMAGE_LINENUMBER ENDS


IMAGE_BASE_RELOCATION STRUCT
    VirtualAddress dd   ?
    SizeOfBlock dd      ?
IMAGE_BASE_RELOCATION ENDS

IMAGE_ARCHIVE_MEMBER_HEADER STRUCT
    Name1 db        16 dup(?)
    Date db         12 dup(?)
    UserID db       6 dup(?)
    GroupID db      6 dup(?)
    Mode db         8 dup(?)
    Size1 db        10 dup(?)
    EndHeader db    2 dup(?)
IMAGE_ARCHIVE_MEMBER_HEADER ENDS


IMAGE_IMPORT_BY_NAME STRUCT
    Hint dw     ?
    Name1 db    ?
IMAGE_IMPORT_BY_NAME ENDS

IMAGE_THUNK_DATA32 STRUCT
    union u1
        ForwarderString dd  ?
        Function dd         ?
        Ordinal dd          ?
        AddressOfData dd    ?
    ends
IMAGE_THUNK_DATA32 ENDS


IMAGE_TLS_DIRECTORY32 STRUCT
    StartAddressOfRawData dd    ?
    EndAddressOfRawData dd      ?
    AddressOfIndex dd           ?
    AddressOfCallBacks dd       ?

    SizeOfZeroFill dd           ?
    Characteristics dd          ?
IMAGE_TLS_DIRECTORY32 ENDS

IMAGE_THUNK_DATA EQU <IMAGE_THUNK_DATA32>
IMAGE_TLS_DIRECTORY EQU <IMAGE_TLS_DIRECTORY32>

IMAGE_IMPORT_DESCRIPTOR STRUCT
    union
        Characteristics dd      ?
        OriginalFirstThunk dd   ?
	 ends
    TimeDateStamp dd    ?
    ForwarderChain dd   ?
    Name1 dd            ?
    FirstThunk dd       ?
IMAGE_IMPORT_DESCRIPTOR ENDS

IMAGE_BOUND_IMPORT_DESCRIPTOR STRUCT
    TimeDateStamp dd    ?
    OffsetModuleName dw ?
    NumberOfModuleForwarderRefs dw ?
IMAGE_BOUND_IMPORT_DESCRIPTOR ENDS

IMAGE_BOUND_FORWARDER_REF STRUCT
    TimeDateStamp dd    ?
    OffsetModuleName dw ?
    Reserved dw         ?
IMAGE_BOUND_FORWARDER_REF ENDS

IMAGE_RESOURCE_DIRECTORY STRUCT
    Characteristics dd      ?
    TimeDateStamp dd        ?
    MajorVersion dw         ?
    MinorVersion dw         ?
    NumberOfNamedEntries dw ?
    NumberOfIdEntries dw    ?
IMAGE_RESOURCE_DIRECTORY ENDS

IMAGE_RESOURCE_DIRECTORY_ENTRY STRUCT
    union
        rName	RECORD NameIsString:1,NameOffset:31
        Name1 dd ?
        Id dw ?
    ends
    union
        OffsetToData dd ?
		  rDirectory	RECORD DataIsDirectory:1,OffsetToDirectory:31
    ends
IMAGE_RESOURCE_DIRECTORY_ENTRY ENDS

IMAGE_RESOURCE_DIRECTORY_STRING STRUCT
    Length1 dw      ?
    NameString db   ?
IMAGE_RESOURCE_DIRECTORY_STRING ENDS

IMAGE_RESOURCE_DIR_STRING_U STRUCT
    Length1 dw      ?
    NameString dw   ?
IMAGE_RESOURCE_DIR_STRING_U ENDS

IMAGE_RESOURCE_DATA_ENTRY STRUCT
    OffsetToData dd ?
    Size1 dd        ?
    CodePage dd     ?
    Reserved dd     ?
IMAGE_RESOURCE_DATA_ENTRY ENDS

IMAGE_LOAD_CONFIG_DIRECTORY STRUCT
    Characteristics dd                  ?
    TimeDateStamp dd                    ?
    MajorVersion dw                     ?
    MinorVersion dw                     ?
    GlobalFlagsClear dd                 ?
    GlobalFlagsSet dd                   ?
    CriticalSectionDefaultTimeout dd    ?
    DeCommitFreeBlockThreshold dd       ?
    DeCommitTotalFreeThreshold dd       ?
    LockPrefixTable dd                  ?
    MaximumAllocationSize dd            ?
    VirtualMemoryThreshold dd           ?
    ProcessHeapFlags dd                 ?
    ProcessAffinityMask dd              ?
    CSDVersion dw                       ?
    Reserved1 dw                        ?
    EditList dd                         ?
    Reserved dd                         ?
IMAGE_LOAD_CONFIG_DIRECTORY ENDS

IMAGE_DEBUG_DIRECTORY STRUCT
    Characteristics dd  ?
    TimeDateStamp dd    ?
    MajorVersion dw     ?
    MinorVersion dw     ?
    Type1 dd            ?
    SizeOfData dd       ?
    AddressOfRawData dd ?
    PointerToRawData dd ?
IMAGE_DEBUG_DIRECTORY ENDS

IMAGE_COFF_SYMBOLS_HEADER STRUCT
    NumberOfSymbols dd      ?
    LvaToFirstSymbol dd     ?
    NumberOfLinenumbers dd  ?
    LvaToFirstLinenumber dd ?
    RvaToFirstByteOfCode dd ?
    RvaToLastByteOfCode dd  ?
    RvaToFirstByteOfData dd ?
    RvaToLastByteOfData dd  ?
IMAGE_COFF_SYMBOLS_HEADER ENDS

FPOProlog	RECORD cbFrame  : 2, reserved : 1, fUseBP   : 1, fHasSEH  : 1, cbRegs   : 3, cbProlog : 8
FPO_DATA STRUCT
    ulOffStart dd   ?
    cbProcSize dd   ?
    cdwLocals dd    ?
    cdwParams dw    ?
	 rProlog FPOProlog <>
FPO_DATA ENDS

IMAGE_DEBUG_MISC STRUCT
    DataType dd     ?
    Length1 dd      ?
    Unicode BOOLEAN ?
    Reserved db 3 dup(?)
    Data db         ?
IMAGE_DEBUG_MISC ENDS


IMAGE_FUNCTION_ENTRY STRUCT
    StartingAddress dd  ?
    EndingAddress dd    ?
    EndOfPrologue dd    ?
IMAGE_FUNCTION_ENTRY ENDS

IMAGE_SEPARATE_DEBUG_HEADER STRUCT
    Signature dw            ?
    Flags dw                ?
    Machine dw              ?
    Characteristics dw      ?
    TimeDateStamp dd        ?
    CheckSum dd             ?
    ImageBase dd            ?
    SizeOfImage dd          ?
    NumberOfSections dd     ?
    ExportedNamesSize dd    ?
    DebugDirectorySize dd   ?
    SectionAlignment dd     ?
    Reserved dd 2 dup(?)
IMAGE_SEPARATE_DEBUG_HEADER ENDS

ImportRec RECORD Reserved : 11,NameType : 3,Type2 : 2

IMPORT_OBJECT_HEADER STRUCT
    Sig1 dw     ?
    Sig2 dw     ?
    Version dw  ?
    Machine dw  ?
    TimeDateStamp dd ?
    SizeOfData dd   ?
    union 
        Ordinal dw  ?
        Hint dw     ?
    ends
    rImport ImportRec <>
IMPORT_OBJECT_HEADER ENDS

MESSAGE_RESOURCE_ENTRY STRUCT
    Length1 dw  ?
    Flags dw    ?
    Text db     ?
MESSAGE_RESOURCE_ENTRY ENDS

MESSAGE_RESOURCE_BLOCK STRUCT
    LowId dd    ?
    HighId dd   ?
    OffsetToEntries dd ?
MESSAGE_RESOURCE_BLOCK ENDS

MESSAGE_RESOURCE_DATA STRUCT
    NumberOfBlocks dd ?
    Blocks MESSAGE_RESOURCE_BLOCK <>
MESSAGE_RESOURCE_DATA ENDS

RTL_CRITICAL_SECTION_DEBUG STRUCT
    Type1 dw            ?
    CreatorBackTraceIndex dw ?
    CriticalSection dd  ?
    ProcessLocksList LIST_ENTRY <>
    EntryCount dd       ?
    ContentionCount dd  ?
    Spare dd 2 dup(?)
RTL_CRITICAL_SECTION_DEBUG ENDS

EVENTSFORLOGFILE STRUCT
    ulSize dd       ?
    szLogicalLogFile dw MAXLOGICALLOGNAMESIZE dup(?)
    ulNumRecords dd ?
	 pEventLogRecords EVENTLOGRECORD <>
EVENTSFORLOGFILE ENDS

PACKEDEVENTINFO STRUCT
    ulSize dd                   ?
    ulNumEventsForLogFile dd    ?
    ulOffsets dd                ?
PACKEDEVENTINFO ENDS

TAPE_ERASE STRUCT
    Type1 dd ?
    Immediate BOOLEAN ?
TAPE_ERASE ENDS

TAPE_PREPARE STRUCT
    Operation dd        ?
    Immediate BOOLEAN   ?
TAPE_PREPARE ENDS

TAPE_WRITE_MARKS STRUCT
    Type1 dd    ?
    Count dd    ?
    Immediate BOOLEAN ?
TAPE_WRITE_MARKS ENDS

TAPE_GET_POSITION STRUCT
    Type1 dd        ?
    Partition dd    ?
    Offset1 QWORD   ?
TAPE_GET_POSITION ENDS

TAPE_SET_POSITION STRUCT
    Method dd           ?
    Partition dd        ?
    Offset1 QWORD       ?
    Immediate BOOLEAN   ?
TAPE_SET_POSITION ENDS

TAPE_GET_DRIVE_PARAMETERS STRUCT
    ECC BOOLEAN             ?
    Compression BOOLEAN     ?
    DataPadding BOOLEAN     ?
    ReportSetmarks BOOLEAN  ?
    DefaultBlockSize dd     ?
    MaximumBlockSize dd     ?
    MinimumBlockSize dd     ?
    MaximumPartitionCount dd ?
    FeaturesLow dd          ?
    FeaturesHigh dd         ?
    EOTWarningZoneSize dd   ?
TAPE_GET_DRIVE_PARAMETERS ENDS

TAPE_SET_DRIVE_PARAMETERS STRUCT
    ECC BOOLEAN             ?
    Compression BOOLEAN     ?
    DataPadding BOOLEAN     ?
    ReportSetmarks BOOLEAN  ?
    EOTWarningZoneSize dd   ?
TAPE_SET_DRIVE_PARAMETERS ENDS

TAPE_GET_MEDIA_PARAMETERS STRUCT
    Capacity QWORD          ?
    Remaining QWORD         ?
    BlockSize dd            ?
    PartitionCount dd       ?
    WriteProtected BOOLEAN  ?
TAPE_GET_MEDIA_PARAMETERS ENDS

TAPE_SET_MEDIA_PARAMETERS STRUCT
    BlockSize dd ?
TAPE_SET_MEDIA_PARAMETERS ENDS

TAPE_CREATE_PARTITION STRUCT
    Method dd   ?
    Count dd    ?
    Size1 dd    ?
TAPE_CREATE_PARTITION ENDS

; --------------------- Imagehlp.h EQUATES --------------------- 

CHECKSUM_SUCCESS                            equ   0
CHECKSUM_OPEN_FAILURE                       equ   1
CHECKSUM_MAP_FAILURE                        equ   2
CHECKSUM_MAPVIEW_FAILURE                    equ   3
CHECKSUM_UNICODE_FAILURE                    equ   4
SPLITSYM_REMOVE_PRIVATE                     equ  000000001h
SPLITSYM_EXTRACT_ALL                        equ  000000002h
SPLITSYM_SYMBOLPATH_IS_SRC                  equ  000000004h
BIND_NO_BOUND_IMPORTS                       equ  000000001h
BIND_NO_UPDATE                              equ  000000002h
BIND_ALL_IMAGES                             equ  000000004h
BIND_CACHE_IMPORT_DLLS                      equ  000000008h

IMAGE_SEPARATION                            equ  65536

UNDNAME_COMPLETE                            equ  00000h
UNDNAME_NO_LEADING_UNDERSCORES              equ  00001h
UNDNAME_NO_MS_KEYWORDS                      equ  00002h
UNDNAME_NO_FUNCTION_RETURNS                 equ  00004h
UNDNAME_NO_ALLOCATION_MODEL                 equ  00008h
UNDNAME_NO_ALLOCATION_LANGUAGE              equ  00010h
UNDNAME_NO_MS_THISTYPE                      equ  00020h
UNDNAME_NO_CV_THISTYPE                      equ  00040h
UNDNAME_NO_THISTYPE                         equ  00060h
UNDNAME_NO_ACCESS_SPECIFIERS                equ  00080h
UNDNAME_NO_THROW_SIGNATURES                 equ  00100h
UNDNAME_NO_MEMBER_TYPE                      equ  00200h
UNDNAME_NO_RETURN_UDT_MODEL                 equ  00400h
UNDNAME_32_BIT_DECODE                       equ  00800h
UNDNAME_NAME_ONLY                           equ  01000h
UNDNAME_NO_ARGUMENTS                        equ  02000h
UNDNAME_NO_SPECIAL_SYMS                     equ  04000h
SYMF_OMAP_GENERATED                         equ  000000001h
SYMF_OMAP_MODIFIED                          equ  000000002h
CBA_DEFERRED_SYMBOL_LOAD_START              equ  000000001h
CBA_DEFERRED_SYMBOL_LOAD_COMPLETE           equ  000000002h
CBA_DEFERRED_SYMBOL_LOAD_FAILURE            equ  000000003h
CBA_SYMBOLS_UNLOADED                        equ  000000004h
CBA_DUPLICATE_SYMBOL                        equ  000000005h
SYMOPT_CASE_INSENSITIVE                     equ  000000001h
SYMOPT_UNDNAME                              equ  000000002h
SYMOPT_DEFERRED_LOADS                       equ  000000004h
SYMOPT_NO_CPP                               equ  000000008h
SYMOPT_LOAD_LINES                           equ  000000010h
SYMOPT_OMAP_FIND_NEAREST                    equ  000000020h
CERT_PE_IMAGE_DIGEST_DEBUG_INFO             equ  001h
CERT_PE_IMAGE_DIGEST_RESOURCES              equ  002h
CERT_PE_IMAGE_DIGEST_ALL_IMPORT_INFO        equ  004h
CERT_PE_IMAGE_DIGEST_NON_PE_INFO            equ  008h
CERT_SECTION_TYPE_ANY                       equ  0FFh

AddrMode1616        equ  0
AddrMode1632        equ  1
AddrModeReal        equ  2
AddrModeFlat        equ  3
SymNone             equ 0
SymCoff             equ 1
SymCv               equ 2
SymPdb              equ 3
SymExport           equ 4
SymDeferred         equ 5
SymSym              equ 6
; --------------------- Imagehlp.h STRUCTURES --------------------- 

LOADED_IMAGE STRUCT
    ModuleName                      DWORD   ?
    hFile                           DWORD   ?
    MappedAddress                   DWORD   ?
    FileHeader                      DWORD	  ?
    LastRvaSection                  DWORD	  ?
    NumberOfSections                DWORD   ?
    Sections                        DWORD	  ?
    Characteristics                 DWORD   ?
    fSystemImage                    BYTE    ?
    align dword
    fDOSImage                       BYTE    ?
    Links                           DWORD	  ?
    SizeOfImage                     DWORD   ?
LOADED_IMAGE ENDS

IMAGE_DEBUG_INFORMATION STRUCT
    List                            LIST_ENTRY <>
    Size1                           DWORD    ?
    MappedBase                      DWORD    ?
    Machine                         WORD     ?
    Characteristics                 WORD     ?
    CheckSum                        DWORD    ?
    ImageBase                       DWORD    ?
    SizeOfImage                     DWORD    ?
    NumberOfSections                DWORD    ?
    Sections                        DWORD	   ?
    ExportedNamesSize               DWORD    ?
    ExportedNames                   DWORD    ?
    NumberOfFunctionTableEntries    DWORD    ?
    FunctionTableEntries            DWORD    ?
    LowestFunctionStartingAddress   DWORD    ?
    HighestFunctionEndingAddress    DWORD    ?
    NumberOfFpoTableEntries         DWORD    ?
    FpoTableEntries                 DWORD	   ?
    SizeOfCoffSymbols               DWORD    ?
    CoffSymbols                     DWORD	   ?
    SizeOfCodeViewSymbols           DWORD    ?
    CodeViewSymbols                 DWORD    ?
    ImageFilePath                   DWORD    ?
    ImageFileName                   DWORD    ?
    DebugFilePath                   DWORD    ?
    TimeDateStamp                   DWORD    ?
    RomImage                        BYTE     ?
    DebugDirectory                  DWORD	   ?
    NumberOfDebugDirectories        DWORD    ?
    Reserved                        DWORD  3 dup (?)
IMAGE_DEBUG_INFORMATION ENDS

ADDRESS STRUCT
    Offset1  DWORD      ?
    Segment1 WORD       ?
    Mode     DWORD	?
ADDRESS ENDS

KDHELP STRUCT
    Thread                      DWORD    ?
    ThCallbackStack             DWORD    ?
    NextCallback                DWORD    ?
    FramePointer                DWORD    ?
    KiCallUserMode              DWORD    ?
    KeUserCallbackDispatcher    DWORD    ?
    SystemRangeStart            DWORD    ?
KDHELP ENDS

STACKFRAME STRUCT
    AddrPC          ADDRESS	<>
    AddrReturn      ADDRESS	<>
    AddrFrame       ADDRESS	<>
    AddrStack       ADDRESS	<>
    FuncTableEntry  DWORD ?
    Params          DWORD 4 dup (?)
    Far1            BOOL	?
    Virtual         BOOL	?
    Reserved        DWORD 3 dup (?)
    KdHelp          KDHELP	<>
STACKFRAME ENDS

API_VERSION STRUCT
    MajorVersion    WORD     ?
    MinorVersion    WORD     ?
    Revision        WORD     ?
    Reserved        WORD     ?
API_VERSION ENDS

IMAGEHLP_SYMBOL STRUCT
    SizeOfStruct    DWORD   ?
    Address         DWORD   ?
    Size1           DWORD   ?
    Flags           DWORD   ?
    MaxNameLength   DWORD   ?
    Name1           db      ?
IMAGEHLP_SYMBOL ENDS

IMAGEHLP_MODULE STRUCT
    SizeOfStruct    DWORD   ?
    BaseOfImage     DWORD   ?
    ImageSize       DWORD   ?
    TimeDateStamp   DWORD   ?
    CheckSum        DWORD   ?
    NumSyms         DWORD   ?
    SymType1        DWORD   ?
    ModuleName      BYTE  32 dup (?)
    ImageName       BYTE 256 dup (?)
    LoadedImageName BYTE 256 dup (?)
IMAGEHLP_MODULE ENDS

IMAGEHLP_LINE STRUCT
    SizeOfStruct    DWORD    ?
    Key             DWORD    ?
    LineNumber      DWORD    ?
    FileName        DWORD    ?
    Address         DWORD    ?
IMAGEHLP_LINE ENDS

IMAGEHLP_DEFERRED_SYMBOL_LOAD STRUCT
    SizeOfStruct    DWORD    ?
    BaseOfImage     DWORD    ?
    CheckSum        DWORD    ?
    TimeDateStamp   DWORD    ?
    FileName        BYTE MAX_PATH dup (?)
    Reparse         BYTE     ?
IMAGEHLP_DEFERRED_SYMBOL_LOAD ENDS

IMAGEHLP_DUPLICATE_SYMBOL STRUCT
    SizeOfStruct    DWORD   ?
    NumberOfDups    DWORD   ?
    Symbol          DWORD   ?
    SelectedSymbol  DWORD   ?
IMAGEHLP_DUPLICATE_SYMBOL ENDS

;--------------------------------------[ Excpt.h ]--------------------------
ExceptionContinueExecution              equ 0
ExceptionContinueSearch                 equ 1
ExceptionNestedException                equ 2
ExceptionCollidedUnwind                 equ 3

EXCEPTION_EXECUTE_HANDLER               equ 1
EXCEPTION_CONTINUE_SEARCH               equ  0
EXCEPTION_CONTINUE_EXECUTION            equ -1

;-----------------------------------[ Winuser.h ]-------------------------------------------
DIFFERENCE equ 11
RT_GROUP_CURSOR 	equ RT_CURSOR + DIFFERENCE
RT_GROUP_ICON 	equ RT_ICON + DIFFERENCE
RT_VERSION 		equ 16
RT_DLGINCLUDE 	equ 17
RT_PLUGPLAY 	equ 19
RT_VXD 		equ 20
RT_ANICURSOR 	equ 21
RT_ANIICON 		equ 22
RT_HTML 		equ 23
AW_HOR_POSITIVE 	equ 1h
AW_HOR_NEGATIVE 	equ 2h
AW_VER_POSITIVE 	equ 4h
AW_VER_NEGATIVE 	equ 8h
AW_CENTER 		equ 10h
AW_HIDE 		equ 10000h
AW_ACTIVATE 	equ 20000h
AW_SLIDE 		equ 40000h
AW_BLEND 		equ 80000h
VK_KANA 		equ 15h
VK_HANGEUL 		equ 15h
VK_HANGUL 		equ 15h
VK_JUNJA 		equ 17h
VK_FINAL 		equ 18h
VK_HANJA 		equ 19h
VK_KANJI 		equ 19h
VK_ESCAPE 		equ 1Bh
VK_CONVERT 		equ 1Ch
VK_NONCONVERT 	equ 1Dh
VK_ACCEPT 		equ 1Eh
VK_MODECHANGE 	equ 1Fh
VK_LWIN 		equ 5Bh
VK_RWIN 		equ 5Ch
VK_APPS 		equ 5Dh
WH_CALLWNDPROCRET equ 12
WH_KEYBOARD_LL 	equ 13
WH_MOUSE_LL 	equ 14
WH_MINHOOK 		equ WH_MIN
WH_MAXHOOK 		equ WH_MAX
MSGF_DIALOGBOX 	equ 0
MSGF_MESSAGEBOX 	equ 1
MSGF_MENU 		equ 2
MSGF_SCROLLBAR 	equ 5
MSGF_NEXTWINDOW 	equ 6
MSGF_MAX 		equ 8
MSGF_USER 		equ 4096
HSHELL_WINDOWACTIVATED 		equ 4
HSHELL_GETMINRECT 		equ 5
HSHELL_REDRAW 			equ 6
HSHELL_TASKMAN 			equ 7
HSHELL_LANGUAGE 			equ 8
HSHELL_ACCESSIBILITYSTATE 	equ 11
ACCESS_STICKYKEYS 		equ 1
ACCESS_FILTERKEYS 		equ 2
ACCESS_MOUSEKEYS 			equ 3
LLKHF_EXTENDED 			equ 1
LLKHF_INJECTED 			equ 10h
LLKHF_ALTDOWN 			equ 20h
LLKHF_UP 				equ 80h
LLMHF_INJECTED 			equ 1
KLF_REPLACELANG 			equ 10h
KLF_NOTELLSHELL 			equ 80h
KLF_SETFORPROCESS 		equ 100h
GMMP_USE_DISPLAY_POINTS 	equ 1
GMMP_USE_DRIVER_POINTS 		equ 2
DESKTOP_SWITCHDESKTOP 		equ 100h
DF_ALLOWOTHERACCOUNTHOOK 	equ 1
WSF_VISIBLE 			equ 1
UOI_FLAGS 				equ 1
UOI_NAME 				equ 2
UOI_TYPE 				equ 3
UOI_USER_SID 			equ 4
GCL_HICONSM 			equ -34
NFR_ANSI 				equ 1
NFR_UNICODE 			equ 2
NF_QUERY 				equ 3
NF_REQUERY 				equ 4
WM_MENURBUTTONUP 			equ 122h
WM_MENUDRAG equ 0123h
WM_MENUGETOBJECT 			equ 124h
WM_UNINITMENUPOPUP 		equ 125h
WM_MENUCOMMAND 			equ 126h
WM_MOUSEWHEEL 			equ 20Ah
WHEEL_DELTA 			equ 120
WHEEL_PAGESCROLL 			equ UINT_MAX
WM_NEXTMENU 			equ 213h
WM_SIZING 				equ 214h
WM_CAPTURECHANGED 		equ 215h
WM_MOVING 				equ 216h
WM_POWERBROADCAST 		equ 218h
PBT_APMQUERYSUSPEND 		equ 0
PBT_APMQUERYSTANDBY 		equ 1
PBT_APMQUERYSUSPENDFAILED 	equ 2
PBT_APMQUERYSTANDBYFAILED 	equ 3
PBT_APMSUSPEND 			equ 4
PBT_APMSTANDBY 			equ 5
PBT_APMRESUMECRITICAL 		equ 6
PBT_APMRESUMESUSPEND 		equ 7
PBT_APMRESUMESTANDBY 		equ 8
PBTF_APMRESUMEFROMFAILURE 	equ 1
PBT_APMBATTERYLOW 		equ 9
PBT_APMPOWERSTATUSCHANGE 	equ 0Ah
PBT_APMOEMEVENT 			equ 0Bh
PBT_APMRESUMEAUTOMATIC 		equ 12h
WM_DEVICECHANGE 			equ 219h
WM_ENTERSIZEMOVE 			equ 231h
WM_EXITSIZEMOVE 			equ 232h
WM_MOUSEHOVER 			equ 2A1h
WM_MOUSELEAVE 			equ 2A3h
WM_HANDHELDFIRST 			equ 358h
WM_HANDHELDLAST 			equ 35Fh
WM_AFXFIRST 			equ 360h
WM_AFXLAST 				equ 37Fh
WM_APP 				equ 8000h
WMSZ_LEFT 				equ 1
WMSZ_RIGHT 				equ 2
WMSZ_TOP 				equ 3
WMSZ_TOPLEFT 			equ 4
WMSZ_TOPRIGHT 			equ 5
WMSZ_BOTTOM 			equ 6
WMSZ_BOTTOMLEFT 			equ 7
WMSZ_BOTTOMRIGHT 			equ 8
ICON_SMALL 				equ 0
ICON_BIG 				equ 1
TME_HOVER 				equ 1
TME_LEAVE 				equ 2
TME_QUERY 				equ 40000000h
TME_CANCEL 				equ 80000000h
DC_ACTIVE 				equ 1
DC_SMALLCAP 			equ 2
DC_ICON 				equ 4
DC_TEXT 				equ 8
DC_INBUTTON 			equ 10h
DC_GRADIENT 			equ 20h
IDANI_OPEN 				equ 1
IDANI_CLOSE 			equ 2
IDANI_CAPTION 			equ 3
ENDSESSION_LOGOFF 		equ 80000000h
BSM_ALLCOMPONENTS 		equ 0
BSM_VXDS 				equ 1
BSM_NETDRIVER 			equ 2
BSM_INSTALLABLEDRIVERS 		equ 4
BSM_APPLICATIONS 			equ 8
BSM_ALLDESKTOPS 			equ 10h
BSF_QUERY 				equ 1
BSF_IGNORECURRENTTASK 		equ 2
BSF_FLUSHDISK 			equ 4
BSF_NOHANG 				equ 8
BSF_POSTMESSAGE 			equ 10h
BSF_FORCEIFHUNG 			equ 20h
BSF_NOTIMEOUTIFNOTHUNG 		equ 40h
BROADCAST_QUERY_DENY 		equ 424D5144h
DEVICE_NOTIFY_WINDOW_HANDLE 	equ 0
ISMEX_NOSEND 			equ 0
ISMEX_SEND 				equ 1h
ISMEX_NOTIFY 			equ 2h
ISMEX_CALLBACK 			equ 4h
ISMEX_REPLIED 			equ 8h
FLASHW_STOP 			equ 0
FLASHW_CAPTION 			equ 1
FLASHW_TRAY 			equ 2
FLASHW_ALL 				equ FLASHW_CAPTION + FLASHW_TRAY
FLASHW_TIMER 			equ 4
FLASHW_TIMERNOFG 			equ 0Ch
INPUT_MOUSE 			equ 0
INPUT_KEYBOARD 			equ 1
INPUT_HARDWARE 			equ 2
MWMO_WAITALL			equ 1
MWMO_ALERTABLE 			equ 2
MWMO_INPUTAVAILABLE 		equ 4
MNC_IGNORE 				equ 0
MNC_CLOSE 				equ 1
MNC_EXECUTE 			equ 2
MNC_SELECT 				equ 3
MNS_NOCHECK 			equ 80000000h
MNS_MODELESS 			equ 40000000h
MNS_DRAGDROP 			equ 20000000h
MNS_AUTODISMISS 			equ 10000000h
MNS_NOTIFYBYPOS 			equ 08000000h
MNS_CHECKORBMP 			equ 04000000h
MIM_MAXHEIGHT 			equ 1
MIM_BACKGROUND 			equ 2
MIM_HELPID 				equ 4
MIM_MENUDATA 			equ 8
MIM_STYLE 				equ 10h
MIM_APPLYTOSUBMENUS 		equ 80000000h
MND_CONTINUE 			equ 0
MND_ENDMENU 			equ 1
MNGOF_GAP 				equ 3
MNGO_NOINTERFACE 			equ 0
MNGO_NOERROR 			equ 1
MIIM_STATE 				equ 1
MIIM_ID 				equ 2
MIIM_SUBMENU 			equ 4
MIIM_CHECKMARKS 			equ 8
MIIM_TYPE 				equ 10h
MIIM_DATA 				equ 20h
MIIM_STRING 			equ 40h
MIIM_BITMAP 			equ 80h
MIIM_FTYPE 				equ 100h
HBMMENU_CALLBACK 			equ -1
HBMMENU_SYSTEM 			equ 1
HBMMENU_MBAR_RESTORE 		equ 2
HBMMENU_MBAR_MINIMIZE 		equ 3
HBMMENU_MBAR_CLOSE 		equ 5
HBMMENU_MBAR_CLOSE_D 		equ 6
HBMMENU_MBAR_MINIMIZE_D 	equ 7
HBMMENU_POPUP_CLOSE 		equ 8
HBMMENU_POPUP_RESTORE 		equ 9
HBMMENU_POPUP_MAXIMIZE 		equ 10
HBMMENU_POPUP_MINIMIZE 		equ 11
GMDI_USEDISABLED 			equ 1
GMDI_GOINTOPOPUPS 		equ 2
DOF_EXECUTABLE 			equ 8001h
DOF_DOCUMENT 			equ 8002h
DOF_DIRECTORY 			equ 8003h
DOF_MULTIPLE 			equ 8004h
DOF_PROGMAN 			equ 1
DOF_SHELLDATA 			equ 2
DO_DROPFILE 			equ 454C4946h
DO_PRINTFILE 			equ 544E5250h
DST_COMPLEX 			equ 0
DST_TEXT 				equ 1
DST_PREFIXTEXT 			equ 2
DST_ICON 				equ 3
DST_BITMAP 				equ 4
DSS_NORMAL 				equ 0
DSS_UNION 				equ 10h
DSS_DISABLED 			equ 20h
DSS_MONO 				equ 80h
DSS_RIGHT 				equ 8000h
CUDR_NORMAL 			equ 0
CUDR_NOSNAPTOGRID 		equ 1
CUDR_NORESOLVEPOSITIONS 	equ 2
CUDR_NOCLOSEGAPS 			equ 4
CUDR_NEGATIVECOORDS 		equ 8
CUDR_NOPRIMARY 			equ 10h
CWP_ALL 				equ 0
CWP_SKIPINVISIBLE 		equ 1
CWP_SKIPDISABLED 			equ 2
CWP_SKIPTRANSPARENT 		equ 4
MFT_STRING 				equ MF_STRING
MFT_BITMAP 				equ MF_BITMAP
MFT_MENUBARBREAK 			equ MF_MENUBARBREAK
MFT_MENUBREAK 			equ MF_MENUBREAK
MFT_OWNERDRAW 			equ MF_OWNERDRAW
MFT_RADIOCHECK 			equ 200h
MFT_SEPARATOR 			equ MF_SEPARATOR
MFT_RIGHTORDER 			equ 2000h
MFT_RIGHTJUSTIFY 			equ MF_RIGHTJUSTIFY
MFS_GRAYED 				equ 3
MFS_DISABLED 			equ MFS_GRAYED
MFS_CHECKED 			equ MF_CHECKED
MFS_HILITE 				equ MF_HILITE
MFS_ENABLED 			equ MF_ENABLED
MFS_UNCHECKED 			equ MF_UNCHECKED
MFS_UNHILITE 			equ MF_UNHILITE
MFS_DEFAULT 			equ MF_DEFAULT
MFS_MASK 				equ 108Bh
MFS_HOTTRACKDRAWN 		equ 10000000h
MFS_CACHEDBMP 			equ 20000000h
MFS_BOTTOMGAPDROP 		equ 40000000h
MFS_TOPGAPDROP 			equ 80000000h
MFS_GAPDROP 			equ 0C0000000h
LR_DEFAULTCOLOR 			equ 0
LR_MONOCHROME 			equ 1
LR_COLOR 				equ 2
LR_COPYRETURNORG 			equ 4
LR_COPYDELETEORG 			equ 8
LR_LOADFROMFILE 			equ 10h
LR_LOADTRANSPARENT 		equ 20h
LR_DEFAULTSIZE 			equ 40h
LR_VGACOLOR 			equ 80h
LR_LOADMAP3DCOLORS 		equ 1000h
LR_CREATEDIBSECTION 		equ 2000h
LR_COPYFROMRESOURCE 		equ 4000h
LR_SHARED 				equ 8000h
DI_MASK 				equ 1
DI_IMAGE 				equ 2
DI_NORMAL 				equ 3
DI_COMPAT 				equ 4
DI_DEFAULTSIZE 			equ 8
RES_ICON 				equ 1
RES_CURSOR 				equ 2
EC_LEFTMARGIN 			equ 1
EC_RIGHTMARGIN 			equ 2
EC_USEFONTINFO 			equ 0ffffh
EMSIS_COMPOSITIONSTRING 	equ 1
EIMES_GETCOMPSTRATONCE 		equ 1
EIMES_CANCELCOMPSTRINFOCUS 	equ 2
EIMES_COMPLETECOMPSTRKILLFOCUS equ 4
SS_OWNERDRAW 			equ 0Dh
SS_BITMAP 				equ 0Eh
SS_ENHMETAFILE 			equ 0Fh
SS_ETCHEDHORZ 			equ 10h
SS_ETCHEDVERT 			equ 11h
SS_ETCHEDFRAME 			equ 12h
SS_TYPEMASK 			equ 1Fh
SS_NOPREFIX 			equ 80h
SS_NOTIFY 				equ 100h
SS_CENTERIMAGE 			equ 200h
SS_RIGHTJUST 			equ 400h
SS_REALSIZEIMAGE 			equ 800h
SS_SUNKEN 				equ 1000h
SS_ENDELLIPSIS 			equ 4000h
SS_PATHELLIPSIS 			equ 8000h
SS_WORDELLIPSIS 			equ 0C000h
SS_ELLIPSISMASK 			equ 0C000h
STM_SETIMAGE 			equ 172h
STM_GETIMAGE 			equ 173h
STN_CLICKED 			equ 0
STN_DBLCLK 				equ 1
STN_ENABLE 				equ 2
STN_DISABLE 			equ 3
DM_REPOSITION 			equ WM_USER+2
LB_INITSTORAGE 			equ 1A8h
LB_ITEMFROMPOINT 			equ 1A9h
LBS_NOSEL 				equ 4000h
CBS_UPPERCASE 			equ 2000h
CBS_LOWERCASE 			equ 4000h
SBM_SETSCROLLINFO 		equ 0E9h
SBM_GETSCROLLINFO 		equ 0EAh
GR_GDIOBJECTS 			equ 0
GR_USEROBJECTS 			equ 1
SPI_SETSCREENSAVERRUNNING 	equ 97
SPI_GETMOUSEHOVERWIDTH 		equ 98
SPI_SETMOUSEHOVERWIDTH 		equ 99
SPI_GETMOUSEHOVERHEIGHT 	equ 100
SPI_SETMOUSEHOVERHEIGHT 	equ 101
SPI_GETMOUSEHOVERTIME 		equ 102
SPI_SETMOUSEHOVERTIME 		equ 103
SPI_GETWHEELSCROLLLINES 	equ 104
SPI_SETWHEELSCROLLLINES 	equ 105
SPI_GETSHOWIMEUI 			equ 110
SPI_SETSHOWIMEUI 			equ 111
SPI_GETMOUSESPEED 		equ 112
SPI_SETMOUSESPEED 		equ 113
SPI_GETSCREENSAVERRUNNING 	equ 114
SPI_GETACTIVEWINDOWTRACKING 	equ 1000h
SPI_SETACTIVEWINDOWTRACKING 	equ 1001h
SPI_GETMENUANIMATION 		equ 1002h
SPI_SETMENUANIMATION 		equ 1003h
SPI_GETCOMBOBOXANIMATION 	equ 1004h
SPI_SETCOMBOBOXANIMATION 	equ 1005h
SPI_GETLISTBOXSMOOTHSCROLLING equ 1006h
SPI_SETLISTBOXSMOOTHSCROLLING equ 1007h
SPI_GETGRADIENTCAPTIONS 	equ 1008h
SPI_SETGRADIENTCAPTIONS 	equ 1009h
SPI_GETMENUUNDERLINES 		equ 100Ah
SPI_SETMENUUNDERLINES 		equ 100Bh
SPI_GETACTIVEWNDTRKZORDER 	equ 100Ch
SPI_SETACTIVEWNDTRKZORDER 	equ 100Dh
SPI_GETHOTTRACKING 		equ 100Eh
SPI_SETHOTTRACKING 		equ 100Fh
SPI_GETFOREGROUNDLOCKTIMEOUT 	equ 2000h
SPI_SETFOREGROUNDLOCKTIMEOUT 	equ 2001h
SPI_GETACTIVEWNDTRKTIMEOUT 	equ 2002h
SPI_SETACTIVEWNDTRKTIMEOUT 	equ 2003h
SPI_GETFOREGROUNDFLASHCOUNT 	equ 2004h
SPI_SETFOREGROUNDFLASHCOUNT 	equ 2005h
SPIF_SENDCHANGE 			equ SPIF_SENDWININICHANGE
METRICS_USEDEFAULT 		equ -1
ARW_BOTTOMLEFT 			equ 0
ARW_BOTTOMRIGHT 			equ 1
ARW_TOPLEFT 			equ 2
ARW_TOPRIGHT 			equ 3
ARW_STARTMASK 			equ 3
ARW_STARTRIGHT 			equ 1
ARW_STARTTOP 			equ 2
ARW_LEFT 				equ 0
ARW_RIGHT 				equ 0
ARW_UP 				equ 4
ARW_DOWN 				equ 4
ARW_HIDE 				equ 8
SERKF_SERIALKEYSON 		equ 1
SERKF_AVAILABLE 			equ 2
SERKF_INDICATOR 			equ 4
HCF_HIGHCONTRASTON 		equ 1
HCF_AVAILABLE 			equ 2
HCF_HOTKEYACTIVE 			equ 4
HCF_CONFIRMHOTKEY 		equ 8
HCF_HOTKEYSOUND 			equ 10h
HCF_INDICATOR 			equ 20h
HCF_HOTKEYAVAILABLE 		equ 40h
CDS_UPDATEREGISTRY 		equ 1
CDS_TEST 				equ 2
CDS_FULLSCREEN 			equ 4
CDS_GLOBAL 				equ 8
CDS_SET_PRIMARY 			equ 10h
CDS_RESET 				equ 40000000h
CDS_SETRECT 			equ 20000000h
CDS_NORESET 			equ 10000000h
DISP_CHANGE_SUCCESSFUL 		equ 0
DISP_CHANGE_RESTART 		equ 1
DISP_CHANGE_FAILED 		equ -1
DISP_CHANGE_BADMODE 		equ -2
DISP_CHANGE_NOTUPDATED 		equ -3
DISP_CHANGE_BADFLAGS 		equ -4
DISP_CHANGE_BADPARAM 		equ -5
ENUM_CURRENT_SETTINGS 		equ -1
ENUM_REGISTRY_SETTINGS 		equ -2
FKF_FILTERKEYSON 			equ 1
FKF_AVAILABLE 			equ 2
FKF_HOTKEYACTIVE 			equ 4
FKF_CONFIRMHOTKEY 		equ 8
FKF_HOTKEYSOUND 			equ 10h
FKF_INDICATOR 			equ 20h
FKF_CLICKON 			equ 40h
SKF_STICKYKEYSON 			equ 1
SKF_AVAILABLE 			equ 2
SKF_HOTKEYACTIVE 			equ 4
SKF_CONFIRMHOTKEY 		equ 8
SKF_HOTKEYSOUND 			equ 10h
SKF_INDICATOR 			equ 20h
SKF_AUDIBLEFEEDBACK 		equ 40h
SKF_TRISTATE 			equ 80h
SKF_TWOKEYSOFF 			equ 100h
SKF_LALTLATCHED 			equ 10000000h

SKF_LCTLLATCHED 			equ 4000000h
SKF_LSHIFTLATCHED 		equ 1000000h
SKF_RALTLATCHED 			equ 20000000h
SKF_RCTLLATCHED 			equ 08000000h
SKF_RSHIFTLATCHED 		equ 2000000h
SKF_LWINLATCHED 			equ 40000000h
SKF_RWINLATCHED 			equ 80000000h
SKF_LALTLOCKED 			equ 100000h
SKF_LCTLLOCKED 			equ 40000h
SKF_LSHIFTLOCKED 			equ 10000h
SKF_RALTLOCKED 			equ 200000h
SKF_RCTLLOCKED 			equ 80000h
SKF_RSHIFTLOCKED 			equ 20000h
SKF_LWINLOCKED 			equ 400000h
SKF_RWINLOCKED 			equ 800000h
MKF_MOUSEKEYSON 			equ 1
MKF_AVAILABLE 			equ 2
MKF_HOTKEYACTIVE 			equ 4
MKF_CONFIRMHOTKEY 		equ 8
MKF_HOTKEYSOUND 			equ 10h
MKF_INDICATOR 			equ 20h
MKF_MODIFIERS 			equ 40h
MKF_REPLACENUMBERS 		equ 80h
MKF_LEFTBUTTONSEL 		equ 10000000h
MKF_RIGHTBUTTONSEL 		equ 20000000h
MKF_LEFTBUTTONDOWN 		equ 1000000h
MKF_RIGHTBUTTONDOWN 		equ 2000000h
MKF_MOUSEMODE 			equ 80000000h
ATF_TIMEOUTON 			equ 1
ATF_ONOFFFEEDBACK 		equ 2
SSGF_NONE 				equ 0
SSGF_DISPLAY 			equ 3
SSTF_NONE 				equ 0
SSTF_CHARS 				equ 1
SSTF_BORDER 			equ 2
SSTF_DISPLAY 			equ 3
SSWF_NONE 				equ 0
SSWF_TITLE 				equ 1
SSWF_WINDOW 			equ 2
SSWF_DISPLAY 			equ 3
SSWF_CUSTOM 			equ 4
SSF_SOUNDSENTRYON 		equ 1
SSF_AVAILABLE 			equ 2
SSF_INDICATOR 			equ 4
TKF_TOGGLEKEYSON 			equ 1
TKF_AVAILABLE 			equ 2
TKF_HOTKEYACTIVE 			equ 4
TKF_CONFIRMHOTKEY 		equ 8
TKF_HOTKEYSOUND 			equ 10h
TKF_INDICATOR 			equ 20h
MONITOR_DEFAULTTONULL 		equ 0
MONITOR_DEFAULTTOPRIMARY 	equ 1
MONITOR_DEFAULTTONEAREST 	equ 2
MONITORINFOF_PRIMARY 		equ 1
CCHDEVICENAME 			equ 32
CHILDID_SELF 			equ 0
INDEXID_OBJECT 			equ 0
INDEXID_CONTAINER 		equ 0
OBJID_WINDOW 			equ 0
OBJID_SYSMENU 			equ 0FFFFFFFFh
OBJID_TITLEBAR 			equ 0FFFFFFFEh
OBJID_MENU 				equ 0FFFFFFFDh
OBJID_CLIENT 			equ 0FFFFFFFCh
OBJID_VSCROLL 			equ 0FFFFFFFBh
OBJID_HSCROLL 			equ 0FFFFFFFAh
OBJID_SIZEGRIP 			equ 0FFFFFFF9h
OBJID_CARET 			equ 0FFFFFFF8h
OBJID_CURSOR 			equ 0FFFFFFF7h
OBJID_ALERT 			equ 0FFFFFFF6h
OBJID_SOUND 			equ 0FFFFFFF5h
EVENT_MIN 				equ 1
EVENT_MAX 				equ 7FFFFFFFh
EVENT_SYSTEM_SOUND 		equ 1
EVENT_SYSTEM_ALERT 		equ 2
EVENT_SYSTEM_FOREGROUND 	equ 3
EVENT_SYSTEM_MENUSTART 		equ 4
EVENT_SYSTEM_MENUEND 		equ 5
EVENT_SYSTEM_MENUPOPUPSTART 	equ 6
EVENT_SYSTEM_MENUPOPUPEND 	equ 7
EVENT_SYSTEM_CAPTURESTART 	equ 8
EVENT_SYSTEM_CAPTUREEND 	equ 9
EVENT_SYSTEM_MOVESIZESTART 	equ 0Ah
EVENT_SYSTEM_MOVESIZEEND 	equ 0Bh
EVENT_SYSTEM_CONTEXTHELPSTART equ 0Ch
EVENT_SYSTEM_CONTEXTHELPEND 	equ 0Dh
EVENT_SYSTEM_DRAGDROPSTART 	equ 0Eh
EVENT_SYSTEM_DRAGDROPEND 	equ 0Fh
EVENT_SYSTEM_DIALOGSTART 	equ 10h
EVENT_SYSTEM_DIALOGEND 		equ 11h
EVENT_SYSTEM_SCROLLINGSTART 	equ 12h
EVENT_SYSTEM_SCROLLINGEND 	equ 13h
EVENT_SYSTEM_SWITCHSTART 	equ 14h
EVENT_SYSTEM_SWITCHEND 		equ 15h
EVENT_SYSTEM_MINIMIZESTART 	equ 16h
EVENT_SYSTEM_MINIMIZEEND 	equ 17h
EVENT_OBJECT_CREATE 		equ 8000h
EVENT_OBJECT_DESTROY 		equ 8001h
EVENT_OBJECT_SHOW 		equ 8002h
EVENT_OBJECT_HIDE 		equ 8003h
EVENT_OBJECT_REORDER 		equ 8004h
EVENT_OBJECT_FOCUS 		equ 8005h
EVENT_OBJECT_SELECTION 		equ 8006h
EVENT_OBJECT_SELECTIONADD 	equ 8007h
EVENT_OBJECT_SELECTIONREMOVE 	equ 8008h
EVENT_OBJECT_SELECTIONWITHIN 	equ 8009h
EVENT_OBJECT_STATECHANGE 	equ 800Ah
EVENT_OBJECT_LOCATIONCHANGE 	equ 800Bh
EVENT_OBJECT_NAMECHANGE 	equ 800Ch
EVENT_OBJECT_DESCRIPTIONCHANGE equ 800Dh
EVENT_OBJECT_VALUECHANGE 	equ 800Eh
EVENT_OBJECT_PARENTCHANGE 	equ 800Fh
EVENT_OBJECT_HELPCHANGE 	equ 8010h
EVENT_OBJECT_DEFACTIONCHANGE 	equ 8011h
EVENT_OBJECT_ACCELERATORCHANGE equ 8012h
SOUND_SYSTEM_STARTUP 		equ 1
SOUND_SYSTEM_SHUTDOWN 		equ 2
SOUND_SYSTEM_BEEP 		equ 3
SOUND_SYSTEM_ERROR 		equ 4
SOUND_SYSTEM_QUESTION 		equ 5
SOUND_SYSTEM_WARNING 		equ 6
SOUND_SYSTEM_INFORMATION 	equ 7
SOUND_SYSTEM_MAXIMIZE 		equ 8
SOUND_SYSTEM_MINIMIZE 		equ 9
SOUND_SYSTEM_RESTOREUP 		equ 10
SOUND_SYSTEM_RESTOREDOWN 	equ 11
SOUND_SYSTEM_APPSTART 		equ 12
SOUND_SYSTEM_FAULT 		equ 13
SOUND_SYSTEM_APPEND 		equ 14
SOUND_SYSTEM_MENUCOMMAND 	equ 15
SOUND_SYSTEM_MENUPOPUP 		equ 16
CSOUND_SYSTEM 			equ 16
ALERT_SYSTEM_INFORMATIONAL 	equ 1
ALERT_SYSTEM_WARNING 		equ 2
ALERT_SYSTEM_ERROR 		equ 3
ALERT_SYSTEM_QUERY 		equ 4
ALERT_SYSTEM_CRITICAL 		equ 5
CALERT_SYSTEM 			equ 6
WINEVENT_OUTOFCONTEXT 		equ 0
WINEVENT_SKIPOWNTHREAD 		equ 1
WINEVENT_SKIPOWNPROCESS 	equ 2
WINEVENT_INCONTEXT 		equ 4
GUI_CARETBLINKING 		equ 1
GUI_INMOVESIZE 			equ 2
GUI_INMENUMODE 			equ 4
GUI_SYSTEMMENUMODE 		equ 8
GUI_POPUPMENUMODE 		equ 10h
STATE_SYSTEM_UNAVAILABLE 	equ 1
STATE_SYSTEM_SELECTED 		equ 2
STATE_SYSTEM_FOCUSED 		equ 4
STATE_SYSTEM_PRESSED 		equ 8
STATE_SYSTEM_CHECKED 		equ 10h
STATE_SYSTEM_MIXED 		equ 20h
STATE_SYSTEM_READONLY 		equ 40h
STATE_SYSTEM_HOTTRACKED 	equ 80h
STATE_SYSTEM_DEFAULT 		equ 100h
STATE_SYSTEM_EXPANDED 		equ 200h
STATE_SYSTEM_COLLAPSED 		equ 400h
STATE_SYSTEM_BUSY 		equ 800h
STATE_SYSTEM_FLOATING 		equ 1000h
STATE_SYSTEM_MARQUEED 		equ 2000h
STATE_SYSTEM_ANIMATED 		equ 4000h
STATE_SYSTEM_INVISIBLE 		equ 8000h
STATE_SYSTEM_OFFSCREEN 		equ 10000h
STATE_SYSTEM_SIZEABLE 		equ 20000h
STATE_SYSTEM_MOVEABLE 		equ 40000h
STATE_SYSTEM_SELFVOICING 	equ 80000h
STATE_SYSTEM_FOCUSABLE 		equ 100000h
STATE_SYSTEM_SELECTABLE 	equ 200000h
STATE_SYSTEM_LINKED 		equ 400000h
STATE_SYSTEM_TRAVERSED 		equ 800000h
STATE_SYSTEM_MULTISELECTABLE 	equ 1000000h
STATE_SYSTEM_EXTSELECTABLE 	equ 2000000h
STATE_SYSTEM_ALERT_LOW 		equ 4000000h
STATE_SYSTEM_ALERT_MEDIUM 	equ 8000000h
STATE_SYSTEM_ALERT_HIGH 	equ 10000000h
STATE_SYSTEM_VALID 		equ 1FFFFFFFh
CCHILDREN_TITLEBAR 		equ 5
CCHILDREN_SCROLLBAR 		equ 5
GA_MIC 				equ 1
GA_PARENT 				equ 1
GA_ROOT 				equ 2
GA_ROOTOWNER 			equ 3
GA_MAC 				equ 4
CURSOR_SHOWING     		equ 1
WS_ACTIVECAPTION    		equ 1

;-----------------------------------[ winuser.h - Structures ]---------------------------------
CWPRETSTRUCT STRUCT
    lResult DWORD   ?
    lParam DWORD    ?
    wParam DWORD    ?
    message DWORD   ?
    hwnd DWORD      ?
CWPRETSTRUCT ENDS

HARDWAREHOOKSTRUCT STRUCT
    hwnd DWORD      ?
    message DWORD   ?
    wParam DWORD    ?
    lParam DWORD    ?
HARDWAREHOOKSTRUCT ENDS

USEROBJECTFLAGS STRUCT
    fInherit DWORD  ?
    fReserved DWORD ?
    dwFlags DWORD   ?
USEROBJECTFLAGS ENDS

MDINEXTMENU STRUCT
    hmenuIn DWORD   ?
    hmenuNext DWORD ?
    hwndNext DWORD  ?
MDINEXTMENU ENDS

NCCALCSIZE_PARAMS STRUCT
    rgrc RECT 3 dup(<>)
    lppos DWORD     ?
NCCALCSIZE_PARAMS ENDS

STYLESTRUCT STRUCT
    styleOld DWORD ?
    styleNew DWORD ?
STYLESTRUCT ENDS

DROPSTRUCT STRUCT
    hwndSource DWORD    ?
    hwndSink DWORD      ?
    wFmt DWORD          ?
    dwData DWORD        ?
    ptDrop POINT        <>
    dwControlData DWORD ?
DROPSTRUCT ENDS

CURSORSHAPE STRUCT
    xHotSpot DWORD  ?
    yHotSpot DWORD  ?
    regCx DWORD     ?
    regCy DWORD     ?
    cbWidth DWORD   ?
    Planes db       ?
    BitsPixel db    ?
CURSORSHAPE ENDS

SERIALKEYSA STRUCT
    cbSize DWORD            ?
    dwFlags DWORD           ?
    lpszActivePort DWORD    ?
    lpszPort DWORD          ?
    iBaudRate DWORD         ?
    iPortState DWORD        ?
    iActive DWORD           ?
SERIALKEYSA ENDS

SERIALKEYS EQU <SERIALKEYSA>

HIGHCONTRASTA STRUCT
    cbSize DWORD            ?
    dwFlags DWORD           ?
    lpszDefaultScheme DWORD ?
HIGHCONTRASTA ENDS

HIGHCONTRAST EQU <HIGHCONTRASTA>

FILTERKEYS STRUCT
    cbSize DWORD        ?
    dwFlags DWORD       ?
    iWaitMSec DWORD     ?
    iDelayMSec DWORD    ?
    iRepeatMSec DWORD   ?
    iBounceMSec DWORD   ?
FILTERKEYS ENDS

STICKYKEYS STRUCT
    cbSize DWORD    ?
    dwFlags DWORD   ?
STICKYKEYS ENDS

MOUSEKEYS STRUCT
    cbSize DWORD            ?
    dwFlags DWORD           ?
    iMaxSpeed DWORD         ?
    iTimeToMaxSpeed DWORD   ?
    iCtrlSpeed DWORD        ?
    dwReserved1 DWORD       ?
    dwReserved2 DWORD       ?
MOUSEKEYS ENDS

ACCESSTIMEOUT STRUCT
    cbSize DWORD        ?
    dwFlags DWORD       ?
    iTimeOutMSec DWORD  ?
ACCESSTIMEOUT ENDS

SOUNDSENTRYA STRUCT
    cbSize DWORD                    ?
    dwFlags DWORD                   ?
    iFSTextEffect DWORD             ?
    iFSTextEffectMSec DWORD         ?
    iFSTextEffectColorBits DWORD    ?
    iFSGrafEffect DWORD             ?
    iFSGrafEffectMSec DWORD         ?
    iFSGrafEffectColor DWORD        ?
    iWindowsEffect DWORD            ?
    iWindowsEffectMSec DWORD        ?
    lpszWindowsEffectDLL DWORD      ?
    iWindowsEffectOrdinal DWORD     ?
SOUNDSENTRYA ENDS

SOUNDSENTRY EQU <SOUNDSENTRYA>

TOGGLEKEYS STRUCT
    cbSize DWORD ?
    dwFlags DWORD ?
TOGGLEKEYS ENDS

;-------------------------------------[ wtypes.h ]------------------------------
IID	EQU <GUID>
CLSID EQU <GUID>
MEMCTX_TASK		equ 1
MEMCTX_SHARED	equ 2
MEMCTX_MACSYSTEM	equ 3
MEMCTX_UNKNOWN	equ -1
MEMCTX_SAME		equ -2
ROTFLAGS_REGISTRATIONKEEPSALIVE	equ 1
ROTFLAGS_ALLOWANYCLIENT			equ 2
ROT_COMPARE_MAX 				equ 2048
CLSCTX_INPROC_SERVER		equ 1
CLSCTX_INPROC_HANDLER		equ 2
CLSCTX_LOCAL_SERVER		equ 4
CLSCTX_INPROC_SERVER16		equ 8
CLSCTX_REMOTE_SERVER		equ 10h
CLSCTX_INPROC_HANDLER16		equ 20h
CLSCTX_INPROC_SERVERX86		equ 40h
CLSCTX_INPROC_HANDLERX86	equ 80h
CLSCTX_ESERVER_HANDLER		equ 100h
MSHLFLAGS_NORMAL		equ 0
MSHLFLAGS_TABLESTRONG	equ 1
MSHLFLAGS_TABLEWEAK	equ 2
MSHLFLAGS_NOPING		equ 4
MSHCTX_LOCAL		equ 0
MSHCTX_NOSHAREDMEM	equ 1
MSHCTX_DIFFERENTMACHINE	equ 2
MSHCTX_INPROC		equ 3
DVASPECT_CONTENT		equ 1
DVASPECT_THUMBNAIL	equ 2
DVASPECT_ICON		equ 4
DVASPECT_DOCPRINT		equ 8
STGC_DEFAULT		equ 0
STGC_OVERWRITE		equ 1
STGC_ONLYIFCURRENT	equ 2
STGC_DANGEROUSLYCOMMITMERELYTODISKCACHE	equ 4
STGC_CONSOLIDATE		equ 8
STGMOVE_MOVE		equ 0
STGMOVE_COPY		equ 1
STGMOVE_SHALLOWCOPY	equ 2
STATFLAG_DEFAULT		equ 0
STATFLAG_NONAME		equ 1
STATFLAG_NOOPEN		equ 2
WDT_INPROC_CALL     equ 48746457h
WDT_REMOTE_CALL     equ 52746457h
VARIANT_TRUE        equ 0ffffh
VARIANT_FALSE       equ 0
DECIMAL_NEG         equ 80
APPINFO_PUBLISHED   equ 1
APPINFO_SHELLNEW    equ APPINFO_PUBLISHED + 1
APPINFO_INSERTABLE  equ APPINFO_SHELLNEW + 1
ACTFLG_RunLocally   equ 1
ACTFLG_RunOnce      equ 2 
ACTFLG_SystemWide   equ 4
ACTFLG_Published    equ 8
ACTFLG_Assigned     equ 16
ACTFLG_UserInstall  equ 32
ExeNamePath         equ 0
DllNamePath         equ ExeNamePath + 1
TlbNamePath         equ DllNamePath + 1
CabFilePath         equ TlbNamePath + 1
InfFilePath         equ CabFilePath + 1
DrwFilePath         equ InfFilePath + 1
SetupNamePath       equ DrwFilePath + 1
TYSPEC_CLSID        equ 0
TYSPEC_FILEEXT      equ TYSPEC_CLSID + 1
TYSPEC_MIMETYPE     equ TYSPEC_FILEEXT + 1
TYSPEC_PROGID       equ TYSPEC_MIMETYPE + 1
TYSPEC_IID          equ TYSPEC_PROGID + 1
TYSPEC_TYPELIB      equ TYSPEC_IID + 1
TYSPEC_FILENAME     equ TYSPEC_TYPELIB + 1
TYSPEC_JAVACLASS    equ TYSPEC_FILENAME + 1
TYSPEC_PACKAGENAME  equ TYSPEC_JAVACLASS + 1
VT_EMPTY            equ 0
VT_NULL             equ 1
VT_I2               equ 2
VT_I4               equ 3
VT_R4               equ 4
VT_R8               equ 5
VT_CY               equ 6
VT_DATE             equ 7
VT_BSTR             equ 8
VT_DISPATCH         equ 9
VT_ERROR            equ 10
VT_BOOL             equ 11
VT_VARIANT          equ 12
VT_UNKNOWN          equ 13
VT_DECIMAL          equ 14
VT_I1               equ 16
VT_UI1              equ 17
VT_UI2              equ 18
VT_UI4              equ 19
VT_I8               equ 20
VT_UI8              equ 21
VT_INT              equ 22
VT_UINT             equ 23
VT_VOID             equ 24
VT_HRESULT          equ 25
VT_PTR              equ 26
VT_SAFEARRAY        equ 27
VT_CARRAY           equ 28
VT_USERDEFINED      equ 29
VT_LPSTR            equ 30
VT_LPWSTR           equ 31
VT_RECORD           equ 36
VT_FILETIME         equ 64
VT_BLOB             equ 65
VT_STREAM           equ 66
VT_STORAGE          equ 67
VT_STREAMED_OBJECT  equ 68
VT_STORED_OBJECT    equ 69
VT_BLOB_OBJECT      equ 70
VT_CF               equ 71
VT_CLSID            equ 72
VT_BSTR_BLOB        equ 0fffh
VT_VECTOR           equ 1000h
VT_ARRAY            equ 2000h
VT_BYREF            equ 4000h
VT_RESERVED         equ 8000h
VT_ILLEGAL          equ 0ffffh
VT_ILLEGALMASKED    equ 0fffh
VT_TYPEMASK         equ 0fffh

RemHGLOBAL STRUCT
    fNullHGlobal DWORD  ?
    cbData DWORD        ?
    data db             ?
RemHGLOBAL ENDS

RemHMETAFILEPICT STRUCT
    imm DWORD       ?
    xExt DWORD      ?
    yExt DWORD      ?
    cbData DWORD    ?
    data db         ?
RemHMETAFILEPICT ENDS

RemHENHMETAFILE STRUCT
    cbData DWORD    ?
    data db         ?
RemHENHMETAFILE ENDS

RemHBITMAP STRUCT
    cbData DWORD    ?
    data db         ?
RemHBITMAP ENDS

RemHPALETTE STRUCT
    cbData DWORD    ?
    data db         ?
RemHPALETTE ENDS

RemHBRUSH STRUCT
    cbData DWORD    ?
    data db         ?
RemHBRUSH ENDS

COAUTHIDENTITY STRUCT
    User DWORD              ?
    UserLength DWORD        ?
    Domain DWORD            ?
    DomainLength DWORD      ?
    Password DWORD          ?
    PasswordLength DWORD    ?
    Flags DWORD             ?
COAUTHIDENTITY ENDS

COAUTHINFO STRUCT
    dwAuthnSvc DWORD            ?
    dwAuthzSvc DWORD            ?
    pwszServerPrincName DWORD   ?
    dwAuthnLevel DWORD          ?
    dwImpersonationLevel DWORD  ?
    pAuthIdentityData DWORD     ?
    dwCapabilities DWORD        ?
COAUTHINFO ENDS

BYTE_BLOB STRUCT
    clSize DWORD    ?
    abData db       ?
BYTE_BLOB ENDS

WORD_BLOB STRUCT
    clSize DWORD    ?
    asData dw       ?
WORD_BLOB ENDS

DWORD_BLOB STRUCT
    clSize DWORD    ?
    alData DWORD    ?
DWORD_BLOB ENDS

FLAGGED_BYTE_BLOB STRUCT
    fFlags DWORD    ?
    clSize DWORD    ?
    abData db       ?
FLAGGED_BYTE_BLOB ENDS

FLAGGED_WORD_BLOB STRUCT
    fFlags DWORD ?
    clSize DWORD ?
    asData dw ?
FLAGGED_WORD_BLOB ENDS

BYTE_SIZEDARR STRUCT
    clSize DWORD    ?
    pData DWORD     ?
BYTE_SIZEDARR ENDS

WORD_SIZEDARR STRUCT
    clSize DWORD    ?
    pData DWORD     ?
WORD_SIZEDARR ENDS

DWORD_SIZEDARR STRUCT
    clSize DWORD        ?
    pData DWORD         ?
DWORD_SIZEDARR ENDS

HYPER_SIZEDARR STRUCT
    clSize DWORD        ?
    pData DWORD         ?
HYPER_SIZEDARR ENDS

userCLIPFORMAT STRUCT
    fContext DWORD      ?
    union u
        dwValue DWORD   ?
        pwszName DWORD  ?
    ends
userCLIPFORMAT ENDS

GDI_NONREMOTE STRUCT
    fContext DWORD      ?
    union u
        hInproc DWORD   ?
        hRemote DWORD   ?
    ends
GDI_NONREMOTE ENDS

userHGLOBAL STRUCT
    fContext DWORD      ?
    union u
        hInproc DWORD   ?
        hRemote DWORD   ?
        hGlobal DWORD   ?
    ends
userHGLOBAL ENDS

userHMETAFILE STRUCT
    fContext DWORD      ?
    union u
        hInproc DWORD   ?
        hRemote DWORD   ?
        hGlobal DWORD   ?
    ends
userHMETAFILE ENDS

remoteMETAFILEPICT STRUCT
    imm DWORD   ?
    xExt DWORD  ?
    yExt DWORD  ?
    hMF DWORD   ?
remoteMETAFILEPICT ENDS

userHMETAFILEPICT STRUCT
    fContext DWORD      ?
    union u
        hInproc DWORD   ?
        hRemote DWORD   ?
        hGlobal DWORD   ?
    ends
userHMETAFILEPICT ENDS

userHENHMETAFILE STRUCT
    fContext DWORD      ?
    union u
        hInproc DWORD   ?
        hRemote DWORD   ?
        hGlobal DWORD   ?
    ends
userHENHMETAFILE ENDS

userBITMAP STRUCT
    bmType DWORD        ?
    bmWidth DWORD       ?
    bmHeight DWORD      ?
    bmWidthBytes DWORD  ?
    bmPlanes dw         ?
    bmBitsPixel dw      ?
    cbSize DWORD        ?
    pBuffer db          ?
userBITMAP ENDS

userHBITMAP STRUCT
    fContext DWORD ?
    union u
        hInproc dd ?
        hRemote dd ?
        hGlobal dd ?
    ends
userHBITMAP ENDS

rpcLOGPALETTE STRUCT
    palVersion dw       ?
    palNumEntries dw    ?
    palPalEntry PALETTEENTRY <>
rpcLOGPALETTE ENDS

userHPALETTE STRUCT
    fContext dd ?
    union u
        hInproc dd ?
        hRemote dd ?
        hGlobal dd ?
    ends
userHPALETTE ENDS

RemotableHandle STRUCT
    fContext dd ?
    union u
        hInproc dd ?
        hRemote dd ?
    ends
RemotableHandle ENDS

BSTRBLOB STRUCT
    cbSize dd   ?
    pData dd    ?
BSTRBLOB ENDS

BLOB STRUCT
    cbSize dd       ?
    pBlobData dd    ?
BLOB ENDS

CLIPDATA STRUCT
    cbSize dd       ?
    ulClipFmt dd    ?
    pClipData dd    ?
CLIPDATA ENDS

CSPLATFORM STRUCT
    dwPlatformId dd     ?
    dwVersionHi dd      ?
    dwVersionLo dd      ?
    dwProcessorArch dd  ?
CSPLATFORM ENDS

QUERYCONTEXT STRUCT
    dwContext dd    ?
    Platform CSPLATFORM <>
    Locale dd       ?
    dwVersionHi dd  ?
    dwVersionLo dd  ?
QUERYCONTEXT ENDS

uCLSSPEC STRUCT
    tyspec dd ?
    union tagged_union
        clsid CLSID         <>
        iid IID             <>
        typelibID GUID      <>
        pFileExt dd         ?
        pMimeType dd        ?
        pProgId dd          ?
        pFileName dd        ?
        pJavaClassName dd   ?
        pPackageName dd     ?
    ends
uCLSSPEC ENDS

PUBLISHEDAPPINFO STRUCT
    pwszFileExtension dd    ?
    pwszDisplayName dd      ?
    pwszPackagePath dd      ?
PUBLISHEDAPPINFO ENDS

APPDETAIL STRUCT
    AppID GUID          <>
    cClasses dd         ?
    prgClsIdList dd     ?
    cTypeLibIds dd      ?
    prgTypeLibIdList dd ?
    cServers dd         ?
    prgServerNames dd   ?
APPDETAIL ENDS

;-------------------------------------------[ mmsystem.h ]--------------------------------
MM_DRVM_OPEN 		equ 3D0h
MM_DRVM_CLOSE 		equ 3D1h
MM_DRVM_DATA 		equ 3D2h
MM_DRVM_ERROR 		equ 3D3h
MM_STREAM_OPEN	 	equ 3D4h
MM_STREAM_CLOSE	 	equ 3D5h
MM_STREAM_DONE	 	equ 3D6h
MM_STREAM_ERROR	 	equ 3D7h
MM_MIXM_LINE_CHANGE 	equ 3D0h
MM_MIXM_CONTROL_CHANGE 	equ 3D1h
MMSYSERR_BADDB 		equ (MMSYSERR_BASE + 14)
MMSYSERR_KEYNOTFOUND 	equ (MMSYSERR_BASE + 15)
MMSYSERR_READERROR 	equ (MMSYSERR_BASE + 16)
MMSYSERR_WRITEERROR 	equ (MMSYSERR_BASE + 17)
MMSYSERR_DELETEERROR 	equ (MMSYSERR_BASE + 18)
MMSYSERR_VALNOTFOUND 	equ (MMSYSERR_BASE + 19)
MMSYSERR_NODRIVERCB 	equ (MMSYSERR_BASE + 20)
CALLBACK_THREAD 		equ (CALLBACK_TASK)
CALLBACK_EVENT 		equ 50000h
SND_PURGE 			equ 40h
SND_APPLICATION 		equ 80h
WAVECAPS_SAMPLEACCURATE equ 20h
WAVECAPS_DIRECTSOUND 	equ 40h
MIDIERR_BADOPENMODE 	equ (MIDIERR_BASE + 6)
MIDIERR_DONT_CONTINUE 	equ (MIDIERR_BASE + 7)
MIDIPATCHSIZE 		equ 128
MEVT_SHORTMSG 		equ 0
MEVT_TEMPO 			equ 1
MEVT_NOP 			equ 2
MEVT_LONGMSG 		equ 80h
MEVT_COMMENT 		equ 82h
MEVT_VERSION 		equ 84h
MMIOERR_PATHNOTFOUND 	equ (MMIOERR_BASE + 11)
MMIOERR_ACCESSDENIED 	equ (MMIOERR_BASE + 12)
MMIOERR_SHARINGVIOLATION equ (MMIOERR_BASE + 13)
MMIOERR_NETWORKERROR 	equ (MMIOERR_BASE + 14)
MMIOERR_TOOMANYOPENFILES equ (MMIOERR_BASE + 15)
MMIOERR_INVALIDFILE 	equ (MMIOERR_BASE + 16)
CFSEPCHAR 			equ '+'
MCIERR_NO_IDENTITY 	equ (MCIERR_BASE + 94)
MCI_INFO_MEDIA_UPC 	equ 400h
MCI_INFO_MEDIA_IDENTITY equ 800h
MCI_INFO_NAME 		equ 1000h
MCI_INFO_COPYRIGHT 	equ 2000h
MCI_CDA_STATUS_TYPE_TRACK equ 4001h
MCI_CDA_TRACK_AUDIO 	equ (MCI_CD_OFFSET + 0)
MCI_CDA_TRACK_OTHER 	equ (MCI_CD_OFFSET + 1)
MCI_SEQ_STATUS_NAME 	equ 400Bh
MCI_SEQ_STATUS_COPYRIGHT equ 400Ch
MCI_SEQ_SET_TEMPO 	equ 10000h
MCI_SEQ_SET_PORT 		equ 20000h
MCI_SEQ_SET_SLAVE 	equ 40000h
MCI_SEQ_SET_MASTER 	equ 80000h
MCI_SEQ_SET_OFFSET 	equ 1000000h

MIXERLINEA STRUCT            DWORD
    cbStruct                DWORD       ?
    dwDestination           DWORD       ?
    dwSource                DWORD       ?
    dwLineID                DWORD       ?
    fdwLine                 DWORD       ?
    dwUser                  DWORD       ?
    dwComponentType         DWORD       ?
    cChannels               DWORD       ?
    cConnections            DWORD       ?
    cControls               DWORD       ?
    szShortName             db    MIXER_SHORT_NAME_CHARS dup(?)
    szName                  db    MIXER_LONG_NAME_CHARS dup(?)
    struct Target
	dwType dd ?
	dwDeviceID dd ?
	wMid dw ?
	wPid dw ?
	vDriverVersion MMVERSION ?
	szPname db MAXPNAMELEN dup(?)
    ends
MIXERLINEA ENDS

MIXERLINE EQU <MIXERLINEA>

MIXERCONTROLA STRUCT         DWORD
     cbStruct               DWORD       ?
     dwControlID            DWORD       ?
     dwControlType          DWORD       ?
     fdwControl             DWORD       ?
     cMultipleItems         DWORD       ?
     szShortName            db    MIXER_SHORT_NAME_CHARS dup(?)
     szName                 db    MIXER_LONG_NAME_CHARS dup(?)
    union Bounds
	struct
		lMinimum dd ?
		lMaximum dd ?
	ends
	struct
		dwMinimum dd ?
		dwMaximum dd ?
	ends
	dwReserved dd 6 dup(?)
    ends
    union Metrics
	cSteps dd ?
	cbCustomData dd ?
	dwReserved dd 6 dup(?)
    ends
MIXERCONTROLA ENDS

MIXERCONTROL EQU <MIXERCONTROLA>

WAVEFORMATEX STRUCT
	wFormatTag dw ?
	nChannels dw ?
	nSamplesPerSec dd ?
	nAvgBytesPerSec dd ?
	nBlockAlign dw ?
	wBitsPerSample dw ?
	cbSize dw ?
WAVEFORMATEX ENDS

MIXERCONTROLDETAILS_LISTTEXTA STRUCT
	dwParam1 dd ?
	dwParam2 dd ?
	szName db MIXER_LONG_NAME_CHARS dup(?)
MIXERCONTROLDETAILS_LISTTEXTA ENDS

MIXERCONTROLDETAILS_LISTTEXT EQU <MIXERCONTROLDETAILS_LISTTEXTA>

;-------------------------------------[ lmcons.h ]--------------------------------------
CNLEN equ 15
LM20_CNLEN equ 15
DNLEN equ CNLEN
LM20_DNLEN equ LM20_CNLEN
UNCLEN equ CNLEN+2
LM20_UNCLEN equ LM20_CNLEN+2
NNLEN equ 80
LM20_NNLEN equ 12
RMLEN equ UNCLEN+1+NNLEN
LM20_RMLEN equ LM20_UNCLEN+1+LM20_NNLEN
SNLEN equ 80
LM20_SNLEN equ 15
STXTLEN equ 256
LM20_STXTLEN equ 63
PATHLEN equ 256
LM20_PATHLEN equ 256
DEVLEN equ 80
LM20_DEVLEN equ 8
EVLEN equ 16
UNLEN equ 256
LM20_UNLEN equ 20
GNLEN equ UNLEN
LM20_GNLEN equ LM20_UNLEN
PWLEN equ 256
LM20_PWLEN equ 14
SHPWLEN equ 8
CLTYPE_LEN equ 12
MAXCOMMENTSZ equ 256
LM20_MAXCOMMENTSZ equ 48
QNLEN equ NNLEN
LM20_QNLEN equ LM20_NNLEN
ALERTSZ equ 128
MAXDEVENTRIES equ sizeof int *8
NETBIOS_NAME_LEN equ 16
MAX_PREFERRED_LENGTH equ -1
CRYPT_KEY_LEN equ 7
CRYPT_TXT_LEN equ 8
ENCRYPTED_PWLEN equ 16
SESSION_PWLEN equ 24
SESSION_CRYPT_KLEN equ 21
PARMNUM_ALL equ 0
PARM_ERROR_UNKNOWN equ -1
PARM_ERROR_NONE equ 0
PARMNUM_BASE_INFOLEVEL equ 1000
MESSAGE_FILENAME equ "NETMSG"
OS2MSG_FILENAME equ "BASE"
HELP_MSG_FILENAME equ "NETH"
BACKUP_MSG_FILENAME equ "BAK.MSG"
PLATFORM_ID_DOS equ 300
PLATFORM_ID_OS2 equ 400
PLATFORM_ID_NT equ 500
PLATFORM_ID_OSF equ 600
PLATFORM_ID_VMS equ 700
MIN_LANMAN_MESSAGE_ID equ NERR_BASE
MAX_LANMAN_MESSAGE_ID equ 5799

;---------------------------------------[ RAS.H ]-----------------------------------------
RAS_MaxDeviceType equ 16
RAS_MaxPhoneNumber equ 128
RAS_MaxIpAddress equ 15
RAS_MaxIpxAddress equ 21
RAS_MaxEntryName equ 256
RAS_MaxDeviceName equ 128
RAS_MaxCallbackNumber equ RAS_MaxPhoneNumber
RAS_MaxAreaCode equ 10
RAS_MaxPadType equ 32
RAS_MaxX25Address equ 200
RAS_MaxFacilities equ 200
RAS_MaxUserData equ 200
RASCS_PAUSED equ 1000h
RASCS_DONE equ 2000h
RDEOPT_UsePrefixSuffix equ 1
RDEOPT_PausedStates equ 2
RDEOPT_IgnoreModemSpeaker equ 4
RDEOPT_SetModemSpeaker equ 8
RDEOPT_IgnoreSoftwareCompression equ 10h
RDEOPT_SetSoftwareCompression equ 20h
RDEOPT_DisableConnectedUI equ 40h
RDEOPT_DisableReconnectUI equ 80h
RDEOPT_DisableReconnect equ 100h
RDEOPT_NoUser equ 200h
RDEOPT_PauseOnScript equ 400h
RASDIALEVENT equ "RasDialEvent"
WM_RASDIALEVENT equ 0CCCDh
RASEO_UseCountryAndAreaCodes equ 1
RASEO_SpecificIpAddr equ 2
RASEO_SpecificNameServers equ 4
RASEO_IpHeaderCompression equ 8
RASEO_RemoteDefaultGateway equ 10h
RASEO_DisableLcpExtensions equ 20h
RASEO_TerminalBeforeDial equ 40h
RASEO_TerminalAfterDial equ 80h
RASEO_ModemLights equ 100h
RASEO_SwCompression equ 200h
RASEO_RequireEncryptedPw equ 400h
RASEO_RequireMsEncryptedPw equ 800h
RASEO_RequireDataEncryption equ 1000h
RASEO_NetworkLogon equ 2000h
RASEO_UseLogonCredentials equ 4000h
RASEO_PromoteAlternates equ 8000h
RASEO_SecureLocalFiles equ 10000h
RASNP_NetBEUI equ 1
RASNP_Ipx equ 2
RASNP_Ip equ 4
RASFP_Ppp equ 1
RASFP_Slip equ 2
RASFP_Ras equ 4
RASDT_Modem equ "modem"
RASDT_Isdn equ "isdn"
RASDT_X25 equ "x25"
RASCN_Connection equ 1
RASCN_Disconnection equ 2
RASCN_BandwidthAdded equ 4
RASCN_BandwidthRemoved equ 8
RASEDM_DialAll equ 1
RASEDM_DialAsNeeded equ 2
RASIDS_Disabled equ 0ffffffffh
RASIDS_UseGlobalValue equ 0
RASADFLG_PositionDlg equ 1
RASCM_UserName equ 1
RASCM_Password equ 2
RASCM_Domain equ 4
RASADP_DisableConnectionQuery equ 0
RASADP_LoginSessionDisable equ 1
RASADP_SavedAddressesLimit equ 2
RASADP_FailedConnectionTimeout equ 3
RASADP_ConnectionQueryTimeout equ 4

RASCS_OpenPort equ 0
RASCS_PortOpened equ 1
RASCS_ConnectDevice equ 2
RASCS_DeviceConnected equ 3
RASCS_AllDevicesConnected equ 4
RASCS_Authenticate equ 5
RASCS_AuthNotify equ 6
RASCS_AuthRetry equ 7
RASCS_AuthCallback equ 8
RASCS_AuthChangePassword equ 9
RASCS_AuthProject equ 10
RASCS_AuthLinkSpeed equ 11
RASCS_AuthAck equ 12
RASCS_ReAuthenticate equ 13
RASCS_Authenticated equ 14
RASCS_PrepareForCallback equ 15
RASCS_WaitForModemReset equ 16
RASCS_WaitForCallback equ 17
RASCS_Projected equ 18
RASCS_StartAuthentication equ 19
RASCS_CallbackComplete equ 20
RASCS_LogonNetwork equ 21
RASCS_SubEntryConnected equ 22
RASCS_SubEntryDisconnected equ 23
RASCS_Interactive equ RASCS_PAUSED
RASCS_RetryAuthentication equ RASCS_PAUSED+1
RASCS_CallbackSetByCaller equ RASCS_PAUSED+2
RASCS_PasswordExpired equ RASCS_PAUSED+3
RASCS_Connected equ RASCS_DONE
RASCS_Disconnected equ RASCS_DONE+1
RASP_Amb equ 10000h
RASP_PppNbf equ 803Fh
RASP_PppIpx equ 802Bh
RASP_PppIp equ 8021h
RASP_PppLcp equ C021h
RASP_Slip equ 20000h

RASCONNA STRUCT
    dwSize dd ?
    hrasconn dd ?
    szEntryName db RAS_MaxEntryName + 1 dup(?)
    szDeviceType db RAS_MaxDeviceType + 1 dup(?)
    szDeviceName db RAS_MaxDeviceName + 1 dup(?)
RASCONNA ENDS 

RASCONN EQU <RASCONNA>

RASCONNSTATUSA STRUCT
    dwSize dd ?
    rasconnstate dd ?
    dwError dd ?
    szDeviceType db RAS_MaxDeviceType + 1 dup(?)
    szDeviceName db RAS_MaxDeviceName + 1 dup(?)
RASCONNSTATUSA ENDS

RASCONNSTATUS EQU <RASCONNSTATUSA>

RASDIALPARAMSA STRUCT
    dwSize dd ?
    szEntryName db RAS_MaxEntryName + 1 dup(?)
    szPhoneNumber db RAS_MaxPhoneNumber + 1 dup(?)
    szCallbackNumber db RAS_MaxCallbackNumber + 1 dup(?)
    szUserName db UNLEN + 1 dup(?)
    szPassword db PWLEN + 1 dup(?)
    szDomain db DNLEN + 1 dup(?)
RASDIALPARAMSA ENDS

RASDIALPARAMS EQU <RASDIALPARAMSA>

RASDIALEXTENSIONS STRUCT
    dwSize dd ?
    dwfOptions dd ?
    hwndParent dd ?
    reserved dd ?
RASDIALEXTENSIONS ENDS

RASENTRYNAMEA STRUCT
    dwSize dd ?
    szEntryName db RAS_MaxEntryName + 1 dup(?)
RASENTRYNAMEA ENDS

RASENTRYNAME EQU <RASENTRYNAMEA>

RASAMBA STRUCT
    dwSize dd ?
    dwError dd ?
    szNetBiosError db NETBIOS_NAME_LEN + 1 dup(?)
    bLana db ?
RASAMBA ENDS

RASAMB EQU <RASAMBA>

RASPPPNBFA STRUCT
    dwSize dd ?
    dwError dd ?
    dwNetBiosError dd ?
    szNetBiosError db NETBIOS_NAME_LEN + 1 dup(?)
    szWorkstationName db NETBIOS_NAME_LEN + 1 dup(?)
    bLana db ?
RASPPPNBFA ENDS

RASPPPNBF EQU <RASPPPNBFA>

RASPPPIPXA STRUCT
    dwSize dd ?
    dwError dd ?
    szIpxAddress db RAS_MaxIpxAddress + 1 dup (?)
RASPPPIPXA ENDS

RASPPPIPX EQU <RASPPPIPXA>

RASPPPIPA STRUCT
    dwSize dd ?
    dwError dd ?
    szIpAddress db RAS_MaxIpAddress + 1 dup(?)
    szServerIpAddress db RAS_MaxIpAddress + 1 dup(?)
RASPPPIPA ENDS

RASPPPIP EQU <RASPPPIPA>

RASPPPLCP STRUCT
    dwSize dd ?
    fBundled BOOL ?
RASPPPLCP ENDS

RASSLIPA STRUCT
    dwSize dd ?
    dwError dd ?
    szIpAddress db RAS_MaxIpAddress + 1 dup(?)
RASSLIPA ENDS

RASSLIP EQU <RASSLIPA>

RASDEVINFOA STRUCT
    dwSize dd ?
    szDeviceType db RAS_MaxDeviceType + 1 dup(?)
    szDeviceName db RAS_MaxDeviceName + 1 dup(?)
RASDEVINFOA ENDS

RASDEVINFO EQU <RASDEVINFOA>

RASCTRYINFO STRUCT
    dwSize dd ?
    dwCountryID dd ?
    dwNextCountryID dd ?
    dwCountryCode dd ?
    dwCountryNameOffset dd ?
RASCTRYINFO ENDS

RASIPADDR STRUCT
    _a db ?
    _b db ?
    _c db ?
    _d db ?
RASIPADDR ENDS

RASENTRYA STRUCT
    dwSize dd ?
    dwfOptions dd ?
    dwCountryID dd ?
    dwCountryCode dd ?
    szAreaCode db RAS_MaxAreaCode + 1 dup(?)
    szLocalPhoneNumber db RAS_MaxPhoneNumber + 1 dup(?)
    dwAlternateOffset dd ?
    ipaddr RASIPADDR <>
    ipaddrDns RASIPADDR <>
    ipaddrDnsAlt RASIPADDR <>
    ipaddrWins RASIPADDR <>
    ipaddrWinsAlt RASIPADDR <>
    dwFrameSize dd ?
    dwfNetProtocols dd ?
    dwFramingProtocol dd ?
    szScript db MAX_PATH dup(?)
    szAutodialDll db MAX_PATH dup(?)
    szAutodialFunc db MAX_PATH dup(?)
    szDeviceType db RAS_MaxDeviceType + 1 dup(?)
    szDeviceName db RAS_MaxDeviceName + 1 dup(?)
    szX25PadType db RAS_MaxPadType + 1 dup(?)
    szX25Address db RAS_MaxX25Address + 1 dup(?)
    szX25Facilities db RAS_MaxFacilities + 1 dup(?)
    szX25UserData db RAS_MaxUserData + 1 dup(?)
    dwChannels dd ?
    dwReserved1 dd ?
    dwReserved2 dd ?
RASENTRYA ENDS

RASENTRY EQU <RASENTRYA>

;-----------------------------------------[ rasdlg.h ]--------------------------------
RASPBDEVENT_AddEntry equ 1
RASPBDEVENT_EditEntry equ 2
RASPBDEVENT_RemoveEntry equ 3
RASPBDEVENT_DialEntry equ 4
RASPBDEVENT_EditGlobals equ 5
RASPBDEVENT_NoUser equ 6
RASPBDEVENT_NoUserEdit equ 7
RASPBDFLAG_PositionDlg equ 1
RASPBDFLAG_ForceCloseOnDial equ 2
RASPBDFLAG_NoUser equ 10h
RASPBDFLAG_UpdateDefaults equ 80000000h
RASMDPAGE_Status equ 0
RASMDPAGE_Summary equ 1
RASMDPAGE_Preferences equ 2
RASMDFLAG_PositionDlg equ 1
RASMDFLAG_UpdateDefaults equ 80000000h
RASEDFLAG_PositionDlg equ 1
RASEDFLAG_NewEntry equ 2
RASEDFLAG_CloneEntry equ 4
RASEDFLAG_NoRename equ 8
RASDDFLAG_PositionDlg equ 1
RASDDFLAG_LinkFailure equ 80000000h

RASPBDLGA STRUCT
    dwSize dd ?
    hwndOwner dd ?
    dwFlags dd ?
    xDlg dd ?
    yDlg dd ?
    dwCallbackId dd ?
    pCallback dd ?
    dwError dd ?
    reserved dd ?
    reserved2 dd ?
RASPBDLGA ENDS

RASPBDLG EQU <RASPBDLGA>

RASENTRYDLGA STRUCT
    dwSize dd ?
    hwndOwner dd ?
    dwFlags dd ?
    xDlg dd ?
    yDlg dd ?
    szEntry db RAS_MaxEntryName + 1 dup(?)
    dwError dd ?
    reserved dd ?
    reserved2 dd ?
RASENTRYDLGA ENDS

RASENTRYDLG EQU <RASENTRYDLGA>

RASDIALDLG STRUCT
    dwSize dd ?
    hwndOwner dd ?
    dwFlags dd ?
    xDlg dd ?
    yDlg dd ?
    dwSubEntry dd ?
    dwError dd ?
    reserved dd ?
    reserved2 dd ?
RASDIALDLG ENDS

RASMONITORDLG STRUCT
    dwSize dd ?
    hwndOwner dd ?
    dwFlags dd ?
    dwStartPage dd ?
    xDlg dd ?
    yDlg dd ?
    dwError dd ?
    reserved dd ?
    reserved2 dd ?
RASMONITORDLG ENDS

RASNOUSERA STRUCT
    dwSize dd ?
    dwFlags dd ?
    dwTimeoutMs dd ?
    szUserName db UNLEN + 1 dup(?)
    szPassword db PWLEN + 1 dup(?)
    szDomain db DNLEN + 1 dup(?)
RASNOUSERA ENDS

RASNOUSER EQU <RASNOUSERA>

;----------------------------------------[ raserror.h ]--------------------------------
RASBASE equ 600
SUCCESS equ 0
PENDING equ (RASBASE+0)
ERROR_INVALID_PORT_HANDLE equ (RASBASE+1)
ERROR_PORT_ALREADY_OPEN equ (RASBASE+2)
ERROR_BUFFER_TOO_SMALL equ (RASBASE+3)
ERROR_WRONG_INFO_SPECIFIED equ (RASBASE+4)
ERROR_CANNOT_SET_PORT_INFO equ (RASBASE+5)
ERROR_PORT_NOT_CONNECTED equ (RASBASE+6)
ERROR_EVENT_INVALID equ (RASBASE+7)
ERROR_DEVICE_DOES_NOT_EXIST equ (RASBASE+8)
ERROR_DEVICETYPE_DOES_NOT_EXIST equ (RASBASE+9)
ERROR_BUFFER_INVALID equ (RASBASE+10)
ERROR_ROUTE_NOT_AVAILABLE equ (RASBASE+11)
ERROR_ROUTE_NOT_ALLOCATED equ (RASBASE+12)
ERROR_INVALID_COMPRESSION_SPECIFIED equ (RASBASE+13)
ERROR_OUT_OF_BUFFERS equ (RASBASE+14)
ERROR_PORT_NOT_FOUND equ (RASBASE+15)
ERROR_ASYNC_REQUEST_PENDING equ (RASBASE+16)
ERROR_ALREADY_DISCONNECTING equ (RASBASE+17)
ERROR_PORT_NOT_OPEN equ (RASBASE+18)
ERROR_PORT_DISCONNECTED equ (RASBASE+19)
ERROR_NO_ENDPOINTS equ (RASBASE+20)
ERROR_CANNOT_OPEN_PHONEBOOK equ (RASBASE+21)
ERROR_CANNOT_LOAD_PHONEBOOK equ (RASBASE+22)
ERROR_CANNOT_FIND_PHONEBOOK_ENTRY equ (RASBASE+23)
ERROR_CANNOT_WRITE_PHONEBOOK equ (RASBASE+24)
ERROR_CORRUPT_PHONEBOOK equ (RASBASE+25)
ERROR_CANNOT_LOAD_STRING equ (RASBASE+26)
ERROR_KEY_NOT_FOUND equ (RASBASE+27)
ERROR_DISCONNECTION equ (RASBASE+28)
ERROR_REMOTE_DISCONNECTION equ (RASBASE+29)
ERROR_HARDWARE_FAILURE equ (RASBASE+30)
ERROR_USER_DISCONNECTION equ (RASBASE+31)
ERROR_INVALID_SIZE equ (RASBASE+32)
ERROR_PORT_NOT_AVAILABLE equ (RASBASE+33)
ERROR_CANNOT_PROJECT_CLIENT equ (RASBASE+34)
ERROR_UNKNOWN equ (RASBASE+35)
ERROR_WRONG_DEVICE_ATTACHED equ (RASBASE+36)
ERROR_BAD_STRING equ (RASBASE+37)
ERROR_REQUEST_TIMEOUT equ (RASBASE+38)
ERROR_CANNOT_GET_LANA equ (RASBASE+39)
ERROR_NETBIOS_ERROR equ (RASBASE+40)
ERROR_SERVER_OUT_OF_RESOURCES equ (RASBASE+41)
ERROR_NAME_EXISTS_ON_NET equ (RASBASE+42)
ERROR_SERVER_GENERAL_NET_FAILURE equ (RASBASE+43)
WARNING_MSG_ALIAS_NOT_ADDED equ (RASBASE+44)
ERROR_AUTH_INTERNAL equ (RASBASE+45)
ERROR_RESTRICTED_LOGON_HOURS equ (RASBASE+46)
ERROR_ACCT_DISABLED equ (RASBASE+47)
ERROR_PASSWD_EXPIRED equ (RASBASE+48)
ERROR_NO_DIALIN_PERMISSION equ (RASBASE+49)
ERROR_SERVER_NOT_RESPONDING equ (RASBASE+50)
ERROR_FROM_DEVICE equ (RASBASE+51)
ERROR_UNRECOGNIZED_RESPONSE equ (RASBASE+52)
ERROR_MACRO_NOT_FOUND equ (RASBASE+53)
ERROR_MACRO_NOT_DEFINED equ (RASBASE+54)
ERROR_MESSAGE_MACRO_NOT_FOUND equ (RASBASE+55)
ERROR_DEFAULTOFF_MACRO_NOT_FOUND equ (RASBASE+56)
ERROR_FILE_COULD_NOT_BE_OPENED equ (RASBASE+57)
ERROR_DEVICENAME_TOO_LONG equ (RASBASE+58)
ERROR_DEVICENAME_NOT_FOUND equ (RASBASE+59)
ERROR_NO_RESPONSES equ (RASBASE+60)
ERROR_NO_COMMAND_FOUND equ (RASBASE+61)
ERROR_WRONG_KEY_SPECIFIED equ (RASBASE+62)
ERROR_UNKNOWN_DEVICE_TYPE equ (RASBASE+63)
ERROR_ALLOCATING_MEMORY equ (RASBASE+64)
ERROR_PORT_NOT_CONFIGURED equ (RASBASE+65)
ERROR_DEVICE_NOT_READY equ (RASBASE+66)
ERROR_READING_INI_FILE equ (RASBASE+67)
ERROR_NO_CONNECTION equ (RASBASE+68)
ERROR_BAD_USAGE_IN_INI_FILE equ (RASBASE+69)
ERROR_READING_SECTIONNAME equ (RASBASE+70)
ERROR_READING_DEVICETYPE equ (RASBASE+71)
ERROR_READING_DEVICENAME equ (RASBASE+72)
ERROR_READING_USAGE equ (RASBASE+73)
ERROR_READING_MAXCONNECTBPS equ (RASBASE+74)
ERROR_READING_MAXCARRIERBPS equ (RASBASE+75)
ERROR_LINE_BUSY equ (RASBASE+76)
ERROR_VOICE_ANSWER equ (RASBASE+77)
ERROR_NO_ANSWER equ (RASBASE+78)
ERROR_NO_CARRIER equ (RASBASE+79)
ERROR_NO_DIALTONE equ (RASBASE+80)
ERROR_IN_COMMAND equ (RASBASE+81)
ERROR_WRITING_SECTIONNAME equ (RASBASE+82)
ERROR_WRITING_DEVICETYPE equ (RASBASE+83)
ERROR_WRITING_DEVICENAME equ (RASBASE+84)
ERROR_WRITING_MAXCONNECTBPS equ (RASBASE+85)
ERROR_WRITING_MAXCARRIERBPS equ (RASBASE+86)
ERROR_WRITING_USAGE equ (RASBASE+87)
ERROR_WRITING_DEFAULTOFF equ (RASBASE+88)
ERROR_READING_DEFAULTOFF equ (RASBASE+89)
ERROR_EMPTY_INI_FILE equ (RASBASE+90)
ERROR_AUTHENTICATION_FAILURE equ (RASBASE+91)
ERROR_PORT_OR_DEVICE equ (RASBASE+92)
ERROR_NOT_BINARY_MACRO equ (RASBASE+93)
ERROR_DCB_NOT_FOUND equ (RASBASE+94)
ERROR_STATE_MACHINES_NOT_STARTED equ (RASBASE+95)
ERROR_STATE_MACHINES_ALREADY_STARTED equ (RASBASE+96)
ERROR_PARTIAL_RESPONSE_LOOPING equ (RASBASE+97)
ERROR_UNKNOWN_RESPONSE_KEY equ (RASBASE+98)
ERROR_RECV_BUF_FULL equ (RASBASE+99)
ERROR_CMD_TOO_LONG equ (RASBASE+100)
ERROR_UNSUPPORTED_BPS equ (RASBASE+101)
ERROR_UNEXPECTED_RESPONSE equ (RASBASE+102)
ERROR_INTERACTIVE_MODE equ (RASBASE+103)
ERROR_BAD_CALLBACK_NUMBER equ (RASBASE+104)
ERROR_INVALID_AUTH_STATE equ (RASBASE+105)
ERROR_WRITING_INITBPS equ (RASBASE+106)
ERROR_X25_DIAGNOSTIC equ (RASBASE+107)
ERROR_ACCT_EXPIRED equ (RASBASE+108)
ERROR_CHANGING_PASSWORD equ (RASBASE+109)
ERROR_OVERRUN equ (RASBASE+110)
ERROR_RASMAN_CANNOT_INITIALIZE	 equ (RASBASE+111)
ERROR_BIPLEX_PORT_NOT_AVAILABLE equ (RASBASE+112)
ERROR_NO_ACTIVE_ISDN_LINES equ (RASBASE+113)
ERROR_NO_ISDN_CHANNELS_AVAILABLE equ (RASBASE+114)
ERROR_TOO_MANY_LINE_ERRORS equ (RASBASE+115)
ERROR_IP_CONFIGURATION equ (RASBASE+116)
ERROR_NO_IP_ADDRESSES equ (RASBASE+117)
ERROR_PPP_TIMEOUT equ (RASBASE+118)
ERROR_PPP_REMOTE_TERMINATED equ (RASBASE+119)
ERROR_PPP_NO_PROTOCOLS_CONFIGURED equ (RASBASE+120)
ERROR_PPP_NO_RESPONSE equ (RASBASE+121)
ERROR_PPP_INVALID_PACKET equ (RASBASE+122)
ERROR_PHONE_NUMBER_TOO_LONG equ (RASBASE+123)
ERROR_IPXCP_NO_DIALOUT_CONFIGURED equ (RASBASE+124)
ERROR_IPXCP_NO_DIALIN_CONFIGURED equ (RASBASE+125)
ERROR_IPXCP_DIALOUT_ALREADY_ACTIVE equ (RASBASE+126)
ERROR_ACCESSING_TCPCFGDLL equ (RASBASE+127)
ERROR_NO_IP_RAS_ADAPTER equ (RASBASE+128)
ERROR_SLIP_REQUIRES_IP equ (RASBASE+129)
ERROR_PROJECTION_NOT_COMPLETE equ (RASBASE+130)
ERROR_PROTOCOL_NOT_CONFIGURED equ (RASBASE+131)
ERROR_PPP_NOT_CONVERGING equ (RASBASE+132)
ERROR_PPP_CP_REJECTED equ (RASBASE+133)
ERROR_PPP_LCP_TERMINATED equ (RASBASE+134)
ERROR_PPP_REQUIRED_ADDRESS_REJECTED equ (RASBASE+135)
ERROR_PPP_NCP_TERMINATED equ (RASBASE+136)
ERROR_PPP_LOOPBACK_DETECTED equ (RASBASE+137)
ERROR_PPP_NO_ADDRESS_ASSIGNED equ (RASBASE+138)
ERROR_CANNOT_USE_LOGON_CREDENTIALS equ (RASBASE+139)
ERROR_TAPI_CONFIGURATION equ (RASBASE+140)
ERROR_NO_LOCAL_ENCRYPTION equ (RASBASE+141)
ERROR_NO_REMOTE_ENCRYPTION equ (RASBASE+142)
ERROR_REMOTE_REQUIRES_ENCRYPTION equ (RASBASE+143)
ERROR_IPXCP_NET_NUMBER_CONFLICT equ (RASBASE+144)
ERROR_INVALID_SMM equ (RASBASE+145)
ERROR_SMM_UNINITIALIZED equ (RASBASE+146)
ERROR_NO_MAC_FOR_PORT equ (RASBASE+147)
ERROR_SMM_TIMEOUT equ (RASBASE+148)
ERROR_BAD_PHONE_NUMBER equ (RASBASE+149)
ERROR_WRONG_MODULE equ (RASBASE+150)
ERROR_INVALID_CALLBACK_NUMBER equ (RASBASE+151)
ERROR_SCRIPT_SYNTAX equ (RASBASE+152)
RASBASEEND equ (RASBASE+152)

;------------------------------------[ rassapi.h ]-------------------------------------

RASSAPI_MAX_PHONENUMBER_SIZE equ 128
RASSAPI_MAX_MEDIA_NAME	 equ 16
RASSAPI_MAX_PORT_NAME	 equ 16
RASSAPI_MAX_DEVICE_NAME equ 128
RASSAPI_MAX_DEVICETYPE_NAME equ 16
RASSAPI_MAX_PARAM_KEY_SIZE equ 32
RASPRIV_NoCallback equ 1
RASPRIV_AdminSetCallback equ 2
RASPRIV_CallerSetCallback equ 4
RASPRIV_DialinPrivilege equ 8
RASPRIV_CallbackType equ (RASPRIV_AdminSetCallback \
                              + RASPRIV_CallerSetCallback \
                              + RASPRIV_NoCallback)
RAS_MODEM_OPERATIONAL	 equ 1
RAS_MODEM_NOT_RESPONDING equ 2
RAS_MODEM_HARDWARE_FAILURE equ 3
RAS_MODEM_INCORRECT_RESPONSE equ 4
RAS_MODEM_UNKNOWN equ 	        5
RAS_PORT_NON_OPERATIONAL equ 1
RAS_PORT_DISCONNECTED	 equ 2
RAS_PORT_CALLING_BACK equ 3
RAS_PORT_LISTENING	 equ 4
RAS_PORT_AUTHENTICATING equ 5
RAS_PORT_AUTHENTICATED	 equ 6
RAS_PORT_INITIALIZING	 equ 7
MEDIA_UNKNOWN equ 0
MEDIA_SERIAL equ 1
MEDIA_RAS10_SERIAL equ 2
MEDIA_X25 equ 3
MEDIA_ISDN equ 4
USER_AUTHENTICATED equ 1
MESSENGER_PRESENT equ 2
PPP_CLIENT equ 4
GATEWAY_ACTIVE equ 8
REMOTE_LISTEN equ 10h
PORT_MULTILINKED equ 20h
RAS_IPADDRESSLEN equ 15
RAS_IPXADDRESSLEN equ 22
RAS_ATADDRESSLEN equ 32
RASDOWNLEVEL equ 10
RASADMIN_35 equ 35
RASADMIN_CURRENT equ 40
ParamNumber equ 0
ParamString	equ 1

RAS_PARAMS_VALUE UNION DWORD
	Number dd ?
	struct String
		iLength dd ?
		Data dd ?
	ends
RAS_PARAMS_VALUE ENDS

RAS_PARAMETERS STRUCT
    P_Key	db RASSAPI_MAX_PARAM_KEY_SIZE dup(?)
    P_Type dd ?
    P_Attributes db ?
    P_Value RAS_PARAMS_VALUE <>
RAS_PARAMETERS ENDS

RAS_USER_0 STRUCT
    bfPrivilege db ?
    szPhoneNumber dw RASSAPI_MAX_PHONENUMBER_SIZE + 1 dup(?)
RAS_USER_0 ENDS

RAS_PORT_0 STRUCT
    wszPortName dw RASSAPI_MAX_PORT_NAME dup(?)
    wszDeviceType dw RASSAPI_MAX_DEVICETYPE_NAME dup(?)
    wszDeviceName dw RASSAPI_MAX_DEVICE_NAME dup(?)
    wszMediaName dw RASSAPI_MAX_MEDIA_NAME dup(?)
    reserved dd ?
    Flags dd ?
    wszUserName dw UNLEN + 1 dup(?)
    wszComputer dw NETBIOS_NAME_LEN dup(?)
    dwStartSessionTime dd ?
    wszLogonDomain dw DNLEN + 1 dup(?)
    fAdvancedServer BOOL ?
RAS_PORT_0 ENDS

RAS_PPP_NBFCP_RESULT STRUCT
    dwError dd ?
    dwNetBiosError dd ?
    szName db NETBIOS_NAME_LEN + 1 dup(?)
    wszWksta dw NETBIOS_NAME_LEN + 1 dup(?)
RAS_PPP_NBFCP_RESULT ENDS

RAS_PPP_IPCP_RESULT STRUCT
    dwError dd ?
    wszAddress dw RAS_IPADDRESSLEN + 1 dup(?)
RAS_PPP_IPCP_RESULT ENDS

RAS_PPP_IPXCP_RESULT STRUCT
    dwError dd ?
    wszAddress dw RAS_IPXADDRESSLEN + 1 dup(?)
RAS_PPP_IPXCP_RESULT ENDS

RAS_PPP_ATCP_RESULT STRUCT
    dwError dd ?
    wszAddress dw RAS_ATADDRESSLEN + 1 dup(?)
RAS_PPP_ATCP_RESULT ENDS

RAS_PPP_PROJECTION_RESULT STRUCT
    nbf RAS_PPP_NBFCP_RESULT <>
    ip RAS_PPP_IPCP_RESULT <>
    ipx RAS_PPP_IPXCP_RESULT <>
    at RAS_PPP_ATCP_RESULT <>
RAS_PPP_PROJECTION_RESULT ENDS

RAS_PORT_1 STRUCT
    rasport0 RAS_PORT_0 <>
    LineCondition dd ?
    HardwareCondition dd ?
    LineSpeed dd ?
    NumStatistics dw ?
    NumMediaParms dw ?
    SizeMediaParms dd ?
    ProjResult RAS_PPP_PROJECTION_RESULT <>
RAS_PORT_1 ENDS

RAS_PORT_STATISTICS STRUCT
    dwBytesXmited dd ?
    dwBytesRcved dd ?
    dwFramesXmited dd ?
    dwFramesRcved dd ?
    dwCrcErr dd ?
    dwTimeoutErr dd ?
    dwAlignmentErr dd ?
    dwHardwareOverrunErr dd ?
    dwFramingErr dd ?
    dwBufferOverrunErr dd ?
    dwBytesXmitedUncompressed dd ?
    dwBytesRcvedUncompressed dd ?
    dwBytesXmitedCompressed dd ?
    dwBytesRcvedCompressed dd ?
    dwPortBytesXmited dd ?
    dwPortBytesRcved dd ?
    dwPortFramesXmited dd ?
    dwPortFramesRcved dd ?
    dwPortCrcErr dd ?
    dwPortTimeoutErr dd ?
    dwPortAlignmentErr dd ?
    dwPortHardwareOverrunErr dd ?
    dwPortFramingErr dd ?
    dwPortBufferOverrunErr dd ?
    dwPortBytesXmitedUncompressed dd ?
    dwPortBytesRcvedUncompressed dd ?
    dwPortBytesXmitedCompressed dd ?
    dwPortBytesRcvedCompressed dd ?
RAS_PORT_STATISTICS ENDS

RAS_SERVER_0 STRUCT
    TotalPorts dw ?
    PortsInUse dw ?
    RasVersion dd ?
RAS_SERVER_0 ENDS

;----------------------------------------[ cpl.h ]--------------------------------

WM_CPL_LAUNCH   equ WM_USER+1000
WM_CPL_LAUNCHED equ WM_USER+1001
CPL_DYNAMIC_RES equ 0
CPL_INIT equ 1
CPL_GETCOUNT equ 2
CPL_INQUIRE equ 3
CPL_SELECT equ 4
CPL_DBLCLK equ 5
CPL_STOP equ 6
CPL_EXIT equ 7
CPL_NEWINQUIRE   equ 8
CPL_STARTWPARMSA equ 9
CPL_STARTWPARMSW equ 10
CPL_STARTWPARMS equ CPL_STARTWPARMSA
CPL_SETUP equ 200

CPLINFO STRUCT
    idIcon dd ?
    idName dd ?
    idInfo dd ?
    lData dd ?
CPLINFO ENDS

NEWCPLINFOA STRUCT
    dwSize dd ?
    dwFlags dd ?
    dwHelpContext dd ?
    lData dd ?
    hIcon dd ?
    szName db 32 dup(?)
    szInfo db 64 dup(?)
    szHelpFile db 128 dup(?)
NEWCPLINFOA ENDS

NEWCPLINFO EQU <NEWCPLINFOA>

;--------------------------------------[ cplext.h ]----------------------------------
CPLPAGE_MOUSE_BUTTONS equ 1
CPLPAGE_MOUSE_PTRMOTION equ 2
CPLPAGE_KEYBOARD_SPEED equ 1

;--------------------------------------[ ctl3d.h ]------------------------------------

CTL3D_SUBCLASS_DYNCREATE equ 1
CTL3D_NOSUBCLASS_DYNCREATE equ 2
CTL3D_BUTTON_CTL equ 0
CTL3D_LISTBOX_CTL equ 1
CTL3D_EDIT_CTL equ 2
CTL3D_COMBO_CTL equ 3
CTL3D_STATIC_CTL equ 4
CTL3D_BUTTONS equ 1
CTL3D_LISTBOXES equ 2		
CTL3D_EDITS	equ 4	
CTL3D_COMBOS equ 8
CTL3D_STATICTEXTS	equ 10h		
CTL3D_STATICFRAMES equ 20h
CTL3D_NODLGWINDOW equ 10000h
CTL3D_ALL	equ 0ffffh
WM_DLGBORDER equ WM_USER+3567
CTL3D_NOBORDER	equ 0
CTL3D_BORDER	equ 1
WM_DLGSUBCLASS equ WM_USER+3568
CTL3D_NOSUBCLASS	equ 0
CTL3D_SUBCLASS	equ 1
CTLMSGOFFSET equ 3569
CTL3D_CTLCOLORMSGBOX	equ WM_USER+CTLMSGOFFSET
CTL3D_CTLCOLOREDIT	equ WM_USER+CTLMSGOFFSET+1
CTL3D_CTLCOLORLISTBOX	equ WM_USER+CTLMSGOFFSET+2
CTL3D_CTLCOLORBTN	equ WM_USER+CTLMSGOFFSET+3
CTL3D_CTLCOLORSCROLLBAR equ WM_USER+CTLMSGOFFSET+4
CTL3D_CTLCOLORSTATIC	equ WM_USER+CTLMSGOFFSET+5
CTL3D_CTLCOLORDLG	equ WM_USER+CTLMSGOFFSET+6
CTL3D_3DCHECK equ 26567

;---------------------------------------[ mapi.h ]---------------------------------------

MAPI_OLE equ 1
MAPI_OLE_STATIC equ 2
MAPI_ORIG equ 0
MAPI_TO equ 1
MAPI_CC equ 2
MAPI_BCC equ 3
MAPI_UNREAD equ 1
MAPI_RECEIPT_REQUESTED equ 2
MAPI_SENT equ 4
MAPI_LOGON_UI equ 1
MAPI_PASSWORD_UI equ 20000h
MAPI_NEW_SESSION equ 2
MAPI_FORCE_DOWNLOAD equ 1000h
MAPI_EXTENDED equ 20h
MAPI_DIALOG equ 8
MAPI_UNREAD_ONLY equ 20h
MAPI_GUARANTEE_FIFO equ 100h
MAPI_LONG_MSGID equ 4000h
MAPI_PEEK equ 80h
MAPI_SUPPRESS_ATTACH equ 800h
MAPI_ENVELOPE_ONLY equ 40h
MAPI_BODY_AS_FILE equ 200h

MAPI_AB_NOMODIFY equ 400h
SUCCESS_SUCCESS equ 0
MAPI_USER_ABORT equ 1
MAPI_E_USER_ABORT equ MAPI_USER_ABORT
MAPI_E_FAILURE equ 2
MAPI_E_LOGON_FAILURE equ 3
MAPI_E_LOGIN_FAILURE equ MAPI_E_LOGON_FAILURE
MAPI_E_DISK_FULL equ 4
MAPI_E_INSUFFICIENT_MEMORY equ 5
MAPI_E_ACCESS_DENIED equ 6
MAPI_E_TOO_MANY_SESSIONS equ 8
MAPI_E_TOO_MANY_FILES equ 9
MAPI_E_TOO_MANY_RECIPIENTS equ 10
MAPI_E_ATTACHMENT_NOT_FOUND equ 11
MAPI_E_ATTACHMENT_OPEN_FAILURE equ 12
MAPI_E_ATTACHMENT_WRITE_FAILURE equ 13
MAPI_E_UNKNOWN_RECIPIENT equ 14
MAPI_E_BAD_RECIPTYPE equ 15
MAPI_E_NO_MESSAGES equ 16
MAPI_E_INVALID_MESSAGE equ 17
MAPI_E_TEXT_TOO_LARGE equ 18
MAPI_E_INVALID_SESSION equ 19
MAPI_E_TYPE_NOT_SUPPORTED equ 20
MAPI_E_AMBIGUOUS_RECIPIENT equ 21
MAPI_E_AMBIG_RECIP equ MAPI_E_AMBIGUOUS_RECIPIENT
MAPI_E_MESSAGE_IN_USE equ 22
MAPI_E_NETWORK_FAILURE equ 23
MAPI_E_INVALID_EDITFIELDS equ 24
MAPI_E_INVALID_RECIPS equ 25
MAPI_E_NOT_SUPPORTED equ 26

MapiFileDesc STRUCT
    ulReserved dd ?
    flFlags dd ?
    nPosition dd ?
    lpszPathName dd ?
    lpszFileName dd ?
    lpFileType dd ?
MapiFileDesc ENDS

MapiFileTagExt STRUCT
    ulReserved dd ?
    cbTag dd ?
    lpTag dd ?
    cbEncoding dd ?
    lpEncoding dd ?
MapiFileTagExt ENDS

MapiRecipDesc STRUCT
    ulReserved dd ?
    ulRecipClass dd ?
    lpszName dd ?
    lpszAddress dd ?
    ulEIDSize dd ?
    lpEntryID dd ?
MapiRecipDesc ENDS

MapiMessage STRUCT
    ulReserved dd ?
    lpszSubject dd ?
    lpszNoteText dd ?
    lpszMessageType dd ?
    lpszDateReceived dd ?
    lpszConversationID dd ?
    flFlags dd ?
    lpOriginator dd ?
    nRecipCount dd ?
    lpRecips dd ?
    nFileCount dd ?
    lpFiles dd ?
MapiMessage ENDS

;-------------------------------------[ mapicode.h ]-------------------------------------

MAPI_E_CALL_FAILED equ E_FAIL
MAPI_E_NOT_ENOUGH_MEMORY equ E_OUTOFMEMORY
MAPI_E_INVALID_PARAMETER equ E_INVALIDARG
MAPI_E_INTERFACE_NOT_SUPPORTED equ E_NOINTERFACE
MAPI_E_NO_ACCESS equ E_ACCESSDENIED
MAPI_E_NO_SUPPORT equ 80040102h
MAPI_E_BAD_CHARWIDTH equ 80040103h
MAPI_E_STRING_TOO_LONG equ 80040105h
MAPI_E_UNKNOWN_FLAGS equ 80040106h
MAPI_E_INVALID_ENTRYID equ 80040107h
MAPI_E_INVALID_OBJECT equ 80040108h
MAPI_E_OBJECT_CHANGED equ 80040109h
MAPI_E_OBJECT_DELETED equ 8004010Ah
MAPI_E_BUSY equ 8004010Bh
MAPI_E_NOT_ENOUGH_DISK equ 8004010Dh
MAPI_E_NOT_ENOUGH_RESOURCES equ 8004010Eh
MAPI_E_NOT_FOUND equ 8004010Fh
MAPI_E_VERSION equ 80040110h
MAPI_E_LOGON_FAILED equ 80040111h
MAPI_E_SESSION_LIMIT equ 80040112h
MAPI_E_USER_CANCEL equ 80040113h
MAPI_E_UNABLE_TO_ABORT equ 80040114h
MAPI_E_NETWORK_ERROR equ 80040115h
MAPI_E_DISK_ERROR equ 80040116h
MAPI_E_TOO_COMPLEX equ 80040117h
MAPI_E_BAD_COLUMN equ 80040118h
MAPI_E_EXTENDED_ERROR equ 80040119h
MAPI_E_COMPUTED equ 8004011Ah
MAPI_E_CORRUPT_DATA equ 8004011Bh
MAPI_E_UNCONFIGURED equ 8004011Ch
MAPI_E_FAILONEPROVIDER equ 8004011Dh
MAPI_E_UNKNOWN_CPID equ 8004011Eh
MAPI_E_UNKNOWN_LCID equ 8004011Fh
MAPI_E_PASSWORD_CHANGE_REQUIRED equ 80040120h
MAPI_E_PASSWORD_EXPIRED equ 80040121h
MAPI_E_INVALID_WORKSTATION_ACCOUNT equ 80040122h
MAPI_E_INVALID_ACCESS_TIME equ 80040123h
MAPI_E_ACCOUNT_DISABLED equ 80040124h
MAPI_E_END_OF_SESSION equ 80040200h
MAPI_E_UNKNOWN_ENTRYID equ 80040201h
MAPI_E_MISSING_REQUIRED_COLUMN equ 80040202h
MAPI_W_NO_SERVICE equ 40203h
MAPI_E_BAD_VALUE equ 80040301h
MAPI_E_INVALID_TYPE equ 80040302h
MAPI_E_TYPE_NO_SUPPORT equ 80040303h
MAPI_E_UNEXPECTED_TYPE equ 80040304h
MAPI_E_TOO_BIG equ 80040305h
MAPI_E_DECLINE_COPY equ 80040306h
MAPI_E_UNEXPECTED_ID equ 80040307h
MAPI_W_ERRORS_RETURNED equ 40380h
MAPI_E_UNABLE_TO_COMPLETE equ 80040400h
MAPI_E_TIMEOUT equ 80040401h
MAPI_E_TABLE_EMPTY equ 80040402h
MAPI_E_TABLE_TOO_BIG equ 80040403h
MAPI_E_INVALID_BOOKMARK equ 80040405h
MAPI_W_POSITION_CHANGED equ 40481h
MAPI_W_APPROX_COUNT equ 40482h
MAPI_E_WAIT equ 80040500h
MAPI_E_CANCEL equ 80040501h
MAPI_E_NOT_ME equ 80040502h
MAPI_W_CANCEL_MESSAGE equ 40580h
MAPI_E_CORRUPT_STORE equ 80040600h
MAPI_E_NOT_IN_QUEUE equ 80040601h
MAPI_E_NO_SUPPRESS equ 80040602h
MAPI_E_COLLISION equ 80040604h
MAPI_E_NOT_INITIALIZED equ 80040605h
MAPI_E_NON_STANDARD equ 80040606h
MAPI_E_NO_RECIPIENTS equ 80040607h
MAPI_E_SUBMITTED equ 80040608h
MAPI_E_HAS_FOLDERS equ 80040609h
MAPI_E_HAS_MESSAGES equ 8004060Ah
MAPI_E_FOLDER_CYCLE equ 8004060Bh
MAPI_W_PARTIAL_COMPLETION equ 40680h
MAPI_E_AMBIGUOUS_RECIP equ 80040700h

;-----------------------------[ mapidefs.h ]-----------------------------
MAPI_DIM equ 1
MAPI_NT_SERVICE equ 10000h
MAPI_MODIFY equ 1
MAPI_ACCESS_MODIFY equ 1
MAPI_ACCESS_READ equ 2
MAPI_ACCESS_DELETE equ 4
MAPI_ACCESS_CREATE_HIERARCHY equ 8
MAPI_ACCESS_CREATE_CONTENTS equ 10h
MAPI_ACCESS_CREATE_ASSOCIATED equ 20h
MAPI_UNICODE equ 80000000h
fMapiUnicode equ 0
hrSuccess equ 0
MAPI_ORIG equ 0
MAPI_TO equ 1
MAPI_CC equ 2
MAPI_BCC equ 3
MAPI_P1 equ 10000000h
MAPI_SUBMITTED equ 80000000h
MAPI_SHORTTERM equ 80h
MAPI_NOTRECIP equ 40h
MAPI_THISSESSION equ 20h
MAPI_NOW equ 10h
MAPI_NOTRESERVED equ 08h
MAPI_COMPOUND equ 80h
MAPI_ONE_OFF_UID equ { 81h, 2bh, 1fh, 0a4h, 0beh, 0a3h, 10h, 19h, \
                           09dh, 6eh, 0h, 0ddh, 01h, 0fh, 54h, 02h }
MAPI_ONE_OFF_UNICODE equ 8000h
MAPI_ONE_OFF_NO_RICH_INFO equ 1h
MAPI_STORE equ 1
MAPI_ADDRBOOK equ 2
MAPI_FOLDER equ 3
MAPI_ABCONT equ 4
MAPI_MESSAGE equ 5
MAPI_MAILUSER equ 6
MAPI_ATTACH equ 7
MAPI_DISTLIST equ 8
MAPI_PROFSECT equ 9
MAPI_STATUS equ 0Ah
MAPI_SESSION equ 0Bh
MAPI_FORMINFO equ 0Ch
cchProfileNameMax equ 64
cchProfilePassMax equ 64
MV_FLAG equ 1000h
PT_UNSPECIFIED equ 0
PT_NULL equ 1
PT_I2 equ 2
PT_LONG equ 3
PT_R4 equ 4
PT_DOUBLE equ 5
PT_CURRENCY equ 6
PT_APPTIME equ 7
PT_ERROR equ 10
PT_BOOLEAN equ 11
PT_OBJECT equ 13
PT_I8 equ 20
PT_STRING8 equ 30
PT_UNICODE equ 31
PT_SYSTIME equ 64
PT_CLSID equ 72
PT_BINARY equ 258
PT_SHORT equ PT_I2
PT_I4 equ PT_LONG
PT_FLOAT equ PT_R4
PT_R8 equ PT_DOUBLE
PT_LONGLONG equ PT_I8
PROP_TYPE_MASK equ 0FFFFh
PROP_ID_NULL equ 0
PROP_ID_INVALID equ 0FFFFh
PR_NULL equ (PROP_ID_NULL shl 16) +PT_NULL
PT_MV_I2 equ MV_FLAG+PT_I2
PT_MV_LONG equ MV_FLAG+PT_LONG
PT_MV_R4 equ MV_FLAG+PT_R4
PT_MV_DOUBLE equ MV_FLAG+PT_DOUBLE
PT_MV_CURRENCY equ MV_FLAG+PT_CURRENCY
PT_MV_APPTIME equ MV_FLAG+PT_APPTIME
PT_MV_SYSTIME equ MV_FLAG+PT_SYSTIME
PT_MV_STRING8 equ MV_FLAG+PT_STRING8
PT_MV_BINARY equ MV_FLAG+PT_BINARY
PT_MV_UNICODE equ MV_FLAG+PT_UNICODE
PT_MV_CLSID equ MV_FLAG+PT_CLSID
PT_MV_I8 equ MV_FLAG+PT_I8
PT_MV_SHORT equ PT_MV_I2
PT_MV_I4 equ PT_MV_LONG
PT_MV_FLOAT equ PT_MV_R4
PT_MV_R8 equ PT_MV_DOUBLE
PT_MV_LONGLONG equ PT_MV_I8
MV_INSTANCE equ 2000h
MVI_FLAG equ MV_FLAG + MV_INSTANCE
fnevCriticalError equ 1
fnevNewMail equ 2
fnevObjectCreated equ 4
fnevObjectDeleted equ 8
fnevObjectModified equ 10h
fnevObjectMoved equ 20h
fnevObjectCopied equ 40h
fnevSearchComplete equ 80h
fnevTableModified equ 100h
fnevStatusObjectModified equ 200h
fnevReservedForMapi equ 40000000h
fnevExtended equ 80000000h
TABLE_CHANGED equ 1
TABLE_ERROR equ 2
TABLE_ROW_ADDED equ 3
TABLE_ROW_DELETED equ 4
TABLE_ROW_MODIFIED equ 5
TABLE_SORT_DONE equ 6
TABLE_RESTRICT_DONE equ 7
TABLE_SETCOL_DONE equ 8
TABLE_RELOAD equ 9
MAPI_TOP_LEVEL equ 01h
MAPI_ERROR_VERSION equ 0
KEEP_OPEN_READONLY equ 1
KEEP_OPEN_READWRITE equ 2
FORCE_SAVE equ 4
MAPI_CREATE equ 2
STREAM_APPEND equ 4
MAPI_MOVE equ 1
MAPI_NOREPLACE equ 2
MAPI_DECLINE_OK equ 4
MAPI_DIALOG equ 8
MAPI_USE_DEFAULT equ 40h
MAPI_NO_STRINGS equ 1
MAPI_NO_IDS equ 2
MNID_ID equ 0
MNID_STRING equ 1
TBLSTAT_COMPLETE equ 0
TBLSTAT_QCHANGED equ 7
TBLSTAT_SORTING equ 9
TBLSTAT_SORT_ERROR equ 10
TBLSTAT_SETTING_COLS equ 11
TBLSTAT_SETCOL_ERROR equ 13
TBLSTAT_RESTRICTING equ 14
TBLSTAT_RESTRICT_ERROR equ 15
TBLTYPE_SNAPSHOT equ 0
TBLTYPE_KEYSET equ 1
TBLTYPE_DYNAMIC equ 2
TABLE_SORT_ASCEND equ 0
TABLE_SORT_DESCEND equ 1
TABLE_SORT_COMBINE equ 2
BOOKMARK_BEGINNING equ 0
BOOKMARK_CURRENT equ 1
BOOKMARK_END equ 2
FL_FULLSTRING equ 0
FL_SUBSTRING equ 1
FL_PREFIX equ 2
FL_IGNORECASE equ 10000h
FL_IGNORENONSPACE equ 20000h
FL_LOOSE equ 40000h
RES_AND equ 0
RES_OR equ 1
RES_NOT equ 2
RES_CONTENT equ 3
RES_PROPERTY equ 4
RES_COMPAREPROPS equ 5
RES_BITMASK equ 6
RES_SIZE equ 7
RES_EXIST equ 8
RES_SUBRESTRICTION equ 9
RES_COMMENT equ 0Ah
RELOP_LT equ 0
RELOP_LE equ 1
RELOP_GT equ 2
RELOP_GE equ 3
RELOP_EQ equ 4
RELOP_NE equ 5
RELOP_RE equ 6
BMR_EQZ equ 0
BMR_NEZ equ 1
TBL_ALL_COLUMNS equ 1
TBL_LEAF_ROW equ 1
TBL_EMPTY_CATEGORY equ 2
TBL_EXPANDED_CATEGORY equ 3
TBL_COLLAPSED_CATEGORY equ 4
TBL_NOWAIT equ 1
TBL_ASYNC equ 1
TBL_BATCH equ 2
DIR_BACKWARD equ 1
TBL_NOADVANCE equ 1
PS_PROFILE_PROPERTIES_INIT equ {098h, 15h, 0ACh, 08h, 0AAh, 0B0h, 10h, 1Ah,8Ch, 093h, 08h, 0h, 2Bh, 2Ah, 56h, 0C2h }
MAPI_STORE_PROVIDER equ 33
MAPI_AB equ 34
MAPI_AB_PROVIDER equ 35
MAPI_TRANSPORT_PROVIDER equ 36
MAPI_SPOOLER equ 37
MAPI_PROFILE_PROVIDER equ 38
MAPI_SUBSYSTEM equ 39
MAPI_HOOK_PROVIDER equ 40
STATUS_VALIDATE_STATE equ 1
STATUS_SETTINGS_DIALOG equ 2
STATUS_CHANGE_PASSWORD equ 4
STATUS_FLUSH_QUEUES equ 8
STATUS_DEFAULT_OUTBOUND equ 1
STATUS_DEFAULT_STORE equ 2
STATUS_PRIMARY_IDENTITY equ 4
STATUS_SIMPLE_STORE equ 8
STATUS_XP_PREFER_LAST equ 10h
STATUS_NO_PRIMARY_IDENTITY equ 20h
STATUS_NO_DEFAULT_STORE equ 40h
STATUS_TEMP_SECTION equ 80h
STATUS_OWN_STORE equ 100h
STATUS_NEED_IPM_TREE equ 800h
STATUS_PRIMARY_STORE equ 1000h
STATUS_SECONDARY_STORE equ 2000h
STATUS_AVAILABLE equ 1
STATUS_OFFLINE equ 2
STATUS_FAILURE equ 4
STATUS_INBOUND_ENABLED equ 10000h
STATUS_INBOUND_ACTIVE equ 20000h
STATUS_INBOUND_FLUSH equ 40000h
STATUS_OUTBOUND_ENABLED equ 100000h
STATUS_OUTBOUND_ACTIVE equ 200000h
STATUS_OUTBOUND_FLUSH equ 400000h
STATUS_REMOTE_ACCESS equ 800000h
SUPPRESS_UI equ 1
REFRESH_XP_HEADER_CACHE equ 10000h
PROCESS_XP_HEADER_CACHE equ 20000h
FORCE_XP_CONNECT equ 40000h
FORCE_XP_DISCONNECT equ 80000h
CONFIG_CHANGED equ 100000h
ABORT_XP_HEADER_OPERATION equ 200000h
SHOW_XP_SESSION_UI equ 400000h
UI_READONLY equ 1
FLUSH_UPLOAD equ 2
FLUSH_DOWNLOAD equ 4
FLUSH_FORCE equ 8
FLUSH_NO_UI equ 10h
FLUSH_ASYNC_OK equ 20h
MAPI_BEST_ACCESS equ 10h
CONVENIENT_DEPTH equ 1
SEARCH_RUNNING equ 1
SEARCH_REBUILD equ 2
SEARCH_RECURSIVE equ 4
SEARCH_FOREGROUND equ 8
STOP_SEARCH equ 1
RESTART_SEARCH equ 2
RECURSIVE_SEARCH equ 4
SHALLOW_SEARCH equ 8
FOREGROUND_SEARCH equ 10h
BACKGROUND_SEARCH equ 20h
AB_RECIPIENTS equ 1
AB_SUBCONTAINERS equ 2
AB_MODIFIABLE equ 4
AB_UNMODIFIABLE equ 8
AB_FIND_ON_OPEN equ 10h
AB_NOT_DEFAULT equ 20h
CREATE_CHECK_DUP_STRICT equ 1
CREATE_CHECK_DUP_LOOSE equ 2
CREATE_REPLACE equ 4
MAPI_UNRESOLVED equ 0
MAPI_AMBIGUOUS equ 1
MAPI_RESOLVED equ 2
MAPI_SEND_NO_RICH_INFO equ 10000h
MAPI_DIAG_NO_DIAGNOSTIC equ -1 
MAPI_DIAG_OR_NAME_UNRECOGNIZED equ 0 
MAPI_DIAG_OR_NAME_AMBIGUOUS equ 1 
MAPI_DIAG_MTS_CONGESTED equ 2 
MAPI_DIAG_LOOP_DETECTED equ 3 
MAPI_DIAG_RECIPIENT_UNAVAILABLE equ 4 
MAPI_DIAG_MAXIMUM_TIME_EXPIRED equ 5 
MAPI_DIAG_EITS_UNSUPPORTED equ 6 
MAPI_DIAG_CONTENT_TOO_LONG equ 7 
MAPI_DIAG_IMPRACTICAL_TO_CONVERT equ 8 
MAPI_DIAG_PROHIBITED_TO_CONVERT equ 9 
MAPI_DIAG_CONVERSION_UNSUBSCRIBED equ 10 
MAPI_DIAG_PARAMETERS_INVALID equ 11 
MAPI_DIAG_CONTENT_SYNTAX_IN_ERROR equ 12 
MAPI_DIAG_LENGTH_CONSTRAINT_VIOLATD equ 13 
MAPI_DIAG_NUMBER_CONSTRAINT_VIOLATD equ 14 
MAPI_DIAG_CONTENT_TYPE_UNSUPPORTED equ 15 
MAPI_DIAG_TOO_MANY_RECIPIENTS equ 16 
MAPI_DIAG_NO_BILATERAL_AGREEMENT equ 17 
MAPI_DIAG_CRITICAL_FUNC_UNSUPPORTED equ 18 
MAPI_DIAG_CONVERSION_LOSS_PROHIB equ 19 
MAPI_DIAG_LINE_TOO_LONG equ 20 
MAPI_DIAG_PAGE_TOO_LONG equ 21 
MAPI_DIAG_PICTORIAL_SYMBOL_LOST equ 22 
MAPI_DIAG_PUNCTUATION_SYMBOL_LOST equ 23 
MAPI_DIAG_ALPHABETIC_CHARACTER_LOST equ 24 
MAPI_DIAG_MULTIPLE_INFO_LOSSES equ 25 
MAPI_DIAG_REASSIGNMENT_PROHIBITED equ 26 
MAPI_DIAG_REDIRECTION_LOOP_DETECTED equ 27 
MAPI_DIAG_EXPANSION_PROHIBITED equ 28 
MAPI_DIAG_SUBMISSION_PROHIBITED equ 29 
MAPI_DIAG_EXPANSION_FAILED equ 30 
MAPI_DIAG_RENDITION_UNSUPPORTED equ 31 
MAPI_DIAG_MAIL_ADDRESS_INCORRECT equ 32 
MAPI_DIAG_MAIL_OFFICE_INCOR_OR_INVD equ 33 
MAPI_DIAG_MAIL_ADDRESS_INCOMPLETE equ 34 
MAPI_DIAG_MAIL_RECIPIENT_UNKNOWN equ 35 
MAPI_DIAG_MAIL_RECIPIENT_DECEASED equ 36 
MAPI_DIAG_MAIL_ORGANIZATION_EXPIRED equ 37 
MAPI_DIAG_MAIL_REFUSED equ 38 
MAPI_DIAG_MAIL_UNCLAIMED equ 39 
MAPI_DIAG_MAIL_RECIPIENT_MOVED equ 40 
MAPI_DIAG_MAIL_RECIPIENT_TRAVELLING equ 41 
MAPI_DIAG_MAIL_RECIPIENT_DEPARTED equ 42 
MAPI_DIAG_MAIL_NEW_ADDRESS_UNKNOWN equ 43 
MAPI_DIAG_MAIL_FORWARDING_UNWANTED equ 44 
MAPI_DIAG_MAIL_FORWARDING_PROHIB equ 45 
MAPI_DIAG_SECURE_MESSAGING_ERROR equ 46 
MAPI_DIAG_DOWNGRADING_IMPOSSIBLE equ 47 
MAPI_MH_DP_PUBLIC_UA equ 0
MAPI_MH_DP_PRIVATE_UA equ 1
MAPI_MH_DP_MS equ 2
MAPI_MH_DP_ML equ 3
MAPI_MH_DP_PDAU equ 4
MAPI_MH_DP_PDS_PATRON equ 5
MAPI_MH_DP_OTHER_AU equ 6
FOLDER_ROOT equ 0
FOLDER_GENERIC equ 1
FOLDER_SEARCH equ 2
MESSAGE_MOVE equ 1
MESSAGE_DIALOG equ 2
OPEN_IF_EXISTS equ 1
DEL_MESSAGES equ 1
FOLDER_DIALOG equ 2
DEL_FOLDERS equ 4
DEL_ASSOCIATED equ 8
FOLDER_MOVE equ 1
COPY_SUBFOLDERS equ 10h
MSGSTATUS_HIGHLIGHTED equ 01h
MSGSTATUS_TAGGED equ 2
MSGSTATUS_HIDDEN equ 4
MSGSTATUS_DELMARKED equ 8
MSGSTATUS_REMOTE_DOWNLOAD equ 01000h
MSGSTATUS_REMOTE_DELETE equ 02000h
RECURSIVE_SORT equ 2
FLDSTATUS_HIGHLIGHTED equ 1
FLDSTATUS_TAGGED equ 2
FLDSTATUS_HIDDEN equ 4
FLDSTATUS_DELMARKED equ 8
STORE_ENTRYID_UNIQUE equ 1
STORE_READONLY equ 2
STORE_SEARCH_OK equ 4
STORE_MODIFY_OK equ 8
STORE_CREATE_OK equ 10h
STORE_ATTACH_OK equ 20h
STORE_OLE_OK equ 40h
STORE_SUBMIT_OK equ 80h
STORE_NOTIFY_OK equ 100h
STORE_MV_PROPS_OK equ 200h
STORE_CATEGORIZE_OK equ 400h
STORE_RTF_OK equ 800h
STORE_RESTRICTION_OK equ 1000h
STORE_SORT_OK equ 2000h
STORE_PUBLIC_FOLDERS equ 4000h
STORE_UNCOMPRESSED_RTF equ 8000h
STORE_HAS_SEARCHES equ 1000000h
LOGOFF_NO_WAIT equ 1
LOGOFF_ORDERLY equ 2
LOGOFF_PURGE equ 4
LOGOFF_ABORT equ 8
LOGOFF_QUIET equ 10h
LOGOFF_COMPLETE equ 10000h
LOGOFF_INBOUND equ 20000h
LOGOFF_OUTBOUND equ 40000h
LOGOFF_OUTBOUND_QUEUE equ 80000h
MSG_LOCKED equ 1
MSG_UNLOCKED equ 0
FOLDER_IPM_SUBTREE_VALID equ 1
FOLDER_IPM_INBOX_VALID equ 2
FOLDER_IPM_OUTBOX_VALID equ 4
FOLDER_IPM_WASTEBASKET_VALID equ 8
FOLDER_IPM_SENTMAIL_VALID equ 10h
FOLDER_VIEWS_VALID equ 20h
FOLDER_COMMON_VIEWS_VALID equ 40h
FOLDER_FINDER_VALID equ 80h
FORCE_SUBMIT equ 1
MSGFLAG_READ equ 1
MSGFLAG_UNMODIFIED equ 2
MSGFLAG_SUBMIT equ 4
MSGFLAG_UNSENT equ 8
MSGFLAG_HASATTACH equ 10h
MSGFLAG_FROMME equ 20h
MSGFLAG_ASSOCIATED equ 40h
MSGFLAG_RESEND equ 80h
MSGFLAG_RN_PENDING equ 100h
MSGFLAG_NRN_PENDING equ 200h
SUBMITFLAG_LOCKED equ 1
SUBMITFLAG_PREPROCESS equ 2
MODRECIP_ADD equ 2
MODRECIP_MODIFY equ 4
MODRECIP_REMOVE equ 8
SUPPRESS_RECEIPT equ 1
CLEAR_READ_FLAG equ 4
GENERATE_RECEIPT_ONLY equ 10h
CLEAR_RN_PENDING equ 20h
CLEAR_NRN_PENDING equ 40h
ATTACH_DIALOG equ 1
SECURITY_SIGNED equ 1
SECURITY_ENCRYPTED equ 2
PRIO_URGENT equ 1
PRIO_NORMAL equ 0
PRIO_NONURGENT equ -1
SENSITIVITY_NONE equ 0
SENSITIVITY_PERSONAL equ 1
SENSITIVITY_PRIVATE equ 2
SENSITIVITY_COMPANY_CONFIDENTIAL equ 3
IMPORTANCE_LOW equ 0
IMPORTANCE_NORMAL equ 1
IMPORTANCE_HIGH equ 2
NO_ATTACHMENT equ 0
ATTACH_BY_VALUE equ 1
ATTACH_BY_REFERENCE equ 2
ATTACH_BY_REF_RESOLVE equ 3
ATTACH_BY_REF_ONLY equ 4
ATTACH_EMBEDDED_MSG equ 5
ATTACH_OLE equ 6
ADRPARM_HELP_CTX equ 0
DIALOG_MODAL equ 1
DIALOG_SDI equ 2
DIALOG_OPTIONS equ 4
ADDRESS_ONE equ 8
AB_SELECTONLY equ 10h
AB_RESOLVE equ 20h
DT_MAILUSER equ 0
DT_DISTLIST equ 1
DT_FORUM equ 2
DT_AGENT equ 3
DT_ORGANIZATION equ 4
DT_PRIVATE_DISTLIST equ 5
DT_REMOTE_MAILUSER equ 6
DT_MODIFIABLE equ 10000h
DT_GLOBAL equ 20000h
DT_LOCAL equ 30000h
DT_WAN equ 40000h
DT_NOT_SPECIFIC equ 50000h
DT_FOLDER equ 1000000h
DT_FOLDER_LINK equ 2000000h
DT_FOLDER_SPECIAL equ 4000000h
MAPI_DEFERRED_ERRORS equ 8h
MAPI_ASSOCIATED equ 40h
MDB_NO_DIALOG equ 1h
MDB_WRITE equ 4h
MDB_TEMPORARY equ 20h
MDB_NO_MAIL equ 80h
AB_NO_DIALOG equ 1
MAPI_ENABLED equ 0
MAPI_DISABLED equ 1
DT_MULTILINE equ 1
DT_EDITABLE equ 2
DT_REQUIRED equ 4
DT_SET_IMMEDIATE equ 8
DT_PASSWORD_EDIT equ 10h
DT_ACCEPT_DBCS equ 20h
DT_SET_SELECTION equ 40h
DTCT_LABEL equ 0
DTCT_EDIT equ 1
DTCT_LBX equ 2
DTCT_COMBOBOX equ 3
DTCT_DDLBX equ 4
DTCT_CHECKBOX equ 5
DTCT_GROUPBOX equ 6
DTCT_BUTTON equ 7
DTCT_PAGE equ 8
DTCT_RADIOBUTTON equ 9
DTCT_MVLISTBOX equ 0Bh
DTCT_MVDDLBX equ 0Ch
MAPI_NO_HBAR equ 1
MAPI_NO_VBAR equ 2
UI_SERVICE equ 2
SERVICE_UI_ALWAYS equ 2
SERVICE_UI_ALLOWED equ 10h
UI_CURRENT_PROVIDER_FIRST equ 4

CY STRUCT
    struct 
        Lo dd ?
        Hi dd ?
    ENDS
    int64 dq ?
CY ENDS

CURRENCY TEXTEQU <CY>

ENTRYID STRUCT
    abFlags db 4 dup(?)
    ab db MAPI_DIM dup(?)
ENTRYID ENDS

MAPIUID STRUCT
    ab db 16 dup(?)
MAPIUID ENDS

SPropTagArray STRUCT
    cValues dd ?
    aulPropTag dd MAPI_DIM dup(?)
SPropTagArray ENDS

SBinary STRUCT
    cb dd ?
    lpb dd ?
SBinary ENDS

SShortArray STRUCT
    cValues dd ?
    lpi dd ?
SShortArray ENDS

SGuidArray STRUCT
    cValues dd ?
    lpguid dd ?
SGuidArray ENDS

SRealArray STRUCT
    cValues dd ?
    lpflt dd ?
SRealArray ENDS

SLongArray STRUCT
    cValues dd ?
    lpl dd ?
SLongArray ENDS

SLargeIntegerArray STRUCT
    cValues dd ?
    lpli dd ?
SLargeIntegerArray ENDS

SDateTimeArray STRUCT
    cValues dd ?
	 lpft dd ?
SDateTimeArray ENDS

SAppTimeArray STRUCT
    cValues dd ?
    lpat dd ?
SAppTimeArray ENDS

SCurrencyArray STRUCT
    cValues dd ?
    lpcur dd ?
SCurrencyArray ENDS

SBinaryArray STRUCT
    cValues dd ?
    lpbin dd ?
SBinaryArray ENDS

SDoubleArray STRUCT
    cValues dd ?
    lpdbl dd ?
SDoubleArray ENDS

SWStringArray STRUCT
    cValues dd ?
    lppszW dd ?
SWStringArray ENDS

SLPSTRArray STRUCT
    cValues dd ?
    lppszA dd ?
SLPSTRArray ENDS

UPV UNION DWORD
    i dw ?
    l dd ?
    ul dd ?
    flt dd ?
    dbl dq ?
    b dw ?
    cur CURRENCY <>
    at dq ?
    ft FILETIME <>
    lpszA dd ?
    bin SBinary <>
    lpszW dd ?
    lpguid dd ?
    li LARGE_INTEGER <>
    MVi SShortArray <>
    MVl SLongArray <>
    MVflt SRealArray <>
    MVdbl SDoubleArray <>
    MVcur SCurrencyArray <>
    MVat SAppTimeArray <>
    MVft SDateTimeArray <>
    MVbin SBinaryArray <>
    MVszA SLPSTRArray <>
    MVszW SWStringArray <>
    MVguid SGuidArray <>
    MVli SLargeIntegerArray <>
    err SCODE ?
    x LONG ?
UPV ENDS

SPropValue STRUCT
    ulPropTag dd ?
    dwAlignPad dd ?
    Value UPV <>
SPropValue ENDS

SPropProblem STRUCT
    ulIndex dd ?
    ulPropTag dd ?
    scode SCODE ?
SPropProblem ENDS

SPropProblemArray STRUCT
    cProblem dd ?
    aProblem SPropProblem MAPI_DIM dup(<>)
SPropProblemArray ENDS

FLATENTRY STRUCT
    cb dd ?
    abEntry db MAPI_DIM dup(?)
FLATENTRY ENDS

FLATENTRYLIST STRUCT
    cEntries dd ?
    cbEntries dd ?
    abEntries db MAPI_DIM dup(?)
FLATENTRYLIST ENDS

MTSID STRUCT
    cb dd ?
    ab db MAPI_DIM dup(?)
MTSID ENDS

FLATMTSIDLIST STRUCT
    cMTSIDs dd ?
    cbMTSIDs dd ?
    abMTSIDs db MAPI_DIM dup(?)
FLATMTSIDLIST ENDS

ADRENTRY STRUCT
    ulReserved1 dd ?
    cValues dd ?
    rgPropVals dd ?
ADRENTRY ENDS

ADRLIST STRUCT
    cEntries dd ?
    aEntries ADRENTRY MAPI_DIM dup(<>)
ADRLIST ENDS

SRow STRUCT
    ulAdrEntryPad dd ?
    cValues dd ?
    lpProps dd ?
SRow ENDS

SRowSet STRUCT
    cRows dd ?
    aRow SRow MAPI_DIM dup(<>)
SRowSet ENDS

MAPIERROR STRUCT
    ulVersion dd ?
    lpszError dd ?
    lpszComponent dd ?
    ulLowLevelError dd ?
    ulContext dd ?
MAPIERROR ENDS

ERROR_NOTIFICATION STRUCT
    cbEntryID dd ?
    lpEntryID dd ?
    scode SCODE ?
    ulFlags dd ?
    lpMAPIError dd ?
ERROR_NOTIFICATION ENDS

NEWMAIL_NOTIFICATION STRUCT
    cbEntryID dd ?
    lpEntryID dd ?
    cbParentID dd ?
    lpParentID dd ?
    ulFlags dd ?
    lpszMessageClass dd ?
    ulMessageFlags dd ?
NEWMAIL_NOTIFICATION ENDS

OBJECT_NOTIFICATION STRUCT
    cbEntryID dd ?
    lpEntryID dd ?
    ulObjType dd ?
    cbParentID dd ?
    lpParentID dd ?
    cbOldID dd ?
    lpOldID dd ?
    cbOldParentID dd ?
    lpOldParentID dd ?
    lpPropTagArray dd ?
OBJECT_NOTIFICATION ENDS

TABLE_NOTIFICATION STRUCT
    ulTableEvent dd ?
    hResult dd ?
    propIndex SPropValue <>
    propPrior SPropValue <>
    row SRow <>
    ulPad dd ?
TABLE_NOTIFICATION ENDS

EXTENDED_NOTIFICATION STRUCT
    ulEvent dd ?
    cb dd ?
    pbEventParameters dd ?
EXTENDED_NOTIFICATION ENDS

STATUS_OBJECT_NOTIFICATION STRUCT
    cbEntryID dd ?
    lpEntryID dd ?
    cValues dd ?
    lpPropVals dd ?
STATUS_OBJECT_NOTIFICATION ENDS

NOTIFICATION STRUCT
    ulEventType dd ?
    ulAlignPad dd ?
    union info
        err ERROR_NOTIFICATION <>
        newmail NEWMAIL_NOTIFICATION <>
        obj OBJECT_NOTIFICATION <>
        tab TABLE_NOTIFICATION <>
        ext EXTENDED_NOTIFICATION <>
        statobj STATUS_OBJECT_NOTIFICATION <>
    ends
NOTIFICATION ENDS

MAPINAMEID STRUCT
    lpguid dd ?
    ulKind dd ?
    union Kind
        lID dd ?
        lpwstrName dd ?
    ends
MAPINAMEID ENDS

SSortOrder STRUCT
    ulPropTag dd ?
    ulOrder dd ?
SSortOrder ENDS

SSortOrderSet STRUCT
    cSorts dd ?
    cCategories dd ?
    cExpanded dd ?
    aSort SSortOrder MAPI_DIM dup(<>)
SSortOrderSet ENDS

SAndRestriction STRUCT
    cRes dd ?
    lpRes dd ?
SAndRestriction ENDS

SOrRestriction STRUCT
    cRes dd ?
    lpRes dd ?
SOrRestriction ENDS

SNotRestriction STRUCT
    ulReserved dd ?
    lpRes dd ?
SNotRestriction ENDS

SContentRestriction STRUCT
    ulFuzzyLevel dd ?
    ulPropTag dd ?
    lpProp dd ?
SContentRestriction ends

SBitMaskRestriction STRUCT
    relBMR dd ?
    ulPropTag dd ?
    ulMask dd ?
SBitMaskRestriction ENDS

SPropertyRestriction STRUCT
    relop dd ?
    ulPropTag dd ?
    lpProp dd ?
SPropertyRestriction ENDS

SComparePropsRestriction STRUCT
    relop dd ?
    ulPropTag1 dd ?
    ulPropTag2 dd ?
SComparePropsRestriction ENDS

SSizeRestriction STRUCT
    relop dd ?
    ulPropTag dd ?

    cb dd ?
SSizeRestriction ENDS

SExistRestriction STRUCT
    ulReserved1 dd ?
    ulPropTag dd ?
    ulReserved2 dd ?
SExistRestriction ENDS

SSubRestriction STRUCT
    ulSubObject dd ?
    lpRes dd ?
SSubRestriction ENDS

SCommentRestriction STRUCT
    cValues dd ?
    lpRes dd ?
    lpProp dd ?
SCommentRestriction ENDS

SRestriction STRUCT
    rt dd ?
    union res
        resCompareProps SComparePropsRestriction <>
        resAnd SAndRestriction <>
        resOr SOrRestriction <>
        resNot SNotRestriction <>
        resContent SContentRestriction <>
        resProperty SPropertyRestriction <>
        resBitMask SBitMaskRestriction <>
        resSize SSizeRestriction <>
        resExist SExistRestriction <>
        resSub SSubRestriction <>
        resComment SCommentRestriction <>
	ends
SRestriction ENDS

FlagList STRUCT
    cFlags dd ?
    ulFlag dd MAPI_DIM dup(?)
FlagList ENDS

ADRPARM STRUCT
    cbABContEntryID dd ?
    lpABContEntryID dd ?
    ulFlags dd ?
    lpReserved dd ?
    ulHelpContext dd ?
    lpszHelpFileName dd ?
    lpfnABSDI dd ?
    lpfnDismiss dd ?
    lpvDismissContext dd ?
    lpszCaption dd ?
    lpszNewEntryTitle dd ?
    lpszDestWellsTitle dd ?
    cDestFields dd ?
    nDestFieldFocus dd ?
    lppszDestTitles dd ?
    lpulDestComps dd ?
    lpContRestriction dd ?
    lpHierRestriction dd ?
ADRPARM ENDS

DTBLLABEL STRUCT
    ulbLpszLabelName dd ?
    ulFlags dd ?
DTBLLABEL ENDS

DTBLEDIT STRUCT
    ulbLpszCharsAllowed dd ?
    ulFlags dd ?
    ulNumCharsAllowed dd ?
    ulPropTag dd ?
DTBLEDIT ENDS

DTBLLBX STRUCT
    ulFlags dd ?
    ulPRSetProperty dd ?
    ulPRTableName dd ?
DTBLLBX ENDS

DTBLCOMBOBOX STRUCT
    ulbLpszCharsAllowed dd ?
    ulFlags dd ?
    ulNumCharsAllowed dd ?
    ulPRPropertyName dd ?
    ulPRTableName dd ?
DTBLCOMBOBOX ENDS

DTBLDDLBX STRUCT
    ulFlags dd ?
    ulPRDisplayProperty dd ?
    ulPRSetProperty dd ?
    ulPRTableName dd ?
DTBLDDLBX ENDS

DTBLCHECKBOX STRUCT
    ulbLpszLabel dd ?
    ulFlags dd ?
    ulPRPropertyName dd ?
DTBLCHECKBOX ENDS

DTBLGROUPBOX STRUCT
    ulbLpszLabel dd ?
    ulFlags dd ?
DTBLGROUPBOX ENDS

DTBLBUTTON STRUCT
    ulbLpszLabel dd ?
    ulFlags dd ?
    ulPRControl dd ?
DTBLBUTTON ENDS

DTBLPAGE STRUCT
    ulbLpszLabel dd ?
    ulFlags dd ?
    ulbLpszComponent dd ?
    ulContext dd ?
DTBLPAGE ENDS

DTBLRADIOBUTTON STRUCT
    ulbLpszLabel dd ?
    ulFlags dd ?
    ulcButtons dd ?
    ulPropTag dd ?
    lReturnValue dd ?
DTBLRADIOBUTTON ENDS

DTBLMVLISTBOX STRUCT
    ulFlags dd ?
    ulMVPropTag dd ?
DTBLMVLISTBOX ENDS

DTBLMVDDLBX STRUCT
    ulFlags dd ?
    ulMVPropTag dd ?
DTBLMVDDLBX ENDS

;-----------------------------------[ mapiform.h ]-------------------------------------
HFRMREG_DEFAULT equ 0
HFRMREG_LOCAL equ 1
HFRMREG_PERSONAL equ 2
HFRMREG_FOLDER equ 3
SAVE_FORMAT_TEXT equ 1
SAVE_FORMAT_RICHTEXT equ 2
VCSTATUS_NEXT equ 1
VCSTATUS_PREV equ 2
VCSTATUS_MODAL equ 4
VCSTATUS_INTERACTIVE equ 8
VCSTATUS_READONLY equ 10h
VCSTATUS_DELETE equ 10000h
VCSTATUS_COPY equ 20000h
VCSTATUS_MOVE equ 40000h
VCSTATUS_SUBMIT equ 80000h
VCSTATUS_DELETE_IS_MOVE equ 100000h
VCSTATUS_SAVE equ 200000h
VCSTATUS_NEW_MESSAGE equ 400000h
VCDIR_NEXT equ VCSTATUS_NEXT
VCDIR_PREV equ VCSTATUS_PREV
VCDIR_DELETE equ VCSTATUS_DELETE
VCDIR_MOVE equ VCSTATUS_MOVE
FPST_VANILLA equ 0
FPST_ENUM_PROP equ 1
MAPI_MESSAGE_BEHAVIOR_IPM equ 0
MAPI_MESSAGE_BEHAVIOR_FOLDER equ 1
MAPIFORM_SELECT_ALL_REGISTRIES equ 0
MAPIFORM_SELECT_FOLDER_REGISTRY_ONLY equ 1
MAPIFORM_SELECT_NON_FOLDER_REGISTRY_ONLY equ 2
FORMPROPSET_UNION equ 0
FORMPROPSET_INTERSECTION equ 1
MAPIFORM_EXACTMATCH equ 20h
MAPIFORM_CPU_X86 equ 1
MAPIFORM_CPU_MIP equ 2
MAPIFORM_CPU_AXP equ 3
MAPIFORM_CPU_PPC equ 4
MAPIFORM_CPU_M68 equ 5
MAPIFORM_OS_WIN_31 equ 1
MAPIFORM_OS_WINNT_35 equ 2
MAPIFORM_OS_WIN_95 equ 3
MAPIFORM_OS_MAC_7x equ 4
MAPIFORM_OS_WINNT_40 equ 5
MAPIFORM_INSTALL_DIALOG equ MAPI_DIALOG
MAPIFORM_INSTALL_OVERWRITEONCONFLICT equ 10h

SAVEOPTS_SAVEIFDIRTY equ 0
SAVEOPTS_NOSAVE equ 1
SAVEOPTS_PROMPTSAVE equ 2

FORMPRINTSETUP STRUCT
    ulFlags dd ?
    hDevMode dd ?
    hDevNames dd ?
    ulFirstPageNumber dd ?
    fPrintAttachments dd ?
FORMPRINTSETUP ENDS

SMAPIFormPropEnumVal STRUCT
    pszDisplayName dd ?
    nVal dd ?
SMAPIFormPropEnumVal ENDS

SMAPIFormProp STRUCT
    ulFlags dd ?
    nPropType dd ?
    nmid MAPINAMEID <>
    pszDisplayName dd ?
    nSpecialType dd ?
    union u
        struct sl
            nmidIdx MAPINAMEID <>
            cfpevAvailable dd ?
            pfpevAvailable dd ?
		  ends
	 ends
SMAPIFormProp ENDS

SMAPIFormPropArray STRUCT
    cProps dd ?
    ulPad dd ?
    aFormProp SMAPIFormProp MAPI_DIM dup(<>)
SMAPIFormPropArray ENDS

SMAPIVerb STRUCT
    lVerb dd ?
    szVerbname dd ?
    fuFlags dd ?
    grfAttribs dd ?
    ulFlags dd ?
SMAPIVerb ENDS

SMAPIVerbArray STRUCT
    cMAPIVerb dd ?
    aMAPIVerb SMAPIVerb MAPI_DIM dup(<>)
SMAPIVerbArray ENDS


SMessageClassArray STRUCT
    cValues dd ?
    aMessageClass dd MAPI_DIM dup(?)
SMessageClassArray ENDS

SMAPIFormInfoArray STRUCT
    cForms dd ?
    aFormInfo dd MAPI_DIM dup(?)
SMAPIFormInfoArray ENDS

;--------------------------------[ mapinls.h ]---------------------------
NORM_IGNORECASE equ 1
NORM_IGNORENONSPACE equ 2
NORM_IGNORESYMBOLS equ 4
NORM_IGNOREKANATYPE equ 10000h
NORM_IGNOREWIDTH equ 20000h
CP_ACP equ 0
CP_OEMCP equ 1

;---------------------------[ mapioid.h ]-------------------------------
OID_TAG         equ 0Ah
OID_ENCODING    equ 0Bh
CB_OID_1        equ 9
CB_OID_2        equ 10
CB_OID_3        equ 11
CB_OID_4        equ 12

;--------------------------[ mapispi.h ]--------------------------------

CURRENT_SPI_VERSION equ 10010h
PDK1_SPI_VERSION equ 10000h
PDK2_SPI_VERSION equ 10008h
PDK3_SPI_VERSION equ 10010h
NOTIFY_SYNC equ 40000000h
NOTIFY_CANCELED equ 80000000h
CALLBACK_DISCONTINUE equ 80000000h
NOTIFY_NEWMAIL equ 1
NOTIFY_READYTOSEND equ 2
NOTIFY_SENTDEFERRED equ 4
NOTIFY_CRITSEC equ 1000h
NOTIFY_NONCRIT equ 2000h
NOTIFY_CONFIG_CHANGE equ 4000h
NOTIFY_CRITICAL_ERROR equ 10000000h
NOTIFY_NEWMAIL_RECEIVED equ 20000000h
STATUSROW_UPDATE equ 10000000h
STGSTRM_RESET equ 0
STGSTRM_CURRENT equ 10000000h
STGSTRM_MODIFY equ 2
STGSTRM_CREATE equ 1000h
MAPI_NON_READ equ 1
FILL_ENTRY equ 1
DEINIT_NORMAL equ 1
DEINIT_HURRY equ 80000000h
LOGON_NO_DIALOG equ 1
LOGON_NO_CONNECT equ 4
LOGON_NO_INBOUND equ 8
LOGON_NO_OUTBOUND equ 10h
LOGON_SP_IDLE equ 10000h
LOGON_SP_POLL equ 20000h
LOGON_SP_RESOLVE equ 40000h
OPTION_TYPE_RECIPIENT equ 1
OPTION_TYPE_MESSAGE equ 2
NOTIFY_ABORT_DEFERRED equ 40000000h
NOTIFY_CANCEL_MESSAGE equ 80000000h
NOTIFY_BEGIN_INBOUND equ 1
NOTIFY_END_INBOUND equ 10000h
NOTIFY_BEGIN_OUTBOUND equ 2
NOTIFY_END_OUTBOUND equ 20000h
NOTIFY_BEGIN_INBOUND_FLUSH equ 4
NOTIFY_END_INBOUND_FLUSH equ 40000h
NOTIFY_BEGIN_OUTBOUND_FLUSH equ 8
NOTIFY_END_OUTBOUND_FLUSH equ 80000h
LOGOFF_NORMAL equ 1
LOGOFF_HURRY equ 80000000h
BEGIN_DEFERRED equ 1
END_RESEND_NOW equ 10000h
END_RESEND_LATER equ 20000h
END_DONT_RESEND equ 40000h
LOGON_SP_TRANSPORT equ 1
LOGON_SP_PROMPT equ 2
LOGON_SP_NEWPW equ 4
LOGON_CHANGED equ 8
DIALOG_FOLDER equ 1
DIALOG_MESSAGE equ 2
DIALOG_PROP equ 4
DIALOG_ATTACH equ 8
DIALOG_MOVE equ 10h
DIALOG_COPY equ 20h
DIALOG_DELETE equ 40h
DIALOG_ALLOW_CANCEL equ 80h
DIALOG_CONFIRM_CANCEL equ 100h
NEEDS_PREPROCESSING equ 1
NEEDS_SPOOLER equ 2
CHECK_SENDER equ 1
NON_STANDARD equ 10000h
MSG_SERVICE_UI_READ_ONLY equ 8
SERVICE_LOGON_FAILED equ 20h
MSG_SERVICE_INSTALL equ 1
MSG_SERVICE_CREATE equ 2
MSG_SERVICE_CONFIGURE equ 3
MSG_SERVICE_DELETE equ 4
MSG_SERVICE_UNINSTALL equ 5
MSG_SERVICE_PROVIDER_CREATE equ 6
MSG_SERVICE_PROVIDER_DELETE equ 7

NOTIFKEY STRUCT
    cb dd ?
    ab db MAPI_DIM dup(?)
NOTIFKEY ENDS

OPTIONDATA STRUCT
    ulFlags dd ?
    lpRecipGUID dd ?
    lpszAdrType dd ?
    lpszDLLName dd ?
    ulOrdinal dd ?
    cbOptionsData dd ?
    lpbOptionsData dd ?
    cOptionsProps dd ?
    lpOptionsProps dd ?
OPTIONDATA ENDS

;--------------------------------[ mapitags.h ]-----------------------------------

PR_ACKNOWLEDGEMENT_MODE equ (PT_LONG shl 16)+ 1
PR_ALTERNATE_RECIPIENT_ALLOWED equ ( PT_BOOLEAN shl 16)+   2
PR_AUTHORIZING_USERS equ ( PT_BINARY shl 16)+    3
PR_AUTO_FORWARD_COMMENT equ ( PT_TSTRING shl 16)+   4
PR_AUTO_FORWARD_COMMENT_W equ ( PT_UNICODE shl 16)+   4
PR_AUTO_FORWARD_COMMENT_A equ ( PT_STRING8 shl 16)+   4
PR_AUTO_FORWARDED equ ( PT_BOOLEAN shl 16)+   5
PR_CONTENT_CONFIDENTIALITY_ALGORITHM_ID equ ( PT_BINARY shl 16)+    6
PR_CONTENT_CORRELATOR equ ( PT_BINARY shl 16)+    7
PR_CONTENT_IDENTIFIER equ ( PT_TSTRING shl 16)+   8
PR_CONTENT_IDENTIFIER_W equ ( PT_UNICODE shl 16)+   8
PR_CONTENT_IDENTIFIER_A equ ( PT_STRING8 shl 16)+   8
PR_CONTENT_LENGTH equ ( PT_LONG shl 16)+      9
PR_CONTENT_RETURN_REQUESTED equ ( PT_BOOLEAN shl 16)+   0Ah
PR_CONVERSATION_KEY equ ( PT_BINARY shl 16)+    0Bh
PR_CONVERSION_EITS equ ( PT_BINARY shl 16)+    0Ch
PR_CONVERSION_WITH_LOSS_PROHIBITED equ ( PT_BOOLEAN shl 16)+   0Dh
PR_CONVERTED_EITS equ ( PT_BINARY shl 16)+    0Eh
PR_DEFERRED_DELIVERY_TIME equ ( PT_SYSTIME shl 16)+   0Fh
PR_DELIVER_TIME equ ( PT_SYSTIME shl 16)+   10h
PR_DISCARD_REASON equ ( PT_LONG shl 16)+      11h
PR_DISCLOSURE_OF_RECIPIENTS equ ( PT_BOOLEAN shl 16)+   12h
PR_DL_EXPANSION_HISTORY equ ( PT_BINARY shl 16)+    13h
PR_DL_EXPANSION_PROHIBITED equ ( PT_BOOLEAN shl 16)+   14h
PR_EXPIRY_TIME equ ( PT_SYSTIME shl 16)+   15h
PR_IMPLICIT_CONVERSION_PROHIBITED equ ( PT_BOOLEAN shl 16)+   16h
PR_IMPORTANCE equ ( PT_LONG shl 16)+      17h
PR_IPM_ID equ ( PT_BINARY shl 16)+    18h
PR_LATEST_DELIVERY_TIME equ ( PT_SYSTIME shl 16)+   19h
PR_MESSAGE_CLASS equ ( PT_TSTRING shl 16)+   1Ah
PR_MESSAGE_CLASS_W equ ( PT_UNICODE shl 16)+   1Ah
PR_MESSAGE_CLASS_A equ ( PT_STRING8 shl 16)+   1Ah
PR_MESSAGE_DELIVERY_ID equ ( PT_BINARY shl 16)+    1Bh
PR_MESSAGE_SECURITY_LABEL equ ( PT_BINARY shl 16)+    1Eh
PR_OBSOLETED_IPMS equ ( PT_BINARY shl 16)+    1Fh
PR_ORIGINALLY_INTENDED_RECIPIENT_NAME equ ( PT_BINARY shl 16)+    20h
PR_ORIGINAL_EITS equ ( PT_BINARY shl 16)+    21h
PR_ORIGINATOR_CERTIFICATE equ ( PT_BINARY shl 16)+    22h
PR_ORIGINATOR_DELIVERY_REPORT_REQUESTED equ ( PT_BOOLEAN shl 16)+   23h
PR_ORIGINATOR_RETURN_ADDRESS equ ( PT_BINARY shl 16)+    24h
PR_PARENT_KEY equ ( PT_BINARY shl 16)+    25h
PR_PRIORITY equ ( PT_LONG shl 16)+      26h
PR_ORIGIN_CHECK equ ( PT_BINARY shl 16)+    27h
PR_PROOF_OF_SUBMISSION_REQUESTED equ ( PT_BOOLEAN shl 16)+   28h
PR_READ_RECEIPT_REQUESTED equ ( PT_BOOLEAN shl 16)+   29h
PR_RECEIPT_TIME equ ( PT_SYSTIME shl 16)+   2Ah
PR_RECIPIENT_REASSIGNMENT_PROHIBITED equ ( PT_BOOLEAN shl 16)+   2Bh
PR_REDIRECTION_HISTORY equ ( PT_BINARY shl 16)+    2Ch
PR_RELATED_IPMS equ ( PT_BINARY shl 16)+    2Dh
PR_ORIGINAL_SENSITIVITY equ ( PT_LONG shl 16)+      2Eh
PR_LANGUAGES equ ( PT_TSTRING shl 16)+   2Fh
PR_LANGUAGES_W equ ( PT_UNICODE shl 16)+   2Fh
PR_LANGUAGES_A equ ( PT_STRING8 shl 16)+   2Fh
PR_REPLY_TIME equ ( PT_SYSTIME shl 16)+   30h
PR_REPORT_TAG equ ( PT_BINARY shl 16)+    31h
PR_REPORT_TIME equ ( PT_SYSTIME shl 16)+   32h
PR_RETURNED_IPM equ ( PT_BOOLEAN shl 16)+   33h
PR_SECURITY equ ( PT_LONG shl 16)+      34h
PR_INCOMPLETE_COPY equ ( PT_BOOLEAN shl 16)+   35h
PR_SENSITIVITY equ ( PT_LONG shl 16)+      36h
PR_SUBJECT equ ( PT_TSTRING shl 16)+   37h
PR_SUBJECT_W equ ( PT_UNICODE shl 16)+   37h
PR_SUBJECT_A equ ( PT_STRING8 shl 16)+   37h
PR_SUBJECT_IPM equ ( PT_BINARY shl 16)+    38h
PR_CLIENT_SUBMIT_TIME equ ( PT_SYSTIME shl 16)+   39h
PR_REPORT_NAME equ ( PT_TSTRING shl 16)+   3Ah
PR_REPORT_NAME_W equ ( PT_UNICODE shl 16)+   3Ah
PR_REPORT_NAME_A equ ( PT_STRING8 shl 16)+   3Ah
PR_SENT_REPRESENTING_SEARCH_KEY equ ( PT_BINARY shl 16)+    3Bh
PR_X400_CONTENT_TYPE equ ( PT_BINARY shl 16)+    3Ch
PR_SUBJECT_PREFIX equ ( PT_TSTRING shl 16)+   3Dh
PR_SUBJECT_PREFIX_W equ ( PT_UNICODE shl 16)+   3Dh
PR_SUBJECT_PREFIX_A equ ( PT_STRING8 shl 16)+   3Dh
PR_NON_RECEIPT_REASON equ ( PT_LONG shl 16)+      3Eh
PR_RECEIVED_BY_ENTRYID equ ( PT_BINARY shl 16)+    3Fh
PR_RECEIVED_BY_NAME equ ( PT_TSTRING shl 16)+   40h
PR_RECEIVED_BY_NAME_W equ ( PT_UNICODE shl 16)+   40h
PR_RECEIVED_BY_NAME_A equ ( PT_STRING8 shl 16)+   40h
PR_SENT_REPRESENTING_ENTRYID equ ( PT_BINARY shl 16)+    41h
PR_SENT_REPRESENTING_NAME equ ( PT_TSTRING shl 16)+   42h
PR_SENT_REPRESENTING_NAME_W equ ( PT_UNICODE shl 16)+   42h
PR_SENT_REPRESENTING_NAME_A equ ( PT_STRING8 shl 16)+   42h
PR_RCVD_REPRESENTING_ENTRYID equ ( PT_BINARY shl 16)+    43h
PR_RCVD_REPRESENTING_NAME equ ( PT_TSTRING shl 16)+   44h
PR_RCVD_REPRESENTING_NAME_W equ ( PT_UNICODE shl 16)+   44h
PR_RCVD_REPRESENTING_NAME_A equ ( PT_STRING8 shl 16)+   44h
PR_REPORT_ENTRYID equ ( PT_BINARY shl 16)+    45h
PR_READ_RECEIPT_ENTRYID equ ( PT_BINARY shl 16)+    46h
PR_MESSAGE_SUBMISSION_ID equ ( PT_BINARY shl 16)+    47h
PR_PROVIDER_SUBMIT_TIME equ ( PT_SYSTIME shl 16)+   48h
PR_ORIGINAL_SUBJECT equ ( PT_TSTRING shl 16)+   49h
PR_ORIGINAL_SUBJECT_W equ ( PT_UNICODE shl 16)+   49h
PR_ORIGINAL_SUBJECT_A equ ( PT_STRING8 shl 16)+   49h
PR_DISC_VAL equ ( PT_BOOLEAN shl 16)+   4Ah
PR_ORIG_MESSAGE_CLASS equ ( PT_TSTRING shl 16)+   4Bh
PR_ORIG_MESSAGE_CLASS_W equ ( PT_UNICODE shl 16)+   4Bh
PR_ORIG_MESSAGE_CLASS_A equ ( PT_STRING8 shl 16)+   4Bh
PR_ORIGINAL_AUTHOR_ENTRYID equ ( PT_BINARY shl 16)+    4Ch
PR_ORIGINAL_AUTHOR_NAME equ ( PT_TSTRING shl 16)+   4Dh
PR_ORIGINAL_AUTHOR_NAME_W equ ( PT_UNICODE shl 16)+   4Dh
PR_ORIGINAL_AUTHOR_NAME_A equ ( PT_STRING8 shl 16)+   4Dh
PR_ORIGINAL_SUBMIT_TIME equ ( PT_SYSTIME shl 16)+   4Eh
PR_REPLY_RECIPIENT_ENTRIES equ ( PT_BINARY shl 16)+    4Fh
PR_REPLY_RECIPIENT_NAMES equ ( PT_TSTRING shl 16)+   50h
PR_REPLY_RECIPIENT_NAMES_W equ ( PT_UNICODE shl 16)+   50h
PR_REPLY_RECIPIENT_NAMES_A equ ( PT_STRING8 shl 16)+   50h
PR_RECEIVED_BY_SEARCH_KEY equ ( PT_BINARY shl 16)+    51h
PR_RCVD_REPRESENTING_SEARCH_KEY equ ( PT_BINARY shl 16)+    52h
PR_READ_RECEIPT_SEARCH_KEY equ ( PT_BINARY shl 16)+    53h
PR_REPORT_SEARCH_KEY equ ( PT_BINARY shl 16)+    54h
PR_ORIGINAL_DELIVERY_TIME equ ( PT_SYSTIME shl 16)+   55h
PR_ORIGINAL_AUTHOR_SEARCH_KEY equ ( PT_BINARY shl 16)+    56h
PR_MESSAGE_TO_ME equ ( PT_BOOLEAN shl 16)+   57h
PR_MESSAGE_CC_ME equ ( PT_BOOLEAN shl 16)+   58h
PR_MESSAGE_RECIP_ME equ ( PT_BOOLEAN shl 16)+   59h
PR_ORIGINAL_SENDER_NAME equ ( PT_TSTRING shl 16)+   5Ah
PR_ORIGINAL_SENDER_NAME_W equ ( PT_UNICODE shl 16)+   5Ah
PR_ORIGINAL_SENDER_NAME_A equ ( PT_STRING8 shl 16)+   5Ah
PR_ORIGINAL_SENDER_ENTRYID equ ( PT_BINARY shl 16)+    5Bh
PR_ORIGINAL_SENDER_SEARCH_KEY equ ( PT_BINARY shl 16)+    5Ch
PR_ORIGINAL_SENT_REPRESENTING_NAME equ ( PT_TSTRING shl 16)+   5Dh
PR_ORIGINAL_SENT_REPRESENTING_NAME_W equ ( PT_UNICODE shl 16)+   5Dh
PR_ORIGINAL_SENT_REPRESENTING_NAME_A equ ( PT_STRING8 shl 16)+   5Dh
PR_ORIGINAL_SENT_REPRESENTING_ENTRYID equ ( PT_BINARY shl 16)+    5Eh
PR_ORIGINAL_SENT_REPRESENTING_SEARCH_KEY equ ( PT_BINARY shl 16)+    5Fh
PR_START_DATE equ ( PT_SYSTIME shl 16)+   60h
PR_END_DATE equ ( PT_SYSTIME shl 16)+   61h
PR_OWNER_APPT_ID equ ( PT_LONG shl 16)+      62h
PR_RESPONSE_REQUESTED equ ( PT_BOOLEAN shl 16)+   63h
PR_SENT_REPRESENTING_ADDRTYPE equ ( PT_TSTRING shl 16)+   64h
PR_SENT_REPRESENTING_ADDRTYPE_W equ ( PT_UNICODE shl 16)+   64h
PR_SENT_REPRESENTING_ADDRTYPE_A equ ( PT_STRING8 shl 16)+   64h
PR_SENT_REPRESENTING_EMAIL_ADDRESS equ ( PT_TSTRING shl 16)+   65h
PR_SENT_REPRESENTING_EMAIL_ADDRESS_W equ ( PT_UNICODE shl 16)+   65h
PR_SENT_REPRESENTING_EMAIL_ADDRESS_A equ ( PT_STRING8 shl 16)+   65h
PR_ORIGINAL_SENDER_ADDRTYPE equ ( PT_TSTRING shl 16)+   66h
PR_ORIGINAL_SENDER_ADDRTYPE_W equ ( PT_UNICODE shl 16)+   66h
PR_ORIGINAL_SENDER_ADDRTYPE_A equ ( PT_STRING8 shl 16)+   66h
PR_ORIGINAL_SENDER_EMAIL_ADDRESS equ ( PT_TSTRING shl 16)+   67h
PR_ORIGINAL_SENDER_EMAIL_ADDRESS_W equ ( PT_UNICODE shl 16)+   67h
PR_ORIGINAL_SENDER_EMAIL_ADDRESS_A equ ( PT_STRING8 shl 16)+   67h
PR_ORIGINAL_SENT_REPRESENTING_ADDRTYPE equ ( PT_TSTRING shl 16)+   68h
PR_ORIGINAL_SENT_REPRESENTING_ADDRTYPE_W equ ( PT_UNICODE shl 16)+   68h
PR_ORIGINAL_SENT_REPRESENTING_ADDRTYPE_A equ ( PT_STRING8 shl 16)+   68h
PR_ORIGINAL_SENT_REPRESENTING_EMAIL_ADDRESS equ ( PT_TSTRING shl 16)+   69h
PR_ORIGINAL_SENT_REPRESENTING_EMAIL_ADDRESS_W equ ( PT_UNICODE shl 16)+   69h
PR_ORIGINAL_SENT_REPRESENTING_EMAIL_ADDRESS_A equ ( PT_STRING8 shl 16)+   69h
PR_CONVERSATION_TOPIC equ ( PT_TSTRING shl 16)+   70h
PR_CONVERSATION_TOPIC_W equ ( PT_UNICODE shl 16)+   70h
PR_CONVERSATION_TOPIC_A equ ( PT_STRING8 shl 16)+   70h
PR_CONVERSATION_INDEX equ ( PT_BINARY shl 16)+    71h
PR_ORIGINAL_DISPLAY_BCC equ ( PT_TSTRING shl 16)+   72h
PR_ORIGINAL_DISPLAY_BCC_W equ ( PT_UNICODE shl 16)+   72h
PR_ORIGINAL_DISPLAY_BCC_A equ ( PT_STRING8 shl 16)+   72h
PR_ORIGINAL_DISPLAY_CC equ ( PT_TSTRING shl 16)+   73h
PR_ORIGINAL_DISPLAY_CC_W equ ( PT_UNICODE shl 16)+   73h
PR_ORIGINAL_DISPLAY_CC_A equ ( PT_STRING8 shl 16)+   73h
PR_ORIGINAL_DISPLAY_TO equ ( PT_TSTRING shl 16)+   74h
PR_ORIGINAL_DISPLAY_TO_W equ ( PT_UNICODE shl 16)+   74h
PR_ORIGINAL_DISPLAY_TO_A equ ( PT_STRING8 shl 16)+   74h
PR_RECEIVED_BY_ADDRTYPE equ ( PT_TSTRING shl 16)+   75h
PR_RECEIVED_BY_ADDRTYPE_W equ ( PT_UNICODE shl 16)+   75h
PR_RECEIVED_BY_ADDRTYPE_A equ ( PT_STRING8 shl 16)+   75h
PR_RECEIVED_BY_EMAIL_ADDRESS equ ( PT_TSTRING shl 16)+   76h
PR_RECEIVED_BY_EMAIL_ADDRESS_W equ ( PT_UNICODE shl 16)+   76h
PR_RECEIVED_BY_EMAIL_ADDRESS_A equ ( PT_STRING8 shl 16)+   76h
PR_RCVD_REPRESENTING_ADDRTYPE equ ( PT_TSTRING shl 16)+   77h
PR_RCVD_REPRESENTING_ADDRTYPE_W equ ( PT_UNICODE shl 16)+   77h
PR_RCVD_REPRESENTING_ADDRTYPE_A equ ( PT_STRING8 shl 16)+   77h
PR_RCVD_REPRESENTING_EMAIL_ADDRESS equ ( PT_TSTRING shl 16)+   78h
PR_RCVD_REPRESENTING_EMAIL_ADDRESS_W equ ( PT_UNICODE shl 16)+   78h
PR_RCVD_REPRESENTING_EMAIL_ADDRESS_A equ ( PT_STRING8 shl 16)+   78h
PR_ORIGINAL_AUTHOR_ADDRTYPE equ ( PT_TSTRING shl 16)+   79h
PR_ORIGINAL_AUTHOR_ADDRTYPE_W equ ( PT_UNICODE shl 16)+   79h
PR_ORIGINAL_AUTHOR_ADDRTYPE_A equ ( PT_STRING8 shl 16)+   79h
PR_ORIGINAL_AUTHOR_EMAIL_ADDRESS equ ( PT_TSTRING shl 16)+   7Ah
PR_ORIGINAL_AUTHOR_EMAIL_ADDRESS_W equ ( PT_UNICODE shl 16)+   7Ah
PR_ORIGINAL_AUTHOR_EMAIL_ADDRESS_A equ ( PT_STRING8 shl 16)+   7Ah
PR_ORIGINALLY_INTENDED_RECIP_ADDRTYPE equ ( PT_TSTRING shl 16)+   7Bh
PR_ORIGINALLY_INTENDED_RECIP_ADDRTYPE_W equ ( PT_UNICODE shl 16)+   7Bh
PR_ORIGINALLY_INTENDED_RECIP_ADDRTYPE_A equ ( PT_STRING8 shl 16)+   7Bh
PR_ORIGINALLY_INTENDED_RECIP_EMAIL_ADDRESS equ ( PT_TSTRING shl 16)+   7Ch
PR_ORIGINALLY_INTENDED_RECIP_EMAIL_ADDRESS_W equ ( PT_UNICODE shl 16)+   7Ch
PR_ORIGINALLY_INTENDED_RECIP_EMAIL_ADDRESS_A equ ( PT_STRING8 shl 16)+   7Ch
PR_TRANSPORT_MESSAGE_HEADERS equ (PT_TSTRING shl 16)+    7Dh
PR_TRANSPORT_MESSAGE_HEADERS_W equ (PT_UNICODE shl 16)+    7Dh
PR_TRANSPORT_MESSAGE_HEADERS_A equ (PT_STRING8 shl 16)+    7Dh
PR_DELEGATION equ (PT_BINARY shl 16)+     7Eh
PR_TNEF_CORRELATION_KEY equ (PT_BINARY shl 16)+     7Fh
PR_BODY equ ( PT_TSTRING shl 16)+   1000h
PR_BODY_W equ ( PT_UNICODE shl 16)+   1000h
PR_BODY_A equ ( PT_STRING8 shl 16)+   1000h
PR_REPORT_TEXT equ ( PT_TSTRING shl 16)+   1001h
PR_REPORT_TEXT_W equ ( PT_UNICODE shl 16)+   1001h
PR_REPORT_TEXT_A equ ( PT_STRING8 shl 16)+   1001h
PR_ORIGINATOR_AND_DL_EXPANSION_HISTORY equ ( PT_BINARY shl 16)+    1002h
PR_REPORTING_DL_NAME equ ( PT_BINARY shl 16)+    1003h
PR_REPORTING_MTA_CERTIFICATE equ ( PT_BINARY shl 16)+    1004h
PR_RTF_SYNC_BODY_CRC equ ( PT_LONG shl 16)+      1006h
PR_RTF_SYNC_BODY_COUNT equ ( PT_LONG shl 16)+      1007h
PR_RTF_SYNC_BODY_TAG equ ( PT_TSTRING shl 16)+   1008h
PR_RTF_SYNC_BODY_TAG_W equ ( PT_UNICODE shl 16)+   1008h
PR_RTF_SYNC_BODY_TAG_A equ ( PT_STRING8 shl 16)+   1008h
PR_RTF_COMPRESSED equ ( PT_BINARY shl 16)+    1009h
PR_RTF_SYNC_PREFIX_COUNT equ ( PT_LONG shl 16)+      1010h
PR_RTF_SYNC_TRAILING_COUNT equ ( PT_LONG shl 16)+      1011h
PR_ORIGINALLY_INTENDED_RECIP_ENTRYID equ ( PT_BINARY shl 16)+    1012h
PR_CONTENT_INTEGRITY_CHECK equ ( PT_BINARY shl 16)+    0C00h
PR_EXPLICIT_CONVERSION equ ( PT_LONG shl 16)+      0C01h
PR_IPM_RETURN_REQUESTED equ ( PT_BOOLEAN shl 16)+   0C02h
PR_MESSAGE_TOKEN equ ( PT_BINARY shl 16)+    0C03h
PR_NDR_REASON_CODE equ ( PT_LONG shl 16)+      0C04h
PR_NDR_DIAG_CODE equ ( PT_LONG shl 16)+      0C05h
PR_NON_RECEIPT_NOTIFICATION_REQUESTED equ ( PT_BOOLEAN shl 16)+   0C06h
PR_DELIVERY_POINT equ ( PT_LONG shl 16)+      0C07h
PR_ORIGINATOR_NON_DELIVERY_REPORT_REQUESTED equ ( PT_BOOLEAN shl 16)+   0C08h
PR_ORIGINATOR_REQUESTED_ALTERNATE_RECIPIENT equ ( PT_BINARY shl 16)+    0C09h
PR_PHYSICAL_DELIVERY_BUREAU_FAX_DELIVERY equ ( PT_BOOLEAN shl 16)+   0C0Ah
PR_PHYSICAL_DELIVERY_MODE equ ( PT_LONG shl 16)+      0C0Bh
PR_PHYSICAL_DELIVERY_REPORT_REQUEST equ ( PT_LONG shl 16)+      0C0Ch
PR_PHYSICAL_FORWARDING_ADDRESS equ ( PT_BINARY shl 16)+    0C0Dh
PR_PHYSICAL_FORWARDING_ADDRESS_REQUESTED equ ( PT_BOOLEAN shl 16)+   0C0Eh
PR_PHYSICAL_FORWARDING_PROHIBITED equ ( PT_BOOLEAN shl 16)+   0C0Fh
PR_PHYSICAL_RENDITION_ATTRIBUTES equ ( PT_BINARY shl 16)+    0C10h
PR_PROOF_OF_DELIVERY equ ( PT_BINARY shl 16)+    0C11h
PR_PROOF_OF_DELIVERY_REQUESTED equ ( PT_BOOLEAN shl 16)+   0C12h
PR_RECIPIENT_CERTIFICATE equ ( PT_BINARY shl 16)+    0C13h
PR_RECIPIENT_NUMBER_FOR_ADVICE equ ( PT_TSTRING shl 16)+   0C14h
PR_RECIPIENT_NUMBER_FOR_ADVICE_W equ ( PT_UNICODE shl 16)+   0C14h
PR_RECIPIENT_NUMBER_FOR_ADVICE_A equ ( PT_STRING8 shl 16)+   0C14h
PR_RECIPIENT_TYPE equ ( PT_LONG shl 16)+      0C15h
PR_REGISTERED_MAIL_TYPE equ ( PT_LONG shl 16)+      0C16h
PR_REPLY_REQUESTED equ ( PT_BOOLEAN shl 16)+   0C17h
PR_REQUESTED_DELIVERY_METHOD equ ( PT_LONG shl 16)+      0C18h
PR_SENDER_ENTRYID equ ( PT_BINARY shl 16)+    0C19h
PR_SENDER_NAME equ ( PT_TSTRING shl 16)+   0C1Ah
PR_SENDER_NAME_W equ ( PT_UNICODE shl 16)+   0C1Ah
PR_SENDER_NAME_A equ ( PT_STRING8 shl 16)+   0C1Ah
PR_SUPPLEMENTARY_INFO equ ( PT_TSTRING shl 16)+   0C1Bh
PR_SUPPLEMENTARY_INFO_W equ ( PT_UNICODE shl 16)+   0C1Bh
PR_SUPPLEMENTARY_INFO_A equ ( PT_STRING8 shl 16)+   0C1Bh
PR_TYPE_OF_MTS_USER equ ( PT_LONG shl 16)+      0C1Ch
PR_SENDER_SEARCH_KEY equ ( PT_BINARY shl 16)+    0C1Dh
PR_SENDER_ADDRTYPE equ ( PT_TSTRING shl 16)+   0C1Eh
PR_SENDER_ADDRTYPE_W equ ( PT_UNICODE shl 16)+   0C1Eh
PR_SENDER_ADDRTYPE_A equ ( PT_STRING8 shl 16)+   0C1Eh
PR_SENDER_EMAIL_ADDRESS equ ( PT_TSTRING shl 16)+   0C1Fh
PR_SENDER_EMAIL_ADDRESS_W equ ( PT_UNICODE shl 16)+   0C1Fh
PR_SENDER_EMAIL_ADDRESS_A equ ( PT_STRING8 shl 16)+   0C1Fh
PR_CURRENT_VERSION equ ( PT_I8 shl 16)+        0E00h
PR_DELETE_AFTER_SUBMIT equ ( PT_BOOLEAN shl 16)+   0E01h
PR_DISPLAY_BCC equ ( PT_TSTRING shl 16)+   0E02h
PR_DISPLAY_BCC_W equ ( PT_UNICODE shl 16)+   0E02h
PR_DISPLAY_BCC_A equ ( PT_STRING8 shl 16)+   0E02h
PR_DISPLAY_CC equ ( PT_TSTRING shl 16)+   0E03h
PR_DISPLAY_CC_W equ ( PT_UNICODE shl 16)+   0E03h
PR_DISPLAY_CC_A equ ( PT_STRING8 shl 16)+   0E03h
PR_DISPLAY_TO equ ( PT_TSTRING shl 16)+   0E04h
PR_DISPLAY_TO_W equ ( PT_UNICODE shl 16)+   0E04h
PR_DISPLAY_TO_A equ ( PT_STRING8 shl 16)+   0E04h
PR_PARENT_DISPLAY equ ( PT_TSTRING shl 16)+   0E05h
PR_PARENT_DISPLAY_W equ ( PT_UNICODE shl 16)+   0E05h
PR_PARENT_DISPLAY_A equ ( PT_STRING8 shl 16)+   0E05h
PR_MESSAGE_DELIVERY_TIME equ ( PT_SYSTIME shl 16)+   0E06h
PR_MESSAGE_FLAGS equ ( PT_LONG shl 16)+      0E07h
PR_MESSAGE_SIZE equ ( PT_LONG shl 16)+      0E08h
PR_PARENT_ENTRYID equ ( PT_BINARY shl 16)+    0E09h
PR_SENTMAIL_ENTRYID equ ( PT_BINARY shl 16)+    0E0Ah
PR_CORRELATE equ ( PT_BOOLEAN shl 16)+   0E0Ch
PR_CORRELATE_MTSID equ ( PT_BINARY shl 16)+    0E0Dh
PR_DISCRETE_VALUES equ ( PT_BOOLEAN shl 16)+   0E0Eh
PR_RESPONSIBILITY equ ( PT_BOOLEAN shl 16)+   0E0Fh
PR_SPOOLER_STATUS equ ( PT_LONG shl 16)+      0E10h
PR_TRANSPORT_STATUS equ ( PT_LONG shl 16)+      0E11h
PR_MESSAGE_RECIPIENTS equ ( PT_OBJECT shl 16)+    0E12h
PR_MESSAGE_ATTACHMENTS equ ( PT_OBJECT shl 16)+    0E13h
PR_SUBMIT_FLAGS equ ( PT_LONG shl 16)+      0E14h
PR_RECIPIENT_STATUS equ ( PT_LONG shl 16)+      0E15h
PR_TRANSPORT_KEY equ ( PT_LONG shl 16)+      0E16h
PR_MSG_STATUS equ ( PT_LONG shl 16)+      0E17h
PR_MESSAGE_DOWNLOAD_TIME equ ( PT_LONG shl 16)+      0E18h
PR_CREATION_VERSION equ ( PT_I8 shl 16)+        0E19h
PR_MODIFY_VERSION equ ( PT_I8 shl 16)+        0E1Ah
PR_HASATTACH equ ( PT_BOOLEAN shl 16)+   0E1Bh
PR_BODY_CRC equ ( PT_LONG shl 16)+      0E1Ch
PR_NORMALIZED_SUBJECT equ ( PT_TSTRING shl 16)+   0E1Dh
PR_NORMALIZED_SUBJECT_W equ ( PT_UNICODE shl 16)+   0E1Dh
PR_NORMALIZED_SUBJECT_A equ ( PT_STRING8 shl 16)+   0E1Dh
PR_RTF_IN_SYNC equ ( PT_BOOLEAN shl 16)+   0E1Fh
PR_ATTACH_SIZE equ ( PT_LONG shl 16)+      0E20h
PR_ATTACH_NUM equ ( PT_LONG shl 16)+      0E21h
PR_PREPROCESS equ ( PT_BOOLEAN shl 16)+   0E22h
PR_ORIGINATING_MTA_CERTIFICATE equ ( PT_BINARY shl 16)+    0E25h
PR_PROOF_OF_SUBMISSION equ ( PT_BINARY shl 16)+    0E26h
PR_ENTRYID equ ( PT_BINARY shl 16)+    0FFFh
PR_OBJECT_TYPE equ ( PT_LONG shl 16)+      0FFEh
PR_ICON equ ( PT_BINARY shl 16)+    0FFDh
PR_MINI_ICON equ ( PT_BINARY shl 16)+    0FFCh
PR_STORE_ENTRYID equ ( PT_BINARY shl 16)+    0FFBh
PR_STORE_RECORD_KEY equ ( PT_BINARY shl 16)+    0FFAh
PR_RECORD_KEY equ ( PT_BINARY shl 16)+    0FF9h
PR_MAPPING_SIGNATURE equ ( PT_BINARY shl 16)+    0FF8h
PR_ACCESS_LEVEL equ ( PT_LONG shl 16)+      0FF7h
PR_INSTANCE_KEY equ ( PT_BINARY shl 16)+    0FF6h
PR_ROW_TYPE equ ( PT_LONG shl 16)+      0FF5h
PR_ACCESS equ ( PT_LONG shl 16)+      0FF4h
PR_ROWID equ ( PT_LONG shl 16)+      3000h
PR_DISPLAY_NAME equ ( PT_TSTRING shl 16)+   3001h
PR_DISPLAY_NAME_W equ ( PT_UNICODE shl 16)+   3001h
PR_DISPLAY_NAME_A equ ( PT_STRING8 shl 16)+   3001h
PR_ADDRTYPE equ ( PT_TSTRING shl 16)+   3002h
PR_ADDRTYPE_W equ ( PT_UNICODE shl 16)+   3002h
PR_ADDRTYPE_A equ ( PT_STRING8 shl 16)+   3002h
PR_EMAIL_ADDRESS equ ( PT_TSTRING shl 16)+   3003h
PR_EMAIL_ADDRESS_W equ ( PT_UNICODE shl 16)+   3003h
PR_EMAIL_ADDRESS_A equ ( PT_STRING8 shl 16)+   3003h
PR_COMMENT equ ( PT_TSTRING shl 16)+   3004h
PR_COMMENT_W equ ( PT_UNICODE shl 16)+   3004h
PR_COMMENT_A equ ( PT_STRING8 shl 16)+   3004h

PR_DEPTH equ ( PT_LONG shl 16)+      3005h
PR_PROVIDER_DISPLAY equ ( PT_TSTRING shl 16)+   3006h
PR_PROVIDER_DISPLAY_W equ ( PT_UNICODE shl 16)+   3006h
PR_PROVIDER_DISPLAY_A equ ( PT_STRING8 shl 16)+   3006h
PR_CREATION_TIME equ ( PT_SYSTIME shl 16)+   3007h
PR_LAST_MODIFICATION_TIME equ ( PT_SYSTIME shl 16)+   3008h
PR_RESOURCE_FLAGS equ ( PT_LONG shl 16)+      3009h
PR_PROVIDER_DLL_NAME equ ( PT_TSTRING shl 16)+   300Ah
PR_PROVIDER_DLL_NAME_W equ ( PT_UNICODE shl 16)+   300Ah
PR_PROVIDER_DLL_NAME_A equ ( PT_STRING8 shl 16)+   300Ah
PR_SEARCH_KEY equ ( PT_BINARY shl 16)+    300Bh
PR_PROVIDER_UID equ ( PT_BINARY shl 16)+    300Ch
PR_PROVIDER_ORDINAL equ ( PT_LONG shl 16)+      300Dh
PR_FORM_VERSION equ (PT_TSTRING shl 16)+    3301h
PR_FORM_VERSION_W equ (PT_UNICODE shl 16)+    3301h
PR_FORM_VERSION_A equ (PT_STRING8 shl 16)+    3301h
PR_FORM_CLSID equ (PT_CLSID shl 16)+      3302h
PR_FORM_CONTACT_NAME equ (PT_TSTRING shl 16)+    3303h
PR_FORM_CONTACT_NAME_W equ (PT_UNICODE shl 16)+    3303h
PR_FORM_CONTACT_NAME_A equ (PT_STRING8 shl 16)+    3303h
PR_FORM_CATEGORY equ (PT_TSTRING shl 16)+    3304h
PR_FORM_CATEGORY_W equ (PT_UNICODE shl 16)+    3304h
PR_FORM_CATEGORY_A equ (PT_STRING8 shl 16)+    3304h
PR_FORM_CATEGORY_SUB equ (PT_TSTRING shl 16)+    3305h
PR_FORM_CATEGORY_SUB_W equ (PT_UNICODE shl 16)+    3305h
PR_FORM_CATEGORY_SUB_A equ (PT_STRING8 shl 16)+    3305h
PR_FORM_HOST_MAP equ (PT_MV_LONG shl 16)+    3306h
PR_FORM_HIDDEN equ (PT_BOOLEAN shl 16)+    3307h
PR_FORM_DESIGNER_NAME equ (PT_TSTRING shl 16)+    3308h
PR_FORM_DESIGNER_NAME_W equ (PT_UNICODE shl 16)+    3308h
PR_FORM_DESIGNER_NAME_A equ (PT_STRING8 shl 16)+    3308h
PR_FORM_DESIGNER_GUID equ (PT_CLSID shl 16)+      3309h
PR_FORM_MESSAGE_BEHAVIOR equ (PT_LONG shl 16)+       330Ah
PR_DEFAULT_STORE equ ( PT_BOOLEAN shl 16)+   3400h
PR_STORE_SUPPORT_MASK equ ( PT_LONG shl 16)+      340Dh
PR_STORE_STATE equ ( PT_LONG shl 16)+      340Eh
PR_IPM_SUBTREE_SEARCH_KEY equ ( PT_BINARY shl 16)+    3410h
PR_IPM_OUTBOX_SEARCH_KEY equ ( PT_BINARY shl 16)+    3411h
PR_IPM_WASTEBASKET_SEARCH_KEY equ ( PT_BINARY shl 16)+    3412h
PR_IPM_SENTMAIL_SEARCH_KEY equ ( PT_BINARY shl 16)+    3413h
PR_MDB_PROVIDER equ ( PT_BINARY shl 16)+    3414h
PR_RECEIVE_FOLDER_SETTINGS equ ( PT_OBJECT shl 16)+    3415h
PR_VALID_FOLDER_MASK equ ( PT_LONG shl 16)+      35DFh
PR_IPM_SUBTREE_ENTRYID equ ( PT_BINARY shl 16)+    35E0h
PR_IPM_OUTBOX_ENTRYID equ ( PT_BINARY shl 16)+    35E2h
PR_IPM_WASTEBASKET_ENTRYID equ ( PT_BINARY shl 16)+    35E3h
PR_IPM_SENTMAIL_ENTRYID equ ( PT_BINARY shl 16)+    35E4h
PR_VIEWS_ENTRYID equ ( PT_BINARY shl 16)+    35E5h
PR_COMMON_VIEWS_ENTRYID equ ( PT_BINARY shl 16)+    35E6h
PR_FINDER_ENTRYID equ ( PT_BINARY shl 16)+    35E7h
PR_CONTAINER_FLAGS equ ( PT_LONG shl 16)+      3600h
PR_FOLDER_TYPE equ ( PT_LONG shl 16)+      3601h
PR_CONTENT_COUNT equ ( PT_LONG shl 16)+      3602h
PR_CONTENT_UNREAD equ ( PT_LONG shl 16)+      3603h
PR_CREATE_TEMPLATES equ ( PT_OBJECT shl 16)+    3604h
PR_DETAILS_TABLE equ ( PT_OBJECT shl 16)+    3605h
PR_SEARCH equ ( PT_OBJECT shl 16)+    3607h
PR_SELECTABLE equ ( PT_BOOLEAN shl 16)+   3609h
PR_SUBFOLDERS equ ( PT_BOOLEAN shl 16)+   360Ah
PR_STATUS equ ( PT_LONG shl 16)+      360Bh
PR_ANR equ ( PT_TSTRING shl 16)+   360Ch
PR_ANR_W equ ( PT_UNICODE shl 16)+   360Ch
PR_ANR_A equ ( PT_STRING8 shl 16)+   360Ch
PR_CONTENTS_SORT_ORDER equ ( PT_MV_LONG shl 16)+   360Dh
PR_CONTAINER_HIERARCHY equ ( PT_OBJECT shl 16)+    360Eh
PR_CONTAINER_CONTENTS equ ( PT_OBJECT shl 16)+    360Fh
PR_FOLDER_ASSOCIATED_CONTENTS equ ( PT_OBJECT shl 16)+    3610h
PR_DEF_CREATE_DL equ ( PT_BINARY shl 16)+    3611h
PR_DEF_CREATE_MAILUSER equ ( PT_BINARY shl 16)+    3612h
PR_CONTAINER_CLASS equ ( PT_TSTRING shl 16)+   3613h
PR_CONTAINER_CLASS_W equ ( PT_UNICODE shl 16)+   3613h
PR_CONTAINER_CLASS_A equ ( PT_STRING8 shl 16)+   3613h
PR_CONTAINER_MODIFY_VERSION equ ( PT_I8 shl 16)+        3614h
PR_AB_PROVIDER_ID equ ( PT_BINARY shl 16)+    3615h
PR_DEFAULT_VIEW_ENTRYID equ ( PT_BINARY shl 16)+    3616h
PR_ASSOC_CONTENT_COUNT equ ( PT_LONG shl 16)+      3617h
PR_ATTACHMENT_X400_PARAMETERS equ ( PT_BINARY shl 16)+    3700h
PR_ATTACH_DATA_OBJ equ ( PT_OBJECT shl 16)+    3701h
PR_ATTACH_DATA_BIN equ ( PT_BINARY shl 16)+    3701h
PR_ATTACH_ENCODING equ ( PT_BINARY shl 16)+    3702h
PR_ATTACH_EXTENSION equ ( PT_TSTRING shl 16)+   3703h
PR_ATTACH_EXTENSION_W equ ( PT_UNICODE shl 16)+   3703h
PR_ATTACH_EXTENSION_A equ ( PT_STRING8 shl 16)+   3703h
PR_ATTACH_FILENAME equ ( PT_TSTRING shl 16)+   3704h
PR_ATTACH_FILENAME_W equ ( PT_UNICODE shl 16)+   3704h
PR_ATTACH_FILENAME_A equ ( PT_STRING8 shl 16)+   3704h
PR_ATTACH_METHOD equ ( PT_LONG shl 16)+      3705h
PR_ATTACH_LONG_FILENAME equ ( PT_TSTRING shl 16)+   3707h
PR_ATTACH_LONG_FILENAME_W equ ( PT_UNICODE shl 16)+   3707h
PR_ATTACH_LONG_FILENAME_A equ ( PT_STRING8 shl 16)+   3707h
PR_ATTACH_PATHNAME equ ( PT_TSTRING shl 16)+   3708h
PR_ATTACH_PATHNAME_W equ ( PT_UNICODE shl 16)+   3708h
PR_ATTACH_PATHNAME_A equ ( PT_STRING8 shl 16)+   3708h
PR_ATTACH_RENDERING equ ( PT_BINARY shl 16)+    3709h
PR_ATTACH_TAG equ ( PT_BINARY shl 16)+    370Ah
PR_RENDERING_POSITION equ ( PT_LONG shl 16)+      370Bh
PR_ATTACH_TRANSPORT_NAME equ ( PT_TSTRING shl 16)+   370Ch
PR_ATTACH_TRANSPORT_NAME_W equ ( PT_UNICODE shl 16)+   370Ch
PR_ATTACH_TRANSPORT_NAME_A equ ( PT_STRING8 shl 16)+   370Ch
PR_ATTACH_LONG_PATHNAME equ ( PT_TSTRING shl 16)+   370Dh
PR_ATTACH_LONG_PATHNAME_W equ ( PT_UNICODE shl 16)+   370Dh
PR_ATTACH_LONG_PATHNAME_A equ ( PT_STRING8 shl 16)+   370Dh
PR_ATTACH_MIME_TAG equ ( PT_TSTRING shl 16)+   370Eh
PR_ATTACH_MIME_TAG_W equ ( PT_UNICODE shl 16)+   370Eh
PR_ATTACH_MIME_TAG_A equ ( PT_STRING8 shl 16)+   370Eh
PR_ATTACH_ADDITIONAL_INFO equ ( PT_BINARY shl 16)+    370Fh
PR_DISPLAY_TYPE equ ( PT_LONG shl 16)+      3900h
PR_TEMPLATEID equ ( PT_BINARY shl 16)+    3902h
PR_PRIMARY_CAPABILITY equ ( PT_BINARY shl 16)+    3904h
PR_7BIT_DISPLAY_NAME equ ( PT_STRING8 shl 16)+   39FFh
PR_ACCOUNT equ ( PT_TSTRING shl 16)+   3A00h
PR_ACCOUNT_W equ ( PT_UNICODE shl 16)+   3A00h
PR_ACCOUNT_A equ ( PT_STRING8 shl 16)+   3A00h
PR_ALTERNATE_RECIPIENT equ ( PT_BINARY shl 16)+    3A01h
PR_CALLBACK_TELEPHONE_NUMBER equ ( PT_TSTRING shl 16)+   3A02h
PR_CALLBACK_TELEPHONE_NUMBER_W equ ( PT_UNICODE shl 16)+   3A02h
PR_CALLBACK_TELEPHONE_NUMBER_A equ ( PT_STRING8 shl 16)+   3A02h
PR_CONVERSION_PROHIBITED equ ( PT_BOOLEAN shl 16)+   3A03h
PR_DISCLOSE_RECIPIENTS equ ( PT_BOOLEAN shl 16)+   3A04h
PR_GENERATION equ ( PT_TSTRING shl 16)+   3A05h
PR_GENERATION_W equ ( PT_UNICODE shl 16)+   3A05h
PR_GENERATION_A equ ( PT_STRING8 shl 16)+   3A05h
PR_GIVEN_NAME equ ( PT_TSTRING shl 16)+   3A06h
PR_GIVEN_NAME_W equ ( PT_UNICODE shl 16)+   3A06h
PR_GIVEN_NAME_A equ ( PT_STRING8 shl 16)+   3A06h
PR_GOVERNMENT_ID_NUMBER equ ( PT_TSTRING shl 16)+   3A07h
PR_GOVERNMENT_ID_NUMBER_W equ ( PT_UNICODE shl 16)+   3A07h
PR_GOVERNMENT_ID_NUMBER_A equ ( PT_STRING8 shl 16)+   3A07h
PR_BUSINESS_TELEPHONE_NUMBER equ ( PT_TSTRING shl 16)+   3A08h
PR_BUSINESS_TELEPHONE_NUMBER_W equ ( PT_UNICODE shl 16)+   3A08h
PR_BUSINESS_TELEPHONE_NUMBER_A equ ( PT_STRING8 shl 16)+   3A08h
PR_OFFICE_TELEPHONE_NUMBER equ PR_BUSINESS_TELEPHONE_NUMBER
PR_OFFICE_TELEPHONE_NUMBER_W equ PR_BUSINESS_TELEPHONE_NUMBER_W
PR_OFFICE_TELEPHONE_NUMBER_A equ PR_BUSINESS_TELEPHONE_NUMBER_A
PR_HOME_TELEPHONE_NUMBER equ ( PT_TSTRING shl 16)+   3A09h
PR_HOME_TELEPHONE_NUMBER_W equ ( PT_UNICODE shl 16)+   3A09h
PR_HOME_TELEPHONE_NUMBER_A equ ( PT_STRING8 shl 16)+   3A09h
PR_INITIALS equ ( PT_TSTRING shl 16)+   3A0Ah
PR_INITIALS_W equ ( PT_UNICODE shl 16)+   3A0Ah
PR_INITIALS_A equ ( PT_STRING8 shl 16)+   3A0Ah
PR_KEYWORD equ ( PT_TSTRING shl 16)+   3A0Bh
PR_KEYWORD_W equ ( PT_UNICODE shl 16)+   3A0Bh
PR_KEYWORD_A equ ( PT_STRING8 shl 16)+   3A0Bh
PR_LANGUAGE equ ( PT_TSTRING shl 16)+   3A0Ch
PR_LANGUAGE_W equ ( PT_UNICODE shl 16)+   3A0Ch
PR_LANGUAGE_A equ ( PT_STRING8 shl 16)+   3A0Ch
PR_LOCATION equ ( PT_TSTRING shl 16)+   3A0Dh
PR_LOCATION_W equ ( PT_UNICODE shl 16)+   3A0Dh
PR_LOCATION_A equ ( PT_STRING8 shl 16)+   3A0Dh
PR_MAIL_PERMISSION equ ( PT_BOOLEAN shl 16)+   3A0Eh
PR_MHS_COMMON_NAME equ ( PT_TSTRING shl 16)+   3A0Fh
PR_MHS_COMMON_NAME_W equ ( PT_UNICODE shl 16)+   3A0Fh
PR_MHS_COMMON_NAME_A equ ( PT_STRING8 shl 16)+   3A0Fh
PR_ORGANIZATIONAL_ID_NUMBER equ ( PT_TSTRING shl 16)+   3A10h
PR_ORGANIZATIONAL_ID_NUMBER_W equ ( PT_UNICODE shl 16)+   3A10h
PR_ORGANIZATIONAL_ID_NUMBER_A equ ( PT_STRING8 shl 16)+   3A10h
PR_SURNAME equ ( PT_TSTRING shl 16)+   3A11h
PR_SURNAME_W equ ( PT_UNICODE shl 16)+   3A11h
PR_SURNAME_A equ ( PT_STRING8 shl 16)+   3A11h
PR_ORIGINAL_ENTRYID equ ( PT_BINARY shl 16)+    3A12h
PR_ORIGINAL_DISPLAY_NAME equ ( PT_TSTRING shl 16)+   3A13h
PR_ORIGINAL_DISPLAY_NAME_W equ ( PT_UNICODE shl 16)+   3A13h
PR_ORIGINAL_DISPLAY_NAME_A equ ( PT_STRING8 shl 16)+   3A13h
PR_ORIGINAL_SEARCH_KEY equ ( PT_BINARY shl 16)+    3A14h
PR_POSTAL_ADDRESS equ ( PT_TSTRING shl 16)+   3A15h
PR_POSTAL_ADDRESS_W equ ( PT_UNICODE shl 16)+   3A15h
PR_POSTAL_ADDRESS_A equ ( PT_STRING8 shl 16)+   3A15h
PR_COMPANY_NAME equ ( PT_TSTRING shl 16)+   3A16h
PR_COMPANY_NAME_W equ ( PT_UNICODE shl 16)+   3A16h
PR_COMPANY_NAME_A equ ( PT_STRING8 shl 16)+   3A16h
PR_TITLE equ ( PT_TSTRING shl 16)+   3A17h
PR_TITLE_W equ ( PT_UNICODE shl 16)+   3A17h
PR_TITLE_A equ ( PT_STRING8 shl 16)+   3A17h
PR_DEPARTMENT_NAME equ ( PT_TSTRING shl 16)+   3A18h
PR_DEPARTMENT_NAME_W equ ( PT_UNICODE shl 16)+   3A18h
PR_DEPARTMENT_NAME_A equ ( PT_STRING8 shl 16)+   3A18h
PR_OFFICE_LOCATION equ ( PT_TSTRING shl 16)+   3A19h
PR_OFFICE_LOCATION_W equ ( PT_UNICODE shl 16)+   3A19h
PR_OFFICE_LOCATION_A equ ( PT_STRING8 shl 16)+   3A19h
PR_PRIMARY_TELEPHONE_NUMBER equ ( PT_TSTRING shl 16)+   3A1Ah
PR_PRIMARY_TELEPHONE_NUMBER_W equ ( PT_UNICODE shl 16)+   3A1Ah
PR_PRIMARY_TELEPHONE_NUMBER_A equ ( PT_STRING8 shl 16)+   3A1Ah
PR_BUSINESS2_TELEPHONE_NUMBER equ ( PT_TSTRING shl 16)+   3A1Bh
PR_BUSINESS2_TELEPHONE_NUMBER_W equ ( PT_UNICODE shl 16)+   3A1Bh
PR_BUSINESS2_TELEPHONE_NUMBER_A equ ( PT_STRING8 shl 16)+   3A1Bh
PR_OFFICE2_TELEPHONE_NUMBER equ PR_BUSINESS2_TELEPHONE_NUMBER
PR_OFFICE2_TELEPHONE_NUMBER_W equ PR_BUSINESS2_TELEPHONE_NUMBER_W
PR_OFFICE2_TELEPHONE_NUMBER_A equ PR_BUSINESS2_TELEPHONE_NUMBER_A
PR_MOBILE_TELEPHONE_NUMBER equ ( PT_TSTRING shl 16)+   3A1Ch
PR_MOBILE_TELEPHONE_NUMBER_W equ ( PT_UNICODE shl 16)+   3A1Ch
PR_MOBILE_TELEPHONE_NUMBER_A equ ( PT_STRING8 shl 16)+   3A1Ch
PR_CELLULAR_TELEPHONE_NUMBER equ PR_MOBILE_TELEPHONE_NUMBER
PR_CELLULAR_TELEPHONE_NUMBER_W equ PR_MOBILE_TELEPHONE_NUMBER_W
PR_CELLULAR_TELEPHONE_NUMBER_A equ PR_MOBILE_TELEPHONE_NUMBER_A
PR_RADIO_TELEPHONE_NUMBER equ ( PT_TSTRING shl 16)+   3A1Dh
PR_RADIO_TELEPHONE_NUMBER_W equ ( PT_UNICODE shl 16)+   3A1Dh
PR_RADIO_TELEPHONE_NUMBER_A equ ( PT_STRING8 shl 16)+   3A1Dh
PR_CAR_TELEPHONE_NUMBER equ ( PT_TSTRING shl 16)+   3A1Eh
PR_CAR_TELEPHONE_NUMBER_W equ ( PT_UNICODE shl 16)+   3A1Eh
PR_CAR_TELEPHONE_NUMBER_A equ ( PT_STRING8 shl 16)+   3A1Eh
PR_OTHER_TELEPHONE_NUMBER equ ( PT_TSTRING shl 16)+   3A1Fh
PR_OTHER_TELEPHONE_NUMBER_W equ ( PT_UNICODE shl 16)+   3A1Fh
PR_OTHER_TELEPHONE_NUMBER_A equ ( PT_STRING8 shl 16)+   3A1Fh
PR_TRANSMITABLE_DISPLAY_NAME equ ( PT_TSTRING shl 16)+   3A20h
PR_TRANSMITABLE_DISPLAY_NAME_W equ ( PT_UNICODE shl 16)+   3A20h
PR_TRANSMITABLE_DISPLAY_NAME_A equ ( PT_STRING8 shl 16)+   3A20h
PR_PAGER_TELEPHONE_NUMBER equ ( PT_TSTRING shl 16)+   3A21h
PR_PAGER_TELEPHONE_NUMBER_W equ ( PT_UNICODE shl 16)+   3A21h
PR_PAGER_TELEPHONE_NUMBER_A equ ( PT_STRING8 shl 16)+   3A21h
PR_BEEPER_TELEPHONE_NUMBER equ PR_PAGER_TELEPHONE_NUMBER
PR_BEEPER_TELEPHONE_NUMBER_W equ PR_PAGER_TELEPHONE_NUMBER_W
PR_BEEPER_TELEPHONE_NUMBER_A equ PR_PAGER_TELEPHONE_NUMBER_A
PR_USER_CERTIFICATE equ ( PT_BINARY shl 16)+    3A22h
PR_PRIMARY_FAX_NUMBER equ ( PT_TSTRING shl 16)+   3A23h
PR_PRIMARY_FAX_NUMBER_W equ ( PT_UNICODE shl 16)+   3A23h
PR_PRIMARY_FAX_NUMBER_A equ ( PT_STRING8 shl 16)+   3A23h
PR_BUSINESS_FAX_NUMBER equ ( PT_TSTRING shl 16)+   3A24h
PR_BUSINESS_FAX_NUMBER_W equ ( PT_UNICODE shl 16)+   3A24h
PR_BUSINESS_FAX_NUMBER_A equ ( PT_STRING8 shl 16)+   3A24h
PR_HOME_FAX_NUMBER equ ( PT_TSTRING shl 16)+   3A25h
PR_HOME_FAX_NUMBER_W equ ( PT_UNICODE shl 16)+   3A25h
PR_HOME_FAX_NUMBER_A equ ( PT_STRING8 shl 16)+   3A25h
PR_COUNTRY equ ( PT_TSTRING shl 16)+   3A26h
PR_COUNTRY_W equ ( PT_UNICODE shl 16)+   3A26h
PR_COUNTRY_A equ ( PT_STRING8 shl 16)+   3A26h
PR_BUSINESS_ADDRESS_COUNTRY equ PR_COUNTRY
PR_BUSINESS_ADDRESS_COUNTRY_W equ PR_COUNTRY_W
PR_BUSINESS_ADDRESS_COUNTRY_A equ PR_COUNTRY_A
PR_LOCALITY equ ( PT_TSTRING shl 16)+   3A27h
PR_LOCALITY_W equ ( PT_UNICODE shl 16)+   3A27h
PR_LOCALITY_A equ ( PT_STRING8 shl 16)+   3A27h
PR_BUSINESS_ADDRESS_CITY equ PR_LOCALITY
PR_BUSINESS_ADDRESS_CITY_W equ PR_LOCALITY_W
PR_BUSINESS_ADDRESS_CITY_A equ PR_LOCALITY_A
PR_STATE_OR_PROVINCE equ ( PT_TSTRING shl 16)+   3A28h
PR_STATE_OR_PROVINCE_W equ ( PT_UNICODE shl 16)+   3A28h
PR_STATE_OR_PROVINCE_A equ ( PT_STRING8 shl 16)+   3A28h
PR_BUSINESS_ADDRESS_STATE_OR_PROVINCE equ PR_STATE_OR_PROVINCE
PR_BUSINESS_ADDRESS_STATE_OR_PROVINCE_W equ PR_STATE_OR_PROVINCE_W
PR_BUSINESS_ADDRESS_STATE_OR_PROVINCE_A equ PR_STATE_OR_PROVINCE_A
PR_STREET_ADDRESS equ ( PT_TSTRING shl 16)+   3A29h
PR_STREET_ADDRESS_W equ ( PT_UNICODE shl 16)+   3A29h
PR_STREET_ADDRESS_A equ ( PT_STRING8 shl 16)+   3A29h
PR_BUSINESS_ADDRESS_STREET equ PR_STREET_ADDRESS
PR_BUSINESS_ADDRESS_STREET_W equ PR_STREET_ADDRESS_W
PR_BUSINESS_ADDRESS_STREET_A equ PR_STREET_ADDRESS_A
PR_POSTAL_CODE equ ( PT_TSTRING shl 16)+   3A2Ah
PR_POSTAL_CODE_W equ ( PT_UNICODE shl 16)+   3A2Ah
PR_POSTAL_CODE_A equ ( PT_STRING8 shl 16)+   3A2Ah
PR_BUSINESS_ADDRESS_POSTAL_CODE equ PR_POSTAL_CODE
PR_BUSINESS_ADDRESS_POSTAL_CODE_W equ PR_POSTAL_CODE_W
PR_BUSINESS_ADDRESS_POSTAL_CODE_A equ PR_POSTAL_CODE_A
PR_POST_OFFICE_BOX equ ( PT_TSTRING shl 16)+   3A2Bh
PR_POST_OFFICE_BOX_W equ ( PT_UNICODE shl 16)+   3A2Bh
PR_POST_OFFICE_BOX_A equ ( PT_STRING8 shl 16)+   3A2Bh
PR_BUSINESS_ADDRESS_POST_OFFICE_BOX equ PR_POST_OFFICE_BOX
PR_BUSINESS_ADDRESS_POST_OFFICE_BOX_W equ PR_POST_OFFICE_BOX_W
PR_BUSINESS_ADDRESS_POST_OFFICE_BOX_A equ PR_POST_OFFICE_BOX_A
PR_TELEX_NUMBER equ ( PT_TSTRING shl 16)+   3A2Ch
PR_TELEX_NUMBER_W equ ( PT_UNICODE shl 16)+   3A2Ch
PR_TELEX_NUMBER_A equ ( PT_STRING8 shl 16)+   3A2Ch
PR_ISDN_NUMBER equ ( PT_TSTRING shl 16)+   3A2Dh
PR_ISDN_NUMBER_W equ ( PT_UNICODE shl 16)+   3A2Dh
PR_ISDN_NUMBER_A equ ( PT_STRING8 shl 16)+   3A2Dh
PR_ASSISTANT_TELEPHONE_NUMBER equ ( PT_TSTRING shl 16)+   3A2Eh
PR_ASSISTANT_TELEPHONE_NUMBER_W equ ( PT_UNICODE shl 16)+   3A2Eh
PR_ASSISTANT_TELEPHONE_NUMBER_A equ ( PT_STRING8 shl 16)+   3A2Eh
PR_HOME2_TELEPHONE_NUMBER equ ( PT_TSTRING shl 16)+   3A2Fh
PR_HOME2_TELEPHONE_NUMBER_W equ ( PT_UNICODE shl 16)+   3A2Fh
PR_HOME2_TELEPHONE_NUMBER_A equ ( PT_STRING8 shl 16)+   3A2Fh
PR_ASSISTANT equ ( PT_TSTRING shl 16)+   3A30h
PR_ASSISTANT_W equ ( PT_UNICODE shl 16)+   3A30h
PR_ASSISTANT_A equ ( PT_STRING8 shl 16)+   3A30h
PR_SEND_RICH_INFO equ ( PT_BOOLEAN shl 16)+   3A40h
PR_WEDDING_ANNIVERSARY equ ( PT_SYSTIME shl 16)+ 3A41h
PR_BIRTHDAY equ ( PT_SYSTIME shl 16)+ 3A42h
PR_HOBBIES equ ( PT_TSTRING shl 16)+ 3A43h
PR_HOBBIES_W equ ( PT_UNICODE shl 16)+ 3A43h
PR_HOBBIES_A equ ( PT_STRING8 shl 16)+ 3A43h
PR_MIDDLE_NAME equ ( PT_TSTRING shl 16)+ 3A44h
PR_MIDDLE_NAME_W equ ( PT_UNICODE shl 16)+ 3A44h
PR_MIDDLE_NAME_A equ ( PT_STRING8 shl 16)+ 3A44h
PR_DISPLAY_NAME_PREFIX equ ( PT_TSTRING shl 16)+ 3A45h
PR_DISPLAY_NAME_PREFIX_W equ ( PT_UNICODE shl 16)+ 3A45h
PR_DISPLAY_NAME_PREFIX_A equ ( PT_STRING8 shl 16)+ 3A45h
PR_PROFESSION equ ( PT_TSTRING shl 16)+ 3A46h
PR_PROFESSION_W equ ( PT_UNICODE shl 16)+ 3A46h
PR_PROFESSION_A equ ( PT_STRING8 shl 16)+ 3A46h
PR_PREFERRED_BY_NAME equ ( PT_TSTRING shl 16)+ 3A47h
PR_PREFERRED_BY_NAME_W equ ( PT_UNICODE shl 16)+ 3A47h
PR_PREFERRED_BY_NAME_A equ ( PT_STRING8 shl 16)+ 3A47h
PR_SPOUSE_NAME equ ( PT_TSTRING shl 16)+ 3A48h
PR_SPOUSE_NAME_W equ ( PT_UNICODE shl 16)+ 3A48h
PR_SPOUSE_NAME_A equ ( PT_STRING8 shl 16)+ 3A48h

PR_COMPUTER_NETWORK_NAME equ ( PT_TSTRING shl 16)+ 3A49h
PR_COMPUTER_NETWORK_NAME_W equ ( PT_UNICODE shl 16)+ 3A49h
PR_COMPUTER_NETWORK_NAME_A equ ( PT_STRING8 shl 16)+ 3A49h
PR_CUSTOMER_ID equ ( PT_TSTRING shl 16)+ 3A4Ah
PR_CUSTOMER_ID_W equ ( PT_UNICODE shl 16)+ 3A4Ah
PR_CUSTOMER_ID_A equ ( PT_STRING8 shl 16)+ 3A4Ah
PR_TTYTDD_PHONE_NUMBER equ ( PT_TSTRING shl 16)+ 3A4Bh
PR_TTYTDD_PHONE_NUMBER_W equ ( PT_UNICODE shl 16)+ 3A4Bh
PR_TTYTDD_PHONE_NUMBER_A equ ( PT_STRING8 shl 16)+ 3A4Bh
PR_FTP_SITE equ ( PT_TSTRING shl 16)+ 3A4Ch
PR_FTP_SITE_W equ ( PT_UNICODE shl 16)+ 3A4Ch
PR_FTP_SITE_A equ ( PT_STRING8 shl 16)+ 3A4Ch
PR_GENDER equ ( PT_SHORT shl 16)+ 3A4Dh
PR_MANAGER_NAME equ ( PT_TSTRING shl 16)+ 3A4Eh
PR_MANAGER_NAME_W equ ( PT_UNICODE shl 16)+ 3A4Eh
PR_MANAGER_NAME_A equ ( PT_STRING8 shl 16)+ 3A4Eh
PR_NICKNAME equ ( PT_TSTRING shl 16)+ 3A4Fh
PR_NICKNAME_W equ ( PT_UNICODE shl 16)+ 3A4Fh
PR_NICKNAME_A equ ( PT_STRING8 shl 16)+ 3A4Fh
PR_PERSONAL_HOME_PAGE equ ( PT_TSTRING shl 16)+ 3A50h
PR_PERSONAL_HOME_PAGE_W equ ( PT_UNICODE shl 16)+ 3A50h
PR_PERSONAL_HOME_PAGE_A equ ( PT_STRING8 shl 16)+ 3A50h
PR_BUSINESS_HOME_PAGE equ ( PT_TSTRING shl 16)+ 3A51h
PR_BUSINESS_HOME_PAGE_W equ ( PT_UNICODE shl 16)+ 3A51h
PR_BUSINESS_HOME_PAGE_A equ ( PT_STRING8 shl 16)+ 3A51h
PR_CONTACT_VERSION equ ( PT_CLSID shl 16)+ 3A52h
PR_CONTACT_ENTRYIDS equ ( PT_MV_BINARY shl 16)+ 3A53h
PR_CONTACT_ADDRTYPES equ ( PT_MV_TSTRING shl 16)+ 3A54h
PR_CONTACT_ADDRTYPES_W equ ( PT_MV_UNICODE shl 16)+ 3A54h
PR_CONTACT_ADDRTYPES_A equ ( PT_MV_STRING8 shl 16)+ 3A54h
PR_CONTACT_DEFAULT_ADDRESS_INDEX equ ( PT_LONG shl 16)+ 3A55h
PR_CONTACT_EMAIL_ADDRESSES equ ( PT_MV_TSTRING shl 16)+ 3A56h
PR_CONTACT_EMAIL_ADDRESSES_W equ ( PT_MV_UNICODE shl 16)+ 3A56h
PR_CONTACT_EMAIL_ADDRESSES_A equ ( PT_MV_STRING8 shl 16)+ 3A56h
PR_COMPANY_MAIN_PHONE_NUMBER equ ( PT_TSTRING shl 16)+ 3A57h
PR_COMPANY_MAIN_PHONE_NUMBER_W equ ( PT_UNICODE shl 16)+ 3A57h
PR_COMPANY_MAIN_PHONE_NUMBER_A equ ( PT_STRING8 shl 16)+ 3A57h
PR_CHILDRENS_NAMES equ ( PT_MV_TSTRING shl 16)+ 3A58h
PR_CHILDRENS_NAMES_W equ ( PT_MV_UNICODE shl 16)+ 3A58h
PR_CHILDRENS_NAMES_A equ ( PT_MV_STRING8 shl 16)+ 3A58h
PR_HOME_ADDRESS_CITY equ ( PT_TSTRING shl 16)+ 3A59h
PR_HOME_ADDRESS_CITY_W equ ( PT_UNICODE shl 16)+ 3A59h
PR_HOME_ADDRESS_CITY_A equ ( PT_STRING8 shl 16)+ 3A59h
PR_HOME_ADDRESS_COUNTRY equ ( PT_TSTRING shl 16)+ 3A5Ah
PR_HOME_ADDRESS_COUNTRY_W equ ( PT_UNICODE shl 16)+ 3A5Ah
PR_HOME_ADDRESS_COUNTRY_A equ ( PT_STRING8 shl 16)+ 3A5Ah
PR_HOME_ADDRESS_POSTAL_CODE equ ( PT_TSTRING shl 16)+ 3A5Bh
PR_HOME_ADDRESS_POSTAL_CODE_W equ ( PT_UNICODE shl 16)+ 3A5Bh
PR_HOME_ADDRESS_POSTAL_CODE_A equ ( PT_STRING8 shl 16)+ 3A5Bh
PR_HOME_ADDRESS_STATE_OR_PROVINCE equ ( PT_TSTRING shl 16)+ 3A5Ch
PR_HOME_ADDRESS_STATE_OR_PROVINCE_W equ ( PT_UNICODE shl 16)+ 3A5Ch
PR_HOME_ADDRESS_STATE_OR_PROVINCE_A equ ( PT_STRING8 shl 16)+ 3A5Ch
PR_HOME_ADDRESS_STREET equ ( PT_TSTRING shl 16)+ 3A5Dh
PR_HOME_ADDRESS_STREET_W equ ( PT_UNICODE shl 16)+ 3A5Dh
PR_HOME_ADDRESS_STREET_A equ ( PT_STRING8 shl 16)+ 3A5Dh
PR_HOME_ADDRESS_POST_OFFICE_BOX equ ( PT_TSTRING shl 16)+ 3A5Eh
PR_HOME_ADDRESS_POST_OFFICE_BOX_W equ ( PT_UNICODE shl 16)+ 3A5Eh
PR_HOME_ADDRESS_POST_OFFICE_BOX_A equ ( PT_STRING8 shl 16)+ 3A5Eh
PR_OTHER_ADDRESS_CITY equ ( PT_TSTRING shl 16)+ 3A5Fh
PR_OTHER_ADDRESS_CITY_W equ ( PT_UNICODE shl 16)+ 3A5Fh
PR_OTHER_ADDRESS_CITY_A equ ( PT_STRING8 shl 16)+ 3A5Fh
PR_OTHER_ADDRESS_COUNTRY equ ( PT_TSTRING shl 16)+ 3A60h
PR_OTHER_ADDRESS_COUNTRY_W equ ( PT_UNICODE shl 16)+ 3A60h
PR_OTHER_ADDRESS_COUNTRY_A equ ( PT_STRING8 shl 16)+ 3A60h
PR_OTHER_ADDRESS_POSTAL_CODE equ ( PT_TSTRING shl 16)+ 3A61h
PR_OTHER_ADDRESS_POSTAL_CODE_W equ ( PT_UNICODE shl 16)+ 3A61h
PR_OTHER_ADDRESS_POSTAL_CODE_A equ ( PT_STRING8 shl 16)+ 3A61h
PR_OTHER_ADDRESS_STATE_OR_PROVINCE equ ( PT_TSTRING shl 16)+ 3A62h
PR_OTHER_ADDRESS_STATE_OR_PROVINCE_W equ ( PT_UNICODE shl 16)+ 3A62h
PR_OTHER_ADDRESS_STATE_OR_PROVINCE_A equ ( PT_STRING8 shl 16)+ 3A62h
PR_OTHER_ADDRESS_STREET equ ( PT_TSTRING shl 16)+ 3A63h
PR_OTHER_ADDRESS_STREET_W equ ( PT_UNICODE shl 16)+ 3A63h
PR_OTHER_ADDRESS_STREET_A equ ( PT_STRING8 shl 16)+ 3A63h
PR_OTHER_ADDRESS_POST_OFFICE_BOX equ ( PT_TSTRING shl 16)+ 3A64h
PR_OTHER_ADDRESS_POST_OFFICE_BOX_W equ ( PT_UNICODE shl 16)+ 3A64h
PR_OTHER_ADDRESS_POST_OFFICE_BOX_A equ ( PT_STRING8 shl 16)+ 3A64h
PR_STORE_PROVIDERS equ ( PT_BINARY shl 16)+    3D00h
PR_AB_PROVIDERS equ ( PT_BINARY shl 16)+    3D01h
PR_TRANSPORT_PROVIDERS equ ( PT_BINARY shl 16)+    3D02h
PR_DEFAULT_PROFILE equ ( PT_BOOLEAN shl 16)+   3D04h
PR_AB_SEARCH_PATH equ ( PT_MV_BINARY shl 16)+ 3D05h
PR_AB_DEFAULT_DIR equ ( PT_BINARY shl 16)+    3D06h
PR_AB_DEFAULT_PAB equ ( PT_BINARY shl 16)+    3D07h
PR_FILTERING_HOOKS equ ( PT_BINARY shl 16)+    3D08h
PR_SERVICE_NAME equ ( PT_TSTRING shl 16)+   3D09h
PR_SERVICE_NAME_W equ ( PT_UNICODE shl 16)+   3D09h
PR_SERVICE_NAME_A equ ( PT_STRING8 shl 16)+   3D09h
PR_SERVICE_DLL_NAME equ ( PT_TSTRING shl 16)+   3D0Ah
PR_SERVICE_DLL_NAME_W equ ( PT_UNICODE shl 16)+   3D0Ah
PR_SERVICE_DLL_NAME_A equ ( PT_STRING8 shl 16)+   3D0Ah
PR_SERVICE_ENTRY_NAME equ ( PT_STRING8 shl 16)+   3D0Bh
PR_SERVICE_UID equ ( PT_BINARY shl 16)+    3D0Ch
PR_SERVICE_EXTRA_UIDS equ ( PT_BINARY shl 16)+    3D0Dh
PR_SERVICES equ ( PT_BINARY shl 16)+    3D0Eh
PR_SERVICE_SUPPORT_FILES equ ( PT_MV_TSTRING shl 16)+ 3D0Fh
PR_SERVICE_SUPPORT_FILES_W equ ( PT_MV_UNICODE shl 16)+ 3D0Fh
PR_SERVICE_SUPPORT_FILES_A equ ( PT_MV_STRING8 shl 16)+ 3D0Fh
PR_SERVICE_DELETE_FILES equ ( PT_MV_TSTRING shl 16)+ 3D10h
PR_SERVICE_DELETE_FILES_W equ ( PT_MV_UNICODE shl 16)+ 3D10h
PR_SERVICE_DELETE_FILES_A equ ( PT_MV_STRING8 shl 16)+ 3D10h
PR_AB_SEARCH_PATH_UPDATE equ ( PT_BINARY shl 16)+     3D11h
PR_PROFILE_NAME equ ( PT_TSTRING shl 16)+   3D12h
PR_PROFILE_NAME_A equ ( PT_STRING8 shl 16)+   3D12h
PR_PROFILE_NAME_W equ ( PT_UNICODE shl 16)+   3D12h
PR_IDENTITY_DISPLAY equ ( PT_TSTRING shl 16)+   3E00h
PR_IDENTITY_DISPLAY_W equ ( PT_UNICODE shl 16)+   3E00h
PR_IDENTITY_DISPLAY_A equ ( PT_STRING8 shl 16)+   3E00h
PR_IDENTITY_ENTRYID equ ( PT_BINARY shl 16)+    3E01h
PR_RESOURCE_METHODS equ ( PT_LONG shl 16)+      3E02h
PR_RESOURCE_TYPE equ ( PT_LONG shl 16)+      3E03h
PR_STATUS_CODE equ ( PT_LONG shl 16)+      3E04h
PR_IDENTITY_SEARCH_KEY equ ( PT_BINARY shl 16)+    3E05h
PR_OWN_STORE_ENTRYID equ ( PT_BINARY shl 16)+    3E06h
PR_RESOURCE_PATH equ ( PT_TSTRING shl 16)+   3E07h
PR_RESOURCE_PATH_W equ ( PT_UNICODE shl 16)+   3E07h
PR_RESOURCE_PATH_A equ ( PT_STRING8 shl 16)+   3E07h
PR_STATUS_STRING equ ( PT_TSTRING shl 16)+   3E08h
PR_STATUS_STRING_W equ ( PT_UNICODE shl 16)+   3E08h
PR_STATUS_STRING_A equ ( PT_STRING8 shl 16)+   3E08h
PR_X400_DEFERRED_DELIVERY_CANCEL equ ( PT_BOOLEAN shl 16)+   3E09h
PR_HEADER_FOLDER_ENTRYID equ ( PT_BINARY shl 16)+    3E0Ah
PR_REMOTE_PROGRESS equ ( PT_LONG shl 16)+      3E0Bh
PR_REMOTE_PROGRESS_TEXT equ ( PT_TSTRING shl 16)+   3E0Ch
PR_REMOTE_PROGRESS_TEXT_W equ ( PT_UNICODE shl 16)+   3E0Ch
PR_REMOTE_PROGRESS_TEXT_A equ ( PT_STRING8 shl 16)+   3E0Ch
PR_REMOTE_VALIDATE_OK equ ( PT_BOOLEAN shl 16)+   3E0Dh
PR_CONTROL_FLAGS equ ( PT_LONG shl 16)+      3F00h
PR_CONTROL_STRUCTURE equ ( PT_BINARY shl 16)+    3F01h
PR_CONTROL_TYPE equ ( PT_LONG shl 16)+      3F02h
PR_DELTAX equ ( PT_LONG shl 16)+      3F03h
PR_DELTAY equ ( PT_LONG shl 16)+      3F04h
PR_XPOS equ ( PT_LONG shl 16)+      3F05h
PR_YPOS equ ( PT_LONG shl 16)+      3F06h
PR_CONTROL_ID equ ( PT_BINARY shl 16)+    3F07h
PR_INITIAL_DETAILS_PANE equ ( PT_LONG shl 16)+      3F08h
PROP_ID_SECURE_MIN equ 67F0h
PROP_ID_SECURE_MAX equ 67FFh

;----------------------------------[ mapiutil.h ]------------------------
IPROP_READONLY equ 1
IPROP_READWRITE equ 2
IPROP_CLEAN equ 10000h
IPROP_DIRTY equ 20000h
PRILOWEST equ -32768
PRIHIGHEST equ 32767
PRIUSER equ 0
IRONULL equ 0
FIROWAIT equ 1
FIROINTERVAL equ 2
FIROPERBLOCK equ 4
FIRODISABLED equ 20h
FIROONCEONLY equ 40h
IRCNULL equ 0
FIRCPFN equ 1
FIRCPV equ 2
FIRCPRI equ 4
FIRCCSEC equ 8
FIRCIRO equ 10h
MAPI_FORCE_CREATE equ 1

MAPI_FULL_IPM_TREE equ 2
RTF_SYNC_RTF_CHANGED equ 1
RTF_SYNC_BODY_CHANGED equ 2

DTCTL STRUCT
    ulCtlType dd ?
    ulCtlFlags dd ?
    lpbNotif dd ?
    cbNotif dd ?
    lpszFilter dd ?
    ulItemID dd ?
    union ctl
        lpv dd ?
        lplabel dd ?
        lpedit dd ?
        lplbx dd ?
        lpcombobox dd ?
        lpddlbx dd ?
        lpcheckbox dd ?
        lpgroupbox dd ?
        lpbutton dd ?
        lpradiobutton dd ?
        lpmvlbx dd ?
        lpmvddlbx dd ?
        lppage dd ?
	  ends
DTCTL ENDS

DTPAGE STRUCT
    cctl dd ?
    lpszResourceName dd ?
    union
        lpszComponent dd ?
        ulItemID dd ?
    ends
    lpctl dd ?
DTPAGE ENDS

;-----------------------------------[mapiwz.h]-------------------------------
WIZ_QUERYNUMPAGES equ WM_USER+10
WIZ_NEXT equ WM_USER +11
WIZ_PREV equ WM_USER +12
MAPI_PW_FIRST_PROFILE equ 1
MAPI_PW_LAUNCHED_BY_CONFIG equ 2
MAPI_PW_ADD_SERVICE_ONLY equ 4
MAPI_PW_PROVIDER_UI_ONLY equ 8
MAPI_PW_HIDE_SERVICES_LIST equ 10h
PR_WIZARD_NO_PST_PAGE equ (PT_BOOLEAN shl 16)+6700h
LAUNCHWIZARDENTRYNAME equ "LAUNCHWIZARD"

;--------------------------------[ mapix.h]-----------------------------
MAPI_LOGON_UI equ 1
MAPI_NEW_SESSION equ 2
MAPI_ALLOW_OTHERS equ 8
MAPI_EXPLICIT_PROFILE equ 10h
MAPI_EXTENDED equ 20h
MAPI_FORCE_DOWNLOAD equ 1000h
MAPI_SERVICE_UI_ALWAYS equ 2000h
MAPI_NO_MAIL equ 8000h
MAPI_PASSWORD_UI equ 20000h
MAPI_TIMEOUT_SHORT equ 100000h
MAPI_SIMPLE_DEFAULT equ (MAPI_LOGON_UI + MAPI_FORCE_DOWNLOAD + MAPI_ALLOW_OTHERS)
MAPI_SIMPLE_EXPLICIT equ (MAPI_NEW_SESSION + MAPI_FORCE_DOWNLOAD + MAPI_EXPLICIT_PROFILE)
MAPI_INIT_VERSION equ 0
MAPI_MULTITHREAD_NOTIFICATIONS equ 1
MAPI_LOGOFF_SHARED equ 1
MAPI_LOGOFF_UI equ 2
MAPI_DEFAULT_STORE equ 1
MAPI_SIMPLE_STORE_TEMPORARY equ 2
MAPI_SIMPLE_STORE_PERMANENT equ 3
MAPI_PRIMARY_STORE equ 4
MAPI_SECONDARY_STORE equ 5
MAPI_POST_MESSAGE equ 1
MAPI_NEW_MESSAGE equ 2
MAPI_DEFAULT_SERVICES equ 1
SERVICE_DEFAULT_STORE equ 1
SERVICE_SINGLE_COPY equ 2
SERVICE_CREATE_WITH_STORE equ 4
SERVICE_PRIMARY_IDENTITY equ 8
SERVICE_NO_PRIMARY_IDENTITY equ 20h

MAPIINIT_0 STRUCT
    ulVersion dd    ?
    ulFlags dd      ?
MAPIINIT_0 ENDS

;------------------------------------------------------------------------------------------
;                                   ODBCInst.h
;------------------------------------------------------------------------------------------
ODBC_ADD_DSN equ 1               
ODBC_CONFIG_DSN equ 2               
ODBC_REMOVE_DSN equ 3               
ODBC_ADD_SYS_DSN equ 4				  
ODBC_CONFIG_SYS_DSN	equ 5		  
ODBC_REMOVE_SYS_DSN	equ 6		  
ODBC_REMOVE_DEFAULT_DSN	equ 7		
ODBC_INSTALL_INQUIRY	equ 1		
ODBC_INSTALL_COMPLETE	equ 2
ODBC_INSTALL_DRIVER	equ 1
ODBC_REMOVE_DRIVER	equ 2
ODBC_CONFIG_DRIVER	equ 3
ODBC_CONFIG_DRIVER_MAX equ 100
ODBC_BOTH_DSN	equ 0
ODBC_USER_DSN	equ 1
ODBC_SYSTEM_DSN	equ 2
ODBC_ERROR_GENERAL_ERR equ 1
ODBC_ERROR_INVALID_BUFF_LEN equ 2
ODBC_ERROR_INVALID_HWND equ 3
ODBC_ERROR_INVALID_STR equ 4
ODBC_ERROR_INVALID_REQUEST_TYPE equ 5
ODBC_ERROR_COMPONENT_NOT_FOUND equ 6
ODBC_ERROR_INVALID_NAME equ 7
ODBC_ERROR_INVALID_KEYWORD_VALUE equ 8
ODBC_ERROR_INVALID_DSN equ 9
ODBC_ERROR_INVALID_INF equ 10
ODBC_ERROR_REQUEST_FAILED equ 11
ODBC_ERROR_INVALID_PATH equ 12
ODBC_ERROR_LOAD_LIB_FAILED equ 13
ODBC_ERROR_INVALID_PARAM_SEQUENCE equ 14
ODBC_ERROR_INVALID_LOG_FILE equ 15
ODBC_ERROR_USER_CANCELED equ 16
ODBC_ERROR_USAGE_UPDATE_FAILED equ 17
ODBC_ERROR_CREATE_DSN_FAILED equ 18
ODBC_ERROR_WRITING_SYSINFO_FAILED equ 19
ODBC_ERROR_REMOVE_DSN_FAILED equ 20
ODBC_ERROR_OUT_OF_MEM equ 21
ODBC_ERROR_OUTPUT_STRING_TRUNCATED equ 22
;---------------------------------------------------------------------------------------------------------------
;							sql.h
;---------------------------------------------------------------------------------------------------------------
SQL_NULL_DATA equ (-1)
SQL_DATA_AT_EXEC equ (-2)
SQL_SUCCESS equ 0
SQL_SUCCESS_WITH_INFO equ 1
SQL_NO_DATA equ 100
SQL_ERROR equ (-1)
SQL_INVALID_HANDLE equ (-2)
SQL_STILL_EXECUTING equ 2
SQL_NEED_DATA equ 99
SQL_NTS equ (-3)
SQL_NTSL equ (-3L)
SQL_MAX_MESSAGE_LENGTH equ 512
SQL_DATE_LEN equ 10
SQL_TIME_LEN equ 8
SQL_TIMESTAMP_LEN equ 19
SQL_HANDLE_ENV equ 1
SQL_HANDLE_DBC equ 2
SQL_HANDLE_STMT equ 3
SQL_HANDLE_DESC equ 4
SQL_ATTR_OUTPUT_NTS equ 10001
SQL_ATTR_AUTO_IPD equ 10001
SQL_ATTR_METADATA_ID equ 10014
SQL_ATTR_APP_ROW_DESC equ 10010
SQL_ATTR_APP_PARAM_DESC equ 10011
SQL_ATTR_IMP_ROW_DESC equ 10012
SQL_ATTR_IMP_PARAM_DESC equ 10013
SQL_ATTR_CURSOR_SCROLLABLE equ (-1)
SQL_ATTR_CURSOR_SENSITIVITY equ (-2)
SQL_NONSCROLLABLE	equ 0
SQL_SCROLLABLE	equ 1
SQL_DESC_COUNT equ 1001
SQL_DESC_TYPE equ 1002
SQL_DESC_LENGTH equ 1003
SQL_DESC_OCTET_LENGTH_PTR equ 1004
SQL_DESC_PRECISION equ 1005
SQL_DESC_SCALE equ 1006
SQL_DESC_DATETIME_INTERVAL_CODE equ 1007
SQL_DESC_NULLABLE equ 1008
SQL_DESC_INDICATOR_PTR equ 1009
SQL_DESC_DATA_PTR equ 1010
SQL_DESC_NAME equ 1011
SQL_DESC_UNNAMED equ 1012
SQL_DESC_OCTET_LENGTH equ 1013
SQL_DESC_ALLOC_TYPE equ 1099
SQL_DIAG_RETURNCODE equ 1
SQL_DIAG_NUMBER equ 2
SQL_DIAG_ROW_COUNT equ 3
SQL_DIAG_SQLSTATE equ 4
SQL_DIAG_NATIVE equ 5
SQL_DIAG_MESSAGE_TEXT equ 6
SQL_DIAG_DYNAMIC_FUNCTION equ 7
SQL_DIAG_CLASS_ORIGIN equ 8
SQL_DIAG_SUBCLASS_ORIGIN equ 9
SQL_DIAG_CONNECTION_NAME equ 10
SQL_DIAG_SERVER_NAME equ 11
SQL_DIAG_DYNAMIC_FUNCTION_CODE equ 12
SQL_DIAG_ALTER_DOMAIN	equ 3
SQL_DIAG_ALTER_TABLE equ 4
SQL_DIAG_CALL	equ 7
SQL_DIAG_CREATE_ASSERTION	equ 6
SQL_DIAG_CREATE_CHARACTER_SET	equ 8
SQL_DIAG_CREATE_COLLATION	equ 10
SQL_DIAG_CREATE_DOMAIN	equ 23
SQL_DIAG_CREATE_INDEX equ (-1)
SQL_DIAG_CREATE_SCHEMA	equ 64
SQL_DIAG_CREATE_TABLE equ 77
SQL_DIAG_CREATE_TRANSLATION	equ 79
SQL_DIAG_CREATE_VIEW equ 84
SQL_DIAG_DELETE_WHERE equ 19
SQL_DIAG_DROP_ASSERTION	equ 24
SQL_DIAG_DROP_CHARACTER_SET	equ 25
SQL_DIAG_DROP_COLLATION	equ 26
SQL_DIAG_DROP_DOMAIN	equ 27
SQL_DIAG_DROP_INDEX equ (-2)
SQL_DIAG_DROP_SCHEMA	equ 31
SQL_DIAG_DROP_TABLE equ 32
SQL_DIAG_DROP_TRANSLATION equ 33
SQL_DIAG_DROP_VIEW equ 36
SQL_DIAG_DYNAMIC_DELETE_CURSOR equ 38
SQL_DIAG_DYNAMIC_UPDATE_CURSOR equ 81
SQL_DIAG_GRANT equ 48
SQL_DIAG_INSERT equ 50
SQL_DIAG_REVOKE equ 59
SQL_DIAG_SELECT_CURSOR equ 85
SQL_DIAG_UNKNOWN_STATEMENT equ 0
SQL_DIAG_UPDATE_WHERE equ 82
SQL_UNKNOWN_TYPE	equ 0
SQL_CHAR equ 1
SQL_NUMERIC equ 2
SQL_DECIMAL equ 3
SQL_INTEGER equ 4
SQL_SMALLINT equ 5
SQL_FLOAT equ 6
SQL_REAL equ 7
SQL_DOUBLE equ 8
SQL_DATETIME equ 9
SQL_VARCHAR equ 12
SQL_TYPE_DATE equ 91
SQL_TYPE_TIME equ 92
SQL_TYPE_TIMESTAMP equ 93
SQL_UNSPECIFIED equ 0
SQL_INSENSITIVE equ 1
SQL_SENSITIVE equ 2
SQL_ALL_TYPES equ 0
SQL_DEFAULT equ 99
SQL_ARD_TYPE equ (-99)
SQL_CODE_DATE equ 1
SQL_CODE_TIME equ 2
SQL_CODE_TIMESTAMP equ 3
SQL_FALSE equ 0
SQL_TRUE equ 1
SQL_NO_NULLS equ 0
SQL_NULLABLE equ 1
SQL_NULLABLE_UNKNOWN equ 2
SQL_PRED_NONE equ 0
SQL_PRED_CHAR equ 1
SQL_PRED_BASIC equ 2
SQL_NAMED equ 0
SQL_UNNAMED equ 1
SQL_DESC_ALLOC_AUTO equ 1
SQL_DESC_ALLOC_USER equ 2
SQL_CLOSE equ 0
SQL_DROP equ 1
SQL_UNBIND equ 2
SQL_RESET_PARAMS equ 3
SQL_FETCH_NEXT equ 1
SQL_FETCH_FIRST equ 2
SQL_FETCH_LAST equ 3
SQL_FETCH_PRIOR equ 4
SQL_FETCH_ABSOLUTE equ 5
SQL_FETCH_RELATIVE equ 6
SQL_COMMIT equ 0
SQL_ROLLBACK equ 1
SQL_NULL_HENV equ 0
SQL_NULL_HDBC equ 0
SQL_NULL_HSTMT equ 0
SQL_NULL_HDESC equ 0
SQL_NULL_HANDLE equ 0
SQL_SCOPE_CURROW equ 0
SQL_SCOPE_TRANSACTION equ 1
SQL_SCOPE_SESSION equ 2
SQL_PC_UNKNOWN equ 0
SQL_PC_NON_PSEUDO equ 1
SQL_PC_PSEUDO equ 2
SQL_ROW_IDENTIFIER equ 1
SQL_INDEX_UNIQUE equ 0
SQL_INDEX_ALL equ 1
SQL_INDEX_CLUSTERED equ 1
SQL_INDEX_HASHED equ 2
SQL_INDEX_OTHER equ 3
SQL_API_SQLALLOCCONNECT equ 1
SQL_API_SQLALLOCENV equ 2
SQL_API_SQLALLOCHANDLE equ 1001
SQL_API_SQLALLOCSTMT equ 3
SQL_API_SQLBINDCOL equ 4
SQL_API_SQLBINDPARAM equ 1002
SQL_API_SQLCANCEL equ 5
SQL_API_SQLCLOSECURSOR equ 1003
SQL_API_SQLCOLATTRIBUTE equ 6
SQL_API_SQLCOLUMNS equ 40
SQL_API_SQLCONNECT equ 7
SQL_API_SQLCOPYDESC equ 1004
SQL_API_SQLDATASOURCES equ 57
SQL_API_SQLDESCRIBECOL equ 8
SQL_API_SQLDISCONNECT equ 9
SQL_API_SQLENDTRAN equ 1005
SQL_API_SQLERROR equ 10
SQL_API_SQLEXECDIRECT equ 11
SQL_API_SQLEXECUTE equ 12
SQL_API_SQLFETCH equ 13
SQL_API_SQLFETCHSCROLL equ 1021
SQL_API_SQLFREECONNECT equ 14
SQL_API_SQLFREEENV equ 15
SQL_API_SQLFREEHANDLE equ 1006
SQL_API_SQLFREESTMT equ 16
SQL_API_SQLGETCONNECTATTR equ 1007
SQL_API_SQLGETCONNECTOPTION equ 42
SQL_API_SQLGETCURSORNAME equ 17
SQL_API_SQLGETDATA equ 43
SQL_API_SQLGETDESCFIELD equ 1008
SQL_API_SQLGETDESCREC equ 1009
SQL_API_SQLGETDIAGFIELD equ 1010
SQL_API_SQLGETDIAGREC equ 1011
SQL_API_SQLGETENVATTR equ 1012
SQL_API_SQLGETFUNCTIONS equ 44
SQL_API_SQLGETINFO equ 45
SQL_API_SQLGETSTMTATTR equ 1014
SQL_API_SQLGETSTMTOPTION equ 46
SQL_API_SQLGETTYPEINFO equ 47
SQL_API_SQLNUMRESULTCOLS equ 18
SQL_API_SQLPARAMDATA equ 48
SQL_API_SQLPREPARE equ 19
SQL_API_SQLPUTDATA equ 49
SQL_API_SQLROWCOUNT equ 20
SQL_API_SQLSETCONNECTATTR equ 1016
SQL_API_SQLSETCONNECTOPTION equ 50
SQL_API_SQLSETCURSORNAME equ 21
SQL_API_SQLSETDESCFIELD equ 1017
SQL_API_SQLSETDESCREC equ 1018
SQL_API_SQLSETENVATTR equ 1019
SQL_API_SQLSETPARAM equ 22
SQL_API_SQLSETSTMTATTR equ 1020
SQL_API_SQLSETSTMTOPTION equ 51
SQL_API_SQLSPECIALCOLUMNS equ 52
SQL_API_SQLSTATISTICS equ 53
SQL_API_SQLTABLES equ 54
SQL_API_SQLTRANSACT equ 23
SQL_MAX_DRIVER_CONNECTIONS equ 0
SQL_MAXIMUM_DRIVER_CONNECTIONS	equ SQL_MAX_DRIVER_CONNECTIONS
SQL_MAX_CONCURRENT_ACTIVITIES equ 1
SQL_MAXIMUM_CONCURRENT_ACTIVITIES	equ SQL_MAX_CONCURRENT_ACTIVITIES
SQL_DATA_SOURCE_NAME equ 2
SQL_FETCH_DIRECTION equ 8
SQL_SERVER_NAME equ 13
SQL_SEARCH_PATTERN_ESCAPE equ 14
SQL_DBMS_NAME equ 17
SQL_DBMS_VER equ 18
SQL_ACCESSIBLE_TABLES equ 19
SQL_ACCESSIBLE_PROCEDURES equ 20
SQL_CURSOR_COMMIT_BEHAVIOR equ 23
SQL_DATA_SOURCE_READ_ONLY equ 25
SQL_DEFAULT_TXN_ISOLATION equ 26
SQL_IDENTIFIER_CASE equ 28
SQL_IDENTIFIER_QUOTE_CHAR equ 29
SQL_MAX_COLUMN_NAME_LEN equ 30
SQL_MAXIMUM_COLUMN_NAME_LENGTH	equ SQL_MAX_COLUMN_NAME_LEN
SQL_MAX_CURSOR_NAME_LEN equ 31
SQL_MAXIMUM_CURSOR_NAME_LENGTH	equ SQL_MAX_CURSOR_NAME_LEN
SQL_MAX_SCHEMA_NAME_LEN equ 32
SQL_MAXIMUM_SCHEMA_NAME_LENGTH	equ SQL_MAX_SCHEMA_NAME_LEN
SQL_MAX_CATALOG_NAME_LEN equ 34
SQL_MAXIMUM_CATALOG_NAME_LENGTH	equ SQL_MAX_CATALOG_NAME_LEN
SQL_MAX_TABLE_NAME_LEN equ 35
SQL_SCROLL_CONCURRENCY equ 43
SQL_TXN_CAPABLE equ 46
SQL_TRANSACTION_CAPABLE	equ SQL_TXN_CAPABLE
SQL_USER_NAME equ 47
SQL_TXN_ISOLATION_OPTION equ 72
SQL_TRANSACTION_ISOLATION_OPTION	equ SQL_TXN_ISOLATION_OPTION
SQL_INTEGRITY equ 73
SQL_GETDATA_EXTENSIONS equ 81
SQL_NULL_COLLATION equ 85
SQL_ALTER_TABLE equ 86
SQL_ORDER_BY_COLUMNS_IN_SELECT equ 90
SQL_SPECIAL_CHARACTERS equ 94
SQL_MAX_COLUMNS_IN_GROUP_BY equ 97
SQL_MAXIMUM_COLUMNS_IN_GROUP_BY	equ SQL_MAX_COLUMNS_IN_GROUP_BY
SQL_MAX_COLUMNS_IN_INDEX equ 98
SQL_MAXIMUM_COLUMNS_IN_INDEX	equ SQL_MAX_COLUMNS_IN_INDEX
SQL_MAX_COLUMNS_IN_ORDER_BY equ 99
SQL_MAXIMUM_COLUMNS_IN_ORDER_BY	equ SQL_MAX_COLUMNS_IN_ORDER_BY
SQL_MAX_COLUMNS_IN_SELECT equ 100
SQL_MAXIMUM_COLUMNS_IN_SELECT	equ SQL_MAX_COLUMNS_IN_SELECT
SQL_MAX_COLUMNS_IN_TABLE equ 101
SQL_MAX_INDEX_SIZE equ 102
SQL_MAXIMUM_INDEX_SIZE	equ SQL_MAX_INDEX_SIZE
SQL_MAX_ROW_SIZE equ 104
SQL_MAXIMUM_ROW_SIZE	equ SQL_MAX_ROW_SIZE
SQL_MAX_STATEMENT_LEN equ 105
SQL_MAXIMUM_STATEMENT_LENGTH	equ SQL_MAX_STATEMENT_LEN
SQL_MAX_TABLES_IN_SELECT equ 106
SQL_MAXIMUM_TABLES_IN_SELECT	equ SQL_MAX_TABLES_IN_SELECT
SQL_MAX_USER_NAME_LEN equ 107
SQL_MAXIMUM_USER_NAME_LENGTH	equ SQL_MAX_USER_NAME_LEN
SQL_OJ_CAPABILITIES equ 115
SQL_OUTER_JOIN_CAPABILITIES	equ SQL_OJ_CAPABILITIES
SQL_XOPEN_CLI_YEAR equ 10000
SQL_CURSOR_SENSITIVITY equ 10001
SQL_DESCRIBE_PARAMETER equ 10002
SQL_CATALOG_NAME equ 10003
SQL_COLLATION_SEQ equ 10004
SQL_MAX_IDENTIFIER_LEN equ 10005
SQL_MAXIMUM_IDENTIFIER_LENGTH	equ SQL_MAX_IDENTIFIER_LEN
SQL_AT_ADD_COLUMN equ 1
SQL_AT_DROP_COLUMN equ 2
SQL_AT_ADD_CONSTRAINT equ 8
SQL_AT_COLUMN_SINGLE	equ 20h	
SQL_AT_ADD_COLUMN_DEFAULT	equ 40h
SQL_AT_ADD_COLUMN_COLLATION	equ 80h
SQL_AT_SET_COLUMN_DEFAULT	equ 100h
SQL_AT_DROP_COLUMN_DEFAULT	equ 200h
SQL_AT_DROP_COLUMN_CASCADE	equ 400h
SQL_AT_DROP_COLUMN_RESTRICT	equ 800h
SQL_AT_ADD_TABLE_CONSTRAINT	equ 1000h		
SQL_AT_DROP_TABLE_CONSTRAINT_CASCADE	equ 2000h		
SQL_AT_DROP_TABLE_CONSTRAINT_RESTRICT	equ 4000h		
SQL_AT_CONSTRAINT_NAME_DEFINITION	equ 8000h
SQL_AT_CONSTRAINT_INITIALLY_DEFERRED	equ 10000h
SQL_AT_CONSTRAINT_INITIALLY_IMMEDIATE	equ 20000h
SQL_AT_CONSTRAINT_DEFERRABLE	equ 40000h
SQL_AT_CONSTRAINT_NON_DEFERRABLE	equ 80000h
SQL_AM_NONE equ 0
SQL_AM_CONNECTION equ 1
SQL_AM_STATEMENT equ 2
SQL_CB_DELETE equ 0
SQL_CB_CLOSE equ 1
SQL_CB_PRESERVE equ 2
SQL_FD_FETCH_NEXT equ 1
SQL_FD_FETCH_FIRST equ 2
SQL_FD_FETCH_LAST equ 4
SQL_FD_FETCH_PRIOR equ 8
SQL_FD_FETCH_ABSOLUTE equ 10h
SQL_FD_FETCH_RELATIVE equ 20h
SQL_GD_ANY_COLUMN equ 1
SQL_GD_ANY_ORDER equ 2
SQL_IC_UPPER equ 1
SQL_IC_LOWER equ 2
SQL_IC_SENSITIVE equ 3
SQL_IC_MIXED equ 4
SQL_OJ_LEFT equ 1
SQL_OJ_RIGHT equ 2
SQL_OJ_FULL equ 4
SQL_OJ_NESTED equ 8
SQL_OJ_NOT_ORDERED equ 10h
SQL_OJ_INNER equ 20h
SQL_OJ_ALL_COMPARISON_OPS equ 40h
SQL_SCCO_READ_ONLY equ 1
SQL_SCCO_LOCK equ 2
SQL_SCCO_OPT_ROWVER equ 4
SQL_SCCO_OPT_VALUES equ 8
SQL_TC_NONE equ 0
SQL_TC_DML equ 1
SQL_TC_ALL equ 2
SQL_TC_DDL_COMMIT equ 3
SQL_TC_DDL_IGNORE equ 4
SQL_TXN_READ_UNCOMMITTED equ 1
SQL_TRANSACTION_READ_UNCOMMITTED	equ SQL_TXN_READ_UNCOMMITTED
SQL_TXN_READ_COMMITTED equ 2
SQL_TRANSACTION_READ_COMMITTED	equ SQL_TXN_READ_COMMITTED
SQL_TXN_REPEATABLE_READ equ 4
SQL_TRANSACTION_REPEATABLE_READ	equ SQL_TXN_REPEATABLE_READ
SQL_TXN_SERIALIZABLE equ 8
SQL_TRANSACTION_SERIALIZABLE	equ SQL_TXN_SERIALIZABLE
SQL_NC_HIGH equ 0
SQL_NC_LOW equ 1

;------------------------------------------------------------------------------------------------------------
;					sqlext.h
;-------------------------------------------------------------------------------------------------------------

SQL_SPEC_MAJOR equ 3
SQL_SPEC_MINOR	equ 51
SQL_SPEC_STRING equ "03.51"
SQL_SQLSTATE_SIZE	equ 5
SQL_MAX_DSN_LENGTH	equ 32
SQL_MAX_OPTION_STRING_LENGTH equ 256
SQL_NO_DATA_FOUND	equ SQL_NO_DATA
SQL_HANDLE_SENV	equ 5
SQL_ATTR_ODBC_VERSION	equ 200
SQL_ATTR_CONNECTION_POOLING	equ 201
SQL_ATTR_CP_MATCH	equ 202
SQL_CP_OFF	equ 0
SQL_CP_ONE_PER_DRIVER	equ 1
SQL_CP_ONE_PER_HENV	equ 2
SQL_CP_DEFAULT	equ SQL_CP_OFF
SQL_CP_STRICT_MATCH	equ 0
SQL_CP_RELAXED_MATCH	equ 1
SQL_CP_MATCH_DEFAULT	equ SQL_CP_STRICT_MATCH		
SQL_OV_ODBC2	equ 2
SQL_OV_ODBC3 equ 3
SQL_ACCESS_MODE equ 101
SQL_AUTOCOMMIT equ 102
SQL_LOGIN_TIMEOUT equ 103
SQL_OPT_TRACE equ 104
SQL_OPT_TRACEFILE equ 105
SQL_TRANSLATE_DLL equ 106
SQL_TRANSLATE_OPTION equ 107
SQL_TXN_ISOLATION equ 108
SQL_CURRENT_QUALIFIER equ 109
SQL_ODBC_CURSORS equ 110
SQL_QUIET_MODE equ 111
SQL_PACKET_SIZE equ 112
SQL_ATTR_ACCESS_MODE	equ SQL_ACCESS_MODE
SQL_ATTR_AUTOCOMMIT	equ SQL_AUTOCOMMIT
SQL_ATTR_CONNECTION_TIMEOUT	equ 113
SQL_ATTR_CURRENT_CATALOG	equ SQL_CURRENT_QUALIFIER
SQL_ATTR_DISCONNECT_BEHAVIOR	equ 114
SQL_ATTR_ENLIST_IN_DTC	equ 1207
SQL_ATTR_ENLIST_IN_XA	equ 1208
SQL_ATTR_LOGIN_TIMEOUT	equ SQL_LOGIN_TIMEOUT
SQL_ATTR_ODBC_CURSORS	equ SQL_ODBC_CURSORS
SQL_ATTR_PACKET_SIZE	equ SQL_PACKET_SIZE
SQL_ATTR_QUIET_MODE	equ SQL_QUIET_MODE
SQL_ATTR_TRACE	equ SQL_OPT_TRACE
SQL_ATTR_TRACEFILE	equ SQL_OPT_TRACEFILE
SQL_ATTR_TRANSLATE_LIB	equ SQL_TRANSLATE_DLL
SQL_ATTR_TRANSLATE_OPTION	equ SQL_TRANSLATE_OPTION
SQL_ATTR_TXN_ISOLATION	equ SQL_TXN_ISOLATION
SQL_ATTR_CONNECTION_DEAD	equ 1209
SQL_ATTR_ANSI_APP	equ 115
SQL_MODE_READ_WRITE equ 0
SQL_MODE_READ_ONLY equ 1
SQL_MODE_DEFAULT equ SQL_MODE_READ_WRITE
SQL_AUTOCOMMIT_OFF equ 0
SQL_AUTOCOMMIT_ON equ 1
SQL_AUTOCOMMIT_DEFAULT equ SQL_AUTOCOMMIT_ON
SQL_LOGIN_TIMEOUT_DEFAULT equ 15
SQL_OPT_TRACE_OFF equ 0
SQL_OPT_TRACE_ON equ 1
SQL_OPT_TRACE_DEFAULT equ SQL_OPT_TRACE_OFF
SQL_OPT_TRACE_FILE_DEFAULT equ "\\SQL.LOG"
SQL_CUR_USE_IF_NEEDED equ 0
SQL_CUR_USE_ODBC equ 1
SQL_CUR_USE_DRIVER equ 2
SQL_CUR_DEFAULT equ SQL_CUR_USE_DRIVER
SQL_DB_RETURN_TO_POOL	equ 0
SQL_DB_DISCONNECT	equ 1
SQL_DB_DEFAULT	equ SQL_DB_RETURN_TO_POOL
SQL_DTC_DONE	equ 0
SQL_CD_TRUE	equ 1
SQL_CD_FALSE	equ 0
SQL_AA_TRUE	equ 1
SQL_AA_FALSE	equ 0
SQL_QUERY_TIMEOUT	equ 0
SQL_MAX_ROWS	equ 1
SQL_NOSCAN	equ 2
SQL_MAX_LENGTH	equ 3
SQL_ASYNC_ENABLE	equ 4
SQL_BIND_TYPE	equ 5
SQL_CURSOR_TYPE	equ 6
SQL_CONCURRENCY	equ 7
SQL_KEYSET_SIZE	equ 8
SQL_ROWSET_SIZE	equ 9
SQL_SIMULATE_CURSOR	equ 10
SQL_RETRIEVE_DATA	equ 11
SQL_USE_BOOKMARKS	equ 12
SQL_GET_BOOKMARK	equ 13
SQL_ROW_NUMBER	equ 14
SQL_ATTR_ASYNC_ENABLE	equ 4
SQL_ATTR_CONCURRENCY	equ SQL_CONCURRENCY
SQL_ATTR_CURSOR_TYPE	equ SQL_CURSOR_TYPE
SQL_ATTR_ENABLE_AUTO_IPD	equ 15
SQL_ATTR_FETCH_BOOKMARK_PTR	equ 16
SQL_ATTR_KEYSET_SIZE	equ SQL_KEYSET_SIZE
SQL_ATTR_MAX_LENGTH	equ SQL_MAX_LENGTH
SQL_ATTR_MAX_ROWS	equ SQL_MAX_ROWS
SQL_ATTR_NOSCAN	equ SQL_NOSCAN
SQL_ATTR_PARAM_BIND_OFFSET_PTR	equ 17
SQL_ATTR_PARAM_BIND_TYPE	equ 18
SQL_ATTR_PARAM_OPERATION_PTR	equ 19
SQL_ATTR_PARAM_STATUS_PTR	equ 20
SQL_ATTR_PARAMS_PROCESSED_PTR	equ 21
SQL_ATTR_PARAMSET_SIZE	equ 22
SQL_ATTR_QUERY_TIMEOUT	equ SQL_QUERY_TIMEOUT
SQL_ATTR_RETRIEVE_DATA	equ SQL_RETRIEVE_DATA
SQL_ATTR_ROW_BIND_OFFSET_PTR	equ 23
SQL_ATTR_ROW_BIND_TYPE	equ SQL_BIND_TYPE
SQL_ATTR_ROW_NUMBER	equ SQL_ROW_NUMBER
SQL_ATTR_ROW_OPERATION_PTR	equ 24
SQL_ATTR_ROW_STATUS_PTR	equ 25
SQL_ATTR_ROWS_FETCHED_PTR	equ 26
SQL_ATTR_ROW_ARRAY_SIZE	equ 27	
SQL_ATTR_SIMULATE_CURSOR	equ SQL_SIMULATE_CURSOR
SQL_ATTR_USE_BOOKMARKS	equ SQL_USE_BOOKMARKS	
SQL_COL_PRED_CHAR	equ SQL_LIKE_ONLY
SQL_COL_PRED_BASIC	equ SQL_ALL_EXCEPT_LIKE
SQL_IS_POINTER	equ (-4)
SQL_IS_UINTEGER	equ (-5)
SQL_IS_INTEGER	equ (-6)
SQL_IS_USMALLINT	equ (-7)
SQL_IS_SMALLINT	equ (-8)
SQL_PARAM_BIND_BY_COLUMN	equ 0
SQL_PARAM_BIND_TYPE_DEFAULT	equ SQL_PARAM_BIND_BY_COLUMN
SQL_QUERY_TIMEOUT_DEFAULT equ 0
SQL_MAX_ROWS_DEFAULT equ 0
SQL_NOSCAN_OFF equ 0
SQL_NOSCAN_ON equ 1
SQL_NOSCAN_DEFAULT equ SQL_NOSCAN_OFF
SQL_MAX_LENGTH_DEFAULT equ 0
SQL_ASYNC_ENABLE_OFF	equ 0
SQL_ASYNC_ENABLE_ON	equ 1
SQL_ASYNC_ENABLE_DEFAULT equ SQL_ASYNC_ENABLE_OFF
SQL_BIND_BY_COLUMN equ 0
SQL_BIND_TYPE_DEFAULT equ SQL_BIND_BY_COLUMN
SQL_CONCUR_READ_ONLY equ 1
SQL_CONCUR_LOCK equ 2
SQL_CONCUR_ROWVER equ 3
SQL_CONCUR_VALUES equ 4
SQL_CONCUR_DEFAULT equ SQL_CONCUR_READ_ONLY
SQL_CURSOR_FORWARD_ONLY equ 0
SQL_CURSOR_KEYSET_DRIVEN equ 1
SQL_CURSOR_DYNAMIC equ 2
SQL_CURSOR_STATIC equ 3
SQL_CURSOR_TYPE_DEFAULT equ SQL_CURSOR_FORWARD_ONLY
SQL_ROWSET_SIZE_DEFAULT equ 1
SQL_KEYSET_SIZE_DEFAULT equ 0
SQL_SC_NON_UNIQUE equ 0
SQL_SC_TRY_UNIQUE equ 1
SQL_SC_UNIQUE equ 2
SQL_RD_OFF equ 0
SQL_RD_ON equ 1
SQL_RD_DEFAULT equ SQL_RD_ON
SQL_UB_OFF equ 0
SQL_UB_ON	equ 01
SQL_UB_DEFAULT equ SQL_UB_OFF
SQL_UB_FIXED	equ SQL_UB_ON
SQL_UB_VARIABLE	equ 2
SQL_DESC_ARRAY_SIZE	equ 20
SQL_DESC_ARRAY_STATUS_PTR	equ 21
SQL_DESC_AUTO_UNIQUE_VALUE	equ SQL_COLUMN_AUTO_INCREMENT
SQL_DESC_BASE_COLUMN_NAME	equ 22
SQL_DESC_BASE_TABLE_NAME	equ 23
SQL_DESC_BIND_OFFSET_PTR	equ 24
SQL_DESC_BIND_TYPE	equ 25
SQL_DESC_CASE_SENSITIVE	equ SQL_COLUMN_CASE_SENSITIVE
SQL_DESC_CATALOG_NAME	equ SQL_COLUMN_QUALIFIER_NAME
SQL_DESC_CONCISE_TYPE	equ SQL_COLUMN_TYPE
SQL_DESC_DATETIME_INTERVAL_PRECISION	equ 26
SQL_DESC_DISPLAY_SIZE	equ SQL_COLUMN_DISPLAY_SIZE
SQL_DESC_FIXED_PREC_SCALE	equ SQL_COLUMN_MONEY
SQL_DESC_LABEL	equ SQL_COLUMN_LABEL
SQL_DESC_LITERAL_PREFIX	equ 27
SQL_DESC_LITERAL_SUFFIX	equ 28
SQL_DESC_LOCAL_TYPE_NAME	equ 29
SQL_DESC_MAXIMUM_SCALE	equ 30
SQL_DESC_MINIMUM_SCALE	equ 31
SQL_DESC_NUM_PREC_RADIX	equ 32
SQL_DESC_PARAMETER_TYPE	equ 33
SQL_DESC_ROWS_PROCESSED_PTR	equ 34
SQL_DESC_ROWVER	equ 35
SQL_DESC_SCHEMA_NAME	equ SQL_COLUMN_OWNER_NAME
SQL_DESC_SEARCHABLE	equ SQL_COLUMN_SEARCHABLE
SQL_DESC_TYPE_NAME	equ SQL_COLUMN_TYPE_NAME
SQL_DESC_TABLE_NAME	equ SQL_COLUMN_TABLE_NAME
SQL_DESC_UNSIGNED	equ SQL_COLUMN_UNSIGNED
SQL_DESC_UPDATABLE	equ SQL_COLUMN_UPDATABLE
SQL_DIAG_CURSOR_ROW_COUNT	equ (-1249)
SQL_DIAG_ROW_NUMBER	equ (-1248)
SQL_DIAG_COLUMN_NUMBER	equ (-1247)	
SQL_DATE equ 9
SQL_INTERVAL	equ 10
SQL_TIME equ 10
SQL_TIMESTAMP equ 11
SQL_LONGVARCHAR equ (-1)
SQL_BINARY equ (-2)
SQL_VARBINARY equ (-3)
SQL_LONGVARBINARY equ (-4)
SQL_BIGINT equ (-5)
SQL_TINYINT equ (-6)
SQL_BIT equ (-7)
SQL_GUID	equ (-11)
SQL_CODE_YEAR	equ 1
SQL_CODE_MONTH	equ 2
SQL_CODE_DAY	equ 3
SQL_CODE_HOUR	equ 4
SQL_CODE_MINUTE	equ 5
SQL_CODE_SECOND	equ 6
SQL_CODE_YEAR_TO_MONTH	equ 7
SQL_CODE_DAY_TO_HOUR	equ 8
SQL_CODE_DAY_TO_MINUTE	equ 9
SQL_CODE_DAY_TO_SECOND	equ 10
SQL_CODE_HOUR_TO_MINUTE	equ 11
SQL_CODE_HOUR_TO_SECOND	equ 12
SQL_CODE_MINUTE_TO_SECOND	equ 13
SQL_INTERVAL_YEAR	equ (100 + SQL_CODE_YEAR)
SQL_INTERVAL_MONTH	equ (100 + SQL_CODE_MONTH)
SQL_INTERVAL_DAY	equ (100 + SQL_CODE_DAY) 
SQL_INTERVAL_HOUR	equ (100 + SQL_CODE_HOUR) 
SQL_INTERVAL_MINUTE	equ (100 + SQL_CODE_MINUTE) 
SQL_INTERVAL_SECOND equ (100 + SQL_CODE_SECOND) 
SQL_INTERVAL_YEAR_TO_MONTH	equ (100 + SQL_CODE_YEAR_TO_MONTH)
SQL_INTERVAL_DAY_TO_HOUR	equ (100 + SQL_CODE_DAY_TO_HOUR) 
SQL_INTERVAL_DAY_TO_MINUTE	equ (100 + SQL_CODE_DAY_TO_MINUTE) 
SQL_INTERVAL_DAY_TO_SECOND	equ (100 + SQL_CODE_DAY_TO_SECOND) 
SQL_INTERVAL_HOUR_TO_MINUTE	equ (100 + SQL_CODE_HOUR_TO_MINUTE)
SQL_INTERVAL_HOUR_TO_SECOND	equ (100 + SQL_CODE_HOUR_TO_SECOND) 
SQL_INTERVAL_MINUTE_TO_SECOND	equ (100 + SQL_CODE_MINUTE_TO_SECOND) 
SQL_UNICODE	equ SQL_WCHAR
SQL_UNICODE_VARCHAR	equ SQL_WVARCHAR
SQL_UNICODE_LONGVARCHAR	equ SQL_WLONGVARCHAR
SQL_UNICODE_CHAR	equ SQL_WCHAR
SQL_C_CHAR equ SQL_CHAR
SQL_C_LONG equ SQL_INTEGER
SQL_C_SHORT equ SQL_SMALLINT
SQL_C_FLOAT equ SQL_REAL
SQL_C_DOUBLE equ SQL_DOUBLE
SQL_C_NUMERIC	equ SQL_NUMERIC
SQL_C_DEFAULT equ 99
SQL_SIGNED_OFFSET equ (-20)
SQL_UNSIGNED_OFFSET equ (-22)
SQL_C_DATE equ SQL_DATE
SQL_C_TIME equ SQL_TIME
SQL_C_TIMESTAMP equ SQL_TIMESTAMP
SQL_C_TYPE_DATE	equ SQL_TYPE_DATE
SQL_C_TYPE_TIME	equ SQL_TYPE_TIME
SQL_C_TYPE_TIMESTAMP	equ SQL_TYPE_TIMESTAMP
SQL_C_INTERVAL_YEAR	equ SQL_INTERVAL_YEAR
SQL_C_INTERVAL_MONTH	equ SQL_INTERVAL_MONTH
SQL_C_INTERVAL_DAY	equ SQL_INTERVAL_DAY
SQL_C_INTERVAL_HOUR	equ SQL_INTERVAL_HOUR
SQL_C_INTERVAL_MINUTE	equ SQL_INTERVAL_MINUTE
SQL_C_INTERVAL_SECOND	equ SQL_INTERVAL_SECOND
SQL_C_INTERVAL_YEAR_TO_MONTH	equ SQL_INTERVAL_YEAR_TO_MONTH
SQL_C_INTERVAL_DAY_TO_HOUR	equ SQL_INTERVAL_DAY_TO_HOUR
SQL_C_INTERVAL_DAY_TO_MINUTE	equ SQL_INTERVAL_DAY_TO_MINUTE
SQL_C_INTERVAL_DAY_TO_SECOND	equ SQL_INTERVAL_DAY_TO_SECOND
SQL_C_INTERVAL_HOUR_TO_MINUTE	equ SQL_INTERVAL_HOUR_TO_MINUTE
SQL_C_INTERVAL_HOUR_TO_SECOND	equ SQL_INTERVAL_HOUR_TO_SECOND
SQL_C_INTERVAL_MINUTE_TO_SECOND	equ SQL_INTERVAL_MINUTE_TO_SECOND
SQL_C_BINARY equ SQL_BINARY
SQL_C_BIT equ SQL_BIT
SQL_C_SBIGINT	equ (SQL_BIGINT+SQL_SIGNED_OFFSET)
SQL_C_UBIGINT	equ (SQL_BIGINT+SQL_UNSIGNED_OFFSET)
SQL_C_TINYINT equ SQL_TINYINT
SQL_C_SLONG equ (SQL_C_LONG+SQL_SIGNED_OFFSET)
SQL_C_SSHORT equ (SQL_C_SHORT+SQL_SIGNED_OFFSET)
SQL_C_STINYINT equ (SQL_TINYINT+SQL_SIGNED_OFFSET)
SQL_C_ULONG equ (SQL_C_LONG+SQL_UNSIGNED_OFFSET)
SQL_C_USHORT equ (SQL_C_SHORT+SQL_UNSIGNED_OFFSET)
SQL_C_UTINYINT equ (SQL_TINYINT+SQL_UNSIGNED_OFFSET)
SQL_C_BOOKMARK equ SQL_C_ULONG
SQL_C_GUID	equ SQL_GUID
SQL_TYPE_NULL equ 0
SQL_C_VARBOOKMARK	equ SQL_C_BINARY
SQL_NO_ROW_NUMBER	equ (-1)
SQL_NO_COLUMN_NUMBER	equ (-1)
SQL_ROW_NUMBER_UNKNOWN	equ (-2)
SQL_COLUMN_NUMBER_UNKNOWN	equ (-2)
SQL_DEFAULT_PARAM equ (-5)
SQL_IGNORE equ (-6)
SQL_COLUMN_IGNORE	equ SQL_IGNORE
SQL_LEN_DATA_AT_EXEC_OFFSET equ (-100)
SQL_LEN_BINARY_ATTR_OFFSET	equ (-100)
SQL_PARAM_TYPE_DEFAULT equ SQL_PARAM_INPUT_OUTPUT
SQL_SETPARAM_VALUE_MAX equ (-1L)
SQL_COLUMN_COUNT equ 0
SQL_COLUMN_NAME equ 1
SQL_COLUMN_TYPE equ 2
SQL_COLUMN_LENGTH equ 3
SQL_COLUMN_PRECISION equ 4
SQL_COLUMN_SCALE equ 5
SQL_COLUMN_DISPLAY_SIZE equ 6
SQL_COLUMN_NULLABLE equ 7
SQL_COLUMN_UNSIGNED equ 8
SQL_COLUMN_MONEY equ 9
SQL_COLUMN_UPDATABLE equ 10
SQL_COLUMN_AUTO_INCREMENT equ 11
SQL_COLUMN_CASE_SENSITIVE equ 12
SQL_COLUMN_SEARCHABLE equ 13
SQL_COLUMN_TYPE_NAME equ 14
SQL_COLUMN_TABLE_NAME equ 15
SQL_COLUMN_OWNER_NAME equ 16
SQL_COLUMN_QUALIFIER_NAME equ 17
SQL_COLUMN_LABEL equ 18
SQL_COLATT_OPT_MAX equ SQL_COLUMN_LABEL
SQL_COLATT_OPT_MIN equ SQL_COLUMN_COUNT
SQL_ATTR_READONLY equ 0
SQL_ATTR_WRITE equ 1
SQL_ATTR_READWRITE_UNKNOWN equ 2
SQL_UNSEARCHABLE equ 0
SQL_LIKE_ONLY equ 1
SQL_ALL_EXCEPT_LIKE equ 2
SQL_SEARCHABLE equ 3
SQL_PRED_SEARCHABLE	equ SQL_SEARCHABLE
SQL_NO_TOTAL equ (-4)
SQL_API_SQLALLOCHANDLESTD	equ 73
SQL_API_SQLBULKOPERATIONS	equ 24
SQL_API_SQLBINDPARAMETER equ 72
SQL_API_SQLBROWSECONNECT equ 55    
SQL_API_SQLCOLATTRIBUTES equ 6 
SQL_API_SQLCOLUMNPRIVILEGES equ 56
SQL_API_SQLDESCRIBEPARAM equ 58
SQL_API_SQLDRIVERCONNECT	equ 41 
SQL_API_SQLDRIVERS equ 71
SQL_API_SQLEXTENDEDFETCH equ 59
SQL_API_SQLFOREIGNKEYS equ 60
SQL_API_SQLMORERESULTS equ 61
SQL_API_SQLNATIVESQL equ 62
SQL_API_SQLNUMPARAMS equ 63
SQL_API_SQLPARAMOPTIONS equ 64
SQL_API_SQLPRIMARYKEYS equ 65
SQL_API_SQLPROCEDURECOLUMNS equ 66
SQL_API_SQLPROCEDURES equ 67
SQL_API_SQLSETPOS equ 68
SQL_API_SQLSETSCROLLOPTIONS equ 69
SQL_API_SQLTABLEPRIVILEGES equ 70
SQL_API_ALL_FUNCTIONS equ 0
SQL_API_LOADBYORDINAL equ 199
SQL_API_ODBC3_ALL_FUNCTIONS	equ 999
SQL_API_ODBC3_ALL_FUNCTIONS_SIZE	equ 250
SQL_INFO_FIRST equ 0		
SQL_ACTIVE_CONNECTIONS equ 0
SQL_ACTIVE_STATEMENTS equ 1
SQL_DRIVER_HDBC equ 3
SQL_DRIVER_HENV equ 4
SQL_DRIVER_HSTMT equ 5
SQL_DRIVER_NAME equ 6
SQL_DRIVER_VER equ 7
SQL_ODBC_API_CONFORMANCE equ 9
SQL_ODBC_VER equ 10
SQL_ROW_UPDATES equ 11
SQL_ODBC_SAG_CLI_CONFORMANCE equ 12
SQL_ODBC_SQL_CONFORMANCE equ 15
SQL_PROCEDURES equ 21
SQL_CONCAT_NULL_BEHAVIOR equ 22
SQL_CURSOR_ROLLBACK_BEHAVIOR equ 24
SQL_EXPRESSIONS_IN_ORDERBY equ 27
SQL_MAX_OWNER_NAME_LEN equ 32
SQL_MAX_PROCEDURE_NAME_LEN equ 33
SQL_MAX_QUALIFIER_NAME_LEN equ 34
SQL_MULT_RESULT_SETS equ 36
SQL_MULTIPLE_ACTIVE_TXN equ 37
SQL_OUTER_JOINS equ 38
SQL_OWNER_TERM equ 39
SQL_PROCEDURE_TERM equ 40
SQL_QUALIFIER_NAME_SEPARATOR equ 41
SQL_QUALIFIER_TERM equ 42
SQL_SCROLL_OPTIONS equ 44
SQL_TABLE_TERM equ 45
SQL_CONVERT_FUNCTIONS equ 48
SQL_NUMERIC_FUNCTIONS equ 49
SQL_STRING_FUNCTIONS equ 50
SQL_SYSTEM_FUNCTIONS equ 51
SQL_TIMEDATE_FUNCTIONS equ 52
SQL_CONVERT_BIGINT equ 53
SQL_CONVERT_BINARY equ 54
SQL_CONVERT_BIT equ 55
SQL_CONVERT_CHAR equ 56
SQL_CONVERT_DATE equ 57
SQL_CONVERT_DECIMAL equ 58
SQL_CONVERT_DOUBLE equ 59
SQL_CONVERT_FLOAT equ 60
SQL_CONVERT_INTEGER equ 61
SQL_CONVERT_LONGVARCHAR equ 62
SQL_CONVERT_NUMERIC equ 63
SQL_CONVERT_REAL equ 64
SQL_CONVERT_SMALLINT equ 65
SQL_CONVERT_TIME equ 66
SQL_CONVERT_TIMESTAMP equ 67
SQL_CONVERT_TINYINT equ 68
SQL_CONVERT_VARBINARY equ 69
SQL_CONVERT_VARCHAR equ 70
SQL_CONVERT_LONGVARBINARY equ 71
SQL_ODBC_SQL_OPT_IEF equ 73
SQL_CORRELATION_NAME equ 74
SQL_NON_NULLABLE_COLUMNS equ 75
SQL_DRIVER_HLIB equ 76
SQL_DRIVER_ODBC_VER equ 77
SQL_LOCK_TYPES equ 78
SQL_POS_OPERATIONS equ 79
SQL_POSITIONED_STATEMENTS equ 80
SQL_BOOKMARK_PERSISTENCE equ 82
SQL_STATIC_SENSITIVITY equ 83
SQL_FILE_USAGE equ 84
SQL_COLUMN_ALIAS equ 87
SQL_GROUP_BY equ 88
SQL_KEYWORDS equ 89
SQL_OWNER_USAGE equ 91
SQL_QUALIFIER_USAGE equ 92
SQL_QUOTED_IDENTIFIER_CASE equ 93
SQL_SUBQUERIES equ 95
SQL_UNION equ 96
SQL_MAX_ROW_SIZE_INCLUDES_LONG equ 103
SQL_MAX_CHAR_LITERAL_LEN equ 108
SQL_TIMEDATE_ADD_INTERVALS equ 109
SQL_TIMEDATE_DIFF_INTERVALS equ 110
SQL_NEED_LONG_DATA_LEN equ 111
SQL_MAX_BINARY_LITERAL_LEN equ 112
SQL_LIKE_ESCAPE_CLAUSE equ 113
SQL_QUALIFIER_LOCATION equ 114
SQL_ACTIVE_ENVIRONMENTS	equ 116
SQL_ALTER_DOMAIN	equ 117
SQL_SQL_CONFORMANCE	equ 118
SQL_DATETIME_LITERALS	equ 119
SQL_ASYNC_MODE	equ 10021
SQL_BATCH_ROW_COUNT	equ 120
SQL_BATCH_SUPPORT	equ 121
SQL_CATALOG_LOCATION	equ SQL_QUALIFIER_LOCATION
SQL_CATALOG_NAME_SEPARATOR	equ SQL_QUALIFIER_NAME_SEPARATOR
SQL_CATALOG_TERM	equ SQL_QUALIFIER_TERM
SQL_CATALOG_USAGE	equ SQL_QUALIFIER_USAGE
SQL_CONVERT_WCHAR	equ 122
SQL_CONVERT_INTERVAL_DAY_TIME	equ 123
SQL_CONVERT_INTERVAL_YEAR_MONTH	equ 124
SQL_CONVERT_WLONGVARCHAR	equ 125
SQL_CONVERT_WVARCHAR	equ 126
SQL_CREATE_ASSERTION	equ 127
SQL_CREATE_CHARACTER_SET	equ 128
SQL_CREATE_COLLATION	equ 129
SQL_CREATE_DOMAIN	equ 130
SQL_CREATE_SCHEMA	equ 131
SQL_CREATE_TABLE	equ 132
SQL_CREATE_TRANSLATION	equ 133
SQL_CREATE_VIEW	equ 134
SQL_DRIVER_HDESC	equ 135
SQL_DROP_ASSERTION	equ 136
SQL_DROP_CHARACTER_SET	equ 137
SQL_DROP_COLLATION	equ 138
SQL_DROP_DOMAIN	equ 139
SQL_DROP_SCHEMA	equ 140
SQL_DROP_TABLE	equ 141
SQL_DROP_TRANSLATION	equ 142
SQL_DROP_VIEW	equ 143
SQL_DYNAMIC_CURSOR_ATTRIBUTES1	equ 144
SQL_DYNAMIC_CURSOR_ATTRIBUTES2	equ 145
SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES1	equ 146		
SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES2	equ 147
SQL_INDEX_KEYWORDS	equ 148
SQL_INFO_SCHEMA_VIEWS	equ 149
SQL_KEYSET_CURSOR_ATTRIBUTES1	equ 150
SQL_KEYSET_CURSOR_ATTRIBUTES2	equ 151
SQL_MAX_ASYNC_CONCURRENT_STATEMENTS	equ 10022
SQL_ODBC_INTERFACE_CONFORMANCE	equ 152
SQL_PARAM_ARRAY_ROW_COUNTS equ 153
SQL_PARAM_ARRAY_SELECTS equ 154
SQL_SCHEMA_TERM	equ SQL_OWNER_TERM
SQL_SCHEMA_USAGE	equ SQL_OWNER_USAGE
SQL_SQL92_DATETIME_FUNCTIONS	equ 155
SQL_SQL92_FOREIGN_KEY_DELETE_RULE	equ 156		
SQL_SQL92_FOREIGN_KEY_UPDATE_RULE	equ 157		
SQL_SQL92_GRANT	equ 158
SQL_SQL92_NUMERIC_VALUE_FUNCTIONS	equ 159
SQL_SQL92_PREDICATES	equ 160
SQL_SQL92_RELATIONAL_JOIN_OPERATORS	equ 161
SQL_SQL92_REVOKE	equ 162
SQL_SQL92_ROW_VALUE_CONSTRUCTOR	equ 163
SQL_SQL92_STRING_FUNCTIONS	equ 164
SQL_SQL92_VALUE_EXPRESSIONS	equ 165
SQL_STANDARD_CLI_CONFORMANCE	equ 166
SQL_STATIC_CURSOR_ATTRIBUTES1	equ 167	
SQL_STATIC_CURSOR_ATTRIBUTES2	equ 168
SQL_AGGREGATE_FUNCTIONS	equ 169
SQL_DDL_INDEX	equ 170
SQL_DM_VER	equ 171
SQL_INSERT_STATEMENT	equ 172
SQL_UNION_STATEMENT	equ SQL_UNION
SQL_DTC_TRANSITION_COST	equ 1750
SQL_AT_ADD_COLUMN equ 1
SQL_AT_DROP_COLUMN equ 2
SQL_AT_ADD_CONSTRAINT equ 8
SQL_AT_ADD_COLUMN_SINGLE	equ 20h	
SQL_AT_ADD_COLUMN_DEFAULT	equ 40h
SQL_AT_ADD_COLUMN_COLLATION	equ 80h
SQL_AT_SET_COLUMN_DEFAULT	equ 100h
SQL_AT_DROP_COLUMN_DEFAULT	equ 200h
SQL_AT_DROP_COLUMN_CASCADE	equ 400h
SQL_AT_DROP_COLUMN_RESTRICT	equ 800h
SQL_AT_ADD_TABLE_CONSTRAINT	equ 1000h		
SQL_AT_DROP_TABLE_CONSTRAINT_CASCADE	equ 2000h		
SQL_AT_DROP_TABLE_CONSTRAINT_RESTRICT	equ 4000h		
SQL_AT_CONSTRAINT_NAME_DEFINITION	equ 8000h
SQL_AT_CONSTRAINT_INITIALLY_DEFERRED	equ 10000h
SQL_AT_CONSTRAINT_INITIALLY_IMMEDIATE	equ 20000h
SQL_AT_CONSTRAINT_DEFERRABLE	equ 40000h
SQL_AT_CONSTRAINT_NON_DEFERRABLE	equ 80000h
SQL_CVT_CHAR equ 1
SQL_CVT_NUMERIC equ 2
SQL_CVT_DECIMAL equ 4
SQL_CVT_INTEGER equ 8
SQL_CVT_SMALLINT equ 10h
SQL_CVT_FLOAT equ 20h
SQL_CVT_REAL equ 40h
SQL_CVT_DOUBLE equ 80h
SQL_CVT_VARCHAR equ 100h
SQL_CVT_LONGVARCHAR equ 200h
SQL_CVT_BINARY equ 400h
SQL_CVT_VARBINARY equ 800h
SQL_CVT_BIT equ 1000h
SQL_CVT_TINYINT equ 2000h
SQL_CVT_BIGINT equ 4000h
SQL_CVT_DATE equ 8000h
SQL_CVT_TIME equ 10000h
SQL_CVT_TIMESTAMP equ 20000h
SQL_CVT_LONGVARBINARY equ 40000h
SQL_CVT_INTERVAL_YEAR_MONTH	equ 80000h
SQL_CVT_INTERVAL_DAY_TIME	equ 100000h
SQL_CVT_WCHAR	equ 200000h
SQL_CVT_WLONGVARCHAR	equ 400000h
SQL_CVT_WVARCHAR	equ 800000h
SQL_FN_CVT_CONVERT equ 1
SQL_FN_CVT_CAST	equ 2
SQL_FN_STR_CONCAT equ 1
SQL_FN_STR_INSERT equ 2
SQL_FN_STR_LEFT equ 4
SQL_FN_STR_LTRIM equ 8
SQL_FN_STR_LENGTH equ 10h
SQL_FN_STR_LOCATE equ 20h
SQL_FN_STR_LCASE equ 40h
SQL_FN_STR_REPEAT equ 80h
SQL_FN_STR_REPLACE equ 100h
SQL_FN_STR_RIGHT equ 200h
SQL_FN_STR_RTRIM equ 400h
SQL_FN_STR_SUBSTRING equ 800h
SQL_FN_STR_UCASE equ 1000h
SQL_FN_STR_ASCII equ 2000h
SQL_FN_STR_CHAR equ 4000h
SQL_FN_STR_DIFFERENCE equ 8000h
SQL_FN_STR_LOCATE_2 equ 10000h
SQL_FN_STR_SOUNDEX equ 20000h
SQL_FN_STR_SPACE equ 40000h
SQL_FN_STR_BIT_LENGTH	equ 80000h
SQL_FN_STR_CHAR_LENGTH	equ 100000h
SQL_FN_STR_CHARACTER_LENGTH	equ 200000h
SQL_FN_STR_OCTET_LENGTH	equ 400000h
SQL_FN_STR_POSITION	equ 800000h
SQL_SSF_CONVERT	equ 1	
SQL_SSF_LOWER	equ 2
SQL_SSF_UPPER	equ 4
SQL_SSF_SUBSTRING	equ 8
SQL_SSF_TRANSLATE	equ 10h
SQL_SSF_TRIM_BOTH	equ 20h
SQL_SSF_TRIM_LEADING	equ 40h
SQL_SSF_TRIM_TRAILING	equ 80h
SQL_FN_NUM_ABS equ 1
SQL_FN_NUM_ACOS equ 2
SQL_FN_NUM_ASIN equ 4
SQL_FN_NUM_ATAN equ 8
SQL_FN_NUM_ATAN2 equ 10h
SQL_FN_NUM_CEILING equ 20h
SQL_FN_NUM_COS equ 40h
SQL_FN_NUM_COT equ 80h
SQL_FN_NUM_EXP equ 100h
SQL_FN_NUM_FLOOR equ 200h
SQL_FN_NUM_LOG equ 400h
SQL_FN_NUM_MOD equ 800h
SQL_FN_NUM_SIGN equ 1000h
SQL_FN_NUM_SIN equ 2000h
SQL_FN_NUM_SQRT equ 4000h
SQL_FN_NUM_TAN equ 8000h
SQL_FN_NUM_PI equ 10000h
SQL_FN_NUM_RAND equ 20000h
SQL_FN_NUM_DEGREES equ 40000h
SQL_FN_NUM_LOG10 equ 80000h
SQL_FN_NUM_POWER equ 100000h
SQL_FN_NUM_RADIANS equ 200000h
SQL_FN_NUM_ROUND equ 400000h
SQL_FN_NUM_TRUNCATE equ 800000h
SQL_SNVF_BIT_LENGTH	equ 1
SQL_SNVF_CHAR_LENGTH	equ 2
SQL_SNVF_CHARACTER_LENGTH	equ 4
SQL_SNVF_EXTRACT	equ 8
SQL_SNVF_OCTET_LENGTH	equ 10h
SQL_SNVF_POSITION	equ 20h
SQL_FN_TD_NOW equ 1
SQL_FN_TD_CURDATE equ 2
SQL_FN_TD_DAYOFMONTH equ 4
SQL_FN_TD_DAYOFWEEK equ 8
SQL_FN_TD_DAYOFYEAR equ 10h
SQL_FN_TD_MONTH equ 20h
SQL_FN_TD_QUARTER equ 40h
SQL_FN_TD_WEEK equ 80h
SQL_FN_TD_YEAR equ 100h
SQL_FN_TD_CURTIME equ 200h
SQL_FN_TD_HOUR equ 400h
SQL_FN_TD_MINUTE equ 800h
SQL_FN_TD_SECOND equ 1000h
SQL_FN_TD_TIMESTAMPADD equ 2000h
SQL_FN_TD_TIMESTAMPDIFF equ 4000h
SQL_FN_TD_DAYNAME equ 8000h
SQL_FN_TD_MONTHNAME equ 10000h
SQL_FN_TD_CURRENT_DATE	equ 20000h
SQL_FN_TD_CURRENT_TIME	equ 40000h
SQL_FN_TD_CURRENT_TIMESTAMP	equ 80000h
SQL_FN_TD_EXTRACT	equ 100000h
SQL_SDF_CURRENT_DATE	equ 1
SQL_SDF_CURRENT_TIME	equ 2
SQL_SDF_CURRENT_TIMESTAMP	equ 4
SQL_FN_SYS_USERNAME equ 1
SQL_FN_SYS_DBNAME equ 2
SQL_FN_SYS_IFNULL equ 4
SQL_FN_TSI_FRAC_SECOND equ 1
SQL_FN_TSI_SECOND equ 2
SQL_FN_TSI_MINUTE equ 4
SQL_FN_TSI_HOUR equ 8
SQL_FN_TSI_DAY equ 10h
SQL_FN_TSI_WEEK equ 20h
SQL_FN_TSI_MONTH equ 40h
SQL_FN_TSI_QUARTER equ 80h
SQL_FN_TSI_YEAR equ 100h
SQL_CA1_NEXT	equ 1
SQL_CA1_ABSOLUTE	equ 2
SQL_CA1_RELATIVE	equ 4
SQL_CA1_BOOKMARK	equ 8
SQL_CA1_LOCK_NO_CHANGE	equ 40h
SQL_CA1_LOCK_EXCLUSIVE	equ 80h
SQL_CA1_LOCK_UNLOCK	equ 100h
SQL_CA1_POS_POSITION	equ 200h
SQL_CA1_POS_UPDATE	equ 400h
SQL_CA1_POS_DELETE	equ 800h
SQL_CA1_POS_REFRESH	equ 1000h
SQL_CA1_POSITIONED_UPDATE	equ 2000h
SQL_CA1_POSITIONED_DELETE	equ 4000h
SQL_CA1_SELECT_FOR_UPDATE	equ 8000h
SQL_CA1_BULK_ADD	equ 10000h
SQL_CA1_BULK_UPDATE_BY_BOOKMARK	equ 20000h
SQL_CA1_BULK_DELETE_BY_BOOKMARK	equ 40000h
SQL_CA1_BULK_FETCH_BY_BOOKMARK	equ 80000h
SQL_CA2_READ_ONLY_CONCURRENCY	equ 1
SQL_CA2_LOCK_CONCURRENCY	equ 2
SQL_CA2_OPT_ROWVER_CONCURRENCY	equ 4
SQL_CA2_OPT_VALUES_CONCURRENCY	equ 8
SQL_CA2_SENSITIVITY_ADDITIONS	equ 10h
SQL_CA2_SENSITIVITY_DELETIONS	equ 20h
SQL_CA2_SENSITIVITY_UPDATES	equ 40h
SQL_CA2_MAX_ROWS_SELECT	equ 80h
SQL_CA2_MAX_ROWS_INSERT	equ 100h
SQL_CA2_MAX_ROWS_DELETE	equ 200h
SQL_CA2_MAX_ROWS_UPDATE	equ 400h
SQL_CA2_MAX_ROWS_CATALOG	equ 800h
SQL_CA2_MAX_ROWS_AFFECTS_ALL	equ (SQL_CA2_MAX_ROWS_SELECT + \
					SQL_CA2_MAX_ROWS_INSERT + SQL_CA2_MAX_ROWS_DELETE + \
					SQL_CA2_MAX_ROWS_UPDATE + SQL_CA2_MAX_ROWS_CATALOG)
SQL_CA2_CRC_EXACT	equ 1000h
SQL_CA2_CRC_APPROXIMATE	equ 2000h
SQL_CA2_SIMULATE_NON_UNIQUE	equ 4000h
SQL_CA2_SIMULATE_TRY_UNIQUE	equ 8000h
SQL_CA2_SIMULATE_UNIQUE	equ 10000h
SQL_OAC_NONE equ 0
SQL_OAC_LEVEL1 equ 1
SQL_OAC_LEVEL2 equ 2
SQL_OSCC_NOT_COMPLIANT equ 0
SQL_OSCC_COMPLIANT equ 1
SQL_OSC_MINIMUM equ 0
SQL_OSC_CORE equ 1
SQL_OSC_EXTENDED equ 2
SQL_CB_NULL equ 0
SQL_CB_NON_NULL equ 1
SQL_SO_FORWARD_ONLY equ 1
SQL_SO_KEYSET_DRIVEN equ 2
SQL_SO_DYNAMIC equ 4
SQL_SO_MIXED equ 8
SQL_SO_STATIC equ 10h
SQL_FD_FETCH_BOOKMARK equ 80h
SQL_CN_NONE equ 0
SQL_CN_DIFFERENT equ 1
SQL_CN_ANY equ 2
SQL_NNC_NULL equ 0
SQL_NNC_NON_NULL equ 1
SQL_NC_START equ 2
SQL_NC_END equ 4
SQL_FILE_NOT_SUPPORTED equ 0
SQL_FILE_TABLE equ 1
SQL_FILE_QUALIFIER equ 2
SQL_FILE_CATALOG	equ SQL_FILE_QUALIFIER
SQL_GD_BLOCK equ 4
SQL_GD_BOUND equ 8
SQL_PS_POSITIONED_DELETE equ 1
SQL_PS_POSITIONED_UPDATE equ 2
SQL_PS_SELECT_FOR_UPDATE equ 4
SQL_GB_NOT_SUPPORTED equ 0
SQL_GB_GROUP_BY_EQUALS_SELECT equ 1
SQL_GB_GROUP_BY_CONTAINS_SELECT equ 2
SQL_GB_NO_RELATION equ 3
SQL_GB_COLLATE	equ 4
SQL_OU_DML_STATEMENTS equ 1
SQL_OU_PROCEDURE_INVOCATION equ 2
SQL_OU_TABLE_DEFINITION equ 4
SQL_OU_INDEX_DEFINITION equ 8
SQL_OU_PRIVILEGE_DEFINITION equ 10h
SQL_SU_DML_STATEMENTS	equ SQL_OU_DML_STATEMENTS 
SQL_SU_PROCEDURE_INVOCATION	equ SQL_OU_PROCEDURE_INVOCATION
SQL_SU_TABLE_DEFINITION	equ SQL_OU_TABLE_DEFINITION
SQL_SU_INDEX_DEFINITION	equ SQL_OU_INDEX_DEFINITION
SQL_SU_PRIVILEGE_DEFINITION	equ SQL_OU_PRIVILEGE_DEFINITION
SQL_QU_DML_STATEMENTS equ 1
SQL_QU_PROCEDURE_INVOCATION equ 2
SQL_QU_TABLE_DEFINITION equ 4
SQL_QU_INDEX_DEFINITION equ 8
SQL_QU_PRIVILEGE_DEFINITION equ 10h
SQL_CU_DML_STATEMENTS	equ SQL_QU_DML_STATEMENTS
SQL_CU_PROCEDURE_INVOCATION	equ SQL_QU_PROCEDURE_INVOCATION 
SQL_CU_TABLE_DEFINITION	equ SQL_QU_TABLE_DEFINITION
SQL_CU_INDEX_DEFINITION	equ SQL_QU_INDEX_DEFINITION 
SQL_CU_PRIVILEGE_DEFINITION	equ SQL_QU_PRIVILEGE_DEFINITION 
SQL_SQ_COMPARISON equ 1
SQL_SQ_EXISTS equ 2
SQL_SQ_IN equ 4
SQL_SQ_QUANTIFIED equ 8
SQL_SQ_CORRELATED_SUBQUERIES equ 10h
SQL_U_UNION equ 1
SQL_U_UNION_ALL equ 2
SQL_BP_CLOSE equ 1
SQL_BP_DELETE equ 2
SQL_BP_DROP equ 4
SQL_BP_TRANSACTION equ 8
SQL_BP_UPDATE equ 10h
SQL_BP_OTHER_HSTMT equ 20h
SQL_BP_SCROLL equ 40h
SQL_SS_ADDITIONS equ 1
SQL_SS_DELETIONS equ 2
SQL_SS_UPDATES equ 4
SQL_CV_CREATE_VIEW	equ 1
SQL_CV_CHECK_OPTION	equ 2
SQL_CV_CASCADED	equ 4
SQL_CV_LOCAL	equ 8
SQL_LCK_NO_CHANGE equ 1
SQL_LCK_EXCLUSIVE equ 2
SQL_LCK_UNLOCK equ 4
SQL_POS_POSITION equ 1
SQL_POS_REFRESH equ 2
SQL_POS_UPDATE equ 4
SQL_POS_DELETE equ 8
SQL_POS_ADD equ 10h
SQL_QL_START equ 1
SQL_QL_END equ 2
SQL_AF_AVG	equ 1
SQL_AF_COUNT	equ 2
SQL_AF_MAX	equ 4
SQL_AF_MIN	equ 8
SQL_AF_SUM	equ 10h
SQL_AF_DISTINCT	equ 20h
SQL_AF_ALL	equ 40h	
SQL_SC_SQL92_ENTRY	equ 1
SQL_SC_FIPS127_2_TRANSITIONAL	equ 2
SQL_SC_SQL92_INTERMEDIATE	equ 4
SQL_SC_SQL92_FULL	equ 8
SQL_DL_SQL92_DATE	equ 1
SQL_DL_SQL92_TIME	equ 2
SQL_DL_SQL92_TIMESTAMP	equ 4
SQL_DL_SQL92_INTERVAL_YEAR	equ 8
SQL_DL_SQL92_INTERVAL_MONTH	equ 10h
SQL_DL_SQL92_INTERVAL_DAY	equ 20h
SQL_DL_SQL92_INTERVAL_HOUR	equ 40h
SQL_DL_SQL92_INTERVAL_MINUTE	equ 80h
SQL_DL_SQL92_INTERVAL_SECOND	equ 100h
SQL_DL_SQL92_INTERVAL_YEAR_TO_MONTH	equ 200h
SQL_DL_SQL92_INTERVAL_DAY_TO_HOUR	equ 400h
SQL_DL_SQL92_INTERVAL_DAY_TO_MINUTE	equ 800h
SQL_DL_SQL92_INTERVAL_DAY_TO_SECOND	equ 1000h
SQL_DL_SQL92_INTERVAL_HOUR_TO_MINUTE	equ 2000h
SQL_DL_SQL92_INTERVAL_HOUR_TO_SECOND	equ 4000h
SQL_DL_SQL92_INTERVAL_MINUTE_TO_SECOND	equ 8000h
SQL_CL_START	equ SQL_QL_START
SQL_CL_END	equ SQL_QL_END
SQL_BRC_PROCEDURES	equ 1
SQL_BRC_EXPLICIT	equ 2
SQL_BRC_ROLLED_UP	equ 4
SQL_BS_SELECT_EXPLICIT	equ 1
SQL_BS_ROW_COUNT_EXPLICIT	equ 2
SQL_BS_SELECT_PROC	equ 4
SQL_BS_ROW_COUNT_PROC	equ 8
SQL_PARC_BATCH	equ 1
SQL_PARC_NO_BATCH	equ 2
SQL_PAS_BATCH	equ 1
SQL_PAS_NO_BATCH	equ 2		
SQL_PAS_NO_SELECT	equ 3
SQL_IK_NONE	equ 0
SQL_IK_ASC	equ 1
SQL_IK_DESC	equ 2
SQL_IK_ALL	equ (SQL_IK_ASC + SQL_IK_DESC)
SQL_ISV_ASSERTIONS	equ 1
SQL_ISV_CHARACTER_SETS	equ 2
SQL_ISV_CHECK_CONSTRAINTS	equ 4
SQL_ISV_COLLATIONS	equ 8
SQL_ISV_COLUMN_DOMAIN_USAGE	equ 10h
SQL_ISV_COLUMN_PRIVILEGES	equ 20h
SQL_ISV_COLUMNS	equ 40h
SQL_ISV_CONSTRAINT_COLUMN_USAGE	equ 80h
SQL_ISV_CONSTRAINT_TABLE_USAGE	equ 100h
SQL_ISV_DOMAIN_CONSTRAINTS	equ 200h
SQL_ISV_DOMAINS	equ 400h
SQL_ISV_KEY_COLUMN_USAGE	equ 800h
SQL_ISV_REFERENTIAL_CONSTRAINTS	equ 1000h
SQL_ISV_SCHEMATA	equ 2000h
SQL_ISV_SQL_LANGUAGES	equ 4000h
SQL_ISV_TABLE_CONSTRAINTS	equ 8000h
SQL_ISV_TABLE_PRIVILEGES	equ 10000h
SQL_ISV_TABLES	equ 20000h
SQL_ISV_TRANSLATIONS	equ 40000h
SQL_ISV_USAGE_PRIVILEGES	equ 80000h
SQL_ISV_VIEW_COLUMN_USAGE	equ 100000h
SQL_ISV_VIEW_TABLE_USAGE	equ 200000h
SQL_ISV_VIEWS	equ 400000h
SQL_AM_NONE	equ 0
SQL_AM_CONNECTION	equ 1
SQL_AM_STATEMENT	equ 2
SQL_AD_CONSTRAINT_NAME_DEFINITION	equ 1	
SQL_AD_ADD_DOMAIN_CONSTRAINT	equ 2
SQL_AD_DROP_DOMAIN_CONSTRAINT	equ 4
SQL_AD_ADD_DOMAIN_DEFAULT equ 8
SQL_AD_DROP_DOMAIN_DEFAULT equ 10h
SQL_AD_ADD_CONSTRAINT_INITIALLY_DEFERRED	equ 20h
SQL_AD_ADD_CONSTRAINT_INITIALLY_IMMEDIATE	equ 40h
SQL_AD_ADD_CONSTRAINT_DEFERRABLE	equ 80h
SQL_AD_ADD_CONSTRAINT_NON_DEFERRABLE	equ 100h
SQL_CS_CREATE_SCHEMA	equ 1
SQL_CS_AUTHORIZATION	equ 2
SQL_CS_DEFAULT_CHARACTER_SET	equ 4
SQL_CTR_CREATE_TRANSLATION	equ 1
SQL_CA_CREATE_ASSERTION	equ 1
SQL_CA_CONSTRAINT_INITIALLY_DEFERRED	equ 10h
SQL_CA_CONSTRAINT_INITIALLY_IMMEDIATE	equ 20h
SQL_CA_CONSTRAINT_DEFERRABLE	equ 40h
SQL_CA_CONSTRAINT_NON_DEFERRABLE	equ 80h
SQL_CCS_CREATE_CHARACTER_SET	equ 1
SQL_CCS_COLLATE_CLAUSE	equ 2
SQL_CCS_LIMITED_COLLATION	equ 4
SQL_CCOL_CREATE_COLLATION	equ 1
SQL_CDO_CREATE_DOMAIN	equ 1
SQL_CDO_DEFAULT	equ 2
SQL_CDO_CONSTRAINT	equ 4
SQL_CDO_COLLATION	equ 8
SQL_CDO_CONSTRAINT_NAME_DEFINITION	equ 10h
SQL_CDO_CONSTRAINT_INITIALLY_DEFERRED	equ 20h
SQL_CDO_CONSTRAINT_INITIALLY_IMMEDIATE	equ 40h
SQL_CDO_CONSTRAINT_DEFERRABLE	equ 80h
SQL_CDO_CONSTRAINT_NON_DEFERRABLE	equ 100h
SQL_CT_CREATE_TABLE	equ 1
SQL_CT_COMMIT_PRESERVE	equ 2
SQL_CT_COMMIT_DELETE	equ 4
SQL_CT_GLOBAL_TEMPORARY	equ 8
SQL_CT_LOCAL_TEMPORARY	equ 10h
SQL_CT_CONSTRAINT_INITIALLY_DEFERRED	equ 20h
SQL_CT_CONSTRAINT_INITIALLY_IMMEDIATE	equ 40h
SQL_CT_CONSTRAINT_DEFERRABLE	equ 80h
SQL_CT_CONSTRAINT_NON_DEFERRABLE	equ 100h
SQL_CT_COLUMN_CONSTRAINT	equ 200h
SQL_CT_COLUMN_DEFAULT	equ 400h
SQL_CT_COLUMN_COLLATION	equ 800h
SQL_CT_TABLE_CONSTRAINT	equ 1000h
SQL_CT_CONSTRAINT_NAME_DEFINITION	equ 2000h
SQL_DI_CREATE_INDEX	equ 1
SQL_DI_DROP_INDEX	equ 2
SQL_DC_DROP_COLLATION	equ 1
SQL_DD_DROP_DOMAIN	equ 1
SQL_DD_RESTRICT	equ 2
SQL_DD_CASCADE	equ 4
SQL_DS_DROP_SCHEMA	equ 1
SQL_DS_RESTRICT	equ 2
SQL_DS_CASCADE	equ 4
SQL_DCS_DROP_CHARACTER_SET	equ 1
SQL_DA_DROP_ASSERTION	equ 1
SQL_DT_DROP_TABLE	equ 1
SQL_DT_RESTRICT	equ 2
SQL_DT_CASCADE	equ 4
SQL_DTR_DROP_TRANSLATION	equ 1
SQL_DV_DROP_VIEW	equ 1
SQL_DV_RESTRICT	equ 2
SQL_DV_CASCADE	equ 4
SQL_IS_INSERT_LITERALS	equ 1
SQL_IS_INSERT_SEARCHED	equ 2
SQL_IS_SELECT_INTO	equ 4
SQL_OIC_CORE	equ 1
SQL_OIC_LEVEL1	equ 2
SQL_OIC_LEVEL2	equ 3
SQL_SFKD_CASCADE	equ 1
SQL_SFKD_NO_ACTION	equ 2
SQL_SFKD_SET_DEFAULT	equ 4
SQL_SFKD_SET_NULL	equ 8
SQL_SFKU_CASCADE	equ 1
SQL_SFKU_NO_ACTION	equ 2
SQL_SFKU_SET_DEFAULT	equ 4
SQL_SFKU_SET_NULL	equ 8
SQL_SG_USAGE_ON_DOMAIN	equ 1
SQL_SG_USAGE_ON_CHARACTER_SET	equ 2
SQL_SG_USAGE_ON_COLLATION	equ 4
SQL_SG_USAGE_ON_TRANSLATION	equ 8
SQL_SG_WITH_GRANT_OPTION	equ 10h
SQL_SG_DELETE_TABLE	equ 20h
SQL_SG_INSERT_TABLE	equ 40h
SQL_SG_INSERT_COLUMN	equ 80h
SQL_SG_REFERENCES_TABLE	equ 100h
SQL_SG_REFERENCES_COLUMN	equ 200h
SQL_SG_SELECT_TABLE	equ 400h
SQL_SG_UPDATE_TABLE	equ 800h
SQL_SG_UPDATE_COLUMN	equ 1000h	
SQL_SP_EXISTS	equ 1
SQL_SP_ISNOTNULL	equ 2
SQL_SP_ISNULL	equ 4
SQL_SP_MATCH_FULL	equ 8
SQL_SP_MATCH_PARTIAL	equ 10h
SQL_SP_MATCH_UNIQUE_FULL	equ 20h
SQL_SP_MATCH_UNIQUE_PARTIAL	equ 40h
SQL_SP_OVERLAPS	equ 80h
SQL_SP_UNIQUE	equ 100h
SQL_SP_LIKE	equ 200h
SQL_SP_IN	equ 400h
SQL_SP_BETWEEN	equ 800h
SQL_SP_COMPARISON	equ 1000h
SQL_SP_QUANTIFIED_COMPARISON	equ 2000h
SQL_SRJO_CORRESPONDING_CLAUSE	equ 1
SQL_SRJO_CROSS_JOIN	equ 2
SQL_SRJO_EXCEPT_JOIN	equ 4
SQL_SRJO_FULL_OUTER_JOIN	equ 8
SQL_SRJO_INNER_JOIN	equ 10h
SQL_SRJO_INTERSECT_JOIN	equ 20h
SQL_SRJO_LEFT_OUTER_JOIN	equ 40h
SQL_SRJO_NATURAL_JOIN	equ 80h
SQL_SRJO_RIGHT_OUTER_JOIN	equ 100h
SQL_SRJO_UNION_JOIN	equ 200h
SQL_SR_USAGE_ON_DOMAIN	equ 1
SQL_SR_USAGE_ON_CHARACTER_SET	equ 2
SQL_SR_USAGE_ON_COLLATION	equ 4
SQL_SR_USAGE_ON_TRANSLATION	equ 8
SQL_SR_GRANT_OPTION_FOR	equ 10h
SQL_SR_CASCADE	equ 20h
SQL_SR_RESTRICT	equ 40h
SQL_SR_DELETE_TABLE	equ 80h
SQL_SR_INSERT_TABLE	equ 100h
SQL_SR_INSERT_COLUMN	equ 200h
SQL_SR_REFERENCES_TABLE	equ 400h
SQL_SR_REFERENCES_COLUMN	equ 800h
SQL_SR_SELECT_TABLE	equ 1000h
SQL_SR_UPDATE_TABLE	equ 2000h
SQL_SR_UPDATE_COLUMN	equ 4000h
SQL_SRVC_VALUE_EXPRESSION	equ 1
SQL_SRVC_NULL	equ 2
SQL_SRVC_DEFAULT	equ 4
SQL_SRVC_ROW_SUBQUERY	equ 8
SQL_SVE_CASE	equ 1
SQL_SVE_CAST	equ 2
SQL_SVE_COALESCE	equ 4
SQL_SVE_NULLIF	equ 8
SQL_SCC_XOPEN_CLI_VERSION1	equ 1
SQL_SCC_ISO92_CLI	equ 2
SQL_US_UNION	equ SQL_U_UNION
SQL_US_UNION_ALL	equ SQL_U_UNION_ALL
SQL_DTC_ENLIST_EXPENSIVE	equ 1
SQL_DTC_UNENLIST_EXPENSIVE	equ 2
SQL_FETCH_FIRST_USER	equ 31
SQL_FETCH_FIRST_SYSTEM	equ 32
SQL_ENTIRE_ROWSET equ 0
SQL_POSITION equ 0
SQL_REFRESH equ 1
SQL_UPDATE equ 2
SQL_DELETE equ 3
SQL_ADD equ 4
SQL_SETPOS_MAX_OPTION_VALUE	equ SQL_ADD
SQL_UPDATE_BY_BOOKMARK	equ 5
SQL_DELETE_BY_BOOKMARK	equ 6
SQL_FETCH_BY_BOOKMARK	equ 7
SQL_LOCK_NO_CHANGE equ 0
SQL_LOCK_EXCLUSIVE equ 1
SQL_LOCK_UNLOCK equ 2
SQL_SETPOS_MAX_LOCK_VALUE	equ SQL_LOCK_UNLOCK
SQL_BEST_ROWID equ 1
SQL_ROWVER equ 2
SQL_PC_NOT_PSEUDO equ 1
SQL_QUICK equ 0
SQL_ENSURE equ 1
SQL_TABLE_STAT equ 0
SQL_ALL_CATALOGS	equ "%"
SQL_ALL_SCHEMAS	equ "%"
SQL_ALL_TABLE_TYPES	equ "%"
SQL_DRIVER_NOPROMPT equ 0
SQL_DRIVER_COMPLETE equ 1
SQL_DRIVER_PROMPT equ 2
SQL_DRIVER_COMPLETE_REQUIRED equ 3
SQL_FETCH_BOOKMARK equ 8
SQL_ROW_SUCCESS equ 0
SQL_ROW_DELETED equ 1
SQL_ROW_UPDATED equ 2
SQL_ROW_NOROW equ 3
SQL_ROW_ADDED equ 4
SQL_ROW_ERROR equ 5
SQL_ROW_SUCCESS_WITH_INFO	equ 6
SQL_ROW_PROCEED	equ 0
SQL_ROW_IGNORE	equ 1
SQL_PARAM_SUCCESS	equ 0
SQL_PARAM_SUCCESS_WITH_INFO	equ 6
SQL_PARAM_ERROR	equ 5
SQL_PARAM_UNUSED	equ 7
SQL_PARAM_DIAG_UNAVAILABLE	equ 1
SQL_PARAM_PROCEED	equ 0
SQL_PARAM_IGNORE	equ 1
SQL_CASCADE equ 0
SQL_RESTRICT equ 1
SQL_SET_NULL equ 2
SQL_NO_ACTION	equ 3
SQL_SET_DEFAULT	equ 4
SQL_INITIALLY_DEFERRED	equ 5
SQL_INITIALLY_IMMEDIATE	equ 6
SQL_NOT_DEFERRABLE	equ 7
SQL_PARAM_TYPE_UNKNOWN equ 0
SQL_PARAM_INPUT equ 1
SQL_PARAM_INPUT_OUTPUT equ 2
SQL_RESULT_COL equ 3
SQL_PARAM_OUTPUT equ 4
SQL_RETURN_VALUE equ 5
SQL_PT_UNKNOWN equ 0
SQL_PT_PROCEDURE equ 1
SQL_PT_FUNCTION equ 2
SQL_YEAR	equ SQL_CODE_YEAR
SQL_MONTH	equ SQL_CODE_MONTH
SQL_DAY	equ SQL_CODE_DAY
SQL_HOUR	equ SQL_CODE_HOUR
SQL_MINUTE	equ SQL_CODE_MINUTE
SQL_SECOND	equ SQL_CODE_SECOND	
SQL_YEAR_TO_MONTH	equ SQL_CODE_YEAR_TO_MONTH
SQL_DAY_TO_HOUR	equ SQL_CODE_DAY_TO_HOUR
SQL_DAY_TO_MINUTE	equ SQL_CODE_DAY_TO_MINUTE
SQL_DAY_TO_SECOND	equ SQL_CODE_DAY_TO_SECOND
SQL_HOUR_TO_MINUTE	equ SQL_CODE_HOUR_TO_MINUTE	
SQL_HOUR_TO_SECOND	equ SQL_CODE_HOUR_TO_SECOND
SQL_MINUTE_TO_SECOND	equ SQL_CODE_MINUTE_TO_SECOND
SQL_DATABASE_NAME equ 16
SQL_FD_FETCH_PREV equ SQL_FD_FETCH_PRIOR
SQL_FETCH_PREV equ SQL_FETCH_PRIOR
SQL_CONCUR_TIMESTAMP equ SQL_CONCUR_ROWVER
SQL_SCCO_OPT_TIMESTAMP equ SQL_SCCO_OPT_ROWVER
SQL_CC_DELETE equ SQL_CB_DELETE
SQL_CR_DELETE equ SQL_CB_DELETE
SQL_CC_CLOSE equ SQL_CB_CLOSE
SQL_CR_CLOSE equ SQL_CB_CLOSE
SQL_CC_PRESERVE equ SQL_CB_PRESERVE
SQL_CR_PRESERVE equ SQL_CB_PRESERVE
SQL_SCROLL_FORWARD_ONLY equ 0L 
SQL_SCROLL_KEYSET_DRIVEN equ (-1L)
SQL_SCROLL_DYNAMIC equ (-2L)
SQL_SCROLL_STATIC equ (-3L)
TRACE_VERSION	equ 1000
TRACE_ON	equ 1
TRACE_VS_EVENT_ON	equ 2	
ODBC_VS_FLAG_UNICODE_ARG	equ 1
ODBC_VS_FLAG_UNICODE_COR	equ 2
ODBC_VS_FLAG_RETCODE	equ 4
ODBC_VS_FLAG_STOP equ 8
;-----------------------------------------------------------------------------------------
;				sqltypes.h
;-----------------------------------------------------------------------------------------
DATE_STRUCT struct
         year dw ?
        month dw ?
        day dw ?
DATE_STRUCT ends
SQL_DATE_STRUCT equ DATE_STRUCT
TIME_STRUCT struct
        hour dw ?
        minute dw ?
        second dw ?
TIME_STRUCT ends
SQL_TIME_STRUCT equ TIME_STRUCT
TIMESTAMP_STRUCT struct
        year dw ?
        month dw ?
        day dw ?
        hour dw ?
        minute dw ?
        second dw ?
        fraction dd ?
TIMESTAMP_STRUCT ends
SQL_TIMESTAMP_STRUCT equ TIMESTAMP_STRUCT
SQL_IS_YEAR						equ 1
SQL_IS_MONTH					equ 2
SQL_IS_DAY						equ 3
SQL_IS_HOUR						equ 4
SQL_IS_MINUTE					equ 5
SQL_IS_SECOND					equ 6
SQL_IS_YEAR_TO_MONTH			equ 7
SQL_IS_DAY_TO_HOUR				equ 8
SQL_IS_DAY_TO_MINUTE			equ 9
SQL_IS_DAY_TO_SECOND			equ 10
SQL_IS_HOUR_TO_MINUTE			equ 11
SQL_IS_HOUR_TO_SECOND			equ 12
SQL_IS_MINUTE_TO_SECOND		equ 13
SQL_YEAR_MONTH_STRUCT struct
		year dword ?
		month dword ?
SQL_YEAR_MONTH_STRUCT ends
SQL_DAY_SECOND_STRUCT struct
		day dword ?
		hour dword ?
		minute dword ?
		second dword ?
		fraction dword ?
SQL_DAY_SECOND_STRUCT ends

SQL_INTERVAL_STRUCT struct
	interval_type dword ?
	interval_sign dword ?
	union intval
		year_month SQL_YEAR_MONTH_STRUCT <>
		day_second SQL_DAY_SECOND_STRUCT <>
	ends
SQL_INTERVAL_STRUCT ends
SQL_MAX_NUMERIC_LEN		equ 16
SQL_NUMERIC_STRUCT struct
	precision db ?
	scale db ?
	sign db ?
	val db SQL_MAX_NUMERIC_LEN dup(?)
SQL_NUMERIC_STRUCT ends
SQLGUID struct
    Data1 dd ?
    Data2 dw ?
    Data3 dw ?
    Data4  db 8 dup(?)
SQLGUID ends
; ------------------------- equates for opengl ----------------------------

GL_VERSION_1_1                     equ  1
GL_ACCUM                           equ  00100h
GL_LOAD                            equ  00101h
GL_RETURN                          equ  00102h
GL_MULT                            equ  00103h
GL_ADD                             equ  00104h
GL_NEVER                           equ  00200h
GL_LESS                            equ  00201h
GL_EQUAL                           equ  00202h
GL_LEQUAL                          equ  00203h
GL_GREATER                         equ  00204h
GL_NOTEQUAL                        equ  00205h
GL_GEQUAL                          equ  00206h
GL_ALWAYS                          equ  00207h
GL_CURRENT_BIT                     equ  000000001h
GL_POINT_BIT                       equ  000000002h
GL_LINE_BIT                        equ  000000004h
GL_POLYGON_BIT                     equ  000000008h
GL_POLYGON_STIPPLE_BIT             equ  000000010h
GL_PIXEL_MODE_BIT                  equ  000000020h
GL_LIGHTING_BIT                    equ  000000040h
GL_FOG_BIT                         equ  000000080h
GL_DEPTH_BUFFER_BIT                equ  000000100h
GL_ACCUM_BUFFER_BIT                equ  000000200h
GL_STENCIL_BUFFER_BIT              equ  000000400h
GL_VIEWPORT_BIT                    equ  000000800h
GL_TRANSFORM_BIT                   equ  000001000h
GL_ENABLE_BIT                      equ  000002000h
GL_COLOR_BUFFER_BIT                equ  000004000h
GL_HINT_BIT                        equ  000008000h
GL_EVAL_BIT                        equ  000010000h
GL_LIST_BIT                        equ  000020000h
GL_TEXTURE_BIT                     equ  000040000h
GL_SCISSOR_BIT                     equ  000080000h
GL_ALL_ATTRIB_BITS                 equ  0000fffffh
GL_POINTS                          equ  00000h
GL_LINES                           equ  00001h
GL_LINE_LOOP                       equ  00002h
GL_LINE_STRIP                      equ  00003h
GL_TRIANGLES                       equ  00004h
GL_TRIANGLE_STRIP                  equ  00005h
GL_TRIANGLE_FAN                    equ  00006h
GL_QUADS                           equ  00007h
GL_QUAD_STRIP                      equ  00008h
GL_POLYGON                         equ  00009h
GL_ZERO                            equ  0
GL_ONE                             equ  1
GL_SRC_COLOR                       equ  00300h
GL_ONE_MINUS_SRC_COLOR             equ  00301h
GL_SRC_ALPHA                       equ  00302h
GL_ONE_MINUS_SRC_ALPHA             equ  00303h
GL_DST_ALPHA                       equ  00304h
GL_ONE_MINUS_DST_ALPHA             equ  00305h
GL_DST_COLOR                       equ  00306h
GL_ONE_MINUS_DST_COLOR             equ  00307h
GL_SRC_ALPHA_SATURATE              equ  00308h
GL_TRUE                            equ  1
GL_FALSE                           equ  0
GL_CLIP_PLANE0                     equ  03000h
GL_CLIP_PLANE1                     equ  03001h
GL_CLIP_PLANE2                     equ  03002h
GL_CLIP_PLANE3                     equ  03003h
GL_CLIP_PLANE4                     equ  03004h
GL_CLIP_PLANE5                     equ  03005h
GL_BYTE                            equ  01400h
GL_UNSIGNED_BYTE                   equ  01401h
GL_SHORT                           equ  01402h
GL_UNSIGNED_SHORT                  equ  01403h
GL_INT                             equ  01404h
GL_UNSIGNED_INT                    equ  01405h
GL_FLOAT                           equ  01406h
GL_2_BYTES                         equ  01407h
GL_3_BYTES                         equ  01408h
GL_4_BYTES                         equ  01409h
GL_DOUBLE                          equ  0140Ah
GL_NONE                            equ  0

GL_FRONT_LEFT                      equ  00400h
GL_FRONT_RIGHT                     equ  00401h
GL_BACK_LEFT                       equ  00402h
GL_BACK_RIGHT                      equ  00403h
GL_FRONT                           equ  00404h
GL_BACK                            equ  00405h
GL_LEFT                            equ  00406h
GL_RIGHT                           equ  00407h
GL_FRONT_AND_BACK                  equ  00408h
GL_AUX0                            equ  00409h
GL_AUX1                            equ  0040Ah
GL_AUX2                            equ  0040Bh
GL_AUX3                            equ  0040Ch
GL_NO_ERROR                        equ  0
GL_INVALID_ENUM                    equ  00500h
GL_INVALID_VALUE                   equ  00501h
GL_INVALID_OPERATION               equ  00502h
GL_STACK_OVERFLOW                  equ  00503h
GL_STACK_UNDERFLOW                 equ  00504h
GL_OUT_OF_MEMORY                   equ  00505h
GL_2D                              equ  00600h
GL_3D                              equ  00601h
GL_3D_COLOR                        equ  00602h
GL_3D_COLOR_TEXTURE                equ  00603h
GL_4D_COLOR_TEXTURE                equ  00604h
GL_PASS_THROUGH_TOKEN              equ  00700h
GL_POINT_TOKEN                     equ  00701h
GL_LINE_TOKEN                      equ  00702h
GL_POLYGON_TOKEN                   equ  00703h
GL_BITMAP_TOKEN                    equ  00704h
GL_DRAW_PIXEL_TOKEN                equ  00705h
GL_COPY_PIXEL_TOKEN                equ  00706h
GL_LINE_RESET_TOKEN                equ  00707h
GL_EXP                             equ  00800h
GL_EXP2                            equ  00801h
GL_CW                              equ  00900h
GL_CCW                             equ  00901h
GL_COEFF                           equ  00A00h
GL_ORDER                           equ  00A01h
GL_DOMAIN                          equ  00A02h
GL_CURRENT_COLOR                   equ  00B00h
GL_CURRENT_INDEX                   equ  00B01h
GL_CURRENT_NORMAL                  equ  00B02h
GL_CURRENT_TEXTURE_COORDS          equ  00B03h
GL_CURRENT_RASTER_COLOR            equ  00B04h
GL_CURRENT_RASTER_INDEX            equ  00B05h
GL_CURRENT_RASTER_TEXTURE_COORDS   equ  00B06h
GL_CURRENT_RASTER_POSITION         equ  00B07h
GL_CURRENT_RASTER_POSITION_VALID   equ  00B08h
GL_CURRENT_RASTER_DISTANCE         equ  00B09h
GL_POINT_SMOOTH                    equ  00B10h
GL_POINT_SIZE                      equ  00B11h
GL_POINT_SIZE_RANGE                equ  00B12h
GL_POINT_SIZE_GRANULARITY          equ  00B13h
GL_LINE_SMOOTH                     equ  00B20h
GL_LINE_WIDTH                      equ  00B21h
GL_LINE_WIDTH_RANGE                equ  00B22h
GL_LINE_WIDTH_GRANULARITY          equ  00B23h
GL_LINE_STIPPLE                    equ  00B24h
GL_LINE_STIPPLE_PATTERN            equ  00B25h
GL_LINE_STIPPLE_REPEAT             equ  00B26h
GL_LIST_MODE                       equ  00B30h
GL_MAX_LIST_NESTING                equ  00B31h
GL_LIST_BASE                       equ  00B32h
GL_LIST_INDEX                      equ  00B33h
GL_POLYGON_MODE                    equ  00B40h
GL_POLYGON_SMOOTH                  equ  00B41h
GL_POLYGON_STIPPLE                 equ  00B42h
GL_EDGE_FLAG                       equ  00B43h
GL_CULL_FACE                       equ  00B44h
GL_CULL_FACE_MODE                  equ  00B45h
GL_FRONT_FACE                      equ  00B46h
GL_LIGHTING                        equ  00B50h
GL_LIGHT_MODEL_LOCAL_VIEWER        equ  00B51h
GL_LIGHT_MODEL_TWO_SIDE            equ  00B52h
GL_LIGHT_MODEL_AMBIENT             equ  00B53h
GL_SHADE_MODEL                     equ  00B54h
GL_COLOR_MATERIAL_FACE             equ  00B55h
GL_COLOR_MATERIAL_PARAMETER        equ  00B56h
GL_COLOR_MATERIAL                  equ  00B57h
GL_FOG                             equ  00B60h
GL_FOG_INDEX                       equ  00B61h
GL_FOG_DENSITY                     equ  00B62h
GL_FOG_START                       equ  00B63h
GL_FOG_END                         equ  00B64h
GL_FOG_MODE                        equ  00B65h
GL_FOG_COLOR                       equ  00B66h
GL_DEPTH_RANGE                     equ  00B70h
GL_DEPTH_TEST                      equ  00B71h
GL_DEPTH_WRITEMASK                 equ  00B72h
GL_DEPTH_CLEAR_VALUE               equ  00B73h
GL_DEPTH_FUNC                      equ  00B74h
GL_ACCUM_CLEAR_VALUE               equ  00B80h
GL_STENCIL_TEST                    equ  00B90h
GL_STENCIL_CLEAR_VALUE             equ  00B91h
GL_STENCIL_FUNC                    equ  00B92h
GL_STENCIL_VALUE_MASK              equ  00B93h
GL_STENCIL_FAIL                    equ  00B94h
GL_STENCIL_PASS_DEPTH_FAIL         equ  00B95h
GL_STENCIL_PASS_DEPTH_PASS         equ  00B96h
GL_STENCIL_REF                     equ  00B97h
GL_STENCIL_WRITEMASK               equ  00B98h
GL_MATRIX_MODE                     equ  00BA0h
GL_NORMALIZE                       equ  00BA1h
GL_VIEWPORT                        equ  00BA2h
GL_MODELVIEW_STACK_DEPTH           equ  00BA3h
GL_PROJECTION_STACK_DEPTH          equ  00BA4h
GL_TEXTURE_STACK_DEPTH             equ  00BA5h
GL_MODELVIEW_MATRIX                equ  00BA6h
GL_PROJECTION_MATRIX               equ  00BA7h
GL_TEXTURE_MATRIX                  equ  00BA8h
GL_ATTRIB_STACK_DEPTH              equ  00BB0h
GL_CLIENT_ATTRIB_STACK_DEPTH       equ  00BB1h
GL_ALPHA_TEST                      equ  00BC0h
GL_ALPHA_TEST_FUNC                 equ  00BC1h
GL_ALPHA_TEST_REF                  equ  00BC2h
GL_DITHER                          equ  00BD0h
GL_BLEND_DST                       equ  00BE0h
GL_BLEND_SRC                       equ  00BE1h
GL_BLEND                           equ  00BE2h
GL_LOGIC_OP_MODE                   equ  00BF0h
GL_INDEX_LOGIC_OP                  equ  00BF1h
GL_COLOR_LOGIC_OP                  equ  00BF2h
GL_AUX_BUFFERS                     equ  00C00h
GL_DRAW_BUFFER                     equ  00C01h
GL_READ_BUFFER                     equ  00C02h
GL_SCISSOR_BOX                     equ  00C10h
GL_SCISSOR_TEST                    equ  00C11h
GL_INDEX_CLEAR_VALUE               equ  00C20h
GL_INDEX_WRITEMASK                 equ  00C21h
GL_COLOR_CLEAR_VALUE               equ  00C22h
GL_COLOR_WRITEMASK                 equ  00C23h
GL_INDEX_MODE                      equ  00C30h
GL_RGBA_MODE                       equ  00C31h
GL_DOUBLEBUFFER                    equ  00C32h
GL_STEREO                          equ  00C33h
GL_RENDER_MODE                     equ  00C40h
GL_PERSPECTIVE_CORRECTION_HINT     equ  00C50h
GL_POINT_SMOOTH_HINT               equ  00C51h
GL_LINE_SMOOTH_HINT                equ  00C52h
GL_POLYGON_SMOOTH_HINT             equ  00C53h
GL_FOG_HINT                        equ  00C54h
GL_TEXTURE_GEN_S                   equ  00C60h
GL_TEXTURE_GEN_T                   equ  00C61h
GL_TEXTURE_GEN_R                   equ  00C62h
GL_TEXTURE_GEN_Q                   equ  00C63h
GL_PIXEL_MAP_I_TO_I                equ  00C70h
GL_PIXEL_MAP_S_TO_S                equ  00C71h
GL_PIXEL_MAP_I_TO_R                equ  00C72h
GL_PIXEL_MAP_I_TO_G                equ  00C73h
GL_PIXEL_MAP_I_TO_B                equ  00C74h
GL_PIXEL_MAP_I_TO_A                equ  00C75h
GL_PIXEL_MAP_R_TO_R                equ  00C76h
GL_PIXEL_MAP_G_TO_G                equ  00C77h
GL_PIXEL_MAP_B_TO_B                equ  00C78h
GL_PIXEL_MAP_A_TO_A                equ  00C79h
GL_PIXEL_MAP_I_TO_I_SIZE           equ  00CB0h
GL_PIXEL_MAP_S_TO_S_SIZE           equ  00CB1h
GL_PIXEL_MAP_I_TO_R_SIZE           equ  00CB2h
GL_PIXEL_MAP_I_TO_G_SIZE           equ  00CB3h
GL_PIXEL_MAP_I_TO_B_SIZE           equ  00CB4h
GL_PIXEL_MAP_I_TO_A_SIZE           equ  00CB5h
GL_PIXEL_MAP_R_TO_R_SIZE           equ  00CB6h
GL_PIXEL_MAP_G_TO_G_SIZE           equ  00CB7h
GL_PIXEL_MAP_B_TO_B_SIZE           equ  00CB8h
GL_PIXEL_MAP_A_TO_A_SIZE           equ  00CB9h
GL_UNPACK_SWAP_BYTES               equ  00CF0h
GL_UNPACK_LSB_FIRST                equ  00CF1h
GL_UNPACK_ROW_LENGTH               equ  00CF2h
GL_UNPACK_SKIP_ROWS                equ  00CF3h
GL_UNPACK_SKIP_PIXELS              equ  00CF4h
GL_UNPACK_ALIGNMENT                equ  00CF5h
GL_PACK_SWAP_BYTES                 equ  00D00h
GL_PACK_LSB_FIRST                  equ  00D01h
GL_PACK_ROW_LENGTH                 equ  00D02h
GL_PACK_SKIP_ROWS                  equ  00D03h
GL_PACK_SKIP_PIXELS                equ  00D04h
GL_PACK_ALIGNMENT                  equ  00D05h
GL_MAP_COLOR                       equ  00D10h
GL_MAP_STENCIL                     equ  00D11h
GL_INDEX_SHIFT                     equ  00D12h
GL_INDEX_OFFSET                    equ  00D13h
GL_RED_SCALE                       equ  00D14h
GL_RED_BIAS                        equ  00D15h
GL_ZOOM_X                          equ  00D16h
GL_ZOOM_Y                          equ  00D17h
GL_GREEN_SCALE                     equ  00D18h
GL_GREEN_BIAS                      equ  00D19h
GL_BLUE_SCALE                      equ  00D1Ah
GL_BLUE_BIAS                       equ  00D1Bh
GL_ALPHA_SCALE                     equ  00D1Ch
GL_ALPHA_BIAS                      equ  00D1Dh
GL_DEPTH_SCALE                     equ  00D1Eh
GL_DEPTH_BIAS                      equ  00D1Fh
GL_MAX_EVAL_ORDER                  equ  00D30h
GL_MAX_LIGHTS                      equ  00D31h
GL_MAX_CLIP_PLANES                 equ  00D32h
GL_MAX_TEXTURE_SIZE                equ  00D33h
GL_MAX_PIXEL_MAP_TABLE             equ  00D34h
GL_MAX_ATTRIB_STACK_DEPTH          equ  00D35h
GL_MAX_MODELVIEW_STACK_DEPTH       equ  00D36h
GL_MAX_NAME_STACK_DEPTH            equ  00D37h
GL_MAX_PROJECTION_STACK_DEPTH      equ  00D38h
GL_MAX_TEXTURE_STACK_DEPTH         equ  00D39h
GL_MAX_VIEWPORT_DIMS               equ  00D3Ah
GL_MAX_CLIENT_ATTRIB_STACK_DEPTH   equ  00D3Bh
GL_SUBPIXEL_BITS                   equ  00D50h
GL_INDEX_BITS                      equ  00D51h
GL_RED_BITS                        equ  00D52h
GL_GREEN_BITS                      equ  00D53h
GL_BLUE_BITS                       equ  00D54h
GL_ALPHA_BITS                      equ  00D55h
GL_DEPTH_BITS                      equ  00D56h
GL_STENCIL_BITS                    equ  00D57h
GL_ACCUM_RED_BITS                  equ  00D58h
GL_ACCUM_GREEN_BITS                equ  00D59h
GL_ACCUM_BLUE_BITS                 equ  00D5Ah
GL_ACCUM_ALPHA_BITS                equ  00D5Bh
GL_NAME_STACK_DEPTH                equ  00D70h
GL_AUTO_NORMAL                     equ  00D80h
GL_MAP1_COLOR_4                    equ  00D90h
GL_MAP1_INDEX                      equ  00D91h
GL_MAP1_NORMAL                     equ  00D92h
GL_MAP1_TEXTURE_COORD_1            equ  00D93h
GL_MAP1_TEXTURE_COORD_2            equ  00D94h
GL_MAP1_TEXTURE_COORD_3            equ  00D95h
GL_MAP1_TEXTURE_COORD_4            equ  00D96h
GL_MAP1_VERTEX_3                   equ  00D97h
GL_MAP1_VERTEX_4                   equ  00D98h
GL_MAP2_COLOR_4                    equ  00DB0h
GL_MAP2_INDEX                      equ  00DB1h
GL_MAP2_NORMAL                     equ  00DB2h
GL_MAP2_TEXTURE_COORD_1            equ  00DB3h
GL_MAP2_TEXTURE_COORD_2            equ  00DB4h
GL_MAP2_TEXTURE_COORD_3            equ  00DB5h
GL_MAP2_TEXTURE_COORD_4            equ  00DB6h
GL_MAP2_VERTEX_3                   equ  00DB7h
GL_MAP2_VERTEX_4                   equ  00DB8h
GL_MAP1_GRID_DOMAIN                equ  00DD0h
GL_MAP1_GRID_SEGMENTS              equ  00DD1h
GL_MAP2_GRID_DOMAIN                equ  00DD2h
GL_MAP2_GRID_SEGMENTS              equ  00DD3h
GL_TEXTURE_1D                      equ  00DE0h
GL_TEXTURE_2D                      equ  00DE1h
GL_FEEDBACK_BUFFER_POINTER         equ  00DF0h
GL_FEEDBACK_BUFFER_SIZE            equ  00DF1h
GL_FEEDBACK_BUFFER_TYPE            equ  00DF2h
GL_SELECTION_BUFFER_POINTER        equ  00DF3h
GL_SELECTION_BUFFER_SIZE           equ  00DF4h
GL_TEXTURE_WIDTH                   equ  01000h
GL_TEXTURE_HEIGHT                  equ  01001h
GL_TEXTURE_INTERNAL_FORMAT         equ  01003h
GL_TEXTURE_BORDER_COLOR            equ  01004h
GL_TEXTURE_BORDER                  equ  01005h
GL_DONT_CARE                       equ  01100h
GL_FASTEST                         equ  01101h
GL_NICEST                          equ  01102h
GL_LIGHT0                          equ  04000h
GL_LIGHT1                          equ  04001h
GL_LIGHT2                          equ  04002h
GL_LIGHT3                          equ  04003h
GL_LIGHT4                          equ  04004h
GL_LIGHT5                          equ  04005h
GL_LIGHT6                          equ  04006h
GL_LIGHT7                          equ  04007h
GL_AMBIENT                         equ  01200h
GL_DIFFUSE                         equ  01201h
GL_SPECULAR                        equ  01202h
GL_POSITION                        equ  01203h
GL_SPOT_DIRECTION                  equ  01204h
GL_SPOT_EXPONENT                   equ  01205h
GL_SPOT_CUTOFF                     equ  01206h
GL_CONSTANT_ATTENUATION            equ  01207h
GL_LINEAR_ATTENUATION              equ  01208h
GL_QUADRATIC_ATTENUATION           equ  01209h
GL_COMPILE                         equ  01300h
GL_COMPILE_AND_EXECUTE             equ  01301h
GL_CLEAR                           equ  01500h
GL_AND                             equ  01501h
GL_AND_REVERSE                     equ  01502h
GL_COPY                            equ  01503h
GL_AND_INVERTED                    equ  01504h
GL_NOOP                            equ  01505h
GL_XOR                             equ  01506h
GL_OR                              equ  01507h
GL_NOR                             equ  01508h
GL_EQUIV                           equ  01509h
GL_INVERT                          equ  0150Ah
GL_OR_REVERSE                      equ  0150Bh
GL_COPY_INVERTED                   equ  0150Ch
GL_OR_INVERTED                     equ  0150Dh
GL_NAND                            equ  0150Eh
GL_SET                             equ  0150Fh
GL_EMISSION                        equ  01600h
GL_SHININESS                       equ  01601h
GL_AMBIENT_AND_DIFFUSE             equ  01602h
GL_COLOR_INDEXES                   equ  01603h
GL_MODELVIEW                       equ  01700h
GL_PROJECTION                      equ  01701h
GL_TEXTURE                         equ  01702h
GL_COLOR                           equ  01800h
GL_DEPTH                           equ  01801h
GL_STENCIL                         equ  01802h
GL_COLOR_INDEX                     equ  01900h
GL_STENCIL_INDEX                   equ  01901h
GL_DEPTH_COMPONENT                 equ  01902h
GL_RED                             equ  01903h
GL_GREEN                           equ  01904h
GL_BLUE                            equ  01905h
GL_ALPHA                           equ  01906h
GL_RGB                             equ  01907h
GL_RGBA                            equ  01908h
GL_LUMINANCE                       equ  01909h
GL_LUMINANCE_ALPHA                 equ  0190Ah
GL_BITMAP                          equ  01A00h
GL_POINT                           equ  01B00h
GL_LINE                            equ  01B01h
GL_FILL                            equ  01B02h
GL_RENDER                          equ  01C00h
GL_FEEDBACK                        equ  01C01h
GL_SELECT                          equ  01C02h
GL_FLAT                            equ  01D00h
GL_SMOOTH                          equ  01D01h
GL_KEEP                            equ  01E00h
GL_REPLACE                         equ  01E01h
GL_INCR                            equ  01E02h
GL_DECR                            equ  01E03h
GL_VENDOR                          equ  01F00h
GL_RENDERER                        equ  01F01h
GL_VERSION                         equ  01F02h
GL_EXTENSIONS                      equ  01F03h
GL_S                               equ  02000h
GL_T                               equ  02001h
GL_R                               equ  02002h
GL_Q                               equ  02003h
GL_MODULATE                        equ  02100h
GL_DECAL                           equ  02101h
GL_TEXTURE_ENV_MODE                equ  02200h
GL_TEXTURE_ENV_COLOR               equ  02201h
GL_TEXTURE_ENV                     equ  02300h
GL_EYE_LINEAR                      equ  02400h
GL_OBJECT_LINEAR                   equ  02401h
GL_SPHERE_MAP                      equ  02402h
GL_TEXTURE_GEN_MODE                equ  02500h
GL_OBJECT_PLANE                    equ  02501h
GL_EYE_PLANE                       equ  02502h
GL_NEAREST                         equ  02600h
GL_LINEAR                          equ  02601h
GL_NEAREST_MIPMAP_NEAREST          equ  02700h
GL_LINEAR_MIPMAP_NEAREST           equ  02701h
GL_NEAREST_MIPMAP_LINEAR           equ  02702h
GL_LINEAR_MIPMAP_LINEAR            equ  02703h
GL_TEXTURE_MAG_FILTER              equ  02800h
GL_TEXTURE_MIN_FILTER              equ  02801h
GL_TEXTURE_WRAP_S                  equ  02802h
GL_TEXTURE_WRAP_T                  equ  02803h
GL_CLAMP                           equ  02900h
GL_REPEAT                          equ  02901h
GL_CLIENT_PIXEL_STORE_BIT          equ  000000001h
GL_CLIENT_VERTEX_ARRAY_BIT         equ  000000002h
GL_CLIENT_ALL_ATTRIB_BITS          equ  0ffffffffh
GL_POLYGON_OFFSET_FACTOR           equ  08038h
GL_POLYGON_OFFSET_UNITS            equ  02A00h
GL_POLYGON_OFFSET_POINT            equ  02A01h
GL_POLYGON_OFFSET_LINE             equ  02A02h
GL_POLYGON_OFFSET_FILL             equ  08037h
GL_ALPHA4                          equ  0803Bh
GL_ALPHA8                          equ  0803Ch
GL_ALPHA12                         equ  0803Dh
GL_ALPHA16                         equ  0803Eh
GL_LUMINANCE4                      equ  0803Fh
GL_LUMINANCE8                      equ  08040h
GL_LUMINANCE12                     equ  08041h
GL_LUMINANCE16                     equ  08042h
GL_LUMINANCE4_ALPHA4               equ  08043h
GL_LUMINANCE6_ALPHA2               equ  08044h
GL_LUMINANCE8_ALPHA8               equ  08045h
GL_LUMINANCE12_ALPHA4              equ  08046h
GL_LUMINANCE12_ALPHA12             equ  08047h
GL_LUMINANCE16_ALPHA16             equ  08048h
GL_INTENSITY                       equ  08049h
GL_INTENSITY4                      equ  0804Ah
GL_INTENSITY8                      equ  0804Bh
GL_INTENSITY12                     equ  0804Ch
GL_INTENSITY16                     equ  0804Dh
GL_R3_G3_B2                        equ  02A10h
GL_RGB4                            equ  0804Fh
GL_RGB5                            equ  08050h
GL_RGB8                            equ  08051h
GL_RGB10                           equ  08052h
GL_RGB12                           equ  08053h
GL_RGB16                           equ  08054h
GL_RGBA2                           equ  08055h
GL_RGBA4                           equ  08056h
GL_RGB5_A1                         equ  08057h
GL_RGBA8                           equ  08058h
GL_RGB10_A2                        equ  08059h
GL_RGBA12                          equ  0805Ah
GL_RGBA16                          equ  0805Bh
GL_TEXTURE_RED_SIZE                equ  0805Ch
GL_TEXTURE_GREEN_SIZE              equ  0805Dh
GL_TEXTURE_BLUE_SIZE               equ  0805Eh
GL_TEXTURE_ALPHA_SIZE              equ  0805Fh
GL_TEXTURE_LUMINANCE_SIZE          equ  08060h
GL_TEXTURE_INTENSITY_SIZE          equ  08061h
GL_PROXY_TEXTURE_1D                equ  08063h
GL_PROXY_TEXTURE_2D                equ  08064h
GL_TEXTURE_PRIORITY                equ  08066h
GL_TEXTURE_RESIDENT                equ  08067h
GL_TEXTURE_BINDING_1D              equ  08068h
GL_TEXTURE_BINDING_2D              equ  08069h
GL_VERTEX_ARRAY                    equ  08074h
GL_NORMAL_ARRAY                    equ  08075h
GL_COLOR_ARRAY                     equ  08076h
GL_INDEX_ARRAY                     equ  08077h
GL_TEXTURE_COORD_ARRAY             equ  08078h
GL_EDGE_FLAG_ARRAY                 equ  08079h
GL_VERTEX_ARRAY_SIZE               equ  0807Ah
GL_VERTEX_ARRAY_TYPE               equ  0807Bh
GL_VERTEX_ARRAY_STRIDE             equ  0807Ch
GL_NORMAL_ARRAY_TYPE               equ  0807Eh
GL_NORMAL_ARRAY_STRIDE             equ  0807Fh
GL_COLOR_ARRAY_SIZE                equ  08081h
GL_COLOR_ARRAY_TYPE                equ  08082h
GL_COLOR_ARRAY_STRIDE              equ  08083h
GL_INDEX_ARRAY_TYPE                equ  08085h
GL_INDEX_ARRAY_STRIDE              equ  08086h
GL_TEXTURE_COORD_ARRAY_SIZE        equ  08088h
GL_TEXTURE_COORD_ARRAY_TYPE        equ  08089h
GL_TEXTURE_COORD_ARRAY_STRIDE      equ  0808Ah
GL_EDGE_FLAG_ARRAY_STRIDE          equ  0808Ch
GL_VERTEX_ARRAY_POINTER            equ  0808Eh
GL_NORMAL_ARRAY_POINTER            equ  0808Fh
GL_COLOR_ARRAY_POINTER             equ  08090h
GL_INDEX_ARRAY_POINTER             equ  08091h
GL_TEXTURE_COORD_ARRAY_POINTER     equ  08092h
GL_EDGE_FLAG_ARRAY_POINTER         equ  08093h
GL_V2F                             equ  02A20h
GL_V3F                             equ  02A21h
GL_C4UB_V2F                        equ  02A22h
GL_C4UB_V3F                        equ  02A23h
GL_C3F_V3F                         equ  02A24h
GL_N3F_V3F                         equ  02A25h
GL_C4F_N3F_V3F                     equ  02A26h
GL_T2F_V3F                         equ  02A27h
GL_T4F_V4F                         equ  02A28h
GL_T2F_C4UB_V3F                    equ  02A29h
GL_T2F_C3F_V3F                     equ  02A2Ah
GL_T2F_N3F_V3F                     equ  02A2Bh
GL_T2F_C4F_N3F_V3F                 equ  02A2Ch
GL_T4F_C4F_N3F_V4F                 equ  02A2Dh
GL_EXT_vertex_array                equ  1
GL_EXT_bgra                        equ  1
GL_EXT_paletted_texture            equ  1
GL_WIN_swap_hint                   equ  1
GL_WIN_draw_range_elements         equ  1
GL_VERTEX_ARRAY_EXT                equ  08074h
GL_NORMAL_ARRAY_EXT                equ  08075h
GL_COLOR_ARRAY_EXT                 equ  08076h
GL_INDEX_ARRAY_EXT                 equ  08077h
GL_TEXTURE_COORD_ARRAY_EXT         equ  08078h
GL_EDGE_FLAG_ARRAY_EXT             equ  08079h
GL_VERTEX_ARRAY_SIZE_EXT           equ  0807Ah
GL_VERTEX_ARRAY_TYPE_EXT           equ  0807Bh
GL_VERTEX_ARRAY_STRIDE_EXT         equ  0807Ch
GL_VERTEX_ARRAY_COUNT_EXT          equ  0807Dh
GL_NORMAL_ARRAY_TYPE_EXT           equ  0807Eh
GL_NORMAL_ARRAY_STRIDE_EXT         equ  0807Fh
GL_NORMAL_ARRAY_COUNT_EXT          equ  08080h
GL_COLOR_ARRAY_SIZE_EXT            equ  08081h
GL_COLOR_ARRAY_TYPE_EXT            equ  08082h
GL_COLOR_ARRAY_STRIDE_EXT          equ  08083h
GL_COLOR_ARRAY_COUNT_EXT           equ  08084h
GL_INDEX_ARRAY_TYPE_EXT            equ  08085h
GL_INDEX_ARRAY_STRIDE_EXT          equ  08086h
GL_INDEX_ARRAY_COUNT_EXT           equ  08087h
GL_TEXTURE_COORD_ARRAY_SIZE_EXT    equ  08088h
GL_TEXTURE_COORD_ARRAY_TYPE_EXT    equ  08089h
GL_TEXTURE_COORD_ARRAY_STRIDE_EXT  equ  0808Ah
GL_TEXTURE_COORD_ARRAY_COUNT_EXT   equ  0808Bh
GL_EDGE_FLAG_ARRAY_STRIDE_EXT      equ  0808Ch
GL_EDGE_FLAG_ARRAY_COUNT_EXT       equ  0808Dh
GL_VERTEX_ARRAY_POINTER_EXT        equ  0808Eh
GL_NORMAL_ARRAY_POINTER_EXT        equ  0808Fh
GL_COLOR_ARRAY_POINTER_EXT         equ  08090h
GL_INDEX_ARRAY_POINTER_EXT         equ  08091h
GL_TEXTURE_COORD_ARRAY_POINTER_EXT equ  08092h
GL_EDGE_FLAG_ARRAY_POINTER_EXT     equ  08093h
GL_DOUBLE_EXT                      equ  GL_DOUBLE
GL_BGR_EXT                         equ  080E0h
GL_BGRA_EXT                        equ  080E1h
GL_COLOR_TABLE_FORMAT_EXT          equ  080D8h
GL_COLOR_TABLE_WIDTH_EXT           equ  080D9h
GL_COLOR_TABLE_RED_SIZE_EXT        equ  080DAh
GL_COLOR_TABLE_GREEN_SIZE_EXT      equ  080DBh
GL_COLOR_TABLE_BLUE_SIZE_EXT       equ  080DCh
GL_COLOR_TABLE_ALPHA_SIZE_EXT      equ  080DDh
GL_COLOR_TABLE_LUMINANCE_SIZE_EXT  equ  080DEh
GL_COLOR_TABLE_INTENSITY_SIZE_EXT  equ  080DFh
GL_COLOR_INDEX1_EXT                equ  080E2h
GL_COLOR_INDEX2_EXT                equ  080E3h
GL_COLOR_INDEX4_EXT                equ  080E4h
GL_COLOR_INDEX8_EXT                equ  080E5h
GL_COLOR_INDEX12_EXT               equ  080E6h
GL_COLOR_INDEX16_EXT               equ  080E7h
GL_MAX_ELEMENTS_VERTICES_WIN       equ  080E8h
GL_MAX_ELEMENTS_INDICES_WIN        equ  080E9h
GL_PHONG_WIN                       equ  080EAh
GL_PHONG_HINT_WIN                  equ  080EBh
GL_FOG_SPECULAR_TEXTURE_WIN        equ  080ECh
GL_LOGIC_OP                        equ  GL_INDEX_LOGIC_OP
GL_TEXTURE_COMPONENTS              equ  GL_TEXTURE_INTERNAL_FORMAT


GLU_VERSION_1_1                 equ  1
GLU_VERSION_1_2                 equ  1
GLU_INVALID_ENUM                equ  100900
GLU_INVALID_VALUE               equ  100901
GLU_OUT_OF_MEMORY               equ  100902
GLU_INCOMPATIBLE_GL_VERSION     equ  100903
GLU_VERSION                     equ  100800
GLU_EXTENSIONS                  equ  100801
GLU_TRUE                        equ  GL_TRUE
GLU_FALSE                       equ  GL_FALSE
GLU_SMOOTH                      equ  100000
GLU_FLAT                        equ  100001
GLU_NONE                        equ  100002
GLU_POINT                       equ  100010
GLU_LINE                        equ  100011
GLU_FILL                        equ  100012
GLU_SILHOUETTE                  equ  100013
GLU_OUTSIDE                     equ  100020
GLU_INSIDE                      equ  100021
GLU_TESS_MAX_COORD              equ  1.0e150
GLU_TESS_WINDING_RULE           equ  100140
GLU_TESS_BOUNDARY_ONLY          equ  100141
GLU_TESS_TOLERANCE              equ  100142
GLU_TESS_WINDING_ODD            equ  100130
GLU_TESS_WINDING_NONZERO        equ  100131
GLU_TESS_WINDING_POSITIVE       equ  100132
GLU_TESS_WINDING_NEGATIVE       equ  100133
GLU_TESS_WINDING_ABS_GEQ_TWO    equ  100134
GLU_TESS_BEGIN                  equ  100100
GLU_TESS_VERTEX                 equ  100101
GLU_TESS_END                    equ  100102
GLU_TESS_ERROR                  equ  100103
GLU_TESS_EDGE_FLAG              equ  100104
GLU_TESS_COMBINE                equ  100105
GLU_TESS_BEGIN_DATA             equ  100106
GLU_TESS_VERTEX_DATA            equ  100107
GLU_TESS_END_DATA               equ  100108
GLU_TESS_ERROR_DATA             equ  100109
GLU_TESS_EDGE_FLAG_DATA         equ  100110
GLU_TESS_COMBINE_DATA           equ  100111
GLU_TESS_ERROR1                 equ  100151
GLU_TESS_ERROR2                 equ  100152
GLU_TESS_ERROR3                 equ  100153
GLU_TESS_ERROR4                 equ  100154
GLU_TESS_ERROR5                 equ  100155
GLU_TESS_ERROR6                 equ  100156
GLU_TESS_ERROR7                 equ  100157
GLU_TESS_ERROR8                 equ  100158
GLU_TESS_MISSING_BEGIN_POLYGON  equ  GLU_TESS_ERROR1
GLU_TESS_MISSING_BEGIN_CONTOUR  equ  GLU_TESS_ERROR2
GLU_TESS_MISSING_END_POLYGON    equ  GLU_TESS_ERROR3
GLU_TESS_MISSING_END_CONTOUR    equ  GLU_TESS_ERROR4
GLU_TESS_COORD_TOO_LARGE        equ  GLU_TESS_ERROR5
GLU_TESS_NEED_COMBINE_CALLBACK  equ  GLU_TESS_ERROR6
GLU_AUTO_LOAD_MATRIX            equ  100200
GLU_CULLING                     equ  100201
GLU_SAMPLING_TOLERANCE          equ  100203
GLU_DISPLAY_MODE                equ  100204
GLU_PARAMETRIC_TOLERANCE        equ  100202
GLU_SAMPLING_METHOD             equ  100205
GLU_U_STEP                      equ  100206
GLU_V_STEP                      equ  100207
GLU_PATH_LENGTH                 equ  100215
GLU_PARAMETRIC_ERROR            equ  100216
GLU_DOMAIN_DISTANCE             equ  100217
GLU_MAP1_TRIM_2                 equ  100210
GLU_MAP1_TRIM_3                 equ  100211
GLU_OUTLINE_POLYGON             equ  100240
GLU_OUTLINE_PATCH               equ  100241
GLU_NURBS_ERROR1                equ  100251
GLU_NURBS_ERROR2                equ  100252
GLU_NURBS_ERROR3                equ  100253
GLU_NURBS_ERROR4                equ  100254
GLU_NURBS_ERROR5                equ  100255
GLU_NURBS_ERROR6                equ  100256
GLU_NURBS_ERROR7                equ  100257
GLU_NURBS_ERROR8                equ  100258
GLU_NURBS_ERROR9                equ  100259
GLU_NURBS_ERROR10               equ  100260
GLU_NURBS_ERROR11               equ  100261
GLU_NURBS_ERROR12               equ  100262
GLU_NURBS_ERROR13               equ  100263
GLU_NURBS_ERROR14               equ  100264
GLU_NURBS_ERROR15               equ  100265
GLU_NURBS_ERROR16               equ  100266
GLU_NURBS_ERROR17               equ  100267
GLU_NURBS_ERROR18               equ  100268
GLU_NURBS_ERROR19               equ  100269
GLU_NURBS_ERROR20               equ  100270
GLU_NURBS_ERROR21               equ  100271
GLU_NURBS_ERROR22               equ  100272
GLU_NURBS_ERROR23               equ  100273
GLU_NURBS_ERROR24               equ  100274
GLU_NURBS_ERROR25               equ  100275
GLU_NURBS_ERROR26               equ  100276
GLU_NURBS_ERROR27               equ  100277
GLU_NURBS_ERROR28               equ  100278
GLU_NURBS_ERROR29               equ  100279
GLU_NURBS_ERROR30               equ  100280
GLU_NURBS_ERROR31               equ  100281
GLU_NURBS_ERROR32               equ  100282
GLU_NURBS_ERROR33               equ  100283
GLU_NURBS_ERROR34               equ  100284
GLU_NURBS_ERROR35               equ  100285
GLU_NURBS_ERROR36               equ  100286
GLU_NURBS_ERROR37               equ  100287
GLU_CW                          equ  100120
GLU_CCW                         equ  100121
GLU_INTERIOR                    equ  100122
GLU_EXTERIOR                    equ  100123
GLU_UNKNOWN                     equ  100124
GLU_BEGIN                       equ  GLU_TESS_BEGIN
GLU_VERTEX                      equ  GLU_TESS_VERTEX
GLU_END                         equ  GLU_TESS_END
GLU_ERROR                       equ  GLU_TESS_ERROR
GLU_EDGE_FLAG                   equ  GLU_TESS_EDGE_FLAG

; ---------------------------- ShellAPI.h -------------------------------

ABM_NEW                 equ 0
ABM_REMOVE              equ 1
ABM_QUERYPOS            equ 2
ABM_SETPOS              equ 3
ABM_GETSTATE            equ 4
ABM_GETTASKBARPOS       equ 5
ABM_ACTIVATE            equ 6 
ABM_GETAUTOHIDEBAR      equ 7
ABM_SETAUTOHIDEBAR      equ 8 
ABM_WINDOWPOSCHANGED    equ 9
ABN_STATECHANGE         equ 0
ABN_POSCHANGED          equ 1
ABN_FULLSCREENAPP       equ 2
ABN_WINDOWARRANGE       equ 3 
ABS_AUTOHIDE            equ 1
ABS_ALWAYSONTOP         equ 2
ABE_LEFT                equ 0
ABE_TOP                 equ 1
ABE_RIGHT               equ 2
ABE_BOTTOM              equ 3
FO_MOVE                 equ 1
FO_COPY                 equ 2
FO_DELETE               equ 3
FO_RENAME               equ 4
FOF_MULTIDESTFILES      equ 1
FOF_CONFIRMMOUSE        equ 2
FOF_SILENT              equ 4 
FOF_RENAMEONCOLLISION   equ 8
FOF_NOCONFIRMATION      equ 10h 
FOF_WANTMAPPINGHANDLE   equ 20h 
FOF_ALLOWUNDO           equ 40h
FOF_FILESONLY           equ 80h 
FOF_SIMPLEPROGRESS      equ 100h 
FOF_NOCONFIRMMKDIR      equ 200h 
FOF_NOERRORUI           equ 400h 
FOF_NOCOPYSECURITYATTRIBS equ 800h 
PO_DELETE               equ 13h 
PO_RENAME               equ 14h 
PO_PORTCHANGE           equ 20h 
PO_REN_PORT             equ 34h 
SE_ERR_FNF              equ 2 
SE_ERR_PNF              equ 3 
SE_ERR_ACCESSDENIED     equ 5 
SE_ERR_OOM              equ 8 
SE_ERR_DLLNOTFOUND      equ 32
SE_ERR_SHARE            equ 26
SE_ERR_ASSOCINCOMPLETE  equ 27
SE_ERR_DDETIMEOUT       equ 28
SE_ERR_DDEFAIL          equ 29
SE_ERR_DDEBUSY          equ 30
SE_ERR_NOASSOC          equ 31
SEE_MASK_CLASSNAME      equ 1
SEE_MASK_CLASSKEY       equ 3
SEE_MASK_IDLIST         equ 4
SEE_MASK_INVOKEIDLIST   equ 0ch
SEE_MASK_ICON           equ 10h
SEE_MASK_HOTKEY         equ 20h
SEE_MASK_NOCLOSEPROCESS equ 40h
SEE_MASK_CONNECTNETDRV  equ 80h
SEE_MASK_FLAG_DDEWAIT   equ 100h
SEE_MASK_DOENVSUBST     equ 200h
SEE_MASK_FLAG_NO_UI     equ 400h
SEE_MASK_UNICODE        equ 4000h
SEE_MASK_NO_CONSOLE     equ 8000h
SEE_MASK_ASYNCOK        equ 100000h
SEE_MASK_HMONITOR       equ 200000h
SEE_MASK_CLASSNAME      equ 1
SEE_MASK_CLASSKEY       equ 3
SEE_MASK_IDLIST         equ 4
SEE_MASK_INVOKEIDLIST   equ 0ch
SEE_MASK_ICON           equ 10h
SEE_MASK_HOTKEY         equ 20h
SEE_MASK_NOCLOSEPROCESS equ 40h
SEE_MASK_CONNECTNETDRV  equ 80h
SEE_MASK_FLAG_DDEWAIT   equ 100h
SEE_MASK_DOENVSUBST     equ 200h
SEE_MASK_FLAG_NO_UI     equ 400h
SEE_MASK_UNICODE        equ 4000h
SEE_MASK_NO_CONSOLE     equ 8000h
SEE_MASK_ASYNCOK        equ 100000h
SEE_MASK_HMONITOR       equ 200000h
SHERB_NOCONFIRMATION    equ 1
SHERB_NOPROGRESSUI      equ 2
SHERB_NOSOUND           equ 4
NIM_ADD                 equ 0
NIM_MODIFY              equ 1
NIM_DELETE              equ 2
NIF_MESSAGE             equ 1
NIF_ICON                equ 2
NIF_TIP                 equ 4
SHGFI_ICON              equ 100h 
SHGFI_DISPLAYNAME       equ 200h 
SHGFI_TYPENAME          equ 400h 
SHGFI_ATTRIBUTES        equ 800h 
SHGFI_ICONLOCATION      equ 1000h 
SHGFI_EXETYPE           equ 2000h 
SHGFI_SYSICONINDEX      equ 4000h 
SHGFI_LINKOVERLAY       equ 8000h 
SHGFI_SELECTED          equ 10000h 
SHGFI_ATTR_SPECIFIED    equ 20000h 
SHGFI_LARGEICON         equ 0 
SHGFI_SMALLICON         equ 1 
SHGFI_OPENICON          equ 2 
SHGFI_SHELLICONSIZE     equ 4 
SHGFI_PIDL              equ 8 
SHGFI_USEFILEATTRIBUTES equ 10h 
SHGNLI_PIDL             equ 1 
SHGNLI_PREFIXNAME       equ 2 
SHGNLI_NOUNIQUE         equ 4 
PRINTACTION_OPEN        equ 0
PRINTACTION_PROPERTIES          equ 1
PRINTACTION_NETINSTALL          equ 2
PRINTACTION_NETINSTALLLINK      equ 3
PRINTACTION_TESTPAGE            equ 4
PRINTACTION_OPENNETPRN          equ 5
PRINTACTION_DOCUMENTDEFAULTS    equ 6
PRINTACTION_SERVERPROPERTIES    equ 7
 
SHQUERYRBINFO STRUCT
 cbSize         dd ?
 i64Size        qword ?
 i64NumItems    qword ?
 SHQUERYRBINFO ENDS

;wininet.h

INTERNET_INVALID_PORT_NUMBER            equ 0 
INTERNET_DEFAULT_FTP_PORT               equ 21 
INTERNET_DEFAULT_GOPHER_PORT            equ 70 
INTERNET_DEFAULT_HTTP_PORT              equ 80 
INTERNET_DEFAULT_HTTPS_PORT             equ 443 
INTERNET_DEFAULT_SOCKS_PORT             equ 1080 
INTERNET_MAX_HOST_NAME_LENGTH           equ 256
INTERNET_MAX_USER_NAME_LENGTH           equ 128
INTERNET_MAX_PASSWORD_LENGTH            equ 128
INTERNET_MAX_PORT_NUMBER_LENGTH         equ 5 
INTERNET_MAX_PORT_NUMBER_VALUE          equ 65535 
INTERNET_MAX_PATH_LENGTH                equ 2048
INTERNET_MAX_SCHEME_LENGTH              equ 32 
INTERNET_MAX_URL_LENGTH                 equ INTERNET_MAX_SCHEME_LENGTH + 3 + INTERNET_MAX_PATH_LENGTH
INTERNET_KEEP_ALIVE_UNKNOWN             equ 0ffffffffh
INTERNET_KEEP_ALIVE_ENABLED             equ 1
INTERNET_KEEP_ALIVE_DISABLED            equ 0
INTERNET_REQFLAG_FROM_CACHE             equ 1 
INTERNET_REQFLAG_ASYNC                  equ 2 
INTERNET_REQFLAG_VIA_PROXY              equ 4 
INTERNET_REQFLAG_NO_HEADERS             equ 8 
INTERNET_REQFLAG_PASSIVE                equ 10h 
INTERNET_REQFLAG_CACHE_WRITE_DISABLED   equ 40h 
INTERNET_FLAG_RELOAD                    equ 80000000h 
INTERNET_FLAG_RAW_DATA                  equ 40000000h 
INTERNET_FLAG_EXISTING_CONNECT          equ 20000000h 
INTERNET_FLAG_ASYNC                     equ 10000000h 
INTERNET_FLAG_PASSIVE                   equ 8000000h 
INTERNET_FLAG_NO_CACHE_WRITE            equ 4000000h 
INTERNET_FLAG_DONT_CACHE equ INTERNET_FLAG_NO_CACHE_WRITE
INTERNET_FLAG_MAKE_PERSISTENT           equ 2000000h 
INTERNET_FLAG_FROM_CACHE                equ 1000000h 
INTERNET_FLAG_OFFLINE                   equ INTERNET_FLAG_FROM_CACHE
INTERNET_FLAG_SECURE                    equ 800000h 
INTERNET_FLAG_KEEP_CONNECTION           equ 400000h 
INTERNET_FLAG_NO_AUTO_REDIRECT          equ 200000h 
INTERNET_FLAG_READ_PREFETCH             equ 100000h 
INTERNET_FLAG_NO_COOKIES                equ 80000h 
INTERNET_FLAG_NO_AUTH                   equ 40000h 
INTERNET_FLAG_CACHE_IF_NET_FAIL         equ 10000h 
INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP   equ 8000h 
INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS  equ 4000h 
INTERNET_FLAG_IGNORE_CERT_DATE_INVALID  equ 2000h 
INTERNET_FLAG_IGNORE_CERT_CN_INVALID    equ 1000h 
INTERNET_FLAG_RESYNCHRONIZE             equ 800h 
INTERNET_FLAG_HYPERLINK                 equ 400h 
INTERNET_FLAG_NO_UI                     equ 200h 
INTERNET_FLAG_PRAGMA_NOCACHE            equ 100h 
INTERNET_FLAG_CACHE_ASYNC               equ 80h 
INTERNET_FLAG_FORMS_SUBMIT              equ 40h 
INTERNET_FLAG_NEED_FILE                 equ 10h 
INTERNET_FLAG_MUST_CACHE_REQUEST    equ INTERNET_FLAG_NEED_FILE
INTERNET_FLAG_TRANSFER_ASCII        equ FTP_TRANSFER_TYPE_ASCII 
INTERNET_FLAG_TRANSFER_BINARY       equ FTP_TRANSFER_TYPE_BINARY 
SECURITY_INTERNET_MASK              equ INTERNET_FLAG_IGNORE_CERT_CN_INVALID + INTERNET_FLAG_IGNORE_CERT_DATE_INVALID + INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS + INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP 
INTERNET_ERROR_MASK_INSERT_CDROM    equ 1
INTERNET_OPTIONS_MASK               equ not INTERNET_FLAGS_MASK
WININET_API_FLAG_ASYNC equ 1 
WININET_API_FLAG_SYNC equ 4 
WININET_API_FLAG_USE_CONTEXT    equ 8 
INTERNET_NO_CALLBACK            equ 0
INTERNET_RFC1123_FORMAT         equ 0
INTERNET_RFC1123_BUFSIZE        equ 30
ICU_ESCAPE      equ 80000000h 
ICU_USERNAME    equ 40000000h 
ICU_NO_ENCODE   equ 20000000h 
ICU_DECODE      equ 10000000h 
ICU_NO_META     equ 8000000h 
ICU_ENCODE_SPACES_ONLY      equ 4000000h 
ICU_BROWSER_MODE            equ 2000000h 
INTERNET_OPEN_TYPE_PRECONFIG equ 0 
INTERNET_OPEN_TYPE_DIRECT   equ 1 
INTERNET_OPEN_TYPE_PROXY    equ 3 
INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY equ 4 
PRE_CONFIG_INTERNET_ACCESS  equ INTERNET_OPEN_TYPE_PRECONFIG
LOCAL_INTERNET_ACCESS       equ INTERNET_OPEN_TYPE_DIRECT
CERN_PROXY_INTERNET_ACCESS  equ INTERNET_OPEN_TYPE_PROXY
INTERNET_SERVICE_URL    equ 0
INTERNET_SERVICE_FTP    equ 1
INTERNET_SERVICE_GOPHER equ 2
INTERNET_SERVICE_HTTP   equ 3
IRF_ASYNC               equ WININET_API_FLAG_ASYNC
IRF_SYNC                equ WININET_API_FLAG_SYNC
IRF_USE_CONTEXT         equ WININET_API_FLAG_USE_CONTEXT
IRF_NO_WAIT             equ 8
ISO_GLOBAL              equ 1 
ISO_REGISTRY            equ 2 
ISO_VALID_FLAGS                         equ (ISO_GLOBAL + ISO_REGISTRY)
INTERNET_OPTION_CALLBACK                equ 1
INTERNET_OPTION_CONNECT_TIMEOUT         equ 2
INTERNET_OPTION_CONNECT_RETRIES         equ 3
INTERNET_OPTION_CONNECT_BACKOFF         equ 4
INTERNET_OPTION_SEND_TIMEOUT            equ 5
INTERNET_OPTION_CONTROL_SEND_TIMEOUT    equ INTERNET_OPTION_SEND_TIMEOUT
INTERNET_OPTION_RECEIVE_TIMEOUT         equ 6
INTERNET_OPTION_CONTROL_RECEIVE_TIMEOUT equ INTERNET_OPTION_RECEIVE_TIMEOUT
INTERNET_OPTION_DATA_SEND_TIMEOUT       equ 7
INTERNET_OPTION_DATA_RECEIVE_TIMEOUT    equ 8
INTERNET_OPTION_HANDLE_TYPE             equ 9
INTERNET_OPTION_LISTEN_TIMEOUT          equ 11
INTERNET_OPTION_READ_BUFFER_SIZE        equ 12
INTERNET_OPTION_WRITE_BUFFER_SIZE       equ 13
INTERNET_OPTION_ASYNC_ID                equ 15
INTERNET_OPTION_ASYNC_PRIORITY          equ 16
INTERNET_OPTION_PARENT_HANDLE           equ 21
INTERNET_OPTION_KEEP_CONNECTION         equ 22
INTERNET_OPTION_REQUEST_FLAGS           equ 23
INTERNET_OPTION_EXTENDED_ERROR          equ 24
INTERNET_OPTION_OFFLINE_MODE            equ 26
INTERNET_OPTION_CACHE_STREAM_HANDLE     equ 27
INTERNET_OPTION_USERNAME                equ 28
INTERNET_OPTION_PASSWORD                equ 29
INTERNET_OPTION_ASYNC                   equ 30
INTERNET_OPTION_SECURITY_FLAGS          equ 31
INTERNET_OPTION_SECURITY_CERTIFICATE_STRUCT equ 32
INTERNET_OPTION_DATAFILE_NAME           equ 33
INTERNET_OPTION_URL                     equ 34
INTERNET_OPTION_SECURITY_CERTIFICATE    equ 35
INTERNET_OPTION_SECURITY_KEY_BITNESS    equ 36
INTERNET_OPTION_REFRESH                 equ 37
INTERNET_OPTION_PROXY                   equ 38
INTERNET_OPTION_SETTINGS_CHANGED        equ 39
INTERNET_OPTION_VERSION                 equ 40
INTERNET_OPTION_USER_AGENT              equ 41
INTERNET_OPTION_END_BROWSER_SESSION     equ 42
INTERNET_OPTION_PROXY_USERNAME          equ 43
INTERNET_OPTION_PROXY_PASSWORD          equ 44
INTERNET_OPTION_CONTEXT_VALUE           equ 45
INTERNET_OPTION_CONNECT_LIMIT           equ 46
INTERNET_OPTION_SECURITY_SELECT_CLIENT_CERT equ 47
INTERNET_OPTION_POLICY                  equ 48
INTERNET_OPTION_DISCONNECTED_TIMEOUT    equ 49
INTERNET_OPTION_CONNECTED_STATE         equ 50
INTERNET_OPTION_IDLE_STATE              equ 51
INTERNET_OPTION_OFFLINE_SEMANTICS       equ 52
INTERNET_OPTION_SECONDARY_CACHE_KEY     equ 53
INTERNET_OPTION_CALLBACK_FILTER         equ 54
INTERNET_OPTION_CONNECT_TIME            equ 55
INTERNET_OPTION_SEND_THROUGHPUT         equ 56
INTERNET_OPTION_RECEIVE_THROUGHPUT      equ 57
INTERNET_OPTION_REQUEST_PRIORITY        equ 58
INTERNET_OPTION_HTTP_VERSION            equ 59
INTERNET_OPTION_RESET_URLCACHE_SESSION  equ 60
INTERNET_OPTION_ERROR_MASK              equ 62
INTERNET_FIRST_OPTION                   equ INTERNET_OPTION_CALLBACK
INTERNET_LAST_OPTION                    equ INTERNET_OPTION_ERROR_MASK
INTERNET_PRIORITY_FOREGROUND            equ 1000
INTERNET_HANDLE_TYPE_INTERNET           equ 1
INTERNET_HANDLE_TYPE_CONNECT_FTP        equ 2
INTERNET_HANDLE_TYPE_CONNECT_GOPHER     equ 3
INTERNET_HANDLE_TYPE_CONNECT_HTTP       equ 4
INTERNET_HANDLE_TYPE_FTP_FIND           equ 5
INTERNET_HANDLE_TYPE_FTP_FIND_HTML      equ 6
INTERNET_HANDLE_TYPE_FTP_FILE           equ 7
INTERNET_HANDLE_TYPE_FTP_FILE_HTML      equ 8
INTERNET_HANDLE_TYPE_GOPHER_FIND        equ 9
INTERNET_HANDLE_TYPE_GOPHER_FIND_HTML   equ 10
INTERNET_HANDLE_TYPE_GOPHER_FILE        equ 11
INTERNET_HANDLE_TYPE_GOPHER_FILE_HTML   equ 12
INTERNET_HANDLE_TYPE_HTTP_REQUEST       equ 13
SECURITY_FLAG_SECURE            equ 1 
SECURITY_FLAG_STRENGTH_WEAK     equ 10000000h
SECURITY_FLAG_STRENGTH_MEDIUM   equ 40000000h
SECURITY_FLAG_STRENGTH_STRONG   equ 20000000h
SECURITY_FLAG_UNKNOWNBIT        equ 80000000h
SECURITY_FLAG_NORMALBITNESS     equ SECURITY_FLAG_STRENGTH_WEAK
SECURITY_FLAG_SSL       equ 2
SECURITY_FLAG_SSL3      equ 4
SECURITY_FLAG_PCT       equ 8
SECURITY_FLAG_PCT4      equ 10h
SECURITY_FLAG_IETFSSL4  equ 20h
SECURITY_FLAG_40BIT     equ SECURITY_FLAG_STRENGTH_WEAK
SECURITY_FLAG_128BIT    equ SECURITY_FLAG_STRENGTH_STRONG
SECURITY_FLAG_56BIT     equ SECURITY_FLAG_STRENGTH_MEDIUM
SECURITY_FLAG_IGNORE_REVOCATION         equ 80h
SECURITY_FLAG_IGNORE_UNKNOWN_CA         equ 100h
SECURITY_FLAG_IGNORE_WRONG_USAGE        equ 200h
SECURITY_FLAG_IGNORE_CERT_CN_INVALID    equ INTERNET_FLAG_IGNORE_CERT_CN_INVALID
SECURITY_FLAG_IGNORE_CERT_DATE_INVALID  equ INTERNET_FLAG_IGNORE_CERT_DATE_INVALID
SECURITY_FLAG_IGNORE_REDIRECT_TO_HTTPS  equ INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS
SECURITY_FLAG_IGNORE_REDIRECT_TO_HTTP   equ INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP
SECURITY_SET_MASK equ (SECURITY_FLAG_IGNORE_REVOCATION +\
    SECURITY_FLAG_IGNORE_UNKNOWN_CA +\
    SECURITY_FLAG_IGNORE_CERT_CN_INVALID +\
    SECURITY_FLAG_IGNORE_CERT_DATE_INVALID +\
    SECURITY_FLAG_IGNORE_WRONG_USAGE)

INTERNET_STATUS_RESOLVING_NAME          equ 10
INTERNET_STATUS_NAME_RESOLVED           equ 11
INTERNET_STATUS_CONNECTING_TO_SERVER    equ 20
INTERNET_STATUS_CONNECTED_TO_SERVER     equ 21
INTERNET_STATUS_SENDING_REQUEST         equ 30
INTERNET_STATUS_REQUEST_SENT            equ 31
INTERNET_STATUS_RECEIVING_RESPONSE      equ 40
INTERNET_STATUS_RESPONSE_RECEIVED       equ 41
INTERNET_STATUS_CTL_RESPONSE_RECEIVED   equ 42
INTERNET_STATUS_PREFETCH                equ 43
INTERNET_STATUS_CLOSING_CONNECTION      equ 50
INTERNET_STATUS_CONNECTION_CLOSED       equ 51
INTERNET_STATUS_HANDLE_CREATED          equ 60
INTERNET_STATUS_HANDLE_CLOSING          equ 70
INTERNET_STATUS_REQUEST_COMPLETE        equ 100
INTERNET_STATUS_REDIRECT                equ 110
INTERNET_STATUS_INTERMEDIATE_RESPONSE   equ 120
INTERNET_STATUS_STATE_CHANGE            equ 200
INTERNET_STATE_CONNECTED                equ 1 
INTERNET_STATE_DISCONNECTED             equ 2 
INTERNET_STATE_DISCONNECTED_BY_USER     equ 10h 
INTERNET_STATE_IDLE                     equ 100h 
INTERNET_STATE_BUSY                     equ 200h 
INTERNET_INVALID_STATUS_CALLBACK equ 0ffffffffh
FTP_TRANSFER_TYPE_UNKNOWN       equ 0
FTP_TRANSFER_TYPE_ASCII         equ 1
FTP_TRANSFER_TYPE_BINARY        equ 2
FTP_TRANSFER_TYPE_MASK          equ (FTP_TRANSFER_TYPE_ASCII + FTP_TRANSFER_TYPE_BINARY)
MAX_GOPHER_DISPLAY_TEXT         equ 128
MAX_GOPHER_SELECTOR_TEXT        equ 256
MAX_GOPHER_HOST_NAME            equ INTERNET_MAX_HOST_NAME_LENGTH
MAX_GOPHER_LOCATOR_LENGTH       equ 1+ MAX_GOPHER_DISPLAY_TEXT+ 1 + MAX_GOPHER_SELECTOR_TEXT+ 1+ MAX_GOPHER_HOST_NAME+ 1+ INTERNET_MAX_PORT_NUMBER_LENGTH+ 1+ 1+ 2
GOPHER_TYPE_TEXT_FILE           equ 1
GOPHER_TYPE_DIRECTORY           equ 2
GOPHER_TYPE_CSO                 equ 4
GOPHER_TYPE_ERROR               equ 8
GOPHER_TYPE_MAC_BINHEX          equ 10h
GOPHER_TYPE_DOS_ARCHIVE         equ 20h
GOPHER_TYPE_UNIX_UUENCODED      equ 40h
GOPHER_TYPE_INDEX_SERVER        equ 80h
GOPHER_TYPE_TELNET              equ 100h
GOPHER_TYPE_BINARY              equ 200h
GOPHER_TYPE_REDUNDANT           equ 400h
GOPHER_TYPE_TN3270              equ 800h
GOPHER_TYPE_GIF                 equ 1000h
GOPHER_TYPE_IMAGE               equ 2000h
GOPHER_TYPE_BITMAP              equ 4000h
GOPHER_TYPE_MOVIE               equ 8000h
GOPHER_TYPE_SOUND               equ 10000h
GOPHER_TYPE_HTML                equ 20000h
GOPHER_TYPE_PDF                 equ 40000h
GOPHER_TYPE_CALENDAR            equ 80000h
GOPHER_TYPE_INLINE              equ 100000h
GOPHER_TYPE_UNKNOWN             equ 20000000h
GOPHER_TYPE_ASK                 equ 40000000h
GOPHER_TYPE_GOPHER_PLUS         equ 80000000h
GOPHER_TEMP                     equ GOPHER_TYPE_TEXT_FILE+GOPHER_TYPE_MAC_BINHEX+GOPHER_TYPE_DOS_ARCHIVE+GOPHER_TYPE_UNIX_UUENCODED+GOPHER_TYPE_BINARY
GOPHER_TYPE_FILE_MASK           equ GOPHER_TEMP+ GOPHER_TYPE_GIF+ GOPHER_TYPE_IMAGE+ GOPHER_TYPE_BITMAP+GOPHER_TYPE_MOVIE+GOPHER_TYPE_SOUND+ GOPHER_TYPE_HTML+GOPHER_TYPE_PDF+GOPHER_TYPE_CALENDAR+GOPHER_TYPE_INLINE
MAX_GOPHER_CATEGORY_NAME        equ 128 
MAX_GOPHER_ATTRIBUTE_NAME       equ 128 
MIN_GOPHER_ATTRIBUTE_LENGTH     equ 256 
GOPHER_INFO_CATEGORY            equ "+INFO"
GOPHER_ADMIN_CATEGORY           equ "+ADMIN"
GOPHER_VIEWS_CATEGORY           equ "+VIEWS"
GOPHER_ABSTRACT_CATEGORY        equ "+ABSTRACT"
GOPHER_VERONICA_CATEGORY        equ "+VERONICA"
GOPHER_ADMIN_ATTRIBUTE          equ "Admin"
GOPHER_MOD_DATE_ATTRIBUTE       equ "Mod-Date"
GOPHER_TTL_ATTRIBUTE            equ "TTL"
GOPHER_SCORE_ATTRIBUTE          equ "Score"
GOPHER_RANGE_ATTRIBUTE          equ "Score-range"
GOPHER_SITE_ATTRIBUTE           equ "Site"
GOPHER_ORG_ATTRIBUTE            equ "Org"

GOPHER_LOCATION_ATTRIBUTE       equ "Loc"
GOPHER_GEOG_ATTRIBUTE           equ "Geog"
GOPHER_TIMEZONE_ATTRIBUTE       equ "TZ"
GOPHER_PROVIDER_ATTRIBUTE       equ "Provider"
GOPHER_VERSION_ATTRIBUTE        equ "Version"
GOPHER_ABSTRACT_ATTRIBUTE       equ "Abstract"
GOPHER_VIEW_ATTRIBUTE           equ "View"
GOPHER_TREEWALK_ATTRIBUTE       equ "treewalk"
GOPHER_ATTRIBUTE_ID_BASE        equ 0abcccc00h
GOPHER_CATEGORY_ID_ALL          equ (GOPHER_ATTRIBUTE_ID_BASE + 1)
GOPHER_CATEGORY_ID_INFO         equ (GOPHER_ATTRIBUTE_ID_BASE + 2)
GOPHER_CATEGORY_ID_ADMIN        equ (GOPHER_ATTRIBUTE_ID_BASE + 3)
GOPHER_CATEGORY_ID_VIEWS        equ (GOPHER_ATTRIBUTE_ID_BASE + 4)
GOPHER_CATEGORY_ID_ABSTRACT     equ (GOPHER_ATTRIBUTE_ID_BASE + 5)
GOPHER_CATEGORY_ID_VERONICA     equ (GOPHER_ATTRIBUTE_ID_BASE + 6)
GOPHER_CATEGORY_ID_ASK          equ (GOPHER_ATTRIBUTE_ID_BASE + 7)
GOPHER_CATEGORY_ID_UNKNOWN      equ (GOPHER_ATTRIBUTE_ID_BASE + 8)
GOPHER_ATTRIBUTE_ID_ALL         equ (GOPHER_ATTRIBUTE_ID_BASE + 9)
GOPHER_ATTRIBUTE_ID_ADMIN       equ (GOPHER_ATTRIBUTE_ID_BASE + 10)
GOPHER_ATTRIBUTE_ID_MOD_DATE    equ (GOPHER_ATTRIBUTE_ID_BASE + 11)
GOPHER_ATTRIBUTE_ID_TTL         equ (GOPHER_ATTRIBUTE_ID_BASE + 12)
GOPHER_ATTRIBUTE_ID_SCORE       equ (GOPHER_ATTRIBUTE_ID_BASE + 13)
GOPHER_ATTRIBUTE_ID_RANGE       equ (GOPHER_ATTRIBUTE_ID_BASE + 14)
GOPHER_ATTRIBUTE_ID_SITE        equ (GOPHER_ATTRIBUTE_ID_BASE + 15)
GOPHER_ATTRIBUTE_ID_ORG         equ (GOPHER_ATTRIBUTE_ID_BASE + 16)
GOPHER_ATTRIBUTE_ID_LOCATION    equ (GOPHER_ATTRIBUTE_ID_BASE + 17)
GOPHER_ATTRIBUTE_ID_GEOG        equ (GOPHER_ATTRIBUTE_ID_BASE + 18)
GOPHER_ATTRIBUTE_ID_TIMEZONE    equ (GOPHER_ATTRIBUTE_ID_BASE + 19)
GOPHER_ATTRIBUTE_ID_PROVIDER    equ (GOPHER_ATTRIBUTE_ID_BASE + 20)
GOPHER_ATTRIBUTE_ID_VERSION     equ (GOPHER_ATTRIBUTE_ID_BASE + 21)
GOPHER_ATTRIBUTE_ID_ABSTRACT    equ (GOPHER_ATTRIBUTE_ID_BASE + 22)
GOPHER_ATTRIBUTE_ID_VIEW        equ (GOPHER_ATTRIBUTE_ID_BASE + 23)
GOPHER_ATTRIBUTE_ID_TREEWALK    equ (GOPHER_ATTRIBUTE_ID_BASE + 24)
GOPHER_ATTRIBUTE_ID_UNKNOWN     equ (GOPHER_ATTRIBUTE_ID_BASE + 25)
HTTP_MAJOR_VERSION              equ 1
HTTP_MINOR_VERSION              equ 0
HTTP_VERSION                    equ "HTTP/1.0"
HTTP_QUERY_MIME_VERSION         equ 0
HTTP_QUERY_CONTENT_TYPE         equ 1
HTTP_QUERY_CONTENT_TRANSFER_ENCODING equ 2
HTTP_QUERY_CONTENT_ID           equ 3
HTTP_QUERY_CONTENT_DESCRIPTION  equ 4
HTTP_QUERY_CONTENT_LENGTH       equ 5
HTTP_QUERY_CONTENT_LANGUAGE     equ 6
HTTP_QUERY_ALLOW                equ 7
HTTP_QUERY_PUBLIC               equ 8
HTTP_QUERY_DATE                 equ 9
HTTP_QUERY_EXPIRES              equ 10
HTTP_QUERY_LAST_MODIFIED        equ 11
HTTP_QUERY_MESSAGE_ID           equ 12
HTTP_QUERY_URI                  equ 13
HTTP_QUERY_DERIVED_FROM         equ 14
HTTP_QUERY_COST                 equ 15
HTTP_QUERY_LINK                 equ 16
HTTP_QUERY_PRAGMA               equ 17
HTTP_QUERY_VERSION              equ 18 
HTTP_QUERY_STATUS_CODE          equ 19 
HTTP_QUERY_STATUS_TEXT          equ 20 
HTTP_QUERY_RAW_HEADERS          equ 21 
HTTP_QUERY_RAW_HEADERS_CRLF     equ 22 
HTTP_QUERY_CONNECTION           equ 23
HTTP_QUERY_ACCEPT               equ 24
HTTP_QUERY_ACCEPT_CHARSET       equ 25
HTTP_QUERY_ACCEPT_ENCODING      equ 26
HTTP_QUERY_ACCEPT_LANGUAGE      equ 27
HTTP_QUERY_AUTHORIZATION        equ 28
HTTP_QUERY_CONTENT_ENCODING     equ 29
HTTP_QUERY_FORWARDED            equ 30
HTTP_QUERY_FROM                 equ 31
HTTP_QUERY_IF_MODIFIED_SINCE    equ 32
HTTP_QUERY_LOCATION             equ 33
HTTP_QUERY_ORIG_URI             equ 34
HTTP_QUERY_REFERER              equ 35
HTTP_QUERY_RETRY_AFTER          equ 36
HTTP_QUERY_SERVER               equ 37
HTTP_QUERY_TITLE                equ 38
HTTP_QUERY_USER_AGENT           equ 39
HTTP_QUERY_WWW_AUTHENTICATE     equ 40
HTTP_QUERY_PROXY_AUTHENTICATE   equ 41
HTTP_QUERY_ACCEPT_RANGES        equ 42
HTTP_QUERY_SET_COOKIE           equ 43
HTTP_QUERY_COOKIE               equ 44
HTTP_QUERY_REQUEST_METHOD       equ 45 

HTTP_QUERY_REFRESH              equ 46
HTTP_QUERY_CONTENT_DISPOSITION  equ 47
HTTP_QUERY_AGE                  equ 48
HTTP_QUERY_CACHE_CONTROL    equ 49
HTTP_QUERY_CONTENT_BASE     equ 50
HTTP_QUERY_CONTENT_LOCATION equ 51
HTTP_QUERY_CONTENT_MD5      equ 52
HTTP_QUERY_CONTENT_RANGE    equ 53
HTTP_QUERY_ETAG             equ 54
HTTP_QUERY_HOST             equ 55
HTTP_QUERY_IF_MATCH         equ 56
HTTP_QUERY_IF_NONE_MATCH    equ 57
HTTP_QUERY_IF_RANGE         equ 58
HTTP_QUERY_IF_UNMODIFIED_SINCE equ 59
HTTP_QUERY_MAX_FORWARDS     equ 60
HTTP_QUERY_PROXY_AUTHORIZATION equ 61
HTTP_QUERY_RANGE            equ 62
HTTP_QUERY_TRANSFER_ENCODING equ 63
HTTP_QUERY_UPGRADE          equ 64
HTTP_QUERY_VARY             equ 65
HTTP_QUERY_VIA              equ 66
HTTP_QUERY_WARNING          equ 67
HTTP_QUERY_MAX              equ 67
HTTP_QUERY_CUSTOM           equ 65535
HTTP_QUERY_FLAG_REQUEST_HEADERS equ 80000000h
HTTP_QUERY_FLAG_SYSTEMTIME  equ 40000000h
HTTP_QUERY_FLAG_NUMBER      equ 20000000h
HTTP_QUERY_FLAG_COALESCE    equ 10000000h
HTTP_QUERY_MODIFIER_FLAGS_MASK equ (HTTP_QUERY_FLAG_REQUEST_HEADERS \
 + HTTP_QUERY_FLAG_SYSTEMTIME \
 + HTTP_QUERY_FLAG_NUMBER \
 + HTTP_QUERY_FLAG_COALESCE \
 )
HTTP_QUERY_HEADER_MASK      equ not HTTP_QUERY_MODIFIER_FLAGS_MASK
HTTP_STATUS_CONTINUE        equ 100 
HTTP_STATUS_SWITCH_PROTOCOLS equ 101 
HTTP_STATUS_OK              equ 200 
HTTP_STATUS_CREATED         equ 201 
HTTP_STATUS_ACCEPTED        equ 202 
HTTP_STATUS_PARTIAL         equ 203 
HTTP_STATUS_NO_CONTENT      equ 204 
HTTP_STATUS_RESET_CONTENT   equ 205 
HTTP_STATUS_PARTIAL_CONTENT equ 206 
HTTP_STATUS_AMBIGUOUS       equ 300 
HTTP_STATUS_MOVED           equ 301 
HTTP_STATUS_REDIRECT        equ 302 
HTTP_STATUS_REDIRECT_METHOD equ 303 
HTTP_STATUS_NOT_MODIFIED    equ 304 
HTTP_STATUS_USE_PROXY       equ 305 
HTTP_STATUS_REDIRECT_KEEP_VERB equ 307 
HTTP_STATUS_BAD_REQUEST     equ 400 
HTTP_STATUS_DENIED          equ 401 
HTTP_STATUS_PAYMENT_REQ     equ 402 
HTTP_STATUS_FORBIDDEN       equ 403 
HTTP_STATUS_NOT_FOUND       equ 404 
HTTP_STATUS_BAD_METHOD      equ 405 
HTTP_STATUS_NONE_ACCEPTABLE equ 406 
HTTP_STATUS_PROXY_AUTH_REQ  equ 407 
HTTP_STATUS_REQUEST_TIMEOUT equ 408 
HTTP_STATUS_CONFLICT        equ 409 
HTTP_STATUS_GONE            equ 410 
HTTP_STATUS_LENGTH_REQUIRED equ 411 
HTTP_STATUS_PRECOND_FAILED  equ 412 
HTTP_STATUS_REQUEST_TOO_LARGE equ 413 
HTTP_STATUS_URI_TOO_LONG    equ 414 
HTTP_STATUS_UNSUPPORTED_MEDIA equ 415 
HTTP_STATUS_SERVER_ERROR    equ 500 
HTTP_STATUS_NOT_SUPPORTED   equ 501 
HTTP_STATUS_BAD_GATEWAY     equ 502 
HTTP_STATUS_SERVICE_UNAVAIL equ 503 
HTTP_STATUS_GATEWAY_TIMEOUT equ 504 
HTTP_STATUS_VERSION_NOT_SUP equ 505 
HTTP_STATUS_FIRST           equ HTTP_STATUS_CONTINUE
HTTP_STATUS_LAST            equ HTTP_STATUS_VERSION_NOT_SUP
HTTP_ADDREQ_INDEX_MASK      equ 0FFFFh
HTTP_ADDREQ_FLAGS_MASK      equ 0FFFF0000h
HTTP_ADDREQ_FLAG_ADD_IF_NEW equ 10000000h
HTTP_ADDREQ_FLAG_ADD        equ 20000000h
HTTP_ADDREQ_FLAG_COALESCE_WITH_COMMA equ 40000000h
HTTP_ADDREQ_FLAG_COALESCE_WITH_SEMICOLON equ 1000000h
HTTP_ADDREQ_FLAG_COALESCE   equ HTTP_ADDREQ_FLAG_COALESCE_WITH_COMMA
HTTP_ADDREQ_FLAG_REPLACE    equ 80000000h
HSR_ASYNC       equ WININET_API_FLAG_ASYNC 
HSR_SYNC        equ WININET_API_FLAG_SYNC 
HSR_USE_CONTEXT equ WININET_API_FLAG_USE_CONTEXT 
HSR_INITIATE    equ 8 
HSR_DOWNLOAD    equ 10h 
HSR_CHUNKED     equ 20h 
FLAGS_ERROR_UI_FILTER_FOR_ERRORS            equ 1
FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS         equ 2
FLAGS_ERROR_UI_FLAGS_GENERATE_DATA          equ 4
FLAGS_ERROR_UI_FLAGS_NO_UI                  equ 8
FLAGS_ERROR_UI_SERIALIZE_DIALOGS            equ 10h
INTERNET_ERROR_BASE                         equ 12000
ERROR_INTERNET_OUT_OF_HANDLES               equ (INTERNET_ERROR_BASE + 1)
ERROR_INTERNET_TIMEOUT                      equ (INTERNET_ERROR_BASE + 2)
ERROR_INTERNET_EXTENDED_ERROR               equ (INTERNET_ERROR_BASE + 3)
ERROR_INTERNET_INTERNAL_ERROR               equ (INTERNET_ERROR_BASE + 4)
ERROR_INTERNET_INVALID_URL                  equ (INTERNET_ERROR_BASE + 5)
ERROR_INTERNET_UNRECOGNIZED_SCHEME          equ (INTERNET_ERROR_BASE + 6)
ERROR_INTERNET_NAME_NOT_RESOLVED            equ (INTERNET_ERROR_BASE + 7)
ERROR_INTERNET_PROTOCOL_NOT_FOUND           equ (INTERNET_ERROR_BASE + 8)
ERROR_INTERNET_INVALID_OPTION               equ (INTERNET_ERROR_BASE + 9)
ERROR_INTERNET_BAD_OPTION_LENGTH            equ (INTERNET_ERROR_BASE + 10)
ERROR_INTERNET_OPTION_NOT_SETTABLE          equ (INTERNET_ERROR_BASE + 11)
ERROR_INTERNET_SHUTDOWN                     equ (INTERNET_ERROR_BASE + 12)
ERROR_INTERNET_INCORRECT_USER_NAME          equ (INTERNET_ERROR_BASE + 13)
ERROR_INTERNET_INCORRECT_PASSWORD           equ (INTERNET_ERROR_BASE + 14)
ERROR_INTERNET_LOGIN_FAILURE                equ (INTERNET_ERROR_BASE + 15)
ERROR_INTERNET_INVALID_OPERATION            equ (INTERNET_ERROR_BASE + 16)
ERROR_INTERNET_OPERATION_CANCELLED          equ (INTERNET_ERROR_BASE + 17)
ERROR_INTERNET_INCORRECT_HANDLE_TYPE        equ (INTERNET_ERROR_BASE + 18)
ERROR_INTERNET_INCORRECT_HANDLE_STATE       equ (INTERNET_ERROR_BASE + 19)
ERROR_INTERNET_NOT_PROXY_REQUEST            equ (INTERNET_ERROR_BASE + 20)
ERROR_INTERNET_REGISTRY_VALUE_NOT_FOUND     equ (INTERNET_ERROR_BASE + 21)
ERROR_INTERNET_BAD_REGISTRY_PARAMETER       equ (INTERNET_ERROR_BASE + 22)
ERROR_INTERNET_NO_DIRECT_ACCESS             equ (INTERNET_ERROR_BASE + 23)
ERROR_INTERNET_NO_CONTEXT                   equ (INTERNET_ERROR_BASE + 24)
ERROR_INTERNET_NO_CALLBACK                  equ (INTERNET_ERROR_BASE + 25)
ERROR_INTERNET_REQUEST_PENDING              equ (INTERNET_ERROR_BASE + 26)
ERROR_INTERNET_INCORRECT_FORMAT             equ (INTERNET_ERROR_BASE + 27)
ERROR_INTERNET_ITEM_NOT_FOUND               equ (INTERNET_ERROR_BASE + 28)
ERROR_INTERNET_CANNOT_CONNECT               equ (INTERNET_ERROR_BASE + 29)
ERROR_INTERNET_CONNECTION_ABORTED           equ (INTERNET_ERROR_BASE + 30)
ERROR_INTERNET_CONNECTION_RESET             equ (INTERNET_ERROR_BASE + 31)
ERROR_INTERNET_FORCE_RETRY                  equ (INTERNET_ERROR_BASE + 32)
ERROR_INTERNET_INVALID_PROXY_REQUEST        equ (INTERNET_ERROR_BASE + 33)
ERROR_INTERNET_NEED_UI                      equ (INTERNET_ERROR_BASE + 34)
ERROR_INTERNET_HANDLE_EXISTS                equ (INTERNET_ERROR_BASE + 36)
ERROR_INTERNET_SEC_CERT_DATE_INVALID        equ (INTERNET_ERROR_BASE + 37)
ERROR_INTERNET_SEC_CERT_CN_INVALID          equ (INTERNET_ERROR_BASE + 38)
ERROR_INTERNET_HTTP_TO_HTTPS_ON_REDIR       equ (INTERNET_ERROR_BASE + 39)
ERROR_INTERNET_HTTPS_TO_HTTP_ON_REDIR       equ (INTERNET_ERROR_BASE + 40)
ERROR_INTERNET_MIXED_SECURITY               equ (INTERNET_ERROR_BASE + 41)
ERROR_INTERNET_CHG_POST_IS_NON_SECURE       equ (INTERNET_ERROR_BASE + 42)
ERROR_INTERNET_POST_IS_NON_SECURE           equ (INTERNET_ERROR_BASE + 43)
ERROR_INTERNET_CLIENT_AUTH_CERT_NEEDED      equ (INTERNET_ERROR_BASE + 44)
ERROR_INTERNET_INVALID_CA                   equ (INTERNET_ERROR_BASE + 45)
ERROR_INTERNET_CLIENT_AUTH_NOT_SETUP        equ (INTERNET_ERROR_BASE + 46)
ERROR_INTERNET_ASYNC_THREAD_FAILED          equ (INTERNET_ERROR_BASE + 47)
ERROR_INTERNET_REDIRECT_SCHEME_CHANGE       equ (INTERNET_ERROR_BASE + 48)
ERROR_INTERNET_DIALOG_PENDING               equ (INTERNET_ERROR_BASE + 49)
ERROR_INTERNET_RETRY_DIALOG                 equ (INTERNET_ERROR_BASE + 50)
ERROR_INTERNET_HTTPS_HTTP_SUBMIT_REDIR      equ (INTERNET_ERROR_BASE + 52)
ERROR_INTERNET_INSERT_CDROM                 equ (INTERNET_ERROR_BASE + 53)
ERROR_FTP_TRANSFER_IN_PROGRESS              equ (INTERNET_ERROR_BASE + 110)
ERROR_FTP_DROPPED                           equ (INTERNET_ERROR_BASE + 111)
ERROR_FTP_NO_PASSIVE_MODE                   equ (INTERNET_ERROR_BASE + 112)
ERROR_GOPHER_PROTOCOL_ERROR                 equ (INTERNET_ERROR_BASE + 130)
ERROR_GOPHER_NOT_FILE                       equ (INTERNET_ERROR_BASE + 131)
ERROR_GOPHER_DATA_ERROR                     equ (INTERNET_ERROR_BASE + 132)
ERROR_GOPHER_END_OF_DATA                    equ (INTERNET_ERROR_BASE + 133)
ERROR_GOPHER_INVALID_LOCATOR                equ (INTERNET_ERROR_BASE + 134)
ERROR_GOPHER_INCORRECT_LOCATOR_TYPE         equ (INTERNET_ERROR_BASE + 135)
ERROR_GOPHER_NOT_GOPHER_PLUS                equ (INTERNET_ERROR_BASE + 136)
ERROR_GOPHER_ATTRIBUTE_NOT_FOUND            equ (INTERNET_ERROR_BASE + 137)
ERROR_GOPHER_UNKNOWN_LOCATOR                equ (INTERNET_ERROR_BASE + 138)
ERROR_HTTP_HEADER_NOT_FOUND                 equ (INTERNET_ERROR_BASE + 150)
ERROR_HTTP_DOWNLEVEL_SERVER                 equ (INTERNET_ERROR_BASE + 151)
ERROR_HTTP_INVALID_SERVER_RESPONSE          equ (INTERNET_ERROR_BASE + 152)
ERROR_HTTP_INVALID_HEADER                   equ (INTERNET_ERROR_BASE + 153)
ERROR_HTTP_INVALID_QUERY_REQUEST            equ (INTERNET_ERROR_BASE + 154)
ERROR_HTTP_HEADER_ALREADY_EXISTS            equ (INTERNET_ERROR_BASE + 155)
ERROR_HTTP_REDIRECT_FAILED                  equ (INTERNET_ERROR_BASE + 156)
ERROR_HTTP_NOT_REDIRECTED                   equ (INTERNET_ERROR_BASE + 160)
ERROR_HTTP_COOKIE_NEEDS_CONFIRMATION        equ (INTERNET_ERROR_BASE + 161)
ERROR_HTTP_COOKIE_DECLINED                  equ (INTERNET_ERROR_BASE + 162)
ERROR_HTTP_REDIRECT_NEEDS_CONFIRMATION      equ (INTERNET_ERROR_BASE + 168)
ERROR_INTERNET_SECURITY_CHANNEL_ERROR       equ (INTERNET_ERROR_BASE + 157)
ERROR_INTERNET_UNABLE_TO_CACHE_FILE         equ (INTERNET_ERROR_BASE + 158)
ERROR_INTERNET_TCPIP_NOT_INSTALLED          equ (INTERNET_ERROR_BASE + 159)
ERROR_INTERNET_DISCONNECTED                 equ (INTERNET_ERROR_BASE + 163)
ERROR_INTERNET_SERVER_UNREACHABLE           equ (INTERNET_ERROR_BASE + 164)
ERROR_INTERNET_PROXY_SERVER_UNREACHABLE     equ (INTERNET_ERROR_BASE + 165)
ERROR_INTERNET_BAD_AUTO_PROXY_SCRIPT        equ (INTERNET_ERROR_BASE + 166)
ERROR_INTERNET_UNABLE_TO_DOWNLOAD_SCRIPT    equ (INTERNET_ERROR_BASE + 167)
ERROR_INTERNET_SEC_INVALID_CERT             equ (INTERNET_ERROR_BASE + 169)
ERROR_INTERNET_SEC_CERT_REVOKED             equ (INTERNET_ERROR_BASE + 170)
ERROR_INTERNET_FAILED_DUETOSECURITYCHECK    equ (INTERNET_ERROR_BASE + 171)
INTERNET_ERROR_LAST                         equ ERROR_INTERNET_FAILED_DUETOSECURITYCHECK
NORMAL_CACHE_ENTRY          equ 1
COOKIE_CACHE_ENTRY          equ 100000h
URLHISTORY_CACHE_ENTRY      equ 200000h
TRACK_OFFLINE_CACHE_ENTRY   equ 10h
TRACK_ONLINE_CACHE_ENTRY    equ 20h
STICKY_CACHE_ENTRY          equ 4
SPARSE_CACHE_ENTRY          equ 10000h
URLCACHE_FIND_DEFAULT_FILTER equ NORMAL_CACHE_ENTRY \
 + COOKIE_CACHE_ENTRY \
 + URLHISTORY_CACHE_ENTRY \
 + TRACK_OFFLINE_CACHE_ENTRY \
 + TRACK_ONLINE_CACHE_ENTRY \
 + STICKY_CACHE_ENTRY
CACHE_ENTRY_ATTRIBUTE_FC    equ 4
CACHE_ENTRY_HITRATE_FC      equ 10h
CACHE_ENTRY_MODTIME_FC      equ 40h
CACHE_ENTRY_EXPTIME_FC      equ 80h
CACHE_ENTRY_ACCTIME_FC      equ 100h
CACHE_ENTRY_SYNCTIME_FC     equ 200h
CACHE_ENTRY_HEADERINFO_FC   equ 400h
CACHE_ENTRY_EXEMPT_DELTA_FC equ 800h
INTERNET_DIAL_UNATTENDED    equ 8000h
INTERENT_GOONLINE_REFRESH   equ 1
INTERENT_GOONLINE_MASK      equ 1
INTERNET_AUTODIAL_FORCE_ONLINE          equ 1
INTERNET_AUTODIAL_FORCE_UNATTENDED      equ 2
INTERNET_AUTODIAL_FAILIFSECURITYCHECK   equ 4
INTERNET_AUTODIAL_FLAGS_MASK equ (INTERNET_AUTODIAL_FORCE_ONLINE + INTERNET_AUTODIAL_FORCE_UNATTENDED + INTERNET_AUTODIAL_FAILIFSECURITYCHECK)
INTERNET_CONNECTION_MODEM   equ 1
INTERNET_CONNECTION_LAN     equ 2
INTERNET_CONNECTION_PROXY   equ 4
INTERNET_CONNECTION_MODEM_BUSY          equ 8
INTERNET_CUSTOMDIAL_CONNECT             equ 0
INTERNET_CUSTOMDIAL_UNATTENDED          equ 1
INTERNET_CUSTOMDIAL_DISCONNECT          equ 2
INTERNET_CUSTOMDIAL_SHOWOFFLINE         equ 4
INTERNET_CUSTOMDIAL_SAFE_FOR_UNATTENDED equ 1
INTERNET_CUSTOMDIAL_WILL_SUPPLY_STATE   equ 2
INTERNET_CUSTOMDIAL_CAN_HANGUP          equ 4
INTERNET_DIALSTATE_DISCONNECTED         equ 1
ISO_FORCE_DISCONNECTED  equ 1
INTERNET_SCHEME_PARTIAL equ -2
INTERNET_SCHEME_UNKNOWN equ -1
INTERNET_SCHEME_DEFAULT equ 0
INTERNET_SCHEME_FTP     equ 1
INTERNET_SCHEME_GOPHER  equ 2
INTERNET_SCHEME_HTTP    equ 3
INTERNET_SCHEME_HTTPS   equ 4
INTERNET_SCHEME_FILE    equ 5
INTERNET_SCHEME_NEWS    equ 6
INTERNET_SCHEME_MAILTO  equ 7
INTERNET_SCHEME_SOCKS   equ 8
INTERNET_SCHEME_FIRST   equ INTERNET_SCHEME_FTP equ 9
INTERNET_SCHEME_LAST    equ INTERNET_SCHEME_SOCKS equ 10

INTERNET_ASYNC_RESULT STRUCT
 dwResult   dd ?
 dwError    dd ?
INTERNET_ASYNC_RESULT ENDS

INTERNET_PROXY_INFO STRUCT
 dwAccessType       dd ?
 lpszProxy          dd ?
 lpszProxyBypass    dd ?
INTERNET_PROXY_INFO ENDS

INTERNET_VERSION_INFO STRUCT
 dwMajorVersion dd ?
 dwMinorVersion dd ?
INTERNET_VERSION_INFO ENDS

HTTP_VERSION_INFO STRUCT
 dwMajorVersion dd ?
 dwMinorVersion dd ?
HTTP_VERSION_INFO ENDS

INTERNET_CONNECTED_INFO STRUCT
 dwConnectedState   dd ?
 dwFlags            dd ?
INTERNET_CONNECTED_INFO ENDS

URL_COMPONENTSA STRUCT dword
 dwStructSize       dd ?
 lpszScheme         dd ?
 dwSchemeLength     dd ?
 nScheme            dd ?
 lpszHostName       dd ?
 dwHostNameLength   dd ?
 nPort              dw ?
 lpszUserName       dd ?
 dwUserNameLength   dd ?
 lpszPassword       dd ?
 dwPasswordLength   dd ?
 lpszUrlPath        dd ?
 dwUrlPathLength    dd ?
 lpszExtraInfo      dd ?
 dwExtraInfoLength  dd ?
URL_COMPONENTSA ENDS
URL_COMPONENTS equ <URL_COMPONENTSA>

INTERNET_CERTIFICATE_INFO STRUCT
 ftExpiry               FILETIME <>
 ftStart                FILETIME <>
 lpszSubjectInfo        dd ?
 lpszIssuerInfo         dd ?
 lpszProtocolName       dd ?
 lpszSignatureAlgName   dd ?
 lpszEncryptionAlgName  dd ?
 dwKeySize              dd ?
INTERNET_CERTIFICATE_INFO ENDS

INTERNET_BUFFERSA STRUCT
 dwStructSize   dd ?
 Next           dd ?
 lpcszHeader    dd ?
 dwHeadersLength dd ?
 dwHeadersTotal dd ?
 lpvBuffer      dd ?
 dwBufferLength dd ?
 dwBufferTotal  dd ?
 dwOffsetLow    dd ?
 dwOffsetHigh   dd ?
INTERNET_BUFFERSA ENDS
INTERNET_BUFFERS equ <INTERNET_BUFFERSA>

GOPHER_FIND_DATAA STRUCT
 DisplayString db MAX_GOPHER_DISPLAY_TEXT + 1 dup(?)
 GopherType dd ?
 SizeLow    dd ?
 SizeHigh   dd ?
 LastModificationTime FILETIME <>
 Locator    db MAX_GOPHER_LOCATOR_LENGTH + 1 dup(?)
GOPHER_FIND_DATAA ENDS
GOPHER_FIND_DATA equ <GOPHER_FIND_DATAA>

GOPHER_ADMIN_ATTRIBUTE_TYPE STRUCT
 Comment        dd ?
 EmailAddress   dd ?
GOPHER_ADMIN_ATTRIBUTE_TYPE ENDS

GOPHER_MOD_DATE_ATTRIBUTE_TYPE STRUCT
 DateAndTime FILETIME <>
GOPHER_MOD_DATE_ATTRIBUTE_TYPE ENDS

GOPHER_TTL_ATTRIBUTE_TYPE STRUCT
 Ttl dd ?
GOPHER_TTL_ATTRIBUTE_TYPE ENDS

GOPHER_SCORE_ATTRIBUTE_TYPE STRUCT
 Score dd ?
GOPHER_SCORE_ATTRIBUTE_TYPE ENDS

GOPHER_SCORE_RANGE_ATTRIBUTE_TYPE STRUCT
 LowerBound dd ?
 UpperBound dd ?
GOPHER_SCORE_RANGE_ATTRIBUTE_TYPE ENDS

GOPHER_SITE_ATTRIBUTE_TYPE STRUCT
 Site dd ?
GOPHER_SITE_ATTRIBUTE_TYPE ENDS

GOPHER_ORGANIZATION_ATTRIBUTE_TYPE STRUCT
 Organization dd ?
GOPHER_ORGANIZATION_ATTRIBUTE_TYPE ENDS

GOPHER_LOCATION_ATTRIBUTE_TYPE STRUCT
 Location dd ?
GOPHER_LOCATION_ATTRIBUTE_TYPE ENDS

GOPHER_GEOGRAPHICAL_LOCATION_ATTRIBUTE_TYPE STRUCT
 DegreesNorth   dd ?
 MinutesNorth   dd ?
 SecondsNorth   dd ?
 DegreesEast    dd ?
 MinutesEast    dd ?
 SecondsEast    dd ?
GOPHER_GEOGRAPHICAL_LOCATION_ATTRIBUTE_TYPE ENDS

GOPHER_TIMEZONE_ATTRIBUTE_TYPE STRUCT
 Zone dd ?
GOPHER_TIMEZONE_ATTRIBUTE_TYPE ENDS

GOPHER_PROVIDER_ATTRIBUTE_TYPE STRUCT
 Provider dd ?
GOPHER_PROVIDER_ATTRIBUTE_TYPE ENDS

GOPHER_VERSION_ATTRIBUTE_TYPE STRUCT
 Version dd ?
GOPHER_VERSION_ATTRIBUTE_TYPE ENDS

GOPHER_ABSTRACT_ATTRIBUTE_TYPE STRUCT
 ShortAbstract  dd ?
 AbstractFile   dd ?
GOPHER_ABSTRACT_ATTRIBUTE_TYPE ENDS

GOPHER_VIEW_ATTRIBUTE_TYPE STRUCT
 ContentType    dd ?
 Language       dd ?
 iSize          dd ?
GOPHER_VIEW_ATTRIBUTE_TYPE ENDS

GOPHER_VERONICA_ATTRIBUTE_TYPE STRUCT
 TreeWalk dd ?
GOPHER_VERONICA_ATTRIBUTE_TYPE ENDS

GOPHER_ASK_ATTRIBUTE_TYPE STRUCT
 QuestionType dd ?
 QuestionText dd ?
GOPHER_ASK_ATTRIBUTE_TYPE ENDS

GOPHER_UNKNOWN_ATTRIBUTE_TYPE STRUCT
 Text dd ?
GOPHER_UNKNOWN_ATTRIBUTE_TYPE ENDS

GOPHER_ATTRIBUTE_TYPE STRUCT
 CategoryId     dd ?
 AttributeId    dd ?

 union AttributeType
 Admin      GOPHER_ADMIN_ATTRIBUTE_TYPE <>
 ModDate    GOPHER_MOD_DATE_ATTRIBUTE_TYPE <>
 Ttl        GOPHER_TTL_ATTRIBUTE_TYPE <>
 Score      GOPHER_SCORE_ATTRIBUTE_TYPE <>
 ScoreRange GOPHER_SCORE_RANGE_ATTRIBUTE_TYPE <>
 Site       GOPHER_SITE_ATTRIBUTE_TYPE <>
 Organization GOPHER_ORGANIZATION_ATTRIBUTE_TYPE <>
 Location   GOPHER_LOCATION_ATTRIBUTE_TYPE <>
 GeographicalLocation GOPHER_GEOGRAPHICAL_LOCATION_ATTRIBUTE_TYPE <>
 TimeZone   GOPHER_TIMEZONE_ATTRIBUTE_TYPE <>
 Provider   GOPHER_PROVIDER_ATTRIBUTE_TYPE <>
 Version    GOPHER_VERSION_ATTRIBUTE_TYPE <>
 Abstract   GOPHER_ABSTRACT_ATTRIBUTE_TYPE <>
 View       GOPHER_VIEW_ATTRIBUTE_TYPE <>
 Veronica   GOPHER_VERONICA_ATTRIBUTE_TYPE <>
 Ask        GOPHER_ASK_ATTRIBUTE_TYPE <>
 Unknown    GOPHER_UNKNOWN_ATTRIBUTE_TYPE <>
 ENDS
GOPHER_ATTRIBUTE_TYPE ENDS

INTERNET_AUTH_NOTIFY_DATA STRUCT
 cbStruct   dd ?
 dwOptions  dd ?
 pfnNotify  dd ?
 dwContext  dd ?
INTERNET_AUTH_NOTIFY_DATA ENDS

INTERNET_CACHE_ENTRY_INFOA STRUCT
 dwStructSize       dd ?
 lpszSourceUrlName  dd ?
 lpszLocalFileName  dd ?
 CacheEntryType     dd ?
 dwUseCount         dd ?
 dwHitRate          dd ?
 dwSizeLow          dd ?
 dwSizeHigh         dd ?
 LastModifiedTime   FILETIME <>
 ExpireTime         FILETIME <>
 LastAccessTime     FILETIME <>
 LastSyncTime       FILETIME <>
 lpHeaderInfo       dd ?
 dwHeaderInfoSize   dd ?
 lpszFileExtension  dd ?
 union
 dwReserved         dd ?
 dwExemptDelta      dd ?
 ENDS
INTERNET_CACHE_ENTRY_INFOA ENDS
INTERNET_CACHE_ENTRY_INFO equ <INTERNET_CACHE_ENTRY_INFOA>

; ----------------------------- shlobj.h -----------------------------

CMF_NORMAL              equ 0
CMF_DEFAULTONLY         equ 1
CMF_VERBSONLY           equ 2
CMF_EXPLORE             equ 4
CMF_NOVERBS             equ 8
CMF_CANRENAME           equ 10h
CMF_NODEFAULT           equ 20h
CMF_INCLUDESTATIC       equ 40h
CMF_RESERVED            equ 0ffff0000h 
GCS_VERBA               equ 0 
GCS_HELPTEXTA           equ 1 
GCS_VALIDATEA           equ 2 
GCS_VERBW               equ 4 
GCS_HELPTEXTW           equ 5 
GCS_VALIDATEW           equ 6 
GCS_UNICODE             equ 4 
GCS_VERB                equ GCS_VERBA
GCS_HELPTEXT            equ GCS_HELPTEXTA
GCS_VALIDATE            equ GCS_VALIDATEA
CMDSTR_NEWFOLDERA       equ "NewFolder"
CMDSTR_VIEWLISTA        equ "ViewList"
CMDSTR_VIEWDETAILSA     equ "ViewDetails"
CMDSTR_NEWFOLDER        equ CMDSTR_NEWFOLDERA
CMDSTR_VIEWLIST         equ CMDSTR_VIEWLISTA
CMDSTR_VIEWDETAILS      equ CMDSTR_VIEWDETAILSA
CMIC_MASK_HOTKEY        equ SEE_MASK_HOTKEY
CMIC_MASK_ICON          equ SEE_MASK_ICON
CMIC_MASK_FLAG_NO_UI    equ SEE_MASK_FLAG_NO_UI
CMIC_MASK_UNICODE       equ SEE_MASK_UNICODE
CMIC_MASK_NO_CONSOLE    equ SEE_MASK_NO_CONSOLE
CMIC_MASK_HASLINKNAME   equ SEE_MASK_HASLINKNAME
CMIC_MASK_FLAG_SEP_VDM  equ SEE_MASK_FLAG_SEPVDM
CMIC_MASK_HASTITLE      equ SEE_MASK_HASTITLE
CMIC_MASK_ASYNCOK       equ SEE_MASK_ASYNCOK
CMIC_MASK_PTINVOKE      equ 20000000h
GIL_OPENICON            equ 1 
GIL_FORSHELL            equ 2 
GIL_ASYNC               equ 20h 
GIL_SIMULATEDOC         equ 1 
GIL_PERINSTANCE         equ 2 
GIL_PERCLASS            equ 4 
GIL_NOTFILENAME         equ 8 
GIL_DONTCACHE           equ 10h 
ISIOI_ICONFILE          equ 1 
ISIOI_ICONINDEX         equ 2 
ISIOI_SYSIMAGELISTINDEX equ 4 
FO_MOVE                 equ 1
FO_COPY                 equ 2
FO_DELETE               equ 3
FO_RENAME               equ 4
FOF_MULTIDESTFILES      equ 1
FOF_CONFIRMMOUSE        equ 2
FOF_SILENT              equ 4 
FOF_RENAMEONCOLLISION   equ 8
FOF_NOCONFIRMATION      equ 10h 
FOF_WANTMAPPINGHANDLE   equ 20h 
FOF_ALLOWUNDO           equ 40h
FOF_FILESONLY           equ 80h 
FOF_SIMPLEPROGRESS      equ 100h 
FOF_NOCONFIRMMKDIR      equ 200h 
FOF_NOERRORUI           equ 400h 
FOF_NOCOPYSECURITYATTRIBS equ 800h 
PO_DELETE               equ 13h 
PO_RENAME               equ 14h 
PO_PORTCHANGE           equ 20h 
PO_REN_PORT             equ 34h 
FVSIF_RECT              equ 1 
FVSIF_PINNED            equ 2 
FVSIF_NEWFAILED         equ 8000000h 
FVSIF_NEWFILE           equ 80000000h 
FVSIF_CANVIEWIT         equ 40000000h 
FCIDM_SHVIEWFIRST       equ 0
FCIDM_SHVIEWLAST        equ 7fffh
FCIDM_BROWSERFIRST      equ 0a000h
FCIDM_BROWSERLAST       equ 0bf00h
FCIDM_GLOBALFIRST       equ 8000h
FCIDM_GLOBALLAST        equ 9fffh
FCIDM_MENU_FILE         equ (FCIDM_GLOBALFIRST+0)
FCIDM_MENU_EDIT         equ (FCIDM_GLOBALFIRST+40h)
FCIDM_MENU_VIEW         equ (FCIDM_GLOBALFIRST+80h)
FCIDM_MENU_VIEW_SEP_OPTIONS equ (FCIDM_GLOBALFIRST+81h)
FCIDM_MENU_TOOLS        equ (FCIDM_GLOBALFIRST+0c0h)
FCIDM_MENU_TOOLS_SEP_GOTO equ (FCIDM_GLOBALFIRST+0c1h)
FCIDM_MENU_HELP         equ (FCIDM_GLOBALFIRST+100h)
FCIDM_MENU_FIND         equ (FCIDM_GLOBALFIRST+140h)
FCIDM_MENU_EXPLORE      equ (FCIDM_GLOBALFIRST+150h)
FCIDM_MENU_FAVORITES    equ (FCIDM_GLOBALFIRST+170h)
FCIDM_TOOLBAR           equ (FCIDM_BROWSERFIRST + 0)
FCIDM_STATUS            equ (FCIDM_BROWSERFIRST + 1)
IDC_OFFLINE_HAND        equ 103
SBSP_DEFBROWSER         equ 0
SBSP_SAMEBROWSER        equ 1
SBSP_NEWBROWSER         equ 2
SBSP_DEFMODE            equ 0
SBSP_OPENMODE           equ 10h
SBSP_EXPLOREMODE        equ 20h
SBSP_ABSOLUTE           equ 0
SBSP_RELATIVE           equ 1000h
SBSP_PARENT             equ 2000h
SBSP_NAVIGATEBACK       equ 4000h
SBSP_NAVIGATEFORWARD    equ 8000h
SBSP_ALLOW_AUTONAVIGATE equ 10000h
SBSP_INITIATEDBYHLINKFRAME equ 80000000h
SBSP_REDIRECT           equ 40000000h
SBSP_WRITENOHISTORY     equ 8000000h
SBSP_NOAUTOSELECT       equ 4000000h
FCW_STATUS              equ 1
FCW_TOOLBAR             equ 2
FCW_TREE                equ 3
FCW_INTERNETBAR         equ 6
FCW_PROGRESS            equ 8
FCT_MERGE               equ 1
FCT_CONFIGABLE          equ 2
FCT_ADDTOEND            equ 4
CDBOSC_SETFOCUS         equ 0
CDBOSC_KILLFOCUS        equ 1
CDBOSC_SELCHANGE        equ 2
CDBOSC_RENAME           equ 3
SVSI_DESELECT           equ 0

SVSI_SELECT             equ 1
SVSI_EDIT               equ 3 
SVSI_DESELECTOTHERS     equ 4
SVSI_ENSUREVISIBLE      equ 8
SVSI_FOCUSED            equ 10h
SVSI_TRANSLATEPT        equ 20h
SVGIO_BACKGROUND        equ 0
SVGIO_SELECTION         equ 1
SVGIO_ALLVIEW           equ 2
SV2GV_CURRENTVIEW       equ 0ffffffffh
SV2GV_DEFAULTVIEW       equ 0fffffffeh
STRRET_WSTR             equ 0 
STRRET_OFFSET           equ 1 
STRRET_CSTR             equ 2 
CSIDL_DESKTOP           equ 0
CSIDL_INTERNET          equ 1
CSIDL_PROGRAMS          equ 2
CSIDL_CONTROLS          equ 3
CSIDL_PRINTERS          equ 4
CSIDL_PERSONAL          equ 5
CSIDL_FAVORITES         equ 6
CSIDL_STARTUP           equ 7
CSIDL_RECENT            equ 8
CSIDL_SENDTO            equ 9
CSIDL_BITBUCKET         equ 0ah
CSIDL_STARTMENU         equ 0bh
CSIDL_DESKTOPDIRECTORY  equ 10h
CSIDL_DRIVES            equ 11h
CSIDL_NETWORK           equ 12h
CSIDL_NETHOOD           equ 13h
CSIDL_FONTS             equ 14h
CSIDL_TEMPLATES         equ 15h
CSIDL_COMMON_STARTMENU  equ 16h
CSIDL_COMMON_PROGRAMS   equ 17h
CSIDL_COMMON_STARTUP    equ 18h
CSIDL_COMMON_DESKTOPDIRECTORY equ 19h
CSIDL_APPDATA           equ 1ah
CSIDL_PRINTHOOD         equ 1bh
CSIDL_ALTSTARTUP        equ 1dh 
CSIDL_COMMON_ALTSTARTUP equ 1eh 
CSIDL_COMMON_FAVORITES  equ 1fh
CSIDL_INTERNET_CACHE    equ 20h
CSIDL_COOKIES           equ 21h
CSIDL_HISTORY           equ 22h
BIF_RETURNONLYFSDIRS    equ 1 
BIF_DONTGOBELOWDOMAIN   equ 2 
BIF_STATUSTEXT          equ 4
BIF_RETURNFSANCESTORS   equ 8
BIF_EDITBOX             equ 10h
BIF_VALIDATE            equ 20h 
BIF_BROWSEFORCOMPUTER   equ 1000h 
BIF_BROWSEFORPRINTER    equ 2000h 
BIF_BROWSEINCLUDEFILES  equ 4000h 
BFFM_INITIALIZED        equ 1
BFFM_SELCHANGED         equ 2
BFFM_VALIDATEFAILEDA    equ 3 
BFFM_VALIDATEFAILEDW    equ 4 
BFFM_SETSTATUSTEXTA     equ (WM_USER + 100)
BFFM_ENABLEOK           equ (WM_USER + 101)
BFFM_SETSELECTIONA      equ (WM_USER + 102)
BFFM_SETSELECTIONW      equ (WM_USER + 103)
BFFM_SETSTATUSTEXTW     equ (WM_USER + 104)
SFGAO_CANCOPY           equ DROPEFFECT_COPY 
SFGAO_CANMOVE           equ DROPEFFECT_MOVE 
SFGAO_CANLINK           equ DROPEFFECT_LINK 
SFGAO_CANRENAME         equ 10h 
SFGAO_CANDELETE         equ 20h 
SFGAO_HASPROPSHEET      equ 40h 
SFGAO_DROPTARGET        equ 100h 
SFGAO_CAPABILITYMASK    equ 177h
SFGAO_LINK              equ 10000h 
SFGAO_SHARE             equ 20000h 
SFGAO_READONLY          equ 40000h 
SFGAO_GHOSTED           equ 80000h 
SFGAO_HIDDEN            equ 80000h 
SFGAO_DISPLAYATTRMASK   equ 0F0000h
SFGAO_FILESYSANCESTOR   equ 10000000h 
SFGAO_FOLDER            equ 20000000h 
SFGAO_FILESYSTEM        equ 40000000h 
SFGAO_HASSUBFOLDER      equ 80000000h 
SFGAO_CONTENTSMASK      equ 80000000h
SFGAO_VALIDATE          equ 1000000h 
SFGAO_REMOVABLE         equ 2000000h 
SFGAO_COMPRESSED        equ 4000000h 
SFGAO_BROWSABLE         equ 8000000h 
SFGAO_NONENUMERATED     equ 100000h 
SFGAO_NEWCONTENT        equ 200000h 
DWFRF_NORMAL            equ 0
DWFRF_DELETECONFIGDATA  equ 1
DWFAF_HIDDEN            equ 1 
DBIM_MINSIZE            equ 1
DBIM_MAXSIZE            equ 2
DBIM_INTEGRAL           equ 4
DBIM_ACTUAL             equ 8
DBIM_TITLE              equ 10h
DBIM_MODEFLAGS          equ 20h
DBIM_BKCOLOR            equ 40h
DBIMF_NORMAL            equ 0
DBIMF_VARIABLEHEIGHT    equ 8
DBIMF_DEBOSSED          equ 20h
DBIMF_BKCOLOR           equ 40h
DBIF_VIEWMODE_NORMAL    equ 0
DBIF_VIEWMODE_VERTICAL  equ 1
DBIF_VIEWMODE_FLOATING  equ 2
DBIF_VIEWMODE_TRANSPARENT equ 4
COMPONENT_TOP           equ (7fffffffh) 
COMP_TYPE_HTMLDOC       equ 0
COMP_TYPE_PICTURE       equ 1
COMP_TYPE_WEBSITE       equ 2
COMP_TYPE_CONTROL       equ 3
COMP_TYPE_CFHTML        equ 4
COMP_TYPE_MAX           equ 4
AD_APPLY_SAVE           equ 1
AD_APPLY_HTMLGEN        equ 2
AD_APPLY_REFRESH        equ 4
AD_APPLY_ALL            equ (AD_APPLY_SAVE + AD_APPLY_HTMLGEN + AD_APPLY_REFRESH)
AD_APPLY_FORCE          equ 8
AD_APPLY_BUFFERED_REFRESH equ 10h
WPSTYLE_CENTER          equ 0
WPSTYLE_TILE            equ 1
WPSTYLE_STRETCH         equ 2
WPSTYLE_MAX             equ 3
COMP_ELEM_TYPE          equ 1
COMP_ELEM_CHECKED       equ 2
COMP_ELEM_DIRTY         equ 4
COMP_ELEM_NOSCROLL      equ 8
COMP_ELEM_POS_LEFT      equ 10h
COMP_ELEM_POS_TOP       equ 20h
COMP_ELEM_SIZE_WIDTH    equ 40h
COMP_ELEM_SIZE_HEIGHT   equ 80h
COMP_ELEM_POS_ZINDEX    equ 100h
COMP_ELEM_SOURCE        equ 200h
COMP_ELEM_FRIENDLYNAME  equ 400h
COMP_ELEM_SUBSCRIBEDURL equ 800h
COMP_ELEM_ALL equ (COMP_ELEM_TYPE + COMP_ELEM_CHECKED + COMP_ELEM_DIRTY + \
 COMP_ELEM_NOSCROLL + COMP_ELEM_POS_LEFT + COMP_ELEM_SIZE_WIDTH \
 COMP_ELEM_SIZE_HEIGHT + COMP_ELEM_POS_ZINDEX + COMP_ELEM_SOURCE \
 COMP_ELEM_FRIENDLYNAME )
ADDURL_SILENT           equ 1
CFSTR_SHELLIDLIST       equ "Shell IDList Array"
CFSTR_SHELLIDLISTOFFSET equ "Shell Object Offsets"
CFSTR_NETRESOURCES      equ "Net Resource"
CFSTR_FILEDESCRIPTORA   equ "FileGroupDescriptor"
CFSTR_FILECONTENTS      equ "FileContents"
CFSTR_FILENAMEA         equ "FileName"
CFSTR_PRINTERGROUP      equ "PrinterFriendlyName"
CFSTR_FILENAMEMAPA      equ "FileNameMap"
CFSTR_SHELLURL          equ "UniformResourceLocator"
CFSTR_PREFERREDDROPEFFECT equ "Preferred DropEffect"
CFSTR_PERFORMEDDROPEFFECT equ "Performed DropEffect"
CFSTR_PASTESUCCEEDED    equ "Paste Succeeded"
CFSTR_INDRAGLOOP        equ "InShellDragLoop"
CFSTR_FILEDESCRIPTOR    equ CFSTR_FILEDESCRIPTORA
CFSTR_FILENAME          equ CFSTR_FILENAMEA
CFSTR_FILENAMEMAP       equ CFSTR_FILENAMEMAPA
DVASPECT_SHORTNAME      equ 2 
SHCNE_RENAMEITEM        equ 1
SHCNE_CREATE            equ 2
SHCNE_DELETE            equ 4
SHCNE_MKDIR             equ 8
SHCNE_RMDIR             equ 10h
SHCNE_MEDIAINSERTED     equ 20h
SHCNE_MEDIAREMOVED      equ 40h
SHCNE_DRIVEREMOVED      equ 80h
SHCNE_DRIVEADD          equ 100h
SHCNE_NETSHARE          equ 200h
SHCNE_NETUNSHARE        equ 400h
SHCNE_ATTRIBUTES        equ 800h
SHCNE_UPDATEDIR         equ 1000h
SHCNE_UPDATEITEM        equ 2000h
SHCNE_SERVERDISCONNECT  equ 4000h
SHCNE_UPDATEIMAGE       equ 8000h
SHCNE_DRIVEADDGUI       equ 10000h
SHCNE_RENAMEFOLDER      equ 20000h
SHCNE_FREESPACE         equ 40000h
SHCNE_EXTENDED_EVENT    equ 4000000h
SHCNE_ASSOCCHANGED      equ 8000000h
SHCNE_DISKEVENTS        equ 2381Fh
SHCNE_GLOBALEVENTS      equ 0C0581E0h 
SHCNE_ALLEVENTS         equ 7FFFFFFFh
SHCNE_INTERRUPT         equ 80000000h 
SHCNEE_ORDERCHANGED     equ 2 
SHCNF_IDLIST            equ 0 
SHCNF_PATHA             equ 1 
SHCNF_PRINTERA          equ 2 
SHCNF_DWORD             equ 3 
SHCNF_PATHW             equ 5 
SHCNF_PRINTERW          equ 6 
SHCNF_TYPE              equ 0FFh
SHCNF_FLUSH             equ 1000h
SHCNF_FLUSHNOWAIT       equ 2000h
SHCNF_PATH              equ SHCNF_PATHA
SHCNF_PRINTER           equ SHCNF_PRINTERA
QIF_CACHED              equ 1
QIF_DONTEXPANDFOLDER    equ 2
SHARD_PIDL              equ 1
SHARD_PATHA             equ 2
SHARD_PATHW             equ 3
SHARD_PATH              equ SHARD_PATHA
SHGDFIL_FINDDATA        equ 1
SHGDFIL_NETRESOURCE     equ 2
SHGDFIL_DESCRIPTIONID   equ 3
SHDID_ROOT_REGITEM      equ 1
SHDID_FS_FILE           equ 2
SHDID_FS_DIRECTORY      equ 3
SHDID_FS_OTHER          equ 4
SHDID_COMPUTER_DRIVE35 equ 5
SHDID_COMPUTER_DRIVE525 equ 6
SHDID_COMPUTER_REMOVABLE equ 7
SHDID_COMPUTER_FIXED    equ 8
SHDID_COMPUTER_NETDRIVE equ 9
SHDID_COMPUTER_CDROM    equ 10
SHDID_COMPUTER_RAMDISK  equ 11
SHDID_COMPUTER_OTHER    equ 12
SHDID_NET_DOMAIN        equ 13
SHDID_NET_SERVER        equ 14
SHDID_NET_SHARE         equ 15
SHDID_NET_RESTOFNET     equ 16
SHDID_NET_OTHER         equ 17
PID_IS_URL              equ 2
PID_IS_NAME             equ 4
PID_IS_WORKINGDIR       equ 5
PID_IS_HOTKEY           equ 6
PID_IS_SHOWCMD          equ 7
PID_IS_ICONINDEX        equ 8
PID_IS_ICONFILE         equ 9
PID_IS_WHATSNEW         equ 10
PID_IS_AUTHOR           equ 11
PID_IS_DESCRIPTION      equ 12
PID_IS_COMMENT          equ 13
PID_INTSITE_WHATSNEW    equ 2
PID_INTSITE_AUTHOR      equ 3
PID_INTSITE_LASTVISIT   equ 4
PID_INTSITE_LASTMOD     equ 5
PID_INTSITE_VISITCOUNT  equ 6
PID_INTSITE_DESCRIPTION equ 7
PID_INTSITE_COMMENT     equ 8
PID_INTSITE_FLAGS       equ 9 
PID_INTSITE_CONTENTLEN  equ 10 
PID_INTSITE_CONTENTCODE equ 11 
PID_INTSITE_RECURSE     equ 12
PID_INTSITE_WATCH       equ 13
PID_INTSITE_SUBSCRIPTION equ 14
PID_INTSITE_URL         equ 15
PID_INTSITE_TITLE       equ 16
PID_INTSITE_CODEPAGE    equ 18
PID_INTSITE_TRACKING    equ 19
PIDISF_RECENTLYCHANGED  equ 1 
PIDISF_CACHEDSTICKY     equ 2 
PIDISF_CACHEIMAGES      equ 10h 
PIDISF_FOLLOWALLLINKS   equ 20h 
PIDISM_GLOBAL           equ 0 
PIDISM_WATCH            equ 1 
PIDISM_DONTWATCH        equ 2 
SSF_SHOWALLOBJECTS      equ 1
SSF_SHOWEXTENSIONS      equ 2
SSF_SHOWCOMPCOLOR       equ 8
SSF_SHOWSYSFILES        equ 20h
SSF_DOUBLECLICKINWEBVIEW equ 80h
SSF_SHOWATTRIBCOL       equ 100h
SSF_DESKTOPHTML         equ 200h
SSF_WIN95CLASSIC        equ 400h
SSF_DONTPRETTYPATH      equ 800h
SSF_SHOWINFOTIP         equ 2000h
SSF_MAPNETDRVBUTTON     equ 1000h
SSF_NOCONFIRMRECYCLE    equ 8000h
SSF_HIDEICONS           equ 4000h
SLR_NO_UI               equ 1
SLR_ANY_MATCH           equ 2
SLR_UPDATE              equ 4
SLR_NOUPDATE            equ 8
SLGP_SHORTPATH          equ 1
SLGP_UNCPRIORITY        equ 2
SLGP_RAWPATH            equ 4
FWF_AUTOARRANGE         equ 1
FWF_ABBREVIATEDNAMES    equ 2
FWF_SNAPTOGRID          equ 4
FWF_OWNERDATA           equ 8
FWF_BESTFITWINDOW       equ 10h
FWF_DESKTOP             equ 20h
FWF_SINGLESEL           equ 40h
FWF_NOSUBFOLDERS        equ 80h
FWF_TRANSPARENT         equ 100h
FWF_NOCLIENTEDGE        equ 200h
FWF_NOSCROLL            equ 400h
FWF_ALIGNLEFT           equ 800h
FWF_NOICONS             equ 1000h
FWF_SINGLECLICKACTIVATE equ 8000h
FVM_ICON                equ 1
FVM_SMALLICON           equ 2
FVM_LIST                equ 3
FVM_DETAILS             equ 4
SBSC_HIDE               equ 0
SBSC_SHOW               equ 1
SBSC_TOGGLE             equ 2

SBSC_QUERY              equ 3
SBO_DEFAULT             equ 0 
SBO_NOBROWSERPAGES      equ 1
SVUIA_DEACTIVATE        equ 0
SVUIA_ACTIVATE_NOFOCUS  equ 1
SVUIA_ACTIVATE_FOCUS    equ 2
SVUIA_INPLACEACTIVATE   equ 3
SHGDN_NORMAL            equ 0
SHGDN_INFOLDER          equ 1
SHGDN_INCLUDE_NONFILESYS equ 2000h
SHGDN_FORADDRESSBAR     equ 4000h
SHGDN_FORPARSING        equ 8000h
SHCONTF_FOLDERS         equ 32
SHCONTF_NONFOLDERS      equ 64
SHCONTF_INCLUDEHIDDEN   equ 128
DBID_BANDINFOCHANGED    equ 0
DBID_SHOWONLY           equ 1
DBID_MAXIMIZEBAND       equ 2
DTI_ADDUI_DEFAULT       equ 0
DTI_ADDUI_DISPSUBWIZARD equ 1
FD_CLSID        equ 1
FD_SIZEPOINT    equ 2
FD_ATTRIBUTES   equ 4
FD_CREATETIME   equ 8
FD_ACCESSTIME   equ 10h
FD_WRITESTIME   equ 20h
FD_FILESIZE     equ 40h
FD_LINKUI       equ 8000h

;SHELLFLAGSTATE record fShowAllObjects:1,fShowExtensions:1, fNoConfirmRecycle:1,fShowSysFiles:1,fShowCompColor:1,fDoubleClickInWebView:1,fDesktopHTML:1,fWin95Classic:1,fDontPrettyPath:1,fShowAttribCol:1,fMapNetDrvBtn:1,fShowInfoTip:1,fHideIcons:1, fRestFlags:3

SHITEMID STRUCT
 cb     dw ?
 abID   db 1 dup(?) 	; variable length
SHITEMID ENDS

ITEMIDLIST STRUCT
 mkid SHITEMID <>
ITEMIDLIST ENDS

CMINVOKECOMMANDINFO STRUCT
 cbSize         dd ?
 fMask          dd ?
 hwnd           dd ?
 lpVerb         dd ?
 lpParameters   dd ?
 lpDirectory    dd ?
 nShow          dd ?
 dwHotKey       dd ?
 hIcon          dd ?
CMINVOKECOMMANDINFO ENDS

CMINVOKECOMMANDINFOEX STRUCT
 cbSize         dd ?
 fMask          dd ?
 hwnd           dd ?
 lpVerb         dd ?
 lpParameters   dd ?
 lpDirectory    dd ?
 nShow          dd ?
 dwHotKey       dd ?
 hIcon          dd ?
 lpTitle        dd ?
 lpVerbW        dd ?
 lpParametersW  dd ?
 lpDirectoryW   dd ?
 lpTitleW       dd ?
 ptInvoke       POINT <>
CMINVOKECOMMANDINFOEX ENDS

FVSHOWINFO STRUCT
 cbSize     dd ?
 hwndOwner  dd ?
 iShow      dd ?
 dwFlags    dd ?
 rect       RECT <>
 punkRel    dd ?
 strNewFile db MAX_PATH dup(?)
FVSHOWINFO ENDS

FOLDERSETTINGS STRUCT
 ViewMode   dd ?
 fFlags     dd ?
FOLDERSETTINGS ENDS

SV2CVW2_PARAMS STRUCT
 cbSize     dd ?
 psvPrev    dd ?
 pfs        dd ?
 psbOwner   dd ?
 prcView    dd ?
 pvid       dd ?
 hwndView   dd ?
SV2CVW2_PARAMS ENDS


BROWSEINFOA STRUCT
 hwndOwner      dd ?
 pidlRoot       dd ?
 pszDisplayName dd ?
 lpszTitle      dd ?
 ulFlags        dd ?
 lpfn           dd ?
 lParam         dd ?
 iImage         dd ?
BROWSEINFOA ENDS
BROWSEINFO equ <BROWSEINFOA>

DESKBANDINFO STRUCT
 dwMask         dd ?
 ptMinSize      POINT <>
 ptMaxSize      POINT <>
 ptIntegral     POINT <>
 ptActual       POINT <>
 wszTitle       WCHAR 256 dup(?)
 dwModeFlags    dd ?
 crBkgnd        COLORREF ?
DESKBANDINFO ENDS

WALLPAPEROPT STRUCT
 dwSize     dd ?
 dwStyle    dd ?
WALLPAPEROPT ENDS

COMPONENTSOPT STRUCT
 dwSize             dd ?
 fEnableComponents  dd ?
 fActiveDesktop     dd ?
COMPONENTSOPT ENDS

COMPPOS STRUCT
 dwSize                 dd ?
 iLeft                  dd ?
 iTop                   dd ?
 dwWidth                dd ?
 dwHeight               dd ?
 izIndex                dd ?
 fCanResize             dd ?
 fCanResizeX            dd ?
 fCanResizeY            dd ?
 iPreferredLeftPercent  dd ?
 iPreferredTopPercent   dd ?
COMPPOS ENDS

COMPONENT STRUCT
 dwSize     dd ?
 dwID       dd ?
 iComponentType dd ?
 fChecked   dd ?
 fDirty     dd ?
 fNoScroll  dd ?
 cpPos      COMPPOS <>
 wszFriendlyName WCHAR MAX_PATH dup(?)
 wszSource WCHAR INTERNET_MAX_URL_LENGTH dup(?)
 wszSubscribedURL WCHAR INTERNET_MAX_URL_LENGTH dup(?)
COMPONENT ENDS

NRESARRAY STRUCT
 cItems dd ?
 nr NETRESOURCE <>
NRESARRAY ENDS

CIDA STRUCT
 cidl dd ?
 aoffset dd 1 dup(?)
CIDA ENDS

FILEDESCRIPTORA STRUCT
 dwFlags        dd ?
 clsid          CLSID    <>
 sizel          SIZEL    <>
 pointl         POINTL  <>
 dwFileAttributes dd ?
 ftCreationTime FILETIME <>
 ftLastAccessTime FILETIME <>
 ftLastWriteTime FILETIME <>
 nFileSizeHigh  dd ?
 nFileSizeLow   dd ?
 cFileName      db MAX_PATH dup(?)
FILEDESCRIPTORA ENDS
FILEDESCRIPTOR equ <FILEDESCRIPTORA>

FILEGROUPDESCRIPTORA STRUCT
 cItems dd ?
 fgd FILEDESCRIPTOR <>
FILEGROUPDESCRIPTORA ENDS
FILEGROUPDESCRIPTOR equ <FILEGROUPDESCRIPTORA>

DROPFILES STRUCT
 pFiles dd ?
 pt     POINT <>
 fNC    dd ?
 fWide  dd ?
DROPFILES ENDS

SHDESCRIPTIONID STRUCT
 dwDescriptionId    dd ?
 clsid              CLSID <>
SHDESCRIPTIONID ENDS

;==================================== LMServer.h =================================
SV_PLATFORM_ID_OS2 equ 400
SV_PLATFORM_ID_NT equ 500
MAJOR_VERSION_MASK equ 0Fh
SV_TYPE_WORKSTATION equ 1
SV_TYPE_SERVER equ 2
SV_TYPE_SQLSERVER equ 4
SV_TYPE_DOMAIN_CTRL equ 8
SV_TYPE_DOMAIN_BAKCTRL equ 10h
SV_TYPE_TIME_SOURCE equ 20h
SV_TYPE_AFP equ 40h
SV_TYPE_NOVELL equ 80h
SV_TYPE_DOMAIN_MEMBER equ 100h
SV_TYPE_PRINTQ_SERVER equ 200h
SV_TYPE_DIALIN_SERVER equ 400h
SV_TYPE_XENIX_SERVER equ 800h
SV_TYPE_SERVER_UNIX equ SV_TYPE_XENIX_SERVER
SV_TYPE_NT equ 1000h
SV_TYPE_WFW equ 2000h
SV_TYPE_SERVER_MFPN equ 4000h
SV_TYPE_SERVER_NT equ 8000h
SV_TYPE_POTENTIAL_BROWSER equ 10000h
SV_TYPE_BACKUP_BROWSER equ 20000h
SV_TYPE_MASTER_BROWSER equ 40000h
SV_TYPE_DOMAIN_MASTER equ 80000h
SV_TYPE_SERVER_OSF equ 100000h
SV_TYPE_SERVER_VMS equ 200000h
SV_TYPE_WINDOWS equ 400000h
SV_TYPE_DFS equ 800000h
SV_TYPE_CLUSTER_NT equ 1000000h
SV_TYPE_DCE equ 10000000h
SV_TYPE_ALTERNATE_XPORT equ 20000000h
SV_TYPE_LOCAL_LIST_ONLY equ 40000000h
SV_TYPE_DOMAIN_ENUM equ 80000000h
SV_TYPE_ALL equ 0FFFFFFFFh
SV_NODISC equ (-1L)
SV_USERSECURITY equ 1
SV_SHARESECURITY equ 0
SV_HIDDEN equ 1
SV_VISIBLE equ 0
SV_PLATFORM_ID_PARMNUM equ 101
SV_NAME_PARMNUM equ 102
SV_VERSION_MAJOR_PARMNUM equ 103
SV_VERSION_MINOR_PARMNUM equ 104
SV_TYPE_PARMNUM equ 105
SV_COMMENT_PARMNUM equ 5
SV_USERS_PARMNUM equ 107
SV_DISC_PARMNUM equ 10
SV_HIDDEN_PARMNUM equ 16
SV_ANNOUNCE_PARMNUM equ 17
SV_ANNDELTA_PARMNUM equ 18
SV_USERPATH_PARMNUM equ 112
SV_ULIST_MTIME_PARMNUM equ 401
SV_GLIST_MTIME_PARMNUM equ 402
SV_ALIST_MTIME_PARMNUM equ 403
SV_ALERTS_PARMNUM equ 11
SV_SECURITY_PARMNUM equ 405
SV_NUMADMIN_PARMNUM equ 406
SV_LANMASK_PARMNUM equ 407
SV_GUESTACC_PARMNUM equ 408
SV_CHDEVQ_PARMNUM equ 410
SV_CHDEVJOBS_PARMNUM equ 411
SV_CONNECTIONS_PARMNUM equ 412
SV_SHARES_PARMNUM equ 413
SV_OPENFILES_PARMNUM equ 414
SV_SESSREQS_PARMNUM equ 417
SV_ACTIVELOCKS_PARMNUM equ 419
SV_NUMREQBUF_PARMNUM equ 420
SV_NUMBIGBUF_PARMNUM equ 422
SV_NUMFILETASKS_PARMNUM equ 423
SV_ALERTSCHED_PARMNUM equ 37
SV_ERRORALERT_PARMNUM equ 38
SV_LOGONALERT_PARMNUM equ 39
SV_ACCESSALERT_PARMNUM equ 40
SV_DISKALERT_PARMNUM equ 41
SV_NETIOALERT_PARMNUM equ 42
SV_MAXAUDITSZ_PARMNUM equ 43
SV_SRVHEURISTICS_PARMNUM equ 431
SV_SESSOPENS_PARMNUM equ 501
SV_SESSVCS_PARMNUM equ 502
SV_OPENSEARCH_PARMNUM equ 503
SV_SIZREQBUF_PARMNUM equ 504
SV_INITWORKITEMS_PARMNUM equ 505
SV_MAXWORKITEMS_PARMNUM equ 506
SV_RAWWORKITEMS_PARMNUM equ 507
SV_IRPSTACKSIZE_PARMNUM equ 508
SV_MAXRAWBUFLEN_PARMNUM equ 509
SV_SESSUSERS_PARMNUM equ 510
SV_SESSCONNS_PARMNUM equ 511
SV_MAXNONPAGEDMEMORYUSAGE_PARMNUM equ 512
SV_MAXPAGEDMEMORYUSAGE_PARMNUM equ 513
SV_ENABLESOFTCOMPAT_PARMNUM equ 514
SV_ENABLEFORCEDLOGOFF_PARMNUM equ 515
SV_TIMESOURCE_PARMNUM equ 516
SV_ACCEPTDOWNLEVELAPIS_PARMNUM equ 517
SV_LMANNOUNCE_PARMNUM equ 518
SV_DOMAIN_PARMNUM equ 519
SV_MAXCOPYREADLEN_PARMNUM equ 520
SV_MAXCOPYWRITELEN_PARMNUM equ 521
SV_MINKEEPSEARCH_PARMNUM equ 522
SV_MAXKEEPSEARCH_PARMNUM equ 523
SV_MINKEEPCOMPLSEARCH_PARMNUM equ 524
SV_MAXKEEPCOMPLSEARCH_PARMNUM equ 525
SV_THREADCOUNTADD_PARMNUM equ 526
SV_NUMBLOCKTHREADS_PARMNUM equ 527
SV_SCAVTIMEOUT_PARMNUM equ 528
SV_MINRCVQUEUE_PARMNUM equ 529
SV_MINFREEWORKITEMS_PARMNUM equ 530
SV_XACTMEMSIZE_PARMNUM equ 531
SV_THREADPRIORITY_PARMNUM equ 532
SV_MAXMPXCT_PARMNUM equ 533
SV_OPLOCKBREAKWAIT_PARMNUM equ 534
SV_OPLOCKBREAKRESPONSEWAIT_PARMNUM equ 535
SV_ENABLEOPLOCKS_PARMNUM equ 536
SV_ENABLEOPLOCKFORCECLOSE_PARMNUM equ 537
SV_ENABLEFCBOPENS_PARMNUM equ 538
SV_ENABLERAW_PARMNUM equ 539
SV_ENABLESHAREDNETDRIVES_PARMNUM equ 540
SV_MINFREECONNECTIONS_PARMNUM equ 541
SV_MAXFREECONNECTIONS_PARMNUM equ 542
SV_INITSESSTABLE_PARMNUM equ 543
SV_INITCONNTABLE_PARMNUM equ 544
SV_INITFILETABLE_PARMNUM equ 545
SV_INITSEARCHTABLE_PARMNUM equ 546
SV_ALERTSCHEDULE_PARMNUM equ 547
SV_ERRORTHRESHOLD_PARMNUM equ 548
SV_NETWORKERRORTHRESHOLD_PARMNUM equ 549
SV_DISKSPACETHRESHOLD_PARMNUM equ 550
SV_MAXLINKDELAY_PARMNUM equ 552
SV_MINLINKTHROUGHPUT_PARMNUM equ 553
SV_LINKINFOVALIDTIME_PARMNUM equ 554
SV_SCAVQOSINFOUPDATETIME_PARMNUM equ 555
SV_MAXWORKITEMIDLETIME_PARMNUM equ 556
SV_MAXRAWWORKITEMS_PARMNUM equ 557
SV_PRODUCTTYPE_PARMNUM equ 560
SV_SERVERSIZE_PARMNUM equ 561
SV_CONNECTIONLESSAUTODISC_PARMNUM equ 562
SV_SHARINGVIOLATIONRETRIES_PARMNUM equ 563
SV_SHARINGVIOLATIONDELAY_PARMNUM equ 564
SV_MAXGLOBALOPENSEARCH_PARMNUM equ 565
SV_REMOVEDUPLICATESEARCHES_PARMNUM equ 566
SV_LOCKVIOLATIONRETRIES_PARMNUM equ 567
SV_LOCKVIOLATIONOFFSET_PARMNUM equ 568
SV_LOCKVIOLATIONDELAY_PARMNUM equ 569
SV_MDLREADSWITCHOVER_PARMNUM equ 570
SV_CACHEDOPENLIMIT_PARMNUM equ 571
SV_CRITICALTHREADS_PARMNUM equ 572
SV_RESTRICTNULLSESSACCESS_PARMNUM equ 573
SV_ENABLEWFW311DIRECTIPX_PARMNUM equ 574
SV_OTHERQUEUEAFFINITY_PARMNUM equ 575
SV_QUEUESAMPLESECS_PARMNUM equ 576
SV_BALANCECOUNT_PARMNUM equ 577
SV_PREFERREDAFFINITY_PARMNUM equ 578
SV_MAXFREERFCBS_PARMNUM equ 579
SV_MAXFREEMFCBS_PARMNUM equ 580
SV_MAXFREELFCBS_PARMNUM equ 581
SV_MAXFREEPAGEDPOOLCHUNKS_PARMNUM equ 582
SV_MINPAGEDPOOLCHUNKSIZE_PARMNUM equ 583
SV_MAXPAGEDPOOLCHUNKSIZE_PARMNUM equ 584
SV_SENDSFROMPREFERREDPROCESSOR_PARMNUM equ 585
SV_MAXTHREADSPERQUEUE_PARMNUM equ 586
SV_CACHEDDIRECTORYLIMIT_PARMNUM equ 587
SV_MAXCOPYLENGTH_PARMNUM equ 588
SV_ENABLEBULKTRANSFER_PARMNUM equ 589
SV_ENABLECOMPRESSION_PARMNUM equ 590
SV_AUTOSHAREWKS_PARMNUM equ 591
SV_AUTOSHARESERVER_PARMNUM equ 592
SV_ENABLESECURITYSIGNATURE_PARMNUM equ 593
SV_REQUIRESECURITYSIGNATURE_PARMNUM equ 594
SV_MINCLIENTBUFFERSIZE_PARMNUM equ 595
SV_CONNECTIONNOSESSIONSTIMEOUT_PARMNUM equ 596
SV_COMMENT_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_COMMENT_PARMNUM)
SV_USERS_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_USERS_PARMNUM)
SV_DISC_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_DISC_PARMNUM)
SV_HIDDEN_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_HIDDEN_PARMNUM)
SV_ANNOUNCE_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_ANNOUNCE_PARMNUM)
SV_ANNDELTA_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_ANNDELTA_PARMNUM)
SV_SESSOPENS_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_SESSOPENS_PARMNUM)
SV_SESSVCS_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_SESSVCS_PARMNUM)
SV_OPENSEARCH_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_OPENSEARCH_PARMNUM)
SV_MAXWORKITEMS_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_MAXWORKITEMS_PARMNUM)
SV_MAXRAWBUFLEN_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_MAXRAWBUFLEN_PARMNUM)
SV_SESSUSERS_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_SESSUSERS_PARMNUM)
SV_SESSCONNS_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_SESSCONNS_PARMNUM)
SV_MAXNONPAGEDMEMORYUSAGE_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_MAXNONPAGEDMEMORYUSAGE_PARMNUM)
SV_MAXPAGEDMEMORYUSAGE_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_MAXPAGEDMEMORYUSAGE_PARMNUM)
SV_ENABLESOFTCOMPAT_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_ENABLESOFTCOMPAT_PARMNUM)
SV_ENABLEFORCEDLOGOFF_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_ENABLEFORCEDLOGOFF_PARMNUM)
SV_TIMESOURCE_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_TIMESOURCE_PARMNUM)
SV_LMANNOUNCE_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_LMANNOUNCE_PARMNUM)
SV_MAXCOPYREADLEN_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_MAXCOPYREADLEN_PARMNUM)
SV_MAXCOPYWRITELEN_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_MAXCOPYWRITELEN_PARMNUM)
SV_MINKEEPSEARCH_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_MINKEEPSEARCH_PARMNUM)
SV_MAXKEEPSEARCH_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_MAXKEEPSEARCH_PARMNUM)
SV_MINKEEPCOMPLSEARCH_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_MINKEEPCOMPLSEARCH_PARMNUM)
SV_MAXKEEPCOMPLSEARCH_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_MAXKEEPCOMPLSEARCH_PARMNUM)
SV_SCAVTIMEOUT_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_SCAVTIMEOUT_PARMNUM)
SV_MINRCVQUEUE_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_MINRCVQUEUE_PARMNUM)
SV_MINFREEWORKITEMS_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_MINFREEWORKITEMS_PARMNUM)
SV_MAXMPXCT_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_MAXMPXCT_PARMNUM)
SV_OPLOCKBREAKWAIT_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_OPLOCKBREAKWAIT_PARMNUM)
SV_OPLOCKBREAKRESPONSEWAIT_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_OPLOCKBREAKRESPONSEWAIT_PARMNUM)
SV_ENABLEOPLOCKS_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_ENABLEOPLOCKS_PARMNUM)
SV_ENABLEOPLOCKFORCECLOSE_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_ENABLEOPLOCKFORCECLOSE_PARMNUM)
SV_ENABLEFCBOPENS_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_ENABLEFCBOPENS_PARMNUM)
SV_ENABLERAW_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_ENABLERAW_PARMNUM)
SV_ENABLESHAREDNETDRIVES_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_ENABLESHAREDNETDRIVES_PARMNUM)
SV_MINFREECONNECTIONS_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_MINFREECONNECTIONS_PARMNUM)
SV_MAXFREECONNECTIONS_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_MAXFREECONNECTIONS_PARMNUM)
SV_INITSESSTABLE_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_INITSESSTABLE_PARMNUM)
SV_INITCONNTABLE_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_INITCONNTABLE_PARMNUM)
SV_INITFILETABLE_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_INITFILETABLE_PARMNUM)
SV_INITSEARCHTABLE_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_INITSEARCHTABLE_PARMNUM)
SV_ALERTSCHEDULE_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_ALERTSCHEDULE_PARMNUM)
SV_ERRORTHRESHOLD_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_ERRORTHRESHOLD_PARMNUM)
SV_NETWORKERRORTHRESHOLD_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_NETWORKERRORTHRESHOLD_PARMNUM)
SV_DISKSPACETHRESHOLD_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_DISKSPACETHRESHOLD_PARMNUM)
SV_MAXLINKDELAY_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_MAXLINKDELAY_PARMNUM)
SV_MINLINKTHROUGHPUT_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_MINLINKTHROUGHPUT_PARMNUM)
SV_LINKINFOVALIDTIME_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_LINKINFOVALIDTIME_PARMNUM)
SV_SCAVQOSINFOUPDATETIME_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_SCAVQOSINFOUPDATETIME_PARMNUM)
SV_MAXWORKITEMIDLETIME_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_MAXWORKITEMIDLETIME_PARMNUM)
SV_MAXRAWWORKITEMS_INFOLOEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_MAXRAWWORKITEMS_PARMNUM)
SV_PRODUCTTYPE_INFOLOEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_PRODUCTTYPE_PARMNUM)
SV_SERVERSIZE_INFOLOEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_SERVERSIZE_PARMNUM)
SV_CONNECTIONLESSAUTODISC_INFOLOEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_CONNECTIONLESSAUTODISC_PARMNUM)
SV_SHARINGVIOLATIONRETRIES_INFOLOEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_SHARINGVIOLATIONRETRIES_PARMNUM)
SV_SHARINGVIOLATIONDELAY_INFOLOEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_SHARINGVIOLATIONDELAY_PARMNUM)
SV_MAXGLOBALOPENSEARCH_INFOLOEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_MAXGLOBALOPENSEARCH_PARMNUM)
SV_REMOVEDUPLICATESEARCHES_INFOLOEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_REMOVEDUPLICATESEARCHES_PARMNUM)
SV_LOCKVIOLATIONRETRIES_INFOLOEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_LOCKVIOLATIONRETRIES_PARMNUM)
SV_LOCKVIOLATIONOFFSET_INFOLOEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_LOCKVIOLATIONOFFSET_PARMNUM)
SV_LOCKVIOLATIONDELAY_INFOLOEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_LOCKVIOLATIONDELAY_PARMNUM)
SV_MDLREADSWITCHOVER_INFOLOEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_MDLREADSWITCHOVER_PARMNUM)
SV_CACHEDOPENLIMIT_INFOLOEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_CACHEDOPENLIMIT_PARMNUM)
SV_CRITICALTHREADS_INFOLOEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_CRITICALTHREADS_PARMNUM)
SV_RESTRICTNULLSESSACCESS_INFOLOEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_RESTRICTNULLSESSACCESS_PARMNUM)
SV_ENABLEWFW311DIRECTIPX_INFOLOEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_ENABLEWFW311DIRECTIPX_PARMNUM)
SV_OTHERQUEUEAFFINITY_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_OTHERQUEUEAFFINITY_PARMNUM)
SV_QUEUESAMPLESECS_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_QUEUESAMPLESECS_PARMNUM)
SV_BALANCECOUNT_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_BALANCECOUNT_PARMNUM)
SV_PREFERREDAFFINITY_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_PREFERREDAFFINITY_PARMNUM)
SV_MAXFREERFCBS_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_MAXFREERFCBS_PARMNUM)
SV_MAXFREEMFCBS_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_MAXFREEMFCBS_PARMNUM)
SV_MAXFREELFCBS_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_MAXFREELFCBS_PARMNUM)
SV_MAXFREEPAGEDPOOLCHUNKS_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_MAXFREEPAGEDPOOLCHUNKS_PARMNUM)
SV_MINPAGEDPOOLCHUNKSIZE_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_MINPAGEDPOOLCHUNKSIZE_PARMNUM)
SV_MAXPAGEDPOOLCHUNKSIZE_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_MAXPAGEDPOOLCHUNKSIZE_PARMNUM)
SV_SENDSFROMPREFERREDPROCESSOR_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_SENDSFROMPREFERREDPROCESSOR_PARMNUM)
SV_MAXTHREADSPERQUEUE_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_MAXTHREADSPERQUEUE_PARMNUM)
SV_CACHEDDIRECTORYLIMIT_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_CACHEDDIRECTORYLIMIT_PARMNUM)
SV_MAXCOPYLENGTH_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_MAXCOPYLENGTH_PARMNUM)
SV_ENABLEBULKTRANSFER_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_ENABLEBULKTRANSFER_PARMNUM)
SV_ENABLECOMPRESSION_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_ENABLECOMPRESSION_PARMNUM)
SV_AUTOSHAREWKS_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_AUTOSHAREWKS_PARMNUM)
SV_AUTOSHARESERVER_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_AUTOSHARESERVER_PARMNUM)
SV_ENABLESECURITYSIGNATURE_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_ENABLESECURITYSIGNATURE_PARMNUM)
SV_REQUIRESECURITYSIGNATURE_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_REQUIRESECURITYSIGNATURE_PARMNUM)
SV_MINCLIENTBUFFERSIZE_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_MINCLIENTBUFFERSIZE_PARMNUM)
SV_CONNECTIONNOSESSIONSTIMEOUT_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + SV_CONNECTIONNOSESSIONSTIMEOUT_PARMNUM)
SVI1_NUM_ELEMENTS equ 5
SVI2_NUM_ELEMENTS equ 40
SVI3_NUM_ELEMENTS equ 44
SV_MAX_CMD_LEN equ PATHLEN
SW_AUTOPROF_LOAD_MASK equ 1
SW_AUTOPROF_SAVE_MASK equ 2
SV_MAX_SRV_HEUR_LEN equ 32      
SV_USERS_PER_LICENSE equ 5
SVTI2_REMAP_PIPE_NAMES equ 2

SERVER_INFO_100 STRUCT
    sv100_platform_id dd ?
    sv100_name dd ?
SERVER_INFO_100 ENDS

SERVER_INFO_101 STRUCT
    sv101_platform_id dd ?
    sv101_name dd ?
    sv101_version_major dd ?
    sv101_version_minor dd ?
    sv101_type dd ?
    sv101_comment dd ?
SERVER_INFO_101 ENDS

SERVER_INFO_102 STRUCT
     sv102_platform_id dd ?
     sv102_name dd ?
     sv102_version_major dd ?
     sv102_version_minor dd ?
     sv102_type dd ?
     sv102_comment dd ?
     sv102_users dd ?
     sv102_disc dd ?
     sv102_hidden dd ?
     sv102_announce dd ?
     sv102_anndelta dd ?
     sv102_licenses dd ?
     sv102_userpath dd ?
SERVER_INFO_102 ENDS

SERVER_INFO_402  STRUCT
     sv402_ulist_mtime dd ?
     sv402_glist_mtime dd ?
     sv402_alist_mtime dd ?
     sv402_alerts dd ?
     sv402_security dd ?
     sv402_numadmin dd ?
     sv402_lanmask dd ?
     sv402_guestacct dd ?
     sv402_chdevs dd ?
     sv402_chdevq dd ?
     sv402_chdevjobs dd ?
     sv402_connections dd ?
     sv402_shares dd ?
     sv402_openfiles dd ?
     sv402_sessopens dd ?
     sv402_sessvcs dd ?
     sv402_sessreqs dd ?
     sv402_opensearch dd ?
     sv402_activelocks dd ?
     sv402_numreqbuf dd ?
     sv402_sizreqbuf dd ?
     sv402_numbigbuf dd ?
     sv402_numfiletasks dd ?
     sv402_alertsched dd ?
     sv402_erroralert dd ?
     sv402_logonalert dd ?
     sv402_accessalert dd ?
     sv402_diskalert dd ?
     sv402_netioalert dd ?
     sv402_maxauditsz dd ?
     sv402_srvheuristics dd ?
SERVER_INFO_402 ENDS

SERVER_INFO_403 STRUCT
     sv403_ulist_mtime dd ?
     sv403_glist_mtime dd ?
     sv403_alist_mtime dd ?
     sv403_alerts dd ?
     sv403_security dd ?
     sv403_numadmin dd ?
     sv403_lanmask dd ?
     sv403_guestacct dd ?
     sv403_chdevs dd ?
     sv403_chdevq dd ?
     sv403_chdevjobs dd ?
     sv403_connections dd ?
     sv403_shares dd ?
     sv403_openfiles dd ?
     sv403_sessopens dd ?
     sv403_sessvcs dd ?
     sv403_sessreqs dd ?
     sv403_opensearch dd ?
     sv403_activelocks dd ?
     sv403_numreqbuf dd ?
     sv403_sizreqbuf dd ?
     sv403_numbigbuf dd ?
     sv403_numfiletasks dd ?
     sv403_alertsched dd ?
     sv403_erroralert dd ?
     sv403_logonalert dd ?
     sv403_accessalert dd ?
     sv403_diskalert dd ?
     sv403_netioalert dd ?
     sv403_maxauditsz dd ?
     sv403_srvheuristics dd ?
     sv403_auditedevents dd ?
     sv403_autoprofile dd ?
     sv403_autopath dd ?
SERVER_INFO_403 ENDS

SERVER_INFO_502 STRUCT
    sv502_sessopens dd ?
    sv502_sessvcs dd ?
    sv502_opensearch dd ?
    sv502_sizreqbuf dd ?
    sv502_initworkitems dd ?
    sv502_maxworkitems dd ?
    sv502_rawworkitems dd ?
    sv502_irpstacksize dd ?
    sv502_maxrawbuflen dd ?
    sv502_sessusers dd ?
    sv502_sessconns dd ?
    sv502_maxpagedmemoryusage dd ?
    sv502_maxnonpagedmemoryusage dd ?
    sv502_enablesoftcompat dd ?
    sv502_enableforcedlogoff dd ?
    sv502_timesource dd ?
    sv502_acceptdownlevelapis dd ?
    sv502_lmannounce dd ?
SERVER_INFO_502 ENDS

SERVER_INFO_503 STRUCT
    sv503_sessopens dd ?
    sv503_sessvcs dd ?
    sv503_opensearch dd ?
    sv503_sizreqbuf dd ?
    sv503_initworkitems dd ?
    sv503_maxworkitems dd ?
    sv503_rawworkitems dd ?
    sv503_irpstacksize dd ?
    sv503_maxrawbuflen dd ?
    sv503_sessusers dd ?
    sv503_sessconns dd ?
    sv503_maxpagedmemoryusage dd ?
    sv503_maxnonpagedmemoryusage dd ?
    sv503_enablesoftcompat dd ?
    sv503_enableforcedlogoff dd ?
    sv503_timesource dd ?
    sv503_acceptdownlevelapis dd ?
    sv503_lmannounce dd ?
    sv503_domain dd ?
    sv503_maxcopyreadlen dd ?
    sv503_maxcopywritelen dd ?
    sv503_minkeepsearch dd ?
    sv503_maxkeepsearch dd ?
    sv503_minkeepcomplsearch dd ?
    sv503_maxkeepcomplsearch dd ?
    sv503_threadcountadd dd ?
    sv503_numblockthreads dd ?
    sv503_scavtimeout dd ?
    sv503_minrcvqueue dd ?
    sv503_minfreeworkitems dd ?
    sv503_xactmemsize dd ?
    sv503_threadpriority dd ?
    sv503_maxmpxct dd ?
    sv503_oplockbreakwait dd ?
    sv503_oplockbreakresponsewait dd ?
    sv503_enableoplocks dd ?
    sv503_enableoplockforceclose dd ?
    sv503_enablefcbopens dd ?
    sv503_enableraw dd ?
    sv503_enablesharednetdrives dd ?
    sv503_minfreeconnections dd ?
    sv503_maxfreeconnections dd ?
SERVER_INFO_503 ENDS

SERVER_INFO_599 STRUCT
    sv599_sessopens dd ?
    sv599_sessvcs dd ?
    sv599_opensearch dd ?
    sv599_sizreqbuf dd ?
    sv599_initworkitems dd ?
    sv599_maxworkitems dd ?
    sv599_rawworkitems dd ?
    sv599_irpstacksize dd ?

    sv599_maxrawbuflen dd ?
    sv599_sessusers dd ?
    sv599_sessconns dd ?
    sv599_maxpagedmemoryusage dd ?
    sv599_maxnonpagedmemoryusage dd ?
    sv599_enablesoftcompat dd ?
    sv599_enableforcedlogoff dd ?
    sv599_timesource dd ?
    sv599_acceptdownlevelapis dd ?
    sv599_lmannounce dd ?
    sv599_domain dd ?
    sv599_maxcopyreadlen dd ?
    sv599_maxcopywritelen dd ?
    sv599_minkeepsearch dd ?
    sv599_maxkeepsearch dd ?
    sv599_minkeepcomplsearch dd ?
    sv599_maxkeepcomplsearch dd ?
    sv599_threadcountadd dd ?
    sv599_numblockthreads dd ?
    sv599_scavtimeout dd ?
    sv599_minrcvqueue dd ?
    sv599_minfreeworkitems dd ?
    sv599_xactmemsize dd ?
    sv599_threadpriority dd ?
    sv599_maxmpxct dd ?
    sv599_oplockbreakwait dd ?
    sv599_oplockbreakresponsewait dd ?
    sv599_enableoplocks dd ?
    sv599_enableoplockforceclose dd ?
    sv599_enablefcbopens dd ?
    sv599_enableraw dd ?
    sv599_enablesharednetdrives dd ?
    sv599_minfreeconnections dd ?
    sv599_maxfreeconnections dd ?
    sv599_initsesstable dd ?
    sv599_initconntable dd ?
    sv599_initfiletable dd ?
    sv599_initsearchtable dd ?
    sv599_alertschedule dd ?
    sv599_errorthreshold dd ?
    sv599_networkerrorthreshold dd ?
    sv599_diskspacethreshold dd ?
    sv599_reserved dd ?
    sv599_maxlinkdelay dd ?
    sv599_minlinkthroughput dd ?
    sv599_linkinfovalidtime dd ?
    sv599_scavqosinfoupdatetime dd ?
    sv599_maxworkitemidletime dd ?
SERVER_INFO_599 ENDS

SERVER_INFO_598 STRUCT
    sv598_maxrawworkitems dd ?
    sv598_maxthreadsperqueue dd ?
    sv598_producttype dd ?
    sv598_serversize dd ?
    sv598_connectionlessautodisc dd ?
    sv598_sharingviolationretries dd ?
    sv598_sharingviolationdelay dd ?
    sv598_maxglobalopensearch dd ?
    sv598_removeduplicatesearches dd ?
    sv598_lockviolationoffset dd ?
    sv598_lockviolationdelay dd ?
    sv598_mdlreadswitchover dd ?
    sv598_cachedopenlimit dd ?
    sv598_otherqueueaffinity dd ?
    sv598_restrictnullsessaccess dd ?
    sv598_enablewfw311directipx dd ?
    sv598_queuesamplesecs dd ?
    sv598_balancecount dd ?
    sv598_preferredaffinity dd ?
    sv598_maxfreerfcbs dd ?
    sv598_maxfreemfcbs dd ?
    sv598_maxfreelfcbs dd ?
    sv598_maxfreepagedpoolchunks dd ?
    sv598_minpagedpoolchunksize dd ?
    sv598_maxpagedpoolchunksize dd ?
    sv598_sendsfrompreferredprocessor dd ?
    sv598_cacheddirectorylimit dd ?
    sv598_maxcopylength dd ?
    sv598_enablebulktransfer dd ?
    sv598_enablecompression dd ?
    sv598_autosharewks dd ?
    sv598_autoshareserver dd ?
    sv598_enablesecuritysignature dd ?
    sv598_requiresecuritysignature dd ?
    sv598_minclientbuffersize dd ?
    sv598_serverguid GUID <>
    sv598_ConnectionNoSessionsTimeout dd ?
SERVER_INFO_598 ENDS

SERVER_INFO_1005 STRUCT
    sv1005_comment dd ?
SERVER_INFO_1005 ENDS

SERVER_INFO_1107 STRUCT
    sv1107_users dd ?
SERVER_INFO_1107 ENDS

SERVER_INFO_1010 STRUCT
    sv1010_disc dd ?
SERVER_INFO_1010 ENDS

SERVER_INFO_1016 STRUCT
    sv1016_hidden dd ?
SERVER_INFO_1016 ENDS

SERVER_INFO_1017 STRUCT
    sv1017_announce dd ?
SERVER_INFO_1017 ENDS

SERVER_INFO_1018 STRUCT
    sv1018_anndelta dd ?
SERVER_INFO_1018 ENDS

SERVER_INFO_1501 STRUCT
    sv1501_sessopens dd ?
SERVER_INFO_1501 ENDS

SERVER_INFO_1502 STRUCT
    sv1502_sessvcs dd ?
SERVER_INFO_1502 ENDS

SERVER_INFO_1503 STRUCT
    sv1503_opensearch dd ?
SERVER_INFO_1503 ENDS

SERVER_INFO_1506 STRUCT
    sv1506_maxworkitems dd ?
SERVER_INFO_1506 ENDS

SERVER_INFO_1509 STRUCT
    sv1509_maxrawbuflen dd ?
SERVER_INFO_1509 ENDS

SERVER_INFO_1510 STRUCT
    sv1510_sessusers dd ?
SERVER_INFO_1510 ENDS

SERVER_INFO_1511 STRUCT
    sv1511_sessconns dd ?
SERVER_INFO_1511 ENDS

SERVER_INFO_1512 STRUCT
    sv1512_maxnonpagedmemoryusage dd ?
SERVER_INFO_1512 ENDS

SERVER_INFO_1513 STRUCT
    sv1513_maxpagedmemoryusage dd ?
SERVER_INFO_1513 ENDS

SERVER_INFO_1514 STRUCT
    sv1514_enablesoftcompat dd ?
SERVER_INFO_1514 ENDS

SERVER_INFO_1515 STRUCT
    sv1515_enableforcedlogoff dd ?
SERVER_INFO_1515 ENDS

SERVER_INFO_1516 STRUCT
    sv1516_timesource dd ?
SERVER_INFO_1516 ENDS

SERVER_INFO_1518 STRUCT
    sv1518_lmannounce dd ?
SERVER_INFO_1518 ENDS

SERVER_INFO_1520 STRUCT
    sv1520_maxcopyreadlen dd ?
SERVER_INFO_1520 ENDS

SERVER_INFO_1521 STRUCT
    sv1521_maxcopywritelen dd ?
SERVER_INFO_1521 ENDS

SERVER_INFO_1522 STRUCT
    sv1522_minkeepsearch dd ?
SERVER_INFO_1522 ENDS

SERVER_INFO_1523 STRUCT
    sv1523_maxkeepsearch dd ?
SERVER_INFO_1523 ENDS

SERVER_INFO_1524 STRUCT
    sv1524_minkeepcomplsearch dd ?
SERVER_INFO_1524 ENDS

SERVER_INFO_1525 STRUCT
    sv1525_maxkeepcomplsearch dd ?
SERVER_INFO_1525 ENDS

SERVER_INFO_1528 STRUCT
    sv1528_scavtimeout dd ?
SERVER_INFO_1528 ENDS

SERVER_INFO_1529 STRUCT
    sv1529_minrcvqueue dd ?
SERVER_INFO_1529 ENDS

SERVER_INFO_1530 STRUCT
    sv1530_minfreeworkitems dd ?
SERVER_INFO_1530 ENDS

SERVER_INFO_1533 STRUCT
    sv1533_maxmpxct dd ?
SERVER_INFO_1533 ENDS

SERVER_INFO_1534 STRUCT
    sv1534_oplockbreakwait dd ?
SERVER_INFO_1534 ENDS

SERVER_INFO_1535 STRUCT
    sv1535_oplockbreakresponsewait dd ?
SERVER_INFO_1535 ENDS

SERVER_INFO_1536 STRUCT
    sv1536_enableoplocks dd ?
SERVER_INFO_1536 ENDS

SERVER_INFO_1537 STRUCT
    sv1537_enableoplockforceclose dd ?
SERVER_INFO_1537 ENDS

SERVER_INFO_1538 STRUCT
    sv1538_enablefcbopens dd ?
SERVER_INFO_1538 ENDS

SERVER_INFO_1539 STRUCT
    sv1539_enableraw dd ?
SERVER_INFO_1539 ENDS

SERVER_INFO_1540 STRUCT
    sv1540_enablesharednetdrives dd ?
SERVER_INFO_1540 ENDS

SERVER_INFO_1541 STRUCT
    sv1541_minfreeconnections dd ?
SERVER_INFO_1541 ENDS

SERVER_INFO_1542 STRUCT
    sv1542_maxfreeconnections dd ?
SERVER_INFO_1542 ENDS

SERVER_INFO_1543 STRUCT
    sv1543_initsesstable dd ?
SERVER_INFO_1543 ENDS

SERVER_INFO_1544 STRUCT
    sv1544_initconntable dd ?
SERVER_INFO_1544 ENDS

SERVER_INFO_1545 STRUCT
    sv1545_initfiletable dd ?
SERVER_INFO_1545 ENDS

SERVER_INFO_1546 STRUCT
    sv1546_initsearchtable dd ?
SERVER_INFO_1546 ENDS

SERVER_INFO_1547 STRUCT
    sv1547_alertschedule dd ?
SERVER_INFO_1547 ENDS

SERVER_INFO_1548 STRUCT
    sv1548_errorthreshold dd ?
SERVER_INFO_1548 ENDS

SERVER_INFO_1549 STRUCT
    sv1549_networkerrorthreshold dd ?
SERVER_INFO_1549 ENDS

SERVER_INFO_1550 STRUCT
    sv1550_diskspacethreshold dd ?
SERVER_INFO_1550 ENDS

SERVER_INFO_1552 STRUCT
    sv1552_maxlinkdelay dd ?
SERVER_INFO_1552 ENDS

SERVER_INFO_1553 STRUCT
    sv1553_minlinkthroughput dd ?
SERVER_INFO_1553 ENDS

SERVER_INFO_1554 STRUCT
    sv1554_linkinfovalidtime dd ?
SERVER_INFO_1554 ENDS

SERVER_INFO_1555 STRUCT
    sv1555_scavqosinfoupdatetime dd ?
SERVER_INFO_1555 ENDS

SERVER_INFO_1556 STRUCT
    sv1556_maxworkitemidletime dd ?
SERVER_INFO_1556 ENDS

SERVER_INFO_1557 STRUCT
    sv1557_maxrawworkitems dd ?
SERVER_INFO_1557 ENDS

SERVER_INFO_1560 STRUCT
    sv1560_producttype dd ?
SERVER_INFO_1560 ENDS

SERVER_INFO_1561 STRUCT
    sv1561_serversize dd ?
SERVER_INFO_1561 ENDS

SERVER_INFO_1562 STRUCT
    sv1562_connectionlessautodisc dd ?
SERVER_INFO_1562 ENDS

SERVER_INFO_1563 STRUCT
    sv1563_sharingviolationretries dd ?
SERVER_INFO_1563 ENDS

SERVER_INFO_1564 STRUCT
    sv1564_sharingviolationdelay dd ?
SERVER_INFO_1564 ENDS

SERVER_INFO_1565 STRUCT
    sv1565_maxglobalopensearch dd ?
SERVER_INFO_1565 ENDS

SERVER_INFO_1566 STRUCT
    sv1566_removeduplicatesearches dd ?
SERVER_INFO_1566 ENDS

SERVER_INFO_1567 STRUCT
    sv1567_lockviolationretries dd ?
SERVER_INFO_1567 ENDS

SERVER_INFO_1568 STRUCT
    sv1568_lockviolationoffset dd ?
SERVER_INFO_1568 ENDS

SERVER_INFO_1569 STRUCT
    sv1569_lockviolationdelay dd ?
SERVER_INFO_1569 ENDS

SERVER_INFO_1570 STRUCT
    sv1570_mdlreadswitchover dd ?
SERVER_INFO_1570 ENDS

SERVER_INFO_1571 STRUCT
    sv1571_cachedopenlimit dd ?
SERVER_INFO_1571 ENDS

SERVER_INFO_1572 STRUCT
    sv1572_criticalthreads dd ?
SERVER_INFO_1572 ENDS

SERVER_INFO_1573 STRUCT
    sv1573_restrictnullsessaccess dd ?
SERVER_INFO_1573 ENDS

SERVER_INFO_1574 STRUCT
    sv1574_enablewfw311directipx dd ?
SERVER_INFO_1574 ENDS

SERVER_INFO_1575 STRUCT
    sv1575_otherqueueaffinity dd ?
SERVER_INFO_1575 ENDS

SERVER_INFO_1576 STRUCT
    sv1576_queuesamplesecs dd ?
SERVER_INFO_1576 ENDS

SERVER_INFO_1577 STRUCT
    sv1577_balancecount dd ?
SERVER_INFO_1577 ENDS

SERVER_INFO_1578 STRUCT
    sv1578_preferredaffinity dd ?
SERVER_INFO_1578 ENDS

SERVER_INFO_1579 STRUCT
    sv1579_maxfreerfcbs dd ?
SERVER_INFO_1579 ENDS

SERVER_INFO_1580 STRUCT
    sv1580_maxfreemfcbs dd ?
SERVER_INFO_1580 ENDS

SERVER_INFO_1581 STRUCT
    sv1581_maxfreemlcbs dd ?
SERVER_INFO_1581 ENDS

SERVER_INFO_1582 STRUCT
    sv1582_maxfreepagedpoolchunks dd ?
SERVER_INFO_1582 ENDS

SERVER_INFO_1583 STRUCT
    sv1583_minpagedpoolchunksize dd ?
SERVER_INFO_1583 ENDS

SERVER_INFO_1584 STRUCT
    sv1584_maxpagedpoolchunksize dd ?
SERVER_INFO_1584 ENDS

SERVER_INFO_1585 STRUCT
    sv1585_sendsfrompreferredprocessor dd ?
SERVER_INFO_1585 ENDS

SERVER_INFO_1586 STRUCT
    sv1586_maxthreadsperqueue dd ?
SERVER_INFO_1586 ENDS

SERVER_INFO_1587 STRUCT
    sv1587_cacheddirectorylimit dd ?
SERVER_INFO_1587 ENDS

SERVER_INFO_1588 STRUCT
    sv1588_maxcopylength dd ?
SERVER_INFO_1588 ENDS

SERVER_INFO_1589 STRUCT
    sv1589_enablebulktransfer dd ?
SERVER_INFO_1589 ENDS

SERVER_INFO_1590 STRUCT
    sv1590_enablecompression dd ?
SERVER_INFO_1590 ENDS

SERVER_INFO_1591 STRUCT
    sv1591_autosharewks dd ?
SERVER_INFO_1591 ENDS

SERVER_INFO_1592 STRUCT
    sv1592_autosharewks dd ?
SERVER_INFO_1592 ENDS

SERVER_INFO_1593 STRUCT
    sv1593_enablesecuritysignature dd ?
SERVER_INFO_1593 ENDS

SERVER_INFO_1594 STRUCT
    sv1594_requiresecuritysignature dd ?
SERVER_INFO_1594 ENDS

SERVER_INFO_1595 STRUCT
    sv1595_minclientbuffersize dd ?
SERVER_INFO_1595 ENDS

SERVER_INFO_1596 STRUCT
    sv1596_ConnectionNoSessionsTimeout dd ?
SERVER_INFO_1596 ENDS

SERVER_TRANSPORT_INFO_0 STRUCT
    svti0_numberofvcs dd ?
    svti0_transportname dd ?
    svti0_transportaddress dd ?
    svti0_transportaddresslength dd ?
    svti0_networkaddress dd ?
SERVER_TRANSPORT_INFO_0 ENDS

SERVER_TRANSPORT_INFO_1 STRUCT
    svti1_numberofvcs dd ?
    svti1_transportname dd ?
    svti1_transportaddress dd ?
    svti1_transportaddresslength dd ?
    svti1_networkaddress dd ?
    svti1_domain dd ?
SERVER_TRANSPORT_INFO_1 ENDS

SERVER_TRANSPORT_INFO_2 STRUCT
    svti2_numberofvcs dd ?
    svti2_transportname dd ?
    svti2_transportaddress dd ?
    svti2_transportaddresslength dd ?
    svti2_networkaddress dd ?
    svti2_domain dd ?
    svti2_flags dd ?
SERVER_TRANSPORT_INFO_2 ENDS

;------------------------------------------------------ LMERR.H --------------------------------------------
NERR_Success equ 0
NERR_BASE equ 2100
NERR_NetNotStarted equ (NERR_BASE+2)
NERR_UnknownServer equ (NERR_BASE+3)
NERR_ShareMem equ (NERR_BASE+4)
NERR_NoNetworkResource equ (NERR_BASE+5)
NERR_RemoteOnly equ (NERR_BASE+6)
NERR_DevNotRedirected equ (NERR_BASE+7)
NERR_ServerNotStarted equ (NERR_BASE+14)
NERR_ItemNotFound equ (NERR_BASE+15)
NERR_UnknownDevDir equ (NERR_BASE+16)
NERR_RedirectedPath equ (NERR_BASE+17)
NERR_DuplicateShare equ (NERR_BASE+18)
NERR_NoRoom equ (NERR_BASE+19)
NERR_TooManyItems equ (NERR_BASE+21)
NERR_InvalidMaxUsers equ (NERR_BASE+22)
NERR_BufTooSmall equ (NERR_BASE+23)
NERR_RemoteErr equ (NERR_BASE+27)
NERR_LanmanIniError equ (NERR_BASE+31)
NERR_NetworkError equ (NERR_BASE+36)
NERR_WkstaInconsistentState equ (NERR_BASE+37)
NERR_WkstaNotStarted equ (NERR_BASE+38)
NERR_BrowserNotStarted equ (NERR_BASE+39)
NERR_InternalError equ (NERR_BASE+40)
NERR_BadTransactConfig equ (NERR_BASE+41)
NERR_InvalidAPI equ (NERR_BASE+42)
NERR_BadEventName equ (NERR_BASE+43)
NERR_DupNameReboot equ (NERR_BASE+44)
NERR_CfgCompNotFound equ (NERR_BASE+46)
NERR_CfgParamNotFound equ (NERR_BASE+47)
NERR_LineTooLong equ (NERR_BASE+49)
NERR_QNotFound equ (NERR_BASE+50)
NERR_JobNotFound equ (NERR_BASE+51)
NERR_DestNotFound equ (NERR_BASE+52)
NERR_DestExists equ (NERR_BASE+53)
NERR_QExists equ (NERR_BASE+54)
NERR_QNoRoom equ (NERR_BASE+55)
NERR_JobNoRoom equ (NERR_BASE+56)
NERR_DestNoRoom equ (NERR_BASE+57)
NERR_DestIdle equ (NERR_BASE+58)
NERR_DestInvalidOp equ (NERR_BASE+59)
NERR_ProcNoRespond equ (NERR_BASE+60)
NERR_SpoolerNotLoaded equ (NERR_BASE+61)
NERR_DestInvalidState equ (NERR_BASE+62)
NERR_QInvalidState equ (NERR_BASE+63)
NERR_JobInvalidState equ (NERR_BASE+64)
NERR_SpoolNoMemory equ (NERR_BASE+65)
NERR_DriverNotFound equ (NERR_BASE+66)
NERR_DataTypeInvalid equ (NERR_BASE+67)
NERR_ProcNotFound equ (NERR_BASE+68)
NERR_ServiceTableLocked equ (NERR_BASE+80)
NERR_ServiceTableFull equ (NERR_BASE+81)
NERR_ServiceInstalled equ (NERR_BASE+82)
NERR_ServiceEntryLocked equ (NERR_BASE+83)
NERR_ServiceNotInstalled equ (NERR_BASE+84)
NERR_BadServiceName equ (NERR_BASE+85)
NERR_ServiceCtlTimeout equ (NERR_BASE+86)
NERR_ServiceCtlBusy equ (NERR_BASE+87)
NERR_BadServiceProgName equ (NERR_BASE+88)
NERR_ServiceNotCtrl equ (NERR_BASE+89)
NERR_ServiceKillProc equ (NERR_BASE+90)
NERR_ServiceCtlNotValid equ (NERR_BASE+91)
NERR_NotInDispatchTbl equ (NERR_BASE+92) 
NERR_BadControlRecv equ (NERR_BASE+93)
NERR_ServiceNotStarting equ (NERR_BASE+94)
NERR_AlreadyLoggedOn equ (NERR_BASE+100)
NERR_NotLoggedOn equ (NERR_BASE+101)
NERR_BadUsername equ (NERR_BASE+102)
NERR_BadPassword equ (NERR_BASE+103)
NERR_UnableToAddName_W equ (NERR_BASE+104)
NERR_UnableToAddName_F equ (NERR_BASE+105)
NERR_UnableToDelName_W equ (NERR_BASE+106)
NERR_UnableToDelName_F equ (NERR_BASE+107)
NERR_LogonsPaused equ (NERR_BASE+109)
NERR_LogonServerConflict equ (NERR_BASE+110)
NERR_LogonNoUserPath equ (NERR_BASE+111)
NERR_LogonScriptError equ (NERR_BASE+112)
NERR_StandaloneLogon equ (NERR_BASE+114)
NERR_LogonServerNotFound equ (NERR_BASE+115)
NERR_LogonDomainExists equ (NERR_BASE+116)

NERR_NonValidatedLogon equ (NERR_BASE+117)
NERR_ACFNotFound equ (NERR_BASE+119)
NERR_GroupNotFound equ (NERR_BASE+120)
NERR_UserNotFound equ (NERR_BASE+121)
NERR_ResourceNotFound equ (NERR_BASE+122)
NERR_GroupExists equ (NERR_BASE+123)
NERR_UserExists equ (NERR_BASE+124)
NERR_ResourceExists equ (NERR_BASE+125)
NERR_NotPrimary equ (NERR_BASE+126)
NERR_ACFNotLoaded equ (NERR_BASE+127)
NERR_ACFNoRoom equ (NERR_BASE+128)
NERR_ACFFileIOFail equ (NERR_BASE+129)
NERR_ACFTooManyLists equ (NERR_BASE+130)
NERR_UserLogon equ (NERR_BASE+131)
NERR_ACFNoParent equ (NERR_BASE+132)
NERR_CanNotGrowSegment equ (NERR_BASE+133)
NERR_SpeGroupOp equ (NERR_BASE+134)
NERR_NotInCache equ (NERR_BASE+135)
NERR_UserInGroup equ (NERR_BASE+136)
NERR_UserNotInGroup equ (NERR_BASE+137) 
NERR_AccountUndefined equ (NERR_BASE+138) 
NERR_AccountExpired equ (NERR_BASE+139) 
NERR_InvalidWorkstation equ (NERR_BASE+140) 
NERR_InvalidLogonHours equ (NERR_BASE+141) 
NERR_PasswordExpired equ (NERR_BASE+142) 
NERR_PasswordCantChange equ (NERR_BASE+143) 
NERR_PasswordHistConflict equ (NERR_BASE+144) 
NERR_PasswordTooShort equ (NERR_BASE+145) 
NERR_PasswordTooRecent equ (NERR_BASE+146) 
NERR_InvalidDatabase equ (NERR_BASE+147) 
NERR_DatabaseUpToDate equ (NERR_BASE+148) 
NERR_SyncRequired equ (NERR_BASE+149) 
NERR_UseNotFound equ (NERR_BASE+150) 
NERR_BadAsgType equ (NERR_BASE+151) 
NERR_DeviceIsShared equ (NERR_BASE+152) 
NERR_NoComputerName equ (NERR_BASE+170) 
NERR_MsgAlreadyStarted equ (NERR_BASE+171) 
NERR_MsgInitFailed equ (NERR_BASE+172) 
NERR_NameNotFound equ (NERR_BASE+173) 
NERR_AlreadyForwarded equ (NERR_BASE+174) 
NERR_AddForwarded equ (NERR_BASE+175) 
NERR_AlreadyExists equ (NERR_BASE+176) 
NERR_TooManyNames equ (NERR_BASE+177) 
NERR_DelComputerName equ (NERR_BASE+178) 
NERR_LocalForward equ (NERR_BASE+179) 
NERR_GrpMsgProcessor equ (NERR_BASE+180) 
NERR_PausedRemote equ (NERR_BASE+181) 
NERR_BadReceive equ (NERR_BASE+182) 
NERR_NameInUse equ (NERR_BASE+183) 
NERR_MsgNotStarted equ (NERR_BASE+184) 
NERR_NotLocalName equ (NERR_BASE+185) 
NERR_NoForwardName equ (NERR_BASE+186) 
NERR_RemoteFull equ (NERR_BASE+187) 
NERR_NameNotForwarded equ (NERR_BASE+188) 
NERR_TruncatedBroadcast equ (NERR_BASE+189) 
NERR_InvalidDevice equ (NERR_BASE+194) 
NERR_WriteFault equ (NERR_BASE+195) 
NERR_DuplicateName equ (NERR_BASE+197) 
NERR_DeleteLater equ (NERR_BASE+198) 
NERR_IncompleteDel equ (NERR_BASE+199) 
NERR_MultipleNets equ (NERR_BASE+200) 
NERR_NetNameNotFound equ (NERR_BASE+210) 
NERR_DeviceNotShared equ (NERR_BASE+211) 
NERR_ClientNameNotFound equ (NERR_BASE+212) 
NERR_FileIdNotFound equ (NERR_BASE+214) 
NERR_ExecFailure equ (NERR_BASE+215) 
NERR_TmpFile equ (NERR_BASE+216) 
NERR_TooMuchData equ (NERR_BASE+217) 
NERR_DeviceShareConflict equ (NERR_BASE+218) 
NERR_BrowserTableIncomplete equ (NERR_BASE+219)  
NERR_NotLocalDomain equ (NERR_BASE+220) 
NERR_IsDfsShare equ (NERR_BASE+221) 
NERR_DevInvalidOpCode equ (NERR_BASE+231) 
NERR_DevNotFound equ (NERR_BASE+232) 
NERR_DevNotOpen equ (NERR_BASE+233) 
NERR_BadQueueDevString equ (NERR_BASE+234) 
NERR_BadQueuePriority equ (NERR_BASE+235) 
NERR_NoCommDevs equ (NERR_BASE+237) 
NERR_QueueNotFound equ (NERR_BASE+238) 
NERR_BadDevString equ (NERR_BASE+240) 
NERR_BadDev equ (NERR_BASE+241) 
NERR_InUseBySpooler equ (NERR_BASE+242) 
NERR_CommDevInUse equ (NERR_BASE+243) 
NERR_InvalidComputer equ (NERR_BASE+251) 
NERR_MaxLenExceeded equ (NERR_BASE+254) 
NERR_BadComponent equ (NERR_BASE+256) 
NERR_CantType equ (NERR_BASE+257) 
NERR_TooManyEntries equ (NERR_BASE+262) 
NERR_ProfileFileTooBig equ (NERR_BASE+270) 
NERR_ProfileOffset equ (NERR_BASE+271) 
NERR_ProfileCleanup equ (NERR_BASE+272) 
NERR_ProfileUnknownCmd equ (NERR_BASE+273) 
NERR_ProfileLoadErr equ (NERR_BASE+274) 
NERR_ProfileSaveErr equ (NERR_BASE+275) 
NERR_LogOverflow equ (NERR_BASE+277)      
NERR_LogFileChanged equ (NERR_BASE+278)      
NERR_LogFileCorrupt equ (NERR_BASE+279)      
NERR_SourceIsDir equ (NERR_BASE+280) 
NERR_BadSource equ (NERR_BASE+281) 
NERR_BadDest equ (NERR_BASE+282) 
NERR_DifferentServers equ (NERR_BASE+283) 
NERR_RunSrvPaused equ (NERR_BASE+285) 
NERR_ErrCommRunSrv equ (NERR_BASE+289) 
NERR_ErrorExecingGhost equ (NERR_BASE+291) 
NERR_ShareNotFound equ (NERR_BASE+292) 
NERR_InvalidLana equ (NERR_BASE+300) 
NERR_OpenFiles equ (NERR_BASE+301) 
NERR_ActiveConns equ (NERR_BASE+302) 
NERR_BadPasswordCore equ (NERR_BASE+303) 
NERR_DevInUse equ (NERR_BASE+304) 
NERR_LocalDrive equ (NERR_BASE+305) 
NERR_AlertExists equ (NERR_BASE+330) 
NERR_TooManyAlerts equ (NERR_BASE+331) 
NERR_NoSuchAlert equ (NERR_BASE+332) 
NERR_BadRecipient equ (NERR_BASE+333) 
NERR_AcctLimitExceeded equ (NERR_BASE+334)
NERR_InvalidLogSeek equ (NERR_BASE+340) 
NERR_BadUasConfig equ (NERR_BASE+350) 
NERR_InvalidUASOp equ (NERR_BASE+351) 
NERR_LastAdmin equ (NERR_BASE+352) 
NERR_DCNotFound equ (NERR_BASE+353) 
NERR_LogonTrackingError equ (NERR_BASE+354) 
NERR_NetlogonNotStarted equ (NERR_BASE+355) 
NERR_CanNotGrowUASFile equ (NERR_BASE+356) 
NERR_TimeDiffAtDC equ (NERR_BASE+357) 
NERR_PasswordMismatch equ (NERR_BASE+358) 
NERR_NoSuchServer equ (NERR_BASE+360) 
NERR_NoSuchSession equ (NERR_BASE+361) 
NERR_NoSuchConnection equ (NERR_BASE+362) 
NERR_TooManyServers equ (NERR_BASE+363) 
NERR_TooManySessions equ (NERR_BASE+364) 
NERR_TooManyConnections equ (NERR_BASE+365) 
NERR_TooManyFiles equ (NERR_BASE+366) 
NERR_NoAlternateServers equ (NERR_BASE+367) 
NERR_TryDownLevel equ (NERR_BASE+370) 
NERR_UPSDriverNotStarted equ (NERR_BASE+380) 
NERR_UPSInvalidConfig equ (NERR_BASE+381) 
NERR_UPSInvalidCommPort equ (NERR_BASE+382) 
NERR_UPSSignalAsserted equ (NERR_BASE+383) 
NERR_UPSShutdownFailed equ (NERR_BASE+384) 
NERR_BadDosRetCode equ (NERR_BASE+400) 
NERR_ProgNeedsExtraMem equ (NERR_BASE+401) 
NERR_BadDosFunction equ (NERR_BASE+402) 
NERR_RemoteBootFailed equ (NERR_BASE+403) 
NERR_BadFileCheckSum equ (NERR_BASE+404) 
NERR_NoRplBootSystem equ (NERR_BASE+405) 
NERR_RplLoadrNetBiosErr equ (NERR_BASE+406) 
NERR_RplLoadrDiskErr equ (NERR_BASE+407) 
NERR_ImageParamErr equ (NERR_BASE+408) 
NERR_TooManyImageParams equ (NERR_BASE+409) 
NERR_NonDosFloppyUsed equ (NERR_BASE+410) 
NERR_RplBootRestart equ (NERR_BASE+411) 
NERR_RplSrvrCallFailed equ (NERR_BASE+412) 
NERR_CantConnectRplSrvr equ (NERR_BASE+413) 
NERR_CantOpenImageFile equ (NERR_BASE+414) 
NERR_CallingRplSrvr equ (NERR_BASE+415) 
NERR_StartingRplBoot equ (NERR_BASE+416) 
NERR_RplBootServiceTerm equ (NERR_BASE+417) 
NERR_RplBootStartFailed equ (NERR_BASE+418) 
NERR_RPL_CONNECTED equ (NERR_BASE+419) 
NERR_BrowserConfiguredToNotRun equ (NERR_BASE+450) 
NERR_RplNoAdaptersStarted equ (NERR_BASE+510) 
NERR_RplBadRegistry equ (NERR_BASE+511) 
NERR_RplBadDatabase equ (NERR_BASE+512) 
NERR_RplRplfilesShare equ (NERR_BASE+513) 
NERR_RplNotRplServer equ (NERR_BASE+514) 
NERR_RplCannotEnum equ (NERR_BASE+515) 
NERR_RplWkstaInfoCorrupted equ (NERR_BASE+516) 
NERR_RplWkstaNotFound equ (NERR_BASE+517) 
NERR_RplWkstaNameUnavailable equ (NERR_BASE+518) 
NERR_RplProfileInfoCorrupted equ (NERR_BASE+519) 
NERR_RplProfileNotFound equ (NERR_BASE+520) 
NERR_RplProfileNameUnavailable equ (NERR_BASE+521) 
NERR_RplProfileNotEmpty equ (NERR_BASE+522) 
NERR_RplConfigInfoCorrupted equ (NERR_BASE+523) 
NERR_RplConfigNotFound equ (NERR_BASE+524) 
NERR_RplAdapterInfoCorrupted equ (NERR_BASE+525) 
NERR_RplInternal equ (NERR_BASE+526) 
NERR_RplVendorInfoCorrupted equ (NERR_BASE+527) 
NERR_RplBootInfoCorrupted equ (NERR_BASE+528) 
NERR_RplWkstaNeedsUserAcct equ (NERR_BASE+529) 
NERR_RplNeedsRPLUSERAcct equ (NERR_BASE+530) 
NERR_RplBootNotFound equ (NERR_BASE+531) 
NERR_RplIncompatibleProfile equ (NERR_BASE+532) 
NERR_RplAdapterNameUnavailable equ (NERR_BASE+533) 
NERR_RplConfigNotEmpty equ (NERR_BASE+534) 
NERR_RplBootInUse equ (NERR_BASE+535) 
NERR_RplBackupDatabase equ (NERR_BASE+536) 
NERR_RplAdapterNotFound equ (NERR_BASE+537) 
NERR_RplVendorNotFound equ (NERR_BASE+538) 
NERR_RplVendorNameUnavailable equ (NERR_BASE+539) 
NERR_RplBootNameUnavailable equ (NERR_BASE+540) 
NERR_RplConfigNameUnavailable equ (NERR_BASE+541) 
NERR_DfsInternalCorruption equ (NERR_BASE+560) 
NERR_DfsVolumeDataCorrupt equ (NERR_BASE+561) 
NERR_DfsNoSuchVolume equ (NERR_BASE+562) 
NERR_DfsVolumeAlreadyExists equ (NERR_BASE+563) 
NERR_DfsAlreadyShared equ (NERR_BASE+564) 
NERR_DfsNoSuchShare equ (NERR_BASE+565) 
NERR_DfsNotALeafVolume equ (NERR_BASE+566) 
NERR_DfsLeafVolume equ (NERR_BASE+567) 
NERR_DfsVolumeHasMultipleServers equ (NERR_BASE+568) 
NERR_DfsCantCreateJunctionPoint equ (NERR_BASE+569) 
NERR_DfsServerNotDfsAware equ (NERR_BASE+570) 
NERR_DfsBadRenamePath equ (NERR_BASE+571) 
NERR_DfsVolumeIsOffline equ (NERR_BASE+572) 
NERR_DfsNoSuchServer equ (NERR_BASE+573) 
NERR_DfsCyclicalName equ (NERR_BASE+574) 
NERR_DfsNotSupportedInServerDfs equ (NERR_BASE+575) 
NERR_DfsInternalError equ (NERR_BASE+590) 
NERR_SetupAlreadyJoined equ (NERR_BASE+591) 
NERR_SetupNotJoined equ (NERR_BASE+592) 
NERR_SetupDomainController equ (NERR_BASE+593) 
MAX_NERR equ (NERR_BASE+899) 

;===================================[ LMWKSTA.H ]==============================
WKSTA_PLATFORM_ID_PARMNUM equ 100
WKSTA_COMPUTERNAME_PARMNUM equ 1
WKSTA_LANGROUP_PARMNUM equ 2
WKSTA_VER_MAJOR_PARMNUM equ 4
WKSTA_VER_MINOR_PARMNUM equ 5
WKSTA_LOGGED_ON_USERS_PARMNUM equ 6
WKSTA_LANROOT_PARMNUM equ 7
WKSTA_LOGON_DOMAIN_PARMNUM equ 8
WKSTA_LOGON_SERVER_PARMNUM equ 9
WKSTA_CHARWAIT_PARMNUM equ 10  
WKSTA_CHARTIME_PARMNUM equ 11  
WKSTA_CHARCOUNT_PARMNUM equ 12  
WKSTA_KEEPCONN_PARMNUM equ 13
WKSTA_KEEPSEARCH_PARMNUM equ 14
WKSTA_MAXCMDS_PARMNUM equ 15
WKSTA_NUMWORKBUF_PARMNUM equ 16
WKSTA_MAXWRKCACHE_PARMNUM equ 17
WKSTA_SESSTIMEOUT_PARMNUM equ 18
WKSTA_SIZERROR_PARMNUM equ 19
WKSTA_NUMALERTS_PARMNUM equ 20
WKSTA_NUMSERVICES_PARMNUM equ 21
WKSTA_NUMCHARBUF_PARMNUM equ 22
WKSTA_SIZCHARBUF_PARMNUM equ 23
WKSTA_ERRLOGSZ_PARMNUM equ 27  
WKSTA_PRINTBUFTIME_PARMNUM equ 28  
WKSTA_SIZWORKBUF_PARMNUM equ 29
WKSTA_MAILSLOTS_PARMNUM equ 30
WKSTA_NUMDGRAMBUF_PARMNUM equ 31
WKSTA_WRKHEURISTICS_PARMNUM equ 32  
WKSTA_MAXTHREADS_PARMNUM equ 33
WKSTA_LOCKQUOTA_PARMNUM equ 41
WKSTA_LOCKINCREMENT_PARMNUM equ 42
WKSTA_LOCKMAXIMUM_PARMNUM equ 43
WKSTA_PIPEINCREMENT_PARMNUM equ 44
WKSTA_PIPEMAXIMUM_PARMNUM equ 45
WKSTA_DORMANTFILELIMIT_PARMNUM equ 46
WKSTA_CACHEFILETIMEOUT_PARMNUM equ 47
WKSTA_USEOPPORTUNISTICLOCKING_PARMNUM equ 48
WKSTA_USEUNLOCKBEHIND_PARMNUM equ 49
WKSTA_USECLOSEBEHIND_PARMNUM equ 50
WKSTA_BUFFERNAMEDPIPES_PARMNUM equ 51
WKSTA_USELOCKANDREADANDUNLOCK_PARMNUM equ 52
WKSTA_UTILIZENTCACHING_PARMNUM equ 53
WKSTA_USERAWREAD_PARMNUM equ 54
WKSTA_USERAWWRITE_PARMNUM equ 55
WKSTA_USEWRITERAWWITHDATA_PARMNUM equ 56
WKSTA_USEENCRYPTION_PARMNUM equ 57
WKSTA_BUFFILESWITHDENYWRITE_PARMNUM equ 58
WKSTA_BUFFERREADONLYFILES_PARMNUM equ 59
WKSTA_FORCECORECREATEMODE_PARMNUM equ 60
WKSTA_USE512BYTESMAXTRANSFER_PARMNUM equ 61
WKSTA_READAHEADTHRUPUT_PARMNUM equ 62
WKSTA_OTH_DOMAINS_PARMNUM equ 101
TRANSPORT_QUALITYOFSERVICE_PARMNUM equ 201
TRANSPORT_NAME_PARMNUM equ 202

WKSTA_INFO_100 struct
    wki100_platform_id dd ?
    wki100_computername dd ?
    wki100_langroup dd ?
    wki100_ver_major dd ?
    wki100_ver_minor dd ?
WKSTA_INFO_100 ends

WKSTA_INFO_101 struct
    wki101_platform_id dd ?
    wki101_computername dd ?
    wki101_langroup dd ?
    wki101_ver_major dd ?
    wki101_ver_minor dd ?
    wki101_lanroot dd ?
WKSTA_INFO_101 ends

WKSTA_INFO_102  struct
    wki102_platform_id dd ?
    wki102_computername dd ?
    wki102_langroup dd ?
    wki102_ver_major dd ?
    wki102_ver_minor dd ?
    wki102_lanroot dd ?
    wki102_logged_on_users dd ?
WKSTA_INFO_102 ends

WKSTA_INFO_302 struct
    wki302_char_wait dd ?
    wki302_collection_time dd ?
    wki302_maximum_collection_count dd ?
    wki302_keep_conn dd ?
    wki302_keep_search dd ?
    wki302_max_cmds dd ?
    wki302_num_work_buf dd ?
    wki302_siz_work_buf dd ?
    wki302_max_wrk_cache dd ?
    wki302_sess_timeout dd ?
    wki302_siz_error dd ?
    wki302_num_alerts dd ?
    wki302_num_services dd ?
    wki302_errlog_sz dd ?
    wki302_print_buf_time dd ?
    wki302_num_char_buf dd ?
    wki302_siz_char_buf dd ?
    wki302_wrk_heuristics dd ?
    wki302_mailslots dd ?
    wki302_num_dgram_buf dd ?
WKSTA_INFO_302 ends

WKSTA_INFO_402 struct
    wki402_char_wait dd ?
    wki402_collection_time dd ?
    wki402_maximum_collection_count dd ?
    wki402_keep_conn dd ?
    wki402_keep_search dd ?
    wki402_max_cmds dd ?
    wki402_num_work_buf dd ?
    wki402_siz_work_buf dd ?
    wki402_max_wrk_cache dd ?
    wki402_sess_timeout dd ?
    wki402_siz_error dd ?
    wki402_num_alerts dd ?
    wki402_num_services dd ?
    wki402_errlog_sz dd ?
    wki402_print_buf_time dd ?
    wki402_num_char_buf dd ?
    wki402_siz_char_buf dd ?
    wki402_wrk_heuristics dd ?
    wki402_mailslots dd ?
    wki402_num_dgram_buf dd ?
    wki402_max_threads dd ?
WKSTA_INFO_402 ends

WKSTA_INFO_502 struct
    wki502_char_wait dd ?
    wki502_collection_time dd ?
    wki502_maximum_collection_count dd ?
    wki502_keep_conn dd ?
    wki502_max_cmds dd ?
    wki502_sess_timeout dd ?
    wki502_siz_char_buf dd ?
    wki502_max_threads dd ?
    wki502_lock_quota dd ?
    wki502_lock_increment dd ?
    wki502_lock_maximum dd ?
    wki502_pipe_increment dd ?
    wki502_pipe_maximum dd ?
    wki502_cache_file_timeout dd ?
    wki502_dormant_file_limit dd ?
    wki502_read_ahead_throughput dd ?
    wki502_num_mailslot_buffers dd ?
    wki502_num_srv_announce_buffers dd ?
    wki502_max_illegal_datagram_events dd ?
    wki502_illegal_datagram_event_reset_frequency dd ?
    wki502_log_election_packets dd ?
    wki502_use_opportunistic_locking dd ?
    wki502_use_unlock_behind dd ?
    wki502_use_close_behind dd ?
    wki502_buf_named_pipes dd ?
    wki502_use_lock_read_unlock dd ?
    wki502_utilize_nt_caching dd ?
    wki502_use_raw_read dd ?
    wki502_use_raw_write dd ?
    wki502_use_write_raw_data dd ?
    wki502_use_encryption dd ?
    wki502_buf_files_deny_write dd ?
    wki502_buf_read_only_files dd ?

    wki502_force_core_create_mode dd ?
    wki502_use_512_byte_max_transfer dd ?
WKSTA_INFO_502 ends

WKSTA_INFO_1010 struct
     wki1010_char_wait dd ?
WKSTA_INFO_1010 ends

WKSTA_INFO_1011 struct
     wki1011_collection_time dd ?
WKSTA_INFO_1011 ends

WKSTA_INFO_1012 struct
     wki1012_maximum_collection_count dd ?
WKSTA_INFO_1012 ends

WKSTA_INFO_1027 struct
     wki1027_errlog_sz dd ?
WKSTA_INFO_1027 ends

WKSTA_INFO_1028 struct
     wki1028_print_buf_time dd ?
WKSTA_INFO_1028 ends

WKSTA_INFO_1032 struct
     wki1032_wrk_heuristics dd ?
WKSTA_INFO_1032 ends

WKSTA_INFO_1013 struct
     wki1013_keep_conn dd ?
WKSTA_INFO_1013 ends

WKSTA_INFO_1018 struct
     wki1018_sess_timeout dd ?
WKSTA_INFO_1018 ends

WKSTA_INFO_1023 struct
     wki1023_siz_char_buf dd ?
WKSTA_INFO_1023 ends

WKSTA_INFO_1033 struct
     wki1033_max_threads dd ?
WKSTA_INFO_1033 ends

WKSTA_INFO_1041 struct
    wki1041_lock_quota dd ?
WKSTA_INFO_1041 ends

WKSTA_INFO_1042 struct
    wki1042_lock_increment dd ?
WKSTA_INFO_1042 ends

WKSTA_INFO_1043 struct
    wki1043_lock_maximum dd ?
WKSTA_INFO_1043 ends

WKSTA_INFO_1044 struct
    wki1044_pipe_increment dd ?
WKSTA_INFO_1044 ends

WKSTA_INFO_1045 struct
    wki1045_pipe_maximum dd ?
WKSTA_INFO_1045 ends

WKSTA_INFO_1046 struct
    wki1046_dormant_file_limit dd ?
WKSTA_INFO_1046 ends

WKSTA_INFO_1047 struct
    wki1047_cache_file_timeout dd ?
WKSTA_INFO_1047 ends

WKSTA_INFO_1048 struct
    wki1048_use_opportunistic_locking dd ?
WKSTA_INFO_1048 ends

WKSTA_INFO_1049 struct
    wki1049_use_unlock_behind dd ?
WKSTA_INFO_1049 ends

WKSTA_INFO_1050 struct
    wki1050_use_close_behind dd ?
WKSTA_INFO_1050 ends

WKSTA_INFO_1051 struct
    wki1051_buf_named_pipes dd ?
WKSTA_INFO_1051 ends

WKSTA_INFO_1052 struct
    wki1052_use_lock_read_unlock dd ?
WKSTA_INFO_1052 ends

WKSTA_INFO_1053 struct
    wki1053_utilize_nt_caching dd ?
WKSTA_INFO_1053 ends

WKSTA_INFO_1054 struct
    wki1054_use_raw_read dd ?
WKSTA_INFO_1054 ends

WKSTA_INFO_1055 struct
    wki1055_use_raw_write dd ?
WKSTA_INFO_1055 ends

WKSTA_INFO_1056 struct
    wki1056_use_write_raw_data dd ?
WKSTA_INFO_1056 ends

WKSTA_INFO_1057 struct
    wki1057_use_encryption dd ?
WKSTA_INFO_1057 ends

WKSTA_INFO_1058 struct
    wki1058_buf_files_deny_write dd ?
WKSTA_INFO_1058 ends

WKSTA_INFO_1059 struct
    wki1059_buf_read_only_files dd ?
WKSTA_INFO_1059 ends

WKSTA_INFO_1060 struct
    wki1060_force_core_create_mode dd ?
WKSTA_INFO_1060 ends

WKSTA_INFO_1061 struct
    wki1061_use_512_byte_max_transfer dd ?
WKSTA_INFO_1061 ends

WKSTA_INFO_1062 struct
    wki1062_read_ahead_throughput dd ?
WKSTA_INFO_1062 ends

WKSTA_USER_INFO_0 struct
    wkui0_username dd ?
WKSTA_USER_INFO_0 ends

WKSTA_USER_INFO_1 struct
    wkui1_username dd ?
    wkui1_logon_domain dd ?
    wkui1_oth_domains dd ?
    wkui1_logon_server dd ?
WKSTA_USER_INFO_1 ends

WKSTA_USER_INFO_1101 struct
     wkui1101_oth_domains dd ?
WKSTA_USER_INFO_1101 ends

WKSTA_TRANSPORT_INFO_0 struct
    wkti0_quality_of_service dd ?
    wkti0_number_of_vcs dd ?
    wkti0_transport_name dd ?
    wkti0_transport_address dd ?
    wkti0_wan_ish dd ?
WKSTA_TRANSPORT_INFO_0 ends

;============================= [ LMCONS.H ]================================
CNLEN equ 15                  
LM20_CNLEN equ 15                  
DNLEN equ CNLEN               
LM20_DNLEN equ LM20_CNLEN          
UNCLEN equ (CNLEN+2)           
LM20_UNCLEN equ (LM20_CNLEN+2)      
NNLEN equ 80                  
LM20_NNLEN equ 12                  
RMLEN equ (UNCLEN+1+NNLEN)    
LM20_RMLEN equ (LM20_UNCLEN+1+LM20_NNLEN) 
SNLEN equ 80                  
LM20_SNLEN equ 15                  
STXTLEN equ 256                 
LM20_STXTLEN equ 63                 
PATHLEN equ 256                 
LM20_PATHLEN equ 256                
DEVLEN equ 80                  
LM20_DEVLEN equ 8                   
EVLEN equ 16                  
UNLEN equ 256                 
LM20_UNLEN equ 20                  
GNLEN equ UNLEN               
LM20_GNLEN equ LM20_UNLEN          
PWLEN equ 256                 
LM20_PWLEN equ 14                  
SHPWLEN equ 8                   
CLTYPE_LEN equ 12                  
MAXCOMMENTSZ equ 256                
LM20_MAXCOMMENTSZ equ 48            
QNLEN equ NNLEN               
LM20_QNLEN equ LM20_NNLEN          
ALERTSZ equ 128                 
MAXDEVENTRIES equ 32
NETBIOS_NAME_LEN equ 16            
MAX_PREFERRED_LENGTH equ 0FFFFFFFFh
CRYPT_KEY_LEN equ 7
CRYPT_TXT_LEN equ 8
ENCRYPTED_PWLEN equ 16
SESSION_PWLEN equ 24
SESSION_CRYPT_KLEN equ 21
PARMNUM_ALL equ 0
PARM_ERROR_UNKNOWN equ 0FFFFFFFFh
PARM_ERROR_NONE equ 0
PARMNUM_BASE_INFOLEVEL equ 1000
MESSAGE_FILENAME equ "NETMSG"
OS2MSG_FILENAME equ "BASE"
HELP_MSG_FILENAME equ "NETH"
BACKUP_MSG_FILENAME equ "BAK.MSG"
PLATFORM_ID_DOS equ 300
PLATFORM_ID_OS2 equ 400
PLATFORM_ID_NT equ 500
PLATFORM_ID_OSF equ 600
PLATFORM_ID_VMS equ 700
MIN_LANMAN_MESSAGE_ID equ NERR_BASE
MAX_LANMAN_MESSAGE_ID equ 5799
NET_API_STATUS TYPEDEF DWORD
API_RET_TYPE TYPEDEF NET_API_STATUS
;=================================[ LMACCESS.H ]=================================
UF_SCRIPT equ 1
UF_ACCOUNTDISABLE equ 2
UF_HOMEDIR_REQUIRED equ 8
UF_LOCKOUT equ 10h
UF_PASSWD_NOTREQD equ 20h
UF_PASSWD_CANT_CHANGE equ 40h
UF_TEMP_DUPLICATE_ACCOUNT equ 100h
UF_NORMAL_ACCOUNT equ 200h
UF_INTERDOMAIN_TRUST_ACCOUNT equ 800h
UF_WORKSTATION_TRUST_ACCOUNT equ 1000h
UF_SERVER_TRUST_ACCOUNT equ 2000h
UF_MACHINE_ACCOUNT_MASK equ ( UF_INTERDOMAIN_TRUST_ACCOUNT + \
                                  UF_WORKSTATION_TRUST_ACCOUNT + \
                                  UF_SERVER_TRUST_ACCOUNT )
UF_ACCOUNT_TYPE_MASK equ ( \
                    UF_TEMP_DUPLICATE_ACCOUNT + \
                    UF_NORMAL_ACCOUNT + \
                    UF_INTERDOMAIN_TRUST_ACCOUNT + \
                    UF_WORKSTATION_TRUST_ACCOUNT + \
                    UF_SERVER_TRUST_ACCOUNT \
                )
UF_DONT_EXPIRE_PASSWD equ 10000h
UF_MNS_LOGON_ACCOUNT equ 20000h
UF_SETTABLE_BITS equ ( \
                    UF_SCRIPT + \
                    UF_ACCOUNTDISABLE + \
                    UF_LOCKOUT + \
                    UF_HOMEDIR_REQUIRED  + \
                    UF_PASSWD_NOTREQD + \
                    UF_PASSWD_CANT_CHANGE + \
                    UF_ACCOUNT_TYPE_MASK + \
                    UF_DONT_EXPIRE_PASSWD + \
                    UF_MNS_LOGON_ACCOUNT \
                )
FILTER_TEMP_DUPLICATE_ACCOUNT equ (1)
FILTER_NORMAL_ACCOUNT equ (2)
FILTER_INTERDOMAIN_TRUST_ACCOUNT equ (8)
FILTER_WORKSTATION_TRUST_ACCOUNT equ (10h)
FILTER_SERVER_TRUST_ACCOUNT equ (20h)
LG_INCLUDE_INDIRECT equ (1)
AF_OP_PRINT equ 1
AF_OP_COMM equ 2
AF_OP_SERVER equ 4
AF_OP_ACCOUNTS equ 8
AF_SETTABLE_BITS equ (AF_OP_PRINT + AF_OP_COMM + \
                                AF_OP_SERVER + AF_OP_ACCOUNTS)
UAS_ROLE_STANDALONE equ 0
UAS_ROLE_MEMBER equ 1
UAS_ROLE_BACKUP equ 2
UAS_ROLE_PRIMARY equ 3
USER_NAME_PARMNUM equ 1
USER_PASSWORD_PARMNUM equ 3
USER_PASSWORD_AGE_PARMNUM equ 4
USER_PRIV_PARMNUM equ 5
USER_HOME_DIR_PARMNUM equ 6
USER_COMMENT_PARMNUM equ 7
USER_FLAGS_PARMNUM equ 8
USER_SCRIPT_PATH_PARMNUM equ 9
USER_AUTH_FLAGS_PARMNUM equ 10
USER_FULL_NAME_PARMNUM equ 11
USER_USR_COMMENT_PARMNUM equ 12
USER_PARMS_PARMNUM equ 13
USER_WORKSTATIONS_PARMNUM equ 14
USER_LAST_LOGON_PARMNUM equ 15
USER_LAST_LOGOFF_PARMNUM equ 16
USER_ACCT_EXPIRES_PARMNUM equ 17
USER_MAX_STORAGE_PARMNUM equ 18
USER_UNITS_PER_WEEK_PARMNUM equ 19
USER_LOGON_HOURS_PARMNUM equ 20
USER_PAD_PW_COUNT_PARMNUM equ 21
USER_NUM_LOGONS_PARMNUM equ 22
USER_LOGON_SERVER_PARMNUM equ 23
USER_COUNTRY_CODE_PARMNUM equ 24
USER_CODE_PAGE_PARMNUM equ 25
USER_PRIMARY_GROUP_PARMNUM equ 51
USER_PROFILE equ 52 
USER_PROFILE_PARMNUM equ 52
USER_HOME_DIR_DRIVE_PARMNUM equ 53
USER_NAME_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + USER_NAME_PARMNUM)
USER_PASSWORD_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + USER_PASSWORD_PARMNUM)
USER_PASSWORD_AGE_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + USER_PASSWORD_AGE_PARMNUM)
USER_PRIV_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + USER_PRIV_PARMNUM)
USER_HOME_DIR_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + USER_HOME_DIR_PARMNUM)
USER_COMMENT_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + USER_COMMENT_PARMNUM)
USER_FLAGS_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + USER_FLAGS_PARMNUM)
USER_SCRIPT_PATH_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + USER_SCRIPT_PATH_PARMNUM)
USER_AUTH_FLAGS_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + USER_AUTH_FLAGS_PARMNUM)
USER_FULL_NAME_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + USER_FULL_NAME_PARMNUM)
USER_USR_COMMENT_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + USER_USR_COMMENT_PARMNUM)
USER_PARMS_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + USER_PARMS_PARMNUM)
USER_WORKSTATIONS_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + USER_WORKSTATIONS_PARMNUM)
USER_LAST_LOGON_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + USER_LAST_LOGON_PARMNUM)
USER_LAST_LOGOFF_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + USER_LAST_LOGOFF_PARMNUM)
USER_ACCT_EXPIRES_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + USER_ACCT_EXPIRES_PARMNUM)
USER_MAX_STORAGE_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + USER_MAX_STORAGE_PARMNUM)
USER_UNITS_PER_WEEK_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + USER_UNITS_PER_WEEK_PARMNUM)
USER_LOGON_HOURS_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + USER_LOGON_HOURS_PARMNUM)
USER_PAD_PW_COUNT_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + USER_PAD_PW_COUNT_PARMNUM)
USER_NUM_LOGONS_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + USER_NUM_LOGONS_PARMNUM)
USER_LOGON_SERVER_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + USER_LOGON_SERVER_PARMNUM)
USER_COUNTRY_CODE_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + USER_COUNTRY_CODE_PARMNUM)
USER_CODE_PAGE_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + USER_CODE_PAGE_PARMNUM)
USER_PRIMARY_GROUP_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + USER_PRIMARY_GROUP_PARMNUM)
USER_POSIX_ID_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + USER_POSIX_ID_PARMNUM)
USER_HOME_DIR_DRIVE_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + USER_HOME_DIR_DRIVE_PARMNUM)
NULL_USERSETINFO_PASSWD equ "              "
TIMEQ_FOREVER equ -1L
USER_MAXSTORAGE_UNLIMITED equ -1L
USER_NO_LOGOFF equ -1L
UNITS_PER_DAY equ 24
UNITS_PER_WEEK equ UNITS_PER_DAY * 7
USER_PRIV_MASK equ 3
USER_PRIV_GUEST equ 0
USER_PRIV_USER equ 1
USER_PRIV_ADMIN equ 2
MAX_PASSWD_LEN equ PWLEN
DEF_MIN_PWLEN equ 6
DEF_PWUNIQUENESS equ 5
DEF_MAX_PWHIST equ 8
DEF_MAX_PWAGE equ TIMEQ_FOREVER               
DEF_MIN_PWAGE equ 0L          
DEF_FORCE_LOGOFF equ 0ffffffffh  
DEF_MAX_BADPW equ 0                           
ONE_DAY equ 01*24*3600  
VALIDATED_LOGON equ 0
PASSWORD_EXPIRED equ 2
NON_VALIDATED_LOGON equ 3
VALID_LOGOFF equ 1
MODALS_MIN_PASSWD_LEN_PARMNUM equ 1
MODALS_MAX_PASSWD_AGE_PARMNUM equ 2
MODALS_MIN_PASSWD_AGE_PARMNUM equ 3
MODALS_FORCE_LOGOFF_PARMNUM equ 4
MODALS_PASSWD_HIST_LEN_PARMNUM equ 5
MODALS_ROLE_PARMNUM equ 6
MODALS_PRIMARY_PARMNUM equ 7
MODALS_DOMAIN_NAME_PARMNUM equ 8
MODALS_DOMAIN_ID_PARMNUM equ 9
MODALS_LOCKOUT_DURATION_PARMNUM equ 10
MODALS_LOCKOUT_OBSERVATION_WINDOW_PARMNUM equ 11
MODALS_LOCKOUT_THRESHOLD_PARMNUM equ 12
MODALS_MIN_PASSWD_LEN_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + MODALS_MIN_PASSWD_LEN_PARMNUM)
MODALS_MAX_PASSWD_AGE_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + MODALS_MAX_PASSWD_AGE_PARMNUM)
MODALS_MIN_PASSWD_AGE_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + MODALS_MIN_PASSWD_AGE_PARMNUM)
MODALS_FORCE_LOGOFF_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + MODALS_FORCE_LOGOFF_PARMNUM)
MODALS_PASSWD_HIST_LEN_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + MODALS_PASSWD_HIST_LEN_PARMNUM)
MODALS_ROLE_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + MODALS_ROLE_PARMNUM)
MODALS_PRIMARY_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + MODALS_PRIMARY_PARMNUM)
MODALS_DOMAIN_NAME_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + MODALS_DOMAIN_NAME_PARMNUM)
MODALS_DOMAIN_ID_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + MODALS_DOMAIN_ID_PARMNUM)
            
GROUPIDMASK equ 8000h      
GROUP_SPECIALGRP_USERS equ L"USERS"
GROUP_SPECIALGRP_ADMINS equ L"ADMINS"
GROUP_SPECIALGRP_GUESTS equ L"GUESTS"
GROUP_SPECIALGRP_LOCAL equ L"LOCAL"
GROUP_ALL_PARMNUM equ 0
GROUP_NAME_PARMNUM equ 1
GROUP_COMMENT_PARMNUM equ 2
GROUP_ATTRIBUTES_PARMNUM equ 3
GROUP_ALL_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + GROUP_ALL_PARMNUM)
GROUP_NAME_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + GROUP_NAME_PARMNUM)
GROUP_COMMENT_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + GROUP_COMMENT_PARMNUM)
GROUP_ATTRIBUTES_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + GROUP_ATTRIBUTES_PARMNUM)
GROUP_POSIX_ID_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + GROUP_POSIX_ID_PARMNUM)
            
LOCALGROUP_NAME_PARMNUM equ 1
LOCALGROUP_COMMENT_PARMNUM equ 2
MAXPERMENTRIES equ 64
ACCESS_NONE equ 0
ACCESS_ALL equ ( ACCESS_READ + \
                                ACCESS_WRITE + \
                                ACCESS_CREATE + \
                                ACCESS_EXEC + \
                                ACCESS_DELETE + \
                                ACCESS_ATRIB + \
                                ACCESS_PERM \
                            )
ACCESS_READ equ 1
ACCESS_WRITE equ 2
ACCESS_CREATE equ 4
ACCESS_EXEC equ 8
ACCESS_DELETE equ 10h
ACCESS_ATRIB equ 20h
ACCESS_PERM equ 40h
ACCESS_GROUP equ 8000h
ACCESS_AUDIT equ 1
ACCESS_SUCCESS_OPEN equ 10h
ACCESS_SUCCESS_WRITE equ 20h
ACCESS_SUCCESS_DELETE equ 40h
ACCESS_SUCCESS_ACL equ 80h
ACCESS_SUCCESS_MASK equ 0F0h
ACCESS_FAIL_OPEN equ 100h
ACCESS_FAIL_WRITE equ 200h
ACCESS_FAIL_DELETE equ 400h
ACCESS_FAIL_ACL equ 800h
ACCESS_FAIL_MASK equ 0F00h
ACCESS_FAIL_SHIFT equ 4
ACCESS_RESOURCE_NAME_PARMNUM equ 1
ACCESS_ATTR_PARMNUM equ 2
ACCESS_COUNT_PARMNUM equ 3
ACCESS_ACCESS_LIST_PARMNUM equ 4
ACCESS_RESOURCE_NAME_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + ACCESS_RESOURCE_NAME_PARMNUM)
ACCESS_ATTR_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + ACCESS_ATTR_PARMNUM)
ACCESS_COUNT_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + ACCESS_COUNT_PARMNUM)
ACCESS_ACCESS_LIST_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + ACCESS_ACCESS_LIST_PARMNUM)
ACCESS_LETTERS equ "RWCXDAP         "
NETLOGON_CONTROL_QUERY equ 1    
NETLOGON_CONTROL_REPLICATE equ 2    
NETLOGON_CONTROL_SYNCHRONIZE equ 3    
NETLOGON_CONTROL_PDC_REPLICATE equ 4    
NETLOGON_CONTROL_REDISCOVER equ 5    
NETLOGON_CONTROL_TC_QUERY equ 6    
NETLOGON_CONTROL_TRANSPORT_NOTIFY equ 7 
NETLOGON_CONTROL_FIND_USER equ 8    
NETLOGON_CONTROL_UNLOAD_NETLOGON_DLL equ 0FFFBh
NETLOGON_CONTROL_BACKUP_CHANGE_LOG equ 0FFFCh
NETLOGON_CONTROL_TRUNCATE_LOG equ 0FFFDh
NETLOGON_CONTROL_SET_DBFLAG equ 0FFFEh
NETLOGON_CONTROL_BREAKPOINT equ 0FFFFh
NETLOGON_REPLICATION_NEEDED equ 1  
NETLOGON_REPLICATION_IN_PROGRESS equ 2  
NETLOGON_FULL_SYNC_REPLICATION equ 4  
NETLOGON_REDO_NEEDED equ 8  

USER_INFO_0 struct
    usri0_name dd ?
USER_INFO_0 ends

USER_INFO_1 struct
    usri1_name dd ?
    usri1_password dd ?
    usri1_password_age dd ?
    usri1_priv dd ?
    usri1_home_dir dd ?
    usri1_comment dd ?
    usri1_flags dd ?
    usri1_script_path dd ?
USER_INFO_1 ends

USER_INFO_2 struct
    usri2_name dd ?
    usri2_password dd ?
    usri2_password_age dd ?
    usri2_priv dd ?
    usri2_home_dir dd ?
    usri2_comment dd ?
    usri2_flags dd ?
    usri2_script_path dd ?
    usri2_auth_flags dd ?
    usri2_full_name dd ?
    usri2_usr_comment dd ?
    usri2_parms dd ?
    usri2_workstations dd ?
    usri2_last_logon dd ?
    usri2_last_logoff dd ?
    usri2_acct_expires dd ?
    usri2_max_storage dd ?
    usri2_units_per_week dd ?

    usri2_logon_hours dd ?
    usri2_bad_pw_count dd ?
    usri2_num_logons dd ?
    usri2_logon_server dd ?
    usri2_country_code dd ?
    usri2_code_page dd ?
USER_INFO_2 ends

USER_INFO_3 struct
    usri3_name dd ?
    usri3_password dd ?
    usri3_password_age dd ?
    usri3_priv dd ?
    usri3_home_dir dd ?
    usri3_comment dd ?
    usri3_flags dd ?
    usri3_script_path dd ?
    usri3_auth_flags dd ?
    usri3_full_name dd ?
    usri3_usr_comment dd ?
    usri3_parms dd ?
    usri3_workstations dd ?
    usri3_last_logon dd ?
    usri3_last_logoff dd ?
    usri3_acct_expires dd ?
    usri3_max_storage dd ?
    usri3_units_per_week dd ?
    usri3_logon_hours dd ?
    usri3_bad_pw_count dd ?
    usri3_num_logons dd ?
    usri3_logon_server dd ?
    usri3_country_code dd ?
    usri3_code_page dd ?
    usri3_user_id dd ?
    usri3_primary_group_id dd ?
    usri3_profile dd ?
    usri3_home_dir_drive dd ?
    usri3_password_expired dd ?
USER_INFO_3 ends

USER_INFO_10 struct
    usri10_name dd ?
    usri10_comment dd ?
    usri10_usr_comment dd ?
    usri10_full_name dd ?
USER_INFO_10 ends

USER_INFO_11 struct
    usri11_name dd ?
    usri11_comment dd ?
    usri11_usr_comment dd ?
    usri11_full_name dd ?
    usri11_priv dd ?
    usri11_auth_flags dd ?
    usri11_password_age dd ?
    usri11_home_dir dd ?
    usri11_parms dd ?
    usri11_last_logon dd ?
    usri11_last_logoff dd ?
    usri11_bad_pw_count dd ?
    usri11_num_logons dd ?
    usri11_logon_server dd ?
    usri11_country_code dd ?
    usri11_workstations dd ?
    usri11_max_storage dd ?
    usri11_units_per_week dd ?
    usri11_logon_hours dd ?
    usri11_code_page dd ?
USER_INFO_11 ends

USER_INFO_20 struct
    usri20_name dd ?
    usri20_full_name dd ?
    usri20_comment dd ?
    usri20_flags dd ?
    usri20_user_id dd ?
USER_INFO_20 ends

USER_INFO_21 struct
    usri21_password db ENCRYPTED_PWLEN dup(?)
USER_INFO_21 ends

USER_INFO_22 struct
    usri22_name dd ?
    usri22_password db ENCRYPTED_PWLEN dup(?)
    usri22_password_age dd ?
    usri22_priv dd ?
    usri22_home_dir dd ?
    usri22_comment dd ?
    usri22_flags dd ?
    usri22_script_path dd ?
    usri22_auth_flags dd ?
    usri22_full_name dd ?
    usri22_usr_comment dd ?
    usri22_parms dd ?
    usri22_workstations dd ?
    usri22_last_logon dd ?
    usri22_last_logoff dd ?
    usri22_acct_expires dd ?
    usri22_max_storage dd ?
    usri22_units_per_week dd ?
    usri22_logon_hours dd ?
    usri22_bad_pw_count dd ?
    usri22_num_logons dd ?
    usri22_logon_server dd ?
    usri22_country_code dd ?
    usri22_code_page dd ?
USER_INFO_22 ends

USER_INFO_1003 struct
     usri1003_password dd ?
USER_INFO_1003 ends

USER_INFO_1005 struct
     usri1005_priv dd ?
USER_INFO_1005 ends

USER_INFO_1006 struct
     usri1006_home_dir dd ?
USER_INFO_1006 ends

USER_INFO_1007 struct
     usri1007_comment dd ?
USER_INFO_1007 ends

USER_INFO_1008 struct
     usri1008_flags dd ?
USER_INFO_1008 ends

USER_INFO_1009 struct
     usri1009_script_path dd ?
USER_INFO_1009 ends

USER_INFO_1010 struct
     usri1010_auth_flags dd ?
USER_INFO_1010 ends

USER_INFO_1011 struct
     usri1011_full_name dd ?
USER_INFO_1011 ends

USER_INFO_1012 struct
     usri1012_usr_comment dd ?
USER_INFO_1012 ends

USER_INFO_1013 struct
     usri1013_parms dd ?
USER_INFO_1013 ends

USER_INFO_1014 struct
     usri1014_workstations dd ?
USER_INFO_1014 ends

USER_INFO_1017 struct
     usri1017_acct_expires dd ?
USER_INFO_1017 ends

USER_INFO_1018 struct
     usri1018_max_storage dd ?
USER_INFO_1018 ends

USER_INFO_1020 struct
    usri1020_units_per_week dd ?
    usri1020_logon_hours dd ?
USER_INFO_1020 ends

USER_INFO_1023 struct
     usri1023_logon_server dd ?
USER_INFO_1023 ends

USER_INFO_1024 struct
     usri1024_country_code dd ?
USER_INFO_1024 ends

USER_INFO_1025 struct
     usri1025_code_page dd ?
USER_INFO_1025 ends

USER_INFO_1051 struct
     usri1051_primary_group_id dd ?
USER_INFO_1051 ends

USER_INFO_1052 struct
     usri1052_profile dd ?
USER_INFO_1052 ends

USER_INFO_1053 struct
     usri1053_home_dir_drive dd ?
USER_INFO_1053 ends

USER_MODALS_INFO_0 struct
    usrmod0_min_passwd_len dd ?
    usrmod0_max_passwd_age dd ?
    usrmod0_min_passwd_age dd ?
    usrmod0_force_logoff dd ?
    usrmod0_password_hist_len dd ?
USER_MODALS_INFO_0 ends

USER_MODALS_INFO_1 struct
    usrmod1_role dd ?
    usrmod1_primary dd ?
USER_MODALS_INFO_1 ends

USER_MODALS_INFO_2 struct
    usrmod2_domain_name dd ?
    usrmod2_domain_id dd ?
USER_MODALS_INFO_2 ends

USER_MODALS_INFO_3 struct
    usrmod3_lockout_duration dd ?
    usrmod3_lockout_observation_window dd ?
    usrmod3_lockout_threshold dd ?
USER_MODALS_INFO_3 ends

USER_MODALS_INFO_1001 struct
     usrmod1001_min_passwd_len dd ?
USER_MODALS_INFO_1001 ends

USER_MODALS_INFO_1002 struct
     usrmod1002_max_passwd_age dd ?
USER_MODALS_INFO_1002 ends

USER_MODALS_INFO_1003 struct
     usrmod1003_min_passwd_age dd ?
USER_MODALS_INFO_1003 ends

USER_MODALS_INFO_1004 struct
     usrmod1004_force_logoff dd ?
USER_MODALS_INFO_1004 ends

USER_MODALS_INFO_1005 struct
    usrmod1005_password_hist_len dd ?
USER_MODALS_INFO_1005 ends

USER_MODALS_INFO_1006 struct
     usrmod1006_role dd ?
USER_MODALS_INFO_1006 ends

USER_MODALS_INFO_1007 struct
     usrmod1007_primary dd ?
USER_MODALS_INFO_1007 ends

GROUP_INFO_0 struct
    grpi0_name dd ?
GROUP_INFO_0 ends

GROUP_INFO_1 struct
    grpi1_name dd ?
    grpi1_comment dd ?
GROUP_INFO_1 ends

GROUP_INFO_2 struct
    grpi2_name dd ?
    grpi2_comment dd ?
    grpi2_group_id dd ?
    grpi2_attributes dd ?
GROUP_INFO_2 ends

GROUP_INFO_1002 struct
     grpi1002_comment dd ?
GROUP_INFO_1002 ends

GROUP_INFO_1005 struct
     grpi1005_attributes dd ?
GROUP_INFO_1005 ends

GROUP_USERS_INFO_0 struct
     grui0_name dd ?
GROUP_USERS_INFO_0 ends

GROUP_USERS_INFO_1 struct
     grui1_name dd ?
     grui1_attributes dd ?
GROUP_USERS_INFO_1 ends

LOCALGROUP_INFO_0 struct
    lgrpi0_name dd ?
LOCALGROUP_INFO_0 ends

LOCALGROUP_INFO_1 struct
    lgrpi1_name dd ?
    lgrpi1_comment dd ?
LOCALGROUP_INFO_1 ends

LOCALGROUP_INFO_1002 struct
     lgrpi1002_comment dd ?
LOCALGROUP_INFO_1002 ends

LOCALGROUP_MEMBERS_INFO_0 struct
     lgrmi0_sid dd ?
LOCALGROUP_MEMBERS_INFO_0 ends

LOCALGROUP_MEMBERS_INFO_1 struct
     lgrmi1_sid dd ?
     lgrmi1_sidusage dd ?
     lgrmi1_name dd ?
LOCALGROUP_MEMBERS_INFO_1 ends

LOCALGROUP_MEMBERS_INFO_2 struct
     lgrmi2_sid dd ?
     lgrmi2_sidusage dd ?
     lgrmi2_domainandname dd ?
LOCALGROUP_MEMBERS_INFO_2 ends

LOCALGROUP_MEMBERS_INFO_3 struct
     lgrmi3_domainandname dd ?
LOCALGROUP_MEMBERS_INFO_3 ends

LOCALGROUP_USERS_INFO_0 struct
     lgrui0_name dd ?
LOCALGROUP_USERS_INFO_0 ends

NET_DISPLAY_USER struct
    usri1_name dd ?
    usri1_comment dd ?
    usri1_flags dd ?
    usri1_full_name dd ?
    usri1_user_id dd ?
    usri1_next_index dd ?
NET_DISPLAY_USER ends

NET_DISPLAY_MACHINE struct
    usri2_name dd ?
    usri2_comment dd ?
    usri2_flags dd ?
    usri2_user_id dd ?
    usri2_next_index dd ?
NET_DISPLAY_MACHINE ends

NET_DISPLAY_GROUP struct
    grpi3_name dd ?
    grpi3_comment dd ?
    grpi3_group_id dd ?
    grpi3_attributes dd ?
    grpi3_next_index dd ?
NET_DISPLAY_GROUP ends

ACCESS_INFO_0 struct
    acc0_resource_name dd ?
ACCESS_INFO_0 ends

ACCESS_INFO_1 struct
    acc1_resource_name dd ?
    acc1_attr dd ?
    acc1_count dd ?
ACCESS_INFO_1 ends

ACCESS_INFO_1002 struct
     acc1002_attr dd ?
ACCESS_INFO_1002 ends


ACCESS_LIST struct
    acl_ugname dd ?
    acl_access dd ?
ACCESS_LIST ends

NETLOGON_INFO_1 struct
        netlog1_flags dd ?
        netlog1_pdc_connection_status NET_API_STATUS ?
NETLOGON_INFO_1 ends

NETLOGON_INFO_2 struct
        netlog2_flags dd ?
        netlog2_pdc_connection_status NET_API_STATUS ?
        netlog2_trusted_dc_name dd ?
        netlog2_tc_connection_status NET_API_STATUS ?
NETLOGON_INFO_2 ends

NETLOGON_INFO_3 struct
        netlog3_flags dd ?
        netlog3_logon_attempts dd ?
        netlog3_reserved1 dd ?
        netlog3_reserved2 dd ?
        netlog3_reserved3 dd ?
        netlog3_reserved4 dd ?
        netlog3_reserved5 dd ?
NETLOGON_INFO_3 ends

NETLOGON_INFO_4 struct
        netlog4_trusted_dc_name dd ?
        netlog4_trusted_domain_name dd ?
NETLOGON_INFO_4 ends

;========================[ RichEdit v3 by Ewayne '==============================
CFM_BACKCOLOR           equ 4000000h
CFM_LCID                equ 2000000h
CFM_UNDERLINETYPE       equ 800000h
CFM_WEIGHT              equ 400000h
CFM_SPACING             equ 200000h
CFM_KERNING             equ 100000h
CFM_STYLE               equ 80000h
CFM_ANIMATION           equ 40000h
CFM_REVAUTHOR           equ 8000h
CFM_SMALLCAPS           equ 40h
CFM_ALLCAPS             equ 80h
CFM_HIDDEN              equ 100h
CFM_OUTLINE             equ 200h
CFM_SHADOW              equ 400h
CFM_EMBOSS              equ 800h
CFM_IMPRINT             equ 1000h
CFM_DISABLED            equ 2000h
CFM_REVISED             equ 4000h
SCF_DEFAULT             equ 0
SCF_SELECTION           equ 1
SCF_WORD                equ 2
SCF_ALL                 equ 4
SCF_SCF_USEUIRULES      equ 8
CFU_CF1UNDERLINE        equ 0ffh
CFU_INVERT              equ 0feh
CFU_UNDERLINEHAIRLINE   equ 10
CFU_UNDERLINETHICK      equ 9
CFU_UNDERLINEWAVE       equ 8
CFU_UNDERLINEDASHDOTDOT equ 7
CFU_UNDERLINEDASHDOT    equ 6
CFU_UNDERLINEDASH       equ 5
CFU_UNDERLINEDOTTED     equ 4
CFU_UNDERLINEDOUBLE     equ 3
CFU_UNDERLINEWORD       equ 2
CFU_UNDERLINE           equ 1
CFU_UNDERLINENONE       equ 0

PFM_SPACEBEFORE         equ 40h
PFM_SPACEAFTER          equ 80h
PFM_LINESPACING         equ 100h
PFM_STYLE               equ 400h
PFM_BORDER              equ 800h
PFM_SHADING             equ 1000h
PFM_NUMBERINGSTYLE      equ 2000h
PFM_NUMBERINGTAB        equ 4000h
PFM_NUMBERINGSTART      equ 8000h
PFM_RTLPARA             equ 10000h
PFM_KEEP                equ 20000h
PFM_KEEPNEXT            equ 40000h
PFM_PAGEBREAKBEFORE     equ 80000h
PFM_NOLINENUMBER        equ 100000h
PFM_NOWIDOWCONTROL      equ 200000h
PFM_DONOTHYPHEN         equ 400000h
PFM_SIDEBYSIDE          equ 800000h
PFM_TABLE               equ 40000000h
PFM_COLLAPSED           equ 1000000h
PFM_OUTLINELEVEL        equ 2000000h
PFM_BOX                 equ 4000000h

CHARFORMAT2 STRUCT
  cbSize            DWORD      ?
  dwMask            DWORD      ?
  dwEffects         DWORD      ?
  yHeight           DWORD      ?
  yOffset           DWORD      ?
  crTextColor       DWORD      ?
  bCharSet          BYTE       ?
  bPitchAndFamily   BYTE       ?
  szFaceName        BYTE LF_FACESIZE dup(?)
  _wPad2            WORD       ?
  wWeight           WORD       ?
  sSpacing          WORD       ?
  crBackColor       DWORD      ?
  lcid              DWORD      ?
  dwReserved        DWORD      ?
  sStyle            WORD       ?
  wKerning          WORD       ?
  bUnderlineType    BYTE       ?
  bAnimation        BYTE       ? 
  bRevAuthor        BYTE       ?
  bReserved1        BYTE       ?
CHARFORMAT2 ENDS
;=============================================[ LMALERT.h ]=========================
ALERTER_MAILSLOT equ "\\\\.\\MAILSLOT\\Alerter"
ALERT_PRINT_EVENT equ "PRINTING"
ALERT_MESSAGE_EVENT equ "MESSAGE"
ALERT_ERRORLOG_EVENT equ "ERRORLOG"
ALERT_ADMIN_EVENT equ "ADMIN"
ALERT_USER_EVENT equ "USER"
PRJOB_QSTATUS equ 3         
PRJOB_DEVSTATUS equ 1fch       
PRJOB_COMPLETE equ 4         
PRJOB_INTERV equ 8         
PRJOB_ERROR equ 10h        
PRJOB_DESTOFFLINE equ 20h        
PRJOB_DESTPAUSED equ 40h        
PRJOB_NOTIFY equ 80h        
PRJOB_DESTNOPAPER equ 100h       
PRJOB_DELETED equ 8000h      
PRJOB_QS_QUEUED equ 0
PRJOB_QS_PAUSED equ 1
PRJOB_QS_SPOOLING equ 2
PRJOB_QS_PRINTING equ 3

STD_ALERT struct
    alrt_timestamp dd ?
    alrt_eventname WCHAR  [EVLEN + 1] dup(?)
    alrt_servicename WCHAR [SNLEN + 1] dup(?)
STD_ALERT ends

ADMIN_OTHER_INFO struct
    alrtad_errcode dd ?
    alrtad_numstrings dd ?
ADMIN_OTHER_INFO ends

ERRLOG_OTHER_INFO struct
    alrter_errcode dd ?
    alrter_offset dd ?
ERRLOG_OTHER_INFO ends

PRINT_OTHER_INFO struct
    alrtpr_jobid dd ?
    alrtpr_status dd ?
    alrtpr_submitted dd ?
    alrtpr_size dd ?
PRINT_OTHER_INFO ends

USER_OTHER_INFO struct
    alrtus_errcode dd ?
    alrtus_numstrings dd ?
USER_OTHER_INFO ends

;================================[ LMAT.H'=====================================
JOB_RUN_PERIODICALLY equ 1    
JOB_EXEC_ERROR equ 2    
JOB_RUNS_TODAY equ 4    
JOB_ADD_CURRENT_DATE equ 8    
JOB_NONINTERACTIVE equ 10h    
JOB_INPUT_FLAGS equ (   JOB_RUN_PERIODICALLY        +   \
                                JOB_ADD_CURRENT_DATE        +   \
                                JOB_NONINTERACTIVE  )
JOB_OUTPUT_FLAGS equ (   JOB_RUN_PERIODICALLY        +   \
                                JOB_EXEC_ERROR              +   \
                                JOB_RUNS_TODAY              +   \
                                JOB_NONINTERACTIVE  )                                
AT_INFO struct
    JobTime dd ?
    DaysOfMonth dd ?
    DaysOfWeek UCHAR ?
    Flags UCHAR ?
    Command dd ?
AT_INFO ends

AT_ENUM struct
    JobId dd ?
    JobTime dd ?
    DaysOfMonth dd ?
    DaysOfWeek UCHAR ?
    Flags UCHAR ?
    Command dd ?
AT_ENUM ends

;=========================================[ LMAUDIT.H ]=========================
LOGFLAGS_FORWARD	equ 0
LOGFLAGS_BACKWARD	equ 1
LOGFLAGS_SEEK	equ 2
ACTION_LOCKOUT equ 00
ACTION_ADMINUNLOCK equ 01
;AE_SRVSTATUS	equ 0
;AE_SESSLOGON	equ 1
;AE_SESSLOGOFF	equ 2
;AE_SESSPWERR	equ 3
;AE_CONNSTART	equ 4
;AE_CONNSTOP	equ 5
;AE_CONNREJ	equ 6
;AE_RESACCESS	equ 7
;AE_RESACCESSREJ	equ 8
;AE_CLOSEFILE	equ 9
;AE_SERVICESTAT	equ 11
;AE_ACLMOD	equ 12
;AE_UASMOD	equ 13
;AE_NETLOGON	equ 14
;AE_NETLOGOFF	equ 15
;AE_NETLOGDENIED equ 16
;AE_ACCLIMITEXCD equ 17
;AE_RESACCESS2	equ 18
;AE_ACLMODFAIL	equ 19
;AE_LOCKOUT equ 20
;AE_GENERIC_TYPE equ 21
AE_SRVSTART	equ 0
AE_SRVPAUSED	equ 1
AE_SRVCONT	equ 2
AE_SRVSTOP	equ 3
AE_GUEST	equ 0		
AE_USER	equ 1
AE_ADMIN	equ 2
AE_NORMAL	equ 0		
AE_USERLIMIT	equ 0
AE_GENERAL	equ 0
AE_ERROR	equ 1
AE_SESSDIS	equ 1
AE_BADPW	equ 1
AE_AUTODIS	equ 2
AE_UNSHARE	equ 2
AE_ADMINPRIVREQD equ 2
AE_ADMINDIS	equ 3
AE_NOACCESSPERM equ 3
AE_ACCRESTRICT	equ 4
AE_NORMAL_CLOSE equ 0
AE_SES_CLOSE equ 1
AE_ADMIN_CLOSE equ 2
AE_LIM_UNKNOWN	equ 0
AE_LIM_LOGONHOURS equ 1
AE_LIM_EXPIRED	equ 2
AE_LIM_INVAL_WKSTA equ 3
AE_LIM_DISABLED equ 4
AE_LIM_DELETED	equ 5
AE_MOD	equ 0
AE_DELETE	equ 1
AE_ADD	equ 2
AE_UAS_USER	equ 0
AE_UAS_GROUP	equ 1
AE_UAS_MODALS	equ 2
SVAUD_SERVICE equ 1
SVAUD_GOODSESSLOGON equ 6
SVAUD_BADSESSLOGON equ 18h
SVAUD_SESSLOGON equ (SVAUD_GOODSESSLOGON + SVAUD_BADSESSLOGON)
SVAUD_GOODNETLOGON equ 60h
SVAUD_BADNETLOGON equ 180h
SVAUD_NETLOGON equ (SVAUD_GOODNETLOGON + SVAUD_BADNETLOGON)
SVAUD_LOGON equ (SVAUD_NETLOGON + SVAUD_SESSLOGON)
SVAUD_GOODUSE equ 600h
SVAUD_BADUSE equ 1800h
SVAUD_USE equ (SVAUD_GOODUSE + SVAUD_BADUSE)
SVAUD_USERLIST equ 2000h
SVAUD_PERMISSIONS equ 4000h
SVAUD_RESOURCE equ 8000h
SVAUD_LOGONLIM	equ 10000h
AA_AUDIT_ALL	equ 1
AA_A_OWNER	equ 4
AA_CLOSE	equ 8
AA_S_OPEN	equ 10h
AA_S_WRITE	equ 20h
AA_S_CREATE	equ 20h
AA_S_DELETE	equ 40h
AA_S_ACL	equ 80h
AA_S_ALL	equ ( AA_S_OPEN + AA_S_WRITE + AA_S_DELETE + AA_S_ACL)
AA_F_OPEN	equ 100h
AA_F_WRITE	equ 200h
AA_F_CREATE	equ 200h
AA_F_DELETE	equ 400h
AA_F_ACL	equ 800h
AA_F_ALL	equ ( AA_F_OPEN + AA_F_WRITE + AA_F_DELETE + AA_F_ACL)
AA_A_OPEN	equ 1000h
AA_A_WRITE	equ 2000h
AA_A_CREATE	equ 2000h
AA_A_DELETE	equ 4000h
AA_A_ACL	equ 8000h
AA_A_ALL	equ ( AA_F_OPEN + AA_F_WRITE + AA_F_DELETE + AA_F_ACL)

HLOG struct
     time dd ?
     last_flags dd ?
     offset1 dd ?
     rec_offset dd ?
HLOG ends

AUDIT_ENTRY struct
     ae_len dd ?
     ae_reserved dd ?
     ae_time dd ?
     ae_type dd ?
     ae_data_offset dd ?
     ae_data_size dd ?
AUDIT_ENTRY ends

AE_SRVSTATUS struct
     ae_sv_status dd ?
AE_SRVSTATUS ends

AE_SESSLOGON struct
     ae_so_compname dd ?
     ae_so_username dd ?
     ae_so_privilege dd ?
AE_SESSLOGON ends

AE_SESSLOGOFF struct
     ae_sf_compname dd ?
     ae_sf_username dd ?
     ae_sf_reason dd ?
AE_SESSLOGOFF ends

AE_SESSPWERR struct
     ae_sp_compname dd ?
     ae_sp_username dd ?
AE_SESSPWERR ends

AE_CONNSTART struct
     ae_ct_compname dd ?
     ae_ct_username dd ?
     ae_ct_netname dd ?
     ae_ct_connid dd ?
AE_CONNSTART ends

AE_CONNSTOP struct
     ae_cp_compname dd ?
     ae_cp_username dd ?
     ae_cp_netname dd ?
     ae_cp_connid dd ?
     ae_cp_reason dd ?
AE_CONNSTOP ends

AE_CONNREJ struct
     ae_cr_compname dd ?
     ae_cr_username dd ?
     ae_cr_netname dd ?
     ae_cr_reason dd ?
AE_CONNREJ ends

AE_RESACCESS struct
     ae_ra_compname dd ?
     ae_ra_username dd ?
     ae_ra_resname dd ?
     ae_ra_operation dd ?
     ae_ra_returncode dd ?
     ae_ra_restype dd ?
     ae_ra_fileid dd ?
AE_RESACCESS ends

AE_RESACCESSREJ struct
     ae_rr_compname dd ?
     ae_rr_username dd ?
     ae_rr_resname dd ?
     ae_rr_operation dd ?
AE_RESACCESSREJ ends

AE_CLOSEFILE struct
     ae_cf_compname dd ?
     ae_cf_username dd ?
     ae_cf_resname dd ?
     ae_cf_fileid dd ?
     ae_cf_duration dd ?
     ae_cf_reason dd ?
AE_CLOSEFILE ends

AE_SERVICESTAT struct
     ae_ss_compname dd ?
     ae_ss_username dd ?
     ae_ss_svcname dd ?
     ae_ss_status dd ?
     ae_ss_code dd ?
     ae_ss_text dd ?
     ae_ss_returnval dd ?
AE_SERVICESTAT ends

AE_ACLMOD struct
     ae_am_compname dd ?
     ae_am_username dd ?
     ae_am_resname dd ?
     ae_am_action dd ?
     ae_am_datalen dd ?
AE_ACLMOD ends

AE_UASMOD struct
     ae_um_compname dd ?
     ae_um_username dd ?
     ae_um_resname dd ?
     ae_um_rectype dd ?
     ae_um_action dd ?
     ae_um_datalen dd ?
AE_UASMOD ends

AE_NETLOGON struct
     ae_no_compname dd ?
     ae_no_username dd ?
     ae_no_privilege dd ?
     ae_no_authflags dd ?
AE_NETLOGON ends

AE_NETLOGOFF struct
     ae_nf_compname dd ?
     ae_nf_username dd ?
     ae_nf_reserved1 dd ?
     ae_nf_reserved2 dd ?
AE_NETLOGOFF ends

AE_ACCLIM struct
     ae_al_compname dd ?
     ae_al_username dd ?
     ae_al_resname dd ?
     ae_al_limit dd ?
AE_ACCLIM ends

AE_LOCKOUT struct
    ae_lk_compname dd ?
    ae_lk_username dd ?
    ae_lk_action dd ?
    ae_lk_bad_pw_count dd ?
AE_LOCKOUT ends

AE_GENERIC struct
     ae_ge_msgfile dd ?
     ae_ge_msgnum dd ?
     ae_ge_params dd ?
     ae_ge_param1 dd ?
     ae_ge_param2 dd ?
     ae_ge_param3 dd ?
     ae_ge_param4 dd ?
     ae_ge_param5 dd ?
     ae_ge_param6 dd ?
     ae_ge_param7 dd ?
     ae_ge_param8 dd ?
     ae_ge_param9 dd ?
AE_GENERIC ends

;==================================[ LMBROWSR.H ]==============================
BROWSER_ROLE_PDC equ 1
BROWSER_ROLE_BDC  equ 2

BROWSER_STATISTICS struct
    StatisticsStartTime LARGE_INTEGER <>
    NumberOfServerAnnouncements LARGE_INTEGER <>
    NumberOfDomainAnnouncements LARGE_INTEGER <>
    NumberOfElectionPackets dd ?
    NumberOfMailslotWrites dd ?
    NumberOfGetBrowserServerListRequests dd ?
    NumberOfServerEnumerations dd ?
    NumberOfDomainEnumerations dd ?
    NumberOfOtherEnumerations dd ?
    NumberOfMissedServerAnnouncements dd ?
    NumberOfMissedMailslotDatagrams dd ?
    NumberOfMissedGetBrowserServerListRequests dd ?
    NumberOfFailedServerAnnounceAllocations dd ?
    NumberOfFailedMailslotAllocations dd ?
    NumberOfFailedMailslotReceives dd ?
    NumberOfFailedMailslotWrites dd ?
    NumberOfFailedMailslotOpens dd ?
    NumberOfDuplicateMasterAnnouncements dd ?
    NumberOfIllegalDatagrams LARGE_INTEGER <>
BROWSER_STATISTICS ends

BROWSER_STATISTICS_100 struct
    StartTime LARGE_INTEGER <>
    NumberOfServerAnnouncements LARGE_INTEGER <>
    NumberOfDomainAnnouncements LARGE_INTEGER <>
    NumberOfElectionPackets dd ?
    NumberOfMailslotWrites dd ?
    NumberOfGetBrowserServerListRequests dd ?
    NumberOfIllegalDatagrams LARGE_INTEGER <>
BROWSER_STATISTICS_100 ends

BROWSER_STATISTICS_101 struct
    StartTime LARGE_INTEGER <>
    NumberOfServerAnnouncements LARGE_INTEGER <>
    NumberOfDomainAnnouncements LARGE_INTEGER <>
    NumberOfElectionPackets dd ?
    NumberOfMailslotWrites dd ?
    NumberOfGetBrowserServerListRequests dd ?
    NumberOfIllegalDatagrams LARGE_INTEGER <>
    NumberOfMissedServerAnnouncements dd ?
    NumberOfMissedMailslotDatagrams dd ?
    NumberOfMissedGetBrowserServerListRequests dd ?
    NumberOfFailedServerAnnounceAllocations dd ?
    NumberOfFailedMailslotAllocations dd ?
    NumberOfFailedMailslotReceives dd ?
    NumberOfFailedMailslotWrites dd ?
    NumberOfFailedMailslotOpens dd ?
    NumberOfDuplicateMasterAnnouncements dd ?
BROWSER_STATISTICS_101 ends


BROWSER_EMULATED_DOMAIN struct
    DomainName dd ?
    EmulatedServerName dd ?
    Role dd ?
BROWSER_EMULATED_DOMAIN ends

;====================================[ LMCHDEV.H ]==============================
CHARDEV_STAT_OPENED equ 2
CHARDEV_STAT_ERROR equ 4
CHARDEV_CLOSE equ 0
CHARDEVQ_DEV_PARMNUM equ 1
CHARDEVQ_PRIORITY_PARMNUM equ 2
CHARDEVQ_DEVS_PARMNUM equ 3
CHARDEVQ_NUMUSERS_PARMNUM equ 4
CHARDEVQ_NUMAHEAD_PARMNUM equ 5
CHARDEVQ_PRIORITY_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + CHARDEVQ_PRIORITY_PARMNUM)
CHARDEVQ_DEVS_INFOLEVEL equ \
            (PARMNUM_BASE_INFOLEVEL + CHARDEVQ_DEVS_PARMNUM)
CHARDEVQ_MAX_PRIORITY equ 1
CHARDEVQ_MIN_PRIORITY equ 9
CHARDEVQ_DEF_PRIORITY equ 5
CHARDEVQ_NO_REQUESTS equ -1
HANDLE_INFO_LEVEL_1 equ 1
HANDLE_CHARTIME_PARMNUM equ 1
HANDLE_CHARCOUNT_PARMNUM equ 2

CHARDEV_INFO_0 struct
    ch0_dev dd ?
CHARDEV_INFO_0 ends

CHARDEV_INFO_1 struct
    ch1_dev dd ?
    ch1_status dd ?
    ch1_username dd ?
    ch1_time dd ?
CHARDEV_INFO_1 ends

CHARDEVQ_INFO_0 struct
    cq0_dev dd ?
CHARDEVQ_INFO_0 ends

CHARDEVQ_INFO_1 struct
    cq1_dev dd ?
    cq1_priority dd ?
    cq1_devs dd ?
    cq1_numusers dd ?
    cq1_numahead dd ?
CHARDEVQ_INFO_1 ends

CHARDEVQ_INFO_1002 struct
    cq1002_priority dd ?
CHARDEVQ_INFO_1002 ends

CHARDEVQ_INFO_1003 struct
    cq1003_devs dd ?
CHARDEVQ_INFO_1003 ends

HANDLE_INFO_1 struct
    hdli1_chartime dd ?
    hdli1_charcount dd ?
HANDLE_INFO_1 ends

;======================================[ LMCONFIG ]========================
CONFIG_INFO_0 struct
     cfgi0_key dd ?
     cfgi0_data dd ?
CONFIG_INFO_0 ends

;=======================================[ LMDFS.H ]========================

DFS_VOLUME_STATE_OK equ 1
DFS_VOLUME_STATE_INCONSISTENT equ 2
DFS_VOLUME_STATE_OFFLINE equ 3
DFS_VOLUME_STATE_ONLINE equ 4
DFS_STORAGE_STATE_OFFLINE equ 1
DFS_STORAGE_STATE_ONLINE equ 2
DFS_ADD_VOLUME equ 1   
DFS_RESTORE_VOLUME equ 2   
DFS_INFO_1 struct
    EntryPath dd ?
DFS_INFO_1 ends

DFS_INFO_2 struct
    EntryPath dd ?
    Comment dd ?
    State dd ?
    NumberOfStorages dd ?
DFS_INFO_2 ends

DFS_STORAGE_INFO struct
    State dd ?
    ServerName dd ?
    ShareName dd ?
DFS_STORAGE_INFO ends

DFS_INFO_3 struct
    EntryPath dd ?
    Comment dd ?
    State dd ?
    NumberOfStorages dd ?
    Storage dd ?
DFS_INFO_3 ends

DFS_INFO_100 struct
    Comment dd ?
DFS_INFO_100 ends

DFS_INFO_101 struct
    State dd ?
DFS_INFO_101 ends

;================================[ LMERRLOG.H ]===============================
ERRLOG_BASE equ 3100
NELOG_Internal_Error equ (ERRLOG_BASE + 0)
NELOG_Resource_Shortage equ (ERRLOG_BASE + 1)
NELOG_Unable_To_Lock_Segment equ (ERRLOG_BASE + 2)
NELOG_Unable_To_Unlock_Segment equ (ERRLOG_BASE + 3)
NELOG_Uninstall_Service equ (ERRLOG_BASE + 4)
NELOG_Init_Exec_Fail equ (ERRLOG_BASE + 5)
NELOG_Ncb_Error equ (ERRLOG_BASE + 6)
NELOG_Net_Not_Started equ (ERRLOG_BASE + 7)
NELOG_Ioctl_Error equ (ERRLOG_BASE + 8)
NELOG_System_Semaphore equ (ERRLOG_BASE + 9)
NELOG_Init_OpenCreate_Err equ (ERRLOG_BASE + 10)
NELOG_NetBios equ (ERRLOG_BASE + 11)
NELOG_SMB_Illegal equ (ERRLOG_BASE + 12)
NELOG_Service_Fail equ (ERRLOG_BASE + 13)
NELOG_Entries_Lost equ (ERRLOG_BASE + 14)
NELOG_Init_Seg_Overflow equ (ERRLOG_BASE + 20)
NELOG_Srv_No_Mem_Grow equ (ERRLOG_BASE + 21)
NELOG_Access_File_Bad equ (ERRLOG_BASE + 22)
NELOG_Srvnet_Not_Started equ (ERRLOG_BASE + 23)
NELOG_Init_Chardev_Err equ (ERRLOG_BASE + 24)
NELOG_Remote_API equ (ERRLOG_BASE + 25)
NELOG_Ncb_TooManyErr equ (ERRLOG_BASE + 26)
NELOG_Mailslot_err equ (ERRLOG_BASE + 27)
NELOG_ReleaseMem_Alert equ (ERRLOG_BASE + 28)
NELOG_AT_cannot_write equ (ERRLOG_BASE + 29)
NELOG_Cant_Make_Msg_File equ (ERRLOG_BASE + 30)
NELOG_Exec_Netservr_NoMem equ (ERRLOG_BASE + 31)
NELOG_Server_Lock_Failure equ (ERRLOG_BASE + 32)
NELOG_Msg_Shutdown equ (ERRLOG_BASE + 40)
NELOG_Msg_Sem_Shutdown equ (ERRLOG_BASE + 41)
NELOG_Msg_Log_Err equ (ERRLOG_BASE + 50)
NELOG_VIO_POPUP_ERR equ (ERRLOG_BASE + 51)
NELOG_Msg_Unexpected_SMB_Type equ (ERRLOG_BASE + 52)
NELOG_Wksta_Infoseg equ (ERRLOG_BASE + 60)
NELOG_Wksta_Compname equ (ERRLOG_BASE + 61)
NELOG_Wksta_BiosThreadFailure equ (ERRLOG_BASE + 62)
NELOG_Wksta_IniSeg equ (ERRLOG_BASE + 63)
NELOG_Wksta_HostTab_Full equ (ERRLOG_BASE + 64)
NELOG_Wksta_Bad_Mailslot_SMB equ (ERRLOG_BASE + 65)
NELOG_Wksta_UASInit equ (ERRLOG_BASE + 66)
NELOG_Wksta_SSIRelogon equ (ERRLOG_BASE + 67)
NELOG_Build_Name equ (ERRLOG_BASE + 70)
NELOG_Name_Expansion equ (ERRLOG_BASE + 71)
NELOG_Message_Send equ (ERRLOG_BASE + 72)
NELOG_Mail_Slt_Err equ (ERRLOG_BASE + 73)
NELOG_AT_cannot_read equ (ERRLOG_BASE + 74)
NELOG_AT_sched_err equ (ERRLOG_BASE + 75)
NELOG_AT_schedule_file_created equ (ERRLOG_BASE + 76)
NELOG_Srvnet_NB_Open equ (ERRLOG_BASE + 77)
NELOG_AT_Exec_Err equ (ERRLOG_BASE + 78)
NELOG_Lazy_Write_Err equ (ERRLOG_BASE + 80)
NELOG_HotFix equ (ERRLOG_BASE + 81)
NELOG_HardErr_From_Server equ (ERRLOG_BASE + 82)
NELOG_LocalSecFail1 equ (ERRLOG_BASE + 83)
NELOG_LocalSecFail2 equ (ERRLOG_BASE + 84)
NELOG_LocalSecFail3 equ (ERRLOG_BASE + 85)
NELOG_LocalSecGeneralFail equ (ERRLOG_BASE + 86)
NELOG_NetWkSta_Internal_Error equ (ERRLOG_BASE + 90)
NELOG_NetWkSta_No_Resource equ (ERRLOG_BASE + 91)
NELOG_NetWkSta_SMB_Err equ (ERRLOG_BASE + 92)
NELOG_NetWkSta_VC_Err equ (ERRLOG_BASE + 93)
NELOG_NetWkSta_Stuck_VC_Err equ (ERRLOG_BASE + 94)
NELOG_NetWkSta_NCB_Err equ (ERRLOG_BASE + 95)
NELOG_NetWkSta_Write_Behind_Err equ (ERRLOG_BASE + 96)
NELOG_NetWkSta_Reset_Err equ (ERRLOG_BASE + 97)
NELOG_NetWkSta_Too_Many equ (ERRLOG_BASE + 98)
NELOG_Srv_Thread_Failure equ (ERRLOG_BASE + 104)
NELOG_Srv_Close_Failure equ (ERRLOG_BASE + 105)
NELOG_ReplUserCurDir equ (ERRLOG_BASE + 106)
NELOG_ReplCannotMasterDir equ (ERRLOG_BASE + 107)
NELOG_ReplUpdateError equ (ERRLOG_BASE + 108)
NELOG_ReplLostMaster equ (ERRLOG_BASE + 109)
NELOG_NetlogonAuthDCFail equ (ERRLOG_BASE + 110)
NELOG_ReplLogonFailed equ (ERRLOG_BASE + 111)
NELOG_ReplNetErr equ (ERRLOG_BASE + 112)
NELOG_ReplMaxFiles equ (ERRLOG_BASE + 113)
NELOG_ReplMaxTreeDepth equ (ERRLOG_BASE + 114)
NELOG_ReplBadMsg equ (ERRLOG_BASE + 115)
NELOG_ReplSysErr equ (ERRLOG_BASE + 116)
NELOG_ReplUserLoged equ (ERRLOG_BASE + 117)
NELOG_ReplBadImport equ (ERRLOG_BASE + 118)
NELOG_ReplBadExport equ (ERRLOG_BASE + 119)
NELOG_ReplSignalFileErr equ (ERRLOG_BASE + 120)
NELOG_DiskFT equ (ERRLOG_BASE+121)
NELOG_ReplAccessDenied equ (ERRLOG_BASE + 122)
NELOG_NetlogonFailedPrimary equ (ERRLOG_BASE + 123)
NELOG_NetlogonPasswdSetFailed equ (ERRLOG_BASE + 124)
NELOG_NetlogonTrackingError equ (ERRLOG_BASE + 125)
NELOG_NetlogonSyncError equ (ERRLOG_BASE + 126)
NELOG_UPS_PowerOut equ (ERRLOG_BASE + 130)
NELOG_UPS_Shutdown equ (ERRLOG_BASE + 131)
NELOG_UPS_CmdFileError equ (ERRLOG_BASE + 132)
NELOG_UPS_CannotOpenDriver equ (ERRLOG_BASE+133)
NELOG_UPS_PowerBack equ (ERRLOG_BASE + 134)
NELOG_UPS_CmdFileConfig equ (ERRLOG_BASE + 135)
NELOG_UPS_CmdFileExec equ (ERRLOG_BASE + 136)
NELOG_Missing_Parameter equ (ERRLOG_BASE + 150)
NELOG_Invalid_Config_Line equ (ERRLOG_BASE + 151)
NELOG_Invalid_Config_File equ (ERRLOG_BASE + 152)
NELOG_File_Changed equ (ERRLOG_BASE + 153)
NELOG_Files_Dont_Fit equ (ERRLOG_BASE + 154)
NELOG_Wrong_DLL_Version equ (ERRLOG_BASE + 155)
NELOG_Error_in_DLL equ (ERRLOG_BASE + 156)
NELOG_System_Error equ (ERRLOG_BASE + 157)
NELOG_FT_ErrLog_Too_Large equ (ERRLOG_BASE + 158)
NELOG_FT_Update_In_Progress equ (ERRLOG_BASE + 159)
NELOG_OEM_Code equ (ERRLOG_BASE + 199)
ERRLOG2_BASE equ 5700
NELOG_NetlogonSSIInitError equ (ERRLOG2_BASE + 0)
NELOG_NetlogonFailedToUpdateTrustList equ (ERRLOG2_BASE + 1)
NELOG_NetlogonFailedToAddRpcInterface equ (ERRLOG2_BASE + 2)
NELOG_NetlogonFailedToReadMailslot equ (ERRLOG2_BASE + 3)
NELOG_NetlogonFailedToRegisterSC equ (ERRLOG2_BASE + 4)
NELOG_NetlogonChangeLogCorrupt equ (ERRLOG2_BASE + 5)
NELOG_NetlogonFailedToCreateShare equ (ERRLOG2_BASE + 6)
NELOG_NetlogonDownLevelLogonFailed equ (ERRLOG2_BASE + 7)
NELOG_NetlogonDownLevelLogoffFailed equ (ERRLOG2_BASE + 8)
NELOG_NetlogonNTLogonFailed equ (ERRLOG2_BASE + 9)
NELOG_NetlogonNTLogoffFailed equ (ERRLOG2_BASE + 10)
NELOG_NetlogonPartialSyncCallSuccess equ (ERRLOG2_BASE + 11)
NELOG_NetlogonPartialSyncCallFailed equ (ERRLOG2_BASE + 12)
NELOG_NetlogonFullSyncCallSuccess equ (ERRLOG2_BASE + 13)
NELOG_NetlogonFullSyncCallFailed equ (ERRLOG2_BASE + 14)
NELOG_NetlogonPartialSyncSuccess equ (ERRLOG2_BASE + 15)
NELOG_NetlogonPartialSyncFailed equ (ERRLOG2_BASE + 16)
NELOG_NetlogonFullSyncSuccess equ (ERRLOG2_BASE + 17)
NELOG_NetlogonFullSyncFailed equ (ERRLOG2_BASE + 18)
NELOG_NetlogonAuthNoDomainController equ (ERRLOG2_BASE + 19)
NELOG_NetlogonAuthNoTrustLsaSecret equ (ERRLOG2_BASE + 20)
NELOG_NetlogonAuthNoTrustSamAccount equ (ERRLOG2_BASE + 21)
NELOG_NetlogonServerAuthFailed equ (ERRLOG2_BASE + 22)
NELOG_NetlogonServerAuthNoTrustSamAccount equ (ERRLOG2_BASE + 23)
NELOG_FailedToRegisterSC equ (ERRLOG2_BASE + 24)
NELOG_FailedToSetServiceStatus equ (ERRLOG2_BASE + 25)
NELOG_FailedToGetComputerName equ (ERRLOG2_BASE + 26)
NELOG_DriverNotLoaded equ (ERRLOG2_BASE + 27)
NELOG_NoTranportLoaded equ (ERRLOG2_BASE + 28)
NELOG_NetlogonFailedDomainDelta equ (ERRLOG2_BASE + 29)
NELOG_NetlogonFailedGlobalGroupDelta equ (ERRLOG2_BASE + 30)
NELOG_NetlogonFailedLocalGroupDelta equ (ERRLOG2_BASE + 31)
NELOG_NetlogonFailedUserDelta equ (ERRLOG2_BASE + 32)
NELOG_NetlogonFailedPolicyDelta equ (ERRLOG2_BASE + 33)
NELOG_NetlogonFailedTrustedDomainDelta equ (ERRLOG2_BASE + 34)
NELOG_NetlogonFailedAccountDelta equ (ERRLOG2_BASE + 35)
NELOG_NetlogonFailedSecretDelta equ (ERRLOG2_BASE + 36)
NELOG_NetlogonSystemError equ (ERRLOG2_BASE + 37)
NELOG_NetlogonDuplicateMachineAccounts equ (ERRLOG2_BASE + 38)
NELOG_NetlogonTooManyGlobalGroups equ (ERRLOG2_BASE + 39)
NELOG_NetlogonBrowserDriver equ (ERRLOG2_BASE + 40)
NELOG_NetlogonAddNameFailure equ (ERRLOG2_BASE + 41)
NELOG_RplMessages equ (ERRLOG2_BASE + 42)
NELOG_RplXnsBoot equ (ERRLOG2_BASE + 43)
NELOG_RplSystem equ (ERRLOG2_BASE + 44)
NELOG_RplWkstaTimeout equ (ERRLOG2_BASE + 45)
NELOG_RplWkstaFileOpen equ (ERRLOG2_BASE + 46)
NELOG_RplWkstaFileRead equ (ERRLOG2_BASE + 47)
NELOG_RplWkstaMemory equ (ERRLOG2_BASE + 48)
NELOG_RplWkstaFileChecksum equ (ERRLOG2_BASE + 49)
NELOG_RplWkstaFileLineCount equ (ERRLOG2_BASE + 50)
NELOG_RplWkstaBbcFile equ (ERRLOG2_BASE + 51)
NELOG_RplWkstaFileSize equ (ERRLOG2_BASE + 52)
NELOG_RplWkstaInternal equ (ERRLOG2_BASE + 53)
NELOG_RplWkstaWrongVersion equ (ERRLOG2_BASE + 54)
NELOG_RplWkstaNetwork equ (ERRLOG2_BASE + 55)
NELOG_RplAdapterResource equ (ERRLOG2_BASE + 56)
NELOG_RplFileCopy equ (ERRLOG2_BASE + 57)
NELOG_RplFileDelete equ (ERRLOG2_BASE + 58)
NELOG_RplFilePerms equ (ERRLOG2_BASE + 59)
NELOG_RplCheckConfigs equ (ERRLOG2_BASE + 60)
NELOG_RplCreateProfiles equ (ERRLOG2_BASE + 61)
NELOG_RplRegistry equ (ERRLOG2_BASE + 62)
NELOG_RplReplaceRPLDISK equ (ERRLOG2_BASE + 63)
NELOG_RplCheckSecurity equ (ERRLOG2_BASE + 64)
NELOG_RplBackupDatabase equ (ERRLOG2_BASE + 65)
NELOG_RplInitDatabase equ (ERRLOG2_BASE + 66)
NELOG_RplRestoreDatabaseFailure equ (ERRLOG2_BASE + 67)
NELOG_RplRestoreDatabaseSuccess equ (ERRLOG2_BASE + 68)
NELOG_RplInitRestoredDatabase equ (ERRLOG2_BASE + 69)
NELOG_NetlogonSessionTypeWrong equ (ERRLOG2_BASE + 70)
NELOG_RplUpgradeDBTo40 equ (ERRLOG2_BASE + 71)
NELOG_NetlogonLanmanBdcsNotAllowed equ (ERRLOG2_BASE + 72)
NELOG_NetlogonNoDynamicDns equ (ERRLOG2_BASE + 73)
NELOG_NetlogonDynamicDnsRegisterFailure equ (ERRLOG2_BASE + 74)
NELOG_NetlogonDynamicDnsDeregisterFailure equ (ERRLOG2_BASE + 75)
NELOG_NetlogonFailedFileCreate equ (ERRLOG2_BASE + 76)
NELOG_NetlogonGetSubnetToSite equ (ERRLOG2_BASE + 77)
NELOG_NetlogonNoSiteForClient equ (ERRLOG2_BASE + 78)
NELOG_NetlogonBadSiteName equ (ERRLOG2_BASE + 79)
NELOG_NetlogonBadSubnetName equ (ERRLOG2_BASE + 80)

ERROR_LOG struct
     el_len dd ?
     el_reserved dd ?
     el_time dd ?
     el_error dd ?
     el_name dd ?
     el_text dd ?
     el_data dd ?
     el_data_size dd ?
     el_nstrings dd ?
ERROR_LOG ends

;===============================[ LMMSG.H]=============================
MSGNAME_NOT_FORWARDED   equ 0
MSGNAME_FORWARDED_TO   equ 4
MSGNAME_FORWARDED_FROM  equ 10h

MSG_INFO_0 struct
    msgi0_name dd ?
MSG_INFO_0 ends

MSG_INFO_1 struct
    msgi1_name dd ?
    msgi1_forward_flag dd ?
    msgi1_forward dd ?
MSG_INFO_1 ends

;=================================[ LMREMUTL.H]==========================
TIME_OF_DAY_INFO struct
    tod_elapsedt dd ?
    tod_msecs dd ?
    tod_hours dd ?
    tod_mins dd ?
    tod_secs dd ?
    tod_hunds dd ?
    tod_timezone dd ?
    tod_tinterval dd ?
    tod_day dd ?
    tod_month dd ?
    tod_year dd ?
    tod_weekday dd ?
TIME_OF_DAY_INFO ends

SUPPORTS_REMOTE_ADMIN_PROTOCOL equ 2
SUPPORTS_RPC equ 4
SUPPORTS_SAM_PROTOCOL equ 8
SUPPORTS_UNICODE equ 10h
SUPPORTS_LOCAL equ 20h
SUPPORTS_ANY equ 0FFFFFFFFh
NO_PERMISSION_REQUIRED equ 1
ALLOCATE_RESPONSE equ 2
USE_SPECIFIC_TRANSPORT equ 80000000h

;===============================[ LMREPL.H]=======================
REPL_ROLE_EXPORT equ 1
REPL_ROLE_IMPORT equ 2
REPL_ROLE_BOTH equ 3
REPL_INTERVAL_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + 0)
REPL_PULSE_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + 1)
REPL_GUARDTIME_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + 2)
REPL_RANDOM_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + 3)
REPL_INTEGRITY_FILE equ 1
REPL_INTEGRITY_TREE equ 2
REPL_EXTENT_FILE equ 1
REPL_EXTENT_TREE equ 2
REPL_EXPORT_INTEGRITY_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + 0)
REPL_EXPORT_EXTENT_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + 1)
REPL_UNLOCK_NOFORCE equ 0
REPL_UNLOCK_FORCE equ 1
REPL_STATE_OK equ 0
REPL_STATE_NO_MASTER equ 1
REPL_STATE_NO_SYNC equ 2
REPL_STATE_NEVER_REPLICATED equ 3

REPL_INFO_0 struct
    rp0_role dd ?
    rp0_exportpath dd ?
    rp0_exportlist dd ?
    rp0_importpath dd ?
    rp0_importlist dd ?
    rp0_logonusername dd ?
    rp0_interval dd ?
    rp0_pulse dd ?
    rp0_guardtime dd ?
    rp0_random dd ?
REPL_INFO_0 ends

REPL_INFO_1000 struct
    rp1000_interval dd ?
REPL_INFO_1000 ends

REPL_INFO_1001 struct
    rp1001_pulse dd ?
REPL_INFO_1001 ends

REPL_INFO_1002 struct
    rp1002_guardtime dd ?
REPL_INFO_1002 ends

REPL_INFO_1003 struct
    rp1003_random dd ?
REPL_INFO_1003 ends

REPL_EDIR_INFO_0 struct
    rped0_dirname dd ?
REPL_EDIR_INFO_0 ends

REPL_EDIR_INFO_1 struct
    rped1_dirname dd ?
    rped1_integrity dd ?
    rped1_extent dd ?
REPL_EDIR_INFO_1 ends

REPL_EDIR_INFO_2 struct
    rped2_dirname dd ?
    rped2_integrity dd ?
    rped2_extent dd ?
    rped2_lockcount dd ?
    rped2_locktime dd ?
REPL_EDIR_INFO_2 ends

REPL_EDIR_INFO_1000 struct
    rped1000_integrity dd ?
REPL_EDIR_INFO_1000 ends

REPL_EDIR_INFO_1001 struct
    rped1001_extent dd ?
REPL_EDIR_INFO_1001 ends

REPL_IDIR_INFO_0 struct 
    rpid0_dirname dd ?
REPL_IDIR_INFO_0 ends

REPL_IDIR_INFO_1 struct
    rpid1_dirname dd ?
    rpid1_state dd ?
    rpid1_mastername dd ?
    rpid1_last_update_time dd ?
    rpid1_lockcount dd ?
    rpid1_locktime dd ?
REPL_IDIR_INFO_1 ends

;==============================[ LMSHARE.H ]=============================
SHARE_NETNAME_PARMNUM equ 1
SHARE_TYPE_PARMNUM equ 3
SHARE_REMARK_PARMNUM equ 4
SHARE_PERMISSIONS_PARMNUM equ 5
SHARE_MAX_USES_PARMNUM equ 6
SHARE_CURRENT_USES_PARMNUM equ 7
SHARE_PATH_PARMNUM equ 8
SHARE_PASSWD_PARMNUM equ 9
SHARE_FILE_SD_PARMNUM equ 501
SHARE_REMARK_INFOLEVEL equ  (PARMNUM_BASE_INFOLEVEL + SHARE_REMARK_PARMNUM)
SHARE_MAX_USES_INFOLEVEL equ  (PARMNUM_BASE_INFOLEVEL + SHARE_MAX_USES_PARMNUM)
SHARE_FILE_SD_INFOLEVEL equ (PARMNUM_BASE_INFOLEVEL + SHARE_FILE_SD_PARMNUM)
SHI1_NUM_ELEMENTS equ 4
SHI2_NUM_ELEMENTS equ 10
STYPE_DISKTREE equ 0
STYPE_PRINTQ equ 1
STYPE_DEVICE equ 2
STYPE_IPC equ 3
STYPE_SPECIAL equ 80000000h
SHI_USES_UNLIMITED equ (DWORD)-1
SHI1005_FLAGS_DFS equ 1    
SHI1005_FLAGS_DFS_ROOT equ 2    
COW_PERMACHINE equ 4    
COW_PERUSER equ 8    
CSC_CACHEABLE equ 10h    
CSC_NOFLOWOPS equ 20h    
CSC_AUTO_INWARD equ 40h    
CSC_AUTO_OUTWARD equ 80h    
SHI1005_VALID_FLAGS_SET equ (   CSC_CACHEABLE + CSC_NOFLOWOPS + CSC_AUTO_INWARD + CSC_AUTO_OUTWARD+ COW_PERMACHINE  + COW_PERUSER)
SHI1007_VALID_FLAGS_SET equ SHI1005_VALID_FLAGS_SET
SESS_GUEST equ 1  
SESS_NOENCRYPTION equ 2  
SESI1_NUM_ELEMENTS equ 8
SESI2_NUM_ELEMENTS equ 9
PERM_FILE_READ equ 1 
PERM_FILE_WRITE equ 2 
PERM_FILE_CREATE equ 4 

SHARE_INFO_0 struct
    shi0_netname dd ?
SHARE_INFO_0 ends

SHARE_INFO_1 struct
    shi1_netname dd ?
    shi1_type dd ?
    shi1_remark dd ?
SHARE_INFO_1 ends

SHARE_INFO_2 struct
    shi2_netname dd ?
    shi2_type dd ?
    shi2_remark dd ?
    shi2_permissions dd ?
    shi2_max_uses dd ?
    shi2_current_uses dd ?
    shi2_path dd ?
    shi2_passwd dd ?
SHARE_INFO_2 ends

SHARE_INFO_501 struct
    shi501_netname dd ?
    shi501_type dd ?
    shi501_remark dd ?
    shi501_flags dd ?
SHARE_INFO_501 ends

SHARE_INFO_502 struct
    shi502_netname dd ?
    shi502_type dd ?
    shi502_remark dd ?
    shi502_permissions dd ?
    shi502_max_uses dd ?
    shi502_current_uses dd ?
    shi502_path dd ?
    shi502_passwd dd ?
    shi502_reserved dd ?
    shi502_security_descriptor dd ?
SHARE_INFO_502 ends

SHARE_INFO_1004 struct 
    shi1004_remark dd ?
SHARE_INFO_1004 ends

SHARE_INFO_1005 struct
    shi1005_flags dd ?
SHARE_INFO_1005 ends

SHARE_INFO_1006 struct
    shi1006_max_uses dd ?
SHARE_INFO_1006 ends

SHARE_INFO_1007 struct
    shi1007_flags dd ?
    shi1007_AlternateDirectoryName dd ?
SHARE_INFO_1007 ends

SHARE_INFO_1501 struct
    shi1501_reserved dd ?
    shi1501_security_descriptor dd ?
SHARE_INFO_1501 ends

SESSION_INFO_0 struct
    sesi0_cname dd ?
SESSION_INFO_0 ends

SESSION_INFO_1 struct
    sesi1_cname dd ?
    sesi1_username dd ?
    sesi1_num_opens dd ?
    sesi1_time dd ?
    sesi1_idle_time dd ?
    sesi1_user_flags dd ?
SESSION_INFO_1 ends

SESSION_INFO_2 struct
    sesi2_cname dd ?
    sesi2_username dd ?
    sesi2_num_opens dd ?
    sesi2_time dd ?
    sesi2_idle_time dd ?
    sesi2_user_flags dd ?
    sesi2_cltype_name dd ?
SESSION_INFO_2 ends

SESSION_INFO_10 struct
    sesi10_cname dd ?
    sesi10_username dd ?
    sesi10_time dd ?
    sesi10_idle_time dd ?
SESSION_INFO_10 ends

SESSION_INFO_502 struct
    sesi502_cname dd ?
    sesi502_username dd ?
    sesi502_num_opens dd ?
    sesi502_time dd ?
    sesi502_idle_time dd ?
    sesi502_user_flags dd ?
    sesi502_cltype_name dd ?
    sesi502_transport dd ?
SESSION_INFO_502 ends

CONNECTION_INFO_0 struct
    coni0_id dd ?
CONNECTION_INFO_0 ends

CONNECTION_INFO_1 struct
    coni1_id dd ?
    coni1_type dd ?
    coni1_num_opens dd ?
    coni1_num_users dd ?
    coni1_time dd ?
    coni1_username dd ?
    coni1_netname dd ?
CONNECTION_INFO_1 ends

FILE_INFO_2 struct
    fi2_id dd ?
FILE_INFO_2 ends

FILE_INFO_3 struct
    fi3_id dd ?
    fi3_permissions dd ?
    fi3_num_locks dd ?
    fi3_pathname dd ?
    fi3_username dd ?
FILE_INFO_3 ends

;===================================[ LMSNAME.H ]===================
SERVICE_WORKSTATION equ "LanmanWorkstation"
SERVICE_LM20_WORKSTATION equ "WORKSTATION"
WORKSTATION_DISPLAY_NAME equ "Workstation"
SERVICE_SERVER equ "LanmanServer"
SERVICE_LM20_SERVER equ "SERVER"
SERVER_DISPLAY_NAME equ "Server"
SERVICE_BROWSER equ "BROWSER"
SERVICE_LM20_BROWSER equ SERVICE_BROWSER
SERVICE_MESSENGER equ "MESSENGER"
SERVICE_LM20_MESSENGER equ SERVICE_MESSENGER
SERVICE_NETRUN equ "NETRUN"
SERVICE_LM20_NETRUN equ SERVICE_NETRUN
SERVICE_SPOOLER equ "SPOOLER"
SERVICE_LM20_SPOOLER equ SERVICE_SPOOLER
SERVICE_ALERTER equ "ALERTER"
SERVICE_LM20_ALERTER equ SERVICE_ALERTER
SERVICE_NETLOGON equ "NETLOGON"
SERVICE_LM20_NETLOGON equ SERVICE_NETLOGON
SERVICE_NETPOPUP equ "NETPOPUP"
SERVICE_LM20_NETPOPUP equ SERVICE_NETPOPUP
SERVICE_SQLSERVER equ "SQLSERVER"
SERVICE_LM20_SQLSERVER equ SERVICE_SQLSERVER
SERVICE_REPL equ "REPLICATOR"
SERVICE_LM20_REPL equ SERVICE_REPL
SERVICE_RIPL equ "REMOTEBOOT"
SERVICE_LM20_RIPL equ SERVICE_RIPL
SERVICE_TIMESOURCE equ "TIMESOURCE"
SERVICE_LM20_TIMESOURCE equ SERVICE_TIMESOURCE
SERVICE_AFP equ "AFP"
SERVICE_LM20_AFP equ SERVICE_AFP
SERVICE_UPS equ "UPS"
SERVICE_LM20_UPS equ SERVICE_UPS
SERVICE_XACTSRV equ "XACTSRV"
SERVICE_LM20_XACTSRV equ SERVICE_XACTSRV
SERVICE_TCPIP equ "TCPIP"
SERVICE_LM20_TCPIP equ SERVICE_TCPIP
SERVICE_NBT equ "NBT"
SERVICE_LM20_NBT equ SERVICE_NBT
SERVICE_LMHOSTS equ "LMHOSTS"
SERVICE_LM20_LMHOSTS equ SERVICE_LMHOSTS
SERVICE_TELNET equ "Telnet"
SERVICE_LM20_TELNET equ SERVICE_TELNET
SERVICE_SCHEDULE equ "Schedule"
SERVICE_LM20_SCHEDULE equ SERVICE_SCHEDULE
SERVICE_NTLMSSP equ "NtLmSsp"
SERVICE_DHCP equ "DHCP"
SERVICE_LM20_DHCP equ SERVICE_DHCP
SERVICE_NWSAP equ "NwSapAgent"
SERVICE_LM20_NWSAP equ SERVICE_NWSAP
NWSAP_DISPLAY_NAME equ "NW Sap Agent"
SERVICE_NWCS equ "NWCWorkstation"
SERVICE_DNS_CACHE equ "DnsCache"
SERVICE_W32TIME equ "w32time"
SERVCE_LM20_W32TIME equ SERVICE_W32TIME
SERVICE_KDC equ "kdc"
SERVICE_LM20_KDC equ SERVICE_KDC
SERVICE_RPCLOCATOR equ "RPCLOCATOR"
SERVICE_LM20_RPCLOCATOR equ SERVICE_RPCLOCATOR
SERVICE_TRKSVR equ "TrkSvr"
SERVICE_LM20_TRKSVR equ SERVICE_TRKSVR
SERVICE_TRKWKS equ "TrkWks"
SERVICE_LM20_TRKWKS equ SERVICE_TRKWKS

;====================================[ LMSTATS.H ]======================
STATSOPT_CLR equ 1
STATS_NO_VALUE  equ -1
STATS_OVERFLOW  equ  -2

STAT_WORKSTATION_0 struct
     stw0_start dd ?
     stw0_numNCB_r dd ?
     stw0_numNCB_s dd ?
     stw0_numNCB_a dd ?
     stw0_fiNCB_r dd ?
     stw0_fiNCB_s dd ?
     stw0_fiNCB_a dd ?
     stw0_fcNCB_r dd ?
     stw0_fcNCB_s dd ?
     stw0_fcNCB_a dd ?
     stw0_sesstart dd ?
     stw0_sessfailcon dd ?
     stw0_sessbroke dd ?
     stw0_uses dd ?
     stw0_usefail dd ?
     stw0_autorec dd ?
     stw0_bytessent_r_lo dd ?
     stw0_bytessent_r_hi dd ?
     stw0_bytesrcvd_r_lo dd ?
     stw0_bytesrcvd_r_hi dd ?
     stw0_bytessent_s_lo dd ?
     stw0_bytessent_s_hi dd ?
     stw0_bytesrcvd_s_lo dd ?
     stw0_bytesrcvd_s_hi dd ?
     stw0_bytessent_a_lo dd ?
     stw0_bytessent_a_hi dd ?
     stw0_bytesrcvd_a_lo dd ?
     stw0_bytesrcvd_a_hi dd ?
     stw0_reqbufneed dd ?
     stw0_bigbufneed dd ?
STAT_WORKSTATION_0 ends

STAT_SERVER_0 struct
     sts0_start dd ?
     sts0_fopens dd ?
     sts0_devopens dd ?
     sts0_jobsqueued dd ?
     sts0_sopens dd ?
     sts0_stimedout dd ?
     sts0_serrorout dd ?
     sts0_pwerrors dd ?
     sts0_permerrors dd ?
     sts0_syserrors dd ?
     sts0_bytessent_low dd ?
     sts0_bytessent_high dd ?
     sts0_bytesrcvd_low dd ?
     sts0_bytesrcvd_high dd ?
     sts0_avresponse dd ?
     sts0_reqbufneed dd ?
     sts0_bigbufneed dd ?
STAT_SERVER_0 ends

;===========================[ LMSVC.H ]============================
SERVICE_INFO_0 struct
    svci0_name dd ?
SERVICE_INFO_0 ends

SERVICE_INFO_1 struct
    svci1_name dd ?
    svci1_status dd ?
    svci1_code dd ?
    svci1_pid dd ?
SERVICE_INFO_1 ends

SERVICE_INFO_2 struct
    svci2_name dd ?
    svci2_status dd ?
    svci2_code dd ?
    svci2_pid dd ?
    svci2_text dd ?
    svci2_specific_error dd ?
    svci2_display_name dd ?
SERVICE_INFO_2 ends

SERVICE_INSTALL_STATE equ 3
SERVICE_UNINSTALLED equ 0
SERVICE_INSTALL_PENDING equ 1
SERVICE_UNINSTALL_PENDING equ 2
SERVICE_INSTALLED equ 3
SERVICE_PAUSE_STATE equ 0Ch
LM20_SERVICE_ACTIVE equ 0
LM20_SERVICE_CONTINUE_PENDING equ 4
LM20_SERVICE_PAUSE_PENDING equ 8
LM20_SERVICE_PAUSED equ 0Ch
SERVICE_NOT_UNINSTALLABLE equ 0
SERVICE_UNINSTALLABLE equ 10h
SERVICE_NOT_PAUSABLE equ 0
SERVICE_PAUSABLE equ 20h
SERVICE_REDIR_PAUSED equ 700h
SERVICE_REDIR_DISK_PAUSED equ 100h
SERVICE_REDIR_PRINT_PAUSED equ 200h
SERVICE_REDIR_COMM_PAUSED equ 400h
SERVICE_DOS_ENCRYPTION equ L"ENCRYPT"
SERVICE_CTRL_INTERROGATE equ 0
SERVICE_CTRL_PAUSE equ 1
SERVICE_CTRL_CONTINUE equ 2
SERVICE_CTRL_UNINSTALL equ 3
SERVICE_CTRL_REDIR_DISK equ 1
SERVICE_CTRL_REDIR_PRINT equ 2
SERVICE_CTRL_REDIR_COMM equ 4
SERVICE_IP_NO_HINT equ 0
SERVICE_CCP_NO_HINT equ 0
SERVICE_IP_QUERY_HINT equ 10000h
SERVICE_CCP_QUERY_HINT equ 10000h
SERVICE_IP_CHKPT_NUM equ 0FFh
SERVICE_CCP_CHKPT_NUM equ 0FFh
SERVICE_IP_WAIT_TIME equ 0FF00h
SERVICE_CCP_WAIT_TIME equ 0FF00h
SERVICE_IP_WAITTIME_SHIFT equ 8
SERVICE_NTIP_WAITTIME_SHIFT equ 12
UPPER_HINT_MASK equ 0FF00h
LOWER_HINT_MASK equ 0FFh
UPPER_GET_HINT_MASK equ 0FF00000h
LOWER_GET_HINT_MASK equ 0FF00h
SERVICE_NT_MAXTIME equ 0FFFFh
SERVICE_RESRV_MASK equ 1FFFFh
SERVICE_MAXTIME equ 0FFh
SERVICE_BASE equ 3050
SERVICE_UIC_NORMAL equ 0
SERVICE_UIC_BADPARMVAL equ (SERVICE_BASE + 1)
SERVICE_UIC_MISSPARM equ (SERVICE_BASE + 2)
SERVICE_UIC_UNKPARM equ (SERVICE_BASE + 3)
SERVICE_UIC_RESOURCE equ (SERVICE_BASE + 4)
SERVICE_UIC_CONFIG equ (SERVICE_BASE + 5)
SERVICE_UIC_SYSTEM equ (SERVICE_BASE + 6)
SERVICE_UIC_INTERNAL equ (SERVICE_BASE + 7)
SERVICE_UIC_AMBIGPARM equ (SERVICE_BASE + 8)
SERVICE_UIC_DUPPARM equ (SERVICE_BASE + 9)
SERVICE_UIC_KILL equ (SERVICE_BASE + 10)
SERVICE_UIC_EXEC equ (SERVICE_BASE + 11)
SERVICE_UIC_SUBSERV equ (SERVICE_BASE + 12)
SERVICE_UIC_CONFLPARM equ (SERVICE_BASE + 13)
SERVICE_UIC_FILE equ (SERVICE_BASE + 14)
SERVICE_UIC_M_NULL equ 0
SERVICE_UIC_M_MEMORY equ (SERVICE_BASE + 20)
SERVICE_UIC_M_DISK equ (SERVICE_BASE + 21)
SERVICE_UIC_M_THREADS equ (SERVICE_BASE + 22)
SERVICE_UIC_M_PROCESSES equ (SERVICE_BASE + 23)
SERVICE_UIC_M_SECURITY equ (SERVICE_BASE + 24)
SERVICE_UIC_M_LANROOT equ (SERVICE_BASE + 25)
SERVICE_UIC_M_REDIR equ (SERVICE_BASE + 26)
SERVICE_UIC_M_SERVER equ (SERVICE_BASE + 27)
SERVICE_UIC_M_SEC_FILE_ERR equ (SERVICE_BASE + 28)
SERVICE_UIC_M_FILES equ (SERVICE_BASE + 29)
SERVICE_UIC_M_LOGS equ (SERVICE_BASE + 30)
SERVICE_UIC_M_LANGROUP equ (SERVICE_BASE + 31)
SERVICE_UIC_M_MSGNAME equ (SERVICE_BASE + 32)
SERVICE_UIC_M_ANNOUNCE equ (SERVICE_BASE + 33)
SERVICE_UIC_M_UAS equ (SERVICE_BASE + 34)
SERVICE_UIC_M_SERVER_SEC_ERR equ (SERVICE_BASE + 35)
SERVICE_UIC_M_WKSTA equ (SERVICE_BASE + 37)
SERVICE_UIC_M_ERRLOG equ (SERVICE_BASE + 38)
SERVICE_UIC_M_FILE_UW equ (SERVICE_BASE + 39)
SERVICE_UIC_M_ADDPAK equ (SERVICE_BASE + 40)
SERVICE_UIC_M_LAZY equ (SERVICE_BASE + 41)
SERVICE_UIC_M_UAS_MACHINE_ACCT equ (SERVICE_BASE + 42)
SERVICE_UIC_M_UAS_SERVERS_NMEMB equ (SERVICE_BASE + 43)
SERVICE_UIC_M_UAS_SERVERS_NOGRP equ (SERVICE_BASE + 44)
SERVICE_UIC_M_UAS_INVALID_ROLE equ (SERVICE_BASE + 45)
SERVICE_UIC_M_NETLOGON_NO_DC equ (SERVICE_BASE + 46)
SERVICE_UIC_M_NETLOGON_DC_CFLCT equ (SERVICE_BASE + 47)
SERVICE_UIC_M_NETLOGON_AUTH equ (SERVICE_BASE + 48)
SERVICE_UIC_M_UAS_PROLOG equ (SERVICE_BASE + 49)
SERVICE2_BASE equ 5600
SERVICE_UIC_M_NETLOGON_MPATH equ (SERVICE2_BASE + 0)
SERVICE_UIC_M_LSA_MACHINE_ACCT equ (SERVICE2_BASE + 1)
SERVICE_UIC_M_DATABASE_ERROR equ (SERVICE2_BASE + 2)

;==============================[ LMUSE.H ]======================
USE_INFO_0 struct
    ui0_local dd ?
    ui0_remote dd ?
USE_INFO_0 ends

USE_INFO_1 struct
    ui1_local dd ?
    ui1_remote dd ?
    ui1_password dd ?
    ui1_status dd ?
    ui1_asg_type dd ?
    ui1_refcount dd ?
    ui1_usecount dd ?
USE_INFO_1 ends

USE_INFO_2 struct
    ui2_local dd ?
    ui2_remote dd ?
    ui2_password dd ?
    ui2_status dd ?
    ui2_asg_type dd ?
    ui2_refcount dd ?
    ui2_usecount dd ?
    ui2_username dd ?
    ui2_domainname dd ?
USE_INFO_2 ends

USE_INFO_3 struct
    ui3_ui2 USE_INFO_2 <>
    ui3_flags dd ?
USE_INFO_3 ends

USE_LOCAL_PARMNUM equ 1
USE_REMOTE_PARMNUM equ 2
USE_PASSWORD_PARMNUM equ 3
USE_ASGTYPE_PARMNUM equ 4
USE_USERNAME_PARMNUM equ 5
USE_DOMAINNAME_PARMNUM equ 6
USE_OK equ 0
USE_PAUSED equ 1
USE_SESSLOST equ 2
USE_DISCONN equ 2
USE_NETERR equ 3
USE_CONN equ 4
USE_RECONN equ 5
USE_WILDCARD equ -1 
USE_DISKDEV equ 0
USE_SPOOLDEV equ 1
USE_CHARDEV equ 2
USE_IPC equ 3
CREATE_NO_CONNECT equ 1

;============================[ LMUSEFLG.H]=======================
USE_NOFORCE equ 0
USE_FORCE equ 1
USE_LOTS_OF_FORCE equ 2

STRRET STRUCT
 uType      dd ?
 union                        
    pOleStr    dd ?
    pStr       dd ?
    uOffset    dd ?
    union 
	    cStr       db MAX_PATH dup(?)
	    cstr       db MAX_PATH dup(?)
    ends
 ends
STRRET ENDS

;============================[ RICHEDIT.H ]=======================
GETTEXTEX STRUCT
  cb             DWORD      ? ;DWORD  
  flags          DWORD      ? ;DWORD  
  codepage       DWORD      ? ;UINT
  lpDefaultChar  DWORD      ? ;LPCSTR 
  lpUsedDefChar  DWORD      ? ;LPBOOL 
GETTEXTEX ENDS

SETTEXTEX STRUCT
  flags          DWORD      ?
  codepage       DWORD      ?
SETTEXTEX ENDS

GETTEXTLENGTHEX STRUCT
  flags          DWORD      ?
  codepage       DWORD      ?
GETTEXTLENGTHEX ENDS

EM_AUTOURLDETECT     equ    WM_USER + 91
EM_GETAUTOURLDETECT  equ    WM_USER + 92
EM_SETPALETTE        equ    WM_USER + 93
EM_GETTEXTEX         equ    WM_USER + 94
EM_GETTEXTLENGTHEX   equ    WM_USER + 95
EM_SHOWSCROLLBAR     equ    WM_USER + 96
EM_SETTEXTEX         equ    WM_USER + 97

ST_DEFAULT           equ    0
ST_KEEPUNDO          equ    1
ST_SELECTION         equ    2


;Misc
;-------------------------------------------------------------------------------------------------
HEAP_GENERATE_EXCEPTIONS              EQU  4
HEAP_NO_SERIALIZE                     EQU  1
HEAP_ZERO_MEMORY                      EQU  8

TB_SETINDENT                          EQU WM_USER + 47
TB_SETIMAGELIST                       EQU WM_USER + 48
TB_GETIMAGELIST                       EQU WM_USER + 49
TB_LOADIMAGES                         EQU WM_USER + 50
TB_GETRECT                            EQU WM_USER + 51
TB_SETHOTIMAGELIST                    EQU WM_USER + 52
TB_GETHOTIMAGELIST                    EQU WM_USER + 53
TB_SETDISABLEDIMAGELIST               EQU WM_USER + 54
TB_GETDISABLEDIMAGELIST               EQU WM_USER + 55
TB_SETSTYLE                           EQU WM_USER + 56
TB_GETSTYLE                           EQU WM_USER + 57
TB_GETBUTTONSIZE                      EQU WM_USER + 58
TB_SETBUTTONWIDTH                     EQU WM_USER + 59
TB_SETMAXTEXTROWS                     EQU WM_USER + 60
TB_GETTEXTROWS                        EQU WM_USER + 61
TB_GETOBJECT                          EQU WM_USER + 62
TB_GETHOTITEM                         EQU WM_USER + 71
TB_SETHOTITEM                         EQU WM_USER + 72
TB_SETANCHORHIGHLIGHT                 EQU WM_USER + 73
TB_GETANCHORHIGHLIGHT                 EQU WM_USER + 74
TB_MAPACCELERATOR                    EQU WM_USER + 78

LVS_EX_GRIDLINES                      EQU 00000001h
LVS_EX_SUBITEMIMAGES                  EQU 00000002h
LVS_EX_CHECKBOXES                     EQU 00000004h
LVS_EX_TRACKSELECT                    EQU 00000008h
LVS_EX_HEADERDRAGDROP                 EQU 00000010h
LVS_EX_FULLROWSELECT                  EQU 00000020h
LVS_EX_ONECLICKACTIVATE               EQU 00000040h
LVS_EX_TWOCLICKACTIVATE               EQU 00000080h
LVS_EX_FLATSB                         EQU 00000100h
LVS_EX_REGIONAL                       EQU 00000200h
LVS_EX_INFOTIP                        EQU 00000400h
LVS_EX_UNDERLINEHOT                   EQU 00000800h
LVS_EX_UNDERLINECOLD                  EQU 00001000h
LVS_EX_MULTIWORKAREAS                 EQU 00002000h
LVS_EX_LABELTIP                       EQU 00004000h

LVCF_FMT                              EQU 0001h
LVCF_WIDTH                            EQU 0002h
LVCF_TEXT                             EQU 0004h
LVCF_SUBITEM                          EQU 0008h
LVCF_IMAGE                            EQU 0010h
LVCF_ORDER                            EQU 0020h
LVCFMT_LEFT                           EQU 0000h
LVCFMT_RIGHT                          EQU 0001h
LVCFMT_CENTER                         EQU 0002h
LVCFMT_JUSTIFYMASK                    EQU 0003h
LVCFMT_IMAGE                          EQU 0800h
LVCFMT_BITMAP_ON_RIGHT                EQU 1000h
LVCFMT_COL_HAS_IMAGES                 EQU 8000h

LVM_SETEXTENDEDLISTVIEWSTYLE          EQU LVM_FIRST + 54
LVM_GETEXTENDEDLISTVIEWSTYLE          EQU LVM_FIRST + 55


;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
LVM_GETHEADER                         EQU LVM_FIRST + 31
LVM_GETITEM                           EQU LVM_FIRST + 5
LVM_GETITEMW                          EQU LVM_FIRST + 75
LVM_SETITEM                           EQU LVM_FIRST + 6
LVM_SETITEMW                          EQU LVM_FIRST + 76
LVM_INSERTITEM                        EQU LVM_FIRST + 7
LVM_INSERTITEMW                       EQU LVM_FIRST + 77
LVM_DELETEITEM                        EQU LVM_FIRST + 8
LVM_DELETEALLITEMS                    EQU LVM_FIRST + 9
LVM_GETCALLBACKMASK                   EQU LVM_FIRST + 10
LVM_SETCALLBACKMASK                   EQU LVM_FIRST + 11


;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
HDM_SETIMAGELIST                      EQU HDM_FIRST + 8
HDM_GETIMAGELIST                      EQU HDM_FIRST + 9


;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
HDI_IMAGE                             EQU 0020h
HDI_DI_SETITEM                        EQU 0040h
HDI_ORDER                             EQU 0080h


;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
HDF_LEFT                              EQU 0
HDF_RIGHT                             EQU 1
HDF_CENTER                            EQU 2
HDF_JUSTIFYMASK                       EQU 0003h
HDF_RTLREADING                        EQU 4
HDF_OWNERDRAW                         EQU 8000h
HDF_STRING                            EQU 4000h
HDF_BITMAP                            EQU 2000h
HDF_BITMAP_ON_RIGHT                   EQU 1000h
HDF_IMAGE                             EQU 0800h

;OLE Structures and Equates
;----------------------------------------------------------------------------

S_OK                    equ  00000000h
S_FALSE                 equ  00000001h

;----------------------------------------------------------------------------

CLSCTX_ALL              equ CLSCTX_INPROC_SERVER or CLSCTX_INPROC_HANDLER or CLSCTX_LOCAL_SERVER
CLSCTX_INPROC           equ CLSCTX_INPROC_SERVER or CLSCTX_INPROC_HANDLER
CLSCTX_SERVER           equ  CLSCTX_INPROC_SERVER or CLSCTX_LOCAL_SERVER 

REGCLS_SINGLEUSE        equ 0
REGCLS_MULTIPLEUSE      equ 1
REGCLS_MULTI_SEPARATE   equ 2

MARSHALINTERFACE_MIN    equ 500
CWCSTORAGENAME          equ 32
STGM_DIRECT             equ 0
STGM_TRANSACTED         equ 00010000h
STGM_SIMPLE             equ 08000000h
STGM_READ               equ 0
STGM_WRITE              equ 00000001h
STGM_READWRITE          equ 00000002h
STGM_SHARE_DENY_NONE    equ 00000040h
STGM_SHARE_DENY_READ    equ 00000030h
STGM_SHARE_DENY_WRITE   equ 00000020h
STGM_SHARE_EXCLUSIVE    equ 00000010h
STGM_PRIORITY           equ 00040000h
STGM_DELETEONRELEASE    equ 04000000h
STGM_CREATE             equ 00001000h
STGM_CONVERT            equ 00020000h
STGM_FAILIFTHERE        equ 0

IID   STRUCT DWORD
    Data1        dd ?
    Data2        dw ?
    Data3        dw ?
    Data4        db 8 dup(?)
IID ENDS

BFFM_SETSTATUSTEXT      equ  WM_USER + 100
BFFM_SETSELECTION       equ  WM_USER + 102

CMIC_VALID_SEE_FLAGS    equ  SEE_VALID_CMIC_FLAGS     ; Internal 

COLORSCHEME struct
   dwSize dd ?
   clrBtnHighlight COLORREF ?
   clrBtnShadow COLORREF ?
COLORSCHEME ends

NMMOUSE struct
    hdr NMHDR <>
    dwItemSpec dd ?
    dwItemData dd ?
    pt POINT <>
    dwHitInfo dd ?
NMMOUSE ends

NMCLICK equ <NMMOUSE>

HOVER_DEFAULT   equ 0FFFFFFFFh

TRACKMOUSEEVENT struct
    cbSize dd ?
    dwFlags dd ?
    hwndTrack dd ?
    dwHoverTime dd ?
TRACKMOUSEEVENT ends

TBSTYLE_AUTOSIZE equ 10h
TB_SETCOLORSCHEME     equ  CCM_SETCOLORSCHEME
SWP_NOSENDCHANGING  equ 400h
CC_ANYCOLOR              equ 100h
TCN_FOCUSCHANGE        equ TCN_FIRST - 4

STREAM_SEEK_SET	equ  0
STREAM_SEEK_CUR	equ 1
STREAM_SEEK_END	equ  2

GTL_DEFAULT	 equ 0
GTL_USECRLF	 equ 1
GTL_PRECISE	equ 2
GTL_CLOSE equ 4
GTL_NUMCHARS equ 8
GTL_NUMBYTES equ 16

TM_PLAINTEXT			equ 1
TM_RICHTEXT				equ 2
TM_SINGLELEVELUNDO		equ 4
TM_MULTILEVELUNDO		equ 8
TM_SINGLECODEPAGE		equ 16
TM_MULTICODEPAGE		equ 32

hWndOwner equ hwndOwner

TO_ADVANCEDTYPOGRAPHY equ	1
TO_SIMPLELINEBREAK	equ	2
TCS_FLATBUTTONS         equ 8

TTN_GETDISPINFO equ TTN_FIRST
TTF_DI_SETITEM          equ 8000h
RBBS_USECHEVRON     equ 200h
GT_DEFAULT	equ	0
GT_USECRLF	equ	1

DLLVERSIONINFO struct
        cbSize dd ?
        dwMajorVersion dd ?
        dwMinorVersion dd ?
        dwBuildNumber dd ?
        dwPlatformID dd ?
DLLVERSIONINFO ends

BTNS_SHOWTEXT   equ 40h
I_IMAGENONE equ -2

DTM_FIRST   equ 1000h
DTM_SETFORMAT equ DTM_FIRST + 5
DTM_SETSYSTEMTIME   equ DTM_FIRST + 2
DTM_GETSYSTEMTIME   equ DTM_FIRST + 1
MAX_COMPUTERNAME_LENGTH equ 15

; ----------------------------------------
; aditional winsock equates and structure
; contributed by Thomas Bleeker.
; ----------------------------------------
FD_READ_BIT                     equ 0
FD_WRITE_BIT                    equ 1
FD_OOB_BIT                      equ 2
FD_ACCEPT_BIT                   equ 3
FD_CONNECT_BIT                  equ 4
FD_CLOSE_BIT                    equ 5

FD_QOS_BIT                      equ 6
FD_GROUP_QOS_BIT                equ 7
FD_ROUTING_INTERFACE_CHANGE_BIT equ 8
FD_ADDRESS_LIST_CHANGE_BIT      equ 9

FD_MAX_EVENTS                   equ 10
FD_ALL_EVENTS                   equ (1 SHL FD_MAX_EVENTS) - 1

WSANETWORKEVENTS STRUCT
    lNetworkEvents              dd  ?
    iErrorCode                  dd  FD_MAX_EVENTS dup (?)
WSANETWORKEVENTS ENDS




