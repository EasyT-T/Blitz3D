//
// Created by EasyT_T on 2025/7/31.
//

#include "gxaudio_fmod.h"

namespace
{
    SOUND_MODE convertFModeToSMode(const FMOD_MODE fmode)
    {
        SOUND_MODE mode = SOUND_DEFAULT;

        if (fmode & FMOD_CREATESTREAM)
        {
            mode |= SOUND_STREAM;
        }

        if (fmode & FMOD_3D)
        {
            mode |= SOUND_3D;
        }

        if (fmode & FMOD_LOOP_NORMAL)
        {
            mode |= SOUND_LOOP;
        }

        return mode;
    }

    FMOD_MODE convertSModeToFMode(const SOUND_MODE smode)
    {
        FMOD_MODE mode = FMOD_DEFAULT;

        if (smode & SOUND_STREAM)
        {
            mode |= FMOD_CREATESTREAM;
        }

        if (smode & SOUND_3D)
        {
            mode |= FMOD_3D;
        }

        if (smode & SOUND_LOOP)
        {
            mode |= FMOD_LOOP_NORMAL;
        }

        return mode;
    }
}

FSound::FSound(FMOD::Sound* fsound_handle) : fsound_handle(fsound_handle)
{
    fsound_handle->getDefaults(&defaultFrequency, &defaultPriority);
}

FSound::~FSound()
{
    fsound_handle->release();
}

void FSound::setVolume(const float volume)
{
    defaultsChanged = true;
    defaultVolume = volume;
}

void FSound::setPitch(const float pitch)
{
    defaultsChanged = true;
    defaultPitch = pitch;
}

void FSound::setPan(const float pan)
{
    defaultsChanged = true;
    defaultPan = pan;
}

void FSound::setFrequency(const float frequency)
{
    defaultFrequency = frequency;

    fsound_handle->setDefaults(defaultFrequency, defaultPriority);
}

void FSound::setMode(const SOUND_MODE mode)
{
    const FMOD_MODE fmode = convertSModeToFMode(mode);

    fsound_handle->setMode(fmode);
}

void FSound::set3DAttributes(const ThreeDAttributes attributes)
{
    defaultAttributes = attributes;
}

bool FSound::getDefaultsChanged() const
{
    return defaultsChanged;
}

float FSound::getVolume() const
{
    return defaultVolume;
}

float FSound::getPitch() const
{
    return defaultPitch;
}

float FSound::getPan() const
{
    return defaultPan;
}

float FSound::getFrequency() const
{
    return defaultFrequency;
}

SOUND_MODE FSound::getMode() const
{
    FMOD_MODE fmode;

    if (fsound_handle->getMode(&fmode) != FMOD_OK)
    {
        return SOUND_DEFAULT;
    }

    const SOUND_MODE mode = convertFModeToSMode(fmode);

    return mode;
}

ThreeDAttributes FSound::get3DAttributes() const
{
    return defaultAttributes;
}

FMOD::Sound* FSound::get() const
{
    return fsound_handle;
}

FChannel::FChannel(FSound* sound, FMOD::Channel* fchannel_handle) : sound(sound), fchannel_handle(fchannel_handle)
{
}

FChannel::~FChannel()
{
    fchannel_handle->stop();
    fchannel_handle = nullptr;
}

void FChannel::setPaused(const bool paused)
{
    fchannel_handle->setPaused(paused);
}

void FChannel::setVolume(const float volume)
{
    fchannel_handle->setVolume(volume);
}

void FChannel::setPitch(const float pitch)
{
    fchannel_handle->setPitch(pitch);
}

void FChannel::setFrequency(const float frequency)
{
    fchannel_handle->setFrequency(frequency);
}

void FChannel::setPan(const float pan)
{
    fchannel_handle->setPan(pan);
}

void FChannel::setMode(SOUND_MODE mode)
{
    const FMOD_MODE fmode = convertSModeToFMode(mode);

    fchannel_handle->setMode(fmode);
}

