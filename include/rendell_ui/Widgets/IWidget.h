#pragma once
#include <string>

namespace rendell_ui
{
	class IWidget
	{
	protected:
		IWidget() = default;

	public:
		virtual ~IWidget() = default;

		virtual const std::string& getName() const = 0;

		virtual bool getVisible() const = 0;
		virtual bool getImplicitVisible() const = 0;
		virtual bool getInteract() const = 0;

		virtual void updateRecursively() = 0;
		virtual void draw() const = 0;

	};
}
