#ifndef OBSTECLE_H
#define OBSTECLE_H
#include "gameobject.h"
class Obstecle : public GameObject
{
public:
    Obstecle(vec3 position);
    virtual ~Obstecle();

    // Setup and initialize object
    void setup();

    // Especify how the object is drawn to the screen.
    virtual void render(const GLuint &MatrixID, const mat4 &Projection, const mat4 &View);

    // Update the object state
    virtual bool update(GLFWwindow* window, std::vector<GameObject *> *);

    bool isInRange() const;

private:
    bool inRange;

    static int obsteclesCount;
};

#endif // OBSTECLE_H
