#include "catch.hpp"

#include "apps/audiobook/AudiobookPlayer.h"
#include <chrono>
#include <thread>

SCENARIO("Interacting with audiobook (choosing, playing, pausing, rewinding)")
{
    GIVEN("Initialized audiobook player and played audiobook")
    {
        Pdb::AudioManager audioManager;
        Pdb::AudiobookPlayer audiobookPlayer(audioManager);
        Pdb::AudioTrack track("powrot-z-gwiazd.mp3");
        audiobookPlayer.playAudiobook(track);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        WHEN ("Playing more audiobooks")
        {
            audiobookPlayer.playAudiobook(track);
            audiobookPlayer.playAudiobook(track);
            audiobookPlayer.playAudiobook(track);
            std::this_thread::sleep_for(std::chrono::seconds(1));

            THEN ("Nothing happens. The original audiobook is still being played")
            {
                REQUIRE ( audioManager.getFreeMp3AudioStreamCount() == audioManager.getMp3AudioStreamCount() - 1 ); 
            }
        }

        WHEN ("Pausing the audiobook")
        {
            audiobookPlayer.pauseAudiobook();

            THEN ("Audiobook is paused")
            {
            }
        }
    }
}