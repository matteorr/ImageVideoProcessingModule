/*
 * IVPM.h
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

#ifndef IVPM_H_
#define IVPM_H_

//other classes
#include "TemplateMatx3.h"
#include "TemplateMatx3.cpp"

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

template <typename Matx3DataType> class IVPM {

private:

	TemplateMatx3<Matx3DataType>* Matx3Pointer;
	int Matx3ImageType;

	cv::Mat cvImage;
	uchar* cvImageData;
	int cvImageImageType;

	cv::VideoCapture cvVideo;

public:

	/*
	 * for saving images
	 */
	enum saveImageFormat{ JPEG, BMP/*, PNG*/ };
	/*
	 * for loading images
	 */
	enum loadImageType{ IMAGE_RGB, IMAGE_GRAY, IMAGE_YUV, IMAGE_RGBA, IMAGE_UNCHANGED };
	/*
	 * possible image processing operations
	 */
	enum imageOperationType{ SHOW_IMG, CONVERT2_RGB, CONVERT2_GRAYSCALE, CONVERT2_YUV, CONVERT2_RGBA, RESIZE_IMG, CHANGE_FORMAT, NO_OP };
	/*
	 * type of interpolation in resizing
	 */
	enum interpolationType{ LINEAR, CUBIC, NEAREST_NEIGHBOR };
	/*
	 * matx3 file type
	 */
	enum matx3FileType{ TEXT, BIN };


	/*
	 *
	 * CONSTRUCTOR - DESTRUCTOR
	 *
	 */

	IVPM();

	virtual ~IVPM();

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

	void loadImageAndProcessAndSave(
			const string& inputDirPath, const string& relativeImagePath, const string& saveDirPath, loadImageType loadImgType = IMAGE_UNCHANGED, saveImageFormat saveImgFormat = JPEG,
			imageOperationType opType = NO_OP, float scaleX = 1.0, float scaleY = 1.0, interpolationType interType = NEAREST_NEIGHBOR
	);

	TemplateMatx3<Matx3DataType>* loadImageAndProcessAndReturn(
			const string& inputDirPath, const string& relativeImagePath, loadImageType loadImgType = IMAGE_UNCHANGED,
			imageOperationType opType = NO_OP, float scaleX = 1.0, float scaleY = 1.0, interpolationType interType = NEAREST_NEIGHBOR
	);

	void loadImagesFromFileAndProcessAndSave(
			const string& inputDirPath, const string& filePath, const string& saveDirPath, loadImageType loadImgType = IMAGE_UNCHANGED, saveImageFormat saveImgFormat = JPEG,
			imageOperationType opType = NO_OP, float scaleX = 1.0, float scaleY = 1.0, interpolationType interType = NEAREST_NEIGHBOR
	);

	vector< TemplateMatx3<Matx3DataType>* >* loadImagesFromFileAndProcessAndReturn(
			const string& inputDirPath, const string& filePath, loadImageType loadImgType = IMAGE_UNCHANGED,
			imageOperationType opType = NO_OP, float scaleX = 1.0, float scaleY = 1.0, interpolationType interType = NEAREST_NEIGHBOR
	);

	void loadVideoAndProcessAndSave(
			const string& videoPath, const string& saveDirPath, saveImageFormat saveImgFormat,
			float startTimeStamp, float endTimeStamp, float frameRate = 0, bool showFrames = false,
			imageOperationType opType = NO_OP, float scaleX = 1.0, float scaleY = 1.0, interpolationType interType = NEAREST_NEIGHBOR
	);

	vector< TemplateMatx3<Matx3DataType>* >* loadVideoAndProcessAndReturn(
			const string& videoPath,
			float startTimeStamp, float endTimeStamp, float frameRate = 0, bool showFrames = false,
			imageOperationType opType = NO_OP, float scaleX = 1.0, float scaleY = 1.0, interpolationType interType = NEAREST_NEIGHBOR
	);

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

	void convertMatx3AndProcessAndSave(
			TemplateMatx3<Matx3DataType>* inputMatx3, const string& relativeImagePath, const string& saveDirPath, saveImageFormat saveImgFormat = JPEG,
			imageOperationType opType = NO_OP, float scaleX = 1.0, float scaleY = 1.0, interpolationType interType = NEAREST_NEIGHBOR
	);

	TemplateMatx3<Matx3DataType>* convertMatx3AndProcessAndReturn(
			TemplateMatx3<Matx3DataType>* inputMatx3,
			imageOperationType opType = NO_OP, float scaleX = 1.0, float scaleY = 1.0, interpolationType interType = NEAREST_NEIGHBOR
	);

	void convertMatx3VectorAndProcessAndSave(
			vector< TemplateMatx3<Matx3DataType>* >* inputVectorPointer, const string& saveDirPath, saveImageFormat saveImgFormat = JPEG,
			imageOperationType opType = NO_OP, float scaleX = 1.0, float scaleY = 1.0, interpolationType interType = NEAREST_NEIGHBOR
	);

	vector< TemplateMatx3<Matx3DataType>* >* convertMatx3VectorAndProcessAndReturn(
			vector< TemplateMatx3<Matx3DataType>* >* inputVectorPointer,
			imageOperationType opType = NO_OP, float scaleX = 1.0, float scaleY = 1.0, interpolationType interType = NEAREST_NEIGHBOR
	);

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

	void loadMatx3AndProcessAndSave(
			const string& inputDirPath, const string& matx3Path, matx3FileType matx3Type, const string& saveDirPath, saveImageFormat saveImgFormat = JPEG,
			imageOperationType opType = NO_OP, float scaleX = 1.0, float scaleY = 1.0, interpolationType interType = NEAREST_NEIGHBOR
	);

	TemplateMatx3<Matx3DataType>* loadMatx3AndProcessAndReturn(
			const string& inputDirPath, const string& matx3Path, matx3FileType matx3Type,
			imageOperationType opType = NO_OP, float scaleX = 1.0, float scaleY = 1.0, interpolationType interType = NEAREST_NEIGHBOR
	);

	void loadMatx3FromFileAndProcessAndSave(
			const string& inputDirPath, const string& filePath, matx3FileType matx3Type, const string& saveDirPath, saveImageFormat saveImgFormat = JPEG,
			imageOperationType opType = NO_OP, float scaleX = 1.0, float scaleY = 1.0, interpolationType interType = NEAREST_NEIGHBOR
	);

	vector< TemplateMatx3<Matx3DataType>* >* loadMatx3FromFileAndProcessAndReturn(
			const string& inputDirPath, const string& filePath, matx3FileType matx3Type,
			imageOperationType opType = NO_OP, float scaleX = 1.0, float scaleY = 1.0, interpolationType interType = NEAREST_NEIGHBOR
	);

