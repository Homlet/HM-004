#include "Base.h"
#include "File.h"


/*!
* Outputs the content of a text file into the referenced string.
*
* @param uri Path to the file.
* @param output Reference for the output string.
*
* @return Returns if the file was read successfully.
*/
bool readTextFile( std::string url, std::string& output )
{
	std::ifstream file_reader( url );

	if ( file_reader.is_open( ) )
	{
		std::string line;
		while ( file_reader.good( ) )
		{
			std::getline( file_reader, line );
			output.append( line + '\n' );
		}

		file_reader.close( );

		return true;
	}
	else
	{
		std::cout << "Unable to open file: " + url + "\n";

		return false;
	}
}


/*!
* Outputs the filename part of a given url.
*
* @return Returns true if a filename could be found in the string.
*/
bool getFilename( std::string url, std::string& output )
{
	output = url;

	const size_t last_slash_index = output.find_last_of( "\\/" );
	if ( last_slash_index != std::string::npos )
		output.erase( 0, last_slash_index + 1 );

	const size_t extension_index = output.rfind( '.' );
	if ( extension_index != std::string::npos )
		output.erase( extension_index );

	return true;
}
