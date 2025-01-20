#pragma once
#include <memory>
#include <set>
#include <rendell/rendell.h>
#include "IWidget.h"
#include "Anchor.h"
#include "Transform2D.h"

class Widget : public IWidget
{
public:
	Widget(Widget* parent = nullptr);
	virtual ~Widget();

	void setParent(Widget* widget);
	Widget* getParent() const;
	const std::set<Widget*>& getChildren() const;

	const Transform2D& getTransform() const;

	virtual void setColor(glm::vec4 value);
	glm::vec4 getColor() const;

	void setOffset(glm::vec2 value);
	glm::vec2 getOffset() const;

	void setSize(glm::vec2 value);
	glm::vec2 getSize() const;

	void setMargins(Margins value);
	void setMargins(float left, float right, float bottom, float top);
	Margins getMargins() const;

	void setAnchor(Anchor value);
	Anchor getAnchor() const;

	virtual void draw() const override {};

protected:
	virtual void updateUniforms() const;

public:
	void updateRecursively();

protected:
	void update();

	Widget* _parent{ nullptr };
	std::set<Widget*> _children{};

	uint32_t _matrixUniformIndex{ 0 };
	uint32_t _sizeUniformIndex{ 0 };
	uint32_t _colorUniformIndex{ 0 };

	Transform2D _transform{};
	glm::vec4 _color{ 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec2 _offset{ 0.0f, 0.0f };
	glm::vec2 _size{ 100.0f, 100.0f };
	Margins _margins{ 0.0f, 0.0f, 0.0f, 0.0f };
	Anchor _anchor{ Anchor::center };
	rendell::ShaderProgramSharedPtr _shaderProgram{ nullptr };

};

typedef std::shared_ptr<Widget> WidgetSharedPtr;
