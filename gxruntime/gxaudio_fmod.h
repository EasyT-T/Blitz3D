//
// Created by EasyT_T on 2025/7/31.
//

#ifndef GXAUDIO
#define GXAUDIO

#include <array>

#include "fmod.hpp"
#include "gxruntime_b3d.h"

#define SOUND_CALLBACK __stdcall

typedef unsigned int SOUND_MODE;
#define SOUND_DEFAULT 0
#define SOUND_STREAM (1 << 1)
#define SOUND_3D (1 << 2)
#define SOUND_LOOP (1 << 3)

struct ThreeDAttributes
{
    std::array<float, 3> position{};
    std::array<float, 3> velocity{};
};

struct ThreeDListenerAttributes
{
    std::array<float, 3> position{};
    std::array<float, 3> velocity{};
    std::array<float, 3> forward{};
    std::array<float, 3> up{};
};

class ISound
{
public:
    virtual ~ISound() = default;

    virtual void setVolume(float volume) = 0;
    virtual void setPitch(float pitch) = 0;
    virtual void setFrequency(float frequency) = 0;
    virtual void setPan(float pan) = 0;
    virtual void setMode(SOUND_MODE mode) = 0;
    virtual void set3DAttributes(ThreeDAttributes attributes) = 0;

    virtual float getVolume() const = 0;
    virtual float getPitch() const = 0;
    virtual float getFrequency() const = 0;
    virtual float getPan() const = 0;
    virtual SOUND_MODE getMode() const = 0;
    virtual ThreeDAttributes get3DAttributes() const = 0;
};

class IChannel
{
public:
    virtual ~IChannel() = default;

    virtual void setPaused(bool paused) = 0;
    virtual void setVolume(float volume) = 0;
    virtual void setPitch(float pitch) = 0;
    virtual void setFrequency(float frequency) = 0;
    virtual void setPan(float pan) = 0;
    virtual void setMode(SOUND_MODE mode) = 0;
    virtual void set3DAttributes(const ThreeDAttributes& attributes) = 0;

    virtual bool getPaused() const = 0;
    virtual float getVolume() const = 0;
    virtual float getPitch() const = 0;
    virtual float getFrequency() const = 0;
    virtual float getPan() const = 0;
    virtual SOUND_MODE getMode() = 0;
    virtual ThreeDAttributes get3DAttributes() const = 0;

    virtual bool isPlaying() const = 0;

    virtual ISound* getSound() const = 0;
};

typedef void (SOUND_CALLBACK *SOUND_CHANNEL_AUTO_RELEASE)(IChannel* channel);

class IAudioSystem
{
public:
    virtual ~IAudioSystem() = default;

    virtual bool init() = 0;
    virtual void update() = 0;

    virtual ISound* loadSound(const char* filename, SOUND_MODE mode) = 0;
    virtual IChannel* playSound(ISound* sound) = 0;

    virtual void set3DSettings(float dopplerscale, float distancefactor, float rolloffscale) = 0;
    virtual void set3DListenerAttributes(const ThreeDListenerAttributes& attributes) = 0;
    virtual void setAutoReleaseCallback(SOUND_CHANNEL_AUTO_RELEASE autoRelease) = 0;

    virtual ThreeDListenerAttributes get3DListenerAttributes() const = 0;
};

class FSound final : public ISound
{
public:
    explicit FSound(FMOD::Sound* fsound_handle);
    ~FSound() override;

    void setVolume(float volume) override;
    void setPitch(float pitch) override;
    void setPan(float pan) override;
    void setFrequency(float frequency) override;
    void setMode(SOUND_MODE mode) override;
    void set3DAttributes(ThreeDAttributes attributes) override;

    bool getDefaultsChanged() const;

    float getVolume() const override;
    float getPitch() const override;
    float getPan() const override;
    float getFrequency() const override;
    SOUND_MODE getMode() const override;
    ThreeDAttributes get3DAttributes() const override;

    FMOD::Sound* get() const;

private:
    FMOD::Sound* fsound_handle;

    bool defaultsChanged = false;
    float defaultVolume = 1.0f;
    float defaultPitch = 1.0f;
    float defaultPan = 0.0f;
    ThreeDAttributes defaultAttributes{};

    float defaultFrequency;
    int defaultPriority;
};

class FChannel final : public IChannel
{
public:
    explicit FChannel(FSound* sound, FMOD::Channel* fchannel_handle);
    ~FChannel() override;

    void setPaused(bool paused) override;
    void setVolume(float volume) override;
    void setPitch(float pitch) override;
    void setFrequency(float frequency) override;
    void setPan(float pan) override;
    void setMode(SOUND_MODE mode) override;
    void set3DAttributes(const ThreeDAttributes& attributes) override;

    bool getPaused() const override;
    float getVolume() const override;
    float getPitch() const override;
    float getFrequency() const override;
    float getPan() const override;
    SOUND_MODE getMode() override;
    ThreeDAttributes get3DAttributes() const override;

    bool isPlaying() const override;

    FSound* getSound() const override;

private:
    FSound* sound;
    FMOD::Channel* fchannel_handle;
};

class FAudioSystem final : public IAudioSystem
{
public:
    explicit FAudioSystem(gxRuntime* runtime);
    ~FAudioSystem() override;

    bool init() override;
    void update() override;

    FSound* loadSound(const char* filename, SOUND_MODE mode) override;
    FChannel* playSound(ISound* sound) override;

    void set3DSettings(float dopplerscale, float distancefactor, float rolloffscale) override;
    void set3DListenerAttributes(const ThreeDListenerAttributes& attributes) override;
    void setAutoReleaseCallback(SOUND_CHANNEL_AUTO_RELEASE autoReleaseCallback) override;

    ThreeDListenerAttributes get3DListenerAttributes() const override;

private:
    gxRuntime* runtime;
    FMOD::System* fmod;

    SOUND_CHANNEL_AUTO_RELEASE on_auto_release = nullptr;

    static FMOD_RESULT _stdcall channelCallback(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype,
                                         FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype, void* commanddata1,
                                         void* commanddata2);
};

class gxAudio
{
public:
    explicit gxAudio(gxRuntime* runtime);
    ~gxAudio();

    bool init() const;
    void update() const;

    ISound* loadSound(const char* filename, SOUND_MODE mode = SOUND_DEFAULT) const;
    IChannel* playSound(ISound* sound) const;

    void set3DSettings(float dopplerscale, float distancefactor, float rolloffscale) const;
    void set3DListenerAttributes(const ThreeDListenerAttributes& attributes) const;
    void setAutoReleaseCallback(SOUND_CHANNEL_AUTO_RELEASE autoReleaseCallback) const;

private:
    gxRuntime* runtime;
    IAudioSystem* system;
};


#endif //GXAUDIO
