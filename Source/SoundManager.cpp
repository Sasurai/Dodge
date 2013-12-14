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

#include <string>

#include "SoundManager.h"

using namespace std;

SoundObj::SoundObj(sf::SoundBuffer &buffer, string &sname){
	m_sound.setBuffer(buffer);
	m_sound.setVolume(80);
	m_sound.setLoop(false);
	name = sname;
}

SoundObj::SoundObj(const SoundObj &other) : m_sound(other.m_sound), name(other.name){
}

bool SoundObj::isUsed(){
	if(m_sound.getStatus() == sf::SoundSource::Stopped)
		return false;
	else
		return true;
}

// SoundManager

SoundManager::SoundManager(){
}

SoundManager::~SoundManager(){
	clear();
}

void SoundManager::playSound(string &soundName){
	if(m_muteSounds)
		return;

	bool played = false;
	vector<SoundObj>::iterator it; // Search for an available sound object with the required sound effect
	for(it=m_sounds.begin();it!=m_sounds.end();++it){
		if(it->name == soundName && !(it->isUsed())){		
			it->m_sound.play();
			played = true;
		}
	}
	if(!played){ // No sound object available, we need to create a new one
		sf::SoundBuffer *buff = 0;
		if(m_soundBuffers.find(soundName) != m_soundBuffers.end()){ // Search for the sound buffer of the desired effect
			buff = m_soundBuffers.find(soundName)->second;
			m_sounds.push_back(SoundObj(*buff,soundName));
			m_sounds.back().m_sound.play();			
		}
	}
}

// Note that loadSound only loads the SoundBuffer of the SFX, without creating any instance (SoundObj) of it.
bool SoundManager::loadSound(const string &soundName){
	sf::SoundBuffer *buffer;
	buffer = new sf::SoundBuffer();
	if(!buffer->loadFromFile(soundName)){
		delete buffer;
		return false;
	}
	// If there's a loaded buffer with the same name we replace it
	if(m_soundBuffers.find(soundName) != m_soundBuffers.end()){
		delete m_soundBuffers[soundName];
		// If there's a sound with the old buffer we delete it. This may interrupt the sound but avoids crashes because of null pointers
		vector<SoundObj>::iterator it;
		for(it=m_sounds.begin();it!=m_sounds.end();++it){
			if(it->name == soundName){
				m_sounds.erase(it);
			}
		}
		m_sounds.clear();
	}
	m_soundBuffers[soundName] = buffer;
	return true;
}

void SoundManager::clear(){
	m_sounds.clear();
	map<string, sf::SoundBuffer*>::iterator it;
	for(it = m_soundBuffers.begin(); it!=m_soundBuffers.end(); ++it){
		delete it->second;
	}
	m_soundBuffers.clear();
}