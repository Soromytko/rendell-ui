#include "StringBuilder.h"
#include <algorithm>
#include <cassert>
#include <iostream>

namespace rendell_ui
{
	StringBuilder::StringBuilder(const StringType& data, size_t chunkSize)
	{
		assert(chunkSize > 0);
		_chunkSize = chunkSize;
		_chunks.push_back(makeStringBuilderChunk(chunkSize));

		if (data.size() > 0)
		{
			append(data);
		}
	}

	StringType StringBuilder::toString() const
	{
		StringType result{};
		result.reserve(getLength());
		for (const StringBuilderChunkSharedPtr& chunk : _chunks)
		{
			result += chunk->toString();
		}
		return result;
	}

	size_t StringBuilder::getLength() const
	{
		return _length;
		size_t result = 0;
		for (const StringBuilderChunkSharedPtr& chunk : _chunks)
		{
			result += chunk->getCurrentLength();
		}
		return result;
	}

	void StringBuilder::append(StringType&& data)
	{
		if (data.size() == 0)
		{
			return;
		}

		const StringBuilderChunkSharedPtr lastChunk = _chunks.back();
		const size_t lastChunkRemainingLength = lastChunk->getRemainingLength();

		size_t remaningDataSize = lastChunk->append(data);
		_length += data.size() - remaningDataSize;
		while (remaningDataSize != 0)
		{
			StringBuilderChunkSharedPtr chunk = makeStringBuilderChunk(_chunkSize);
			const size_t startIndex = data.size() - remaningDataSize;
			remaningDataSize = chunk->append(data, startIndex);
			_length += data.size() - startIndex - remaningDataSize;
			_chunks.push_back(chunk);
		}
	}

	void StringBuilder::append(const StringType& data)
	{
		StringType chunkData = data;
		append(std::move(chunkData));
	}

	void StringBuilder::insert(size_t index, const StringType& value)
	{
		size_t indexInChunk;
		ChunkConstIter chunkIt = findChunkByItemIndex(index, &indexInChunk);
		StringBuilderChunkSharedPtr chunk = *chunkIt;

		if (chunk->getRemainingLength() >= value.size())
		{
			const size_t remainingDataLength = chunk->insert(indexInChunk, value);
			assert(remainingDataLength == 0);
			return;
		}

		const StringType& subData = chunk->getSubData(indexInChunk);
		chunk->remove(indexInChunk, chunk->getCurrentLength());
		size_t remainingDataLength = chunk->insert(indexInChunk, value);

		while (remainingDataLength > 0)
		{
			const size_t dataOffsetIndex = value.size() - remainingDataLength;
			StringBuilderChunkSharedPtr newChunk = makeStringBuilderChunk(_chunkSize);
			remainingDataLength = newChunk->insert(0, value, dataOffsetIndex);

			if (ChunkConstIter nextChunkIt = std::next(chunkIt); nextChunkIt != _chunks.end())
			{
				_chunks.insert(nextChunkIt, newChunk);
			}
			else
			{
				_chunks.push_back(newChunk);
			}
			chunkIt++;
		}

	}

	void StringBuilder::removeAt(size_t index)
	{
		assert(index < _length);
		size_t indexInChunk;
		auto it = findChunkByItemIndex(index, &indexInChunk);
		assert(it != _chunks.end());
		it->operator->()->removeAt(indexInChunk);

		if (auto nextChunkIt = it++; nextChunkIt != _chunks.end())
		{
			mergeChunks(it, nextChunkIt);
		}
	}

	void StringBuilder::remove(size_t indexFrom, size_t count)
	{
		assert(indexFrom < _length);
		assert(indexFrom + count <= _length);

		std::vector<ChunkConstIter> affectedChunkIters;

		size_t indexInChunk;
		size_t remainingCount = count;
		for (auto it = findChunkByItemIndex(indexFrom, &indexInChunk); it != _chunks.end(); it++)
		{
			bool isBreak = false;

			StringBuilderChunkSharedPtr currentChunk = *it;
			if (indexInChunk + remainingCount > currentChunk->getCurrentLength())
			{
				remainingCount -= currentChunk->getCurrentLength() - indexInChunk;
				currentChunk->removeEnd(indexInChunk);
				indexInChunk = 0;
			}
			else
			{
				currentChunk->remove(indexInChunk, remainingCount);
				isBreak = true;
			}

			if (currentChunk->getCurrentLength() == 0)
			{
				auto itForErase = it;
				it--;
				_chunks.erase(itForErase);
			}
			else
			{
				affectedChunkIters.push_back(it);
			}

			if (isBreak)
			{
				break;
			}
		}


		if (affectedChunkIters.size() >= 2)
		{
			mergeChunks(affectedChunkIters[0], affectedChunkIters.size());
		}
	}

	StringBuilder::ChunkConstIter StringBuilder::findChunkByItemIndex(size_t itemIndex, size_t* indexInChunk) const
	{
		size_t currentIndex{};
		for (auto it = _chunks.begin(); it != _chunks.end(); it++)
		{
			const size_t newCurrentIndex = currentIndex + (*it)->getCurrentLength();
			if (newCurrentIndex > itemIndex)
			{
				if (indexInChunk != nullptr)
				{
					*indexInChunk = itemIndex - currentIndex;
				}
				return it;
			}
			else
			{
				currentIndex = newCurrentIndex;
			}
		}
		return _chunks.end();
	}

	bool StringBuilder::mergeChunks(ChunkConstIter chunkIt1, ChunkConstIter chunkIt2)
	{
		auto chunk1 = *chunkIt1;
		auto chunk2 = *chunkIt2;

		if (chunk1->getCurrentLength() + chunk2->getCurrentLength() <= _chunkSize)
		{
			assert(chunk1->append(*chunk2) == 0);
			_chunks.erase(chunkIt2);
			return true;
		}

		return false;
	}

	bool StringBuilder::mergeChunks(ChunkConstIter chunkIt, size_t neighborCount)
	{
		if (neighborCount < 1)
		{
			return false;
		}

		bool result = false;
		size_t currentCount = neighborCount;
		ChunkConstIter it1 = chunkIt;
		for (size_t i = 0; i < currentCount;)
		{
			if (ChunkConstIter it2 = it1; ++it2 != _chunks.end())
			{
				if (mergeChunks(it1, it2))
				{
					currentCount--;
					result = true;
				}
				else
				{
					it1++;
					i++;
				}
			}
			else
			{
				return result;
			}
		}

		return result;
	}


}
