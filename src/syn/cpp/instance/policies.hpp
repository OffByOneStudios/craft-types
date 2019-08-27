#pragma once
#include "syn/syn.h"

// TODO: make an extra asserts define for this chunk of code

namespace syn
{
	// todo, move this
	namespace _details
	{
		template < template <typename...> class base,typename derived>
		struct is_base_of_template_impl
		{
			template<typename... Ts>
			static constexpr std::true_type  test(const base<Ts...> *);
			static constexpr std::false_type test(...);
			using type = decltype(test(std::declval<derived*>()));
		};

		template < template <typename...> class base,typename derived>
		using is_base_of_template = typename is_base_of_template_impl<base,derived>::type;
	}

	namespace instance_policy
	{
		// The reference counted memory policy
		class ReferenceCounted
		{
		public:
			template<typename TBaseForm>
			struct InstanceForm
				: public TBaseForm
			{
				using BaseForm = TBaseForm;
				using BaseForm::BaseForm;

				static_assert(std::is_base_of_v<InstanceChunkHeader, BaseForm>,
					"syn::instance_policy::ReferenceCounted requires main header.");
			};

		public:
			template<typename TBaseLibrary>
			struct InstanceLibrary
				: public TBaseLibrary
			{
				using BaseLibrary = TBaseLibrary;
				using BaseLibrary::BaseLibrary;

				template<typename TDst, typename TSrc, typename Enable> friend struct copy;
				template<typename TDst, typename TSrc, typename Enable> friend struct move;

			public:
				inline ~InstanceLibrary()
				{
					if (this->_header != nullptr)
						decref();
				}

			public:
				inline InstanceLibrary(InstanceHeader* hdr)
					: BaseLibrary (hdr)
				{
					assert((this->_header == nullptr || (this->_header->lifecycle &= InstanceLifecycle::ModeReferenceCounted))
						&& "Violation of instance template invariant");
					
					if (this->_header != nullptr)
						incref();
				}

			public:
				inline TypeId typeId() const
				{
					return (TypeId) this->_header->concrete;
				}

				uint32_t incref()
				{
					assert(this->_header != nullptr && "Can only incref live instances");
					assert((this->_header->lifecycle &= InstanceLifecycle::ModeReferenceCounted)
						&& "Violation of instance template invariant");

					return *(this->_header->lifecycle) += 1;
				}

				uint32_t decref()
				{
					assert(this->_header != nullptr && "Can only decref live instances");
					assert((this->_header->lifecycle &= InstanceLifecycle::ModeReferenceCounted)
						&& "Violation of instance template invariant");

					auto ref = *(this->_header->lifecycle) -= 1;
					if (ref == 0)
						this->_destroy();
					return ref;
				}

				// < 0 for "not ref counted"
				inline int64_t refCount() const
				{
					if (this->_header->lifecycle &= InstanceLifecycle::ModeReferenceCounted)
						return *this->_header->lifecycle;
					else
						return -1;
				}
			};
		};

		template<typename TDst, typename TSrc>
		struct copy<TDst, TSrc, typename std::enable_if<
			_details::is_base_of_template<ReferenceCounted::InstanceLibrary, TDst>::value
			&& _details::is_base_of_template<ReferenceCounted::InstanceLibrary, TSrc>::value, void>::type>
		{
			inline static void _(TDst& dst, TSrc const& src)
			{
				if (dst._header != nullptr)
					dst.decref();
				dst._header = src._header;
				dst.incref();
			}
		};
		template<typename TDst, typename TSrc>
		struct move<TDst, TSrc, typename std::enable_if<
			_details::is_base_of_template<ReferenceCounted::InstanceLibrary, TDst>::value
			&& _details::is_base_of_template<ReferenceCounted::InstanceLibrary, TSrc>::value, void>::type>
		{
			inline static void _(TDst& dst, TSrc&& src)
			{
				std::swap(dst._header, src._header);
			}
		};


		class CppBase
		{
		public:
			template<typename TBaseForm>
			struct InstanceForm
				: public TBaseForm
			{
				using BaseForm = TBaseForm;
				using BaseForm::BaseForm;

				static_assert(std::is_base_of_v<InstanceChunkHeader, BaseForm>,
					"syn::instance_policy::CppBase requires main header.");

			protected:
				inline bool _destroy_with_manager()
				{
					if (this->_header->isManaged())
					{
						// TODO: run manager
						return true;
					}
					return false;
				}
				inline bool _destroy_with_deleter()
				{
					if (this->_header->lifecycle &= InstanceLifecycle::Mask_Deleter)
					{
						switch (this->_header->lifecycle.deleterMode())
						{
							case InstanceLifecycle::DeleterNoAction:
								return true;
							case InstanceLifecycle::DeleterDirect:
								// intentially casting this to a pointer to a function pointer
								// function pointers are not guarnteed to fit in pointers
								// but pointers to function pointers are.
								(**reinterpret_cast<InstanceDirectDeleter*>(this->_header->manager))(this->_header->memory);
								return true;
							case InstanceLifecycle::DeleterHeader:
								// ditto above
								(**reinterpret_cast<InstanceHeaderDeleter*>(this->_header->manager))(this->_header);
								return true;
							default:
								assert(false && "unknown deleter");
						}
					}
					return false;
				}
				inline bool _destroy_with_graph()
				{
					return false;
				} 

				// In theory this code path should rarely be run, the last surviving instance should never be an any
				inline void _destroy()
				{
					if (!_destroy_with_manager()
						&& !_destroy_with_deleter()
						&& !_destroy_with_graph())
					{
						assert(false && "memory leak, don't void store undeletable types");
					}

					delete this->_header;
					this->_header = nullptr;
				}
			};

