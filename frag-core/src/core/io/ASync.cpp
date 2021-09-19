#include "Core/IO/ASync.h"
#include "Core/IO/IFileSystem.h"
#include "Exception/InvalidArgumentException.h"
#include "Exception/RuntimeException.h"

#include <condition_variable>
#include <mutex>
#include <taskSch.h> //TOOD remove once semaphore class has been implemented.
#include <thread>
#include<fmt/core.h>
using namespace fragcore;

class FVDECLSPEC AsyncTask : public Task {
  public:
	virtual void Execute(void) noexcept override {}
	virtual void Complete(void) noexcept override {}
};

ASyncHandle ASync::asyncOpen(Ref<IO> &io) {

	if (*scheduler == nullptr)
		throw RuntimeException("Async not initialized with a scheduler object");
	/*  Check parameters.   */
	if (!*io)
		throw InvalidArgumentException("Invalid IO reference.");

	/*  Increment reference.    */
	io->increment();

	/*  Create async object.*/
	ASyncHandle handle = this->uidGenerator.getNextLUID();
	AsyncObject *asyncObject = createObject(handle);
	asyncObject->ref = io;
	asyncObject->semaphore = nullptr;
	asyncObject->buffer = nullptr;
	asyncObject->size = 0;
	asyncObject->userData = nullptr;
	asyncObject->callback = nullptr;

	return handle;
}

void ASync::asyncReadFile(ASyncHandle handle, char *buffer, unsigned int size, AsyncComplete complete) {
	int error;
	/*  Get object and check if exists. */
	AsyncObject *ao = getObject(handle);

	if (!ao->ref->isReadable())
		throw RuntimeException(fmt::format("IO object is not readable {}", ao->ref->getUID()));

	/*  Assign variables.   */
	error = schCreateSemaphore((schSemaphore **)&ao->semaphore);
	if (error != SCH_OK)
		throw RuntimeException(fmt::format("Failed to create semaphore %s", schErrorMsg(error)));

	ao->buffer = buffer;
	ao->size = size;
	ao->callback = complete;
	ao->userData = nullptr;

	/*  Reset status counter.   */
	ao->status.nbytes = 0;
	ao->status.offset = 0;

	AsyncTask readTask;
	readTask.callback = async_read;
	readTask.userData = ao;

	this->scheduler->addTask(&readTask);
}

void ASync::asyncReadFile(ASyncHandle handle, Ref<IO> &writeIO, AsyncComplete complete) {
	AsyncObject *ao = getObject(handle);

	/*  Check that open file is readable.   */
	if (!ao->ref->isReadable())
		throw RuntimeException(fmt::format("IO object is not writable {}", ao->ref->getUID()));
	/*  Check if IO object is writeable.    */
	if (!writeIO->isWriteable())
		throw RuntimeException(fmt::format("IO object is not writable {}", writeIO->getUID()));
}

void ASync::asyncWriteFile(ASyncHandle handle, char *buffer, unsigned int size, AsyncComplete complete) {
	int error;
	AsyncObject *ao = getObject(handle);
	// TODO perhaps can to use the IOBuffer has a interface object for reduced coupling and higher cohesion.
	if (!ao->ref->isWriteable())
		throw RuntimeException(fmt::format("IO object is not writable {}", ao->ref->getUID()));

	/*  Assign variables.   */
	error = schCreateSemaphore((schSemaphore **)&ao->semaphore);
	if (error != SCH_OK)
		throw RuntimeException(fmt::format("Failed to create semaphore %s", schErrorMsg(error)));

	ao->buffer = buffer;
	ao->size = size;
	ao->callback = complete;
	ao->userData = nullptr;

	/*  Reset status counter.   */
	ao->status.nbytes = 0;
	ao->status.offset = 0;

	AsyncTask readTask;
	readTask.callback = async_write;
	readTask.userData = ao;
	this->scheduler->addTask(&readTask);
}

void ASync::asyncWriteFile(ASyncHandle handle, Ref<IO> &io, AsyncComplete complete) {}

Ref<IO> ASync::getIO(ASyncHandle handle) const { return getObject(handle)->ref; }

const ASync::IOStatus &ASync::getIOStatus(ASyncHandle handle) const { return this->getObject(handle)->status; }

Ref<IScheduler> ASync::getScheduler(void) const { return this->scheduler; }

void ASync::asyncWait(fragcore::ASyncHandle handle) {
	AsyncObject *ao = getObject(handle);

	schSemaphoreWait((schSemaphore *)ao->semaphore);

	// schCreateSemaphore(ao->semaphore);
	//	schSemaphorePost((schSemaphore *) ao->semaphore);
	/*  Set as finished.    */
	// ao->status.
}

