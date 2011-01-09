/*
---------------------------------------------------------------------------
Open Asset Import Library (ASSIMP)
---------------------------------------------------------------------------

Copyright (c) 2006-2011, ASSIMP Development Team

All rights reserved.

Redistribution and use of this software in source and binary forms, 
with or without modification, are permitted provided that the following 
conditions are met:

* Redistributions of source code must retain the above
copyright notice, this list of conditions and the
following disclaimer.

* Redistributions in binary form must reproduce the above
copyright notice, this list of conditions and the
following disclaimer in the documentation and/or other
materials provided with the distribution.

* Neither the name of the ASSIMP team, nor the names of its
contributors may be used to endorse or promote products
derived from this software without specific prior
written permission of the ASSIMP Development Team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
---------------------------------------------------------------------------
*/

/** @file  export.h
*  @brief Defines the C-API for the Assimp export interface
*/
#ifndef AI_EXPORT_H_INC
#define AI_EXPORT_H_INC

#ifndef ASSIMP_BUILD_NO_EXPORT

#include "aiTypes.h"

#ifdef __cplusplus
#include <boost/utility.hpp>
#endif  //__cplusplus

#ifdef __cplusplus
extern "C" {
#endif

struct aiScene;  // aiScene.h

/** Describes an file format which Assimp can export to. Use aiGetExportFormatCount() to 
* learn how many export formats the current Assimp build supports and aiGetExportFormatDescription()
* to retrieve a description of an export format option.
*/
struct aiExportFormatDesc
{
	/// a short string ID to uniquely identify the export format. Use this ID string to 
	/// specify which file format you want to export to when calling aiExportScene().
	/// Example: "dae" or "obj"
	const char* id; 

	/// A short description of the file format to present to users. Useful if you want
	/// to allow the user to select an export format.
	const char* description;

	/// Recommended file extension for the exported file in lower case. 
	const char* fileExtension;
};

/** Returns the number of export file formats available in the current Assimp build.
* Use aiGetExportFormatDescription() to retrieve infos of a specific export format.
*/
ASSIMP_API size_t aiGetExportFormatCount(void);

/** Returns a description of the nth export file format. Use aiGetExportFormatCount()
* to learn how many export formats are supported. 
* @param pIndex Index of the export format to retrieve information for. Valid range is 0 to aiGetExportFormatCount()
* @return A description of that specific export format. NULL if pIndex is out of range.
*/
ASSIMP_API const C_STRUCT aiExportFormatDesc* aiGetExportFormatDescription( size_t pIndex);

/** Describes a blob of exported scene data. Use aiExportScene() to create a blob containing an
* exported scene. The memory referred by this structure is owned by Assimp. Use aiReleaseExportedFile()
* to free its resources. Don't try to free the memory on your side - it will crash for most build configurations
* due to conflicting heaps.
*/
struct aiExportDataBlob 
#ifdef __cplusplus
	: public boost::noncopyable
#endif // __cplusplus
{
	/// Size of the data in bytes
	size_t size;

	/// The data. 
	void* data;

#ifdef __cplusplus
	/// Default constructor
	aiExportDataBlob() { size = 0; data = NULL; }
	/// Releases the data
	~aiExportDataBlob() { delete data; }
#endif // __cplusplus
};


// --------------------------------------------------------------------------------
/** Exports the given scene to a chosen file format. Returns the exported data as a binary blob which
* you can write into a file or something. When you're done with the data, use aiReleaseExportedData()
* to free the resources associated with the export. 
* @param pScene The scene to export. Stays in possession of the caller, is not changed by the function.
* @param pFormatId ID string to specify to which format you want to export to. Use 
* aiGetExportFormatCount() / aiGetExportFormatDescription() to learn which export formats are available.
* @return the exported data or NULL in case of error
*/
ASSIMP_API const C_STRUCT aiExportDataBlob* aiExportScene( const C_STRUCT aiScene* pScene, const char* pFormatId );


// --------------------------------------------------------------------------------
/** Convenience function to write a blob to a file. The file is written using standard C
 *  file IO functionality or via a user-supplied IOSystem implementation. 
 *  @param pBlob A data blob obtained from a previous call to #aiExportScene. Must not be NULL.
 *  @param pPath Full target file name. Target must be accessible.
 *  @param pIOSystem Custom IO implementation to be used for writing. Pass NULL to
 *    use the default implementation, which uses the standard C file IO functionality.
 *  @return AI_SUCCESS if everything was fine. */
ASSIMP_API aiReturn aiWriteBlobToFile( const C_STRUCT aiExportDataBlob* pBlob, const char* pPath, const C_STRUCT aiFileIO* pIOSystem );


// --------------------------------------------------------------------------------
/** Releases the memory associated with the given exported data. Use this function to free a data blob
* returned by aiExportScene(). 
* @param pData the data blob returned by aiExportScene
*/
ASSIMP_API C_STRUCT void aiReleaseExportData( const C_STRUCT aiExportDataBlob* pData );

#ifdef __cplusplus
}
#endif

#endif // ASSIMP_BUILD_NO_EXPORT
#endif // AI_EXPORT_H_INC

