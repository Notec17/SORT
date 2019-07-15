/*
    This file is a part of SORT(Simple Open Ray Tracing), an open-source cross
    platform physically based renderer.
 
    Copyright (c) 2011-2018 by Cao Jiayin - All rights reserved.
 
    SORT is a free software written for educational purpose. Anyone can distribute
    or modify it under the the terms of the GNU General Public License Version 3 as
    published by the Free Software Foundation. However, there is NO warranty that
    all components are functional in a perfect manner. Without even the implied
    warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    General Public License for more details.
 
    You should have received a copy of the GNU General Public License along with
    this program. If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
 */

#pragma once

#include "core/define.h"
#include <thread>
#include <atomic>

// get the thread id
int ThreadId();
// get the number of cpu cores in the system
unsigned NumSystemCores();

class WorkerThread
{
public:
    // Constructor
    WorkerThread( unsigned tid ) : m_tid(tid) {}
    
	// Begin thread
	void BeginThread();
    
	// Run the thread
	void RunThread();

	// Whether the thread is finished
	void Join() {
		return m_thread.join();
	}
    
private:
    std::thread m_thread;
    unsigned    m_tid = 0;
};

class spinlock_mutex
{
public:
    void lock() {
        while (locked.test_and_set(std::memory_order_acquire)) { ; }
    }
    void unlock() {
        locked.clear(std::memory_order_release);
    }

private:
    std::atomic_flag locked = ATOMIC_FLAG_INIT ;
};
