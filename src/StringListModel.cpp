#include <rendell_ui/StringListModel.h>
#include <iostream>
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
			std::wcerr << "ERROR::StringListModel::insertItem: Invalid index; index = " << index
				<< ", value = " << value << ", _data.size() = " << _data.size() << std::endl;
			return;
		}

		_data.insert(_data.begin() + index, value);
		itemAdded.emit(value, _data.size() - 1);
	}

	void StringListModel::setItem(const String& value, size_t index)
	{
		if (index >= _data.size())
		{
			std::wcerr << "ERROR::StringListModel::setItem: Invalid index; index = "
				<< index << ", _data.size() = " << _data.size() << std::endl;
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
			std::wcerr << "ERROR::StringListModel::removeItem: Invalid index; index = "
				<< index << ", _data.size() = " << _data.size() << std::endl;
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
			std::wcerr << "ERROR::StringListModel::removeItem: Invalid item name; itemName = " << value << std::endl;
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
			std::wcerr << "ERROR::StringListModel::removeItem: Invalid index; index = "
				<< i << ", _data.size() = " << _data.size() << std::endl;
			return {};
		}
		return _data[i];
	}
}