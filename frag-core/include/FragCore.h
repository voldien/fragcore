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
#include "IO/BufferIO.h"
#include "IO/FileSystem.h"
#include "IO/GZFileIO.h"
#include "IO/ZipFileSystem.h"

/*  Data Structure. */
#include "DataStructure/Queue.h"
#include "DataStructure/Stack.h"
#include "DataStructure/StackAllactor.h"
#include "DataStructure/StackBufferedAllocator.h"

/*  */
#include "TaskScheduler/IScheduler.h"

/*  Networking. */
#include "Network/NetSocket.h"

/*  Math.   */
#include "Math/Math.h"
#include "Math3D/AABB.h"
#include "Math3D/BoundingSphere.h"
#include "Math3D/LinAlg.h"
#include "Math3D/OBB.h"
#include "Math3D/Plane.h"

/*  */
#include "Compression/GZIPCompression.h"
#include "Compression/LZ4Compression.h"

#endif
