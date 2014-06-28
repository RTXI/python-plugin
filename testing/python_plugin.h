
/* 
 * python_plugin.h
 *
 *
 *
 * Version $Revision: 1.2 $
 *
 */


#ifndef _PYTHON_PLUGIN_H
#define _PYTHON_PLUGIN_H

#include <string>
#include <string.h>
#include <pthread.h>

#include <qobject.h>

#include <io.h>
#include <plugin.h>
#include <event.h>
#include <workspace.h>


class PythonWorkspace;


class PythonPlugin:  public QObject, public ::Plugin::Object, public Event::Handler
{

    Q_OBJECT

public:
    PythonWorkspace *getWorkspace(char *name);

    static PythonPlugin *getInstance(void);
    
    static pthread_t tid;
    static bool initialized;

    int getInputFileno (void);
    int getOutputFileno (void);

    int setPeriod (long);

public slots:

    void run(void);

private:
    PythonPlugin(void);
    ~PythonPlugin(void);
    PythonPlugin(const PythonPlugin &) {};
    PythonPlugin &operator=(const PythonPlugin &) { return *getInstance(); };

    int menuID;

    int input_fd;
    int output_fd;

    static PythonPlugin *instance;
    Workspace::Manager *wm;
};


#endif /* _PYTHON_PLUGIN_H */

/*
 * $Id: python_plugin.h,v 1.2 2007/02/27 20:13:38 ivan_raikov Exp $
 *
 * 
 * $Log: python_plugin.h,v $
 * Revision 1.2  2007/02/27 20:13:38  ivan_raikov
 * - Update to the Chicken Scheme and Python bindings to support
 *   setPeriod with long long argument instead of double.
 * - Update to the build system to support relative paths for building
 *   models, and to use realpath(1) to supply libtool with the
 *   corresponding absolute paths.
 *
 * Revision 1.1  2007/02/12 17:46:53  ivan_raikov
 * Incorporated the Python plugin and many changes to the build system, the
 * console interface, and the Debian packages.
 *
 *
 */
