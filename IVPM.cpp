/*
 * IVPM.cpp
 *
 *  Created on: Feb 15, 2012
 *      Author: Matteo Ruggero Ronchi
 *
 *  Copyright 2012 Matteo Ruggero Ronchi - matrronchi.license@gmail.com
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "IVPM.h"

//standard c libraries
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//standard Template Library
#include <string>
#include <vector>

//to access type information
#include <type_traits>
#include <climits>

//to read and write directories
#include <dirent.h>

//for input/output from files
#include <fstream>
#include <iostream>

//opencv
#include <cv.h>
#include <highgui.h>

//using namespace cv;
using namespace std;

/********************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************
 *
 *	CONSTRUCTOR - DESTRUCTOR
 *
 ********************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************/


template <typename Matx3DataType> IVPM<Matx3DataType>::IVPM()
{
	this->Matx3Pointer = new TemplateMatx3<Matx3DataType>();
	this->cvImageData = NULL;
}


template <typename Matx3DataType> IVPM<Matx3DataType>::~IVPM()
{
	if( this->Matx3Pointer != NULL )
		delete this->Matx3Pointer;

	if( this->cvImageData != NULL )
		delete[] this->cvImageData;

	cv::destroyAllWindows();
}


/********************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************
 *
 * IVPM CLASS INTERFACE
 *
 * POSSIBLE INPUT: 	- IMAGE FILE
 * 					- LIST OF IMAGE FILES
 * 					- VIDEO FILE
 *
 * OPERATION TO DO SPECIFIED BY AN ENUM TYPE
 *
 * POSSIBLE OUTPUT: - SAVE ON DISK
 * 					- POINTER TO SINGLE MATX3 (IN CASE OF SINGLE IMAGE FILE) (MATX3 ON HEAP)
 * 					- POINTER TO VECTOR OF POINTERS TO MATX3 (IN CASE OF LIST OF FILES) (VECTOR AND MATX3 ON HEAP)
 *
 ********************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************/




template <typename Matx3DataType> void IVPM<Matx3DataType>::loadImageAndProcessAndSave(

		const string& inputDirPath, const string& relativeImagePath, const string& saveDirPath, loadImageType loadImgType, saveImageFormat saveImgFormat,
		imageOperationType opType, float scaleX, float scaleY, interpolationType interType ){

	this->loadImage( inputDirPath + relativeImagePath, loadImgType );

	this->processImage( opType, relativeImagePath, scaleX, scaleY, interType );

	this->saveImage( saveDirPath + relativeImagePath, saveImgFormat );

}


template <typename Matx3DataType> TemplateMatx3<Matx3DataType>* IVPM<Matx3DataType>::loadImageAndProcessAndReturn(

		const string& inputDirPath, const string& relativeImagePath, loadImageType loadImgType,
		imageOperationType opType, float scaleX, float scaleY, interpolationType interType ){

	this->loadImage( inputDirPath + relativeImagePath, loadImgType );

	this->processImage( opType, relativeImagePath, scaleX, scaleY, interType );

	this->ImageToMatx3();
	//TemplateMatx3<Matx3DataType> * ret = new TemplateMatx3<Matx3DataType>( this->Matx3Pointer );

	//return ret;
	return this->Matx3Pointer;
}

template <typename Matx3DataType> void IVPM<Matx3DataType>::loadImagesFromFileAndProcessAndSave(

		const string& inputDirPath, const string& filePath, const string& saveDirPath, loadImageType loadImgType, saveImageFormat saveImgFormat,
		imageOperationType opType, float scaleX, float scaleY, interpolationType interType ){

	if( filePath.compare("") != 0 ){

		ifstream inFile;
		string relativeImagePath;

		inFile.open( filePath, ifstream::in );

		if( inFile.is_open() ){

			getline( inFile, relativeImagePath );
			while( inFile.good() ){

				this->loadImageAndProcessAndSave( inputDirPath, relativeImagePath, saveDirPath, loadImgType, saveImgFormat, opType, scaleX, scaleY, interType );
				getline( inFile, relativeImagePath );
			}

			inFile.close();

		}else{
			string s("Unable to open specified File! -> " + filePath );
			cout << s << endl;
			throw s;
		}

	}else{

		DIR* dirPointer;
		struct dirent* dirEntryPointer;

		/*
		 * 0x4 is esadecimal constant for folders
		 */
		unsigned char isFolder = 0x4;

		dirPointer = opendir( inputDirPath.c_str() );
		if( dirPointer == NULL ){
			string s("Error opening directory -> " + inputDirPath );
			cout << s << endl;
			throw s;
		}

		while( (dirEntryPointer = readdir( dirPointer )) ){

			if( !strcmp( dirEntryPointer->d_name, "." ) || !strcmp( dirEntryPointer->d_name, ".." ) || dirEntryPointer->d_type == isFolder );
			else{

				this->loadImageAndProcessAndSave( inputDirPath, dirEntryPointer->d_name, saveDirPath, loadImgType, saveImgFormat, opType, scaleX, scaleY, interType );
			}

		}

		closedir( dirPointer );

	}

}


