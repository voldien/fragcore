#include "Core/IConfig.h"
#include "Core/IO/FileSystem.h"
#include "Core/IO/IOUtil.h"
#include "Core/Log.h"
#include "Core/SystemInfo.h"
#include <Core/IO/FileIO.h>
#include <fmt/core.h>
#include <json-c/json.h>
#include <libxml/encoding.h>
#include <libxml/parser.h>
#include <libxml/xmlwriter.h>
#include <stdio.h>
#include <yaml.h>

using namespace fragcore;

IConfig::IConfig(void) { this->parent = nullptr; }

IConfig::IConfig(const IConfig &other) {
	// TODO add copy
}

bool IConfig::isSet(const std::string &key) {
	if (this->iconfig.find(key) != this->iconfig.end())
		return true;
	if (this->fconfig.find(key) != this->fconfig.end())
		return true;
	if (this->sconfig.find(key) != this->sconfig.end())
		return true;
	return false;
}

void IConfig::setInternal(const std::string &key, const void *value, const std::type_info &type) {

	/*  Supported data types.   */
	const std::type_info &i = typeid(int);
	const std::type_info &f = typeid(float);
	const std::type_info &s0 = typeid(std::string);
	const std::type_info &s1 = typeid(char);
	const std::type_info &sp1 = typeid(const char *);
	const std::type_info &b = typeid(bool);

	if (i == type) {
		this->iconfig[key] = *((int *)value);
	} else if (b == type) {
		this->iconfig[key] = (int)*((bool *)value);
	} else if (f == type) {
		this->fconfig[key] = *((float *)value);
	} else if (s0 == type) {
		this->sconfig[key] = *((std::string *)value);
	} else if (type == sp1) {
		const char *cstr = *((const char **)value);
		this->sconfig[key] = std::string(cstr);
	} else if (type.before(s1)) {
		const char *cstr = (const char *)value;
		this->sconfig[key] = std::string(cstr);
	} else
		throw InvalidArgumentException(fmt::format("Invalid argument type %s.", type.name()));
}

void IConfig::getInternal(const std::string &key, void *value, const std::type_info &type) const {

	/*  Supported data types.   */
	const std::type_info &i = typeid(int);
	const std::type_info &f = typeid(float);
	const std::type_info &s0 = typeid(std::string);
	const std::type_info &s1 = typeid(char);
	const std::type_info &b = typeid(bool);
	const std::type_info &s2 = typeid(const char *);

	/*	*/
	std::map<std::string, int>::const_iterator iti;
	std::map<std::string, float>::const_iterator fti;
	std::map<std::string, std::string>::const_iterator sti;

	/*	*/
	if (i == type) {
		iti = iconfig.find(key);
		if (iti != iconfig.end())
			*((int *)value) = (*iti).second;
		else
			throw InvalidArgumentException(fmt::format("Key '%s' don't exists", key.c_str()));
	} else if (type == b) {
		iti = iconfig.find(key);
		if (iti != iconfig.end())
			*((bool *)value) = (bool)(*iti).second;
		else
			throw InvalidArgumentException(fmt::format("Key '%s' don't exists", key.c_str()));
	} else if (f == type) {
		fti = fconfig.find(key);
		if (fti != fconfig.end())
			*((float *)value) = (*fti).second;
		else
			throw InvalidArgumentException(fmt::format("Key '%s' don't exists", key.c_str()));
	} else if (s0 == type) {
		sti = sconfig.find(key);
		if (sti != sconfig.end())
			*((std::string *)value) = (*sti).second;
		else
			throw InvalidArgumentException(fmt::format("Key '%s' don't exists", key.c_str()));
	} else if (s2 == type) {
		sti = sconfig.find(key);
		if (sti != sconfig.end())
			*((const char **)value) = (*sti).second.c_str();
		else
			throw InvalidArgumentException(fmt::format("Key '%s' don't exists", key.c_str()));
	} else if (type.before(s1)) {

	} else
		throw InvalidArgumentException(fmt::format("Invalid argument type '%s'.", type.name()));
}

