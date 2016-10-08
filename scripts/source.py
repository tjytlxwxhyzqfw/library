#! /bin/pyton
#
# Process C source code by lines
#
# @author wcc

import os
import re

class Source(object):
	def __init__(self, path):
		self.code = []
		self.path = ""
		if not path or not os.path.exists(path):
			raise RuntimeError("Source(): __init__(): invalid 'path': %s"%path)
		if not path.endswith(".c") and not path.endswith(".h") and not path.endswith(".cpp"):
			raise RuntimeError("Source(): __init__(): need .c or .h or .cpp file: %s"%path)
		self.path = path
		self.code = self.__parse_code()
		self.headers = self.__parse_headers()

	#FIXME: // /*
	#FIXME: "//", "/*"
	def erase_comment(self):
		comment1 = r"/\*.*?\*/"
		comment2 = r"//.*"
		comment_left = r"/\*.*?$"
		comment_right = r"^.*?\*/"
		result = []
		incomment = False
		for line in self.code:
			line = re.sub(comment1, "", line)
			if re.search(comment_left, line) != None:
				line = re.sub(comment_left, "", line)
				incomment = True
			if re.search(comment_right, line) != None:
				line = re.sub(comment_right, "", line)
				incomment = False
			line = re.sub(comment2, "", line)
			if not incomment and line.strip() != "":
				result.append(line)
		self.code = result
		return self

	def erase_line(self, pat):
		result = []
		for line in self.code:
			if re.search(pat, line):
				continue
			result.append(line)
		self.code = result
		return self

	def replace_line(self, pat, repl):
		result = []
		for line in self.code:
			if re.search(pat, line):
				line = repl
			result.append(line)
		self.code = result
		return self

	def write_to_file(self, path):
		out = open(path, "w")
		out.write(self.__str__())
		out.close

	def reset_headers(self):
		self.headers = self.__parse_headers()

	def __parse_code(self):
		try:
			source_file = open(self.path)
			return [line for line in source_file]
		except:
			print "Cannot open: %s"%self.path
			raise RuntimeError("Source(): __parse_code(): cannot open file: %s"%self.path)
		finally:
			source_file.close()
	def __parse_headers(self):
		header = r'#include\s+"(.*?)"'
		headers = []
		for line in self.code:
			match = re.search(header, line)
			if match:
				full_header = match.groups()[0]
				name = full_header.split("/")[-1:][0]
				headers.append(name)
		return headers

	def __str__(self):
		return reduce(lambda x, y: x + y, self.code)

if __name__ == "__main__":
	import os

	path = "/home/wcc/OnlineJudge/source/src/math/numth.c"
	source = Source(path);
	print source.erase_comment().erase_line("#").replace_line("numth", "&")
	print source.headers
	
