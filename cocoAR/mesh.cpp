#define _CRT_SECURE_NO_WARNINGS 
#include "cocos2d.h"
#include <math.h>

#include <string>

#define GL_GLEXT_PROTOTYPES

#include "vector.h"
#include "matrix.h"
#include "mesh.h"
#include "quaternion.h"
#include "ResourcesPath.h"

using namespace std;


char name[256];
char ext[6];
char dir[256];
char drive[3];

void ResourcesPath::splitpath(const char* path, char* drv, char* dir, char* name, char* ext)
{
	const char* end; /* end of processed string */
	const char* p;   /* search pointer */
	const char* s;   /* copy pointer */
	
	/* extract drive name */
	if (path[0] && path[1]==':') {
		if (drv) {
			*drv++ = *path++;
			*drv++ = *path++;
			*drv = '\0';
		} else {
			path++;  // Cambio para que no falle si no se le pide el directorio
			path++;  // Tengo que hacer generica esta funcion para que pueda encontrar 
			// Nombres de unidad mayores de 1 caracter
		}
	} else if (drv)
		*drv = '\0';
	
	/* search for end of string or stream separator */
	for(end=path; *end && *end!=':'; )
		end++;
	
	/* search for begin of file extension */
	for(p=end; p>path && *--p!='\\' && *p!='/'; )
		if (*p == '.') {
			end = p;
			break;
		}
  
  if (ext)
    for(s=end; (*ext=*s++); )
      ext++;
  
  /* search for end of directory name */
  for(p=end; p>path; )
    if (*--p=='\\' || *p=='/') {
      p++;
      break;
    }
  
  if (name) {
    for(s=p; s<end; ) *name++ = *s++;							 
    *name = '\0';
  }
  
  if (dir) {
    for(s=path; s<p; )
      *dir++ = *s++;
    
    *dir = '\0';
  }
}

string ResourcesPath::getTemporalFile(const char *Filename)
{
	std::string myPath;	
  
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)	
	myPath=ResourcesPath::getRealPath(Filename);
	return myPath;
#else
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
  return Filename;
#else
  splitpath(Filename, drive, dir, name, ext);
  myPath=cocos2d::CCFileUtils::getWriteablePath()+std::string(name)+std::string(ext);	
#endif	
#endif
	//CCLOG("+ getTemporalFile:: %s into %s ",Filename, myPath.c_str());
  
	if (myPath==Filename) {
		//CCLOG("+ getTemporalFile:: Samepath %s ",Filename);
		return myPath;
	}
  /*
   FILE *p=fopen(myPath.c_str(),"rb");
   if (p!=NULL) {
   CCLOG("+ getTemporalFile:: Cached ");
   fclose(p);
   return myPath;
   }
   */
	cocos2d::CCFileData *myData=new cocos2d::CCFileData(Filename, "rb");
	if (myData->getBuffer()) {
		//CCLOG("+ getTemporalFile:: %s %i ",Filename,myData->getSize());	
		FILE *p=fopen(myPath.c_str(),"wb+");
		if (p) {
			//CCLOG(" getTemporalFile:: Write ");
			fwrite(myData->getBuffer(),1,myData->getSize(),p);
			fclose(p);		
		}		
	} else {
		CCLOG(" getTemporalFile:: Failed ");
	}
	delete myData;
	return myPath;
}

std::string ResourcesPath::getRealPath(std::string path){
//  CCLOG("mypath: %s",mypath);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)	
  ResourcesPath::splitpath(path.c_str(), NULL, dir, name, ext);
	//sprintf(szDir,"%s/%s%s",path,name,ext);
	string mypath=string(name)+string(ext);
	mypath=cocos2d::CCFileUtils::fullPathFromRelativePath(mypath.c_str());	
	return mypath;
#endif
	return path;
}


bool Obj_Vertex::contentEquals(Obj_Vertex *v) 
{
	//CCLOG(" (%2.2f, %2.2f, %2.2f)  (%2.2f, %2.2f, %2.2f) ",vertices.x,vertices.y,vertices.z,v->vertices.x,v->vertices.y,v->vertices.z);
	if (vertices.x!=v->vertices.x || vertices.y!=v->vertices.y || vertices.z!=v->vertices.z) 
		return false;

	if (normal.x!=v->normal.x || normal.y!=v->normal.y || normal.z!=v->normal.z) 
		return false;

//	if (colors.r!=v->colors.r || colors.g!=v->colors.g || colors.b!=v->colors.b || colors.a!=v->colors.a) 
//		return false;

	if (texCoords1.u!=v->texCoords1.u || texCoords1.v!=v->texCoords1.v)
		return false;

	return true;
}

