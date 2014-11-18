//
//  MeshObject.h
//  project
//
// Class that loads and draws the mesh file
//
//  Created by Alona Bas on 5/25/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#include "MeshObject.h"


MeshObject::MeshObject() :
_vao(0), _vbo(0), _ebo(0), _vboNormal(0),_radius(2.0f), _modelScale(1.0f), _modelRotate(1.0f), _modelTranslate(1.0f), _moveToCenter(1.0f)
{

}
MeshObject::~MeshObject() {
	if (_vbo != 0)
		glDeleteBuffers(1, &_vbo);
	if (_ebo != 0)
		glDeleteBuffers(1, &_ebo);
	if (_vboNormal != 0)
		glDeleteBuffers(1, &_vboNormal);
	if (_vao != 0)
		glDeleteVertexArrays(1, &_vao);
}

void MeshObject::init(GLint program, const char* meshFile) {
    _program = program;
	Mesh mesh;
    // mesh.request_vertex_texcoords2D();
	if (!OpenMesh::IO::read_mesh(mesh, meshFile)) {
		// if we didn't make it, exit...
		fprintf(stderr, "Error loading mesh, Aborting.\n");
		exit(-1);
	} else {
		std::cout << "Loaded a mesh with " << mesh.n_vertices() << " vertices."
				<< std::endl;
	}
    
    // mesh.request_vertex_texcoords2D();
    // add normals to facees
	mesh.request_face_normals();
	// adding normals to each vector
	mesh.request_vertex_normals();
	// let the mesh update the normals
	mesh.update_normals();
    
	// The vector will be use as a buffer, before transfer the vertices to
	// the VBO:
	std::vector<glm::vec4> vertices(mesh.n_vertices());
	std::vector<glm::vec3> normals(mesh.n_vertices());

	Mesh::VertexIter vertexIter = mesh.vertices_begin();
	Mesh::Point p = mesh.point(vertexIter);
	Mesh::Normal n;
	const float fm = std::numeric_limits<float>::max();
	vec3 cur_center = vec3(0.0f);
	size_t i = 0;
	for (vertexIter = mesh.vertices_begin(); vertexIter != mesh.vertices_end();
			++vertexIter) {
		n = mesh.normal(vertexIter);
		glm::vec3 normal(n[0], n[1], n[2]);
		normals[i] = normal;

		p = mesh.point(vertexIter);
		cur_center = cur_center + vec3(p[0], p[1], p[2]);
		// define center of object to be at (0,0,0)
		glm::vec4 position(p[0], p[1], p[2], 1.0f);
		vertices[i] = position;
		i++;
	}
	float radius = -fm;
	cur_center = cur_center / (float) vertices.size();
	for (vertexIter = mesh.vertices_begin(); vertexIter != mesh.vertices_end();
			++vertexIter) {
		p = mesh.point(vertexIter);
		radius = fmax(radius, length(vec3(p[0], p[1], p[2]) - cur_center));
	}
	float newRadius = 2.0f / radius;
	// normalize objects coordinates,define basic translation and scale
	_moveToCenter = glm::translate(glm::mat4(1.f), -cur_center);
	glm::vec3 scalePositionVector = glm::vec3(newRadius);
	_moveToCenter = glm::scale(mat4(1.0f), scalePositionVector)
			* _moveToCenter;

	i = 0;
	std::vector<face_indices_t> faces(mesh.n_faces());

	for (Mesh::FaceIter faceIter = mesh.faces_begin();
			faceIter != mesh.faces_end(); ++faceIter) {
		Mesh::ConstFaceVertexIter cfvlt = mesh.cfv_iter(faceIter.handle());
		face_indices_t face;
		face.a = cfvlt.handle().idx();
		++cfvlt;
		face.b = cfvlt.handle().idx();
		++cfvlt;
		face.c = cfvlt.handle().idx();
		faces[i++] = face;
	}

	_numTriangles = faces.size();

	mesh.release_vertex_normals();
	mesh.release_face_normals();
    mesh.release_vertex_texcoords2D();
    
	// get uniforms
	_MnormalUniform = glGetUniformLocation(program, "normalMat");
    _Muniform = glGetUniformLocation(program, "modelMat");
    _Vuniform = glGetUniformLocation(program, "viewMat");
    _Puniform = glGetUniformLocation(program, "projMat");
    
	// atributes
	_attrib[0] = glGetAttribLocation(program, "position");
	_attrib[1] = glGetAttribLocation(program, "normal");
    
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	// Create and load vertex data into a Vertex Buffer Object:
	glGenBuffers(1, &_vbo);
	// Tells OpenGL that there is vertex data in this buffer object and what form that vertex data takes:
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * vertices.size(),
			&(vertices[0]), GL_STATIC_DRAW);

	// Create and load face (elements) data into an Element Buffer Object:
	glGenBuffers(1, &_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(face_indices_t) * faces.size(),
			&(faces[0]), GL_STATIC_DRAW);
	// Obtain attribute handles:

	glEnableVertexAttribArray(_attrib[0]);
	glVertexAttribPointer(_attrib[0], // attribute handle
			4,          // number of scalars per vertex
			GL_FLOAT,   // scalar type
			GL_FALSE, 0, 0);

	glGenBuffers(1, &_vboNormal);
	glBindBuffer(GL_ARRAY_BUFFER, _vboNormal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * normals.size(),
			&(normals[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(_attrib[1]);
	glVertexAttribPointer(_attrib[1], // attribute handle
			3,          // number of scalars per vertex
			GL_FLOAT,   // scalar type
			GL_FALSE, 0, 0);
    
	glBindVertexArray(0);
}

void MeshObject::setRotation(mat4 matrix)
{
    _modelRotate = matrix;
}

void MeshObject::setTranslation(mat4 matrix)
{
    _modelTranslate = matrix;
}

void MeshObject::setScale(mat4 matrix)
{
    _modelScale = matrix;
}
float MeshObject::getRadius()
{
    return _radius;
}

void MeshObject::draw() {

    Camera* camera = Camera::instance();
    mat4 modelMat =  _modelTranslate * _modelScale * _modelRotate* _moveToCenter;
	glm::mat4x4 model_vew_mat = camera->getV() * modelMat;
	// inverse transpose for normals
	glm::mat4 normalMat = glm::transpose(glm::inverse(model_vew_mat));
    
	glUniformMatrix4fv(_MnormalUniform, 1, GL_FALSE, value_ptr(normalMat));
    glUniformMatrix4fv(_Muniform, 1, GL_FALSE, value_ptr(modelMat));
    glUniformMatrix4fv(_Vuniform, 1, GL_FALSE, value_ptr(camera->getV()));
    glUniformMatrix4fv(_Puniform, 1, GL_FALSE, value_ptr(camera->getP()));
    
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(_vao);

	glDrawElements(GL_TRIANGLES, 3 * (int) _numTriangles, GL_UNSIGNED_INT, 0);
	glDisable(GL_BLEND);
	// Unbind the Vertex Array object
	glBindVertexArray(0);
}
