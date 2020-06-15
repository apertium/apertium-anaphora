add_executable(apertium-anaphora
	pattern_arx.cc
	parse_arx.cc
	parse_biltrans.cc
	score.cc
	anaphora.cc
	)
target_link_libraries(apertium-anaphora PRIVATE
	xml2
	lttoolbox3
	)

install(TARGETS apertium-anaphora DESTINATION ${CMAKE_INSTALL_BINDIR})

configure_file(apertium-validate-anaphora.in apertium-validate-anaphora @ONLY)
install(FILES anaphora.dtd DESTINATION "${CMAKE_INSTALL_DATADIR}/apertium-anaphora/")
install(PROGRAMS "${CMAKE_CURRENT_BINARY_DIR}/apertium-validate-anaphora" DESTINATION ${CMAKE_INSTALL_BINDIR})
