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
<<<<<<< HEAD


static std::string filepath= "..\uploads_files_3162983_f104starfighter.obj";
=======
#include<utility>
#include "reactphysics3d/engine/Entity.h"
#include "reactphysics3d/engine/PhysicsWorld.h"
#include <reactphysics3d/reactphysics3d.h>
#include"assimp/quaternion.h"



static std::string filepath= "E:\uploads_files_3162983_f104starfighter.obj";


uint32_t numvertexinmodel;


using uint = unsigned int;
using uchar = unsigned char;
using ushort = unsigned short;
using luint = long unsigned int;

using int8 = std::int8_t;
using uint8 = std::uint8_t;
using int16 = std::int16_t;
using uint16 = std::uint16_t;
using int32 = std::int32_t;
using uint32 = std::uint32_t;
using int64 = std::int64_t;
using uint64 = std::uint64_t;

>>>>>>> 3cb51ec (Added Ogre-next source)



void writeMeshToMetisFile(const aiScene*scene, const std::string& filename)
{
	std::vector<unsigned int> xadjVec; // Index array for METIS
	std::vector<unsigned int> adjncyVec; // Adjacency list

	// Initialize xadj for number of vertices
	xadjVec.push_back(0); // Start at index 0

<<<<<<< HEAD
	size_t numvertexinmodel;
=======
	

>>>>>>> 3cb51ec (Added Ogre-next source)
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

   
<<<<<<< HEAD
   writeMeshToMetisFile(scene_local,metisFilename);

   
   return 0;
=======
   //writeMeshToMetisFile(scene_local,metisFilename);

   reactphysics3d::PhysicsCommon physicsCommon;

   reactphysics3d::PhysicsWorld* world = physicsCommon.createPhysicsWorld();
    
	std::vector<aiMatrix4x4>transfroms_meshes(scene_local->mNumMeshes);
	std::vector<aiQuaternion>meshes_rotation;
	std::vector<aiVector3D>position_mesh;


	uint32_t nodes_count=0;

	auto node_child = (scene_local->mRootNode->mChildren);

   while((*node_child)->mChildren!=nullptr)
   {
	   
	   node_child++;
       nodes_count++;
	   	  
	   

	   transfroms_meshes.push_back((*node_child)->mTransformation);
	   
	   
	}
	meshes_rotation.resize(102);
	position_mesh.resize(102);



   for (int i=0;i<transfroms_meshes.size();i++)
   {
       
	   transfroms_meshes[i].DecomposeNoScaling(meshes_rotation[i],position_mesh[i]);

	}

   

	std::vector<aiVector3D>mesh_array(10000000);
	std::vector<aiVector3D>normal_array(10000000);
	std::vector<unsigned int> index_array(10000000);
	uint32_t stride = 0;
	uint32 vertex_count=0;

	for (int i = 0; i < scene_local->mNumMeshes; i++)
	{
	    auto mesh=scene_local->mMeshes[i];
		vertex_count=+mesh->mNumVertices;
		
		uint32_t stride=+vertex_count;
		
		for (int j = 0; j < mesh->mNumVertices; j++)
		{   
		
			
				normal_array[i*vertex_count+j]=mesh->mVertices[j];
		}
	
			
		

	 }


	for (int i = 0; i < scene_local->mNumMeshes; i++)
	{
		auto mesh = scene_local->mMeshes[i];
		vertex_count = +mesh->mNumVertices;

		uint32_t stride = +vertex_count;

		for (int j = 0; j < mesh->mNumVertices; j++)
		{

		   
			mesh_array[i * vertex_count + j] = mesh->mNormals[j];
		}




	}



	 uint32 faces_count=0;

	for (int i = 0; i < scene_local->mNumMeshes; i++)
	{
		auto mesh = scene_local->mMeshes[i];
			

		for (int j = 0; j < mesh->mNumFaces; j++)
		{
		     faces_count+=mesh->mNumFaces;

			auto face_index= mesh->mFaces[j];

			for (int k = 0; k < 3; k++)
			{
			    
				index_array[i*mesh->mNumFaces+j+k]=face_index.mIndices[k];

			}

			
		}


	}
   

   auto vertex_array=static_cast<const void*>(mesh_array.data());

   auto index_array_react3D=static_cast<const void*>(index_array.data());

   auto normal_array_pass=static_cast<const void*>(normal_array.data());

	reactphysics3d::TriangleVertexArray array_mesh_converted(vertex_count,vertex_array,sizeof(aiVector3D),normal_array_pass,sizeof(aiVector3D),faces_count,index_array_react3D,sizeof(unsigned int),reactphysics3d::TriangleVertexArray::VertexDataType::VERTEX_DOUBLE_TYPE,reactphysics3d::TriangleVertexArray::NormalDataType::NORMAL_DOUBLE_TYPE,reactphysics3d::TriangleVertexArray::IndexDataType::INDEX_INTEGER_TYPE);

	reactphysics3d::Vector3 position(10, 20, 15);
	reactphysics3d::Quaternion orientation = reactphysics3d::Quaternion::identity();
	reactphysics3d::Transform transform(position, orientation);


	
    reactphysics3d::CollisionBody* body_2 = world->createCollisionBody(transform);


	reactphysics3d::TriangleMesh* mesh_polygon=physicsCommon.createTriangleMesh();
     
	 mesh_polygon->addSubpart(&array_mesh_converted);
  
  

  reactphysics3d::CollisionBody*body_1=world->createCollisionBody(transform);

  reactphysics3d::DynamicAABBTree tree();


 reactphysics3d::CollisionCallback  callback_collision();
 
 reactphysics3d::RaycastCallback call();

 auto intersect=world->testCollision(body_1,body_2,call);


 

	


   return 0;

>>>>>>> 3cb51ec (Added Ogre-next source)
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
