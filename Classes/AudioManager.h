#include "cocos2d.h"
#include "audio/include/AudioEngine.h"
#ifndef __AUDIO_MANAGER_H__
#define __AUDIO_MANAGER_H__

class AudioManager {
private:
    AudioManager(); // Constructor ẩn để ngăn chặn việc tạo đối tượng từ bên ngoài
    ~AudioManager(); // Destructor


public:

    static AudioManager* getInstance();
    static void destroyInstance();


    // Phát âm thanh
    static void playAudio(const std::string& audioFilePath, bool loop = false, float volume = 0.0f);

	
};
#endif





































