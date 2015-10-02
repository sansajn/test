// extrahuje nazvy textur
#include <string>
#include <iostream>
#include <cassert>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/material.h>

using std::string;
using std::cout;

string const mesh_path = "blaster_view.md5mesh";

int main(int argc, char * argv[])
{
	Assimp::Importer importer;
	aiScene const * scene = importer.ReadFile(mesh_path, 0);

	cout << "textures\n";
	for (int i = 0; i < scene->mNumMaterials; ++i)
	{
		aiMaterial * material = scene->mMaterials[i];
		aiString str;
		material->Get(AI_MATKEY_NAME, str);
		cout << "  " << str.C_Str() << "\n";
	}

	return 0;
}
