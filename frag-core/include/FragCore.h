/*
 *	FragCore - Core Framework Functionalities for Game Engines
 *	Copyright (C) 2018  Valdemar Lindberg
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef _FRAG_CORE_FRAG_CORECORE_H_
#define _FRAG_CORE_FRAG_CORECORE_H_ 1
#include "Prerequisites.h"

/**
 * @file FragCore.h
 * @
 * @brief
 * @version 0.1
 *
 */

/*  */
#include "Core/IConfig.h"
#include "Core/SystemInfo.h"
/*  */
#include "Core/Ref.h"
#include "Core/RefPtr.h"
#include "Core/SmartReference.h"
/*  */
#include "Core/IO/BufferIO.h"
#include "Core/IO/FileSystem.h"
#include "Core/IO/GZFileIO.h"
#include "Core/IO/ZipFileSystem.h"
/*  */
#include "Core/dataStructure/DoubleBufferedAllocator.h"
/*  */
#include "Core/Library.h"
#include "Core/Log.h"
/*  */
#include "Core/TaskScheduler/IScheduler.h"
#include "Core/TaskScheduler/TaskScheduler.h"
/*  */
#include "Window/WindowFactory.h"
/*  */
#include "Core/Network/NetSocket.h"

#include "Core/dataStructure/AABB.h"
#include "Core/dataStructure/BoundingSphere.h"
#include "Core/dataStructure/OBB.h"
#include "Core/dataStructure/Plane.h"

#endif
