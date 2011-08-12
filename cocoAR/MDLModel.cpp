#define _CRT_SECURE_NO_WARNINGS 
#define GL_GLEXT_PROTOTYPES

#include "cocos2d.h"
#include <math.h>

#include "MdlModel.h"

#define GL_GLEXT_PROTOTYPES

//#include "gles\gl.h"
//#include "gles\glext.h"

#include "vector.h"
#include "matrix.h"
#include "quaternion.h"
#include "ResourcesPath.h"

/*
void CCLOG(const char  *format, ...)
{
	char buffer[1000];
	va_list argptr;
	va_start(argptr, format);
	vsprintf_s(buffer, format, argptr);
	va_end(argptr);
	OutputDebugStringA(buffer);
	OutputDebugStringA("\n");
}
*/

#include "MdlModel.h"

//-----------------------------------------------------------------------------------
// Pallete precalculation temporals 
int g_iMaxMatrices=10;
int g_iMatrixPalleteSize=0;

unsigned char g_iMatrixPalleteIndex[MAXBONES];
bool g_bMatrixPallete[MAXBONES];
unsigned char g_iMatrixPalleteFinal[MAXBONES];

Matrix model_view; 

void InitPalleteIndex()
{
	g_iMatrixPalleteSize=0;
	memset(g_iMatrixPalleteIndex,0,sizeof(g_iMatrixPalleteIndex));
	memset(g_iMatrixPalleteFinal,0,sizeof(g_iMatrixPalleteFinal));
	memset(g_bMatrixPallete,0,sizeof(g_bMatrixPallete));
}

float	BoneTransforms[MAXBONES][3][4];	// Bone transformation matrix


void LoadBoneIndex(float *modelview, int boneIndex, int iSlot)
{
	glCurrentPaletteMatrixOES(iSlot);
	float fm[16];		
	fm[0]=BoneTransforms[boneIndex][0][0]; fm[4]=BoneTransforms[boneIndex][0][1]; fm[8]=BoneTransforms[boneIndex][0][2]; fm[12]=BoneTransforms[boneIndex][0][3];
	fm[1]=BoneTransforms[boneIndex][1][0]; fm[5]=BoneTransforms[boneIndex][1][1]; fm[9]=BoneTransforms[boneIndex][1][2]; fm[13]=BoneTransforms[boneIndex][1][3];
	fm[2]=BoneTransforms[boneIndex][2][0]; fm[6]=BoneTransforms[boneIndex][2][1]; fm[10]=BoneTransforms[boneIndex][2][2]; fm[14]=BoneTransforms[boneIndex][2][3];		
	fm[3]=0.0f; fm[7]=0.0f; fm[11]=0.0f; fm[15]=1.0f;
	//printMatrix(fm);
	glLoadMatrixf(modelview);
	glMultMatrixf(fm);
}

//-----------------------------------------------------------------------------------


typedef float CGFloat;

class CCPoint
{
public:
	float x;
	float y;

public:

	CCPoint();
	CCPoint(float x, float y);

public:
	static bool CCPointEqualToPoint(const CCPoint& point1, const CCPoint& point2);
};

void DrawPoint(float originx, float originy, float originz)
{
	float vertices[3] =  {originx, originy,originz};    

	// Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	// Needed states: GL_VERTEX_ARRAY, 
	// Unneeded states: GL_TEXTURE_2D, GL_TEXTURE_COORD_ARRAY, GL_COLOR_ARRAY	
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertices);	
	glDrawArrays(GL_POINTS, 0, 1);

	// restore default state
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_TEXTURE_2D);	
}

void DrawLine(float originx, float originy, float originz, float destinationx, float destinationy, float destinationz)
{
	float vertices[6] =  {originx, originy,originz, destinationx, destinationy, destinationz };    
	glEnableClientState(GL_VERTEX_ARRAY);

	// Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	// Needed states: GL_VERTEX_ARRAY, 
	// Unneeded states: GL_TEXTURE_2D, GL_TEXTURE_COORD_ARRAY, GL_COLOR_ARRAY	
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertices);	
	glDrawArrays(GL_LINES, 0, 2);

	// restore default state
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);	
}


GLuint CurrentTexture	= 1;
vec3_t	TransformVertices[MAXVERTICES];	// Transformed vertices
vec3_t	LightValues[MAXVERTICES];		// Light surface normals
vec3_t	*TransformVertPtr;
vec3_t	*LightValuesPtr;
vec3_t	LightVector;					// Light vector in model reference frame
vec3_t	BoneLightVector[MAXBONES];		// Light vectors in bone reference frames
long	AmbientLightColor;				// Ambient world light
float	ShadeLight;						// Direct world light
vec3_t	LightColor;

//----------------------------------------------------------------------------
float	OES_BoneMatrix[MAXBONES][16];	// Bone transformation matrix for 


void TMDLModel::Init(char *Filename)
{ 
	int numUnits;
	glGetIntegerv(GL_MAX_VERTEX_UNITS_OES, &numUnits);
	CCLOG(" numUnits %i ",numUnits);

	glGetIntegerv(GL_MAX_PALETTE_MATRICES_OES, &g_iMaxMatrices); 
	CCLOG(" maxMatrices %i ",g_iMaxMatrices);

	char	TextureName[256];
	char	SeqGroupName[256];

	std::string myPath=ResourcesPath::getTemporalFile(Filename);

	CCLOG(" LOAD MODEL %s ",Filename);
	Header	= LoadModel((char *) myPath.c_str());

	CCLOG(" LOAD HEADER ");
	
//	Header	= LoadModel(Filename);

	if (0 == Header->NumTextures) {
		strcpy(TextureName, Filename);
		strcpy(&TextureName[strlen(TextureName) - 4], "t.mdl");
		
		myPath=ResourcesPath::getTemporalFile(TextureName);

		CCLOG(" LOAD TEXTURE %s ",myPath.c_str());
		TextureHeader	= LoadModel((char *)myPath.c_str());
		CCLOG(" OK ");
		//TextureHeader	= LoadModel(TextureName);
	} else {
		TextureHeader	= Header;
	}

	if (Header->NumSeqGroups > 1) {
		for (long Loop = 1; Loop < Header->NumSeqGroups; Loop++)
		{
			strcpy(SeqGroupName, Filename);
			sprintf(&SeqGroupName[strlen(SeqGroupName) - 4], "%02d.mdl", Loop);
			AnimationHeader[Loop]	= LoadDemandSequences(SeqGroupName);
		}
	}

	CCLOG(" END LOAD ");
}

void TMDLModel::Draw()
{
  DrawModel();
}

void TMDLModel::DrawModel()
{
	TransformVertPtr	= &TransformVertices[0];
	LightValuesPtr		= &LightValues[0];

	if (0 == Header->NumBodyParts) {		
		return;
	}

	glPushMatrix();

	glTranslatef(Origin[0], Origin[1], Origin[2]);

	glRotatef(Rotation[1], 0, 0, 1);
	glRotatef(Rotation[0], 0, 1, 0);
	glRotatef(Rotation[2], 1, 0, 0);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glGetFloatv(GL_MODELVIEW_MATRIX, model_view.m);   

	SetUpBones();
	SetupLighting();

	for (long Loop = 0; Loop < Header->NumBodyParts; Loop++) {
		SetupModel(Loop);
		DrawPoints(Loop);	
	}

	//DrawBones();
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glPopMatrix();
}

