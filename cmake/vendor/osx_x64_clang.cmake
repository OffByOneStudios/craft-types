
SET(CRAFT_TYPE_LIBS "")
SET(CRAFT_TYPE_DEBUG_LIBS "")
SET(CRAFT_TYPE_RELEASE_LIBS "")

vendor_resolve_lib(mpir CRAFT_TYPE_LIBS CRAFT_TYPE_DEBUG_LIBS CRAFT_TYPE_RELEASE_LIBS)
vendor_resolve_lib(mpirxx CRAFT_TYPE_LIBS CRAFT_TYPE_DEBUG_LIBS CRAFT_TYPE_RELEASE_LIBS)

list(LENGTH CRAFT_TYPE_LIBS l)
if(NOT ${l} EQUAL 0)
target_link_libraries(CraftType ${CRAFT_TYPE_LIBS})
endif()

list(LENGTH CRAFT_TYPE_DEBUG_LIBS l)
if(NOT ${l} EQUAL 0)
target_link_libraries(CraftType debug ${CRAFT_TYPE_DEBUG_LIBS})
endif()

list(LENGTH CRAFT_TYPE_RELEASE_LIBS l)
if(NOT ${l} EQUAL 0)
target_link_libraries(CraftType optimized ${CRAFT_TYPE_RELEASE_LIBS})
endif()
