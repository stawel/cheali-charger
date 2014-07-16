//*****************************************************************************
//
//! \file ModuleID.h
//! \brief 
//! \version v1.02.002
//! \date 21/09/2011
//! \author Nuvoton
//! \copy
//!
//! Copyright (c) Nuvoton Technology Corp. All rights reserved.
//
//*****************************************************************************

#ifndef __MODULE_ID_H__
#define __MODULE_ID_H__


typedef enum
{
    //*************************************************************************
    //
    //! Module ID valid range: 0 ~ 255
    //
    //*************************************************************************

    //*************************************************************************
    //
    //! Driver: Module ID
    //
    //*************************************************************************

    //
    // DrvProtect
    //
    MODULE_ID_DRVPROTECT        = 0,        

    //
    // DrvADC
    //
    MODULE_ID_DRVADC            = 2,        

    //
    // DrvAIC
    //
    MODULE_ID_DRVAIC            = 4,        

    //
    // DrvAPU
    //
    MODULE_ID_DRVAPU            = 6,        

    //
    // DrvAudioADC
    //
    MODULE_ID_DRVAUDIOADC       = 8,        

    //
    // DrvCache
    //
    MODULE_ID_DRVCACHE          = 10,        

    //
    // DrvCAN
    //
    MODULE_ID_DRVCAN            = 11,        

    //
    // DrvEBI
    //
    MODULE_ID_DRVEBI            = 12,    

    //
    // DrvEDMA
    //
    MODULE_ID_DRVEDMA           = 13,    

    //
    // DrvGDMA
    //
    MODULE_ID_DRVGDMA           = 14,    

    //
    // DrvFSC
    //
    MODULE_ID_DRVFSC            = 15,        

    //
    // DrvGE
    //
    MODULE_ID_DRVGE             = 16,        

    //
    // DrvFMC
    //
    MODULE_ID_DRVFMC            = 17,        

    //
    // DrvGPIO
    //
    MODULE_ID_DRVGPIO           = 18,        

    //
    // DrvGPU
    //
    MODULE_ID_DRVGPU            = 20,        

    //
    // DrvI2C (S/W I2C for Non-UL; H/W I2C for UL)
    //
    MODULE_ID_DRVI2C            = 22,        

    //
    // DrvI2S
    //
    MODULE_ID_DRVI2S            = 24,        

    //
    // DrvI2SM
    //
    MODULE_ID_DRVI2SM           = 26,        

    //
    // DrvMPU
    //
    MODULE_ID_DRVMPU            = 28,        

    //
    // DrvNAND
    //
    MODULE_ID_DRVNAND           = 30,    

    //
    // DrvNOR
    //
    MODULE_ID_DRVNOR            = 32,        

    //
    // DrvPDMA
    //
    MODULE_ID_DRVPDMA           = 33,       

    //
    // DrvPWM
    //
    MODULE_ID_DRVPWM            = 34,        

    //
    // DrvPS2
    //
    MODULE_ID_DRVPS2            = 35,        

    //
    // DrvRTC
    //
    MODULE_ID_DRVRTC            = 36,        

    //
    // DrvSDCard
    //
    MODULE_ID_DRVSDCARD         = 38,        

    //
    // DrvSerialIO
    //
    MODULE_ID_DRVSIO            = 39,        
    
    //
    // DrvSPI
    //
    MODULE_ID_DRVSPI            = 40,        

    //
    // DrvSPIMS
    //
    MODULE_ID_DRVSPIMS          = 41,        

    //
    // DrvSPIFlash
    //
    MODULE_ID_DRVSPIFLASH       = 42,        

    //
    // DrvSPIM
    //
    MODULE_ID_DRVSPIM           = 43,        

    //
    // DrvSystem
    //
    MODULE_ID_DRVSYS            = 44,        

    //
    // DrvSPU
    //
    MODULE_ID_DRVSPU            = 45,        

    //
    // DrvTimer
    //
    MODULE_ID_DRVTIMER          = 46,    

    //
    // DrvUART
    //
    MODULE_ID_DRVUART           = 48,    

    //
    // DrvUSB
    //
    MODULE_ID_DRVUSB            = 50,        

    //
    // DrvUSBH
    //
    MODULE_ID_DRVUSBH           = 52,    

    //
    // DrvVDMA
    //
    MODULE_ID_DRVVDMA           = 54,        

    //
    // DrvVideoIn
    //
    MODULE_ID_DRVVIDEOIN        = 56,        
    //
    // DrvVPOST
    //
    MODULE_ID_DRVVPOST          = 58,    
    
    //
    // DrvVRAM
    //
    MODULE_ID_DRVVRAM           = 60,        

    //
    // DrvW55U02
    //
    MODULE_ID_DRVW55U02         = 62,    

    //
    // DrvI2CH (H/W I2C for Non-UL)
    //
    MODULE_ID_DRVI2CH           = 64,    

    //
    // DrvWDT
    //
    MODULE_ID_DRVWDT            = 66,        

    //
    // DrvJPEG
    //
    MODULE_ID_DRVJPEG           = 68,        
    
    //
    // DrvZeroG
    //
    MODULE_ID_DRVZEROG          = 70,    

    //
    // DrvSI2C (S/W I2C for Non-UL)
    //
    MODULE_ID_DRVSI2C           = 71,        
    
    //*************************************************************************
    //
    //! Audio: Module ID    
    //
    //*************************************************************************  
    
    //
    // AEC
    //
    MODULE_ID_AEC               = 81,        

    //
    // BeatDetection
    //
    MODULE_ID_BEATDET           = 82,    

    //
    // SoundEffect
    //
    MODULE_ID_SNDEFF            = 83,        

    //
    // AudioSynthesizer
    //
    MODULE_ID_AUDIOSYN          = 84,        

    //
    // G726ADPCM
    //
    MODULE_ID_G726ADPCM         = 85,        

    //
    // IMAAdpcmCodec
    //
    MODULE_ID_IMAADPCM          = 86,        

    //
    // MP3Decoder
    //
    MODULE_ID_MP3DEC            = 88,        

    //
    // PitchChanger
    //
    MODULE_ID_PITCHCHANGE       = 90,        

    //
    // WavFileUtil
    //
    MODULE_ID_WAVFILEUTIL       = 92,        

    //
    // WMADecoder
    //
    MODULE_ID_WMADEC            = 96,    

    //
    // WMADecoderWithDrm
    //
    MODULE_ID_WMADECDRM         = 98,        

    //
    // AudioCtrl
    //
    MODULE_ID_AUDIOCTRL         = 100,    

    //
    // Equalizer
    //
    MODULE_ID_EQ                = 106,        

    //
    // OggDecoder
    //
    MODULE_ID_OGGDEC            = 110,        

    //
    // MP3Encoder
    //
    MODULE_ID_MP3ENC            = 112,        

    //
    // UltraAudioDec
    //
    MODULE_ID_UADEC             = 114,        

    //
    // UltraLowSpeechDec
    //
    MODULE_ID_ULSPEECHDEC       = 115,    

    //
    // UltraSpeechDec
    //
    MODULE_ID_USPEECHDEC        = 116,        

    //
    // SpeechRecog
    //
    MODULE_ID_SPEECHRECOG       = 118,    
    
    //*************************************************************************
    //
    //! File System: Module ID    
    //
    //************************************************************************* 

    //
    // FileSystem
    //
    MODULE_ID_FS                = 120,    
    
    //*************************************************************************
    //
    //! Flash Lite: Module ID
    //
    //*************************************************************************     

    //
    // FlashLite
    //
    MODULE_ID_FL                = 128,        
    
    //*************************************************************************
    //
    //! Communication: Module ID    
    //
    //*************************************************************************     

    //
    // Keypad
    //
    MODULE_ID_KEYPAD            = 130,        

    //
    // LwIP
    //
    MODULE_ID_LWIP              = 132,        

    //
    // WLanMgr
    //
    MODULE_ID_WLANMGR           = 134,        

    //
    // HTTPD
    //
    MODULE_ID_HTTPD             = 136,        

    //
    // VirtualCOM
    //
    MODULE_ID_VIRTUALCOM        = 139,        

    //*************************************************************************
    //
    //! Graphics: Module ID
    //
    //*************************************************************************

    //
    // GraphicsResLoader
    //
    MODULE_ID_GFXRESLDR         = 140,        

    //
    // GraphicsLib
    //
    MODULE_ID_GFXLIB            = 141,        

    //
    // ImageProcess
    // 
    MODULE_ID_IMGPROC           = 142,        

    //
    // JPEGCodec
    //
    MODULE_ID_JPEG              = 144,    

    //
    // PNGCodec
    //
    MODULE_ID_PNGDEC            = 146,        

    //
    // BarCode2D
    //
    MODULE_ID_BARCODE2D         = 148,    

    //
    // PtnRecog
    //
    MODULE_ID_PTNRECOG          = 150,    

    //
    // MotionDection
    //
    MODULE_ID_MOTIONDET         = 152,    

    //*************************************************************************
    //
    //! Storage: Module ID
    //
    //*************************************************************************

    //
    // StorageInterface
    //
    MODULE_ID_STORIF            = 160,        

    //
    // SDCard
    //
    MODULE_ID_SDCARD            = 161,        

    //
    // SysNand
    //
    MODULE_ID_SYSNAND           = 162,        

    //
    // SPIFlash
    //
    MODULE_ID_SPIFLASH          = 163,        

    //
    // WriterInterface
     //
    MODULE_ID_WTRIF             = 164,    

    //
    // NORFlash
    //
    MODULE_ID_NORFLASH          = 165,        

    //
    // SysNandLite
    //
    MODULE_ID_SYSNANDLITE       = 166,        
    
    //*************************************************************************
    //
    //! System: Module ID
    //
    //*************************************************************************    

    //
    // InterruptManager
    //
    MODULE_ID_INTMGR            = 180,    

    //
    // BlockLoader
    //
    MODULE_ID_BLKLDR            = 181,        

    //
    // MemoryManager
    //
    MODULE_ID_MEMMGR            = 182,        

    //
    // EventManager
    //
    MODULE_ID_EVTMGR            = 183,    

    //
    // Profiling
    //
    MODULE_ID_PROF              = 184,    

    //
    // ProgramLoader
    //
    MODULE_ID_PROGLDR           = 186,        

    //
    // SysInfra
    //
    MODULE_ID_SYSINFRA          = 188,        

    //
    // TimerCtrl
    //
    MODULE_ID_TIMERCTRL         = 190,    

    //
    // TimeUtil
    //
    MODULE_ID_TIMEUTIL          = 192,        

    //
    // ConsoleProgramLoader
    //
    MODULE_ID_CONPROGLDR        = 194,    

    //*************************************************************************
    //
    //! USB: Module ID    
    //
    //*************************************************************************        

    //
    // USBCoreH
    //
    MODULE_ID_USBCOREH          = 78,        

    //
    // HID
    //
    MODULE_ID_HID               = 220,        

    //
    // MassStor
    //
    MODULE_ID_MASSSTOR          = 222,    

    //
    // MassStorHID
    //
    MODULE_ID_MASSSTORHID       = 224,    

    //
    // MassStorLite
    //
    MODULE_ID_MASSSTORLITE      = 226,        

    //
    // MTP
    //
    MODULE_ID_MTP               = 230,        

    //
    // USBInfra
    //
    MODULE_ID_USBINFRA          = 232,    

    //
    // UAC
    //
    MODULE_ID_UAC               = 234,        

    //
    // UAVC
    //
    MODULE_ID_UAVC              = 236,    

    //
    // UVC
    //
    MODULE_ID_UVC               = 238,    

    //
    // MassStorH
    //
    MODULE_ID_MASSSTORH         = 252,    

    //
    // HIDH
    //
    MODULE_ID_HIDH              = 254,        

    //*************************************************************************
    //
    //! Security: Module ID    
    //
    //*************************************************************************        

    //
    // MsDrmPd
    //
    MODULE_ID_MSDRMPD           = 228,        
    
    //*************************************************************************
    //
    //! Video: Module ID    
    //
    //*************************************************************************        

    //
    // AVICodec
    //
    MODULE_ID_AVICODEC          = 240,        

    //
    // MJpegCodec
    //
    MODULE_ID_MJPEG             = 242,

    //
    // WiViCore
    //
    MODULE_ID_WIVICORE          = 244,    

    //
    // WiViDec
    //
    MODULE_ID_WIVI              = 246,        

    //
    // AVControl
    //
    MODULE_ID_AVCTRL            = 248,    

    //
    // AVIUtility
    //
    MODULE_ID_AVIUTIL           = 250,        

    //*************************************************************************
    //
    //! Wireless Toy    
    //
    //*************************************************************************    

    //
    // WTChannel
    //
    MODULE_ID_WTCHAN            = 168,        

    //
    // WTCMDService
    //
    MODULE_ID_WTCMDSERV         = 170,        

    //
    // WTDisplay
    //
    MODULE_ID_WTDISPLAY         = 172,    

    //
    // WTMedia
    //
    MODULE_ID_WTMEDIA           = 174,        

    //
    // WTSystem
    //
    MODULE_ID_WTSYS             = 176,        

    //
    // WTTransport
    //
    MODULE_ID_WTTRANS           = 178,        
    
    //*************************************************************************
    //
    //! Module ID valid range: 0 ~ 255
    //
    //*************************************************************************            
} E_SYSINFRA_MODULE_ID;


#endif /* __MODULE_ID_H__ */
