#pragma once
#include <framework/opengl_includes.h>
// Suppress warnings in third-party code.
#include <framework/disable_all_warnings.h>
DISABLE_WARNINGS_PUSH()
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
DISABLE_WARNINGS_POP()
#include <span>
#include <string>
#include <vector>

// !!! DO NOT MODIFY THIS STRUCT !!!
// Celestial body = planet / moon / star (including the sun)
struct CelestrialBody {
    // The radius of the sphere that represents the celestial body.
    float radius { 1.0f };
    // A celestial body may spin around it's own axis. The speed at which it rotates is
    //  expressed as the spin period. The spin period is the amount of time (in hours) it
    // takes for the body to complete one full rotation.
    float spinPeriod { 0.0f };

    // A celestial body may orbit another celestial body (e.g. a moon orbiting a planet).
    // orbitAround stores the index of the object inside the bodies collection that this
    // celestial body is orbiting. The altitude describes the distance at which the object
    // is orbited (as measured from the center of both objects). The orbit period describes
    // how much time it takes for the celestial body to complete one rotation around the
    // object being orbited.
    //
    // If orbitAround is negative then this celestial body is not orbiting anything and it
    // should instead be placed at the origin (and orbitAltitude/orbitVelocity may be ignored).
    int orbitAround { -1 };
    float orbitAltitude { 0.0f };
    float orbitPeriod { 0.0f };

    // !!! YOU SHOULD IGNORE THE VARIABLES BELOW !!!
    // Only used for UI & drawing.
    std::string name;
    GLuint texture;
};

// ==========================
// =====    EXERCISE    =====
// ==========================
// Compute the positions/orientations of the solar system (list of celestial bodies) at the given time.
// Orbiting should not impact the spin of the body in any way. E.g. if the body had zero spin and was
// orbiting around another body then it should not rotate at all (only translate). If you implement
// orbit/spin correctly then the earth should always see the same side of the moon.
//
// Both spin and orbiting should be performed around the positive Y axis (glm::vec3(0, 1, 0)).
std::vector<glm::mat4> computeCelestrialBodyTransformations(std::span<const CelestrialBody> celestialBodies, float time)
{
    // For each celestial body, compute the matrix which scales the object (relative to a sphere at the origin with radius=1),
    //  translates and rotates it such that it respects the orbit / spin / scale stored in the input.
    std::vector<glm::mat4> transforms;
    for (const CelestrialBody& body : celestialBodies) {
        // Your implementation goes here
        glm::mat4 matrix = glm::identity<glm::mat4>();
        transforms.push_back(matrix);

        // glm::mat4 scale2= glm::scale(matrix, glm::vec3(body.radius,body.radius,body.radius));
      // glm::mat3 translating = glm::translate(matrix, glm::vec3(0,body.orbitAltitude,0));
        glm::mat4 translating;
        glm::mat4 orbiting2;
        glm::mat4 orbiting;
        float altitude;

        if (body.orbitAround != -1) {
            if (celestialBodies[body.orbitAround].orbitAround < 0) {
                matrix = glm::rotate(matrix, 2.0f * glm::radians(180.0f) * time / body.orbitPeriod, glm::vec3(0, 1, 0));
                matrix = glm::translate(matrix, glm::vec3(body.orbitAltitude, 0, 0));
            }

            else {

                matrix = glm::rotate(matrix, 2.0f * glm::radians(180.0f) * time / celestialBodies[body.orbitAround].orbitPeriod, glm::vec3(0, 1, 0));
                matrix = glm::translate(matrix, glm::vec3(celestialBodies[body.orbitAround].orbitAltitude, 0, 0));

                matrix = glm::rotate(matrix, 2.0f * glm::radians(180.0f) * (time / body.orbitPeriod-time/celestialBodies[body.orbitAround].orbitPeriod), glm::vec3(0, 1, 0));
                matrix = glm::translate(matrix, glm::vec3(body.orbitAltitude, 0, 0));

            }
        }
        if (body.orbitAround == -1) {
            matrix = glm::rotate(matrix, 2.0f * glm::radians(180.0f) * (time / body.spinPeriod), glm::vec3(0, 1, 0));
        }

        if(body.orbitAround!=-1)
        matrix = glm::rotate(matrix, 2.0f * glm::radians(180.0f) *( time / body.spinPeriod-time/body.orbitPeriod), glm::vec3(0, 1, 0));
        matrix = glm::scale(matrix, glm::vec3(body.radius, body.radius, body.radius));
        
          
         transforms.pop_back();
         transforms.push_back(matrix);

    }
    return transforms;
}