#pragma once
#include "common.h"
#include "core.h"

namespace craft {
namespace types
{
	/******************************************************************************
	** Aspect
	******************************************************************************/

	/* Per Instance interfaces
	
	Aspects require an instaniated object
	*/
	class Aspect abstract
		: public Feature
	{
	public:
		//
		// Aspect
		//
		virtual instance<> craft_featuredInstance() = 0;
	};

	/******************************************************************************
	** IAspectManager
	******************************************************************************/

	class IAspectManager abstract
		: public IFeatureManager
	{
	public:
		virtual ~IAspectManager() = default;

		//
		// IFeatureManager
		//
		inline virtual FeatureKind featureKind() const override { return FeatureKind::Aspect; }

		//
		// IAspectManager
		//
		virtual bool hasAspect(TypeId tid, void* instance) = 0;
		virtual Aspect* getAspect(TypeId tid, void* instance) = 0;
		virtual void deleteAspect(Aspect* aspect) = 0;
	};

	/******************************************************************************
	** IAspectFactory
	******************************************************************************/

	template <typename TAspect>
	class IAspectFactory abstract
	{
	public:
		virtual ~IAspectFactory() = default;

		virtual bool isMultiUseSafe() const = 0;

		virtual TAspect* build(IAspectManager* man, void* inst) = 0;
		virtual void destroy(TAspect* aspect) = 0;
	};

	/******************************************************************************
	** CastingAspectFactory
	******************************************************************************/

	template <typename TAspect, typename TObject>
	class CastingAspectFactory
		: public IAspectFactory<TAspect>
	{
		static_assert(std::is_base_of<TAspect, TObject>::value,
			"Can only use CastingAspectFactory if `class TObject : public TAspect ...`");
	public:
		inline virtual bool isMultiUseSafe() const override { return true; }

		inline virtual TAspect* build(IAspectManager* man, void* inst) override { return (TAspect*)(TObject*)(inst); }
		inline virtual void destroy(TAspect* aspect) override { }
	};

	/******************************************************************************
	** ForwardingAspectFactory
	******************************************************************************/

	template <typename TAspect, typename TObject, typename TForwarded>
	class ForwardingAspectFactory
		: public IAspectFactory<TAspect>
	{
		typedef instance<TForwarded> TObject::* TObjectForwardingMember;

		TObjectForwardingMember _memPtr;
	public:
		inline ForwardingAspectFactory(TObjectForwardingMember mem_ptr) : _memPtr(mem_ptr) { }

		inline virtual bool isMultiUseSafe() const override { return true; }

		inline virtual TAspect* build(IAspectManager* man, void* inst) override { return (((TObject*)(inst))->*_memPtr).template getFeature<TAspect>(); }
		inline virtual void destroy(TAspect* aspect) override { }
	};

}}

#define CRAFT_ASPECT_DECLARE(x, name, manager) \
	static ::craft::types::Feature::_static_init __si; \
protected: \
	typedef manager < x > T_Manager; \
private: \
	friend T_Manager; \
	friend class ::craft::types::Types; \
public: \
	static constexpr auto craft_c_featureKind = ::craft::types::FeatureKind::Aspect; \
	static constexpr auto craft_c_featureName = name; \
	static inline ::craft::types::FeatureId craft_s_featureId() { return x::__si.id; } \
protected: \
	virtual ::craft::types::IFeatureManager* craft_featureManager() const override \
	{ return ::craft::types::system().getManager(x::craft_s_featureId()); } \
private:

#define CRAFT_ASPECT_DEFINE(x) \
	::craft::types::Feature::_static_init x::__si([](){ ::craft::types::system().getManager< x >(); });
