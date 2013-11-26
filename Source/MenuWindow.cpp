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

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "MenuWindow.h"

MenuWindow::MenuWindow(sf::RenderWindow &window) : BaseWindow(window, 0), m_playingMusic(false),
	m_muteMusic(false), m_muteSounds(false)
{
	// Initialize text
	m_playText.setString("Play");
	m_playText.setCharacterSize(30);
	m_playText.setColor(sf::Color::Green);
	m_playText.setPosition(200,120);

	m_instructionsText.setString("Instructions");
	m_instructionsText.setCharacterSize(30);
	m_instructionsText.setColor(sf::Color::Green);
	m_instructionsText.setPosition(120,270);

	m_exitText.setString("Exit");
	m_exitText.setCharacterSize(30);
	m_exitText.setColor(sf::Color::Green);
	m_exitText.setPosition(200,420);

	// Load menu music
	m_music.openFromFile("Sounds/Dodge_Menu_Music.ogg");
	m_music.setLoop(true);
	m_music.setVolume(100);
}


MenuWindow::~MenuWindow()
{
}

void MenuWindow::setTextFont(sf::Font &textFont){
	m_playText.setFont(textFont);
	m_instructionsText.setFont(textFont);
	m_exitText.setFont(textFont);
}

void MenuWindow::draw(){
	// Draw music and sound icons
	if(!m_muteMusic){
		if(m_musicTexture)
			sf::Texture::bind(m_musicTexture);
	}else if(m_noMusicTexture)
		sf::Texture::bind(m_noMusicTexture);
	glPushMatrix();
	{
		glTranslatef(16.0,18.0,0);
		glScalef(2, 2, 0.0f);
		glBegin(GL_QUADS);
		{
			glTexCoord2d(0.0f,1.0f);
			glVertex3f( -0.5f, -0.5f,  0.0f);
			glTexCoord2d(1.0f,1.0f);
			glVertex3f( 0.5f, -0.5f,  0.0f);
			glTexCoord2d(1.0f,0.0f);
			glVertex3f( 0.5f, 0.5f,  0.0f);
			glTexCoord2d(0.0f,0.0f);
			glVertex3f( -0.5f, 0.5f,  0.0f);
		}
		glEnd();
	}
	glPopMatrix();
	sf::Texture::bind(NULL);
	if(!m_muteSounds){
		if(m_soundTexture)
			sf::Texture::bind(m_soundTexture);
	}else if(m_noSoundTexture)
		sf::Texture::bind(m_noSoundTexture);
	glPushMatrix();
	{
		glTranslatef(18.0,18.0,0);
		glScalef(2, 2, 0.0f);
		glBegin(GL_QUADS);
		{
			glTexCoord2d(0.0f,1.0f);
			glVertex3f( -0.5f, -0.5f,  0.0f);
			glTexCoord2d(1.0f,1.0f);
			glVertex3f( 0.5f, -0.5f,  0.0f);
			glTexCoord2d(1.0f,0.0f);
			glVertex3f( 0.5f, 0.5f,  0.0f);
			glTexCoord2d(0.0f,0.0f);
			glVertex3f( -0.5f, 0.5f,  0.0f);
		}
		glEnd();
	}
	glPopMatrix();
	sf::Texture::bind(NULL);
	// Draw GameJolt icon
	if(m_loginTexture)
		sf::Texture::bind(m_loginTexture);
	glPushMatrix();
	{
		glTranslatef(16.0,-16.0,0);
		glScalef(4, 4, 0.0f);
		glBegin(GL_QUADS);
		{
			glTexCoord2d(0.0f,1.0f);
			glVertex3f( -0.5f, -0.5f,  0.0f);
			glTexCoord2d(1.0f,1.0f);
			glVertex3f( 0.5f, -0.5f,  0.0f);
			glTexCoord2d(1.0f,0.0f);
			glVertex3f( 0.5f, 0.5f,  0.0f);
			glTexCoord2d(0.0f,0.0f);
			glVertex3f( -0.5f, 0.5f,  0.0f);
		}
		glEnd();
	}
	glPopMatrix();
	sf::Texture::bind(NULL);

	// Draw text
	m_window.pushGLStates();
	m_window.draw(m_playText);
	m_window.draw(m_instructionsText);
	m_window.draw(m_exitText);
	m_window.popGLStates();
}

int MenuWindow::checkEvents(){
	if(!m_playingMusic){
		m_music.play();
		m_playingMusic = true;
	}
	sf::Event event;
	m_nextState = 0;
    while (m_window.pollEvent(event))
	{
        if(event.type == sf::Event::Closed)
            return -1;				
		if(event.type == sf::Event::MouseButtonPressed){
			sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
			if(mousePos.x > 200 && mousePos.x < 350 &&
				mousePos.y > 120 && mousePos.y < 150){
				m_music.pause();
				m_playingMusic = false;
				m_nextState = 4; // Go to playing
			}
			if(mousePos.x > 120 && mousePos.x < 500 &&
				mousePos.y > 270 && mousePos.y < 300)
				m_nextState = 2; // Instructions
			if(mousePos.x > 200 && mousePos.x < 350 &&
				mousePos.y > 420 && mousePos.y < 450)
				m_window.close();
			if(mousePos.x > 525 && mousePos.x < 555 &&
				mousePos.y > 15 && mousePos.y < 45){ // Toggle music
				if(m_muteMusic){
					m_muteMusic = false;
					m_music.setVolume(100);
				}else{
					m_muteMusic = true;
					m_music.setVolume(0);
				}
			}
			if(mousePos.x > 555 && mousePos.x < 585 &&
				mousePos.y > 15 && mousePos.y < 45) // Toggle sound
				m_muteSounds = !m_muteSounds;
			if(mousePos.x > 510 && mousePos.x < 570 &&
				mousePos.y > 510 && mousePos.y < 570) // Go to login window
				m_nextState = 5;
		}
			
    }
	return 0;
}

void MenuWindow::setSoundIcon(sf::Texture *texture){
	m_soundTexture = texture;
}

void MenuWindow::setNoSoundIcon(sf::Texture *texture){
	m_noSoundTexture = texture;
}

void MenuWindow::setMusicIcon(sf::Texture *texture){
	m_musicTexture = texture;
}

void MenuWindow::setNoMusicIcon(sf::Texture *texture){
	m_noMusicTexture = texture;
}

void MenuWindow::setLoginIcon(sf::Texture *texture){
	m_loginTexture = texture;
}