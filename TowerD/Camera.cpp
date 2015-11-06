#include "stdafx.h"
#include "Camera.h"

Camera::Camera(sf::Vector2i* aim, sf::Vector2f* pos)
	:mAimPos(aim), mPlayerPos(pos)
{

}
void Camera::Update(sf::Vector2i mouseSPos)
{
	float dist = sqrt(pow(((mouseSPos.y - 300) / 300.f) * 400, 2) + pow(mouseSPos.x - 400, 2));
	mScale = 3 - fmin(fmax(dist / 200.f, 1.f), 1.4f);
	float length = sqrt(pow(mAimPos->y - mPlayerPos->y, 2) + pow(mAimPos->x - mPlayerPos->x, 2));
	float ang = atan2(mAimPos->y - mPlayerPos->y, mAimPos->x - mPlayerPos->x);
	sf::Vector2f offset = sf::Vector2f(cos(ang), sin(ang)) * length / 2.f;
	if (fabs(offset.x) > 0.0000015)
		offset.x = (fmax(fabs(cos(ang)) * 60, fabs(offset.x)) - fabs(cos(ang)) * 60) * (offset.x / fabs(offset.x));
	if (fabs(offset.y) > 0.0000015)
		offset.y = (fmax(fabs(sin(ang)) * 60, fabs(offset.y)) - fabs(sin(ang)) * 60) * (offset.y / fabs(offset.y));
	mOffset = (*mPlayerPos + offset + sf::Vector2f(-400, -300) / mScale);
}

sf::Vector2f Camera::Offset() const
{
	return mOffset;
}

float Camera::Scale() const
{
	return mScale;
}

void Camera::SetScale(float s)
{
	mScale = s;
}