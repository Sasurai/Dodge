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

#include "GameObject.h"

GameObject::GameObject(GameState *gameState) : m_gameState(gameState),
						   m_position(0.0f, 0.0f),
						   m_rotation(0.0f),
						   m_size(1.0f, 1.0f),
						   m_visible(true),
						   m_alive(true),
						   m_active(true),
						   m_texture(NULL),
						   m_firstUpdate(true),
						   m_id(0)
{
	setColor(1.0f, 0.2f, 0.2f, 1.0f);
}

GameObject::~GameObject(){}

bool GameObject::hasProperty(const string &name) const{
	return m_properties.count(name);
}

void GameObject::addProperty(const string &name){
	m_properties.insert(name);
}

void GameObject::deleteProperty(const string &name){
	m_properties.erase(m_properties.find(name));
}

const b2Vec2& GameObject::getPosition() const{ 
	return m_position; 
}

void GameObject::setPosition(const b2Vec2 &position){ 
	m_position = position;
}

float GameObject::getRotation() const{ 
	return m_rotation; 
}

void GameObject::setRotation(float radians){
	m_rotation = radians;
}

const b2Vec2& GameObject::getSize() const{ 
	return m_size;
}

void GameObject::setSize(const b2Vec2 &size){
	m_size = size;
}

sf::Texture* GameObject::getTexture(){
	return m_texture;
}

void GameObject::setTexture(sf::Texture &texture){ 
	m_texture = &texture;
}

const float* GameObject::getColor() const{
	return m_color;
}

void GameObject::setColor(float r, float g, float b, float a){
	m_color[0] = r;
	m_color[1] = g;
	m_color[2] = b;
	m_color[3] = a;
}

void GameObject::draw(){
	if(!m_visible)
		return;
	if(m_texture)
		sf::Texture::bind(m_texture);
	else
		glColor4fv(m_color);
	m_drawCurrentTexture();
	sf::Texture::bind(NULL);
}

void GameObject::m_drawCurrentTexture(){
	b2Vec2 position = getPosition();
	float rotation = getRotation();
	b2Vec2 size = getSize();
	glPushMatrix();
	{
		glTranslatef(position.x,position.y,0);
		glRotatef(rotation,0,0,1);
		glScalef(size.x, size.y, 0.0f);
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
}