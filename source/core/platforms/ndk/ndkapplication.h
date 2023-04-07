// AllegroApplication.h
// The header file defines the interface of the class

#ifndef __NDK_APPLICATION_H__
#define __NDK_APPLICATION_H__

#include "../../application.h"
#include "ndkinput.h"
#include "ndkdebug.h"
#include "ndkrender.h"

class NDKApplication : public Application
{
public:
	NDKApplication();
	void Run();
	virtual void Init() = 0;
	virtual void UpdateFrame() = 0;
	void Update();

	ndkdebug Debug;
	ndkinput Input;

	NDKRender *m_renderer;

protected:
	// Entry point of application; can be overloaded by game
	// if different behaviour is wanted
	virtual int Main(int argumentQuantity, char* arguments[]);
};

#endif // __NDK_APPLICATION_H__
