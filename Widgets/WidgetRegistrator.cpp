#include "WidgetRegistrator.h"
#include <cassert>

static WidgetRegistrator* s_instance{ nullptr };

WidgetRegistrator::WidgetRegistrator(std::initializer_list<IWidgetCollectionSharedPtr> collections)
{
	for (const IWidgetCollectionSharedPtr& collection : collections)
	{
		assert(collection);
	}
	_collections = collections;
}

void WidgetRegistrator::init(std::initializer_list<IWidgetCollectionSharedPtr> collections)
{
	assert(!s_instance);
	s_instance = new WidgetRegistrator(collections);
}

void WidgetRegistrator::release()
{
	assert(s_instance);
	delete s_instance;
	s_instance = nullptr;
}

WidgetRegistrator* WidgetRegistrator::getInstance()
{
	assert(s_instance);
	return s_instance;
}

void WidgetRegistrator::registerWidget(IWidget* widget)
{
	for (const IWidgetCollectionSharedPtr& collection : _collections)
	{
		collection->append(widget);
	}
}

void WidgetRegistrator::unregisterWidget(IWidget* widget)
{
	for (const IWidgetCollectionSharedPtr& collection : _collections)
	{
		collection->remove(widget);
	}
}
