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
#ifndef _FRAG_CORE_ICONFIG_H_
#define _FRAG_CORE_ICONFIG_H_ 1
#include"../Def.h"
#include"SmartReference.h"
#include"Ref.h"
#include"IO/IO.h"
#include"dataStructure/ITree.h"
#include"dataStructure/Iterator.h"
#include <typeinfo>
#include <string>
#include <map>


struct json_object;
namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC IConfig : public SmartReference, public ITree<IConfig> {
	public:
		~IConfig(void);

	public:    /*	Get and set methods.	*/

		/**
		 *
		 */
		enum ConfigFormat {
			Unknown = 0,
			XML = 1,    /*  */
			YAML = 2,    /*  */
			JSON = 3,    /*  */
			Custom = (unsigned int)(-1)
		};

		/**
		 *	Get configuration value.
		 *
		 *	\key key associated with the configuration value.
		 *
		 *	@throws invalid_argument: if the configuration key
		 *	did not exist.
		 *
		 *	@Return configuration value.
		 */
		template<class T>
		T get(const std::string &key) const {
			T tmp;
			const std::type_info &type = typeid(T);
			this->getInternal(key, (void *) &tmp, type);
			return tmp;
		}

		/**
		*	Set configuration value.
		*
		*	\key key associated with the value.
		*
		*	\value value
		*
		*	@throws invalid_argument: if value type is not supported.
		*/
		template<class T>
		void set(const std::string &key, const T &value) {
			const std::type_info &type = typeid(T);
			this->setInternal(key, (const void *) &value, type);
		}

		template<class T>
		T* getBlob(const std::string &key, size_t* size) const {
			T tmp;
			const std::type_info &type = typeid(T);
			this->getInternal(key, (void *) &tmp, type);
			return tmp;
		}

		template<class T>
		void setBlob(const std::string &key, const T &value, size_t t) {
			const std::type_info &type = typeid(T);
			this->setInternal(key, (const void *) &value, type);
		}

		/**
		* Get sub configuration node space.
		* @param key
		* @return
		*/
		virtual IConfig &getSubConfig(const std::string &key);

		/**
		* Check if there exist a element with specific
		* key.
		* @param key string key.
		* @return true if exist, false otherwise.
		*/
		virtual bool isSet(const std::string &key);

		/**
		* Print all elements in the configuration
		* object.
		*/
		virtual void printTable(void) const;

		/**
		* Print all elements in the configuration
		* object.
		* @param file
		*/
		//virtual void printTable(FILE *IO) const;	//TODO change to the reference to make it thread safe.
		//TODO add support for the IO object.
		virtual void printTable(Ref<IO>& io) const;

		//TODO deal with the name.
		virtual void save(Ref<IO>& io, ConfigFormat format);

		virtual void parseConfigFile(Ref<IO>& io, ConfigFormat format = ConfigFormat::Unknown);

		//virtual std::vector<const char*> getKeys();	//TODO add support.

	private:    /*	Internal methods.	*/

		/**
		 *	Set internal configuration value.
		 *
		 *	\key key associated with the value.
		 *
		 *	\value pointer to the value.
		 *
		 *	\type object describing the value data type.
		 */
		virtual void setInternal(const std::string &key, const void *value,
		                         const std::type_info &type);

		/**
		 *	Get configuration value.
		 *
		 *	\key key associated with the value.
		 *
		 *	\value pointer to the value.
		 *
		 *	\type object describing the value data type.
		 */
		virtual void getInternal(const std::string &key, void *value,
		                         const std::type_info &type) const;

		template<class T>
		class IConfigIterator : public Iterator<T>{
			/*	*/
		};
		//TODO add iterator for config arguments.
		//virtual Iterator getIterator(void);

	protected:
		virtual void parse_xml(Ref<IO>& io);
		virtual void parse_yaml(Ref<IO>& io);
		virtual void parse_json(Ref<IO>& io);
		virtual void save_xml(Ref<IO>& io);
		virtual void save_yaml(Ref<IO>& io);
		virtual void save_json(Ref<IO>& io);
	private:
		//TODO deal with the type.
		static void save_json_recursive_config(const IConfig *config, struct json_object *root);
		static void parse_json_recursive_config(IConfig *config, struct json_object *root);

	public:
		virtual IConfig *getSuperInstance(void);

	public:
		IConfig(void);
		IConfig(const IConfig &other);
		IConfig(const IConfig &&other);

	  protected: /*	*/

		/*  Config tree hierarchy.  */
		std::map<std::string, IConfig *> config; /*  */
		IConfig *parent;                        /*  Parent in the config tree.  */

		/*  List of all attributes. */
		std::map<std::string, int> iconfig;         /*	Integer.	*/
		std::map<std::string, float> fconfig;       /*	Float.	*/
		std::map<std::string, std::string> sconfig; /*	String.	*/
		typedef struct blob_t {
			size_t nbytes;
			void *blob;
		} Blob;
		std::map<std::string, Blob> bconfig;
	};
}

#endif
