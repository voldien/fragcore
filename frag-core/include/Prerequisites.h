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
#ifndef _FRAGCORE_PREREQUISITES_H_
#define _FRAGCORE_PREREQUISITES_H_ 1

/*	Forward declaration of all objects.	*/
namespace fragcore {

	/*  Core.   */
	class Object;
	class UIDObject;

	/*	Compression.	*/

	/*	*/
	class IScheduler;
	class ISpinLock;
	class IMutex;
	class ISemaphore;

	/*  IO  */
	class IO;
	class FileIO;
	class ZipFileIO;
	class BufferIO;
	class FileAccess;
	class ASyncIO;
	class GZFileIO;
	class ZipFileSystem;
	class IFileSystem;
	class FileSystem;
	
	/*	*/
	class Hash;
	class Random;
	class Base64;
	class Base32;
	class Base16;

	/*  Network.    */
	class IPInterface;
	class NetSocket;
	class TCPNetSocket;
	class UDPNetSocket;
	class INetAddress;
	class IPAddress;

} // namespace fragcore

#endif