template <typename Matx3DataType> vector< TemplateMatx3<Matx3DataType>* >* IVPM<Matx3DataType>::loadImagesFromFileAndProcessAndReturn(

		const string& inputDirPath, const string& filePath, loadImageType loadImgType,
		imageOperationType opType, float scaleX, float scaleY, interpolationType interType ){

	/*
	 * allocates a vector on heap and returns a pointer to its memory
	 */
	vector< TemplateMatx3<Matx3DataType>* >* retPointer = new vector< TemplateMatx3<Matx3DataType>* >();

	if( filePath.compare("") != 0 ){

		ifstream inFile;
		string relativeImagePath;

		inFile.open( filePath, ifstream::in );

		if( inFile.is_open() ){

			getline( inFile, relativeImagePath );
			while( inFile.good() ){

				retPointer->push_back( this->loadImageAndProcessAndReturn( inputDirPath, relativeImagePath, loadImgType, opType, scaleX, scaleY, interType ) );
				this->Matx3Pointer = NULL;
				getline( inFile, relativeImagePath );
			}

			inFile.close();

		}else{
			string s("Unable to open specified File! -> " + filePath );
			cout << s << endl;
			throw s;
		}

	}else{

		/*
		 * if file path is not specified in input -> filePath = ""
		 * method will process all images found in inputDirPath directory
		 */

		DIR* dirPointer;
		struct dirent* dirEntryPointer;

		/*
		 * 0x4 is esadecimal constant for folders
		 */
		unsigned char isFolder = 0x4;

		dirPointer = opendir( inputDirPath.c_str() );
		if( dirPointer == NULL ){
			string s("Error opening directory -> " + inputDirPath );
			cout << s << endl;
			throw s;
		}

		while( (dirEntryPointer = readdir( dirPointer )) ){

			if( !strcmp( dirEntryPointer->d_name, "." ) || !strcmp( dirEntryPointer->d_name, ".." ) || dirEntryPointer->d_type == isFolder );
			else{

				retPointer->push_back( this->loadImageAndProcessAndReturn( inputDirPath, dirEntryPointer->d_name, loadImgType, opType, scaleX, scaleY, interType ) );
			}

		}

		closedir( dirPointer );

	}

	return retPointer;

}


template <typename Matx3DataType> void IVPM<Matx3DataType>::loadVideoAndProcessAndSave(

		const string& videoPath, const string& saveDirPath, saveImageFormat saveImgFormat,
		float startTimeStamp, float endTimeStamp, float frameRate, bool showFrames,
		imageOperationType opType, float scaleX, float scaleY, interpolationType interType ){

	if( !this->cvVideo.open( videoPath ) ){

		string s("Error in opening video at path -> " + videoPath);
		cout << s << endl;
		throw s;
	}

	/*
	 * procedure to extract video name from its path
	 */
	string videoName( videoPath );
	size_t lastSlashPosition;

	lastSlashPosition = videoName.rfind("/");
	if( lastSlashPosition == string::npos ){

		string s("Can't find videoName -> " + videoName );
		cout << s << endl;
		throw s;
	}else{

		videoName = videoName.substr( lastSlashPosition + 1, videoName.length() - lastSlashPosition );
	}


	/*
	 * frames are supposed to be represented in RGB color space
	 */
	this->cvVideo.set( CV_CAP_PROP_CONVERT_RGB, true );

	float currentTimeStamp = startTimeStamp;
	string accessedFrame;

	/*
	 * setting frame rate
	 */
	if( frameRate == 0 )
		frameRate = this->cvVideo.get( CV_CAP_PROP_FPS );
	else{
		if( frameRate > this->cvVideo.get( CV_CAP_PROP_FPS ) ){
			string s("Wanted frame rate is higher than video frame rate -> " + to_string(this->cvVideo.get( CV_CAP_PROP_FPS )));
			cout << s << endl;
			throw s;
		}
	}

	while( currentTimeStamp <= endTimeStamp ){

		this->cvVideo.set( CV_CAP_PROP_POS_MSEC, currentTimeStamp );
		this->cvVideo >> this->cvImage;

		/*
		 * frames are supposed to be represented in RGB color space
		 */
		this->cvImageImageType = this->IMAGE_RGB;

		stringstream ss;
		ss << videoName << "@Frame_" << currentTimeStamp << "_ms.JPEG";
		accessedFrame = ss.str();

		/*
		 * processes all frames in same way
		 */
		this->processImage( opType, accessedFrame, scaleX, scaleY, interType );

		if( showFrames ){
			this->showImage( accessedFrame, true );
		}

		/*
		 * saves frame after processing
		 */
		this->saveImage( saveDirPath + accessedFrame, saveImgFormat );

		//goes to next frame
		currentTimeStamp += ( 1 / frameRate ) * 1000;

	}

}


