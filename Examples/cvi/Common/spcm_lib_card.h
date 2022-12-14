/*
**************************************************************************

spcm_lib_card.h                                (c) Spectrum GmbH , 01/2006

**************************************************************************

defines the library functions as external to use them with projects where
the lib is directly included (like dll) and where the lib has to be loaded
separately (like LabWindows or unsupported C compiler)

**************************************************************************
*/

#ifndef SPCM_LIB_CARD_H
#define SPCM_LIB_CARD_H

// ----- include standard driver header from library -----
#include "../c_header/dlltyp.h"
#include "../c_header/regs.h"
#include "../c_header/spcerr.h"
#include "../c_header/spcm_drv.h"



#define SPCM_MAX_AIRANGE    8
#define SPCM_MAX_AICHANNEL  16
#define SPCM_MAX_AOCHANNEL  4
#define SPCM_MAX_DIOCHANNEL 64


/*
**************************************************************************
structure with different card information and setup information that is
used by the setup and data processing routines
**************************************************************************
*/

// different card functionalities
typedef enum E_SPCM_CARDFNC {AnalogIn, AnalogOut, DigitalOut, DigitalIn, DigitalIO} E_SPCM_CARDFNC;

// card information structure
typedef struct 
    {

    // information from the card
    drv_handle      hDrv;                   // handle to opened card driver
    int32           lCardIdx;               // index of card (from open), just for display
    int32           lCardType;              // card type as listed in the manual
    E_SPCM_CARDFNC  eCardFunction;          // function of the card
    int32           lSerialNumber;          // serial number of card
    int64           llInstMemBytes;         // installed on-board memory in bytes
    int32           lFeatureMap;            // bitmap with installed card features
    int32           lMaxChannels;           // number of channels (analog or digital)
    int32           lModulesCount;          // number of installed modules for data sorting algorithm
    int32           lBytesPerSample;        // number of bytes for each sample (analog data)
    int64           llMaxSamplerate;        // maximum sampling rate

    int32           lLibVersion;            // version of the library
    int32           lKernelVersion;         // version of the kernel driver

    int32           lCtrlFwVersion;         // version of main control firmware
    int32           lBaseHwVersion;         // version of base hardware
    int32           lModHwVersion;          // version of module hardware
    int32           lModFwVersion;          // version of module firmware

    // current settings
    bool            bSetError;              // one of the functions generated an error
    char            szError[ERRORTEXTLEN];  // space for the error text
    uint64          qwSetChEnableMap;       // current channel enable map
    int64           llSetMemsize;           // programmed memory size
    int32           lSetChannels;           // number of used channels for this run
    int64           llSetSamplerate;        // current selected sampling rate (1 for external)
    int32           lOversampling;          // currently active oversampling factor

    // card function dependant details
    union
        {

        // analog input cards
        struct ST_SPCM_AI
            {
            int32   lResolution;                    // resolution of analog channels
            int32   lRangeCount;                    // number of analog input ranges
            int32   lRangeMin[SPCM_MAX_AIRANGE];    // analog input ranges
            int32   lRangeMax[SPCM_MAX_AIRANGE];    // ...
            bool    bInputTermAvailable;            // input termination available
            bool    bDiffModeAvailable;             // differential mode available
            bool    bOffsPercentMode;               // offset programmed in percent of range

            int32   lSetRange[SPCM_MAX_AICHANNEL];  // current used input range for each channel
            int32   lSetOffset[SPCM_MAX_AICHANNEL]; // current set input offset
            } stAI;

        // analog output cards
        struct ST_SPCM_AO
            {
            int32   lResolution;                    // resolution of analog channels
            bool    bGainProgrammable;              // programmable gain available 
            bool    bOffsetProgrammable;            // programmable offset available
            bool    bFilterAvailable;               // programmable filters available
            bool    bStopLevelProgrammable;         // programmable stop level available
            bool    bDiffModeAvailable;             // differential mode available
            } stAO;

        // digital input, outputs or i/o cards
        struct ST_SPCM_DIO
            {
            int32   lGroups;                        // number of channel groups that have individual setup
            bool    bInputTermAvailable;            // input termination available
            bool    bDiffModeAvailable;             // differential mode available
            bool    bStopLevelProgrammable;         // programmable stop level available
            bool    bOutputLevelProgrammable;       // low and high output level is programmable
            } stDIO;
        
        } uCfg;
    } ST_SPCM_CARDINFO;





