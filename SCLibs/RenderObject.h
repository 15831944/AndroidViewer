#ifndef _RENDEROBJECT_H_
#define _RENDEROBJECT_H_

#ifndef NO_GLVIEW

#include  <stdlib.h>
#include  <math.h>

#include "sclib.h"
#include "openglshader\Shader.h"

#define USETINYXML
#ifdef USETINYXML
#include "xml\xml.h"
// tiny will now be a part of SCLIB
#else
class TiXmlElement;
#endif

#define STRIP_NORMAL_DEVIATION_TOLERANCE 0.75 

#pragma warning( push )
#pragma warning( disable : 4250 ) // C4250 - 'class1' : inherits 'class2::member' via dominance

#define MAX_TEXTURE_SIZE 2048 // Was crashing when tile size was >= 4096
class TextureImage : public ScObject {
public:
	TextureImage() {
      imageHeight = 0;
      imageWidth = 0;
      imageBits = NULL;
      imageBits_per_pixel = 0;
      nTextures = 0;
      textures = NULL;
      rangeX = rangeY = 0.0;
      rows = 1;
      columns = 1;
      size = 0;
      pixelHeight = 0.0;
      pixelWidth = 0.0;
      exactRows = 0.0;
      exactColumns = 0.0;
	};
	virtual ~TextureImage() {
      CleanTiles();
      ReleaseImageBits();
	};

	void SetParams(const unsigned int width,
      const unsigned int height,
      const unsigned char bits_per_pixel,
      const unsigned char *bits,
      const double pixelHeight,
      const double pixelWidth,
      const unsigned char* palette=NULL);

	BOOL MakeTiles();
	void CleanTiles();
	int GetBytesPerPixel(void);
	BOOL IsSized(void) { return size > 0; };
	GLuint GetTextureName(unsigned int index) {
      if (textures && (index < nTextures))
        return textures[index];
      else
        return 0;
	};
	unsigned int GetRows(void) { return rows; };
	unsigned int GetColumns(void) { return columns; };
	void SetRows(unsigned int rows) { this->rows=rows; };
	void SetColumns(unsigned int columns) { this->columns=columns; };
	GLsizei GetTileSize(void) { return size; };
	void ReleaseImageBits(void) {
      if (imageBits) {
        delete imageBits;
        imageBits = NULL;
        imageWidth = 0;
        imageHeight = 0;
        imageBits_per_pixel = 0;
        imageBits = 0;
      }
	};

public: // public virtual
  virtual unsigned int GetIndex(const double x, const double y);
  virtual BOOL GetTexCoords( const double x, const double y, double *s, double*t );
  virtual BOOL IsInside(ads_point /*min*/, ads_point /*max*/) { return TRUE; };
  virtual BOOL AdjustTextureSize(void);
protected:
  //----Image reprocessing variables (used by BuildGeoTextures/MakeTiles)
  GLuint size;
  double pixelHeight, pixelWidth;
  GLenum format;
  GLint internalFormat;

  //----BMGImageStruct info
  unsigned int imageWidth;
  unsigned int imageHeight;
  unsigned char imageBits_per_pixel;
  unsigned char *imageBits;
  GLuint nTextures, *textures;

  unsigned int rows, columns;
  double exactRows, exactColumns;
  double rangeX, rangeY;
};

class GeoTextureImage : public TextureImage {
public:
  GeoTextureImage() {
    minX = maxY = 0.0;
    limitX = baseX = 0.0;
    limitY = baseY = 0.0;
  };
  virtual ~GeoTextureImage() {
  };

  void SetParams(const unsigned int width,
    const unsigned int height,
    const unsigned char bits_per_pixel,
    const unsigned char *bits,
    const double map_resolution[2],
    const double image_origin[2],
    const unsigned char* palette=NULL);

  BOOL GetMinMax(const unsigned int row, const unsigned int column, double& minimumX, double& minimumY, double& maximumX, double& maximumY);
  csString GetTextureFile(void) { return TextureFile; };
  void SetTextureFile(const csString& csTextureFile) { TextureFile=csTextureFile; };

public: // public virtual
  virtual unsigned int GetIndex(const double x, const double y);
  virtual BOOL GetTexCoords( const double x, const double y, double *s, double*t );
  virtual BOOL IsInside(ads_point min, ads_point max) {
    return (min[0] <= imgMax[0] && min[1] <= imgMax[1] && max[0] >= imgMin[0] && max[1] >= imgMin[1]);};
  virtual BOOL AdjustTextureSize(void);

protected:
  ads_point imgMin, imgMax;
  double limitX, limitY;
  double minX, maxY;
  double baseX, baseY;
  csString TextureFile;
};

typedef enum RenderModes {
  GLOBAL_TRANS_ROT, // typical scene 
  LOCAL_TRANS_GLOBAL_ROT, // objects with coords relative to center of scene, global rotation
  LOCAL_TRANS_DUAL_ROT, // objects with coords relative to center of scene, global+local rotation
  GLOBAL_TRANS_LOCAL_ROT, // objects in global coordinates, but its own rotation
  LOCAL_TRANS_ROT, // objects in relative coordinates and their own rotation
  VIEWER_PLANE, //objects in view plane (like text or frame)
  WORLD_MODEL, // Uses its own coordinate system.  The Center member specifies the object's world location.
  MODEL, // Uses its own coordinate system.  
} RenderModes;

class ScScene;

class TextDataExtractObj : public ScObject {
public:
  TextDataExtractObj() {
    Angle=0.0; 
    Pnt[0]=0.0;Pnt[1]=0.0;Pnt[2]=0.0;
    Color=7;
    Processed=FALSE;
    Visible=TRUE;
    LayerName="0";
    TextSize=1.0;
  };
  virtual ~TextDataExtractObj() {};
  
  inline void SetPoint(ads_point Pnt) {this->Pnt[0]=Pnt[0]; this->Pnt[1]=Pnt[1]; this->Pnt[2]=Pnt[2];};
  inline void GetPoint(ads_point Pnt) {Pnt[0]=this->Pnt[0];Pnt[1]=this->Pnt[1];Pnt[2]=this->Pnt[2];};
  inline void SetElev(double Elev) {Pnt[2]=Elev;};
  inline double GetElev(void) {return Pnt[2];};
  inline void SetString(csString& String) {this->String=String;};
  inline csString GetString(void) {return String;};
  inline void SetAngle(double Angle) {this->Angle=Angle;};
  inline double GetAngle(void) {return Angle;};
  inline void SetColor(long Color) {this->Color=Color;};
  long GetColor(void) {return Color;};
  inline void SetProcessed(BOOL Processed=TRUE) {this->Processed=Processed;};
  inline BOOL GetProcessed(void) {return Processed;};
  inline void SetLayerName(csString& LayerName) {this->LayerName=LayerName;};
  inline csString GetLayerName(void) {return LayerName;};
  inline void SetVisible(BOOL Visible=TRUE) {this->Visible=Visible;};
  inline BOOL GetVisible(void) {return Visible;};
  inline void SetTextSize(double TextSize) {this->TextSize=TextSize;};
  inline double GetTextSize(void) {return TextSize;};
protected:
  ads_point Pnt;
  csString String;
  double Angle;
  long Color;
  csString LayerName;
  BOOL Visible;
  BOOL Processed;
  double TextSize;
};

typedef enum List_t {
  NORMAL,         // Non-BlockMap lists
  BLOCKMAP,       // For BlockMap Block lists.
  BLOCKMAPPOINTS, // For BlockMap Point lists.
} List_t;

class LayeredDisplayListObject : public ScObject {
public:
  LayeredDisplayListObject() {
    Layer=-1;
    DisplayList=0;
    ListType = NORMAL;
  };
  virtual ~LayeredDisplayListObject() {
    if (DisplayList)
      glDeleteLists(DisplayList, 1);
  };
public:
  int Layer;
  int DisplayList;
  List_t ListType;
};

class ScModelObjectsList;
class ScRenderObjectsList;

class LayerDef : public ScObject {
public:
  LayerDef(int Color, int Status=1) {this->Color=Color; this->Status=Status;};

  int GetColor(void) {return Color;};
  void SetColor(int Color) {this->Color = Color;};

  int GetStatus(void) {return Status;};
  void SetStatus(int Status) {this->Status = Status;};
protected:
  int Color;
  int Status;
};
typedef std::map<std::string, LayerDef*> LayerList;

// LayerList helper
static void DeleteAllLayers( LayerList* Layers ) {
  for ( LayerList::iterator itr = Layers->begin(); itr != Layers->end(); ++itr ) {
      delete itr->second;
  }
  Layers->clear();
}


#ifdef USETINYXML
//===========================================================

class ParmsStreamerBase {
public:
  virtual BOOL WriteXML(TiXmlElement *rootIn, csString& ErrMsg) = 0;
  virtual BOOL ReadXML(TiXmlElement *rootIn, csString& ErrMsg) = 0;
};

class XMLModel {
public:
  XMLModel();
  virtual ~XMLModel();

  BOOL WriteXML(char* szName, ScModelObjectsList& ModelObjects, csString& ErrMsg, LayerList& Layers,
    ParmsStreamerBase* ParmsStreamer=NULL);
  BOOL ReadXML(char* szName, ScModelObjectsList& ModelObjects, csString& ErrMsg, LayerList& Layers,
    ParmsStreamerBase* ParmsStreamer=NULL);
  void CleanTempDir(void);

  csString& GetErrorMsg(void) { return LastErrMsg; };

protected:
  csString LastErrMsg;
};

#endif // USETINYXML


//TimS_Start

class YAMLMaterial : public ScObject {
public:
  YAMLMaterial(long matId);
  YAMLMaterial(long matId, double *matColor, int matGroup);
  
  static void GetRGBFromColor(double *returnColor, int color);
  
