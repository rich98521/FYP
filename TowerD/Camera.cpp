#include "stdafx.h"
#include "Camera.h"

Camera::Camera(sf::Vector2i* aim, sf::Vector2f* pos, sf::Vector2f screensize)
	:mAimPos(aim), mPlayerPos(pos), mScreenSize(screensize)
{
	mCurrentOffset = sf::Vector2f();
	mScale = 1;
}

//uses mouse position and player position to work out correct offset
void Camera::Update(sf::Vector2i mouseSPos)
{
	//uses mouses distance from centre of the screen to zoom out when the mouse is near the edge
	float dist = sqrt(pow(((mouseSPos.y - mScreenSize.y / 2) / (mScreenSize.y / 2)) * (mScreenSize.x / 2), 2) + pow(mouseSPos.x - (mScreenSize.x / 2), 2));
	mScale = 3 - fmin(fmax(dist / (mScreenSize.x / 3), 1.f), 1.4f);
	float length = sqrt(pow(mAimPos->y - mPlayerPos->y, 2) + pow(mAimPos->x - mPlayerPos->x, 2));
	float ang = atan2(mAimPos->y - mPlayerPos->y, mAimPos->x - mPlayerPos->x);
	//centres the camera between mouse position and player location
	sf::Vector2f offset = sf::Vector2f(cos(ang), sin(ang)) * length / 2.f;
	//calculates offset
	if (fabs(offset.x) > 0.0000015)
		offset.x = (fmax(fabs(cos(ang)) * (mScreenSize.y / 50), fabs(offset.x)) - fabs(cos(ang)) * (mScreenSize.y / 50)) * (offset.x / fabs(offset.x));
	if (fabs(offset.y) > 0.0000015)
		offset.y = (fmax(fabs(sin(ang)) * (mScreenSize.y / 50), fabs(offset.y)) - fabs(sin(ang)) * (mScreenSize.y / 50)) * (offset.y / fabs(offset.y));
	//centres camera on player
	mOffset = (*mPlayerPos + offset - (mScreenSize / 2.f) / mScale);
	//gradually increases actual offset towards desired offset to make camera smooth
	mCurrentOffset += (mOffset - mCurrentOffset) / 20.f;
}

sf::Vector2f Camera::Offset() const
{
	return mCurrentOffset;
}

float Camera::Scale() const
{
	return mScale;
}

void Camera::SetScale(float s)
{
	mScale = s;
}