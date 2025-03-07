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
#ifndef _FRAGCORE_SFTP_FILESYSTEM_H_
#define _FRAGCORE_SFTP_FILESYSTEM_H_ 1
#include "FTPFileSystem.h"

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC SFTPFileSystem : public FTPFileSystem {
	  public:
		/**
		 * @brief Create a File System object
		 *
		 * @param ip
		 * @param port
		 * @param sch
		 * @return FTPFileSystem*
		 */
		static SFTPFileSystem *createFileSystem(const char *ipAddress, int port,
												const Ref<IScheduler> &sch = Ref<IScheduler>(nullptr));

		/**
		 * @brief Create a File System object
		 *
		 * @param port
		 * @param sch
		 * @return FTPFileSystem*
		 */
		static SFTPFileSystem *createFileSystem(const IPAddress &ipAddress, int port,
												const Ref<IScheduler> &sch = Ref<IScheduler>(nullptr));

		SFTPFileSystem(SFTPFileSystem &&other);
		// TODO add remove function.

	  
		/**
		 * @brief Set the Credentials object
		 *
		 * @param username
		 * @param password
		 */
		void setCredentials(const std::string &username, const std::string &password);

	  protected:
		SFTPFileSystem() = default;
		SFTPFileSystem(const char *ipAddress, int port, const char *username, const char *password,
					   const Ref<IScheduler> &sch);
		SFTPFileSystem(const char *ipAddress, int port, const char *username, const char *password);
		~SFTPFileSystem() override;

	  private:
		CURL *handle{};
		CURL *multi{};
		char *URL{};
		int port{};
	};
} // namespace fragcore

#endif
