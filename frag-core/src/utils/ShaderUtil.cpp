#include "Utils/ShaderUtil.h"
#include "Core/IO/FileSystem.h"
#include "Core/IO/IOUtil.h"
#include "Renderer/RenderPipeline.h"
#include <Renderer/RenderDesc.h>
#include <Utils/ShaderUtil.h>

#include <assert.h>
#include<fmt/core.h>

using namespace fragcore;

void ShaderUtil::loadProgramFromMem(const ShaderObject *vshader, const ShaderObject *fshader,
									const ShaderObject *gshader, const ShaderObject *tcshader,
									const ShaderObject *teshader, const ShaderObject *compute, Ref<IRenderer> &renderer,
									Shader **pshader) {

	//	validateShaderArguments(type, language, codetype);
	if (pshader == nullptr)
		throw InvalidArgumentException("");

	ShaderDesc desc = {0};
	Shader *shader = nullptr;
	int nsources;

	/*	Source code constants for language supporting preprocessors.*/
	static const char *vconst = "#define FV_VERTEX\n";
	static const char *fconst = "#define FV_FRAGMENT\n";
	static const char *gconst = "#define FV_GEOMETRY\n";
	static const char *tcconst = "#define FV_TCTESSELLATION\n";
	static const char *teconst = "#define FV_TETESSELLATION\n";
	static const char *comConst = "#define FV_COMPUTE\n";

	// 	/*  Process the shader description based on the shader language.    */
	// 	//TODO add glsl debug when builiding debug.
	// 	//TODO add extension list and macro for features and etc.
	// 	switch (language)
	// 	{
	// 	case GLSL:
	// 		/*  Get GLSL version.   */
	// #if defined(_DEBUG)
	// 		sprintf(glver, "#version %d\n//#pragma debug(on)\n", (int)(strtof(renderer->getShaderVersion(GLSL),
	// nullptr))); #elif defined(NDEBUG) 		sprintf(glver, "#version %d\n //#pragma optimize(on)\n  //#pragma
	// debug(off)\n", (int)(strtof(renderer->getShaderVersion(GLSL), nullptr))); #endif 		sources[0] = glver;
	// 		/*  */
	// 		sources[1] = invshad;
	// 		/*  */
	// 		sources[3] = source;
	// 		nsources = 4;
	// 		break;
	// 	case SPIRV:
	// 		sources[0] = source;
	// 		nsources = size;
	// 		nsources = 1;
	// 		break;
	// 	case CLC:
	// 	case HLSL:
	// 	case CG:
	// 	default:
	// 		assert(nullptr);
	// 	}

	char vglver[256];
	char fglver[256];
	char gglver[256];
	const char *vsources[4] = {nullptr};
	const char *fsources[4] = {nullptr};
	const char *gsources[4] = {nullptr};
	const char *csources[4] = {nullptr};
	if (vshader) {

		switch (vshader->language) {
		case GLSL:
			sprintf(vglver, "#version %d\n", (int)(strtof(renderer->getShaderVersion(GLSL), nullptr)));
			/*  Get GLSL version.   */
			vsources[0] = vglver;
			/*  */
			vsources[1] = vconst;
			/*  */
			vsources[2] = vshader->buf;
			nsources = 3;
			break;
		case SPIRV:
			vsources[0] = vshader->buf;
			nsources = 1;
			break;
		case CLC:
		case HLSL:
		case CG:
		default:
			assert(nullptr);
		}
		desc.vertex.language = vshader->language;
		desc.vertex.size = vshader->size;
		desc.vertex.type = vshader->type;
		desc.vertex.numvert = nsources;
		if (vshader->type == eSourceCode)
			desc.vertex.vertexsource = (const char **)&vsources;
		else {
			desc.vertex.vertexBinary = (const char *)vshader->buf;
			desc.vertex.binaryFormat = 0;
		}
	}

	if (fshader) {
		switch (fshader->language) {
		case GLSL:
			sprintf(fglver, "#version %d\n", (int)(strtof(renderer->getShaderVersion(GLSL), nullptr)));
			/*  Get GLSL version.   */
			fsources[0] = fglver;
			/*  */
			fsources[1] = fconst;
			/*  */
			fsources[2] = fshader->buf;
			nsources = 3;
			break;
		case SPIRV:
			fsources[0] = fshader->buf;
			nsources = 1;
			break;
		case CLC:
		case HLSL:
		case CG:
		default:
			assert(nullptr);
		}
		desc.fragment.language = fshader->language;
		desc.fragment.size = fshader->size;
		desc.fragment.type = fshader->type;
		desc.fragment.numfrag = nsources;
		if (fshader->type == eSourceCode)
			desc.fragment.fragmentsource = (const char **)&fsources;
		else {
			desc.fragment.fragmentBinary = (const char *)fshader->buf;
			desc.fragment.binaryFormat = 0;
		}
	}

	if (gshader) {
		switch (gshader->language) {
		case GLSL:
			sprintf(fglver, "#version %d\n", (int)(strtof(renderer->getShaderVersion(GLSL), nullptr)));
			/*  Get GLSL version.   */
			gsources[0] = fglver;
			/*  */
			gsources[1] = fconst;
			/*  */
			gsources[2] = gshader->buf;
			nsources = 3;
			break;
		case SPIRV:
			gsources[0] = gshader->buf;
			nsources = 1;
			break;
		case CLC:
		case HLSL:
		case CG:
		default:
			assert(nullptr);
		}
		desc.geometry.language = gshader->language;
		desc.geometry.size = gshader->size;
		desc.geometry.type = gshader->type;
		desc.geometry.numgeo = nsources;
		if (gshader->type == eSourceCode)
			desc.geometry.geometrysource = (const char **)&gsources;
		else {
			desc.geometry.geometryBinary = (const char *)gshader->buf;
			desc.geometry.binaryFormat = 0;
		}
	}

	if (compute) {
		switch (compute->language) {
		case GLSL:
			sprintf(gglver, "#version %d\n", (int)(strtof(renderer->getShaderVersion(GLSL), nullptr)));
			/*  Get GLSL version.   */
			csources[0] = gglver;
			/*  */
			csources[1] = gconst;
			/*  */
			csources[2] = fshader->buf;
			nsources = 3;
			break;
		case SPIRV:
			csources[0] = fshader->buf;
			nsources = 1;
			break;
		case CLC:
		case HLSL:
		case CG:
		default:
			assert(nullptr);
		}
		desc.Compute.language = compute->language;
		desc.Compute.size = compute->size;
		desc.Compute.type = compute->type;
		desc.Compute.numcompute = nsources;
		if (fshader->type == eSourceCode)
			desc.Compute.computeSource = (const char **)&csources;
		else {
			desc.Compute.computeBinary = (const char *)compute->buf;
			desc.Compute.binaryFormat = 0;
		}
	}

	// //TODO add support for combo of shaders
	// //TODO add support for code type.

	// switch (type)
	// {
	// case eVertex:
	// 	sources[2] = vconst;
	// 	desc.vertex.numvert = nsources;
	// 	desc.vertex.vertexsource = sources;
	// 	desc.vertex.type = (ShaderCodeType)codetype;
	// 	desc.vertex.language = language;
	// 	break;
	// case eFrag:
	// 	sources[2] = fconst;
	// 	desc.fragment.numfrag = nsources;
	// 	desc.fragment.fragmentsource = sources;
	// 	desc.fragment.type = (ShaderCodeType)codetype;
	// 	desc.fragment.language = language;
	// 	break;
	// case eGeom:
	// 	sources[2] = gconst;
	// 	desc.geometry.numgeo = nsources;
	// 	desc.geometry.geometrysource = sources;
	// 	desc.geometry.type = (ShaderCodeType)codetype;
	// 	desc.geometry.language = language;
	// 	break;
	// case eTesseC:
	// 	sources[2] = tcconst;
	// 	desc.tessellationControl.numtesco = nsources;
	// 	desc.tessellationControl.tessellationco = sources;
	// 	desc.tessellationControl.language = language;
	// 	desc.tessellationControl.type = (ShaderCodeType)codetype;
	// 	break;
	// case eTesseE:
	// 	sources[2] = teconst;
	// 	desc.tessellationEvolution.numtesev = nsources;
	// 	desc.tessellationEvolution.tessellationev = sources;
	// 	desc.tessellationEvolution.language = language;
	// 	desc.tessellationEvolution.type = (ShaderCodeType)codetype;
	// 	break;
	// case eCompute:
	// 	sources[2] = comConst;
	// 	desc.Compute.numcompute = nsources;
	// 	desc.Compute.computeSource = sources;
	// 	desc.Compute.type = (ShaderCodeType)codetype;
	// 	desc.Compute.language = language;
	// 	break;
	// default:
	// 	assert(nullptr);
	// 	break;
	// }

	/*	Create shader object.	*/
	shader = renderer->createShader(&desc);
	*pshader = shader;
}

