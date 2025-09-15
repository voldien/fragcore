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
#ifndef _FRAGCORE_CONFIG_H_
#define _FRAGCORE_CONFIG_H_ 1
#include "DataStructure/ITree.h"
#include "FragDef.h"
#include "Object.h"
#include <cctype>
#include <cstring>
#include <fmt/format.h>
#include <map>
#include <string>
#include <typeinfo>
#include <utility>

#if __cplusplus >= 201603L
#define FRAG_CORE_NODISCARD [[nodiscard]]
#else
#define FRAG_CORE_NODISCARD
#endif

namespace fragcore {

	/*	forward decl.	*/
	template <typename T> class ValueType;

	class AbstractValue {
	  public:
		virtual ~AbstractValue() = default;

		template <class T> const ValueType<T> &as() const { return reinterpret_cast<const ValueType<T> &>(*this); }
		template <class T> ValueType<T> &as() { return reinterpret_cast<ValueType<T> &>(*this); }
	};

	template <typename T> class ValueType : public AbstractValue {
	  public:
		using ObjecType = T;
		ValueType() = default;
		ValueType(const T &val) : value(val) {}
		~ValueType() override = default;

		const T &getValue() const noexcept {
#ifdef CXXCONF_NO_RTTI
			return static_cast<const T &>(this->value);
#else
			return this->value;
#endif
		}
		virtual unsigned int getHashCode() { return typeid(T).hash_code(); }

	  private:
		T value;
	};

	struct blob_t {
		size_t nbytes;
		void *blob;
	};

	template class ValueType<float>;
	template class ValueType<int>;
	template class ValueType<std::string>;
	template class ValueType<bool>;
	template class ValueType<struct blob_t>;

	class IConfigBase : Object {
	  public:
		IConfigBase() = default;

	  private:
		std::string name;
	};

	/**
	 * @brief
	 *
	 */
	class IConfig : public ITree<ValueType<IConfigBase>>, public ValueType<IConfigBase> {
	  public:
		/**
		 * @brief Construct a new IConfig object
		 *
		 * @param parent
		 */
		IConfig(IConfig *parent = nullptr) { this->setParent(parent); }
		IConfig(const IConfig &other) = default;
		IConfig(IConfig &&other) : _mapSubConfig(std::move(other._mapSubConfig)), va_va(std::move(other.va_va)) {}
		~IConfig() override  {
			/*	Delete attributes.	*/
			for (auto it = this->va_va.begin(); it != this->va_va.end(); it++) {
				delete (*it).second;
			}
		}

		IConfig &operator=(const IConfig &other) {
			ValueType<IConfigBase>::operator=(other);

			this->va_va = other.va_va;
			this->_mapSubConfig = other._mapSubConfig;
			return *this;
		}
		IConfig &operator=(IConfig &&other) {
			ValueType<IConfigBase>::operator=(other);

			this->_mapSubConfig = std::move(other._mapSubConfig);
			this->va_va = std::move(other.va_va);
			return *this;
		}

		/*	Get and set methods.	*/
		const AbstractValue &operator[](const std::string &key) { return *this->va_va[key]; }

		template <class T> T get(const std::string &key) const {
			static_assert(std::is_class_v<ValueType<T>> || std::is_floating_point_v<T> ||
							  std::is_integral_v<T>,
						  "Invalid Data Type");
			if (this->has_ref(key)) {
				return this->va_va.at(key)->as<T>().getValue();
			}
			throw InvalidArgumentException("Element with key {} not found", key);
		}

		template <class T> T &get_ref(const std::string &key) const {
			if (this->has_ref(key)) {
				return this->va_va.at(key)->as<T &>().getValue();
			}
			throw InvalidArgumentException("Element with key {} not found", key);
		}

		template <class T> T *get_ref(const std::string &key) const {
			if (this->has_ref(key)) {
				return this->va_va.at(key)->as<T *>().getValue();
			}
			throw InvalidArgumentException("Element with key {} not found", key);
		}

		bool has_ref(const std::string &key) const { return this->va_va.find(key) != this->va_va.end(); }

		template <class T> void set(const std::string &key, const T &value) /*noexcept(noexcept(isSet(key)))*/ {
			static_assert(std::is_class_v<ValueType<T>> || std::is_floating_point_v<T> ||
							  std::is_integral_v<T>,
						  "Must be a supported type");
			auto val = new ValueType<T>(value);
			this->va_va[key] = static_cast<AbstractValue *>(val);
		}

		virtual IConfig &getSubConfig(const std::string &key) {
			IConfig config(this);

			this->set(key, config);

			return this->get<IConfig &>(key);
		}

		virtual bool tryGetSubConfig(const std::string &key, IConfig &config) noexcept {
			if (this->getNumChildren() <= 0) {
				return false;
			}
			if (!this->has_ref(key)) {
				return false;
			}
			config = getSubConfig(key);
			return true;
		}

		/**
		 * Check if there exist a element with specific
		 * key.
		 * @param key string key.
		 * @return true if exist, false otherwise.
		 */
		virtual bool isSet(const std::string &key) { return this->va_va.find(key) != this->va_va.end(); }

	  protected: /*	*/
		/*  Config tree hierarchy.  */
		std::map<std::string, ITree<IConfig> *> _mapSubConfig; /*  */
		std::map<std::string, AbstractValue *> va_va;
	};

} // namespace fragcore

#endif
