#include"VDAudioGeometry.h"
#include"VDEngine.h"

VDAudioGeometry::VDAudioGeometry(void): VDBehavior(){
	audioGeometry = NULL;
}

VDAudioGeometry::VDAudioGeometry(const VDAudioGeometry& audioGeometry) : VDBehavior(){
	*this = audioGeometry;
}

VDAudioGeometry::VDAudioGeometry(const VDMesh* mesh) : VDBehavior(){
	this->audioGeometry = NULL;
}

void VDAudioGeometry::instanceInitilize(void){

}

void VDAudioGeometry::initializeComponent(void){
	this->initGeometry();

	if(this->audioGeometry){
		// transformation of the Audio geometry
		this->audioGeometry->setPosition((FMOD_VECTOR*)&this->transform()->getPosition());
		this->audioGeometry->setRotation(
			(const FMOD_VECTOR*)&this->transform()->rotation.forward(),
			(const FMOD_VECTOR*)&this->transform()->rotation.up());

		this->audioGeometry->setScale((FMOD_VECTOR*)&this->transform()->getScale());
	}
}

void VDAudioGeometry::onEnable(void){
	this->audioGeometry->setActive(true);
}

void VDAudioGeometry::onDisable(void){
	this->audioGeometry->setActive(false);
}

void VDAudioGeometry::onDestroy(void){
	this->audioGeometry->release();
}

VDBehavior* VDAudioGeometry::copyComponent(unsigned int& dataSize){
	VDAudioGeometry* audiogeometry = (VDAudioGeometry*)malloc(sizeof(VDAudioGeometry));
	memcpy(audiogeometry,this, sizeof(VDAudioGeometry));
	dataSize = sizeof(VDAudioGeometry);
	return (VDBehavior*)audiogeometry;
}

int VDAudioGeometry::initGeometry(void){
	int polyCount;

	if(this->renderer()){ // find a render component
		if(this->renderer()->getMesh()){
			if(this->audioGeometry){

				this->audioGeometry->getNumPolygons(&polyCount);
				if(this->renderer()->meshFilter->getPolygoneCount() == polyCount) // Geometry already defined!
					return SDL_TRUE;
			}

			/*	TODO something about.	*/
			//HandleMeshEx hMesh = this->renderer()->getMesh()->createMeshDescription();
			//ExFreeMeshHandler(&hMesh);
			//return initGeometry(hMesh);

		}
	}
	return SDL_FALSE;
}

int VDAudioGeometry::initGeometry(const VDMesh* mesh){
	//vailidate if current geometry is this geometry data
	//initlize and assigned data to the geometry
	VDCASTP(FMOD::System*,engine.audioContext)->createGeometry(
		mesh->getPolygoneCount(),
		mesh->getVertexCount(),
		&this->audioGeometry);
	this->audioGeometry->setUserData(this);


	for(unsigned int x = 0; x < mesh->getPolygoneCount(); x++){
		this->audioGeometry->setPolygonVertex(x,
											mesh->GetIndices(x),
											(FMOD_VECTOR*)((unsigned char*)&mesh->getVertex(x)));

	}
	return SDL_TRUE;
}

void VDAudioGeometry::setPosition(const VDVector3& position){
	if(this->audioGeometry)
		this->audioGeometry->setPosition(VDCASTP(const FMOD_VECTOR*,&position));
}

void VDAudioGeometry::setRotation(const VDQuaternion& rotation){
	if(this->audioGeometry)
		this->audioGeometry->setRotation(
		VDCASTP(const FMOD_VECTOR*,&rotation.forward()),
		VDCASTP(const FMOD_VECTOR*,&rotation.up()));
}
