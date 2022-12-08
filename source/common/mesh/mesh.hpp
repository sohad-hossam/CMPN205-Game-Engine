#pragma once

#include <glad/gl.h>
#include "vertex.hpp"

namespace our {

    #define ATTRIB_LOC_POSITION 0
    #define ATTRIB_LOC_COLOR    1
    #define ATTRIB_LOC_TEXCOORD 2
    #define ATTRIB_LOC_NORMAL   3

    class Mesh {
        // Here, we store the object names of the 3 main components of a mesh:
        // A vertex array object, A vertex buffer and an element buffer
        unsigned int VBO, EBO;
        unsigned int VAO;
        // We need to remember the number of elements that will be draw by glDrawElements 
        GLsizei elementCount;
    public:

        // The constructor takes two vectors:
        // - vertices which contain the vertex data.
        // - elements which contain the indices of the vertices out of which each rectangle will be constructed.
        // The mesh class does not keep a these data on the RAM. Instead, it should create
        // a vertex buffer to store the vertex data on the VRAM,
        // an element buffer to store the element data on the VRAM,
        // a vertex array object to define how to read the vertex & element buffer during rendering 
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& elements )
        {
            //TODO: (Req 2) Write this function
            // remember to store the number of elements in "elementCount" since you will need it for drawing
            // For the attribute locations, use the constants defined above: ATTRIB_LOC_POSITION, ATTRIB_LOC_COLOR, etc
             elementCount=(GLsizei)elements.size(); //storing with each call the element count we end at the summition to use in draw
             setupMesh(vertices,elements); //call of set up on every construction

            
        }

        void setupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& elements )
        {
            glGenVertexArrays(1, &VAO);//generate vertex array object names
            glGenBuffers(1, &VBO);// generate buffer object names
            glGenBuffers(1, &EBO);
  
            glBindVertexArray(VAO);//bind a vertex array object
            glBindBuffer(GL_ARRAY_BUFFER, VBO);// bind creation of vertex buffer

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //creation of element buffer
   
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  //creates and initializes vertex array buffer object's data store 
  
            // vertex positions
            glEnableVertexAttribArray(ATTRIB_LOC_POSITION);	//Enable or disable a generic vertex attribute array , here the position arrtribute by use of the constatnts defined above
            glVertexAttribPointer(ATTRIB_LOC_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); //define an array of generic vertex attribute data
            // vertex normals
            glEnableVertexAttribArray(ATTRIB_LOC_NORMAL);	
            glVertexAttribPointer(ATTRIB_LOC_NORMAL,3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
            // vertex texture coords
            glEnableVertexAttribArray(ATTRIB_LOC_TEXCOORD);	
            glVertexAttribPointer(ATTRIB_LOC_TEXCOORD,2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));
            // vertex texture color
            glEnableVertexAttribArray(ATTRIB_LOC_COLOR);	
            glVertexAttribPointer(ATTRIB_LOC_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

     

            glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(unsigned int), 
                     &elements[0], GL_STATIC_DRAW);//creates and initializes a element buffer object's data store
        } 


        // this function should render the mesh
        void draw() 
        {
            //TODO: (Req 2) Write this function
              glBindVertexArray(VAO);
              glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, 0);
              glBindVertexArray(0);
        }

        // this function should delete the vertex & element buffers and the vertex array object
        ~Mesh(){

            //TODO: (Req 2) Write this function
             
              glDeleteBuffers(1, &VBO);//delete vertex buffer object
              glDeleteBuffers(1, &EBO); //delete element buffer object
              glDeleteVertexArrays(1, &VAO);//delete vertex array object
              
        }


        Mesh(Mesh const &) = delete;
        Mesh &operator=(Mesh const &) = delete;
    };

}