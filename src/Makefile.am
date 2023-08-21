AM_LDFLAGS=$(LIBS)
AM_CPPFLAGS = -DAPAN_I18N_DATA='"$(APAN_I18N_DATA)"' -DLTTB_I18N_DATA='"$(LTTB_I18N_DATA)"'


bin_PROGRAMS = apertium-anaphora
bin_SCRIPTS = apertium-validate-anaphora

apertium_anaphora_SOURCES = \
	anaphora.cc \
	parse_arx.cc \
	parse_biltrans.cc \
	pattern_arx.cc \
	score.cc

apertium_anaphoradir = $(prefix)/share/apertium-anaphora
apertium_anaphora_DATA = anaphora.dtd

apertium-validate-anaphora: Makefile.am validate-header.sh
	@echo "Creating apertium-validate-anaphora script"
	@echo "#!/usr/bin/env bash" > $@
	@cat $(abs_srcdir)/validate-header.sh >> $@
	@echo "xmllint --dtdvalid \"$(apertium_anaphoradir)\"/anaphora.dtd --noout \"\$$FILE1\"" >>$@
	@chmod a+x $@

EXTRA_DIST = anaphora.dtd
CLEANFILES = $(bin_SCRIPTS)
