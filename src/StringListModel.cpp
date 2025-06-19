#include <rendell_ui/StringListModel.h>
#include <logging.h>
#include <algorithm>

namespace rendell_ui
{
	StringListModel::StringListModel(std::initializer_list<StringListModel::String> data) :
		_data(data)
	{

	}

	size_t StringListModel::getSize() const
	{
		return _data.size();
	}

	const std::vector<StringListModel::String>& StringListModel::getData() const
	{
		return _data;
	}

	void StringListModel::addItem(const String& value)
	{
		_data.push_back(value);
		itemAdded.emit(value, _data.size() - 1);
	}

	void StringListModel::insertItem(const String& value, size_t index)
	{
		if (index > _data.size())
		{
			RUI_ERROR(L"Invalid index; index = {}, value = {}, _data.size() = ", index, value, _data.size());
			return;
		}

		_data.insert(_data.begin() + index, value);
		itemAdded.emit(value, _data.size() - 1);
	}

	void StringListModel::setItem(const String& value, size_t index)
	{
		if (index >= _data.size())
		{
			RUI_ERROR("Invalid index; index = {}, _data.size() = {}", index, _data.size());
			return;
		}

		if (_data[index] != value)
		{
			_data[index] = value;
			itemChanged.emit(value, index);
		}
	}

	void StringListModel::removeItem(size_t index)
	{
		if (index >= _data.size())
		{
			RUI_ERROR("Invalid index; index = {}, _data.size() = {}", index, _data.size());
			return;
		}

		auto it = _data.begin() + index;
		const String value = *it;
		_data.erase(it);
		itemRemoved.emit(value, index);
	}

	void StringListModel::removeItem(const String& value)
	{
		auto it = std::find(_data.begin(), _data.end(), value);

		if (it == _data.end())
		{
			RUI_ERROR(L"Invalid item name; itemName = {}", value);
			return;
		}

		const size_t index = std::distance(_data.begin(), it);
		_data.erase(it);
		itemRemoved.emit(value, index);
	}

	const StringListModel::String& StringListModel::operator[](size_t i)
	{
		if (i >= _data.size())
		{
			RUI_ERROR("Invalid index; index = {}, _data.size() = {}", i, _data.size());
			static String emptyString;
			return emptyString;
		}
		return _data[i];
	}
}