#ifndef _FRAG_CORE_FILEACCESS_H_
#define _FRAG_CORE_FILEACCESS_H_ 1
#include "../SmartReference.h"

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC FileAccess : public SmartReference {
	  public:
		FileAccess();
		~FileAccess();

		virtual bool readable() const;
		virtual bool writeable() const;
		virtual bool executable() const;

		//		virtual uint32_t _get_unix_permissions() = 0;
		//		virtual void _set_unix_permissions(uint32_t p_permissions) = 0;
	};
} // namespace fragcore

#endif
