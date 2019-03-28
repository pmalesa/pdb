#include "AudiobookPlayer.h"
#include <boost/log/trivial.hpp>
#include <future>
#include <memory>

namespace Pdb
{

AudiobookPlayer::AudiobookPlayer(AudioManager& audioManager, VoiceManager& voiceManager) 
    : audioManager_(audioManager), voiceManager_(voiceManager)
{
    
}

void AudiobookPlayer::playAudiobook(AudioTrack& audioTrack)
{
    BOOST_LOG_TRIVIAL(info) << "Playing audiotrack: " << audioTrack.getTrackName() << " (" << audioTrack.getFilePath() << ")";
    currentlyPlayedAudioStream_ = audioManager_.playMultipleAndGetLastAudioStream(std::vector<AudioTrack>({
            voiceManager_.getSynthesizedVoiceAudioTracks().at("playing_audiobook"),
            voiceManager_.getSynthesizedVoiceAudioTracks().at(audioTrack.getTrackName()),
            audioTrack
        })).get();
}

void AudiobookPlayer::pauseAudiobook(AudioTrack& audioTrack)
{
    BOOST_LOG_TRIVIAL(info) << "Pausing audiotrack: " << audioTrack.getTrackName();
    currentlyPlayedAudioStream_->pause();
}

}