#include "ObjLoader.h"
#include <iostream>
#include <fstream>
#include <fstream>

OBJLoader::OBJLoader(const char *path) : vertices_(),facesV_(),facesT_(),facesN_() {
    std::ifstream fin;
    char input,input2;
    fin.open (path, std::ifstream::in);
    if(fin.fail()) return;
    std::string line;
	fin.get(input);
    while(!fin.eof()) {
        if(input == 'v') {
            fin.get(input);
			// std::cout << input << std::endl;
            if(input = ' ') {
                /*Vertex*/
                // fin >> vertices.emplace_back(x) >> vertices[vertexIndex].y >> vertices[vertexIndex].z;
                Vec3f v;
                fin >> v.x >> v.y >> v.z;
				
                vertices_.emplace_back(v);
            }
            else if(input == 't') {
                /*TODO: Texture Coordinate*/
            }
            else if(input == 'n') {
                /*TODO: Normal*/
            }
        }
        else if (input == 'f') {    // face
			fin.get(input);
			if(input == ' ') {
                Vec3i fv,ft,fn;
                fin >> fv.x >> input2 >> ft.x >> input2 >> fn.x
                    >> fv.y >> input2 >> ft.y >> input2 >> fn.y
                    >> fv.z >> input2 >> ft.z >> input2 >> fn.z;
				fv.x--;
				fv.y--;
				fv.z--;
                facesV_.emplace_back(fv);
                facesT_.emplace_back(ft);
                facesN_.emplace_back(fn);
            }
        }
		// Read in the remainder of the line.
		while(input != '\n')
		{
			fin.get(input);
		}

		// Start reading the beginning of the next line.
		fin.get(input);
    }
}

int OBJLoader::nfaces() {
    return facesV_.size();
}

Vec3i OBJLoader::face(int idx) {
    return facesV_[idx];
}

Vec3f OBJLoader::vert(int idx) {
    return vertices_[idx];
}

void OBJLoader::dump() {
	std::cout << "begin dump" << std::endl;
	std::cout << "vertex" << std::endl;
	for(int i = 0;i < vertices_.size();i++) 
		std::cout << vertices_[i].x<< "/"  << vertices_[i].y << "/" << vertices_[i].z <<std::endl;
	std::cout << "face" << std::endl;
    for(int i = 0;i < facesV_.size();i++)
        std::cout << facesV_[i].x << "/" << facesV_[i].y << "/" << facesV_[i].z << std::endl;
}