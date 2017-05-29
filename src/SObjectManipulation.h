#pragma once
#include "common.h"

namespace craft {
	/******************************************************************************
	** SObjectManipulation
	******************************************************************************/

	class SObjectManipulation abstract
		: public Aspect
	{
		CRAFT_ASPECT_DECLARE(SObjectManipulation, "types.funcs", Factory);
	public:

		typedef uintptr_t ListingId;

		static const ListingId c_BadListingId = -1;

		struct Listing
		{
			ListingId id;
			std::string name;

			TypeId returnType;
			std::vector<TypeId> argTypes;
		};

		virtual std::vector<Listing> const& objectManipulation_listings() const = 0;
		virtual Listing objectManipulation_getMeta(ListingId id) const = 0;

		virtual instance<> objectManipulation_call(ListingId const&, std::vector<instance<>> const&) const = 0;

	public:

		inline std::vector<ListingId> objectManipulation_findListings(std::string name)
		{
			std::vector<ListingId> ret;
			auto listings = objectManipulation_listings();
			for (auto it = listings.begin(); it != listings.end(); ++it)
			{
				if (it->name == name)
					ret.push_back(it->id);
			}
			return ret;
		}
		inline ListingId objectManipulation_findListing(std::string name)
		{
			auto ret = objectManipulation_findListings(name);
			return (ret.size() == 1) ? ret[0] : c_BadListingId;
		}
	};


	/******************************************************************************
	** ObjectManipulater
	******************************************************************************/

