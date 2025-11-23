#pragma once
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <string>
#include <iostream>
#include <cstdio>
#include <map>


class AudioStream
{
private:
	Uint8* wav_data;
	Uint32 wav_data_len;
	SDL_AudioStream* stream;
	SDL_AudioDeviceID* audio_device_id;
	SDL_AudioSpec spec;

public:
	bool isLooping = false;

	AudioStream(SDL_AudioDeviceID* audio_device_id) //constructor 
	{
		this->audio_device_id = audio_device_id;
	}

	bool LoadSound(std::string filepath)
	{
		/*
		Loads the audio file then it creates the audio file object
		*/

		char* wav_path = NULL;
		
		SDL_asprintf(&wav_path, "%s%s", SDL_GetBasePath(), filepath.c_str());
		if (!SDL_LoadWAV(wav_path, &spec, &this->wav_data, &this->wav_data_len))
		{
			std::cout << "\nCouldn't load .wav file: %s";
			return false;
		}

		/*
		Creates audio stream and hooks the input / output together
		*/
		char buffer[1000];
		this->stream = SDL_CreateAudioStream(&spec, NULL);
		if (!this->stream) {
			std::cout << "\nCouldn't create audio stream: %s" << std::endl;
		}
		else if (!SDL_BindAudioStream(*this->audio_device_id, this->stream)) {  /* once bound, it'll start playing when there is data available! */
			sprintf_s(buffer, "\nFailed to bind stream to device: %s", SDL_GetError());
		}

		return true;
	}

	void PlaySound()
	{
		/*
		Queues up the sound into the audio stream, and if the queue is empty loads the data into the queue and plays it
		*/
		bool test_bool;
		char buffer[1000];
		//if the song is meant to loop, this loads it into the buffer multiple times
		if (isLooping == true)
		{
			if (SDL_GetAudioStreamQueued(this->stream) < (this->wav_data_len))
			{
				test_bool = SDL_PutAudioStreamData(this->stream, this->wav_data, this->wav_data_len);
				if (test_bool == 0)
				{
					sprintf_s(buffer, "\nFailed to play audio to device in true case: %s", SDL_GetError());
					std::cout << buffer << std::endl;
				}
			}
		}
		//if song is not meant to loop, queues it up once to be played
		else
		{
			if (SDL_GetAudioStreamAvailable(this->stream) == 0)
			{
				test_bool = SDL_PutAudioStreamData(this->stream, this->wav_data, this->wav_data_len);
				if (test_bool == 0)
				{
					sprintf_s(buffer, "\nFailed to play audio to device in false case: %s", SDL_GetError());
					std::cout << buffer << std::endl;
				}
			}
		}
	}

	void SetVolume(float volume)
	{
		/*
		Takes in a float between 0(silence) and 1(loudest) and set's the volume.
		*/
		SDL_SetAudioStreamGain(this->stream, volume);
	}