IConfig &IConfig::getSubConfig(const std::string &key) {
	std::map<std::string, IConfig *>::iterator conIt;

	/*  Check if sub config node exists.    */
	conIt = this->config.find(key);
	if (conIt != this->config.end()) {
		/*  */
		return *(*conIt).second;
	} else {

		/*  Create new subconfig node.  */
		IConfig *_config = getSuperInstance();
		this->addChild(_config);

		/*  */
		_config->setName(key.c_str());
		this->config[key] = _config;

		return *_config;
	}
}

void IConfig::printTable(void) const { this->printTable(SystemInfo::getStdOut()); }

void IConfig::printTable(Ref<IO> &io) const {

	/*	*/
	std::map<std::string, int>::const_iterator iti;
	std::map<std::string, float>::const_iterator fti;
	std::map<std::string, std::string>::const_iterator sti;

	/*  */
	iti = this->iconfig.begin();
	fti = this->fconfig.begin();
	sti = this->sconfig.begin();

	/*  */
	if (this->parent == nullptr)
		IOUtil::format(io, "Configuration table:\n");

	/*  Check the depth.    */
	const IConfig *config = this;
	int depth = 0;
	while (config) {
		config = config->parent;
		depth++;
	}
	char tabNestedSpace[depth * 2 + 1];
	for (int i = 0; i < depth; i++)
		tabNestedSpace[i] = ' ';
	tabNestedSpace[depth] = '\0';

	/*  Display all subconfigs. */
	std::map<std::string, IConfig *>::const_iterator conIT;
	conIT = this->config.begin();
	for (; conIT != this->config.cend(); conIT++) {
		const std::string &name = (*conIT).first;
		IOUtil::format(io, "----- {} ----\n", name.c_str());
		const IConfig *_config = (*conIT).second;
		_config->printTable(io);
	}

	// Display all variables.
	for (; iti != this->iconfig.end(); iti++) {
		const std::string &key = (*iti).first;
		const int hvalue = (*iti).second;
		IOUtil::format(io, "{}{} : {}\n", "\t", key.c_str(), hvalue);
	}

	for (; fti != this->fconfig.end(); fti++) {
		const std::string &key = (*fti).first;
		const float hvalue = (*fti).second;
		IOUtil::format(io, "{}{} : {}\n", "\t", key.c_str(), hvalue);
	}

	for (; sti != this->sconfig.end(); sti++) {
		const std::string &key = (*sti).first;
		const std::string hvalue = (*sti).second;
		IOUtil::format(io, "{}{} : {}\n", "\t", key.c_str(), hvalue.c_str());
	}
}

static IConfig::ConfigFormat predictConfigExtFormat(Ref<IO> &ref) {
	IConfig::ConfigFormat format = IConfig::JSON;

	if (format == IConfig::ConfigFormat::Unknown)
		throw InvalidArgumentException(
			fmt::format("Could not predict file format of IO object: %s", ref->getName().c_str()));

	return format;
}

void IConfig::save(Ref<IO> &io, ConfigFormat format) {
	if (!io->isWriteable())
		throw RuntimeException(fmt::format("IO object %s is not writable.", io->getName().c_str()));

	if (format == ConfigFormat::Unknown)
		format = predictConfigExtFormat(io);

	/*	Call corresponding format save function.    */
	switch (format) {
	case YAML:
		save_yaml(io);
		break;
	case XML:
		save_xml(io);
		break;
	case JSON:
		save_json(io);
		break;
	default:
		throw InvalidArgumentException(::fmt::format("Invalid configuration file fmt::format - {}", format));
	}
}

