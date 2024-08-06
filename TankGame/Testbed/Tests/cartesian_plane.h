
#include "../Framework/test.h"

class CartesianPlane : public Test
{
public:

	CartesianPlane();

	void Step(Settings* settings);

	static Test* Create()
	{
		return new CartesianPlane;
	}


protected:

	void DrawAxes();
	void DrawGrid();
	void DrawUnitCircle();

	int32 m_gridSize = 20;
	float m_gridSpacing = 1.0f;

	float m_pointSize = 0.1f;
	float m_angle = 0.0f;

	b2Color m_axisColor;
	b2Color m_gridColor;
	b2Color m_circleColor;

};


