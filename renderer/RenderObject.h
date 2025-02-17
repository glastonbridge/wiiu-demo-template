#pragma once

#include <cstdint>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <gx2r/buffer.h>
#include <memory>
#include <vector>

struct Model;
struct RenderObjectImpl;
struct RenderMaterial;

enum BufferType {
  VERTEX,
  TEXCOORD,
  NORMAL,
  COLOR,
  BONE_IDX,
  BONE_WEIGHT,
  _MAX
};

enum UniformType { CAMERA_PROJECTION, TRANSFORM, BONE_TRANSFORM, CAMERA_VIEW };

/**
 * Data related to a specific instance of a render object.
 * This is used for variable uniforms such as object transform.
 */
struct RenderInstance {
  RenderInstance();
  RenderInstance(RenderInstance &&) = default;
  ~RenderInstance();

  void setUniformFloatMat(UniformType bt, const float *mat, size_t numFloats);

  GX2RBuffer transformBuffer = {};
  GX2RBuffer boneTransformBuffer = {};

  size_t numBones = 0;
  std::unique_ptr<float[]> boneMatInterpBuffer;
};

/**
 * Data related to a camera view of a render object.
 * This is used for variable uniforms such as projection and camera matrix.
 */
struct RenderView {
  RenderView();
  ~RenderView();

  GX2RBuffer projectionBuffer = {};
  GX2RBuffer viewBuffer = {};
  GX2RBuffer extraBuffer = {};

  void setUniformFloatMat(UniformType bt, const float *mat, size_t numFloats);
  void setExtraUniform(int index, glm::vec4 data);
};

/**
 * The parts of an object that are concerned with the rendering implementation
 * This contains the mesh data, material etc. If we don't expect it to be
 * modified after creation, it goes here. A single render object can be rendered
 * multiple times in different places in a scene.
 */
struct RenderObject {
  virtual ~RenderObject() = default;
  virtual void render(RenderInstance const &ri, RenderView const &rv) const = 0;
  virtual void setMaterial(RenderMaterial *material) = 0;
  virtual RenderMaterial *getMaterial() const = 0;

  // Messing with the buffers involves doing GX2-specific memory locking
  virtual void setAttribBuffer(BufferType bt, const void *data,
                               uint32_t elemSize, size_t elemCount) = 0;

  std::vector<std::vector<glm::mat4>> animFrames;

  // Gets the bone matrices for a given animation frame into the buffer.
  void applyAnimation(float frame, RenderInstance &instance) const;

  static std::unique_ptr<RenderObject> create();
  static std::unique_ptr<RenderObject> create(Model const &model);
  static std::unique_ptr<RenderObject> create(Model &&model);

  void load(const char *path, const char *name, RenderMaterial *material);

protected:
  RenderObject() = default;
};
