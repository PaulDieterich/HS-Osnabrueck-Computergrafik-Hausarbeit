//
//  SoundPlayer.h
//  Spielt Soundeffekte und Ambiences als Teil eines Radios ab
//
//  @author Justin Falkenstein
//

#pragma once
#include <irrKlang.h>
#include <vector>
#include <thread>
#include <iostream>
#include <string>
#include <map>
#include "Utility.h"

#define _GLIBCXX_USE_NANOSLEEP
#define GLOBAL_VOLUME 0.1
enum SoundIndex {
	SHIP_THRUST,
	SHIP_LASER,
	SHIP_ERROR_SOFT,
	SHIP_ALARM_SOFT,
	RADIO_STATIC
} typedef SoundIndex;

using namespace std;
using namespace irrklang;
using namespace Logger;


class SoundPlayer
{
private:
	ISoundEngine* SoundEngine = createIrrKlangDevice(); // Das Kernobjekt von irrklang
	map<SoundIndex, ISoundSource*> sources;				// Die sources, die gemappt werden

	// Radio variablen
	vector<ISoundSource*> radioSources;					// Die sources für die Musik
	ISound* currentMusic;								// Das Soundobjekt des momentanen Songs
	int radioIndex = 0;									// Der Index für den momentanen Song aus den Resourcen
	bool radioActive = false;							// Bestimmt, ob das Radio eingeschaltet ist

public:
	SoundPlayer();
	~SoundPlayer();

	/**
	 * Spielt einen Sound unabhängig der Position ab. Für Bewegungsparameter playSound3D implementieren.
	 * Aktive, geloopte Sounds werden in die Liste activeSounds gespeichert, um später wieder gestoppt werden zu können
	 * @param soundIndex der an die registrierten Sounds gemappte Key aus dem Enum
	 * @param loop Bestimmt, ob der Sound geloopt werden soll
	 * @param onlyIfNotPlaying Bestimmt, ob der Effekt nur gespielt werden soll, wenn er nicht gerade schon gespielt wird
	 * @param volume Optional. Lautstärke zwischen 0 und 1
	 */
	void playSound2D(SoundIndex soundIndex, bool loop, bool onlyIfNotPlaying = true, float volume = GLOBAL_VOLUME);

	/**
	 * Stoppt einen Soundeffekt, sofern er in der Liste der aktiven Sounds ist
	 * @param soundIndex der an die registrierten Sounds gemappte Key aus dem Enum
	 */
	void stopSound(SoundIndex soundIndex);

	/**
	 * Fadet einen Sound ein. Sollte asynchron aufgerufen und ggf. vorher geloopt werden
	 * @param sound Das irrklang sound objekt
	 * @param time Die Zeit in Sekunden, in denen der Sound eingefadet werden soll
	 * @param targetVolume die Endlautstärke
	 */
	void fadeIn(ISound* sound, float time, float targetVolume);

	/**
	 * Fadet einen Sound aus und stoppt ihn dann. Sollte asynchron aufgerufen werden!
	 * @param sound Das irrklang sound objekt
	 * @param time Die Zeit in Sekunden, in denen der Sound ausgefadet werden soll
	 */
	void fadeOut(ISound* sound, float time);

	/**
	 * Überladene Funktionen zum bestimmen, ob ein Sound momentan abgespielt wird
	 * Da irrklang das über die isPlaying Funktion nur bei parsen des Namens macht, ist das hier unsere Lösung.
	 * @param sound Der Zeiger auf ein ISound objekt, die source oder den gemappten Key
	 */
	bool isPlaying(ISound* sound);

	//
	// Radio Funktionen
	//

	/**
	 * Spielt das Lied am aktuellen Index für das Radio im Spiel
	 */
	void playRadioMusic();

	/**
	 * Stoppt das aktuelle lied im Radio und gibt die Ressource frei
	 */
	void stopRadioMusic();

	/**
	 * Setzt den Zeiger für das Radio auf das nächste Lied und spielt es ab.
	 * Wenn der Zeiger auf dem letzten Element liegt wird es wieder nach vorne gesetzt.
	 */
	void next();

	/**
	 * Setzt den Zeiger für das Radio auf das vorherige Lied und spielt es ab.
	 * Wenn der Zeiger auf dem ersten Element liegt wird es wieder nach hinten gesetzt.
	 */
	void previous();

	/**
	 * Wechselt zwischen an und ausschalten, je nachdem, wie das radioActive flag gesetzt ist.
	 */
	void toggleRadio();

};

