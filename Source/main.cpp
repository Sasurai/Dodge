/* **************************************************************************************** *
	Copyright (c) 2013 Jorge Martínez Vargas												*
*																							*
*	Permission is hereby granted, free of charge, to any person obtaining a copy			*
*	of this software and associated documentation files (the "Software"), to deal			*
*	in the Software without restriction, including without limitation the rights			*
*	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell				*
*	copies of the Software, and to permit persons to whom the Software is					*
*	furnished to do so, subject to the following conditions:								*
*																							*
*	The above copyright notice and this permission notice shall be included in				*
*	all copies or substantial portions of the Software.										*
*																							*
*	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR				*
*	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,				*
*	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE				*
*	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER					*
*	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,			*
*	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN				*
*	THE SOFTWARE.																			*
*	 ____________________________________________________									*
*	| This file is part of the "Dodge!" game source code.|									*
*	|	gamejolt.com/games/other/dodge/18599/			 |									*
*	 ----------------------------------------------------									*
*																							*
* ***************************************************************************************** */

#include <iostream>
#include <SFML/OpenGL.hpp>
#include <SFML/Audio.hpp>
#include "GameJoltAPI/gjAPI.h"

#include "MenuWindow.h"
#include "InstructionsWindow.h"
#include "LoginWindow.h"
#include "Background.h"
#include "GameState.h"
#include "GameController.h"
#include "PhysicalObject.h"

using namespace std;

int main()
{
	// Dummy call, you need to put the actual info. of the game for the API to work
	gjAPI gjApi(00000,"PrivateKey");
	glEnable(GL_TEXTURE_2D);
	glClearColor(0.1f,0.3f,0.3f,1.0f);
	sf::RenderWindow window(sf::VideoMode(WINDOWSIZE, WINDOWSIZE), "Dodge!");
	MenuWindow menuWindow(window);
	InstructionsWindow instructionsWindow(window);
	LoginWindow loginWindow(window,&gjApi);
	GameState gameState(window,&gjApi);
	GameController gameController(&gameState);
	gameState.setGameController(&gameController);
	Background background(&gameState);

	sf::Font textFont;
	textFont.loadFromFile("Fonts/sofachri.ttf");
	gameState.setTextFont(textFont);
	menuWindow.setTextFont(textFont);
	loginWindow.setTextFont(textFont);
	
	// Load textures needed for the menu and instructions
	gameState.getTextureManager().loadTexture("Textures/Instructions.png");
	instructionsWindow.setImage(gameState.getTextureManager().getTexture("Textures/Instructions.png"));
	menuWindow.setMusicIcon(gameState.getTextureManager().getTexture("Textures/Music.png"));
	menuWindow.setNoMusicIcon(gameState.getTextureManager().getTexture("Textures/NoMusic.png"));
	menuWindow.setSoundIcon(gameState.getTextureManager().getTexture("Textures/Sound.png"));
	menuWindow.setNoSoundIcon(gameState.getTextureManager().getTexture("Textures/NoSound.png"));
	menuWindow.setLoginIcon(gameState.getTextureManager().getTexture("Textures/GameJolt.png"));

	sf::Clock clock;
	window.setVerticalSyncEnabled(true);
	
	sf::Time time = clock.restart();
	gameState.update(time.asSeconds());
	int state = 0; // Menu

	while (window.isOpen())
	{	
	sf::Time time = clock.restart();
	int ev;
	gjApi.Update();
		switch(state){
		case 5: // Login window
			ev = loginWindow.checkEvents();
			if(ev == -1) // Close window event
				window.close();
			window.clear(sf::Color(0, 0, 0));
			background.draw();
			loginWindow.draw();
			window.display();
			state = loginWindow.getNextState();
			break;
		case 0: // Menu
			ev = menuWindow.checkEvents();
			if(ev == -1) // Close window event
				window.close();
			window.clear(sf::Color(0, 0, 0));
			background.draw();
			menuWindow.draw();
			window.display();
			state = menuWindow.getNextState();
			break;
		case 4: // Transition from menu to playing
			window.clear(sf::Color(0, 0, 0));
			gameState.setFinished(false);
			gameState.setMuteMusic(menuWindow.getMuteMusic());
			gameState.setMuteSounds(menuWindow.getMuteSounds());
			gameState.setLogged(loginWindow.getLogged());
			gameState.setUsername(loginWindow.getUsername());
			background.draw();
			gameState.draw();	
			window.display();
		case 1: // Playing
			ev = gameState.checkEvents();
			if(ev == -1) // Close window event
				window.close();
			if(ev == 1){ // Focus restored
				window.clear(sf::Color(0, 0, 0));
				background.draw();
				gameState.draw();	
				window.display();
			}
			gameState.update(time.asSeconds());
			if(!gameState.getPaused()){
				window.clear(sf::Color(0, 0, 0));
				background.draw();
				gameState.draw();	
				window.display();
				if(gameState.getFinished()){
					gameState.setPaused(true);
				}
			}
			state = gameState.getNextState();
			break;
		case 2: // Instructions
			ev = instructionsWindow.checkEvents();
			if(ev == -1) // Close window event
				window.close();
			window.clear(sf::Color(0, 0, 0));
			instructionsWindow.draw();
			window.display();
			state = instructionsWindow.getNextState();
			break;
		case 3: // Credits
			// I need to make a credits screen before release
			break;
		}
	}
	return 0;
}