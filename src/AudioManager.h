#pragma once
#include <SDL3/SDL.h>
#include <string>
#include <iostream>
#include <cstdio>

/*
From UML diagram:
Class Audio Manager:
	public:
		audioFormat : enum
		channels: int
		freq: int
	manages the device / playing stuff
	whenever you load a new sound it creates an audio stream and hands it to scripter
	needs to yield an audio stream object

Class AudioStream
	public:
		DestroyAudioStream(in stream: *AudioStream) : bool
		PlayAudio(): bool
		PauseAudio(): bool
		SetVolume(in volume : float): bool	(SetAudioStreamGain)

example of programming:		
	Soundfile MainVillageTheme("filepath/path/path/song.wav")  //creates a sound object and store it in a vector?
	PlayAudio(MainVillageTheme, 1.0 to 100.00, looping bool) call SetVolume()?, LoadSound()?
	PauseAudio(MainVillageTheme)
*/

class AudioStream
{
private:
	Uint8* wav_data;
	Uint32 wav_data_len;
	SDL_AudioStream* stream;
	SDL_AudioDeviceID* audio_device_id;
	SDL_AudioSpec spec;

public:
	AudioStream(SDL_AudioDeviceID* audio_device_id) //constructor 
	{
		this->audio_device_id = audio_device_id;
	}

	bool LoadSound(std::string filepath)
	{
		char* wav_path = NULL;
		
		SDL_asprintf(&wav_path, "%s%s", SDL_GetBasePath(), filepath.c_str());
		if (!SDL_LoadWAV(wav_path, &spec, &this->wav_data, &this->wav_data_len))
		{
			std::cout << "\nCouldn't load .wav file: %s";
			return false;
		}

		char buffer[1000];
		this->stream = SDL_CreateAudioStream(&spec, NULL);
		if (!this->stream) {
			std::cout << "\nCouldn't create audio stream: %s";
		}
		else if (!SDL_BindAudioStream(*this->audio_device_id, this->stream)) {  /* once bound, it'll start playing when there is data available! */
			sprintf_s(buffer, "\nFailed to bind stream to device: %s", SDL_GetError());
		}
	}

	void PlaySound()
	{
		if (SDL_GetAudioStreamQueued(this->stream) < (this->wav_data_len)) {
			SDL_PutAudioStreamData(this->stream, this->wav_data, this->wav_data_len);
		}
	}
};

class AudioManager
{
	/*
		Creates AudioStream Objects to be managed by the game window.
		Destroying them is also going to be the responsibility of the game window programmer.

		Wherever the scripter stores the sound, you will need to delete the sounds using delete keyword.
	*/
	private:
		SDL_AudioDeviceID audio_device;

	public:
		AudioManager() //constructor
		{
			/*
			* Sets audio_device ID to whatever default is
			*/
			audio_device = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
		}

		AudioStream* CreateAudioStream(std::string filepath)
		{
			AudioStream* audio_stream_object;
			audio_stream_object =  new AudioStream(&this->audio_device);
			audio_stream_object->LoadSound(filepath);
			return audio_stream_object;
		}
};