  long GetMaterialId() {return Id;};
  
  void SetColor(double *objColor){memcpy(color, objColor, sizeof(double)*3);};
  void GetColor(double *returnColor){memcpy(returnColor, color, sizeof(double)*3);};
  
  void SetAmbientColor(double *ambColor){memcpy(ambiantColor, ambColor, sizeof(double)*3);};
  void GetAmbientColor(double *returnColor){memcpy(returnColor, ambiantColor, sizeof(double)*3);};
  
  void SetGroup(int matGroup) {group = matGroup;};
  int GetGroup() {return group;};
  
private:
  double color[3];
  double ambiantColor[3];
  int group;	//Object Materials must be group 0 or group 4. Group 0 will not show up in TV Model. Group 4 will show up in TV Model.
				//Surface Materials must be group 1-3. 1 for surface material, 2 for walls/edges, 3 for marking material
};

#ifdef HAVE_CAD
int GetColorFromLayerName(const char* layerName);
#endif //HAVE_CAD

class YAMLLayer : public ScObject {
public:
  YAMLLayer(long l_layerId, csString l_layerName, int l_markings);
  
  long GetLayerId() {return Id;};
  const char *GetLayerName() {return layerName.Get();};
  int GetLayerColor() {return layerColor;};
  int GetMarkings() {return markings;};
  
private:
  csString layerName;
  int layerColor;
  int markings;
};


//Object Inserts reference an Object Definition, and then describe how to/where 
//to place that Object Defintion, as well as properties such as if it is 'read only'.
class YAMLObjectInsert : public ScObject {
public:
  YAMLObjectInsert(long objId);
  
  long GetId() {return Id;};
  
  void SetInsertType(int insType) {insertType = insType;};
  int GetInsertType() {return insertType;};
  
  void SetInsertName(csString instName) {insertName = instName;};
  const char *GetInsertName() {return insertName.Get();};
  
  void SetInsertDefinitionId(long defId) {insertDefinitionId = defId;};
  long GetInsertDefinitionId() {return insertDefinitionId;};
  
  void SetLayerId(long layId) {layerId = layId;};
  long GetLayerId() {return layerId;};
  
  void SetPosition(double *insertPt) {ptcpy(position, insertPt);};
  void GetPosition(double *returnPosition) {ptcpy(returnPosition, position);};
  
  void SetScale(double *objScale) {memcpy(scale, objScale, sizeof(double)*3);};
  void GetScale(double *returnScale) {memcpy(returnScale, scale, sizeof(double)*3);};
  
  void SetRotation(double *objRotation) {memcpy(rotation, objRotation, sizeof(double)*3);};
  void GetRotation(double *returnRotation) {memcpy(returnRotation, rotation, sizeof(double)*3);};
  
  void SetReadOnly(int *objReadOnly) {memcpy(readOnly, objReadOnly, sizeof(int)*3);};
  void GetReadOnly(int *returnReadOnly) {memcpy(returnReadOnly, readOnly, sizeof(int)*3);};
  
  void SetStickToTerrain(int willStick) {stickToTerrain = willStick;};
  int GetStickToTerrain() {return stickToTerrain;};
 
private:
  int insertType;
  csString insertName;
  long insertDefinitionId;
  long layerId;
  ads_point position;
  double scale[3];
  double rotation[3];
  int readOnly[3];
  int stickToTerrain;
};

class YAMLVertex : public ScObject {
public:
  YAMLVertex(ads_point vertPoint) {ptcpy(vertexPoint, vertPoint);};
  void GetPoint(double *returnPt) {ptcpy(returnPt, vertexPoint);};
  void SetPoint(double *newVertexPt) {ptcpy(vertexPoint, newVertexPt);};
  
private:
  ads_point vertexPoint;
};

//Each Face is made up of a number of references to a number of vertices (at least 3)
class YAMLFace : public ScObject {
public:
  YAMLFace() {vertexIndexList.Clear();};
  
  void AddVertexIndex(long index);
  void WriteFaceDataToFile(CStdioFile *yamlFile);
  
private:
  DefList vertexIndexList; //List of ScObject members
};

//A Submesh groups 3D faces that are made of a shared material. Submeshes are made up
//of an optional list of Vertices, and list of faces, defined by references to vertices,
//which can either be defined in the Object Definition, or here in a submesh (in which case 
//the index value should be equal to the total number of vertices in the 
//Object definition + the index in the submesh). I recommend either defining all vertices in
//the Object Definition, or defining all the vertices in the submeshes, but not mixing the two
class YAMLSubmesh : public ScObject {
public:
  YAMLSubmesh(long subMeshId, long subMeshMaterialId, int subMeshIsDTM = 0, int subMeshCountPerFace = 3);
  
  //Submesh Properties
  long GetMaterialId() {return materialId;};
  long GetSubmeshId() {return Id;};
  int GetIsDTM() {return isDTM;};
  void SetIsDTM(int objIsDTM) {isDTM = objIsDTM;};
  
  //Vertex Functions
  long AddVertex(ads_point vertPoint);
  long GetVertexCount() {return vertexList.ItemNum;};
  
  //Face Functions
  YAMLFace *AddFace();
  void AddVertIndexToFace(long faceId, long vertexIndex);
  long GetFaceCount() {return faceList.ItemNum;};
  int GetCountPerFace() {return countPerFace;};
  void SetCountPerFace(int vertCountPerFace) {countPerFace = vertCountPerFace;};
  
  //Output Functions
  void WriteVertexDataToFile(CStdioFile *yamlFile, int is_metric);
  void WriteFaceDataToFile(CStdioFile *yamlFile);
  
  //Management Functions
  void TranslatePoints(ads_point defOffset);
  
private:
  long materialId;	  //Id of the material that the faces are made of.
  int countPerFace;	  //Number of vertices that make up each face. All faces must have same number of vertices.
  DefList vertexList;  //List of YAMLVertex members
  DefList faceList;	  //List of YAMLFace members
  int isDTM;			  //Marks Definition as part of Data Terrain Map (will be used as surface for 'stick to terrain' objects)
};

//Class stores an Object Definition, which is essentially a group of 3D faces. Faces are seperated by
//material and stored in Submeshes (submeshList). The points that define each face are stored as Vertices,
//either here in the Object Definition, or in the submesh. The faces store indexes to the Vertices that define
//it, rather than the point value itself.
class YAMLObjectDef : public ScObject {
public:
  YAMLObjectDef(long objId, BOOL objIsBlockDef = false);
  ~YAMLObjectDef(){
	  delete vertexTree;
  };

  //Vertex Functions
  long AddVertex(ads_point vertPoint);
  long GetVertexCount() {return vertexTree->ItemNum;};
  void WriteVertexDataToFile(CStdioFile *yamlFile);
  
  //Submesh Functions
  YAMLSubmesh *AddSubMesh(long subMeshMaterialId, int subMeshIsDTM, int subMeshCountPerFace);
  long GetSubmeshCount() {return submeshList.ItemNum;};
  YAMLSubmesh *FindMatchingSubmesh(long materialId);
  long GetTotalFaceCount();
  void WriteSubmeshDataToFile(CStdioFile *yamlFile);
  
  //Object Definition Properties
  const char *GetDefinitionName() {return definitionName.Get();};
  void SetDefinitionName(csString objName) {definitionName = objName;};
  int GetDefinitionType() {return definitionType;};
  void SetDefinitionType(int defType) {definitionType = defType;};
  long GetId() {return Id;};
  
  //Model Management Functions
  void SetDefOffset(double *objDefOffset) {memcpy(defOffset, objDefOffset, sizeof(double)*3);};
  void GetDefOffset(double *returnOffset) {memcpy(returnOffset, defOffset, sizeof(double)*3);};
  BOOL IsDefOffsetSet();
  BOOL IsBlockDef() {return defIsBlock;};
  void SetWorkingLayer(YAMLLayer *curWorkingLayer) { workingLayer = curWorkingLayer;};
  YAMLLayer *GetWorkingLayer() {return workingLayer;};
  void AdjustInsertionPointIfNecessary(ads_point modelPt);
  void AdjustInsertionPointIfNecessary(lwpoint *lwRoot);
  void TranslatePointsToOrigin();
 
private:
  ScKdTree *vertexTree;	  //Tree of Vertices
  DefList submeshList;	  //List of YAMLSubmesh members

  csString definitionName;
  int definitionType;	  //0 = Surface, 1 = Object
  
  double defOffset[3];	  //Translates the Object definition to (0,0,0) based on the point in the definition closest to (0,0,0).
						  //This value must be added to the insertion point of any instance of this object definition.
  BOOL defIsBlock;
  int is_metric;
  
  YAMLLayer *workingLayer;	//The layer currently being defined
};

class YAMLPolyline : public ScObject {
public:
  YAMLPolyline(long lineId, int lineIsClosed, long lineLayerId, int lineColor)
					: polylineWidth(0.5), polylineIsClosed(lineIsClosed), polylineMode(0), polylineLayerId(lineLayerId)
  {
	  Id = lineId;
	  vertexList.Clear();
	  
	  YAMLMaterial::GetRGBFromColor(polylineColor, lineColor);
	  //polylineColor[0] = polylineColor[1] = polylineColor[2] = 0;
  };
  
  long GetId() {return Id;};
  
  long AddVertex(ads_point vertPoint) {
	  YAMLVertex *newVertex = new YAMLVertex(vertPoint);
	  return vertexList.AddObject(newVertex);
  };
  long GetVertexCount() {return vertexList.ItemNum;};
  
  int GetPolylineMode() {return polylineMode;};
  void SetPolylineMode(int lineMode) {polylineMode = lineMode;};
  
  double GetPolylineWidth() {return polylineWidth;};
  
