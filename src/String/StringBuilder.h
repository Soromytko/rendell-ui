#pragma once
#include <memory>
#include <list>
#include "StringBuilderChunk.h"

namespace rendell_ui
{
	class StringBuilder final
	{
		using ChunkConstIter = std::list<StringBuilderChunkSharedPtr>::const_iterator;
	public:
		StringBuilder(const StringType& data = {}, size_t chunkSize = 16);
		~StringBuilder() = default;

		StringType toString() const;
		size_t getLength() const;

		void append(StringType&& data);
		void append(const StringType& data);
		void removeAt(size_t index);
		void remove(size_t indexFrom, size_t count = 1);

	protected:
		ChunkConstIter findChunkByItemIndex(size_t itemIndex, size_t* indexInChunk = nullptr) const;
		bool mergeChunks(ChunkConstIter chunkIt1, ChunkConstIter chunkIt2);
		bool mergeChunks(ChunkConstIter chunkIt, size_t neighbor—ount = 1);

		size_t _length{ 0 };
		size_t _chunkSize;
		std::list<StringBuilderChunkSharedPtr> _chunks;

	};

	DECLARE_SHARED_PTR_FACTORY(StringBuilder)

}
