// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#include "SoundManager.h"
#include <QRandomGenerator>
#include <QDebug>
#include <QSound>
#include <QMediaPlaylist>

CSoundManager::CSoundManager(QObject *parent)
    : QObject(parent)
    , bgMusicPlayer(nullptr)
    , enableSfx(true)
{
    // Populate the paths for the sound effects
    soundFiles[SoundEffectType::CardClick] = {":/assets/card_01.wav", ":/assets/card_02.wav", ":/assets/card_03.wav", ":/assets/card_04.wav"};
    soundFiles[SoundEffectType::CardFlip] = {":/assets/card_flip.wav"};
    soundFiles[SoundEffectType::CardStack] = {":/assets/card_stack_01.wav", ":/assets/card_stack_02.wav", ":/assets/card_stack_03.wav", ":/assets/card_stack_04.wav", ":/assets/card_stack_05.wav"};
    soundFiles[SoundEffectType::CardCannotMove] = {":/assets/card_cant_move.wav"};

    // Create a media playlist containing the background music on a loop
    QMediaPlaylist* musicPlaylist = new QMediaPlaylist();
    musicPlaylist->addMedia(QUrl("qrc:/assets/background_music.mp3"));
    musicPlaylist->setPlaybackMode(QMediaPlaylist::Loop);

    // Create the background music player
    this->bgMusicPlayer = new QMediaPlayer(this);
    this->bgMusicPlayer->setPlaylist(musicPlaylist);
    this->bgMusicPlayer->setVolume(20);
    this->bgMusicPlayer->play();
}

void CSoundManager::playSoundEffect(SoundEffectType sfxType)
{
    // Don't if SFX are disabled
    if (!enableSfx) return;

    // Get the sfx files for this type
    QVector<QString> effects = soundFiles[sfxType];

    // Randomize which sound to use
    int soundIndex = effects.length() > 1 ? QRandomGenerator::global()->bounded(0, effects.length() - 1) : 0;

    // Play the sound effect
    QSound::play(effects[soundIndex]);
}

void CSoundManager::setEnableSoundEffects(bool enabled)
{
    // Set the flag
    this->enableSfx = enabled;
}

void CSoundManager::setEnableMusic(bool enabled)
{
    // Set the music's volume
    this->bgMusicPlayer->setVolume(enabled ? 50 : 0);
}
