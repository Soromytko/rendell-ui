#pragma once

namespace rendell_ui
{
	class IWidget
	{
	protected:
		IWidget() = default;

	public:
		virtual ~IWidget() = default;

		virtual bool getVisible() const = 0;

		virtual void updateRecursively() = 0;
		virtual void draw() const = 0;

	};
}
