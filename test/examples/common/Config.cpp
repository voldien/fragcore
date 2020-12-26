#include"Config.h"
#include<Utils/StringUtil.h>
#include<getopt.h>
#include<stdexcept>
#include<Renderer/RendererFactory.h>
#include"Core/IO/FileSystem.h"
#include"Core/Log.h"
#include<cstdlib>
#include<csignal>
#include <Exception/InvalidArgumentException.h>

using namespace fragcore;

/*	TODO reorder    */
static const char *shortarg = "vVdqh" "wSp:r:P:dD:Isar:g:C:f:A:t:vF:cRH:E:nNC:G:p:UbP:~_";
static struct option longoptions[] = {
		/*  First pass arguments.   */
		{"version",                no_argument,       NULL, 'v'},    /*	Print version of application.	*/
		{"verbose",                no_argument,       NULL, 'V'},    /*	Print.	*/
		{"debug",                  no_argument,       NULL, 'd'},    /*	Debug.	*/
		{"quite",                  no_argument,       NULL, 'q'},    /*	Quite .	*/
		{"help",                   no_argument,       NULL, 'h'},    /*	Help.	*/
		{"config",                 required_argument, NULL, 'D'},    /*	Override the configuration.	*/

		/*  Screen override options.    */
		{"fullscreen",             no_argument,       NULL, ' '},   /*  	*/
		{"screen-width",           required_argument, NULL, '_'},   /*	    */
		{"screen-height",          required_argument, NULL, '_'},   /*	    */
		{"screen-x",               required_argument, NULL, '_'},   /*	    */
		{"screen-y",               required_argument, NULL, '_'},   /*	    */
		{"no-decoration",          no_argument,       NULL, '~'},   /*	Use no window decoration.	*/
		{"wallpaper",              optional_argument, NULL, 'w'},   /*	use as wallpaper.	*/

		/*	Long options for rendering interface.   */
		{"renderer-opengl",        no_argument,       NULL, '~'},    /*	Force rendering API OpenGL. */
		{"renderer-opencl",        no_argument,       NULL, '~'},    /*	Force rendering API OpenCL. */
		{"renderer-vulkan",        no_argument,       NULL, '~'},    /*	Force rendering API Vulkan. */
		{"renderer-directx",       no_argument,       NULL, '~'},    /*	Force rendering API DirectX. */
		/*  Long options with arguments.    */
		{"renderer",               required_argument, NULL, 'r'},    /*	Set renderer by rendering path.   */

		/*	Rendering Quality.  */
		{"quality",                required_argument, NULL, '_'},    /*	*/
		{"resolution-scale",       required_argument, NULL, 'R'},   /*	Texture scale resolution (required gl_framebuffer_object for OpenGL).*/

		/*  Rendering Settings. */
		{"anti-aliasing",          required_argument, NULL, '_'},    /*	Set antialiasing.   */
		{"srgb",                   no_argument,       NULL, 'S'},    /*	sRGB, gamma correction.	*/
		{"alpha",                  no_argument,       NULL, 'a'},    /*	use alpha channel.	*/
		{"v-sync",                 no_argument,       NULL, '~'},    /*	Enable vsync.   */

		/*  FragView behaviour options.  */
		{"disable-notify-file",  no_argument,       NULL, 'N'},   /*  Disable inotify notification.	*/
		{"cache-shader",         optional_argument, NULL, '_'},   /*  Enable cache shader.	*/
		{"use-cache-shaders",    no_argument,       NULL, '_'},   /*  Enable cache shader.	*/
		{"no-use-cache-shaders", no_argument,       NULL, '_'},   /*  Enable cache shader.	*/

		{"scene",                  required_argument, NULL, 'B'},   /*		*/

