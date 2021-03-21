//
//  ui.h
//  Stellt das User interface auf Basis der imGUI lib bereit
//
//  @author Paul Dieterich
//

#pragma once

#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "imgui.h"
#include "vector.h"
#include "Matrix.h"
#include "list"
#include "Asteroid.h"
#include "stb_image.h"
#define STB_IMAGE_IMPLEMENTATION

enum notification {
	CARGO_SPACE_LOW = 0,
	AMMO_LOW = 1,
	SHIELD_LOW = 2,
	HEALTH_LOW = 3,
	GAME_OVER = 4
};

class UI {
public: 
	Vector pos;
	float shild;
	float leben ;
	int maxMunition = 100;
	int munition;
	int maxAmmo;
	float treibstoff;
	int credit = 0;
	int kohlenstoff = 0;
	int plutonium = 0;
	int phosphor = 0;
	int eisen = 0;
	int gold = 0;
	int bronze = 0;
	int titan = 0;
	int platin = 0;
	int asteroidenstaub = 0;
	list<Asteroid*> asteroids;
	int tex[16];
	int my_image_width = 16;
	int my_image_height = 16;
	GLuint iconCredit,iconShild, iconShild_full, iconLeben, iconMunition, iconKohlenstoff, iconEisen, iconPhosphor, iconGold, iconBronze, iconPlatin, iconPlutionium, iconTitan,iconStaub = 0;
	/**
	* addIcons
	* add mit hilfe der LoadTextureFormFile funktion alle bilder für das UI, einmalig ausgeführt in der Main beim start des programmes
	*/
	void addIcons();
	/**
	* zur übergabe von Inforamtionen zum anzeigen für das UI
	*/
	void updateUI(const float shild, const float leben, const int munition, const int maxAmmo, const float credit, const float treibstoff,map<Resource*, int> cargo, list<Asteroid*> asteroids);
	void setUICord(Vector v) { pos = v; };
	void PrintUI();
	void PrintMenu();
	void SelectPlay();
	void SelectBeenden();
	void displayNotification(int notification);
	void displayGameOver();
	void setMaxTreibstoff(float t) { maxTreibstoff = t; }
	/** 
	* LoadTextureFromFile 
	* funktion von: https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples#Example-for-OpenGL-users
	* @param filename Der Dateipfad,
	* @param out_texture speicherort in GLuinit format
	* @param out_width  breite des gespeicherten bildes
	* @param out_height höhe des gespeicherten bildes
	*/
	bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);

private: 
	float maxTreibstoff;
};

