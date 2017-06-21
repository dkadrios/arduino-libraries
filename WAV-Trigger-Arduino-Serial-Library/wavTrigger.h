// **************************************************************
//     Filename: wavTrigger.h
// Date Created: 2/23/2014
//
//     Comments: Robertsonics WAV Trigger serial control library
//
// Programmers: Jamie Robertson, jamie@robertsonics.com
//
// **************************************************************
//
// Revision History
//
// Date      Description
// --------  -----------
//
// 02/22/14  First version created.
//           LIMITATIONS: Hard-coded for AltSoftwareSerial Library.
//           Also only supports commands TO the WAV Trigger. Will
//           fix these things.
//
// 05/10/14  Tested with UNO. Added new functions for fades, cross-
//           fades and starting multiple tracks in sample sync.
//
// 04/26/15  Added support for sample-rate / pitch bend control,
//           and compile macro switches for hardware serial ports.
//

#ifndef WAVTRIGGER_H
#define WAVTRIGGER_H

// ==================================================================
// The following defines are used to control which serial class is
//  used. Uncomment only the one you wish to use. If all of them are
//  commented out, the library will use Hardware Serial
#define __WT_USE_ALTSOFTSERIAL__
// #define __WT_USE_SERIAL1__
//#define __WT_USE_SERIAL2__
//#define __WT_USE_SERIAL3__
// ==================================================================

#define CMD_GET_VERSION					1
#define CMD_GET_SYS_INFO				2
#define CMD_TRACK_CONTROL				3
#define CMD_STOP_ALL						4
#define CMD_MASTER_VOLUME				5
#define CMD_GET_STATUS					7
#define CMD_TRACK_VOLUME				8
#define CMD_TRACK_FADE					10
#define CMD_RESUME_ALL_SYNC			11
#define CMD_SAMPLERATE_OFFSET		12

#define RSP_VERSION_STRING			0x81
#define RSP_SYS_INFO						0x82
#define RSP_STATUS							0x83

#define TRK_PLAY_SOLO						0
#define TRK_PLAY_POLY						1
#define TRK_PAUSE								2
#define TRK_RESUME							3
#define TRK_STOP								4
#define TRK_LOOP_ON							5
#define TRK_LOOP_OFF						6
#define TRK_LOAD								7

#define HEAD_1									0xf0
#define HEAD_2									0xaa
#define EOM											0x55

#ifdef __WT_USE_ALTSOFTSERIAL__
	#include "../AltSoftSerial/AltSoftSerial.h"
#else
	#include <HardwareSerial.h>
	#ifdef __WT_USE_SERIAL1__
		#define WTSerial Serial1
		#define __WT_SERIAL_ASSIGNED__
	#endif
	#ifdef __WT_USE_SERIAL2__
		#define WTSerial Serial2
		#define __WT_SERIAL_ASSIGNED__
	#endif
	#ifdef __WT_USE_SERIAL3__
		#define WTSerial Serial3
		#define __WT_SERIAL_ASSIGNED__
	#endif
	#ifndef __WT_SERIAL_ASSIGNED__
		#define WTSerial Serial
	#endif
#endif

class wavTrigger
{
public:
	wavTrigger() {;}
	~wavTrigger() {;}

	void start(void);
	void masterGain(int gain);
	void stopAllTracks(void);
	void resumeAllInSync(void);
	void trackPlaySolo(int trk);
	void trackPlayPoly(int trk);
	void trackLoad(int trk);
	void trackStop(int trk);
	void trackPause(int trk);
	void trackResume(int trk);
	void trackLoop(int trk, bool enable);
	void trackGain(int trk, int gain);
	void trackFade(int trk, int gain, int time, bool stopFlag);
	void trackCrossFade(int trkFrom, int trkTo, int gain, int time);
	void samplerateOffset(int offset);

	void getVersion(void);
	void getSysInfo(void);
	void getStatus(void);

	uint8_t* returnSysVersion(void);
	uint8_t returnSysinfoVoices(void);
	uint16_t returnSysinfoTracks(void);
	uint8_t returnTracksPlayingCount(void);
	uint16_t* returnTracksPlaying(void);

private:
	void trackControl(int trk, int code);

	void response(uint8_t responseCommand);
	void readResponse(unsigned long wait);
	void parseResponse();

	#ifdef __WT_USE_ALTSOFTSERIAL__
		AltSoftSerial WTSerial;
	#endif

	uint8_t packet[40];

	uint8_t sysVersion[20];
	uint8_t sysinfoVoices;
	uint16_t sysinfoTracks;
	uint8_t tracksPlayingCount;
	uint16_t tracksPlaying[14];

};

#endif