		/*  */
		{"poly",                   required_argument, NULL, 'p'},   /*	Polygon - .	*/
		{"param",                  required_argument, NULL, 'P'},   /*  Parameters. */
		{"stdin",                  optional_argument, NULL, 'I'},   /*	stdin data as buffer.	*/
		{"stream",                 required_argument, NULL, 's'},   /*  Stream file. */
		{"stream-pipe",            no_argument,       NULL, 'O'},   /*  Stream, enabled via stream object. */

		{"sample",                 required_argument, NULL, '_'},    /*	*/
		/*  */
		{"sandbox",                optional_argument, NULL, 'b'},   /*  Enable the sandbox. */

		/*  Shader types.   */
		{"file",                   required_argument, NULL, 'f'},   /*	Default shader file.	*///TODO resolve if to be remove.
		{"fragment",               required_argument, NULL, 'F'},   /*	Fragment shader source.	*/
		{"geometry",               required_argument, NULL, 'G'},   /*	Geometry shader source.	*/
		{"compute",                required_argument, NULL, 'H'},   /*	Compute shader source.	*/
		{"tessellation-control",   required_argument, NULL, 'C'},   /*	Tessellation C shader source.	*/
		{"tessellation-evolution", required_argument, NULL, 'E'},   /*	Tessellation E shader source.	*/
		{"binary-program",         required_argument, NULL, 'B'},   /*		*/

		/*  Texture arguments.  16 texture unit support by default. */
		{"texture0",               required_argument, NULL, ' '},   /*	Texture on index 0. */
		{"texture1",               required_argument, NULL, ' '},   /*	*/
		{"texture2",               required_argument, NULL, ' '},   /*	*/
		{"texture3",               required_argument, NULL, ' '},   /*	*/
		{"texture4",               required_argument, NULL, ' '},   /*	*/
		{"texture5",               required_argument, NULL, ' '},   /*	*/
		{"texture6",               required_argument, NULL, ' '},   /*	*/
		{"texture7",               required_argument, NULL, ' '},   /*	*/
		{"texture8",               required_argument, NULL, ' '},   /*	*/
		{"texture9",               required_argument, NULL, ' '},   /*	*/
		{"texture10",              required_argument, NULL, ' '},   /*	*/
		{"texture11",              required_argument, NULL, ' '},   /*	*/
		{"texture12",              required_argument, NULL, ' '},   /*	*/
		{"texture13",              required_argument, NULL, ' '},   /*	*/
		{"texture14",              required_argument, NULL, ' '},   /*	*/
		{"texture15",              required_argument, NULL, ' '},   /*	*/
		{"texture",                required_argument, NULL, 't'},   /*	Texture, next texture unit. */
		{"compression",            optional_argument, NULL, 'c'},   /*	Texture compression.	*/

		/*  TODO: Future version using DSL for materials.   */
		{"inline-script",          required_argument, NULL, 'O'},   /*  Inline script. */
		{"script",                 required_argument, NULL, 'O'},   /*  Script file. */

		/*  */
		{"save-config",            optional_argument, NULL, 'U'},   /*  Enable saving of configuration file to either default or specific filepath. */
		{NULL, 0,                                     NULL, 0},
};

Config::Config(void) : fragcore::IConfig() {}

Config::Config(const Config &other) {
	//*this = other;
}

Config::~Config(void) {

}

fragcore::IConfig *Config::getSuperInstance(void) {
	return new Config();
}

