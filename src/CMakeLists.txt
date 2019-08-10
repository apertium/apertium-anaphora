add_executable(apertium-anaphora
	pattern_ref.cc
	parse_ref.cc
	parse_biltrans.cc
	score.cc
	anaphora.cc
	)
target_link_libraries(apertium-anaphora PRIVATE
	xml2
	lttoolbox3
	)

install(TARGETS apertium-anaphora
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	)