void TMDLModel::AdvanceFrame(float Time)
{
	tagMDLSeqDescription	*SeqDescription	= (tagMDLSeqDescription *)((BYTE *)Header + 
		Header->SequenceOffset) + CurrentSequence;

	if (Time > 0.1f) {
		Time	= 0.1f;
	}

	CurrentFrame	+= Time * SeqDescription->Timing;

	if (SeqDescription->NumFrames <= 1) {
		CurrentFrame	= 0;
	} else {
		CurrentFrame	-= (long)(CurrentFrame / (SeqDescription->NumFrames - 1)) * 
			(SeqDescription->NumFrames - 1);
	}
}

void TMDLModel::ExtractBoundBox(float *Minimums, float *Maximums)
{
	tagMDLSeqDescription	*SeqDescription	= (tagMDLSeqDescription *)((BYTE *)Header + 
		Header->SequenceOffset);

	Minimums[0]	= SeqDescription[CurrentSequence].BoundingBoxMinimum[0];
	Minimums[1]	= SeqDescription[CurrentSequence].BoundingBoxMinimum[1];
	Minimums[2]	= SeqDescription[CurrentSequence].BoundingBoxMinimum[2];

	Maximums[0]	= SeqDescription[CurrentSequence].BoundingBoxMaximum[0];
	Maximums[1]	= SeqDescription[CurrentSequence].BoundingBoxMaximum[1];
	Maximums[2]	= SeqDescription[CurrentSequence].BoundingBoxMaximum[2];
}

long TMDLModel::SetSequence(long Sequence)
{
	if (Sequence > Header->NumSequences) {
		Sequence	= 0;
	} else if (Sequence < 0) {
		Sequence	= Header->NumSequences - 1;
	}

	CurrentSequence	= Sequence;
	CurrentFrame	= 0;
	return	CurrentSequence;
}

long TMDLModel::GetSequence()
{
	return	CurrentSequence;
}

void TMDLModel::GetSequenceInfo(float *FrameRate, float *GroundSpeed)
{
	tagMDLSeqDescription	*SeqDescription	= (tagMDLSeqDescription *)((BYTE *)Header + 
		Header->SequenceOffset) + CurrentSequence;

	if (SeqDescription->NumFrames > 1)
	{
		*FrameRate		= 256 * SeqDescription->Timing / (SeqDescription->NumFrames - 1);
		*GroundSpeed	= (float)sqrt(SeqDescription->LinearMovement[0] * 
			SeqDescription->LinearMovement[0] + SeqDescription->LinearMovement[1] * 
			SeqDescription->LinearMovement[1] + SeqDescription->LinearMovement[2] * 
			SeqDescription->LinearMovement[2]);

		*GroundSpeed	= *GroundSpeed * SeqDescription->Timing / (SeqDescription->NumFrames - 1);
	}

	else
	{
		*FrameRate		= 256.0f;
		*GroundSpeed	= 0.0f;
	}
}

float TMDLModel::SetController(long ControllerIndex, float Value)
{
	tagMDLBoneController	*BoneController	= 
		(tagMDLBoneController *)((BYTE *)Header + Header->BoneControllerOffset);

	long Loop;
	for (Loop = 0; Loop < Header->NumBoneControllers; Loop++, BoneController++)
	{
		if (BoneController->Index == ControllerIndex)
		{			
			break;
		}
	}

	if (Loop >= Header->NumBoneControllers) {
		return	Value;
	}

	if (BoneController->Type & (TRANSITION_XR | TRANSITION_YR | TRANSITION_ZR))
	{
		if (BoneController->End < BoneController->Start) {
			Value	= -Value;
		}

		if (BoneController->Start + 359.0f >= BoneController->End) {
			if (Value > ((BoneController->Start + BoneController->End) / 2.0f) + 180) {
				Value = Value - 360.0f;
			}

			if (Value < ((BoneController->Start + BoneController->End) / 2.0f) - 180) {				
				Value = Value + 360.0f;
			}
		}

		else
		{
			if (Value > 360.0f) {
				Value	= Value - (long)(Value / 360.0f) * 360.0f;
			}

			else if (Value < 0.0f) {
				Value	= Value + (long)((Value / -360.0f) + 1.0f) * 360.0f;
			}
		}
	}

	long	Setting	= (long)(255.0f * (Value - BoneController->Start) / (BoneController->End - 
		BoneController->Start));

	if (Setting < 0) 
		Setting	= 0;	
	else if (Setting > 255)	
		Setting	= 255;	

	Controller[ControllerIndex]	= (BYTE)Setting;

	return	Setting * (1.0f / 255.0f) * (BoneController->End - BoneController->Start) + 
		BoneController->Start;
}

float TMDLModel::SetMouth(float Value)
{
	tagMDLBoneController	*BoneController = 
		(tagMDLBoneController *)((BYTE *)Header + Header->BoneControllerOffset);

	for (long Loop = 0; Loop < Header->NumBoneControllers; Loop++, BoneController++)
	{
		if (4 == BoneController->Index)
		{
			break;
		}
	}

	if (BoneController->Type & (TRANSITION_XR | TRANSITION_YR | TRANSITION_ZR))
	{
		if (BoneController->End < BoneController->Start)
		{
			Value = -Value;
		}

		if (BoneController->Start + 359.0f >= BoneController->End)
		{
			if (Value > ((BoneController->Start + BoneController->End) / 2.0f) + 180.0f)
			{				
				Value = Value - 360.0f;
			}

			if (Value < ((BoneController->Start + BoneController->End) / 2.0f) - 180.0f)
			{
				Value = Value + 360.0f;
			}
		}

		else
		{
			if (Value > 360.0f) 
			{
				Value	= Value - (long)(Value / 360.0f) * 360.0f;
			}

			else if (Value < 0.0f)
			{
				Value	= Value + (long)((Value / -360.0f) + 1.0f) * 360.0f;
			}
		}
	}

	long	Setting	= (long)(64.0f * (Value - BoneController->Start) / (BoneController->End - 
		BoneController->Start));

	if (Setting < 0)
		Setting	= 0;

	if (Setting > 64)
		Setting	= 64;

	MouthPosition	= (BYTE)Setting;

	return	Setting * (1.0f / 64.0f) * (BoneController->End - BoneController->Start) + 
		BoneController->Start;
}

float TMDLModel::SetBlending(long Blender, float Value)
{
	tagMDLSeqDescription	*SeqDescription	= 
		(tagMDLSeqDescription *)((BYTE *)Header + Header->SequenceOffset) + CurrentSequence;

	if (0 == SeqDescription->BlendType[Blender])
	{
		return	Value;
	}

	if (SeqDescription->BlendType[Blender] & (TRANSITION_XR | TRANSITION_YR | TRANSITION_ZR))
	{
		if (SeqDescription->BlendEnd[Blender] < SeqDescription->BlendStart[Blender])
		{
			Value = -Value;
		}

		if (SeqDescription->BlendStart[Blender] + 359.0f >= SeqDescription->BlendEnd[Blender])
		{
			if (Value > ((SeqDescription->BlendStart[Blender] + 
				SeqDescription->BlendEnd[Blender]) / 2.0f) + 180.0f)
			{
				Value = Value - 360.0f;
			}

			if (Value < ((SeqDescription->BlendStart[Blender] + 
				SeqDescription->BlendEnd[Blender]) / 2.0f) - 180.0f)
			{
				Value = Value + 360.0f;
			}
		}
	}

	long	Setting	= (long)(255.0f * (Value - SeqDescription->BlendStart[Blender]) / 
		(SeqDescription->BlendEnd[Blender] - SeqDescription->BlendStart[Blender]));

	if (Setting < 0)
		Setting	= 0;

	if (Setting > 255)
		Setting = 255;

	Blending[Blender]	= (BYTE)Setting;

	return	Setting * (1.0f / 255.0f) * (SeqDescription->BlendEnd[Blender] - 
		SeqDescription->BlendStart[Blender]) + SeqDescription->BlendStart[Blender];
}

