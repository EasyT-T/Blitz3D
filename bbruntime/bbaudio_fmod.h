//
// Created by EasyT_T on 2025/7/31.
//

#ifndef BBAUDIO_FMOD_H
#define BBAUDIO_FMOD_H

#include "bbsys.h"
#include "gxruntime/gxaudio_fmod.h"

ISound* bbLoadSound(const BBStr* path);

ISound* bbLoad3DSound(const BBStr* path);

void bbFreeSound(ISound* sound);

void bbLoopSound(ISound* sound);

void bbSoundVolume(ISound* sound, float volume);

void bbSoundPan(ISound* sound, float pan);

void bbSoundPitch(ISound* sound, int pitch);

IChannel* bbPlaySound(ISound* sound);

IChannel* bbPlayMusic(const BBStr* path, bool loop);

void bbStopChannel(IChannel* channel);

void bbPauseChannel(IChannel* channel);

void bbResumeChannel(IChannel* channel);

void bbChannelVolume(IChannel* channel, float volume);

void bbChannelPan(IChannel* channel, float pan);

void bbChannelPitch(IChannel* channel, int pitch);

int bbChannelPlaying(IChannel* channel);

#endif //BBAUDIO_FMOD_H
