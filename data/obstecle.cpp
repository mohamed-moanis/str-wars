#include "obstecle.h"

int Obstecle::obsteclesCount = 0;

Obstecle::Obstecle(vec3 position): GameObject(position, OBSTECL)
{
    angley = 0;
    anglex = 0;
    anglez = 0;
    obsteclesCount++;
    scalex = scaley = scalez = 0.02f;
    printf("Obstecle::Obstecle() created obstecle #%d at x=%d y=%d z=%d\n", obsteclesCount, (int)position.x, (int)position.y, (int)position.z);
    RotationMatrix = eulerAngleYXZ(angley, anglex, anglez);//yaw, pitch and roll. Measured in radians
    ScalingMatrix = scale(mat4(), vec3(scalex, scaley, scalez));
    inRange = true;

    // set the collider
    this->collider.sizex = this->collider.sizey = this->collider.sizez = 0.50f;
}

// Destructor
Obstecle::~Obstecle()
{
    printf("Obstecle::~Obstecle() destructed\n");
}

// Draws the Monster on the screen
void Obstecle::render(const GLuint &MatrixID, const GLuint &ModelMatrixID, const GLuint &ViewMatrixID, const mat4 &Projection, const mat4 &View)
{
    //make the matrices for the transformation
    TranslationMatrix = translate(mat4(), vec3(_position.x, _position.y,_position.z));

    glm::mat4 Model = TranslationMatrix* RotationMatrix* ScalingMatrix;//*translate(mat4(), vec3(_position.x * -1, _position.y * -1, _position.z * -1));//order of multiplication is important (try different values above and different order of multiplication)

    //make the MVP matix
    glm::mat4 MVP        = Projection * View * Model;

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    // For each object you render, since the MVP will be different (at least the Model part)
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &Model[0][0]);
    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &View[0][0]);

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Set our "myTextureSampler" sampler to user Texture Unit 0
    glUniform1i(textureID, 0);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(vertexPosition_modelspaceID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        vertexPosition_modelspaceID,  // The attribute we want to configure
        3,                            // size
        GL_FLOAT,                     // type
        GL_FALSE,                     // normalized?
        0,                            // stride
        (void*)0                      // array buffer offset
    );

    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(vertexUVID);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(
        vertexUVID,                   // The attribute we want to configure
        2,                            // size : U+V => 2
        GL_FLOAT,                     // type
        GL_FALSE,                     // normalized?
        0,                            // stride
        (void*)0                      // array buffer offset
    );

    // 3rd attribute buffer : normals
    glEnableVertexAttribArray(vertexNormal_modelspaceID);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glVertexAttribPointer(
        vertexNormal_modelspaceID,    // The attribute we want to configure
        3,                            // size
        GL_FLOAT,                     // type
        GL_FALSE,                     // normalized?
        0,                            // stride
        (void*)0                      // array buffer offset
    );

    // Draw the triangleS !
    glDrawArrays(GL_TRIANGLES, 0, _vertices.size());

    glDisableVertexAttribArray(vertexPosition_modelspaceID);
    glDisableVertexAttribArray(vertexUVID);
    glDisableVertexAttribArray(vertexNormal_modelspaceID);
}

// Update the obstecle state
bool Obstecle::update(GLFWwindow*, std::list<GameObject *> *)
{
    if (_position.z >= MAX_NEGATIVE_Z)
    {
        //printf("updated %f\n",(float) _position.z);
        _position.z-= 0.05f;
        //printf("updated %f\n",(float) _position.z);
    }
    else
        inRange = false;    //mark dead

    return true;
}
