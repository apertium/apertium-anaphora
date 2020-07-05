pkgconfigdir = $(libdir)/pkgconfig
dist_pkgconfig_DATA = apertium-anaphora.pc

SUBDIRS = src

EXTRA_DIST=autogen.sh

test:
	pushd tests/ && bash test.sh ../src/apertium-anaphora && popd
