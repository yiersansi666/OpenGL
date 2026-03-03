#pragma once
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb_image.h"

class Model
{
public:
	Model(char *path)
	{
		textures_loaded.clear();
		loadModel(path);
	}
	void Draw(Shader shader);
private:
	std::vector<Texture> textures_loaded;
	vector<Mesh> meshes;
	string directory;
	void loadModel(string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	vector<Texture> loadMatrialTextures(aiMaterial *mat, aiTextureType type, string typeName);
};