void IConfig::save_xml(Ref<IO> &io) {
	int rc;
	xmlTextWriterPtr writer;
	xmlBufferPtr buf;
	char snbuf[64];

	/*	*/
	std::map<std::string, int>::iterator iti;
	std::map<std::string, float>::iterator fti;
	std::map<std::string, std::string>::iterator sti;

	xmlInitGlobals();
	xmlInitMemory();
	xmlInitParser();

	xmlResetLastError();

	buf = xmlBufferCreate();
	if (buf == nullptr)
		throw RuntimeException(fmt::format("testXmlwriterMemory: Error creating the xml buffer\n"));

	/*	Create write file.	*/
	writer = xmlNewTextWriterMemory(buf, 0);
	if (!writer)
		throw RuntimeException(fmt::format("xmlNewTextWriterFilename failed"));

	/*	*/
	rc = xmlTextWriterStartDocument(writer, nullptr, "utf-8", nullptr);
	if (rc < 0)
		throw RuntimeException(fmt::format("xmlTextWriterStartDocument failed - {}.", rc));

	/*  */
	rc = xmlTextWriterStartElement(writer, BAD_CAST "Configuration");
	if (rc < 0) {
		xmlFreeTextWriter(writer);
		throw RuntimeException(fmt::format("xmlTextWriterStartElement failed - {}.", rc));
	}

	// TODO add subconfig.
	/*  Display all subconfigs. */
	std::map<std::string, IConfig *>::const_iterator conIT;
	conIT = this->config.begin();
	for (; conIT != this->config.cend(); conIT++) {
		const std::string &name = (*conIT).first;
		IConfig *_config = (IConfig *)(*conIT).second;
		_config->save_xml(io);
	}

	/*  */
	rc = xmlTextWriterSetIndent(writer, 1);
	if (rc < 0)
		throw RuntimeException(fmt::format("Xml Identation failed: %s", xmlGetLastError()->message));

	iti = this->iconfig.begin();
	fti = this->fconfig.begin();
	sti = this->sconfig.begin();

	/*  Write comment.  */
	rc = xmlTextWriterWriteComment(writer, BAD_CAST "sub-element contains all integers configuration values.");
	if (rc == -1) {
		xmlErrorPtr error = xmlGetLastError();
		throw RuntimeException(fmt::format("Failed writing comment : %s", xmlGetLastError()->message));
	}
	/*	Iterate through each int configuration.	*/
	rc = xmlTextWriterSetIndent(writer, 1);
	if (rc == -1)
		throw RuntimeException(fmt::format("Failed to set indentation: %s", xmlGetLastError()->message));
	for (; iti != this->iconfig.end(); iti++) {
		const std::string &key = (*iti).first;
		const int hvalue = (*iti).second;
		sprintf(snbuf, "%d", hvalue);
		xmlTextWriterStartElement(writer, BAD_CAST key.c_str());
		rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "value", BAD_CAST snbuf);
		rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "type", BAD_CAST typeid(int).name());
		if (rc == -1)
			throw RuntimeException(
				fmt::format("Failed writing attribute: %s - key :%s", key.c_str(), snbuf, xmlGetLastError()->message));
		rc = xmlTextWriterEndElement(writer);
		if (rc == -1)
			throw RuntimeException(fmt::format("Failed writing end attribute: %s - key :%s", key.c_str(), snbuf,
											   xmlGetLastError()->message));
	}
	rc = xmlTextWriterSetIndent(writer, 1);
	if (rc == -1)
		throw RuntimeException(fmt::format("Failed to set indentation: %s", xmlGetLastError()->message));

	rc = xmlTextWriterWriteComment(writer, BAD_CAST "sub-element contains all float configuration values.");
	if (rc == -1) {
		xmlErrorPtr error = xmlGetLastError();
		throw RuntimeException(fmt::format("Failed writing comment : %s", xmlGetLastError()->message));
	}

	/*	Iterate through each float configuration.	*/
	rc = xmlTextWriterSetIndent(writer, 2);
	for (; fti != this->fconfig.end(); fti++) {
		const std::string &key = (*fti).first;
		const float hvalue = (*fti).second;
		sprintf(snbuf, "%f", hvalue);
		xmlTextWriterStartElement(writer, BAD_CAST key.c_str());
		rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "value", BAD_CAST snbuf);
		rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "type", BAD_CAST typeid(float).name());
		if (rc == -1)
			throw RuntimeException(fmt::format("Failed writing attribute: %s - key :%s", key.c_str(), snbuf));
		rc = xmlTextWriterEndElement(writer);
		if (rc == -1)
			throw RuntimeException(fmt::format("Failed writing end attribute: %s - key :%s", key.c_str(), snbuf,
											   xmlGetLastError()->message));
	}
	/*  */
	rc = xmlTextWriterSetIndent(writer, 1);
	if (rc == -1)
		throw RuntimeException(fmt::format("Failed to set indentation: %s", xmlGetLastError()->message));

	/*  */
	rc = xmlTextWriterWriteComment(writer, BAD_CAST "sub-element contains all string configuration values.");
	if (rc == -1) {
		throw RuntimeException(fmt::format("Failed writing comment : %s", xmlGetLastError()->message));
	}

	/*	Iterate through each string configuration.	*/
	rc = xmlTextWriterSetIndent(writer, 2);
	for (; sti != this->sconfig.end(); sti++) {
		const std::string &key = (*sti).first;
		const std::string hvalue = (*sti).second;
		xmlTextWriterStartElement(writer, BAD_CAST key.c_str());
		rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "value", BAD_CAST hvalue.c_str());
		rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "type", BAD_CAST typeid(const char *).name());
		if (rc == -1)
			throw RuntimeException(fmt::format("Failed writing attribute: %s - key :%s", key.c_str(), hvalue.c_str()));
		rc = xmlTextWriterEndElement(writer);
		if (rc == -1)
			throw RuntimeException(fmt::format("Failed writing end attribute: %s - key :%s", key.c_str(), snbuf,
											   xmlGetLastError()->message));
	}
	rc = xmlTextWriterSetIndent(writer, 1);
	if (rc == -1)
		throw RuntimeException(fmt::format("Failed to set indentation: %s", xmlGetLastError()->message));

	/*  */
	rc = xmlTextWriterSetIndent(writer, 0);
	if (rc == -1)
		throw RuntimeException(fmt::format("Failed to set indentation: %s", xmlGetLastError()->message));
	rc = xmlTextWriterEndElement(writer);
	if (rc == -1)
		throw RuntimeException(fmt::format("Failed to set indentation: %s", xmlGetLastError()->message));
	rc = xmlTextWriterEndDocument(writer);
	if (rc == -1)
		throw RuntimeException(fmt::format("Failed to set indentation: %s", xmlGetLastError()->message));

	/*	Clean up.	*/
	xmlFreeTextWriter(writer);
	xmlCleanupParser();
	xmlMemoryDump();

	/*  Write.  */
	io->write(buf->use, buf->content);
	xmlBufferFree(buf);
}