  int GetIsClosed() {return polylineIsClosed;};
  
  long GetPolylineLayerId() {return polylineLayerId;};
  
  void GetPolylineColor(double *returnColor){memcpy(returnColor, polylineColor, sizeof(double)*3);};
  
  void WriteVertexDataToFile(CStdioFile *yamlFile){
	  char buf[256];
	  for (long Count=1;Count<=vertexList.ItemNum;Count++) {
		  YAMLVertex *vertex = static_cast<YAMLVertex *>(vertexList.GetObject(Count));
		  
		  if(vertex == NULL){
			  continue;
		  }
		  
		  ads_point vertPt;
		  vertex->GetPoint(vertPt);
		  
		  sprintf(buf, "      - [%f, %f]\n", vertPt[0], vertPt[1]);
		  
		  yamlFile->WriteString(buf);
	  }
  };
 
private:
  DefList vertexList;	//List of YAMLVertex members

  int polylineIsClosed;
  double polylineWidth;
  int polylineMode;
  long polylineLayerId;
  double polylineColor[3];
};

//The YAMLModel class is meant to translate our model into one recognized by TerraVision.
class YAMLModel
{
public:
  YAMLModel();
  ~YAMLModel();

  BOOL ConvertAndLoadModel(ScModelObjectsList& ModelObjects, csString& ErrMsg);
  BOOL WriteYAML(char OutFileNm[FILENMSZ], csString& ErrMsg);
  void CleanTempDir(void);
  
  //Material Functions
  void AddMaterial(double color[3], int group);
  long AddMaterial(int color, int group);
  void AddMaterial(YAMLMaterial *newMaterial){ materials.AddObject(newMaterial);};
  long GetMaterialId(int color, int group);
  bool MaterialExists(int color);
  
  //Layer Functions
  YAMLLayer *AddLayer(csString layerName, int markings);
  void AddLayer(YAMLLayer *newLayer) {layers.AddObject(newLayer);};
  YAMLLayer *GetLayer(csString checkLayerName);
  BOOL LayerExists(csString checkLayerName);
  
  //Object Definition Functions
  void AddObjectDef(YAMLObjectDef *newObjectDef) {objectDefs.AddObject(newObjectDef);};
  long GetObjectDefsCount() {return objectDefs.ItemNum;};
  YAMLObjectDef *GetObjectFromHandle(csString handle);
  
  //Object Insert Functions
  void AddObjectInsert(YAMLObjectInsert *newObjectInsert){objectInserts.AddObject(newObjectInsert);};
  long GetObjectInsertCount() {return objectInserts.ItemNum;};
  void TranslateInsertsAroundOrigin();
  
  //Polyline Functions
  void AddPolyLine(YAMLPolyline *newPolyline) {polylines.AddObject(newPolyline);};
  long GetPolylineCount() {return polylines.ItemNum;};
  
  //Temporary "Surface" section. I expect this to go away when we find a better way to distinguish surfaces
  void SetSurfaceObjPtr(YAMLObjectDef *newSurfaceObjPtr) {
	  surfaceObjPtr = newSurfaceObjPtr;
	  if(surfaceObjPtr != NULL){
		  surfaceObjPtr->SetDefinitionName("Surface");
	  }
  };
  YAMLObjectDef *GetSurfaceObjPtr() {return surfaceObjPtr;};

  //Error retrieval
  csString& GetErrorMsg(void) { return LastErrMsg; };


protected:
  void TranslateAllObjectDefs();

protected:
  DefList materials;		//list of YAMLMaterial members
  DefList layers;		//list of YAMLLayer memebers
  DefList objectDefs;	//list of YAMLObjectDef members
  DefList objectInserts;	//list of YAMLObjectInsert members
  DefList polylines;		//list of YAMLPolyline members
  
  YAMLObjectDef *surfaceObjPtr;//Pointer to the YAMLObjectDef that defines the surface model. Initialized to NULL

  csString LastErrMsg;
};

void AddTinToYAMLObjectDef(ScTIN *myTin, YAMLModel *YAMLModelPtr, YAMLObjectDef *objectDefPtr, int color, int isDTM);

//TimS_End

// ScModeller --- Abstract base class:
// For any ScModel that wants to be friendly with the ModelModelObject/ModelRenderObject
// The subclass must supply WriteXML and ReadXML methods. This is so the model can be
// saved and reloaded from persistent storage.
class ScModeller : public ScModel {
public:
  ScModeller(short Dims=3) : ScModel(Dims) {};
  ~ScModeller() {};

  virtual BOOL WriteXML(TiXmlElement *rootIn, csString& name, csString& ErrMsg) = 0;
  virtual BOOL ReadXML(TiXmlElement *rootIn, csString& ErrMsg) = 0;
};

// The "smart model" is just an ScModel that is smart enought to write and read
// itself to and from XML.
class ScSmartModel : public ScModeller {
public:
  ScSmartModel(short Dims=3) : ScModeller(Dims) {};
  ~ScSmartModel() {};

  virtual BOOL WriteXML(TiXmlElement *rootIn, csString& name, csString& ErrMsg);
  virtual BOOL ReadXML(TiXmlElement *rootIn, csString& ErrMsg);
};

class ModelObjectBase : public ScObject {
friend class RenderObjectBase;
public:
  ModelObjectBase();
  virtual ~ModelObjectBase();

  virtual const char* ClassName(void) { return "ModelObjectBase"; };
  virtual const char *GetName() { return m_name.Get(); };
  virtual void SetName(const char *name) { m_name=name; };

  virtual BOOL WriteXML(TiXmlElement *rootIn, csString& ErrMsg);
  virtual BOOL ReadXML(TiXmlElement *rootIn, csString& ErrMsg);
  
  virtual BOOL AddToYAMLObjectDef(YAMLModel *YAMLModelPtr, YAMLObjectDef *objectDefPtr, csString& ErrMsg);
  virtual BOOL AddToYAMLObjectInsert(YAMLModel *YAMLModelPtr, YAMLObjectInsert *objectInsPtr, csString& ErrMsg);
  //virtual BOOL WriteYAML(YAMLModel *YAMLModelPtr, YAMLObjectDefInsert *objectDefInsPtr, csString& ErrMsg);

  static lwpoint* CoordsToLW(const char* pText);

  inline float GetAlpha(void) { return Alpha; };
  inline void SetAlpha(float Alpha) { this->Alpha = Alpha; };

  void SetCenter(ads_point InCenter) {
	Center[0]=InCenter[0];Center[1]=InCenter[1];Center[2]=InCenter[2];
  };
  void GetCenter(ads_point InCenter) {
	InCenter[0]=Center[0];InCenter[1]=Center[1];InCenter[2]=Center[2];
  };

  void SetRotation(ads_point InRotation) {
	Rotation[0]=InRotation[0];Rotation[1]=InRotation[1];Rotation[2]=InRotation[2];
  };
  void GetRotation(ads_point InRotation) {
	InRotation[0]=Rotation[0];InRotation[1]=Rotation[1];InRotation[2]=Rotation[2];
  };

protected:
  BOOL CheckIfDuplicate(ScModelObjectsList& ModelObjects);

#ifdef HAVE_CAD
protected:
  static BOOL IsExtruded(ads_name ename) {
    BOOL bIsExtruded = FALSE;
    acad_resbuf* type;

    acad_resbuf* entity=acad_entget(ename);
    if (entity) {
      type=acad_entitem(entity,AcDb::kDxfNormalX);
      if ((type!=NULL)&&(fabs(type->resval.rpoint[2]-1.0)>SMALL))
        bIsExtruded=TRUE;
      else if (acad_entitem(entity,AcDb::kDxfNormalY)!=NULL)
        bIsExtruded=TRUE;
      else if (acad_entitem(entity,AcDb::kDxfNormalZ)!=NULL)
        bIsExtruded=TRUE;
      acad_relrb(entity);
    }

    return bIsExtruded;
  };
#endif //HAVE_CAD

public:
  ads_point Center;
  ads_point Rotation;   
  double Scale;

  csString LayerName;

  int BaseColor;
  DefList *TextObjects;

  BOOL Visible;
  BOOL ZeroTrans;
  RenderModes RenderMode;
protected:
  csString m_name;
  float Alpha;
};

class ScModelObjectsListBase : public DefList {
public:
  ScModelObjectsListBase() {};
  virtual ~ScModelObjectsListBase() {};

public:
  inline virtual ModelObjectBase *GetModelObject(long Index) const {
    return (ModelObjectBase*)(DefList::GetObject(Index));
  };
  virtual inline void RemoveObjectsByName(const char *Name) {
    ScObject *Object;
    long Index;
    const char *res;
      
    if (ItemNum==0) return;
    Index=1;
      do {
      Object=DefList::GetObject(Index);   
      if (Object!=NULL) {
        res=Object->GetName();
        if (res!=NULL) {
          if (stricmp(Name,res)==0) {
              RemoveObject(Index);  
          }
        }
      }
      Index=Next(Index);
    } while (Index!=0);
  }
private:
  // By hiding GetObject we force the correct Get method
  // which is either GetModelObject or GetRenderObject
  // (see ScRenderObjectsListBase below).
  virtual ScObject* GetObject(long Index) const {return DefList::GetObject(Index);};
};

class ScModelObjectsList : public ScObject {
public:
  ScModelObjectsList();
  virtual ~ScModelObjectsList();
  
  inline long GetItemNum(const char* LayerName);
  inline long GetItemNum(void) {return GetItemNum(NULL);};
  __declspec(property(get = GetItemNum)) long ItemNum;

  virtual ModelObjectBase* GetModelObject(long Index, const char* LayerName = NULL);
  virtual long FindObjectByName(const char *Name, const char* LayerName = NULL);