private:

	/********************************************************************************************************************************************************************************
	 ********************************************************************************************************************************************************************************
	 *
	 *	UTILITY METHODS
	 *
	 ********************************************************************************************************************************************************************************
	 ********************************************************************************************************************************************************************************/

	/*
	 * loads an image from specified path and represents it as an OpenCV matrix (of specified type)
	 */
	void loadImage( const string& absoluteImagePath, loadImageType loadImgType );

	/*
	 * saves image (represented as a OpenCV matrix) with the desired name and format at a fixed output path
	 */
	void saveImage( const string& absoluteImagePath, saveImageFormat saveImgFormat = JPEG, int JPEG_QUALITY = 95, int PNG_COMPRESSION = 3, int PXM_BINARY = 1 );

	/*
	 * processes member class cv::Mat image as indicated by enumerative type specified in input
	 */
	void processImage(
			imageOperationType opType,
			const string& relativeImagePath,
			float scaleX = 1.0, float scaleY = 1.0, interpolationType interType = NEAREST_NEIGHBOR
	);

	/*
	 * converts private Matx3 to OpenCV matrix format
	 */
	void Matx3ToImage( bool normalize = true );

	/*
	 * converts private OpenCV matrix to Matx3 format
	 */
	void ImageToMatx3();

	/********************************************************************************************************************************************************************************
	 ********************************************************************************************************************************************************************************
	 *
	 *	IMAGE PROCESSING METHODS
	 *
	 ********************************************************************************************************************************************************************************
	 ********************************************************************************************************************************************************************************/

	/*
	 * creates a window and shows IVPM private image (represented as a OpenCV matrix) in it
	 */
	void showImage(const string& windowName, bool temp = false );

	/*
	 * converts private OpenCV matrix from current color space to GrayScale
	 */
	void convertImageToGray();

	/*
	 * converts private OpenCV matrix from current color space to RGB
	 */
	void convertImageToRGB();

	/*
	 * converts private OpenCV matrix from current color space to YUV
	 */
	void convertImageToYUV();

	/*
	 * converts private OpenCV matrix from current color space to RGBA
	 */
	void convertImageToRGBA();

	/*
	 * resizes image changing its dimensions by factor scaleX on X direction and scaleY factor on Y direction and with specified interpolation
	 */
	void resizeImage( float scaleX, float scaleY, interpolationType interType );

	/*
	 * change image format from the one loaded to the one specified in input
	 */
	void changeFormat( string* imagePath, saveImageFormat saveImgFormat );


};

#endif /* IVPM_H_ */

