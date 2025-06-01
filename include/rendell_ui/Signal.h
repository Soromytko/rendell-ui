#pragma once
#include <vector>
#include <unordered_map>
#include <functional>
#include <type_traits>
#include <cassert>
#include <iostream>
//TODO: Bad include
#include "../../internal/OStream.h"

namespace rendell_ui
{
	template <typename ReturnType, typename... Args>
	class Signal
	{
		using FunctionSignature = std::function<ReturnType(Args...)>;

	public:
		template<typename ClassType, typename MethodType>
		uint32_t connect(ClassType* object, MethodType method)
		{
			if (!object) {
				error << "Object pointer cannot be null." << std::endl;
				return -1;
			}

			_connections.emplace_back([object, method](Args... args) -> ReturnType
				{
					return (object->*method)(args...);
				});

			return registerConnection(_connections.size() - 1);
		}

		uint32_t connect(FunctionSignature function)
		{
			if (!function) {
				error << "Object pointer cannot be null." << std::endl;
				return -1;
			}

			_connections.push_back(function);

			return registerConnection(_connections.size() - 1);
		}

		uint32_t connect(Signal<ReturnType, Args...>& signal)
		{
			_connections.emplace_back([&signal](Args... args) -> ReturnType
				{
					return signal.emit(args...);
				});

			return registerConnection(_connections.size() - 1);
		}

		bool disconnect(uint32_t connectionId)
		{
			auto it = _map.find(connectionId);
			if (it == _map.end())
			{
				error << "Invalid connection ID " << connectionId << std::endl;
				return false;
			}

			const size_t index = it->second;
			_connections.erase(_connections.begin() + index);
			_map.erase(it);
			return true;
		}

		bool isConnected(uint32_t connectionId)
		{
			return _map.find(connectionId) != _map.end();
		}

		void emit(Args... args)
		{
			for (const auto& connection : _connections)
			{
				connection(args...);
			}
		}

	private:
		uint32_t registerConnection(size_t connectionIndex)
		{
			const uint32_t connectionId = generateConnectionId();
			assert(_map.find(connectionId) == _map.end());
			_map.insert({ connectionId, connectionIndex });
			return connectionId;
		}

		uint32_t generateConnectionId()
		{
			return _connectionIdCounter++;
		}

		uint32_t _connectionIdCounter{ 0 };
		std::vector<FunctionSignature> _connections;
		std::unordered_map<uint32_t, size_t> _map;
	};

	class Action : public Signal<void>
	{

	};
}

