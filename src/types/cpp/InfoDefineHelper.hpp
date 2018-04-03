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

		public:

		};
	}
}}
