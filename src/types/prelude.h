#pragma once
#include "common.h"

// Forward Declarations
namespace craft {
namespace types
{
	//
	// Public Formward Declarations
	//

	class Types;
	inline Types& system();

	template <typename T, typename = void> struct type;
	template<typename TType = void, typename T_ = void> struct instance;

	class Object;
	class Feature;
	class IFeatureManager;
	class Provider;
	class Aspect;

	//
	// Detail Formward Declarations
	//

	namespace _details
	{
		const size_t _offset_table_size = 1024;
		extern unsigned char* _offset_table;

		template <typename TClass, typename TValue>
		inline size_t t_offset(TValue TClass::* TMember)
		{
			return ((size_t)(&(reinterpret_cast<const TClass*>((unsigned char const*)_details::_offset_table)->*TMember)))
				- reinterpret_cast<size_t>((unsigned char const*)_details::_offset_table);
		}

		template<typename T>
		class TypeDefineHelper;

		CRAFT_TYPES_EXPORTED TypeGraphSystem* _cpp_init_graph();

		typedef void(*_fn_register_type_init)(TypeDefineHelper<void> _);
		CRAFT_TYPES_EXPORTED void _register_type_init(TypeId, _fn_register_type_init);

		struct type_impl
		{
			// This must be zero width
		private:
			CRAFT_TYPES_EXPORTED static TypeId _id;

			template<typename T, typename>
			friend struct type;
		public:
			struct _static_init : Ider<TypeId>
			{
				inline _static_init(_details::_fn_register_type_init _init)
					: Ider<TypeId>(type_impl::_id.increment())
				{
					_details::_register_type_init(_id, _init);
				}
			};
		};
	}

	//
	// type<void> Definition
	//

	template<>
	struct type<void>
	{
		// This must be zero width
	public:
		static constexpr bool isExternal = false;
		static constexpr bool isObject = false;
		static constexpr bool isFeature = false;

		inline static TypeId typeId() { return 0; }
		inline static std::string name() { return ""; }
	};
}

	using ::craft::types::type;
}
