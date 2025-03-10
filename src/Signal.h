#include <vector>
#include <unordered_map>
#include <functional>
#include <type_traits>
#include <cassert>

template <typename ReturnType, typename... Args>
class Signal
{
	using FunctionSignature = std::function<ReturnType(Args...)>;

public:
	template<typename ClassType, typename MethodType>
	uint32_t connect(ClassType* object, MethodType method)
	{
		if (!object) {
			std::cerr << "Error: object pointer cannot be null." << std::endl;
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
			std::cerr << "Error: object pointer cannot be null." << std::endl;
			return -1;
		}

		_connections.push_back(function);

		return registerConnection(_connections.size() - 1);
	}

	bool disconnect(uint32_t connectionId)
	{
		auto it = _map.find(connectionId);
		if (it == _map.end())
		{
			std::cerr << "ERROR::Signal: Invalid connection ID " << connectionId << std::endl;
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









//#include <iostream>
//#include <vector>
//#include <type_traits>
//
//// Helper struct для получения типов аргументов метода класса
//template <typename ClassType, typename MethodType>
//struct method_traits;
//
//template <typename ClassType, typename ReturnType, typename... Args>
//struct method_traits<ReturnType(ClassType::*)(Args...)> {
//    using return_type = ReturnType;
//    using arguments_tuple = std::tuple<Args...>;
//    static constexpr size_t arity = sizeof...(Args);
//};
//
//template <typename ClassType, typename ReturnType, typename... Args>
//struct method_traits<ReturnType(ClassType::*)(Args...) const> {
//    using return_type = ReturnType;
//    using arguments_tuple = std::tuple<Args...>;
//    static constexpr size_t arity = sizeof...(Args);
//};
//
//// Проверка совместимости типа метода класса с аргументами сигнала
//template <typename ClassType, typename MethodType, typename... SignalArgs>
//struct is_method_compatible {
//    using MethodTraits = method_traits<MethodType>;
//    using MethodArgsTuple = typename MethodTraits::arguments_tuple;
//
//    // Проверяем, что количество аргументов совпадает
//    static constexpr bool arity_match = (MethodTraits::arity == sizeof...(SignalArgs));
//
//    // Проверяем, что типы аргументов совпадают
//    template <size_t I, typename ArgType>
//    using check_arg_type = std::enable_if_t<std::is_same_v<std::tuple_element_t<I, MethodArgsTuple>, ArgType>, bool>;
//
//    template <size_t... I>
//    static constexpr bool check_arg_types(std::index_sequence<I...>) {
//        return (arity_match && ... && (check_arg_type<I, SignalArgs>::value));
//    }
//
//    static constexpr bool value = check_arg_types(std::make_index_sequence<sizeof...(SignalArgs)>());
//};
//
//
//template <typename... Args>
//class Signal {
//public:
//    template <typename ClassType, typename MethodType, typename F = MethodType> // F - псевдоним для MethodType
//    typename std::enable_if<is_method_compatible<ClassType, MethodType, Args...>::value, size_t>::type
//        connect(ClassType* obj, MethodType method) {
//        static_assert(std::is_member_function_pointer_v<MethodType>, "Method must be a member function pointer");
//        static_assert(std::is_class_v<ClassType>, "First argument must be a pointer to a class type");
//
//        // Убедимся, что тип method соответствует объявленному F
//        using ExpectedType = void(ClassType::*)(Args...); // Создаем ожидаемый тип.
//        static_assert(std::is_convertible_v<MethodType, ExpectedType>, "MethodType is not convertible to the expected type.");
//
//        connections_.emplace_back(obj, method);
//        return connection_counter_++;
//    }
//
//
//    void disconnect(size_t handle) {
//        for (size_t i = 0; i < connections_.size(); ++i) {
//            if (i == handle) {
//                connections_.erase(connections_.begin() + i);
//                return;
//            }
//        }
//    }
//
//    void emit(Args... args) {
//        for (const auto& connection : connections_) {
//            (connection.first->*connection.second)(args...); // Вызов метода
//        }
//    }
//
//private:
//    std::vector<std::pair<void*, void*>> connections_; // Храним пары: указатель на объект и указатель на метод.  void* для совместимости
//    size_t connection_counter_ = 0;
//};




//#include <iostream>
//#include <vector>
//#include <functional>
//#include <type_traits>
//
//// Helper struct для получения типов аргументов функции
//template <typename T>
//struct function_traits;
//
//template <typename R, typename... Args>
//struct function_traits<R(Args...)> {
//    using return_type = R;
//    using arguments_tuple = std::tuple<Args...>;
//    static constexpr size_t arity = sizeof...(Args);
//};
//
//template <typename ClassType, typename R, typename... Args>
//struct function_traits<R(ClassType::*)(Args...)> : function_traits<R(Args...)> {};
//
//template <typename ClassType, typename R, typename... Args>
//struct function_traits<R(ClassType::*)(Args...) const> : function_traits<R(Args...)> {};
//
//// Проверка совместимости типов (для методов классов)
//template <typename MemberFunctionPointer, typename... SignalArgs>
//struct is_member_function_compatible {
//    using FunctionTraits = function_traits<MemberFunctionPointer>;
//    using FunctionArgsTuple = typename FunctionTraits::arguments_tuple;
//
//    // Проверяем, что количество аргументов совпадает
//    static constexpr bool arity_match = (FunctionTraits::arity == sizeof...(SignalArgs));
//
//    // Проверяем, что типы аргументов совпадают (можно сделать более строгую проверку)
//    template <size_t I, typename ArgType>
//    using check_arg_type = std::enable_if_t<std::is_same_v<std::tuple_element_t<I, FunctionArgsTuple>, ArgType>, bool>;
//
//    template <size_t... I>
//    static constexpr bool check_arg_types(std::index_sequence<I...>) {
//        return (arity_match && ... && (check_arg_type<I, SignalArgs>::value));
//    }
//
//    static constexpr bool value = check_arg_types(std::make_index_sequence<sizeof...(SignalArgs)>());
//};
//
//
//template <typename... Args>
//class Signal {
//public:
//    using SignalArgsTuple = std::tuple<Args...>;
//
//    template <typename ClassType, typename ReturnType, typename... MethodArgs, typename F>
//    typename std::enable_if<
//        std::is_bind_expression_v<std::decay_t<F>>&&
//        is_member_function_compatible<ReturnType(ClassType::*)(MethodArgs...), Args...>::value,
//        size_t>::type
//        connect(F slot) {
//        static_assert(std::is_bind_expression_v<std::decay_t<F>>, "Only bound member functions are allowed.");
//
//        connections_.push_back(slot);
//        return connection_counter_++;
//    }
//
//    void disconnect(size_t handle) {
//        for (size_t i = 0; i < connections_.size(); ++i) {
//            if (i == handle) {
//                connections_.erase(connections_.begin() + i);
//                return;
//            }
//        }
//    }
//
//    void emit(Args... args) {
//        for (const auto& connection : connections_) {
//            connection(args...);
//        }
//    }
//
//private:
//    std::vector<std::function<void(Args...)>> connections_; //  Теперь храним std::function
//    size_t connection_counter_ = 0;
//};