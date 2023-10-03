#include "pch.h"
#include "AudioSystem_SoLoud.h"
#include "soloud_wav.h"

#ifdef AUDIOSYSTEM_SOLOUD
	Grengine::AudioSystem_SoLoud grAudioSystemSoLoud;
	Grengine::AudioSystem* Grengine::grAudioSystem = &grAudioSystemSoLoud;
#endif

int Grengine::AudioSystem_SoLoud::Initialise() {
	soLoud.init();
}

int Grengine::AudioSystem_SoLoud::Shutdown() {
	soLoud.deinit();
}

void Grengine::AudioSystem_SoLoud::play(std::string name) {
	soLoud.play(audioSources[name]);
}

void Grengine::AudioSystem_SoLoud::loadSound(std::string name, std::string filename) {
	SoLoud::Wav source;
	source.load(filename.c_str());
	audioSources[name] = source;
}