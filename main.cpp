/*
   Copyright (C) 2014  Mutzii

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#include "index.h"
#include "openpolygon.h"
#include "bvhloader.h"
#include "testscene.h"
#include "rendereditor.h"
#include "frustum.h"
#include "animationmanager.h"

#include "resourcemanager.h"


using namespace Engine;

int main()
{

  ResourceManager * manager = ResourceManager::getSingeltonPtr();
  //manager->createResource("resource.dat","./Shaders/");
  //manager->openResourceFile("resource.dat");

  OpenPolygon engine;
  //engine.writeConfig("./scripts/config.conf");        // Load GUI - write config

  DisplayConfig config("./scripts/config.conf");        // Read from File
  GLFWDisplay * display = engine.initialize( &config ); // Create Display

  //FT_Face face = FontManager::getSingletonPtr()->createFontFace("./resource/blue.ttf");
  //FontManager::getSingletonPtr()->load(face,"!#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");


  /* Register .bvh extension for Motions ( Animation ) */

  Arch::DatabaseManager * database_manager = Arch::DatabaseManager::getSingletonPtr();
  AnimationManager   * animation_manager   = (AnimationManager   *) database_manager->getDatabase("AnimationManager");
  animation_manager->registerLoader( new BvhLoader() );

  /* Add a Scene */
  RenderManager::getSingletonPtr()->addFrameListener( new TestScene() );

  /* Initialize OpenGL and start RenderLoop with Display */
  engine.RenderLoop( display );
  engine.finish();

  return 0;
}
