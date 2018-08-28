//=================================================================
// Copyright SijieRuan, Dr.Little
// First finish: 2018-08-28
// Last modify: 2018-
//
// This file ..............
// It is one part of bim/cim uc data platform demo.
//
// The techniques used in this file
//     1) CGAL: Point_3, Bbox_3, do_overlap()
//     2) STL: vector
//     3) stringstream
//     4) rand(), double rand
//     5) ifstream, ofstream
//     6) setprecision()
//
//=================================================================

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Bbox_3.h>

using namespace std;
using namespace boost;

typedef CGAL::Simple_cartesian<double> K;
typedef K::Point_3 Point;
typedef CGAL::Bbox_3 Bbox_3;

//Global variables
vector<Bbox_3> bboxes;
vector<Point> vertices;
vector<Point> new_vertices;
vector<string> head_lines;
vector<string> tail_lines;

//Function declaration
void readObjFile(const char* in_objfile_name);
bool checkValid(double x_offset, double y_offset, double z_offset);
void transferVertices(double x_offset, double y_offset, double z_offset);
void batchTransfer(int house_num, const char* out_objfile_prefix);
void writeFiles(const char* out_objfile_prefix);


/**
 * Function: read the .obj (Converted from .rvt)
*/
void readObjFile(const char* in_objfile_name) {

	ifstream in_obj_file(in_objfile_name);

	string line, first_word;
	istringstream iss;
	bool before_verticle = true;

	double x_min = FLT_MAX;
	double y_min = FLT_MAX;
	double z_min = FLT_MAX;
	double x_max = -FLT_MAX;
	double y_max = -FLT_MAX;
	double z_max = -FLT_MAX;

	// find the beginning of the vertices
	while (getline(in_obj_file, line)) {
		
		iss.clear();
		iss.str(line);
		iss >> first_word;

		//cout << "first word: " << first_word.c_str() << endl;

		if (first_word != "v") {
			if (before_verticle) {
				cout << "head line: " << line << endl;
				head_lines.push_back(line);
			}
			else {
//				cout << "tail line: " << line << endl;
				tail_lines.push_back(line);
			}

			//cout << endl;
		}
		else {
			before_verticle = false;

			double x, y, z;
			iss >> x >> y >> z;

			vertices.push_back(Point(x, y, z));

			//cout << "verticle: " << vertices[vertices.size() - 1].x()
			//	<< " " << vertices[vertices.size() - 1].y()
			//	<< " " << vertices[vertices.size() - 1].z()
			//	<< endl;

			//update the bbox
			x_min = x_min < x ? x_min : x;
			y_min = y_min < y ? y_min : y;
			z_min = z_min < z ? z_min : z;
			x_max = x_max > x ? x_max : x;
			y_max = y_max > y ? y_max : y;
			z_max = z_max > z ? z_max : z;

			//cout << "min xyz: " << x_min << ", " << y_min << ", " << z_min << endl;
			//cout << "max xyz: " << x_max << " " << y_max << " " << z_max << endl;
			//cout << endl;
		}		
	}

	Bbox_3 bbox = Bbox_3(x_min, y_min, z_min, x_max, y_max, z_max);
	bboxes.push_back(bbox);

	cout << "the bounding box: (" << x_min << "," << y_min << "," << z_min
		<< ", " << x_max << "," << y_max << "," << z_max << ")" << endl << endl;
}