void Config::setDefaultOption(void) {

	IConfig &global = *this;
#if defined(_DEBUG)
	global.set("debug", true);  // Enable global debug for the application.
#else
	global.set("debug", false);
#endif

	/*  Sandbox fragview.   */
	global.set("sandbox", false);
	/*  Behavior functions.   */
	global.set("notify-file", true);
	global.set("drag-and-drop", true);
	/*  Internal.   */
	global.set("cache-shaders", true);
	global.set("use-cache-shaders", true);

	/*	Default interfaces configuration.	*/
	global.set("renderer-dynamicInterface",
	           fragcore::RenderingFactory::getInterfaceLibraryPath(fragcore::RenderingFactory::OpenGL));
	global.set<int>("SIMD", Hpm::eHPM_DEFAULT);

	/*TODO determine if to relocate.    */
	/*	Engine default configuration.	*/
	IConfig &renderingConfig = this->getSubConfig("render-driver");
	renderingConfig.set("extensions", "");
	renderingConfig.set("layers", "");
	// OpenGL configuration.
	renderingConfig.set("opengl", -1);
	renderingConfig.set("core", 1);
	// Vulkan configuration.
	renderingConfig.set("vulkan-version", 110);
	// DirectX configuration.
	renderingConfig.set("direct-x-version", 0);
	//
	renderingConfig.set("alpha", false);
	renderingConfig.set("v-sync", true);
	renderingConfig.set("gamma-correction", true);
	renderingConfig.set("debug", false);
	renderingConfig.set("debug-tracer", true);
	renderingConfig.set("anti-aliasing-samples", 0);
	renderingConfig.set("anti-aliasing", false);
	renderingConfig.set("version", "auto"); //TODO check if a better name is better.

	/*  Global main window settings.    */
	IConfig &renderWindowSetting = this->getSubConfig("render-window-settings");
	renderWindowSetting.set("fullscreen", 0);
	renderWindowSetting.set("isFullscreen", 0);
	renderWindowSetting.set("screen-width", -1);
	renderWindowSetting.set("screen-height", -1);
	renderWindowSetting.set("screen-max-width", -1);
	renderWindowSetting.set("screen-max-height", -1);
	renderWindowSetting.set("screen-min-width", -1);
	renderWindowSetting.set("screen-min-height", -1);
	renderWindowSetting.set("screen-x", -1);
	renderWindowSetting.set("screen-y", -1);
	renderWindowSetting.set("background-rendering", false);
	renderWindowSetting.set("window-resizable", true);
	renderWindowSetting.set("window-bordered", true);

	/*  */
	IConfig &render3DSettingConfig = this->getSubConfig("render-graphic-settings");
	/*  Anti aliasing.  */
	render3DSettingConfig.set("anti-aliasing-samples", 0);
	render3DSettingConfig.set("anti-aliasing", false);
	render3DSettingConfig.set("anti-aliasing-technique", 0);
	/*  */
	render3DSettingConfig.set("texture-quality", 4);
	render3DSettingConfig.set("resolution-scale", 1.0f);
	/*  Shadow. */
	render3DSettingConfig.set("shadow", true);
	render3DSettingConfig.set("shadow-distance", 100.0f);
	render3DSettingConfig.set("shadow-resolution", 2048);
	render3DSettingConfig.set("shadow-cascaded", 4);
	//TODO change to percentage.
	render3DSettingConfig.set("shadow-cascaded-0", 20.0f);
	render3DSettingConfig.set("shadow-cascaded-1", 100.0f);
	render3DSettingConfig.set("shadow-cascaded-2", 250.0f);
	render3DSettingConfig.set("shadow-cascaded-3", 800.0f);
	/*  Light.  */

	/*  */

	IConfig &sceneConfig = this->getSubConfig("default-scene-settings");
	/*  Camera default. */
	IConfig& cameraConfig = sceneConfig.getSubConfig("main-camera");
	cameraConfig.set("camera-fov", 70.0f);
	cameraConfig.set("camera-near", 0.15f);
	cameraConfig.set("camera-far", 1000.0f);
	cameraConfig.set("camera-aspect", -1.0f);
	cameraConfig.set("enable-free-camera", false);
	cameraConfig.set("camera-", 0);

	/*  Scene properties.   */
	sceneConfig.set("free-rotation", true);
	sceneConfig.set("free-rotation-speed", 3.14f / 10.0f);
	sceneConfig.set("default-focus", true);

	/*  */
	IConfig &resourceConfig = this->getSubConfig("resource-settings");
	/*	Resources default configuration.	*/
	resourceConfig.set("shaddir", "/usr/share/fragview");
	resourceConfig.set("settings", "");
	resourceConfig.set("dump-configuration", "fragview.xml");
	resourceConfig.set("save-configuration", false);
	resourceConfig.set("fragview-internal-shaders-files", "fragview-internal-shaders.zip");
	resourceConfig.set("cache-directory", ".");

	IConfig &sandboxConfig = this->getSubConfig("sandbox");
	/*  Assets import.  */
	//sandboxConfig.set("texture0", 0);
	//sandboxConfig.set("shader0", 0);
	//sandboxConfig.set("compression", false);
	/*  Variables.  */
	sandboxConfig.set("num_textures", 0);
	sandboxConfig.set("num_shaders", 0);
	sandboxConfig.set("num_compute", 0);
	sandboxConfig.set("num_program_shaders", 0);

	/*  sandbox rendering settings. */
	IConfig &renderSandboxSettingsConfig = this->getSubConfig("render-sandbox-graphic-settings");

	/*	Rendering default configuration.	*/
	renderSandboxSettingsConfig.set("anti-aliasing-samples", 0);
	renderSandboxSettingsConfig.set("anti-aliasing", false);
	renderSandboxSettingsConfig.set("texture-quality", 4);
	renderSandboxSettingsConfig.set("resolution-scale", 1.0f);
	renderSandboxSettingsConfig.set("coverage", 0);

	IConfig &sceneAsset = this->getSubConfig("scene-asset");
}

