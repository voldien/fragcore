#ifndef _FRAG_CORE_ICOMPUTE_H_
#define _FRAG_CORE_ICOMPUTE_H_ 1
#include "../Def.h"
#include "../Core/SmartReference.h"
#include "../Prerequisites.h"
#include "../Core/Module.h"

namespace fragcore {
	/**
	 * 
	 */
	class FVDECLSPEC ICompute : public Module {
	public:

		virtual void OnInitialization(void);
		virtual void OnDestruction(void);
	
		/*	*/
		virtual bool supportCompute(void);
		/**
		 * Dispatch compute program.
		 * @param global global workgroup.
		 * @param local local workgroup
		 * @param offset offset in indirect buffer in number of bytes.
		 */
		//virtual void dispatchCompute(Shaer* shader, unsigned int* global);
		virtual void dispatchCompute(unsigned int *global, unsigned int *local, unsigned int offset = 0) = 0;
		//TODO add memory barrier.
		//TODO add enumerator as argumment for all types of barries.
		virtual void memoryBarrier(void) = 0;

		// virtual Sync *createSync(SyncDesc *desc);

		// virtual void deleteSync(Sync *sync);

		virtual const char *getVersion(void) const;

		// virtual Shader* createComputeProgram(void);
		// virtual Shader* deleteComputeProgram(void);
		// virtual void getComputeCapabilties(void);
		//virtual void getDeviceInfo(std::vector<DeviceInfo> &devices);

		ICompute(void) = default;
		~ICompute(void) = default;
	};
}
#endif