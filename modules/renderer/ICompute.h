/*
 *	FragCore - Core Framework Functionalities for Game Engines
 *	Copyright (C) 2018  Valdemar Lindberg
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef _FRAG_CORE_ICOMPUTE_H_
#define _FRAG_CORE_ICOMPUTE_H_ 1
#include "RenderPrerequisites.h"
#include <Core/Module.h>
#include <Core/Ref.h>
#include <Core/SmartReference.h>
#include <FragDef.h>

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC ICompute : public Module {
	  public:
		virtual void OnInitialization();
		virtual void OnDestruction();

		/**
		 * @brief
		 *
		 * @return true
		 * @return false
		 */
		virtual bool supportCompute() noexcept;
		/**
		 * Dispatch compute program.
		 * @param global global workgroup.
		 * @param local local workgroup
		 * @param offset offset in indirect buffer in number of bytes.
		 */
		// virtual void dispatchCompute(Shaer* shader, unsigned int* global);
		// TODO remove and let command list!
		//virtual void dispatchCompute(unsigned int *global, unsigned int *local, unsigned int offset = 0) = 0;
		// TODO remove and let command list!
		// TODO add memory barrier.
		// TODO add enumerator as argumment for all types of barries.
		//virtual void memoryBarrier() = 0;

		// virtual Sync *createSync(SyncDesc *desc);

		// virtual void deleteSync(Sync *sync);

		virtual const char *getVersion() const;

		// virtual Shader* createComputeProgram();
		// virtual Shader* deleteComputeProgram();
		// virtual void getComputeCapabilties();
		// virtual void getDeviceInfo(std::vector<DeviceInfo> &devices);

		/**
		 * @brief Create a Command Buffer object
		 *
		 * @return CommandList*
		 */
		virtual CommandList *createCommandBuffer();

		/**
		 * @brief
		 *
		 * @param list
		 */
		virtual void submittCommand(Ref<CommandList> &list);

		/**
		 * @brief
		 *
		 * @param list
		 */
		virtual void execute(CommandList *list);

		ICompute() = default;
		ICompute(const ICompute &other) = delete;
		ICompute(ICompute &&other) = delete;
		virtual ~ICompute() = default;
	};
} // namespace fragcore
#endif
