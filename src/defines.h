#pragma once

#define RENDELL_UI_DECLARE_SHARED_PTR(ClassName) \
	typedef std::shared_ptr<ClassName> ClassName##SharedPtr;

#define RENDELL_UI_DECLARE_WEAK_PTR(ClassName) \
	typedef std::weak_ptr<ClassName> ClassName##WeakPtr;

#define RENDELL_UI_DECLARE_SHARED_PTR_FACTORY(ClassName) \
	RENDELL_UI_DECLARE_SHARED_PTR(ClassName) \
	RENDELL_UI_DECLARE_WEAK_PTR(ClassName) \
	template <typename... Args> \
    ClassName##SharedPtr make##ClassName(Args&&... args) { \
		return std::make_shared<ClassName>(std::forward<Args>(args)...); \
    }

#define RENDELL_UI_DECLARE_WIDGET(WidgetClassName) \
	RENDELL_UI_DECLARE_SHARED_PTR(WidgetClassName) \
	RENDELL_UI_DECLARE_WEAK_PTR(WidgetClassName) \
	template<typename... Args> \
	WidgetClassName##SharedPtr create##WidgetClassName(WidgetWeakPtr parent = {}, Args&&... args) \
	{ \
		return createWidget<WidgetClassName>(parent, std::forward<Args>(args)...); \
	}

#define RENDELL_UI_FRIEND_WIDGET \
	template <typename WidgetType, typename... Args> \
	friend std::shared_ptr<WidgetType> createWidget(WidgetWeakPtr parent, Args&&... args);