long TMDLModel::SetBodyGroup(long Group, long Value)
{
	if (Group > Header->NumBodyParts)
		return -1;

	tagMDLBodyPart	*BodyPart	= (tagMDLBodyPart *)((BYTE *)Header + Header->BodyPartOffset) + Group;

	long	Current	= (CurrentBodyPart / BodyPart->Base) % BodyPart->NumModels;

	if (Value >= BodyPart->NumModels)
		return	Current;

	CurrentBodyPart	= (CurrentBodyPart - (Current * BodyPart->Base) + (Value * BodyPart->Base));
	return	Value;
}

long TMDLModel::SetSkin(long Value)
{
	if (Value < Header->NumSkinFamilies)
		return	CurrentSkin;

	CurrentSkin	= Value;
	return	Value;
}

tagMDLHeader *TMDLModel::LoadModel(char *Filename)
{
	FILE	*Handle;
	long	Size;
	BYTE	*Buffer;

	Handle	= fopen(Filename, "rb");

	if (NULL == Handle) {
		printf("Unable to open %s\n", Filename);		
		exit(1);
	}

	fseek(Handle, 0, SEEK_END);
	Size	= ftell(Handle);	

	rewind(Handle);

	Buffer	= new BYTE[Size];

	if (NULL == Buffer){
		printf("Unable to allocate memory\n");		
		exit(1);
	}

	fread(Buffer, Size, 1, Handle);
	fclose(Handle);

	tagMDLHeader	*MDLHeader	= (tagMDLHeader *)Buffer;
	tagMDLTexture	*Texture	= (tagMDLTexture *)(Buffer + MDLHeader->TextureOffset);

	if (MDLHeader->TextureOffset != 0)
	{
		for (long Loop = 0; Loop < MDLHeader->NumTextures; Loop++)
		{
			UploadTexture(&Texture[Loop], Buffer + Texture[Loop].Index, Buffer + 
				Texture[Loop].Width * Texture[Loop].Height + Texture[Loop].Index);
		}
	}
  m_vtxMin.x = MDLHeader->BoundingBoxMinimum[0];		// Bounding box	
	m_vtxMin.y = MDLHeader->BoundingBoxMinimum[1];	
  m_vtxMin.z = MDLHeader->BoundingBoxMinimum[2];

	m_vtxMax.x = MDLHeader->BoundingBoxMaximum[0];		// Bounding box	
	m_vtxMax.y = MDLHeader->BoundingBoxMaximum[1];	
  m_vtxMax.z = MDLHeader->BoundingBoxMaximum[2];
	
  return	(tagMDLHeader *)Buffer;
}

tagMDLSeqHeader *TMDLModel::LoadDemandSequences(char *Filename)
{
	FILE	*Handle;
	long	Size;
	BYTE	*Buffer;

	Handle	= fopen(Filename, "rb");

	if (NULL == Handle) {
		printf("Unable to open %s\n", Filename);		
		exit(1);
	}

	fseek(Handle, 0, SEEK_END);
	Size	= ftell(Handle);	

	rewind(Handle);

	Buffer	= new BYTE[Size];

	if (NULL == Buffer)	{
		printf("Unable to allocate memory\n");		
		exit(1);
	}

	fread(Buffer, Size, 1, Handle);
	fclose(Handle);

	return	(tagMDLSeqHeader *)Buffer;
}

TMDLModel::TMDLModel(std::string modelName){
  modelName = modelName;
  TMDLModel();
}
TMDLModel::TMDLModel(){}

void TMDLModel::CalcBoneQuaternion(long Frame, float Value, tagMDLBone *Bone, tagAnimation *Anim, 
								   float *Q)
{
	vec3_t			Angle1;
	vec3_t			Angle2;
	tagMDLAnimFrame	*AnimValue;

	for (long Loop = 0; Loop < 3; Loop++)
	{

		if (0 == Anim->Offset[Loop + 3]) {
			Angle2[Loop]	= Angle1[Loop]	= Bone->Value[Loop + 3];
		}

		else
		{
			AnimValue	= (tagMDLAnimFrame *)((BYTE *)Anim + Anim->Offset[Loop + 3]);

			long Index	= Frame;

			while (AnimValue->Total <= Index)
			{
				Index		-= AnimValue->Total;
				AnimValue	+= AnimValue->Valid + 1;
			}

			if (AnimValue->Valid > Index)
			{
				Angle1[Loop]	= AnimValue[Index + 1].Value;
				if (AnimValue->Valid > Index + 1) {
					Angle2[Loop]	= AnimValue[Index + 2].Value;
				} else {
					if (AnimValue->Total > Index + 1) {
						Angle2[Loop]	= Angle1[Loop];
					} else {						
						Angle2[Loop]	= AnimValue[AnimValue->Valid + 2].Value;
					}
				}
			}

			else
			{
				Angle1[Loop]	= AnimValue[AnimValue->Valid].Value;

				if (AnimValue->Total > Index + 1)
				{
					Angle2[Loop]	= Angle1[Loop];
				}

				else
				{
					Angle2[Loop]	= AnimValue[AnimValue->Valid + 2].Value;
				}
			}

			Angle1[Loop]	= Bone->Value[Loop + 3] + Angle1[Loop] * Bone->Scale[Loop + 3];
			Angle2[Loop]	= Bone->Value[Loop + 3] + Angle2[Loop] * Bone->Scale[Loop + 3];
		}
	}

	if (!VectorCompare(Angle1, Angle2))
	{
		vec4_t	Q1;
		vec4_t	Q2;

		AngleQuaternion(Angle1, Q1);
		AngleQuaternion(Angle2, Q2);
		QuaternionSlerp(Q1, Q2, Value, Q);
	}

	else
	{
		AngleQuaternion(Angle1, Q);
	}
}

void TMDLModel::CalcBonePosition(long Frame, float Value, tagMDLBone *Bone, tagAnimation *Anim, 
								 float *Pos)
{
	tagMDLAnimFrame	*AnimValue;

	for (long Loop = 0; Loop < 3; Loop++)
	{
		Pos[Loop]	= Bone->Value[Loop];

		if (Anim->Offset[Loop] != 0)
		{
			AnimValue	= (tagMDLAnimFrame *)((BYTE *)Anim + Anim->Offset[Loop]);

			long	Index	= Frame;

			while (AnimValue->Total <= Index)
			{
				Index		-= AnimValue->Total;
				AnimValue	+= AnimValue->Valid + 1;
			}

			if (AnimValue->Valid > Index)
			{
				if (AnimValue->Valid > Index + 1)
				{
					Pos[Loop]	+= (AnimValue[Index + 1].Value * (1.0f - Value) + Value * 
						AnimValue[Index + 2].Value) * Bone->Scale[Loop];
				}

				else
				{
					Pos[Loop]	+= AnimValue[Index + 1].Value * Bone->Scale[Loop];
				}
			}

			else
			{
				if (AnimValue->Total <= Index + 1)
				{
					Pos[Loop]	+= (AnimValue[AnimValue->Valid].Value * (1.0f - Value) + Value * 
						AnimValue[AnimValue->Valid + 2].Value) * Bone->Scale[Loop];
				}

				else
				{
					Pos[Loop]	+= AnimValue[AnimValue->Valid].Value * Bone->Scale[Loop];
				}
			}
		}
	}
}

