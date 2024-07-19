#pragma once
#include "Testbed/Framework/Test.h"

class Particle
{
public:
	Particle();
	Particle(float massParticle, b2Color);

	virtual void UpdateParticle(float deltaTime);
	virtual void RenderParticle(DebugDraw& debugDraw);
	//Getters
	b2Vec2 GetPosition();
	b2Vec2 GetVelocity();
	b2Vec2 GetAppliedForce();
	float GetMass();
	float GetRadius();
	float GetLifeTime();
	float GetSpeed();
	b2Color GetColor();
	//Setters
	void SetMass(float newMass);
	void SetRadius(float newRadius);
	void SetColor(b2Color newColor);
	void SetPosition(b2Vec2 newPos);
	void SetVelocity(b2Vec2 newVel);
	void SetVisibility(bool vis);
	void ApplyForce(b2Vec2 newForce);

protected:
	float m_mass;
	float m_invMass;//calculated in setMass
	float m_radius;
	float m_lifeTime;
	float m_speed;
	bool m_isVisible;

	b2Vec2 m_position;
	b2Vec2 m_velocity;
	b2Vec2 m_appliedForce;
	b2Color m_color;
};





