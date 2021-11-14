#include "Core/IO/ASyncIO.h"
#include "Core/IO/IFileSystem.h"
//#include "Exception/InvalidArgumentException.h"
//#include "Exception/RuntimeException.h"

#include <condition_variable>
#include <fmt/core.h>
#include <mutex>
#include <taskSch.h> //TOOD remove once semaphore class has been implemented.
#include <thread>
using namespace fragcore;

class FVDECLSPEC AsyncTask : public Task {
  public:
	virtual void Execute() noexcept override {}
	virtual void Complete() noexcept override {}
};

ASyncHandle ASyncIO::asyncOpen(Ref<IO> &io) {

	/*	Check if scheduler is initialized.	*/
	if (*scheduler == nullptr)
		throw RuntimeException("Async not initialized with a scheduler object");
	/*  Check parameters.   */
	if (!*io)
		throw InvalidArgumentException("Invalid IO reference.");

	const IO::IOOperation requiredIOSupported = static_cast<IO::IOOperation>(IO::OP_READ | IO::OP_WRITE);

	/*	Check if IO operations are supported.	*/
	if (!io->isOperationSupported(requiredIOSupported))
		throw InvalidArgumentException("IO: {} requires read/write operation support", io->getName());

	/*  Increment reference.    */
	// io->increment();

	/*  Create async object.*/
	ASyncHandle handle = this->uidGenerator.getNextUID();
	AsyncObject *asyncObject = createObject(handle);
	/*	*/
	asyncObject->ref = io;
	asyncObject->semaphore = nullptr;
	asyncObject->buffer = nullptr;
	asyncObject->size = 0;
	asyncObject->userData = nullptr;
	asyncObject->callback = nullptr;

	return handle;
}

