#pragma once
#include "../common.h"

namespace craft {
namespace types
{
	/******************************************************************************
	** Type Id Helper
	******************************************************************************/

	struct TypeId final
	{
		// TODO rename this type?
		Graph::Node* node;

		inline TypeId() : node(nullptr) {}
		inline TypeId(Graph::Node* n) : node(n) {}
		inline TypeId(uintptr_t v) : node((Graph::Node*)v) {}

		// From IdValue, depreciate this:
		inline explicit operator uintptr_t() const { return (uintptr_t)node; }
		
		inline bool operator <(TypeId const& that) const { return this->node < that.node; }
		inline bool operator >(TypeId const& that) const { return this->node > that.node; }
		inline bool operator ==(TypeId const& that) const { return this->node == that.node; }
		inline bool operator !=(TypeId const& that) const { return this->node != that.node; }

		// Helpers defined here
		template<typename TType>
		inline bool isType()
		{
			const TypeId that_type = types::cpptype<TType>::typeDesc();
			return that_type.node != nullptr && this->node == that_type.node;
		}

		inline bool isType(TypeId that_type)
		{
			return that_type.node != nullptr && this->node == that_type.node;
		}

		//
		// Defined in cpp_interface.h
		//
		template<typename TFeature> inline bool hasFeature() const;
		template<typename TFeature> inline TFeature* getFeature() const;

		//
		// Defined in to_string.cpp
		//
		CRAFT_TYPES_EXPORTED std::string toString(bool verbose = true) const;
	};

	const static TypeId None = nullptr;

	inline std::ostream & operator<<(std::ostream & s, TypeId const & v) { s << v.toString(); return s; }

	/******************************************************************************
	** Identifiers
	******************************************************************************/

	// TODO convert to index
	class Identifiers final
	{
	public:
		struct Record
		{
			TypeId node;
			void* desc;
		};

	private:

		struct _Data
		{
			std::atomic<int> refcount;
			std::recursive_mutex operation;

			plf::colony <Record> types;
			std::map<void*, size_t> types_byPtr;
		};

		_Data* _contents;

	// 
	// Lifecycle
	//
	public:
		CRAFT_TYPES_EXPORTED Identifiers();
		CRAFT_TYPES_EXPORTED Identifiers(Identifiers const&);
		CRAFT_TYPES_EXPORTED ~Identifiers();

	//
	// Registry
	//
	public:
		CRAFT_TYPES_EXPORTED size_t count() const;

		CRAFT_TYPES_EXPORTED TypeId add(Graph::Node* node_ptr);

		CRAFT_TYPES_EXPORTED Record const& get(TypeId id) const;
		CRAFT_TYPES_EXPORTED Record const& get(void* ptr) const;

		CRAFT_TYPES_EXPORTED Record const& getByIndex(size_t index) const;
	};
}}
