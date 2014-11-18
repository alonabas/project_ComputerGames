//
//  Object.h
//  project_temp
//
//  Created by Alona Bas on 9/24/14.
//  Copyright (c) 2014 Alona Bas. All rights reserved.
//

#ifndef project_temp_Object_h
#define project_temp_Object_h

#include "Defs.h"
#include "BoundingSphere.h"
#include "Camera.h"


class Object{
public:
    bool _found;
    mat4 _modelScale, _modelRotate, _modelTranslate, _moveToCenter, _modelMat;
    BoundingSphere _bound;
    Object(): _found(false), _modelScale(1.0f), _modelRotate(1.0f), _modelTranslate(1.0f) {}
    virtual ~Object(){}
    virtual void init(vec3 position){}
    virtual void draw(){}
    virtual bool isInside(vec3 ray)
    {
        Camera* cam = Camera::instance();
        vec3 temp = _bound._center - cam->getEye();
        vec3 local = ray - temp;
        float A = dot(ray, ray);
        float B = 2.0f * dot(local, ray);
        float C = dot(local,local) - (_bound._radius * _bound._radius);
        float discr = (B * B) - (4 * (A * C));
        if(discr <= EPS ) return false;
        float root = glm::sqrt(discr);
        float denom = 1/(2*A);
        float t0 = (denom * (-B - root));
        float t1 = (denom * (-B + root));
        float tTemp;
        if(t0 > EPS && t1 > EPS){
            tTemp = fmin(t0,t1);
            if(tTemp < EPS) return false;
        }
        else if(t0<EPS && t1<EPS) return false;
        else{
            tTemp = fmax(t0,t1);
            if(tTemp < EPS) return false;
        }
        return true;

    }
    virtual bool isFound(){
        return _found;
    }
    virtual void found(){
        _found = true;
    }
    virtual void animate(bool sign, float DT){
        vec3 cam_target = Camera::instance()->getTarget();
        vec3 cam_position = Camera::instance()->getEye();
        vec3 to_move = DT * (cam_position + 2.0f * PERSPECTIVE_START * (cam_target - cam_position) -_bound._center);
        if (!sign){
            _bound._center = _bound._center - to_move;
            _modelTranslate = translate(_modelTranslate, -to_move);
        }
        else{
            _bound._center = _bound._center + to_move;
            _modelTranslate = translate(_modelTranslate, to_move);
        }
    }
};


#endif
