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
#ifndef _FRAG_CORE_PREREQUISITES_H_
#define _FRAG_CORE_PREREQUISITES_H_ 1

namespace fragcore {

	/*  Core.   */
	class Object;

	/*	Compression.	*/

	/*	*/
	class IScheduler;

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
