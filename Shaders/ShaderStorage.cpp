#include "ShaderStorage.h"
#include <cassert>

static ShaderStorage *s_instance;

ShaderStorage::ShaderStorage() :
	rectangleShader(new RectangleShader)
{

}

void ShaderStorage::init()
{
	assert(!s_instance);
	s_instance = new ShaderStorage;
}

void ShaderStorage::release()
{
	assert(s_instance);
	delete s_instance;
	s_instance = nullptr;
}

ShaderStorage* ShaderStorage::getInstance()
{
	assert(s_instance);
	return s_instance;
}
