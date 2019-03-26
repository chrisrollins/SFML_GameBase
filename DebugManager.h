#ifndef DEBUG_MANAGER
#define DEBUG_MANAGER

#include <iostream>
#include <set>
#include <string>
using std::cout;
using std::endl;
using std::set;
using std::string;
using std::ostream;

namespace Engine
{
	class DebugManager
	{
	public:
		enum class MessageType
		{
			RESOURCE_REPORTING,
			PERFORMANCE_REPORTING,
			ERROR_REPORTING
		};

		static inline void PrintMessage(MessageType msgType, const string& msg)
		{
#ifdef _DEBUG
			if (getEnabledMessageTypeSet().find(msgType) != getEnabledMessageTypeSet().end()) { cout << msg << endl; }
#endif
		}

		static inline void EnableMessageType(MessageType msgType)
		{
#ifdef _DEBUG
			getEnabledMessageTypeSet().insert(msgType);
#endif
		}

		static inline void DisableMessageType(MessageType msgType)
		{
#ifdef _DEBUG
			getEnabledMessageTypeSet().erase(msgType);
#endif
		}

	private:
#ifdef _DEBUG
		static set<MessageType>& getEnabledMessageTypeSet()
		{
			static set<MessageType> enabled;
			return enabled;
		}
#endif
	};
}

#endif