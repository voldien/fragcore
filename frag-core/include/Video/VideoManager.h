/**
	FragEngine, A Two layer Game Engine.
    Copyright (C) 2018  Valdemar Lindberg

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef _FRAG_CORE_VIDEO_MANAGER_H_
#define _FRAG_CORE_VIDEO_MANAGER_H_ 1
#include "../Core/Module.h"
#include "../Core/Singleton.h"
#include "../Core/TaskScheduler/IScheduler.h"
#include "../Def.h"
#include "VideoTexture.h"

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC VideoManager : public Module, public Singleton<VideoManager> {
	  public:
		static void computeVideoTask(Task *task);

		void registerVideo(VideoTexture *video);
		void unRegisterVideo(VideoTexture *video);

	  private: /*	Prevent one from creating an instance of this class.	*/
		VideoManager(void);

		VideoManager(const VideoManager &other);
	};
} // namespace fragcore
#endif