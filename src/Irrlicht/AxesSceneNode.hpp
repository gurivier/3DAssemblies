#ifndef AXESSCENENODE_H
#define AXESSCENENODE_H

class AxesSceneNode : public scene::ISceneNode {

private:

  scene::SMeshBuffer ZMeshBuffer;
  scene::SMeshBuffer YMeshBuffer;
  scene::SMeshBuffer XMeshBuffer;
       
  video::SColor ZColor;
  video::SColor YColor;
  video::SColor XColor;

public:

  AxesSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id) ;

  virtual ~AxesSceneNode() ;

  virtual void OnRegisterSceneNode() ;

  virtual void render() ;

  void setAxesCoordinates() ;

  virtual const core::aabbox3d<f32>& getBoundingBox() const ;

  void setAxesScale(f32 scale) ;

} ;

#endif /* AXESSCENENODE_H */
