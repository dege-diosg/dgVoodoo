;MultimÇdia header
;----------------------------------------------------------------------------

MAXPNAMELEN     EQU     32

MM_WAVE_MAPPER  EQU     2               ;Wave Mapper
;----------------------------------------------------------------------------
;Kimeneti eszkîz jellemzìit le°r¢ strukt£ra
WAVEOUTCAPS     STRUC

wMid            DW      ?               ;manufacturer ID
wPid            DW      ?               ;product ID
vDriverVersion  DD      ?               ;version of the driver
szPname         DB      MAXPNAMELEN DUP(?)
                                        ;product name (NULL terminated string)
dwFormats       DD      ?               ;formats supported
wChannels       DW      ?               ;number of sources supported
wReserved1      DW      ?               ;packing
dwSupport       DD      ?               ;functionality supported by driver

WAVEOUTCAPS     ENDS
;----------------------------------------------------------------------------
;KikÅldendì adatot jellemzì strukt£ra

WAVEFORMATEX    STRUC

wFormatTag      DW      ?               ;format type
nChannels       DW      ?               ;number of channels (i.e. mono, stereo...)
nSamplesPerSec  DD      ?               ;sample rate
nAvgBytesPerSec DD      ?               ;for buffer estimation
nBlockAlign     DW      ?               ;block size of data
wBitsPerSample  DW      ?               ;number of bits per sample of mono data
cbSize          DW      ?               ;the count in bytes of the size of
                                        ;extra information (after cbSize)
WAVEFORMATEX    ENDS

;----------------------------------------------------------------------------

WAVEHDR         STRUC

lpData          LPSTR   ?               ;pointer to locked data buffer
dwBufferLength  DD      ?               ;length of data buffer
dwBytesRecorded DD      ?               ;used for input only
dwUser          DD      ?               ;for client's use
dwFlags         DD      ?               ;assorted flags (see defines)
dwLoops         DD      ?               ;loop control counter
                DD      ?               ;reserved for driver
reserved        DD      ?               ;reserved for driver

WAVEHDR         ENDS

;----------------------------------------------------------------------------
;flags for wFormatTag field of WAVEFORMAT
WAVE_FORMAT_PCM EQU    1

;defines for dwFormat field of WAVEINCAPS and WAVEOUTCAPS
WAVE_INVALIDFORMAT      EQU     00000000h       ;invalid format */
WAVE_FORMAT_1M08        EQU     00000001h       ;11.025 kHz, Mono,   8-bit
WAVE_FORMAT_1S08        EQU     00000002h       ;11.025 kHz, Stereo, 8-bit
WAVE_FORMAT_1M16        EQU     00000004h       ;11.025 kHz, Mono,   16-bit
WAVE_FORMAT_1S16        EQU     00000008h       ;11.025 kHz, Stereo, 16-bit
WAVE_FORMAT_2M08        EQU     00000010h       ;22.05  kHz, Mono,   8-bit
WAVE_FORMAT_2S08        EQU     00000020h       ;22.05  kHz, Stereo, 8-bit
WAVE_FORMAT_2M16        EQU     00000040h       ;22.05  kHz, Mono,   16-bit
WAVE_FORMAT_2S16        EQU     00000080h       ;22.05  kHz, Stereo, 16-bit
WAVE_FORMAT_4M08        EQU     00000100h       ;44.1   kHz, Mono,   8-bit
WAVE_FORMAT_4S08        EQU     00000200h       ;44.1   kHz, Stereo, 8-bit
WAVE_FORMAT_4M16        EQU     00000400h       ;44.1   kHz, Mono,   16-bit
WAVE_FORMAT_4S16        EQU     00000800h       ;44.1   kHz, Stereo, 16-bit


;flags used with waveOutOpen(), waveInOpen(), midiInOpen(), and
;midiOutOpen() to specify the type of the dwCallback parameter.

CALLBACK_TYPEMASK       EQU     00070000h      ;callback type mask
CALLBACK_NULL           EQU     00000000h      ;no callback
CALLBACK_WINDOW         EQU     00010000h      ;dwCallback is a HWND
CALLBACK_TASK           EQU     00020000h      ;dwCallback is a HTASK
CALLBACK_FUNCTION       EQU     00030000h      ;dwCallback is a FARPROC
CALLBACK_THREAD     EQU (CALLBACK_TASK)        ;thread ID replaces 16 bit task
CALLBACK_EVENT          EQU     00050000h      ;dwCallback is an EVENT Handle

;----------------------------------------------------------------------------
;flags for dwFlags field of WAVEHDR
WHDR_DONE               EQU     00000001        ;done bit
WHDR_PREPARED           EQU     00000002        ;set if this header has been prepared
WHDR_BEGINLOOP          EQU     00000004        ;loop start block
WHDR_ENDLOOP            EQU     00000008        ;loop end block
WHDR_INQUEUE            EQU     00000010        ;reserved for driver