void ShaderUtil::loadProgramPipeline(const ShaderObject *vshader, const ShaderObject *fshader,
									 const ShaderObject *gshader, const ShaderObject *tcshader,
									 const ShaderObject *teshader, IRenderer *renderer, RenderPipeline **pshader) {
	ProgramPipelineDesc pipelineDesc = {0};
	RenderPipeline *pipeline;

	// if (vshader) {
	// 	Shader *vertex;
	// 	loadShader(vshader->buf, vshader->size, eVertex, renderer, &vertex, vshader->language, vshader->type);
	// 	pipelineDesc.v = vertex;
	// }
	// if (fshader) {
	// 	Shader *fragment;
	// 	loadShader(fshader->buf, fshader->size, eFrag, renderer, &fragment, fshader->language, fshader->type);
	// 	pipelineDesc.f = fragment;
	// }
	// if (gshader) {
	// 	Shader *geometry;
	// 	loadShader(gshader->buf, gshader->size, eGeom, renderer, &geometry, gshader->language, gshader->type);
	// 	pipelineDesc.g = geometry;
	// }
	// if (tcshader) {
	// 	Shader *tessC;
	// 	loadShader(tcshader->buf, tcshader->size, eTesseC, renderer, &tessC, tcshader->language, tcshader->type);
	// 	pipelineDesc.tc = tessC;
	// }
	// if (teshader) {
	// 	Shader *tessE;
	// 	loadShader(teshader->buf, teshader->size, eTesseE, renderer, &tessE, teshader->language, teshader->type);
	// 	pipelineDesc.te = tessE;
	// }

	/*	Create pipeline.	*/
	pipeline = renderer->createPipeline(&pipelineDesc);

	/*	*/
	// defaultUniformMap(pipeline);

	/*  */
	*pshader = pipeline;
}

