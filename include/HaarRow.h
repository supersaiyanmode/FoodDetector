/*
 * HaarRow.h
 *
 *  Created on: Mar 27, 2016
 *      Author: sunkenglory
 */

#ifndef INCLUDE_HAARROW_H_
#define INCLUDE_HAARROW_H_

class HaarRow {
public:
	int x, y, size_width, size_height;
	HaarRow(int new_x, int new_y, int new_size_width, int new_size_height) :
			x(new_x), y(new_y), size_width(new_size_width), size_height(
					new_size_height) {
	}

};

#endif /* INCLUDE_HAARROW_H_ */
