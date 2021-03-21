#include "Player.h"
#include "Ship.h"

bool Player::playerInput(GLFWwindow* pWindow, int& roll, int& forward) {
	roll = 0; forward = 0;
	
	bool LeftRollPressed = glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS;
	bool RightRollPressed = glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS;
	bool ForwardPressed = glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS;
	bool BackwardPressed = glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS;
	
	if (!LeftRollPressed && !RightRollPressed && !ForwardPressed && !BackwardPressed) return false;

	//if (UpPressed) up = 1;
	//else if (DownPressed) up = -1;

	//if (LeftPressed) turn = 1;
	//else if(RightPressed) turn = -1;

	if (LeftRollPressed) roll = 1;
	else if (RightRollPressed) roll = -1;

	if (ForwardPressed) forward = 1;
	else if (BackwardPressed) forward = -1;

	return true;
}

void Player::cursorMovement(GLFWwindow* pWindow, double& x, double& y)
{
	glfwGetCursorPos(pWindow, &x, &y);
}


// Hey, habe die so angepasst, dass eine identitätsmatrix zurückgegeben wird, wenn kein Input erfolgt ist, sodass wir das abfangen können!
// ~ Justin
Matrix Player::playerMoveMatrix(GLFWwindow* pWindow, float dtime, float shipSpeed, bool& drive) {
	
	Matrix moveMatrix, upM, rollM, turnM, forwardM;
	int roll, forward;
	double up, turn;
	bool keyInput = playerInput(pWindow, roll, forward);

	if (!keyInput) drive = false; // Dann verbrauchen wir keinen Treibstoff, wenn nur die Maus bewegt wird :)
	else {
		forwardM.identity();
		rollM.identity();
	}

	// Mauskoordinaten aus dem Frame hier nehmen und mit denen des vorherigen vergleichen, darüber 
	// die up und turn Werte setzen und in Relation zur deltatime setzen.
	// Wichtig: X achse ist hier invertiert, darum müssen wir den Wert auch invertierenssss
	cursorMovement(pWindow, turn, up); // TODO: Ich glaube hier wird eigentlich int akzeptiert

	// Maus input transformation
	// Normale Koordinaten wären zu groß -> normalisieren relativ zur WindowSize (wurde in der Main festgelegt und ist die halbe größe)
	int width, height;
	glfwGetWindowSize(pWindow, &width, &height);
	upM.rotationX(-(height/2 - up)/height * dtime);
	turnM.rotationY((width/2 - turn)/width * dtime);

	// Tastatur input transformation
	rollM.rotationZ(roll * dtime);
	forwardM.translation(0, 0, forward * 4 * dtime * shipSpeed);
	
	moveMatrix = upM * rollM * turnM * forwardM;

	return moveMatrix;

}

bool Player::isPressingButton(GLFWwindow* pWindow, Key glfwKey)
{
	return glfwGetKey(pWindow, glfwKey) == GLFW_PRESS;
}

bool Player::isClickingMouseButton(GLFWwindow* pWindow, MouseKey key)
{
	return glfwGetMouseButton(pWindow, key) == GLFW_PRESS;
}

