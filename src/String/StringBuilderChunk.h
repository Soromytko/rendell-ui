#pragma once
#include <memory>
#include <vector>
#include <string>
#include "../defines.h"

namespace rendell_ui
{
	typedef std::wstring StringType;

	class StringBuilderChunk final
	{
	public:
		StringBuilderChunk(size_t size = 16, StringType&& data = {});
		~StringBuilderChunk() = default;

		size_t getCurrentLength() const;
		size_t getRemainingLength() const;
		const StringType& getData() const;
		StringType toString() const;

		size_t append(const StringType& data, size_t startIndex = 0);
		size_t append(const StringBuilderChunk& chunk);

		void removeAt(size_t index);
		void removeEnd(size_t indexFrom);
		void remove(size_t indexFrom, size_t count);

	protected:
		size_t _currentLength{ 0 };
		size_t _size;
		StringType _data;
	};

	DECLARE_SHARED_PTR_FACTORY(StringBuilderChunk)

}