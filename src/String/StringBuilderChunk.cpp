#include "StringBuilderChunk.h"
#include <cassert>

namespace rendell_ui
{
	StringBuilderChunk::StringBuilderChunk(size_t size, StringType&& data)
	{
		_size = size;
		assert(data.size() < size);
		_data = std::move(data);
		_data.resize(_size);
	}

	bool StringBuilderChunk::isFull() const
	{
		return getRemainingLength() > 0;
	}

	size_t StringBuilderChunk::getCurrentLength() const
	{
		return _currentLength;
	}

	size_t StringBuilderChunk::getRemainingLength() const
	{
		return _size - _currentLength;
	}

	const StringType& StringBuilderChunk::getData() const
	{
		return _data;
	}

	StringType StringBuilderChunk::toString() const
	{
		return StringType(_data.begin(), _data.begin() + _currentLength);
	}

	StringType StringBuilderChunk::getSubData(size_t indexFrom) const
	{
		return getSubData(indexFrom, _currentLength - indexFrom);
	}

	StringType StringBuilderChunk::getSubData(size_t indexFrom, size_t count) const
	{
		assert(indexFrom < _currentLength);
		assert(indexFrom + count <= _currentLength);

		return StringType(_data.begin() + indexFrom, _data.begin() + indexFrom + count);
	}

	size_t StringBuilderChunk::append(const StringType& data, size_t startIndex)
	{
		if (data.size() == 0)
		{
			return 0;
		}

		auto it = data.begin() + startIndex;
		const size_t remainingLength = getRemainingLength();

		while (_currentLength < _size)
		{
			_data[_currentLength] = *it;
			_currentLength++;

			if (++it == data.end())
			{
				return 0;
			}
		}

		return data.size() - startIndex - remainingLength;
	}

	size_t StringBuilderChunk::append(const StringBuilderChunk& chunk)
	{
		size_t result = chunk.getCurrentLength();
		const StringType& chunkData = chunk.getData();
		for (size_t i = 0; i < chunk.getCurrentLength(); i++)
		{
			if (_currentLength < _size)
			{
				_data[_currentLength] = chunkData[i];
				_currentLength++;
			}
			else
			{
				return chunk.getCurrentLength() - i;
			}
		}

		return 0;
	}

	size_t StringBuilderChunk::insert(size_t index, const StringType& data, size_t dataOffsetIndex)
	{
		assert(index <= _currentLength);
		assert(index < _size);

		const size_t remainingLength = getRemainingLength();

		if (remainingLength == 0)
		{
			return data.size();
		}

		const size_t shiftSize = remainingLength < data.size() ? remainingLength : data.size();
		const size_t startInsertIndex = _currentLength;
		for (size_t i = 0; i < shiftSize; i++)
		{
			_data[_currentLength + remainingLength - 1] = _data[index + i];
			_data[index + i] = data[dataOffsetIndex + i];
		}

		_currentLength = _size;
		return data.size() - shiftSize;
	}

	void StringBuilderChunk::removeAt(size_t index)
	{
		assert(index < _currentLength);
		if (index == _currentLength - 1)
		{
			_currentLength--;
			return;
		}

		for (size_t i = index; i < _currentLength - 1; i++)
		{
			_data[i] = _data[i + 1];
		}
		_currentLength--;
	}

	void StringBuilderChunk::removeEnd(size_t indexFrom)
	{
		assert(indexFrom < _currentLength);
		_currentLength = indexFrom;
	}

	void StringBuilderChunk::remove(size_t indexFrom, size_t count)
	{
		assert(indexFrom < _currentLength);
		assert(indexFrom + count <= _currentLength);

		if (indexFrom + count == _currentLength)
		{
			removeEnd(indexFrom);
			return;
		}

		for (size_t i = indexFrom; i + count < _currentLength; i++)
		{
			_data[i] = _data[i + count];
		}
		_currentLength -= count;
	}

}