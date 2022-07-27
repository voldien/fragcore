# By default exclude most of the sub-project to prevent it compile some target
# is not needed by the project.

ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/task-scheduler EXCLUDE_FROM_ALL)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/cxx-config EXCLUDE_FROM_ALL)

# 	# https://github.com/googlefonts/noto-fonts
# 	# https://github.com/source-foundry/Hack

# 	# https://github.com/baldurk/renderdoc.git
# 	# https://github.com/mborgerding/kissfft.git

# 	# https://github.com/turbulenz/NvTriStrip.git
# 	# https://github.com/microsoft/ShaderConductor.git
# 	# https://github.com/AcademySoftwareFoundation/openexr.git
# 	# https://github.com/erikd/libsamplerate.git
# ELSE()
# 	MESSAGE(WARNING "Require Git for loading external source codes.")
# ENDIF()

# Check that all the depdends variables are found