	void DestroyAudioStream()
	{
		/*
		destroys the audio stream and releases all allocated data
		*/
		SDL_DestroyAudioStream(this->stream);
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
		std::map<std::string, AudioStream*> loopingSounds; //maybe use a map if we need to swap songs on the fly, can remove a song from the map and then add another
		std::map<std::string, AudioStream*> nonLoopingSounds; //used for sounds that don't loop

	public:
		AudioManager() //constructor
		{
			/*
			* Sets audio_device ID to whatever default is
			*/
			char buffer[1000];
			audio_device = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
			if (audio_device == 0)
			{
				sprintf_s(buffer, "\nFailed to audio device: %s", SDL_GetError());
				std::cout << buffer << std::endl;
			}
		}

		~AudioManager() //destructor
		/*
		* Deletes all the stored songs and sounds subobjects
		* Once all the subobjects are removed, it should delete itself
		*/
		{
			this->DeleteAllSongs();
    		this->DeleteAllSounds();
		}

		AudioStream* CreateAudioStream(std::string filepath)
		{
			/*
			instantiates the audio stream, binds it into the audio device, returns the object to be used
			*/
			AudioStream* audio_stream_object;
			audio_stream_object =  new AudioStream(&this->audio_device);
			audio_stream_object->LoadSound(filepath);
			return audio_stream_object;
		}

		void AddSong(std::string filename)
		{
			/*
			Adds a song to the vector for continuous playback, to be called on every frame
			*/
			loopingSounds.insert({filename, CreateAudioStream(filename)});
			loopingSounds[filename]->isLooping = true;
		}

		void AddSound(std::string filename)
		{
			/*
			Adds a song to the vector for continuous playback, to be called on every frame
			*/
			nonLoopingSounds.insert({ filename, CreateAudioStream(filename)});
			nonLoopingSounds[filename]->isLooping = false;
		}

		void PlaySound(std::string filename)
		{
			/*
			Play a Sound effect,etc that is only meant to be played once per keypress
			*/
			if (nonLoopingSounds.find(filename) != nonLoopingSounds.end())
			{
				nonLoopingSounds[filename]->PlaySound();
			}
			else
			{
				std::cout << "Error playing sound. Check filepath, file name, or that it was added." << std::endl;
			}
		}

		void PlayAllSongs()
		{
			/*
			This is for audio that plays continuously
			*/
			for (auto i = loopingSounds.begin(); i != loopingSounds.end(); i++)
			{
				loopingSounds[i->first]->PlaySound();
			}
		}

		void DeleteAllSongs()
		{
			//iterates through the map and deletes the references, then clears all the elements from the map
			for (auto i = loopingSounds.begin(); i != loopingSounds.end(); i++)
			{

				loopingSounds[i->first]->DestroyAudioStream();
				delete loopingSounds[i->first];
			}
			loopingSounds.clear();
		}

		void DeleteAllSounds()
		{
			//iterates through the map and deletes the references, then clears all the elements from the map
			for (auto i = loopingSounds.begin(); i != loopingSounds.end(); i++)
			{

				nonLoopingSounds[i->first]->DestroyAudioStream();
				delete nonLoopingSounds[i->first];
			}
			nonLoopingSounds.clear();
		}

		void DeleteASong(std::string filename)
		{
			/*
			used to delete an item from the loopingSound map
			*/
			if (loopingSounds.find(filename) != loopingSounds.end())
			{
				loopingSounds[filename]->DestroyAudioStream();
				delete loopingSounds[filename];
				loopingSounds.erase(filename);
			}
		}

		void DeleteASound(std::string filename)
		{
			/*
			used to delete an item from the nonLoopingSound map
			*/
			if(nonLoopingSounds.find(filename) != nonLoopingSounds.end())
			{
				nonLoopingSounds[filename]->DestroyAudioStream();
				delete nonLoopingSounds[filename];
				nonLoopingSounds.erase(filename);
			}
		}

		void SetSongVolume(std::string filename, float volume)
		{
			/*
			Sets the volume for a song, that is meant to loop
			only accepts float values from 0.0 to 1.0
			*/
			if (loopingSounds.find(filename) != loopingSounds.end())
			{
				loopingSounds[filename]->SetVolume(volume);
			}
		}

		void SetSoundVolume(std::string filename, float volume)
		{
			/*
			Sets the volume for a sound, that is not meant to loop
			only accepts float values from 0.0 to 1.0
			*/
			if (nonLoopingSounds.find(filename) != nonLoopingSounds.end())
			{
				nonLoopingSounds[filename]->SetVolume(volume);
			}
		}

		void PauseAllAudio()
		{
			/*
			Pauses audio device, stopping all songs and sounds from playing
			*/
			SDL_PauseAudioDevice(this->audio_device);
		}

		void ResumeAllAudio()
		{
			/*
			Resumes audio device, letting all songs and sounds play again.
			Will play what was queued while paused.
			*/
			SDL_ResumeAudioDevice(this->audio_device);
		}
};
