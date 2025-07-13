/*
 *	FragCore - Fragment Core
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
#ifndef _FRAGCORE_FTP_FILESYSTEM_H_
#define _FRAGCORE_FTP_FILESYSTEM_H_ 1
#include <FragCore.h>
#include <curl/curl.h>

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC FTPFileSystem : public IFileSystem {
	  public:
		IO *openFile(const char *path, IO::IOMode mode) override; /*  Open based on the filename extension.*/

		void closeFile(IO *io) override;

		void remove(const char *path) override;

		void rename(const char *oldPath, const char *newPath) override;

		void createFile(const char *path) override;

		void createDirectory(const char *path) override;

		bool isReadable(const char *path) const override;

		bool isWriteable(const char *path) const override;

		bool exists(const char *path) const override;

		bool isASyncSupported() const override;
		bool isDirectory(const char *path) const override;
		bool isFile(const char *path) const override;

		std::vector<std::string> listFiles(const char *directory) const override;

		std::vector<std::string> listDirectories(const char *directory) const override;

		std::vector<std::string> list(const char *directory) const override;

		// FileAccess getFileAccess(const char *path) override;
		// TODO determine how to deal with.
		// virtual const char *getBaseName(const char *path);
		// virtual std::string getAbsolutePath(const char *path);
		// virtual std::string getRelativePath(const char *path);
		// virtual const char *getFileExtension(const char *path);

	  public:
		/**
		 * @brief Create a File System object
		 *
		 * @param ip
		 * @param port
		 * @param sch
		 * @return FTPFileSystem*
		 */
		static FTPFileSystem *createFileSystem(const char *ip_address, int port,
											   const Ref<IScheduler> &sch = Ref<IScheduler>(nullptr));

		/**
		 * @brief Create a File System object
		 *
		 * @param port
		 * @param sch
		 * @return FTPFileSystem*
		 */
		static FTPFileSystem *createFileSystem(const IPAddress & ip_address, int port,
											   const Ref<IScheduler> &sch = Ref<IScheduler>(nullptr));

		FTPFileSystem(FTPFileSystem &&other);
		// TODO add remove function.

	  public:
		/**
		 * @brief Set the Credentials object
		 *
		 * @param username
		 * @param password
		 */
		void setCredentials(const std::string &username, const std::string &password);

	  protected:
		FTPFileSystem() = default;
		FTPFileSystem(const char *ip_address, int port, const char *username, const char *password, const Ref<IScheduler> &sch);
		FTPFileSystem(const char *ip_address, int port, const char *username, const char *password);
		~FTPFileSystem() override;

	  private:
		CURL *handle{};
		CURL *multi{};
		std::string URL;
		int port{};
	};
} // namespace fragcore

#endif