void FChannel::set3DAttributes(const ThreeDAttributes& attributes)
{
    const FMOD_VECTOR position{attributes.position[0], attributes.position[1], attributes.position[2]};
    const FMOD_VECTOR velocity{attributes.velocity[0], attributes.velocity[1], attributes.velocity[2]};

    fchannel_handle->set3DAttributes(&position, &velocity);
}

bool FChannel::getPaused() const
{
    bool paused;

    const FMOD_RESULT result = fchannel_handle->getPaused(&paused);

    return result == FMOD_OK ? paused : true;
}

float FChannel::getVolume() const
{
    float volume;

    const FMOD_RESULT result = fchannel_handle->getVolume(&volume);

    return result == FMOD_OK ? volume : 0.0f;
}

float FChannel::getPitch() const
{
    float pitch;

    const FMOD_RESULT result = fchannel_handle->getPitch(&pitch);

    return result == FMOD_OK ? pitch : 0.0f;
}

float FChannel::getFrequency() const
{
    float frequency;

    const FMOD_RESULT result = fchannel_handle->getFrequency(&frequency);

    return result == FMOD_OK ? frequency : 0.0f;
}

float FChannel::getPan() const
{
    float pan;

    const FMOD_RESULT result = fchannel_handle->getPitch(&pan);

    return result == FMOD_OK ? pan : 0.0f;
}

SOUND_MODE FChannel::getMode()
{
    FMOD_MODE fmode;

    if (fchannel_handle->getMode(&fmode) != FMOD_OK)
    {
        return SOUND_DEFAULT;
    }

    const SOUND_MODE mode = convertFModeToSMode(fmode);

    return mode;
}

ThreeDAttributes FChannel::get3DAttributes() const
{
    FMOD_VECTOR fPosition;
    FMOD_VECTOR fVelocity;

    fchannel_handle->get3DAttributes(&fPosition, &fVelocity);

    return ThreeDAttributes{{fPosition.x, fPosition.y, fPosition.z}, {fVelocity.x, fVelocity.y, fVelocity.z}};
}

bool FChannel::isPlaying() const
{
    bool playing;
    const FMOD_RESULT result = fchannel_handle->isPlaying(&playing);

    return result == FMOD_OK ? playing : false;
}

FSound* FChannel::getSound() const
{
    return sound;
}

FAudioSystem::FAudioSystem(gxRuntime* runtime) : runtime(runtime), fmod(nullptr)
{
}

FAudioSystem::~FAudioSystem()
{
    fmod->release();
}

bool FAudioSystem::init()
{
    FMOD_RESULT result;

    result = FMOD::System_Create(&fmod);

    if (result != FMOD_OK)
    {
        return false;
    }

    result = fmod->init(4095, FMOD_INIT_NORMAL, nullptr);
    fmod->update();

    if (result != FMOD_OK)
    {
        return false;
    }

    return true;
}

void FAudioSystem::update()
{
    fmod->update();
}

FSound* FAudioSystem::loadSound(const char* filename, const SOUND_MODE mode)
{
    FMOD::Sound* fsound_handle;

    FMOD_MODE fmode = FMOD_DEFAULT;

    if (mode & SOUND_3D)
    {
        fmode |= FMOD_3D;
    }

    if (mode & SOUND_STREAM)
    {
        fmode |= FMOD_CREATESTREAM;
    }

    if (mode & SOUND_LOOP)
    {
        fmode |= FMOD_LOOP_NORMAL;
    }

    const FMOD_RESULT result = fmod->createSound(filename, fmode, nullptr, &fsound_handle);

    return result == FMOD_OK ? new FSound(fsound_handle) : nullptr;
}

