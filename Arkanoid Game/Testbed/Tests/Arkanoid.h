#ifndef ARKANOID_H
#define ARKANOID_H

#include "Testbed/Framework/Test.h"
#include <vector>


struct Contact
{
	Contact(const b2Vec2& normal, const b2Vec2& contactPt, float32 penetration = 0.0f)
		: m_normal(normal)
		, m_contactPt(contactPt)
		, m_penetrationDepth(penetration) {}

	b2Vec2 m_normal;
	b2Vec2 m_contactPt;
	float32 m_penetrationDepth;
};



class Ball
{
public:
	Ball(): m_position(0.0f,0.0f), m_velocity(1.0f,1.0f), m_radius(0.5f){}
	Ball(const b2Vec2& position,const b2Vec2& velocity, float32 radius = 0.5f)
		: m_position(position)
		, m_velocity(velocity)
		, m_radius(radius)
	{

	}

	const b2Vec2& GetPosition(){return m_position;}
	void SetPosition(const b2Vec2& newPosition){m_position = newPosition;}

	float GetRadius() { return m_radius; }

	void Update(float deltaTime);
	void Render(DebugDraw& debugDraw);

	void AddContact(const Contact& cp);
	void HandleContacts();


protected:

	b2Vec2 m_position;
	b2Vec2 m_velocity;
	float32 m_radius;

	std::vector<Contact> m_contacts;

};


class Paddle
{
public:

	Paddle(b2Vec2& center, float width);

	void Update(float deltaTime);
	void Render(DebugDraw& debugDraw);

	void SetSpeed(float speed) { m_speed = speed; }

	b2Vec2 GetLeftPos() { return m_center - b2Vec2( m_width *0.5f, 0.0f ); }
	b2Vec2 GetRightPos() { return m_center + b2Vec2( m_width *0.5f, 0.0f ); }
	float GetHeight() { return m_center.y; }


	void SetMoveRight();
	void SetMoveLeft();
	void Stop();

	void SetWorldLimits(float min, float max);

protected:

	b2Vec2 m_center;
	float m_width;

	float m_min_X;
	float m_max_X;

	b2Vec2 m_direction;
	float m_speed;
};

class ArkanoidGame : public Test
{
public:
	static Test* Create()
	{
		return new ArkanoidGame;
	}

	ArkanoidGame();

	virtual void Step(Settings* settings);

	void CheckCollisions();
	void CheckOutofWorld();
	bool IsOutofWorld(Ball* ball);

	void UpdateBalls(float deltaTime);
	void Render();
	void Keyboard(unsigned char key);
	void KeyboardUp(unsigned char key);

	void AddBall();
	void RemoveBall(Ball* ball);

	b2Vec2 m_worldBoxMin;
	b2Vec2 m_worldBoxMax;

	Paddle m_paddle;
	std::vector<Ball*> m_balls;
	
};

#endif