  virtual long AddObject(ModelObjectBase* MOB, const char* LayerName = NULL);
  virtual long AddObjectCompactly(ModelObjectBase *, const char* LayerName = NULL);
  virtual void RemoveObjectsByName(const char *Name, const char* LayerName = NULL);
  virtual void RemoveObject(long Index, BOOL SkipRelease = FALSE, const char* LayerName = NULL);
  virtual void RemoveAllObjects(BOOL SkipRelease = FALSE, const char* LayerName = NULL);
  virtual ScModelObjectsList* FindNamedModelObjects(const char *Name, const char* LayerName = NULL );

protected:
  typedef std::map<std::string, ScModelObjectsListBase*> LayeredLists;
  LayeredLists layers;
};

class ModelObject : public ModelObjectBase {
public:
  ModelObject();
  virtual ~ModelObject();

  virtual BOOL WriteXML(TiXmlElement *rootIn, csString& ErrMsg);
  virtual BOOL ReadXML(TiXmlElement *rootIn, csString& ErrMsg);
  
  BOOL AddToYAMLObjectDef(YAMLModel *YAMLModelPtr, YAMLObjectDef *objectDefPtr, csString& ErrMsg);
  BOOL AddToYAMLObjectInsert(YAMLModel *YAMLModelPtr, YAMLObjectInsert *objectInsPtr, csString& ErrMsg);
  
  virtual const char* ClassName(void) { return "ModelObject"; };
public:
  double Ambient;
  int ShadeMode;
  int FillMode;
  int StartColor, MidColor1, MidColor2, EndColor, EdgeColor;
  BOOL ColorByElev;
  BOOL ShowEdges;
};

typedef enum Faces_t {
  FRONT_FACE,
  REAR_FACE,
  LEFT_FACE,
  RIGHT_FACE,
  TOP_FACE,
  BOTTOM_FACE
} Faces_t;

class FacesToRender {
public:    
  FacesToRender();  
  ads_point pnt;
  double dx,dy,elev,top_elev;
  int color;
  std::string label;
  int layer[MAX_PATH];
  BOOL ignore;
  int top,bottom;
  BOOL isTop,isBottom;
  long DataId;    
};

typedef struct strltpred
{
  bool operator()(std::string a, std::string b) const
  {
    return a.compare(b) < 0;
  }
} strltpred;

typedef std::map<int,FacesToRender *> Column_t;
#if (_MSC_VER >= 1310)
  typedef stdext::hash_map<std::string,Column_t *,stdext::hash_compare<std::string, strltpred> > BlockMap_t;
#else
  typedef std::map<std::string,Column_t *> BlockMap_t;
#endif

class BlockMapModelObject : public ModelObject {
friend class BlockMapRenderObject;
public:
  BlockMapModelObject();
#ifdef HAVE_CAD
  BlockMapModelObject(ads_name Selection, LayerList* Layers);
#endif //HAVE_CAD
  virtual ~BlockMapModelObject();

  int BuildModelFromFile(csString& csFileName);
  ScUniRecordBase* GetObject(long id);
  void ClearSQL(void) {EntityInfo.Clear();};
  BOOL bIsEmpty(void) {return !bHasBlocks;};

  virtual BOOL WriteXML(TiXmlElement *rootIn, csString& ErrMsg);
  virtual BOOL ReadXML(TiXmlElement *rootIn, csString& ErrMsg);

protected:
#ifdef HAVE_CAD
  long CADDataExtract(ads_name ssname, LayerList* Layers);
  int ProcessBlockModel(ads_name ename); 
#endif //HAVE_CAD
  void PutBlockInMap(ads_point pnt, double dx, double dy, double elev,
    double top_elev, int color, char label[260]);

protected:
  std::list<int> ListOfLayers;
  void FreeMap();
  ScListSqlite EntityInfo;
  BlockMap_t BlockMap;
  BOOL bHasBlocks;
};

class ModelModelObject : public ModelObject {
public:

  ModelModelObject();
  virtual ~ModelModelObject();

  virtual BOOL WriteXML(TiXmlElement *rootIn, csString& ErrMsg);
  virtual BOOL ReadXML(TiXmlElement *rootIn, csString& ErrMsg);
  
  BOOL AddToYAMLObjectInsert(YAMLModel *YAMLModelPtr, YAMLObjectInsert *objectInsPtr, csString& ErrMsg);
  BOOL AddToYAMLObjectDef(YAMLModel *YAMLModelPtr, YAMLObjectDef *objectDefPtr, csString& ErrMsg);
  
  virtual const char* ClassName(void) { return "ModelModelObject"; };

  ScModeller* GetModel() {return Model;};
  void SetModel(ScModeller* Model, BOOL OwnModel=TRUE) {this->Model = Model;this->OwnModel=OwnModel;};
  BOOL GetOwnModel() {return OwnModel;};
  void SetOwnModel(BOOL OwnModel) {this->OwnModel=OwnModel;};
  void DeleteModel();
  void SetBuildByLayer(BOOL BuildByLayer) { this->BuildByLayer = BuildByLayer; };
  BOOL GetBuildByLayer( void ) { return BuildByLayer; };

protected:
  ScModeller *Model;
  BOOL OwnModel;
  BOOL BuildByLayer;
};

class EntityModelObject : public ModelObject {
friend class EntityRenderObject;
public:
  EntityModelObject();
#ifdef HAVE_CAD
  EntityModelObject(ads_name ename, LayerList* Layers);
#endif //HAVE_CAD

  virtual ~EntityModelObject();

  void Init();

  virtual BOOL WriteXML(TiXmlElement *rootIn, csString& ErrMsg);
  virtual BOOL ReadXML(TiXmlElement *rootIn, BOOL bSaveHandles, csString& ErrMsg);
  virtual const char* ClassName(void) { return "EntityModelObject"; }
  
  //virtual BOOL WriteYAML(YAMLModel *YAMLModelPtr, YAMLObjectDefInsert *objectDefInsPtr, csString& ErrMsg);
  virtual BOOL AddToYAMLObjectInsert(YAMLModel *YAMLModelPtr, YAMLObjectInsert *objectInsPtr, csString& ErrMsg);
  virtual BOOL AddToYAMLObjectDef(YAMLModel *YAMLModelPtr, YAMLObjectDef *objectDefPtr, csString& ErrMsg);

  inline BOOL GetIs3DFace(void) { return bIs3DFace; };
  inline lwpoint* Getlwroot(void) { return m_lwroot; };
  inline DefList& GetTDEO(void) { return TextDataExtractObjs; };

protected:
#ifdef HAVE_CAD
  void CADDataExtract(ads_name ename, LayerList* Layers);
#endif //HAVE_CAD
  csString Handle;
  lwpoint *m_lwroot;
  long Shape, Color;
  BOOL Closed;
  BOOL bIs3DFace;
  BOOL bIsBlockMember;
protected:
  DefList TextDataExtractObjs;
};

class SelectionSetModelObject : public ModelObject {
friend class SelectionSetRenderObject;
public:
  SelectionSetModelObject();
#ifdef HAVE_CAD
  SelectionSetModelObject(ScSSet& SelectionSet, ScModelObjectsList* ModelObjects, LayerList* Layers, BOOL bFromBlock, double *OriginPtr=NULL);
  SelectionSetModelObject(ads_name Selection, ScModelObjectsList* ModelObjects, LayerList* Layers, BOOL bFromBlock=FALSE, double *OriginPtr=NULL);
#endif //HAVE_CAD
  void Init();

  ~SelectionSetModelObject();

  void SetIsBlock(BOOL IsBlockDef=TRUE) {this->IsBlockDef = IsBlockDef;};
  BOOL GetIsBlock(void) { return IsBlockDef; };

  virtual BOOL WriteXML(TiXmlElement *rootIn, csString& ErrMsg);
  virtual BOOL ReadXML(TiXmlElement *rootIn, ScModelObjectsList& ModelObjects, BOOL bSaveHandles, csString& ErrMsg);
  
  virtual BOOL AddBlockDefToYAMLModel(YAMLModel *YAMLModelPtr, csString& ErrMsg);
  virtual BOOL AddEntityToYAMLModel(YAMLModel *YAMLModelPtr, csString& ErrMsg);
  
  //virtual BOOL WriteYAML(YAMLModel *YAMLModelPtr, YAMLObjectDefInsert *objectDefInsPtr, csString& ErrMsg);
  
  virtual const char* ClassName(void) { return "SelectionSetModelObject"; }

  ScModelObjectsList* GetEntities(void) { return Entities; };
protected:
#ifdef HAVE_CAD
  void CADDataExtract(ScSSet& SelectionSet, ScModelObjectsList* ModelObjects, LayerList* Layers, BOOL bFromBlock);
#endif //HAVE_CAD
  ScModelObjectsList* Entities;
  ScModelObjectsList* BlockRefs; 
  BOOL IsBlockDef;
  ads_point Origin; // Blocks have non-zero origin. This does not need to be property, we can translate entities as they are created
};

class BlockRefModelObject : public ModelObjectBase {
friend class BlockRefRenderObject;
public:
  BlockRefModelObject();
  ~BlockRefModelObject();

#ifdef HAVE_CAD
  BlockRefModelObject(char *Handle, ScModelObjectsList* ModelObjects, LayerList* Layers);
  BlockRefModelObject(ads_name ename, ScModelObjectsList* ModelObjects, LayerList* Layers);
#endif //HAVE_CAD

  virtual BOOL WriteXML(TiXmlElement *rootIn, csString& ErrMsg);
  virtual BOOL ReadXML(TiXmlElement *rootIn, ScModelObjectsList& ModelObjects, BOOL bSaveHandles, csString& ErrMsg);
  virtual const char* ClassName(void) { return "BlockRefModelObject"; };
  
