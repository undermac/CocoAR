#ifndef MESH_H
#define MESH_H

//#include "base.h"

#include "MDLModel.h"

struct Obj_Material
{	
	float diffuse[4];   // Kd
	float ambient[4];   // Ka
	float specular[4];  // Ks
	float no_shininess;
	float emission[4];  // Ke
		
	GLuint textureID;
	bool hasAlpha;
	bool autoIlum;	
};

struct Obj_Vertex
{
	cocos2d::ccVertex3F vertices;
	cocos2d::_ccTex2F texCoords1;
	cocos2d::_ccTex2F texCoords2;
	cocos2d::ccVertex3F normal;	
	bool contentEquals(Obj_Vertex *v);	
	Obj_Vertex *getCopy();
};

class my_MaterialVBO
{
public:
	std::string m_strName;
	Obj_Material m_Material;

	my_MaterialVBO() {
		m_strName="empty";
		m_uiVertexVBO=0;
		m_iRecycled=0;
		m_uiIndexVBO=0;
		m_iNumTriangles=0;
		m_iNumVertex=0;
		m_iNumVertex=0;

		m_Material.hasAlpha=false;
		m_Material.autoIlum=false;

	}

	std::vector<Obj_Vertex *> m_pTempVtx;		
	std::vector<GLuint> m_vecTempIDX;

	void InitVBO();

	//------ Vertex VBO -----------------
	GLuint m_uiVertexVBO;
	GLushort m_iNumVertex;

	//------ TRIANGLES INDEX -----------------
	GLuint m_uiIndexVBO;
	GLushort m_iNumTriangles;

	void SetupMaterial(Obj_Material *pMaterial);
	void setMaterial();

	int m_iRecycled;
	int FindVertex(Obj_Vertex *pVertexFinal);
	int AddVertex(Obj_Vertex *pVertexTemporal);
	void RenderVBO();
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	bool m_bVBOInit;
	void InitVBO();

	std::vector<cocos2d::ccVertex3F> m_vecTempVertex;		
	std::vector<cocos2d::ccVertex3F> m_vecNormals;		
	std::vector<cocos2d::_ccTex2F> m_vecTextCoordsA;
	std::vector<cocos2d::_ccTex2F> m_vecTextCoordsB;
	
	std::map<std::string, my_MaterialVBO *> m_pVBOMaterialMap;	
	void LoadMaterialList(char* filename, char *orgPath);
	void LoadMaterial(char* filename);
	my_MaterialVBO *m_pCurrentMaterial;
	
	
	void LoadModel(char* filename);	
	void SetMaterial(float r, float g, float b, float a);
	void SetAutoIlum(int set);
	virtual void Draw();
	virtual void DrawTriangles();
	void MorphWith(Mesh* other, Mesh* dest, float p);
	void DisplaceUV(float U, float V);
	
protected:	
	Obj_Material m_material;
};


#endif