;----------------------------------------------------------------------------
;flags for callback
MM_WOM_OPEN             EQU     3BBh
MM_WOM_CLOSE            EQU     3BCh
MM_WOM_DONE             EQU     3BDh

;flags for fuEvent parameter of timeSetEvent() function
TIME_ONESHOT            EQU     0000h           ;program timer for single event
TIME_PERIODIC           EQU     0001h           ;program for continuous periodic event

;----------------------------------------------------------------------------
MMSYSERR_BASE           EQU     0
WAVERR_BASE             EQU     32
MIDIERR_BASE            EQU     64
TIMERR_BASE             EQU     96
JOYERR_BASE             EQU     160
MCIERR_BASE             EQU     256
MIXERR_BASE             EQU     1024

MCI_STRING_OFFSET       EQU     512
MCI_VD_OFFSET           EQU     1024
MCI_CD_OFFSET           EQU     1088
MCI_WAVE_OFFSET         EQU     1152
MCI_SEQ_OFFSET          EQU     1216

;----------------------------------------------------------------------------
;                       General error return values

;general error return values
MMSYSERR_NOERROR        EQU     0                    ;no error
MMSYSERR_ERROR          EQU     (MMSYSERR_BASE + 1)  ;unspecified error
MMSYSERR_BADDEVICEID    EQU     (MMSYSERR_BASE + 2)  ;device ID out of range
MMSYSERR_NOTENABLED     EQU     (MMSYSERR_BASE + 3)  ;driver failed enable
MMSYSERR_ALLOCATED      EQU     (MMSYSERR_BASE + 4)  ;device already allocated
MMSYSERR_INVALHANDLE    EQU     (MMSYSERR_BASE + 5)  ;device handle is invalid
MMSYSERR_NODRIVER       EQU     (MMSYSERR_BASE + 6)  ;no device driver present
MMSYSERR_NOMEM          EQU     (MMSYSERR_BASE + 7)  ;memory allocation error
MMSYSERR_NOTSUPPORTED   EQU     (MMSYSERR_BASE + 8)  ;function isn't supported
MMSYSERR_BADERRNUM      EQU     (MMSYSERR_BASE + 9)  ;error value out of range
MMSYSERR_INVALFLAG      EQU     (MMSYSERR_BASE + 10) ;invalid flag passed
MMSYSERR_INVALPARAM     EQU     (MMSYSERR_BASE + 11) ;invalid parameter passed
MMSYSERR_HANDLEBUSY     EQU     (MMSYSERR_BASE + 12) ;handle being used
                                                     ;simultaneously on another
                                                     ;thread (eg callback)
MMSYSERR_INVALIDALIAS   EQU     (MMSYSERR_BASE + 13) ;specified alias not found
MMSYSERR_BADDB          EQU     (MMSYSERR_BASE + 14) ;bad registry database
MMSYSERR_KEYNOTFOUND    EQU     (MMSYSERR_BASE + 15) ;registry key not found
MMSYSERR_READERROR      EQU     (MMSYSERR_BASE + 16) ;registry read error
MMSYSERR_WRITEERROR     EQU     (MMSYSERR_BASE + 17) ;registry write error
MMSYSERR_DELETEERROR    EQU     (MMSYSERR_BASE + 18) ;registry delete error
MMSYSERR_VALNOTFOUND    EQU     (MMSYSERR_BASE + 19) ;registry value not found
MMSYSERR_NODRIVERCB     EQU     (MMSYSERR_BASE + 20) ;driver does not call DriverCallback
MMSYSERR_LASTERROR      EQU     (MMSYSERR_BASE + 20) ;last error in range

;----------------------------------------------------------------------------
;waveform audio error return values
WAVERR_BADFORMAT        EQU     (WAVERR_BASE + 0)    ;unsupported wave format
WAVERR_STILLPLAYING     EQU     (WAVERR_BASE + 1)    ;still something playing
WAVERR_UNPREPARED       EQU     (WAVERR_BASE + 2)    ;header not prepared
WAVERR_SYNC             EQU     (WAVERR_BASE + 3)    ;device is synchronous
WAVERR_LASTERROR        EQU     (WAVERR_BASE + 3)    ;last error in range
;----------------------------------------------------------------------------
;device ID for wave device mapper
WAVE_MAPPER             EQU	-1

;flags for dwFlags parameter in waveOutOpen() and waveInOpen()
WAVE_FORMAT_QUERY       EQU     0001h
WAVE_ALLOWSYNC          EQU     0002h
WAVE_MAPPED             EQU     0004h
WAVE_FORMAT_DIRECT      EQU     0008h
WAVE_FORMAT_DIRECT_QUERY EQU     (WAVE_FORMAT_QUERY | WAVE_FORMAT_DIRECT)
