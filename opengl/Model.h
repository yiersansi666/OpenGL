#pragma once
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb_image.h"

class Model
{
public:
	Model(const char *path)
	{
		loadModel(path);
	}
	void Draw(Shader shader);
private:
	Assimp::Importer importer;
	aiScene *scene;
	std::vector<Texture> textures_loaded;
	vector<Mesh> meshes;
	string directory;
	void loadModel(string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
};