template <typename Matx3DataType> vector< TemplateMatx3<Matx3DataType>* >* IVPM<Matx3DataType>::loadVideoAndProcessAndReturn(

		const string& videoPath,
		float startTimeStamp, float endTimeStamp, float frameRate, bool showFrames,
		imageOperationType opType, float scaleX, float scaleY, interpolationType interType ){

	if( !this->cvVideo.open( videoPath ) ){

		string s("Error in opening video at path -> " + videoPath);
		cout << s << endl;
		throw s;
	}

	/*
	 * procedure to extract video name from its path
	 */
	string videoName( videoPath );
	size_t lastSlashPosition;

	lastSlashPosition = videoName.rfind("/");
	if( lastSlashPosition == string::npos ){

		string s("Can't find videoName -> " + videoName );
		cout << s << endl;
		throw s;
	}else{

		videoName = videoName.substr( lastSlashPosition + 1, videoName.length() - lastSlashPosition );
	}

	/*
	 * frames are supposed to be represented in RGB color space
	 */
	this->cvVideo.set( CV_CAP_PROP_CONVERT_RGB, true );

	float currentTimeStamp = startTimeStamp;
	string accessedFrame;

	/*
	 * setting frame rate
	 */
	if( frameRate == 0 )
		frameRate = this->cvVideo.get( CV_CAP_PROP_FPS );
	else{
		if( frameRate > this->cvVideo.get( CV_CAP_PROP_FPS ) ){
			string s("Wanted frame rate is higher than video frame rate -> " + to_string(this->cvVideo.get( CV_CAP_PROP_FPS )));
			cout << s << endl;
			throw s;
		}
	}

	/*
	 * allocates a vector on heap and returns a pointer to its memory
	 */
	vector< TemplateMatx3<Matx3DataType>* >* retPointer = new vector< TemplateMatx3<Matx3DataType>* >();

	while( currentTimeStamp <= endTimeStamp ){

		this->cvVideo.set( CV_CAP_PROP_POS_MSEC, currentTimeStamp );
		this->cvVideo >> this->cvImage;

		/*
		 * frames are supposed to be represented in RGB color space
		 */
		this->cvImageImageType = this->IMAGE_RGB;

		stringstream ss;
		ss << videoName << "@Frame_" << currentTimeStamp << "_ms.JPEG";
		accessedFrame = ss.str();

		/*
		 * processes all frames in same way
		 */
		this->processImage( opType, accessedFrame, scaleX, scaleY, interType );

		if( showFrames ){
			this->showImage( accessedFrame, true );
		}

		this->ImageToMatx3();

		/*
		 * allocates memory on heap for processed image
		 */
		//TemplateMatx3<Matx3DataType> * processedFramePointer = new TemplateMatx3<Matx3DataType>( this->Matx3Pointer );

		/*
		 * pushes frame pointer back in vector on heap
		 */
		//retPointer->push_back( processedFramePointer );
		retPointer->push_back( this->Matx3Pointer );

		//moves to next frame
		currentTimeStamp += ( 1 / frameRate ) * 1000;

	}

	return retPointer;

}


/********************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************
 *
 * IVPM CLASS INTERFACE
 *
 * POSSIBLE INPUT: 	- POINTER TO MATX3 OBJECT
 * 					- POINTER TO VECTOR OF POINTERS TO MATX3
 *
 * OPERATION TO DO SPECIFIED BY AN ENUM TYPE
 *
 * POSSIBLE OUTPUT: - SAVE ON DISK
 * 					- POINTER TO SINGLE MATX3 (IN CASE OF SINGLE IMAGE FILE) (MATX3 ON HEAP)
 * 					- POINTER TO VECTOR OF POINTERS TO MATX3 (IN CASE OF LIST OF FILES) (VECTOR AND MATX3 ON HEAP)
 *
 ********************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************/


template <typename Matx3DataType> void IVPM<Matx3DataType>::convertMatx3AndProcessAndSave(

		TemplateMatx3<Matx3DataType>* inputMatx3, const string& relativeImagePath, const string& saveDirPath, saveImageFormat saveImgFormat,
		imageOperationType opType, float scaleX, float scaleY, interpolationType interType ){

	/*
	 * copy member Matx3 from input
	 */
	//this->Matx3Pointer->copy_me( inputMatx3 );
	this->Matx3Pointer = inputMatx3;

	if ( this->Matx3Pointer->data == NULL) {
		string s("Error IVPM Object has EMPTY Matx3");
		cout << s << endl;
		throw s;
	}

	string newRelativeImagePath( relativeImagePath + "_saved_from_converted_Matx3.JPEG" ); //probem with name of image to save <-----------------------------------

	/*
	 * convert Matx3 to OpenCV image for processing (normalize for default)
	 */
	this->Matx3ToImage();

	this->processImage( opType, newRelativeImagePath, scaleX, scaleY, interType );

	this->saveImage( saveDirPath + newRelativeImagePath, saveImgFormat );

}


template <typename Matx3DataType> TemplateMatx3<Matx3DataType>* IVPM<Matx3DataType>::convertMatx3AndProcessAndReturn(

		TemplateMatx3<Matx3DataType>* inputMatx3,
		imageOperationType opType, float scaleX, float scaleY, interpolationType interType ){

	/*
	 * copy member Matx3 from input
	 */
	//this->Matx3Pointer->copy_me( inputMatx3 );
	this->Matx3Pointer = inputMatx3;

	if ( this->Matx3Pointer->data == NULL) {
		string s("Error IVPM Object has EMPTY Matx3");
		cout << s << endl;
		throw s;
	}

	string windowName( "converted_from_Matx3" );

	/*
	 * convert Matx3 to OpenCV image for processing (normalize for default)
	 */
	this->Matx3ToImage();

	this->processImage( opType, windowName, scaleX, scaleY, interType );

	/*
	 * converts processed image into Matx3 format to return
	 */
	this->ImageToMatx3();

	//TemplateMatx3<Matx3DataType> * ret = new TemplateMatx3<Matx3DataType>( this->Matx3Pointer );

	//return ret;
	return this->Matx3Pointer;
}


template <typename Matx3DataType> void IVPM<Matx3DataType>::convertMatx3VectorAndProcessAndSave(

		vector< TemplateMatx3<Matx3DataType>* >* inputVectorPointer, const string& saveDirPath, saveImageFormat saveImgFormat,
		imageOperationType opType, float scaleX, float scaleY, interpolationType interType ){

	typename vector< TemplateMatx3<Matx3DataType>* >::const_iterator it;
	int i = 0;
	for( it = inputVectorPointer->begin(); it != inputVectorPointer->end(); it++ ){
		i++;
		this->convertMatx3AndProcessAndSave( (*it), to_string(i), saveDirPath, saveImgFormat, opType, scaleX, scaleY, interType );//problem with name of image to save <--------------------------------
	}

}


