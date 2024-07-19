#include "Arkanoid.h"

#include <vector>
#include <iostream>

void Ball::Render(DebugDraw& debugDraw)
{
	debugDraw.DrawSolidCircle(m_position, m_radius, b2Vec2(0.0f, 1.0f), b2Color(1.0f, 0.0f, 0.0f));
}

void Ball::Update(float deltaTime)
{
	HandleContacts();

	m_position += m_velocity * deltaTime;
}

void Ball::AddContact(const Contact& cp)
{
	m_contacts.push_back(cp);
}

void Ball::HandleContacts()
{
	//Resolve Collision
	if (m_contacts.size() > 0)
	{
		//Prevent interpenetration => directly update position with the deltaPos
		b2Vec2 deltaPos(0.0f, 0.0f);
		for (size_t i = 0; i<m_contacts.size(); ++i)
		{
			deltaPos += m_contacts[i].m_normal * (m_contacts[i].m_penetrationDepth + m_radius);
		}

		m_position += deltaPos;

		//Average contact normal
		b2Vec2 collisionNormal(0.0f, 0.0f);
		for (size_t i = 0; i<m_contacts.size(); ++i)
		{
			collisionNormal += m_contacts[i].m_normal;
		}

		collisionNormal.Normalize();

		b2Vec2 tempVec(collisionNormal.x * abs(m_velocity.x), collisionNormal.y * abs(m_velocity.y));
		b2Vec2 tempVec2(2 * tempVec);
		m_velocity += tempVec2;
	}

	m_contacts.clear();
}

Paddle::Paddle(b2Vec2& center, float width)
	: m_center(center)
	, m_width(width)
	, m_direction(0.0f, 0.0f)
	, m_speed(12.0f) 
	, m_min_X(-b2_maxFloat)
	, m_max_X(b2_maxFloat)
{

}

void Paddle::SetWorldLimits(float min, float max)
{
	m_min_X = min;
	m_max_X = max;
}

void Paddle::Update(float deltaTime)
{
	if (GetLeftPos().x <= m_min_X && m_direction.x < 0) { return; }
	if (GetRightPos().x >= m_max_X && m_direction.x > 0) { return; }

	m_center.Set((m_center.x + m_direction.x * m_speed * deltaTime), m_center.y);
}

void Paddle::Render(DebugDraw& debugDraw)
{
	b2Vec2 halfExtent(m_width*0.5f, 0.0f);
	debugDraw.DrawSegment(m_center + halfExtent, m_center - halfExtent, b2Color(0.0f, 1.0f, 0.0f));
}

void Paddle::SetMoveRight()
{
	m_direction = b2Vec2(1.0f, 0.0f);
}

void Paddle::SetMoveLeft()
{
	m_direction = b2Vec2(-1.0f, 0.0f);
}

void Paddle::Stop()
{
	m_direction = b2Vec2_zero;
}

ArkanoidGame::ArkanoidGame()
: m_worldBoxMin(-25.0f,0.0f)
, m_worldBoxMax(25.0f,50.0f)
, m_paddle(b2Vec2(0.0f, 2.0f), 4.0f)
{
	m_paddle.SetWorldLimits(m_worldBoxMin.x, m_worldBoxMax.x);
}

void ArkanoidGame::Step(Settings* settings)
{
	float timeStep = settings->hz > 0.0f ? 1.0f / settings->hz : float32(0.0f);

	m_paddle.Update(timeStep);
	UpdateBalls(timeStep);

	CheckCollisions();

	CheckOutofWorld();

	Render();
}

void ArkanoidGame::UpdateBalls(float deltaTime)
{
	for (size_t i = 0; i < m_balls.size(); ++i)
	{
		m_balls[i]->Update(deltaTime);
	}
}

