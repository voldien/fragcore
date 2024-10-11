/*
 *	FragCore - Fragment Core - Engine Core
 *	Copyright (C) 2018 Valdemar Lindberg
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program;
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

/*  Core.   */
#include "Core/IConfig.h"
#include "Core/Library.h"
#include "Core/SystemInfo.h"

/*  Memory Management.  */
#include "Core/Ref.h"
#include "Core/RefPtr.h"
#include "Core/SmartReference.h"

/*  IO. */
#include "Core/IO/BufferIO.h"
#include "Core/IO/FileSystem.h"
#include "Core/IO/GZFileIO.h"
#include "Core/IO/ZipFileSystem.h"

/*  Data Structure. */
#include "Core/DataStructure/Queue.h"
#include "Core/DataStructure/Stack.h"
#include "Core/DataStructure/StackAllactor.h"
#include "Core/DataStructure/StackBufferedAllocator.h"

/*  */
#include "Core/TaskScheduler/IScheduler.h"

/*  Networking. */
#include "Core/Network/NetSocket.h"

/*  Math.   */
#include "Core/Math/Math.h"
#include "Core/Math3D/AABB.h"
#include "Core/Math3D/BoundingSphere.h"
#include "Core/Math3D/OBB.h"
#include "Core/Math3D/Plane.h"

#endif