void IConfig::save_yaml(Ref<IO> &io) {

	yaml_document_t document;
	yaml_tag_directive_t s, e;

	if (!yaml_document_initialize(&document, nullptr, &s, &e, 1, 1)) {
		throw RuntimeException("");
	}

	yaml_node_t *root = yaml_document_get_root_node(&document);
	// yaml_doc

	yaml_emitter_t emitter;
	/* Initialize parser */
	if (!yaml_emitter_initialize(&emitter)) {
		throw RuntimeException("");
	}

	// yaml_emitter_set_output_file(&emitter, file);
	yaml_emitter_dump(&emitter, &document);

	/*  Release.    */
	yaml_emitter_delete(&emitter);
	yaml_document_delete(&document);

	//
	//    yaml_parser_t parser;
	//    yaml_token_t token;
	//
	//
	//    if (!yaml_parser_initialize(&parser))
	//        fputs("Failed to initialize parser!\n", stderr);
	//    if (fh == nullptr)
	//        fputs("Failed to open file!\n", stderr);
	//    yaml_parser_set_encoding(&parser, YAML_UTF8_ENCODING);
	//    yaml_parser_delete(&parser);
}

void IConfig::save_json_recursive_config(const IConfig *config, struct json_object *root) {

	std::map<std::string, IConfig *>::const_iterator conIT;
	std::map<std::string, int>::const_iterator iti;
	std::map<std::string, float>::const_iterator fti;
	std::map<std::string, std::string>::const_iterator sti;
	std::map<std::string, Blob>::const_iterator bti;

	struct json_object *configRoot = json_object_new_object();

	iti = config->iconfig.cbegin();
	fti = config->fconfig.cbegin();
	sti = config->sconfig.cbegin();
	bti = config->bconfig.cbegin();
	for (; iti != config->iconfig.cend(); iti++) {
		json_object_object_add(configRoot, (*iti).first.c_str(), json_object_new_int((*iti).second));
	}
	for (; fti != config->fconfig.cend(); fti++) {
		json_object_object_add(configRoot, (*fti).first.c_str(), json_object_new_double((*fti).second));
	}
	for (; sti != config->sconfig.cend(); sti++) {
		json_object_object_add(configRoot, (*sti).first.c_str(), json_object_new_string((*sti).second.c_str()));
	}
	for (; bti != config->bconfig.cend(); bti++) {
		// json_object_object_add(configRoot, (*sti).first.c_str(), json_object_new_string((*bti).second.blob));
	}

	json_object_object_add(root, config->getName().c_str(), configRoot);

	/*  Save recursive. */
	conIT = config->config.cbegin();
	for (conIT; conIT != config->config.end(); conIT++)
		save_json_recursive_config((*conIT).second, configRoot);
}

