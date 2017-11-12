

SET(CRAFT_TYPES_LIBS "")
SET(CRAFT_TYPES_DEBUG_LIBS "")
SET(CRAFT_TYPES_RELEASE_LIBS "")

vendor_resolve_lib(mpir CRAFT_TYPES_LIBS CRAFT_TYPES_DEBUG_LIBS CRAFT_TYPES_RELEASE_LIBS)
vendor_resolve_lib(mpirxx CRAFT_TYPES_LIBS CRAFT_TYPES_DEBUG_LIBS CRAFT_TYPES_RELEASE_LIBS)


list(LENGTH CRAFT_TYPES_LIBS l)
if(NOT ${l} EQUAL 0)
target_link_libraries(CraftEngineDepsTypes ${CRAFT_TYPES_LIBS})
endif()

list(LENGTH CRAFT_TYPES_DEBUG_LIBS l)
if(NOT ${l} EQUAL 0)
target_link_libraries(CraftEngineDepsTypes debug ${CRAFT_TYPES_DEBUG_LIBS})
endif()

list(LENGTH CRAFT_BUILD_RELEASE_LIBS l)
if(NOT ${l} EQUAL 0)
target_link_libraries(CraftBuild optimized ${CRAFT_TYPES_RELEASE_LIBS})
endif()

