#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

/**
 * @file SoundManager.h
 * @brief Simple sound manager (placeholder for future sound files)
 */

class SoundManager {
public:
    SoundManager() : volume(50.f), muted(false) {}
    
    void playEat() {}
    void playPowerUp() {}
    void playCollision() {}
    void playGameOver() {}
    void playPowerUpCollect() {}
    
    void setVolume(float v) { volume = v; }
    void mute(bool m) { muted = m; }
    bool isMuted() const { return muted; }

private:
    float volume;
    bool muted;
};

#endif