static void validateShaderArguments(ShaderType type, ShaderLanguage language, ShaderCodeType codetype) {
	// Validate the arguments.
	if (type < eVertex || type > eCompute)
		throw InvalidArgumentException(
			fmt::format("Invalid shader type - %d", type)); // TODO add enumerator to string for shader type.
	if (language & ~(GLSL | SPIRV | HLSL | CLC))
		throw InvalidArgumentException("None supported shader language by the application - {}",
												   language); // TODO add enumerator to string for shader language.
	if (codetype <= ShaderCodeType::eNoShaderType || codetype > ShaderCodeType::eBinary)
		throw InvalidArgumentException("None supported shader code format - {}",
												   codetype); // TODO add enumerator to string for shader language.
}

void ShaderUtil::loadShader(Ref<IO> &io, ShaderType type, Ref<IRenderer> &renderer, ShaderLanguage language,
							ShaderCodeType codetype, Shader **shader) {
	ShaderObjectDesc desc = {};
	desc.language = language;
	desc.source.push_back(io);
	desc.type = codetype;

	ShaderUtil::loadShader(&desc, type, renderer, language, codetype, shader);

	long int size = io->length();
	//	ShaderUtil::loadShader(nullptr, size, type, renderer, language, shader);
}

void ShaderUtil::loadShader(const char *source, const int size, ShaderType type, Ref<IRenderer> &renderer,
							ShaderLanguage language, ShaderCodeType codetype, Shader **pshader) {
	validateShaderArguments(type, language, codetype);
	if (pshader == nullptr)
		throw InvalidArgumentException("");

	ShaderDesc desc = {0};
	Shader *shader = nullptr;
	char *invshad = "";
	int nsources;
	char glver[256];
	const char *sources[4] = {nullptr};

	/*	Source code constants for language supporting preprocessors.*/
	static const char *vconst = "#define FV_VERTEX\n";
	static const char *fconst = "#define FV_FRAGMENT\n";
	static const char *gconst = "#define FV_GEOMETRY\n";
	static const char *tcconst = "#define FV_TCTESSELLATION\n";
	static const char *teconst = "#define FV_TETESSELLATION\n";
	static const char *comConst = "#define FV_COMPUTE\n";

	/*  Process the shader description based on the shader language.    */
	// TODO add glsl debug when builiding debug.
	// TODO add extension list and macro for features and etc.
	switch (language) {
	case GLSL:
		/*  Get GLSL version.   */
#if defined(_DEBUG)
		sprintf(glver, "#version %d\n//#pragma debug(on)\n", (int)(strtof(renderer->getShaderVersion(GLSL), nullptr)));
#elif defined(NDEBUG)
		sprintf(glver, "#version %d\n //#pragma optimize(on)\n  //#pragma debug(off)\n",
				(int)(strtof(renderer->getShaderVersion(GLSL), nullptr)));
#endif
		sources[0] = glver;
		/*  */
		sources[1] = invshad;
		/*  */
		sources[3] = source;
		nsources = 4;
		break;
	case SPIRV:
		sources[0] = source;
		nsources = size;
		nsources = 1;
		break;
	case CLC:
	case HLSL:
	case CG:
	default:
		assert(nullptr);
	}

	// TODO add support for combo of shaders
	// TODO add support for code type.

	switch (type) {
	case eVertex:
		sources[2] = vconst;
		desc.vertex.numvert = nsources;
		desc.vertex.vertexsource = sources;
		desc.vertex.type = (ShaderCodeType)codetype;
		desc.vertex.language = language;
		break;
	case eFrag:
		sources[2] = fconst;
		desc.fragment.numfrag = nsources;
		desc.fragment.fragmentsource = sources;
		desc.fragment.type = (ShaderCodeType)codetype;
		desc.fragment.language = language;
		break;
	case eGeom:
		sources[2] = gconst;
		desc.geometry.numgeo = nsources;
		desc.geometry.geometrysource = sources;
		desc.geometry.type = (ShaderCodeType)codetype;
		desc.geometry.language = language;
		break;
	case eTesseC:
		sources[2] = tcconst;
		desc.tessellationControl.numtesco = nsources;
		desc.tessellationControl.tessellationco = sources;
		desc.tessellationControl.language = language;
		desc.tessellationControl.type = (ShaderCodeType)codetype;
		break;
	case eTesseE:
		sources[2] = teconst;
		desc.tessellationEvolution.numtesev = nsources;
		desc.tessellationEvolution.tessellationev = sources;
		desc.tessellationEvolution.language = language;
		desc.tessellationEvolution.type = (ShaderCodeType)codetype;
		break;
	case eCompute:
		sources[2] = comConst;
		desc.Compute.numcompute = nsources;
		desc.Compute.computeSource = sources;
		desc.Compute.type = (ShaderCodeType)codetype;
		desc.Compute.language = language;
		break;
	default:
		assert(nullptr);
		break;
	}

	/*	Shader configurations.	*/
	desc.separatetable = true;

	/*	Create shader object.	*/
	shader = renderer->createShader(&desc);
	*pshader = shader;
}