Mesh::Mesh()
{
	m_material.textureID = 0;
	m_material.hasAlpha = 0;
	m_material.autoIlum = 0;
	m_pCurrentMaterial=NULL;
}

Mesh::~Mesh()
{
}

Obj_Vertex *Obj_Vertex::getCopy()
{
	Obj_Vertex *c=new Obj_Vertex();
	memcpy(c,this, sizeof(Obj_Vertex));
	return c;
}

void my_MaterialVBO::setMaterial()
{
	glEnable(GL_COLOR_MATERIAL);			

    GLfloat ambientAndDiffuse[] = {0.0f, 0.1f, 0.9f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambientAndDiffuse);

	
	memset(&m_Material.ambient[0],0,sizeof(m_Material.ambient));
	memset(&m_Material.diffuse[0],0,sizeof(m_Material.diffuse));
	memset(&m_Material.specular[0],0,sizeof(m_Material.specular));
	memset(&m_Material.emission[0],0,sizeof(m_Material.emission));
  
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_Material.ambient);	
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_Material.diffuse);	
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_Material.specular);	
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, m_Material.emission);	 
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_Material.no_shininess);
	
/*
  float colorBronzeDiff[4] = { 0.8, 0.6, 0.0, 1.0 };
  float colorBronzeSpec[4] = { 1.0, 1.0, 0.4, 1.0 };
  float colorBlue[4]       = { 0.0, 0.2, 1.0, 1.0 };
  float colorNone[4]       = { 0.0, 0.0, 0.0, 0.0 };

  glMaterialfv(GL_FRONT, GL_DIFFUSE, colorBronzeDiff);
  glMaterialfv(GL_FRONT, GL_SPECULAR, colorBronzeSpec);
  glMaterialf(GL_FRONT, GL_SHININESS, 50.0);
*/
}

int my_MaterialVBO::AddVertex(Obj_Vertex *pVertexTemporal)
{
	int idx=FindVertex(pVertexTemporal);
	if (idx<0) {
		//CCLOG(" NUEVO VERTICE ");
		Obj_Vertex *myVtx=pVertexTemporal->getCopy();
		m_pTempVtx.push_back(myVtx);
		idx=m_pTempVtx.size()-1;
	} else {
		//CCLOG(" RECYCLE VERTEX ");
		m_iRecycled++;
	}

	m_vecTempIDX.push_back(idx);
	return idx;
}

int my_MaterialVBO::FindVertex(Obj_Vertex *pVertexFinal)
{
	std::vector<Obj_Vertex *>::iterator it = m_pTempVtx.begin();
	int size=m_pTempVtx.size();
	for (int t=0; t<size; t++) {
		Obj_Vertex *testVertex=*it;
		if (testVertex->contentEquals(pVertexFinal))
			return t;

		it++;
	}
	return -1;
}

void my_MaterialVBO::RenderVBO()
{
	if (m_Material.textureID==0)
		return;

	glDisable(GL_CULL_FACE);

	int zMaskDisabled = 0;
//	int lightDisabled = 0;
	GLboolean state = 0;

	if (m_Material.textureID) 
		glBindTexture(GL_TEXTURE_2D, m_Material.textureID);

	glGetBooleanv(GL_DEPTH_WRITEMASK, &state);
	if ((m_Material.hasAlpha) && (!state))
	{
		zMaskDisabled = 1;
		glDepthMask(0);
	}

	glEnable(GL_LIGHTING);
/*
	glGetBooleanv(GL_LIGHTING, &state);
	if ((m_Material.autoIlum) && (state))
	{
		lightDisabled = 1;
		glDisable(GL_LIGHTING);
	}		
*/
	if (m_iNumTriangles==0)
		return;

	//if (m_Material.autoIlum)
	//	glColor4f(m_Material.r, m_material.g, m_material.b, m_material.a);

	setMaterial();
	#define STRIDE sizeof(Obj_Vertex)

	glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexVBO);
	glVertexPointer(3, GL_FLOAT, STRIDE , (GLvoid*) offsetof( Obj_Vertex, vertices));
	glTexCoordPointer(2, GL_FLOAT, STRIDE , (GLvoid*) offsetof( Obj_Vertex, texCoords1));	
    glNormalPointer(GL_FLOAT, STRIDE,  (GLvoid*) offsetof( Obj_Vertex, normal));

	glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiIndexVBO);
	glDrawElements(GL_TRIANGLES, m_iNumTriangles*3, GL_UNSIGNED_SHORT, (GLvoid*)0); 

    glDisableClientState(GL_NORMAL_ARRAY);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnableClientState(GL_COLOR_ARRAY);
	//if (lightDisabled)
	//	glEnable(GL_LIGHTING);

	glDisable(GL_LIGHTING);

	if (zMaskDisabled)
		glDepthMask(1);
}


