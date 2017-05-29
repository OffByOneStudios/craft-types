#pragma once
#include "common.h"

namespace craft {

	/******************************************************************************
	** PIdentifier
	******************************************************************************/

	/* T:
	Provides an interface for constructing types.

	Notes:
		* ListingId 0 is the default constructor.
	*/

	class PConstructor abstract
		: public Provider
	{
		CRAFT_PROVIDER_DECLARE(PConstructor, "types.constructor", Singleton)
	public:
		typedef uintptr_t ListingId;

		static const ListingId c_BadListingId = -1;

		struct Listing
		{
			ListingId id;

			std::vector<TypeId> argTypes;
		};

		virtual std::vector<Listing> const& listings() const = 0;
		virtual instance<> construct(ListingId = 0, std::vector<instance<>> = {}) const = 0;

	public:
		inline bool hasDefault() const { auto l = listings(); return l.size() != 0 && l[0].id == 0; }
	};

	/******************************************************************************
	** DefaultConstructor
	******************************************************************************/

	namespace _details
	{
		template <typename T>
		class DefaultConstructor
			: public Implements<PConstructor>::For<T>
		{
			bool _hasDefault;

			std::vector<std::function<instance<> (std::vector<instance<>> const&)>> _functions;
			std::vector<PConstructor::Listing> _listings;

			// if /*constexpr*/ (std::is_default_constructible<T>::value)
			template <typename _T = T,
				typename std::enable_if< std::is_default_constructible<_T>::value  >::type* = nullptr>
			inline void _defaultConstructor(size_t& id)
			{
				_hasDefault = true;
				_listings.push_back({ id++,{} });
				_functions.push_back([](std::vector<instance<>> const& _) { return instance<T>::make(); });
			}
			template <typename _T = T,
				typename std::enable_if< !std::is_default_constructible<_T>::value  >::type* = nullptr>
			inline void _defaultConstructor(size_t& id)
			{
				_hasDefault = false;
			}
			// end
			

		public:
			inline DefaultConstructor()
			{
				size_t id = 0;
				_defaultConstructor(id);
			}

			inline virtual std::vector<PConstructor::Listing> const& listings() const
			{
				return _listings;
			}

			inline virtual instance<> construct(PConstructor::ListingId id = 0, std::vector<instance<>> args = {}) const
			{
				if (!_hasDefault)
					id -= 1;

				if (id < 0 || id >= _listings.size())
					throw new type_error("Bad listing index.");
				
				PConstructor::Listing const& con = _listings[id];

				if (con.argTypes.size() == args.size())
				{
					return _functions[id](args);
				}

				return instance<>();
			}
		};
	}
}