/*
**************************************************************************
bSpcMInitCardByIdx:

opens the driver with the given indes, reads out card information and
fills the CARDINFO structure
**************************************************************************
*/

bool bSpcMInitCardByIdx (               // returns false if error occured, otherwise true
    ST_SPCM_CARDINFO   *pstCardInfo,    // pointer to an allocated and empty card info structure
    int32               lCardIdx);      // index of card to open, index starts with zero



/*
**************************************************************************
nErrorMessageStdOut:

prints the error message to std out and ends the driver if it's active
program can be left with this function
**************************************************************************
*/

int nSpcMErrorMessageStdOut (                   // returns -1
    ST_SPCM_CARDINFO   *pstCardInfo,            // pointer to a filled card info structure
    char*               pszMessage,             // user error message,
    bool                bPrintCardErr);  		// add card error message



/*
**************************************************************************
pszSpcMTranslateRuntimeError

translation of a runtime error code into a text message. The buffer need 
to be at least ERRORTEXTLEN long to cover any current or future messages
**************************************************************************
*/

char* pszSpcMTranslateRuntimeError (
    uint32 dwErrorCode, 
    char* pszBuffer);


/*
**************************************************************************
vSpcMCloseCard

closes the driver
**************************************************************************
*/

void vSpcMCloseCard (
    ST_SPCM_CARDINFO   *pstCardInfo);           // pointer to a filled card info structure



/*
**************************************************************************
pszSpcMPrintCardInfo

prints the card information to a string for display.
**************************************************************************
*/

char* pszSpcMPrintCardInfo (                    // returns the pointer to the printed string
    ST_SPCM_CARDINFO   *pstCardInfo,            // pointer to a filled card info structure
    char*               pszBuffer,              // buffer for printing
    int32               lStrLen);               // length of the buffer



/*
**************************************************************************
bSpcMSetupModeXXX

setup one of the card modes
**************************************************************************
*/

// record standard mode single
bool bSpcMSetupModeRecStdSingle (               // returns false if error occured, otherwise true
    ST_SPCM_CARDINFO   *pstCardInfo,            // pointer to a filled card info structure
    uint64              qwChEnable,             // channel enable mask for the next acquisition
    int64               llMemSamples,           // recording length in samples per channel
    int64               llPostSamples);         // samples to record after trigger event

// record FIFO mode single
bool bSpcMSetupModeRecFIFOSingle (              // returns false if error occured, otherwise true
    ST_SPCM_CARDINFO   *pstCardInfo,            // pointer to a filled card info structure
    uint64              qwChEnable,             // channel enable mask for the next acquisition
    int64               llPreSamples,           // number of samples to be stored before the trigger event
    int64               llBlockToRec,       	// blocks and loops can define the maximum recording length
    int64               llLoopToRec);       	// in FIFO mode as Block * Loop. If zero we run continuously

// ***********************************************************************


// record standard mode multiple recording
bool bSpcMSetupModeRecStdMulti (                // returns false if error occured, otherwise true
    ST_SPCM_CARDINFO   *pstCardInfo,            // pointer to a filled card info structure
    uint64              qwChEnable,             // channel enable mask for the next acquisition
    int64               llMemSamples,           // recording length in samples per channel
    int64               llSegmentSize,          // size of each multiple recording segment
    int64               llPostSamples);         // samples to record after trigger event for each segment

// record standard mode ABA
bool bSpcMSetupModeRecStdABA (                  // returns false if error occured, otherwise true
    ST_SPCM_CARDINFO   *pstCardInfo,            // pointer to a filled card info structure
    uint64              qwChEnable,             // channel enable mask for the next acquisition
    int64               llMemSamples,           // recording length in samples per channel
    int64               llSegmentSize,          // size of each multiple recording segment
    int64               llPostSamples,          // samples to record after trigger event for each segment
    int32               lABADivider);           // divider for ABA mode slow samples

// record FIFO mode multiple recording
bool bSpcMSetupModeRecFIFOMulti (               // returns false if error occured, otherwise true
    ST_SPCM_CARDINFO   *pstCardInfo,            // pointer to a filled card info structure
    uint64              qwChEnable,             // channel enable mask for the next acquisition
    int64               llSegmentSize,          // size of each multiple recording segment
    int64               llPostSamples,          // samples to record after trigger event for each segment
    int64               llSegmentsToRec);   	// numbe of segments to record in total. If zero we reun continuously

