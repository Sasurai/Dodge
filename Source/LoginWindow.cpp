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

#include "LoginWindow.h"

LoginWindow::LoginWindow(sf::RenderWindow &window, gjAPI *gjApi) : BaseWindow(window, 2), m_gjApi(gjApi), m_focus(0), m_logged(false)
{
	// Initialize text elements
  m_usernameText.setString("Username");
	m_username = "Guest";
	m_usernameText.setCharacterSize(20);
	m_usernameText.setColor(sf::Color::Black);
	m_usernameText.setPosition(150,180);

	m_tokenText.setString("guest");
	m_token = "GUEST";
	m_tokenText.setCharacterSize(20);
	m_tokenText.setColor(sf::Color::Black);
	m_tokenText.setPosition(150,255);

	m_exitText.setString("Back");
	m_exitText.setCharacterSize(30);
	m_exitText.setColor(sf::Color::Green);
	m_exitText.setPosition(100,420);

	m_loginText.setString("Login");
	m_loginText.setCharacterSize(30);
	m_loginText.setColor(sf::Color::Green);
	m_loginText.setPosition(350,420);

	m_wrongLoginText.setString("Wrong login information");
	m_wrongLoginText.setCharacterSize(20);
	m_wrongLoginText.setColor(sf::Color::Transparent);
	m_wrongLoginText.setPosition(50, 350);
}

LoginWindow::~LoginWindow()
{
}

int LoginWindow::checkEvents(){
  sf::Event event;
  m_nextState = 5;
    while (m_window.pollEvent(event))
	{
        if(event.type == sf::Event::Closed)
            return -1;		
		if(event.type == sf::Event::MouseButtonPressed){
			sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
			if(mousePos.x > 145 && mousePos.x < 450 &&
				mousePos.y > 170 && mousePos.y < 215){ // Username focus
				m_usernameText.setString("");
				m_username = "";
				m_wrongLoginText.setColor(sf::Color::Transparent);
				m_focus = 1;
			}
			if(mousePos.x > 145 && mousePos.x < 450 &&
				mousePos.y > 245 && mousePos.y < 290){ // Token focus
				m_tokenText.setString("");
				m_token = "";
				m_wrongLoginText.setColor(sf::Color::Transparent);
				m_focus = 2;
			}
			if(mousePos.x > 100 && mousePos.x < 250 &&
				mousePos.y > 420 && mousePos.y < 450){ // Back to menu
				m_wrongLoginText.setColor(sf::Color::Transparent);
				m_nextState = 0;
			}
			if(mousePos.x > 350 && mousePos.x < 510 &&
				mousePos.y > 420 && mousePos.y < 450){ // Attempt login
					m_focus = 0;
					if(m_token.compare("GUEST") == 0){ 
						// "Login" as guest allows to customize the name for submiting a score without having a GameJolt account
						m_logged = false;
						m_wrongLoginText.setColor(sf::Color::Transparent);
						m_nextState = 0;
					}else if(m_gjApi->Login(true,m_username,m_token) != GJ_OK){
						m_logged = false;
						m_wrongLoginText.setColor(sf::Color::Red);
					}else{
						m_logged = true;
						m_wrongLoginText.setColor(sf::Color::Transparent);
						m_nextState = 0;
					}
			}
		}
		if(event.type == sf::Event::KeyPressed){
			// alphanumeric, - and _, not case sensitive
			char pressedKey = getKeyPressed(event.key.code);
			if(pressedKey != '*'){ // Valid key pressed
				if(m_focus == 1){ // Username
					m_username += pressedKey;
					m_usernameText.setString(m_username);
				}
				if(m_focus == 2){ // Token
					m_token += pressedKey;
					m_tokenText.setString(m_token);
				}
			}
		}	
    }
	return 0;
}

