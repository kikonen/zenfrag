
#ifndef __MD5MODEL_H__
#define __MD5MODEL_H__

/* Vectors */
typedef float vec2_t[2];
typedef float vec3_t[3];

/* Quaternion (x, y, z, w) */
typedef float quat4_t[4];

enum {
  X, Y, Z, W
};

/* Joint */
struct md5_joint_t
{
  char name[64];
  int parent;

  vec3_t pos;
  quat4_t orient;
};

/* Vertex */
struct md5_vertex_t
{
  vec2_t st;

  int start; /* start weight */
  int count; /* weight count */
};

/* Triangle */
struct md5_triangle_t
{
  int index[3];
};

/* Weight */
struct md5_weight_t
{
  int joint;
  float bias;

  vec3_t pos;
};

/* Bounding box */
struct md5_bbox_t
{
  vec3_t min;
  vec3_t max;
};

/* MD5 mesh */
struct md5_mesh_t
{
  struct md5_vertex_t *vertices;
  struct md5_triangle_t *triangles;
  struct md5_weight_t *weights;

  int num_verts;
  int num_tris;
  int num_weights;

  char shader[256];
};

/* MD5 model structure */
struct md5_model_t
{
  struct md5_joint_t *baseSkel;
  struct md5_mesh_t *meshes;

  int num_joints;
  int num_meshes;
};

/* Animation data */
struct md5_anim_t
{
  int num_frames;
  int num_joints;
  int frameRate;

  struct md5_joint_t **skelFrames;
  struct md5_bbox_t *bboxes;
};

/* Animation info */
struct anim_info_t
{
  int curr_frame;
  int next_frame;

  double last_time;
  double max_time;
};

void init (const char *filename, const char *animfile);
/**
 * Quaternion prototypes
 */
void Quat_computeW (quat4_t q);
void Quat_normalize (quat4_t q);
void Quat_multQuat (const quat4_t qa, const quat4_t qb, quat4_t out);
void Quat_multVec (const quat4_t q, const vec3_t v, quat4_t out);
void Quat_rotatePoint (const quat4_t q, const vec3_t in, vec3_t out);
float Quat_dotProduct (const quat4_t qa, const quat4_t qb);
void Quat_slerp (const quat4_t qa, const quat4_t qb, float t, quat4_t out);

/**
 * md5mesh prototypes
 */
int ReadMD5Model (const char *filename, struct md5_model_t *mdl);
void FreeModel (struct md5_model_t *mdl);
void PrepareMesh (const struct md5_mesh_t *mesh,
		  const struct md5_joint_t *skeleton);
void AllocVertexArrays ();
void FreeVertexArrays ();
void DrawSkeleton (const struct md5_joint_t *skeleton, int num_joints);

/**
 * md5anim prototypes
 */
int CheckAnimValidity (const struct md5_model_t *mdl,
		       const struct md5_anim_t *anim);
int ReadMD5Anim (const char *filename, struct md5_anim_t *anim);
void FreeAnim (struct md5_anim_t *anim);
void InterpolateSkeletons (const struct md5_joint_t *skelA,
			   const struct md5_joint_t *skelB,
			   int num_joints, float interp,
			   struct md5_joint_t *out);
void Animate (const struct md5_anim_t *anim,
	      struct anim_info_t *animInfo, double dt);

void display ();
#endif /* __MD5MODEL_H__ */