// record FIFO mode ABA
bool bSpcMSetupModeRecFIFOABA (                 // returns false if error occured, otherwise true
    ST_SPCM_CARDINFO   *pstCardInfo,            // pointer to a filled card info structure
    uint64              qwChEnable,             // channel enable mask for the next acquisition
    int64               llSegmentSize,          // size of each multiple recording segment
    int64               llPostSamples,          // samples to record after trigger event for each segment
    int32               lABADivider,            // divider for ABA mode slow samples
    int64               llSegmentsToRec);   	// numbe of segments to record in total. If zero we reun continuously

// recalculates the data start address of segment no. idx
void* pvGetSegmentDataPointer (                 // returns an pointer to the segment start address
    ST_SPCM_CARDINFO   *pstCardInfo,            // pointer to a filled card info structure
    void*               pvDataBuffer,           // pointer to the data array that holds all segments
    int32               lSegmentsize,           // size of one segment
    int32               lSegmentIdx);           // index of the segment of which we wish to get the pointer

// ***********************************************************************

// record standard mode gated sampling
bool bSpcMSetupModeRecStdGate (                 // returns false if error occured, otherwise true
    ST_SPCM_CARDINFO   *pstCardInfo,            // pointer to a filled card info structure
    uint64              qwChEnable,             // channel enable mask for the next acquisition
    int64               llMemSamples,           // recording length in samples per channel
    int64               llPreSamples,           // number of samples to record before gate starts
    int64               llPostSamples);         // number of samples to record after gate ends

// record FIFO mode gated sampling
bool bSpcMSetupModeRecFIFOGate (                // returns false if error occured, otherwise true
    ST_SPCM_CARDINFO   *pstCardInfo,            // pointer to a filled card info structure
    uint64              qwChEnable,             // channel enable mask for the next acquisition
    int64               llPreSamples,           // number of samples to record before gate starts
    int64               llPostSamples,          // number of samples to record after gate ends
    int64               llGatesToRec);      	// number of gates to record


// ***********************************************************************

// replay standard mode single
bool bSpcMSetupModeRepStdSingle (               // returns false if error occured, otherwise true
    ST_SPCM_CARDINFO   *pstCardInfo,            // pointer to a filled card info structure
    uint64              qwChEnable,             // channel enable mask for the next generation
    int64               llMemSamples);          // samples to replay after trigger event

// replay standard mode looped
bool bSpcMSetupModeRepStdLoops (                // returns false if error occured, otherwise true
    ST_SPCM_CARDINFO   *pstCardInfo,            // pointer to a filled card info structure
    uint64              qwChEnable,             // channel enable mask for the next generation
    int64               llMemSamples,           // samples to replay after trigger event
    int64               llLoops);           	// loops to replay (0 --> infinite continuous replay)

// replay standard mode single restart
bool bSpcMSetupModeRepStdSingleRestart (        // returns false if error occured, otherwise true
    ST_SPCM_CARDINFO   *pstCardInfo,            // pointer to a filled card info structure
    uint64              qwChEnable,             // channel enable mask for the next generation
    int64               llMemSamples,           // samples to replay after trigger event
    int64               llLoops );           	// loops to replay (0 --> infinite continuous replay)

// replay FIFO mode single
bool bSpcMSetupModeRepFIFOSingle (              // returns false if error occured, otherwise true
    ST_SPCM_CARDINFO   *pstCardInfo,            // pointer to a filled card info structure
    uint64              qwChEnable,             // channel enable mask for the next acquisition
    int64               llBlockToRep,       	// blocks and loops can define the maximum replay length
    int64               llLoopToRep);       	// in FIFO mode as Block * Loop. If zero we run continuously

// ***********************************************************************

// standard mode multiple replay
bool bSpcMSetupModeRepStdMulti (                // returns false if error occured, otherwise true
    ST_SPCM_CARDINFO   *pstCardInfo,            // pointer to a filled card info structure
    uint64              qwChEnable,             // channel enable mask for the next generation
    int64               llMemSamples,           // replay length in samples per channel
    int64               llSegmentSize,          // size of each segment
    int64               llSegmentsToRep);   	// segments to replay (0 = infinite, 1 = memsize once, N = number of segments)

