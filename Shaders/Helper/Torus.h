#ifndef TORUS_H
#define TORUS_H

#include "TriangleMesh.h"

class Torus : public TriangleMesh
{
public:
    Torus(GLfloat outerRadius, GLfloat innerRadius, GLuint nsides, GLuint nrings);
};

#endif // TORUS_H
