/* -*-c-file-style: "bsd" -*- */

/* 
 * python_plugin.cpp
 *
 *
 *
 * Version $Revision: 1.1 $
 *
 */

#include <string>
#include <stdio.h>
#include <Python.h>
#include <main_window.h>

#ifndef _PYTHON_PLUGIN_H
#include <python_plugin.h>
#endif

#ifdef QONSOLE
#include <console.h>
#endif
#include <cmdline.h>

extern "C" Plugin::Object *createRTXIPlugin(void *) 
{
	DEBUG_MSG("PythonPlugin: createRTXIPlugin\n");
	return PythonPlugin::getInstance();
}


extern "C" void *python_run (void *arg)
{
     DEBUG_MSG("python_run : starting\n");

     DEBUG_MSG("python_run : PythonPlugin::initialized = %d\n", 
	       PythonPlugin::initialized );

     if (!(PythonPlugin::initialized))
     {
	     Py_Initialize();
	     PythonPlugin::initialized = true;
     }

     PyRun_SimpleString("import code\n"
			"code.interact()\n");

     DEBUG_MSG("python_run : end\n");

     return NULL;
}


/*
 *
 * Default constructor
 *
 */

PythonPlugin::PythonPlugin(void)
{
	DEBUG_MSG("PythonPlugin::PythonPlugin : starting\n");

	wm = Workspace::Manager::getInstance();

	//menuID = MainWindow::getInstance()->createControlMenuItem("Python",this,SLOT(run(void)));

#ifdef QONSOLE
	int *fd;

	if ((fd = Console::getInstance()->requestIoFd()) == NULL)
	{
		DEBUG_MSG("PythonPlugin::PythonPlugin: unable to allocate I/O file descriptors\n");
		return;
	}

	input_fd      = dup(fd[1]);
	output_fd     = dup(fd[2]);

	Console::getInstance()->setTabLabel (fd[0],"Python");

	delete fd;
#else
	input_fd = dup(STDOUT_FILENO);
	output_fd = dup(STDIN_FILENO);
#endif

	run();
}

/*
 *
 * Destroys the object
 *
 */

PythonPlugin::~PythonPlugin(void)
{
	if (initialized) Py_Finalize();
	if (tid != 0) pthread_cancel(tid);
	//MainWindow::getInstance()->removeControlMenuItem(menuID);

	close (input_fd);
	close (output_fd);

	PythonPlugin::instance = NULL;
}


void PythonPlugin::run (void)
{
	int status; 
	
	if (tid != 0)
	{
		pthread_cancel(tid);
	}

	if ((status = pthread_create (&tid, NULL, python_run, NULL)) != 0)
		return;
}

static Mutex mutex;
PythonPlugin *PythonPlugin::instance = NULL;

PythonPlugin *PythonPlugin::getInstance(void) 
{
    if(instance != NULL)
        return instance;

    Mutex::Locker lock(&::mutex);
    if(instance == NULL)
        instance = new PythonPlugin();

    return instance;
}


bool PythonPlugin::initialized = false;

pthread_t PythonPlugin::tid = 0;


/*
 * $Id: python_plugin.cpp,v 1.1 2007/02/12 17:46:53 ivan_raikov Exp $
 *
 * 
 * $Log: python_plugin.cpp,v $
 * Revision 1.1  2007/02/12 17:46:53  ivan_raikov
 * Incorporated the Python plugin and many changes to the build system, the
 * console interface, and the Debian packages.
 *
 *
 *
 */
