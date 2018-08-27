//=================================================================
// Copyright SijieRuan, Dr.Little
// First finish: 2018-08-27
// Last modify: 2018-
//
// This file reads one .off file and puts data into a triangle list
// It is one part of bim/cim uc data platform demo.
//
// The techniques used in this file
//     1) CGAL::Triangle_3/Point_3/Line_3/Ray_3
//     2) std::ifstream (read in word)
//     3) std::vector
//     4) std::list
//     5) constructs AABB tree
//     6) counts #intersections
//     7) compute closest point and squared distance
//     8) access the vertices coordinates in a triangle list
//
//=================================================================

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <list>
#include <vector>

#include <stdio.h>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits.h>
#include <CGAL/AABB_triangle_primitive.h>

typedef CGAL::Simple_cartesian<double> K;

typedef K::FT FT;
typedef K::Ray_3 Ray;
typedef K::Line_3 Line;
typedef K::Point_3 Point;
typedef K::Triangle_3 Triangle;

typedef std::list<Triangle>::iterator Iterator;
typedef CGAL::AABB_triangle_primitive<K, Iterator> Primitive;
typedef CGAL::AABB_traits<K, Primitive> AABB_triangle_traits;
typedef CGAL::AABB_tree<AABB_triangle_traits> Tree;

std::list<Triangle> readOffFile(const char* off_file_name);

std::list<Triangle> readOffFile(const char* off_file_name) {
	
	std::string line, off_line;
	std::istringstream ss;
	int line_num = 0;
	int n, f, e;
	std::vector<Point> vertices;
	std::list<Triangle> triangles;

	std::ifstream in_obj_file(off_file_name);
	in_obj_file >> off_line;
	in_obj_file >> n >> f >> e;

	printf("n = %d, f = %d, e = %d\n\n", n, f, e);

	//read the vertices into the vertices_list
	for (int i = 0; i < n; i++) {
		double x, y, z;
		in_obj_file >> x >> y >> z;

		printf("Point: %.1f, %.1f, %.1f\n", x, y, z);

		vertices.push_back(Point(x,y,z));
	}
	printf("\n");

	//read all face
	for (int i = 0; i < f; i++) {
		int a, v1, v2, v3;
		in_obj_file >> a >> v1 >> v2 >> v3;

		printf("Face vertices: %d, %d, %d\n", v1, v2, v3);

		triangles.push_back(Triangle(vertices[v1], vertices[v2], vertices[v3]));
	}
	printf("\n");

	return triangles;
}

int main(int argc, char* argv[])
{
	if(argc < 2) {
	    std::cerr << "Usage: ./drlittle ../data/xxx.off" << std::endl;
	    return EXIT_FAILURE;
	}

	std::list<Triangle> triangles = readOffFile(argv[1]);

	printf("The 3 verticles of the first triangle: \
		\n(%.1f, %.1f, %.1f),\n(%.1f, %.1f, %.1f),\n(%.1f, %.1f, %.1f).\n\n",
		triangles.begin()->vertex(0).x(),
		triangles.begin()->vertex(0).y(),
		triangles.begin()->vertex(0).z(),
		triangles.begin()->vertex(1).x(),
		triangles.begin()->vertex(1).y(),
		triangles.begin()->vertex(1).z(),
		triangles.begin()->vertex(2).x(),
		triangles.begin()->vertex(2).y(),
		triangles.begin()->vertex(2).z());
//	print(triangles.begin()->vertext(0));

	// constructs AABB tree
	Tree tree(triangles.begin(), triangles.end());

	Point a(1.0, 0.0, 0.0);
	Point b(0.0, 1.0, 0.0);

	// counts #intersections
	Ray ray_query(a, b);
	std::cout << tree.number_of_intersected_primitives(ray_query)
		<< " intersections(s) with ray query" << std::endl;

	// compute closest point and squared distance
	Point point_query(2.0, 2.0, 2.0);
	Point closest_point = tree.closest_point(point_query);
	std::cerr << "closest point is: " << closest_point << std::endl;
	FT sqd = tree.squared_distance(point_query);
	std::cout << "squared distance: " << sqd << std::endl;
	std::cout << std::endl;

	return EXIT_SUCCESS;
}

