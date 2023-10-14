pkgconfigdir = $(libdir)/pkgconfig
dist_pkgconfig_DATA = apertium-anaphora.pc

SUBDIRS = src locales

EXTRA_DIST=autogen.sh

test:
	pushd $(abs_srcdir)/tests/ && bash test.sh $(abs_builddir)/src/apertium-anaphora && popd

export ALT_I18N_DATA=$(datadir)/lttoolbox/lttoolbox.dat
export LOCALES_DIR=$(datadir)/$(PACKAGE_NAME)
export ANA_I18N_DATA=$(LOCALES_DIR)/ana.dat
