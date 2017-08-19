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

	/******************************************************************************
	** Type Id Helper
	******************************************************************************/

	struct TypeId
	{
		size_t id;

		inline TypeId() : id(0) { }
		inline TypeId(size_t const& v) : id(v) { }
		inline operator size_t() const { return id; }
		inline TypeId operator ++(int i) { size_t v = id; id++; return v; }

		//
		// Defined in Types.h
		//
		template<typename TFeature> inline bool hasFeature() const;
		template<typename TFeature> inline TFeature* getFeature() const;

		//
		// Defined in to_string.h
		//
		CRAFT_TYPE_EXPORTED std::string toString(bool verbose = true) const;
	};

	const static TypeId None = 0;

	inline bool operator <(TypeId const& _this, TypeId const& _that) { return _this.id < _that.id; }
	inline bool operator >(TypeId const& _this, TypeId const& _that) { return _this.id > _that.id; }
	inline bool operator ==(TypeId const& _this, TypeId const& _that) { return _this.id == _that.id; }
	inline bool operator !=(TypeId const& _this, TypeId const& _that) { return _this.id != _that.id; }

	inline std::ostream & operator<<(std::ostream & s, TypeId const & v) { s << v.toString(); return s; }

	/******************************************************************************
	** Feature Id Helper
	******************************************************************************/

	struct FeatureId
	{
		size_t id;

		inline FeatureId() : id(0) { }
		inline FeatureId(size_t const& v) : id(v) { }
		inline operator size_t() const { return id; }
		inline FeatureId operator ++(int i) { size_t v = id; id++; return v; }

		//
		// Defined in Types.hpp
		//
		inline IFeatureManager* getManager() const;

		//
		// Defined in to_string.h
		//
		CRAFT_TYPE_EXPORTED std::string toString(bool verbose = true) const;
	};

	const static FeatureId NoFeature = 0;

	inline bool operator <(FeatureId const& _this, FeatureId const& _that) { return _this.id < _that.id; }
	inline bool operator >(FeatureId const& _this, FeatureId const& _that) { return _this.id > _that.id; }
	inline bool operator ==(FeatureId const& _this, FeatureId const& _that) { return _this.id == _that.id; }
	inline bool operator !=(FeatureId const& _this, FeatureId const& _that) { return _this.id != _that.id; }

	inline std::ostream & operator<<(std::ostream & s, FeatureId const & v) { s << v.toString(); return s; }

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
		class ObjectDefineHelper;

		typedef void(*_fn_register_type_init)(ObjectDefineHelper<void> _);
		void CRAFT_TYPE_EXPORTED _register_type_init(TypeId, _fn_register_type_init);

		typedef void(*_fn_register_feature_init)();
		void CRAFT_TYPE_EXPORTED _register_feature_init(FeatureId, _fn_register_feature_init);

		struct type_impl
		{
			// This must be zero width
		private:
			CRAFT_TYPE_EXPORTED static TypeId _id;

			template<typename T, typename>
			friend struct type;
		public:
			struct _static_init : Ider<TypeId>
			{
				inline _static_init(_details::_fn_register_type_init _init)
					: Ider<TypeId>(type_impl::_id++)
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
		inline static FeatureId featureId() { return 0; }
		inline static std::string name() { return ""; }
	};
}

	using ::craft::types::type;
}
