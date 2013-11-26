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

#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <string>

#include "GameJoltAPI/gjAPI.h"
#include "BaseWindow.h"

using namespace std;

class LoginWindow : public BaseWindow
{
public:
	LoginWindow(sf::RenderWindow &window, gjAPI *gjApi);
	~LoginWindow();

	int checkEvents();
	
	void draw();

	void setTextFont(sf::Font &textFont);
	
	bool getLogged(){ return m_logged; }
	string getUsername() { return m_username; }

private:
	gjAPI *m_gjApi;
	sf::Text m_exitText;
	sf::Text m_loginText;
	sf::Text m_usernameText;
	string m_username;
	sf::Text m_tokenText;
	string m_token;
	sf::Text m_wrongLoginText;

	int m_focus; // To know if the focus is on username, token or neither

	bool m_logged; // To know if the user is logged or not

	// Auxiliary method for detecting which key was pressed
	char getKeyPressed(int keyCode);
};

#endif