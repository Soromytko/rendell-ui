#pragma once
#include <memory>
#include <unordered_set>
#include <rendell/rendell.h>
#include <rendell_ui/defines.h>
#include <rendell_ui/Signal.h>
#include <rendell_ui/Widgets/IWidget.h>
#include <rendell_ui/Widgets/Anchor.h>
#include <rendell_ui/Window/window_input.h>
#include "../src/Widgets/WidgetRegistrator.h"
#include "../src/Widgets/Transform2D.h"

namespace rendell_ui
{
	class Widget;

	RENDELL_UI_DECLARE_SHARED_PTR(Widget)
	RENDELL_UI_DECLARE_WEAK_PTR(Widget)

	void widget_deleter(Widget* widgetPtr);
	void release_widget(const WidgetSharedPtr& widget);
	void initWidget(WidgetSharedPtr widget, WidgetWeakPtr parent);

	template <typename WidgetType, typename... Args>
	std::shared_ptr<WidgetType> createWidget(WidgetWeakPtr parent = {}, Args&&... args)
	{
		std::shared_ptr<WidgetType> result(new WidgetType(std::forward<Args>(args)...), widget_deleter);
		initWidget(result, parent);
		return result;
	}

	class Widget : public IWidget
	{
		friend void initWidget(WidgetSharedPtr widget, WidgetWeakPtr parent);
	public:
		Signal<void> destroyed;
		Signal<void, bool> visibleChanged;
		Signal<void, bool> interactChanged;
		Signal<void, const WidgetSharedPtr&> parentChanged;
		Signal<void, glm::vec2> sizeChanged;
		Signal<void, Margins> marginsChanged;
		Signal<void, Anchor> anchorChanged;

	protected:
		Widget();
	public:
		virtual ~Widget();

	private:
		void setSelfWeakPtr(WidgetWeakPtr value);

	public:
		void setVisible(bool value);
		bool getVisible() const override;

		void setInteract(bool value);
		bool getInteract() const override;

		bool getImplicitVisible() const override;

		void setParent(WidgetWeakPtr widget);
		WidgetWeakPtr getParent() const;
		const std::unordered_set<WidgetSharedPtr>& getChildren() const;

		const Transform2D& getTransform() const;

		void removeParent();

		void setName(const std::string& name);
		const std::string& getName() const override;

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

		bool intersect(glm::vec2 point) const;

		virtual void draw() const override {};

		virtual void onFocused() {};
		virtual void onUnfocused() {};
		virtual void onMouseClick() {};
		virtual void onMouseDown(glm::dvec2 cursorPosition) {};
		virtual void onMouseUp(glm::dvec2 cursorPosition) {};
		virtual void onMouseHovered(glm::dvec2 cursorPosition) {}
		virtual void onMouseEntered() {}
		virtual void onMouseExited() {}
		virtual void onMouseScrolled(glm::dvec2 scroll) {}
		virtual void onCaptured() {}
		virtual void onFreed() {}
		virtual void onDragged(glm::dvec2 startPoint, glm::dvec2 endPoint) {}
		virtual void onKeyInputted(const KeyboardInput& keyboardInput) {};
		virtual void onCharInputted(unsigned char character) {};

	protected:
		virtual void updateUniforms() const;
		virtual void onSelfWeakPtrChanged() {}

	public:
		void updateRecursively();

	protected:
		void update();
		void updateImplicitVisibleRecursively();

		WidgetWeakPtr _selfWeakPtr;

		bool _visible{ true };
		bool _implicitVisible{ true };
		bool _interact{ true };

		WidgetWeakPtr _parent{};
		std::unordered_set<WidgetSharedPtr> _children{};

		std::string _name{ "Widget" };

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

	RENDELL_UI_DECLARE_WIDGET(Widget)

}
