#pragma once
#include <hash_map>
#include "Singleton.h"
enum eMainLoopTask
{
	eMainLoopTask_Device,
	eMainLoopTask_Finale,
	eMainLoopTask_Script,
	eMainLoopTask_Size,
};
class MainLoopTask
{
public:
	MainLoopTask() : enableDraw(true) {	}
	virtual ~MainLoopTask() { }

	virtual bool init() { return true; }
	virtual void fini()	{ }

	virtual void tick( float dTime ) { }
	virtual void draw() { }
	virtual void inactiveTick( float dTime ) {}

	bool	enableDraw;
	eMainLoopTask order_;
};
class MainLoopTaskManager : public MainLoopTask, public Singleton<MainLoopTaskManager>
{
public:
	MainLoopTaskManager()
	{

	}
	virtual ~MainLoopTaskManager()
	{
		TaskMap::iterator it = tasks_.begin();
		for (; it != tasks_.end(); ++it)
		{
			delete it->second;
		}
		tasks_.clear();
	}

	virtual bool init()
	{
		TaskMap::iterator it = tasks_.begin();
		for (; it != tasks_.end(); ++it)
		{
			it->second->init();
		}

		return true;
	}
	virtual void fini()
	{
		TaskMap::iterator it = tasks_.begin();
		for (; it != tasks_.end(); ++it)
		{
			it->second->fini();
		}
	}

	virtual void tick( float dTime )
	{
		TaskMap::iterator it = tasks_.begin();
		for (; it != tasks_.end(); ++it)
		{
			it->second->tick(dTime);
		}
	}
	virtual void draw()
	{
		TaskMap::iterator it = tasks_.begin();
		for (; it != tasks_.end(); ++it)
		{
			it->second->draw();
		}
	}

	// this gets called rather than tick when the application is minimised
	virtual void inactiveTick( float dTime )
	{
		TaskMap::iterator it = tasks_.begin();
		for (; it != tasks_.end(); ++it)
		{
			it->second->inactiveTick(dTime);
		}
	}

	// rules are strings like ">TaskA", "<TaskB"
	void add( MainLoopTask * pTask)
	{
		tasks_[pTask->order_] = pTask;
	}
	void del( MainLoopTask * pTask)
	{

	}

	void outputOrder()
	{

	}

	MainLoopTask * getMainLoopTask( const char * name )
	{

	}

	bool initted()							{ return initted_; }

	static void deleteOrphans()
	{

	}
private:
	void sort();

	typedef stdext::hash_map<size_t, MainLoopTask*> TaskMap;
	TaskMap			tasks_;
	bool			initted_;
	bool			finished_;

	static std::vector<MainLoopTask*> *s_orphans_;
};
