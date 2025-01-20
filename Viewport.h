#pragma once
#include <memory>
#include <glm/glm.hpp>

class Viewport;

typedef std::shared_ptr<Viewport> ViewportSharedPtr;

class Viewport final
{
public:
	Viewport() = default;
	~Viewport() = default;

	static void setCurrent(const ViewportSharedPtr viewport);
	static ViewportSharedPtr getCurrent();

	void apply();
	void setParameters(int x, int y, int width, int height);

	void setProjectMat(const glm::mat4& value);
	void setViewMat(const glm::mat4& value);

	const glm::mat4& getProjectMat() const;
	const glm::mat4& getViewMat() const;

	glm::ivec2 getSize() const;

private:
	int _x{ 0 }, _y{ 0 }, _width{ 100 }, _height{ 100 };
	glm::mat4 _projectMat = glm::mat4(1.0f);
	glm::mat4 _viewMat = glm::mat4(1.0f);

};