void TMDLModel::CalcRotations(vec3_t *Pos, vec4_t *Q, tagMDLSeqDescription *SeqDescription, 
							  tagAnimation *Anim, float FrameValue)
{
	long		Frame;
	float		Fractional;

	Frame		= (long)FrameValue;
	Fractional	= (FrameValue - Frame);

	tagMDLBone	*Bone	= (tagMDLBone *)((BYTE *)Header + Header->BoneOffset);

	for (long Loop = 0; Loop < Header->NumBones; Loop++, Bone++, Anim++) 
	{
		CalcBoneQuaternion(Frame, Fractional, Bone, Anim, Q[Loop]);
		CalcBonePosition(Frame, Fractional, Bone, Anim, Pos[Loop]);
	}

	if (SeqDescription->MotionType & TRANSITION_X) {
		Pos[SeqDescription->MotionBone][0]	= 0.0f;
	}

	if (SeqDescription->MotionType & TRANSITION_Y) {
		Pos[SeqDescription->MotionBone][1]	= 0.0f;
	}

	if (SeqDescription->MotionType & TRANSITION_Z) {
		Pos[SeqDescription->MotionBone][2]	= 0.0f;
	}
}

tagAnimation *TMDLModel::GetAnim(tagMDLSeqDescription *SeqDescription)
{
	tagMDLSeqGroup	*pseqgroup	= (tagMDLSeqGroup *)((BYTE *)Header + 
		Header->SeqGroupOffset) + SeqDescription->SeqGroup;

	if (SeqDescription->SeqGroup == 0)
	{
		return	(tagAnimation *)((BYTE *)Header + pseqgroup->Data + 
			SeqDescription->AnimOffset);
	}

	return	(tagAnimation *)((BYTE *)AnimationHeader[SeqDescription->SeqGroup] + 
		SeqDescription->AnimOffset);
}

void TMDLModel::SlerpBones(vec4_t Q1[], vec3_t Pos1[], vec4_t Q2[], vec3_t Pos2[], float Value)
{
	vec4_t	Q3;
	float	Inverse;

	if (Value < 0.0f)
	{
		Value	= 0.0f;
	}

	else if (Value > 1.0f)
	{
		Value	= 1.0f;
	}

	Inverse	= 1.0f - Value;

	for (long Loop = 0; Loop < Header->NumBones; Loop++)
	{
		QuaternionSlerp(Q1[Loop], Q2[Loop], Value, Q3);
		Q1[Loop][0]		= Q3[0];
		Q1[Loop][1]		= Q3[1];
		Q1[Loop][2]		= Q3[2];
		Q1[Loop][3]		= Q3[3];
		Pos1[Loop][0]	= Pos1[Loop][0] * Inverse + Pos2[Loop][0] * Value;
		Pos1[Loop][1]	= Pos1[Loop][1] * Inverse + Pos2[Loop][1] * Value;
		Pos1[Loop][2]	= Pos1[Loop][2] * Inverse + Pos2[Loop][2] * Value;
	}
}

extern GLfloat xrot3;
extern GLfloat yrot3;
extern GLfloat zrot3;

void getQuaternionMatrix(float x,float y,float z,float w, float *m16) 
{
	float x2 = x * x;
	float y2 = y * y;
	float z2 = z * z;
	float xy = x * y;
	float xz = x * z;
	float yz = y * z;
	float wx = w * x;
	float wy = w * y;
	float wz = w * z;

	// This calculation would be a lot more complicated for non-unit length quaternions
	// Note: The constructor of Matrix4 expects the Matrix in column-major format like expected by
	//   OpenGL
	/*
	fm[0]=1.0f; fm[4]=0.0f; fm[8]=0.0f; fm[12]=BoneTransforms[boneIndex][0][3];
	fm[1]=0.0f; fm[5]=1.0f; fm[9]=0.0f; fm[13]=BoneTransforms[boneIndex][1][3];
	fm[2]=0.0f; fm[6]=0.0f; fm[10]=1.0f; fm[14]=BoneTransforms[boneIndex][2][3];
	fm[3]=0.0f; fm[7]=0.0f; fm[11]=0.0f; fm[15]=1.0f;
	*/

	m16[0]=1.0f - 2.0f * (y2 + z2);
	m16[4]=2.0f * (xy - wz);
	m16[8]=2.0f * (xz + wy);
	m16[12]=0.0f;

	m16[1]=2.0f * (xy + wz);
	m16[5]=1.0f - 2.0f * (x2 + z2);
	m16[9]=2.0f * (yz - wx);
	m16[13]=0.0f;

	m16[2]=2.0f * (xz - wy);
	m16[6]=2.0f * (yz + wx);
	m16[10]=1.0f - 2.0f * (x2 + y2);
	m16[14]=0.0f;

	m16[3]=0.0f;
	m16[7]=0.0f;
	m16[11]=0.0f;
	m16[15]=1.0f;		
}

void printMatrix(float *fm) {

	CCLOG("-----------------------");
	CCLOG(" %2.2f %2.2f %2.2f %2.2f ",fm[0],fm[4],fm[8],fm[12]);
	CCLOG(" %2.2f %2.2f %2.2f %2.2f ",fm[1],fm[5],fm[9],fm[13]);
	CCLOG(" %2.2f %2.2f %2.2f %2.2f ",fm[2],fm[6],fm[10],fm[14]);
	CCLOG(" %2.2f %2.2f %2.2f %2.2f ",fm[3],fm[7],fm[11],fm[15]);
}


void TMDLModel::DrawBones()
{
	glDisable (GL_DEPTH_TEST);

	tagMDLBone	*Bones	= (tagMDLBone *)((BYTE *)Header + Header->BoneOffset);

	for (int i=0;i<Header->NumBones; i++)
	{
		if (Bones[i].Parent >= 0)
		{			
			glPointSize (3.0f);
			glColor4f(0.0f,0.0f,1.0f,1.0f);		
			DrawPoint(BoneTransforms[i][0][3], BoneTransforms[i][1][3],	BoneTransforms[i][2][3]);

			glColor4f(0.0f,1.0f,0.0f,0.5f);		
			DrawLine(BoneTransforms[Bones[i].Parent][0][3], BoneTransforms[Bones[i].Parent][1][3],	BoneTransforms[Bones[i].Parent][2][3],
				BoneTransforms[i][0][3], BoneTransforms[i][1][3], BoneTransforms[i][2][3]);

		} else {

			glPointSize (5.0f);
			glColor4f(0.0f,1.0f,1.0f,1.0f);		
			DrawPoint(BoneTransforms[i][0][3], BoneTransforms[i][1][3],	BoneTransforms[i][2][3]);
		}
	}

	glEnable (GL_DEPTH_TEST);
	glColor4f(1.0f,1.0f,1.0f,1.0f);	
}