  virtual BOOL AddToYAMLObjectInsert(YAMLModel *YAMLModelPtr, YAMLObjectInsert *objectInsPtr, csString& ErrMsg);
  virtual BOOL AddToYAMLObjectDef(YAMLModel *YAMLModelPtr, YAMLObjectDef *objectDefPtr, csString& ErrMsg);
  //virtual BOOL WriteYAML(YAMLModel *YAMLModelPtr, YAMLObjectDefInsert *objectDefInsPtr, csString& ErrMsg);
  
  void Init();

public:
  csString BlockName;
  double YScale;
  double ZScale;
  BOOL NonSurface;

  SelectionSetModelObject* GetBlockModelDef(void) {return BlockModelDef;};
  void SetBlockModelDef(SelectionSetModelObject* BlockModelDef) {this->BlockModelDef=BlockModelDef;};

protected:
#ifdef HAVE_CAD
  void CADDataExtract(ads_name ename, ScModelObjectsList* ModelObjects, LayerList* Layers);
#endif //HAVE_CAD
  csString Handle;
  SelectionSetModelObject *BlockModelDef;
};

class SurfaceModelObject : public ModelModelObject {
friend class SurfaceRenderObject;
public:
  SurfaceModelObject();
  virtual ~SurfaceModelObject();
  virtual void ClearSurfaceData(void);

  virtual BOOL WriteXML(TiXmlElement *rootIn, csString& ErrMsg);
  virtual BOOL ReadXML(TiXmlElement *rootIn, csString& ErrMsg);
  virtual const char* ClassName(void) { return "SurfaceModelObject"; }
  
  virtual BOOL AddToYAMLObjectInsert(YAMLModel *YAMLModelPtr, YAMLObjectInsert *objectInsPtr, csString& ErrMsg);
  virtual BOOL AddToYAMLObjectDef(YAMLModel *YAMLModelPtr, YAMLObjectDef *objectDefPtr, csString& ErrMsg);
  //virtual BOOL WriteYAML(YAMLModel *YAMLModelPtr, YAMLObjectDefInsert *objectDefInsPtr, csString& ErrMsg);

  const char* GetSurfaceFile(void) { return SurfaceFile.Get(); };
  void SetSurfaceFile(const char* SurfaceFile, BOOL bRefTin=FALSE) {
    if (bRefTin)
      RefSurfaceFile = SurfaceFile;
    else
      this->SurfaceFile = SurfaceFile;
  };

  BOOL HasTin(void) { return Tin != NULL; };
  BOOL HasRefTin(void) { return ReferenceTin != NULL; };
  
  void SetTin(ScTIN* Tin) {this->Tin = Tin;};
  void SetTin(ScTIN* Tin, ScModeller* Model) {this->Tin = Tin; this->Model = Model;};
  void SetTin(ScTIN* Tin, const char* SurfaceFile, BOOL bRefTIN = FALSE) {
    if (bRefTIN)
      this->ReferenceTin = Tin;
    else
      this->Tin = Tin;
    SetSurfaceFile(SurfaceFile, bRefTIN);
  };
  ScTIN* GetTin(void) { return Tin; };
  ScTIN* GetReferenceTin(void) { return ReferenceTin; };

  void SetGrd(grid_head* Grd) { this->Grd = Grd;};
  grid_head* GetGrd(void) { return Grd; };

  long LoadSurfaceFile(const char* SurfaceFile, BOOL bRefTIN=FALSE, BOOL bKeepGridasGrid=FALSE);
    
protected:
  csString SurfaceFile;
  csString RefSurfaceFile;
  ScTIN* Tin;
  ScTIN* ReferenceTin;
  grid_head* Grd;
private:
  BOOL OwnSurfaceData;
};

class GeoTexturedModelObj : public SurfaceModelObject {
friend class GeoTexturedRenderObj;
public:
  GeoTexturedModelObj() {};
  virtual ~GeoTexturedModelObj() {};

  virtual BOOL WriteXML(TiXmlElement *rootIn, csString& ErrMsg);
  virtual BOOL ReadXML(TiXmlElement *rootIn, csString& ErrMsg);
  virtual const char* ClassName(void) { return "GeoTexturedModelObj"; }
  
  //virtual BOOL WriteYAML(YAMLModel *YAMLModelPtr, YAMLObjectDefInsert *objectDefInsPtr, csString& ErrMsg);
  virtual BOOL AddToYAMLObjectInsert(YAMLModel *YAMLModelPtr, YAMLObjectInsert *objectInsPtr, csString& ErrMsg);
  virtual BOOL AddToYAMLObjectDef(YAMLModel *YAMLModelPtr, YAMLObjectDef *objectDefPtr, csString& ErrMsg);

  virtual void SetGeoImgFile( const csString& GeoImgFile ) { this->GeoImgFile = GeoImgFile; };
  virtual csString GetGeoImgFile(void) { return GeoImgFile; };
protected:
  csString GeoImgFile;
};

class TexturedModelObj : public SurfaceModelObject {
friend class TexturedRenderObj;
public:
  TexturedModelObj() {};
  virtual ~TexturedModelObj() {};

  virtual BOOL WriteXML(TiXmlElement *rootIn, csString& ErrMsg);
  virtual BOOL ReadXML(TiXmlElement *rootIn, csString& ErrMsg);
  virtual const char* ClassName(void) { return "TexturedModelObj"; }
  
  inline const char* GetTextureName(void) { return TextureName.Get(); };

  //virtual BOOL WriteYAML(YAMLModel *YAMLModelPtr, YAMLObjectDefInsert *objectDefInsPtr, csString& ErrMsg);
  virtual BOOL AddToYAMLObjectInsert(YAMLModel *YAMLModelPtr, YAMLObjectInsert *objectInsPtr, csString& ErrMsg);
  virtual BOOL AddToYAMLObjectDef(YAMLModel *YAMLModelPtr, YAMLObjectDef *objectDefPtr, csString& ErrMsg);
protected:
  csString TextureName;
  csString TexturePath;
};

class RenderObjectBase;
class RenderObjectBuilder {
public:
  RenderObjectBuilder() {};
  ~RenderObjectBuilder() {};
  RenderObjectBase* BuildRenderObject(ModelObjectBase*, ScRenderObjectsList&);  
};

class RenderObjectBase : public ScObject {
public:
  RenderObjectBase();
  RenderObjectBase(ModelObjectBase* Composite);
  virtual ~RenderObjectBase();
  
  void Init(void);
  virtual const char* ClassName(void) { return "RenderObjectBase"; };

  // "sealed" insures that Process() can't be overridden in subclasses.
  // The subclasses must supply an OnProcess instead.
  // The idea is to give the RenderObjects a common entry point
  // for the Process method that does work needed by all subclasses.
  // void Process(LayerList* Layers=NULL) sealed;
  // TODO... make sealed conditional as some compilers don't support this.
  void Process(LayerList* Layers=NULL);

  virtual void Clear();
  virtual void ClearText(BOOL bRelease=FALSE);
  void ProcessTextObjects(void);

  ///Compile display list, actual worker
  virtual void OnProcess(LayerList* Layers) = 0;
  virtual void Display(BOOL Dragging=FALSE, int Layer=-1);

  virtual void SetLighting() {};

  virtual void ReleaseResources(void);
  virtual void ReleaseDisplayLists(int Layer=-1);

  virtual const BOOL GetLateProcessing() { return LateProcessing; };
  virtual void SetLateProcessing(BOOL late) { LateProcessing=late; };
  virtual BOOL GetBoundingCube( double& dMinX, double& dMinY, double& dMinZ,
                            double& dMaxX, double& dMaxY, double& dMaxZ);

  virtual void SetParent(ScScene *Parent) { this->Parent=Parent; };
  virtual ScScene *GetParent() { return Parent; };

  BOOL IsProcessed(void) { return Processed; };
  virtual void SetProcessed(BOOL Processed) {this->Processed = Processed;};

  inline ModelObjectBase* GetModel(void) { return MOB; };
  inline void SetModel(ModelObjectBase *MOBase) {MOB = MOBase;};
  __declspec(property(get = GetModel, put = SetModel)) ModelObjectBase *MOBase;

  inline DefList* GetTextObjects(void) { return MOB->TextObjects; };
  inline void SetTextObjects(DefList* TextObjects) {MOB->TextObjects = TextObjects;};
  __declspec(property(get = GetTextObjects, put = SetTextObjects)) DefList* TextObjects;

  inline BOOL GetZeroTrans(void) { return MOB->ZeroTrans; };
  inline void SetZeroTrans(BOOL ZeroTrans) {MOB->ZeroTrans = ZeroTrans;};
  __declspec(property(get = GetZeroTrans, put = SetZeroTrans)) BOOL ZeroTrans;

  inline void SetName(const char *name) { MOB->SetName(name); };
  inline const char *GetName(void) { return MOB->GetName(); };

  inline BOOL GetVisible(void) { return MOB->Visible; };
  inline void SetVisible(BOOL Visible) {MOB->Visible = Visible;};
  __declspec(property(get = GetVisible, put = SetVisible)) BOOL Visible;

  inline float GetAlpha(void) { return MOB->Alpha; };
  inline void SetAlpha(float Alpha) {MOB->Alpha = Alpha;};
  __declspec(property(get = GetAlpha, put = SetAlpha)) float Alpha;

  inline int GetBaseColor(void) { return MOB->BaseColor; };
  inline void SetBaseColor(int BaseColor) {MOB->BaseColor = BaseColor;};
  __declspec(property(get = GetBaseColor, put = SetBaseColor)) int BaseColor;

  inline double* GetCenter(void) { return MOB->Center; };
  inline void SetCenter(double* Center) { MOB->Center[0] = Center[0]; MOB->Center[1] = Center[1]; MOB->Center[2] = Center[2];};
  __declspec(property(get = GetCenter, put = SetCenter)) double* Center;

