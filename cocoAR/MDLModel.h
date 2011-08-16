#ifndef _MDLMODEL_H_
#define _MDLMODEL_H_

#include "MdlFormat.h"
#include "Geo3DObject.h"
#include <map>
#include <vector>

namespace   cocos2d {

	struct VertexBones 
	{
		ccVertex3F vertices;
		ccColor4B colors;		    
		_ccTex2F texCoords;
		GLfloat BoneWeight;
		unsigned char BoneIndices;
		unsigned short Padding1;
		bool contentEquals(VertexBones *v);
		VertexBones *getCopy();
	};

}

//---------------------------------------------------------------------------------------
class myHardwareVBO;

class myTriangleList
{
public:	
	unsigned char m_iMaxMatrices;
	unsigned char *m_iMatrixSetup;  // Array containing what to index on the matrix list before rendering

	GLuint m_uiIndexVBO;
	GLushort m_iNumTriangles;
	GLushort *m_pIndices;
};

class myMeshVBO
{
public:	
	tagMDLMesh *m_pMeshPtr;
	tagMDLModel	*m_pModel;

	bool m_bVBOInit;
	cocos2d::VertexBones *m_pVBOVertices;
	std::vector<cocos2d::VertexBones *> m_pTempVtx;
	GLuint m_uiVertexVBO; 	
	
	//------------------------------------------------------------------------------
	myHardwareVBO *m_pHardware;		

	//--------------------------------------
	// Temp triangle list 
	myTriangleList *m_currentTriangle;
	std::vector<GLuint> m_pTempIDX;
	void setPrimitive(int iRenderingMode);
	void closeTriangleList();
	//--------------------------------------

	std::vector<myTriangleList *> m_pTriangleList;
	int m_iRecycled;
	bool m_bFirstPass;
	int FindVertex(cocos2d::VertexBones *pVertexFinal);

	myMeshVBO() {		
		m_currentTriangle=new myTriangleList();
		m_bFirstPass=false;
		m_iRecycled=0;
		m_pModel=NULL;
		m_pVBOVertices=NULL;
		m_uiVertexVBO=0;
	}	

	~myMeshVBO() {
	}

	void InitVBO();

	static void StartVBO(); // Just rendering states
	static void CloseVBO(); // Close the rendering

	void RenderVBO();
	int AddVertex(cocos2d::VertexBones *pVertexFinal, int numVertex);
};	

//---------------------------------------------------------------------------------------

class myHardwareVBO
{
public:	
	bool m_bVBOInit;
	void InitVBO();

public:	

	bool m_bHardwareInit;
	tagMDLModel	*m_pModel;	

	GLuint m_vertexVBOId; 
	cocos2d::VertexBones *m_pVertices;

	//---------------------------------------------------------------------

	std::map<long, myMeshVBO *> m_pVBOMeshMap;	
	myMeshVBO *getMeshVBO(tagMDLMesh *pMesh);
	void RenderVBO();

	//---------------------------------------------------------------------

	GLuint *m_pIndexBuffersVBO;

	myHardwareVBO() {
		m_bVBOInit=false;
		m_bHardwareInit=false;
		m_pModel=NULL;
		m_pVertices=NULL;
		m_vertexVBOId=0;
	}

	~myHardwareVBO() {
	}
	
	void init(tagMDLHeader	*Header, tagMDLModel *Model);
	void draw();
};


class TMDLModel: public CCARGeneric3DModel
{
	std::vector<myMeshVBO> m_vecMeshVBO;

protected:
	vec3_t			Origin;					// Origin
	vec3_t			Rotation;				// Rotation
	long			CurrentSequence;		// Current sequence
	float			CurrentFrame;			// Current frame
	long			CurrentBodyPart;		// Current body part
	long			CurrentSkin;			// Current skin
	BYTE			Controller[4];			// Controllers
	BYTE			Blending[2];			// Animation blending
	BYTE			MouthPosition;			// Mouth position
	tagMDLHeader	*Header;				// Model header
	tagMDLModel		*Model;					// Model
	tagMDLHeader	*TextureHeader;			// Texture
	tagMDLSeqHeader	*AnimationHeader[32];	// Animation header

public:
  TMDLModel();
  TMDLModel(std::string modelName);
	void Init(char *Filename);
  void Draw();
  void draw3D();
	void DrawModel();
	void AdvanceFrame(float Time);
	void ExtractBoundBox(float *Minimums, float *Maximums);
	long SetSequence(long Sequence);
	long GetSequence();
	void GetSequenceInfo(float *FrameRate, float *GroundSpeed);
	float SetController(long ControllerIndex, float Value);
	float SetMouth(float Value);
	float SetBlending(long Blender, float Value);
	long SetBodyGroup(long Group, long Value);
	long SetSkin(long Value);
//	void LoadBoneIndex(float *modelview, int boneIndex, int iSlot);

	//--- HARDWARE DATA -----------------------
	std::map<long, myHardwareVBO *> m_VBOMap;	
	myHardwareVBO *getModelVBO(tagMDLModel *Model);
	//--- HARDWARE DATA -----------------------

	void BuildVBO();

protected:
	tagMDLHeader *LoadModel(char *Filename);
	tagMDLSeqHeader *LoadDemandSequences(char *Filename);
	void CalcBoneQuaternion(long Frame, float Value, tagMDLBone *Bone, tagAnimation *Anim, float *Q);
	void CalcBonePosition(long Frame, float Value, tagMDLBone *Bone, tagAnimation *Anim, float *Pos);
	void CalcRotations (vec3_t *Pos, vec4_t *Q, tagMDLSeqDescription *SeqDescription, tagAnimation *Anim, float FrameValue);
	tagAnimation *GetAnim(tagMDLSeqDescription *SeqDescription);
	void SlerpBones(vec4_t Q1[], vec3_t Pos1[], vec4_t Q2[], vec3_t Pos2[], float Value);
	void SetUpBones();
	void DrawBones();
	void DrawPoints(int bodyPart);
	void Lighting(float *lv, long Bone, long Flags, vec3_t Normal);
	void SetupLighting();
	void SetupModel(long BodyPart);
	void UploadTexture(tagMDLTexture *Texture, BYTE *Data, BYTE *Palette);
};

#endif