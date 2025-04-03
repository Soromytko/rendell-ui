#pragma once

#define RENDELL_UI_NAMESPACE rendell_ui

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
	WidgetClassName##SharedPtr create##WidgetClassName(RENDELL_UI_NAMESPACE::WidgetWeakPtr parent = {}, Args&&... args) \
	{ \
		return RENDELL_UI_NAMESPACE::createWidget<WidgetClassName>(parent, true, std::forward<Args>(args)...); \
	} \
	\
	template<typename... Args> \
	WidgetClassName##SharedPtr createUnregistered##WidgetClassName(RENDELL_UI_NAMESPACE::WidgetWeakPtr parent = {}, Args&&... args) \
	{ \
	return RENDELL_UI_NAMESPACE::createWidget<WidgetClassName>(parent, false, std::forward<Args>(args)...); \
	}
	
