//
//  Player.h
//  Stellt den Spieler dar und hält Funktionen für In-Game Interaktionen wie Steuerung
//
//  @author Justin Falkenstein & Paul Dieterich
//
#pragma once

#ifdef WIN32
#include <GL/glew.h>
#include <glfw/glfw3.h>
#define _USE_MATH_DEFINES
#include <math.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#include <glfw/glfw3.h>
#endif
#include "vector.h"
#include "Matrix.h"

enum Key {
	RADIO_NEXT = GLFW_KEY_PAGE_UP,
	RADIO_PREVIOUS = GLFW_KEY_PAGE_DOWN,
	RADIO_SWITCH = GLFW_KEY_R,

	SHIP_FORWARD = GLFW_KEY_W,
	SHIP_BACKWARDS = GLFW_KEY_S,
	SHIP_LEFT = GLFW_KEY_A,
	SHIP_RIGHT = GLFW_KEY_D,
	SHIP_BOOST = GLFW_KEY_LEFT_SHIFT,

	END_GAME = GLFW_KEY_ESCAPE,
	RESTART_GAME = GLFW_KEY_DELETE
} typedef Key;

enum MouseKey {
	SHOOT_PRIMARY = GLFW_MOUSE_BUTTON_LEFT,
	SHOOT_CHARGED = GLFW_MOUSE_BUTTON_RIGHT,

} typedef MouseKey;

class Player
{
private:
	float money;
public:
	Player(float startMoney): money(startMoney) {}

	/**
	 * Bestimmt den Bewegungsinput des Spielers
	 * @param pWindow Das GLFWWindow Referenzobjekt als Zeiger
	 * @param roll Float Referenz zur Speicherung des wertes 0 für keine Interaktion, 1 für Taste A und -1 für D
	 * @param forward Float Referenz zur Speicherung des wertes 0 für keine Interaktion, 1 für Taste W und -1 für S
	 */
	bool playerInput(GLFWwindow* pWindow, int& roll, int& forward);

	/**
	 * Liefert die Mauscursor Werte
	 * @param pWindow Das GLFWWindow Referenzobjekt als Zeiger
	 * @param x Die float Referenz für die X Koordinate
	 * @param y Die float Referenz für die Y Koordinate
	 */
	void cursorMovement(GLFWwindow* pWindow, double& x, double& y);

	
	/**
	 * Gibt die Spielerbewegungsmatrix zurück, die für die Schiffbewegung steht
	 * @param pWindow Das GLFWWindow Referenzobjekt als Zeiger
	 * @param float dtime Die aktuelle Deltatime
	 * @param shipSpeed Die Geschwindigkeit des Schiffes zur Berechnung der Bewegung
	 * @param drive Referenz, in der gespeichert wird, ob das Schiff auch bewegt wird oder nur die Kamera
	 */
	Matrix playerMoveMatrix(GLFWwindow* pWindow, float dtime, float shipSpeed, bool& drive);

	/**
	 * Gibt einen booleeschen Wert zurück, der bestimmt, ob der angegebene Key gedrückt wurde
	 * @param pWindow Das GLFWWindow Referenzobjekt als Zeiger
	 * @param key Der gemappte Key
	 */
	bool isPressingButton(GLFWwindow* pWindow, Key key);

	/**
	 * Gibt einen booleschen Wert zurück, der bestimmt, ob die angegebene Maustaste geklickt wurde
	 * @param pWindow Das GLFWWindow Referenzobjekt als Zeiger
	 * @param key Der gemappte Key
	 */
	bool isClickingMouseButton(GLFWwindow* pWindow, MouseKey key);

	// 
	// Getter
	//
	const float getMoney() const { return money; }
	//
	// Setter
	//
	void setMoney(const float m) { money = m; }
};

