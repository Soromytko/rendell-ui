#pragma once
#include <memory>
#include <memory>
#include "IWidget.h"

class IWidgetCollection
{
protected:
	IWidgetCollection() = default;

public:
	~IWidgetCollection() = default;

	virtual void append(IWidget* value) = 0;
	virtual void remove(IWidget* value) = 0;
};

typedef std::shared_ptr<IWidgetCollection> IWidgetCollectionSharedPtr;
