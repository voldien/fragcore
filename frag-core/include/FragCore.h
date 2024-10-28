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
#ifndef _FRAGCORE_FRAGCORECORE_H_
#define _FRAGCORE_FRAGCORECORE_H_ 1
#include "Prerequisites.h"

/**
 * @file FragCore.h
 * @
 * @brief
 * @version 0.1
 *
 */

/*  Core.   */
#include "Core/Environment.h"
#include "Core/IConfig.h"
#include "Core/Library.h"
#include "Core/SystemInfo.h"

/*  Memory Management.  */
#include "Core/Ref.h"
#include "Core/RefPtr.h"
#include "Core/SmartReference.h"

/*  IO. */
#include "IO/BufferIO.h"
#include "IO/FileSystem.h"
#include "IO/GZFileIO.h"
#include "IO/ZipFileSystem.h"

/*  Data Structure. */
#include "DataStructure/PoolAllocator.h"
#include "DataStructure/Queue.h"
#include "DataStructure/Stack.h"
#include "DataStructure/StackAllactor.h"
#include "DataStructure/StackBufferedAllocator.h"

/*  Task Schedular/Threading.   */
#include "TaskScheduler/IScheduler.h"

/*  Networking. */
#include "Network/NetSocket.h"

/*  Math.   */
#include "Math/Math.h"
#include "Math3D/AABB.h"
#include "Math3D/BoundingSphere.h"
#include "Math3D/LinAlg.h"
#include "Math3D/Line.h"
#include "Math3D/OBB.h"
#include "Math3D/Plane.h"
#include "Math3D/Transform.h"
#include "Math3D/Triangle.h"

/*  Compression.    */
#include "Compression/GZIPCompression.h"
#include "Compression/LZ4Compression.h"

#endif
