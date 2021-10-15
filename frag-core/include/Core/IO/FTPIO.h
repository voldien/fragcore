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
#ifndef _FRAG_CORE_FTP_IO_H_
#define _FRAG_CORE_FTP_IO_H_ 1
#include "../Ref.h"
#include "FileIO.h"
#include "IO.h"
#if defined(FRAG_CORE_INTERNAL_IMP) // TODO resolve to a single file or something later
#include <curl/curl.h>
#endif

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC FTPFileIO : public IO { // TODO determine how to handle with the FileIO dep.
	  public:
		long read(long int nbytes, void *pbuffer) override;

		long write(long int nbytes, const void *pbuffer) override;

		long length() override;

		bool eof() const override;

		void seek(long int nbytes, Seek seek) override;

		unsigned long getPos() override;

		bool isWriteable() const override;

		bool isReadable() const override;

		bool flush() override;

		void close() override;

		void open(const char *path, IOMode mode) override;

		virtual bool isOperationSupported(IOOperation operations) const noexcept override {
			const IOOperation supportedIO = static_cast<IOOperation>(OP_READ | OP_WRITE);
			return (operations & supportedIO) != operations;
		};

	  private:
#if defined(FRAG_CORE_INTERNAL_IMP)
#else
		void *data;
#endif
	  public:
		FTPFileIO(const char *path, IOMode mode); // TODO remove filepath
		FTPFileIO(std::string &path, IOMode mode, Ref<IO>& io);

	  private:
#if defined(FRAG_CORE_INTERNAL_IMP) // TODO resolve to a single file or something later
		FTPFileIO(CURL *handle, const char *path, IOMode mode);
		CURL *handle;
#endif
		Ref<IO> ref;
		// GZFileIO(Ref<IO> &io);
	};
} // namespace fragcore

#endif