void ASyncIO::asyncReadFile(ASyncHandle handle, char *buffer, unsigned int size, AsyncComplete complete) {

	/*  Get object and check if exists. */
	AsyncObject *ao = getObject(handle);

	/*	Verify that is has read access.	*/
	if (!ao->ref->isReadable())
		throw RuntimeException(fmt::format("IO object is not readable {}", ao->ref->getUID()));

	// TODO replace with an abstract version
	/*  Assign variables.   */
	int error;
	error = schCreateSemaphore((schSemaphore **)&ao->semaphore);
	if (error != SCH_OK)
		throw RuntimeException(fmt::format("Failed to create semaphore {}", schErrorMsg(error)));

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

void ASyncIO::asyncReadFile(ASyncHandle handle, Ref<IO> &writeIO, AsyncComplete complete) {
	AsyncObject *ao = getObject(handle);

	/*  Check that open file is readable.   */
	if (!ao->ref->isReadable())
		throw RuntimeException(fmt::format("IO object is not writable {}", ao->ref->getUID()));
	/*  Check if IO object is writeable.    */
	if (!writeIO->isWriteable())
		throw RuntimeException(fmt::format("IO object is not writable {}", writeIO->getUID()));

	ao->callback = complete;
}

void ASyncIO::asyncWriteFile(ASyncHandle handle, char *buffer, unsigned int size, AsyncComplete complete) {
	int error;
	AsyncObject *ao = getObject(handle);
	// TODO perhaps can to use the IOBuffer has a interface object for reduced coupling and higher cohesion.
	if (!ao->ref->isWriteable())
		throw RuntimeException(fmt::format("IO object is not writable {}", ao->ref->getUID()));

	/*  Assign variables.   */
	error = schCreateSemaphore((schSemaphore **)&ao->semaphore);
	if (error != SCH_OK)
		throw RuntimeException(fmt::format("Failed to create semaphore {}", schErrorMsg(error)));

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

void ASyncIO::asyncWriteFile(ASyncHandle handle, Ref<IO> &io, AsyncComplete complete) { /*	*/
}

Ref<IO> ASyncIO::getIO(ASyncHandle handle) const { return getObject(handle)->ref; }

const ASyncIO::IOStatus &ASyncIO::getIOStatus(ASyncHandle handle) const { return this->getObject(handle)->status; }

Ref<IScheduler> ASyncIO::getScheduler() const { return this->scheduler; }

void ASyncIO::asyncWait(ASyncHandle handle) { asyncWait(handle, -1); }

bool ASyncIO::asyncWait(ASyncHandle handle, long int timeout) {

	AsyncObject *ao = getObject(handle);

	// TODO add wait mechanic.
	//ao->sem->wait();

	// schSemaphoreWait((schSemaphore *)ao->semaphore);

	// schCreateSemaphore(ao->semaphore);
	//	schSemaphorePost((schSemaphore *) ao->semaphore);
	/*  Set as finished.    */
	// ao->status.

	return true; // schSemaphoreTimedWait((schSemaphore *)ao->semaphore, timeout) == SCH_OK;
}

void ASyncIO::asyncClose(ASyncHandle handle) {
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

void ASyncIO::async_open(Task *task) {

	// ASyncHandle handle = (ASyncHandle)task->userData;
	AsyncObject *ao = (AsyncObject *)task->userData;

	// const char *path = (const char *)ao->begin;
	// ASync *async = (ASync *) package->puser;
	// IFileSystem *fileSystem;

	// Ref<IO> refIO = Ref<IO>(fileSystem->openFile(path, IO::READ));

	/*  Create the task in succession.  */
}

void ASyncIO::async_read(Task *task) {
	AsyncObject *ao = (AsyncObject *)task->userData;
	const size_t block_size = 512;

	Ref<IO> &io = ao->ref;
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

void ASyncIO::async_read_io(Task *task) {
	AsyncObject *ao = (AsyncObject *)task->userData;
	const size_t block_size = 512;
	// TODO update for the IO based..
	Ref<IO> &io = ao->ref;
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

void ASyncIO::async_write(Task *task) {
	AsyncObject *ao = (AsyncObject *)task->userData;
	const size_t block_size = 512;

	Ref<IO> &io = ao->ref;
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

void ASyncIO::async_write_io(Task *task) {
	task->Execute();

	return;
}

ASyncIO::AsyncObject *ASyncIO::getObject(ASyncHandle handle) {

	/*	Check if handle exits.	*/
	if (this->asyncs.find(handle) != this->asyncs.end())
		return &this->asyncs[handle];

	throw RuntimeException(fmt::format("Invalid Async object {}", handle));
}

const ASyncIO::AsyncObject *ASyncIO::getObject(ASyncHandle handle) const {
	std::map<ASyncHandle, AsyncObject>::const_iterator it = this->asyncs.find(handle);
	if (it != this->asyncs.cend())
		return &it->second;

	throw RuntimeException(fmt::format("Invalid Async object {}", handle));
}

ASyncIO::AsyncObject *ASyncIO::createObject(ASyncHandle handle) { return &this->asyncs[handle]; }

void ASyncIO::setScheduleReference(Ref<IScheduler> &sch) { this->scheduler = sch; }

ASyncIO::~ASyncIO() { this->getScheduler(); }

ASyncIO::ASyncIO() {
	this->scheduler = nullptr;
}

ASyncIO::ASyncIO(Ref<IScheduler> &scheduler) {
	this->scheduler = scheduler;
	this->uidGenerator = UIDGenerator<size_t>();
	/*  Take out the 0 UID that is invalid for the async handle
	 * for allowing checking if it is a valid handle.*/
	this->uidGenerator.getNextUID();
}

ASyncIO::ASyncIO(ASyncIO &&other) {
	// Move over the things!
	this->scheduler = std::exchange(other.scheduler, nullptr);
	this->uidGenerator = other.uidGenerator;
}

ASyncIO::ASyncIO(const ASyncIO &other) {
	this->scheduler = other.scheduler;
	this->uidGenerator = other.uidGenerator;
}
