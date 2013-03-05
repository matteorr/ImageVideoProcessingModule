/*
 * TemplateMatx3.cpp
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

#include "TemplateMatx3.h"

#include <fstream>
#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <string>
#include <cmath>


template <typename Matx3DataType> TemplateMatx3<Matx3DataType>::TemplateMatx3()
		: data(NULL), depth(0), rows(0), columns(0), desc(NULL)
{
}


template <typename Matx3DataType> TemplateMatx3<Matx3DataType>::TemplateMatx3(int depth, int rows, int columns, const std::string& description)
		: depth(depth), rows(rows), columns(columns)
{
	//allocates contiguous memory for maps

	int totalElements = rows * columns;

	this->data = new Matx3DataType**[depth];	//array of pointers to 2D maps
	Matx3DataType* map;

	for (int k = 0; k < depth; k++) {

		map = new Matx3DataType[totalElements];	//each map is allocated in a contiguous way

		this->data[k] = new Matx3DataType*[rows];	//array of pointers to rows of a single 2Dmap

		for (int i = 0; i < rows; i++) {
			this->data[k][i] = map + ( i * columns );
		}
	}

	this->desc = NULL;

	set_description(description);
	this->size = get_size_in_megabytes();
}


template <typename Matx3DataType> TemplateMatx3<Matx3DataType>::TemplateMatx3(int depth, int rows, int columns, Matx3DataType value, const std::string& description)
		: depth(depth), rows(rows), columns(columns)
{
	//allocates contiguous memory for maps

	int totalElements = rows * columns;

	this->data = new Matx3DataType**[depth];	//array of pointers to 2D maps
	Matx3DataType* map;

	for (int k = 0; k < depth; k++) {

		map = new Matx3DataType[totalElements];	//each map is allocated in a contiguous way

		this->data[k] = new Matx3DataType*[rows];	//array of pointers to rows of a single 2Dmap

		for (int i = 0; i < rows; i++) {
			this->data[k][i] = map + ( i * columns );
		}

		for( int j = 0; j < totalElements; j++ ){

			*map = value;
			map++;
		}
	}

	this->desc = NULL;

	set_description(description);
	this->size = get_size_in_megabytes();
}


template <typename Matx3DataType> TemplateMatx3<Matx3DataType>::TemplateMatx3(int depth, int rows, int columns, Matx3DataType rand_low, Matx3DataType rand_high, PostGenerationOpt opt, const std::string& description)
		: depth(depth), rows(rows), columns(columns)
{
	//allocates memory for a map in a contiguous fashion

	int totalElements = rows * columns;

	double normalizer = double(RAND_MAX);										//CHECK IF DOUBLE IS CORRECT!	<--------------------------------------
	double m, s;
	Matx3DataType** z;
	Matx3DataType* zMapElementPointer;

	this->data = new Matx3DataType**[depth];	//array of pointers to 2D maps
	Matx3DataType* map;

	switch (opt) {
	case NONE:

		for (int k = 0; k < depth; k++) {

			map = new Matx3DataType[totalElements];	//each map is allocated in a contiguous way

			this->data[k] = new Matx3DataType*[rows];	//array of pointers to rows of a single 2Dmap

			for (int i = 0; i < rows; i++) {
				this->data[k][i] = map + ( i * columns );
			}

			for( int j = 0; j < totalElements; j++ ){
				*map = rand_low
						+ ((rand_high - rand_low) * (double) rand()) / normalizer;
				map++;
			}
		}

		break;

	case MAP_ZEROMEAN:

		for (int k = 0; k < depth; k++) {
			m = 0;
			map = new Matx3DataType[totalElements];	//each map is allocated in a contiguous way

			this->data[k] = new Matx3DataType*[rows];	//array of pointers to rows of a single 2Dmap

			for (int i = 0; i < rows; i++) {
				this->data[k][i] = map + ( i * columns );
			}

			for( int j = 0; j < totalElements; j++ ){
				*map = rand_low
						+ ((rand_high - rand_low) * (double) rand()) / normalizer;
				m += *map;
				map++;
			}

			m /= totalElements;
			for (int j = 0; j < totalElements; j++){
				map--;
				*map -= m;
			}
		}

		break;

	case MAP_UNITSTD:

		for (int k = 0; k < depth; k++) {
			m = 0;
			map = new Matx3DataType[totalElements];	//each map is allocated in a contiguous way

			this->data[k] = new Matx3DataType*[rows];	//array of pointers to rows of a single 2Dmap

			for (int i = 0; i < rows; i++) {
				this->data[k][i] = map + ( i * columns );
			}

			for( int j = 0; j < totalElements; j++ ){
				*map = rand_low
						+ ((rand_high - rand_low) * (double) rand()) / normalizer;
				m += *map;
				map++;
			}

			m /= totalElements;
			s = this->std(k, m);
			for (int j = 0; j < totalElements; j++){
				map--;
				*map /= s;
			}
		}

		break;

	case MAP_ZEROMEAN_UNITSTD:

		for (int k = 0; k < depth; k++) {
			m = 0;
			map = new Matx3DataType[totalElements];	//each map is allocated in a contiguous way

			this->data[k] = new Matx3DataType*[rows];	//array of pointers to rows of a single 2Dmap

			for (int i = 0; i < rows; i++) {
				this->data[k][i] = map + ( i * columns );
			}

			for( int j = 0; j < totalElements; j++ ){
				*map = rand_low
						+ ((rand_high - rand_low) * (double) rand()) / normalizer;
				m += *map;
				map++;
			}

			m /= totalElements;
			s = this->std(k, m);
			for (int j = 0; j < totalElements; j++){
				map--;
				*map -= m;
				*map /= s;
			}
		}

		break;

	case PP_UNITNORM:

		TemplateMatx3<Matx3DataType>* n;
		n = new TemplateMatx3<Matx3DataType>(1, rows, columns, 0.0);
		z = n->get_map(0);

		for (int k = 0; k < depth; k++) {

			map = new Matx3DataType[totalElements];	//each map is allocated in a contiguous way

			this->data[k] = new Matx3DataType*[rows];	//array of pointers to rows of a single 2Dmap

			for (int i = 0; i < rows; i++) {
				this->data[k][i] = map + ( i * columns );
			}

			zMapElementPointer = z[0];

			for (int j = 0; j < totalElements; j++) {
				*map = rand_low
						+ ((rand_high - rand_low) * (double) rand()) / normalizer;
				*zMapElementPointer += (*map) * (*map);
				map++;
				zMapElementPointer++;
			}
		}

		for (int k = 0; k < depth; k++) {

			map = this->data[k][0];
			zMapElementPointer = z[0];

			for (int j = 0; j < totalElements; j++) {
				*map /= sqrt(*zMapElementPointer);
				map++;
				zMapElementPointer++;
			}
		}
		delete n;
		break;

	case PP_PROB:

		n = new TemplateMatx3<Matx3DataType>(1, rows, columns, 0.0);
		z = n->get_map(0);

		for (int k = 0; k < depth; k++) {

			map = new Matx3DataType[totalElements];	//each map is allocated in a contiguous way

			this->data[k] = new Matx3DataType*[rows];	//array of pointers to rows of a single 2Dmap

			for (int i = 0; i < rows; i++) {
				this->data[k][i] = map + ( i * columns );
			}

			zMapElementPointer = z[0];

			for (int j = 0; j < totalElements; j++) {
				*map = exp(
						rand_low
								+ ((rand_high - rand_low) * rand())
										/ normalizer);
				*zMapElementPointer += *map;
				map++;
				zMapElementPointer++;
			}
		}

		for (int k = 0; k < depth; k++) {

			map = this->data[k][0];
			zMapElementPointer = z[0];

			for (int j = 0; j < totalElements; j++) {
				*map /= *zMapElementPointer;
				map++;
				zMapElementPointer++;
			}
		}
		delete n;
		break;

	}

	this->desc = NULL;

	set_description(description);
	this->size = get_size_in_megabytes();
}


template <typename Matx3DataType> TemplateMatx3<Matx3DataType>::TemplateMatx3(int depth, int rows, int columns, const std::vector<Matx3DataType>& rand_value_domain, PostGenerationOpt opt, const std::string & description)
		: depth(depth), rows(rows), columns(columns)
{
	//allocates memory for a map in a contiguous fashion

	int totalElements = rows * columns;

	double normalizer = double(RAND_MAX);										//CHECK IF DOUBLE IS CORRECT!	<--------------------------------------
	double rand_low = 0;
	double rand_high = rand_value_domain.size() - 1;
	Matx3DataType** z;
	Matx3DataType* zMapElementPointer;

	double m, s;

	this->data = new Matx3DataType**[depth];	//array of pointers to 2D maps
	Matx3DataType* map;

	switch (opt) {
	case NONE:

		for (int k = 0; k < depth; k++) {

			map = new Matx3DataType[totalElements];	//each map is allocated in a contiguous way

			this->data[k] = new Matx3DataType*[rows];	//array of pointers to rows of a single 2Dmap

			for (int i = 0; i < rows; i++) {
				this->data[k][i] = map + ( i * columns );
			}

			for( int j = 0; j < totalElements; j++ ){
				*map = rand_value_domain.at(
						(int) round(
								rand_low
										+ ((rand_high - rand_low) * (double) rand())
												/ normalizer));
				map++;
			}
		}

		break;

	case MAP_ZEROMEAN:

		for (int k = 0; k < depth; k++) {
			m = 0;
			map = new Matx3DataType[totalElements];	//each map is allocated in a contiguous way

			this->data[k] = new Matx3DataType*[rows];	//array of pointers to rows of a single 2Dmap

			for (int i = 0; i < rows; i++) {
				this->data[k][i] = map + ( i * columns );
			}

			for( int j = 0; j < totalElements; j++ ){
				*map = rand_value_domain.at(
						(int) round(
								rand_low
										+ ((rand_high - rand_low) * (double) rand())
												/ normalizer));
				m += *map;
				map++;
			}

			m /= totalElements;
			for (int j = 0; j < totalElements; j++){
				map--;
				*map -= m;
			}
		}

		break;

	case MAP_UNITSTD:

		for (int k = 0; k < depth; k++) {
			m = 0;
			map = new Matx3DataType[totalElements];	//each map is allocated in a contiguous way

			this->data[k] = new Matx3DataType*[rows];	//array of pointers to rows of a single 2Dmap

			for (int i = 0; i < rows; i++) {
				this->data[k][i] = map + ( i * columns );
			}

			for( int j = 0; j < totalElements; j++ ){
				*map = rand_value_domain.at(
						(int) round(
								rand_low
										+ ((rand_high - rand_low) * (double) rand())
												/ normalizer));
				m += *map;
				map++;
			}

			m /= totalElements;
			s = this->std(k, m);
			for (int j = 0; j < totalElements; j++){
				map--;
				*map /= s;
			}
		}

		break;

	case MAP_ZEROMEAN_UNITSTD:

		for (int k = 0; k < depth; k++) {
			m = 0;
			map = new Matx3DataType[totalElements];	//each map is allocated in a contiguous way

			this->data[k] = new Matx3DataType*[rows];	//array of pointers to rows of a single 2Dmap

			for (int i = 0; i < rows; i++) {
				this->data[k][i] = map + ( i * columns );
			}

			for( int j = 0; j < totalElements; j++ ){
				*map = rand_value_domain.at(
						(int) round(
								rand_low
										+ ((rand_high - rand_low) * (double) rand())
												/ normalizer));
				m += *map;
				map++;
			}

			m /= totalElements;
			s = this->std(k, m);
			for (int j = 0; j < totalElements; j++){
				map--;
				*map -= m;
				*map /= s;
			}
		}

		break;

	case PP_UNITNORM:

		TemplateMatx3<Matx3DataType>* n;
		n = new TemplateMatx3<Matx3DataType>(1, rows, columns, 0.0);
		z = n->get_map(0);

		for (int k = 0; k < depth; k++) {

			map = new Matx3DataType[totalElements];	//each map is allocated in a contiguous way

			this->data[k] = new Matx3DataType*[rows];	//array of pointers to rows of a single 2Dmap

			for (int i = 0; i < rows; i++) {
				this->data[k][i] = map + ( i * columns );
			}

			zMapElementPointer = z[0];

			for (int j = 0; j < totalElements; j++) {
				*map = rand_value_domain.at(
						(int) round(
								rand_low
										+ ((rand_high - rand_low) * (double) rand())
												/ normalizer));
				*zMapElementPointer += (*map) * (*map);
				map++;
				zMapElementPointer++;
			}
		}

		for (int k = 0; k < depth; k++) {

			map = this->data[k][0];
			zMapElementPointer = z[0];

			for (int j = 0; j < totalElements; j++) {
				*map /= sqrt(*zMapElementPointer);
				map++;
				zMapElementPointer++;
			}
		}
		delete n;
		break;

	case PP_PROB:

		n = new TemplateMatx3<Matx3DataType>(1, rows, columns, 0.0);
		z = n->get_map(0);

		for (int k = 0; k < depth; k++) {

			map = new Matx3DataType[totalElements];	//each map is allocated in a contiguous way

			this->data[k] = new Matx3DataType*[rows];	//array of pointers to rows of a single 2Dmap

			for (int i = 0; i < rows; i++) {
				this->data[k][i] = map + ( i * columns );
			}

			zMapElementPointer = z[0];

			for (int j = 0; j < totalElements; j++) {
				*map = exp(
						rand_value_domain.at(
								(int) round(
										rand_low
												+ ((rand_high - rand_low)
														* (double) rand())
														/ normalizer)));
				*zMapElementPointer += *map;
				map++;
				zMapElementPointer++;
			}
		}

		for (int k = 0; k < depth; k++) {

			map = this->data[k][0];
			zMapElementPointer = z[0];

			for (int j = 0; j < totalElements; j++) {
				*map /= *zMapElementPointer;
				map++;
				zMapElementPointer++;
			}
		}
		delete n;
		break;

	}

	this->desc = NULL;

	set_description(description);
	this->size = get_size_in_megabytes();
}


template <typename Matx3DataType> TemplateMatx3<Matx3DataType>::TemplateMatx3(Matx3DataType *array, int n, const std::string& description, bool copy)
{
	//allocates contiguous memory for maps

	depth = 1;
	rows = 1;
	columns = n;

	this->data = new Matx3DataType **[depth];					//array of pointers to 2D maps
	Matx3DataType* map = new Matx3DataType[columns];	//map is allocated in a contiguous way
	this->data[0] = new Matx3DataType*[rows];					//array of pointers to rows of a single 2Dmap

	if (copy) {
		this->data[0][0] = map;

		for (int j = 0; j < columns; j++){
			*map = array[j];
			map++;
		}
	} else {
		this->data[0][0] = array;
	}

	this->desc = NULL;

	set_description(description);
	this->size = get_size_in_megabytes();
}


template <typename Matx3DataType> TemplateMatx3<Matx3DataType>::TemplateMatx3(Matx3DataType* array, int n, int m, const std::string& description, bool copy)
{
	//allocates contiguous memory for maps

	depth = 1;
	rows = n;
	columns = m;
	int totalElements = rows * columns;

	this->data = new Matx3DataType **[depth];					//array of pointers to 2D maps
	Matx3DataType* map = new Matx3DataType[totalElements];	//map is allocated in a contiguous way
	this->data[0] = new Matx3DataType*[rows];					//array of pointers to rows of a single 2Dmap

	if (copy) {
		for (int i = 0; i < rows; i++) {
					this->data[0][i] = map + ( i * columns );
		}
		for (int j = 0; j < totalElements; j++){
			 *map = array[j];
			 map++;
		}
	} else {
		for (int i = 0; i < rows; i++) {
			this->data[0][i] = array + (i * m);
		}
	}

	this->desc = NULL;

	set_description(description);
	this->size = get_size_in_megabytes();
}


template <typename Matx3DataType> TemplateMatx3<Matx3DataType>::TemplateMatx3(Matx3DataType** matrix2d, int n, int m, const std::string& description, bool copy)
{
	//allocates contiguous memory for maps

	depth = 1;
	rows = n;
	columns = m;
	int totalElements = rows * columns;

	this->data = new Matx3DataType **[depth];					//array of pointers to 2D maps

	if (copy) {

		Matx3DataType* map = new Matx3DataType[totalElements];	//map is allocated in a contiguous way
		this->data[0] = new Matx3DataType*[rows];					//array of pointers to rows of a single 2Dmap

		for (int i = 0; i < rows; i++) {
			this->data[0][i] = map + ( i * columns );
		}

		for (int i = 0; i < rows; i++){
			for (int j = 0; j < columns; j++){
				*map = matrix2d[i][j];
				map++;
			}
		}
	} else {
		this->data[0] = matrix2d;
	}

	this->desc = NULL;

	set_description(description);
	this->size = get_size_in_megabytes();
}


template <typename Matx3DataType> TemplateMatx3<Matx3DataType>::TemplateMatx3(const TemplateMatx3<Matx3DataType>& copy_me)
{
	//allocates contiguous memory for maps

	depth = copy_me.depth;
	rows = copy_me.rows;
	columns = copy_me.columns;
	int totalElements = rows * columns;

	this->data = new Matx3DataType **[depth];					//array of pointers to 2D maps

	Matx3DataType* copy_meElementPointer;
	Matx3DataType* map;

	for (int k = 0; k < depth; k++) {

		map = new Matx3DataType[totalElements];	//each map is allocated in a contiguous way
		this->data[k] = new Matx3DataType*[rows];	//array of pointers to rows of a single 2Dmap

		copy_meElementPointer = copy_me.data[k][0];

		for (int i = 0; i < rows; i++) {
			this->data[k][i] = map + ( i * columns );
		}

		for (int j = 0; j < totalElements; j++){
			*map = *copy_meElementPointer;
			map++;
			copy_meElementPointer++;
		}
	}

	if (copy_me.desc != NULL) {
		this->desc = new char[strlen(copy_me.desc) + 1];
		strcpy(this->desc, copy_me.desc);
		this->desc[strlen(copy_me.desc)] = '\0';
	} else
		this->desc = NULL;

	this->size = copy_me.size;
}


template <typename Matx3DataType> TemplateMatx3<Matx3DataType>::TemplateMatx3(const TemplateMatx3<Matx3DataType> *copy_me)
{
	//allocates contiguous memory for maps

	depth = copy_me->depth;
	rows = copy_me->rows;
	columns = copy_me->columns;
	int totalElements = rows * columns;

	this->data = new Matx3DataType **[depth];					//array of pointers to 2D maps
	Matx3DataType* map;
	Matx3DataType* copy_meElementPointer;

	for (int k = 0; k < depth; k++) {

		map = new Matx3DataType[totalElements];	//each map is allocated in a contiguous way
		this->data[k] = new Matx3DataType*[rows];	//array of pointers to rows of a single 2Dmap

		copy_meElementPointer = copy_me->data[k][0];

		for (int i = 0; i < rows; i++) {
			this->data[k][i] = map + ( i * columns );
		}

		for (int j = 0; j < totalElements; j++){
			*map = *copy_meElementPointer;
			map++;
			copy_meElementPointer++;
		}
	}

	if (copy_me->desc != NULL) {
		this->desc = new char[strlen(copy_me->desc) + 1];
		strcpy(this->desc, copy_me->desc);
		this->desc[strlen(copy_me->desc)] = '\0';
	} else
		this->desc = NULL;

	this->size = copy_me->size;
}


template <typename Matx3DataType> TemplateMatx3<Matx3DataType>::TemplateMatx3(const std::string& filename, FileType type)
{
	this->data = NULL;
	this->desc = NULL;
	this->rows = 0;
	this->columns = 0;
	this->depth = 0;

	switch (type) {
	case TEXT:
		this->load(filename, TEXT);
		break;
	case BIN:
		this->load(filename, BIN);
		break;
	case IMAGE_RGB:
		this->load_from_image(filename, IMAGE_RGB);
		break;
	case IMAGE_GRAY:
		this->load_from_image(filename, IMAGE_GRAY);
		break;
	}

	this->size = get_size_in_megabytes();
}


template <typename Matx3DataType> TemplateMatx3<Matx3DataType>::~TemplateMatx3()
{
	//deallocates memory of maps knowing it's contiguous

	if (this->data != NULL)
	{
		int totalElements = rows * columns;

		for (int k = 0; k < depth; k++)
		{

			if (totalElements > 1)
				delete[] this->data[k][0];
			else
				delete this->data[k][0];

			if (rows > 1)
				delete[] this->data[k];
			else if (this->data[k])
				delete this->data[k];

		}

		if (depth > 1)
			delete[] this->data;
		else
			delete this->data;
	}

	if (this->desc != NULL)
		delete this->desc;

}


template <typename Matx3DataType> Matx3DataType **TemplateMatx3<Matx3DataType>::get_map(int d)
{
	if (d < 0 || d >= depth) {
		std::string s("Invalid access to maps.");
		std::cout << s << std::endl;
		throw s;
	}
	return this->data[d];
}


template <typename Matx3DataType> Matx3DataType ***TemplateMatx3<Matx3DataType>::get_maps()
{
	return this->data;
}


template <typename Matx3DataType> void TemplateMatx3<Matx3DataType>::add_map(Matx3DataType** matrix2d, int r, int c, bool copy)
{
	//uses contiguous allocation to add a single map to actual data

	if (r != this->rows && c != this->columns) {
		std::string s("Invalid candidate map size.");
		std::cout << s << std::endl;
		throw s;
	}

	int totalElements = r * c;

	if (copy) {

		Matx3DataType* matrix2dElementPointer = matrix2d[0];

		Matx3DataType** newMatrix2d = new Matx3DataType*[r];
		Matx3DataType* newMatrix2dDataPointer = new Matx3DataType[totalElements];			// new map is allocated in a contiguous way

		for (int i = 0; i < r; i++){
			newMatrix2d[i] = newMatrix2dDataPointer + (i * c);
		}

		for (int j = 0; j < totalElements; j++) {
			*newMatrix2dDataPointer = *matrix2dElementPointer;
			matrix2dElementPointer++;
			newMatrix2dDataPointer++;
		}


		matrix2d = newMatrix2d;
	}

	Matx3DataType*** newdata = new Matx3DataType**[this->depth + 1];

	for (int k = 0; k < this->depth; k++) {
		newdata[k] = this->data[k];
	}
	newdata[this->depth] = matrix2d;
	this->depth++;

	delete[] this->data;

	this->data = newdata;
}


template <typename Matx3DataType> void TemplateMatx3<Matx3DataType>::merge_with(TemplateMatx3<Matx3DataType> *m, bool copy)
{
	for (int k = 0; k < m->depth; k++) {
		add_map(m->get_map(k), m->rows, m->columns, copy);
	}
}


template <typename Matx3DataType> bool TemplateMatx3<Matx3DataType>::compare_with(TemplateMatx3<Matx3DataType> *m, double max_tol_diff)
{
	//uses contiguous allocation to compare maps of two Matx3

	if (m->depth != this->depth && m->rows != this->rows && m->columns != this->columns) {
		std::string s("Invalid map size, unable to compare.");
		std::cout << s << std::endl;
		throw s;
	}

	int totalElements = rows * columns;
	double max_diff = 0;

	Matx3DataType* mElementPointer;
	Matx3DataType* mapElementPointer;

	for (int k = 0; k < depth; k++) {

		mElementPointer = m->data[k][0];
		mapElementPointer = this->data[k][0];

		for (int j = 0; j < totalElements; j++) {
			max_diff = std::max( max_diff,
					(double) std::abs(*mapElementPointer - *mElementPointer) );
			mapElementPointer++;
			mElementPointer++;
		}
	}

	return max_diff <= max_tol_diff;
}


template <typename Matx3DataType> TemplateMatx3<Matx3DataType>* TemplateMatx3<Matx3DataType>::difference_from(TemplateMatx3<Matx3DataType> *m)
{
	//uses contiguous allocation to make differrence of two Matx3

	if (m->depth != this->depth && m->rows != this->rows && m->columns != this->columns) {
		std::string s("Invalid map size, unable to compare.");
		std::cout << s << std::endl;
		throw s;
	}

	int totalElements = rows * columns;

	TemplateMatx3<Matx3DataType>* diffs = new TemplateMatx3<Matx3DataType>(m->depth, m->rows, m->columns);

	Matx3DataType* mElementPointer;
	Matx3DataType* mapElementPointer;
	Matx3DataType* diffsElementPointer;

	for (int k = 0; k < depth; k++) {

		mElementPointer = m->data[k][0];
		mapElementPointer = this->data[k][0];
		diffsElementPointer = diffs->data[k][0];

		for (int j = 0; j < totalElements; j++) {

			*diffsElementPointer = (Matx3DataType) fabs(*mapElementPointer - *mElementPointer);
			mElementPointer++;
			mapElementPointer++;
			diffsElementPointer++;
		}
	}

	return diffs;
}


template <typename Matx3DataType> void TemplateMatx3<Matx3DataType>::set_description(const std::string& description)
{
	int cur_len = 0;

	if (this->desc != NULL)
		cur_len = strlen(this->desc) + 1; // +1 is due to the \0

	if (description.length() > 0) {
		this->desc = new char[description.length() + 1];
		strcpy(this->desc, description.c_str());
		this->size -= cur_len;
		this->size += description.length() + 1;
	} else {
		this->desc = NULL;
		this->size -= cur_len;
	}
}


template <typename Matx3DataType> void TemplateMatx3<Matx3DataType>::clear_description()
{
	if (this->desc != NULL) {
		delete[] this->desc;
		this->desc = NULL;
	}
}


template <typename Matx3DataType> void TemplateMatx3<Matx3DataType>::clear_data()
{
	//uses contiguous allocation to clear all data

	if (this->data != NULL) {

		int totalElements = rows * columns;

		for (int k = 0; k < depth; k++)
		{

			if (totalElements > 1)
				delete[] this->data[k][0];
			else
				delete this->data[k][0];

			if (rows > 1)
				delete[] this->data[k];
			else if (this->data[k])
				delete this->data[k];

		}

		if (depth > 1)
			delete[] this->data;
		else
			delete this->data;

		this->columns = 0;
		this->rows = 0;
		this->depth = 0;

		this->size = this->get_size_in_megabytes();
	}
}


template <typename Matx3DataType> void TemplateMatx3<Matx3DataType>::set_to_zero()
{
	//uses contiguous allocation to set to zero each map elements

	int totalElements = rows * columns;
	Matx3DataType* mapElementPointer;

	for( int k = 0; k < depth; k++ ){

		mapElementPointer = this->data[k][0];

		for( int j = 0; j < totalElements; j++ ){
			*mapElementPointer = (Matx3DataType) 0.0;
			mapElementPointer++;
		}
	}
}


template <typename Matx3DataType> void TemplateMatx3<Matx3DataType>::set_to_value(Matx3DataType v)
{
	//uses contiguous allocation to set to value all elements of every map
	int totalElements = rows * columns;
	Matx3DataType* mapElementPointer;

	for( int k = 0; k < depth; k++ ){

		mapElementPointer = this->data[k][0];

		for( int j = 0; j < totalElements; j++ ){
			*mapElementPointer = v;
			mapElementPointer++;
		}
	}
}


template <typename Matx3DataType> void TemplateMatx3<Matx3DataType>::set_to_random(Matx3DataType rand_low, Matx3DataType rand_high, PostGenerationOpt opt)
{
	//uses contiguous allocation to set randomly all elements of every map

	int totalElements = rows * columns;

	double normalizer = double(RAND_MAX);						//CHECK IF DOUBLE IS CORRECT!	<--------------------------------------
	double m, s;												//CHECK IF DOUBLE IS CORRECT!	<--------------------------------------

	Matx3DataType** z;
	Matx3DataType* zMapElementPointer;
	Matx3DataType* mapElementPointer;

	switch (opt) {

	case NONE:

		for (int k = 0; k < depth; k++) {

			mapElementPointer = this->data[k][0];

			for (int j = 0; j < totalElements; j++) {
				*mapElementPointer = rand_low
						+ ((rand_high - rand_low) * (double) rand()) / normalizer;
				mapElementPointer++;
			}
		}
		break;

	case MAP_ZEROMEAN:

		for (int k = 0; k < depth; k++) {

			m = 0;
			mapElementPointer = this->data[k][0];

			for (int j = 0; j < totalElements; j++) {
				*mapElementPointer = rand_low
					+ ((rand_high - rand_low) * (double) rand()) / normalizer;
				m += *mapElementPointer;
				mapElementPointer++;
			}

			m /= totalElements;

			for (int j = 0; j < totalElements; j++) {				//note that assignment goes from last element of map to first (mapElementPointer--)
				mapElementPointer--;
				*mapElementPointer -= m;
			}
		}
		break;

	case MAP_UNITSTD:

		for (int k = 0; k < depth; k++) {

			m = 0;
			mapElementPointer = this->data[k][0];

			for (int j = 0; j < totalElements; j++) {
				*mapElementPointer = rand_low
						+ ((rand_high - rand_low) * (double) rand()) / normalizer;
				m += *mapElementPointer;
				mapElementPointer++;
			}

			m /= totalElements;
			s = this->std(k, m);

			for (int j = 0; j < totalElements; j++) {				//note that assignment goes from last element of map to first (mapElementPointer--)
				mapElementPointer--;
				*mapElementPointer /= s;
			}
		}
		break;

	case MAP_ZEROMEAN_UNITSTD:

		for (int k = 0; k < depth; k++) {

			m = 0;
			mapElementPointer = this->data[k][0];

			for (int j = 0; j < totalElements; j++) {
				*mapElementPointer = rand_low
						+ ((rand_high - rand_low) * (double) rand()) / normalizer;
				m += *mapElementPointer;
				mapElementPointer++;
			}

			m /= totalElements;
			s = this->std(k, m);

			for (int j = 0; j < totalElements; j++) {				//note that assignment goes from last element of map to first (mapElementPointer--)
				mapElementPointer--;
				*mapElementPointer -= m;
				*mapElementPointer /= s;
			}
		}
		break;

	case PP_UNITNORM:

		TemplateMatx3<Matx3DataType>* n;
		n = new TemplateMatx3<Matx3DataType>(1, rows, columns, 0.0);
		z = n->get_map(0);

		for (int k = 0; k < depth; k++) {

			mapElementPointer = this->data[k][0];
			zMapElementPointer = z[0];

			for (int j = 0; j < totalElements; j++) {
				*mapElementPointer = rand_low
						+ ((rand_high - rand_low) * (double) rand()) / normalizer;
				*zMapElementPointer += (*mapElementPointer) * (*mapElementPointer);
				mapElementPointer++;
				zMapElementPointer++;
			}
		}

		for (int k = 0; k < depth; k++) {

			mapElementPointer = this->data[k][0];
			zMapElementPointer = z[0];

			for (int j = 0; j < totalElements; j++) {
				*mapElementPointer /= sqrt(*zMapElementPointer);
				mapElementPointer++;
				zMapElementPointer++;
			}
		}
		delete n;
		break;

	case PP_PROB:

		n = new TemplateMatx3<Matx3DataType>(1, rows, columns, 0.0);
		z = n->get_map(0);

		for (int k = 0; k < depth; k++) {

			mapElementPointer = this->data[k][0];
			zMapElementPointer = z[0];

			for (int j = 0; j < totalElements; j++) {
				*mapElementPointer = exp(
						rand_low
								+ ((rand_high - rand_low) * (double) rand())
										/ normalizer);
				*zMapElementPointer += *mapElementPointer;
				mapElementPointer++;
				zMapElementPointer++;
			}
		}
		for (int k = 0; k < depth; k++) {

			mapElementPointer = this->data[k][0];
			zMapElementPointer = z[0];

			for (int j = 0; j < totalElements; j++) {
				*mapElementPointer /= *zMapElementPointer;
				mapElementPointer++;
				zMapElementPointer++;
			}
		}
		delete n;
		break;
	}

}


template <typename Matx3DataType> void TemplateMatx3<Matx3DataType>::set_to_random(const std::vector<Matx3DataType>& rand_value_domain, PostGenerationOpt opt)
{
	//uses contiguous allocation to set randomly each map elements

	int totalElements = rows * columns;

	double normalizer = double(RAND_MAX);						//<-----------------------CHECK IF DOUBLE IS CORRECT!!!!!!!!!!!
	double rand_low = 0;										//<-----------------------CHECK IF DOUBLE IS CORRECT!!!!!!!!!!!
	double rand_high = rand_value_domain.size() - 1;			//<-----------------------CHECK IF DOUBLE IS CORRECT!!!!!!!!!!!

	Matx3DataType* mapElementPointer;

	for (int k = 0; k < depth; k++) {

		mapElementPointer = this->data[k][0];
		for (int j = 0; j < totalElements; j++) {
				*mapElementPointer = rand_value_domain.at(
						(int) round(
								rand_low
										+ ((rand_high - rand_low) * (double) rand())
												/ normalizer));
				mapElementPointer++;
		}
	}
}


template <typename Matx3DataType> void TemplateMatx3<Matx3DataType>::softmax(bool avoid_overflow)
{
	softmax(this, avoid_overflow);
}


template <typename Matx3DataType> TemplateMatx3<Matx3DataType>* TemplateMatx3<Matx3DataType>::softmax(TemplateMatx3<Matx3DataType>* dest, bool avoid_overflow)
{
	//uses contiguous allocation of memory of maps

	if (dest != NULL) {

		if ((dest->rows != this->rows) || (dest->columns != this->columns)
			|| (dest->depth != this->depth)) {
				std::string s("Invalid source/dest sizes. They must match.");
				std::cout << s << std::endl;
				throw s;
		}
	} else {
		dest = new TemplateMatx3<Matx3DataType>(this->depth, this->rows, this->columns);
	}

	int totalElements = this->rows * this->columns;

	double z;													//<--------------------------------- CHECK IF DOUBLE IS CORRECT
	Matx3DataType*** src = this->get_maps();
	Matx3DataType*** dst = dest->get_maps();

	if (avoid_overflow) {

		Matx3DataType* mapElementPointerArray[this->depth];  //may be dinamically allocated with new
		Matx3DataType* dstElementPointerArray[dest->depth];  //may be dinamically allocated with new

		for (int k = 0; k < depth; k++){
			mapElementPointerArray[k] = src[k][0];
			dstElementPointerArray[k] = dst[k][0];					//<--------------------------------------CHECK IF CORRECTLY FILLS DESTINATION!!!!!!!!!!!!!!!!!!!!
		}

		Matx3DataType max;

		for (int j = 0; j < totalElements; j++) {

			max = *mapElementPointerArray[0];
			for (int k = 1; k < depth; k++) {
				max = std::max(*mapElementPointerArray[k], max);
			}

			z = exp(*mapElementPointerArray[0] - max);
			for (int k = 1; k < depth; k++) {
				z += exp(*mapElementPointerArray[k] - max);
			}

			for (int k = 0; k < depth; k++) {
				*dstElementPointerArray[k] = exp(*mapElementPointerArray[k] - max) / z;				//<---------------------------------------DOES AUTOMATIC CAST??
			}

			for (int k = 0; k < depth; k++){
				mapElementPointerArray[k]++;
				dstElementPointerArray[k]++;
			}
		}
	} else {

		Matx3DataType* mapElementPointerArray[this->depth];  //may be dinamically allocated with new
		Matx3DataType* dstElementPointerArray[dest->depth];  //may be dinamically allocated with new

		for (int k = 0; k < depth; k++){
			mapElementPointerArray[k] = src[k][0];
			dstElementPointerArray[k] = dst[k][0];					//<--------------------------------------CHECK IF CORRECTLY FILLS DESTINATION!!!!!!!!!!!!!!!!!!!!
		}

		for (int j = 0; j < totalElements; j++) {

			z = exp(*mapElementPointerArray[0]);
			for (int k = 1; k < depth; k++) {
				z += exp(*mapElementPointerArray[k]);
			}
			for (int k = 0; k < depth; k++) {
				*dstElementPointerArray[k] = exp(*mapElementPointerArray[k]) / z;					//<-----------------------------------------DOES AUTOMATIC CAST??
			}

			for (int k = 0; k < depth; k++){
				mapElementPointerArray[k]++;
				dstElementPointerArray[k]++;
			}
		}
	}

	return dest;
}


template <typename Matx3DataType> void TemplateMatx3<Matx3DataType>::zeromean()
{
	//uses contiguous allocation of memory of maps

	int totalElements = rows * columns;

	double* m = this->means();

	Matx3DataType* mapElementPointer;

	for (int k = 0; k < depth; k++) {

		mapElementPointer = this->data[k][0];

		for (int j = 0; j < totalElements; j++){
			*mapElementPointer -= m[k];
			mapElementPointer++;
		}

	}
	delete[] m;
}


template <typename Matx3DataType> void TemplateMatx3<Matx3DataType>::unitstd()
{
	//uses contiguous allocation of memory of maps

	int totalElements = rows * columns;

	double* m = this->means();
	double* s = this->stds(m);

	Matx3DataType* mapElementPointer;

	for (int k = 0; k < depth; k++) {

		mapElementPointer = this->data[k][0];
		for (int j = 0; j < totalElements; j++){
			*mapElementPointer /= s[k];
			mapElementPointer++;
		}
	}

	delete[] s;
	delete[] m;
}


template <typename Matx3DataType> void TemplateMatx3<Matx3DataType>::zeromean_unitstd()
{
	//uses contiguous allocation of memory of maps

	int totalElements = rows * columns;

	double* m = this->means();
	double* s = this->stds(m);

	Matx3DataType* mapElementPointer;

	for (int k = 0; k < depth; k++) {

		mapElementPointer = this->data[k][0];

		for (int j = 0; j < totalElements; j++) {
			*mapElementPointer -= m[k];
			*mapElementPointer /= s[k];
			mapElementPointer++;
		}

	}
	delete[] s;
	delete[] m;
}


template <typename Matx3DataType> void TemplateMatx3<Matx3DataType>::abs()
{
	//uses contiguous allocation of memory of maps

	int totalElements = rows * columns;

	Matx3DataType* mapElementPointer;

	for (int k = 0; k < depth; k++) {

		mapElementPointer = this->data[k][0];

		for (int j = 0; j < totalElements; j++) {
				*mapElementPointer = fabs( *mapElementPointer );
				mapElementPointer++;
		}
	}
}


template <typename Matx3DataType> void TemplateMatx3<Matx3DataType>::save(const std::string& filename, FileType type)
{
	//uses contiguous allocation to save data

	int totalElements = rows * columns;

	if (type != BIN && type != TEXT) {
		std::string s(
				"Invalid file type specified (only TEXT and BIN are allowed).");
		std::cout << s << std::endl;
		throw s;
	}

	if (type == BIN) {
		std::ofstream out(filename.c_str(), std::ios::out | std::ios::binary);
		if (!out) {
			std::string s("Cannot save to file: " + filename);
			std::cout << s << std::endl;
			throw s;
		}

		int desc_len = 0;
		if (this->desc != NULL) {
			desc_len = strlen(this->desc);
		}

		int header[] = { 1, depth, rows, columns, desc_len }; // header: format_version, depth, rows, columns, description_length

		out.write((char *) &header, 5 * sizeof(int));

		if (desc_len > 0) {
			out.write(this->desc, desc_len * sizeof(char));
		}

		for (int k = 0; k < depth; k++) {
			out.write((char *) data[k][0], totalElements * sizeof(Matx3DataType));			//correct?it's all contiguous
			if (out.bad()) {
				std::string s("Error while saving to file: " + filename);
				std::cout << s << std::endl;
				throw s;
			}
		}
		out.close();
	} else {
		std::ofstream out(filename.c_str(), std::ios::out);
		if (!out) {
			std::string s("Cannot save to file: " + filename);
			std::cout << s << std::endl;
			throw s;
		}

		out << "# Format:  1" << '\n';
		out << "# Depth:   " << depth << '\n';
		out << "# Rows:    " << rows << '\n';
		out << "# Columns: " << columns << '\n';
		out << "# Desc: " << (desc == NULL ? "" : desc) << '\n';
		out << "#" << '\n';

		Matx3DataType* mapElementPointer;

		for (int k = 0; k < depth; k++) {
			if (depth > 1)
				out << "# Map" << k << '\n';
			else
				out << "#" << '\n';

			mapElementPointer = this->data[k][0];

			for( int j = 0; j < totalElements; j++ ){

				if( (j + 1) % (columns) != 0 )
					out << *mapElementPointer << '\t';
				else
					out << *mapElementPointer << '\n';

				mapElementPointer++;
			}
			if (out.bad()) {
				std::string s("Error while saving to file: " + filename);
				std::cout << s << std::endl;
				throw s;
			}
		}
		out.close();
	}
}


template <typename Matx3DataType> void TemplateMatx3<Matx3DataType>::load(const std::string& filename, FileType type)
{
	if (type != BIN && type != TEXT) {
		std::string s(
				"Invalid file type specified (only TEXT and BIN are allowed).");
		std::cout << s << std::endl;
		throw s;
	}

	if (type == BIN) {
		std::ifstream in(filename.c_str(), std::ios::in | std::ios::binary);
		if (!in) {
			std::string s("Cannot open file: " + filename);
			std::cout << s << std::endl;
			throw s;
		}

		int header[5]; // header: format_version, depth, rows, columns, description_length
		in.read((char *) &header, 5 * sizeof(int));

		if (header[0] != 1) {
			std::string s("Invalid version of file: " + filename);
			std::cout << s << std::endl;
			throw s;
		}

		this->clear_data();

		this->depth = header[1];
		this->rows = header[2];
		this->columns = header[3];

		int totalElements = rows * columns;

		if (header[4] > 0) {
			int desc_len = header[4];
			this->desc = new char[desc_len + 1];
			in.read(this->desc, desc_len * sizeof(char));
			this->desc[desc_len] = '\0';
		}

		this->data = new Matx3DataType**[depth];	//array of pointers to 2D maps
		Matx3DataType* map;

		for (int k = 0; k < depth; k++) {

			this->data[k] = new Matx3DataType*[rows];	//array of pointers to rows of a single 2Dmap
			map = new Matx3DataType[totalElements];		//each map is allocated in a contiguous way

			for (int i = 0; i < rows; i++) {
				this->data[k][i] = map + ( i * columns );
			}
			in.read((char *) data[k][0], totalElements * sizeof(Matx3DataType));			//correct?it's all contiguous

			if (!in.good()) {
				std::string s("Error while loading file: " + filename);
				std::cout << s << std::endl;
				throw s;
			}
		}
		in.close();
	} else {
		std::ifstream in(filename.c_str(), std::ios::in);
		if (!in) {
			std::string s("Cannot open file: " + filename);
			std::cout << s << std::endl;
			throw s;
		}

		char buf[512];
		in.get(buf, 11 + 1);

		if (strcmp(buf, "# Format:  ") != 0) {
			std::string s("Invalid header (line 1) on file: " + filename);
			std::cout << s << std::endl;
			throw s;
		}

		int ver;
		in >> ver;
		if (ver != 1) {
			std::string s("Invalid version of file: " + filename);
			std::cout << s << std::endl;
			throw s;
		}

		this->clear_data();

		in.get(); // eat \n
		in.get(buf, 11 + 1);

		if (strcmp(buf, "# Depth:   ") != 0) {
			std::string s("Invalid header (line 2) on file: " + filename);
			std::cout << s << std::endl;
			throw s;
		}

		in >> this->depth;
		if (this->depth <= 0) {
			std::string s("Invalid input file: " + filename);
			std::cout << s << std::endl;
			throw s;
		}

		in.get(); // eat \n
		in.get(buf, 11 + 1);

		if (strcmp(buf, "# Rows:    ") != 0) {
			std::string s("Invalid header (line 3) on file: " + filename);
			std::cout << s << std::endl;
			throw s;
		}

		in >> this->rows;
		if (this->rows <= 0) {
			std::string s("Invalid input file: " + filename);
			std::cout << s << std::endl;
			throw s;
		}

		in.get(); // eat \n
		in.get(buf, 11 + 1);

		if (strcmp(buf, "# Columns: ") != 0) {
			std::string s("Invalid header (line 4) on file: " + filename);
			std::cout << s << std::endl;
			throw s;
		}

		in >> this->columns;
		if (this->columns <= 0) {
			std::string s("Invalid input file: " + filename);
			std::cout << s << std::endl;
			throw s;
		}

		int totalElements = rows * columns;

		in.get(); // eat \n
		in.get(buf, 8 + 1); // get "# Desc: "

		if (strcmp(buf, "# Desc: ") != 0) {
			std::string s("Invalid header (line 5) on file: " + filename);
			std::cout << s << std::endl;
			throw s;
		}

		in.get(buf, 512); // get the description (max 512 chars)
		std::string s(buf);
		set_description(s);
		if (s.length() == 0) {
			in.clear(); // if no description is present, the previous get will fail and the failbit will be set
		}

		in.get(); // eat \n
		in.get(); // get the "#"
		in.get(); // eat \n
		in.get(buf, 256); // get the other "#" or "# Map0" (discarded)
		in.get(); // eat \n

		if (!in.good()) {
			std::string s(
					"Error while parsing the header of the file: " + filename);
			std::cout << s << std::endl;
			throw s;
		}

		this->data = new Matx3DataType**[depth];	//array of pointers to 2D maps
		Matx3DataType* map;

		for (int k = 0; k < depth; k++) {
			this->data[k] = new Matx3DataType*[rows];	//array of pointers to rows of a single 2Dmap
			map = new Matx3DataType[totalElements];	//each map is allocated in a contiguous way

			for (int i = 0; i < rows; i++) {
				this->data[k][i] = map + ( i * columns );
			}

			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < columns; j++) {
					in >> *map;
					//std::cout << *map << " ";						//INSERTED SPACE IN OUTPUT <-----------------------------
					map++;
				}
				//std::cout << "\n";						//INSERTED new line <-----------------------------
				if (!in.good()) {
					std::string s("Error while loading file: " + filename);
					std::cout << s << std::endl;
					throw s;
				}
			}

			in.get(); // eat \n
			in.get(buf, 512); // get the other "#" or "# Mapk" (discarded)
			in.get(); // eat \n
		}

		in.close();
	}
}


template <typename Matx3DataType> void TemplateMatx3<Matx3DataType>::load_from_image(const std::string& filename, FileType type)
{
	//loads image in a matrix of maps allocated in a contiguous fashion

	if (type != IMAGE_RGB && type != IMAGE_GRAY) {
		std::string s(
				"Invalid file type specified (only IMAGE_RGB and IMAGE_GRAY are allowed).");
		std::cout << s << std::endl;
		throw s;
	}

	cv::Mat image;
	if (type == IMAGE_RGB)
		image = cv::imread(filename, 1); // second parameter > 0 means RBG
	else
		image = cv::imread(filename, 0); // second parameter == 0 means gray scale

	if (!image.data) {
		std::string s("Error while loading: " + filename);
		std::cout << s << std::endl;
		throw s;
	}

	this->clear_data();

	this->rows = image.size().height;
	this->columns = image.size().width;
	int stepw, steph, offset;
	int totalElements = this->rows * this->columns;

	if (type == IMAGE_RGB) {
		this->depth = 3;
		stepw = this->columns * 3;
		steph = 3; // number of channels
	} else {
		this->depth = 1;
		stepw = this->columns;
		steph = 1; // number of channels
	}
	offset = steph - 1;


	this->data = new Matx3DataType**[depth];	//array of pointers to 2D maps
	Matx3DataType* map;

	for (int k = 0; k < depth; k++) {

		map = new Matx3DataType[totalElements];	//each map is allocated in a contiguous way

		this->data[k] = new Matx3DataType*[rows];	//array of pointers to rows of a single 2Dmap

		for (int i = 0; i < rows; i++) {
			this->data[k][i] = map + ( i * columns );
		}

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				*map = image.data[i * stepw + j * steph + offset - k] / 255.0;
				map++;
			}
		}
	}

	set_description(filename);
}


template <typename Matx3DataType> void TemplateMatx3<Matx3DataType>::save_to_images(const std::string& filename, FileType type, bool normalize)
{
	//uses contiguous allocation of maps to save image

	if (type != IMAGE_RGB && type != IMAGE_GRAY) {
		std::string s(
				"Invalid file type specified (only IMAGE_RGB and IMAGE_GRAY are allowed).");
		std::cout << s << std::endl;
		throw s;
	}

	uchar* datacv;
	int totalElements = this->rows * this->columns;

	if (type == IMAGE_RGB && this->depth == 3) {

		datacv = new uchar[totalElements * 3];
		int z = 0;

		Matx3DataType* mapElementPointerArray[this->depth];  //may be dinamically allocated with new

		if (normalize) {
			Matx3DataType** minmaxsp = this->minmaxs();

			for (int k = 0; k < depth; k++){
				mapElementPointerArray[k] = this->data[k][0];
			}

			for (int j = 0; j < totalElements; j++){

				for (int k = depth - 1; k >= 0; k--) {
					datacv[z++] = (uchar) std::max(	std::min((int) round(((*mapElementPointerArray[k] - minmaxsp[k][0]) / minmaxsp[k][1]) * 255.0), 255), 0 );
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
					datacv[z++] = (uchar) std::max( std::min((int) round(*mapElementPointerArray[k] * 255.0), 255), 0);
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

		cv::Mat image(this->rows, this->columns, CV_8UC3, datacv, 0);
		bool ret = cv::imwrite(filename, image);
		if (!ret) {
			std::string s("Error while saving: " + filename);
			std::cout << s << std::endl;
			throw s;
		}

		delete[] datacv;
	} else {

		size_t pos = filename.find_last_of('.', filename.length() - 1);
		std::string name, ext;
		if (pos == std::string::npos) {
			pos = filename.length();
			ext = "";
		} else {
			ext = filename.substr(pos + 1, filename.length());
		}
		name = filename.substr(0, pos);

		datacv = new uchar[totalElements];
		Matx3DataType *mapElementPointer;

		for (int k = 0; k < depth; k++) {

			mapElementPointer = this->data[k][0];

			int z = 0;
			if (normalize) {

				Matx3DataType* minmaxp = this->minmax(k);
				for (int j = 0; j < totalElements; j++) {
					datacv[z++] = (uchar) std::max(std::min((int) round(((*mapElementPointer - minmaxp[0]) / minmaxp[1]) * 255.0), 255), 0);
					mapElementPointer++;
				}

				delete[] minmaxp;
			} else {
				for (int j = 0; j < totalElements; j++) {
					datacv[z++] = (uchar) std::max(	std::min((int) round(*mapElementPointer * 255.0), 255), 0);
					mapElementPointer++;
				}
			}

			cv::Mat image(this->rows, this->columns, CV_8UC1, datacv, 0);
			std::stringstream ss;
			ss << k;
			std::string fname(name + "_" + ss.str() + "." + ext);
			bool ret = cv::imwrite(fname, image);
			if (!ret) {
				std::string s("Error while saving: " + filename);
				std::cout << s << std::endl;
				throw s;
			}
		}

		delete[] datacv;
	}
}


template <typename Matx3DataType> void TemplateMatx3<Matx3DataType>::print_maps()
{
	for (int k = 0; k < depth; k++) {
		std::cout << "# Map" << k << std::endl;
		print_map(k);
	}
}


template <typename Matx3DataType> void TemplateMatx3<Matx3DataType>::print_map(int d)
{
	//uses contiguous memory allocation to print map

	int totalElements = rows * columns;

	Matx3DataType* mapElementPointer = this->data[d][0];

	for( int j = 0; j < totalElements; j++ ){

		if( (j + 1) % (columns) != 0 )
			std::cout << *mapElementPointer << ' ';
		else
			std::cout << *mapElementPointer << std::endl;

		mapElementPointer++;
	}

}


template <typename Matx3DataType> void TemplateMatx3<Matx3DataType>::print_description()
{
	if (this->desc != NULL)
		std::cout << this->desc << std::endl;
}


template <typename Matx3DataType> void TemplateMatx3<Matx3DataType>::print_size()
{
	std::cout << "Depth=" << depth << ", " << "Rows=" << rows << ", " << "Cols="
			  << columns << std::endl;

	std::cout << "Size of instantiated object is: ( "
			  << this->get_size_in_megabytes() << " MB )" << std::endl << std::endl;

}


template <typename Matx3DataType> void TemplateMatx3<Matx3DataType>::print()
{
	print_description();
	print_size();
	print_maps();
}


template <typename Matx3DataType> TemplateMatx3<Matx3DataType> TemplateMatx3<Matx3DataType>::stats()
{
	//uses contiguous memory allocation to calculate stats

	int totalElements = rows * columns;

	Matx3DataType **map;
	Matx3DataType m;
	Matx3DataType min;
	Matx3DataType max;

	Matx3DataType* mapElementPointer;

	m = 0.0;
	min = this->data[0][0][0];
	max = min;

	for (int k = 0; k < depth; k++) {
		map = get_map(k);
		mapElementPointer = map[0];

		for (int  j = 0; j < totalElements; j++) {
			m += *mapElementPointer;
			min = std::min(min, *mapElementPointer);
			max = std::max(max, *mapElementPointer);
			mapElementPointer++;
		}
	}
	m /= double(totalElements * depth);

	TemplateMatx3<Matx3DataType> ret(1, 1, 4);
	ret.data[0][0][0] = m;
	ret.data[0][0][1] = (Matx3DataType) std(m);							//CHECK IF CAST IS CORRECT <------------------------------------------
	ret.data[0][0][2] = min;
	ret.data[0][0][3] = max;

	ret.set_description(
			"GLOBAL STATS. Col0: mean, Col1: standard deviation, Col2: mininum, Col3: maximum. They are computed considering all-the-elements in all-the-maps.");
	return ret;
}


template <typename Matx3DataType> TemplateMatx3<Matx3DataType> TemplateMatx3<Matx3DataType>::stats_map(int d)
{
	//uses contiguous memory allocation to calculate stats

	int totalElements = rows * columns;

	Matx3DataType **map = this->get_map(d);
	Matx3DataType* mapElementPointer = map[0];

	Matx3DataType min = *mapElementPointer;
	Matx3DataType max = min;

	Matx3DataType m = 0.0;
	for (int j = 0; j < totalElements; j++) {
		m += *mapElementPointer;
		min = std::min(min, *mapElementPointer);
		max = std::max(max, *mapElementPointer);
		mapElementPointer++;
	}
	m /= double(totalElements);

	TemplateMatx3<Matx3DataType> ret(1, 1, 4);
	ret.data[0][0][0] = m;
	ret.data[0][0][1] = (Matx3DataType) std(d, m); 						//CHECK IF CAST IS CORRECT <------------------------------------------
	ret.data[0][0][2] = min;
	ret.data[0][0][3] = max;

	ret.set_description(
			"SINGLE MAP STATS. Col0: mean, Col1: standard deviation, Col2: mininum, Col3: maximum. They are computed considering the selected map only.");
	return ret;
}


template <typename Matx3DataType> TemplateMatx3<Matx3DataType> TemplateMatx3<Matx3DataType>::stats_maps()
{
	TemplateMatx3<Matx3DataType> ret(depth, 1, 4);
	for (int k = 0; k < depth; k++) {
		TemplateMatx3<Matx3DataType> cur = stats_map(k);
		ret.data[k][0][0] = cur.data[0][0][0];
		ret.data[k][0][1] = cur.data[0][0][1];
		ret.data[k][0][2] = cur.data[0][0][2];
		ret.data[k][0][3] = cur.data[0][0][3];
	}

	ret.set_description(
			"MULTIPLE MAPS STATS. Col0: mean, Col1: standard deviation, Col2: mininum, Col3: maximum. They are computed considering each map independently.");
	return ret;
}


template <typename Matx3DataType> TemplateMatx3<Matx3DataType> TemplateMatx3<Matx3DataType>::stats_pp()
{
	//uses contiguous memory allocation to calculate stats

	int totalElements = rows * columns;

	Matx3DataType cur_min, cur_max;
	Matx3DataType m, s = 0.0;
	TemplateMatx3<Matx3DataType> ret(4, rows, columns);

	Matx3DataType* mapElementPointerArray[this->depth];  //may be dinamically allocated with new

	for (int k = 0; k < depth; k++){
		mapElementPointerArray[k] = this->data[k][0];
	}

	Matx3DataType* data0ElementPointer = ret.data[0][0];
	Matx3DataType* data1ElementPointer = ret.data[1][0];
	Matx3DataType* data2ElementPointer = ret.data[2][0];
	Matx3DataType* data3ElementPointer = ret.data[3][0];

	for (int j = 0; j < totalElements; j++) {

		cur_min = *mapElementPointerArray[0];
		cur_max = cur_min;
		m = cur_min;

		for (int k = 1; k < depth; k++) {

			cur_min = std::min(cur_min, *mapElementPointerArray[k]);
			cur_max = std::max(cur_max, *mapElementPointerArray[k]);
			m += *mapElementPointerArray[k];
		}

		m /= depth;

		for (int k = 0; k < depth; k++)
			s += (*mapElementPointerArray[k] - m) * (*mapElementPointerArray[k] - m);

		s = sqrt(s / double(depth));

		*data0ElementPointer = m;
		*data1ElementPointer = s;
		*data2ElementPointer = cur_min;
		*data3ElementPointer = cur_max;

		data0ElementPointer++;
		data1ElementPointer++;
		data2ElementPointer++;
		data3ElementPointer++;

		for (int k = 0; k < depth; k++)
			mapElementPointerArray[k]++;

	}

	ret.set_description(
			"(ROW,COLUMN) STATS. Map0: mean, Map1: standard deviation, Map2: mininum, Map3: maximum. "
			"They are computed for each pair (row,column) over the different maps.");
	return ret;
}


template <typename Matx3DataType> double TemplateMatx3<Matx3DataType>::mean(int d)
{
	//uses contiguous memory allocation to calculate mean of map

	int totalElements = rows * columns;

	Matx3DataType **map = this->get_map(d);
	Matx3DataType* mapElementPointer = map[0];

	double m = 0.0;

	for (int j = 0; j < totalElements; j++) {
			m += *mapElementPointer;
			mapElementPointer++;
	}

	return m / double( totalElements );

}


template <typename Matx3DataType> double* TemplateMatx3<Matx3DataType>::means()
{
	double* means = new double[depth];
	for (int k = 0; k < depth; k++) {
		means[k] = this->mean(k);
	}
	return means;
}


template <typename Matx3DataType> void TemplateMatx3<Matx3DataType>::means(double *means)
{
	for (int k = 0; k < depth; k++) {
		means[k] = this->mean(k);
	}
}


template <typename Matx3DataType> double TemplateMatx3<Matx3DataType>::std(int d, double mean_d)
{
	//uses contiguous memory allocation to calculate std of given map with known mean

	int totalElements = rows * columns;

	Matx3DataType **map = this->get_map(d);
	Matx3DataType* mapElementPointer = map[0];

	double s = 0.0;

	for (int j = 0; j < totalElements; j++) {
		s += (*mapElementPointer - mean_d) * (*mapElementPointer - mean_d);
		mapElementPointer++;
	}
	return sqrt( s / double( totalElements ) );
}


template <typename Matx3DataType> double* TemplateMatx3<Matx3DataType>::stds(const double* means)
{
	double* stds = new double[depth];
	for (int k = 0; k < depth; k++) {
		stds[k] = this->std(k, means[k]);
	}
	return stds;
}


template <typename Matx3DataType> double TemplateMatx3<Matx3DataType>::std(double mean_all)
{
	//uses contiguous memory allocation to calculate std over a complete data matrix

	int totalElements = rows * columns;

	double s = 0.0;
	Matx3DataType **map;
	Matx3DataType* mapElementPointer;

	for (int k = 0; k < depth; k++) {

		map = this->get_map(k);
		mapElementPointer = map[0];

		for (int j = 0; j < totalElements; j++) {
			s += (*mapElementPointer - mean_all) * (*mapElementPointer - mean_all);
			mapElementPointer++;
		}
	}

	return sqrt(s / double( totalElements * depth ));

}


template <typename Matx3DataType> Matx3DataType** TemplateMatx3<Matx3DataType>::minmaxs()
{
	Matx3DataType** ret = new Matx3DataType*[depth];
	for (int k = 0; k < depth; k++) {
		ret[k] = this->minmax(k);
	}
	return ret;
}


template <typename Matx3DataType> Matx3DataType* TemplateMatx3<Matx3DataType>::minmax(int d)
{
	//uses contiguous memory allocation to calculate min and max of a map

	int totalElements = rows * columns;

	Matx3DataType **map = this->get_map(d);
	Matx3DataType* mapElementPointer = map[0];

	Matx3DataType minElement = *mapElementPointer;
	Matx3DataType maxElement = minElement;

	for (int j = 0; j < totalElements; j++) {
		minElement = std::min(minElement, *mapElementPointer);
		maxElement = std::max(maxElement, *mapElementPointer);
		mapElementPointer++;
	}
	Matx3DataType* ret = new Matx3DataType[2];
	ret[0] = minElement; ret[1] = maxElement;
	return ret;
}


template <typename Matx3DataType> void TemplateMatx3<Matx3DataType>::unitnorm()
{
	//uses contiguous memory allocation to set to unit norm all maps

	int totalElements = rows * columns;

	TemplateMatx3<Matx3DataType>* n;
	n = new TemplateMatx3<Matx3DataType>(1, rows, columns, 0.0);
	Matx3DataType** z = n->get_map(0);
	Matx3DataType* zMapElementPointer;

	Matx3DataType* mapElementPointer;

	for (int k = 0; k < depth; k++) {

		mapElementPointer = this->data[k][0];
		zMapElementPointer = z[0];

		for (int j = 0; j < totalElements; j++) {
			*zMapElementPointer += *mapElementPointer * *mapElementPointer;
			zMapElementPointer++;
			mapElementPointer++;
		}
	}

	for (int k = 0; k < depth; k++) {

		mapElementPointer = this->data[k][0];
		zMapElementPointer = z[0];

		for (int j = 0; j < totalElements; j++) {
			*mapElementPointer /= sqrt(*zMapElementPointer);
			mapElementPointer++;
			zMapElementPointer++;
		}
	}

	delete n;
}


template <typename Matx3DataType> double TemplateMatx3<Matx3DataType>::get_size_in_megabytes()
{
	//size considering contiguous memory allocation

	unsigned long s = 0;
	s += depth * rows * columns * sizeof(Matx3DataType); // data
	s += sizeof(Matx3DataType***) * depth + rows * sizeof(Matx3DataType*) * depth; // pointers
	s += 3 * sizeof(int); // rows, columns, depth
	s += sizeof(char*); // desc

	if (desc != NULL)
		s += (strlen(desc) + 1) * sizeof(char); // desc string (null-terminated)
	return s/double(1048576);
}


template <typename Matx3DataType> void TemplateMatx3<Matx3DataType>::sum_pointwise(TemplateMatx3<Matx3DataType>* other)
{
	//uses contiguous memory allocation to sum maps pointwise

	int totalElements = rows * columns;

	Matx3DataType **map;
	Matx3DataType **otherMap;

	Matx3DataType* mapElementPointer;
	Matx3DataType* otherMapElementPointer;

	for (int k = 0; k < depth; k++) {

		map = this->get_map(k);
		mapElementPointer = map[0];

		otherMap = other->get_map(k);
		otherMapElementPointer = otherMap[0];

		for (int j = 0; j < totalElements; j++) {
			*mapElementPointer += *otherMapElementPointer;
			mapElementPointer++;
			otherMapElementPointer++;
		}
	}
}


template <typename Matx3DataType> void TemplateMatx3<Matx3DataType>::diff_pointwise(TemplateMatx3<Matx3DataType>* other)
{
	//uses contiguous memory allocation to diff maps pointwise

	int totalElements = rows * columns;

	Matx3DataType **map;
	Matx3DataType **otherMap;

	Matx3DataType* mapElementPointer;
	Matx3DataType* otherMapElementPointer;

	for (int k = 0; k < depth; k++) {

		map = this->get_map(k);
		mapElementPointer = map[0];

		otherMap = other->get_map(k);
		otherMapElementPointer = otherMap[0];

		for (int j = 0; j < totalElements; j++) {
			*mapElementPointer -= *otherMapElementPointer;
			mapElementPointer++;
			otherMapElementPointer++;
		}
	}
}


template <typename Matx3DataType> void TemplateMatx3<Matx3DataType>::prod_pointwise(TemplateMatx3<Matx3DataType>* other)
{
	//uses contiguous memory allocation to mult maps pointwise

	int totalElements = rows * columns;

	Matx3DataType **map;
	Matx3DataType **otherMap;

	Matx3DataType* mapElementPointer;
	Matx3DataType* otherMapElementPointer;

	for (int k = 0; k < depth; k++) {

		map = this->get_map(k);
		mapElementPointer = map[0];

		otherMap = other->get_map(k);
		otherMapElementPointer = otherMap[0];

		for (int j = 0; j < totalElements; j++) {
			*mapElementPointer *= *otherMapElementPointer;
			mapElementPointer++;
			otherMapElementPointer++;
		}
	}
}

/*
 * copy method - ADDED BY MATTEO for the sake of copying a Matx3 into another without needing constructor
 */

