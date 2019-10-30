// scheduler.cc 
//	Routines to choose the next thread to run, and to dispatch to
//	that thread.
//
// 	These routines assume that interrupts are already disabled.
//	If interrupts are disabled, we can assume mutual exclusion
//	(since we are on a uniprocessor).
//
// 	NOTE: We can't use Locks to provide mutual exclusion here, since
// 	if we needed to wait for a lock, and the lock was busy, we would 
//	end up calling FindNextToRun(), and that would put us in an 
//	infinite loop.
//
// 	Very simple implementation -- no priorities, straight FIFO.
//	Might need to be improved in later assignments.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "debug.h"
#include "scheduler.h"
#include "main.h"
int cmpSJF(Thread* thd1,Thread* thd2)
{
   if(thd1->getTimeLeft() < thd2->getTimeLeft())
         return -1;

    else
         return 0;
}
int cmpPriority(Thread* thd1, Thread* thd2)
{
    if(thd1->getPriority()<thd2->getPriority())
	return -1;
    else	
	return 0;
}
int cmpSRTF(Thread* thd1,Thread* thd2)       //一開始宣告global scope
{
      if(thd1->getburst() < thd2->getburst())
         return -1;
      else
         return 0;
   }
int cmpFCFS(Thread* thd1, Thread* thd2)
{
    return 0;
}
//----------------------------------------------------------------------
// Scheduler::Scheduler
// 	Initialize the list of ready but not running threads.
//	Initially, no ready threads.
//----------------------------------------------------------------------

Scheduler::Scheduler(SchedulerType type)
{
//	schedulerType = type;
	schedulerType = type;
	switch(schedulerType)
	{
		case Priority:
			cout <<"type priority" <<endl; 
			readyList = new SortedList<Thread *>(cmpPriority);
			break;
		case SJF:
			cout << "type: SJF" <<endl;
			readyList = new SortedList<Thread *>(cmpSJF);
			break;
		case SRTF:
			cout << "type SRTF" <<endl;
			readyList = new SortedList<Thread *>(cmpSRTF);
			break;
		case FCFS:
			cout << "type FCFS" <<endl;
			readyList = new SortedList<Thread *>(cmpFCFS);
			break;
		default:
			//cout << "please enter para" <<endl;
			break;
	} 
	toBeDestroyed = NULL;
} 

Scheduler::Scheduler()
{
    Scheduler(FCFS);
}
//----------------------------------------------------------------------
// Scheduler::~Scheduler
// 	De-allocate the list of ready threads.
//----------------------------------------------------------------------

Scheduler::~Scheduler()
{ 
    delete readyList; 
} 

//----------------------------------------------------------------------
// Scheduler::ReadyToRun
// 	Mark a thread as ready, but not running.
//	Put it on the ready list, for later scheduling onto the CPU.
//
//	"thread" is the thread to be put on the ready list.
//----------------------------------------------------------------------

void
Scheduler::ReadyToRun (Thread *thread)
{
    ASSERT(kernel->interrupt->getLevel() == IntOff);
    DEBUG(dbgThread, "Putting thread on ready list: " << thread->getName());

    thread->setStatus(READY); 
    switch(schedulerType)
    {
	case Priority:
	  readyList->Insert(thread);
	  cout << "Thread's name: " << thread->getName()<< "priority: ->" <<thread->getPriority()<<endl;
	  break;
	case SJF:
	    readyList-> Insert(thread);    //將最低的行程插入到Queue的最前面
	  // cout << thread -> getTimeLeft() <<endl;
	   // cout<<" Thread's name: "<<thread->getName()<< " BurstTime: -> "<<thread->getTimeLeft() <<endl;  //印出thread 的名稱和thread SJF的值 
	    break;
	case SRTF:
	   readyList->Insert(thread);
	   cout <<"ready queue first Thread name：" <<thread->getName()<<"  BurstTime:"<<thread->getburst()<<"\n";
	   break;
	default:
	     readyList->Insert(thread); 
	     break;
    }
}

//----------------------------------------------------------------------
// Scheduler::FindNextToRun
// 	Return the next thread to be scheduled onto the CPU.
//	If there are no ready threads, return NULL.
// Side effect:
//	Thread is removed from the ready list.
//----------------------------------------------------------------------

Thread *
Scheduler::FindNextToRun ()
{
    ASSERT(kernel->interrupt->getLevel() == IntOff);

    if (readyList->IsEmpty()) {
	return NULL;
    } else {
    	return readyList->RemoveFront();
    }
}

