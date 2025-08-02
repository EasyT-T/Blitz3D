//
// Created by EasyT_T on 2025/7/31.
//

#include "bbaudio_fmod.h"

#include <algorithm>

gxAudio* gx_audio = nullptr;

namespace
{
    std::set<ISound*> sounds;
    std::set<IChannel*> channels;

    bool debugSound(ISound* sound)
    {
        const bool result = sounds.find(sound) == sounds.end();

        if (debug && result)
            RTEX("Sound does not exist"); // NOLINT

        return result;
    }

    bool debugChannel(IChannel* channel)
    {
        const bool result = channels.find(channel) == channels.end();

        if (debug && result)
            RTEX("Channel does not exist"); // NOLINT

        return result;
    }

    void SOUND_CALLBACK OnAutoRelease(IChannel* channel)
    {
        bbStopChannel(channel);
    }
}

void bbUpdateAudio()
{
    gx_audio->update();
}

ISound* bbLoadSound(const BBStr* path)
{
    ISound* sound = gx_audio->loadSound(path->c_str());

    if (sound != nullptr)
    {
        sounds.emplace(sound);
    }

    delete path;

    return sound;
}

ISound* bbLoad3DSound(const BBStr* path)
{
    ISound* sound = gx_audio->loadSound(path->c_str(), SOUND_3D);

    if (sound != nullptr)
    {
        sounds.emplace(sound);
    }

    delete path;

    return sound;
}

void bbFreeSound(ISound* sound)
{
    if (debugSound(sound))
    {
        return;
    }

    sounds.erase(sound);

    delete sound;
}

void bbLoopSound(ISound* sound)
{
    if (debugSound(sound))
    {
        return;
    }

    sound->setMode(sound->getMode() | SOUND_LOOP);
}

void bbSoundVolume(ISound* sound, const float volume)
{
    if (debugSound(sound))
    {
        return;
    }

    sound->setVolume(std::clamp(volume, 0.0f, 1.0f));
}

void bbSoundPan(ISound* sound, const float pan)
{
    if (debugSound(sound))
    {
        return;
    }

    sound->setPan(pan);
}

void bbSoundPitch(ISound* sound, int pitch)
{
    if (debugSound(sound))
    {
        return;
    }

    sound->setPitch(pitch);
}

IChannel* bbPlaySound(ISound* sound)
{
    if (debugSound(sound))
    {
        return nullptr;
    }

    IChannel* channel = gx_audio->playSound(sound);

    if (channel != nullptr)
    {
        channels.emplace(channel);
    }

    return channel;
}

IChannel* bbPlayMusic(const BBStr* path, bool loop)
{
    SOUND_MODE mode = SOUND_STREAM;

    if (loop)
    {
        mode |= SOUND_LOOP;
    }

    ISound* sound = gx_audio->loadSound(path->c_str(), mode);
    IChannel* channel = gx_audio->playSound(sound);

    if (channel != nullptr)
    {
        channels.emplace(channel);
    }

    delete path;

    return channel;
}

void bbStopChannel(IChannel* channel)
{
    if (channels.find(channel) == channels.end())
    {
        return;
    }

    channels.erase(channel);

    if (channel->getMode() & SOUND_STREAM)
    {
        const ISound* sound = channel->getSound();

        delete sound;
    }

    delete channel;
}

void bbPauseChannel(IChannel* channel)
{
    if (debugChannel(channel))
    {
        return;
    }

    channel->setPaused(true);
}

void bbResumeChannel(IChannel* channel)
{
    if (debugChannel(channel))
    {
        return;
    }

    channel->setPaused(false);
}

void bbChannelVolume(IChannel* channel, float volume)
{
    if (debugChannel(channel))
    {
        return;
    }

    channel->setVolume(std::clamp(volume, 0.0f, 1.0f));
}

void bbChannelPan(IChannel* channel, float pan)
{
    if (debugChannel(channel))
    {
        return;
    }

    channel->setPan(pan);
}

void bbChannelPitch(IChannel* channel, int pitch)
{
    if (debugChannel(channel))
    {
        return;
    }

    channel->setPitch(pitch);
}

int bbChannelPlaying(IChannel* channel)
{
    if (channels.find(channel) == channels.end())
    {
        return false;
    }

    return channel->isPlaying();
}

bool audio_create()
{
    gx_audio = new gxAudio(gx_runtime);

    gx_audio->setAutoReleaseCallback(OnAutoRelease);

    return gx_audio->init();
}

bool audio_destroy()
{
    if (gx_audio == nullptr)
    {
        return false;
    }

    delete gx_audio;
    gx_audio = nullptr;

    return true;
}

void audio_link(void (*rtSym)(const char*, void*))
{
    rtSym("UpdateAudio", bbUpdateAudio);
    rtSym("%LoadSound$filename", bbLoadSound);
    rtSym("%Load3DSound$filename", bbLoadSound);
    rtSym("FreeSound%sound", bbFreeSound);
    rtSym("LoopSound%sound", bbLoopSound);
    rtSym("SoundPitch%sound%pitch", bbSoundPitch);
    rtSym("SoundVolume%sound#volume", bbSoundVolume);
    rtSym("SoundPan%sound#pan", bbSoundPan);
    rtSym("%PlaySound%sound", bbPlaySound);
    rtSym("%PlayMusic$midifile%loop=0", bbPlayMusic);
    rtSym("StopChannel%channel", bbStopChannel);
    rtSym("PauseChannel%channel", bbPauseChannel);
    rtSym("ResumeChannel%channel", bbResumeChannel);
    rtSym("ChannelPitch%channel%pitch", bbChannelPitch);
    rtSym("ChannelVolume%channel#volume", bbChannelVolume);
    rtSym("ChannelPan%channel#pan", bbChannelPan);
    rtSym("%ChannelPlaying%channel", bbChannelPlaying);
}