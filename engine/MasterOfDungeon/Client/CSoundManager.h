#pragma once

#include <dsound.h>
//#include <dmusici.h>

#pragma comment(lib, "dsound.lib")

class CSoundManager
{
private:

public:

	CSoundManager();
	~CSoundManager();

	void LoadSound(void* SoundData);
	void PlayBuffSound();
	void StopBuffSound();
};