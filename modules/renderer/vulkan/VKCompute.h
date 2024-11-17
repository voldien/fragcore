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
#ifndef _FRAGCORE_VK_COMPUTE_H_
#define _FRAGCORE_VK_COMPUTE_H_ 1
#include "../ICompute.h"
#include "../RenderPrerequisites.h"
#include <Core/Module.h>
#include <Core/SmartReference.h>
#include <FragDef.h>

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC VKCompute : public ICompute {
	  public:
		void OnInitialization() override;
		void OnDestruction() override;

		/**
		 * @brief
		 *
		 * @return true
		 * @return false
		 */
		bool supportCompute() noexcept override;
		/**
		 * Dispatch compute program.
		 * @param global global workgroup.
		 * @param local local workgroup
		 * @param offset offset in indirect buffer in number of bytes.
		 */
		// virtual void dispatchCompute(Shaer* shader, unsigned int* global);
		// TODO remove and let command list!
		virtual void dispatchCompute(unsigned int *global, unsigned int *local, unsigned int offset = 0) = 0;
		// TODO remove and let command list!
		// TODO add memory barrier.
		// TODO add enumerator as argumment for all types of barries.
		virtual void memoryBarrier() = 0;

		// virtual Sync *createSync(SyncDesc *desc);

		// virtual void deleteSync(Sync *sync);

		const char *getVersion() const override;

		// virtual Shader* createComputeProgram();
		// virtual Shader* deleteComputeProgram();
		// virtual void getComputeCapabilties();
		// virtual void getDeviceInfo(std::vector<DeviceInfo> &devices);

		/**
		 * @brief Create a Command Buffer object
		 *
		 * @return CommandList*
		 */
		CommandList *createCommandBuffer() override;

		/**
		 * @brief
		 *
		 * @param list
		 */
		void submittCommand(Ref<CommandList> &list) override;

		/**
		 * @brief
		 *
		 * @param list
		 */
		void execute(CommandList *list) override;

		ICompute() = default;
		ICompute(const ICompute &other) = default;
		ICompute(ICompute &&other) = delete;
		~VKCompute() override = default;
	};
} // namespace fragcore
#endif
