#include "Collider.h"
#include<iostream>
bool  learning::Intersect(ColliderCircle const& lhs, ColliderCircle const& rhs)
{
	return (rhs.center - lhs.center).LengthSquared() <= pow(lhs.radius + rhs.radius, 2);
}

//learning::Vector2f learning::CollisionOccured(ColliderCircle const& ball, ColliderCircle const& player)
//{
//	float directionX = ball.center.x - player.center.x;
//	Vector2f dir = {directionX, -1.0f}; 
//	//Vector2f dir = ball.center - player.center;
//	return dir.Normalized();
//}

float learning::Colli(ColliderCircle const& ball, ColliderCircle const& player)
{
	float diff = ball.center.x - player.center.x;
	//std::cout << diff << std::endl;
	return diff/100;
}

bool learning::Intersect(ColliderBox const& lhs, ColliderBox const& rhs)
{
	//x-direction
	if (lhs.center.x - lhs.halfSize.x > rhs.center.x + rhs.halfSize.x) return false;
	if (lhs.center.x + lhs.halfSize.x < rhs.center.x - rhs.halfSize.x) return false;

	//y-direction
	if (lhs.center.y - lhs.halfSize.y > rhs.center.y + rhs.halfSize.y) return false;
	if (lhs.center.y + lhs.halfSize.y < rhs.center.y - rhs.halfSize.y) return false;

	return true;

}