void TMDLModel::SetUpBones()
{

	float			BoneMatrix[3][4];

	static vec3_t	Pos1[MAXBONES];
	static vec4_t	Q1[MAXBONES];
	static vec3_t	Pos2[MAXBONES];
	static vec4_t	Q2[MAXBONES];
	static vec3_t	Pos3[MAXBONES];
	static vec4_t	Q3[MAXBONES];
	static vec3_t	Pos4[MAXBONES];
	static vec4_t	Q4[MAXBONES];

	if (CurrentSequence >= Header->NumSequences)
	{
		CurrentSequence	= 0;
	}

	tagMDLSeqDescription	*SeqDescription	= (tagMDLSeqDescription *)((BYTE *)Header + 
		Header->SequenceOffset) + CurrentSequence;

	tagAnimation	*Anim	= GetAnim(SeqDescription);

	CalcRotations(Pos1, Q1, SeqDescription, Anim, CurrentFrame);

	if (SeqDescription->NumBlends > 1)
	{
		float	Value;

		Anim	+= Header->NumBones;
		CalcRotations(Pos2, Q2, SeqDescription, Anim, CurrentFrame);

		Value	= Blending[0] / 255.0f;

		SlerpBones(Q1, Pos1, Q2, Pos2, Value);

		if (4 == SeqDescription->NumBlends)
		{
			Anim	+= Header->NumBones;
			CalcRotations(Pos3, Q3, SeqDescription, Anim, CurrentFrame);

			Anim	+= Header->NumBones;
			CalcRotations(Pos4, Q4, SeqDescription, Anim, CurrentFrame);

			Value	= Blending[0] / 255.0f;
			SlerpBones(Q3, Pos3, Q4, Pos4, Value);

			Value	= Blending[1] / 255.0f;
			SlerpBones(Q1, Pos1, Q3, Pos3, Value);
		}
	}

	tagMDLBone	*Bones	= (tagMDLBone *)((BYTE *)Header + Header->BoneOffset);

	for (long Loop = 0; Loop < Header->NumBones; Loop++)
	{
		QuaternionMatrix(Q1[Loop], BoneMatrix);


		BoneMatrix[0][3]	= Pos1[Loop][0];
		BoneMatrix[1][3]	= Pos1[Loop][1];
		BoneMatrix[2][3]	= Pos1[Loop][2];
		/*
		CCLOG(" ********************************** THEIR MATRIX ****************************** ");
		CCLOG(" %2.2f %2.2f %2.2f ",BoneMatrix[0][0],BoneMatrix[1][0],BoneMatrix[2][0]);
		CCLOG(" %2.2f %2.2f %2.2f ",BoneMatrix[0][1],BoneMatrix[1][1],BoneMatrix[2][1]);
		CCLOG(" %2.2f %2.2f %2.2f ",BoneMatrix[0][2],BoneMatrix[1][2],BoneMatrix[2][2]);
		CCLOG(" %2.2f %2.2f %2.2f ",BoneMatrix[0][3],BoneMatrix[1][3],BoneMatrix[2][3]);

		Quaternion q=Quaternion_withValues(Q1[Loop][0],Q1[Loop][1],Q1[Loop][2],Q1[Loop][3]);
		Matrix m=Quaternion_toMatrix(q);
		m.m[12]= Pos1[Loop][0];
		m.m[13]= Pos1[Loop][1];
		m.m[14]= Pos1[Loop][2];

		printMatrix(m.m);
		*/		
		if (-1 == Bones[Loop].Parent) {
			memcpy(BoneTransforms[Loop], BoneMatrix, sizeof(float) * 12);
		} else {
			R_ConcatTransforms(BoneTransforms[Bones[Loop].Parent], BoneMatrix, 	BoneTransforms[Loop]);
		}
	}

	//glDisable (GL_TEXTURE_2D);

}

void TMDLModel::Lighting(float *lv, long Bone, long Flags, vec3_t Normal)
{
	float 	Illumination;
	float	LightCosine;

	Illumination	= (float)AmbientLightColor;

	if (Flags & LIGHT_FLATSHADE) {
		Illumination	+= ShadeLight * 0.8f;
	} else  {
		float	Value;
		LightCosine	= DotProduct (Normal, BoneLightVector[Bone]);
		if (LightCosine > 1.0f) 
			LightCosine = 1.0f;

		Illumination	+= ShadeLight;
		Value	= 1.5f;		
		if (Value <= 1.0f)
			Value	= 1.0f;	

		LightCosine	= (LightCosine + (Value - 1.0f)) / Value; 		
		if (LightCosine > 0.0f) 
			Illumination	-= ShadeLight * LightCosine; 

		if (Illumination <= 0)
			Illumination	= 0;
	}

	if (Illumination > 255) 	
		Illumination	= 255;	

	*lv	= Illumination / 255.0f;
}

void TMDLModel::SetupLighting()
{
	AmbientLightColor	= 32;
	ShadeLight			= 192;

	LightVector[0]		= 0.0f;
	LightVector[1]		= 0.0f;
	LightVector[2]		= -1.0f;

	LightColor[0]		= 1.0f;
	LightColor[1]		= 1.0f;
	LightColor[2]		= 1.0f;

	for (long Loop = 0; Loop < Header->NumBones; Loop++) {
		VectorIRotate(LightVector, BoneTransforms[Loop], BoneLightVector[Loop]);
	}
}


void TMDLModel::SetupModel(long BodyPart)
{
	if (BodyPart > Header->NumBodyParts)
	{
		BodyPart	= 0;
	}

	tagMDLBodyPart   *BodyPartPtr	= (tagMDLBodyPart *)((BYTE *)Header + Header->BodyPartOffset) + 
		BodyPart;

	long	Index	= CurrentBodyPart / BodyPartPtr->Base;
	Index			= Index % BodyPartPtr->NumModels;
	Model	= (tagMDLModel *)((BYTE *)Header + BodyPartPtr->ModelOffset) + Index;
}

void myMeshVBO::InitVBO()
{
	CCLOG(" InitVBO ");
	//--------------------------------------------------------------------------------------------------------

	unsigned int dataSize=m_pTempVtx.size()*sizeof(cocos2d::VertexBones);
	m_pVBOVertices=(cocos2d::VertexBones *)calloc(dataSize, 1 );		

	for (int t=0; t<m_pTempVtx.size(); t++) {
		cocos2d::VertexBones *vtx=m_pTempVtx[t];
		memcpy(&m_pVBOVertices[t],vtx,sizeof(cocos2d::VertexBones));
		delete vtx;
	}
	m_pTempVtx.clear();

	glGenBuffers(1, &m_uiVertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexVBO);
	glBufferData(GL_ARRAY_BUFFER, dataSize, m_pVBOVertices, GL_STATIC_DRAW);	
	glBindBuffer(GL_ARRAY_BUFFER, 0);	

	//--------------------------------------------------------------------------------------------------------
	int total_trianglelists=m_pTriangleList.size();
	for (int t=0; t<total_trianglelists; t++) {
		myTriangleList *trianglelist=m_pTriangleList[t];		

		glGenBuffers(1, &trianglelist->m_uiIndexVBO);		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, trianglelist->m_uiIndexVBO);		
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(trianglelist->m_pIndices[0])*trianglelist->m_iNumTriangles*3, trianglelist->m_pIndices, GL_STATIC_DRAW);				
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void myMeshVBO::StartVBO()
{
	glEnable (GL_DEPTH_TEST);

	glEnable(GL_MATRIX_PALETTE_OES);
	glMatrixMode(GL_MATRIX_PALETTE_OES);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_WEIGHT_ARRAY_OES);
	glEnableClientState(GL_MATRIX_INDEX_ARRAY_OES);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
}

void myMeshVBO::CloseVBO()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisable(GL_MATRIX_PALETTE_OES);
	glMatrixMode(GL_MODELVIEW);

	glDisableClientState(GL_WEIGHT_ARRAY_OES);
	glDisableClientState(GL_MATRIX_INDEX_ARRAY_OES);
	//glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);	
}