void Config::parseGetOpt(int argc, const char **argv) {

	int c;
	int index;
	/*  TODO determine all the sandbox or none-sandbox variables.   */
	IConfig &sandboxConfig = this->getSubConfig("sandbox");

	/*  First pass options. */
	while ((c = getopt_long(argc, (char *const *) argv, shortarg, longoptions, &index)) != EOF) {

		switch (c) {
			case 'h':
				/*  Print help. */
				Log::log(Log::Quite, "Help\n");
				exit(EXIT_SUCCESS);
			case 'd':   /*  Enable debug mode.  */
				this->set<int>("debug", 1);
				Log::setVerbosity(Log::Debug);
				break;
			case 'V':   /*  Enable verbosity logging.   */
				Log::setVerbosity(Log::Verbose);
				break;
			case 'q':   /*  Set logging to quite.   */
				Log::setVerbosity(Log::Quite);
				break;
			default:
				break;
		}
	}

	/*	Reset getopt.	*/
	optind = 0;
	optopt = 0;
	opterr = 0;

	while ((c = getopt_long(argc, (char *const *) argv, shortarg, longoptions, &index)) != EOF) {
		const char *option = longoptions[index].name;

		switch (c) {
			case 'w':   /*  */
				this->set<bool>("wallpaper", true);
				break;
			case 'a':   /*  */
				this->getSubConfig("render-driver").set<bool>("alpha", true);
				break;
			case 'S':   /*  */
				this->set<bool>("gamma-correction", true);
				break;
			case 'N':   /*  */
				this->set<bool>("notify-file", false);
				break;
			case 'I':   /*  */
				break;
			case 'c':   /*  */
				break;
			case 'f': {   /*  */
				/*  */
				int num = sandboxConfig.get<int>("num_shaders");
				sandboxConfig.set<const char *>(fvformatf("shader%d", num).c_str(), optarg);

				/*  Update number of textures.  */
				num++;
				sandboxConfig.set("num_shaders", num);
			}
				break;
			case 't':   /*  */
				if (optarg) {
					/*  */
					int num = sandboxConfig.get<int>("num_textures");
					sandboxConfig.set<const char *>(fvformatf("texture%d", num).c_str(), optarg);

					/*  Update number of textures.  */
					num++;
					sandboxConfig.set("num_textures", num);

				}
				break;
			case 'F':   /*  */
				break;
			case 'C': {   /*  */
				/*  */
				int num = sandboxConfig.get<int>("num_compute");
				sandboxConfig.set<const char *>(fvformatf("compute%d", num).c_str(), optarg);

				/*  Update number of textures.  */
				num++;
				sandboxConfig.set("num_compute", num);
			}
				break;
			case 'G':   /*  */
				break;
			case 'T':   /*  */
				break;
			case 'E':   /*  */
				break;
			case 'B':   /*  */
				break;
			case 'p':   /*  */
				break;
			case 'P':   /*  */
				break;
			case 's':   /*  */
				break;
			case 'b':   /*  Sandbox.    */
				this->set<bool>("sandbox", true);
				if (optarg) {

				}
				break;
			case 'O':   /*  */
				break;
			case 'U':   /*  */
				this->set<bool>("save-configuration", true);
				if (optarg) {
					this->set<const char *>("dump-configuration", optarg);
				}
				break;
			case ' ':    /*	Parse long option with string argument.	*/
				/*  renderer-dynamicInterface   */
				if (optarg) {
					this->set<const char *>(longoptions[index].name, (const char *) optarg);
				}
				break;
			case '_': {    /*	Parse long option with no argument.	*/
				if (optarg) {
					this->set(longoptions[index].name, (int) strtol(optarg, NULL, 10));
				}
			}
				break;
			case '~':   /*  Special type of options.    */
				if (strcmp(option, "renderer-opengl") == 0)
					this->set<const char *>("renderer-dynamicInterface",
					                        RenderingFactory::getInterfaceLibraryPath(RenderingFactory::OpenGL));
				if (strcmp(option, "renderer-vulkan") == 0)
					this->set<const char *>("renderer-dynamicInterface",
					                        RenderingFactory::getInterfaceLibraryPath(RenderingFactory::Vulkan));
				if (strcmp(option, "renderer-opencl") == 0)
					this->set<const char *>("renderer-dynamicInterface",
					                        RenderingFactory::getInterfaceLibraryPath(RenderingFactory::eOpenCL));
				if (strcmp(option, "renderer-directx") == 0)
					this->set<const char *>("renderer-dynamicInterface",
					                        RenderingFactory::getInterfaceLibraryPath(RenderingFactory::DirectX));
				if (strcmp(option, "no-decoration") == 0)
					this->set("window-bordered", false);
				if (strcmp(option, "v-sync") == 0)
					this->set("v-sync", true);
				break;
			default:    /*  */
				break;
		}
	}

	/*	Reset getopt.	*/
	optind = 0;
	optopt = 0;
	opterr = 0;
}