void IConfig::save_json(Ref<IO> &io) {
	struct json_object *root;

	std::map<std::string, int>::iterator iti;
	std::map<std::string, float>::iterator fti;
	std::map<std::string, std::string>::iterator sti;
	std::map<std::string, Blob>::iterator bti;

	root = json_object_new_object();
	/*  Save recursive. */
	save_json_recursive_config(this, root);

	/*  Get the final output. */
	const char *json = json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY);
	long int size = strlen(json);

	/*  Write to file.  */
	io->write(size, json);
	json_object_put(root); // Delete the json object
}

void IConfig::parseConfigFile(Ref<IO> &io, ConfigFormat format) {
	if (!io->isReadable())
		throw RuntimeException(fmt::format("IO object %s is not readable.", io->getName().c_str()));

	if (format == ConfigFormat::Unknown)
		format = predictConfigExtFormat(io);

	/*	Call corresponding format parse function.    */
	switch (format) {
	case YAML:
		parse_yaml(io);
		break;
	case XML:
		parse_xml(io);
		break;
	case JSON:
		parse_json(io);
		break;
	default:
		throw InvalidArgumentException(fmt::format("Invalid configuration file format - {}", format));
	}
}

void IConfig::parse_xml(Ref<IO> &io) {
	xmlDoc *document;
	xmlNode *root, *first_child, *node, *rootnode;

	char *buffer;
	int size = IOUtil::loadFileMem(io, &buffer);

	/*	Load xml file.  */
	// document = xmlReadFile(path, nullptr, 0);
	document = xmlReadMemory((const char *)buffer, size, nullptr, "UTF-8", 0);
	root = xmlDocGetRootElement(document);

	for (rootnode = root; rootnode; rootnode = rootnode->next) {
		if (strcmp((char *)rootnode->name, "Configuration") == 0) {
			first_child = root->children;
			/*	Iterate through each children of root node.	*/
			for (node = first_child; node; node = node->next) {
				if (strcmp((char *)node->name, "text") != 0 && node->type == XML_ELEMENT_NODE) {
					std::string name = std::string((const char *)node->name);
					const char *value = (char *)xmlGetProp(node, (xmlChar *)"value");
					const char *type = (char *)xmlGetProp(node, (xmlChar *)"type");

					/*  Write each xml entry to the config.   */
					if (strcmp(type, "f") == 0)
						this->set<float>(name, strtof(value, nullptr));
					else if (strcmp(type, "i") == 0)
						this->set<int>(name, strtol(value, nullptr, 10));
					else if (strcmp(type, "PKc") == 0)
						this->set(name, value);
					else
						throw RuntimeException(fmt::format("Invalid attribute %s", type));
				}
			}
		}
	}

	/*	Cleanup.	*/
	xmlCleanupParser();
	xmlMemoryDump();
	free(buffer);
}

