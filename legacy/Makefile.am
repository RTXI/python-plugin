
include $(top_srcdir)/.buildvars

if BUILD_PYTHON
pkglib_LTLIBRARIES = python_plugin.la
endif

DEFS = -fPIC
INCLUDES = ${QONSOLE_INCLUDES} -I${top_srcdir} ${NDS_INCLUDES} -I/usr/include/python2.4
LIBS =  $(PYTHON_LDFLAGS) ${NDS_LIBS}

python_plugin_la_LDFLAGS = -module -avoid-version

python_plugin_la_SOURCES =        \
		python_plugin.h   \
		python_plugin.cpp 

nodist_python_plugin_la_SOURCES = \
		moc_python_plugin.cpp

# MOC Rule - builds meta-object files as needed
moc_%.cpp: %.h
	$(MOC) -o $@ $<

SWIG_SOURCES = python_rtxi.i

if BUILD_PYTHON
python_PYTHON = python_rtxi.py

pyexec_LTLIBRARIES = _python_rtxi.la
endif

_python_rtxi_la_LDFLAGS = -module -avoid-version 

_python_rtxi_la_CPPFLAGS = $(SWIG_PYTHON_CPPFLAGS) 

_python_rtxi_la_SOURCES = $(SWIG_SOURCES)     \
			python_rtxi_wrap.cpp  \
			python_rtxi_iface.h   \
			python_rtxi_iface.cpp 


%_wrap.cpp: %.i
	$(SWIG) $(SWIG_PYTHON_OPT)  -o $@ $<


CLEANFILES = moc_*.cpp *~
MAINTAINERCLEANFILES = Makefile.in
