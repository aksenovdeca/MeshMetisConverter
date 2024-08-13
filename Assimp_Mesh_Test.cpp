// Assimp_Mesh_Test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include <map>
#include"glm.hpp"
#include<vector>
#include<fstream>
#include<algorithm>
#include<optional>
#include<tuple>


static std::string filepath= "..\uploads_files_3162983_f104starfighter.obj";



void writeMeshToMetisFile(const aiScene*scene, const std::string& filename)
{
	std::vector<unsigned int> xadjVec; // Index array for METIS
	std::vector<unsigned int> adjncyVec; // Adjacency list

	// Initialize xadj for number of vertices
	xadjVec.push_back(0); // Start at index 0

	size_t numvertexinmodel;
	for(int a=0;a<scene->mNumMeshes;a++)
	{ 
	    auto mesh=scene->mMeshes[a];
		numvertexinmodel=+mesh->mNumVertices;
	// Iterate through each vertex in the mesh
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		std::vector<unsigned int> adjacency; // Track adjacent vertices for current vertex

		// Check every face in the mesh
		for (unsigned int j = 0; j < mesh->mNumFaces; ++j)
		{
			const aiFace& face = mesh->mFaces[j];

			// Only process triangular faces
			if (face.mNumIndices == 3)
			{
				// Check if the current vertex is part of this face
				for (unsigned int k = 0; k < 3; ++k) {
					if (face.mIndices[k] == i) {
						// Add all vertices of this face to the adjacency list
						for (unsigned int l = 0; l < 3; ++l)
						{
							if (face.mIndices[l] != i) {

								adjncyVec.push_back(face.mIndices[l]+1);
							}
						}
					}
				}
			}
		}

		// Update xadjVec with the number of adjacent vertices found
		xadjVec.push_back(adjncyVec.size());
	}

	// Write the data out to a file

}

	std::ofstream ofs(filename);
	if (!ofs.is_open()) {
		throw std::runtime_error("Could not open file for writing: " + filename);
	}

	// Output the number of vertices, followed by the adjacency structure
	ofs << numvertexinmodel << " "<<std::endl;

 

	std::vector<unsigned int>::iterator it_2 = xadjVec.begin();
	std::vector<unsigned int>::iterator it = adjncyVec.begin();
	
	// Write adjncyVec
	for (it,it_2; it!=adjncyVec.end()-2,it_2!=xadjVec.end()-1;++it,it_2++)
	{   
	    
		ofs<<*it << " " << *(it + 1) << " " << *(it + 2) << std::endl;
	
	}
	ofs << std::endl;

	// Close the file
	ofs.close();

	
}
   

int main(int argc,char**argv)
{   
    
	//std::string inputMeshFile = "E:\uploads_files_3162983_f104starfighter.obj"; // Path to your input file
	//std::string outputMetisFile = "output.graph"; // Desired output file path
  
   Assimp::Importer imp;
   auto scene_local=imp.ReadFile(filepath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);


  
   std::string metisFilename = "OBJ.graph";

   
   writeMeshToMetisFile(scene_local,metisFilename);

   
   return 0;
 }

  



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
