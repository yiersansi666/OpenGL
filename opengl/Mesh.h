#pragma once
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "Shader.h"
#include <assimp/types.h>
using namespace std;

typedef struct {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
}Vertex;

typedef struct
{
	unsigned int id;
	string type;
	aiString path;  // 我们储存纹理的路径用于与其它纹理进行比较
}Texture;

class Mesh {
public:
	/* 网格数据 */
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	Mesh(vector<Vertex> vertices, vector<unsigned int>indices, vector<Texture> textures);
	void Draw(Shader &shader);
private:
	/* 渲染数据 */
	unsigned int VAO, VBO, EBO;
	/* 函数 */
	void setupMesh();
};