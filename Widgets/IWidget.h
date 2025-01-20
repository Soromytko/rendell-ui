#pragma once

class IWidget
{
protected:
	IWidget() = default;

public:
	virtual ~IWidget() = default;

	virtual void updateRecursively() = 0;
	virtual void draw() const = 0;

};