template <typename Matx3DataType> vector< TemplateMatx3<Matx3DataType>* >* IVPM<Matx3DataType>::convertMatx3VectorAndProcessAndReturn(

		vector< TemplateMatx3<Matx3DataType>* >* inputVectorPointer,
		imageOperationType opType, float scaleX, float scaleY, interpolationType interType ){

	/*
	 * allocates a vector on heap and returns a pointer to its memory
	 */
	vector< TemplateMatx3<Matx3DataType>* >* retPointer = new vector< TemplateMatx3<Matx3DataType>* >();

	typename vector< TemplateMatx3<Matx3DataType>* >::const_iterator it;

	for( it = inputVectorPointer->begin(); it != inputVectorPointer->end(); it++ ){

		retPointer->push_back( this->convertMatx3AndProcessAndReturn( (*it), opType, scaleX, scaleY, interType ) );
	}

	return retPointer;

}


/********************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************
 *
 * IVPM CLASS INTERFACE
 *
 * POSSIBLE INPUT: 	- MATX3 FILE
 * 					- LIST OF MATX3 FILES
 *
 * OPERATION TO DO SPECIFIED BY AN ENUM TYPE
 *
 * POSSIBLE OUTPUT: - SAVE ON DISK
 * 					- POINTER TO SINGLE MATX3 (IN CASE OF SINGLE IMAGE FILE) (MATX3 ON HEAP)
 * 					- POINTER TO VECTOR OF POINTERS TO MATX3 (IN CASE OF LIST OF FILES) (VECTOR AND MATX3 ON HEAP)
 *
 ********************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************/


template <typename Matx3DataType> void IVPM<Matx3DataType>::loadMatx3AndProcessAndSave(

		const string& inputDirPath, const string& matx3Path, matx3FileType matx3Type, const string& saveDirPath, saveImageFormat saveImgFormat,
		imageOperationType opType, float scaleX, float scaleY, interpolationType interType ){

	/*
	 * load new member Matx3 from file
	 */
	switch( matx3Type ){

		case TEXT:
			//this->Matx3Pointer->load( inputDirPath + matx3Path, TemplateMatx3<Matx3DataType>::TEXT );
			this->Matx3Pointer = new TemplateMatx3<Matx3DataType>( inputDirPath + matx3Path, TemplateMatx3<Matx3DataType>::TEXT );
			break;

		case BIN:
			//this->Matx3Pointer->load( inputDirPath + matx3Path, TemplateMatx3<Matx3DataType>::BIN );
			this->Matx3Pointer = new TemplateMatx3<Matx3DataType>( inputDirPath + matx3Path, TemplateMatx3<Matx3DataType>::BIN );
			break;

		default:
			break;
	}

	if ( this->Matx3Pointer->data == NULL) {
		string s("Error in loading Matx3 file. IVPM Object has EMPTY Matx3");
		cout << s << endl;
		throw s;
	}

	string relativeImagePath( matx3Path + ".JPEG" );

	/*
	 * convert Matx3 to OpenCV image for processing (normalize for default)
	 */
	this->Matx3ToImage();

	this->processImage( opType, relativeImagePath, scaleX, scaleY, interType );

	this->saveImage( saveDirPath + relativeImagePath, saveImgFormat );

}


template <typename Matx3DataType> TemplateMatx3<Matx3DataType>* IVPM<Matx3DataType>::loadMatx3AndProcessAndReturn(

		const string& inputDirPath, const string& matx3Path, matx3FileType matx3Type,
		imageOperationType opType, float scaleX, float scaleY, interpolationType interType ){

	/*
	 * load new member Matx3 from file
	 */
	switch( matx3Type ){

		case TEXT:
			//this->Matx3Pointer->load( inputDirPath + matx3Path, TemplateMatx3<Matx3DataType>::TEXT );
			this->Matx3Pointer = new TemplateMatx3<Matx3DataType>( inputDirPath + matx3Path, TemplateMatx3<Matx3DataType>::TEXT );
			break;

		case BIN:
			//this->Matx3Pointer->load( inputDirPath + matx3Path, TemplateMatx3<Matx3DataType>::BIN );
			this->Matx3Pointer = new TemplateMatx3<Matx3DataType>( inputDirPath + matx3Path, TemplateMatx3<Matx3DataType>::BIN );
			break;

		default:
			break;
	}

	if ( this->Matx3Pointer->data == NULL) {
		string s("Error in loading Matx3 file. IVPM Object has EMPTY Matx3");
		cout << s << endl;
		throw s;
	}

	string relativeImagePath( matx3Path + ".JPEG" );

	/*
	 * convert Matx3 to OpenCV image for processing (normalize for default)
	 */
	this->Matx3ToImage();

	this->processImage( opType, relativeImagePath, scaleX, scaleY, interType );

	/*
	 * converts processed image into Matx3 format to return
	 */
	this->ImageToMatx3();

	//TemplateMatx3<Matx3DataType> * ret = new TemplateMatx3<Matx3DataType>( this->Matx3Pointer );

	//return ret;
	return this->Matx3Pointer;
}


template <typename Matx3DataType> void IVPM<Matx3DataType>::loadMatx3FromFileAndProcessAndSave(

		const string& inputDirPath, const string& filePath, matx3FileType matx3Type, const string& saveDirPath, saveImageFormat saveImgFormat,
		imageOperationType opType, float scaleX, float scaleY, interpolationType interType ){

	ifstream inFile;
	string matx3Path;

	inFile.open( filePath, ifstream::in );

	if( inFile.is_open() ){

		getline( inFile, matx3Path );
		while( inFile.good() ){

			this->loadMatx3AndProcessAndSave( inputDirPath, matx3Path, matx3Type, saveDirPath, saveImgFormat, opType, scaleX, scaleY, interType );
			getline( inFile, matx3Path );
		}

		inFile.close();

	}else{
		string s("Unable to open specified File!");
		cout << s << endl;
		throw s;
	}


}


