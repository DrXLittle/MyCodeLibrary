//================================================================
// Copyright Dr.Little
// First finish: 2018-01-01
// Last modify: 2018-01-13
//
// This file is used to read a .bin file
// It is one part of proximity experiments
//
// The techniques used in this file:
//     1) int main(int argc, char* argv[])
//     2) ifstream to read binary file
//     3) fin.peek() != EOF
//     4) string -> int
//     5) long long
//     6) sprintf()
//
//================================================================

#include <cstdlib>
#include <iostream>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

long long readBinFile(const char* in_name)
{
	long long line_num = 0;

	ifstream fin(in_name);

	double lat, lng;

	int id = 0, ts = 0;
	while(fin.peek() != EOF)
	{
		fin.read((char*)&lat, sizeof(double));
		fin.read((char*)&lng, sizeof(double));
		
//		cout << id << ' ' << ts << ' ' << lat << ' ' << lng << endl;

		if(ts < 19) ts++;
		else {id++; ts = 0;}

		line_num++;
	}

	return line_num;
}

int main(int argc, char* argv[])
{
	char* src_dir = new char[200];
	char* prefix  = new char[200];
	char* in_file_name  = new char[500];

	sprintf(src_dir, "/mnt/disk_root_ori/home/xuying/proxim/0_all_data/geolife/pred_kalman_50wid_bin/");
	sprintf(prefix,  "kalman_geolife");

	int st_id = atoi(argv[1]);
	int ed_id = atoi(argv[2]);

	for(int i = st_id; i < ed_id; i++)
	{
		sprintf(in_file_name, "%s%s_%d.bin", src_dir, prefix, i);

		long long read_line_num = readBinFile(in_file_name);
		if(read_line_num != 10000000)
			cout << prefix << "_" << i << ".bin's line num: " << read_line_num <<  endl;
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