		public:
			template<typename TBaseLibrary>
			struct InstanceLibrary
				: public TBaseLibrary
			{
				using BaseLibrary = TBaseLibrary;
				using BaseLibrary::BaseLibrary;

				inline void const* get() const
				{
					if (this->_header == nullptr)
						return nullptr;
					return this->_header->memory;
				}

				inline void* get()
				{
					if (this->_header == nullptr)
						return nullptr;
					return this->_header->memory;
				}

				inline bool isNull() const
				{
					return get() == nullptr;
				}

				inline operator bool() const
				{
					return !isNull();
				}
			};
		};



		template<typename TType>
		class CppTypedObject
			: CppBase
		{
		public:
			template <typename TBase>
			using InstanceFormBase
				= typename CppBase::template InstanceForm
				< TBase >;

			template<typename TBaseForm>
			struct InstanceForm
				: public InstanceFormBase<TBaseForm>
			{
				using BaseForm = InstanceFormBase<TBaseForm>;
				using BaseForm::BaseForm;

			protected:
				inline void _destroy()
				{
					if (!this->_destroy_with_manager()
						&& !this->_destroy_with_deleter())
					{
						delete (TType*)this->_header->memory;
					}
					
					delete this->_header;
					this->_header = nullptr;
				}
			};

		public:
			template <typename TBase>
			using InstanceLibraryBase
				= typename CppBase::template InstanceLibrary
				< TBase >;

			template<typename TBaseLibrary>
			struct InstanceLibrary
				: public InstanceLibraryBase<TBaseLibrary>
			{
				using BaseLibrary = InstanceLibraryBase<TBaseLibrary>;
				using BaseLibrary::BaseLibrary;

			public:
				inline TType const* get() const
				{
					if (this->_header == nullptr)
						return nullptr;
					return reinterpret_cast<TType const *>(this->_header->memory);
				}

				inline TType* get()
				{
					if (this->_header == nullptr)
						return nullptr;
					return reinterpret_cast<TType*>(this->_header->memory);
				}

				inline TType const& operator*() const
				{
					return *get();
				}

				inline TType& operator*()
				{
					return *get();
				}

				inline TType const* operator->() const
				{
					return get();
				}

				inline TType* operator->()
				{
					return get();
				}

			protected:
				inline static void _deleter(void* p)
				{
					delete (TType*)p;
				}

				inline static InstanceDirectDeleter _deleterPtr = &_deleter;

			public:
				template<typename... TArgs>
				inline static instance<TType> make(TArgs &&... args)
				{
					auto ptr = new TType(std::forward<TArgs>(args)...);
					return {
						new InstanceHeader(
							reinterpret_cast<void*>(ptr),
							(uintptr_t)syn::type<TType>::desc().asId(),
							InstanceLifecycle::ReferenceCounted(0) | InstanceLifecycle::DeleterDirect, 
							reinterpret_cast<void*>(&_deleterPtr)
						)};
				}
			};
		};



		template <>
		class CppReferenceCounted<void>
			: CppBase
			, ReferenceCounted
		{
		public:
			template <typename TBase>
			using InstanceFormBase
				= typename ReferenceCounted::template InstanceForm
				< typename CppBase::template InstanceForm
				< TBase >>;

			template<typename TBaseForm>
			struct InstanceForm
				: public InstanceFormBase<TBaseForm>
			{
				using BaseForm = InstanceFormBase<TBaseForm>;
				using BaseForm::BaseForm;
			};

		public:
			template <typename TBase>
			using InstanceLibraryBase
				= typename ReferenceCounted::template InstanceLibrary
				< typename CppBase::template InstanceLibrary
				< TBase >>;

			template<typename TBaseLibrary>
			struct InstanceLibrary
				: public InstanceLibraryBase<TBaseLibrary>
			{
				using BaseLibrary = InstanceLibraryBase<TBaseLibrary>;
				using BaseLibrary::BaseLibrary;
			};

		public:
			struct InstanceFormFactor
				: public InstanceChunkHeader
			{
				InstanceFormFactor()
					: InstanceChunkHeader(nullptr)
					{ }

				InstanceFormFactor(InstanceHeader* header)
					: InstanceChunkHeader(header)
					{ }
			};

			using FinalForm = InstanceForm<InstanceFormFactor>;

			using FinalInstance = InstanceLibrary<FinalForm>;
		};

		template <typename TType>
		class CppReferenceCounted<TType,
			typename std::enable_if_t<std::is_object_v<TType>>>
			: CppTypedObject<TType>
			, ReferenceCounted
		{
		public:
			template <typename TBase>
			using InstanceFormBase
				= typename ReferenceCounted::template InstanceForm
				< typename CppTypedObject<TType>::template InstanceForm
				< TBase >>;

			template<typename TBaseForm>
			struct InstanceForm
				: public InstanceFormBase<TBaseForm>
			{
				using BaseForm = InstanceFormBase<TBaseForm>;
				using BaseForm::BaseForm;
			};

		public:
			template <typename TBase>
			using InstanceLibraryBase
				= typename ReferenceCounted::template InstanceLibrary
				< typename CppTypedObject<TType>::template InstanceLibrary
				< TBase >>;

			template<typename TBase>
			struct InstanceLibrary
				: public InstanceLibraryBase<TBase>
			{
				using Base = InstanceLibraryBase<TBase>;
				using Base::Base;
			};

		public:
			struct InstanceFormFactor
				: public InstanceChunkHeader
			{
				InstanceFormFactor()
					: InstanceChunkHeader { nullptr }
				{ }

				InstanceFormFactor(InstanceHeader* hdr)
					: InstanceChunkHeader { hdr }
				{ }
			};

			using FinalForm = InstanceForm<InstanceFormFactor>;

			using FinalInstance = InstanceLibrary<FinalForm>;
		};
	}
}
