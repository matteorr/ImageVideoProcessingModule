/*
 * TemplateMatx3.h
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

#ifndef TEMPLATEMATX3_H_
#define TEMPLATEMATX3_H_

//#include "defs.h"
#include <string>
#include <iostream>
#include <vector>

//typedef enum {TEXT, BIN, IMAGE_RGB, IMAGE_GRAY} FileType;

template <typename Matx3DataType> class TemplateMatx3
{

	public:
		/*! Post processing operations to be performed after a random initialization of the data. */
		enum PostGenerationOpt{NONE, MAP_ZEROMEAN, MAP_UNITSTD, MAP_ZEROMEAN_UNITSTD, PP_UNITNORM, PP_PROB};
		/*! Types of file to be loaded/saved. */
		enum FileType{TEXT, BIN, IMAGE_RGB, IMAGE_GRAY};

		/*! Default constructor. It generates and empty data structure. */
		TemplateMatx3();
		/*! Basic constructor. It allocates the data structure without any specific initialization. */
		TemplateMatx3(int depth, int rows, int columns, const std::string& description = "");
		/*! It allocates the data structure initialized to a specific value. */
		TemplateMatx3(int depth, int rows, int columns, Matx3DataType value, const std::string& description = "");
		/*! It allocates the data structure, randomly initialized within a given range of values, and with an eventual post-processing. */
		TemplateMatx3(int depth, int rows, int columns, Matx3DataType rand_low, Matx3DataType rand_high, PostGenerationOpt opt = NONE, const std::string& description = "");
		/*! It allocates the data structure, randomly initialized from values belonging to a given discrete set, and with an eventual post-processing. */
		TemplateMatx3(int depth, int rows, int columns, const std::vector<Matx3DataType>& rand_value_domain, PostGenerationOpt opt = NONE, const std::string& description = "");
		/*! From a mono-dimensional array (with n elements) to a Matx3 object (the data are copied). */
		TemplateMatx3(Matx3DataType* array, int n, const std::string& description = "", bool copy = true);
		/*! From a two-dimensional array (with n rows, m columns) to a Matx3 object (the data are copied). */
		TemplateMatx3(Matx3DataType* array, int n, int m, const std::string& description = "", bool copy = true);
		/*! From a two-dimensional matrix dynamically allocated (with n rows, m columns) to a Matx3 object (the data are copied).
		 *  In case data are not copied two-dimensional matrix must be allocated contiguously
		 */
		TemplateMatx3(Matx3DataType** matrix2d, int n, int m, const std::string& description = "", bool copy = true);
		/*! Copy-based-constructor. */
		TemplateMatx3(const TemplateMatx3<Matx3DataType>& copy_me);
		/*! Copy-based-constructor. */
		TemplateMatx3(const TemplateMatx3<Matx3DataType>* copy_me);
		/*! This constructor initializes the data from a given file (binary, text, image). */
		TemplateMatx3(const std::string& filename, FileType type);
		/*! Guess what it does. */
		virtual ~TemplateMatx3();

		/*! The 3d data matrix, row-major, [depth][row][column]. */
		Matx3DataType*** data;
		/* Number of 2d maps. */
		int depth;
		/*! Number of rows. */
		int rows;
		/*! Number of columns. */
		int columns;
		/* Description of the data (null-terminated string, it can be NULL). */
		char* desc;
		/* Size (in megabytes) of the current object */
		double size;

		/*! It returns a pointer to the a given 2-dimensional map (matrix). */
		Matx3DataType** get_map(int d);
		/*! It returns a pointer to the 3-dimensional data matrix. */
		Matx3DataType*** get_maps();
		/*! It adds another map to the current object.
		 *  Added map must be allocated with contiguous memory
		 */
		void add_map(Matx3DataType** matrix2d, int r, int c, bool copy = true);
		/*! It adds the maps of the input object to the current one. They must be of the same size. */
		void merge_with(TemplateMatx3<Matx3DataType>* m, bool copy = true);
		/*! Compare the current object with another one by element-by-element differences. */
		bool compare_with(TemplateMatx3<Matx3DataType>* m, double max_tol_diff = 1e-8);
		/*! Compare the current object with another one by element-by-element differences, returning the absolute valued differences. */
		TemplateMatx3<Matx3DataType>* difference_from(TemplateMatx3<Matx3DataType>* m);
		/*! It sets the description field of the current Matx3 object. */
		void set_description(const std::string& description);
		/*! It purges the description field of the current Matx3 object (NULL). */
		void clear_description();
		/*! It kills the stored numerical data from memory. */
		void clear_data();


		/*! It sets all the data to zero. */
		void set_to_zero();
		/*! It sets all the data to a given value. */
		void set_to_value(Matx3DataType v);


		/*! A random initialization of the data matrix (from a given range). */
		void set_to_random(Matx3DataType low = 0.0, Matx3DataType high = 1.0, PostGenerationOpt opt = NONE);
		/*! A random initialization of the data matrix (from a given discrete domain). */
		void set_to_random(const std::vector<Matx3DataType>& rand_value_domain, PostGenerationOpt opt = NONE);

		/*! What about the introduction of a probabilistic normalization among data on the same row, column of different maps? */
		void softmax(bool avoid_overflow = true);
		/*!
		 * It computes the probabilistic normalization of the current maps (as the method above),
		 * but the result is stored on a destination Matx3 object of the same size, that must be manually preallocated.
		 * If dest = NULL, then the allocation is automatically performed. A pointer to the result is returned.
		 */
		TemplateMatx3<Matx3DataType>* softmax(TemplateMatx3<Matx3DataType>* dest, bool avoid_overflow = true);


		/*! It sets the mean of each map to zero. */
		void zeromean();
		/*! It sets the standard deviation of each map to one. */
		void unitstd();
		/*! It sets the mean of each map to zero and the standard deviation to one. */
		void zeromean_unitstd();
		/*! Set to unit norm (along the maps). */
		void unitnorm();
		/*! Absolute value (element-wise). */
		void abs();
		/*! Array of mean values for each map */
		double* means();
		/*! Array of mean values for each map (allocated by the user) */
		void means(double *m);
		/*! Array of min and max values for each map */
		Matx3DataType** minmaxs();

		/*! Point-wise sum with another Matx3 */
		void sum_pointwise(TemplateMatx3<Matx3DataType>* other);
		/*! Point-wise difference with another Matx3 */
		void diff_pointwise(TemplateMatx3<Matx3DataType>* other);
		/*! Point-by-point product with another Matx3 */
		void prod_pointwise(TemplateMatx3<Matx3DataType>* other);

		/*! Save the 3d data to a binary or text file, using an internal (simple) format. */
		void save(const std::string& filename, FileType type);
		/*! This method loads the 3d data from a binary or text file, using the internal format enforced by 'save'. */
		void load(const std::string& filename, FileType type);
		/*!
		 * It loads the 3d data from an input image. If considering color images,
		 * then 3 maps are generate (R-G-B), otherwise only 1 map is create (gray scale).
		 * WARNING: this method is based on external libraries.
		 */
		void load_from_image(const std::string& filename, FileType type);
		/*!
		 * It saves the 3d data to a given image file. The option for generating a color image
		 * is only valid when the data is composed of 3 maps, converted to the R-G-B channels of
		 * the image itself. Otherwise, the suffix '_1', '_2', ... is added to the supplied
		 * filename, and each image is a gray scale instance of a single map. If normalize = true,
		 * then the input data is scaled into [0,1].
		 * WARNING: this method is based on external libraries.
		 */
		void save_to_images(const std::string& filename, FileType type, bool normalize = true);


		/*! This methods prints all the numeric data contents on the screen. */
		void print_maps();
		/*! It prints the data of a selected map. */
		void print_map(int d);
		/*! It prints the description of the current Matx3 object. */
		void print_description();
		/*! It prints the depth x rows x columns info. */
		void print_size();
		/*! This method prints the description and all the data of the current Matx3 object. */
		void print();


		/*!
		 * Compute mean, standard deviation, minimum, max values of all the data, and
		 * store them (with a description) on a row-major 1-dim array.
		 */
		TemplateMatx3<Matx3DataType> stats();
		/*!
		 * Compute mean, standard deviation, minimum, max values of a selected map, and
		 * store them (with a description) on a row-major 1-dim array.
		 */
		TemplateMatx3<Matx3DataType> stats_map(int d);
		/*!
		 * Compute mean, standard deviation, minimum, max values of each map, and
		 * store them (with a description) on a set of row-major 1-dim arrays.
		 */
		TemplateMatx3<Matx3DataType> stats_maps();
		/*!
		 * Compute mean, standard deviation, minimum, max values of each (row,column)
		 * coordinates along the depth of the 3d data, and store them (with
		 * a description) on an analogous 3d structure with 4 map (the first map are the means,
		 * the second one are the standard deviations, the third one are the minimum
		 * values, the fourth one are the maximum value).
		 */
		TemplateMatx3<Matx3DataType> stats_pp();


		/*
		 * copy method - ADDED BY MATTEO for the sake of copying a Matx3 into another without needing constructor
		 */
		void copy_me( const TemplateMatx3<Matx3DataType>* copy_me );


	private:
		/*
		 * Computes mean of map at given depth d
		 */
		double mean(int d);
		/*
		 * Computes standard deviation of map at given depth d and mean of map in input
		 */
		double std(int d, double mean_d);
		/*
		 * Computes an array containing standard deviation of all maps given in input an array with the means of all maps
		 */
		double* stds(const double* means);
		/*
		 * Computes complessive standard deviation with respect to all maps given the complessive mean of values in all maps
		 */
		double std(double mean_all);
		/*
		 * Returns a 2 element array of Matx3DataType containing minimum and maximum value of the map at given depth
		 */
		Matx3DataType* minmax(int d);
		/*! Guess what it does. */
		double get_size_in_megabytes();

};

#endif /* TEMPLATEMATX3_H_ */
