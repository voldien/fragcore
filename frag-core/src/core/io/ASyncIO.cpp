#include "IO/ASyncIO.h"
#include "IO/BufferIO.h"
#include "Threading/StdSemaphore.h"
#include <fmt/core.h>
using namespace fragcore;

ASyncHandle ASyncIO::asyncOpen(Ref<IO> &ioRef) {

	/*	Check if scheduler is initialized.	*/
	if (this->scheduler == nullptr) {
		throw RuntimeException("Async not initialized with a scheduler object");
	}
	/*  Check parameters.   */
	if (ioRef == nullptr) {
		throw InvalidArgumentException("Invalid IO reference.");
	}

	/*	Required IO operation to be supported.	*/
	const IO::IOOperation requiredIOReadSupported = static_cast<IO::IOOperation>(IO::OP_READ | IO::OP_WRITE);

	/*	Check if IO operations are supported.	*/
	if (!ioRef->isOperationSupported(requiredIOReadSupported)) {
		throw InvalidArgumentException("IO: {} requires read/write operation support", ioRef->getName());
	}

	/*	Generate new handle*/
	ASyncHandle handle = this->generateHandle();

	/*  Create async object.*/
	AsyncObject *asyncObject = this->createObject(handle);
	/*	*/
	asyncObject->ref = ioRef;
	asyncObject->sem = nullptr;
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
	if (!ao->ref->isReadable()) {
		throw RuntimeException("IO object is not readable {}", ao->ref->getUID());
	}

	/*	*/
	if (buffer == nullptr || size == 0) {
		throw InvalidArgumentException("");
	}

	/*  Assign variables.   */
	ao->sem = new stdSemaphore();

	/*	*/
	ao->buffer = buffer;
	ao->size = size;
	ao->callback = complete;
	ao->userData = nullptr;

	/*  Reset status counter.   */
	ao->status.nbytes = 0;
	ao->status.offset = 0;

	AsyncTask readTask(*ao);
	readTask.callback = async_read;
	readTask.userData = ao;

	this->scheduler->addTask(&readTask);
}

void ASyncIO::asyncReadFile(ASyncHandle handle, Ref<IO> &writeIO, AsyncComplete complete) {
	AsyncObject *async_o = getObject(handle);

	/*  Check that open file is readable.   */
	if (!async_o->ref->isReadable()) {
		throw RuntimeException("IO object is not writable {}", async_o->ref->getUID());
	}
	/*  Check if IO object is writeable.    */
	if (!writeIO->isWriteable()) {
		throw RuntimeException("IO object is not writable {}", writeIO->getUID());
	}

	async_o->callback = complete;
}

void ASyncIO::asyncWriteFile(ASyncHandle handle, char *buffer, unsigned int size, AsyncComplete complete) {
	AsyncObject *ao = getObject(handle);

	assert(ao);

	if (buffer == nullptr || complete == nullptr || size <= 0) {
		throw InvalidPointerException("");
	}

	BufferIO bufferIO(buffer, size);

	if (!ao->ref->isWriteable()) {
		throw RuntimeException("IO object is not writable {}", ao->ref->getUID());
	}

	// TODO perhaps can to use the IOBuffer has a interface object for reduced coupling and higher cohesion.

	/*  Assign variables.   */
	ao->sem = new stdSemaphore();

	ao->buffer = buffer;
	ao->size = size;
	ao->callback = complete;
	ao->userData = nullptr;

	/*  Reset status counter.   */
	ao->status.nbytes = 0;
	ao->status.offset = 0;

	AsyncTask readTask(*ao);
	readTask.callback = async_write;
	readTask.userData = ao;
	this->scheduler->addTask(&readTask);
}

void ASyncIO::asyncWriteFile(ASyncHandle handle, Ref<IO> &io, AsyncComplete complete) { /*	*/

	AsyncObject *async_o = getObject(handle);

	assert(async_o);

	if (!async_o->ref->isWriteable()) {
		throw RuntimeException("IO object is not writable {}", async_o->ref->getUID());
	}

	/*  Assign variables.   */
	async_o->sem = new stdSemaphore();

	async_o->target = io;
	async_o->size = io->length();
	async_o->callback = complete;
	async_o->userData = nullptr;

	/*  Reset status counter.   */
	async_o->status.nbytes = 0;
	async_o->status.offset = 0;

	AsyncTask readTask(*async_o);
	readTask.callback = async_write;
	readTask.userData = async_o;
	this->scheduler->addTask(&readTask);
}

Ref<IO> ASyncIO::getIO(ASyncHandle handle) const { return getObject(handle)->ref; }

const ASyncIO::IOStatus &ASyncIO::getIOStatus(ASyncHandle handle) const { return this->getObject(handle)->status; }

Ref<IScheduler> ASyncIO::getScheduler() const { return this->scheduler; }

void ASyncIO::asyncWait(ASyncHandle handle) { asyncWait(handle, -1); }

bool ASyncIO::asyncWait(ASyncHandle handle, long int timeout) {

	AsyncObject *async_o = getObject(handle);

	assert(async_o);

	if (async_o->sem) {
		/*	Wait	*/
		async_o->sem->wait(timeout);
	}

	// TODO return status.
	return true;
}

