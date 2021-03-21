//
//  Utility.h
//  Ein Mischmasch aus allem möglichen nützlichen Zeug, organisiert in namespaces
//
//  @author Justin Falkenstein
//

#pragma once

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

#include <iostream>
#include <stdlib.h>
#include "vector.h"
#include "color.h"
#define PI 3.1415
#define RAD 0.017453292519943 // Man hätte auch mit PI rechnen können, aber das ist einfacher ;3

// Cout colors
#define RESET   "\033[0m"
#define BLACK   "\033[30m" 
#define RED     "\033[31m" 
#define GREEN   "\033[32m" 
#define YELLOW  "\033[33m"  
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"    
#define CYAN    "\033[36m"   
#define WHITE   "\033[37m"     
#define BOLDBLACK   "\033[1m\033[30m"  
#define BOLDRED     "\033[1m\033[31m"     
#define BOLDGREEN   "\033[1m\033[32m"   
#define BOLDYELLOW  "\033[1m\033[33m"    
#define BOLDBLUE    "\033[1m\033[34m"     
#define BOLDMAGENTA "\033[1m\033[35m"    
#define BOLDCYAN    "\033[1m\033[36m"     
#define BOLDWHITE   "\033[1m\033[37m"  

using namespace std;

namespace Math {
	
	/**
	 * Gibt ein zufälligen int Wert in einem Bereich zurück
	 * Um bessere Zufallswerte zu generieren ist in der main einmal der seed gesetzt worden
	 */
	static int getRandomIntInRange(int min, int max)
	{
		return min + (std::rand() % (max - min + 1));

	}

	/**
	 * Gibt ein zufälligen float Wert in einem Bereich zurück
	 * Um bessere Zufallswerte zu generieren ist in der main einmal der seed gesetzt worden
	 */
	static float getRandomFloatInRange(float min, float max) {
		float random = ((float)rand()) / (float)RAND_MAX;
		float diff = max - min;
		float r = random * diff;
		return min + r;
	}

	/**
	 * Gibt einen zufälligen Vector zwischen zwei Vectoren zurück
	 * Dabei ist jede Achse zufällig zwischen min und max der jeweiligen Achse!
	 */
	static Vector getRandomVectorInRange(Vector& min, Vector& max) {
		return Vector(getRandomFloatInRange(min.X, max.X), getRandomFloatInRange(min.Y, max.Y), getRandomFloatInRange(min.Z, max.Z));
	}

	/**
	 * Gibt einen zufälligen, ebenen Vector zwischen zwei Werten zurück
	 * Dabei ist jede Achse gleich!
	 */
	static Vector getRandomEvenVectorInRange(float min, float max) {
		float rand = getRandomFloatInRange(min, max);
		return Vector(rand, rand, rand);
	}

	/**
	 * Gibt eine zufällige Farbe zurück
	 */
	static Color getRandomColor() {
		return Color(getRandomFloatInRange(0, 1), getRandomFloatInRange(0, 1), getRandomFloatInRange(0, 1));
	}

	/**
	 * Rechnet einen GRAD Wert in einen RADIANT um
	 */
	static float degInRad(float deg) {
		return deg * RAD;
	}

	/**
	 * Rechnet einen RADIANT in einen GRAD Wert um
	 */
	static float radInDeg(float rad) {
		return rad / RAD;
	}

	/**
	 * Gibt einen Wert für sinusförmige Frequenzen zurück. Nützlich für Pendelbewegungen
	 * Je höher die Amplitude, umso höher die Maxima
	 * @param relative Eine von einer stetig veränderten, inkrementierten Wert abhänhige Komponente
	 * @param amplitude Bestimmt die Maxima (Wertebereich)
	 * @param wavelength Die Wellenlänge, bestimmt wie viele Perioden in einer bestimmten Zeit vorhanden sind
	 */
	static float calcuateFrequency(float relative, float amplitude, float wavelength) {
		return (sin(relative * wavelength)) * amplitude;
	}


}

namespace Logger {

	enum Severity {
		INFO,
		WARN,
		ERROR
	} typedef Severity;

	static void cry() {
		cout << ":(" << endl;
	}

	/**
	 * Log Funktion für verschiedene severities, akzeptiert parameter pack um auch verschiedenste auszugeben
	 * ohne vorher konvertieren zu müssen...
	 */
	template <typename Arg, typename... Args>
	void log(Severity severity, Arg&& arg, Args&&... args)
	{
		cout << RESET;
		switch (severity)
		{
		case Logger::INFO:
			cout << BLUE << "[INFO]: ";
			break;
		case Logger::WARN:
			cout << YELLOW << "[WARN]: ";
			break;
		case Logger::ERROR:
			cout << RED << "[ERROR]: ";
			break;
		default:
			throw new exception("Error logging error");
			break;
		}
		cout << RESET << std::forward<Arg>(arg);
		using expander = int[];
		(void)expander {
			0, (void(cout << std::forward<Args>(args)), 0)...
		};

		cout << endl;
	}


}