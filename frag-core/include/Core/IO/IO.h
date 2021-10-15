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
#ifndef _FRAG_CORE_ABSTRACT_IO_H_
#define _FRAG_CORE_ABSTRACT_IO_H_ 1
#include "../../Prerequisites.h"
#include "FileAccess.h"

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC IO : public FileAccess {
	  public:
		enum Seek {
			SET = 0x0, /*  From beginning position.    */
			CUR = 0x1, /*  Current position.   */
			END = 0x2  /*  From end position.  */
		};

		enum IOMode {
			READ = 0x1,				 /*  */
			WRITE = 0x2,			 /*  */
			ACCESS = (READ | WRITE), /*  */
			APPEND = 0x4			 /*  Set mode to append with WRITE.  */
		};

	  public:
		virtual ~IO() = default;

		virtual void close() = 0;

		virtual long int read(long int nbytes, void *pbuffer) = 0;

		virtual long int write(long int nbytes, const void *pbuffer) = 0;

		virtual long int peek(long int nBytes, void *pbuffer) {}

		virtual bool eof() const = 0;

		virtual long int length() = 0;

		virtual void seek(long int nbytes, Seek seek) = 0;

		virtual unsigned long getPos() = 0;

		virtual bool isWriteable() const = 0;

		virtual bool isReadable() const = 0;

		virtual bool flush() = 0;

		//		virtual String get_path() const { return ""; } /// returns the path for the current open file
		//		virtual String get_path_absolute() const { return ""; } /// returns the absolute path for the current
		// open file

		//		virtual void set_endian_swap(bool p_swap) { endian_swap = p_swap; }
		//		inline bool get_endian_swap() const { return endian_swap; }

		// TODO determine first how its behaviour before determine if it shall be included.
		// virtual std::optional<IFileSystem*> getFileSystem() = 0;

		// 		virtual bool isBlocking();
		// virtual void setBlocking(bool blocking);

		enum IOOperation : unsigned int {
			OP_ALL = (unsigned int)(-1),
			OP_READ = 1 << 0,
			OP_WRITE = 1 << 1,
			OP_EOF = 1 << 2,
			OP_LENGTH = 1 << 3,
			OP_SEEK = 1 << 4,
			OP_GETPOS = 1 << 5,
			OP_FLUSH = 1 << 6,
			OP_FILEACCESS = 1 << 7,
			OP_PEEK = 1 << 8
		};

		virtual bool isOperationSupported(IOOperation operationFlags) const noexcept = 0;

		template <typename T> T &as() { return static_cast<T>(*this); }

	  protected: /*  Internal methods.   */
		virtual void open(const char *path, IOMode mode) = 0;
		virtual void open(std::string &path, IOMode mode) { this->open(path.c_str(), mode); }
	};

} // namespace fragcore

#endif