void myMeshVBO::RenderVBO()
{
#define STRIDE sizeof(cocos2d::VertexBones)

	glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexVBO);
	glVertexPointer(3, GL_FLOAT, STRIDE , (GLvoid*) offsetof( cocos2d::VertexBones, vertices));
	glColorPointer(4, GL_UNSIGNED_BYTE, STRIDE , (GLvoid*) offsetof( cocos2d::VertexBones, colors));
	glTexCoordPointer(2, GL_FLOAT, STRIDE , (GLvoid*) offsetof( cocos2d::VertexBones, texCoords));
	glMatrixIndexPointerOES(1, GL_UNSIGNED_BYTE, STRIDE ,(GLvoid*)  offsetof(cocos2d::VertexBones, BoneIndices));
	glWeightPointerOES(1, GL_FLOAT, STRIDE , (GLvoid*) offsetof(cocos2d::VertexBones, BoneWeight));

	StartVBO();

	int total_trianglelists=m_pTriangleList.size();
	for (int t=0; t<total_trianglelists; t++) {
		myTriangleList *trianglelist=m_pTriangleList[t];		

		for (int boneindex = 0; boneindex <=trianglelist->m_iMaxMatrices; ++boneindex) {								
			LoadBoneIndex(model_view.m, trianglelist->m_iMatrixSetup[boneindex],boneindex);
		}		

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, trianglelist->m_uiIndexVBO);
		glDrawElements(GL_TRIANGLES, trianglelist->m_iNumTriangles*3, GL_UNSIGNED_SHORT, (GLvoid*)0); 
		//CCLOG(" RenderVBO %i ",trianglelist->m_iNumTriangles);
	}

	CloseVBO();
}

void myHardwareVBO::InitVBO()
{
	if (m_bVBOInit)
		return;

	m_bVBOInit=true;

	std::map<long, myMeshVBO *>::iterator mIter;
	for (mIter  = m_pVBOMeshMap.begin();
		mIter != m_pVBOMeshMap.end();
		mIter++)
	{
		myMeshVBO *mesh=mIter->second;
		mesh->InitVBO();
	}

	free(m_pVertices);
	m_pVertices=NULL;
}

void myHardwareVBO::RenderVBO()
{
	if (!m_bVBOInit)
		return;

	std::map<long, myMeshVBO *>::iterator mIter;
	for (mIter  = m_pVBOMeshMap.begin();
		mIter != m_pVBOMeshMap.end();
		mIter++)
	{
		myMeshVBO *mesh=mIter->second;
		
		mesh->RenderVBO();
	}
}


void myHardwareVBO::init(tagMDLHeader *Header, tagMDLModel *Model)
{
	if (m_bHardwareInit)
		return;

	m_bHardwareInit=true;
	m_pModel=Model;

	unsigned int dataSize=m_pModel->NumVertices*sizeof(cocos2d::VertexBones);
	m_pVertices=(cocos2d::VertexBones *)calloc(dataSize, 1 );		
/*
	// Vertex buffer ID
	glGenBuffers(1, &m_vertexVBOId);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBOId);
	glBufferData(GL_ARRAY_BUFFER, dataSize, m_pVertices, GL_DYNAMIC_DRAW);	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
*/

}

void myHardwareVBO::draw()
{

}

myHardwareVBO *TMDLModel::getModelVBO(tagMDLModel *Model)
{

	myHardwareVBO *pHardWare=m_VBOMap[(long) Model];
	if (pHardWare==NULL) {
		pHardWare=new myHardwareVBO();
		pHardWare->init(Header, Model);
		m_VBOMap[(long) Model]=pHardWare;
		// INIT HARDWARE 

		InitPalleteIndex();
	}

	if (pHardWare->m_bHardwareInit)
		return pHardWare;

	printf(" Model Init ");
	return pHardWare;
}

//----------------------------------------------------------------------------

void TMDLModel::BuildVBO()
{
	int maxMatrices;
	glGetIntegerv(GL_MAX_PALETTE_MATRICES_OES, &maxMatrices); 
	CCLOG(" maxMatrices %i ",maxMatrices);

	tagMDLMesh *MeshPtr	= (tagMDLMesh *)((BYTE *)Header + Model->MeshOffset);
}

myMeshVBO *myHardwareVBO::getMeshVBO(tagMDLMesh *MeshPtr)
{
	myMeshVBO *pHardWare=m_pVBOMeshMap[(long) MeshPtr];
	if (pHardWare!=NULL) 
		return pHardWare;

	pHardWare=new myMeshVBO();	
	pHardWare->m_pHardware=this;
	m_pVBOMeshMap[(long) MeshPtr]=pHardWare;		
	return pHardWare;
}

//----------------------------------------------------------------------------

cocos2d::VertexBones *cocos2d::VertexBones::getCopy()
{
	cocos2d::VertexBones *c=new cocos2d::VertexBones();
	memcpy(c,this, sizeof(cocos2d::VertexBones));
	return c;
}

bool cocos2d::VertexBones::contentEquals(cocos2d::VertexBones *v) 
{
	//CCLOG(" (%2.2f, %2.2f, %2.2f)  (%2.2f, %2.2f, %2.2f) ",vertices.x,vertices.y,vertices.z,v->vertices.x,v->vertices.y,v->vertices.z);
	if (vertices.x!=v->vertices.x || vertices.y!=v->vertices.y || vertices.z!=v->vertices.z) 
		return false;

	if (colors.r!=v->colors.r || colors.g!=v->colors.g || colors.b!=v->colors.b || colors.a!=v->colors.a) 
		return false;

	if (texCoords.u!=v->texCoords.u || texCoords.v!=v->texCoords.v)
		return false;

	if (BoneWeight!=v->BoneWeight)
		return false;

	if (BoneIndices!= v->BoneIndices) {
		return false;
	}

	return true;
}


int myMeshVBO::FindVertex(cocos2d::VertexBones *pVertexFinal)
{

	std::vector<cocos2d::VertexBones *>::iterator it = m_pTempVtx.begin();
	int size=m_pTempVtx.size();
	for (int t=0; t<size; t++) {
		cocos2d::VertexBones *testVertex=*it;
		if (testVertex->contentEquals(pVertexFinal))
			return t;

		it++;
	}
	return -1;
}

//---------------------------
int g_iRenderingMode=-1;
int g_iCurrentIndex=0;
int g_iLastIndexes[2];
int g_iGlobalTriangles=0;

void ResetClose()
{
	g_iRenderingMode=-1;
	g_iCurrentIndex=0;
	g_iLastIndexes[0]=-1;	
	g_iLastIndexes[1]=-1;
}

void myMeshVBO::closeTriangleList()
{
	//CCLOG("---------------- CLOSE TRIANGLE LIST ----------------");
	int numTrianglesTemp=m_pTempIDX.size()/3;

//	g_iGlobalTriangles+=numTrianglesTemp;

	m_currentTriangle->m_iNumTriangles=numTrianglesTemp;
	m_currentTriangle->m_pIndices = (GLushort *)calloc( sizeof(GLushort)*numTrianglesTemp*3,1 );

	int size=m_pTempIDX.size();

	m_currentTriangle->m_iMatrixSetup=new unsigned char[g_iMatrixPalleteSize];
	for (int t=0;t<g_iMatrixPalleteSize;t++) {
		m_currentTriangle->m_iMatrixSetup[t]=g_iMatrixPalleteFinal[t];
	}
	m_currentTriangle->m_iMaxMatrices=g_iMatrixPalleteSize;

	for (int t=0; t<size; t++) {
		m_currentTriangle->m_pIndices[t]=m_pTempIDX[t];
	}	

	m_pTempIDX.clear();

	m_pTriangleList.push_back(m_currentTriangle);
	m_currentTriangle=new myTriangleList();

	//g_iRenderingMode=-1;
	//g_iCurrentIndex=0;
	//g_iLastIndexes[0]=-1;	
	//g_iLastIndexes[1]=-1;
	
	InitPalleteIndex();
}