// FIFO mode multiple replay
bool bSpcMSetupModeRepFIFOMulti (               // returns false if error occured, otherwise true
    ST_SPCM_CARDINFO   *pstCardInfo,            // pointer to a filled card info structure
    uint64              qwChEnable,             // channel enable mask for the next generation
    int64               llSegmentSize,          // size of each segment
    int64               llSegmentsToRep); 		// segments to replay (0 = infinite, N = number of segments)

// ***********************************************************************

// standard mode gated replay 
bool bSpcMSetupModeRepStdGate (                 // returns false if error occured, otherwise true
    ST_SPCM_CARDINFO   *pstCardInfo,            // pointer to a filled card info structure
    uint64              qwChEnable,             // channel enable mask for the next generation
    int64               llMemSamples,           // recording length in samples per channel
    int64               llGatesToRep);     		// gates to replay (0 = infinte, 1 = memsize once, N = number of gates)

// FIFO mode gated replay 
bool bSpcMSetupModeRepFIFOGate (                // returns false if error occured, otherwise true
    ST_SPCM_CARDINFO   *pstCardInfo,            // pointer to a filled card info structure
    uint64              qwChEnable,             // channel enable mask for the next generation
    int64               llGatesToRep);      	// gates to replay (0 = infinte, N = number of gates)

// ***********************************************************************



/*
**************************************************************************
bSpcMSetupClockXXX

setup the clock engine for different modes
**************************************************************************
*/

// internal clock using PLL
bool bSpcMSetupClockPLL (                       // returns false if error occured, otherwise true
    ST_SPCM_CARDINFO   *pstCardInfo,            // pointer to a filled card info structure
    int64               llSamplerate,           // desired sampling rate 
    bool                bClockOut); 		    // clock output enable

// internal clock using high precision quartz
bool bSpcMSetupClockQuartz (                    // returns false if error occured, otherwise true
    ST_SPCM_CARDINFO   *pstCardInfo,            // pointer to a filled card info structure
    int64               llSamplerate,           // sampling rate if internal clock mode
    bool                bClockOut);     		// clock output enable

// external clock 
bool bSpcMSetupClockExternal (                  // returns false if error occured, otherwise true
    ST_SPCM_CARDINFO   *pstCardInfo,            // pointer to a filled card info structure
    int32               lExtRange,              // external clock range if external clock mode is used
    bool                bClockTerm,      		// enable clock termination (50 ohm)
    int32               lDivider);          	// clock divider

// reference clock
bool bSpcMSetupClockRefClock (                  // returns false if error occured, otherwise true
    ST_SPCM_CARDINFO   *pstCardInfo,            // pointer to a filled card info structure
    int32               lRefClock,              // reference clock speed if using ref clock mode
    int64               llSamplerate,           // desired sampling rate
    bool                bClockTerm);		    // enable clock termination (50 ohm)



/*
**************************************************************************
bSpcMSetupTriggerXXX

setup the trigger engine for different modes
**************************************************************************
*/

// software trigger
bool bSpcMSetupTrigSoftware (                   // returns false if error occured, otherwise true
    ST_SPCM_CARDINFO   *pstCardInfo,            // pointer to a filled card info structure
    bool                bTrigOut);      		// enable trigger output

// external trigger
bool bSpcMSetupTrigExternal (                   // returns false if error occured, otherwise true
    ST_SPCM_CARDINFO   *pstCardInfo,            // pointer to a filled card info structure
    int32               lExtMode,               // external trigger mode
    bool                bTrigTerm,       		// trigger termination active
    int32               lPulsewidth,        	// programmable pulsewidth for all external + pulsewidth modes
    bool                bSingleSrc,      		// acts as single trigger source, all other masks cleared
    int32               lExtLine);          	// standard external trigger is line 0

// additional BaseXIO trigger
bool bSpcMSetupTrigXIO (                        // returns false if error occured, otherwise true
    ST_SPCM_CARDINFO   *pstCardInfo,            // pointer to a filled card info structure
    int32               lXIOMode,               // external trigger mode
    bool                bSingleSrc,			    // acts as single trigger source, all other masks cleared
    int32               lXIOLine);          	// standard XIO trigger is line 0

