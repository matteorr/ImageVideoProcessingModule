/*
 * main.cpp
 *
 *  Created on: Feb 15, 2012
 *      Author: matteo
 */

#include "IVPM.h"
#include "IVPM.cpp"

#include <iostream>
#include <string>
#include <type_traits>
#include <climits>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>


using namespace std;

int main(int argc, char** argv) {

	IVPM<double> ivpmDouble;

	/*
	 * test of SHOW_IMG
	 *
	ivpmDouble.loadImageAndProcessAndSave(
			"/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/imagesDir/", "totti.jpg", "/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/saveDir/",
			ivpmDouble.IMAGE_UNCHANGED, ivpmDouble.JPEG, ivpmDouble.SHOW_IMG
	);
	*/

	/*
	 * test of CHANGE_FORMAT
	 *
	ivpmDouble.loadImageAndProcessAndSave(
			"/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/imagesDir/", "totti.BMP", "/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/saveDir/",
			ivpmDouble.IMAGE_UNCHANGED, ivpmDouble.JPEG, ivpmDouble.CHANGE_FORMAT
	);
	*/

	/*
	 * test of RESIZE_IMG
	 *
	ivpmDouble.loadImageAndProcessAndSave(
			"/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/imagesDir/", "totti.BMP", "/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/saveDir/",
			ivpmDouble.IMAGE_UNCHANGED, ivpmDouble.JPEG, ivpmDouble.RESIZE_IMG, 0.5, 1.0
	);
	*/

	/*
	 * test of CONVERT2_RGB - CONVERT2_YUV - CONVERT2_GRAYSCALE - CONVERT2_RGBA
	 *
	ivpmDouble.loadImageAndProcessAndSave(
			"/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/imagesDir/", "totti.BMP", "/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/saveDir/",
			ivpmDouble.IMAGE_UNCHANGED, ivpmDouble.JPEG, ivpmDouble.CONVERT2_YUV
	);

	ivpmDouble.loadImageAndProcessAndSave(
			"/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/imagesDir/", "totti.BMP", "/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/saveDir/",
			ivpmDouble.IMAGE_UNCHANGED, ivpmDouble.JPEG, ivpmDouble.CONVERT2_GRAYSCALE
	);

	ivpmDouble.loadImageAndProcessAndSave(
			"/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/imagesDir/", "albero.JPEG", "/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/saveDir/",
			ivpmDouble.IMAGE_UNCHANGED, ivpmDouble.JPEG, ivpmDouble.CONVERT2_RGB
	);

	ivpmDouble.loadImageAndProcessAndSave(
			"/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/imagesDir/", "albero.JPEG", "/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/saveDir/",
			ivpmDouble.IMAGE_UNCHANGED, ivpmDouble.JPEG, ivpmDouble.CONVERT2_RGBA
	);
	*/

	/*
	 * test of process video (insert wanted frame rate or 0 for video's frame rate)
	 */
	ivpmDouble.loadVideoAndProcessAndSave(
			"/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/LIMITLESS.mp4", "/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/saveDir/", ivpmDouble.JPEG,
			500, 2750, 50, true, ivpmDouble.CONVERT2_YUV
	);


	/*
	 * test of loading images from file
	 *
	ivpmDouble.loadImagesFromFileAndProcessAndSave(
			"/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/imagesDir/", "/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/imageList",
			"/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/saveDir/", ivpmDouble.IMAGE_UNCHANGED,
			ivpmDouble.JPEG, ivpmDouble.RESIZE_IMG, 3.0, 3.0
	);
	*/

	/*
	 * test of loading images from file and return Matx3
	 *
	vector< TemplateMatx3<double>* >* matx3VectorPointer;

	matx3VectorPointer = ivpmDouble.loadImagesFromFileAndProcessAndReturn(
			"/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/imagesDir/", "/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/imageList",
			ivpmDouble.IMAGE_RGB,
			ivpmDouble.RESIZE_IMG, 0.2, 0.2
	);

	vector< TemplateMatx3<double>* >::iterator it;
int i = 0;
	for( it = matx3VectorPointer->begin(); it != matx3VectorPointer->end(); it++ ){
i++;
		(*it)->print_size();
		(*it)->save("/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/imagesDir/immagine_TEXT_exp_" + to_string(i),(*it)->TEXT );
		cout << endl << i << endl;
		delete (*it);
	}

	delete matx3VectorPointer;

*/
	/*
	 * test of load video and return Matx3
	 *
	vector< TemplateMatx3<double>* >* matx3VectorPointer;

	matx3VectorPointer = ivpmDouble.loadVideoAndProcessAndReturn("/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/LIMITLESS.mp4", 134500,147680, 2, true,
			ivpmDouble.CONVERT2_YUV
	);

	vector< TemplateMatx3<double>* >::iterator it;
	int i = 0;
	for( it = matx3VectorPointer->begin(); it != matx3VectorPointer->end(); it++ ){
		i++;
		(*it)->print_size();
		(*it)->save_to_images("/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/saveDir/converted_from_video_" + to_string(i) + ".jpeg",(*it)->IMAGE_RGB);
	}

	delete matx3VectorPointer;
	*/

	/*
	 * test of loading matx3 from file and return Matx3
	 *
	vector< TemplateMatx3<double>* >* matx3VectorPointer;

	matx3VectorPointer = ivpmDouble.loadMatx3FromFileAndProcessAndReturn(
			"/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/imagesDir/", "/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/imageList",
			ivpmDouble.TEXT,
			ivpmDouble.SHOW_IMG
	);

	vector< TemplateMatx3<double>* >::iterator it;
	int i =0;
	for( it = matx3VectorPointer->begin(); it != matx3VectorPointer->end(); it++ ){
		i++;
		(*it)->print_size();
		(*it)->save_to_images("/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/saveDir/immagine_caricata_" + to_string(i) + ".JPEG",(*it)->IMAGE_RGB, true );
	}

	delete matx3VectorPointer;
	*/

	/*
	 * test of loading matx3 from file and save
	 *

	ivpmDouble.loadMatx3FromFileAndProcessAndSave(
			"/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/imagesDir/", "/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/imageList", ivpmDouble.BIN,
			"/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/saveDir/", ivpmDouble.BMP,
			ivpmDouble.RESIZE_IMG, 0.1, 0.1
	);
	*/

	/*
	 * convert Matx3 Vector and Process and Return
	 *
	vector< TemplateMatx3<double>* >* matx3InputVectorPointer;

	matx3InputVectorPointer = ivpmDouble.loadMatx3FromFileAndProcessAndReturn(
			"/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/imagesDir/", "/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/imageList",
			ivpmDouble.BIN,
			ivpmDouble.SHOW_IMG
	);

	vector< TemplateMatx3<double>* >* matx3OutputVectorPointer;

	matx3OutputVectorPointer = ivpmDouble.convertMatx3VectorAndProcessAndReturn(matx3InputVectorPointer, ivpmDouble.RESIZE_IMG,2.0,2.0 );

	vector< TemplateMatx3<double>* >::iterator it;
	int i =0;
	for( it = matx3OutputVectorPointer->begin(); it != matx3OutputVectorPointer->end(); it++ ){
		i++;
		(*it)->print_size();
		(*it)->save_to_images("/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/saveDir/immagine_caricata_e ridimensionata" + to_string(i) + ".JPEG",(*it)->IMAGE_RGB, true );
	}

	delete matx3InputVectorPointer;
	delete matx3OutputVectorPointer;
	*/


	/*
	 * convert Matx3 Vector and Process and Save
	 *
	vector< TemplateMatx3<double>* >* matx3InputVectorPointer;

	matx3InputVectorPointer = ivpmDouble.loadMatx3FromFileAndProcessAndReturn(
			"/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/imagesDir/", "/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/Matx3List",
			ivpmDouble.TEXT,
			ivpmDouble.SHOW_IMG
	);

	cout <<endl << "DEBUG---------------------> size of vector(" << matx3InputVectorPointer->size() << ")" << endl;
	int i = 0;
	vector< TemplateMatx3<double>* >::iterator it;
	for( it = matx3InputVectorPointer->begin(); it != matx3InputVectorPointer->end(); it++ ){
		cout << endl <<i << "------------------------------------------------------------" << endl;
		(*it)->print_size();
		cout << endl << "------------------------------------------------------------" << endl;
		i++;
	}

	ivpmDouble.convertMatx3VectorAndProcessAndSave(
			matx3InputVectorPointer,
			"/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/saveDir/", ivpmDouble.BMP,
			ivpmDouble.RESIZE_IMG,2.0,5.0
	);
	cout <<endl << "DEBUG---------------------> size of vector(" << matx3InputVectorPointer->size() << ")" << endl;
//	vector< TemplateMatx3<double>* >::iterator it;
	int j =0;
	for( it = matx3InputVectorPointer->begin(); it != matx3InputVectorPointer->end(); it++ ){
		j++;
		(*it)->print_size();
		(*it)->save_to_images("/home/matteo/Documents/Università/Tesi Magistrale/IVPM/workFolder/saveDir/immagine_caricata_" + to_string(j) + ".JPEG",(*it)->IMAGE_RGB, true );
		delete (*it);
	}

	delete matx3InputVectorPointer;
*/
	cout << endl << "END" << endl;


}