template <typename Matx3DataType> vector< TemplateMatx3<Matx3DataType>* >* IVPM<Matx3DataType>::loadMatx3FromFileAndProcessAndReturn(

		const string& inputDirPath, const string& filePath, matx3FileType matx3Type,
		imageOperationType opType, float scaleX, float scaleY, interpolationType interType ){

	/*
	 * allocates a vector on heap and returns a pointer to its memory
	 */
	vector< TemplateMatx3<Matx3DataType>* >* retPointer = new vector< TemplateMatx3<Matx3DataType>* >();

	ifstream inFile;
	string matx3Path;

	inFile.open( filePath, ifstream::in );

	if( inFile.is_open() ){

		getline( inFile, matx3Path );
		while( inFile.good() ){

			retPointer->push_back( this->loadMatx3AndProcessAndReturn( inputDirPath, matx3Path, matx3Type, opType, scaleX, scaleY, interType ) );
			getline( inFile, matx3Path );
		}

		inFile.close();

	}else{
		string s("Unable to open specified File!");
		cout << s << endl;
		throw s;
	}

	return retPointer;


}


/********************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************
 *
 *	UTILITY METHODS
 *
 ********************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************/


template <typename Matx3DataType> void IVPM<Matx3DataType>::loadImage(

		const string& absoluteImagePath, loadImageType loadImgType ){

	cout << "Launched Method IVPM::loadImage" << endl;

	switch( loadImgType ){

		case IMAGE_GRAY:
			this->cvImage = cv::imread( absoluteImagePath, CV_LOAD_IMAGE_GRAYSCALE );
			this->cvImageImageType = this->IMAGE_GRAY;
			break;

		case IMAGE_RGB:
			this->cvImage = cv::imread( absoluteImagePath, CV_LOAD_IMAGE_COLOR );
			this->cvImageImageType = this->IMAGE_RGB;
			break;

		case IMAGE_UNCHANGED:
			this->cvImage = cv::imread( absoluteImagePath, CV_LOAD_IMAGE_UNCHANGED );

			if( this->cvImage.channels() == 3 )
				//3 channel image =>  RGB
				this->cvImageImageType = this->IMAGE_RGB;
			else if( this->cvImage.channels() == 1 )
				//1 channel image =>  GRAYSCALE
				this->cvImageImageType = this->IMAGE_GRAY;
			else if( this->cvImage.channels() == 4 )
				//4 channel image =>  RGBA
				this->cvImageImageType = this->IMAGE_RGBA;
			else{
				string s("Can't read image -> Color space not recognized.");
				cout << s << endl;
				throw s;}
			break;

		default:
			string s("Invalid image type specified.");
			cout << s << endl;
			throw s;
			break;

	}

	/*
	 * 	CV_LOAD_IMAGE_COLOR (>0)		-> the loaded image is forced to be a 3-channel color image
	 *  CV_LOAD_IMAGE_GRAYSCALE (=0) 	-> the loaded image is forced to be grayscale
	 *  CV_LOAD_IMAGE_UNCHANGED (<0)	-> the loaded image will be loaded as is
	 *
	 *  support formats: jpeg, png, bmp, and others.
	 */

	if ( this->cvImage.data == NULL) {
		string s("Error while loading image at path: " + absoluteImagePath);
		cout << s << endl;
		throw s;
	}

	cout << "Finished Method IVPM::loadImage" << endl;

}


template <typename Matx3DataType> void IVPM<Matx3DataType>::saveImage(

		const string& absoluteImagePath, saveImageFormat saveImgFormat,
		int JPEG_QUALITY, int PNG_COMPRESSION, int PXM_BINARY ){

	cout << "Launched Method IVPM::saveImage" << endl;

	if ( this->cvImage.data == NULL) {
		string s("Error IVPM Object has EMPTY cvImage");
		cout << s << endl;
		throw s;
	}

	string newFormatAbsoluteImagePath( absoluteImagePath );
	this->changeFormat( &newFormatAbsoluteImagePath, saveImgFormat );

	/*
	 * defining vector with saving parameters
	 *
	 */
	int parametersArr[] = { CV_IMWRITE_JPEG_QUALITY, JPEG_QUALITY, CV_IMWRITE_PNG_COMPRESSION, PNG_COMPRESSION, CV_IMWRITE_PXM_BINARY, PXM_BINARY };
	vector<int> parametersVec ( parametersArr, parametersArr + sizeof(parametersArr) / sizeof(int) );

	bool ret = cv::imwrite( newFormatAbsoluteImagePath, this->cvImage, parametersVec );
	if (!ret) {
		string s("Error while saving image at following path -> " + newFormatAbsoluteImagePath);
		cout << s << endl;
	 	throw s;
	}

	cout << "Finished Method IVPM::saveImage" << endl;

}


template <typename Matx3DataType> void IVPM<Matx3DataType>::processImage(

		imageOperationType opType,
		const string& relativeImagePath,
		float scaleX, float scaleY, interpolationType interType ){

	cout << "Launched Method IVPM::processImage" << endl;

	switch( opType ){

		case SHOW_IMG:
			this->showImage( relativeImagePath );
			break;

		case CONVERT2_RGB:
			this->convertImageToRGB();
			break;

		case CONVERT2_GRAYSCALE:
			this->convertImageToGray();
			break;

		case CONVERT2_YUV:
			this->convertImageToYUV();
			break;

		case CONVERT2_RGBA:
			this->convertImageToRGBA();
			break;

		case RESIZE_IMG:
			this->resizeImage( scaleX, scaleY, interType );
			break;

		case CHANGE_FORMAT:
			/*this->changeFormat( &relativeImagePath, saveImgFormat );
			break;*/

		case NO_OP:
			break;

		default:
			string s("Bad Operation Specifier!");
			cout << s << endl;
			throw s;
			break;
	}

	cout << "Finished Method IVPM::processImage" << endl;

}


