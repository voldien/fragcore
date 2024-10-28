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
#ifndef _FRAGCORE_BASE64_H_
#define _FRAGCORE_BASE64_H_ 1
#include "../FragDef.h"
#include "IBase.h"

namespace fragcore {
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC Base64 : public IBase {
	  public:
		 void encode(Ref<IO> &input, Ref<IO> &encoded) override;
		 void decode(Ref<IO> &encoded, Ref<IO> &input) override;
	};
} // namespace fragcore

#endif
