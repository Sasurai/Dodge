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

#ifndef PHYSICALOBJECT_H_
#define PHYSICALOBJECT_H_

#include <vector>
#include <Box2D/Box2D.h>

#include "GameObject.h"

using namespace std;

class PhysicalObject : public GameObject
{
public:
	PhysicalObject(GameState *gameState);
	~PhysicalObject();

	b2BodyDef* getBodyDef(){ return &m_bodyDef; }
	void setBodyDef(const b2BodyDef &bodyDef){ m_bodyDef = bodyDef; }
	b2Body* getBody(){ return m_body; }
	void setBody(b2Body *body){ m_body = body; }
	void addFixtureDef(b2FixtureDef *def);
	b2FixtureDef* getFixtureDef(int i);
	int getFixtureDefCount() { return m_fixtureDefs.size(); }
	void removeFixtureDef(int i);

	// Transformations
	virtual const b2Vec2& getPosition() const;
	virtual void setPosition(const b2Vec2 &position);
	virtual float getRotation() const;
	virtual void setRotation(float radians);

	// Rendering
	virtual void draw();

	// Collisions
	virtual void beginContact(b2Contact *contact);
	virtual void endContact(b2Contact *contact);
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

protected:
	b2Body *m_body;   // This body must be allocated by the b2World

private:
	b2BodyDef m_bodyDef;
	vector<b2FixtureDef*> m_fixtureDefs;
};

#endif
