#ifndef MODELLOADER_H
#define MODELLOADER_H

#include "RenderObject.h"
//#include <Windows.h>

int LoadModel(char* szFile, ScRenderObjectsList* EntityRenderObjects) {
  int code = 0;
  csString ErrMsg;
  XMLModel ModelLoad;
  ScModelObjectsList* ModelObjects = new ScModelObjectsList;
  LayerList* ListOfLayers = new LayerList;
  long Count;

  code = ModelLoad.ReadXML(szFile, *ModelObjects, ErrMsg, *ListOfLayers, &Parms);
  BOOL bContainsSurface = FALSE;
  if (code) {
    ModelObjectBase* pMO;
    const char* MOName;
    for (Count=1;Count<=ModelObjects->ItemNum;Count++) {
      pMO = ModelObjects->GetModelObject(Count);
      if (pMO) {
        MOName = pMO->GetName();
        if (stricmp(MOName, "FlyBy.SurfaceObj")==0) {
          bContainsSurface = TRUE;
          break;
        }
      }
    }
  }

  if (bContainsSurface) {
    Parms.CleanLayers();
    Parms.SetListOfLayers(ListOfLayers);

    // TODO... Standalone needs separate UI for this.
    int force_soft_opengl=0;
    //get_lisp_int(&force_soft_opengl,"force_soft_opengl");
    Parms.SetSoftOpenGL(force_soft_opengl);

    RenderObjectBuilder Builder;
    const char* pszTextureName = NULL;
    for (Count=1;Count<=ModelObjects->ItemNum;Count++) {
      ModelObjectBase* pMO = ModelObjects->GetModelObject(Count);
      if (pMO) {
        RenderObjectBase* RenderObj = Builder.BuildRenderObject(pMO, *EntityRenderObjects);
        if (RenderObj)
          EntityRenderObjects->AddObject(RenderObj);
        const char* MOName = pMO->GetName();
        if (stricmp(MOName, "Flyby.MainModelObj")==0) {
          ModelModelObject* ThisMMO = dynamic_cast<ModelModelObject*>(pMO);
          if (ThisMMO) {
            ThisMMO->SetOwnModel(FALSE);      // Steal the model away from the model object
            Parms.SetModel((ScSmartModel*)ThisMMO->GetModel());
          }
        }
        else if (stricmp(MOName, "FlyBy.SurfaceObj")==0) {
          SurfaceModelObject* pSMO = dynamic_cast<SurfaceModelObject*>(pMO);
          if (pSMO) {
            char usrdir[MAX_PATH];
            GetTempPath(MAX_PATH, usrdir);
            csString csFullPath = usrdir;
            csString csFilePath;
            csFullPath += "modeltemp";
            EnsureDirectory(csFullPath.Get());
            csFullPath += "\\";
            csFullPath += pSMO->GetSurfaceFile();

            ScSurface* Surface = new ScSurface(csFullPath.Get(),NULL,TRUE);
            if (Surface->Status!=1 && Surface!=NULL)
              delete Surface;
            else {
              ScSurface* pOldSurface = Parms.GetSurface();
              if (pOldSurface)
                delete pOldSurface;
              Parms.SetSurface(Surface);
            }
            TexturedModelObj* pTMO = dynamic_cast<TexturedModelObj*>(pSMO);
            if (pTMO)
              pszTextureName = pTMO->GetTextureName();
          }
        }
      }
    }
    // Since these come through the Parms and will be created from same,
    // delete these copies as they will conflict.
    EntityRenderObjects->RemoveObjectsByName("Flyby.MainModelObj");
    EntityRenderObjects->RemoveObjectsByName("FlyBy.SurfaceObj");
    code = 1;
  }
  else {
    MessageBox(NULL, "Does not appear to be a flyby 3dx model file.", MB_ICONEXCLAMATION | MB_OK );
  }
  return code;
}

















#endif // MODELLOADER_H