template <typename Matx3DataType> void IVPM<Matx3DataType>::Matx3ToImage( bool normalize ){

	cout << "Launched Method IVPM::Matx3ToImage" << endl;

	/*
	 * 3 channel Matx3 will be represented by default as a RGB image, 1 channel as GRAYSCALE
	 */

	if ( this->Matx3Pointer->data == NULL) {
		string s("Error IVPM Object has EMPTY Matx3");
		cout << s << endl;
		throw s;
	}

	//uchar* datacv;
	int totalElements = this->Matx3Pointer->rows * this->Matx3Pointer->columns;
	int depth = this->Matx3Pointer->depth;

	if( /*this->Matx3ImageType == this->IMAGE_RGB &&*/ depth == 3 ){

		/*
		 * one to one association between depth and color channel
		 * data[0] -> R - data[1] -> G, data[2] -> B
		 */

		//datacv = new uchar[totalElements * 3];
		if( this->cvImageData != NULL )
			delete[] this->cvImageData;

		this->cvImageData = new uchar[totalElements * 3];
		int z = 0;

		Matx3DataType* mapElementPointerArray[depth];  //may be dinamically allocated with new

		if (normalize) {
			Matx3DataType** minmaxsp = this->Matx3Pointer->minmaxs();

			for (int k = 0; k < depth; k++){
				mapElementPointerArray[k] = this->Matx3Pointer->data[k][0];
			}

			/*
			 * elements order in cv::Mat is the following: x0_y0_z0_x1_y1_z1_ ... _xn_yn_zn
			 * for each position all elements of all maps are printed
			 * this is the reason why ther is before the cicle on totalElements and inside it the one on depth
			 */

			for (int j = 0; j < totalElements; j++){

				for (int k = depth - 1; k >= 0; k--) {
					// data is normalized rounded and converted to unsigned char - might cause a loss of precision
					//datacv[z++] = (uchar) max(	min((int) round(((*mapElementPointerArray[k] - minmaxsp[k][0]) / minmaxsp[k][1]) * 255.0), 255), 0 );
					this->cvImageData[z++] = (uchar) max(	min((int) round(((*mapElementPointerArray[k] - minmaxsp[k][0]) / minmaxsp[k][1]) * 255.0), 255), 0 );
				}

				for (int k = 0; k < depth; k++)
					mapElementPointerArray[k]++;
			}

			for (int k = 0; k < depth; k++) {
				delete[] minmaxsp[k];
			}

			delete[] minmaxsp;

		} else {

			for (int j = 0; j < totalElements; j++){

				for (int k = depth - 1; k >= 0; k--) {
					// data is rounded and converted to unsigned char - might cause a loss of precision
					//datacv[z++] = (uchar) max( min((int) round(*mapElementPointerArray[k] * 255.0), 255), 0);
					this->cvImageData[z++] = (uchar) max( min((int) round(*mapElementPointerArray[k] * 255.0), 255), 0);
				}

				for (int k = 0; k < depth; k++)
					mapElementPointerArray[k]++;
			}
		}

		/*
		 * type of a cv::Mat can be any of a long list of predefined types of the form:
		 * CV_<bit_depth>(S|U|F)C<number_of_channels>.
		 *
		 * S-> signed (char, int, long int depends on bit_depth)
		 * U->unsigned (char, int, long int depends on bit_depth)
		 * F->floating point (float, double, long double depends on bit_depth)
		 *
		 */

		//this->cvImage = cv::Mat(this->Matx3Pointer->rows, this->Matx3Pointer->columns, CV_8UC3, datacv, 0);
		this->cvImage = cv::Mat(this->Matx3Pointer->rows, this->Matx3Pointer->columns, CV_8UC3, this->cvImageData, 0);
		this->cvImageImageType = this->IMAGE_RGB;


		/*
		 * delete[] datacv;		<----CAN NOT DELETE DATACV OTHERWHISE cv::Mat WILL LOSE ITS DATA.
		 * datacv will be automatically deleted (freed) by IVPM destructor when it calls cv::Mat default destructor
		 */

	} else if ( /*this->Matx3ImageType == this->IMAGE_GRAY &&*/ depth == 1 ){

		/*
		 *  map is copied to a one channel grayscale image (just intensity of pixels)
		 */

		//datacv = new uchar[totalElements];
		this->cvImageData = new uchar[totalElements];
		Matx3DataType *mapElementPointer;

		mapElementPointer = this->Matx3Pointer->data[0][0];

		int z = 0;
		if (normalize) {

			Matx3DataType** minmaxp = this->Matx3Pointer->minmaxs();
			for (int j = 0; j < totalElements; j++) {
				//datacv[z++] = (uchar) max( min((int) round(((*mapElementPointer - minmaxp[0][0]) / minmaxp[0][1]) * 255.0), 255), 0);
				this->cvImageData[z++] = (uchar) max( min((int) round(((*mapElementPointer - minmaxp[0][0]) / minmaxp[0][1]) * 255.0), 255), 0);
				mapElementPointer++;
			}

			delete[] minmaxp;

		} else {
			for (int j = 0; j < totalElements; j++) {
				//datacv[z++] = (uchar) max( min((int) round(*mapElementPointer * 255.0), 255), 0);
				this->cvImageData[z++] = (uchar) max( min((int) round(*mapElementPointer * 255.0), 255), 0);
				mapElementPointer++;
			}
		}

		//this->cvImage = cv::Mat(this->Matx3Pointer->rows, this->Matx3Pointer->columns, CV_8UC1, datacv, 0);
		this->cvImage = cv::Mat(this->Matx3Pointer->rows, this->Matx3Pointer->columns, CV_8UC1, this->cvImageData, 0);
		this->cvImageImageType = this->IMAGE_GRAY;

		/*
		 * delete[] datacv;		<----CAN NOT DELETE DATACV OTHERWHISE cv::Mat WILL LOSE ITS DATA.
		 * datacv will be automatically deleted (freed) by IVPM destructor when it calls cv::Mat default destructor
		 */
	} else {

		/*
		 * one to one association between depth and cv::Mat channel?
		 * but how to keep track of image type?
		 * how to handle images with 4 channels representation CMYK RGBA
		 * how to handle 3 channel YUV
		 */

		string s("-->more than 3 channels in Matx3! must still understand how to manage this conversion case, and its use! <--");
		cout << endl << s << endl;
		throw s;

	}

	cout << "Finished Method IVPM::Matx3ToImage" << endl;

}


