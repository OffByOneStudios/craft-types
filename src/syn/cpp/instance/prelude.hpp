#pragma once
#include "syn/syn.h"

namespace syn
{
    namespace instance_policy
    {
	    // Defined in `cpp/instance_policies`
		template<typename TType, typename TEnable = void>
		class CppReferenceCounted;

        template<typename TType>
        using CppReferneceCountedActual = CppReferenceCounted<TType>;


        struct InstanceChunkHeader
        {
        protected:
            InstanceHeader* _header;

            InstanceChunkHeader()
                : _header(nullptr)
                { }

            InstanceChunkHeader(InstanceHeader* header)
                : _header(header)
                { }
        };

        struct InstanceChunkActual
        {
        protected:
            void* _actual;
        };

        struct InstanceChunkExtras
        {

        };
    }

	// Defined in `cpp/instance`
	template<typename TType = void, template<typename> typename TPolicy = instance_policy::CppReferneceCountedActual> struct instance;
}