void LoginWindow::draw(){
	glPushMatrix();
	// Draw rectangle for username
	{
		glTranslatef(0.0,7.0,0);
		glBegin(GL_QUADS);
		glColor4f(1,1,1,1);
		{
			glVertex3f( -10.0f, -1.5f,  0.0f);
			glVertex3f( 10.0f, -1.5f,  0.0f);
			glVertex3f( 10.0f, 1.5f,  0.0f);
			glVertex3f( -10.0f, 1.5f,  0.0f);
		}
		glEnd();
	}
	glPopMatrix();
	// Draw rectangle for token
	glPushMatrix();
	{
		glTranslatef(0.0,2.0,0);
		glBegin(GL_QUADS);
		glColor4f(1,1,1,1);
		{
			glVertex3f( -10.0f, -1.5f,  0.0f);
			glVertex3f( 10.0f, -1.5f,  0.0f);
			glVertex3f( 10.0f, 1.5f,  0.0f);
			glVertex3f( -10.0f, 1.5f,  0.0f);
		}
		glEnd();
	}
	glPopMatrix();

	m_window.pushGLStates();
	m_window.draw(m_usernameText);
	m_window.draw(m_tokenText);
	m_window.draw(m_loginText);
	m_window.draw(m_exitText);
	m_window.draw(m_wrongLoginText);
	m_window.popGLStates();
}

void LoginWindow::setTextFont(sf::Font &textFont){
	m_usernameText.setFont(textFont);
	m_tokenText.setFont(textFont);
	m_loginText.setFont(textFont);
	m_exitText.setFont(textFont);
	m_wrongLoginText.setFont(textFont);
}

char LoginWindow::getKeyPressed(int keyCode){
	switch(keyCode){
	case sf::Keyboard::A:
		return 'A';
	case sf::Keyboard::B:
		return 'B';
	case sf::Keyboard::C:
		return 'C';
	case sf::Keyboard::D:
		return 'D';
	case sf::Keyboard::E:
		return 'E';
	case sf::Keyboard::F:
		return 'F';
	case sf::Keyboard::G:
		return 'G';
	case sf::Keyboard::H:
		return 'H';
	case sf::Keyboard::I:
		return 'I';
	case sf::Keyboard::J:
		return 'J';
	case sf::Keyboard::K:
		return 'K';
	case sf::Keyboard::L:
		return 'L';
	case sf::Keyboard::M:
		return 'M';
	case sf::Keyboard::N:
		return 'N';
	case sf::Keyboard::O:
		return 'O';
	case sf::Keyboard::P:
		return 'P';
	case sf::Keyboard::Q:
		return 'Q';
	case sf::Keyboard::R:
		return 'R';
	case sf::Keyboard::S:
		return 'S';
	case sf::Keyboard::T:
		return 'T';
	case sf::Keyboard::U:
		return 'U';
	case sf::Keyboard::V:
		return 'V';
	case sf::Keyboard::W:
		return 'W';
	case sf::Keyboard::X:
		return 'X';
	case sf::Keyboard::Y:
		return 'Y';
	case sf::Keyboard::Z:
		return 'Z';

	case sf::Keyboard::Num0:
	case sf::Keyboard::Numpad0:
		return '0';
	case sf::Keyboard::Num1:
	case sf::Keyboard::Numpad1:
		return '1';
	case sf::Keyboard::Num2:
	case sf::Keyboard::Numpad2:
		return '2';
	case sf::Keyboard::Num3:
	case sf::Keyboard::Numpad3:
		return '3';
	case sf::Keyboard::Num4:
	case sf::Keyboard::Numpad4:
		return '4';
	case sf::Keyboard::Num5:
	case sf::Keyboard::Numpad5:
		return '5';
	case sf::Keyboard::Num6:
	case sf::Keyboard::Numpad6:
		return '6';
	case sf::Keyboard::Num7:
	case sf::Keyboard::Numpad7:
		return '7';
	case sf::Keyboard::Num8:
	case sf::Keyboard::Numpad8:
		return '8';
	case sf::Keyboard::Num9:
	case sf::Keyboard::Numpad9:
		return '9';

	case sf::Keyboard::Up: // Hyphen
		return '-';
	case sf::Keyboard::Down: // Underscore
		return '_';

	default: // Not supported key
		return '*'; 
	}
}