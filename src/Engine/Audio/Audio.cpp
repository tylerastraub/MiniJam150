#include "Audio.h"

#include <iostream>
#include <SDL.h>

Audio::Audio() {
    _context = cs_make_context(NULL, 44100, 1024, 0, NULL);
    cs_spawn_mix_thread(_context);
    if(cs_error_reason) {
        std::cout << "Error creating cute_sound context! cs_error_reason: " << cs_error_reason << std::endl;
        return;
    }
}

Audio::~Audio() {
    stopAllAudio();
    for(auto it : _playingAudio) {
        for(auto jt : it.second) {
            if(jt.second) delete jt.second;
            jt.second = nullptr;
        }
    }
    for(auto it : _loadedAudio) {
        cs_free_sound(it.second);
        it.second = nullptr;
    }
}

bool Audio::addAudio(AudioSound soundName, const char * filepath) {
    std::string fullPath;
    fullPath += SDL_GetBasePath();
    fullPath += filepath;
    cs_loaded_sound_t* sound = new cs_loaded_sound_t(cs_load_wav(fullPath.c_str()));
    if(cs_error_reason) {
        std::cout << "Error loading '" << filepath << "'! cs_error_reason: " << cs_error_reason << std::endl;
        return false;
    }
    _loadedAudio[soundName] = sound;
    return true;
}

void Audio::playAudio(entt::entity entity, AudioSound audio, float volume, bool looping) {
    if(!_playingAudio[entity][audio]) _playingAudio[entity][audio] = new AudioPool(_context);
    _playingAudio[entity][audio]->addPlayingSound(_loadedAudio[audio], volume, looping);
}

void Audio::stopAudio(entt::entity entity, AudioSound audio) {
    if(_playingAudio[entity][audio]) _playingAudio[entity][audio]->stopSound();
}

void Audio::stopAllAudioById(entt::entity id) {
    for(auto it : _playingAudio[id]) {
        if(it.second) it.second->stopSound();
    }
}

void Audio::stopAllAudio() {
    for(auto e : _playingAudio) {
        for(auto s : e.second) {
            if(s.second) s.second->stopSound();
        }
    }
}

bool Audio::isPlaying(entt::entity entity, AudioSound soundName) {
    if(_playingAudio[entity][soundName]) return _playingAudio[entity][soundName]->isPlaying();
    return false;
}