/**
 * Function: check the validation of the generated offset vector
*/
bool checkValid(double x_offset, double y_offset, double z_offset) {

	double new_x_min = bboxes.begin()->xmin() + x_offset;
	double new_y_min = bboxes.begin()->ymin() + y_offset;
	double new_z_min = bboxes.begin()->zmin() + z_offset;
	double new_x_max = bboxes.begin()->xmax() + x_offset;
	double new_y_max = bboxes.begin()->ymax() + y_offset;
	double new_z_max = bboxes.begin()->zmax() + z_offset;

	cout << endl;
	cout << "new_x_min = " << new_x_min << endl;
	cout << "new_y_min = " << new_y_min << endl;
	cout << "new_z_min = " << new_z_min << endl;
	cout << "new_x_max = " << new_x_max << endl;
	cout << "new_y_max = " << new_y_max << endl;
	cout << "new_z_max = " << new_z_max << endl;
	cout << endl;

	//maybe over here
	if (new_x_min < 0 || new_y_min < 0 || new_z_min < 0) {
		cout << "Failed: the one min coordinate is nagetive!" << endl;
		return false;
	}

	Bbox_3 new_bbox = Bbox_3(new_x_min, new_y_min, new_z_min, new_x_max, new_y_max, new_z_max);

	for (int i = 0; i < bboxes.size(); i++) {
		
		if (do_overlap(*(bboxes.begin() + i), new_bbox)) {
			cout << "Failed: the bbox " << i << " has overlap with the new bbox." << endl;
			return false;
		}

		//if (do_overlap(new_bbox, new_bbox)) {
		//	cout << "Failed: the new bbox and the new bbox." << endl;
		//}
	}
	
	bboxes.push_back(new_bbox);
	cout << "Success: the new bbox has added into the bboxes." << endl;
	cout << "the NEW bounding box: (" << new_x_min << "," << new_y_min << "," << new_z_min
		<< ", " << new_x_max << "," << new_y_max << "," << new_z_max << ")" << endl << endl;
	return true;
}


/**
 * Function: generate the random number in double type
*/
double gen_rand() {
	return (rand() / double(RAND_MAX)) * 1000;
}


/**
 * Function: tranfer old vertices to new ones according to the offset vector
*/
void transferVertices(double x_offset, double y_offset, double z_offset) {
	for (int v = 0; v < vertices.size(); v++) {
		double new_x = vertices[v].x() + x_offset;
		double new_y = vertices[v].y() + y_offset;
		double new_z = vertices[v].z() + z_offset;

		new_vertices.push_back(Point(new_x, new_y, new_z));

		//cout << "new verticle: " << new_vertices[new_vertices.size() - 1].x()
		//	<< " " << new_vertices[new_vertices.size() - 1].y()
		//	<< " " << new_vertices[new_vertices.size() - 1].z()
		//	<< endl;
	}
}


/**
 * Function: write the new generated data into a new .obj
*/
void writeOneFile(const char* out_objfile_prefix, int id) {

	cout << endl;

	char* out_objfile_name = new char[200];
	sprintf(out_objfile_name, "%s%d.obj", out_objfile_prefix, id);

	ofstream out_obj_file(out_objfile_name);

	if (out_obj_file.is_open()) {

		for (int i = 0; i < head_lines.size(); i++)
			out_obj_file << head_lines[i] << endl;
		
		for (int j = 0; j < new_vertices.size(); j++) {
			string new_line;
			stringstream xx, yy, zz;

			xx << fixed << setprecision(4) << new_vertices[j].x();
			yy << fixed << setprecision(4) << new_vertices[j].y();
			zz << fixed << setprecision(4) << new_vertices[j].z();

			new_line = "v " + xx.str() + " " + yy.str() + " " + zz.str();

			//cout << "The new verticle line: " << new_line << endl;

			out_obj_file << new_line << endl;
		}

		for (int k = 0; k < tail_lines.size(); k++)
			out_obj_file << tail_lines[k] << endl;

		out_obj_file.close();
	}

	cout << out_objfile_name << "has finished writing." << endl;
}


/**
 * Function: generate new datasets with the number of house_num
*/
void batchTransfer(int house_num, const char* out_objfile_prefix) {

	int id = 0;

	while (id <= house_num) {
		double x_offset = gen_rand();
		double y_offset = gen_rand();
		double z_offset = gen_rand();

		cout << endl << "The random offset: " << x_offset << " "
			<< y_offset << " " << z_offset << endl << endl;

		if (!checkValid(x_offset, y_offset, z_offset))
			continue;
		else {
			transferVertices(x_offset, y_offset, z_offset);
			writeOneFile(out_objfile_prefix, id);
			vertices.clear();
		}

		id++;
	}
}


/**
 * Function: the MAIN entry
*/
int main(int argc, char* argv[]) {

	srand(time(NULL));

	//read in the parameters from the cmd
	char* in_objfile_name  = new char[200];
	char* out_objfile_prefix = new char[200];
	sprintf(in_objfile_name, "%s", argv[1]);
	sprintf(out_objfile_prefix, "%s", argv[2]);
	int house_num = atoi(argv[3]);

	//fill the vertices and post_lines
	readObjFile(in_objfile_name);

	//transfer the vertices value in the batch manner
	batchTransfer(house_num, out_objfile_prefix);

	return 0;
}