//----------------------------------------------------------------------
// Scheduler::Run
// 	Dispatch the CPU to nextThread.  Save the state of the old thread,
//	and load the state of the new thread, by calling the machine
//	dependent context switch routine, SWITCH.
//
//      Note: we assume the state of the previously running thread has
//	already been changed from running to blocked or ready (depending).
// Side effect:
//	The global variable kernel->currentThread becomes nextThread.
//
//	"nextThread" is the thread to be put into the CPU.
//	"finishing" is set if the current thread is to be deleted
//		once we're no longer running on its stack
//		(when the next thread starts running)
//----------------------------------------------------------------------

void
Scheduler::Run (Thread *nextThread, bool finishing)
{
    Thread *oldThread = kernel->currentThread;
    switch(schedulerType)
    {
	case Priority: 
	 cout << "current thread priority: " << oldThread->getPriority() <<" next thread priority: " << nextThread->getPriority()<<endl;
	  break;
	case SJF:
	  //cout << "current Thread BurstTime: " <<oldThread->getBurstTime()<<"  Next Thread BurstTime-> "<<nextThread->getBurstTime()<<endl;  
	    break;
	case SRTF:
	   oldThread->setTime(20);   //oldthreadtime - timetick
    	   //cout << "current Thread BurstTime ->" << oldThread->getburst() << "  Next Thread BurstTime -> "<<nextThread->getburst()<<endl;
	   break; 
	default:
	    // cout <<"running............."<<endl;
	     break;
    }
    
//	cout << "Current Thread" <<oldThread->getName() << "    Next Thread"<<nextThread->getName()<<endl;
   
    ASSERT(kernel->interrupt->getLevel() == IntOff);

    if (finishing) {	// mark that we need to delete current thread
         ASSERT(toBeDestroyed == NULL);
	 toBeDestroyed = oldThread;
    }
    
#ifdef USER_PROGRAM			// ignore until running user programs 
    if (oldThread->space != NULL) {	// if this thread is a user program,
        oldThread->SaveUserState(); 	// save the user's CPU registers
	oldThread->space->SaveState();
    }
#endif
    
    oldThread->CheckOverflow();		    // check if the old thread
					    // had an undetected stack overflow

    kernel->currentThread = nextThread;  // switch to the next thread
    nextThread->setStatus(RUNNING);      // nextThread is now running
    
    DEBUG(dbgThread, "Switching from: " << oldThread->getName() << " to: " << nextThread->getName());
    
    // This is a machine-dependent assembly language routine defined 
    // in switch.s.  You may have to think
    // a bit to figure out what happens after this, both from the point
    // of view of the thread and from the perspective of the "outside world".

    SWITCH(oldThread, nextThread);

    // we're back, running oldThread
      
    // interrupts are off when we return from switch!
    ASSERT(kernel->interrupt->getLevel() == IntOff);

    DEBUG(dbgThread, "Now in thread: " << oldThread->getName());

    CheckToBeDestroyed();		// check if thread we were running
					// before this one has finished
					// and needs to be cleaned up
    
#ifdef USER_PROGRAM
    if (oldThread->space != NULL) {	    // if there is an address space
        oldThread->RestoreUserState();     // to restore, do it.
	oldThread->space->RestoreState();
    }
#endif
}

//----------------------------------------------------------------------
// Scheduler::CheckToBeDestroyed
// 	If the old thread gave up the processor because it was finishing,
// 	we need to delete its carcass.  Note we cannot delete the thread
// 	before now (for example, in Thread::Finish()), because up to this
// 	point, we were still running on the old thread's stack!
//----------------------------------------------------------------------

void
Scheduler::CheckToBeDestroyed()
{
    if (toBeDestroyed != NULL) {
        delete toBeDestroyed;
	toBeDestroyed = NULL;
    }
}
 
//----------------------------------------------------------------------
// Scheduler::Print
// 	Print the scheduler state -- in other words, the contents of
//	the ready list.  For debugging.
//----------------------------------------------------------------------
void
Scheduler::Print()
{
    cout << "Ready list contents:\n";
    readyList->Apply(ThreadPrint);
}

bool sleepList::IsEmpty() {
    return _threadlist.size() == 0;
}
void sleepList::PutToSleep(Thread*t, int x) {
    ASSERT(kernel->interrupt->getLevel() == IntOff);
    _threadlist.push_back(sleepThread(t, _current_interrupt + x)); //each increment will take 1 ms
    t->Sleep(false);
}
bool sleepList::PutToReady() {
    bool woken = false;
    _current_interrupt ++;
    for(std::list<sleepThread>::iterator it = _threadlist.begin();
        it != _threadlist.end(); ) {
        if(_current_interrupt >= it->when) {
            woken = true;
            cout << "sleepList::PutToReady Thread woken" << endl;
            kernel->scheduler->ReadyToRun(it->sleeper);
            it = _threadlist.erase(it);
        } else {
            it++;
        }
    }
    return woken;
}