	template <typename TType>
	class ObjectManipulater
		: public IAspectFactory<SObjectManipulation>
	{
	private:
		static constexpr size_t _c_supportedArgs = 4;

		struct ListingActual
		{
			// We use a union because it is unclear what the size of these pointers might be
			// and while the c++ standard says they should all be reinterpret castable to each
			// other, it is safer to simply union them togeather.
			union FuncPtr
			{
				void (TType::*n0)();
				void (TType::*n1)(instance<>);
				void (TType::*n2)(instance<>, instance<>);
				void (TType::*n3)(instance<>, instance<>, instance<>);
				void (TType::*n4)(instance<>, instance<>, instance<>, instance<>);

				instance<> (TType::*y0)();
				instance<> (TType::*y1)(instance<>);
				instance<> (TType::*y2)(instance<>, instance<>);
				instance<> (TType::*y3)(instance<>, instance<>, instance<>);
				instance<> (TType::*y4)(instance<>, instance<>, instance<>, instance<>);

				template <typename T>
				inline void assign(T const& v)
				{
					*((T*)(this)) = v;
				}
			};

			FuncPtr v;
			size_t argCount;
			bool hasRet;

			inline instance<> call(instance<TType> _this, std::vector<instance<>> const& args)
			{
				if (args.size() != argCount)
					throw type_error("Given argument count does not match arguments for function.");

				if (hasRet)
				{
					switch (argCount)
					{
					default: throw type_error("That amount of arguments are not supported."); break;
					case 0: return (_this.get()->*(v.y0))(); break;
					case 1: return (_this.get()->*(v.y1))(args[0]); break;
					case 2: return (_this.get()->*(v.y2))(args[0], args[1]); break;
					case 3: return (_this.get()->*(v.y3))(args[0], args[1], args[2]); break;
					case 4: return (_this.get()->*(v.y4))(args[0], args[1], args[2], args[3]); break;
					}
				}
				else
				{
					switch (argCount)
					{
					default: throw type_error("That amount of arguments are not supported."); break;
					case 0: (_this.get()->*(v.n0))(); break;
					case 1: (_this.get()->*(v.n1))(args[0]); break;
					case 2: (_this.get()->*(v.n2))(args[0], args[1]); break;
					case 3: (_this.get()->*(v.n3))(args[0], args[1], args[2]); break;
					case 4: (_this.get()->*(v.n4))(args[0], args[1], args[2], args[3]); break;
					}
					return instance<>();
				}
			}
		};
	private:
		std::vector<SObjectManipulation::Listing> _listing;
		std::vector<ListingActual> _listingActual;
	private:

		// Generated aspect
		class Description
			: public Implements<SObjectManipulation>::For<TType>
		{
			ObjectManipulater* _parent;
		public:
			inline Description(ObjectManipulater* parent, instance<TType> inst)
				: _parent(parent)
				, Implements<SObjectManipulation>::For<TType>(inst)
			{ }

			inline virtual std::vector<SObjectManipulation::Listing> const& objectManipulation_listings() const
			{
				return _parent->_listing;
			}
			inline virtual SObjectManipulation::Listing objectManipulation_getMeta(SObjectManipulation::ListingId id) const
			{
				return _parent->_listing[id];
			}

			inline virtual instance<> objectManipulation_call(SObjectManipulation::ListingId const& id, std::vector<instance<>> const& args) const
			{
				auto& listing = _parent->_listingActual[id];

				return listing.call(this->tance(), args);
			}
		};

		friend class Description;

	//
	// Configuration
	//
	private:
		inline SObjectManipulation::ListingId registerWith(
			std::string name,
			typename ListingActual::FuncPtr ptr,
			bool hasRet,
			TypeId retTypeId,
			std::vector<TypeId> const& argTypeIds
		)
		{
			SObjectManipulation::ListingId listingId = _listing.size();

			SObjectManipulation::Listing l;
			l.id = listingId;
			l.name = name;
			l.returnType = retTypeId;
			l.argTypes = argTypeIds;
			_listing.push_back(l);

			ListingActual a;
			a.v = ptr;
			a.argCount = argTypeIds.size();
			a.hasRet = hasRet;
			_listingActual.push_back(a);

			return listingId;
		}

	public:
		template<typename _TType = TType>
		inline ObjectManipulater* withMethod(std::string name, void(_TType::*mem)(void))
		{
			std::vector<TypeId> argTypes;
			typename ListingActual::FuncPtr fp;
			fp.assign((instance<>(_TType::*)())mem);

			registerWith(name, fp, false, 0, argTypes);

			return this;
		}

		template<typename _TType = TType,
			typename TRet>
		inline ObjectManipulater* withMethod(std::string name, ::craft::instance<TRet> (_TType::*mem)(void))
		{
			std::vector<TypeId> argTypes;
			typename ListingActual::FuncPtr fp;
			fp.assign((instance<>(_TType::*)())mem);

			registerWith(name, fp, true, ::craft::type<TRet>::typeId(), argTypes);

			return this;
		}

		template<typename _TType = TType,
			typename... TArgs>
			inline ObjectManipulater* withMethod(std::string name, void(_TType::*mem)(::craft::instance<TArgs>...))
		{
			static_assert(sizeof...(TArgs) <= _c_supportedArgs, "ObjectManipulater::withMethod does not support that many arguments");

			std::vector<TypeId> argTypes = { (::craft::type<TArgs>::typeId())... };
			typename ListingActual::FuncPtr fp;
			fp.assign((void (_TType::*)(typename stdext::typechange<TArgs, instance<>>::type...))mem);

			registerWith(name, fp, false, 0, argTypes);

			return this;
		}

		template<typename _TType = TType,
			typename TRet,
			typename... TArgs>
			inline ObjectManipulater* withMethod(std::string name, ::craft::instance<TRet>(_TType::*mem)(::craft::instance<TArgs>...))
		{
			static_assert(sizeof...(TArgs) <= _c_supportedArgs, "ObjectManipulater::withMethod does not support that many arguments");

			std::vector<TypeId> argTypes = { (::craft::type<TArgs>::typeId())... };
			typename ListingActual::FuncPtr fp;
			fp.assign((instance<>(_TType::*)(typename stdext::typechange<TArgs, instance<>>::type...))mem);

			registerWith(name, fp, true, ::craft::type<TRet>::typeId(), argTypes);

			return this;
		}

	//
	// IAspectManager
	//
	protected:
		inline virtual bool isMultiUseSafe() const { return true; }

		inline virtual SObjectManipulation* build(IAspectManager* man, void* inst)
		{
			return new Description(this, instance<TType>((TType*)inst));
		}
		inline virtual void destroy(SObjectManipulation* aspect)
		{
			delete (Description*)aspect;
		}
	};
}
