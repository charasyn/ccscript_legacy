#include "util.h"
#include "fs.h"

std::string util::FindIncludedFile(const std::string& name, const std::string& filedir, const std::string& libdir)
{
	// Complete paths aren't looked for in include directories
	if (fs::path(name).is_absolute())
		return fs::exists( name )? name : std::string();

	// First, try in the provided file directory.
	fs::path base(filedir);
	base /= name;
	if( fs::exists( base ) )
		return base.string();

	// Next, try in the compilation working directory
	if( fs::exists( name ) )
		return name;

	// Finally, check the libs directory, if it has been provided.
	if(!libdir.empty()) {
		fs::path libpath = fs::path(libdir) / name;
		if( fs::exists(libpath) )
			return libpath.string();
	}
	return std::string();
}

