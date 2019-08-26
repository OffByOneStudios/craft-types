#pragma once
#include "syn/syn.h"

/* See section 1.1 of the manual */

namespace syn
{
	/******************************************************************************
	** InstanceLifecycle
	******************************************************************************/

	struct InstanceLifecycle
	{
		/* split into two, right hand side (lower bytes) is determined by left hand (higher bytes) */
		uintptr_t value;

		static constexpr uint8_t Offset_Deleter = 4;

		enum Flags : uint32_t
		{
			ManagerUse = 1u << 31, // Ignore all other flags

			Mask_Deleter = 0b1111 << Offset_Deleter,
			Mask_Mode = 0b1111 << 0,
		};

		enum Mode : uint8_t // actually 4
		{
			ModeUnknown = 0,
			ModeReferenceCounted = 1,
		};

		enum DeleterMode : uint8_t // actually 4
		{
			DeleterUnknown = 0,
			DeleterNoAction = 1,
			DeleterDirect = 2,
			DeleterHeader = 3,
		};

	public:
		inline InstanceLifecycle() = default;

		inline InstanceLifecycle(uintptr_t value)
			: value(value)
		{ }

	public:
		inline static InstanceLifecycle ReferenceCounted(uint32_t startingRefCount) noexcept
		{
			return InstanceLifecycle(startingRefCount) | ModeReferenceCounted;
		}

	public:
		inline uint32_t& operator*()
		{
			return (uint32_t&)value;
		}

		inline uint32_t operator*() const
		{
			return (uint32_t)value;
		}

		inline DeleterMode deleterMode()
		{
			return DeleterMode((value >> (32) & InstanceLifecycle::Mask_Deleter) >> InstanceLifecycle::Offset_Deleter);
		}

	public:
		inline InstanceLifecycle operator|(Flags f) const
		{
			return InstanceLifecycle(this->value | (uint64_t(f) << 32));
		}
		inline InstanceLifecycle operator|(Mode m) const
		{
			return InstanceLifecycle(this->value | (uint64_t(m) << 32));
		}
		inline InstanceLifecycle operator|(DeleterMode m) const
		{
			return InstanceLifecycle(this->value | (uint64_t(m) << (32 + Offset_Deleter)));
		}
	};

	inline bool operator&=(InstanceLifecycle l, InstanceLifecycle::Flags f)
	{
		return ((l.value >> 32) & f) != 0;
	}
	inline bool operator&=(InstanceLifecycle::Flags f, InstanceLifecycle l)
	{
		return ((l.value >> 32) & f) != 0;
	}
	inline bool operator&=(InstanceLifecycle l, InstanceLifecycle::Mode m)
	{
		return ((l.value >> 32) & InstanceLifecycle::Mask_Mode) == m;
	}
	inline bool operator&=(InstanceLifecycle::Mode m, InstanceLifecycle l)
	{
		return ((l.value >> 32) & InstanceLifecycle::Mask_Mode) == m;
	}
	inline bool operator&=(InstanceLifecycle l, InstanceLifecycle::DeleterMode m)
	{
		return l.deleterMode() == m;
	}
	inline bool operator&=(InstanceLifecycle::DeleterMode m, InstanceLifecycle l)
	{
		return l.deleterMode() == m;
	}

	/******************************************************************************
	** InstanceHeader
	******************************************************************************/

	struct InstanceHeader
	{
		void* memory;
		uintptr_t concrete;
		InstanceLifecycle lifecycle;
		void* manager;

	public:
		// In the default case only assume the manager is null
		inline InstanceHeader()
			: memory (nullptr)
			, concrete(0)
			, lifecycle(0)
			, manager(nullptr)
		{ }


		inline InstanceHeader(void* memory, uintptr_t concrete, InstanceLifecycle lifecycle, void* manager = nullptr)
			: memory(memory)
			, concrete(concrete)
			, lifecycle(lifecycle)
			, manager(manager)
		{ }

		// destructor does nothing, interpreter is responsible for it.
		inline ~InstanceHeader()
		{ }

	// Disallow copying and moving and assignment
	protected:

		inline InstanceHeader(const InstanceHeader& other) = default;
		inline InstanceHeader(InstanceHeader&& other) noexcept = default;
	
		inline InstanceHeader& operator=(const InstanceHeader& other) = default;
		inline InstanceHeader& operator=(InstanceHeader&& other) noexcept = default;

	public:
		inline bool isManaged() const
		{
			// manager is not null
			return manager != nullptr
				// and it isn't using the lifecycle
				&& !(lifecycle &= InstanceLifecycle::ManagerUse)
				// and it is not a deleter
				&& !(lifecycle &= InstanceLifecycle::Mask_Deleter);
		}
	};

	/******************************************************************************
	** InstanceHeaderPacked
	******************************************************************************/

	template<size_t TSize>
	struct InstanceHeaderPacked
		: InstanceHeader
	{
		std::byte object[TSize];

		inline InstanceHeaderPacked(uintptr_t concrete, InstanceLifecycle lifecycle, void* manager = nullptr)
			: InstanceHeader(&object, concrete, lifecycle, manager)
		{ }
	};

	/******************************************************************************
	** Deleter
	******************************************************************************/

	typedef void (*InstanceDirectDeleter)(void*);
	typedef void (*InstanceHeaderDeleter)(InstanceHeader*);
}
