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

#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include <list>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Audio.hpp>
#include <Box2D/Box2D.h>

#include "GameJoltAPI/gjAPI.h"
#include "TextureManager.h"
#include "SoundManager.h"

// Constants needed for window/game position conversions
#define WINDOWSIZE 600
#define GAMESIZE 40

class GameObject;
class GameController;
class Player;

using namespace std;

const double TIMESTEP = 0.001;

// We used a fixed camera in this game, but the code is prepared to have the camera move around
struct Camera {
	float posX;
	float posY;
	float widthUnits;
	float targetX;
	float targetY;
	float stepSize;

	Camera();

	void update(double dt);

};

// Is has methods in common with BaseWindow but needs to inherit from b2ContactListener for the physics to work
class GameState  : public b2ContactListener
{
public:
	GameState(sf::RenderWindow &window, gjAPI *gjApi);
	~GameState();

	// Objects handling
	void addObject(GameObject* object);
	void setPlayer(Player* player);
	Player* getPlayer(){ return m_player; }

	// Update
	void update(double dt);

	// Game state
	void togglePause();
	bool getPaused(){ return m_paused;}
	void setPaused(bool paused);
	bool getFinished(){ return m_finished;}
	void setFinished(bool finished);
	int getNextState();
	double getSlowTime() { return m_slowTime; }
	void setSlowTime(double slowTime);
	void killAll();
	void increasePoints();
	
	// Collisions
	virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

	TextureManager& getTextureManager(){ return m_textureManager;}
	SoundManager& getSoundManager(){ return m_soundManager; }
	
	void setGameController(GameController *gameController);
	GameController* getGameController(){ return m_gameController; }
	
	int checkEvents();
	
	sf::Vector2i getMousePosition();

	void setTextFont(sf::Font &textFont);

	// Sound and music options
	void setMuteSounds(bool muteSounds);
	void setMuteMusic(bool muteMusic);

	// Rendering
	Camera camera;
	void draw();

	// User information for highscores
	void setLogged(bool logged);
	void setUsername(string username);

private:
	// Object handling
	list<GameObject*> m_objects;
	Player *m_player; // We save a pointer outside the objects list for quick access, but it's also in the objects list
	unsigned int m_nextId;

	b2World m_world;

	TextureManager m_textureManager;
	GameController *m_gameController;
	sf::RenderWindow &m_window;

	// GameJolt API
	gjAPI *m_gjApi;

	// Player info
	bool m_logged;
	string m_username;

	// Time handling
	double m_currentTime;
	int m_lastIntTime;
	double m_accumulator;

	// Game music and sounds
	bool m_muteMusic;
	sf::Music m_music;
	sf::Music m_endMusic;
	SoundManager m_soundManager;
	

	// Scores and stats
	double m_totalTime;
	int m_enemiesKilled;
	int m_score;

	// Game state
	bool m_paused;
	bool m_finished;
	int m_nextState;
	double m_slowTime;
	int m_mineCount;

	// Drawable text
	sf::Text m_scoreText;
	sf::Text m_scoreEndText;
	sf::Text m_survivedText;
	sf::Text m_killedText;
	sf::Text m_playAgainText;
	sf::Text m_menuText;
	sf::Text m_scoreSentText;	
	/*
		Drawable text could be handled more ellegantly with a map<string,sf::Text*), instead of having a bunch of atributes and 
		having to change code if we add or remove one, but since there is little text we left it as it is.
	*/
};

#endif