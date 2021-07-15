#pragma once

#include <map>
#include <Windows.h>

//#include <audiere.h>
//#pragma comment(lib,"audiere.lib")

//using namespace audiere;

#include "../NVarray.h"

class CSoundManager
{
private:

	std::map<int, char*> sounds;
	VArray2	<char*, char*> BuffersBusy;

	//AudioDevicePtr device;
	//OutputStreamPtr firstBuff;
	//OutputStreamPtr seccondBuff;

public:

	CSoundManager();
	~CSoundManager();
	
	void Update();
	void PlayCustomBuffSound(char* sound, float volume, bool repeat, float pan, float pitchShift);
	void StopCustomBuffSound(char* sound);

	void PlayBuffSound(int sound, float volume, bool repeat, float pan, float pitchShift);
	void StopBuffSound(int sound);
};