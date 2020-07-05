AM_LDFLAGS=$(LIBS)

apertiumdir = $(prefix)/share/apertium

bin_PROGRAMS = apertium-anaphora 

GENERATEDSCRIPTS = apertium-validate-anaphora

apertium_anaphora_SOURCES = anaphora.cc  parse_arx.cc  parse_biltrans.cc  pattern_arx.cc  score.cc

apertium_DATA = anaphora.dtd

bin_SCRIPTS =  $(GENERATEDSCRIPTS)

apertium-validate-anaphora: Makefile.am validate-header.sh
	@echo "Creating apertium-validate-anaphora script"
	@echo "#!$(BASH)" > $@
	@cat validate-header.sh >> $@
	@echo "$(XMLLINT) --dtdvalid \"$(apertiumdir)\"/anaphora.dtd --noout \"\$$FILE1\"" >>$@
	@chmod a+x $@

EXTRA_DIST = anaphora.dtd
