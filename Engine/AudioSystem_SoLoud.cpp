#include "pch.h"
#include "AudioSystem_SoLoud.h"
#include "soloud_wav.h"

using namespace Engine::Internal;

AudioSystem_SoLoud* audio_soloud = new AudioSystem_SoLoud;

AudioSystem_SoLoud::AudioSystem_SoLoud() {
	ProvideSystem(this);
	audio_soloud = nullptr;
}

int AudioSystem_SoLoud::Initialise() {
	int result = soLoud.init();
	std::cout << "\nresult:" << result;
	return result;
}

int AudioSystem_SoLoud::Shutdown() {
	soLoud.deinit();
	return 0;
}

void AudioSystem_SoLoud::Play(std::string name) {
	soLoud.play(*audioSources[name]);
}

void AudioSystem_SoLoud::LoadSound(std::string name, std::string filename) {
	SoLoud::Wav* source = new SoLoud::Wav;
	std::string path = "../Game/" + filename;
	source->load(path.c_str());
	audioSources[name] = source;
}