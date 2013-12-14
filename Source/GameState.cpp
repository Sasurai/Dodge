/* **************************************************************************************** *
	Copyright (c) 2013 Jorge Mart�nez Vargas												*
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
#include <sstream>
#include <SFML/OpenGL.hpp>

#include "GameState.h"
#include "PhysicalObject.h"
#include "GameController.h"
#include "Player.h"


using namespace std;

Camera::Camera():posX(0), posY(0), widthUnits(GAMESIZE), targetX(0), targetY(0), stepSize(10) {}

void Camera::update(double dt){
	float difX = targetX - posX;
	float difY = targetY - posY;
	float dist = sqrtf(difX*difX+difY*difY);
	float angle = difX;
	angle /= dist;
	angle = acosf(angle);
	if(difY < 0.0f){
		angle *= -1;
	}
	if(dist >= stepSize*dt){
		posX += cosf(angle)*stepSize*dist*0.5*dt;
		posY += sinf(angle)*stepSize*dist*0.5*dt;
	}else{
		posX = targetX;
		posY = targetY;
	}
}

GameState::GameState(sf::RenderWindow &window, gjAPI* gjApi) : 
	m_world(b2Vec2(0.0f, -100.0f)),
	m_window(window),			 
	m_accumulator(0.0),
	m_currentTime(0.0),
	m_totalTime(0.0),
	m_lastIntTime(0),
	m_enemiesKilled(0),
	m_nextId(1),
	m_paused(true),
	m_finished(false),
	m_score(0),
	m_nextState(1),
	m_slowTime(1),
	m_muteMusic(false),
	m_gjApi(gjApi)
{
	m_world.SetGravity(b2Vec2(0.0f,0.0f)); // No gravity
	// Camera and graphics initialitzation
	float wHalf = camera.widthUnits/2.0f;
	float hHalf = camera.widthUnits*(float(m_window.getSize().y)/m_window.getSize().x)/2.0f;
	glOrtho(-wHalf, wHalf, 
			-hHalf, hHalf, -100, 100);	
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.1f, 0.3f, 0.3f, 1.0f);

	m_world.SetContactListener(this);

	// Load all the textures we're going to use
	m_textureManager.loadTexture("Textures/Player.png");
	m_textureManager.loadTexture("Textures/PlayerShield.png");
	m_textureManager.loadTexture("Textures/Enemy1.png");
	m_textureManager.loadTexture("Textures/Enemy2.png");
	m_textureManager.loadTexture("Textures/Boom.png");
	m_textureManager.loadTexture("Textures/Points.png");
	m_textureManager.loadTexture("Textures/Shield.png");
	m_textureManager.loadTexture("Textures/Background.png");
	m_textureManager.loadTexture("Textures/Light.png");
	m_textureManager.loadTexture("Textures/Particle.png");
	m_textureManager.loadTexture("Textures/Mine.png");
	m_textureManager.loadTexture("Textures/Fireball.png");
	m_textureManager.loadTexture("Textures/SlowTime.png");
	m_textureManager.loadTexture("Textures/Music.png");
	m_textureManager.loadTexture("Textures/NoMusic.png");
	m_textureManager.loadTexture("Textures/Sound.png");
	m_textureManager.loadTexture("Textures/NoSound.png");
	m_textureManager.loadTexture("Textures/GameJolt.png");

	// Initialize text elements
	m_scoreText.setString("0");
	m_scoreText.setCharacterSize(20);
	m_scoreText.setColor(sf::Color::Green);
	m_scoreText.setPosition(0,0);

	m_menuText.setString("Back to menu");
	m_menuText.setCharacterSize(15);
	m_menuText.setColor(sf::Color::Green);
	m_menuText.setPosition(375,500);

	m_playAgainText.setString("Click the player to play again");
	m_playAgainText.setCharacterSize(17);
	m_playAgainText.setColor(sf::Color::Green);
	m_playAgainText.setPosition(40,400);

	m_scoreEndText.setString("");
	m_scoreEndText.setCharacterSize(22);
	m_scoreEndText.setColor(sf::Color::Green);
	m_scoreEndText.setPosition(50, 100);

	m_survivedText.setString("");
	m_survivedText.setCharacterSize(18);
	m_survivedText.setColor(sf::Color::Green);
	m_survivedText.setPosition(60, 140);

	m_killedText.setString("");
	m_killedText.setCharacterSize(18);
	m_killedText.setColor(sf::Color::Green);
	m_killedText.setPosition(60, 175);

	m_scoreSentText.setCharacterSize(14);
	m_scoreSentText.setPosition(60, 220);
	m_scoreSentText.setColor(sf::Color::Green);

	// Load music and sounds
	m_music.openFromFile("Sounds/Dodge_Level_Music.ogg");
	m_music.setLoop(true);
	m_music.setVolume(100);
	m_endMusic.openFromFile("Sounds/End_Screen_Music.ogg");
	m_endMusic.setLoop(true);
	m_endMusic.setVolume(100);
	m_soundManager.loadSound("Sounds/Explosion1.ogg");
	m_soundManager.loadSound("Sounds/Explosion2.ogg");
	m_soundManager.loadSound("Sounds/Explosion3.ogg");
	m_soundManager.loadSound("Sounds/Points.ogg");
	m_soundManager.loadSound("Sounds/Shield.ogg");
	m_soundManager.loadSound("Sounds/Slow_Time.ogg");
}

GameState::~GameState()
{
}

void GameState::addObject(GameObject* object){
	if(object->hasProperty("physical")){
		PhysicalObject *temp = static_cast<PhysicalObject*>(object);
		b2Body *body = m_world.CreateBody(temp->getBodyDef());
		for(int i = 0; i < temp->getFixtureDefCount(); i++){
			b2FixtureDef *def = temp->getFixtureDef(i);
			body->CreateFixture(def);
		}
		body->SetUserData(temp);
		temp->setBody(body);
	}
	object->setId(m_nextId);
	m_nextId++;
	m_objects.push_back(object);
}

void GameState::setPlayer(Player *player){
	addObject((GameObject*)player);
	m_player = player;
}

void GameState::update(double dt){
	if ( dt > 0.25 )
		dt = 0.25;	  // note: max frame time to avoid spiral of death 
	m_slowTime += dt/5;
	if(m_slowTime>1)
		m_slowTime = 1;

	list<GameObject*>::iterator it = m_objects.begin();
	if(!m_player->isAlive()){ // Player is dead, reset game
		setFinished(true);	
		while(it!=m_objects.end()){ // Delete all the objects
			GameObject *object = (*it);		
			if(object->hasProperty("physical")){
				PhysicalObject *physObject = static_cast<PhysicalObject*>(object);
				m_world.DestroyBody(physObject->getBody());
			}
			delete object;
			it = m_objects.erase(it);
		}
		// Set final score and game stats text
		stringstream ss;
		ss << "YOUR FINAL SCORE IS: " << m_score;
		m_scoreEndText.setString(ss.str());
		stringstream ss1;
		ss1 << "Survived for " << int(m_totalTime) << " seconds";
		m_survivedText.setString(ss1.str());
		stringstream ss2;
		ss2 << "Killed " << m_enemiesKilled << " enemies";
		m_killedText.setString(ss2.str());
		m_gameController->resetGame();
		// Send score using the GameJolt API
		stringstream scorestr;
		scorestr << m_score << " Points";
		if(m_logged){
			m_gjApi->InterScore()->GetPrimaryTable()->AddScoreCall(scorestr.str(),m_score,"","");
		}else{ // Send score as guest
			m_gjApi->InterScore()->GetPrimaryTable()->AddScoreCall(scorestr.str(),m_score,"",m_username);
		}
		return;
	}

	int oldscore = m_score;
	if(!m_paused){
		m_gameController->update(dt);
		m_totalTime += dt;
		if(m_lastIntTime == int(m_totalTime)-1){
			m_lastIntTime++;
			m_score += m_lastIntTime;
		}
	}
	while(it!=m_objects.end()){
		GameObject *object = (*it);
		if(object->isFirstUpdate()){
			object->firstUpdate();
			object->setFirstUpdate(false);
		}
		object->update(dt);
		
		if(!object->isAlive()){ // If a object has been killed we need to take the needed actions and then delete it
			if(object->hasProperty("enemy")){
				if(!object->hasProperty("fireball")){
					m_enemiesKilled++;
					m_score += m_gameController->getInternalLevel();
					b2Vec2 pos = object->getPosition();
					m_gameController->generateParticles(pos);
				}
				if(object->hasProperty("mine")){
					b2Vec2 pos = object->getPosition();
					m_gameController->explodeMine(pos);
				}
			}
			if(object->hasProperty("physical")){
				PhysicalObject *physObject = static_cast<PhysicalObject*>(object);
				m_world.DestroyBody(physObject->getBody());
			}
			delete object;
			it = m_objects.erase(it);
		}else{
			++it;
		}
	}
	if(m_score != oldscore){ // Only update the score text if it has changed
		stringstream ss;
		ss << m_score;
		m_scoreText.setString(ss.str());
	}
	
	camera.update(dt);

	m_accumulator += dt;
	
    while ( m_accumulator >= TIMESTEP ){
        m_world.Step(TIMESTEP, 13, 8); 
        m_accumulator -= TIMESTEP;
    }
}

void GameState::draw(){
	// Draw the scene
	glPushMatrix();
	{
		glTranslatef(-camera.posX, -camera.posY, 0);
		list<GameObject*>::iterator it;
		for(it = m_objects.begin(); it!=m_objects.end(); ++it){
			GameObject *object = (*it);
			object->draw();
			glColor3f(1.0f, 1.0f, 1.0f);
		}
	}
	glPopMatrix();
	// Draw text
	m_window.pushGLStates();
	m_window.draw(m_scoreText);
	if(m_finished){
		m_window.draw(m_menuText);
		m_window.draw(m_playAgainText);
		m_window.draw(m_scoreEndText);
		m_window.draw(m_survivedText);
		m_window.draw(m_killedText);
		m_window.draw(m_scoreSentText);
	}
	m_window.popGLStates();
}

void GameState::BeginContact(b2Contact* contact){
	if(contact->IsEnabled()){
		PhysicalObject *objectA = (PhysicalObject*)contact->GetFixtureA()->GetBody()->GetUserData();
		objectA->beginContact(contact);
		PhysicalObject *objectB = (PhysicalObject*)contact->GetFixtureB()->GetBody()->GetUserData();
		objectB->beginContact(contact);
	}
}

void GameState::EndContact(b2Contact* contact){
	if(contact->IsEnabled()){
		PhysicalObject *objectA = (PhysicalObject*)contact->GetFixtureA()->GetBody()->GetUserData();
		objectA->endContact(contact);
		PhysicalObject *objectB = (PhysicalObject*)contact->GetFixtureB()->GetBody()->GetUserData();
		objectB->endContact(contact);
	}
}

void GameState::PreSolve(b2Contact* contact, const b2Manifold* oldManifold){
	contact->SetEnabled(false);
	PhysicalObject *objectA = (PhysicalObject*)contact->GetFixtureA()->GetBody()->GetUserData();
	objectA->PreSolve(contact, oldManifold);
	PhysicalObject *objectB = (PhysicalObject*)contact->GetFixtureB()->GetBody()->GetUserData();
	objectB->PreSolve(contact, oldManifold);
}

void GameState::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse){
	if(contact->IsEnabled()){
		PhysicalObject *objectA = (PhysicalObject*)contact->GetFixtureA()->GetBody()->GetUserData();
		objectA->PostSolve(contact, impulse);
		PhysicalObject *objectB = (PhysicalObject*)contact->GetFixtureB()->GetBody()->GetUserData();
		objectB->PostSolve(contact, impulse);
	}
}

void GameState::togglePause(){
	if(m_paused){
		setPaused(false);
	}else{
		setPaused(true);
	}	
}

void GameState::setPaused(bool paused){
	m_paused = paused;
	if(m_paused){
		m_window.setMouseCursorVisible(true);
		list<GameObject*>::iterator it = m_objects.begin();
		while(it!=m_objects.end()){
			GameObject *object = (*it);
			object->pause();
			++it;
		}
		m_music.pause();
	}else{
		m_window.setMouseCursorVisible(false); // Cursor is hidden while playing
		list<GameObject*>::iterator it = m_objects.begin();
		while(it!=m_objects.end()){
			GameObject *object = (*it);
			object->unpause();
			++it;
		}
		m_endMusic.stop();
		m_music.play();
	}
	if(m_gameController)
		m_gameController->setPaused(m_paused);
}

void GameState::setGameController(GameController *gameController){
	m_gameController = gameController;
}

sf::Vector2i GameState::getMousePosition(){
	return sf::Mouse::getPosition(m_window);
}

void GameState::setFinished(bool finished){
	if(finished == false){ // Game restarting, reset score and counters
		m_totalTime = 0.0;
		m_lastIntTime = 0;
		m_enemiesKilled = 0;
		m_score = 0;
		stringstream ss;
		ss << m_score;
		m_scoreText.setString(ss.str());
		m_nextState = 1;
	}
	if(finished == true){
		m_music.stop();
		m_endMusic.play();

	}
	m_finished = finished;
}

int GameState::checkEvents(){
	sf::Event event;
    while (m_window.pollEvent(event))
	{
        if(event.type == sf::Event::Closed)
            return -1;		
		if(event.type == sf::Event::LostFocus)
			setPaused(true);
		if(event.type == sf::Event::GainedFocus){
			return 1;
		}
		if(event.type == sf::Event::MouseButtonPressed){
			sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
			if(m_finished){ // Only if the game is finished will the back to menu and play again options show
				if(mousePos.x > 375 && mousePos.x < 575 &&
					mousePos.y > 500 && mousePos.y < 515){
					m_endMusic.stop();
					m_nextState = 0; // Back to menu
				}
				if(mousePos.x > 120 && mousePos.x < 500 &&
					mousePos.y > 260 && mousePos.y < 320){ // Play again
						setFinished(false);
				}
			}
			m_player->mousePressed(mousePos);
		}
    }
	return 0;
}

// Method used by the "boom" powerup
void GameState::killAll(){
	list<GameObject*>::iterator it = m_objects.begin();
	while(it!=m_objects.end()){ // We need to delete all the enemies
		GameObject *object = (*it);		
		if(object->hasProperty("enemy"))
			object->kill();
		++it;
	}
}

// Method used by the "points" powerup
void GameState::increasePoints(){
	m_score += m_gameController->getInternalLevel()*10;
	stringstream ss;
	ss << m_score;
	m_scoreText.setString(ss.str());
}

void GameState::setTextFont(sf::Font &textFont){
	m_scoreText.setFont(textFont);
	m_menuText.setFont(textFont);
	m_playAgainText.setFont(textFont);
	m_scoreEndText.setFont(textFont);
	m_killedText.setFont(textFont);
	m_survivedText.setFont(textFont);
	m_scoreSentText.setFont(textFont);
}

int GameState::getNextState(){
	return m_nextState;
}

// Method used by the "slow-time" powerup
void GameState::setSlowTime(double slowTime){
	m_slowTime = slowTime;
}

void GameState::setMuteSounds(bool muteSounds){
	m_soundManager.setMuteSounds(muteSounds);
}

void GameState::setMuteMusic(bool muteMusic){
	m_muteMusic = muteMusic;
	if(m_muteMusic){
		m_music.setVolume(0);
		m_endMusic.setVolume(0);
	}
	else{
		m_music.setVolume(100);
		m_endMusic.setVolume(100);
	}
}

void GameState::setLogged(bool logged){ 
	m_logged = logged; 
}

void GameState::setUsername(string username){
	m_username = username; 
	stringstream ss;
	ss << "Score saved as " << m_username;
	m_scoreSentText.setString(ss.str());
}
