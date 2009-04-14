#pragma once

#include "core-Sound\Sound.h"
#include <windows.h>
#include <stdio.h>
#include <string>


///////////////////////////////////////////////////////////////////////////////

#define MAX_NUM_WAVEID			1024

///////////////////////////////////////////////////////////////////////////////

enum WAVEFILETYPE
{
	WF_EX  = 1,
	WF_EXT = 2
};

///////////////////////////////////////////////////////////////////////////////

#ifndef _WAVEFORMATEX_
#define _WAVEFORMATEX_

struct WAVEFORMATEX
{
    WORD    wFormatTag;
    WORD    nChannels;
    DWORD   nSamplesPerSec;
    DWORD   nAvgBytesPerSec;
    WORD    nBlockAlign;
    WORD    wBitsPerSample;
    WORD    cbSize;
};

#endif 

///////////////////////////////////////////////////////////////////////////////

#ifndef _WAVEFORMATEXTENSIBLE_
#define _WAVEFORMATEXTENSIBLE_

struct WAVEFORMATEXTENSIBLE
{
    WAVEFORMATEX    Format;
    union {
        WORD wValidBitsPerSample;       /* bits of precision  */
        WORD wSamplesPerBlock;          /* valid if wBitsPerSample==0 */
        WORD wReserved;                 /* If neither applies, set to zero. */
    } Samples;
    DWORD           dwChannelMask;      /* which channels are */
                                        /* present in stream  */
    GUID            SubFormat;
};

#endif

///////////////////////////////////////////////////////////////////////////////

class WavFile : public Sound  
{
private:
   WAVEFILETYPE	m_type;
	WAVEFORMATEXTENSIBLE m_ext;		// For non-WAVEFORMATEXTENSIBLE wavefiles, the header is stored in the Format member of wfEXT
	char*          m_data;
	DWORD          m_dataSize;
	FILE*          m_file;
	DWORD	         m_dataOffset;
   DWORD	         m_currentOffset;

   std::string m_format;

public:
   WavFile(const std::string& fileName);
	~WavFile();

   DWORD getDataOffset() const;
   void setDataOffset(DWORD pos);

   DWORD getData(char* data, DWORD bufSize);
   std::string getFormat() const;
   unsigned int getFrequency() const;

   DWORD getBytesPerSec() const;
   DWORD getBlockAlignment() const;

private:
	void parseFile(const std::string& fileName);
   std::string recognizeFormat();
};

///////////////////////////////////////////////////////////////////////////////
