// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#include "SoundManager.h"
#include <QRandomGenerator>

CSoundManager::CSoundManager(QObject *parent)
    : QObject(parent)
{
    // Populate the paths for the sound effects
    soundFiles[SoundEffectType::CardClick] = {":/assets/card_01.wav", ":/assets/card_02.wav", ":/assets/card_03.wav", ":/assets/card_04.wav"};
    soundFiles[SoundEffectType::CardFlip] = {":/assets/card_flip.mp3"};
    soundFiles[SoundEffectType::CardStack] = {":/assets/card_stack_01.wav", ":/assets/card_stack_02.wav", ":/assets/card_stack_03.wav", ":/assets/card_stack_04.wav", ":/assets/card_stack_05.wav"};

    // Create the QSoundEffect player
    this->player = new QSoundEffect(this);
}

void CSoundManager::playSoundEffect(SoundEffectType sfxType)
{
    // Get the sfx files for this type
    QVector<QString> effects = soundFiles[sfxType];

    // Randomize which sound to use
    int soundIndex = QRandomGenerator::global()->bounded(0, effects.length() - 1);

    // Assign the file
    player->setSource(QUrl::fromLocalFile(effects[soundIndex]));

    // Play it
    player->play();
}