FChannel* FAudioSystem::playSound(ISound* sound)
{
    const auto fsound = dynamic_cast<FSound*>(sound);

    if (fsound == nullptr)
    {
        return nullptr;
    }

    const bool shouldPause = fsound->getDefaultsChanged();

    FMOD::Channel* fchannel_handle;

    const FMOD_RESULT result = fmod->playSound(fsound->get(), nullptr, shouldPause, &fchannel_handle);

    if (result != FMOD_OK)
    {
        return nullptr;
    }

    const auto channel = new FChannel(fsound, fchannel_handle);

    if (shouldPause)
    {
        channel->setVolume(sound->getVolume());
        channel->setPitch(sound->getPitch());
        channel->setFrequency(sound->getFrequency());
        channel->setPan(sound->getPan());

        if (sound->getMode() & SOUND_3D)
        {
            channel->set3DAttributes(sound->get3DAttributes());
        }

        channel->setPaused(false);
    }

    fchannel_handle->setCallback(channelCallback);
    fchannel_handle->setUserData(new std::pair<FAudioSystem*, FChannel*>(this, channel));

    return channel;
}

void FAudioSystem::set3DSettings(const float dopplerscale, const float distancefactor, const float rolloffscale)
{
    fmod->set3DSettings(dopplerscale, distancefactor, rolloffscale);
}

void FAudioSystem::set3DListenerAttributes(const ThreeDListenerAttributes& attributes)
{
    const FMOD_VECTOR position{attributes.position[0], attributes.position[1], attributes.position[2]};
    const FMOD_VECTOR velocity{attributes.velocity[0], attributes.velocity[1], attributes.velocity[2]};
    const FMOD_VECTOR forward{attributes.forward[0], attributes.forward[1], attributes.forward[2]};
    const FMOD_VECTOR up{attributes.up[0], attributes.up[1], attributes.up[2]};

    fmod->set3DListenerAttributes(0, &position, &velocity, &forward, &up);
}

void FAudioSystem::setAutoReleaseCallback(const SOUND_CHANNEL_AUTO_RELEASE autoReleaseCallback)
{
    on_auto_release = autoReleaseCallback;
}

ThreeDListenerAttributes FAudioSystem::get3DListenerAttributes() const
{
    FMOD_VECTOR position;
    FMOD_VECTOR velocity;
    FMOD_VECTOR forward;
    FMOD_VECTOR up;

    fmod->get3DListenerAttributes(0, &position, &velocity, &forward, &up);

    return ThreeDListenerAttributes
    {
        {position.x, position.y, position.z},
        {velocity.x, velocity.y, velocity.z},
        {forward.x, forward.y, forward.z},
        {up.x, up.y, up.z}
    };
}

FMOD_RESULT FAudioSystem::channelCallback(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE,
                                          const FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype, void*, void*)
{
    const auto fchannel_handle = reinterpret_cast<FMOD::Channel*>(channelcontrol);
    std::pair<FAudioSystem*, FChannel*>* pair;

    fchannel_handle->getUserData(reinterpret_cast<void**>(&pair));

    const auto system = pair->first;
    const auto channel = pair->second;

    if (callbacktype == FMOD_CHANNELCONTROL_CALLBACK_END && system->on_auto_release != nullptr)
    {
        system->on_auto_release(channel);
    }

    return FMOD_OK;
}

gxAudio::gxAudio(gxRuntime* runtime) : runtime(runtime), system(new FAudioSystem(runtime))
{
}

gxAudio::~gxAudio()
{
    delete system;
    system = nullptr;
}

bool gxAudio::init() const
{
    return system->init();
}

void gxAudio::update() const
{
    system->update();
}

ISound* gxAudio::loadSound(const char* filename, const SOUND_MODE mode) const
{
    return system->loadSound(filename, mode);
}

IChannel* gxAudio::playSound(ISound* sound) const
{
    return system->playSound(sound);
}

void gxAudio::set3DSettings(const float dopplerscale, const float distancefactor, const float rolloffscale) const
{
    system->set3DSettings(dopplerscale, distancefactor, rolloffscale);
}

void gxAudio::set3DListenerAttributes(const ThreeDListenerAttributes& attributes) const
{
    system->set3DListenerAttributes(attributes);
}

void gxAudio::setAutoReleaseCallback(const SOUND_CHANNEL_AUTO_RELEASE autoReleaseCallback) const
{
    system->setAutoReleaseCallback(autoReleaseCallback);
}