void myMeshVBO::setPrimitive(int iRenderingMode)
{
	g_iRenderingMode=iRenderingMode;
	g_iLastIndexes[0]=-1;
	g_iLastIndexes[1]=-1;	
	g_iCurrentIndex=0;	
}

int iCache=0;
cocos2d::VertexBones *myVertexCache[2];
cocos2d::VertexBones *myFanCache;
int myMeshVBO::AddVertex(cocos2d::VertexBones *pVertexFinal, int iTriangle)
{	
	//---------------------------------------------------------------------------------------------
	int bonesidx=pVertexFinal->BoneIndices;

	// Miramos si hemos llegado al maximo de paletas permitidas en esta lista
	if (!g_bMatrixPallete[bonesidx]) {

		//CCLOG(" %i :: FOUND PALLETE %i ",g_iMatrixPalleteSize, bonesidx);
		g_bMatrixPallete[bonesidx]=true;
		g_iMatrixPalleteIndex[bonesidx]=g_iMatrixPalleteSize;

		g_iMatrixPalleteFinal[g_iMatrixPalleteSize]=bonesidx;
		bonesidx=g_iMatrixPalleteSize;
		g_iMatrixPalleteSize++;

		if (g_iMatrixPalleteSize>=g_iMaxMatrices) {
			closeTriangleList();

			int len=g_iCurrentIndex;
			if (g_iRenderingMode==GL_TRIANGLE_FAN) {	
//g_iCurrentIndex=0;
//g_iRenderingMode=-1;

				g_iCurrentIndex=0;				
				AddVertex(myFanCache, iTriangle);
				if (len>0) {
					//myVertexCache[1]->colors.r=255; myVertexCache[1]->colors.g=0; myVertexCache[1]->colors.b=0;
					AddVertex(myVertexCache[1],iTriangle);
					//myVertexCache[1]->colors.r=0; myVertexCache[1]->colors.g=0;	myVertexCache[1]->colors.b=0;
				}

			} else {

				if (len>0) {
					g_iCurrentIndex=0;
					if (len==1) {						
						AddVertex(myVertexCache[1],iTriangle);						
					} else 
					for (int t=0;t<len;t++) {											
						AddVertex(myVertexCache[0],iTriangle);					 // La funcion hace scroll en la cache por eso es 0						
					}									
				}
			}
			AddVertex(pVertexFinal,iTriangle);
			return 0;
		}
	} else {
		bonesidx=g_iMatrixPalleteIndex[bonesidx];	
	}

	int iOriginalIndex=pVertexFinal->BoneIndices;
	pVertexFinal->BoneIndices=bonesidx;

	myVertexCache[0]=myVertexCache[1];
	myVertexCache[1]=pVertexFinal;

	//---------------------------------------------------------------------------------------------
	int idx=FindVertex(pVertexFinal);
	if (idx<0) {
		//CCLOG(" NUEVO VERTICE ");
		cocos2d::VertexBones *myVtx=pVertexFinal->getCopy();
		m_pTempVtx.push_back(myVtx);
		idx=m_pTempVtx.size()-1;
	} else {
		//CCLOG(" RECYCLE VERTEX ");
		m_iRecycled++;
	}	
	
	pVertexFinal->BoneIndices=iOriginalIndex;

	if (g_iCurrentIndex==0) {
		if (g_iRenderingMode==GL_TRIANGLE_FAN) {
			myFanCache=pVertexFinal;
		}

		//CCLOG(" %i No tengo datos aun ", idx);
		g_iLastIndexes[0]=idx;
		g_iCurrentIndex++;
		return idx;
	}

	if (g_iCurrentIndex==1) {
		//CCLOG(" %i Solo tengo el central ", idx);
		g_iLastIndexes[1]=idx;
		g_iCurrentIndex++;
		return idx;
	}

	// Si es un triangle fan, los poligonos se centran en uno del medio
	if (g_iRenderingMode==GL_TRIANGLE_FAN) {
		//CCLOG(" FAN (%i,%i,%i) ", g_iLastIndexes[0], g_iLastIndexes[1], idx);
		//[TODO] Queda cambiar el orden de los poligonos pues supongo que ira rotando la orientacion

		m_pTempIDX.push_back(idx);		
		m_pTempIDX.push_back(g_iLastIndexes[1]);
		m_pTempIDX.push_back(g_iLastIndexes[0]);

		g_iGlobalTriangles++;
		g_iLastIndexes[1]=idx;
	}

	if (g_iRenderingMode==GL_TRIANGLE_STRIP) {
		//CCLOG(" STRIP (%i,%i,%i) ", g_iLastIndexes[0], g_iLastIndexes[1], idx);

		if (iTriangle%2) {
			m_pTempIDX.push_back(g_iLastIndexes[0]);
			m_pTempIDX.push_back(g_iLastIndexes[1]);
			m_pTempIDX.push_back(idx);		
		} else {
			m_pTempIDX.push_back(idx);		
			m_pTempIDX.push_back(g_iLastIndexes[1]);			
			m_pTempIDX.push_back(g_iLastIndexes[0]);
		}

		g_iGlobalTriangles++;
		g_iLastIndexes[0]=g_iLastIndexes[1];
		g_iLastIndexes[1]=idx;
	}	
	return idx;
}

int g_iTotalTriangles_NOTVBO=0;

