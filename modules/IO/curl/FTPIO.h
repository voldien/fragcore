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
#ifndef _FRAG_CORE_FTP_IO_H_
#define _FRAG_CORE_FTP_IO_H_ 1
#include <FragCore.h>
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

		long int peek(long int nBytes, void *pbuffer) override { return 0; }

		long length() override;

		bool eof() const override;

		void seek(long int nbytes, const Seek seek) override;

		unsigned long getPos() override;

		bool isWriteable() const override;

		bool isReadable() const override;

		bool flush() override;

		void close() override;

		void open(const char *path, IOMode mode) override;

		bool isOperationSupported(IOOperation operations) const noexcept override {
			const IOOperation supportedIO = static_cast<IOOperation>(OP_READ | OP_WRITE | OP_LENGTH);
			return (operations & supportedIO) != operations;
		};

	  private:
#if defined(FRAG_CORE_INTERNAL_IMP)
#else
		void *data;
#endif
	  public:
		FTPFileIO(const char *path, IOMode mode); // TODO remove filepath
		FTPFileIO(std::string &path, IOMode mode, Ref<IO> &io);

#if defined(FRAG_CORE_INTERNAL_IMP) // TODO resolve to a single file or something later
		FTPFileIO(CURL *handle, const char *path, IOMode mode);
		CURL *handle;
#endif
		Ref<IO> ref;
		IO::IOMode ioMode;
		int sockfd;
	};
} // namespace fragcore

#endif
