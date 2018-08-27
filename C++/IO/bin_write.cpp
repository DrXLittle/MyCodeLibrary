//================================================================
// Copyright Dr.Little
// First finish: 2018-01-01
// Last modify: 2018-01-13
//
// This file is used to read a .txt file and write to a .bin file.
// It is one part of proximity experiments
//
// The techniques used in this file:
//     1) ifstream fin(in_name);
//     2) ofstream fout(out_name, ios::binary);
//     3) while(getline(fin, line))
//     4) stringstream ss(line);
//     5) int main(int argc, char* argv[])
//
//================================================================


#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

void convert(const char* in_name, const char* out_name)
{
	ifstream fin(in_name);
	ofstream fout(out_name, ios::binary);

	string line;
	int i = 0;
	int id, ts;
	double lat, lng;

	while(getline(fin, line))
	{
		stringstream ss(line);
		ss >> id >> ts >> lat >> lng;
		fout.write((char*)&lat, sizeof(double));
		fout.write((char*)&lng, sizeof(double));

	}

	fin.close();
	fout.close();
}

int main(int argc, char* argv[])
{
	char* src_dir = new char[200];
	char* dst_dir = new char[200];
	char* prefix  = new char[200];
	char* in_file_name  = new char[500];
	char* out_file_name = new char[500];

	sprintf(src_dir, "/mnt/disk_root_ori/home/xuying/proxim/0_all_data/truck/pred_kalman_50wid/");
	sprintf(dst_dir, "/mnt/disk_root_ori/home/xuying/proxim/0_all_data/truck/pred_kalman_50wid_bin/");
	sprintf(prefix,  "kalman_truck");
/*	src_dir = argv[1];
	dst_dir = argv[2];
	prefix  = argv[3];*/
	int st_id = atoi(argv[1]);
	int ed_id = atoi(argv[2]);

	for(int i = st_id; i < ed_id; i++)
	{
		sprintf(in_file_name, "%s%s_%d.txt", src_dir, prefix, i);
		sprintf(out_file_name, "%s%s_%d.bin", dst_dir, prefix, i);

		convert(in_file_name, out_file_name);
		cout << prefix << "_" << i << ".bin has been generated." << endl;
	}

	return 0;
}

/*int main(int argc,char * argv[])
{
  ifstream fin(argv[1]);
  ofstream fout(argv[2],ios::binary);
  string line;
  int i=0;
  double lat,lng;
  while(getline(fin,line))
  { 
   // if(i>=200)
   //   break;
   //stringstream is(line);
   //is>>lat>>lng; 
   //fout.write((char*)&lat,sizeof(double));
   //fout.write((char*)&lng,sizeof(double));
   fout.write(line.c_str(),sizeof(char)*256); 
   //i++;
  }  
  fin.close();
  fout.close();
  char buff[256]={0};
  ifstream f(argv[2],ios::binary);
  while(f.peek()!=EOF)
  {
    f.read(buff,sizeof(char)*256);
    //f.read((char*)&lat,sizeof(double));
    //f.read((char*)&lng,sizeof(double));
    cout<<buff<<endl;
    //cout<<lat<<' '<<lng<<endl;
  }
  f.close();
  return 0;
}*/