void ShaderUtil::loadShader(const ShaderObjectDesc *desc, ShaderType type, Ref<IRenderer> &renderer,
							ShaderLanguage language, ShaderCodeType codetype, Shader **pshader) {

	// std::vector<Ref<IO>>::const_iterator eit = desc->source.cbegin();

	// for (; eit != desc->source.cned(); eit++){
	// 	(*eit)->length();
	// }
}

void ShaderUtil::loadProgram(IO *io, IRenderer *renderer, Shader **pShader, unsigned int format) {

	// char* pdata;
	// long int nBytes = IOUtil::loadFile(io, &pdata);
	// ShaderUtil::loadProgram(pdata, nBytes, renderer, pShader, format);
	// free(pdata);
}

void ShaderUtil::loadProgram(const void *pData, long int nBytes, IRenderer *renderer, Shader **pShader,
							 unsigned int format) {

	ShaderDesc desc = {0};
	Shader *shader;

	/*  */
	desc.program.format = format;
	desc.program.pdata = pData;
	desc.program.binarySize = nBytes;

	/*  */
	shader = renderer->createShader(&desc);
	*pShader = shader;
}

void ShaderUtil::loadProgram(const void *vertex, const int vsize, const void *fragment, const int fsize,
							 const void *geometry, const int gsize, const void *tesselationc, const int tcsize,
							 const void *tesselatione, const int tesize, ShaderCodeType codetype,
							 ShaderLanguage language, Ref<IRenderer> &renderer, Shader **pshader) {
	validateShaderArguments(eVertex, language, codetype);
	if (pshader == nullptr)
		throw InvalidArgumentException("");

	ShaderObject v, f, g, tc, te;
	ShaderObject *pv, *pf, *pg, *ptc, *pte;
	pv = pf = pg = ptc = pte = nullptr;
	if (vertex) {
		v.language = language;
		v.type = codetype;
		v.buf = (const char *)vertex;
		v.size = vsize;
		pv = &v;
	}
	if (fragment) {
		f.language = language;
		f.type = codetype;
		f.buf = (const char *)fragment;
		f.size = fsize;
		pf = &f;
	}
	if (geometry) {
		g.language = language;
		g.type = codetype;
		g.buf = (const char *)geometry;
		g.size = gsize;
		pg = &g;
	}
	if (tesselationc) {
		v.language = language;
		v.type = codetype;
		v.buf = (const char *)tesselationc;
		v.size = tcsize;
		ptc = &tc;
	}
	if (tesselatione) {
		te.language = language;
		te.type = codetype;
		te.buf = (const char *)tesselatione;
		te.size = tesize;
		pte = &te;
	}

	loadProgramFromMem(pv, pf, pg, ptc, pte, nullptr, renderer, pshader);
}

