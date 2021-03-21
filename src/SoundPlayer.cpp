#include "SoundPlayer.h"


void SoundPlayer::fadeIn(ISound* sound, float time, float targetVolume)
{
	if (!sound) return;

	float increasePerSecond = targetVolume / (time*60); // Pro millisekunde
	
	sound->setVolume(0);
	sound->setIsPaused(false);
	while (sound->getVolume() < targetVolume) {
		sound->setVolume(sound->getVolume() + increasePerSecond);
		this_thread::sleep_for(chrono::milliseconds(1));
	}
}

void SoundPlayer::fadeOut(ISound* sound, float time)
{
	if (!sound) return;
	float decreasePerSecond = sound->getVolume() / (time*60);

	while (sound && sound->getVolume() > 0) {
		sound->setVolume(sound->getVolume() - decreasePerSecond);
		this_thread::sleep_for(chrono::milliseconds(1));
	}

	sound->stop(); // Gibt ressourcen frei!
}

bool SoundPlayer::isPlaying(ISound* sound)
{
	if (!sound) return false;
	return !sound->isFinished() && !sound->getIsPaused();
}


void SoundPlayer::playRadioMusic()
{
	if (radioActive) return;

	ISoundSource* source = radioSources[radioIndex];
	if (!source) log(ERROR, "Radio source nicht gefunden!");

	currentMusic = SoundEngine->play2D(source, true, true, true, true);
	currentMusic->setVolume(GLOBAL_VOLUME);
	currentMusic->setIsPaused(false);
	radioActive = true;
}


void SoundPlayer::next()
{
	if (!radioActive) return;
	stopRadioMusic();
	if (++radioIndex > radioSources.size()-1) radioIndex = 0;
	playRadioMusic();
}

void SoundPlayer::previous()
{
	if (!radioActive) return;

	stopRadioMusic();
	if (--radioIndex < 0) radioIndex = radioSources.size()-1;
	playRadioMusic();
}

void SoundPlayer::stopRadioMusic()
{
	if (!radioActive) return;
	radioActive = false;
	currentMusic->stop();
}

void SoundPlayer::toggleRadio()
{
	if (!radioActive) playRadioMusic();
	else stopRadioMusic();
}


SoundPlayer::SoundPlayer()
{
	// Mögliche Sounds initialisieren
	sources = {
		{ SHIP_THRUST,  SoundEngine->addSoundSourceFromFile(ASSET_DIRECTORY "sounds/thrusters/thrusterslow.wav") },
		{ SHIP_LASER, SoundEngine->addSoundSourceFromFile(ASSET_DIRECTORY "sounds/weapons/lasershot.wav") },
		{ RADIO_STATIC, SoundEngine->addSoundSourceFromFile(ASSET_DIRECTORY "sounds/ship/radiostaticshort.wav")},
		{ SHIP_ERROR_SOFT, SoundEngine->addSoundSourceFromFile(ASSET_DIRECTORY "sounds/ship/errorsoft.wav") },
		{ SHIP_ALARM_SOFT, SoundEngine->addSoundSourceFromFile(ASSET_DIRECTORY "sounds/ship/alarmsoft.wav")},
	};

	// Radio Musik initialisieren
	radioSources = {
		SoundEngine->addSoundSourceFromFile(ASSET_DIRECTORY "music/interstellar.mp3"),
		SoundEngine->addSoundSourceFromFile(ASSET_DIRECTORY "music/fallout.mp3"),
		SoundEngine->addSoundSourceFromFile(ASSET_DIRECTORY "music/horizon.mp3"),
		SoundEngine->addSoundSourceFromFile(ASSET_DIRECTORY "music/mario.mp3")
	};
}

SoundPlayer::~SoundPlayer()
{
	// Ressourcen freigeben
	SoundEngine->stopAllSounds();
	SoundEngine->drop();
}

void SoundPlayer::playSound2D(SoundIndex soundIndex, bool loop, bool onlyIfNotPlaying, float volume)
{
	ISoundSource* source = sources.at(soundIndex);

	// Gucken, ob sound schon gespielt wird. Wenn ja, prüfen, ob mehrfach darf
	if (SoundEngine->isCurrentlyPlaying(source) && onlyIfNotPlaying) return;
	
	// Sound pausiert erzeugen, bearbeiten und dann abspielen
	ISound* sound = SoundEngine->play2D(source, loop, true, true, true);
	sound->setVolume(volume);
	sound->setIsPaused(false);

}

void SoundPlayer::stopSound(SoundIndex soundIndex)
{
	
	ISoundSource* source = sources.at(soundIndex);
	if (!SoundEngine->isCurrentlyPlaying(source)) return;

	// Sound stoppen, aber neu initialisieren, da irrklang doch ganz schön scheiße ist
	SoundEngine->stopAllSoundsOfSoundSource(source);
	

}
	


