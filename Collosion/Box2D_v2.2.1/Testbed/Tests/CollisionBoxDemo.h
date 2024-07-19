#pragma once

#include "Testbed/Framework/Test.h"
#include "Testbed/Tests/ClassParticle.h"
#include <vector>
using namespace std;

class CollisionBoxDemo: public Test
{
public:
	CollisionBoxDemo();
	~CollisionBoxDemo() {}
	static Test* Create() { return new CollisionBoxDemo; }

	void Step(Settings* settings) override;
	void KeyboardUp(unsigned char key) override;

	void CheckWallCollision();
	void PreventWallOverlap(Particle& particleA, float penetrationDepth, b2Vec2 normal);
	void CheckParticleToParticleCollision();

	void ApplyCollisionWithStaticObject(Particle& particle, b2Vec2 normal);
	void ApplyCollisionBetweenParticles(Particle& partA, Particle& partB, b2Vec2 normal);

	void UpdateParticles(float deltaTime);
	void PreventParticleOverlap(Particle& particleA, Particle& particleB, float penetrationDepth);
	void CreateParticle();

	void DrawBoundingBox();

	std::vector<Particle> m_particles;
	int m_maxParticle;

	b2Vec2 m_boundingBoxMin;
	b2Vec2 m_boundingBoxMax;
};