void ShaderUtil::loadProgram(const ShaderObjectDesc *vertex, const ShaderObjectDesc *fragment,
							 const ShaderObjectDesc *geometry, const ShaderObjectDesc *tesseC,
							 const ShaderObjectDesc *tesseT, const ShaderObjectDesc *compute, Ref<IRenderer> &renderer,
							 Shader **shader) {}

void ShaderUtil::loadComputeProgram(Ref<IO> &computeIO, Ref<IRenderer> &renderer, ShaderLanguage language,
									ShaderCodeType codetype, Shader **program) {

	ShaderObjectDesc desc;
	desc.language = language;
	desc.source.push_back(computeIO);
	desc.type = codetype;
	ShaderUtil::loadProgram(nullptr, nullptr, nullptr, nullptr, nullptr, &desc, renderer, program);
}

void ShaderUtil::loadComputeProgram(const char *pData, long int nBytes, ShaderLanguage language,
									ShaderCodeType codetype, Ref<IRenderer> &renderer, Shader **program) {
	ShaderObject compute;
	compute.language = language;
	compute.size = nBytes;
	compute.type = codetype;
	compute.buf = pData;
	ShaderUtil::loadProgramFromMem(nullptr, nullptr, nullptr, nullptr, nullptr, &compute, renderer, program);
}

void ShaderUtil::loadComputeShaderSource(ShaderObject *shaderDesc, IRenderer *renderer,
										 RenderPipeline **programPipeline) {
	Shader *shader;
	ProgramPipelineDesc progDes = {0};

	/*  */
	// loadShader(shaderDesc->buf, shaderDesc->size, eCompute, renderer, shaderDesc->language, shaderDesc->type, &
	// shader);

	/*  */
	progDes.c = shader;
	*programPipeline = renderer->createPipeline(&progDes);
}

ShaderCodeType ShaderUtil::getCodeType(const char *filePath) {
	const char *basename = FileSystem::getBaseName(filePath);

	return ShaderCodeType::eSourceCode;
}

ShaderLanguage ShaderUtil::getFileLanguage(const char *filePath) {
	const char *buf;

	/*	Extract file extension.	*/
	const char *basename = FileSystem::getBaseName(filePath);
	buf = FileSystem::getFileExtension(basename);

	/*  Iterate through each file.*/
	if (strcmp(buf, "vert") == 0)
		return GLSL;
	if (strcmp(buf, "frag") == 0)
		return GLSL;
	if (strcmp(buf, "geo") == 0)
		return GLSL;
	if (strcmp(buf, "tesc") == 0)
		return GLSL;
	if (strcmp(buf, "tese") == 0)
		return GLSL;
	if (strcmp(buf, "comp") == 0)
		return GLSL;
	if (strcmp(buf, "glsl") == 0)
		return GLSL;
	if (strcmp(buf, "sprv") == 0)
		return SPIRV;
	if (strcmp(buf, "cl") == 0)
		return CLC;

	return unKnownLanguage;
}

ShaderType ShaderUtil::getShaderType(const char *filePath) {
	const char *basename = FileSystem::getBaseName(filePath);

	return eFrag;
}

std::vector<ShaderUtil::UniformLocation> ShaderUtil::getShaderUniformAttributes(const Ref<Shader> &shader) {}

void ShaderUtil::copyUniform(Ref<Shader> &shader) {
	// NNshader->getUniform
}
void ShaderUtil::pastUniform(Ref<Shader> &shader) {}