void TMDLModel::DrawPoints(int bodyPart)
{
	float			*LightValue;

	BYTE			*BoneVertexPtr	= ((BYTE *)Header + Model->VertexInfoOffset);
	BYTE			*BoneNormalPtr	= ((BYTE *)Header + Model->NormalInfoOffset);
	tagMDLTexture	*Texture		= (tagMDLTexture *)((BYTE *)TextureHeader + TextureHeader->TextureOffset);
	tagMDLMesh		*MeshPtr		= (tagMDLMesh *)((BYTE *)Header + Model->MeshOffset);
	vec3_t			*Vertices		= (vec3_t *)((BYTE *)Header + Model->VertexOffset);
	vec3_t			*Normals		= (vec3_t *)((BYTE *)Header + Model->NormalOffset);
	short			*SkinReference	= (short *)((BYTE *)TextureHeader + TextureHeader->SkinOffset);

	if (CurrentSkin != 0 && CurrentSkin < TextureHeader->NumSkinFamilies) {
		SkinReference	+= (CurrentSkin * TextureHeader->NumSkinReferences);
	}

	myHardwareVBO *myVBO=getModelVBO(Model);

	if (!myVBO->m_bVBOInit) {
		LightValue	= (float *)LightValuesPtr;

		for (long OuterLoop = 0; OuterLoop < Model->NumMesh; OuterLoop++) 
		{
			long	Flags	= Texture[SkinReference[MeshPtr[OuterLoop].SkinReference]].Flags;		
			for (long InnerLoop = 0; InnerLoop < MeshPtr[OuterLoop].NumNormals; InnerLoop++, LightValue += 3, Normals++, BoneNormalPtr++)
			{
				float	Temp;
				Lighting(&Temp, *BoneNormalPtr, Flags, (float *)Normals);
				LightValue[0]	= Temp * LightColor[0];
				LightValue[1]	= Temp * LightColor[1];
				LightValue[2]	= Temp * LightColor[2];
			}
		}
	}

	glCullFace(GL_BACK); //	
	glEnable(GL_CULL_FACE);

	int totalTriangles=0;

	int max=Header->NumBones;
	if (max>=g_iMaxMatrices) 
		max=g_iMaxMatrices-1;

	for (int Loop = Model->NumMesh-1; Loop >=0 ; Loop--) 
	{		
		//CCLOG(" NUM MESH %i ",Model->NumMesh);

 		MeshPtr	= (tagMDLMesh *)((BYTE *)Header + Model->MeshOffset) + (Loop);
		glBindTexture(GL_TEXTURE_2D, Texture[SkinReference[MeshPtr->SkinReference]].Index);
		myMeshVBO *pMeshVBO=myVBO->getMeshVBO(MeshPtr);
		

		if (pMeshVBO->m_bFirstPass) {			
			pMeshVBO->RenderVBO();
		// START PRECALCULATION 
		}  else {

			CCLOG(" First pass %i ", Loop);

			short	*Triangles	= (short *)((BYTE *)Header + MeshPtr->TriangleOffset);

			float	U			= 1.0f / (float)Texture[SkinReference[MeshPtr->SkinReference]].Width;
			float	V			= 1.0f / (float)Texture[SkinReference[MeshPtr->SkinReference]].Height;

			int renderingMode;
			long NumTriangles;

			while (NumTriangles = *(Triangles++))
			{
				//CCLOG(" ---------------------------------- ");
				//CCLOG(" NumTriangles %i ",NumTriangles);
				totalTriangles+=NumTriangles;

				if (NumTriangles < 0) {		
					renderingMode=GL_TRIANGLE_FAN;
					NumTriangles= -NumTriangles;
				} else {
					renderingMode=GL_TRIANGLE_STRIP;				
				}

				int numVertex=0;			
				pMeshVBO->setPrimitive(renderingMode);
				for (; NumTriangles > 0; NumTriangles--, Triangles += 4) {
					//CCLOG(" Triangle * %i ",Triangles[0]);
					cocos2d::VertexBones *p=&myVBO->m_pVertices[numVertex]; 
					float	*Vertex	= Vertices[Triangles[0]];								
					p->vertices.x=Vertex[0]; p->vertices.y=Vertex[1]; p->vertices.z=Vertex[2];
					int iBoneIndex=BoneVertexPtr[Triangles[0]];
					p->BoneIndices=iBoneIndex;				
					p->BoneWeight=1.0f;				
					p->texCoords.u=Triangles[2]*U; p->texCoords.v=Triangles[3]*V;
					LightValue=LightValuesPtr[Triangles[1]];
					p->colors.r=LightValue[0]*255; p->colors.g=LightValue[1]*255; p->colors.b=LightValue[2]*255;  p->colors.a=255;

					if (!pMeshVBO->m_bFirstPass)
						pMeshVBO->AddVertex(p,numVertex); // Vertex to calculate on the strip if we have to rotate
					numVertex++;
				}						

				//for (int t=0; t<numVertex; t++)
				//	CCLOG(" (%2.2f, %2.2f, %2.2f) ",myVBO->m_pVertices[t].vertices.x,myVBO->m_pVertices[t].vertices.y,myVBO->m_pVertices[t].vertices.z);
				
				g_iTotalTriangles_NOTVBO+=numVertex-2;
				//if (g_iGlobalTriangles!=g_iTotalTriangles_NOTVBO)
				//	CCLOG(" FINAL VBO TRIANGLES %i -- %i  ",g_iGlobalTriangles, g_iTotalTriangles_NOTVBO);	
			}


			if (!pMeshVBO->m_bFirstPass) {
				pMeshVBO->closeTriangleList();
				ResetClose();
				pMeshVBO->m_bFirstPass=true;
				//CCLOG(" MESH %i Total vertex %i Recycled %i ",Loop, pMeshVBO->m_pTempVtx.size(),pMeshVBO->m_iRecycled);
				//CCLOG(" TOTAL VERTEX %i TRIANGLES %i ",Model->NumVertices,totalTriangles);
			}		

		} // END PRECALCULATION 
	}

	if (!myVBO->m_bVBOInit) {
		myVBO->InitVBO();
		CCLOG(" FINAL VBO TRIANGLES %i -- %i  ",g_iGlobalTriangles, g_iTotalTriangles_NOTVBO);		
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisable(GL_MATRIX_PALETTE_OES);
}


void TMDLModel::UploadTexture(tagMDLTexture *Texture, BYTE *Data, BYTE *Palette)
{
	long	Row1[256];
	long	Row2[256];
	long	Column1[256];
	long	Column2[256];
	BYTE	*TextureBuffer;
	BYTE	*BufferPtr;

	long OutWidth;
	for (OutWidth = 1; OutWidth < Texture->Width; OutWidth <<= 1);

	if (OutWidth > 256)
	{
		OutWidth = 256;
	}

	long OutHeight;
	for (OutHeight = 1; OutHeight < Texture->Height; OutHeight <<= 1);

	if (OutHeight > 256)
	{
		OutHeight = 256;
	}

	TextureBuffer	= new BYTE[OutWidth * OutHeight * 4];

	if (NULL == TextureBuffer)
	{
		printf("Unable to allocate memory.\n");

		exit(1);
	}

	BufferPtr	= TextureBuffer;

	long Loop;
	for (Loop = 0; Loop < OutWidth; Loop++)
	{
		Column1[Loop]	= (long)((Loop + 0.25f) * (Texture->Width / (float)OutWidth));
		Column2[Loop]	= (long)((Loop + 0.75f) * (Texture->Width / (float)OutWidth));
	}

	for (Loop = 0; Loop < OutHeight; Loop++)
	{
		Row1[Loop]	= (long)((Loop + 0.25f) * (Texture->Height / (float)OutHeight)) * 
			Texture->Width;

		Row2[Loop]	= (long)((Loop + 0.75f) * (Texture->Height / (float)OutHeight)) * 
			Texture->Width;
	}

	for (long OuterLoop = 0;  OuterLoop < OutHeight; OuterLoop++)
	{
		for (long InnerLoop = 0; InnerLoop < OutWidth; InnerLoop++, BufferPtr += 4)
		{
			BYTE	*Pixel1	= &Palette[Data[Row1[OuterLoop] + Column1[InnerLoop]] * 3];
			BYTE	*Pixel2	= &Palette[Data[Row1[OuterLoop] + Column2[InnerLoop]] * 3];
			BYTE	*Pixel3	= &Palette[Data[Row2[OuterLoop] + Column1[InnerLoop]] * 3];
			BYTE	*Pixel4	= &Palette[Data[Row2[OuterLoop] + Column2[InnerLoop]] * 3];

			BufferPtr[0]	= (Pixel1[0] + Pixel2[0] + Pixel3[0] + Pixel4[0]) >> 2;
			BufferPtr[1]	= (Pixel1[1] + Pixel2[1] + Pixel3[1] + Pixel4[1]) >> 2;
			BufferPtr[2]	= (Pixel1[2] + Pixel2[2] + Pixel3[2] + Pixel4[2]) >> 2;
			BufferPtr[3]	= 0xFF;
		}
	}

	glGenTextures(1, &CurrentTexture);
	glBindTexture(GL_TEXTURE_2D, CurrentTexture);		
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, OutWidth, OutHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
		TextureBuffer);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	Texture->Index	= CurrentTexture;

	CurrentTexture++;

	delete [] TextureBuffer;
}

