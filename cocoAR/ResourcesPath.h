//
//  ResourcesPath.h
//  ARcocos
//
//  Created by Javier de la Peña Ojea on 27/06/11.
//  Copyright 2011 Artifact. All rights reserved.
//

#include <iostream>

#ifndef ARcocos_ResourcesPath_h
#define ARcocos_ResourcesPath_h

class ResourcesPath{
public:
  static void splitpath(const char* path, char* drv, char* dir, char* name, char* ext);
  static std::string getTemporalFile(const char *Filename);
  static std::string getRealPath(std::string path);
};

#endif