void my_MaterialVBO::InitVBO()
{
	m_iNumVertex=m_pTempVtx.size();
	m_iNumTriangles=m_vecTempIDX.size()/3;

	CCLOG(" %s - Num vertex %i Num triangles %i ",m_strName.c_str(), m_iNumVertex, m_iNumTriangles);

	unsigned int dataSize=m_iNumVertex*sizeof(Obj_Vertex);
	Obj_Vertex *pVertices=(Obj_Vertex *)calloc(dataSize, 1 );		
	for (int t=0;t<m_iNumVertex;t++) {
		Obj_Vertex *vt=m_pTempVtx[t];
		memcpy(&pVertices[t],vt,sizeof(Obj_Vertex));		
		delete vt;
	}
	m_pTempVtx.clear();
/*
	float	U = 1.0f/Text_Width;
	float	V = 1.0f/Text_Height;
*/
	// Vertex buffer ID
	glGenBuffers(1, &m_uiVertexVBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexVBO);
	glBufferData(GL_ARRAY_BUFFER, dataSize, pVertices, GL_STATIC_DRAW);	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
//--------------------------------------------

	int length=m_iNumTriangles*3;

	dataSize=length*sizeof(GLushort);
	GLushort *pIDX=(GLushort *)calloc(dataSize, 1 );		
	for (int t=0;t<length;t++) {
		pIDX[t]=m_vecTempIDX[t];	
		//CCLOG(" %i (%2.2f %2.2f %2.2f) ",pIDX[t], pVertices[pIDX[t]].vertices.x,pVertices[pIDX[t]].vertices.y,pVertices[pIDX[t]].vertices.z);
	}

 	m_vecTempIDX.clear();

	glGenBuffers(1, &m_uiIndexVBO);		
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiIndexVBO);		
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, pIDX, GL_STATIC_DRAW);				
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete pIDX;
	delete pVertices;

}

void Mesh::SetAutoIlum(int set)
{
	m_material.autoIlum = set;
}

void Mesh::DrawTriangles()
{
}
/*
void Mesh::DisplaceUV(float U, float V)
{
	if (!m_tB)
		return;
	for (int i = 0; i < m_nMaps; i++)
	{
		m_t[i]->u = m_tB[i]->u + U;
		m_t[i]->v = m_tB[i]->v + V;
	}
}
*/

void Mesh::LoadMaterial(char* szMaterialName)
{
	m_pCurrentMaterial=m_pVBOMaterialMap[szMaterialName];
	if (m_pCurrentMaterial==NULL) {
		CCLOG(" [%s] Material not found ",szMaterialName);		
	}
}



