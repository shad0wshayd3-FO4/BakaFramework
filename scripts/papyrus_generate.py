import os

SOURCE_TYPES = (
	".psc"
)

def make_cmake(a_directories):
	tmp = []
	for directory in a_directories:
		for root, dirs, files in os.walk(directory):
			for file in files:
				if file.endswith(SOURCE_TYPES):
					path = os.path.join(root, file)
					tmp.append(os.path.normpath(path))

	sources = []
	for file in tmp:
		sources.append(file.replace("\\", "/"))
	sources.sort()

	out = open(os.path.join("cmake", "sourcelist_papyrus.cmake"), "w")
	out.write("set(SOURCE_FILES\n")
	for source in sources:
		src = os.path.splitext(os.path.basename(source))
		out.write("\t{}\n".format(src[0]))
	out.write(")\n")

def main():
	cur = os.path.dirname(os.path.realpath(__file__))
	os.chdir(cur + "/..")
	make_cmake([ "src/papyrus" ])

if __name__ == "__main__":
	main()
