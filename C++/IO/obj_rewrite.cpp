//================================================================
// Copyright Dr.Little
// First finish: 2018-08-27
// Last modify: 2018-
//
// This file is used to read one .obj file and write N .off files.
// It is one part of bim/cim uc data platform demo.
//
// The techniques used in this file
//     1) ifstream
//     2) getline()
//     3) sprintf()
//     4) fprintf()
//     5) istringstream()
//     6) print string object(string.c_str())
//     7) split a string(boost/.../split())
//
//================================================================

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <string.h>
#include <algorithm>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

vector<string> v_list;
vector<string> f_list;

int face_verticle_num = 3;


void write_obj_file(const char* outname_prefix, int obj_id) {

	char* out_obj_file_name = new char[200];
	sprintf(out_obj_file_name, "%s%d.off", outname_prefix, obj_id);
	FILE* out_obj_file = fopen(out_obj_file_name, "a");

	if (out_obj_file == NULL)
		printf("The out_obj_file is invalid.\n");

	int v_num = v_list.size();
	int f_num = f_list.size();
	int e_num = 0;

	//write the head 2 lines
	fprintf(out_obj_file, "OFF\n%d %d %d\n\n", v_num, f_num, e_num);

	//write the vertices
	for (int v = 0; v < v_num; v++) {
		printf("v_list[%d] = %s\n", v, v_list[v].c_str());
		fprintf(out_obj_file, "%s\n", v_list[v].c_str());
	}

	//write the vertices
	for (int f = 0; f < f_num; f++) {
		printf("f_list[%d] = %s\n", f, f_list[f].c_str());
		fprintf(out_obj_file, "%d %s\n", face_verticle_num, f_list[f].c_str());
	}

	fclose(out_obj_file);
	printf("Finish writing obj_%d.off\n", obj_id);
}


void rewriteObjFile(const char* filename, const char* outname_prefix) {
//	FILE* in_obj_file = fopen(filename, "r");
//	if (in_obj_file == NULL)
		//printf("The in_obj_file is invalid.\n");

	string s, first_word;
	istringstream ss;
	int object_id = -1;
	int line_num = 0;

	ifstream in_obj_file(filename);
	while (getline(in_obj_file, s)) {

		ss.clear();
		ss.str(s);
		ss >> first_word;
		printf("Line %d: %s, first_word = %s\n", line_num, s.c_str(), first_word.c_str());
		line_num++;

		if (first_word == "v") {
			string x, y, z, verticle;
			ss >> x >> y >> z;
			verticle = x + " " + y + " " + z;
			v_list.push_back(verticle);
			printf("The last v_list element: %s\n", verticle.c_str());
		}

		else if (first_word == "o") {

			if(object_id >= 0)
				write_obj_file(outname_prefix, object_id);
			
			object_id++;
			f_list.clear();

//			if (object_id == 1)
//				exit(0);
		}

		else if (first_word == "f") {
			string v1, v2, v3, face;
			ss >> v1 >> v2 >> v3;

			vector<string> v1_split, v2_split, v3_split;
			split(v1_split, v1, is_any_of("/"));
			split(v2_split, v2, is_any_of("/"));
			split(v3_split, v3, is_any_of("/"));

			face = v1_split[0] + " " + v2_split[0] + " " + v3_split[0];		
			f_list.push_back(face);
			printf("The last f_list element: %s\n", face.c_str());
		}

		else
			continue;
	}
	write_obj_file(outname_prefix, object_id++);

//	fclose(in_obj_file);	
}


int main() {
	char* in_obj_file_name = new char[200];
	char* outname_prefix   = new char[200];

	sprintf(in_obj_file_name, "../data/house.obj");
	sprintf(outname_prefix, "../data/obj_files/object_");

	rewriteObjFile(in_obj_file_name, outname_prefix);
}
