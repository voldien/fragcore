/**
	FrameView for rendering shaders in screen space.
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
#ifndef _FRAG_CORE_CONFIG_H_
#define _FRAG_CORE_CONFIG_H_ 1
#include <Core/IConfig.h>
#include <Core/Object.h>
#include <FragDef.h>
#include <map>
#include <string>
#include <typeinfo>

/**
 *	Responsible for the configuration
 *	of the application.
 */
class FVDECLSPEC Config : public fragcore::IConfig, public fragcore::Object {
  public:
	~Config();

  private: /*	Internal methods.	*/
	/**
	 *	Parser getopt option argument and
	 *	assign it to config object.
	 *
	 */
	virtual void parseGetOpt(int argc, const char **argv);

	fragcore::IConfig *getSuperInstance(void);

	/**
	 *	Set default configuration options
	 *	for this application.
	 */
	virtual void setDefaultOption();

  private: /*	*/
	Config();
	Config(const Config &other);

  public: /*	Static factory methods.	*/
	/**
	 *
	 * @param argc number of argument in argv pointer array.
	 * @param argv string pointer array.
	 * @param configpath path for override the default
	 * configuration of the system
	 * @throws invalid_argument: for invalid configuration file path.
	 * @return non-null Config object.
	 */
	static Config *createConfig(int argc, const char **argv, // TODO add sandbox config
								const char *configpath);

	/**
	 * Get configuration file path.
	 * @param argc
	 * @param argv
	 * @return non-null string.
	 */
	static const char *getConfigFilePath(int argc, const char **argv);
};

#endif
