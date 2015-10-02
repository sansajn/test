// material extraction sample
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

	cout << "materials:" << scene->mNumMaterials << "\n";
	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		aiMaterial * material = scene->mMaterials[i];
		cout << "properties:" << material->mNumProperties << " ";

		for (unsigned j = 0; j < material->mNumProperties; ++j)
		{
			aiMaterialProperty * property = material->mProperties[j];
			cout << property->mKey.C_Str() << ":";
			switch (property->mType)
			{
				case aiPTI_Float:	cout << "float"; break;

				case aiPTI_String:
				{
					aiString str;
					aiGetMaterialString(material, property->mKey.C_Str(), 0, 0, &str);
					cout << str.C_Str();
					break;
				}

				case aiPTI_Integer: cout << "integer"; break;
				case aiPTI_Buffer: cout << "buffer"; break;
			}
			cout << ", ";
		}
		cout << "\n";

		string type_names[] = {"none", "diffuse", "specular", "ambient", "emissive",
			"height", "normals", "shininess", "opacity", "displacement", "lightmap",
			"reflection", "unknown"};

		for (unsigned j = aiTextureType_NONE; j < aiTextureType_UNKNOWN+1; ++j)
		{
			int texture_cout = material->GetTextureCount((aiTextureType)j);
			if (texture_cout == 0)
				continue;

			cout << "  " << type_names[j] << "-textures:" << texture_cout;
			for (int k = 0; k < texture_cout; ++k)
			{
				aiString path;
				material->GetTexture((aiTextureType)j, k, &path);
				cout << ", " << path.C_Str();
			}
			cout << "\n";
		}
	}

	assert(scene);

	return 0;
}
