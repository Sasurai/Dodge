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

#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include "SFML\Graphics\RenderWindow.hpp"
#include "SFML\Graphics\Font.hpp"
#include "SFML\Graphics\Text.hpp"
#include "SFML\Audio.hpp"

#include "BaseWindow.h"

class MenuWindow : public BaseWindow
{
public:
	MenuWindow(sf::RenderWindow &window);
	~MenuWindow();

	int checkEvents();

	void draw();

	void setTextFont(sf::Font &textFont);

	void setSoundIcon(sf::Texture *texture);
	void setNoSoundIcon(sf::Texture *texture);
	void setMusicIcon(sf::Texture *texture);
	void setNoMusicIcon(sf::Texture *texture);
	void setLoginIcon(sf::Texture *texture);

	bool getMuteSounds(){ return m_muteSounds; }
	bool getMuteMusic(){ return m_muteMusic; }

private:
	sf::Text m_exitText;
	sf::Text m_playText;
	sf::Text m_instructionsText;

	sf::Music m_music;
	
	bool m_playingMusic;
	bool m_muteSounds;
	bool m_muteMusic;

	sf::Texture *m_soundTexture;
	sf::Texture *m_noSoundTexture;
	sf::Texture *m_musicTexture;
	sf::Texture *m_noMusicTexture;
	sf::Texture *m_loginTexture;
};

#endif