#include "AudioPlayer.hpp"
#include <SDL.h>
#include "Debug.hpp"
#include "World.hpp"
#include "AudioSource.hpp"

using namespace RavEngine;
using namespace std;

static WeakRef<World> worldToRender;

/**
 The audio player tick function. Called every time there is an audio update
 @param udata user data for application
 @param stream buffer to write the data into
 @param len the length of the buffer
 */
static void AudioPlayer_Tick(void *udata, Uint8 *stream, int len){
	std::memset(stream,0,len);		//fill with silence
	Ref<World> world = worldToRender.lock();
	if (world){
		auto sources = world->GetAllComponentsOfTypeFastPath<AudioSourceComponent>();
		
		float shared_buffer[len];
		
		for(const auto& s : sources){
			Ref<AudioSourceComponent> source = static_pointer_cast<AudioSourceComponent>(s);
			std::memset(shared_buffer, 0, len);
			source->GetSampleRegionAndAdvance(shared_buffer, len);
			memcpy(stream, shared_buffer, len);
//			for(int i = 0; i < len; i++){
//				stream[i] += shared_buffer[i];
//			}
		}
		
		//TODO: get appropriate area in source's buffer if it is playing
		//TODO: update buffer in all Rooms (silence if not currently playing)
		//TODO: render all Rooms
		//TODO: mix output buffers of all rooms
		//TODO: update stream pointer with rendered output
	}
}

void AudioPlayer::Init(){
	SDL_InitSubSystem(SDL_INIT_AUDIO);
	
	SDL_AudioSpec want, have;
	
	std::memset(&want, 0, sizeof(want));
	want.freq = 22050;
	want.format = AUDIO_F32;
	want.channels = 1;
	want.samples = 4096;
	want.callback = AudioPlayer_Tick;
	
	device = SDL_OpenAudioDevice(NULL, 0, &want, &have, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
	
	if (device == 0){
		Debug::Fatal("could not open audio device: {}",SDL_GetError());
	}
	else{
		if (have.format != want.format){
			Debug::Fatal("Could not get Float32 audio format");
		}
	}
	Debug::LogTemp("Audio Subsystem initialized");
	SDL_PauseAudioDevice(device,0);	//begin audio playback
}

void AudioPlayer::Shutdown(){
	SDL_CloseAudioDevice(device);
}

void AudioPlayer::SetWorld(Ref<World> w){
	worldToRender = w;
}