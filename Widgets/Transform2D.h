#pragma once
#include <glm/glm.hpp>

class Transform2D final
{
public:
	Transform2D() = default;
	~Transform2D() = default;

	void setPosition(glm::vec2 value);
	void setRotation(float value);
	void setScale(glm::vec2 value);

	glm::vec2 getPosition() const;
	float getRotation() const;
	glm::vec2 getScale() const;

	const glm::mat4& getMatrix() const;

private:
	void updateMatrix();

	glm::vec2 _position{ 0.0f, 0.0f };
	float _rotation{ 0.0f };
	glm::vec2 _scale{ 1.0f, 1.0f };

	glm::mat4 _mat{ glm::mat4(1.0f) };

};