template <typename Matx3DataType> void IVPM<Matx3DataType>::ImageToMatx3(){

	cout << "Launched Method IVPM::ImageToMatx3" << endl;

	if( this->cvImage.data == NULL ){
		string s("Error IVPM Object has EMPTY image");
		cout << s << endl;
		throw s;
	}

	/*
	 * clear old data contained in Matx3 or create a new Matx3
	 */
	if( this->Matx3Pointer == NULL )
		this->Matx3Pointer = new TemplateMatx3<Matx3DataType>();
	else
		this->Matx3Pointer->clear_data();

	this->Matx3Pointer->rows = this->cvImage.size().height;
	this->Matx3Pointer->columns = this->cvImage.size().width;
	int stepw, steph, offset;
	int totalElements = this->Matx3Pointer->rows * this->Matx3Pointer->columns;

	if( this->cvImage.channels() == 4 ){

		this->Matx3Pointer->depth = 4;
		stepw = this->Matx3Pointer->columns * 4;
		steph = 4; // number of channels

	} else if( this->cvImage.channels() == 3 ){

		this->Matx3Pointer->depth = 3;
		stepw = this->Matx3Pointer->columns * 3;
		steph = 3; // number of channels

	} else if( this->cvImage.channels() == 1 ){

		this->Matx3Pointer->depth = 1;
		stepw = this->Matx3Pointer->columns * 1;
		steph = 1; // number of channels

	}else{
		string s("Error in number of channels of Image");
		cout << s << endl;
		throw s;
	}

	offset = steph - 1;

	int depth = this->Matx3Pointer->depth;
	int rows = this->Matx3Pointer->rows;
	int columns = this->Matx3Pointer->columns;

	this->Matx3Pointer->data = new Matx3DataType**[depth];	//array of pointers to 2D maps
	Matx3DataType* map;

	for (int k = 0; k < depth; k++) {

		map = new Matx3DataType[totalElements];	//each map is allocated in a contiguous way

		this->Matx3Pointer->data[k] = new Matx3DataType*[rows];	//array of pointers to rows of a single 2Dmap

		for (int i = 0; i < rows; i++) {
			this->Matx3Pointer->data[k][i] = map + ( i * columns );
		}

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				*map = (Matx3DataType) this->cvImage.data[i * stepw + j * steph + offset - k] / 255.0;
				map++;
			}
		}
	}

	this->Matx3Pointer->set_description( "" );
	this->Matx3ImageType = this->cvImageImageType;

	cout << "Finished Method IVPM::ImageToMatx3" << endl;
}

/********************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************
 *
 *	IMAGE PROCESSING METHODS
 *
 ********************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************/


template <typename Matx3DataType> void IVPM<Matx3DataType>::showImage( const string& windowName, bool temp ){

	cout << "Launched Method IVPM::showImage" << endl;

	if ( this->cvImage.data == NULL ) {
		string s("Error IVPM Object has EMPTY cvImage");
		cout << s << endl;
		throw s;
	}

	cvStartWindowThread();

	/*
	 * CV_WINDOW_NORMAL		-> the user can resize the window
	 * CV_WINDOW_AUTOSIZE	-> the user cannot resize the window
	 */
	cv::namedWindow( windowName, CV_WINDOW_NORMAL );

	cv::imshow( windowName, this->cvImage );

	/*
	 * checks if window is still there (or has been closed with mouse)
	 */
	while( cvGetWindowHandle( windowName.c_str() ) ) {
		/*
		 * checks if escaper key is pressed
		 */
		if( cvWaitKey(100) == 27 ) break;
	}

	if( temp == true)
		cv::destroyWindow( windowName );

	cout << "Finished Method IVPM::showImage" << endl;

}


template <typename Matx3DataType> void IVPM<Matx3DataType>::convertImageToGray(){

	cout << "Launched Method IVPM::convertImageToGray" << endl;

	if( this->cvImage.data == NULL ){
		string s("Error IVPM Object has EMPTY image");
		cout << s << endl;
		throw s;
	}

	switch( this->cvImageImageType ){

		case IMAGE_GRAY:
			cout << "Image already in GrayScale representation" << endl;
			break;

		case IMAGE_YUV:
			//converts to RGB and goes in following case
			this->convertImageToRGB();

		case IMAGE_RGB:
			cv::cvtColor( this->cvImage, this->cvImage, CV_RGB2GRAY );
			this->cvImageImageType = this->IMAGE_GRAY;
			break;

		case IMAGE_RGBA:
			cv::cvtColor( this->cvImage, this->cvImage, CV_RGBA2GRAY );
			this->cvImageImageType = this->IMAGE_GRAY;
			break;

		default:
			string s("Invalid color space conversion request");
			cout << s << endl;
			throw s;
			break;
	}

	cout << "Finished Method IVPM::convertImageToGray" << endl;
}


