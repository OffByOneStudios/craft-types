

SET(CRAFT_TYPES_LIBS "")
SET(CRAFT_TYPES_DEBUG_LIBS "")
SET(CRAFT_TYPES_RELEASE_LIBS "")

vendor_resolve_lib(mpir CRAFT_TYPES_LIBS CRAFT_TYPES_DEBUG_LIBS CRAFT_TYPES_RELEASE_LIBS)
vendor_resolve_lib(mpirxx CRAFT_TYPES_LIBS CRAFT_TYPES_DEBUG_LIBS CRAFT_TYPES_RELEASE_LIBS)

message(1 ${CRAFT_TYPES_LIBS})
message(2 ${CRAFT_TYPES_DEBUG_LIBS})
message(3 ${CRAFT_TYPES_RELEASE_LIBS})


foreach(lib ${CRAFT_TYPES_LIBS})
  target_link_libraries(CraftEngineDepsTypes ${lib})
endforeach()

foreach(lib ${CRAFT_TYPES_DEBUG_LIBS})
  target_link_libraries(CraftEngineDepsTypes debug ${lib})
endforeach()

foreach(lib ${CRAFT_TYPES_RELEASE_LIBS})
  target_link_libraries(CraftEngineDepsTypes optimized ${lib})
endforeach()