Config *Config::createConfig(int argc, const char **argv, const char *configpath) {

	/*	Check path.	*/
	if (configpath) {
		/*	Check if file exists.	*/
		if (!FileSystem::getFileSystem()->exists(configpath))
			throw InvalidArgumentException(fvformatf("Configuration file '%s' does not exists", configpath));
	}
	/*	Assess the parameter argument.	*/
	if (!argv)
		throw InvalidArgumentException("The argv parameter cannot be NULL");

	/*	Allocate config object.	*/
	Config *config = new Config();
	config->setName("global");
	assert(config);

	/*	Default settings.	*/
	config->setDefaultOption();

	/*	Read and parse configuration file.	*/
	if (configpath) {
		Ref<IO> Ref_io = Ref<IO>(FileSystem::getFileSystem()->openFile(configpath, IO::READ));
		/*  */
		config->parseConfigFile(Ref_io);
		FileSystem::getFileSystem()->closeFile(*Ref_io);
	}

	/*	Read user option and parse and override if config file used.	*/
	config->parseGetOpt(argc, argv);

	return config;
}

const char *Config::getConfigFilePath(int argc, const char **argv) {

	int c, index;
	const char *path = NULL;

	/*	Iterate through each option - find the override config option.	*/
	while ((c = getopt_long(argc, (char *const *) argv, shortarg, longoptions, &index)) != EOF) {
		if (c == 'D') {
			path = optarg;
			break;
		}
	}

	/*	Reset getopt.   */
	optind = 0;
	optopt = 0;
	opterr = 0;

	return path;
}

