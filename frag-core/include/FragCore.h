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
//TOOD rename to match the library tier layer name.
#ifndef _FRAG_CORE_FRAGVIEWCORE_H_
#define _FRAG_CORE_FRAGVIEWCORE_H_ 1
#include"Prerequisites.h"
#include"Utils/StringUtil.h"
/*  */
#include"Core/SystemInfo.h"
#include"Core/IConfig.h"
/*  */
#include"Core/SmartReference.h"
#include"Core/Ref.h"
#include"Core/RefPtr.h"
/*  */
#include"Core/IO/FileSystem.h"
#include"Core/IO/ZipFileSystem.h"
#include"Core/IO/GZFileIO.h"
#include"Core/IO/BufferIO.h"
/*  */
#include "Core/dataStructure/DoubleBufferedAllocator.h"
/*  */
#include"Core/Log.h"
#include"Core/Library.h"
/*  */
#include"Core/TaskScheduler/IScheduler.h"
#include "Core/TaskScheduler/TaskScheduler.h"
/*  */
#include"Video/VideoManager.h"
#include"Window/WindowFactory.h"
/*  */
#include"Core/Network/NetSocket.h"
/*  */
#include"Renderer/RendererFactory.h"
#include"Renderer/CommandList.h"
/*  */
#include"Audio/AudioFactory.h"
/*  */
#include"Physic/PhysicFactory.h"
/*  */
#include"Exception/IException.h"
#include"Exception/RuntimeException.h"
#include"Exception/InvalidArgumentException.h"
#include"Exception/NotImplementedException.h"
#include"Exception/NotSupportedException.h"
#include"Exception/PermissionDeniedException.h"
#include"Exception/InvalidPointerException.h"
#endif