void IConfig::parse_yaml(Ref<IO> &io) {
	yaml_parser_t parser;
	yaml_token_t token;
	char *data;
	if (!yaml_parser_initialize(&parser))
		throw RuntimeException("Failed to initialize parser.");

	size_t size = (size_t)IOUtil::loadFileMem(io, &data);
	yaml_parser_set_input_string(&parser, (const unsigned char *)data, size);
	yaml_parser_set_encoding(&parser, YAML_UTF8_ENCODING);

	do {
		/* As this is an example, I'll just use:
		 *  state = 0 = expect key
		 *  state = 1 = expect value
		 */
		int state = 0;
		char **datap;
		char *tk;

		int c = yaml_parser_scan(&parser, &token);
		if (c == 0)
			throw RuntimeException("");

		switch (token.type) {
		case YAML_KEY_TOKEN:
			state = 0;
			break;
		case YAML_VALUE_TOKEN:
			state = 1;
			break;
		case YAML_SCALAR_TOKEN:
			tk = (char *)token.data.scalar.value;
			break;
		default:
			break;
		}
		if (token.type != YAML_STREAM_END_TOKEN)
			yaml_token_delete(&token);

	} while (token.type != YAML_STREAM_END_TOKEN);
	yaml_token_delete(&token);

	yaml_parser_delete(&parser);
	free(data);
}

void IConfig::parse_json_recursive_config(IConfig *config, struct json_object *root) {
	int val_type, i;
	char *val_type_str, *str;
	int intvalue, str_len;
	json_object *jsonObject;
	long nbyteRead = 0;
	const int nbatch = 1000;
	double dou;
	int pbatch = 0;

	json_object_object_foreach(root, key, value) {
		Log::log(Log::Verbose, "key: \"%s\", type of val: ", key);
		val_type = json_object_get_type(value);

		switch (val_type) {
		case json_type_null:
			config->set<char *>(key, nullptr);
			break;
		case json_type_boolean:
			intvalue = json_object_get_boolean(value);
			config->set<bool>(key, (bool)intvalue);
			break;
		case json_type_double:
			dou = json_object_get_double(value);
			config->set<float>(key, (float)dou);
			break;
		case json_type_int:
			intvalue = json_object_get_int(value);
			config->set<int>(key, intvalue);
			break;
		case json_type_string:
			str = (char *)json_object_get_string(value);
			config->set<char *>(key, str);
			break;

		case json_type_object: {
			jsonObject = json_object_get(value);
			IConfig &subConfig = config->getSubConfig(key);
			parse_json_recursive_config(&subConfig, jsonObject);
		} break;
		case json_type_array:
			break;
		default:
			break;
		}
	}
}

void IConfig::parse_json(Ref<IO> &io) {

	/*  */
	char buf[1024 * 4];
	int len;

	/*  */
	json_object *jsonObject;
	long nbyteRead = 0;
	struct json_tokener *jsonTokener = json_tokener_new();

	while ((len = io->read(sizeof(buf), buf)) > 0) {
		int offbuf = 0;
		do {
			json_object *obj = json_tokener_parse_ex(jsonTokener, &buf[offbuf], len - offbuf);
			enum json_tokener_error jerr = json_tokener_get_error(jsonTokener);
			if (jerr == json_tokener_continue) {
				// Need more memory.
				break;
			} else if (jerr != json_tokener_success) {
				throw RuntimeException(fmt::format("Failed parsing Json file: %s with error: %s", io->getName().c_str(),
												   json_tokener_error_desc(jerr)));
			}
			offbuf += jsonTokener->char_offset;

			if (obj == nullptr)
				continue;
			this->parse_json_recursive_config(this, obj);

			json_object_put(obj);
		} while (1);
		nbyteRead += len;
	}
	json_tokener_free(jsonTokener);
}

IConfig::~IConfig(void) {
	/*  Release all resources.  */

	/*  */
	std::map<std::string, IConfig *>::iterator conIT = this->config.begin();
	for (; conIT != this->config.end(); conIT++) {
		IConfig *_config = (*conIT).second;
		// TODO deal with the references.
		if (_config->deincreemnt())
			delete _config;
	}
}

IConfig *IConfig::getSuperInstance(void) { return new IConfig(); }
