SET(plugins_implementation_files
  LoadedPlugins.cxx)

set(plugins_header_files
  Instantiate.hxx
  NamedSO.hxx
  traits.hxx)

add_library(nft_plugins SHARED ${plugins_implementation_files})

install(TARGETS nft_plugins DESTINATION lib)
install(FILES ${plugins_header_files} DESTINATION include/plugins)
