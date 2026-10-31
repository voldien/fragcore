#pragma once
#include "../FragDef.h"
#include <functional>
#include <vector>

namespace fragcore {

	template <typename... Args> class FVDECLSPEC Action {
	  public:
		Action() { this->listeners.reserve(16); }

		/*	*/
		void invoke(Args &...args) const noexcept {
			for (auto &listener : listeners) {
				listener(args...);
			}
		}

		/*	*/
		void addListener(const std::function<void(Args...)> &func) { this->listeners.push_back(func); }
		void removeListener(const std::function<void(Args...)> &func) {

			/*	*/
			const auto find_it = std::find_if(
				listeners.cbegin(), listeners.cend(), [&func](const std::function<void(Args...)> &element) {
					return element.template target<void(Args...)>() == func.template target<void(Args...)>();
				});

			/*	*/
			if (find_it != listeners.cend()) {
				listeners.erase(find_it);
			}
		}

		/*	*/
		void operator+=(const std::function<void(Args...)> &func) { this->addListener(func); }
		void operator-=(const std::function<void(Args...)> &func) { this->removeListener(func); }

		/**
		 * @brief Add Object Method Function.
		 */
		template <typename ObjType, typename Func> void constexpr addBinding(ObjType &&obj, Func &&func) {
			this->addListener(std::bind(func, obj));
		}

		/**
		 * @brief Remove Object Method Function.
		 */
		template <typename ObjType, typename Func> void constexpr removeBinding(ObjType &&obj, Func &&func) {
			this->removeListener(std::bind(func, obj));
		}

		void operator()(Args &...args) noexcept { this->invoke(args...); }

	  private:
		std::vector<std::function<void(Args...)>> listeners;
	};

} // namespace fragcore