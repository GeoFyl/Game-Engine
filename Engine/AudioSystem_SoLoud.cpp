#include "pch.h"
#include "AudioSystem_SoLoud.h"
#include "SystemsLocator.h"
#include <soloud_wav.h>

using namespace Engine::Internal;

AudioSystem_SoLoud* audio_soloud = new AudioSystem_SoLoud;

AudioSystem_SoLoud::AudioSystem_SoLoud() {
	ProvideSystem(this);
	audio_soloud = nullptr;
}

int AudioSystem_SoLoud::Initialise() {
	int result = soLoud_.init();
	return result;
}

int AudioSystem_SoLoud::Shutdown() {
	soLoud_.deinit();
	return 0;
}

void AudioSystem_SoLoud::Play(std::string name) {
	SoLoud::Wav* source = reinterpret_cast<SoLoud::Wav*>(SystemsAPI::Resources()->GetResource(name));
	if(source) soLoud_.play(*source);
}

//void AudioSystem_SoLoud::LoadSound(std::string name, std::string filename) {
//	SoLoud::Wav* source = new SoLoud::Wav;
//	std::string path = "../Game/" + filename;
//	source->load(path.c_str());
//	audioSources_[name] = source;
//}