void ArkanoidGame::CheckCollisions()
{
	//Box interior normals
	b2Vec2 rightN(-1.0f,0.0f);
	b2Vec2 leftN(1.0f,0.0f);
	b2Vec2 upN(0.0f,-1.0f);
	b2Vec2 downN(0.0f, 1.0f);

	//Check collisions for all particules
	std::vector<Ball*>::iterator it;
	for (it=m_balls.begin(); it!= m_balls.end(); ++it)
	{
		Ball* ball = *it;
		
		b2Vec2 pos = ball->GetPosition();
		float32 radius = ball->GetRadius();

		//Check collisions for each wall
		
		//Right
		float right = pos.x + radius;
		if( right >= m_worldBoxMax.x )
		{
			ball->AddContact(Contact(rightN, b2Vec2( right, pos.y ), right - m_worldBoxMax.x ) );
		}

		//Left
		float left = pos.x - radius;
		if( left <= m_worldBoxMin.x )
		{
			ball->AddContact( Contact(leftN, b2Vec2( left, pos.y ), m_worldBoxMin.x - left ) );
		}

		//Up
		float up = pos.y + radius;
		if( up >= m_worldBoxMax.y )
		{
			ball->AddContact(Contact(upN, b2Vec2( pos.x, up ), up - m_worldBoxMax.y ) );
		}


		//Check Collision with paddle

		float down = pos.y - radius;
		b2Vec2 ballHitbox(ball->GetPosition().x + ball->GetRadius() / 2,
			ball->GetPosition().x - ball->GetRadius() / 2);

		if (down <= m_paddle.GetHeight() && (ballHitbox.x > m_paddle.GetLeftPos().x && ballHitbox.y < m_paddle.GetRightPos().x))
		{
			ball->AddContact(Contact(downN, b2Vec2(pos.x, down), m_paddle.GetHeight() - down));
		}
	}
	
}

void ArkanoidGame::CheckOutofWorld()
{
	for (size_t i = 0; i < m_balls.size(); )
	{
		if (IsOutofWorld(m_balls[i]))
		{
			//Remove ball
			RemoveBall(m_balls[i]);
		}
		else
		{
			i++;
		}
	}
}

bool ArkanoidGame::IsOutofWorld(Ball* ball)
{
	b2Vec2 pos = ball->GetPosition();
	float32 radius = ball->GetRadius();

	//Left
	float left = pos.x + radius;
	if (left <= m_worldBoxMin.x)
	{
		return true;
	}

	//Right
	float right = pos.x - radius;
	if (right >= m_worldBoxMax.x)
	{
		return true;
	}

	//Up
	float up = pos.y - radius;
	if (up >= m_worldBoxMax.y)
	{
		return true;
	}

	//Down
	float down = pos.y + radius;
	if (down <= m_worldBoxMin.y)
	{
		return true;
	}

	return false;
}

void ArkanoidGame::Render()
{
	m_paddle.Render(m_debugDraw);

	//Render Ball
	for (size_t i = 0; i < m_balls.size();++i)
	{
		m_balls[i]->Render(m_debugDraw);
	}

	//Render Box
	b2Vec2 box[4];
	box[0].Set(m_worldBoxMin.x, m_worldBoxMin.y);
	box[1].Set(m_worldBoxMin.x, m_worldBoxMax.y);
	box[2].Set(m_worldBoxMax.x, m_worldBoxMax.y);
	box[3].Set(m_worldBoxMax.x, m_worldBoxMin.y);

	m_debugDraw.DrawSegment(box[0], box[1], b2Color(0.0f, 0.0f, 1.0f));
	m_debugDraw.DrawSegment(box[1], box[2], b2Color(0.0f, 0.0f, 1.0f));
	m_debugDraw.DrawSegment(box[2], box[3], b2Color(0.0f, 0.0f, 1.0f));
}


void ArkanoidGame::Keyboard(unsigned char key)
{
	switch (key)
	{
	case 'a':
		{
			m_paddle.SetMoveLeft();
		}
		break;

	case 'd':
		{
			m_paddle.SetMoveRight();
		}
		break;

	case 'n':
		{
			AddBall();
		}
		break;
	}
}

void ArkanoidGame::KeyboardUp(unsigned char key)
{
	switch (key)
	{
	case 'a':
	case 'd':
		m_paddle.Stop();
	break;
	}
}

void ArkanoidGame::AddBall()
{
	float angle = RandomFloat(-b2_pi*0.25f, b2_pi*0.25f);
	b2Rot rot(angle);

	float speed = RandomFloat(10.0f, 20.0f);
	b2Vec2 dir(0.0f, speed);

	Ball* ball = new Ball(b2Vec2(0.0f, 5.0f), b2Mul(rot, dir));
	m_balls.push_back(ball);
}

void ArkanoidGame::RemoveBall(Ball* ball)
{
	std::vector<Ball*>::iterator it = m_balls.begin();

	while (it != m_balls.end())
	{
		if ((*it) == ball)
		{
			std::swap(*it, m_balls.back());
			m_balls.pop_back();
			break;
		}
		++it;
	}
}