  inline double GetCenterIndexed(int i) { return MOB->Center[i]; };
  __declspec(property(get = GetCenterIndexed)) double CenterIndexed[];

  inline double GetScale(void) { return MOB->Scale; };
  inline void SetScale(double Scale) {MOB->Scale = Scale;};
  __declspec(property(get = GetScale, put = SetScale)) double Scale;

  inline double* GetRotation(void) { return MOB->Rotation; };
  inline void SetRotation(double* Rotation) { MOB->Rotation[0] = Rotation[0]; MOB->Rotation[1] = Rotation[1]; MOB->Rotation[2] = Rotation[2];};
  __declspec(property(get = GetRotation, put = SetRotation)) double* Rotation;

  inline double GetRotationIndexed(int i) { return MOB->Rotation[i]; };
  __declspec(property(get = GetRotationIndexed)) double RotationIndexed[];

  inline RenderModes GetRenderMode(void) { return MOB->RenderMode; };
  inline void SetRenderMode(RenderModes RenderMode) { MOB->RenderMode = RenderMode; };
  __declspec(property(get = GetRenderMode, put = SetRenderMode)) RenderModes RenderMode;

protected:
  double dMinX, dMinY, dMinZ, dMaxX, dMaxY, dMaxZ;
  BOOL LateProcessing;
  ScScene *Parent;
  DefList DisplayLists;
  int DragDisplayList;
  BOOL Processed;
  ModelObjectBase* MOB;
};


class ScRenderObjectsList : public ScModelObjectsList {
public:
  ScRenderObjectsList();
  virtual ~ScRenderObjectsList() {};
  
  virtual ModelObjectBase* GetModelObject(long Index, const char* LayerName = NULL);
  virtual RenderObjectBase *GetRenderObject(long Index, const char* LayerName = NULL);

  virtual long AddObject(RenderObjectBase* ROB, const char* LayerName = NULL);
  virtual long AddObjectCompactly(RenderObjectBase* ROB, const char* LayerName = NULL);
  virtual ScRenderObjectsList* FindNamedRenderObjects(const char *Name, const char* LayerName=NULL );
};

class ScRenderObjectsListBase : public ScModelObjectsListBase {
public:
  ScRenderObjectsListBase() {};
  virtual ~ScRenderObjectsListBase() {};

public:
  inline virtual ModelObjectBase *GetModelObject(long Index) const {
    RenderObjectBase* ROB = (RenderObjectBase*)(DefList::GetObject(Index));
    if (ROB)
      return ROB->GetModel();
    else
      return NULL;
  };
  inline RenderObjectBase *GetRenderObject(long Index) const {
    return (RenderObjectBase*)(DefList::GetObject(Index));
  };
};

class RenderObject : public RenderObjectBase {
public:
  RenderObject();  
  RenderObject(ModelObject* Composite);
  virtual ~RenderObject();

  void Init(void);

  virtual const char* ClassName(void) { return "RenderObject"; };
  virtual void OnProcess(LayerList* Layers);

  virtual RenderObject *GetTopRO() { return this; };
  void SetShadeColor(ads_point pnt, double MinZ, double MaxZ,
                                    ScTIN *Reference, int TriColor, float Alpha=1.0);
  void GetShadeColorArray(ads_point pnt, double MinZ, double MaxZ,
                                    ScTIN *Reference, int TriColor,
                                    GLfloat mat[], float Alpha );
  void GetCrossColorArray(int StartColor, int EndColor, double Coef, GLfloat mat[],float Alpha=1.0);
  int SetNormal(double x1,double y1,double z1,double x2,double y2,double z2, double x3,double y3,double z3);
  virtual void SetLighting();

  inline ModelObject* GetModel(void) { return (ModelObject*)MOB; };
  inline void SetModel(ModelObject *MO) {MOB = (ModelObjectBase*)MO;};
  __declspec(property(get = GetModel, put = SetModel)) ModelObject *MO;

  inline double GetAmbient(void) { return ((ModelObject*)MOB)->Ambient; };
  inline void SetAmbient(double Ambient) {((ModelObject*)MOB)->Ambient = Ambient;};
  __declspec(property(get = GetAmbient, put = SetAmbient)) double Ambient;

  inline int GetShadeMode(void) { return ((ModelObject*)MOB)->ShadeMode; };
  inline void SetShadeMode(int ShadeMode) {((ModelObject*)MOB)->ShadeMode = ShadeMode;};
  __declspec(property(get = GetShadeMode, put = SetShadeMode)) int ShadeMode;

  inline int GetFillMode(void) { return ((ModelObject*)MOB)->FillMode; };
  inline void SetFillMode(int FillMode) {((ModelObject*)MOB)->FillMode = FillMode;};
  __declspec(property(get = GetFillMode, put = SetFillMode)) int FillMode;

  inline int GetStartColor(void) { return ((ModelObject*)MOB)->StartColor; };
  inline void SetStartColor(int StartColor) {((ModelObject*)MOB)->StartColor = StartColor;};
  __declspec(property(get = GetStartColor, put = SetStartColor)) int StartColor;

  inline int GetMidColor1(void) { return ((ModelObject*)MOB)->MidColor1; };
  inline void SetMidColor1(int MidColor1) {((ModelObject*)MOB)->MidColor1 = MidColor1;};
  __declspec(property(get = GetMidColor1, put = SetMidColor1)) int MidColor1;

  inline int GetMidColor2(void) { return ((ModelObject*)MOB)->MidColor2; };
  inline void SetMidColor2(int MidColor2) {((ModelObject*)MOB)->MidColor2 = MidColor2;};
  __declspec(property(get = GetMidColor2, put = SetMidColor2)) int MidColor2;

  inline int GetEndColor(void) { return ((ModelObject*)MOB)->EndColor; };
  inline void SetEndColor(int EndColor) {((ModelObject*)MOB)->EndColor = EndColor;};
  __declspec(property(get = GetEndColor, put = SetEndColor)) int EndColor;

  inline int GetEdgeColor(void) { return ((ModelObject*)MOB)->EdgeColor; };
  inline void SetEdgeColor(int EdgeColor) {((ModelObject*)MOB)->EdgeColor = EdgeColor;};
  __declspec(property(get = GetEdgeColor, put = SetEdgeColor)) int EdgeColor;

  inline BOOL GetColorByElev(void) { return ((ModelObject*)MOB)->ColorByElev; };
  inline void SetColorByElev(BOOL ColorByElev) {((ModelObject*)MOB)->ColorByElev = ColorByElev;};
  __declspec(property(get = GetColorByElev, put = SetColorByElev)) BOOL ColorByElev;

  inline BOOL GetShowEdges(void) { return ((ModelObject*)MOB)->ShowEdges; };
  inline void SetShowEdges(BOOL ShowEdges) {((ModelObject*)MOB)->ShowEdges = ShowEdges;};
  __declspec(property(get = GetShowEdges, put = SetShowEdges)) BOOL ShowEdges;

public:
  // If bOwnShading is true, this renderobject is reposible for its own
  // shade mode control.
  inline BOOL GetOwnShading(void) { return bOwnShading; };
  inline void SetOwnShading(BOOL bOwnShading) {this->bOwnShading = bOwnShading;};
  
  inline BOOL GetMode(void) { return bGLShading; };
  virtual void SetMode(BOOL bGLShading, BOOL bSmooth);
public:
  double StartElev, EndElev;
protected:
  int MaxLayer;
  // If this flag is true, this renderobject is reponsibile for its own shading and fillmode
  // handling.
  BOOL bOwnShading;
  BOOL bGLShading;
};

class BlockMapRenderObject : public RenderObject {
public:
  BlockMapRenderObject();
  BlockMapRenderObject(BlockMapModelObject* BMO);
  virtual ~BlockMapRenderObject();
 
  virtual const char* ClassName(void) { return "BlockMapRenderObject"; };

  virtual void OnProcess(LayerList* Layers);
  virtual BOOL GetBoundingCube( double& dMinX, double& dMinY, double& dMinZ,
                            double& dMaxX, double& dMaxY, double& dMaxZ);

  // Since BlockMaps Free the map to once rendered, don't allow the processed flag to be reset.
  virtual void SetProcessed(BOOL Processed) {};
  
  inline BlockMap_t& GetBlockMap(void) { return ((BlockMapModelObject*)MOB)->BlockMap; };
  __declspec(property(get = GetBlockMap)) BlockMap_t BlockMap;

  inline ScListSqlite& GetEntityInfo(void) { return ((BlockMapModelObject*)MOB)->EntityInfo; };
  __declspec(property(get = GetEntityInfo)) ScListSqlite EntityInfo;

  inline std::list<int>& GetLayerList(void) { return ((BlockMapModelObject*)MOB)->ListOfLayers; };
  __declspec(property(get = GetLayerList)) std::list<int>& ListOfLayers;
protected:
  void ProcessMap(int layer, BOOL bAsPoints=FALSE);
  void CheckCols(int layer, BOOL first);
  long _AddSingleFace(FacesToRender &Face, Faces_t side);
};

class ModelRenderObject : public RenderObject  {
public:
  ModelRenderObject();
  ModelRenderObject(ModelModelObject* MMO);
  virtual ~ModelRenderObject();
 
  virtual const char* ClassName(void) { return "ModelRenderObject"; };

  virtual void OnProcess(LayerList* Layers);

  void BuildLayerList(int Layer, ScSetLongEx *LayerSet=NULL);

  inline ModelModelObject* GetModel(void) { return (ModelModelObject*)MOB; };
  inline void SetModel(ModelModelObject *MMO) {MOB = (ModelObjectBase*)MMO;};
  __declspec(property(get = GetModel, put = SetModel)) ModelModelObject *MMO;

