// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#pragma once

#include <QObject>
#include <QMap>
#include <QVector>
#include <QSoundEffect>
#include <QMediaPlayer>

// The types of sound effects we can play
enum SoundEffectType
{
    CardClick,
    CardFlip,
    CardStack,
    CardCannotMove
};

// A global class to handle various audio from the game
class CSoundManager : public QObject
{
    Q_OBJECT

public:
    // Constructor
    CSoundManager(QObject *parent = nullptr);

public:
    // Plays the given sound effect
    void playSoundEffect(SoundEffectType sfxType);

    // Sets whether to enable sound effects
    void setEnableSoundEffects(bool enabled);

    // Sets whether to enable music
    void setEnableMusic(bool enabled);

private:
    // List of sound effect files associated to their resepctive effect type
    QMap<SoundEffectType, QVector<QString>> soundFiles;

    // The media player for the bg music
    QMediaPlayer* bgMusicPlayer;

    // Whether to enable sound effects
    bool enableSfx;
};
