#pragma once
#include "common.h"
#include "core.h"

namespace craft {
namespace types
{
	class IContextQueryable abstract
	{
	public:
		virtual std::shared_ptr<IContextQueryable> byName(std::string const&) const = 0;
		virtual std::shared_ptr<IContextQueryable> byFeature(FeatureId const&) const = 0;
		virtual std::shared_ptr<IContextQueryable> byType(TypeId const&) const = 0;

		virtual std::set<instance<>> objects() const = 0;
		virtual instance<> prime() const = 0;

		inline instance<> only() const
		{
			auto res = this->objects();
			return res.size() == 1 ? *res.begin() : nullptr;
		}
	};

	/******************************************************************************
	** Context
	******************************************************************************/

	namespace _details
	{
		class _ContextQueryable;
	}

	class Context
		: public std::enable_shared_from_this<Context>
		, public virtual Object
		, public IContextQueryable
	{
		CRAFT_OBJECT_DECLARE(craft::types::Context);

		std::set<instance<>> _all;
		instance<> _prime;

		std::map<TypeId, std::set<instance<>>> _objectsByType;
		std::map<FeatureId, std::set<instance<>>> _objectsByFeature;
		std::map<std::string, std::set<instance<>>> _objectsByName;

		std::map<TypeId, instance<>> _primesByType;
		std::map<FeatureId, instance<>> _primesByFeature;
		std::map<std::string, instance<>> _primesByName;

		std::shared_ptr<Context> _parent;

		bool _finalized;

		friend class _details::_ContextQueryable;
	public:
		CRAFT_TYPES_EXPORTED Context(
			std::shared_ptr<Context> parent = std::shared_ptr<Context>(),
			instance<> prime = nullptr
		);

		CRAFT_TYPES_EXPORTED std::shared_ptr<Context> copy() const;
		CRAFT_TYPES_EXPORTED std::shared_ptr<Context> parent() const;

		CRAFT_TYPES_EXPORTED void addOnName(std::string const& name, instance<> obj);
		CRAFT_TYPES_EXPORTED void addOnType(TypeId t_id, instance<> obj);
		CRAFT_TYPES_EXPORTED void addOnFeature(FeatureId i_id, instance<> obj);
		template<typename T,
			typename std::enable_if<type<T>::isObject>::type* = nullptr>
		inline void add(instance<T> obj)
		{
			this->addOnType(type<T>::typeId(), static_cast<instance<>>(obj));
		}
		template<typename T,
			typename std::enable_if<type<T>::isFeature>::type* = nullptr>
		inline void add(instance<T> obj)
		{
			this->addOnFeature(type<T>::featureId(), obj);
		}

		CRAFT_TYPES_EXPORTED void promoteOnName(std::string const& name, instance<> obj);
		CRAFT_TYPES_EXPORTED void promoteOnType(TypeId t_id, instance<> obj);
		CRAFT_TYPES_EXPORTED void promoteOnFeature(FeatureId i_id, instance<> obj);
		template<typename T,
			typename std::enable_if<type<T>::isObject>::type* = nullptr>
			inline void promote(instance<T> obj)
		{
			this->promoteOnType(type<T>::typeId(), static_cast<instance<>>(obj));
		}
		template<typename T,
			typename std::enable_if<type<T>::isFeature>::type* = nullptr>
		inline void promote(instance<> obj)
		{
			this->promoteOnFeature(type<T>::featureId(), obj);
		}

		CRAFT_TYPES_EXPORTED void finalize();
		CRAFT_TYPES_EXPORTED bool finalized() const;

		CRAFT_TYPES_EXPORTED virtual std::shared_ptr<IContextQueryable> byName(std::string const&) const override;
		CRAFT_TYPES_EXPORTED virtual std::shared_ptr<IContextQueryable> byType(TypeId const&) const override;
		CRAFT_TYPES_EXPORTED virtual std::shared_ptr<IContextQueryable> byFeature(FeatureId const&) const override;
		template<typename T,
			typename std::enable_if<type<T>::isObject>::type* = nullptr>
			inline std::shared_ptr<IContextQueryable> by()
		{
			return this->byType(type<T>::typeId());
		}
		template<typename T,
			typename std::enable_if<type<T>::isFeature>::type* = nullptr>
			inline std::shared_ptr<IContextQueryable> by()
		{
			return this->byFeature(type<T>::featureId());
		}

		CRAFT_TYPES_EXPORTED virtual std::set<instance<>> objects() const override;
		CRAFT_TYPES_EXPORTED virtual instance<> prime() const override;
	};
}}