  inline ScModeller* GetScModel(void) { return ((ModelModelObject*)MOB)->GetModel(); };
  inline void SetScModel(ScModeller* Model) {((ModelModelObject*)MOB)->SetModel(Model);};
  __declspec(property(get = GetScModel, put = SetScModel)) ScModeller* Model;

  inline BOOL GetBuildByLayer(void) { return ((ModelModelObject*)MOB)->GetBuildByLayer(); };
  inline void SetBuildByLayer(BOOL BuildByLayer) {((ModelModelObject*)MOB)->SetBuildByLayer(BuildByLayer);};
  __declspec(property(get = GetBuildByLayer, put = SetBuildByLayer)) BOOL BuildByLayer;

  inline BOOL GetOwnModel(void) { return ((ModelModelObject*)MOB)->GetOwnModel(); };
  inline void SetOwnModel(BOOL OwnModel) {((ModelModelObject*)MOB)->SetOwnModel(OwnModel);};
  __declspec(property(get = GetOwnModel, put = SetOwnModel)) BOOL OwnModel;

  void DeleteModel() {((ModelModelObject*)MOB)->DeleteModel();};
  void SetModel(ScModeller* Model, BOOL OwnModel=TRUE) {((ModelModelObject*)MOB)->SetModel(Model, OwnModel);};
protected:
  void RenderModelLayer(int Layer, BOOL ZeroTrans, ScSetLongEx *LayerSet);
};

class SurfaceRenderObject : public ModelRenderObject  {
public:
  SurfaceRenderObject();
  SurfaceRenderObject(SurfaceModelObject*);
  virtual ~SurfaceRenderObject();

  void Init(void);

  virtual const char* ClassName(void) { return "SurfaceRenderObject"; };
    
  virtual void OnProcess(LayerList* Layers);
  long RenderTIN(BOOL DragMode=FALSE);
  void RenderGRID(BOOL DragMode=FALSE);

  void InitSurface(void);
  virtual BOOL GetBoundingCube( double& dMinX, double& dMinY, double& dMinZ,
                            double& dMaxX, double& dMaxY, double& dMaxZ);
  BOOL IsMultiColor(void);
  BOOL IsFixedColor(void);
  void RemoveFixedColor(void);
  int SetGridNormal(long counter);
  void GridNodeNormal(long counter,double *trend_vec, double &valid1);
  BOOL IsShadeColorByNode(ScTIN *Reference);
  virtual BOOL HasTextureFile(void) { return FALSE; };

  long LoadSurfaceFile(const char* SurfaceFile, BOOL bRefTIN=FALSE, BOOL bKeepGridasGrid=FALSE) {
    return ((SurfaceModelObject*)MOB)->LoadSurfaceFile(SurfaceFile, bRefTIN, bKeepGridasGrid);
  };

  inline SurfaceModelObject* GetModel(void) { return (SurfaceModelObject*)MOB; };
  inline void SetModel(SurfaceModelObject *SMO) {MOB = (ModelObjectBase*)SMO;};
  __declspec(property(get = GetModel, put = SetModel)) SurfaceModelObject *SMO;

  inline ScTIN* GetTin(void) { return ((SurfaceModelObject*)MOB)->GetTin(); };
  inline void SetTin(ScTIN* Tin) {((SurfaceModelObject*)MOB)->SetTin(Tin);};
  __declspec(property(get = GetTin, put = SetTin)) ScTIN* Tin;

  inline ScTIN* GetReferenceTin(void) { return ((SurfaceModelObject*)MOB)->ReferenceTin; };
  inline void SetReferenceTin(ScTIN* ReferenceTin) {((SurfaceModelObject*)MOB)->ReferenceTin = ReferenceTin;};
  __declspec(property(get = GetReferenceTin, put = SetReferenceTin)) ScTIN* ReferenceTin;

  inline grid_head* GetGrd(void) { return ((SurfaceModelObject*)MOB)->GetGrd(); };
  inline void SetGrd(grid_head* Grd) {((SurfaceModelObject*)MOB)->SetGrd(Grd);};
  __declspec(property(get = GetGrd, put = SetGrd)) grid_head* Grd;

  inline const char* GetSurfaceFile(void) { return ((SurfaceModelObject*)MOB)->GetSurfaceFile(); };
  inline void SetSurfaceFile(const char* SurfaceFile) {((SurfaceModelObject*)MOB)->SetSurfaceFile(SurfaceFile);};
  __declspec(property(get = GetSurfaceFile, put = SetSurfaceFile))  const char* SurfaceFile;


  void SetTin(ScTIN* Tin, ScModeller* Model) {((SurfaceModelObject*)MOB)->SetTin(Tin, Model);};
  BOOL HasTin(void) { return ((SurfaceModelObject*)MOB)->HasTin(); };
  BOOL HasRefTin(void) { return ((SurfaceModelObject*)MOB)->HasRefTin(); };
  void SetTin(ScTIN* Tin, const char* SurfaceFile, BOOL bRefTIN = FALSE) {((SurfaceModelObject*)MOB)->SetTin(Tin, SurfaceFile, bRefTIN);};
  BOOL RayIntersect(ads_point pnt1, ads_point pnt2, ads_point Cross);
protected:
  DefList DisplayListObjects;
  DefList DragDisplayListObjects;
  int FixedColor; // state var for IsFixedColor()
};

class GeoTextureImage;

//----Geo-Texturing (Image Draping) Class
class GeoTexturedRenderObj : public SurfaceRenderObject  {
public:
  GeoTexturedRenderObj();
  GeoTexturedRenderObj(GeoTexturedModelObj*);
  virtual ~GeoTexturedRenderObj();

  virtual const char* ClassName(void) { return "GeoTexturedRenderObj"; };

  virtual void OnProcess(LayerList* Layers);
  void BuildGeoTextures();

  virtual void SetGeoImgFile( const csString& GeoImgFile );
  virtual BOOL GeoTextured(void) { return m_GeoTextured; };
  virtual void SetGeoTextured(BOOL bGeoTextured) { m_GeoTextured = bGeoTextured; };
  virtual BOOL HasGeoTextureFile(void) { return m_pGeoImg && m_pGeoImg->GetTextureFile().isEmpty()==FALSE; };
  virtual BOOL HasTextureFile(void) { return HasGeoTextureFile(); };

  csString GetGeoImgFile(void) { return m_pGeoImg == NULL ? "" : m_pGeoImg->GetTextureFile(); };
  GeoTextureImage* GetGeoImgFileObj(void) { return m_pGeoImg; };
  void SetGeoImageParams( const unsigned int width,
	  const unsigned int height,
	  const unsigned char bits_per_pixel,
	  const unsigned char *bits,
	  const double map_resolution[2],
	  const double image_origin[2],
	  const unsigned char* palette=NULL);

  BOOL BuildGeometry(ScTIN *Tin );

  inline GeoTexturedModelObj* GetModel(void) { return (GeoTexturedModelObj*)MOB; };
  inline void SetModel(GeoTexturedModelObj *GTMO) {MOB = (ModelObjectBase*)GTMO;};
  __declspec(property(get = GetModel, put = SetModel)) GeoTexturedModelObj *GTMO;
private:
  BOOL m_GeoTextured;
  GeoTextureImage* m_pGeoImg;
};

class TexturedRenderObj : public SurfaceRenderObject {
public:
  TexturedRenderObj();
  TexturedRenderObj(TexturedModelObj*);
  virtual ~TexturedRenderObj();
  virtual const char* ClassName(void) { return "TexturedRenderObj"; };

  DefList m_Textures;

  BOOL AddTextureImage( const long width, const long height, const unsigned char nBPP, const unsigned char *bits, const double scale );
  void ClearTextures(void);

  virtual BOOL HasTextures(void) { return (m_bTextured && m_Textures.CountObjects() > 0); };
  virtual void SetHasTextures(BOOL bTextured) { m_bTextured = bTextured; };
  virtual BOOL HasTextureFile(void) { return HasTextures(); };

  TextureImage* MakeTiles(void);

  inline csString& GetLayerName(void) { return ((TexturedModelObj*)MOB)->LayerName; };
  inline void SetLayerName(csString& LayerName) {((TexturedModelObj*)MOB)->LayerName = LayerName;};
  __declspec(property(get = GetLayerName, put = SetLayerName)) csString& LayerName;

  inline const char* GetTextureName(void) { return ((TexturedModelObj*)MOB)->TextureName.Get(); };
  inline void SetTextureName(const char* TextureName) { ((TexturedModelObj*)MOB)->TextureName = TextureName; };
  __declspec(property(get = GetTextureName, put = SetTextureName)) const char* TextureName;

  inline const char* GetTexturePath(void) { return ((TexturedModelObj*)MOB)->TexturePath.Get(); };
  inline void SetTexturePath(const char* TexturePath) { ((TexturedModelObj*)MOB)->TexturePath = TexturePath; };
  __declspec(property(get = GetTexturePath, put = SetTexturePath)) const char* TexturePath;
private:
  BOOL m_bTextured;
};

class EntityRenderObject : public RenderObject {
public:  
  EntityRenderObject(EntityModelObject*);
  EntityRenderObject(lwpoint *Data, long Shape);
  virtual ~EntityRenderObject();

  virtual const char* ClassName(void) { return "EntityRenderObject"; };

  void RenderAnEntity(void);
  virtual void OnProcess(LayerList* Layers);
  virtual BOOL IsZeroElev() { return !NotZero; };
  virtual RenderObject *GetTopRO() { return TopRO; };
  virtual void SetTopRO(RenderObject *TRO) { this->TopRO=TRO; };
  virtual BOOL GetBoundingCube( double& dMinX, double& dMinY, double& dMinZ,
                            double& dMaxX, double& dMaxY, double& dMaxZ);

