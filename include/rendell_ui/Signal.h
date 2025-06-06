#pragma once
#include <vector>
#include <unordered_map>
#include <functional>
#include <type_traits>
#include <cassert>
#include <iostream>
//TODO: Bad include
#include "../../internal/logging.h"

namespace rendell_ui
{
	using signal_connection_id_t = size_t;

	template <typename ReturnType, typename... Args>
	class Signal
	{
		using FunctionSignature = std::function<ReturnType(Args...)>;

	public:
		template<typename ClassType, typename MethodType>
		signal_connection_id_t connect(ClassType* object, MethodType method)
		{
			if (!object) {
				RUI_ERROR << "Object pointer cannot be null";
				return -1;
			}

			_connections.emplace_back([object, method](Args... args) -> ReturnType
				{
					return (object->*method)(args...);
				});

			return registerConnection(_connections.size() - 1);
		}

		signal_connection_id_t connect(FunctionSignature function)
		{
			if (!function) {
				RUI_ERROR << "Object pointer cannot be null";
				return -1;
			}

			_connections.push_back(function);

			return registerConnection(_connections.size() - 1);
		}

		signal_connection_id_t connect(Signal<ReturnType, Args...>& signal)
		{
			_connections.emplace_back([&signal](Args... args) -> ReturnType
				{
					return signal.emit(args...);
				});

			return registerConnection(_connections.size() - 1);
		}

		bool disconnect(signal_connection_id_t connectionId)
		{
			auto it = _map.find(connectionId);
			if (it == _map.end())
			{
				RUI_ERROR << "Invalid connection ID " << connectionId;
				return false;
			}

			const size_t index = it->second;
			_connections.erase(_connections.begin() + index);
			_map.erase(it);
			return true;
		}

		bool isConnected(signal_connection_id_t connectionId)
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
		signal_connection_id_t registerConnection(size_t connectionIndex)
		{
			const signal_connection_id_t connectionId = generateConnectionId();
			assert(_map.find(connectionId) == _map.end());
			_map.insert({ connectionId, connectionIndex });
			return connectionId;
		}

		signal_connection_id_t generateConnectionId()
		{
			return _connectionIdCounter++;
		}

		signal_connection_id_t _connectionIdCounter{ 0 };
		std::vector<FunctionSignature> _connections;
		std::unordered_map<signal_connection_id_t, size_t> _map;
	};

	class Action : public Signal<void>
	{

	};
}

