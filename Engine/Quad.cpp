#include "pch.h"
#include "Quad.h"


Quad::Quad(float width, float height) {
	vertex = new float[12] {	-width/2,  height/2, 0,			
								-width/2, -height/2, 0,		
								 width/2, -height/2, 0,		
								 width/2,  height/2, 0		
									   		 
								/* 0.5, -height/2, 0.5,
								 0.5, -height/2, -0.5,
								 0.5,  height/2, -0.5,
								 0.5,  height/2,  0.5,
									  		 
								-0.5,  height/2,  -0.5,
								-0.5, -height/2, -0.5,
								 0.5, -height/2, -0.5,
								 0.5,  height/2, -0.5,
									   		 
								-0.5, -height/2, 0.5,
								-0.5, -height/2, -0.5,
								-0.5,  height/2, -0.5,
								-0.5,  height/2,  0.5,
									 		 
								-0.5,  height/2, 0.5,
								-0.5,  height/2, -0.5,
								 0.5,  height/2, -0.5,
								 0.5,  height/2,  0.5,
									  		 
								-0.5, -height/2, 0.5,
								-0.5, -height/2, -0.5,
								 0.5, -height/2, -0.5,
								 0.5, -height/2,  0.5,*/
	};

	normals = new float[12]{ 0.0, 0.0, 1.0,
							  0.0, 0.0, 1.0,
							  0.0, 0.0, 1.0,
							  0.0, 0.0, 1.0

							  /*1.0, 0.0, 0.0,
							  1.0, 0.0, 0.0,
							  1.0, 0.0, 0.0,
							  1.0, 0.0, 0.0,

							  0.0, 0.0, 1.0,
							  0.0, 0.0, 1.0,
							  0.0, 0.0, 1.0,
							  0.0, 0.0, 1.0,

							  -1.0, 0.0, 0.0,
							  -1.0, 0.0, 0.0,
							  -1.0, 0.0, 0.0,
							  -1.0, 0.0, 0.0,

							  0.0, 1.0, 0.0,
							  0.0, 1.0, 0.0,
							  0.0, 1.0, 0.0,
							  0.0, 1.0, 0.0,

							  0.0, -1.0, 0.0,
							  0.0, -1.0, 0.0,
							  0.0, -1.0, 0.0,
							  0.0, -1.0, 0.0,*/
	};

	texCoords = new float[8]{   0.0, 0.0,
								 0.0, 1.0,
								 1.0, 1.0,
								 1.0, 0.0

								 /*1.0, 0.0,
								 0.0, 0.0,
								 0.0, 1.0,
								 1.0, 1.0,

								1.0, 0.0,
								1.0, 1.0,
								 0.0, 1.0,
								 0.0, 0.0,

								 0.0, 0.0,
								1.0, 0.0,
								 1.0, 1.0,
								 0.0, 1.0,

								 1.0, 0.0,
								1.0, 1.0,
								 0.0, 1.0,
								 0.0, 0.0,

								 0.0, 0.0,
								0.0, 1.0,
								 1.0, 1.0,
								 1.0, 0.0,*/
	};
}

void Quad::render() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertex);

	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, normals);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords);

	glDrawArrays(GL_QUADS, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}