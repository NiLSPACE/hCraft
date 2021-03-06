/* 
 * hCraft - A custom Minecraft server.
 * Copyright (C) 2012	Jacob Zhitomirsky
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _hCraft__LIGHTING_H_
#define _hCraft__LIGHTING_H_

#include <queue>
#include <mutex>
#include <bitset>


namespace hCraft {
	
	// forward decs:
	class world;
	class logger;
	
	
	/* 
	 * The structure of a queued update.
	 */
	struct light_update
	{
		int x; 
		int y;
		int z;
		bool a;
		
		light_update (int xx, int yy, int zz, bool aa = true)
		{
			this->x = xx;
			this->y = yy;
			this->z = zz;
			this->a = aa;
		}
	};
	
	
	/* 
	 * Handles block\sky lighting for a world or a chunk.
	 */
	class lighting_manager
	{
		logger &log;
		world *wr;
		std::queue<light_update> updates;
		std::mutex lock;
		bool overloaded;
		int limit;
		int handled_since_empty;
		
	public:
		inline world* get_world () const { return this->wr; }
		inline logger& get_logger () const { return this->log; }
		
	public:
		/* 
		 * Constructs a new lighting manager on top of the given world.
		 */
		lighting_manager (logger &log, world *wr, int limit = 500000);
		
		
		/* 
		 * Goes through all queued updates and handles them (No more than
		 * @{max_updates} updates are handled).
		 * 
		 * Returns the total amount of updates handled.
		 */
		int update (int max_updates = 384);
		
		
		/* 
		 * Pushes a lighting update to the update queue.
		 */
		void enqueue (int x, int y, int z);
		void enqueue_nolock (int x, int y, int z, bool a = true); // not thread-safe
	};
}

#endif

