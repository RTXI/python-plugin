
/* 
 * python_rtxi_iface.h
 *
 *
 *
 * Version $Revision: 1.1 $
 *
 */


#ifndef _PYTHON_RTXI_IFACE_H
#define _PYTHON_RTXI_IFACE_H

#include <string>
#include <string.h>
#include <pthread.h>

#include <io.h>
#include <plugin.h>
#include <workspace.h>

#include <plugins/performance_measurement/performance_measurement.h>
#include <plugins/data_recorder/data_recorder.h>


typedef unsigned long flags_t;

class PythonWorkspace
{

    static const flags_t INPUT  = 0x1;
    static const flags_t OUTPUT = 0x2;
    static const flags_t PARAMETER  = 0x4;
    static const flags_t STATE  = 0x8;

public:
    PythonWorkspace(Workspace::Instance *);
    PythonWorkspace(void);

    const char *getWorkspaceName (void);
    size_t getCount(flags_t type);
    const char *getName(flags_t type, size_t index);
    double getValue(flags_t type,size_t index);
    void setValue(size_t index,double value);
    void addRecordVar (flags_t type,size_t index);
    void removeRecordVar (flags_t type,size_t index);
    void toggleRecord (bool f);

private:

    Workspace::Instance *workspace;
    DataRecorder::Panel *recorder;
};


size_t writeInput (unsigned char *buf, size_t size);
size_t readOutput (unsigned char *buf, size_t size);

PythonWorkspace *findPythonWorkspace (char *name);

class PythonTiming
{
public:
     void timing(void);
     PythonTiming(PerformanceMeasurement::Plugin *);
	 

private:
     PerformanceMeasurement::Plugin *pm;
};

PythonTiming *getPythonTiming (void);

void startTiming (void);


#endif /* _PYTHON_RTXI_IFACE_H */

/*
 * $Id: python_rtxi_iface.h,v 1.1 2007/02/12 17:46:53 ivan_raikov Exp $
 *
 * 
 * $Log: python_rtxi_iface.h,v $
 * Revision 1.1  2007/02/12 17:46:53  ivan_raikov
 * Incorporated the Python plugin and many changes to the build system, the
 * console interface, and the Debian packages.
 *
 *
 */