void Mesh::LoadMaterialList(char* filename, char *orgPath)
{
	string finalPath=ResourcesPath::getTemporalFile((string(orgPath)+string(filename)).c_str());	
	CCLOG(" Loading material [%s] ",finalPath.c_str());
	FILE *file = fopen(finalPath.c_str(),"rb");	
	if (!file) {
		CCLOG(" Loading FAILED ");
		return;
	}

	/*
	newmtl Zap1.bmp
	illum 2
	Kd 0.800000 0.800000 0.800000
	Ka 0.200000 0.200000 0.200000
	Ks 0.000000 0.000000 0.000000
	Ke 0.000000 0.000000 0.000000
	Ns 0.000000
	map_Kd Zap1.bmp
	*/

	char str[256];
	std::string materialName;
	while (!feof(file))
	{
		fscanf(file, "%s", str);
		
		if(!strcmp(str, "Ns")) {
			fscanf(file, "%f \n", &m_pCurrentMaterial->m_Material.no_shininess);			
		} else 
		if(!strcmp(str, "Ke")) {
			fscanf(file, "%f %f %f \n", &m_pCurrentMaterial->m_Material.emission[0], &m_pCurrentMaterial->m_Material.emission[1], &m_pCurrentMaterial->m_Material.emission[2]);
			m_pCurrentMaterial->m_Material.emission[3]=0.0f;
		} else 
		if(!strcmp(str, "Kd")) {
			fscanf(file, "%f %f %f \n", &m_pCurrentMaterial->m_Material.diffuse[0], &m_pCurrentMaterial->m_Material.diffuse[1], &m_pCurrentMaterial->m_Material.diffuse[2]);
			m_pCurrentMaterial->m_Material.diffuse[3]=1.0f;
		} else 
		if(!strcmp(str, "Ks")) {
			fscanf(file, "%f %f %f \n", &m_pCurrentMaterial->m_Material.specular[0], &m_pCurrentMaterial->m_Material.specular[1], &m_pCurrentMaterial->m_Material.specular[2]);
			m_pCurrentMaterial->m_Material.specular[3]=1.0f;
		} else 
		if(!strcmp(str, "Ka")) {
			fscanf(file, "%f %f %f \n", &m_pCurrentMaterial->m_Material.ambient[0], &m_pCurrentMaterial->m_Material.ambient[1], &m_pCurrentMaterial->m_Material.ambient[2]);
			m_pCurrentMaterial->m_Material.ambient[3]=1.0f;
		} else 
		if(!strcmp(str, "newmtl")) {
			fgets(str,255,file);
			int len=strlen(str);
			if (len>2) {
				if (str[len-2]==13)
					str[len-2]=0;
				else 
				if (str[len-1]==10)
					str[len-1]=0;

				CCLOG(" newmtl [%s] ",&str[1]);			
				materialName=&str[1];
				m_pCurrentMaterial=new my_MaterialVBO();

				m_pVBOMaterialMap.insert(std::pair<std::string, my_MaterialVBO *>(materialName, m_pCurrentMaterial));

				m_pCurrentMaterial->m_strName=materialName;
				m_pCurrentMaterial->m_Material.textureID=0;
				memset(&m_pCurrentMaterial->m_Material.ambient[0],0,sizeof(m_pCurrentMaterial->m_Material.ambient));
				memset(&m_pCurrentMaterial->m_Material.diffuse[0],0,sizeof(m_pCurrentMaterial->m_Material.diffuse));
				memset(&m_pCurrentMaterial->m_Material.specular[0],0,sizeof(m_pCurrentMaterial->m_Material.specular));
				memset(&m_pCurrentMaterial->m_Material.emission[0],0,sizeof(m_pCurrentMaterial->m_Material.emission));
				m_pCurrentMaterial->m_Material.no_shininess=0;

			} else {
				CCLOG(" Empty name ? ");
			}
			//[TODO] Find material in resources!
		} else
			if(!strcmp(str, "map_Kd")) {

				fscanf(file, "%s", str);

				int len=strlen(str);
				if (len>4) {
					if (str[len-3]=='b' && str[len-2]=='m' && str[len-1]=='p') {
						str[len-3]='p'; str[len-2]='n'; str[len-1]='g';
					}
				}

				finalPath=string(orgPath)+string(str);
				CCLOG(" Loading texture %s ",finalPath.c_str());

				m_pCurrentMaterial->m_Material.textureID=1;
								
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)	
        // <TODO> ARREGLAR ESTO
        finalPath=ResourcesPath::getRealPath(finalPath);
#endif
//        printf(" Loading texture %s ",finalPath.c_str());
				cocos2d::CCTexture2D *pTexture = cocos2d::CCTextureCache::sharedTextureCache()->addImage(finalPath.c_str());				
				if (pTexture!=NULL)
					m_pCurrentMaterial->m_Material.textureID=pTexture->getName();
			} else {
				fgets(str,255,file);
			}
	}
	fclose(file);
}

void Mesh::InitVBO()
{
	if (m_bVBOInit)
		return;

	m_bVBOInit=true;
	
	m_vecTempVertex.clear();	
	m_vecNormals.clear();	
	m_vecTextCoordsA.clear();	
	m_vecTextCoordsB.clear();	

	std::map<string, my_MaterialVBO *>::iterator mIter;
	for (mIter  = m_pVBOMaterialMap.begin();
		mIter != m_pVBOMaterialMap.end();
		mIter++)
	{
		my_MaterialVBO *materialMesh=mIter->second;
		if (materialMesh!=NULL)
			materialMesh->InitVBO();
		else {
			//CCLOG(" Empty VBO material %s ",mIter->first.c_str());
		}
	}

}

void Mesh::Draw()
{
	if (!m_bVBOInit)
		return;

	std::map<string, my_MaterialVBO *>::iterator mIter;
	for (mIter  = m_pVBOMaterialMap.begin();
		mIter != m_pVBOMaterialMap.end();
		mIter++)
	{
		my_MaterialVBO *materialMesh=mIter->second;
		if (materialMesh!=NULL)
			materialMesh->RenderVBO();
		else {
			//CCLOG(" Empty material %s ",mIter->first.c_str());
		}
	}

}

