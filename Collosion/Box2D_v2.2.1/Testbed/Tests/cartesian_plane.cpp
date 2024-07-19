
#include "cartesian_plane.h"


CartesianPlane::CartesianPlane()
{
	m_axisColor = b2Color(255.0f, 0.0f, 0.0f);
    m_gridColor = b2Color(255.0f, 250.0f, 205.0f);
	m_circleColor = b2Color(0.0f, 128.0f, 0.0f);
}

void CartesianPlane::Step(Settings* settings)
{
	DrawGrid();
	DrawAxes();
	DrawUnitCircle();

	//Draw
	b2Vec2 vec(cosf(m_angle), sinf(m_angle));

	m_debugDraw.DrawSegment(b2Vec2(0.0f, 0.0f ), vec, m_circleColor);
	m_angle += 0.01f;


	float angle30 = 30.0f / 180.0f * b2_pi;
	float r = 5.0f;
	b2Vec2 rVec(r * cosf(angle30), r * sinf(angle30));
	m_debugDraw.DrawSegment(b2Vec2(0.0f, 0.0f), rVec, b2Color(255.0f,0.0f,255.0f));

	float length = rVec.Normalize();

	m_debugDraw.DrawSegment(b2Vec2(0.0f, 0.0f), rVec, b2Color(0.0f, 0.0f, 255.0f));
}


void CartesianPlane::DrawAxes()
{
	float gridDimension = m_gridSize * m_gridSpacing;

	m_debugDraw.DrawSegment(b2Vec2(0.0f, -gridDimension), b2Vec2(0.0f, gridDimension), m_axisColor);
	m_debugDraw.DrawSegment(b2Vec2( -gridDimension, 0.0f ), b2Vec2(gridDimension, 0.0f ), m_axisColor);
}

void CartesianPlane::DrawGrid()
{

	float gridDimension =  m_gridSize * m_gridSpacing;
	int32 nbLines = 2 * m_gridSize +1;

	for (int32 i = 0; i < nbLines; i++)
	{
		float posX = -gridDimension + i * m_gridSpacing;

		m_debugDraw.DrawSegment(b2Vec2(posX, -gridDimension), b2Vec2(posX, gridDimension), m_gridColor);
	}

	
	for (int32 i = 0; i < nbLines; i++)
	{
		float poxY = -gridDimension + i * m_gridSpacing;

		m_debugDraw.DrawSegment(b2Vec2(-gridDimension, poxY), b2Vec2(gridDimension, poxY), m_gridColor);

	}
}

void CartesianPlane::DrawUnitCircle()
{
	m_debugDraw.DrawPoint(b2Vec2(0.0f, 0.0f), m_pointSize, m_axisColor);
	m_debugDraw.DrawCircle(b2Vec2(0.0f, 0.0f), 1.0f, m_circleColor);
}




