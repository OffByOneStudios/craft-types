#pragma once
#include "../common.h"
#include "../core.h"

namespace craft {
namespace types
{
	namespace cpp
	{
		template<typename TInfoType>
		class InfoDefineHelper
		{
		private:
			info_desc* _id;
			InfoDefineHelper* _parent;

			InfoDefineHelper(info_desc* id, InfoDefineHelper* parent = nullptr)
				: _id(id), _parent(parent)
			{
			}

			InfoDefineHelper(InfoDefineHelper const&) = default;

			friend class ::craft::types::CppSystem;

		public:

			// Basic function
			template<
				typename T,
				typename TInfoType_ = TInfoType,
				typename std::enable_if< stdext::is_specialization<TInfoType_, cpp::Multimethod>::value >::type* = nullptr>
				inline void add_method(T actual)
			{
				((TInfoType_*)(_id->repr))->add(actual);
			}

			template<
				typename T,
				typename TTemp,
				typename TInfoType_ = TInfoType,
				typename std::enable_if< stdext::is_specialization<TInfoType_, cpp::Multimethod>::value >::type* = nullptr>
				inline void add_method_lambdaHack(TTemp actual)
			{
				T temp = actual;
				((TInfoType_*)(_id->repr))->add(temp);
			}

		public:

		};
	}
}}
