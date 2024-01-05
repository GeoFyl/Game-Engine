#include "pch.h"
#include "AudioSystem_SoLoud.h"
#include "SystemsLocator.h"
#include <soloud_wav.h>

using namespace Toffee::Internal;

AudioSystem_SoLoud* audio_soloud = new AudioSystem_SoLoud;

AudioSystem_SoLoud::AudioSystem_SoLoud() {
	ProvideSystem(this);
	audio_soloud = nullptr;
}

// Init soLoud
int AudioSystem_SoLoud::Initialise() {
	int result = soLoud_.init();
	return result;
}

// Free memory
int AudioSystem_SoLoud::Shutdown() {
	soLoud_.deinit();
	return 0;
}

// Retreive an audio source and start playing it
void AudioSystem_SoLoud::Play(std::string name) {
	SoLoud::Wav* source = reinterpret_cast<SoLoud::Wav*>(ToffeeAPI::Resources()->GetResource(name));
	if(source) active_sounds_[name] = soLoud_.play(*source); // Added to map of currently playing sounds
}

// Stop a sound
void AudioSystem_SoLoud::Stop(std::string name) {
	// If sound is currently playing, stop it
	if(active_sounds_.count(name)) soLoud_.stop(active_sounds_[name]);
}

// Stop all sounds 
void Toffee::Internal::AudioSystem_SoLoud::StopAll() {
	soLoud_.stopAll();
}
