#include "AudioManager.h"
static AudioManager* audio_instance = nullptr;

AudioManager* AudioManager::getInstance() {
    if (!audio_instance)
    {
        audio_instance = new AudioManager();
    }
    return audio_instance;
}
void AudioManager::destroyInstance() {
    CC_SAFE_DELETE(audio_instance);
}
AudioManager::AudioManager() {
    // Constructor
}
AudioManager::~AudioManager() {
    //Destructor
}

void AudioManager::playAudio(const std::string& audioFilePath, bool loop, float volume) {

        int audioId = cocos2d::AudioEngine::play2d(audioFilePath, loop,volume);
        // Bạn có thể lưu lại audioId để sử dụng trong các trường hợp khác
    
}

