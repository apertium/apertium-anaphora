pkgconfigdir = $(libdir)/pkgconfig
dist_pkgconfig_DATA = apertium-anaphora.pc

SUBDIRS = src

EXTRA_DIST=autogen.sh

test:
	pushd $(abs_srcdir)/tests/ && bash test.sh $(abs_builddir)/src/apertium-anaphora && popd
