# By default exclude most of the sub-project to prevent it compile some target
# is not needed by the project.
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/hpmcpp EXCLUDE_FROM_ALL)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/task-scheduler EXCLUDE_FROM_ALL)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/fvkcore EXCLUDE_FROM_ALL)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/infoware EXCLUDE_FROM_ALL)


IF(BUILTIN_JEMALLOC)
	#https://github.com/jemalloc/jemalloc.git
ENDIF()

# 	# https://github.com/googlefonts/noto-fonts
# 	# https://github.com/source-foundry/Hack
# 	# https://github.com/google/etc2comp

# 	#https://gitlab.com/libeigen/eigen
# 	# https://github.com/baldurk/renderdoc.git
# 	# https://github.com/ispc/ispc.git
# 	# https://github.com/mborgerding/kissfft.git
# 	# https://github.com/NVIDIAGameWorks/PhysX.git

# 	# https://github.com/jemalloc/jemalloc.git
# 	# https://github.com/turbulenz/NvTriStrip.git
# 	# https://github.com/microsoft/ShaderConductor.git
# 	# https://github.com/AcademySoftwareFoundation/openexr.git
# 	# https://github.com/erikd/libsamplerate.git
# 	# https://bitbucket.org/jthlim/pvrtccompressor
# 	# https://sourceforge.net/projects/libsquish
# 	# https://github.com/syoyo/tinyexr
# 	# https://github.com/tatsuhiro-t/wslay.git

# ELSE()
# 	MESSAGE(WARNING "Require Git for loading external source codes.")
# ENDIF()

# Check that all the depdends variables are found
