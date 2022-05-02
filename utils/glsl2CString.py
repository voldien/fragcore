#!/usr/bin/env python
# Convert text to header files and c source file.
# Copyright (C) 2018 Valdemar Lindberg
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
import os
import sys
from os.path import isfile
from shutil import move
import codecs

HEADERNAME = "Shaders.h"
CNAME = "Shaders.cpp"


def convert2CString(cpbuf):
	'''
	Converts text file into a c-string.
	:param cpbuf: text.
	:return: c-string text.
	:rtype str
	'''
	stringbuf = ""
	lines = cpbuf.split('\n')

	for line in lines:
		utf8line = line  # .decode("utf-8", "strict")
		stringbuf += "\"{}\\n\"\n".format(utf8line)

	return stringbuf[0:len(stringbuf) - 1]


def variable_extension(path):
	path_extension = path.split('.')[-1]
	print(path_extension)
	extensions = {"vert": "vs", "frag": "fs", "geom": "ge",
				  "tesc": "tec", "tese": "tes", "comp": "cm"}
	if path_extension in extensions:
		return "_{}".format(extensions[path_extension])
	return ""


def main(argv):
	if len(argv) != 2:
		sys.exit(1)

	dir = os.path.abspath(argv[1])
	print(dir)
	shadfiles = os.listdir(dir)

	#
	headpath = "./{}".format(HEADERNAME)
	cpath = "./{}".format(CNAME)

	# Final destination files.
	headerFinalDest = "./include/Shaders/{}".format(HEADERNAME)
	sourceFinalDest = "./src/shaders/{}".format(CNAME)

	if os.path.exists(headerFinalDest) and os.path.exists(sourceFinalDest):
		# Check if need to be updated.
		needUpdate = False
		destTime = os.path.getmtime(headerFinalDest)
		for file in shadfiles:
			filepath = "{}/{}".format(dir, file)
			if isfile(filepath) and file.endswith((".glsl", ".vert", ".frag", ".geom", ".tesc", ".tese", ".comp")):
				srcTime = os.path.getmtime(filepath)
				if destTime < srcTime:
					needUpdate = True
					break

		if not needUpdate:
			print("No changes. Nothing was written")
			sys.exit(0)
	else:
		print("No files exists, forcing to create the header and source files.")
	#
	header = codecs.open(headpath, 'w', encoding='utf8')
	cfile = codecs.open(cpath, 'w', encoding='utf8')

	cfile.write("#include\"Shaders/{}\"\n".format(HEADERNAME))
	header.write(
		"#ifndef _CONVERTED_2_SHADER_\n#define _CONVERTED_2_SHADER_ 1\n")

	# Iterate through each files.
	for file in shadfiles:
		filepath = "{}/{}".format(dir, file)
		if isfile(filepath) and file.endswith((".glsl", ".vert", ".frag", ".geom", ".tesc", ".tese", ".comp")):
			with codecs.open(filepath, 'r', encoding='utf8') as f:
				# Convert text to c-string.
				glsl = convert2CString(f.read())
				cvariable = file.split('.')[0]
				cvariable.replace('.', '_')
				cvariable = "gc_shader_{}".format(cvariable)
				cvariable += variable_extension(filepath)

				# TODO add support for size of the variable.

				# update header.
				header.write("extern const char* {};\n".format(cvariable))
				header.write(
					"extern const unsigned int {}_size;\n".format(cvariable))

				# update c file.
				cfile.write("const char* {} = {};\n\n".format(cvariable, glsl))
				cfile.write("const unsigned int {}_size = {};\n\n".format(
					cvariable, "{}".format(len(glsl))))

				print("Converted: {}".format(filepath))

	# End header file.
	header.write("#endif")

	# Close file and flush the remaining content.
	cfile.close()
	header.close()

	# Copy file to directory.
	move(headpath, headerFinalDest)
	move(cpath, sourceFinalDest)

	print("Finish converting files from directory {} to {}:{}".format(
		argv[1], headerFinalDest, sourceFinalDest))


if __name__ == '__main__':
	main(sys.argv)
