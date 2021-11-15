#ifndef _FRAG_CORE_VK_COMPUTE_H_
#define _FRAG_CORE_VK_COMPUTE_H_ 1
#include "../ICompute.h"
#include "../RenderPrerequisites.h"
#include <Core/Module.h>
#include <Core/Ref.h>
#include <Core/SmartReference.h>
#include <Def.h>

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC VKCompute : public ICompute {
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
		virtual void dispatchCompute(unsigned int *global, unsigned int *local, unsigned int offset = 0) = 0;
		// TODO remove and let command list!
		// TODO add memory barrier.
		// TODO add enumerator as argumment for all types of barries.
		virtual void memoryBarrier() = 0;

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
		ICompute(const ICompute &other) = default;
		ICompute(ICompute &&other) = delete;
		virtual ~ICompute() = default;
	};
} // namespace fragcore
#endif
