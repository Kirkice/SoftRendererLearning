#pragma once
typedef struct
{
	float m[4][4];
} Matrix4x4;

typedef struct
{
	float x, y, z, w;
} Vector4;

typedef struct
{
	float x, y, z;
} Vector3;

typedef struct
{
	float x, y;
} Vector2;

typedef struct
{
	float r, g, b;
} Color;


typedef struct
{
	int vertex_index[3];
	int texcoord_index[3];
	int normal_index[3];
	Vector3 normal;
}Face;

typedef struct
{
	float u, v;
} Texcoord;

typedef struct
{
	float		fovy;
	float		aspect_ratio;
	float		clip_near;
	float		clip_far;

	Vector3		position;
	Vector3		lookat;
	Vector3		up_direction;

	Matrix4x4	matrix_projection;
	Matrix4x4	matrix_view;

} Camera;


typedef struct
{
	int width;
	int height;
	UINT32 texbuf[256][256]; 

}Texture;

typedef struct
{
	char name[64];

	unsigned char visible;
	unsigned char useuv;

	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	unsigned int	n_vertex;
	unsigned int	n_face;
	unsigned int    n_texcord;
	unsigned int    n_normal;

	Texcoord* texcoords;	//[uv����]
	Vector3* vertices;		//[��������]
	Vector3* normals;		//[��������]
	Face* faces;			//[������]

	Matrix4x4 matrix_world;

	Texture texture;

}Mesh;


typedef struct
{
	Vector4 pos;
	Texcoord texcoord;
	Color col;
	float rhw;					//Reciprocal of Homogeneous W
	Vector3 normal;
	Vector3 pos_world;
} Vertex;

enum
{
	LIGHT_DIRECTIONAL = 0,
	LIGHT_POINT = 1,
	LIGHT_SPOT = 2
};

typedef struct
{
	unsigned char light_type;
	Color	color;
	Vector3	direction;
	Vector3	position;
}Light;