bool ASync::asyncWait(ASyncHandle handle, long int timeout) {
	AsyncObject *ao = getObject(handle);

	return true; // schSemaphoreTimedWait((schSemaphore *)ao->semaphore, timeout) == SCH_OK;
}

void ASync::asyncClose(ASyncHandle handle) {
	AsyncObject *ao = this->getObject(handle);

	/*  Check status of the scheduler.  */
	// TODO determine.

	asyncWait(handle);

	/*  If not used by scheduler anymore, delete all references.    */
	schDeleteSemaphore(ao->semaphore);
	if (ao->ref->deincreemnt())
		ao->ref->close();

	this->asyncs.erase(this->asyncs.find(handle));
}

void ASync::async_open(Task *task) {

	// ASyncHandle handle = (ASyncHandle)task->userData;
	AsyncObject *ao = (AsyncObject *)task->userData;

	// const char *path = (const char *)ao->begin;
	// ASync *async = (ASync *) package->puser;
	// IFileSystem *fileSystem;

	// Ref<IO> refIO = Ref<IO>(fileSystem->openFile(path, IO::READ));

	/*  Create the task in succession.  */
}

void ASync::async_read(Task *task) {
	AsyncObject *ao = (AsyncObject *)task->userData;
	const size_t block_size = 512;

	Ref<IO> &io = ao->ref;
	long int nread;
	while (ao->status.nbytes < ao->size) {
		long int nread = io->read(block_size, &ao->buffer[ao->status.nbytes]);
		if (nread <= 0)
			break;
		ao->status.nbytes += nread;
	}

	/*  Finished.   */
	if (ao->callback)
		ao->callback(nullptr, 0);
	/*  Finish and posted for the data is available.  */
	schSemaphorePost((schSemaphore *)ao->semaphore);
}

void ASync::async_read_io(Task *task) {
	AsyncObject *ao = (AsyncObject *)task->userData;
	const size_t block_size = 512;
	// TODO update for the IO based..
	Ref<IO> &io = ao->ref;
	long int nread;
	while (ao->status.nbytes < ao->size) {
		long int nread = io->read(block_size, &ao->buffer[ao->status.nbytes]);
		if (nread <= 0)
			break;
		ao->status.nbytes += nread;
	}

	/*  Finished.   */
	if (ao->callback)
		ao->callback(nullptr, 0);
	/*  Finish and posted for the data is available.  */
	schSemaphorePost((schSemaphore *)ao->semaphore);
}

void ASync::async_write(Task *task) {
	AsyncObject *ao = (AsyncObject *)task->userData;
	const size_t block_size = 512;

	Ref<IO> &io = ao->ref;
	long int nwritten;
	while (ao->status.nbytes < ao->size) {
		long int nwritten = io->write(block_size, &ao->buffer[ao->status.nbytes]);
		if (nwritten <= 0)
			break;
		ao->status.nbytes += nwritten;
	}

	io->flush();
	if (ao->callback)
		ao->callback(nullptr, 0);
	/*  Finish and posted for the data is available.  */
	schSemaphorePost((schSemaphore *)ao->semaphore);
}

void ASync::async_write_io(Task *task) { return; }

ASync::AsyncObject *ASync::getObject(ASyncHandle handle) {

	if (this->asyncs.find(handle) != this->asyncs.end())
		return &this->asyncs[handle];

	throw RuntimeException(fmt::format("Invalid Async object {}", handle));
}

const ASync::AsyncObject *ASync::getObject(ASyncHandle handle) const {
	std::map<ASyncHandle, AsyncObject>::const_iterator it = this->asyncs.find(handle);
	if (it != this->asyncs.cend())
		return &it->second;

	throw RuntimeException(fmt::format("Invalid Async object {}", handle));
}

ASync::AsyncObject *ASync::createObject(ASyncHandle handle) { return &this->asyncs[handle]; }

void ASync::setScheduleReference(Ref<IScheduler> &sch) { this->scheduler = sch; }

ASync::~ASync(void) { this->getScheduler(); }

ASync::ASync(void) {
	this->scheduler = nullptr;
	// this->sch = nullptr;
}

ASync::ASync(Ref<IScheduler> &scheduler) {
	this->scheduler = scheduler;
	this->uidGenerator = UIDGenerator();
	/*  Take out the 0 UID that is invalid for the async handle
	 * for allowing checking if it is a valid handle.*/
	this->uidGenerator.getNextLUID();
}

ASync::ASync(ASync &&other) {
	// Move over the things!
}

ASync::ASync(const ASync &other) {
	this->scheduler = other.scheduler;
	this->uidGenerator = other.uidGenerator;
}