  inline EntityModelObject* GetModel(void) { return (EntityModelObject*)MOB; };
  inline void SetModel(EntityModelObject *EMO) {MOB = (ModelObjectBase*)EMO;};
  __declspec(property(get = GetModel, put = SetModel)) EntityModelObject *EMO;

  inline lwpoint* Getlwroot(void) { return ((EntityModelObject*)MOB)->m_lwroot; };
  inline void Setlwroot(lwpoint* m_lwroot) {((EntityModelObject*)MOB)->m_lwroot = m_lwroot;};
  __declspec(property(get = Getlwroot, put = Setlwroot)) lwpoint* m_lwroot;

  inline long GetShape(void) { return ((EntityModelObject*)MOB)->Shape; };
  inline void SetShape(long Shape) {((EntityModelObject*)MOB)->Shape = Shape;};
  __declspec(property(get = GetShape, put = SetShape)) long Shape;

  inline long GetColor(void) { return ((EntityModelObject*)MOB)->Color; };
  inline void SetColor(long Color) {((EntityModelObject*)MOB)->Color = Color;};
  __declspec(property(get = GetColor, put = SetColor)) long Color;

  inline BOOL GetClosed(void) { return ((EntityModelObject*)MOB)->Closed; };
  inline void SetClosed(BOOL Closed) {((EntityModelObject*)MOB)->Closed = Closed;};
  __declspec(property(get = GetClosed, put = SetClosed)) BOOL Closed;

  inline BOOL GetIs3DFace(void) { return ((EntityModelObject*)MOB)->bIs3DFace; };
  inline void SetIs3DFace(BOOL bIs3DFace) {((EntityModelObject*)MOB)->bIs3DFace = bIs3DFace;};
  __declspec(property(get = GetIs3DFace, put = SetIs3DFace)) BOOL bIs3DFace;

  inline BOOL GetIsBlockMember(void) { return ((EntityModelObject*)MOB)->bIsBlockMember; };
  inline void SetIsBlockMember(BOOL bIsBlockMember) {((EntityModelObject*)MOB)->bIsBlockMember = bIsBlockMember;};
  __declspec(property(get = GetIsBlockMember, put = SetIsBlockMember)) BOOL bIsBlockMember;

  inline csString& GetHandle(void) { return ((EntityModelObject*)MOB)->Handle; };
  inline void SetHandle(csString& Handle) {((EntityModelObject*)MOB)->Handle = Handle;};
  __declspec(property(get = GetHandle, put = SetHandle)) csString& Handle;

  inline csString& GetLayerName(void) { return ((EntityModelObject*)MOB)->LayerName; };
  inline void SetLayerName(csString& LayerName) {((EntityModelObject*)MOB)->LayerName = LayerName;};
  __declspec(property(get = GetLayerName, put = SetLayerName)) csString& LayerName;

protected:
  void Init();
  RenderObject *TopRO;
  ads_name ename;
  int NotZero;
};

class SelectionSetRenderObject : public RenderObject {
friend class BlockRefRenderObject;
public:  
  SelectionSetRenderObject(SelectionSetModelObject*, ScRenderObjectsList& SSROs);
  virtual ~SelectionSetRenderObject();

  // Each subclass needs to know how to perform compositing of its Model Class.
  virtual void Composite(void) {if (MOB==NULL) MOB = (ModelObjectBase*)(new SelectionSetModelObject); };

  virtual const char* ClassName(void) { return "SelectionSetRenderObject"; };

  virtual void OnProcess(LayerList* Layers);
  virtual BOOL GetBoundingCube( double& dMinX, double& dMinY, double& dMinZ,
                            double& dMaxX, double& dMaxY, double& dMaxZ);
  
  virtual void SetParent(ScScene *Parent);

  inline BOOL GetIsBlock(void) { return ((SelectionSetModelObject*)MOB)->GetIsBlock(); };
  void SetIsBlock(BOOL IsBlockDef=TRUE) {
    ((SelectionSetModelObject*)MOB)->SetIsBlock(IsBlockDef);
    ZeroTrans = !IsBlockDef;
  };

  inline ScRenderObjectsList* GetEntities(void) { return (ScRenderObjectsList*)(((SelectionSetModelObject*)MOB)->Entities); };
  __declspec(property(get = GetEntities, put = SetEntities)) ScRenderObjectsList *Entities;

  inline ScRenderObjectsList* GetBlockRefs(void) { return (ScRenderObjectsList*)(((SelectionSetModelObject*)MOB)->BlockRefs); };
  __declspec(property(get = GetBlockRefs, put = SetBlockRefs)) ScRenderObjectsList *BlockRefs;
protected:
  void SetEntities(ScRenderObjectsList *Entities) {((SelectionSetModelObject*)MOB)->Entities = (ScModelObjectsList*)Entities;};

  void SetBlockRefs(ScRenderObjectsList *BlockRefs) {((SelectionSetModelObject*)MOB)->BlockRefs = (ScModelObjectsList*)BlockRefs;};

  inline SelectionSetModelObject* GetModel(void) { return (SelectionSetModelObject*)MOB; };
  inline void SetModel(SelectionSetModelObject *SSMO) {MOB = (ModelObjectBase*)SSMO;};
  __declspec(property(get = GetModel, put = SetModel)) SelectionSetModelObject *SSMO;

  void DisplayEntities(std::string LayerName, LayerDef* LayerDefinition);
};

class BlockRefRenderObject : public RenderObjectBase {
public:
  BlockRefRenderObject();
  BlockRefRenderObject(BlockRefModelObject* BlockRef, ScRenderObjectsList& SSROs);

  virtual ~BlockRefRenderObject();

  // Each subclass needs to know how to perform compositing of its Model Class.
  virtual void Composite(void) {if (MOB==NULL) MOB = (ModelObjectBase*)(new BlockRefModelObject); };

  virtual const char* ClassName(void) { return "BlockRefRenderObject"; };

  void Init();
  virtual void OnProcess(LayerList* Layers);

  virtual BOOL GetBoundingCube( double& dMinX, double& dMinY, double& dMinZ,
                            double& dMaxX, double& dMaxY, double& dMaxZ);

  virtual RenderObject *GetTopRO() { return TopRO; };
  virtual void SetTopRO(RenderObject *TRO) { this->TopRO=TRO; };
  virtual BOOL IsZeroElev(void);

  void SetBlockDef(SelectionSetRenderObject *BlockDef) {
    if (BlockDef)
      ((BlockRefModelObject*)MOB)->BlockModelDef = (SelectionSetModelObject*)(BlockDef->GetModel());
    else
      ((BlockRefModelObject*)MOB)->BlockModelDef = NULL;
    BlockDefPrivate = BlockDef;
  };
  SelectionSetRenderObject* GetBlockDef(void) { return BlockDefPrivate; };
  // Using property insures changes to BlockDef are applied to BlockRefModelObject::BlockModelDef
  __declspec(property(get = GetBlockDef, put = SetBlockDef)) SelectionSetRenderObject *BlockDef;

  inline BlockRefModelObject* GetModel(void) { return (BlockRefModelObject*)MOB; };
  inline void SetModel(BlockRefModelObject *BRMO) {MOB = (ModelObjectBase*)BRMO;};
  __declspec(property(get = GetModel, put = SetModel)) BlockRefModelObject *BRMO;

  inline csString& GetHandle(void) { return ((BlockRefModelObject*)MOB)->Handle; };
  inline void SetHandle(csString& Handle) {((BlockRefModelObject*)MOB)->Handle = Handle;};
  __declspec(property(get = GetHandle, put = SetHandle)) csString& Handle;

  inline csString& GetLayerName(void) { return ((BlockRefModelObject*)MOB)->LayerName; };
  inline void SetLayerName(csString& LayerName) {((BlockRefModelObject*)MOB)->LayerName = LayerName;};
  __declspec(property(get = GetLayerName, put = SetLayerName)) csString& LayerName;

  inline csString& GetBlockName(void) { return ((BlockRefModelObject*)MOB)->BlockName; };
  inline void SetBlockName(csString& BlockName) {((BlockRefModelObject*)MOB)->BlockName = BlockName;};
  __declspec(property(get = GetBlockName, put = SetBlockName)) csString& BlockName;

  inline double GetYScale(void) { return ((BlockRefModelObject*)MOB)->YScale; };
  inline void SetYScale(double YScale) {((BlockRefModelObject*)MOB)->YScale = YScale;};
  __declspec(property(get = GetYScale, put = SetYScale)) double YScale;

  inline double GetZScale(void) { return ((BlockRefModelObject*)MOB)->ZScale; };
  inline void SetZScale(double ZScale) {((BlockRefModelObject*)MOB)->ZScale = ZScale;};
  __declspec(property(get = GetZScale, put = SetZScale)) double ZScale;
protected:
  RenderObject *TopRO;
  ads_name ename;
private:
  SelectionSetRenderObject *BlockDefPrivate;
};

BOOL AddFaceToTin(lwpoint *points,ScTIN** tin, ScSmartModel** model);
long BuildTin(ScModelObjectsList* ModelObjects, ScTIN** tin, ScSmartModel** model, const char* LayerName=NULL);

class Model_sgl;
class SkyRenderer : public RenderObject {
public:
  SkyRenderer();
  virtual ~SkyRenderer();

  virtual const char* ClassName(void) { return "SkyRenderer"; };

  BOOL Init(const long width, const long height, const unsigned char *bits);
  virtual void OnProcess(LayerList* Layers);
  void Clean(void);

protected:
  Model_sgl*    m_pSkyModel;
  Shader*       m_pSkyShader;
  unsigned int	m_CloudTexture;
};

#pragma warning( pop ) // C4250

#endif //NO_GLVIEW

#endif //_RENDEROBJECT_H_