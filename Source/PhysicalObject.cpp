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

#include "PhysicalObject.h"

PhysicalObject::PhysicalObject(GameState *gameState) : GameObject(gameState), m_body(NULL){
	addProperty("physical");
}

PhysicalObject::~PhysicalObject(){
	for(int i = 0; i < m_fixtureDefs.size(); i++){
		delete m_fixtureDefs[i];
	}
	m_fixtureDefs.clear();
}

void PhysicalObject::addFixtureDef(b2FixtureDef *def){
	m_fixtureDefs.push_back(def);
}

b2FixtureDef* PhysicalObject::getFixtureDef(int i){
	return m_fixtureDefs[i];
}

void PhysicalObject::removeFixtureDef(int i){
	m_fixtureDefs.erase(m_fixtureDefs.begin() + i);
}

const b2Vec2& PhysicalObject::getPosition() const{ 
	if(m_body){
		return m_body->GetPosition();
	}else{
		return m_bodyDef.position;
	}
}

void PhysicalObject::setPosition(const b2Vec2 &position){ 
	if(m_body){
		float angle = m_body->GetAngle();
		m_body->SetTransform(position, angle);
	}else{
		m_bodyDef.position = position;
	}
}

float PhysicalObject::getRotation() const{ 
	if(m_body){
		return m_body->GetAngle();
	}else{
		return m_bodyDef.angle;
	}
}

void PhysicalObject::setRotation(float radians){
	if(m_body){
		b2Vec2 position = m_body->GetPosition();
		m_body->SetTransform(position, radians);
	}else{
		m_bodyDef.angle = radians;
	}
}

void PhysicalObject::draw(){
	if(!m_visible)
		return;
	if(!m_body)
		return;
	if(m_texture)
		sf::Texture::bind(m_texture);
	else
		glColor4fv(m_color);
	b2Vec2 position = getPosition();
	float rotation = getRotation();
	b2Vec2 size = getSize();
	glPushMatrix();
	{
		glTranslatef(position.x,position.y,0);
		glRotatef(rotation*57.2957795,0,0,1);
		glScalef(size.x, size.y, 0.0f);

		for(b2Fixture *f = m_body->GetFixtureList(); f; f = f->GetNext()){	
			b2Shape *shape = f->GetShape();
			if(shape->GetType() == b2Shape::Type::e_polygon){
				b2PolygonShape *polShape = static_cast<b2PolygonShape*>(shape);
				glBegin(GL_POLYGON);
				{
					for(int v = 0; v < polShape->GetVertexCount(); v++){
						const b2Vec2 &vertex = polShape->GetVertex(v);
						glVertex2f(vertex.x, vertex.y);
					}
				}
				glEnd();
			}else if(shape->GetType() == b2Shape::Type::e_circle){
				float radius = shape->m_radius;
				glBegin(GL_TRIANGLE_FAN);
				{
					glVertex2f(0, 0);
					for(int angle = 0; angle <= 360; angle+=5){
						glVertex2f(sin(float(angle)) * radius, cos(float(angle))*radius);
 
					}
				}
				glEnd();
			}
		}
	}
	glPopMatrix();
	sf::Texture::bind(NULL);
}

void PhysicalObject::beginContact(b2Contact *contact){}

void PhysicalObject::endContact(b2Contact *contact){}

void PhysicalObject::PreSolve(b2Contact* contact, const b2Manifold* oldManifold){}

void PhysicalObject::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse){}