template <typename Matx3DataType> void TemplateMatx3<Matx3DataType>::copy_me( const TemplateMatx3<Matx3DataType>* copy_me ){

	/*
	 * deallocates assigned memory
	 */
	this->clear_data();

	//allocates contiguous memory for maps

	this->depth = copy_me->depth;
	this->rows = copy_me->rows;
	this->columns = copy_me->columns;
	int totalElements = rows * columns;

	this->data = new Matx3DataType **[depth];					//array of pointers to 2D maps
	Matx3DataType* map;
	Matx3DataType* copy_meElementPointer;

	for( int k = 0; k < depth; k++ ){

		map = new Matx3DataType[totalElements];	//each map is allocated in a contiguous way
		this->data[k] = new Matx3DataType*[rows];	//array of pointers to rows of a single 2Dmap

		copy_meElementPointer = copy_me->data[k][0];

		for (int i = 0; i < rows; i++) {
			this->data[k][i] = map + ( i * columns );
		}

		for (int j = 0; j < totalElements; j++){
			*map = *copy_meElementPointer;
			map++;
			copy_meElementPointer++;
		}
	}

	if( copy_me->desc != NULL ){

		this->desc = new char[strlen(copy_me->desc) + 1];
		strcpy(this->desc, copy_me->desc);
		this->desc[strlen(copy_me->desc)] = '\0';
	}else
		this->desc = NULL;

	this->size = copy_me->size;

}

