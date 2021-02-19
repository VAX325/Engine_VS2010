#include "CSoundManager.h"
#include "ClientPerems.h"

#include <Base_include.h>

CSoundManager::CSoundManager()
{
	//device = OpenDevice();
	Files fls = GetFileSystemObjCl()->GetAllFilesInFolder((char*)"../gamedata/sounds/", (char*)"wav");

	auto it = fls.begin();

	int i = 0;

	while (it != fls.end())
	{
		sounds.insert(std::make_pair(i, it->second));
		++it;
		++i;
	}

	//firstBuff = OutputStreamPtr();
	//seccondBuff = OutputStreamPtr();
}

CSoundManager::~CSoundManager()
{
	//device->clearCallbacks();
}

void CSoundManager::PlayCustomBuffSound(char* sound, float volume, bool repeat, float pan, float pitchShift)
{
	/*if (firstBuff.get() == NULL || !firstBuff->isPlaying())
	{
		firstBuff = OpenSound(device, sound, true);
		firstBuff->setVolume(volume);
		firstBuff->setRepeat(repeat);
		firstBuff->setPan(pan);
		firstBuff->setPitchShift(pitchShift);
		firstBuff->play();
		BuffersBusy.first = sound;
		return;
	}
	else if (seccondBuff.get() == NULL || !seccondBuff->isPlaying())
	{
		seccondBuff = OpenSound(device, sound, true);
		seccondBuff->setVolume(volume);
		seccondBuff->setRepeat(repeat);
		seccondBuff->setPan(pan);
		seccondBuff->setPitchShift(pitchShift);
		seccondBuff->play();
		BuffersBusy.second = sound;
		return;
	}*/
}

void CSoundManager::Update()
{
	/*if (firstBuff != NULL)
	{
		if (!firstBuff->isPlaying())
		{
			BuffersBusy.first = NULL;
		}
	}

	if (seccondBuff != NULL)
	{
		if (!seccondBuff->isPlaying())
		{
			BuffersBusy.first = NULL;
		}
	}*/
}

void CSoundManager::StopCustomBuffSound(char* sound)
{
	/*if (strcmp(BuffersBusy.first, sound) == 0)
	{
		firstBuff->stop();
	}
	else if(strcmp(BuffersBusy.second, sound) == 0)
	{
		seccondBuff->stop();
	}*/
}

void CSoundManager::PlayBuffSound(int sound, float volume, bool repeat, float pan, float pitchShift)
{
	/*if(firstBuff.get() == NULL || !firstBuff->isPlaying())
	{
		char* path = (char*)malloc(512);

		strcpy(path, "../gamedata/sounds/");

		strcat(path, sounds[sound]);

		firstBuff = OpenSound(device, path, true);
		firstBuff->setVolume(volume);
		firstBuff->setRepeat(repeat);
		firstBuff->setPan(pan);
		firstBuff->setPitchShift(pitchShift);
		firstBuff->play();
		BuffersBusy.first = sounds[sound];
		return;
	}
	else if (seccondBuff.get() == NULL || !seccondBuff->isPlaying())
	{
		seccondBuff = OpenSound(device, sounds[sound], true);
		seccondBuff->setVolume(volume);
		seccondBuff->setRepeat(repeat);
		seccondBuff->setPan(pan);
		seccondBuff->setPitchShift(pitchShift);
		seccondBuff->play();
		BuffersBusy.second = sounds[sound];
		return;
	}*/
}

void CSoundManager::StopBuffSound(int sound)
{
	/*if (strcmp(BuffersBusy.first, sounds[sound]) == 0)
	{
		firstBuff->stop();
	}
	else if (strcmp(BuffersBusy.second, sounds[sound]) == 0)
	{
		seccondBuff->stop();
	}*/
}