void ASyncIO::asyncClose(ASyncHandle handle) {
	AsyncObject *ao = this->getObject(handle);
	assert(ao);

	this->asyncWait(handle);

	/*  If not used by scheduler anymore, delete all references.    */
	if (ao->ref->deincreemnt()) {
		ao->ref->close();
	}

	/*	Release sync objects.	*/
	delete ao->sem;

	/*	*/
	auto it = this->asyncs.find(handle);
	this->asyncs.erase(it);
}

void ASyncIO::async_open(Task *task) {

	// ASyncHandle handle = (ASyncHandle)task->userData;
	// AsyncObject *ao = static_cast<AsyncObject *>(task->userData);

	// const char *path = (const char *)ao->begin;
	// ASync *async = (ASync *) package->puser;
	// IFileSystem *fileSystem;

	// Ref<IO> refIO = Ref<IO>(fileSystem->openFile(path, IO::READ));

	/*  Create the task in succession.  */
}

void ASyncIO::async_read(Task *task) {
	AsyncTask *asynctask = dynamic_cast<AsyncTask *>(task);
	AsyncObject *async_obj = &asynctask->asyncObject;
	const size_t block_size = 512;

	async_obj->sem->lock();

	Ref<IO> &io = async_obj->ref;
	async_obj->status.offset = io->getPos();
	while (async_obj->status.nbytes < async_obj->size) {
		/*	*/
		long int nread = io->read(block_size, &async_obj->buffer[async_obj->status.nbytes]);
		if (nread <= 0) {
			break;
		}

		async_obj->status.nbytes += nread;
		async_obj->status.offset += nread;
	}

	/*  Finished.   */
	if (async_obj->callback) {
		async_obj->callback(nullptr, 0);
	}
	/*  Finish and posted for the data is available.  */
	async_obj->sem->unlock();
}

void ASyncIO::async_read_io(Task *task) {
	AsyncObject *ao = static_cast<AsyncObject *>(task->userData);
	const size_t block_size = 512;

	ao->sem->lock();

	Ref<IO> &io = ao->ref;
	ao->status.offset = io->getPos();
	while (ao->status.nbytes < ao->size) {
		long int nread = io->read(block_size, &ao->buffer[ao->status.nbytes]);
		if (nread <= 0) {
			break;
		}

		ao->status.nbytes += nread;
		ao->status.offset += nread;
	}

	/*  Finished.   */
	if (ao->callback) {
		ao->callback(nullptr, 0);
	}
	/*  Finish and posted for the data is available.  */
	ao->sem->unlock();
}

void ASyncIO::async_write(Task *task) {

	AsyncObject *ao = static_cast<AsyncObject *>(task->userData);
	const size_t block_size = 512;
	ao->sem->lock();

	Ref<IO> &io = ao->ref;
	ao->status.offset = io->getPos();
	/*	*/
	while (ao->status.nbytes < ao->size) {
		long int nwritten = io->write(block_size, &ao->buffer[ao->status.nbytes]);
		if (nwritten <= 0) {
			break;
		}
		ao->status.nbytes += nwritten;
		ao->status.offset += nwritten;
	}

	io->flush();
	if (ao->callback) {
		ao->callback(nullptr, 0);
	}
	/*  Finish and posted for the data is available.  */
	ao->sem->unlock();
}

void ASyncIO::async_write_io(Task *task) {
	AsyncObject *ao = static_cast<AsyncObject *>(task->userData);

	task->Execute();

	ao->sem->lock();

	ao->sem->unlock();
}

ASyncHandle ASyncIO::generateHandle() { return this->uidGenerator.getNextUID(); }

ASyncIO::AsyncObject *ASyncIO::getObject(ASyncHandle handle) {

	/*	Check if handle exits.	*/
	std::map<ASyncHandle, AsyncObject>::iterator it = this->asyncs.find(handle);

	if (it != this->asyncs.end()) {
		return &this->asyncs[handle];
	}

	throw RuntimeException("Invalid Async object {}", handle);

	return nullptr;
}

const ASyncIO::AsyncObject *ASyncIO::getObject(ASyncHandle handle) const {

	/*	Check if the object exits.	*/
	std::map<ASyncHandle, AsyncObject>::const_iterator it = this->asyncs.find(handle);
	if (it != this->asyncs.cend()) {
		return &it->second;
	}

	throw RuntimeException("Invalid Async object {}", handle);

	return nullptr;
}

ASyncIO::AsyncObject *ASyncIO::createObject(ASyncHandle handle) { return &this->asyncs[handle]; }

void ASyncIO::setScheduleReference(const Ref<IScheduler> &sch) { this->scheduler = sch; }

ASyncIO::~ASyncIO() {
	// TODO add support for close all the handle.
}

ASyncIO::ASyncIO() { this->scheduler = nullptr; }

ASyncIO::ASyncIO(const Ref<IScheduler> &scheduler) {
	this->setScheduleReference(scheduler);
	this->uidGenerator = UIDGenerator<size_t>();
	/*  Take out the 0 UID that is invalid for the async handle
	 * for allowing checking if it is a valid handle.*/
	this->uidGenerator.getNextUID();
}

ASyncIO::ASyncIO(ASyncIO &&other) : uidGenerator(other.uidGenerator) {
	// Move over the things!
	this->scheduler = std::exchange(other.scheduler, nullptr);
}

ASyncIO::ASyncIO(const ASyncIO &other) : uidGenerator(other.uidGenerator), scheduler(other.scheduler) {}