template <typename Matx3DataType> void IVPM<Matx3DataType>::convertImageToRGB(){

	cout << "Launched Method IVPM::convertImageToRGB" << endl;

	if( this->cvImage.data == NULL ){
		string s("Error IVPM Object has EMPTY image");
		cout << s << endl;
		throw s;
	}

	switch( this->cvImageImageType ){

		case IMAGE_RGB:
			cout << "Image already in RGB representation" << endl;
			break;

		case IMAGE_YUV:
			cv::cvtColor( this->cvImage, this->cvImage, CV_YUV2RGB );
			this->cvImageImageType = this->IMAGE_RGB;
			break;

		case IMAGE_GRAY:
			cv::cvtColor( this->cvImage, this->cvImage, CV_GRAY2RGB );
			this->cvImageImageType = this->IMAGE_RGB;
			break;

		case IMAGE_RGBA:
			cv::cvtColor( this->cvImage, this->cvImage, CV_RGBA2RGB );
			this->cvImageImageType = this->IMAGE_RGB;
			break;

		default:
			string s("Invalid color space conversion request");
			cout << s << endl;
			throw s;
			break;
	}

	cout << "Finished Method IVPM::convertImageToRGB" << endl;
}


template <typename Matx3DataType> void IVPM<Matx3DataType>::convertImageToYUV(){

	cout << "Launched Method IVPM::convertImageToYUV" << endl;

	if( this->cvImage.data == NULL ){
		string s("Error IVPM Object has EMPTY image");
		cout << s << endl;
		throw s;
	}

	switch( this->cvImageImageType ){

		case IMAGE_YUV:
			cout << "Image already in YUV representation" << endl;
			break;

		case IMAGE_GRAY:
			//goes in following case

		case IMAGE_RGBA:
			//converts to RGB and goes in following case
			this->convertImageToRGB();

		case IMAGE_RGB:
			cv::cvtColor( this->cvImage, this->cvImage, CV_RGB2YUV );
			this->cvImageImageType = this->IMAGE_YUV;
			break;

		default:
			string s("Invalid color space conversion request");
			cout << s << endl;
			throw s;
			break;
	}

	cout << "Finished Method IVPM::convertImageToYUV" << endl;
}


template <typename Matx3DataType> void IVPM<Matx3DataType>::convertImageToRGBA(){

	cout << "Launched Method IVPM::convertImageToRGBA" << endl;

	if( this->cvImage.data == NULL ){
		string s("Error IVPM Object has EMPTY image");
		cout << s << endl;
		throw s;
	}

	switch( this->cvImageImageType ){

		case IMAGE_RGBA:
			cout << "Image already in RGBA representation" << endl;
			break;

		case IMAGE_GRAY:
			//goes in following case

		case IMAGE_YUV:
			//converts to RGB and goes in following case
			this->convertImageToRGB();

		case IMAGE_RGB:
			cv::cvtColor( this->cvImage, this->cvImage, CV_RGB2RGBA );
			this->cvImageImageType = this->IMAGE_RGBA;
			break;

		default:
			string s("Invalid color space conversion request");
			cout << s << endl;
			throw s;
			break;
	}

	cout << "Finished Method IVPM::convertImageToRGBA" << endl;
}


template <typename Matx3DataType> void IVPM<Matx3DataType>::resizeImage( float scaleX, float scaleY, interpolationType interType ){

	cout << "Launched Method IVPM::resizeImage" << endl;

	switch( interType ){

		case LINEAR:
			cv::resize( this->cvImage, this->cvImage, cv::Size(), scaleX, scaleY, CV_INTER_LINEAR );
			break;

		case CUBIC:
			cv::resize( this->cvImage, this->cvImage, cv::Size(), scaleX, scaleY, CV_INTER_CUBIC );
			break;

		case NEAREST_NEIGHBOR:
			cv::resize( this->cvImage, this->cvImage, cv::Size(), scaleX, scaleY, CV_INTER_NN );
			break;

		default:
			string s("Invalid resize interpolation parameter");
			cout << s << endl;
			throw s;
			break;
	}


	cout << "Finished Method IVPM::resizeImage" << endl;
}

template <typename Matx3DataType> void IVPM<Matx3DataType>::changeFormat( string* imagePath, saveImageFormat saveImgFormat ){

	cout << "Launched Method IVPM::changeFormat" << endl;

	size_t lastPointPosition;

	lastPointPosition = imagePath->rfind(".");
	if( lastPointPosition == string::npos ){

		string s("Can't find extension in image path -> " + *imagePath );
		cout << s << endl;
		throw s;

	}else{

		switch( saveImgFormat ){

			case JPEG: 	imagePath->replace( lastPointPosition, imagePath->length() - lastPointPosition, ".JPEG" );
						break;

			case BMP: 	imagePath->replace( lastPointPosition, imagePath->length() - lastPointPosition, ".BMP" );
						break;

			/*case PNG:	imagePath->replace( lastPointPosition, imagePath->length() - lastPointPosition, ".PNG" );
						break;*/

			default:	string s("Can't save image -> image format not recognized.");
						cout << s << endl;
						throw s;
						break;

		}
	}

	cout << "Finished Method IVPM::changeFormat" << endl;
}
