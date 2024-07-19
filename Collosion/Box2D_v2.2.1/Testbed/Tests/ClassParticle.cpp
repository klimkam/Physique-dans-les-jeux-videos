#include <iostream>
#include "ClassParticle.h"
//#include "Header.h"

using namespace std;
/********** CONSTRUCTORS **************/
Particle::Particle() {}

Particle::Particle(float massParticle, b2Color colorParticle) : m_mass(massParticle), m_color(colorParticle)
{
	SetMass(m_mass);//if we don't use the setter we might get a problem
	SetRadius(1.0);
	m_position = b2Vec2(0.0, -10.0);
	m_velocity = b2Vec2_zero;
	m_appliedForce = b2Vec2_zero;
	m_speed = 10.0f;
	m_lifeTime = 0.0;
	m_isVisible = true;
}

/********** GETTERS/SETTERS **************/
//Getters
b2Vec2 Particle::GetPosition() { return m_position; }
b2Vec2 Particle::GetVelocity() { return m_velocity; }
b2Vec2 Particle::GetAppliedForce() { return m_appliedForce; }
float Particle::GetMass() { return m_mass; }
float Particle::GetRadius() { return m_radius; }
float Particle::GetLifeTime(){ return m_lifeTime;}
float Particle::GetSpeed(){	return m_speed;}
b2Color Particle::GetColor() { return m_color; }
//Setters - SEtMass there's calculation to be made
void Particle::SetMass(float newMass)
{
	//si la masse est zero, l'inverse est zero pas 1/0
	m_mass = newMass;
	if (m_mass < 1.0e-7)
	{
		m_mass = 0.0;
		m_invMass = 0.0;
	}
	else
	{
		m_invMass = 1.0 / m_mass;
	}
}
void Particle::SetRadius(float newRadius) { m_radius = newRadius; }
void Particle::SetColor(b2Color newColor) { m_color = newColor; }
void Particle::SetPosition(b2Vec2 newPos) { m_position = newPos; }
void Particle::SetVelocity(b2Vec2 newVel) { m_velocity = newVel; }
void Particle::SetVisibility(bool vis) { m_isVisible = vis; }

/********** DISPLAY METHODS **************/
void Particle::UpdateParticle(float deltaTime)
{
	m_lifeTime += deltaTime;
	//Gravity always apply
	ApplyForce(b2Vec2(0.0, -9.8 * m_mass));
	//add force 2 velocity n velocity 2 position (all vecs)
	m_velocity += m_appliedForce * deltaTime * m_invMass;
	m_position += m_velocity * deltaTime;
	//we set the force to zero to be applied again at each framme
	m_appliedForce.SetZero();
	
}

void Particle::RenderParticle(DebugDraw& debugDraw)
{
	if (m_isVisible)
	{
		b2Vec2 bottomCircle = m_position + m_radius * b2Vec2(0.0, -1.0);
		debugDraw.DrawCircle(m_position, m_radius, m_color);
		debugDraw.DrawSegment(m_position, bottomCircle, m_color);
	}	
}

void Particle::ApplyForce(b2Vec2 newForce)
{
	m_appliedForce += newForce;
}