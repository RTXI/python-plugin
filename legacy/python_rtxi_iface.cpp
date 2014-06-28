/* -*-c-file-style: "bsd" -*- */

/* 
 * python_rtxi_iface.cpp
 *
 *
 *
 * Version $Revision: 1.2 $
 *
 */

#include <string>
#include <stdio.h>

#ifndef _PYTHON_RTXI_IFACE_H
#include <python_rtxi_iface.h>
#endif

#include <plugins/performance_measurement/performance_measurement.h>
#include <plugins/data_recorder/data_recorder.h>
#include <plugins/dynamo_model_loader/dynamo_model_loader.h>
#include <cmdline.h>

const char *PythonWorkspace::getWorkspaceName()
{
	return (((IO::Block *)workspace)->getName()).c_str();
}

PythonWorkspace::PythonWorkspace (Workspace::Instance *w)
{
	this->workspace = w;
	this->recorder = DataRecorder::Plugin::getInstance()->createDataRecorderPanel();
}

PythonWorkspace::PythonWorkspace (void)
{
	this->workspace = new Workspace::Instance("NULL",NULL,0);
	this->recorder = NULL;
}

size_t PythonWorkspace::getCount(flags_t type)
{
	return workspace->getCount(type);
}

const char *PythonWorkspace::getName(flags_t type, size_t index)
{
	return (workspace->getName(type,index)).c_str();
}

double PythonWorkspace::getValue(flags_t type,size_t index)
{
	return workspace->getValue(type,index);
}

void PythonWorkspace::setValue(size_t index,double value)
{
	workspace->setValue(index,value);
}
void PythonWorkspace::addRecordVar (flags_t type,size_t index)
{
	if (recorder != NULL)
		recorder->addWorkspaceVar(workspace,type,index);
}

void PythonWorkspace::removeRecordVar (flags_t type,size_t index)
{
	if (recorder != NULL)
		recorder->removeWorkspaceVar(workspace,type,index);
}

void PythonWorkspace::toggleRecord (bool f)
{
	if (recorder != NULL)
		recorder->toggleRecord(f);
}


struct cmp_t
{
	char *name;
	Workspace::Instance *x; 
};

void find (Workspace::Instance *w, cmp_t *ax)
{
	IO::Block *b;
	
	b = (IO::Block *)w;
	if (strcmp((b->getName()).c_str(), ax->name) == 0)
		ax->x = w;
}

PythonWorkspace *findPythonWorkspace (char *name)
{
	struct cmp_t ax;
	
	ax.name = name;
	ax.x = NULL;
	
	Workspace::Manager::getInstance()->foreachWorkspace 
		((void (*)(Workspace::Instance*, void*))find, &ax);
	
	if (ax.x != NULL)
		return (new PythonWorkspace(ax.x));
	else
		return (new PythonWorkspace());
}

int setPeriod (long period)
{
        return RT::System::getInstance()->setPeriod((long long)period);
}

size_t writeInput (unsigned char *buf, size_t size)
{
	int fd; size_t s;
	fd = STDIN_FILENO;
	s = write(fd,(void *)buf,size);
	return s;
}

size_t readOutput (unsigned char *buf, size_t size)
{
	int fd;  size_t s;
	fd = STDOUT_FILENO;
	memset(buf,0,size);
	s = read(fd,(void *)buf,size);
	return s;
}

int shellExecute (char *cmd)
{
	int status; 
	std::string cwd;

	if ((cwd=std::string(getcwd(NULL,0))) == NULL) return -1;

	std::string chwd = (std::string("cd ")) + cwd + (std::string("; "));
	std::string cmdline = chwd + (std::string(cmd));
	status = CmdLine::getInstance()->execute(cmdline);

	return status;
}

int changeCwd (char *path)
{
	int status; 
	
	status = chdir(path);

	return status;
}

void PythonTiming::timing(void)
{
	pm->createPerformanceMeasurementPanel();
}

PythonTiming::PythonTiming(PerformanceMeasurement::Plugin *pm)
{
	this->pm = pm;
}

PythonTiming *getPythonTiming (void)
{
	return new PythonTiming (PerformanceMeasurement::Plugin::getInstance ());
}

void startTiming (void)
{
	getPythonTiming()->timing();
}

void loadModel (char *path)
{
	DynamoModelLoader::getInstance()->load(path);
}

/*
 * $Id: python_rtxi_iface.cpp,v 1.2 2007/02/27 20:13:38 ivan_raikov Exp $
 *
 * 
 * $Log: python_rtxi_iface.cpp,v $
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
 *
 */
