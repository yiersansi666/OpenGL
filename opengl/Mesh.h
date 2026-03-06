#pragma once
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "Shader.h"
#include <assimp/types.h>
using namespace std;
#define MAX_BONE_INFLUENCE 4
typedef struct {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
	//bone indexes which will influence this vertex
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	//weights from each bone
	float m_Weights[MAX_BONE_INFLUENCE];
}Vertex;

typedef struct
{
	unsigned int id;
	string type;
	aiString path;  // 我们储存纹理的路径用于与其它纹理进行比较
	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	float shininess;
}Texture;

class Mesh {
public:
	/* 网格数据 */
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	Mesh(vector<Vertex> vertices, vector<unsigned int>indices, vector<Texture> textures);
	void Draw(Shader &shader);
	void DrawLight();
private:
	/* 渲染数据 */
	unsigned int VAO, VBO, EBO, Norma;
	/* 函数 */
	void setupMesh();
};