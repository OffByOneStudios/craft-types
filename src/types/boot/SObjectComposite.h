#pragma once
#include "../common.h"

namespace craft {
namespace types
{
	/******************************************************************************
	** SObjectComposite
	******************************************************************************/

	class SObjectComposite abstract
		: public Aspect
	{
		CRAFT_TYPES_EXPORTED CRAFT_LEGACY_FEATURE_DECLARE(craft::types::SObjectComposite, "types.attrs", FactoryAspectManager);

	public:

		typedef uintptr_t ListingId;

		static const ListingId c_BadListingId = -1;

		struct Listing
		{
			ListingId id;
			TypeId typeId;
			std::string name;

			// True if this listing supports updates
			bool runtimeTyped;
			bool isUpdateable;
		};

		virtual std::vector<Listing> const& objectComposite_listings() const = 0;
		virtual Listing objectComposite_getMeta(ListingId id) const = 0;

		virtual instance<> objectComposite_getInstance(ListingId id) = 0;

		/* T:
		Provides the objectComposite's signal for when the parts collection is updated. It has a
		couple of different call styles:

			* Add new object: when (newInst != nullptr, oldInst == nullptr).
			* Rem old object: when (newInst == nullptr, oldInst != nullptr).
			* Replace object: when (newInst != nullptr, oldInst != nullptr). Replaces old with new.

		May be null to not support updates.
		*/
		virtual Signal<void (ListingId id, instance<> newInst, instance<> oldInst)>* objectComposite_onUpdate() = 0;

	public:

		inline ListingId objectComposite_findListing(std::string name)
		{
			ListingId ret = c_BadListingId;
			auto listings = objectComposite_listings();
			for (auto it = listings.begin(); it != listings.end(); ++it)
			{
				if (it->name == name)
					ret = it->id;
			}
			return ret;
		}
	};

	/******************************************************************************
	** ObjectCompositer
	******************************************************************************/

	template <typename TType>
	class ObjectCompositer
		: public IAspectFactory<SObjectComposite>
	{
	private:
		struct ListingActual
		{
			size_t offset;
			TypeId type;
			bool isRaw;

			inline instance<> computeInstance(instance<> inst) const
			{
				return *(instance<>*)(((char *)inst.get()) + offset);
			}
			inline void* computeRaw(instance<> inst) const
			{
				return (void*)(((char *)inst.get()) + offset);
			}
		};
	private:
		std::map<SObjectComposite::ListingId, size_t> _map;
		std::vector<SObjectComposite::Listing> _listing;
		std::vector<ListingActual> _listingActual;
	private:
		// Returns listing index or throws exception
		inline size_t _index(SObjectComposite::ListingId id)
		{
			auto it = _map.find(id);

			if (it == _map.end())
				throw type_error("Bad listing id.");

			auto value = it->second;

			if (value >= _listing.size())
				throw type_error("Internal error: map does not map.");

			return value;
		}

		// Generated aspect
		class Description
			: public Implements<SObjectComposite>::For<TType>
		{
			ObjectCompositer* _parent;
		public:
			inline Description(ObjectCompositer* parent, instance<TType> inst)
				: _parent(parent)
				, Implements<SObjectComposite>::For<TType>(inst)
			{ }

			inline virtual std::vector<SObjectComposite::Listing> const& objectComposite_listings() const
			{
				return _parent->_listing;
			}
			inline virtual SObjectComposite::Listing objectComposite_getMeta(SObjectComposite::ListingId id) const
			{
				return _parent->_listing[_parent->_index(id)];
			}

			inline virtual instance<> objectComposite_getInstance(SObjectComposite::ListingId id)
			{
				auto& listing = _parent->_listingActual[_parent->_index(id)];

				if (listing.isRaw)
					return instance<void>(listing.computeRaw(this->tance()), listing.type);
				else
					return listing.computeInstance(this->tance());
			}

			inline virtual Signal<void(SObjectComposite::ListingId id, instance<> newInst, instance<> oldInst)>* objectComposite_onUpdate()
			{
				return nullptr;
			}
		};

		friend class Description;

	//
	// Configuration
	//
	private:
		inline void registerWith(
			std::string name,
			SObjectComposite::ListingId listingId,
			TypeId typeId,
			bool isRaw = false
			)
		{
			_map[listingId] = _listing.size();

			SObjectComposite::Listing l;
			l.id = listingId;
			l.name = name;
			l.typeId = typeId;
			l.runtimeTyped = typeId == types::None;
			l.isUpdateable = false;
			_listing.push_back(l);

			ListingActual a;
			a.offset = (size_t)listingId;
			a.type = typeId;
			a.isRaw = isRaw;
			_listingActual.push_back(a);
		}

	public:
		template<typename T>
		inline ObjectCompositer* withOffset(std::string name, size_t offset)
		{
			registerWith(name, offset, cpptype<T>::typeDesc());
			return this;
		}

		template<typename T>
		inline ObjectCompositer* withOffsetRaw(std::string name, size_t offset)
		{
			registerWith(name, offset, cpptype<T>::typeDesc(), true);
			return this;
		}

		inline ObjectCompositer* withOffsetDynamic(std::string name, size_t offset)
		{
			registerWith(name, offset, 0);
			return this;
		}

	//
	// IAspectManager
	//
	protected:
		inline virtual bool isMultiUseSafe() const { return true; }

		inline virtual SObjectComposite* build(IAspectManager* man, void* inst)
		{
			return new Description(this, instance<TType>((TType*)inst));
		}
		inline virtual void destroy(SObjectComposite* aspect)
		{
			delete (Description*)aspect;
		}
	};
}}