void Mesh::LoadModel(char* filename)
{
	m_bVBOInit=false;
	m_pVBOMaterialMap.clear();

	enum rowType {VERTEX, NORMAL, TEX_MAP, FACE, NONE};
	int rType = NONE;

	float p1, p2, p3;
	int v1, v2, v3, t1, t2, t3, n1, n2, n3;
	n1 = n2 = n3 =0;

	string myPath=ResourcesPath::getTemporalFile(filename);

	CCLOG(" Loading model [%s] ",myPath.c_str());
//  printf("%s", myPath.c_str());

	FILE* file = fopen(myPath.c_str(),"r");	
	if (!file) return;

	// <TODO> ARREGLAR ESTO
  ResourcesPath::splitpath(filename, NULL, dir, NULL, NULL);

	char str[256];

	Obj_Vertex tempVertex[3];
	while (!feof(file))
	{
		fscanf(file, "%s", str);

		rType = NONE;
		if(!strcmp(str, "mtllib")) {		
			fscanf(file, "%s", str);
			LoadMaterialList(str,dir);
		} else 
		if(!strcmp(str, "usemtl")) {
			fscanf(file, "%s", str);
			LoadMaterial(str);
		} else 		
		if(!strcmp(str, "#")) {
			fgets(str, 255, file);			
		} else {


			if(!strcmp(str, "vt"))
				rType = TEX_MAP; else
			if(!strcmp(str, "v"))
				rType = VERTEX; else
			if(!strcmp(str, "vn"))
				rType = NORMAL; else
			if(!strcmp(str, "f"))
				rType = FACE;
			else 
				rType = -1;

			switch (rType)
			{
			case VERTEX:
				{
					fscanf(file, "%f %f %f \n", &p1, &p2, &p3);				
					m_vecTempVertex.push_back(cocos2d::vertex3(p1,p2,p3));
					break;
				}
			case NORMAL:
				{
					fscanf(file, "%f %f %f \n", &p1, &p2, &p3);				
					m_vecNormals.push_back(cocos2d::vertex3(p1,p2,p3));
					break;
				}
			case TEX_MAP:
				{
					fscanf(file, "%f %f", &p1, &p2);
					if (!fscanf(file, "%f", &p3))
						fscanf(file, "\n");

					m_vecTextCoordsA.push_back(cocos2d::tex2(p1,1.0f-p2));				
					break;
				}
			case FACE:
				{
					if (m_pCurrentMaterial!=NULL) {
						//if (m_n != NULL)
						fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d \n", &v1, &t1, &n1, &v2, &t2, &n2, &v3, &t3, &n3);
						//else fscanf(file, "%d/%d %d/%d %d/%d \n", &v1, &t1, &v2, &t2, &v3, &t3);

						tempVertex[0].vertices=m_vecTempVertex[v1-1];
						tempVertex[0].normal=m_vecNormals[n1-1];
						tempVertex[0].texCoords1=m_vecTextCoordsA[t1-1];

						tempVertex[1].vertices=m_vecTempVertex[v2-1];
						tempVertex[1].normal=m_vecNormals[n2-1];
						tempVertex[1].texCoords1=m_vecTextCoordsA[t2-1];

						tempVertex[2].vertices=m_vecTempVertex[v3-1];
						tempVertex[2].normal=m_vecNormals[n3-1];
						tempVertex[2].texCoords1=m_vecTextCoordsA[t3-1];

						for (int t=0;t<3;t++) {
							int idx=m_pCurrentMaterial->AddVertex(&tempVertex[t]);
						}
					} else {
//						CCLOG(" We dont have a material! ");
					}
					break;
				}
			default:
				{
					CCLOG("%s Not recognized ",str);
					fgets(str, 255, file);		
					//CCLOG("%s Skip ",str);
					//fscanf (file, "%*[^\n]\n");
					break;
				}
			}
		}
	}


	CCLOG(" Finished Loading file ");
	InitVBO();
/*
	m_tB = new TexUV*[m_nMaps];
	for (int i = 0 ; i < m_nMaps; i++)
	{
		m_tB[i] = new TexUV;
		*(m_tB[i]) = *(m_t[i]);
	}
*/
	//printf("loaded %s. Total %d faces, %d vertices, %d normals, %d TexMaps \n", filename, m_nFaces, m_nVertices, m_nNormals, m_nMaps);

	fclose(file);
}
