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

#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include <set>
#include <string>
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "GameState.h"

using namespace std;

class GameObject
{
public:
	GameObject(GameState *gameState);
	~GameObject();

	// Id handling
	unsigned int getId() const { return m_id; }
	void setId(unsigned int id) { m_id = id; }

	// Properties (those are used to identify objects as enemies, players, etc)
	bool hasProperty(const string &name) const;
	void addProperty(const string &name);
	void deleteProperty(const string &name);

	// GameState
	void setGameState(GameState *state) { m_gameState = state; }

	// Update
	virtual void update(double dt) { return; }
	virtual void firstUpdate() { return; }
	bool isFirstUpdate() { return m_firstUpdate; }
	void setFirstUpdate(bool value) { m_firstUpdate = value;}
	virtual void pause() { return; }
	virtual void unpause() { return; }

	// Transformations
	virtual const b2Vec2& getPosition() const;
	virtual void setPosition(const b2Vec2 &position);
	virtual float getRotation() const;
	virtual void setRotation(float radians);
	virtual const b2Vec2& getSize() const;
	virtual void setSize(const b2Vec2 &size);

	// State
	bool isAlive(){ return m_alive; }
	bool isActive(){ return m_active; }
	virtual void kill() { m_alive = false; }

	// Rendering
	bool isVisible(){ return m_visible; }
	void setVisible(bool visible){ m_visible = visible; }
	sf::Texture* getTexture();
	void setTexture(sf::Texture &texture); // By passing as a reference we avoid null pointers
	const float* getColor() const;
	void setColor(float r, float g, float b, float a = 1.0f);
	virtual void draw();

protected:
	GameState *m_gameState;
	b2Vec2 m_position;
	float m_rotation; // radians
	b2Vec2 m_size;
	bool m_alive;
	bool m_active;
	bool m_visible;
	sf::Texture *m_texture;
	float m_color[4];
	bool m_firstUpdate;

	/* Protected method to draw the current texture, we made this method to avoid repeating code in every subclass, 
		since they do some stuff to select the texture to be used, but then draw it the same exact way.
	*/
	void m_drawCurrentTexture();

private:
	unsigned int m_id;
	set<string> m_properties;
};

#endif