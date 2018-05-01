#pragma once
#include "../common.h"

namespace craft {
namespace types
{

	/******************************************************************************
	** SContainer
	******************************************************************************/

	class SContainer abstract
		: public Aspect
	{
		CRAFT_TYPES_EXPORTED CRAFT_LEGACY_FEATURE_DECLARE(craft::types::SContainer, "types.container", FactoryAspectManager);

	public:
		struct Description
		{
			bool isDict;
			bool isList;
			bool isSet;
		};

		virtual Description container_desc() const = 0;

		virtual std::vector<std::string> container_keys() const = 0;
		virtual instance<> container_map(std::string key) = 0;

		virtual size_t container_size() const = 0;
		virtual instance<> container_index(size_t index) = 0;

		virtual bool container_has(instance<> value) const = 0;
	public:
	};

	/******************************************************************************
	** ContainerViaStdMap
	******************************************************************************/

	template <typename TType>
	class ContainerViaStds
		: public IAspectFactory<SContainer>
	{
	private:
		// helper union to cast pointer to member
		template<typename TClass, typename TMember>
		union _u_ptm_cast {
			TMember TClass::*member;
			void *p;
		};
	private:
		std::map<std::string, instance<>> TType::* _member_map;
		std::vector<instance<>> TType::* _member_vector;

		//
		// Init
		//
	public:
		ContainerViaStds()
		{

		}

	private:

		// Generated aspect
		class Container
			: public Implements<SContainer>::For<TType>
		{
			ContainerViaStds* _parent;
			std::map<std::string, instance<>>* _map;
			std::vector<instance<>>* _vector;
		public:
			inline Container(ContainerViaStds* parent, instance<TType> inst)
				: Implements<SContainer>::For<TType>(inst)
				, _parent(parent)
				, _map((_parent->_member_map != nullptr)
					? &(inst.get()->*(_parent->_member_map))
					: nullptr)
				, _vector((_parent->_member_vector != nullptr)
					? &(inst.get()->*(_parent->_member_vector))
					: nullptr)
			{ }

			inline virtual SContainer::Description container_desc() const override
			{
				return SContainer::Description{ _map != nullptr, _vector != nullptr, false };
			}

			inline virtual std::vector<std::string> container_keys() const override
			{
				if (_map != nullptr)
					return std::vector<std::string>(stdext::key_iterator(_map->begin()), stdext::key_iterator(_map->end()));
				return { };
			}
			inline virtual instance<> container_map(std::string key) override
			{
				if (_map != nullptr)
					return _map->at(key);
				else
					return nullptr;
			}

			virtual size_t container_size() const override
			{
				if (_map != nullptr)
					return _map->size();
				else if (_vector != nullptr)
					return _vector->size();
				else
					return 0;
			}

			virtual instance<> container_index(size_t index) override
			{
				if (_vector != nullptr)
					return _vector->at(index);
				else
					return nullptr;
			}

			virtual bool container_has(instance<> value) const override
			{
				throw type_error("Not a set");
			}
		};

		friend class Container;

	//
	// Configuration
	//
	public:
		template<typename TFeature>
		inline ContainerViaStds* withOffsetMap(std::map<std::string, instance<TFeature>> TType::* member)
		{
			//_hint = type<TFeature>::featureId();
			_member_map = reinterpret_cast<std::map<std::string, instance<>> TType::*>(member);
			return this;
		}

		template<typename TFeature>
		inline ContainerViaStds* withOffsetVector(std::vector<instance<TFeature>> TType::* member)
		{
			//_hint = type<TFeature>::featureId();
			_member_vector = reinterpret_cast<std::vector<instance<>> TType::*>(member);
			return this;
		}

	//
	// IAspectManager
	//
	public:
		inline virtual bool isMultiUseSafe() const { return true; }

		inline virtual SContainer* build(IAspectManager* man, void* inst)
		{
			return new Container(this, instance<TType>((TType*)inst));
		}
		inline virtual void destroy(SContainer* aspect)
		{
			delete (Container*)aspect;
		}

	};
}}
