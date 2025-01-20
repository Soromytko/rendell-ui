#include "Transform2D.h"
#include <glm/gtc/matrix_transform.hpp>

void Transform2D::setPosition(glm::vec2 value)
{
	_position = value;
	updateMatrix();
}

void Transform2D::setRotation(float value)
{
	_rotation = value;
	updateMatrix();
}

void Transform2D::setScale(glm::vec2 value)
{
	_scale = value;
	updateMatrix();
}

glm::vec2 Transform2D::getPosition() const
{
	return _position;
}

float Transform2D::getRotation() const
{
	return _rotation;
}

glm::vec2 Transform2D::getScale() const
{
	return _scale;
}

const glm::mat4& Transform2D::getMatrix() const
{
	return _mat;
}

void Transform2D::updateMatrix()
{
	_mat = glm::translate(glm::mat4(1.0f), glm::vec3(_position, -1.0f));
	_mat = glm::rotate(_mat, _rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	_mat = glm::scale(_mat, glm::vec3(_scale, 1.0f));
}