// channel trigger is set for each channel separately
bool bSpcMSetupTrigChannel (                    // returns false if error occured, otherwise true
    ST_SPCM_CARDINFO   *pstCardInfo,            // pointer to a filled card info structure
    int32               lChannel,               // channel to modify
    int32               lTrigMode,              // channel trigger mode
    int32               lTrigLevel0,            // level 0
    int32               lTrigLevel1,        	// level 1
    int32               lPulsewidth ,        	// programmable pulsewidth for channel
    bool                bTrigOut,       		// trigger output
    bool                bSingleSrc);     		// acts as single trigger source, all other masks cleared

// this function sets the trigger masks (bSingleSrc of other commands must be false to use this)
bool bSpcMSetupTrigMask (                       // returns false if error occured, otherwise true
    ST_SPCM_CARDINFO   *pstCardInfo,            // pointer to a filled card info structure
    uint32              dwChannelOrMask0,       // or mask of all available channels
    uint32              dwChannelOrMask1,   	// ...
    uint32              dwChannelAndMask0,  	// and mask of all available channels
    uint32              dwChannelAndMask1,  	// ...
    uint32              dwTrigOrMask,       	// trigger or mask (software external, basexio)
    uint32              dwTrigAndMask);     	// trigger and mask (software external, basexio)



/*
**************************************************************************
bSpcMSetupInputChannel

allows all input channel related settings. if one of the setup like
termination or differential inputs is not available on the card the
setting is simply ignored
**************************************************************************
*/

bool bSpcMSetupInputChannel (                   // returns false if error occured, otherwise true
    ST_SPCM_CARDINFO   *pstCardInfo,            // pointer to a filled card info structure
    int32               lChannel,               // channel to change
    int32               lInputRange,            // input range in mV = (max-min)/2, =1000 for +/-1V range
    bool                bTerm,           		// set input termination (50 ohm) if available
    int32               lInputOffset,       	// programmable input offset as listed in the manual
    bool                bDiffInput);    		// set differential input if available



/*
**************************************************************************
bSpcMSetupAnalogOutputChannel

allows all analog output channel related settings. if one of the setup like
DoubleOut is not available on the card the setting is simply ignored
**************************************************************************
*/

bool bSpcMSetupAnalogOutputChannel (                    // returns false if error occured, otherwise true
    ST_SPCM_CARDINFO    *pstCardInfo,                   // pointer to a filled card info structure
    int32               lChannel,                       // channel to change
    int32               lAmplitude,                     // output amplitude in mV = (max-min)/2, =1000 for +/-1V range
    int32               lOutputOffset,              	// programmable output offset as listed in the manual
    int32               lFilter,                    	// programmable output filter as listed in the manual
    int32               lStopMode,  					// defines the behavior after replay or while replay is pausing         
    bool                bDoubleOut,             		// enables identical    output on two channels of one module (if available)
    bool                bDifferential);         		// enables differential output on two channels of one module (if available)



/*
**************************************************************************
bSpcMSetupDigitalXXXModul

allows all input and output channel related settings for one group of 
channels. If one of the setups like the programmable output levels is not
available this setup is simply ignored
**************************************************************************
*/

bool bSpcMSetupDigitalOutput (   			            // returns false if error occured, otherwise true
	ST_SPCM_CARDINFO	*pstCardInfo,			        // pointer to a filled card info structure
	int32				lGroup,					        // module/group of channels to change
	int32				lStopMode,   					// defines the behavior after replay or while replay is pausing
    int32               lLowLevel,                  	// low level in mV if output is programmable
    int32               lHighLevel,              		// high level in mV if output levels are programmable
    bool                bDiffMode);             		// hardware differential mode if available

bool bSpcMSetupDigitalInput (   			            // returns false if error occured, otherwise true
	ST_SPCM_CARDINFO	*pstCardInfo,			        // pointer to a filled card info structure
	int32				lGroup,					        // module/group of channels to change
    bool                bTerm);                  		// input termination



/*
**************************************************************************
bSpcMSetupTimestamp

set up the timestamp mode and performs a synchronisation with refernce
clock if that mode is activated
**************************************************************************
*/

bool bSpcMSetupTimestamp (                      // returns false if error occured, otherwise true
    ST_SPCM_CARDINFO   *pstCardInfo,            // pointer to a filled card info structure
    int32               lMode,                  // mode for timestamp
    uint32              dwRefTimeoutMS);        // timeout in milli seconds for synchronisation with reference clock

#endif
